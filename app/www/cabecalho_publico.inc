<?php
/**
 * Desenvolvedor: MESOTEC Informática LTDA
 * Responsável: Adonias Malosso <adonias@mesotec.com.br>
 * Baseado no SIMEC - cedido pela DTI/SE/MEC como software público brasileiro
 */
//Verifica Temas
if(isset($_COOKIE["theme_simec"])){
    $theme = $_COOKIE["theme_simec"];
}

if(isset($_POST["theme_simec"])){
    $theme = $_POST["theme_simec"];
    setcookie("theme_simec", $_POST["theme_simec"] , time()+60*60*24*30, "/");
} else {

    $theme = "verde";
}

//Define um tema existente (padrão), caso nenhum tenha sido escolhido

if(!$theme) {
    $diretorio = APPRAIZ."www/includes/layout";
    if(is_dir($diretorio)){
        if ($handle = opendir($diretorio)) {
            while (false !== ($file = readdir($handle))) {
                if ($file != "." && $file != ".." && $file != ".svn" && is_dir($diretorio."/".$file)) {
                    $dirs[] = $file;
                }
            }
            closedir($handle);
        }
    }

    if($dirs) {
        // sorteia um tema para exibição
        $theme = $dirs[rand(0, (count($dirs)-1))];
        $_SESSION['theme_temp'] = $theme;
    }
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<!--
/**
 * Desenvolvedor: MESOTEC Informática LTDA
 * Analistas: Adonias Malosso <adonias@mesotec.com.br>, Davison Silva <davison@mesotec.com.br>
 * Programadores: Adonias Malosso <adonias@mesotec.com.br>, Halisson Gomides <halisson@mesotec.com.br>, Paulo Estevao <paulo@mesotec.com.br>
 * Baseado no SIMEC - cedido pela DTI/SE/MEC como software público brasileiro
 */
 -->
<meta http-equiv="X-UA-Compatible" content="IE=7" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1"/>
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title><?= $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'] ?></title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-3.1.0.min.js"></script>
<script type="text/javascript">jQuery.noConflict();</script>
<script type="text/javascript" src="../includes/JQuery/jquery.accordion.source.js"></script>
<script src="../includes/BeautyTips/excanvas.js" type="text/javascript"></script>
<script type="text/javascript" src="../includes/BeautyTips/jquery.bt.min.js"></script>
<link rel="stylesheet" href="/includes/login.css" type="text/css" media="screen" />
<link rel="shortcut icon" type="image/png" href="/favicon.png"/>

<link rel="stylesheet" href="../includes/bootstrap-3.3.7/css/bootstrap.min.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/bootstrap-3.3.7/js/bootstrap.min.js"></script>

</head>

<body>
    <div id="conteudo">
    	<?php include "barragoverno.php"; ?>
    	    <div id="detalhe-topo"></div>
        	<div id="caixa-menu">
        	    <div class="conteudo">
                    <ul id="menu">
                        <li>
                            <a href="login.php">Início</a>
                        </li>
                        <li>
                            <a href="cadastrar_usuario.php">Solicitar Cadastro</a>
                        </li>
                        <li>
                            <a href="recupera_senha.php">Esqueceu a senha?</a>
                        </li>
                    </ul>
        	    </div>
        	</div>
            <div id="principal">