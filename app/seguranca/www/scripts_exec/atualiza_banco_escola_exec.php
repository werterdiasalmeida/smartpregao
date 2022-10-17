<?php 

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

$db = new cls_banco();


if(is_file('update_coord_escolas.txt')) {
	$dados = file('update_coord_escolas.txt');
	if($dados) {
		foreach($dados as $d) {
			if($d) {
				$db->executar(str_replace("\'","'",$d));
			}
		}
	}
	$fp = fopen('update_coord_escolas.txt', 'w+');
	fwrite($fp, '');
	fclose($fp);
	$db->commit();
}



?>