<?php

set_time_limit(30000);

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$regs = file("cargabiblioteca.csv");

if($regs[0]) {
	
	unset($regs[0]);
	
	foreach($regs as $r) {
		
		$dados = explode(";",$r);
		
		if($dados[6]) {
		
		$sql = "INSERT INTO entidade.endereco(
            	endcep, endlog, muncod, estuf, endnum, endstatus)
    			VALUES ('".substr(str_replace("-","",$dados[6]),0,8)."', 
    					'".pg_escape_string($dados[2])."', 
    					'".$dados[0]."', 
    					'".$dados[12]."', 
    					'".substr($dados[3],0,10)."', 
    					'A') RETURNING endid;";

		$endid = $db->pegaUm($sql);
		
		$sql = "INSERT INTO pdeescola.mepontobiblioteca(
	            bibnome, endid)
    			VALUES ('".pg_escape_string($dados[1])."', '".$endid."');";
		
		$db->executar($sql);
		
		}
		
	}
	
	$db->commit();
	
}

?>

