<?php

/**
 * Classe responsável por guardar os métodos que serão acessados pelo webservice
 */
class AdmWs extends AbstractWs
{
    public function listarProntuario($dados = array())
    {
        $cpf = formatar_cpf($_SESSION['usucpf']);
        $mPessoa = new PessoaFisica();
        $mProntuario = new Prontuario();
        $mAnexo = new AnexoProntuario();
        $limit = isset($dados['limit']) ? $dados['limit'] : null;
        $offset = isset($dados['offset']) ? $dados['offset'] : null;

        $pessoaPacienteId = $mPessoa->recuperarUm('id', array("cpf = '{$cpf}'"));

        if(empty($pessoaPacienteId)){
            return $this->gerarRespostaSucesso(array());
        }

        $serverUrl = $this->_getServerUrl(false);
        $avatarProfissional = array(
            '' => "{$serverUrl}/imagens/man.png",
            'M' => "{$serverUrl}/imagens/man.png",
            'F' => "{$serverUrl}/imagens/woman.png"
        );

        $prontuarios = $mProntuario->listarPorPessoaId($pessoaPacienteId, $limit, $offset);
        $prontuariosId = array_map(function($prontuario){
            return $prontuario['id'];
        }, $prontuarios);
        $anexos = $mAnexo->listarAgrupadosProntuarioPorPessoa($pessoaPacienteId, $prontuariosId);
        $prontuarios = array_map(function($prontuario) use ($anexos, $serverUrl, $avatarProfissional){
            $anexosProntuario = array('arquivos' => array(), 'imagens' => array());
            if(isset($anexos[$prontuario['id']])){
                foreach($anexos[$prontuario['id']] as $anexo){
                    $anexoProntuario = array(
                        'arquivo_id' => $anexo['arquivo_id'],
                        'nome' => $anexo['arqnome'],
                        'extensao' => $anexo['arqextensao'],
                        'tipo' => $anexo['arqtipo'],
                        'tamanho' => formatarTamanhoArquivo($anexo['arqtamanho'])
                    );

                    if(isImagem($anexo['arqextensao'])){
                        $anexoProntuario['url'] = "{$serverUrl}/slideshow/slideshow/verimagem.php?arqid={$anexo['arquivo_id']}&_sisarquivo=adm&newwidth=350";
                        $anexosProntuario['imagens'][] = $anexoProntuario;
                    }else{
                        $anexosProntuario['arquivos'][] = $anexoProntuario;
                    }
                }
            }

            $dadoProntuario = array(
                'nome_profissional' => $prontuario['nome_profissional'],
                'criado_em_formatado' => $prontuario['criado_em_formatado'],
                'descricao' => $prontuario['descricao'],
                'tipo_prontuario' => $prontuario['tipo_prontuario'],
                'tipo_sigla' => $prontuario['tipo_sigla'],
                'truncating' => true,
                'foto_profissional' => $prontuario['foto_profissional']
                    ? "{$serverUrl}/slideshow/slideshow/verimagem.php?arqid={$prontuario['foto_profissional']}&_sisarquivo=adm&newwidth=150"
                    : $avatarProfissional[$prontuario['profissional_sexo']],
                'anexos' => $anexosProntuario
            );

            return $dadoProntuario;
        }, $prontuarios);

        return $this->gerarRespostaSucesso($prontuarios);
    }


