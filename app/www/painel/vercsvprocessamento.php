<?php

include_once "config.inc";

ini_set('memory_limit','3000M');

if (file_exists(APPRAIZ . "arquivos/painel/webservice_files/".$_REQUEST['data'].".csv")) {

	echo "<pre>";
	print_r(file_get_contents(APPRAIZ . "arquivos/painel/webservice_files/".$_REQUEST['data'].".csv"));

} else {

	echo "Não existe log nesta data";
	
}

?>