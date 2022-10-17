<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/classes/MontaListaAjax.class.inc";

$db = new cls_banco();

if(isset($_POST['requisicao']) && !empty($_POST['requisicao']) ){
	$obMontaListaAjax = new MontaListaAjax($db, false, false);
	$obMontaListaAjax->$_POST['requisicao']($_POST);
}