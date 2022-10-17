<?php

// inicializa sistema
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/workflow.php";
if ( !$db )
{
	$db = new cls_banco();
}

$docid = (integer) $_REQUEST['docid'];
$esdid = (integer) $_REQUEST['esdid'];
$aedid = (integer) $_REQUEST['aedid'];
$verificacao = stripcslashes( (string) $_REQUEST['verificacao'] );

$documento = wf_pegarDocumento( $docid );
$acao = wf_pegarAcao2( $aedid );

if ( !$documento || !$acao )
{
	?>
	<script type="text/javascript">
		alert( 'Documento ou ação inválida.' );
		window.close();
	</script>
	<?php
	exit();
}

?>
<html>
	<head>
		<title><?= $titulo ?><?= $maximo != 0 ? ' - Ecolha no máximo ' . $maximo . ' itens' : '' ; ?></title>
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta content="width=device-width, initial-scale=1" name="viewport"/>
        <meta content="" name="description"/>
        <meta content="" name="author"/>
        <!-- BEGIN GLOBAL MANDATORY STYLES -->
        <link href="http://fonts.googleapis.com/css?family=Open+Sans:400,300,600,700&subset=all" rel="stylesheet"
              type="text/css"/>
        <link href="/includes/metronic/global/plugins/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css"/>
        <link href="/includes/metronic/global/plugins/simple-line-icons/simple-line-icons.min.css" rel="stylesheet" type="text/css"/>
        <link href="/includes/metronic/global/plugins/bootstrap/css/bootstrap.min.css" rel="stylesheet" type="text/css"/>
        <link href="/includes/metronic/global/plugins/bootstrap-switch/css/bootstrap-switch.min.css" rel="stylesheet"
              type="text/css"/>
        <!-- END GLOBAL MANDATORY STYLES -->
        <!-- BEGIN PAGE LEVEL PLUGINS -->
        <link href="/includes/metronic/global/plugins/bootstrap-daterangepicker/daterangepicker.min.css" rel="stylesheet"
              type="text/css"/>
        <link href="/includes/metronic/global/plugins/morris/morris.css" rel="stylesheet" type="text/css"/>
        <link href="/includes/metronic/global/plugins/fullcalendar/fullcalendar.min.css" rel="stylesheet" type="text/css"/>
        <link href="/includes/metronic/global/plugins/jqvmap/jqvmap/jqvmap.css" rel="stylesheet" type="text/css"/>
        <link href="/includes/metronic/global/plugins/bootstrap-fileinput/bootstrap-fileinput.css" rel="stylesheet" type="text/css" />
        <link href="/includes/metronic/global/plugins/bootstrap-table/bootstrap-table.min.css" rel="stylesheet" type="text/css" />
        <link href="/includes/metronic/global/plugins/bootstrap-select/css/bootstrap-select.min.css" rel="stylesheet" type="text/css" />
        <link href="/includes/metronic/global/plugins/bootstrap-datepicker/css/bootstrap-datepicker3.min.css" rel="stylesheet" type="text/css" />
        <link href="/includes/bootstrap-datetimepicker/css/bootstrap-datetimepicker.css" rel="stylesheet" type="text/css" />
        <link href="/includes/metronic/global/plugins/bootstrap-timepicker/css/bootstrap-timepicker.min.css" rel="stylesheet" type="text/css" />
        <link href="/includes/metronic/global/plugins/dropzone/dropzone.min.css" rel="stylesheet" type="text/css" />
        <link href="/includes/metronic/global/plugins/jquery-ui/jquery-ui.min.css" rel="stylesheet" type="text/css" />

        <link href="/includes/estilom.css" rel="stylesheet" type="text/css" />

        <!-- END PAGE LEVEL PLUGINS -->
        <!-- BEGIN THEME GLOBAL STYLES -->
        <link href="/includes/metronic/global/css/components.min.css" rel="stylesheet" id="style_components" type="text/css"/>
        <link href="/includes/metronic/global/css/plugins.min.css" rel="stylesheet" type="text/css"/>
        <!-- END THEME GLOBAL STYLES -->
        <!-- BEGIN THEME LAYOUT STYLES -->
        <link href="/includes/metronic/layouts/layout/css/layout.min.css" rel="stylesheet" type="text/css"/>
        <link href="/includes/metronic/layouts/layout/css/themes/lightblue.css" rel="stylesheet" type="text/css"
              id="style_color"/>
        <!-- END THEME LAYOUT STYLES -->
        <link rel="shortcut icon" href="favicon.ico"/>
    </head>
	<body>
        <div class="portlet light bordered">
            <div class="portlet-title">
                <div class="caption font-blue-sharp">
                    
                </div>
            </div>
            <div class="portlet-body form">
                <form role="form" method="post" action="https://<?= $_SERVER['SERVER_NAME'] ?>/geral/workflow/alterar_estado.php" name="combo_alterar_estado_comentario">
                    <input type="hidden" name="docid" value="<?= $docid ?>" />
                    <input type="hidden" name="esdid" value="<?= $esdid ?>" />
                    <input type="hidden" name="aedid" value="<?= $aedid ?>" />
                    <input type="hidden" name="verificacao" value="<?php echo htmlentities( $verificacao ) ?>"/>

                    <div class="form-body">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="row">
                                        <div class="col-xs-3">
                                            <label>Estado atual:</label>
                                        </div>
                                        <div class="col-xs-9">
                                            <strong><?= $acao['esddscorigem'] ?></strong>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="row">
                                        <div class="col-xs-3">
                                            <label>Ação:</label>
                                        </div>
                                        <div class="col-xs-9">
                                            <strong><?= $acao['aeddscrealizar'] ?></strong>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label>Comentário</label>
                                    <?php
                                    echo campo_textaream(
                                        "cmddsc",		// nome do campo
                                        "N",			// obrigatorio
                                        "S",			// habilitado
                                        "Comentário",	// label
                                        61,				// colunas
                                        10,				// linhas
                                        6000			// quantidade maximo de caracteres
                                    )
                                    ?>
                                </div>
                            </div>
                        </div>

                    </div>

                    <div class="form-actions">
                        <button type="submit" name="alterar_estado" class="btn blue">
                            <i class="fa fa-save"></i> Salvar
                        </button>
                    </div>
                </form>
            </div>
        </div>

	</body>
</html>