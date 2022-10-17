<?php
$p = getParam();
$estiloTipo = TipoProntuario::getEstilo();

$mAgenda = new EstabelecimentoProfissionalAgenda();
$mTag = new TagPaciente();
$mProntuarioCid = new ProntuarioCid();

global $teleconsulta_url;
global $teleconsulta_token;
global $whatsapp_template;
global $telegram_template;

$prepararDadosAgenda = function ($agendaId) {
    $mAgenda = new EstabelecimentoProfissionalAgenda();
    $dadosAgenda = $mAgenda->getTodosDados($agendaId);
    $emAtendimento = $dadosAgenda['wf_estado_id'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_EM_ATENDIMENTO;
    $p = getParam();

    if ($dadosAgenda['wf_estado_id'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_AGUARDANDO_ATENDIMENTO
        && isset($p['iniciar_atendimento']) && $p['iniciar_atendimento'] == 'true') {
        $mAgenda->atender($p['agenda_id']);
        $mAgenda->commit();

        // Pega os dados atualizados da agenda
        $dadosAgenda = $mAgenda->getTodosDados($p['agenda_id']);
        $emAtendimento = true;
    }

    $permiteAlteracaoTempo = false;
    if ($dadosAgenda['wf_estado_id'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA) {
        $permiteAlteracaoTempo = $mAgenda->isAlteracaoAgendaDisponivel($agendaId);
    }

    $duracaoConsulta = $mAgenda->calcularDuracaoRealizacaoConsulta($agendaId);
    $pessoaPacienteId = $dadosAgenda['pessoa_paciente_id'];
    $telaAtendimento = true;
    $podeAtender = $mAgenda->isUsuarioPodeAtender($p['agenda_id']);

    return array(
        'dadosAgenda' => $dadosAgenda,
        'emAtendimento' => $emAtendimento,
        'permiteAlteracaoTempo' => $permiteAlteracaoTempo,
        'duracaoConsulta' => $duracaoConsulta,
        'pessoaPacienteId' => $pessoaPacienteId,
        'telaAtendimento' => $telaAtendimento,
        'podeAtender' => $podeAtender
    );
};

if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'prontuarioFormulario' :
        case 'salvarProntuario' :
        case 'salvarAnexo' :
        case 'downloadAnexo' :
        case 'filtrarCIDCombo' :
            include_once 'prontuarioFormulario.inc';
            die;
            break;
        case 'imprimir' :
            include_once 'prontuarioImprimir.inc';
            die;
            break;
        case 'listarProntuario' :
            listarProntuario($p['pessoa_paciente_id'], $_POST);
            die;
            break;
        case 'finalizarAtendimento' :
            $mAgenda->finalizarAtendimento($p['agenda_id']);
            $mAgenda->commit();

            addMsgInfo('Atendimento finalizado');
            $html = '<script type="text/javascript">if(opener != null && opener.location != null){opener.location.reload()}else{window.location=\'adm.php?modulo=inicio&acao=C\'};</script> ';
            die($html);
            die;
            break;
        case 'incluirTagPaciente' :
            $mTag = new TagPaciente();
            $mTag->adicionarTagPaciente(utf8_decode($_REQUEST['descricao']), $p['pessoa_paciente_id']);
            $db->commit();
            $db->sucessomAjax();
            die;
            break;
        case 'excluirTagPaciente' :
            $mTag->excluirTagPaciente(utf8_decode($_REQUEST['descricao']), $p['pessoa_paciente_id']);
            $db->commit();
            $db->sucessomAjax();
            die;
            break;
        case 'excluirRegistroProntuario' :
            $mProntuario = new Prontuario();
            $mProntuario->excluirProntuario($p['prontuario_id']);
            $db->commit();
            $db->sucessomAjax();
            die;
            break;
        case 'enviarLinkJitsiPorEmail' :
            $mMensagem = new Mensagem();
            $retorno = $mMensagem->enviarMensagem($_REQUEST['mensagem'], $_REQUEST['destinatario'], TipoMensagem::TIPO_MENSAGEM_EMAIL, NotificacaoAtendimento::CATEGORIA_LINK_TELECONSULTA);
            if(isset($retorno['id'])){
                die('ok');
            }else{
                die('erro');
            }
            break;
        case 'enviarLinkJitsiPorSMS' :
            $mMensagem = new Mensagem();
            $retorno = $mMensagem->enviarMensagem($_REQUEST['mensagem'], $_REQUEST['destinatario'], TipoMensagem::TIPO_MENSAGEM_SMS, NotificacaoAtendimento::CATEGORIA_LINK_TELECONSULTA);
            if(isset($retorno['id'])){
                die('ok');
            }else{
                die('erro');
            }
            break;
        case 'salvarAssinado':
            $paramsPost = utf8ArrayDecode($_POST);
            include_once APPRAIZ . "adm/servicos/ProntuarioServico.class.inc";
            $prontuarioServico = new ProntuarioServico();
            $prontuarioServico->salvarProntuarioAssinado($paramsPost['pdfAssinadoBase64'], $paramsPost['entidade']['prontuario_id']);
            die(
            json_encode(
                utf8ArrayEncode(
                    array(
                        'salvo' => true,
                    )
                )
            )
            );
            break;
    }
}

$dadosAgenda = array();
$telaAtendimento = false;
$podeAtender = false;
$emAtendimento = false;
$pessoaPacienteId = null;
$permiteAlteracaoTempo = false;

if (isset($p['agenda_id']) && !empty($p['agenda_id'])) {
    extract($prepararDadosAgenda($p['agenda_id']));
} else if (isset($p['pessoa_paciente_id']) && !empty($p['pessoa_paciente_id'])) {
    $pessoaPacienteId = $p['pessoa_paciente_id'];
} else {
    addMsgDanger('Por favor, selecione a consulta.');
    header('Location: ?modulo=principal/medico/minhasConsultas&acao=C');
    die;
}
?>

<?php
$mPessoa = new PessoaFisica();
$dadosPaciente = $mPessoa->getTodosDados($pessoaPacienteId);
include APPRAIZ . "includes/cabecalhom.inc";

EstabelecimentoProfissionalAgenda::montaAvisoAtendimento();

$mContatoPessoaFisica = new ContatoPessoaFisica();
$contatosPaciente = null;
$contatosPaciente = $mContatoPessoaFisica->listarEspecificosPorPessoaFisica($pessoaPacienteId, true);

//monta_titulom($emAtendimento ? 'Atendimento' : 'Prontuário');
monta_titulom($emAtendimento ? '' : 'Prontuário');
?>
    <!-- incluindo script do mnemed antes do cabecalho para evitar problemas com resuloções pequenas -->
    <script type="text/javascript" src="//memed.com.br/modulos/plataforma.sinapse/build/sinapse.min.js"
            data-api-key="$2y$10$r75QRT0YCGXn3QI8qdL/w.YL3B0GWk7Fn1BCYLD071CF7tdlGaWOm"
            data-usuario="<?= $_SESSION['usuario_id'] ?>"
            data-width="55%" data-font="Open Sans" data-color="#0A6FFF"></script>

    <script src="/includes/hwcrypto/hwcrypto-legacy.js"></script>
    <script src="/includes/hwcrypto/hwcrypto.js"></script>
    <script src="/includes/hwcrypto/hex2base.js"></script>
    <script src="/includes/assinatura/assinatura.js"></script>


    <link href="/includes/lightbox2/css/lightbox.css" rel="stylesheet" type="text/css"/>
    <style type="text/css">

        .portlet-title {
            border-bottom: 0px !important;
            padding-bottom: 0px !important;
            margin-bottom: 0px !important;
        }

        .portlet-body {
            padding-top: 0px !important;
        }

        .miniatura-imagem {
            margin: 5px;
            float: left;
            background-color: #FFF;
            max-width: 170px;
            max-height: 170px;
            overflow: hidden;
            border: 1px solid #e7ecf1;
            padding: 10px;
        }

        .miniatura-imagem .miniatura-imagem-container {
            overflow: hidden;
            height: 150px;
        }

        .miniatura-imagem img {
            height: 100%;
            /*margin-left: -50%;*/
            /*height: 150px;*/
        }

        .resumo-agenda {
            text-align: left;
            padding: 5px;
        }

        .resumo-agenda.resumo-agenda-tooltip > div {
            line-height: 60%;
        }

        .resumo-agenda > div:not(:last-child) {
            margin-bottom: 10px;
        }

        .resumo-agenda label {
            font-weight: bold;
        }

        .nav-prontuario > .nav-tabs > li.active > a, .nav-tabs > li.active > a:hover {
            background-color: #FFF;
        }

        .nav-prontuario > .nav-tabs > li > a {
            color: #555;
        }

        <?php foreach ($estiloTipo as $siglaTipo => $dadosEstiloTipo): ?>
        .nav-prontuario > .nav-tabs > li.<?= $siglaTipo ?>.active {
            border-left: 3px solid #<?= $dadosEstiloTipo['estilo']['cor_hexadecimal'] ?>;
            position: relative;
        }

        .nav-prontuario > .nav-tabs > li.<?= $siglaTipo ?> > a {
            color: #<?= $dadosEstiloTipo['estilo']['cor_hexadecimal'] ?>;
        }

        <?php endforeach ?>

        .data-prontuario {
            text-align: center;
            font-weight: bolder;
            font-size: 10px;
            padding-top: 17px;
            line-height: 14px;
            cursor: pointer;
        }

        .data-prontuario .dia {
            font-size: 25px !important;
        }

        .data-prontuario .hora {
            font-weight: normal;
        }

        .nav-prontuario .tab-content {
            border: 1px solid #c2c6c8;
            border-top: 0;
            margin-top: 0;
            padding: 10px 0px 10px 10px;
        }

        .nav-prontuario .nav-tabs {
            margin-bottom: 0px;
        }

        .timeline .timeline-icon {
            width: 80px;
            height: 80px;
            color: #ffffff;
            -webkit-border-radius: 50% !important;
            -moz-border-radius: 50% !important;
            border-radius: 50% !important;
            padding-top: 0px !important;
            padding-left: 0px !important;
        }

        .timeline .timeline-badge {
            top: -10px;
        }

        .timeline .timeline-body {
            padding: 0px;
            margin-top: 10px;
            margin-bottom: 35px;
        }

        .timeline .timeline-body-arrow {
            position: absolute;
            top: 0px;
            left: -14px;
            width: 0;
            height: 0;
            border-style: solid;
            border-width: 0 14px 14px 0;
            border-color: transparent #e8e9ed transparent transparent;
        }

        .timeline:before {
            background-color: #bbbbbb;
        }

        .timeline-short-body {
            max-height: 50px !important;
        }

        .timeline .seta-prontuario {
            color: #cacaca;
            font-size: 12px;
            vertical-align: text-top;
        }

        .timeline .timeline-body-head {
            padding: 10px 5px 10px 5px;
            height: 50px;
            background-color: #e8e9ed;
        }

        .timeline .timeline-body-head-caption {
            cursor: pointer;
        }

        .timeline .timeline-body-content {
            margin-top: 0px;
            font-size: 14px;
            padding: 20px;
        }

        .timeline .btn-action-timeline {
            color: #555;
        }
        #teleconsulta_link_paciente:focus {
            border: none!important;
            outline: unset!important;
            box-shadow: unset!important;
        }
    </style>

    <div class="form">
        <div class="row">
            <div class="col-md-3">
                <div class="row">
                    <?php if ($telaAtendimento && $podeAtender && ($emAtendimento || $dadosAgenda['wf_estado_id'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_AGUARDANDO_ATENDIMENTO)) : ?>
                        <div class="col-md-12">
                            <?php if ($telaAtendimento && $podeAtender && $dadosAgenda['wf_estado_id'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_AGUARDANDO_ATENDIMENTO): ?>
                                <div class="actions">
                                    <?php if (!$mAgenda->isAtendendoOutro($p['agenda_id'], $dadosAgenda['estabelecimento_id'], $dadosAgenda['profissional_id'])): ?>
                                        <button class="btn btn-success btn-block" type="button"
                                                id="btn-iniciar-atendimento" style="font-size: 16px;">
                                            <i class="fa fa-check-circle"></i>
                                            Iniciar atendimento
                                        </button>
                                    <?php else: ?>
                                        <button class="btn btn-block" type="button" style="font-size: 16px;"
                                                title="Finalize os atendimentos pendentes para iniciar esse.">
                                            <i class="fa fa-ban"></i>
                                            Iniciar atendimento
                                        </button>
                                    <?php endif; ?>
                                    <script type="text/javascript">
                                        $(function () {
                                            $('#btn-iniciar-atendimento').popover({
                                                placement: 'top',
                                                title: 'Deseja iniciar o atendimento?',
                                                html: true,
                                                popout: true,
                                                content: function () {
                                                    return '<div class="btn-group" style="width: 100%; text-align: center;">' +
                                                        '    <div style="margin: auto;">' +
                                                        '        <a class="btn btn-sm btn-success" data-apply="confirmation" href="?modulo=principal/medico/atendimento&acao=C<?= setParam(array('agenda_id' => $p['agenda_id'], 'iniciar_atendimento' => 'true')) ?>">' +
                                                        '            <i class="icon-like"></i> ' +
                                                        '            Sim' +
                                                        '        </a>' +
                                                        '       <a class="btn btn-sm btn-danger" onclick="$(\'#btn-iniciar-atendimento\').click();">' +
                                                        '           <i class="icon-close"></i> ' +
                                                        '           Não' +
                                                        '       </a>' +
                                                        '    </div>' +
                                                        '</div>';
                                                }
                                            });
                                        });
                                    </script>
                                </div>
                            <?php elseif ($emAtendimento) : ?>
                                <div class="mt-widget-1" style="text-align: left; padding: 10px;">
                                    <strong>Duração da consulta</strong>
                                    <div style="border: 1px solid #eef1f5; padding: 15px; margin-top: 5px; text-align: center; font-size: 36px;">
                                        <i class="fa fa-clock-o blue-oleo"></i>
                                        <span class="time"></span>
                                    </div>
                                    <?php if ($podeAtender) : ?>
                                        <?php if ($dadosAgenda['tipo_consulta'] == 'Consulta remota' || $dadosAgenda['tipo_consulta'] == 'Retorno remoto') : ?>
                                            <!--<button type="button" class="btn blue btn-block" style="font-size: 16px;"
                                                    onclick="exibirModalTeleconsulta(
                                                        '<?= $teleconsulta_url ?>',
                                                        '<?= $teleconsulta_token ?>',
                                                        '<?= $whatsapp_template ?>',
                                                        '<?= $telegram_template ?>',
                                                        '<?= $dadosAgenda['profissional'] ?>',
                                                        '<?= $dadosAgenda['data_inicio'] . ' ' . substr($dadosAgenda['hora_inicio'], 0, 5) ?>',
                                                        '<?= $dadosAgenda['data_fim'] . ' ' . substr($dadosAgenda['hora_fim'], 0, 5) ?>'    )">
                                                <i class="fa fa-camera-retro" style="margin-right: 10px"></i>Iniciar
                                                teleconsulta
                                            </button>-->
                                            <button type="button" class="btn blue btn-block" id="btn_jitsi_iniciar" style="font-size: 16px;" onclick="initJitsiIframeAPI()">
                                                <i class="fa fa-camera-retro" style="margin-right: 10px"></i>Iniciar teleconsulta
                                            </button>
                                            <button type="button" class="btn red btn-block" id="btn_jitsi_finalizar" style="display:none;font-size: 16px;" onclick="finishJitsiIframeAPI()">
                                                <i class="fa fa-camera-retro" style="margin-right: 10px"></i>Finalizar teleconsulta
                                            </button>
                                        <?php endif; ?>
                                        <button type="button" class="btn red btn-block" style="font-size: 16px;"
                                                onclick="exibirModalfinalizarAtendimento();">
                                            <i class="fa fa-check"></i> Finalizar atendimento
                                        </button>
                                    <?php endif; ?>
                                </div>
                            <?php endif; ?>
                        </div>
                    <?php endif; ?>
                    <div class="col-md-12">
                        
                        <div class="mt-widget-1 div_jitsi" style="display:none;height:400px">
                            <div style="width:100%;height:100%" id="jitsi_room_consulta"></div>
                        </div>
                        <div class="div_jitsi mt-widget-1" style="display:none; margin: 10px 0; padding: 5px; text-align: center;">
                            <label style="margin-right: 20px; cursor: pointer" onclick="copiarLinkJitsi()" name="share_link" title="Copiar Link da Consulta" >
                                <i class="fa fa-link" style="font-size: 1.8em;  background: #80808069;
                                    padding: 15px 10px 10px 10px;  color: black;
                                    border-radius: 50px!important; width: 50px; height: 50px;">
                                </i>
                            </label>
                            <?php if($contatosPaciente != null && isset($contatosPaciente['contato_email'])): ?>
                            <label style="margin-right: 20px; cursor: pointer" onclick="enviarLinkJitsiPorEmail('<?=$contatosPaciente['contato_email']?>')" name="share_link_email" title="Enviar Link da Consulta por E-mail" >
                                <i class="fa fa-envelope-o" style="font-size: 1.8em;  background: #80808069;
                                    padding: 15px 10px 10px 10px;  color: black;
                                    border-radius: 50px!important; width: 50px; height: 50px;">
                                </i>
                            </label>
                            <?php endif; ?>
                            <?php if($contatosPaciente != null && isset($contatosPaciente['contato_email'])): ?>
                            <label style="margin-right: 20px; cursor: pointer" onclick="enviarLinkJitsiPorWhastApp('<?=$contatosPaciente['contato_celular']?>')" name="share_link_email" title="Enviar Link da Consulta por WhatsApp" >
                                <i class="fa fa-whatsapp" style="font-size: 1.8em;  background: #80808069;
                                    padding: 15px 10px 10px 10px;  color: black;
                                    border-radius: 50px!important; width: 50px; height: 50px;">
                                </i>
                            </label>
                            <?php endif; ?>
                            <?php if($contatosPaciente != null && isset($contatosPaciente['contato_email'])): ?>
                            <label style="margin-right: 20px; cursor: pointer" onclick="enviarLinkJitsiPorSMS('<?=$contatosPaciente['contato_celular']?>')" name="share_link_email" title="Enviar Link da Consulta por SMS" >
                                <i class="fa fa-comment-o" style="font-size: 1.8em;  background: #80808069;
                                    padding: 15px 10px 10px 10px;  color: black;
                                    border-radius: 50px!important; width: 50px; height: 50px;">
                                </i>
                            </label>
                            <?php endif; ?>
                            <br/>
                            <input name="hidden_jitsi_link" id="hidden_jitsi_link" readonly="true" style="width:90%;padding:10px;border:solid 1px" />
                        </div>
                        <div class="mt-widget-1">
                            <div class="mt-img" id="div_foto_paciente" >
                                <?= montaImgFoto($dadosPaciente['foto_arquivo_id'], $dadosPaciente['sexo'], '180px', '', '200', '200', 'img-circle bg-white', true) ?>
                            </div>
                            <div class="mt-body">
                                <div class="text-left" style="padding: 10px;">
                                    <?= campo_textom('tags', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', $mTag->getStrTagsPorPaciente($dadosPaciente['id']), '', null, null, 'Tags do paciente'); ?>
                                </div>
                            </div>
                        </div>
                    </div>
                    <?php if ($p['agenda_id'] && $podeAtender && (in_array($dadosAgenda['wf_estado_id'], array(EstabelecimentoProfissionalAgenda::WF_ESTADO_AGUARDANDO_ATENDIMENTO, EstabelecimentoProfissionalAgenda::WF_ESTADO_EM_ATENDIMENTO)) || $permiteAlteracaoTempo)) : ; ?>
                        <div class="col-md-12">
                            <div class="mt-widget-1">
                                <div class="mt-body" style="padding-top: 5px;">
                                    <h3 class="mt-username font-blue-steel">
                                        <i class="fa fa-calendar font-blue-steel"></i> Agenda
                                    </h3>
                                    <p class="mt-user-title">
                                        <strong>Data:</strong> <?= formata_data($dadosAgenda['data_inicio']) ?>
                                        <br/>
                                        <strong>Hora:</strong> <?= substr($dadosAgenda['hora_inicio'], 0, 5) ?>
                                        - <?= substr($dadosAgenda['hora_fim'], 0, 5) ?>
                                        <br/>
                                        <strong>Procedimento:</strong> <?= $dadosAgenda['procedimento'] ?>
                                        <br/>
                                        <strong>Tipo de
                                            serviço:</strong> <?= ($dadosAgenda['tipo_consulta'] ? $dadosAgenda['tipo_consulta'] : '-') ?>
                                        <br/>
                                        <strong>Responsável:</strong> <?= $dadosAgenda['profissional'] ?>
                                    </p>
                                </div>
                            </div>
                        </div>
                    <?php endif; ?>
                </div>
            </div>
            <div class="col-md-9">
                <div class="portlet light portlet-fit bordered" style="background-color: transparent;">
                    <div class="portlet-title">
                        <div class="caption">
                            <i class="icon-user font-blue-steel"></i>
                            <span class="caption-subject bold font-blue-steel uppercase"> Histórico do paciente</span>
                            <div style="clear:both;font-size:14px" >
                                <br/>
                                <strong><?= $dadosPaciente['nome_completo'] ?></strong><br/>
                                <strong>Idade: </strong>
                                <?= calcularIdade($dadosPaciente['data_nascimento']) ?>
                                <br/>
                                <strong>Nascimento: </strong>
                                <?= formata_data($dadosPaciente['data_nascimento']) ?>
                                <br/>
                                <strong>Sexo: </strong>
                                <?= (($dadosPaciente['sexo']) == 'F' ? 'Feminino' : 'Masculino') ?>
                            </div>
                        </div>
                        <?php if ($p['agenda_id'] && $telaAtendimento && $podeAtender && ($emAtendimento || $permiteAlteracaoTempo)): ?>
                            <div class="actions">
                                <!--<strong>Adicionar</strong>-->
                                <?php foreach ($estiloTipo as $sigla => $tipo) : ?>
                                    <a class="btn <?= $tipo['estilo']['classe_cor'] ?>"
                                       href="javascript:adicionarProntuario('<?= $sigla ?>');"
                                       style="margin-bottom: 3px;">
                                        <i class="fa fa-plus-circle"></i>
                                        <?= $tipo['descricao'] ?>
                                    </a>
                                <?php endforeach; ?>
                            </div>
                        <?php endif; ?>
                    </div>
                    <div id="lista-prontuario" class="portlet-body">
                        <div>
                            <form class="form-pesquisar" method="post">
                                <div class="row pesquisa-simples">
                                    <div class="col-md-12">
                                        <div class="input-group">
                                            <?= campo_textom('pesquisa_simples', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquisar no prontuário'); ?>
                                            <span class="input-group-btn" data-toggle="tooltip" title="Limpar Pesquisa"
                                                  style="display: none;">
                                            <button class="btn red btn-limpar-pesquisa" type="button">
                                                <i class="fa fa-times"></i>
                                            </button>
                                        </span>
                                            <span class="input-group-btn">
                                            <button class="btn blue" type="submit" data-toggle="tooltip"
                                                    title="Pesquisar">
                                                <i class="fa fa-search"></i>
                                            </button>
                                        </span>
                                            <span class="input-group-btn" data-toggle="tooltip"
                                                  title="Pesquisa avançada">
                                            <button class="btn green-jungle btn-pesquisa-avancada" type="button">
                                                <i class="fa fa-search-plus"></i>
                                            </button>
                                        </span>
                                        </div>
                                    </div>
                                </div>
                                <div class="row pesquisa-avancada hidden">
                                    <div class="col-xs-6 col-sm-6 col-md-6 col-lg-3">
                                        <label class="control-label">Descrição</label>
                                        <?= campo_textom('descricao', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', '', '', null, null, 'Qualquer descrição no prontuário'); ?>
                                    </div>
                                    <div class="col-xs-6 col-sm-6 col-md-6 col-lg-4">
                                        <label class="control-label">Tipo</label>
                                        <select id="tipo_prontuario" name="tipo_prontuario" multiple
                                                style="display: none;"></select>
                                    </div>
                                    <div class="col-xs-12 col-sm-12 col-md-12 col-lg-5">
                                        <label class="control-label">Data</label>
                                        <div class="input-group">
                                            <span class="input-group-addon">De</span>
                                            <div>
                                                <?= campo_datam('data_de', 'N', 'S', '', 'S'); ?>
                                            </div>
                                            <span class="input-group-addon">Até</span>
                                            <div>
                                                <?= campo_datam('data_ate', 'N', 'S', '', 'S'); ?>
                                            </div>
                                            <span class="input-group-btn" data-toggle="tooltip" title="Limpar Pesquisa"
                                                  style="display: none;">
                                            <button class="btn red btn-limpar-pesquisa" type="button">
                                                <i class="fa fa-times"></i>
                                            </button>
                                        </span>
                                            <span class="input-group-btn">
                                        <button type="submit" class="btn blue btn-pesquisar" data-toggle="tooltip"
                                                title="Pesquisar">
                                            <i class="fa fa-search"></i>
                                        </button>
                                    </span>
                                            <span class="input-group-btn">
                                        <button type="button" class="btn green-jungle btn-pesquisa-simples"
                                                data-toggle="tooltip" title="Pesquisa simples">
                                            <i class="fa fa-search-minus"></i>
                                        </button>
                                    </span>
                                        </div>
                                    </div>
                                </div>
                            </form>
                        </div>
                        <br>
                        <div class="conteudo">
                            <?php listarProntuario($pessoaPacienteId); ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div class="modal fade in" id="modal-prontuario" tabindex="-1" role="modal-prontuario" aria-hidden="true">
        <div class="modal-dialog" style="width: 90%;" id="div_class_modal_prontuario" >
            <div class="modal-content">
                <form id="form-prontuario" method="post">
                    <div id="form-prontuario-content"></div>
                </form>
            </div>
        </div>
    </div>

    <form method="post" class="form-finalizar-atendimento">
        <input type="hidden" name="act" value="finalizarAtendimento"/>
        <input type="hidden" name="p" value="<?= setParam($p, false); ?>"/>
    </form>

    <div class="modal fade in" id="modal-finalizar-atendimento" tabindex="-1" role="modal-finalizar-atendimento"
         aria-hidden="true">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Finalizar atendimento</h4>
                </div>
                <div class="modal-body">
                    Deseja finalizar o atendimento?
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn blue" onclick="finalizarAtendimento();">Sim</button>
                    <button type="button" class="btn red" data-dismiss="modal">Não</button>
                </div>
            </div>
            <!-- /.modal-content -->
        </div>
        <!-- /.modal-dialog -->
    </div>


    <div class="modal fade in" id="modal-teleconsulta" tabindex="-1" role="modal-teleconsulta"
         aria-hidden="true">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title" style="font-weight: 1000">Teleconsulta</h4>
                </div>
                <div class="modal-body">
                    <div style="font-size: 1.2em!important; font-weight: 800!important">
                        Este são os dados necessários para iniciar o atendimento remoto:
                        <div style="margin: 5px 0;  display: flex; justify-content: center; align-items: center;"
                        ">
                        <a target="_blank"
                           id="teleconsulta_link_medico"
                           href="">
                            Prosseguir para a sala
                        </a>
                    </div>
                    <div style="margin: 20px 0">
                        Compartilhe a sala com seu paciente:
                        <div style="margin: 20px 0; display: flex; justify-content: center;">
                            <input id="teleconsulta_link_paciente" style="width: 100%; border: none!important; text-align: center;
                             color: #337ab7;"
                                   value=""/>
                        </div>
                        <div style="margin: 10px 0; display: flex; justify-content: center; align-items: center;">

                            <label style="margin-right: 20px; cursor: pointer" onclick="copiarDadosTeleConsulta()"
                                   name="share_link"
                                   id="share_link" >
                                <i class="fa fa-link" style="font-size: 1.8em;  background: #80808069;
                                     padding: 18px 5px 10px 10px;  color: black;
                                      border-radius: 50px!important; width: 50px; height: 50px;">
                                </i>
                            </label>
                            <a target="_blank"
                               id="teleconsulta_link_whatsapp"
                               href="">
                                <img src="../imagens/icones/whatsapp.png"
                                     style="width: 50px; border-radius: 50px!important"/>
                            </a>
                            <a target="_blank"
                               id="teleconsulta_link_telegram"
                               href="">
                                <img src="../imagens/icones/telegram.png"
                                     style="width: 50px; border-radius: 50px!important; margin-left: 20px"/>
                            </a>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
    </div>


    <div class="modal fade in" id="modal-dados-agenda" tabindex="-1" role="modal-dados-agenda"
         aria-hidden="true">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Dados da agenda</h4>
                </div>
                <div class="modal-body">
                    <div class='resumo-agenda'>
                        <div><label>Data</label><br/><span class="data"></span></div>
                        <div><label>Hora</label><br/><span class="hora"></span></div>
                        <div><label>Procedimento</label><br/><span class="procedimento"></span></div>
                        <div><label>Tipo de serviço</label><br/><span class="tipo_servico"></span></div>
                        <div><label>Por </label><br/><span class="por"></span></div>
                    </div>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn" data-dismiss="modal">Fechar</button>
                </div>
            </div>
            <!-- /.modal-content -->
        </div>
        <!-- /.modal-dialog -->
    </div>

    <div class="modal fade in" id="modal-excluir" tabindex="-1" role="modal-excluir" aria-hidden="true">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Excluir registro do prontuário</h4>
                </div>
                <div class="modal-body">
                    Você tem certeza que deseja excluir o registro do prontuário?
                    <br/>
                    <br/>
                    <div class="descricao" style="text-align: justify; font-style: italic;">

                    </div>
                    <input type="hidden" name="prontuario_excluir"/>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn red" onclick="excluirRegistro();">Sim</button>
                    <button type="button" class="btn blue" data-dismiss="modal">Não</button>
                </div>
            </div>
            <!-- /.modal-content -->
        </div>
        <!-- /.modal-dialog -->
    </div>

    <script type="text/javascript" src="../includes/lightbox2/js/lightbox.js"></script>
    <script type="text/javascript">
        var selectizeTipoProntuario;
        var cronometro = {
            time: new Date(),
            inverv: null,
            init: function () {
                this.time.setHours(
                    <?= ($duracaoConsulta['hora'] ? $duracaoConsulta['hora'] : 0) ?>,
                    <?= ($duracaoConsulta['minuto'] ? $duracaoConsulta['minuto'] : 0) ?>,
                    <?= ($duracaoConsulta['segundo'] ? $duracaoConsulta['segundo'] : 0) ?>,
                    0);
                this.displayTime();

                var crn = this;
                this.interv = setInterval(function () {
                    crn.time.setSeconds(crn.time.getSeconds() + 1);

                    if (crn.time <= 0) {
                        crn.done();
                        clearInterval(crn.interv);
                    }

                    crn.displayTime();
                }, 1000);
            },
            displayTime: function () {
                $(".time").text(this.fillZeroes(this.time.getHours()) + ":" + this.fillZeroes(this.time.getMinutes()) + ":" + this.fillZeroes(this.time.getSeconds()));
            },
            fillZeroes: function (t) {
                t = t + "";
                if (t.length === 1)
                    return "0" + t;
                else
                    return t;
            }
        };

        $(function () {
            cronometro.init();

            $('.timeline .btn-action-timeline').hover(function () {
                $(this).addClass('btn-default');
            }, function () {
                $(this).removeClass('btn-default');
            });

            $('[name=tags]').selectize({
                plugins: ['remove_button'],
                delimiter: ',',
                persist: false,
                create: function (input) {
                    return {
                        value: input,
                        text: input
                    }
                },
                render: {
                    option_create: function (data, escape) {
                        return '<div class="create">Adicionar: <strong>' + escape(data.input) + '</strong>&hellip;</div>';
                    }
                },
                onItemAdd: function (value) {
                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        dataType: 'json',
                        data: {
                            act: 'incluirTagPaciente',
                            descricao: value,
                            p: '<?= setParam(array('pessoa_paciente_id' => $pessoaPacienteId), false) ?>',
                        },
                        success: function (retorno) {

                        },
                        error: function (result) {
                            exibirAlert('Não foi possível adicionar a tag ao paciente.');
                            console.error(result);
                        }
                    });
                },
                onItemRemove: function (value) {
                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        dataType: 'json',
                        data: {
                            act: 'excluirTagPaciente',
                            descricao: value,
                            p: '<?= setParam(array('pessoa_paciente_id' => $pessoaPacienteId), false) ?>',
                        },
                        success: function (retorno) {

                        },
                        error: function (result) {
                            exibirAlert('Não foi possível remover a tag do paciente.');
                            console.error(result);
                        }
                    });
                }
            });

            $('.form-pesquisar').validateForm({
                submitHandler: function (e) {
                    atualizarListaProntuario();
                }
            });

            var selectTipoProntuario = $('#tipo_prontuario').selectize({
                persist: false,
                maxItems: null,
                placeholder: 'Todas',
                plugins: ['remove_button'],
                valueField: 'id',
                labelField: 'descricao',
                searchField: ['descricao', 'descricao'],
                options: <?= json_encode(utf8ArrayEncode(array_values($estiloTipo))) ?>,
                items: <?= json_encode(utf8ArrayEncode(array_map(function ($tipo) {
                    return $tipo['id'];
                }, array_values($estiloTipo)))) ?>,
                render: {
                    item: function (item, escape) {
                        var name = escape(item.descricao);
                        return '<div class="bg-' + item.estilo.classe_cor + '" style="color: #FFFFFF;">' +
                            '<i class="' + item.estilo.classe_icone + '" style="width: 12px; height: 12px; margin: 0 2px 0 0;"></i>' +
                            (name ? '<span class="name"> ' + name + '</span>' : '') +
                            '</div>';
                    },
                    option: function (item, escape) {
                        var name = escape(item.descricao);

                        return '<div>' +
                            '<i class="font-' + item.estilo.classe_cor + ' ' + item.estilo.classe_icone + '" style="font-size: 20px;"></i>' +
                            '<span style="vertical-align: middle;"> ' + name + '</span>' +
                            '</div>';
                    }
                }
            });
            selectizeTipoProntuario = selectTipoProntuario[0].selectize;

            $('.btn-limpar-pesquisa').click(function () {
                if ($('.pesquisa-simples:visible').length == 0) {
                    $('.btn-pesquisa-simples').trigger('click');
                    selectizeTipoProntuario.clearOptions();
                    $('#descricao').val('');
                    $('#data_de').val('');
                    $('#data_ate').val('');
                } else {
                    $('#pesquisa_simples').val('');
                }

                $('.btn-pesquisar').trigger('click');
            });
        });

        function adicionarProntuario(tipo, p) {
            console.log(tipo);
            var content = $('#form-prontuario-content');
            content.html('<div style="height: 150px; width: 100%;"></div>');

            App.blockUI({
                message: 'Carregando...',
                target: '#form-prontuario-content'
            });

            content.load(window.location.href, {
                act: 'prontuarioFormulario',
                tipo: tipo,
                jitsi_video_iframe: typeof api_jitsi != 'undefined' && api_jitsi != undefined && api_jitsi != null ? 1 : '',
                p: p ? p : '<?= setParam(array(
                    'pessoa_paciente_id' => $pessoaPacienteId,
                    'agenda_id' => $p['agenda_id'],
                    'estabelecimento_id' => ($p['estabelecimento_id'] ? $p['estabelecimento_id'] : $dadosAgenda['estabelecimento_id']),
                ), false) ?>',
            }, function () {
                App.unblockUI('#form-prontuario-content');
            });

            $('#modal-prontuario').modal({
                backdrop: 'static'
            }).modal('show');
            if(typeof api_jitsi != 'undefined' && api_jitsi != undefined && api_jitsi != null)
            {
                $('.modal-backdrop.in').css('opacity', '0');
            }
        }

        function atualizarListaProntuario() {
            var content = $('#lista-prontuario .conteudo');
            var formPesquisa = $('.form-pesquisar');
            var isPesquisaSimples = $('.form-pesquisar .pesquisa-simples:visible').length > 0;

            content.unhighlight();

            App.blockUI({
                message: 'Carregando...',
                target: '#lista-prontuario .conteudo'
            });

            var params = {
                act: 'listarProntuario',
                p: '<?= setParam(array('pessoa_paciente_id' => $pessoaPacienteId, 'agenda_id' => $p['agenda_id']), false) ?>',
            };

            if (isPesquisaSimples) {
                params.pesquisa_simples = formPesquisa.find('[name=pesquisa_simples]').val();

                if (params.pesquisa_simples != '') {
                    $('.btn-limpar-pesquisa').parent().show();
                } else {
                    $('.btn-limpar-pesquisa').parent().hide();
                }
            } else {
                params.descricao = formPesquisa.find('[name=descricao]').val();
                params.tipo_prontuario = formPesquisa.find('[name=tipo_prontuario]').val();
                params.data_de = formPesquisa.find('[name=data_de]').val();
                params.data_ate = formPesquisa.find('[name=data_ate]').val();

                if (params.descricao != '' || params.tipo_prontuario != '' || params.data_de != '' || params.data_ate != '') {
                    $('.btn-limpar-pesquisa').parent().show();
                } else {
                    $('.btn-limpar-pesquisa').parent().hide();
                }
            }

            content.load(window.location.href, params, function () {
                App.unblockUI('#lista-prontuario .conteudo');

                var pesquisa = '';
                if (isPesquisaSimples) {
                    pesquisa = formPesquisa.find('[name=pesquisa_simples]').val();
                } else {
                    pesquisa = formPesquisa.find('[name=descricao]').val();
                }
                content.find('[class^=descricao-prontuario], .timeline-body-alerttitle, [class^=cids-prontuario]').highlight(pesquisa);
            });
        }

        function exibirModalfinalizarAtendimento() {
            $('#modal-finalizar-atendimento').modal({
                backdrop: 'static',
            }).modal('show');
        }

        function exibirModalTeleconsulta(url, token, whatsAppTemplate, telegramTemplate, nomeMedico, dataInicio, dataFim) {
            $.ajax({
                url: url,
                type: 'POST',
                data: {
                    meeting:{
                        emails: ['clubvida@mail.com'],
                        ends_at: dataFim,
                        starts_at: dataInicio
                    },
                    doctorName: nomeMedico
                },
                headers: {
                    Authorization: token
                },
                dataType: 'json',
                success: function (data) {
                    console.log(data);
                    const input = document.getElementById('teleconsulta_link_paciente');
                    input.addEventListener('keypress', updateValue);
                    input.addEventListener('keydown', updateValue);
                    function updateValue(e) {
                        e.preventDefault();
                        e.stopPropagation();
                    }

                    const linkWhatsApp = document.getElementById('teleconsulta_link_whatsapp');
                    const linkTelegram = document.getElementById('teleconsulta_link_telegram');
                    const linkMedico = document.getElementById('teleconsulta_link_medico');
                    const linkPaciente = document.getElementById('teleconsulta_link_paciente');
                    linkPaciente.value = data.urls.client;
                    linkMedico.href = data.urls.doctor;

                    let encodedUrl = encodeURI(data.urls.client);
                    linkWhatsApp.href = whatsAppTemplate.replace('mensagem', encodedUrl);
                    linkTelegram.href = telegramTemplate
                                        .replace('mensagem', encodeURI('Sua teleconsulta com o ' + nomeMedico + ' já está disponível'))
                                        .replace('link', encodedUrl)    ;
                    $('#modal-teleconsulta').modal({
                        backdrop: 'static',
                    }).modal('show');
                },
                error: function (data) {
                    exibirAlert('Não foi possível excluir o registro do prontuário');
                    App.unblockUI('#modal-excluir');
                },
            });

        }


        function copiarDadosTeleConsulta() {
            var input = document.getElementById('teleconsulta_link_paciente');
            input.select();
            var result = document.execCommand('copy');
            exibirSucesso('Link copiado para área de transferência!');
        }

        function fecharModalTeleconsulta() {
            $('#modal-finalizar-atendimento').modal({
                backdrop: 'static',
            }).modal('hide');
        }

        function finalizarAtendimento() {
            $('.form-finalizar-atendimento').submit();
        }

        function exibirDadosConsulta(data, inicio, fim, procedimento, tipoServico, nome) {
            var modal = $('#modal-dados-agenda');

            $(modal).find('.data').html(data);
            $(modal).find('.hora').html(inicio + ' - ' + fim);
            $(modal).find('.procedimento').html(procedimento);
            $(modal).find('.tipo_servico').html(tipoServico);
            $(modal).find('.por').html(nome);

            modal.modal({
                backdrop: 'static'
            }).modal('show');
        }

        function excluirProntuario(descricaoClass, p) {
            var modal = $('#modal-excluir');
            modal.find('.descricao').html($('.' + descricaoClass).html());
            modal.find('[name=prontuario_excluir]').val(p);
            modal.modal({
                backdrop: 'static'
            }).modal('show');
        }

        function excluirRegistro() {
            var modal = $('#modal-excluir');
            App.blockUI({
                message: 'Carregando...',
                target: '#modal-excluir'
            });

            $.ajax({
                url: window.location.href,
                type: 'POST',
                data: {
                    act: 'excluirRegistroProntuario',
                    p: modal.find('[name=prontuario_excluir]').val()
                },
                dataType: 'json',
                success: function (data) {
                    exibirSucesso(data.msg);
                    App.unblockUI('#modal-excluir');
                    modal.modal('hide');
                    atualizarListaProntuario();
                },
                error: function (data) {
                    exibirAlert('Não foi possível excluir o registro do prontuário');
                    App.unblockUI('#modal-excluir');
                },
            });
        }

        function controlaExibicaoProntuario(exibe, prontuario) {
            if (exibe == null) {
                $('.seta-prontuario[class$=prontuario-' + prontuario + ']:visible')[0].click();
                return true;
            }

            if (exibe == true) {
                $('.body-prontuario-' + prontuario + ' .timeline-body-content').show();
                $('.body-prontuario-' + prontuario).removeClass('timeline-short-body');
                $('.recolhe-prontuario-' + prontuario).show();
                $('.exibe-prontuario-' + prontuario).hide();
            } else {
                $('.body-prontuario-' + prontuario + ' .timeline-body-content').hide();
                $('.body-prontuario-' + prontuario).addClass('timeline-short-body');
                $('.recolhe-prontuario-' + prontuario).hide();
                $('.exibe-prontuario-' + prontuario).show();
            }
        }

        function imprimir(event, url) {
            event.preventDefault();
            event.stopPropagation();

            $.ajax({
                url: window.location.href + url,
                type: 'POST',
                data: {
                    act: 'imprimir'
                },
                dataType: 'json',
                error: function () {
                },
                success: function (res) {
                    assinatura.openGenericFile(res.arquivo);
                }
            });
        }





    </script>
