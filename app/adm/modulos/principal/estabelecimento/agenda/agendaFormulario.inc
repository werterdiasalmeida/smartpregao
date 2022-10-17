<?php
$iniciarAtendimento = isset($iniciarAtendimento) ? $iniciarAtendimento : false;

$mEstabelecimentoEspecialidadeProfissional = new EstabelecimentoEspecialidadeProfissional();
$mAgenda = new EstabelecimentoProfissionalAgenda();
$mSolicitacaoCorrecao = new SolicitacaoCorrecaoAgenda();

$mGuia = new Guia();

$p = getParam();
$profissional_id = $_REQUEST['profissional_id'] ? $_REQUEST['profissional_id'] : '';

if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'filtrarEspecialidades' :
            $jsComboDuracaoConsulta = comboEspecialidadesPorProfissionalEstabelecimento($_REQUEST['profissional_id'], $p['id']);
            die($jsComboDuracaoConsulta);
            break;
        case 'excluirHorarioAgenda' :
            $mAgenda->excluirHorario($p['agenda_id']);
            $mAgenda->commit();
            $db->sucessomAjax();
            break;
        case 'verificarHorario' :
            $p = getParam();
            $disponbilidade = $mAgenda->verificarDisponbilidadeHorarios($_POST['horario'], $p['id'], $_POST['profissional_id'], $_POST['data_agenda'], $_POST['sala_estabelecimento_id'], $p['agenda_id'], true);

            die($disponbilidade);
            break;
        case 'adicionarObservacao' :
            $params = array_merge($_POST, getParam());
            $params = utf8ArrayDecode($params);

            $params['id'] = isset($params['observacao_id']) ? $params['observacao_id'] : null;
            $params['estabelecimento_profissional_agenda_id'] = $params['agenda_id'];
            $params['descricao'] = $params['observacao'];

            $mObservacao = new ObservacaoAgenda();
            $mObservacao->salvarObservacao($params);
            $mObservacao->commit();

            include_once 'observacoesAgenda.inc';
            die;
            break;
        case 'adicionarObservacaoPessoa' :
            $params = array_merge($_POST, getParam());
            $params = utf8ArrayDecode($params);

            $mAgenda = new EstabelecimentoProfissionalAgenda($params['agenda_id']);

            $params['id'] = isset($params['observacao_id']) ? $params['observacao_id'] : null;
            $params['pessoafisica_id'] = $mAgenda->pessoa_paciente_id;
            $params['descricao'] = $params['observacao'];

            $mObservacao = new ObservacaoPessoaFisica();
            $mObservacao->salvarObservacao($params);
            $mObservacao->commit();

            include_once 'observacoesPessoa.inc';
            die;
            break;
        case 'alterarFoto':
            $params = array_merge($_POST, getParam());

            $mAgenda = new EstabelecimentoProfissionalAgenda($params['agenda_id']);

            $dadosPaciente['id'] = $mAgenda->pessoa_paciente_id;
            $mPessoa = new PessoaFisica($dadosPaciente['id']);
            $dadosPaciente['nome_completo'] = $mPessoa->nome_completo;
            $resultado = $mPessoa->alteraFoto($dadosPaciente, $_FILES);
            $mPessoa->commit();

            $db->sucessomAjax('', $resultado);
            die;
            break;
        case 'formularioGuia':
        case 'salvarGuia':
        case 'imprimirGuia':
        case 'buscarTuss':
            include_once 'guia/formulario.inc';
            die;
            break;
    }
}

$podeAtender = false;
$data_agenda = $_POST['data_agenda'];
$hora_inicio = substr($_POST['hora_inicio'], 0, 5);
$alteracao = false;
$habilitado = 'S';
if(isset($p['agenda_id']) && !empty($p['agenda_id'])){
    $mAgenda = new EstabelecimentoProfissionalAgenda();
    extract($mAgenda->getTodosDados($p['agenda_id']));
    $data_agenda = $data_inicio;
    $hora_inicio = substr($hora_inicio, 0, 5);
    $hora_fim = substr($hora_fim, 0, 5);

    $alteracao = true;
    $podeAtender = $mAgenda->isUsuarioPodeAtender($p['agenda_id']);
}

if($wf_estado_id && $wf_estado_id != EstabelecimentoProfissionalAgenda::WF_ESTADO_DISPONIVEL){
    $habilitado = 'N';
}

$isAgendaPossuiGuia = $mGuia->isAgendaPossuiGuia($p['agenda_id']);
$isVisualizaGuia = Guia::isVisualizaGuia($p['id']);

$permissaoExcluirHorarioAgenda = in_array($wf_estado_id, array(EstabelecimentoProfissionalAgenda::WF_ESTADO_DISPONIVEL, EstabelecimentoProfissionalAgenda::WF_ESTADO_BLOQUEADA));
$permissaoAlterarDadosConsulta = in_array($wf_estado_id, array(
    EstabelecimentoProfissionalAgenda::WF_ESTADO_AGENDADA,
    EstabelecimentoProfissionalAgenda::WF_ESTADO_CONFIRMADA
));

$realizada = $wf_estado_id == EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA;

