<?php

class NotificacaoAtendimento extends Modelo
{
    const CATEGORIA_AGENDAMENTO_ATENDIMENTO = 1;
    const CATEGORIA_CONFIRMACAO_ATENDIMENTO = 2;
    const CATEGORIA_ATENDIMENTO_CONFIRMADO  = 3;
    const CATEGORIA_AVALIACAO_ATENDIMENTO   = 4;
    const CATEGORIA_SOLICITACAO_CORRECAO    = 5;
    const CATEGORIA_LINK_TELECONSULTA       = 6;

    public function enviarEmailParaConfirmacaoDeAtendimento($agendaId)
    {
        $categoriaMensagem = self::CATEGORIA_CONFIRMACAO_ATENDIMENTO;
        $templateEmail = APPRAIZ . 'includes/email/adm/emailConfirmacaoConsulta.php';

        $this->enviarEmailParaPaciente($agendaId, $categoriaMensagem, $templateEmail);
    }


    public function enviarEmailDeAtendimentoConfirmado($agendaId)
    {
        $categoriaMensagem = self::CATEGORIA_ATENDIMENTO_CONFIRMADO;
        $templateEmail = APPRAIZ . 'includes/email/adm/emailConsultaConfirmada.php';

        $this->enviarEmailParaPaciente($agendaId, $categoriaMensagem, $templateEmail);
    }

    public function enviarEmailDeAtendimentoAgendado($agendaId)
    {
        $categoriaMensagem = self::CATEGORIA_AGENDAMENTO_ATENDIMENTO;
        $templateEmail = APPRAIZ . 'includes/email/adm/emailAgendamentoConsulta.php';

        $this->enviarEmailParaPaciente($agendaId, $categoriaMensagem, $templateEmail);
    }

    public function enviarEmailDeAvaliacaoDoAtendimento($agendaId)
    {
        $categoriaMensagem = self::CATEGORIA_AVALIACAO_ATENDIMENTO;
        $templateEmail = APPRAIZ . 'includes/email/adm/emailAvaliacaoConsulta.php';

        $this->enviarEmailParaPaciente($agendaId, $categoriaMensagem, $templateEmail);
    }

    /**
     * @param $agendaId
     * @param $categoriaMensagem
     * @param $templateCorpoEmail
     * @return bool
     */
    public function enviarEmailParaPaciente($agendaId, $categoriaMensagem, $templateCorpoEmail)
    {
        global $email_from;
        $dadosEmail = $this->getDadosDoAtendimento($agendaId);
        $mEstabelecimentoMensagem = new EstabelecimentoMensagem();

        $dadosEmail['conteudoEmail'] = render_php($templateCorpoEmail, $dadosEmail);
        $templateEmail = APPRAIZ . 'includes/email/adm/templateEmail.php';
        $dscEmail = render_php($templateEmail, $dadosEmail);

        $remetente['email'] = $email_from;
        $remetente['nome'] = $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'];

        try {
            if($dadosEmail['email_paciente']){
                $mEstabelecimentoMensagem->enviarMensagem($dadosEmail['estabelecimento_id'], TipoMensagem::TIPO_MENSAGEM_EMAIL,
                    $categoriaMensagem, $dscEmail, $dadosEmail['email_paciente']);
            }
        } catch (Exception $e) {
            return false;
        }

        return true;
    }

    /**
     * @param $agendaId
     * @return array|bool
     */
    public function getDadosDoAtendimento($agendaId)
    {
        $mHashSimec = new SimecHashids();
        $mEstabelecimentoProfissionalAgenda = new EstabelecimentoProfissionalAgenda();
        $mEstabelecimento = new Estabelecimento();
        $mContatoPessoaFisica = new ContatoPessoaFisica();
        $mEnderecoPessoaJuridica = new EnderecoPessoaJuridica();

        $dadosAgenda = $mEstabelecimentoProfissionalAgenda->getTodosDados($agendaId);
        $dadosEstabelecimento = $mEstabelecimento->getTodosDados($dadosAgenda['estabelecimento_id']);
        $contatos = $mContatoPessoaFisica->listarEspecificosPorPessoaFisica($dadosAgenda['pessoa_paciente_id']);
        $endereco = $mEnderecoPessoaJuridica->getPrincipalPorPessoaJuridica($dadosEstabelecimento['pessoajuridica_id']);

        $inicioConsulta = new DateTime($dadosAgenda['inicio']);
        $fimConsulta = new DateTime($dadosAgenda['fim']);


        $mTipoConsulta = new TipoConsulta();
        if($mTipoConsulta->isConsultaRemota($dadosAgenda['tipo_consulta_id'])) {
            $dadosAgenda['url_atendimento_remoto'] = $mEstabelecimentoProfissionalAgenda->gerarUrlAtendimentoRemoto($agendaId);
        }else if($endereco) {
            $dadosAgenda['endereco_estabelecimento'] = $endereco;
            $dadosAgenda['endereco_estabelecimento']['descricao'] = $endereco['logradouro'] . ", " . $endereco['complemento'];
        }

        $dadosAgenda['data_inicio'] = formata_data($dadosAgenda['data_inicio']);
        $dadosAgenda['hora_inicio'] = $inicioConsulta->format('H:i');
        $dadosAgenda['hora_fim'] = $fimConsulta->format('H:i');
        $dadosAgenda['nome_fantasia'] = $dadosEstabelecimento['nome_fantasia'];
        $dadosAgenda['hash_agenda_id'] = $mHashSimec->encode($agendaId, 6);
        $dadosAgenda['email_paciente'] = isset($contatos['contato_email']) ? $contatos['contato_email'] : null;

        return $dadosAgenda;
    }
}