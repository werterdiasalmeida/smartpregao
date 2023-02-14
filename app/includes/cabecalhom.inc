<?php
if ($_POST['act']) {
    switch ($_POST['act']) {
        case 'carregarSimulacaoUsuario':
            if (($_SESSION['superuser'] or $db->testa_uma() or $_SESSION['usuuma'])) {
                include_once 'listaSimulacaoUsuario.inc';
            }
            die;
            break;
        case 'abrirChangelog':
            include_once 'listaChangeLog.inc';
            die;
            break;
    }
}
// ativa o flag de inclus?o do cabe?alho
$cabecalhoMetronic = true;
$moduloTitle = '';
if ($modulo_atual != "") {
    $sql = "SELECT DISTINCT 
                                  COALESCE(mnu1.mnudsc, '')  || '|' || COALESCE(mnu2.mnudsc, '') || '|' || COALESCE(mnu3.mnudsc, '') as rastro 
                            FROM 
                                  seguranca.menu mnu3 
                            LEFT JOIN 
                                  seguranca.menu mnu2 ON mnu3.mnuidpai=mnu2.mnuid 
                            LEFT JOIN 
                                  seguranca.menu mnu1 ON mnu2.mnuidpai=mnu1.mnuid 
                            WHERE 
                                  mnu3.mnuid = {$modulo_atual}";
    $breadcrumb  = $db->pegaUm($sql);
    $moduloTitle = array_pop(explode('|', $breadcrumb));
}
?>
<!DOCTYPE html>
<html lang="pt">
<!--<![endif]-->
<!-- BEGIN HEAD -->

