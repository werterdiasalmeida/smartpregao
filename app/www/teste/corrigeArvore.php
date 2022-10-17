<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

$db = new cls_banco();

header( 'Content-Type: text/plain' );

$atiid = (integer) $_REQUEST['atiid'];

$sqlBaseCount = "
select count(*)
from pde.atividade a
where
	a.atistatus = 'A' and
	a.atiidpai = $atiid
";

function atividade_calcular_dados( $atividade ){
	global $db;
	// pega dados da atividade
	$atividade = (integer) $atividade;
	$sql = "select _atiordem, _atinumero, _atiprofundidade, _atiprojeto from pde.atividade where atiid = " . $atividade;
	$pai = $db->recuperar( $sql );
	
	// pega filhos
	$sql = "select atiid, atiordem from pde.atividade where atiidpai = " . $atividade . " and atistatus = 'A'";
	$filhos = $db->carregar( $sql );
	$filhos = $filhos ? $filhos : array();
	$sql = "update pde.atividade set _atifilhos = " . count( $filhos ) . " where atiid = " . $atividade;
	echo $sql . ";\n";
	//$db->executar( $sql, false );
	
	// atualiza filhos
	foreach ( $filhos as $filho ){
		$_atinumero  = ( $pai['_atinumero'] ? $pai['_atinumero'] . "." : '' ) . $filho['atiordem'];
		$_atiordem   = ( $pai['_atiordem'] ? $pai['_atiordem'] : '' ) . sprintf( '%04d', $filho['atiordem'] );
		$_atiprojeto = (integer) $pai['_atiprojeto']; 
		$sql = "update pde.atividade set _atinumero = '" . $_atinumero . "', _atiordem = '" . $_atiordem . "', _atiprofundidade = " . ( $pai['_atiprofundidade'] + 1 ) . ", _atiirmaos = " . count( $filhos ) . ", _atiprojeto = " . $_atiprojeto . " where atiid = " . $filho['atiid'];
		echo $sql . ";\n\n";
		//$db->executar( $sql, false );
		atividade_calcular_dados( $filho['atiid'] );
	}
}

function corrige( $atiid )
{
	global $db;
	$sql = "
		select a.atiid
		from pde.atividade a
		where
			a.atistatus = 'A' and
			a.atiidpai = $atiid
		order by
			a.atiordem
	";
	$filhos = $db->carregar( $sql );
	$filhos = $filhos ? $filhos : array();
	$quantidade = count( $filhos );
	$numero = 1;
	foreach ( $filhos as $filho )
	{
		$atiidfilho = $filho['atiid'];
		$sql = "update pde.atividade set atiordem = $numero where atiid = $atiidfilho;\n\n";
		echo $sql;
		corrige( $atiidfilho );
		$numero++;
	}
}

corrige( $atiid );
atividade_calcular_dados( $atiid );

?>