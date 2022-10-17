<?php

include_once "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/SimecHashids.class.inc";

$db = new cls_banco();
$_SESSION['superuser'] = true;

function __autoload($class_name)
{
    $arCaminho = array(
        APPRAIZ . "adm/classes/",
        APPRAIZ . "includes/classes/modelo/public/",
        APPRAIZ . "includes/classes/modelo/app/",
        APPRAIZ . "includes/classes/modelo/territorios/",
        APPRAIZ . "includes/classes/modelo/entidade/",
        APPRAIZ . "includes/classes/modelo/corporativo/",
        APPRAIZ . "includes/classes/modelo/seguranca/",
        APPRAIZ . "includes/classes/modelo/mensageria/",
        APPRAIZ . "includes/classes/modelo/financeiro/",
        APPRAIZ . "includes/classes/",
    );

    foreach ($arCaminho as $caminho) {
        $arquivo = $caminho . $class_name . '.class.inc';
        if (file_exists($arquivo)) {
            require_once($arquivo);
            break;
        }
    }
}

include_once 'adm/_funcoes.php';
include_once APPRAIZ . 'includes/workflow.php';

$hashids = new SimecHashids();
$agenda_id = $hashids->decode($_GET['p']);

$mAgenda = new EstabelecimentoProfissionalAgenda();
$mEstabelecimento = new Estabelecimento();

$dadosAgenda = [];
$mensagemValidacao = "";
$tipoValidacao = "success";
if (!$agenda_id) {
    $mensagemValidacao = '<strong>Atenção</strong> Nenhum atendimento informado!';
    $tipoValidacao = "warning";
} else {
    $dadosAgenda = $mAgenda->getTodosDados($agenda_id);
    $dadosEstabelecimento = $mEstabelecimento->getTodosDados($dadosAgenda['estabelecimento_id']);
    $dadosAgenda['hora_inicio'] = substr($dadosAgenda['hora_inicio'], 0, 5);
    $dadosAgenda['hora_fim'] = substr($dadosAgenda['hora_fim'], 0, 5);

    if (!$dadosAgenda) {
        $mensagemValidacao = '<strong>Atenção</strong> Nenhum atendimento encontrado!';
        $tipoValidacao = "warning";
    } else if ($dadosAgenda['wf_estado_id'] != EstabelecimentoProfissionalAgenda::WF_ESTADO_AGENDADA) {
        $mensagemValidacao = 'Formulário já respondido.';
        $tipoValidacao = "success";
    }
}