$permissaoSolicitacaoCorrecao = false;
$permissaoEfetuarCorrecao = false;
if($realizada){
    $permissaoSolicitacaoCorrecao = true;

    if($p['agenda_id'] && possuiPerfil(ADM_PERFIL_CORRETOR_AGENDA) && $mSolicitacaoCorrecao->getSolicitacaoEmAberto($p['agenda_id'])){
        $permissaoEfetuarCorrecao = true;
    }
}
?>
<style type="text/css">
    .input-group-addon-desabilitado{
        display: table-cell;
        padding: 0 5px;
    }

    .input-group-addon-desabilitado:first-child{
        padding-left: 0;
    }

    .botao-foto {
        transition: .5s ease;
        opacity: 0;
        position: absolute;
        top: 50%;
        left: 50%;
        transform: translate(-50%, -50%);
        -ms-transform: translate(-50%, -50%);
        text-align: center;
    }

    .image-container:hover .foto-pessoa {
        opacity: 0.3;
    }

    .image-container:hover .botao-foto {
        opacity: 1;
    }
</style>

<div id="agenda-formulario">
    <input type="hidden" name="p" value="<?= setParam(getParam(),false); ?>" />
    <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
        <h4 class="modal-title"><?= ($alteracao ? 'Alterar agenda' : 'Abrir agenda') ?></h4>
    </div>
    <div class="modal-body">

        <?php
        if($pessoa_paciente_id) :
            $mPessoa = new PessoaFisica($pessoa_paciente_id);
            $mContatoPessoaFisica = new ContatoPessoaFisica();
            $contatos = $mContatoPessoaFisica->listarEspecificosPorPessoaFisica($mPessoa->id, true);
            ?>
            <div class="row">
                <div class="col-md-6">
                    <div class="form">
                        <h4 class="form-section" style="margin-top: 0;">Dados do paciente</h4>
                        <div class="row">
                            <div class="col-sm-5 col-lg-4 text-center image-container">
                                <div id="div-imagem">
                                <?= montaImgFoto($mPessoa->foto_arquivo_id, $mPessoa->sexo, '180px', '180px', '', '', 'img-circle bg-white foto-pessoa', true); ?>
                                </div>
                                <?php if(true): ?>
                                <div class="botao-foto">
                                    <span class="btn default btn-file">
                                        Alterar
                                        <input type="file" name="foto" onchange="javascript:alterarFoto(this);"/>
                                    </span>
                                </div>
                                <?php endif; ?>
                            </div>

                            <div class='col-sm-7 col-lg-8 text-left'>
                                <div class='row'>
                                    <div class="col-md-12">
                                        <div class="row">
                                            <div class="col-md-4">
                                                <div class="form-group">
                                                    <label>Nome completo</label>
                                                    <br/>
                                                    <strong><?= $mPessoa->nome_completo ?></strong>
                                                </div>
                                            </div>
                                            <div class="col-md-4">
                                                <div class="form-group">
                                                    <label>Data de Nascimento</label>
                                                    <br/>
                                                    <strong><?= ($mPessoa->data_nascimento ? formata_data($mPessoa->data_nascimento) : '-'); ?></strong>
                                                </div>
                                            </div>
                                            <div class="col-md-4">
                                                <div class="form-group">
                                                    <label>CPF</label>
                                                    <br/>
                                                    <strong><?= ($mPessoa->cpf ?: '-') ?></strong>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="row">
                                            <div class="col-md-4">
                                                <div class="form-group">
                                                    <label>Celular/Whatsapp</label>
                                                    <br/>
                                                    <strong><?= ($contatos['contato_celular'] ?: '-') ?></strong>
                                                </div>
                                            </div>
                                            <div class="col-md-4">
                                                <div class="form-group">
                                                    <label>Email</label>
                                                    <br/>
                                                    <strong><?= ($contatos['contato_email'] ?: '-') ?></strong>
                                                </div>
                                            </div>
                                            <?php if ($mPessoa->cartao_nacional_saude): ?>
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>CNS</label>
                                                        <br/>
                                                        <strong><?= $mPessoa->cartao_nacional_saude ?></strong>
                                                    </div>
                                                </div>
                                            <?php endif;?>
                                            <div class="col-md-12">
                                                <?php if (UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id'], false, 'adm.php?modulo=principal/medico/atendimento&acao=C')
                                                    || $mAgenda->isUsuarioPodeAtender($p['agenda_id'])) : ?>
                                                    <a class="btn blue" type="button" href="?modulo=principal/medico/atendimento&acao=C<?= setParam(array('agenda_id' => $p['agenda_id'], 'estabelecimento_id' => $p['id'])) ?>">
                                                        <i class="icon-notebook"></i>
                                                        Prontuário
                                                    </a>
                                                <?php endif; ?>
                                                <?php if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id'] , false, 'adm.php?modulo=principal/pessoafisica/formulario&acao=C')) : ?>
                                                    <a class="btn green" type="button" target="_blank" href="?modulo=principal/pessoafisica/formulario&acao=C<?= setParam(array('id' => $mPessoa->id)) ?>">
                                                        <i class="icon-user"></i>
                                                        Dados do paciente
                                                    </a>
                                                <?php endif; ?>

                                                <?php if($isAgendaPossuiGuia && $isVisualizaGuia) : ?>
                                                    <a class="btn green-jungle" type="button" href="javascript:;" onclick="carregarGuia(<?= $p['agenda_id'] ?>);">
                                                        <i class="fa fa-file-text"></i>
                                                        Guia
                                                    </a>
                                                <?php endif; ?>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <h4 class="form-section" style="margin-bottom: 0;">Dados da consulta</h4>
                    </div>
        <?php endif; ?>

                    <?php if ($alteracao) : ?>
                        <div class="row">
                            <div style="float: right; padding-right: 15px;">
                                <?php wf_desenhaBarraNavegacaoHorizontalM($documento_id, array('id' => $p['agenda_id']), null, null, 'fecharModalAlterarAgenda', 'Agenda do profissional disponibilizada') ?>
                            </div>
                            <div style="padding: 23px 0 0 15px; float: left;">
                                <?php if ($wf_estado_id == EstabelecimentoProfissionalAgenda::WF_ESTADO_DISPONIVEL && strtotime($inicio) >= strtotime(date('Y-m-d H:i:s'))) : ?>
                                    <button class="btn btn-success" type="button" title="Marcar consulta" onclick="marcarConsultaPaciente();">
                                                    <i class="fa fa-check-circle"></i>
                                                    Marcar consulta
                                    </button>
                                <?php endif; ?>

                                <?php if (!$iniciarAtendimento && in_array($wf_estado_id, array(EstabelecimentoProfissionalAgenda::WF_ESTADO_AGENDADA, EstabelecimentoProfissionalAgenda::WF_ESTADO_CONFIRMADA))) : ?>
                                    <button class="btn btn-success margin-bottom-5" type="button" title="Reagendar consulta" onclick="reagendarConsultaPaciente();">
                                                    <i class="fa fa-calendar"></i>
                                                    Reagendar consulta
                                    </button>
                                <?php endif; ?>

                                <?php
                                    if ($permissaoSolicitacaoCorrecao || $permissaoEfetuarCorrecao) :
                                        $textoBotaoCorrecao = $permissaoEfetuarCorrecao ? 'Efetuar correção' :  'Solicitar correção';
                                ?>
                                    <button class="btn purple margin-bottom-5" type="button" title="<?= $textoBotaoCorrecao ?>" onclick="solicitarCorrecao();">
                                        <i class="fa fa-eraser"></i>
                                        <?= $textoBotaoCorrecao ?>
                                    </button>
                                <?php endif; ?>

                                <?php if ($permissaoAlterarDadosConsulta) : ?>
                                    <button class="btn blue margin-bottom-5" type="button" title="Alterar dados da consulta" onclick="alterarDadosConsulta();">
                                        <i class="fa fa-edit"></i>
                                        Alterar dados da consulta
                                    </button>
                                <?php endif; ?>

                                <?php if ($permissaoExcluirHorarioAgenda) : ?>
                                    <button class="btn btn-danger" type="button" id="btn-excluir-horario-agenda">
                                                    <i class="fa fa-close"></i>
                                                    Excluir da agenda
                                    </button>
                                <?php endif; ?>
                            </div>
                        </div>
                    <?php endif; ?>

                    <div class="row">
                        <div class="col-sm-<?= (($realizada || $procedimento || ($valor_procedimento && ValorProcedimento::isVisualizaValores($p['id']))) ? 6 : 12) ?>">
                            <div class="row">
                                <div class="col-sm-12">
                                    <div class="form-group">
                                        <label>Profissional</label>
                                        <?php
                                        $profissionais = $mEstabelecimentoEspecialidadeProfissional->getComboProfissionaisPorEstabelecimento($p['id']);
                                        $db->monta_combom("profissional_id", $profissionais, $habilitado, 'Selecione...', '', '', '', '', 'S');
                                        ?>
                                    </div>
                                </div>
                            </div>
                            <div class="row">
                                <div class="col-sm-12">
                                    <div class="form-group">
                                        <label>Especialidade</label>
                                        <div id="combo-especialidade-container">
                                            <?php $jsComboDuracaoConsulta = comboEspecialidadesPorProfissionalEstabelecimento($profissional_id, $p['id'], $habilitado) ?>
                                        </div>
                                    </div>
                                </div>
                            </div>

                            <?php
                            $mSala = new SalaEstabelecimento();
                            $comboSalas = $mSala->getComboPorEstabelecimento($p['id']);
                            if (count($comboSalas) > 0 || $tipo_consulta_id): ?>
                                <div class="row">
                                    <div class="col-sm-12">
                                        <div class="form-group">
                                            <label>Sala</label>
                                            <?php
                                            $exibeSala = true;
                                            if ($tipo_consulta_id) :
                                                $mTipoConsulta = new TipoConsulta();
                                                if($mTipoConsulta->isConsultaRemota($tipo_consulta_id)) :
                                                    $exibeSala = false;
                                                    $urlSala = $mAgenda->gerarUrlAtendimentoRemoto($p['agenda_id']);
                                                    ?>
                                                    <br/>
                                                    <a href="?modulo=principal/medico/atendimento&acao=C<?= setParam(array('agenda_id' => $p['agenda_id'], 'estabelecimento_id' => $p['id'])) ?>" >Virtual (<?= $urlSala ?>)</a>
                                                <?php endif;
                                            endif;

                                            if($exibeSala) :
                                                $db->monta_combom("sala_estabelecimento_id", $comboSalas, $habilitado, 'Selecione...', '', '', '', '', 'N', '', false);
                                            endif;
                                            ?>
                                        </div>
                                    </div>
                                </div>
                            <?php endif; ?>
                            <?php
                            if ($tipo_consulta_id): ?>
                                <div class="row">
                                    <div class="col-sm-12">
                                        <div class="form-group">
                                            <label>Tipo de serviço</label>
                                            <br/>
                                            <?php
                                            $mTipoConsulta = new TipoConsulta($tipo_consulta_id);
                                            echo "<strong>{$mTipoConsulta->descricao}</strong>";
                                            ?>
                                        </div>
                                    </div>
                                </div>
                            <?php endif; ?>
                            <?php
                            if ($encaixe && $encaixe === 't'): ?>
                                <div class="row">
                                    <div class="col-sm-12">
                                        <div class="form-group">
                                            <label>Encaixe?</label>
                                            <br/>
                                            <strong>Sim</strong>
                                        </div>
                                    </div>
                                </div>
                            <?php endif; ?>
                            <div class="row">
                                <div class="col-sm-12">
                                    <div class="form-group">
                                        <label>Data</label>
                                        <?php
                                        if ($alteracao):
                                            $data_agenda = formata_data($data_agenda);
                                            echo campo_textom('data_agenda', 'S', 'N', '', 100, 10, '99/99/9999', '');
                                        else :
                                            echo campo_datam('data_agenda', 'S', $habilitado, '', 'S');
                                        endif;
                                        ?>
                                    </div>
                                </div>
                            </div>

                            <div class="row horario-container" style="display: none;">
                                <div class="col-sm-12">
                                    <div class="form-group">
                                        <div class="row" style="margin-bottom: 5px;">
                                            <div class="col-xs-6" style="padding-top: 10px;">
                                                <label>
                                                    Horário
                                                </label>
                                            </div>
                                            <?php if (!$alteracao) : ?>
                                                <div class="col-xs-6 text-right">
                                        <button class="btn btn-success" type="button" title="Adicionar horário" onclick="adicionarHorario();">
                                                        <i class="fa fa-plus"></i>
                                                    </button>
                                                </div>
                                            <?php endif; ?>
                                        </div>

                                        <?php if ($alteracao) : ?>
                                            <div class="form-group horario">
                                                <div class="input-group">
                                                    <span class="<?= $habilitado == 'S' ? 'input-group-addon' : 'input-group-addon-desabilitado' ?>">De</span>
                                                    <div>
                                                        <?= campo_textom('hora_inicio[]', 'S', $habilitado, '', 10, 10, '99:99', '', '', '', 0, '', '', $hora_inicio, '', null, null, null, 'timepicker timepicker-24 startdate'); ?>
                                                    </div>
                                                    <span class="<?= $habilitado == 'S' ? 'input-group-addon' : 'input-group-addon-desabilitado' ?>">Até</span>
                                                    <div>
                                                        <?= campo_textom('hora_fim[]', 'N', 'N', '', 10, 10, '99:99', '', '', '', 0, '', '', $hora_fim, '', null, null, null, 'timepicker timepicker-24 enddate', $habilitado == 'S'); ?>
                                                    </div>
                                                </div>
                                            </div>
                                        <?php else: ?>
                                            <div class="horarios" style="max-height: 250px; overflow-y: auto;"></div>
                                            <div class="form-group horario hidden" style="margin-top: 10px;">
                                                <div class="input-group">
                                                    <span class="input-group-addon">De</span>
                                                    <div>
                                                        <?= campo_textom('hora_inicio[]', 'S', $habilitado, '', 10, 10, '99:99', '', '', '', 0, '', '', $hora_inicio, '', null, null, null, 'timepicker timepicker-24 startdate'); ?>
                                                    </div>
                                                    <span class="input-group-addon">Até</span>
                                                    <div>
                                                        <?= campo_textom('hora_fim[]', 'N', 'N', '', 10, 10, '99:99', '', '', '', 0, '', '', null, '', null, null, null, 'timepicker timepicker-24 enddate', true); ?>
                                                    </div>
                                                    <span class="input-group-btn">
                                            <button class="btn btn-danger" type="button" title="Remover" onclick="removerHorario(this);">
                                                <i class="icon-trash"></i>
                                            </button>
                                        </span>
                                                </div>
                                            </div>
                                        <?php endif; ?>

                                        <small id="duracao-consulta" style="font-style: italic;"></small>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <?php if ($convenio && ConvenioEstabelecimento::isVisualizaConvenio($p['id'])) : ?>
                            <div class="col-md-6">
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Convênio</label>
                                            <br/>
                                            <strong><?= $convenio . (isset($numero_convenio_plano) ? " ({$numero_convenio_plano})": '' ) ?></strong>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        <?php endif; ?>
                        <?php if ($procedimento) : ?>
                            <div class="col-md-6">
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Procedimento</label>
                                            <br/>
                                            <strong><?= $procedimento ?></strong>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        <?php endif; ?>

                        <?php if ($valor_procedimento && ValorProcedimento::isVisualizaValores($p['id'])) : ?>
                            <div class="col-md-6">
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Valor</label>
                                            <br/>
                                            <strong>R$ <?= formata_valor($valor_procedimento) ?></strong>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        <?php endif; ?>

                        <?php if ($realizada) : ?>
                            <div class="col-sm-6">
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Início do atendimento</label>
                                            <div style="font-weight: bold;"><?= $mAgenda->getDataHoraAcao($p['agenda_id'], EstabelecimentoProfissionalAgenda::WF_ACAO_ATENDER) ?></div>
                                        </div>
                                    </div>
                                </div>
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Conclusão do atendimento</label>
                                            <div style="font-weight: bold;"><?= $mAgenda->getDataHoraAcao($p['agenda_id'], EstabelecimentoProfissionalAgenda::WF_ACAO_FINALIZAR_ATENDIMENTO) ?></div>
                                        </div>
                                    </div>
                                </div>
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Duração do atendimento</label>
                                            <div style="font-weight: bold;"><?= $duracao_consulta ?></div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        <?php endif; ?>
                    </div>

                    <?php if ($pessoa_paciente_id) : ?>
                </div> <!-- col-md-6 -->

                <div class="col-md-6" id="observacao-container">
                    <ul class="nav nav-tabs">
                        <li class="active">
                            <a href="#tab_observacao_agenda" data-toggle="tab"> Observações da Agenda </a>
                        </li>
                        <li>
                            <a href="#tab_observacao_pessoa" data-toggle="tab"> Observações da Pessoa </a>
                        </li>
                    </ul>

                    <div class="tab-content">
                        <div class="tab-pane fade active in" id="tab_observacao_agenda">
                            <div class="form">
                                <?= campo_textaream('observacao_agenda', 'N', 'S', '', '10', '5', '1000', '', 0, '') ?>
                                <div class="text-right">
                                    <button class="btn btn-success add-observacao-agenda" type="button" style="margin-top: 10px;"><i class="fa fa-plus fa-fw"></i> Adicionar</button>
                                </div>
                            </div>
                            <br/>

                            <div id="tabela-observacoes-container-agenda">
                                <?php include_once 'observacoesAgenda.inc'; ?>
                            </div>
                        </div>
                        <div class="tab-pane fade" id="tab_observacao_pessoa">
                            <div class="form">
                                <?= campo_textaream('observacao_pessoa', 'N', 'S', '', '10', '5', '1000', '', 0, '') ?>
                                <div class="text-right">
                                    <button class="btn btn-success add-observacao-pessoa" type="button" style="margin-top: 10px;"><i class="fa fa-plus fa-fw"></i> Adicionar</button>
                                </div>
                            </div>
                            <br/>

                            <div id="tabela-observacoes-container-pessoa">
                                <?php include_once 'observacoesPessoa.inc'; ?>
                            </div>
                        </div>
                    </div>
                </div>

            </div> <!-- row -->
        <?php endif; ?>
        </div>
        <div class="modal-footer">
            <?php if ($habilitado === 'S') : ?>
                <button type="submit" class="btn blue btn-incluir">
                    <i class="fa fa-plus"></i>
                    <?= ($alteracao ? 'Alterar agenda' : 'Incluir na agenda') ?>
                </button>
            <?php endif; ?>
            <button type="button" class="btn" data-dismiss="modal">Fechar</button>
        </div>
    </div>
    <input type="hidden" name='act' value="incluirAgenda"/>

    <script type="text/javascript">
        $(function () {
            var container = $('#agenda-formulario');
            adicionarLabelObrigatorio(container.find(':required'));
            adicionarDatePicker(container);
            configurarPopoverExcluirHorarioAgenda();

            $('[name=profissional_id]').change(function () {
                App.blockUI({
                    message: 'Carregando...',
                    target: '#combo-especialidade-container'
                });

                $('#combo-especialidade-container').load(window.location.href, {
                    act: 'filtrarEspecialidades',
                    profissional_id: $(this).val(),
                }, function () {
                    calcularHorarioFinal();
                    App.unblockUI('#combo-especialidade-container');
                });
            });

            setTimePicker();

            $("#form-abrir-agenda").validateForm({
                rules: {
                    data_agenda: {dateITA: true}
                },
                submitHandler: function (e) {
                    verificarHorario(function () {
                        var form = $("#form-abrir-agenda");
                        $('div.horario.hidden').remove();

                        App.blockUI({
                            message: 'Carregando...',
                            target: '#form-abrir-agenda-content'
                        });

                        var horaInicio = [];
                        $.each(form.find('[name="hora_inicio[]"]'), function () {
                            horaInicio.push($(this).val());
                        });

                        var horaFim = [];
                        $.each(form.find('[name="hora_fim[]"]'), function () {
                            horaFim.push($(this).val());
                        });

                        $.ajax({
                            type: "POST",
                            url: window.location.href,
                            dataType: 'json',
                            data: {
                                p: form.find('[name=p]').val(),
                                act: form.find('[name=act]').val(),
                                profissional_id: form.find('[name=profissional_id]').val(),
                                estabelecimento_especialidade_profissional_id: form.find('[name=estabelecimento_especialidade_profissional_id]').val(),
                                sala_estabelecimento_id: form.find('[name=sala_estabelecimento_id]').val(),
                                data_agenda: form.find('[name=data_agenda]').val(),
                                hora_inicio: horaInicio,
                                hora_fim: horaFim
                            },
                            success: function (retorno) {
                                App.unblockUI('#form-abrir-agenda-content');
                                exibirMsg(retorno.msg);
                                fecharModalAlterarAgenda();
                            },
                            error: function (result) {
                                App.unblockUI('#form-abrir-agenda-content');
                                exibirAlert('Não foi possível incluir na agenda');
                            }
                        });
                    });
                }
            });

            $('[name=profissional_id], [name=data_agenda]').change(function () {
                calcularHorarioFinal();
            });

            $.validator.addMethod("enddate", function (value, element) {
                var starttime = $('.startdate').val();
                var endtime = value;

                var dt = new Date();
                //convert both time into timestamp
                var stt = new Date((dt.getMonth() + 1) + "/" + dt.getDate() + "/" + dt.getFullYear() + " " + starttime);
                var endt = new Date((dt.getMonth() + 1) + "/" + dt.getDate() + "/" + dt.getFullYear() + " " + endtime);

                return endt > stt;
            }, "O horário final deve ser depois do horário inicial.");

            $('[name=data_agenda]:not(.disabled)').datepicker('destroy').datepicker({
                orientation: "left",
                autoclose: true,
                language: 'pt-BR',
                startDate: new Date()
            });

            <?php if($alteracao) : ?>
            $('[name="hora_inicio[]"]').change(function () {
                calcularHorarioFinal();
            });
            <?php else : ?>
            adicionarHorario();
            <?php endif; ?>

            <?php if($pessoa_paciente_id): ?>
            $('#modal-abrir-agenda').find('.modal-dialog').css({width: '95%'});

            $('.add-observacao-agenda').click(function () {
                var container = $('#agenda-formulario');
                var observacao = container.find('[name=observacao_agenda]').val();
                var p = container.find('[name=p]').val();

                if (observacao) {
                    App.blockUI({
                        message: 'Carregando...',
                        target: '#observacao-container'
                    });

                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        dataType: 'html',
                        data: {
                            act: 'adicionarObservacao',
                            p: p,
                            observacao: observacao
                        },
                        success: function (retorno) {
                            App.unblockUI('#observacao-container');
                            container.find('#tabela-observacoes-container-agenda').html(retorno);

                            container.find('[name=observacao_agenda]').val('');
                        },
                        error: function (result) {
                            App.unblockUI('#observacao-container');
                            exibirAlert('Não foi possível realizar a operação');
                        }
                    });
                }
            });

            $('.add-observacao-pessoa').click(function () {
                var container = $('#agenda-formulario');
                var observacao = container.find('[name=observacao_pessoa]').val();
                var p = container.find('[name=p]').val();

                if (observacao) {
                    App.blockUI({
                        message: 'Carregando...',
                        target: '#observacao-container'
                    });

                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        dataType: 'html',
                        data: {
                            act: 'adicionarObservacaoPessoa',
                            p: p,
                            observacao: observacao
                        },
                        success: function (retorno) {
                            App.unblockUI('#observacao-container');
                            container.find('#tabela-observacoes-container-pessoa').html(retorno);

                            container.find('[name=observacao_agenda]').val('');
                        },
                        error: function (result) {
                            App.unblockUI('#observacao-container');
                            exibirAlert('Não foi possível realizar a operação');
                        }
                    });
                }
            });
            <?php endif; ?>
        });

        function configurarPopoverExcluirHorarioAgenda() {
            $('#btn-excluir-horario-agenda').popover({
                placement: 'bottom',
                title: 'Você tem certeza que deseja excluir este <b>horário da agenda</b>?',
                html: true,
                popout : true,
                content: function() {
                    return '<div class="btn-group" style="width: 100%; text-align: center;">' +
                        '    <div style="margin: auto;">' +
                        '        <a class="btn btn-sm btn-success" data-apply="confirmation" onclick="excluirHorarioAgenda();">' +
                        '            <i class="icon-like"></i> ' +
                        '            Sim' +
                        '        </a>' +
                        '       <a class="btn btn-sm btn-danger" onclick="$(\'#btn-excluir-horario-agenda\').click();">' +
                        '           <i class="icon-close"></i> ' +
                        '           Não' +
                        '       </a>' +
                        '    </div>' +
                        '</div>';
                }
            });
        }

        function excluirHorarioAgenda() {
            App.blockUI({
                message: 'Aguarde...',
                target: '#form-abrir-agenda-content'
            });

            var container = $('#agenda-formulario');
            var p = container.find('[name=p]').val();

            var dataAjax = {
                'act': 'excluirHorarioAgenda',
                'p': p
            };
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: dataAjax,
                success: function (retorno) {
                    App.unblockUI('#form-abrir-agenda-content');
                    $('#modal-abrir-agenda').modal('hide');
                    atualizarAgendaCalendario();

                    exibirSucesso('Operação realizada com sucesso.');
                },
                error: function (result) {
                    App.unblockUI('#form-abrir-agenda-content');
                    exibirAlert('Não foi possível realizar a operação');
                }
            });
        }

        function verificarHorario(success, error) {
            success = success ? success : function (result) { };
            error = error ? error : function (result) { };

            var container = $('#agenda-formulario');

            var dataAgenda = container.find('#data_agenda').val();
            var profissionalId = container.find('[name=profissional_id]').val();
            var salaId = container.find('[name=sala_estabelecimento_id]').val();
            var p = container.find('[name=p]').val();

            var horario = [];
            container.find('[name="hora_inicio[]"]:visible').each(function () {
                var inicio = $(this).val();
                var fim = $(this).closest('.horario').find('[name="hora_fim[]"]').val();

                if (inicio && fim) {
                    horario.push({
                        inicio: inicio,
                        fim: fim
                    });
                }
            });

            if (dataAgenda === '' || horario.length === 0 || profissionalId === '') {
                success();
                return;
            }

            App.blockUI({
                message: 'Carregando...',
                target: '#form-abrir-agenda-content'
            });

            $.ajax({
                type: "POST",
                url: window.location.href,
                dataType: 'json',
                data: {
                    act: 'verificarHorario',
                    p: p,
                    data_agenda: dataAgenda,
                    horario: horario,
                    profissional_id: profissionalId,
                    sala_estabelecimento_id: salaId
                },
                success: function (retorno) {
                    App.unblockUI('#form-abrir-agenda-content');
                    if (retorno.disponibilidade || retorno.disponibilidade_sala) {
                        var horariosDisponiveis = true;

                        if (retorno.disponibilidade) {
                            $.each(retorno.disponibilidade, function (i, disponivel) {
                                horariosDisponiveis &= disponivel.disponivel;

                                if (!disponivel.disponivel) {
                                    addMsgErroCampo($('[name="hora_inicio[]"]:visible:eq(' + i + ')').closest('.input-group'), disponivel.msg);
                                }
                            });
                        }

                        if (retorno.disponibilidade_sala) {
                            $.each(retorno.disponibilidade_sala, function (i, disponivel) {
                                horariosDisponiveis &= disponivel.disponivel;

                                if (!disponivel.disponivel) {
                                    addMsgErroCampo($('[name="hora_inicio[]"]:visible:eq(' + i + ')').closest('.input-group'), disponivel.msg);
                                }
                            });
                        }

                        if (horariosDisponiveis) {
                            success(retorno);
                        }
                    } else {
                        success(retorno);
                    }
                },
                error: function (result) {
                    App.unblockUI('#form-abrir-agenda-content');
                    exibirAlert('Não foi possível realizar a operação');
                }
            });
        }

        function calcularHorarioFinal() {
            var horarioContainer = $('.horario-container');
            var idEspecialidade = $('[name=estabelecimento_especialidade_profissional_id]').val();
            var dataAgenda = $('[name=data_agenda]').val();

            if (!idEspecialidade || !dataAgenda) {
                horarioContainer.hide();
                return;
            } else {
                horarioContainer.show();
            }

            $('[name="hora_inicio[]"]').each(function () {
                var container = $(this).closest('div.horario');
                var horarioInicial = $(this).val();
                var campoHoraFim = container.find('[name="hora_fim[]"]');

                if (!idEspecialidade || !horarioInicial || !dataAgenda) {
                    campoHoraFim.val('');
                    return;
                }

                var duracaoConsulta = 0;
                if (typeof getDuracaoByEspecialidade === "function") {
                    duracaoConsulta = getDuracaoByEspecialidade(idEspecialidade);
                }
                campoHoraFim.val(somarMinutosAoHorario(dataAgenda, horarioInicial, duracaoConsulta));
            });
        }

        function somarMinutosAoHorario(dataAgenda, horaInicial, minutos) {
            var dataAgendaSeparada = dataAgenda.split('/');
            var horarioInicial = horaInicial.split(':');

            var tmsInicial = new Date(dataAgendaSeparada[2], (dataAgendaSeparada[1] - 1), dataAgendaSeparada[0], horarioInicial[0], horarioInicial[1]);
            var tmsFinal = moment(tmsInicial).add(parseInt(minutos), 'm').toDate();

            var hora = tmsFinal.getHours().toString().padStart(2, '0');
            var minuto = tmsFinal.getMinutes().toString().padStart(2, '0');

            return hora + ':' + minuto;
        }

        function adicionarHorario() {
            var divHorarios = $('div.horarios');
            var ultimoHorarioFinal = $("[name='hora_fim[]']:visible").last().val();

            var divHorario = $('div.horario.hidden').clone();
            divHorario.removeClass('hidden');
            divHorarios.append(divHorario);

            gerenciarBtnExclusaoHorario();

            $('[name="hora_inicio[]"]').unbind('change').change(function () {
                calcularHorarioFinal();
            });

            $("[name='hora_inicio[]']").inputmask("99:99", {placeholder: ''});
            $('[name="hora_inicio[]"]:visible').last().val(ultimoHorarioFinal).change();
            setTimePicker();

            divHorarios[0].scrollTop = divHorarios[0].scrollHeight;
        }

        function removerHorario(elem) {
            $(elem).closest('div.horario').remove();
            gerenciarBtnExclusaoHorario();
        }

        function gerenciarBtnExclusaoHorario() {
            var divsHorario = $('div.horarios div.horario');
            if (divsHorario.length > 1) {
                divsHorario.find('button.btn-danger').removeProp('disabled');
            } else {
                divsHorario.find('button.btn-danger').prop('disabled', true);
            }
        }

        function setTimePicker() {
            $('.timepicker-24').timepicker({
                autoclose: true,
                minuteStep: 1,
                showSeconds: false,
                showMeridian: false,
                defaultTime: null,
                format: 'HH:mm'
            }).on('keydown', function (e) {
                if (e.keyCode === 9) {
                    $(this).timepicker('hideWidget');
                }
            }).on('show.timepicker', function (e) {
                if (e.time.value === '') {
                    $(this).timepicker('setTime', '08:00');
                }
            });
        }

        function marcarConsultaPaciente() {
            $('#modal-abrir-agenda').modal('hide');

            abrirFormularioPacienteConsulta({
                agenda_id: '<?= $p['agenda_id'] ?>',
                act: 'selecionarPacienteConsulta'
            });

            $('#modal-marcar-consulta').modal({
                backdrop: 'static',
            }).modal('show');
        }

        function solicitarCorrecao() {
            $('#modal-abrir-agenda').modal('hide');

            abrirFormularioSolicitacaoCorrecao({
                agenda_id: '<?= $p['agenda_id'] ?>',
                act: 'solicitarCorrecao'
            });

            $('#modal-solicitacao-correcao').modal({
                backdrop: 'static',
            }).modal('show');
        }


        function alterarDadosConsulta() {
            $('#modal-abrir-agenda').modal('hide');

            abrirFormularioPacienteConsulta({
                agenda_id: '<?= $p['agenda_id'] ?>',
                tipo_formulario: 'alterar-dados-consulta',
                act: 'selecionarPacienteConsulta'
            });

            $('#modal-marcar-consulta').modal({
                backdrop: 'static',
            }).modal('show');
        }

        function reagendarConsultaPaciente() {
            $('#modal-abrir-agenda').modal('hide');

            abrirFormularioPacienteConsulta({
                reagenda_id: '<?= $p['agenda_id'] ?>',
                tipo_formulario: 'reagendar',
                act: 'formularioMarcarConsulta'
            });

            $('#modal-marcar-consulta').modal({
                backdrop: 'static',
            }).modal('show');
        }

        function fecharModalAlterarAgenda(msg) {
            if (msg) {
                exibirMsg(msg);
            }

            $('#modal-abrir-agenda').modal('hide');
            atualizarAgendaCalendario();
        }

        function alterarFoto(input){
            var img, file, form, formData;
            file = input.files[0];

            if (!file.type.match(/image.*/)) {
                exibirAlert('Não foi possível alterar a imagem.');
                return false;
            }

            App.blockUI({
                message: 'Carregando...',
                target: '#form-abrir-agenda-content'
            });

            $('[name=act]').val('alterarFoto');
            form = $('[name=form-abrir-agenda]');
            formData = new FormData(form[0]);
            $.ajax({
                type: "POST",
                url: window.location.href + '&act=alterarFoto',
                data: formData,
                contentType: false,
                enctype: 'multipart/form-data',
                dataType : 'json',
                processData: false,
                success: function (retorno) {
                    App.unblockUI('#form-abrir-agenda-content');
                    if(retorno.params.foto_arquivo_id) {
                        img = '<img src="../slideshow/slideshow/verimagem.php?arqid=' + retorno.params.foto_arquivo_id + '" alt="" style="width: 180px; height: 180px" class="img-circle bg-white foto-pessoa" />';
                    }

                    $(form).find('#div-imagem').html(img);
                    exibirSucesso('Imagem alterada com sucesso.');

                },
                error: function (result) {
                    App.unblockUI('#form-abrir-agenda-content');
                    exibirAlert('Não foi possível realizar a operação');
                }
            });
        }

        <?php if($isVisualizaGuia) : ?>
            function carregarGuia(agendaId)
            {
                $('#modal-abrir-agenda').modal('hide');
                $('#modal-guia').modal({
                    backdrop: 'static',
                }).modal('show');

                let guiaForm = $('form.guia-form');

                App.blockUI({
                    target : guiaForm,
                    message : 'Carregando...'
                });

                guiaForm.load(window.location.href, {
                    act : 'formularioGuia',
                    agenda_id : agendaId
                }, function() {
                    App.unblockUI(guiaForm);
                });
            }
        <?php endif; ?>
    </script>