    public function listarProntuarioPorAgenda($dados)
    {
        $agendaId = $dados['agenda_id'];
        $mAgenda = new EstabelecimentoProfissionalAgenda();
        $dadosAgenda = $mAgenda->getTodosDados($agendaId);
        $pessoaPacienteId = $dadosAgenda['pessoa_paciente_id'];
        $podeAtender = $mAgenda->isUsuarioPodeAtender($agendaId);

        $mProntuario = new Prontuario();
        $mAnexo = new AnexoProntuario();
        $limit = isset($dados['limit']) ? $dados['limit'] : null;
        $offset = isset($dados['offset']) ? $dados['offset'] : null;

        if(empty($pessoaPacienteId)){
            return $this->gerarRespostaSucesso(array());
        }

        $permiteAlteracao = false;
        if($podeAtender && $dadosAgenda['wf_estado_id'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_EM_ATENDIMENTO) {
            $permiteAlteracao = true;
        }else if($podeAtender && $dadosAgenda['wf_estado_id'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA){
            $permiteAlteracao = $mAgenda->isAlteracaoAgendaDisponivel($agendaId);
        }

        $duracaoConsulta = 0;
        if($podeAtender){
            $duracaoRealizacaoConsulta = $mAgenda->calcularDuracaoRealizacaoConsulta($agendaId);
            $duracaoConsulta = $duracaoRealizacaoConsulta['duracao_segundos'];
        }

        $serverUrl = $this->_getServerUrl(false);
        $avatarProfissional = array(
            '' => "{$serverUrl}/imagens/man.png",
            'M' => "{$serverUrl}/imagens/man.png",
            'F' => "{$serverUrl}/imagens/woman.png"
        );

        $prontuarios = $mProntuario->listarPorPessoaId($pessoaPacienteId, $limit, $offset);
        $prontuariosId = array_map(function($prontuario){
            return $prontuario['id'];
        }, $prontuarios);
        $anexos = $mAnexo->listarAgrupadosProntuarioPorPessoa($pessoaPacienteId, $prontuariosId);
        $prontuarios = array_map(function($prontuario) use ($anexos, $serverUrl, $avatarProfissional){
            $anexosProntuario = array('arquivos' => array(), 'imagens' => array());
            if(isset($anexos[$prontuario['id']])){
                foreach($anexos[$prontuario['id']] as $anexo){
                    $anexoProntuario = array(
                        'arquivo_id' => $anexo['arquivo_id'],
                        'nome' => $anexo['arqnome'],
                        'extensao' => $anexo['arqextensao'],
                        'tipo' => $anexo['arqtipo'],
                        'tamanho' => formatarTamanhoArquivo($anexo['arqtamanho'])
                    );

                    if(isImagem($anexo['arqextensao'])){
                        $anexoProntuario['url'] = "{$serverUrl}/slideshow/slideshow/verimagem.php?arqid={$anexo['arquivo_id']}&_sisarquivo=adm&newwidth=350";
                        $anexosProntuario['imagens'][] = $anexoProntuario;
                    }else{
                        $anexosProntuario['arquivos'][] = $anexoProntuario;
                    }
                }
            }

            $dadoProntuario = array(
                'nome_profissional' => $prontuario['nome_profissional'],
                'criado_em_formatado' => $prontuario['criado_em_formatado'],
                'descricao' => $prontuario['descricao'],
                'tipo_prontuario' => $prontuario['tipo_prontuario'],
                'tipo_sigla' => $prontuario['tipo_sigla'],
                'agenda_id' => $prontuario['agenda_id'],
                'id' => $prontuario['id'],
                'truncating' => true,
                'foto_profissional' => $prontuario['foto_profissional']
                    ? "{$serverUrl}/slideshow/slideshow/verimagem.php?arqid={$prontuario['foto_profissional']}&_sisarquivo=adm&newwidth=150"
                    : $avatarProfissional[$prontuario['profissional_sexo']],
                'anexos' => $anexosProntuario
            );

            return $dadoProntuario;
        }, $prontuarios);

        return $this->gerarRespostaSucesso(array(
            'prontuarios' => $prontuarios,
            'permite_alteracao' => $permiteAlteracao,
            'duracao' => $duracaoConsulta
        ));
    }

    /**
     * @param $dados
     * @return array
     * @throws CpfIndisponivel
     * @throws Exception
     */
    public function salvarProntuario($dados)
    {
        $cpf = formatar_cpf($_SESSION['usucpf']);
        $mPessoa = new PessoaFisica();
        $pessoaPacienteId = $mPessoa->recuperarUm('id', array("cpf = '{$cpf}'"));
        if(empty($pessoaPacienteId)){
            $mUsuario = new Usuario();
            $usuario = $mUsuario->recuperarLinha('*', array("usucpf = '{$_SESSION['usucpf']}'"), false);
            $pessoaPacienteId = $mPessoa->salvarPessoaFisica(array(
                'nome_completo' => $usuario['usunome'],
                'nome_social' => $usuario['usunome'],
                'foto_arquivo_id' => $usuario['arqid'],
                'cpf' => $cpf,
                'nome_mae' => '',
                'data_nascimento' => formata_data($usuario['usudatanascimento']),
                'sexo' => $usuario['ususexo'],
                'cartao_nacional_saude' => '',
            ), array(), false, false, false);
        }

        $dados['pessoa_paciente_id'] = $pessoaPacienteId;
        $mProntuario = new Prontuario();
        $id = $mProntuario->salvarProntuario($dados);
        $mProntuario->commit();

        return $this->gerarRespostaSucesso(array('id' => $id));
    }

    /**
     * @param $dados
     * @return array
     * @throws Exception
     */
    public function salvarProntuarioAgenda($dados)
    {
        $agendaId = $dados['agenda_id'];
        $mAgenda = new EstabelecimentoProfissionalAgenda();
        $dadosAgenda = $mAgenda->getTodosDados($agendaId);
        $pessoaPacienteId = $dadosAgenda['pessoa_paciente_id'];
        $podeAtender = $mAgenda->isUsuarioPodeAtender($agendaId);
        $alteracaoDisponivel = $mAgenda->isAlteracaoAgendaDisponivel($agendaId);

        if(empty($pessoaPacienteId) || !$podeAtender || !$alteracaoDisponivel){
            return $this->gerarRespostaErro();
        }

        $dados['pessoa_paciente_id'] = $pessoaPacienteId;
        $mProntuario = new Prontuario();
        $id = $mProntuario->salvarProntuario($dados);
        $mProntuario->commit();

        return $this->gerarRespostaSucesso(array('id' => $id));
    }

    /**
     * @param $dados
     * @return array
     * @throws Exception
     */
    public function excluirRegistroProntuario($dados)
    {
        $prontuarioId = $dados['id'];
        $mProntuario = new Prontuario($prontuarioId);
        $prontuario = $mProntuario->getDados();
        $podeAtender = true;
        $alteracaoDisponivel = true;

        if($prontuario['agenda_id']){
            $mAgenda = new EstabelecimentoProfissionalAgenda();
            $podeAtender = $mAgenda->isUsuarioPodeAtender($prontuario['agenda_id']);
            $alteracaoDisponivel = $mAgenda->isAlteracaoAgendaDisponivel($prontuario['agenda_id']);
        }

        if(!$podeAtender || !$alteracaoDisponivel){
            return $this->gerarRespostaErro();
        }

        $id = $mProntuario->excluirLogicamente($prontuarioId);
        $mProntuario->commit();

        return $this->gerarRespostaSucesso(array('id' => $id));
    }

    public function getEstabelecimentosProfissional($dados)
    {
        $usucpfFormatado = formatar_cpf($_SESSION['usucpf']);
        $mProfissional = new Profissional();
        $serverUrl = $this->_getServerUrl(false);

        $estabelecimentos = array_map(function($estabelecimento) use ($serverUrl){
            return array(
                'codigo' => $estabelecimento['codigo'],
                'descricao' => $estabelecimento['descricao'],
                'estabelecimento_logo' => $estabelecimento['estabelecimento_logo']
                    ? "{$serverUrl}/slideshow/slideshow/verimagem.php?arqid={$estabelecimento['estabelecimento_logo']}&_sisarquivo=adm&newwidth=150"
                    : null,
            );
        },$mProfissional->listaEstabelecimentosPorCpfProfissional($usucpfFormatado));

        return $this->gerarRespostaSucesso($estabelecimentos);
    }

    public function listarAgendasProfissional($dados)
    {
        $mAgenda = new EstabelecimentoProfissionalAgenda();
        $estabelecimentoId = $dados['estabelecimento_id'];
        $data = $dados['data'];
        $serverUrl = $this->_getServerUrl(false);
        $usucpfFormatado = formatar_cpf($_SESSION['usucpf']);
        $limit = isset($dados['limit']) ? $dados['limit'] : null;
        $offset = isset($dados['offset']) ? $dados['offset'] : null;
        $situacoesWf = array(
            EstabelecimentoProfissionalAgenda::WF_ESTADO_AGENDADA,
            EstabelecimentoProfissionalAgenda::WF_ESTADO_CONFIRMADA,
            EstabelecimentoProfissionalAgenda::WF_ESTADO_AGUARDANDO_ATENDIMENTO,
            EstabelecimentoProfissionalAgenda::WF_ESTADO_EM_ATENDIMENTO,
            EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA
        );
        $avatarPessoa = array(
            '' => "{$serverUrl}/imagens/man.png",
            'M' => "{$serverUrl}/imagens/man.png",
            'F' => "{$serverUrl}/imagens/woman.png"
        );

        $arWherePadraoAgenda = array(
            "epa.inicio::date = '{$data}'",
            ($estabelecimentoId ? "eep.estabelecimento_id = {$estabelecimentoId}" : "FALSE"),
            "doc.esdid IN (" . implode(', ', $situacoesWf) . ")",
            "p.cpf = '{$usucpfFormatado}'"
        );

        $consultas = array_map(function($consulta) use ($serverUrl, $avatarPessoa, $mAgenda) {
            $podeAtender = $mAgenda->isUsuarioPodeAtender($consulta['id']);
            $iniciarAtendimento = $consulta['estado_codigo'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_AGUARDANDO_ATENDIMENTO;
            $continuarAtendimento = $consulta['estado_codigo'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_EM_ATENDIMENTO;

            return array(
                'data_agenda' => $consulta['data_agenda'],
                'encaixe' => $consulta['encaixe'],
                'especialidade' => $consulta['especialidade'],
                'estado' => $consulta['estado'] == 'Aguardando atendimento' ? 'Aguardando' : $consulta['estado'],
                'estado_codigo' => $consulta['estado_codigo'],
                'hora_inicio' => $consulta['hora_inicio'],
                'hora_fim' => $consulta['hora_fim'],
                'paciente_nome_completo' => $consulta['paciente_nome_completo'],
                'paciente_data_nascimento' => $consulta['paciente_data_nascimento'],
                'paciente_idade' => $consulta['paciente_idade'],
                'procedimento' => $consulta['procedimento'],
                'sala' => $consulta['sala'],
                'iniciar_atendimento' => $podeAtender && $iniciarAtendimento,
                'continuar_atendimento' => $podeAtender && $continuarAtendimento,
                'tipo_consulta' => $consulta['tipo_consulta'],
                'agenda_id' => $consulta['id'],
                'paciente_foto' => $consulta['paciente_foto']
                    ? "{$serverUrl}/slideshow/slideshow/verimagem.php?arqid={$consulta['paciente_foto']}&_sisarquivo=adm&newwidth=150"
                    : $avatarPessoa[$consulta['paciente_sexo']],
            );
        }, $mAgenda->getList($arWherePadraoAgenda, false, null, $limit, $offset));
        return $this->gerarRespostaSucesso($consultas);
    }

    public function iniciarAtendimento($dados)
    {
        $mAgenda = new EstabelecimentoProfissionalAgenda();
        $agendaId = $dados['agenda_id'];
        $agenda = $mAgenda->getTodosDados($agendaId);
        $podeAtender = $mAgenda->isUsuarioPodeAtender($agendaId);
        $retornoAtendimento = array('atendimentoIniciado' => false);

        $atendimento = EstabelecimentoProfissionalAgenda::getAtendimentoByUsuarioLogado($agenda['estabelecimento_id']);

        $duracaoConsulta = 0;
        if($podeAtender){
            $duracaoRealizacaoConsulta = $mAgenda->calcularDuracaoRealizacaoConsulta($agendaId);
            $duracaoConsulta = $duracaoRealizacaoConsulta['duracao_segundos'];
        }

        if($atendimento){
            $retornoAtendimento['msg'] = "Não é possível iniciar este atendimento pois 
            o do dia {$atendimento['data_agenda']} às {$atendimento['hora_inicio']} ainda está em andamento.";
        }else if($podeAtender && $agenda['wf_estado_id'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_AGUARDANDO_ATENDIMENTO) {
            $mAgenda->atender($agendaId);
            $mAgenda->commit();

            $agenda = $mAgenda->getTodosDados($agendaId);

            $retornoAtendimento = array(
                'atendimentoIniciado' => true,
                'permite_alteracao' => true,
                'duracao' => $duracaoConsulta,
                'agenda' => array(
                    'estado' => $agenda['wf_estado'],
                    'estado_codigo' => $agenda['wf_estado_id'],
                    'iniciar_atendimento' => false,
                    'continuar_atendimento' => true,
                )
            );
        }

        return $this->gerarRespostaSucesso($retornoAtendimento);
    }

    public function finalizarAtendimento($dados)
    {
        $mAgenda = new EstabelecimentoProfissionalAgenda();
        $agendaId = $dados['agenda_id'];
        $agenda = $mAgenda->getTodosDados($agendaId);
        $podeAtender = $mAgenda->isUsuarioPodeAtender($agendaId);
        $retornoAtendimento = array('atendimentoIniciado' => false);

        if($podeAtender && $agenda['wf_estado_id'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_EM_ATENDIMENTO) {
            $mAgenda->finalizarAtendimento($agendaId);
            $mAgenda->commit();

            $agenda = $mAgenda->getTodosDados($agendaId);

            $retornoAtendimento = array(
                'atendimentoIniciado' => true,
                'permite_alteracao' => $mAgenda->isAlteracaoAgendaDisponivel($agendaId),
                'agenda' => array(
                    'estado' => $agenda['wf_estado'],
                    'estado_codigo' => $agenda['wf_estado_id'],
                    'iniciar_atendimento' => false,
                    'continuar_atendimento' => false,
                )
            );
        }

        return $this->gerarRespostaSucesso($retornoAtendimento);
    }


    public function getModelosPorAgendaTipo($dados)
    {
        $agendaId = $dados['agenda_id'];
        $tipoSigla = $dados['tipo_sigla'];
        $mAgenda = new EstabelecimentoProfissionalAgenda();
        $agenda = $mAgenda->getTodosDados($agendaId);
        $podeAtender = $mAgenda->isUsuarioPodeAtender($agendaId);
        $mTipoProntuario = new TipoProntuario();
        $tipoProntuario = $mTipoProntuario->getPorSigla($tipoSigla);

        if(!$podeAtender){
            return $this->gerarRespostaErro(array());
        }

        $mModeloDoc = new ModeloDocumento();
        $modelosAgenda = $mModeloDoc->getComboPorEstabelecimento($agenda['estabelecimento_id'], $tipoProntuario['id'], $agendaId);

        return $this->gerarRespostaSucesso($modelosAgenda);
    }
}