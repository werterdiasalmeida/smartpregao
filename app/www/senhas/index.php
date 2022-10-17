<?php
include_once "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'www/adm/_constantes.php';

$db                    = new cls_banco();
$_SESSION['superuser'] = true;

$painelSenha = new PainelSenha();

if ($_GET['p']) {
    $_SESSION['codigo_painel_senhas'] = $_GET['p'];
    header("Location: /senhas");
}

if ($_POST['logoff'] == '1') {
    $_SESSION['erro_painel_senha'] = $_POST['msg'];
    unset($_SESSION['codigo_painel_senhas']);
    //header("Location: /senhas");
}

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

// Ações da tela
if (isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'enviar-codigo':
            if (!empty($_POST['codigo'])) {
                $_SESSION['codigo_painel_senhas'] = $_POST['codigo'];
            }

            break;
        case 'atualizar':
            $_SESSION['codigo_painel_senhas'] = $_SESSION['codigo_painel_senhas'] ?: $_POST['codigo'];
            $dadosPainel = $painelSenha->getDadosByCodigo($_SESSION['codigo_painel_senhas']);
            $db->sucessomAjax('', $dadosPainel);
            break;
    }
}

$dadosPainel = $painelSenha->getDadosByCodigo($_SESSION['codigo_painel_senhas']);
if ($_POST['codigo'] && !$dadosPainel) {
    $_SESSION['erro_painel_senha'] = "Código não encontrado!";
}

?>

<html>
    <!-- BEGIN HEAD -->
    <head>
        <title>Painel de senhas</title>

        <!-- BEGIN GLOBAL MANDATORY STYLES -->
        <link href="/includes/metronic/global/plugins/bootstrap/css/bootstrap.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css"/>
        <link href="//fonts.googleapis.com/css?family=Open+Sans:400,300,600,700&subset=all" rel="stylesheet" type="text/css"/>
        <link href="/includes/metronic/global/plugins/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css"/>
        <link href="/includes/metronic/global/plugins/simple-line-icons/simple-line-icons.min.css" rel="stylesheet" type="text/css"/>
        <link href="/includes/metronic/global/plugins/bootstrap/css/bootstrap.min.css" rel="stylesheet" type="text/css"/>
        <link href="/includes/metronic/global/plugins/bootstrap-switch/css/bootstrap-switch.min.css" rel="stylesheet" type="text/css"/>
        <!-- END GLOBAL MANDATORY STYLES -->

        <!-- BEGIN THEME GLOBAL STYLES -->
        <link href="/includes/metronic/global/css/components.min.css" rel="stylesheet" id="style_components" type="text/css"/>
        <link href="/includes/metronic/global/css/plugins.min.css" rel="stylesheet" type="text/css"/>
        <!-- END THEME GLOBAL STYLES -->

        <link rel="shortcut icon" href="/favicon.ico?v=<?= VERSAO ?>"/>

        <!--[if lt IE 9]>
        <script src="/includes/metronic/global/plugins/respond.min.js"></script>
        <script src="/includes/metronic/global/plugins/excanvas.min.js"></script>
        <![endif]-->

        <!-- BEGIN CORE PLUGINS -->
        <script src="/includes/metronic/global/plugins/jquery.min.js" type="text/javascript"></script>
        <script src="/includes/metronic/global/plugins/bootstrap/js/bootstrap.min.js" type="text/javascript"></script>
        <!-- END CORE PLUGINS -->

        <!-- BEGIN PAGE LEVEL PLUGINS -->
        <script src="/includes/marquee/jquery.marquee.min.js" type="text/javascript"></script>

<!--        <script src="/includes/FitText/jquery.fittext.js" type="text/javascript"></script>-->
<!--        <script src="/includes/FlowType/flowtype.js" type="text/javascript"></script>-->
<!--        <script src="/includes/fitty/fitty.min.js" type="text/javascript"></script>-->
        <script src="/includes/textFit/textFit.min.js" type="text/javascript"></script>

        <script src="/includes/metronic/global/plugins/jquery-validation/js/jquery.validate.min.js" type="text/javascript"></script>
        <script src="/includes/metronic/global/plugins/jquery-validation/js/additional-methods.min.js" type="text/javascript"></script>
        <script src="/includes/metronic/global/plugins/select2/js/select2.full.min.js" type="text/javascript"></script>
        <script src="/includes/metronic/global/plugins/jquery-inputmask/jquery.inputmask.bundle.min.js" type="text/javascript"></script>
        <script src="/includes/funcoes.js" type="text/javascript"></script>
        <!-- END PAGE LEVEL PLUGINS -->

    </head>
    <!-- END HEAD -->

    <body>
        <input type="hidden" name="hdn-codigo" value="<?= $_SESSION['codigo_painel_senhas'] ?>">
        <?php
            if ($dadosPainel) {
                require_once "painel.php";
            } else {
                require_once "inicial.php";
            }

        ?>
    </body>

</html>