<head>
    <meta charset="ISO-8859-1" />
    <title><?php echo ($moduloTitle ? "{$moduloTitle} - " : '') . $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'] ?></title>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />
    <meta content="width=device-width, initial-scale=1" name="viewport" />
    <meta content="" name="description" />
    <meta content="" name="author" />
    <!-- BEGIN GLOBAL MANDATORY STYLES -->
    <link href="//fonts.googleapis.com/css?family=Open+Sans:400,300,600,700&subset=all" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/font-awesome/css/font-awesome.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/simple-line-icons/simple-line-icons.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/bootstrap/css/bootstrap.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/bootstrap-switch/css/bootstrap-switch.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <!-- END GLOBAL MANDATORY STYLES -->
    <!-- BEGIN PAGE LEVEL PLUGINS -->
    <link href="/includes/metronic/global/plugins/bootstrap-daterangepicker/daterangepicker.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/morris/morris.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/fullcalendar/fullcalendar.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/jqvmap/jqvmap/jqvmap.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/bootstrap-fileinput/bootstrap-fileinput.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/bootstrap-table/bootstrap-table.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/bootstrap-select/css/bootstrap-select.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/bootstrap-datepicker/css/bootstrap-datepicker3.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/bootstrap-datetimepicker/css/bootstrap-datetimepicker.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/bootstrap-timepicker/css/bootstrap-timepicker.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/dropzone/dropzone.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/jquery-ui/jquery-ui.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/selectize-0.12.1/css/selectize.bootstrap3.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/select2/css/select2.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/select2/css/select2-bootstrap.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/estilom.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <!-- END PAGE LEVEL PLUGINS -->
    <!-- BEGIN THEME GLOBAL STYLES -->
    <link href="/includes/metronic/global/css/components.min.css?v=<?= VERSAO ?>" rel="stylesheet" id="style_components" type="text/css" />
    <link href="/includes/metronic/global/css/plugins.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <!-- END THEME GLOBAL STYLES -->
    <!-- BEGIN THEME LAYOUT STYLES -->
    <link href="/includes/metronic/layouts/layout/css/layout.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/layouts/layout/css/themes/lightblue.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" id="style_color" />
    <!-- END THEME LAYOUT STYLES -->
    <link rel="shortcut icon" href="favicon.ico?v=<?= VERSAO ?>" />
    <!--[if lt IE 9]>
    <script src="/includes/metronic/global/plugins/respond.min.js?v=<?= VERSAO ?>"></script>
    <script src="/includes/metronic/global/plugins/excanvas.min.js?v=<?= VERSAO ?>"></script>
    <![endif]-->
    <!-- BEGIN CORE PLUGINS -->
    <script src="/includes/metronic/global/plugins/jquery.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-ui-1.12.0.min.js?v=<?= VERSAO ?>"></script>
    <script src="/includes/metronic/global/plugins/bootstrap/js/bootstrap.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/js.cookie.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-hover-dropdown/bootstrap-hover-dropdown.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-slimscroll/jquery.slimscroll.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery.blockui.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-switch/js/bootstrap-switch.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <!-- END CORE PLUGINS -->
    <!-- BEGIN PAGE LEVEL PLUGINS -->
    <script src="/includes/metronic/global/plugins/morris/morris.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/morris/raphael-min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/counterup/jquery.waypoints.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/counterup/jquery.counterup.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/amcharts/amcharts/amcharts.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/amcharts/amcharts/serial.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/amcharts/amcharts/pie.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/amcharts/amcharts/radar.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/amcharts/amcharts/themes/light.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/amcharts/amcharts/themes/patterns.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/amcharts/amcharts/themes/chalk.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/amcharts/ammap/ammap.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/amcharts/ammap/maps/js/worldLow.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/amcharts/amstockcharts/amstock.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/horizontal-timeline/horozontal-timeline.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/flot/jquery.flot.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/flot/jquery.flot.resize.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/flot/jquery.flot.categories.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-easypiechart/jquery.easypiechart.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery.sparkline.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jqvmap/jqvmap/jquery.vmap.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jqvmap/jqvmap/maps/jquery.vmap.russia.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jqvmap/jqvmap/maps/jquery.vmap.world.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jqvmap/jqvmap/maps/jquery.vmap.europe.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jqvmap/jqvmap/maps/jquery.vmap.germany.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jqvmap/jqvmap/maps/jquery.vmap.usa.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jqvmap/jqvmap/data/jquery.vmap.sampledata.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-validation/js/jquery.validate.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-validation/js/additional-methods.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-inputmask/jquery.inputmask.bundle.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-fileinput/bootstrap-fileinput.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-table/bootstrap-table.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-table/locale/bootstrap-table-pt-BR.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-select/js/bootstrap-select.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-datepicker/js/bootstrap-datepicker.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-timepicker/js/bootstrap-timepicker.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-datepicker/locales/bootstrap-datepicker.pt-BR.min.js?v=<?= VERSAO ?>"></script>
    <script src="/includes/metronic/global/plugins/dropzone/dropzone.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-touchspin/bootstrap.touchspin.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery.scrollTo.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/moment.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/bootstrap-datetimepicker/js/bootstrap-datetimepicker.min.js?v=<?= VERSAO ?>"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-growl/jquery.bootstrap-growl.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/clipboardjs/clipboard.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/selectize-0.12.1/js//standalone/selectize.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/selectize-0.12.1/js//standalone/disableOptions.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/select2/js/select2.full.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/select2/js/i18n/pt-BR.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/jquery.highlight.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script type="text/javascript" src="../includes/sweetalert2/sweetalert2.js"></script>
    <script src="/includes/funcoes.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/funcoesm.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <!-- END PAGE LEVEL PLUGINS -->
    <!-- BEGIN THEME GLOBAL SCRIPTS -->
    <script src="/includes/metronic/global/scripts/app.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <!-- END THEME GLOBAL SCRIPTS -->
    <!-- BEGIN PAGE LEVEL SCRIPTS -->
    <script src="/includes/metronic/pages/scripts/dashboard.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <!-- END PAGE LEVEL SCRIPTS -->
    <!-- BEGIN THEME LAYOUT SCRIPTS -->
    <script src="/includes/metronic/layouts/layout/scripts/layout.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/layouts/global/scripts/quick-sidebar.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/estouvivo.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.10.3/font/bootstrap-icons.css">
    <!-- END THEME LAYOUT SCRIPTS -->
</head>
<!-- END HEAD -->
<?php
// pega a página inicial do sistema
$sql = sprintf(
    "SELECT TRIM( paginainicial ) AS paginic FROM seguranca.sistema WHERE sisid = %d",
    $_SESSION['sisid']
);
$paginic = $db->pegaUm($sql);
?>
<!-- page-sidebar-fixed -->