if (empty($mensagemValidacao) && $_POST && $_POST['act'] == 'salvar') {
    $confirmacaoConsulta = $_POST['confirmacao_consulta'] === 'true' ? true : false;
    $acao = EstabelecimentoProfissionalAgenda::WF_ACAO_CONFIRMAR;
    $msgOperacao = "Obrigado pela confirmação!";

    $comentario = "";

    if (!$confirmacaoConsulta) {
        $acao = EstabelecimentoProfissionalAgenda::WF_ACAO_CANCELAR;
        $comentario = 'Cencelado pelo paciente';
        $msgOperacao = "Consulta cancelada com sucesso!";
    }

    wf_alterarEstado(
        $dadosAgenda['documento_id'],
        $acao,
        $comentario,
        array('id' => $dadosAgenda['estabelecimento_profissional_agenda_id'])
    );
    $mAgenda->commit();

    $mensagemValidacao = "<strong>{$msgOperacao}</strong>";
    $tipoValidacao = "info";
}
?>
<html>
<head>
    <meta charset="ISO-8859-1"/>
    <title>ClubVida</title>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1"/>
    <meta content="width=device-width, initial-scale=1" name="viewport"/>
    <meta content="" name="description"/>
    <meta content="" name="author"/>
    <!-- BEGIN GLOBAL MANDATORY STYLES -->
    <link href="http://fonts.googleapis.com/css?family=Open+Sans:400,300,600,700&subset=all" rel="stylesheet"
          type="text/css"/>
    <link href="/includes/metronic/global/plugins/font-awesome/css/font-awesome.min.css" rel="stylesheet"
          type="text/css"/>
    <link href="/includes/metronic/global/plugins/simple-line-icons/simple-line-icons.min.css" rel="stylesheet"
          type="text/css"/>
    <link href="/includes/metronic/global/plugins/bootstrap/css/bootstrap.min.css" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic/global/plugins/bootstrap-switch/css/bootstrap-switch.min.css" rel="stylesheet"
          type="text/css"/>
    <!-- END GLOBAL MANDATORY STYLES -->
    <!-- BEGIN PAGE LEVEL PLUGINS -->
    <link href="/includes/metronic/global/plugins/select2/css/select2.min.css" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic/global/plugins/select2/css/select2-bootstrap.min.css" rel="stylesheet"
          type="text/css"/>
    <!-- END PAGE LEVEL PLUGINS -->
    <!-- BEGIN THEME GLOBAL STYLES -->
    <link href="/includes/metronic/global/css/components.min.css" rel="stylesheet" id="style_components"
          type="text/css"/>
    <link href="/includes/metronic/global/css/plugins.min.css" rel="stylesheet" type="text/css"/>
    <!-- END THEME GLOBAL STYLES -->
    <!-- BEGIN PAGE LEVEL STYLES -->
    <link href="/includes/metronic/pages/css/login.min.css" rel="stylesheet" type="text/css"/>
    <!-- END PAGE LEVEL STYLES -->
    <!-- BEGIN THEME LAYOUT STYLES -->
    <!-- END THEME LAYOUT STYLES -->
    <link rel="shortcut icon" href="/favicon.ico?v=<?= VERSAO ?>"/>
    <style type="text/css">

        fieldset, label {
            margin: 0;
            padding: 0;
        }

        body {
            margin: 20px;
            background-color: #f1f4f7;
        }

        /****** Style Star Rating Widget *****/

        .rating {
            border: none;
            float: left;
        }

        .rating > input {
            display: none;
        }

        .rating > label:before {
            margin: 5px;
            font-size: 2.00em;
            font-family: FontAwesome;
            display: inline-block;
            content: "\f005";
        }

        .rating > .half:before {
            content: "\f089";
            position: absolute;
        }

        .rating > label {
            color: #ddd;
            float: right;
        }

        /***** CSS Magic to Highlight Stars on Hover *****/

        .rating > input:checked ~ label, /* show gold star when clicked */
        .rating:not(:checked) > label:hover, /* hover current star */
        .rating:not(:checked) > label:hover ~ label {
            color: #FFD700;
        }

        /* hover previous stars in list */

        .rating > input:checked + label:hover, /* hover current star when changing rating */
        .rating > input:checked ~ label:hover,
        .rating > label:hover ~ input:checked ~ label, /* lighten current selection */
        .rating > input:checked ~ label:hover ~ label {
            color: #FFED85;
        }

        .content {
            padding: 20px;
        }

        .content .stars {
            width: 180px;
            margin: auto;
        }

        .content .subtitulo {
            color: #666;
            font-size: 14px;
        }

        .btn-group .btn {
            margin: 5px !important;
        }

        .form-actions {
            margin-top: 15px !important;
        }

        .qtd-caracteres-comentario {
            color: #666 !important;
            font-style: italic;
        }

        .descricao-estrela {
            font-weight: bolder;
            font-style: italic;
        }

        .avaliacao-melhorias, .avaliacao-comentario {
            display: none;
        }
    </style>
</head>
<body>

