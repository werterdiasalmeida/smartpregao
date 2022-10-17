<?php
// carrega as funções gerais
include_once "config.inc";
//include "verificasistema.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

include_once APPRAIZ."includes/classes/fileSimec.class.inc";
//$campos		= array("taeid"	 => $_POST["taeid"], 
//					"entid"  => $entid,
//					"aendsc" => $_POST["aendsc"]);
	
foreach($_POST['imgcrop'] as $k => $v) {
	$file  = new FilesSimec();
	$arqid = $file->setCrop($v);
	
	die("{arqid:{$arqid}}");
}
