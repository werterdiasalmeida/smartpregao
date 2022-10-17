<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/classes/Modelo.class.inc";
require_once APPRAIZ . "includes/classes/Controle.class.inc";
require_once APPRAIZ . "includes/classes/Visao.class.inc";
require_once APPRAIZ . "includes/classes/PaginacaoAjax.class.inc";
require_once APPRAIZ . "www/{$_SESSION['sisdiretorio']}/_funcoes.php";
require_once APPRAIZ . "www/{$_SESSION['sisdiretorio']}/_constantes.php";
require_once APPRAIZ . "includes/classes_simec.inc";

if(!is_object($db)){
	$db = new cls_banco();
}

$classe = 'classes';
$controle = 'controle';
if($_SESSION['sisdiretorio']=='obras'){
	$classe = 'classe';
	$controle = 'controller';
}
define('CLASSES_GERAL',    APPRAIZ . "/includes/classes/");
define('CLASSES_CONTROLE_GERAL', APPRAIZ . "/includes/classes/controller/");
define('CLASSES_CONTROLE', APPRAIZ . "/{$_SESSION['sisdiretorio']}/$classe/$controle/");
define('CLASSES_MODELO'  , APPRAIZ . "/{$_SESSION['sisdiretorio']}/$classe/modelo/");
define('CLASSES_VISAO'  ,  APPRAIZ . "/includes/classes/view/");
define('CLASSES_HTML'  ,   APPRAIZ . "/includes/classes/html/");

set_include_path(CLASSES_GERAL. PATH_SEPARATOR .
				 CLASSES_CONTROLE_GERAL . PATH_SEPARATOR . 
				 CLASSES_CONTROLE . PATH_SEPARATOR . 
				 CLASSES_MODELO . PATH_SEPARATOR . 
				 CLASSES_VISAO . PATH_SEPARATOR . 
				 CLASSES_HTML . PATH_SEPARATOR . 
				 get_include_path() );

function __autoload($class) {
    if(PHP_OS != "WINNT") { // Se "não for Windows"
    	$separaDiretorio = ":";
	    $include_path = get_include_path();
	    $include_path_tokens = explode($separaDiretorio, $include_path);
	   	$include_path_tokens = str_replace("//", "/", $include_path_tokens);
	} else { // Se for Windows
    	$separaDiretorio = ";c:";
	    $include_path = get_include_path();
	    $include_path_tokens = explode($separaDiretorio, $include_path);
	    $include_path_tokens = str_replace("//", "/", $include_path_tokens);
    	$include_path_tokens = explode(";", $include_path_tokens[0]);
	}

    foreach($include_path_tokens as $prefix){
            $path[0] = $prefix . $class . '.class.inc';
            $path[1] = $prefix . $class . '.php';
     
     	foreach($path as $thisPath){
        	if(file_exists($thisPath)){
            	require_once $thisPath;
                return;
            }
		}
    }
}

call_user_func($_POST['nmControleMetodo'], $_POST);