<div class="content">
    <div class="row">
        <div class="col-md-4 col-md-offset-4" style="text-align: center; padding: 15px 0 35px 0;">
            <img src="/imagens/logo_assinatura_email.png?v=<?= VERSAO ?>" style="height: 65px;" alt=""/>
        </div>
    </div>
    <div class="row">
        <div class="col-sm-6 col-sm-offset-3">
            <? if (empty($mensagemValidacao)): ?>
                <div class="alert alert-warning">
                    <strong>Atenção</strong> Verifique os dados antes de confirmar o atendimento
                </div>
                <div class="portlet light bordered">
                    <div class="portlet-body form">
                        <div>
                            <div class="col-md-12">
                                <div class="col-xs-12" style="text-align: center;">
                                    <h3 class="font-blue" style="font-weight: bold;">Confirmação de Atendimento</h3>
                                </div>
                            </div>
                        </div>

                        <form id="form-confirmacao" name="form-confirmacao" method="post">
                            <div>
                                <div class="row text-center">
                                    <h4>Dados do atendimento em <strong><?= $dadosEstabelecimento['nome_fantasia'] ?></strong></h4>
                                </div>

                                <div class="row">
                                    <div class="col-sm-6">
                                        <div class="form-group">
                                            <label class="control-label">Nome completo do paciente</label><br>
                                            <strong><?= $dadosAgenda['paciente'] ?></strong>
                                        </div>

                                        <div class="form-group">
                                            <label class="control-label">Nome do profissional </label><br>
                                            <strong><?= $dadosAgenda['profissional'] ?></strong>
                                        </div>

                                        <div class="form-group">
                                            <label class="control-label">Tipo de atendimento </label><br>
                                            <strong><?= $dadosAgenda['tipo_consulta'] ?></strong>
                                        </div>

                                        <div class="form-group">
                                            <label class="control-label">Convênio </label><br>
                                            <strong><?= $dadosAgenda['convenio'] ?></strong>
                                        </div>
                                    </div>

                                    <div class="col-sm-6">
                                        <div class="form-group">
                                            <label class="control-label">Data</label><br>
                                            <strong><?= formata_data($dadosAgenda['data_inicio']) ?></strong>
                                        </div>

                                        <div class="form-group">
                                            <label class="control-label">Horário </label><br>
                                            De <strong><?= $dadosAgenda['hora_inicio'] ?></strong> Até
                                            <strong><?= $dadosAgenda['hora_fim'] ?></strong>
                                        </div>

                                    </div>
                                </div>


                                <div class="row text-center">
                                    <input type="hidden" name="act" value="salvar">
                                    <input type="hidden" name="confirmacao_consulta">
                                    <div class="col-sm-6" style="padding: 5px;">
                                        <input type="button" class="btn green-jungle" value="Confirmar Consulta"
                                               onclick="confirmarConsulta();">
                                    </div>
                                    <div class="col-sm-6" style="padding: 5px;">
                                        <input type="button" class="btn red-intense" value="Cancelar Consulta"
                                               onclick="cancelarConsulta();">
                                    </div>
                                </div>
                            </div>
                        </form>
                    </div>
                </div>
            <? else: ?>
                <div class="alert alert-<?= $tipoValidacao; ?>">
                    <?= $mensagemValidacao ?>
                </div>
            <? endif; ?>
        </div>
    </div>
</div>
<!--[if lt IE 9]>
<script src="/includes/metronic/global/plugins/respond.min.js"></script>
<script src="/includes/metronic/global/plugins/excanvas.min.js"></script>
<![endif]-->
<!-- BEGIN CORE PLUGINS -->
<script src="/includes/metronic/global/plugins/jquery.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/bootstrap/js/bootstrap.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/js.cookie.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/bootstrap-hover-dropdown/bootstrap-hover-dropdown.min.js"
        type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/jquery-slimscroll/jquery.slimscroll.min.js"
        type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/jquery.blockui.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/bootstrap-switch/js/bootstrap-switch.min.js"
        type="text/javascript"></script>
<!-- END CORE PLUGINS -->
<!-- BEGIN PAGE LEVEL PLUGINS -->
<script src="/includes/metronic/global/plugins/jquery-validation/js/jquery.validate.min.js"
        type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/jquery-validation/js/additional-methods.min.js"
        type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/select2/js/select2.full.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/jquery-inputmask/jquery.inputmask.bundle.min.js"
        type="text/javascript"></script>
<script src="/includes/funcoes.js" type="text/javascript"></script>
<!-- END PAGE LEVEL PLUGINS -->
<!-- BEGIN PAGE LEVEL SCRIPTS -->
<script type="text/javascript">
    function confirmarConsulta() {
        $('[name=confirmacao_consulta]').val('true');
        $('form[name=form-confirmacao]').submit();
    }

    function cancelarConsulta() {
        $('[name=confirmacao_consulta]').val('false');
        $('form[name=form-confirmacao]').submit();
    }
</script>
<!-- END PAGE LEVEL SCRIPTS -->
<!-- BEGIN THEME LAYOUT SCRIPTS -->
<!-- END THEME LAYOUT SCRIPTS -->
</body>
</html>