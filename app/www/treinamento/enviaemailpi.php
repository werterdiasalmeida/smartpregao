<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

$db = new cls_banco();


 	flush();

	include APPRAIZ . "monitora/www/planotrabalho/_funcoes.php";
	
	$sql = "SELECT plicod
	  		FROM monitora.planointerno
			where plisituacao <> 'P' ";
	
	
	$dados = $db->carregar( $sql );
			
	foreach($dados as $d){
	
		$pi = "'".$d['plicod']."'";
		 		
		enviaEmailStatusPi($pi);
	
		echo $pi . ",";
		
		flush();
		//sleep(1);
	
	}

?>