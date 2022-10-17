<?
session_start();
$_SESSION["imgparams"] = $_SESSION['imgparametos'][$_REQUEST["arqid"]];
header("location: index.php?pagina=". $_REQUEST['pagina'] ."&_sisarquivo=".$_REQUEST['_sisarquivo']."&arqid=".$_REQUEST['arqid']);
exit;
?>