<?php
function listarProntuario($pessoaPacienteId, $filtros = array())
{
    global $estiloTipo, $prepararDadosAgenda, $db;
    $p = getParam();
    $dadosAgenda = array();

    $podeAtender = false;
    $telaAtendimento = false;
    $permiteAlteracaoTempo = false;
    if ($p['agenda_id']) {
        extract($prepararDadosAgenda($p['agenda_id']));
    }

    $arWhere = array();

    if ($filtros['pesquisa_simples']) {
        $filtros['pesquisa_simples'] = utf8_decode($filtros['pesquisa_simples']);

        $arWhere[] = "(
            pron.descricao ILIKE '%{$filtros['pesquisa_simples']}%'
            OR
            tpo.descricao ILIKE '%{$filtros['pesquisa_simples']}%'
            OR
            vw_cid.cids ILIKE '%{$filtros['pesquisa_simples']}%'
        )";
    } else {
        if ($filtros['descricao']) {
            $filtros['descricao'] = utf8_decode($filtros['descricao']);
            $arWhere[] = "pron.descricao ILIKE '%{$filtros['descricao']}%'";
        }

        if ($filtros['tipo_prontuario']) {
            $arWhere[] = "tpo.id IN (" . implode(', ', $filtros['tipo_prontuario']) . ")";
        }

        if ($filtros['data_de']) {
            $filtros['data_de'] = formata_data_sql($filtros['data_de']);
            $arWhere[] = "pron.criado_em >= '{$filtros['data_de']} 00:00:00'";
        }

        if ($filtros['data_ate']) {
            $filtros['data_ate'] = formata_data_sql($filtros['data_ate']);
            $arWhere[] = "pron.criado_em <= '{$filtros['data_ate']} 23:59:00'";
        }
    }

    $mProntuario = new Prontuario();
    $mAnexo = new AnexoProntuario();
    $mProntuarioCid = new ProntuarioCid();

    $prontuarios = $mProntuario->listarPorPessoaId($pessoaPacienteId, null, null, $arWhere);
    $anexos = $mAnexo->listarAgrupadosProntuarioPorPessoa($pessoaPacienteId, array_map(function ($prontuario) {
        return $prontuario['id'];
    }, $prontuarios));

    $arrProntuarioPorTipoServico = array(
        0 => array(
            'nome' => 'Todos',
            'sigla' => 'todos',
            'arrServico' => $prontuarios
        )
    );

    $prontuariosComCid = [];
    foreach ($prontuarios as $prontuario) {
        if (!isset($arrProntuarioPorTipoServico[$prontuario['tipo_sigla']])) {
            $arrProntuarioPorTipoServico[$prontuario['tipo_sigla']] = array(
                'nome' => $prontuario['tipo_prontuario'],
                'sigla' => $prontuario['tipo_sigla'],
                'arrServico' => array()
            );
        }

        $arrProntuarioPorTipoServico[$prontuario['tipo_sigla']]['arrServico'][] = $prontuario;

        if (!empty($prontuario['cids'])) {
            $prontuariosComCid[] = $prontuario;
        }
    }

    if (count($prontuariosComCid) > 0) {
        $arrProntuarioPorTipoServico['DIA'] = array(
            'nome' => 'Diagnósticos',
            'sigla' => 'DIA',
            'arrServico' => $prontuariosComCid,
            'estilo' => array(
                'classe_cor' => 'purple',
                'classe_icone' => 'fa fa-stethoscope',
                'cor_hexadecimal' => '8E44AD'
            )
        );
    }

    if (count($prontuarios) > 0) : ?>
        <div class="nav-prontuario">
            <ul class="nav nav-tabs">
                <?php foreach ($arrProntuarioPorTipoServico as $siglaTipo => $prontuarioTipo): ?>
                    <li class="<?= $siglaTipo === 0 ? 'active' : '' ?> <?= $prontuarioTipo['sigla'] ?>">
                        <a href="#tab_<?= $prontuarioTipo['sigla'] ?>" data-toggle="tab"
                           aria-expanded="<?= $siglaTipo === 0 ? 'true' : 'false' ?>">
                            <i class="<?= $estiloTipo[$prontuarioTipo['sigla']]['estilo']['classe_icone'] ?>
                            font-<?= $estiloTipo[$prontuarioTipo['sigla']]['estilo']['classe_cor'] ?>"></i>
                            <?= $prontuarioTipo['nome']; ?>
                        </a>
                    </li>
                <?php endforeach; ?>
            </ul>
            <div class="tab-content">
                <?php foreach ($arrProntuarioPorTipoServico as $siglaTipo => $prontuarioTipo): ?>
                    <div class="tab-pane fade <?= $siglaTipo === 0 ? 'active in' : '' ?>"
                         id="tab_<?= $prontuarioTipo['sigla'] ?>">
                        <div style="height: 500px; padding-right: 10px; overflow: auto;">
                            <div class="timeline">
                                <?php foreach ($prontuarioTipo['arrServico'] as $prontuario) :
                                    $prontuario['tipo_servico'] = $prontuario['tipo_servico'] ? $prontuario['tipo_servico'] : '-';
                                    $titleAgenda = "<div class='resumo-agenda resumo-agenda-tooltip'>
                                            <div><label>Data</label><br />{$prontuario['data_agenda']}</div>
                                            <div><label>Hora</label><br />{$prontuario['inicio_agenda']} - {$prontuario['fim_agenda']}</div>
                                            <div><label>Procedimento</label><br />{$prontuario['procedimento']}</div>
                                            <div><label>Tipo de serviço</label><br />{$prontuario['tipo_servico']}</div>
                                            <div><label>Por </label><br />{$prontuario['nome_profissional']}</div>
                                        </div>";

                                    ?>
                                    <!-- TIMELINE ITEM -->
                                    <div class="timeline-item">
                                        <div class="timeline-badge">
                                            <div class="timeline-icon"
                                                 style="background-color: #<?= $estiloTipo[$prontuario['tipo_sigla']]['estilo']['cor_hexadecimal'] ?> !important">
                                                <div class="data-prontuario"
                                                     onclick="controlaExibicaoProntuario(null, '<?= $prontuario['id'] ?>')">
                                                    <span class="dia"><?= $prontuario['dia_agenda'] ?></span><br>
                                                    <span class="mes"><?= mes_abreviado($prontuario['mes_agenda']) ?></span>
                                                    <span><?= $prontuario['ano_agenda'] ?></span><br>
                                                    <span class="hora"><?= $prontuario['inicio_agenda'] ?></span>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-body body-prontuario-<?= $prontuario['id'] ?>"
                                             style="border-right: 5px solid #<?= $estiloTipo[$prontuario['tipo_sigla']]['estilo']['cor_hexadecimal'] ?> !important;">
                                            <div class="timeline-body-arrow"></div>
                                            <div class="timeline-body-head">
                                                <div class="timeline-body-head-caption"
                                                     onclick="controlaExibicaoProntuario(null, '<?= $prontuario['id'] ?>')">
                                                    <span class="timeline-body-alerttitle font-<?= $estiloTipo[$prontuario['tipo_sigla']]['estilo']['classe_cor'] ?>">
                                                        <span style="margin-right: 10px;">
                                                        <a class="seta-prontuario recolhe-prontuario-<?= $prontuario['id'] ?>"
                                                           title="Recolher prontuário" data-toggle="tooltip"
                                                           data-placement="left"
                                                           href="javascript: controlaExibicaoProntuario(false, '<?= $prontuario['id'] ?>')">
                                                            <i class="fa fa-chevron-right"></i>
                                                        </a>
                                                        <a style="display: none;"
                                                           class="seta-prontuario exibe-prontuario-<?= $prontuario['id'] ?>"
                                                           title="Exibir prontuário" data-toggle="tooltip"
                                                           data-placement="left"
                                                           href="javascript: controlaExibicaoProntuario(true, '<?= $prontuario['id'] ?>')">
                                                            <i class="fa fa-chevron-down"></i>
                                                        </a>
                                                    </span>
                                                        <i class="<?= $estiloTipo[$prontuario['tipo_sigla']]['estilo']['classe_icone'] ?> font-<?= $estiloTipo[$prontuario['tipo_sigla']]['estilo']['classe_cor'] ?>"></i>
                                                        <?= $prontuario['tipo_prontuario'] ?>
                                                    </span>
                                                    <span class="timeline-body-time font-grey-cascade">
                                                        por <?= $prontuario['nome_profissional'] ?>
                                                    </span>
                                                </div>
                                                <div class="timeline-body-head-actions">
                                                    <?php
                                                    $paramsProntuario = array(
                                                        'prontuario_id' => $prontuario['id'],
                                                        'pessoa_paciente_id' => $pessoaPacienteId,
                                                        'estabelecimento_id' => $p['estabelecimento_id']
                                                    );

                                                    if ($prontuario['agenda_id']) {
                                                        $paramsProntuario['agenda_id'] = $prontuario['agenda_id'];
                                                    }

                                                    $pProntuario = setParam($paramsProntuario, false);
                                                    ?>
                                                    <div style="float: right; margin-top: 0px;">
                                                        <?php if (($p['agenda_id'] && (($prontuario['agenda_id'] === $p['agenda_id'] && ($dadosAgenda['wf_estado_id'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_EM_ATENDIMENTO || $permiteAlteracaoTempo) && $telaAtendimento && $podeAtender) || $db->testa_superuser())) && $prontuario['arqid'] == null) : ?>
                                                            <a class="btn btn-circle btn-icon-only btn-action-timeline"
                                                               title="Editar prontuário" data-toggle="tooltip"
                                                               data-placement="left"
                                                               href="javascript: adicionarProntuario('', '<?= $pProntuario ?>')">
                                                                <i class="icon-pencil"></i>
                                                            </a>
                                                            <a class="btn btn-circle btn-icon-only btn-action-timeline"
                                                               title="Excluir prontuário" data-toggle="tooltip"
                                                               data-placement="left"
                                                               href="javascript: excluirProntuario('descricao-prontuario-<?= $prontuario['id'] ?>', '<?= $pProntuario ?>')">
                                                                <i class="icon-trash"></i>
                                                            </a>
                                                        <?php endif; ?>

                                                        <?php if ($prontuario['arqid'] != null) : ?>
                                                            <span class="btn btn-circle btn-icon-only btn-action-timeline"
                                                                  data-toggle="tooltip" data-placement="left"
                                                                  title="Registro Assinado Digitalmente">
                                                                <i class="fa fa-lock"></i>
                                                            </span>
                                                            <a class="btn btn-circle btn-icon-only btn-action-timeline"
                                                               title="Excluir Registro" data-toggle="tooltip"
                                                               data-placement="left"
                                                               href="javascript: excluirProntuario('descricao-prontuario-<?= $prontuario['id'] ?>', '<?= $pProntuario ?>')">
                                                                <i class="icon-trash"></i>
                                                            </a>
                                                        <?php endif; ?>
                                                        <a class="btn btn-circle btn-icon-only btn-action-timeline"
                                                           title="Imprimir" data-toggle="tooltip" data-placement="left"
                                                           href="#"
                                                           onclick="imprimir(event, '?modulo=principal/medico/atendimento&acao=C&act=imprimir<?= setParam(array('prontuario_id' => $prontuario['id'], 'agenda_id' => $p['agenda_id'], 'estabelecimento_id' => $p['estabelecimento_id'])) ?>')">
                                                            <i class="fa fa-print"></i>
                                                        </a>

                                                        <?php if ($prontuario['agenda_id']) : ?>
                                                            <a class="btn btn-circle btn-icon-only btn-action-timeline"
                                                               title="<?= $titleAgenda ?>" data-toggle="tooltip"
                                                               data-placement="left" data-html="true"
                                                               href="javascript: exibirDadosConsulta('<?= $prontuario['data_agenda'] ?>', '<?= $prontuario['inicio_agenda'] ?>','<?= $prontuario['fim_agenda'] ?>', '<?= $prontuario['procedimento'] ?>', '<?= $prontuario['tipo_servico'] ?>', '<?= $prontuario['nome_profissional'] ?>');">
                                                                <i class="fa fa-calendar"></i>
                                                            </a>
                                                        <?php endif; ?>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class="timeline-body-content">
                                                <span class="font-grey-cascade">
                                                    <div class="descricao-prontuario-<?= $prontuario['id'] ?>"
                                                         style="text-align: justify;">
                                                        <?= (isHtml($prontuario['descricao']) ? $prontuario['descricao'] : nl2br($prontuario['descricao'])) ?>
                                                    </div>
                                                    <? if (!empty($prontuario['cids'])): ?>
                                                        <br>
                                                        <div class="cids-prontuario-<?= $prontuario['id'] ?>">
                                                        <strong>CID: </strong><?= $prontuario['cids'] ?>
                                                    </div>
                                                    <? endif; ?>
                                                    <?php if (isset($anexos[$prontuario['id']]) && count($anexos[$prontuario['id']]) > 0) : ?>
                                                        <ul class="feeds" style="margin-top: 25px;">
                                                            <?php
                                                            $imagens = array();
                                                            foreach ($anexos[$prontuario['id']] as $anexo) :
                                                                if (isImagem($anexo['arqextensao'])):
                                                                    $imagens[] = $anexo;
                                                                    continue;
                                                                endif;
                                                                ?>
                                                                <li>
                                                                    <div class="col1"
                                                                         style="border-top-left-radius: 100px !important; border-bottom-left-radius: 100px !important;">
                                                                        <div class="cont">
                                                                            <div class="cont-col2">
                                                                                <a href="?modulo=principal/medico/atendimento&acao=C&act=downloadAnexo&arquivo_id=<?= $anexo['arquivo_id'] ?>"
                                                                                   title="Download">
                                                                                    <button class="btn btn-circle btn-icon-only btn-default blue">
                                                                                        <i class="icon-cloud-download"></i>
                                                                                    </button>
                                                                                    <?= $anexo['arqnome'] ?>
                                                                                    .<?= $anexo['arqextensao'] ?>
                                                                                </a>
                                                                            </div>
                                                                        </div>
                                                                    </div>
                                                                    <div class="col2"
                                                                         style="width: 100px; margin-left: -100px;">
                                                                        <div class="date">
                                                                            <?= formatarTamanhoArquivo($anexo['arqtamanho']) ?>
                                                                        </div>
                                                                    </div>
                                                                </li>
                                                            <?php
                                                            endforeach;

                                                            foreach ($imagens as $imagem) : ?>
                                                                <div class="miniatura-imagem">
                                                                    <div class="miniatura-imagem-container">
                                                                        <a href="../slideshow/slideshow/verimagem.php?arqid=<?= $imagem['arquivo_id'] ?>"
                                                                           data-lightbox="prontuario_<?= $prontuario['id'] ?>"
                                                                           data-title="<?= $imagem['arqnome'] ?>.<?= $imagem['arqextensao'] ?> (<?= formatarTamanhoArquivo($imagem['arqtamanho']) ?>)">
                                                                            <img src="../slideshow/slideshow/verimagem.php?arqid=<?= $imagem['arquivo_id'] ?>&newwidth=300"
                                                                                 alt=""/>
                                                                        </a>
                                                                    </div>
                                                                </div>
                                                            <?php endforeach; ?>
                                                        </ul>
                                                    <?php endif; ?>
                                                </span>

                                            </div>
                                        </div>
                                    </div>
                                    <!-- END TIMELINE ITEM -->
                                <?php endforeach; ?>
                            </div>
                        </div>
                    </div>
                <?php endforeach; ?>
            </div>
        </div>

        <script type="text/javascript">
            $(function () {
                $('#lista-prontuario [data-toggle="tooltip"]').tooltip();
            });
        </script>
    <?php else: ?>
        <div style="text-align: center;">
            <h4>Nenhum registro foi incluído no prontuário do paciente.</h4>
        </div>
    <?php endif;
}
?>
<div style="width: 100%; text-align:center">
    <button class="btn btn-secondary" type="button" style="font-size: 16px;" onclick="window.location.href=document.referrer" >
        <i class="fa fa-arrow-left"></i>
        Voltar
    </button>
