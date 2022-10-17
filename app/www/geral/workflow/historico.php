<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

include APPRAIZ . 'includes/workflow.php';

$docid = (integer) $_REQUEST['docid'];
$documento = wf_pegarDocumento( $docid );
$atual = wf_pegarEstadoAtual( $docid );
$historico = wf_pegarHistorico( $docid, $_REQUEST['inicial'] );

?>
<html>
	<head>
		<title><?= $GLOBALS['parametros_sistema_tela']['sigla-nome_completo']; ?></title>
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

		<!-- biblioteca javascript local -->
		<script type="text/javascript">
			
			IE = !!document.all;
			
			function exebirOcultarComentario( docid )
			{
				id = 'comentario' + docid;
				div = document.getElementById( id );
				if ( !div )
				{
					return;
				}
				var display = div.style.display != 'none' ? 'none' : 'table-row';
				if ( display == 'table-row' && IE == true )
				{
					display = 'block';
				}
				div.style.display = display;
			}
		</script>
	</head>
	<body style="background-color: #F1F4F9;">
        <div style="padding: 10px;" class="text-center">
            <strong>Histórico de Tramitações<br/></strong>
            <div><?php echo utf8ArrayDecode($documento['docdsc']); ?></div>
        </div>
		<form action="" method="post" name="formulario">
			<table class="table table-striped table-hover">
				<thead>
					<?php if ( count( $historico ) ) : ?>
						<tr>
							<td style="width: 10%;"><b>Seq.</b></td>
							<td style="width: 25%;"><b>Onde Estava</b></td>
							<td style="width: 25%;"><b>O que aconteceu</b></td>
							<td style="width: 15%;"><b>Quem fez</b></td>
							<td style="width: 10%;"><b>Quando fez</b></td>
							<td style="width: 15%;">&nbsp;</td>
						</tr>
					<?php endif; ?>
				</thead>
				<?php $i = 1; ?>
				<?php foreach ( $historico as $item ) : ?>
					<?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
					<tr>
						<td align="right"><?=$i?>.</td>
						<td>
							<?php echo $item['esddsc']; ?>
						</td>
						<td valign="middle">
							<?php echo $item['aeddscrealizada']; ?>
						</td>
						<td>
							<?php echo $item['usunome']; ?>
						</td>
						<td>
							<?php echo $item['htddata']; ?>
						</td>
						<td>
							<?php if( $item['cmddsc'] ) : ?>
                                <div class="text-center" style="padding-top: 10px;">
                                    <i class="fa fa-warning" onclick="exebirOcultarComentario( '<?php echo $i; ?>' );"
                                       style="cursor: pointer; font-size: 22px;" title="Ver comentário"></i>
                                </div>
							<?php endif; ?>
						</td>
					</tr>
					<tr id="comentario<?php echo $i; ?>" style="display: none;" bgcolor="<?=$marcado?>">
						<td colspan="6">
							<div >
								<?php echo htmlentities( $item['cmddsc'] ); ?>
							</div>
						</td>
					</tr>
					<?php $i++; ?>
				<?php endforeach; ?>
				<?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
				<tr bgcolor="<?=$marcado?>">
					<td class="text-right" colspan="6">
						Estado atual: <strong><?= $atual['esddsc']; ?></strong>
					</td>
				</tr>
			</table>
			<div class="text-center">
				<input class="botao btn red" type="button" value="Fechar" onclick="window.close();">
			</div>
		</form>
	</body>
</html>