<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$campo = $_REQUEST["campo"];
$sql = $_SESSION["SQLAJAX"][$campo];
$valor = $_REQUEST["q"];

if($sql && $valor !== null) {
	if ( !is_array( $valor ) )
	{
		// compatibilidade com códigos antigos
		$sql = str_replace( '%s', $valor, $sql );
	}
	else
	{
		$sql = vsprintf( $sql, $valor );
	}
	$rs = $db->carregar( $sql );
	if(is_array($rs) && @count($rs)>0) {
		header('Content-Type: text/javascript; charset=iso-8859-1');
		echo "var resultado=new Array(" . count($rs) . ");";
		$i = 0;
		foreach ($rs as $linha) {
			echo "resultado[".$i++."] = new Array(\"" . trim($linha["valor"]) . "\", \"" . trim($linha["descricao"]) . "\");";
		}
	}
	else {
		echo "";
		exit();
	}
}
?>