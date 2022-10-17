<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

/*
$nome_bd     = 'simec_desenvolvimento';
$servidor_bd = 'simec-d';
$porta_bd    = '5432';
$usuario_db  = 'seguranca';
$senha_bd    = 'phpseguranca';
*/

$db = new cls_banco();

header( 'Content-Type: text/plain' );

$sql = "select atiid, atidescricao from pde.atividade where atistatus = 'A'";
$atividades = $db->carregar( $sql );
$atividades = $atividades ? $atividades : array();
foreach ( $atividades as $atividade )
{
	$atiid = $atividade['atiid'];
	$sql = "select atiid from pde.atividade where atiidpai = " . $atiid . " and atistatus = 'A' order by _atiordem";
	$filhos = $db->carregar( $sql );
	$filhos = $filhos ? $filhos : array();
	if ( !count( $filhos ) )
	{
		continue;
	}
	$atiordem = 1;
	//echo "-- " . $atividade['atidescricao'] . "\n\n";
	foreach ( $filhos as $filho )
	{
		$atiid = $filho['atiid'];
		$sql = "update pde.atividade set atiordem = " . $atiordem . " where atiid = " . $atiid . ";";
		$atiordem++;
		echo $sql . "\n";
	}
	echo "\n\n\n";
}