<body class="page-header-fixed page-sidebar-closed-hide-logo page-content-white page-sidebar-closed">
    <!-- BEGIN HEADER -->
    <div class="page-header navbar navbar-fixed-top">
        <!-- BEGIN HEADER INNER -->
        <div class="page-header-inner ">
            <!-- BEGIN LOGO -->
            <img src="/imagens/logo2.png?v=<?= VERSAO ?>" alt="logo" class="logo-default" style="height: 42px; margin: 5px; float:left;" />
            <!-- END LOGO -->
            <!-- BEGIN RESPONSIVE MENU TOGGLER -->

            <!-- END RESPONSIVE MENU TOGGLER -->
            <!-- BEGIN TOP NAVIGATION MENU -->
            <div class="top-menu">
                <a href="javascript:;" class="menu-toggler responsive-toggler" data-toggle="collapse" data-target=".navbar-collapse">
                    <span></span>
                </a>
                <ul class="nav navbar-nav pull-right">
                    <li class="dropdown dropdown-user">
                        <a href="javascript:;" class="dropdown-toggle" data-toggle="dropdown" data-hover="dropdown" data-close-others="true">
                            <?php if (isset($_SESSION['usuarqid']) && !empty($_SESSION['usuarqid'])) : ?>
                                <img alt="" class="img-circle" style="background-color: #FFFFFF; width: 29px;" src="../slideshow/slideshow/verimagem.php?arqid=<?= $_SESSION['usuarqid']; ?>&newwidth=100&newheight=100" />
                            <?php else : ?>
                                <img alt="" class="img-circle" style="background-color: #FFFFFF;" src="<?= ($_SESSION['ususexo'] === 'F' ? '/imagens/woman.png' : '/imagens/man.png'); ?>" />
                            <?php endif; ?>
                            <span class="username username-hide-on-mobile">
                                <?php if ($_SESSION['superuser'] or $db->testa_uma() or $_SESSION['usuuma']) : ?>
                                    <?= $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf = '{$_SESSION['usucpf']}'") ?>
                                <?php else : ?>
                                    <?= $_SESSION['usunome']; ?>
                                <?php endif; ?>
                            </span>
                            <i class="fa fa-angle-down"></i>
                        </a>
                        <ul class="dropdown-menu dropdown-menu-default">
                            <?php $_SESSION['usucpf'] = $_REQUEST['usucpf_simu'] ? $_REQUEST['usucpf_simu'] : $_SESSION['usucpf']; ?>
                            <?php if ($_SESSION['superuser'] or $db->testa_uma() or $_SESSION['usuuma']) : ?>
                                <?php if ($_SESSION['usucpforigem'] != $_SESSION['usucpf']) : ?>
                                    <li>
                                        <a data-toggle="modal" onclick="setpfl('<?= $_SESSION['usucpforigem']; ?>');">
                                            <i class="fa fa-user-times"></i>Cancelar simulação
                                        </a>
                                    </li>
                                <?php endif; ?>
                                <li>
                                    <a data-toggle="modal" onclick="abrirSimulacaoUsuario();">
                                        <i class="fa fa-user-secret"></i>Simular usuário
                                    </a>
                                    <script type="application/javascript">
                                        function abrirSimulacaoUsuario() {
                                            var modal = $('#simular-usuario');
                                            modal.find('.modal-body').html('<div style="height: 250px;"></div>')

                                            App.blockUI({
                                                message: 'Carregando...',
                                                target: '#simular-usuario .modal-body'
                                            });

                                            modal.find('.modal-body').load(window.location.href, {
                                                act: 'carregarSimulacaoUsuario'
                                            }, function() {
                                                App.unblockUI('#simular-usuario .modal-body');
                                            });

                                            modal.modal({
                                                backdrop: 'static'
                                            }).modal('show');
                                        }
                                    </script>
                                </li>
                            <? endif; ?>
                            <li>
                                <a href="?modulo=sistema/usuario/altsenha&acao=X">
                                    <i class="fa fa-key"></i> Alterar senha </a>
                            </li>
                            <li>
                                <a href="?modulo=sistema/usuario/dados&acao=X">
                                    <i class="fa fa-user"></i> Minhas informações </a>
                            </li>
                            <li>
                                <a href="/logout.php">
                                    <i class="icon-logout"></i> Sair </a>
                            </li>
                        </ul>
                    </li>
                    <?php if ($db->testa_superuser()) : ?>
                        <li class="dropdown">
                            <a href="../muda_sistema.php?sisid=4" class="dropdown-toggle" title="SIS" style="padding-right: 10px;">
                                <i class="icon-settings"></i>
                            </a>
                        </li>
                    <?php endif; ?>
                    <!-- END USER LOGIN DROPDOWN -->
                    <!-- BEGIN QUICK SIDEBAR TOGGLER -->
                    <!-- DOC: Apply "dropdown-dark" class after below "dropdown-extended" to change the dropdown styte -->
                    
                    <!-- END QUICK SIDEBAR TOGGLER -->
                </ul>
            </div>
            <!-- END TOP NAVIGATION MENU -->
        </div>
        <!-- END HEADER INNER -->
    </div>
    <!-- END HEADER -->
    <!-- BEGIN HEADER & CONTENT DIVIDER -->
    <div class="clearfix"></div>
    <!-- END HEADER & CONTENT DIVIDER -->
    <!-- BEGIN CONTAINER -->
    <div class="page-container">
        <!-- BEGIN SIDEBAR -->
        <div class="page-sidebar-wrapper">
            <!-- BEGIN SIDEBAR -->
            <?php include APPRAIZ . "includes/gera_menum.inc"; ?>
            <!-- END SIDEBAR -->
        </div>
        <!-- END SIDEBAR -->
        <!-- BEGIN CONTENT -->
        <div class="page-content-wrapper">
            <!-- BEGIN CONTENT BODY -->
            <div class="page-content">
                <?php if ($_SESSION['superuser'] or $db->testa_uma() or $_SESSION['usuuma']) : ?>
                    <div class="modal fade" id="simular-usuario" tabindex="-1" role="basic" aria-hidden="true">
                        <div class="modal-dialog modal-lg">
                            <div class="modal-content">
                                <div class="modal-header">
                                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                                    <h4 class="modal-title">Simular usuário</h4>
                                </div>
                                <form id="setperfil2" name="setperfil2" action="<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>" method="post">
                                    <input type="hidden" name="usucpf_simu" id="usucpf_simu" />
                                </form>
                                <script type="text/javascript">
                                    function setpfl(usucpf) {
                                        document.getElementById('usucpf_simu').value = usucpf;
                                        document.getElementById('setperfil2').submit();
                                    }
                                </script>
                                <div class="modal-body">
                                </div>
                                <div class="modal-footer">
                                    <button type="button" class="btn dark btn-outline" data-dismiss="modal">Fechar</button>
                                </div>
                            </div>
                            <!-- /.modal-content -->
                        </div>
                        <!-- /.modal-dialog -->
                    </div>
                <?php endif;
                /*
             * Exibe rastro de navegação. Mostra em qual
             * local da hierarquia de telas o usuário está.
             */
                if ($breadcrumb) :
                    $breadcrumb = explode('|', $breadcrumb);
                    $breadcrumb = array_filter($breadcrumb);
                ?>
                    <style type="text/css">
                        .page-content {
                            padding: 25px 20px 10px !important;
                        }
                        .top-menu{
                            clear: none !important;
                        }
                        img{border-radius: 10px !important;}
                    </style>
                    <div class="page-bar">
                        <ul class="page-breadcrumb">
                            <?php foreach ($breadcrumb as $indexBreadcrumb => $itemBreadcrumb) : ?>
                                <li>
                                    <span><?= $itemBreadcrumb ?></span>
                                    <?= (isset($breadcrumb[$indexBreadcrumb + 1]) ? '<i class="fa fa-circle"></i>' : '') ?>
                                </li>
                            <?php endforeach; ?>
                        </ul>
                    </div>
                <?php
                endif;
                ?>

                <script>
                    function exibirModalLembretesTarefas(p) {
                        App.blockUI({
                            message: 'Carregando...',
                            target: '#lembretes-tarefas'
                        });

                        $('#lembretes-tarefas').load(window.location.href, {
                            p: p,
                            act: 'lembretes-tarefas'
                        }, function() {
                            App.unblockUI('#lembretes-tarefas');
                        });

                        $('#modal-lembretes-tarefas').modal({
                            backdrop: 'static',
                        }).modal('show');
                    }
                </script>
                <div class="modal fade in" id="modal-lembretes-tarefas" tabindex="-1" role="modal-lembretes-tarefas" aria-hidden="true">
                    <div class="modal-dialog modal-lg" style="width: 95%;">
                        <div class="modal-content">
                            <div class="modal-header">
                                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                                <h4 class="modal-title">Lembretes e tarefas</h4>
                            </div>

                            <div class="lembretes-tarefas-form">
                                <div class="modal-body">
                                    <form id="form-lembretes-tarefas" method="post">
                                        <div id="lembretes-tarefas" style="min-height: 100px;">

                                        </div>
                                    </form>
                                </div>
                                <div class="modal-footer">
                                    <div class="row">
                                        <div class="col-sm-4 text-left">
                                            <button type="button" class="btn green btn-voltar hidden">
                                                <i class="fa fa-arrow-left"></i>
                                                Voltar
                                            </button>
                                        </div>
                                        <div class="col-sm-8 text-right">
                                            <button type="button" class="btn blue hidden btn-salvar"></button>
                                            <button type="button" class="btn" data-dismiss="modal">Fechar</button>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>