</div>

<?php if($podeAtender && ($dadosAgenda['tipo_consulta'] == 'Consulta remota' || $dadosAgenda['tipo_consulta'] == 'Retorno remoto')) : ?>
    <?php 
    $url_consulta = $mAgenda->gerarUrlAtendimentoRemoto($p['agenda_id']);
    $room_id = substr($url_consulta,-6);
    $sala_virtual = new Jitsi($room_id , true);
    $password_jitsi = rand(1000,9999);
    ?>
    <script src='https://8x8.vc/external_api.js' async></script>
    <script type="text/javascript">
    let api_jitsi = null;

    const initJitsiIframeAPI = () => {

        $(".div_jitsi, #btn_jitsi_finalizar").show();
        $("#div_foto_paciente, #btn_jitsi_iniciar").hide();

        const domain = '<?php echo $sala_virtual->getDomain() ?>';
        const options = {
            roomName: '<?php echo $sala_virtual->getAppID() . '/' . $sala_virtual->getRoom() ?>',
            jwt: '<?php echo $sala_virtual->getToken() ?>',
            lang: 'ptBR',
            userInfo: {
                displayName: '<?= $dadosAgenda['profissional'] ?>'
            },
            enableWelcomePage: false,
            parentNode: document.querySelector('#jitsi_room_consulta'),        
            configOverwrite: { toolbarButtons: ['microphone','camera','fullscreen','hangup','settings','chat'], enableWelcomePage: false, },
            interfaceConfigOverwrite: { LANG_DETECTION: false, enableWelcomePage: false, },
        };

        if(api_jitsi == null)
        {
            api_jitsi = new JitsiMeetExternalAPI(domain, options);
        }
        $("#hidden_jitsi_link").val('<?php echo $url_consulta ?>');

        api_jitsi.addListener("videoConferenceLeft", () => {
            finishJitsiIframeAPI();
        });
        
        $("#div_class_modal_prontuario").attr('style','width: 72%; right:-14%; top:11%');
    }   

    const finishJitsiIframeAPI = () => {
        $(".div_jitsi, #btn_jitsi_finalizar").hide();
        $("#div_foto_paciente, #btn_jitsi_iniciar").show();
        $("#jitsi_room_consulta").html('');
        $("#hidden_jitsi_link").val('');
        api_jitsi.executeCommand('hangup');
        api_jitsi = null;
        $("#div_class_modal_prontuario").attr('style','width: 90%');
    }

    /*window.onload = () => {
        initJitsiIframeAPI();
    }*/

    function copiarLinkJitsi()
    {
        var input = document.getElementById('hidden_jitsi_link');
        input.select();
        var result = document.execCommand('copy');
        exibirSucesso('Link copiado para área de transferência!');
    }

    var enviarLinkJitsiPorEmail_loading = false;

    function enviarLinkJitsiPorEmail(email)
    {
        if(enviarLinkJitsiPorEmail_loading === true)
        {
            exibirAlert('Por favor Aguarde...');
            return false;
        }
        const mensagem = 'Prezado(a) <?=explode(' ',$dadosPaciente['nome_completo'])[0]?>,<br> Utilize o link a seguir para acessar a teleconsulta: <br> ' + $('#hidden_jitsi_link').val();
        enviarLinkJitsiPorEmail_loading = true;
        $.ajax({
            type: "POST",
            url: window.location.href,
            dataType: 'json',
            data: {
                act: 'enviarLinkJitsiPorEmail',
                mensagem: mensagem,
                destinatario: email,
                p: '<?= setParam(array('pessoa_paciente_id' => $pessoaPacienteId), false) ?>',
            },
            success: function (retorno) {
                enviarLinkJitsiPorEmail_loading = false;
                if(retorno.responseText == 'ok'){
                    exibirSucesso('Link enviador por e-mail para ' + email);
                }else{
                    exibirAlert('Não foi possível enviar o link por SMS.');
                }
            },
            error: function (result) {
                enviarLinkJitsiPorEmail_loading = false;
                if(result.responseText == 'ok'){
                    exibirSucesso('Link enviador por e-mail para ' + email);
                }else{
                    exibirAlert('Não foi possível enviar o link por SMS.');
                    console.error(result);
                }

            }
        });
        
    }

    var enviarLinkJitsiPorSMS_loading = false;

    function enviarLinkJitsiPorSMS(celular)
    {
        if(enviarLinkJitsiPorSMS_loading === true)
        {
            exibirAlert('Por favor Aguarde...');
            return false;
        }
        let mensagem = 'Ola <?=explode(' ',$dadosPaciente['nome_completo'])[0]?>, acesse a teleconsulta em: ' + $('#hidden_jitsi_link').val();
        // SMS com no máximo 160 caracteres
        if(mensagem.length > 160)
        {
            mensagem = 'Acesse sua teleconsulta em: ' + $('#hidden_jitsi_link').val();
        }
        if(mensagem.length > 160)
        {
            mensagem = 'Teleconsulta: ' + $('#hidden_jitsi_link').val();
        }
        enviarLinkJitsiPorSMS_loading = true;
        $.ajax({
            type: "POST",
            url: window.location.href,
            dataType: 'json',
            data: {
                act: 'enviarLinkJitsiPorSMS',
                mensagem: mensagem,
                destinatario: celular,
                p: '<?= setParam(array('pessoa_paciente_id' => $pessoaPacienteId), false) ?>',
            },
            success: function (retorno) {
                enviarLinkJitsiPorSMS_loading = false;
                if(retorno.responseText == 'ok'){
                    exibirSucesso('Link enviador por SMS para ' + celular);
                }else{
                    exibirAlert('Não foi possível enviar o link por SMS.');
                }
            },
            error: function (result) {
                enviarLinkJitsiPorSMS_loading = false;
                if(result.responseText == 'ok'){
                    exibirSucesso('Link enviador por SMS para ' + celular);
                }else{
                    exibirAlert('Não foi possível enviar o link por SMS.');
                    console.error(result);
                }
            }
        });
        
    }

    function enviarLinkJitsiPorWhastApp(celular)
    {
        const mensagem = 'Prezado(a) <?=explode(' ',$dadosPaciente['nome_completo'])[0]?>, utilize o link a seguir para acessar a teleconsulta: ' + $('#hidden_jitsi_link').val();
        let link = 'https://api.whatsapp.com/send/?phone=+55' + celular.replace(/\D/g, "") + '&text=' + mensagem + '&app_absent=0';
        window.open(link, '_blank');
    }

    </script>
<?php endif; ?>