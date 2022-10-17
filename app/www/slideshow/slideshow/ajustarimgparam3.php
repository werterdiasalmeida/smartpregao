<?
session_start();
$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");
$_SESSION['imgparams'] = array("filtro" => "cnt.obrid=".$_REQUEST['obrid']." AND aqostatus = 'A'", 
										   "tabela" => "obras.arquivosobra");

header("location: index.php?pagina=". $_REQUEST['pagina'] ."&_sisarquivo=".$_REQUEST['_sisarquivo']);
exit;
?>