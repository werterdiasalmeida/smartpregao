<?php

class SolicitacaoCorrecaoAgenda extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_solicitacao_correcao_agenda";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("id");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'id' => null,
        'agenda_id' => null,
        'descricao_solicitacao' => null,
        'finalizada' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null
    );

    public function salvarSolicitacao($params)
    {
        if($this->getSolicitacaoEmAberto($params['agenda_id'])){
            throw new SolicitacaoCorrecaoExistente();
        }

        $retorno = $this->manter($params);

        //Enviar o email para todas as pessoas que possuem o perfil de correção e estão no mesmo estabelecimento
        $this->enviarEmailPerfilCorrecao($retorno);

        return $retorno;
    }

    /**
     * @param $agendaId
     * @param $categoriaMensagem
     * @param $templateCorpoEmail
     * @return bool
     */
    public function enviarEmailPerfilCorrecao($solicitacaoCorrecaoAgendaId)
    {
        global $email_from;
        $mEstabelecimentoMensagem = new EstabelecimentoMensagem();

        $categoriaMensagem = NotificacaoAtendimento::CATEGORIA_SOLICITACAO_CORRECAO;
        $templateCorpoEmail = APPRAIZ . 'includes/email/adm/emailAvisoSolicitacaoCorrecao.php';

        $dadosEmail = $this->getDadosEmail($solicitacaoCorrecaoAgendaId);
        $usuariosAviso = $this->getUsuariosEmailAvisoSolicitacao($dadosEmail['estabelecimento_id']);

        $templateEmail = APPRAIZ . 'includes/email/adm/templateEmail.php';
        $remetente['email'] = $email_from;
        $remetente['nome'] = $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'];

        try {
            if(is_array($usuariosAviso) && count($usuariosAviso) > 0){
                foreach ($usuariosAviso AS $usuario){
                    $dadosEmail['email_destinatario'] = $usuario['email_destinatario'];
                    $dadosEmail['nome_destinatario']  = $usuario['nome_destinatario'];
                    $dadosEmail['conteudoEmail'] = render_php($templateCorpoEmail, $dadosEmail);
                    $dscEmail = render_php($templateEmail, $dadosEmail);

                    $mEstabelecimentoMensagem->enviarMensagem($dadosEmail['estabelecimento_id'], TipoMensagem::TIPO_MENSAGEM_EMAIL,
                        $categoriaMensagem, $dscEmail, $dadosEmail['email_destinatario']);
                }
            }
        } catch (Exception $e) {
            return false;
        }

        return true;
    }

    public function getUsuariosEmailAvisoSolicitacao($estabelecimento_id)
    {
        $usuariosEncontrados = array();
        $usuarios = UsuarioResponsabilidadeEstabelecimento::getDadosUsuariosPossuemResponsabilidadeEstabelecimento($estabelecimento_id);

        if($usuarios && is_array($usuarios)){
            foreach ($usuarios AS $usuario){
                if(possuiPerfil(ADM_PERFIL_CORRETOR_AGENDA,$usuario['usucpf'])){
                    $usuariosEncontrados[] = array(
                        'nome_destinatario'     => $usuario['usunome'],
                        'email_destinatario'    => $usuario['usuemail'],
                    );
                }

            }
        }

        return $usuariosEncontrados;
    }

    /**
     * @param $agendaId
     * @return array|bool
     */
    public function getDadosEmail($solicitacaoCorrecaoAgendaId)
    {
        $mEstabelecimentoProfissionalAgenda = new EstabelecimentoProfissionalAgenda();
        $mEstabelecimento = new Estabelecimento();
        $mContatoPessoaFisica = new ContatoPessoaFisica();
        $mEnderecoPessoaJuridica = new EnderecoPessoaJuridica();

        $dados = $this->getTodosDados($solicitacaoCorrecaoAgendaId);
        $dadosAgenda = $mEstabelecimentoProfissionalAgenda->getTodosDados($dados['agenda_id']);
        $dadosEstabelecimento = $mEstabelecimento->getTodosDados($dadosAgenda['estabelecimento_id']);
        $contatos = $mContatoPessoaFisica->listarEspecificosPorPessoaFisica($dadosAgenda['pessoa_paciente_id']);
        $endereco = $mEnderecoPessoaJuridica->getPrincipalPorPessoaJuridica($dadosEstabelecimento['pessoajuridica_id']);

        $inicioConsulta = new DateTime($dadosAgenda['inicio']);
        $fimConsulta = new DateTime($dadosAgenda['fim']);

        if($endereco) {
            $dadosAgenda['endereco_estabelecimento'] = $endereco;
            $dadosAgenda['endereco_estabelecimento']['descricao'] = $endereco['logradouro'] . ", " . $endereco['complemento'];
        }
        $dadosAgenda['data_inicio'] = formata_data($dadosAgenda['data_inicio']);
        $dadosAgenda['hora_inicio'] = $inicioConsulta->format('H:i');
        $dadosAgenda['hora_fim'] = $fimConsulta->format('H:i');
        $dadosAgenda['nome_fantasia'] = $dadosEstabelecimento['nome_fantasia'];
        $dadosAgenda['nome'] = $dados['solicitante'];
        $dadosAgenda['descricao_solicitacao'] = $dados['descricao_solicitacao'];
        $dadosAgenda['email_paciente'] = isset($contatos['contato_email']) ? $contatos['contato_email'] : null;

        return $dadosAgenda;
    }

    public function cancelarSolicitacao($id)
    {
        $this->carregarPorId($id);
        $dados = $this->getDados();

        if($dados['finalizada'] == 't'){
            throw new Exception('Não é possível cancelar a solicitação pois a mesma já se encontra finalizada');
        }

        $dados['excluido'] = 'true';
        return $this->manter($dados);
    }

    public function finalizarSolicitacao($id)
    {
        $this->carregarPorId($id);
        $dados = $this->getDados();

        if($dados['finalizada'] == 't'){
            throw new Exception('Não é possível finalizar a solicitação pois a mesma já se encontra finalizada');
        }

        $dados['finalizada'] = 'true';
        return $this->manter($dados);
    }

    public function getSolicitacaoEmAberto($agenda_id)
    {
        return $this->recuperarLinha("*", ["excluido IS FALSE","finalizada IS FALSE", "agenda_id = {$agenda_id}"], false);
    }

    public function getTodosDados($id)
    {
        $sql = "SELECT
                  sca.*,
                  usu_cadastro.usunome AS solicitante
                FROM
                  {$this->stNomeTabela} sca
                INNER JOIN
                  seguranca.usuario usu_cadastro ON usu_cadastro.id = sca.criado_por
                WHERE
                  sca.id = {$id}";
        $retorno = $this->pegaLinha($sql);

        return $retorno ? $retorno : array();
    }
}