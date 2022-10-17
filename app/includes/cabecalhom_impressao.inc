<!DOCTYPE html>
<html lang="pt">
<!--<![endif]-->
<!-- BEGIN HEAD -->

<head>
    <meta charset="utf-8"/>
    <title><?php echo $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'] ?></title>
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
    <link href="/includes/estilom.css" rel="stylesheet" type="text/css" />


    <!-- END PAGE LEVEL PLUGINS -->
    <!-- BEGIN THEME GLOBAL STYLES -->
    <link href="/includes/metronic/global/css/components.min.css" rel="stylesheet" id="style_components" type="text/css"/>
    <link href="/includes/metronic/global/css/plugins.min.css" rel="stylesheet" type="text/css"/>
    <!-- END THEME GLOBAL STYLES -->
    <!-- BEGIN THEME LAYOUT STYLES -->
    <link href="/includes/metronic/layouts/layout/css/layout.min.css" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic/layouts/layout/css/custom.min.css" rel="stylesheet" type="text/css"/>
    <!-- END THEME LAYOUT STYLES -->
    <link rel="shortcut icon" href="favicon.ico"/>


    <!-- BEGIN CORE PLUGINS -->
    <script src="/includes/metronic/global/plugins/jquery.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap/js/bootstrap.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-table/bootstrap-table.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-table/locale/bootstrap-table-pt-BR.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-datepicker/js/bootstrap-datepicker.min.js" type="text/javascript"></script>

    <script src="/includes/metronic/global/plugins/moment.min.js" type="text/javascript"></script>

    <script src="/includes/metronic/global/scripts/app.min.js" type="text/javascript"></script>
    <script src="/includes/funcoesm.js" type="text/javascript"></script>
    <!-- END THEME LAYOUT SCRIPTS -->


    <style type="text/css">
        table.cabecalho{
            width: 100%;
        }

        table.cabecalho tr > td{
            padding: 5px;
        }

        table.corpo tr > td{
            vertical-align: text-top;
        }

        div.imagem-container{
            width: 150px;
            height: 100px;
            margin-right: 15px;
            float: left;
        }

        div.imagem{
            width: 150px;
            height: 100px;
        }

        table.corpo label{
            font-weight: bold;
            font-size: 12px;
            margin-bottom: 0;
        }

        h2{
            text-align: center;
        }

        .margin-10{
            margin: 10px;
        }
    </style>
    <style type="text/css" media="print">
        .not-print{
            display: none;
        }

        div.pull-right.search{
            display: none;
        }
    </style>
    <style type="text/css" media="screen">
        html, body {
            padding: 10px;
        }
    </style>
</head>
<!-- END HEAD -->
<body class="page-header-fixed page-sidebar-closed-hide-logo page-content-white">
<table class="cabecalho">
    <tr>
        <td>
            <img src="/imagens/logo_fundo.png" alt="logo" class="logo-default">
        </td>
        <td height="20" align="right">
            Impresso por: <label><?= $_SESSION['usunome']; ?></label><br>
            Hora da Impressão: <?= date("d/m/Y - H:i:s") ?>
        </td>
    </tr>
</table>