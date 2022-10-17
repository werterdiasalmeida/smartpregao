<?php

include_once "config.inc";

ini_set('memory_limit','4096M');

if (file_exists(APPRAIZ . "arquivos/painel/logs/".$_REQUEST['data'].".log")) {

	echo "<pre>";
	print_r(file_get_contents(APPRAIZ . "arquivos/painel/logs/".$_REQUEST['data'].".log"));

} else {

	echo "Não existe log nesta data";
	
}

?>