<?= $jsComboDuracaoConsulta; ?>

<?php
function comboEspecialidadesPorProfissionalEstabelecimento($profissionalId, $estabelecimentoId, $habilitadoParam = 'S')
{
    global $db, $mEstabelecimentoEspecialidadeProfissional;

    $especialidadesCombo = array();
    $especialidadeDuracao = array();
    $habilitado = 'N';
    $labelSelecione = 'Primeiro, selecione o profissional';
    $jsOnload = '';

    if ($profissionalId && $estabelecimentoId) {
        $especialidades = $mEstabelecimentoEspecialidadeProfissional->getEspecialidadesPorProfissionalEstabelecimento($profissionalId, $estabelecimentoId);
        $labelSelecione = 'Nenhuma especialidade encontrada';

        if (count($especialidades) > 0) {
            $habilitado = $habilitadoParam;
            $labelSelecione = 'Selecione a especialidade';

            foreach ($especialidades as $especialidade) {
                $especialidadesCombo[] = array(
                    'descricao' => $especialidade['especialidade_descricao'],
                    'codigo' => $especialidade['estabelecimento_especialidade_profissional_id'],
                );

                $especialidadeDuracao[$especialidade['estabelecimento_especialidade_profissional_id']] = $especialidade['duracao_consulta'];
            }
        }

        $especialidadeDuracao = json_encode($especialidadeDuracao);

        $jsOnload = <<<JS
            <script type='text/javascript'>
                $(function(){
                    $('[name=estabelecimento_especialidade_profissional_id]').change(function(){
                        var idEspecialidade = $(this).val();
                        var msg = idEspecialidade ? 'Duração da consulta: ' + getDuracaoByEspecialidade(idEspecialidade) + ' minutos' : '';
                        
                        $('#duracao-consulta').html(msg);

                        calcularHorarioFinal();
                    }).trigger('change');
                });
                
                function getDuracaoByEspecialidade(id){
                    var duracaoEspecialidades = {$especialidadeDuracao};
                    return duracaoEspecialidades[id];
                }
            </script>
JS;
    }

    $value = null;
    if (count($especialidadesCombo) == 1) {
        $value = $especialidadesCombo[0]['codigo'];
    }

    $db->monta_combom("estabelecimento_especialidade_profissional_id", $especialidadesCombo, $habilitado, $labelSelecione, '', '', '', '', 'S', '', false, $value);

    return $jsOnload;
}
