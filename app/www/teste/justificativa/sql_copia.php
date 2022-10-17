<?php

header( "Content-Type: text/plain;" );

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

error_reporting( E_ALL );

//$nome_bd     = 'simec_espelho_producao';
//$servidor_bd = 'simec-d';
//$porta_bd    = '5432';
//$usuario_db  = 'seguranca';
//$senha_bd    = 'phpseguranca';

$db = new cls_banco();

$sql = "select * from elabrev.justificativacredito where mcrid = 1";
$linhas = $db->carregar( $sql );

//dbg( $linhas );

echo "
start transaction;

-- commit;
-- rollback;

-- select * from elabrev.justificativacredito where mcrid = 7
-- delete from elabrev.justificativacredito where mcrid = 7

";

foreach ( $linhas as $linha )
{
	$campos = array();
	$valores = array();
	foreach ( $linha as $campo => $valor )
	{
		if ( $campo == "mcrid" )
		{
			$valor = 7;
		}
		else
		{
			$valor = str_replace( "'", "\\'", $valor );
			$valor = str_replace( "\r", "", $valor );
		}
		array_push( $campos, $campo );
		array_push( $valores, $valor );
	}
	$sqlInsert =  "
insert into elabrev.justificativacredito ( " . implode( ",", $campos ) . " )
values ( '" . implode( "','", $valores ) . "' );
";
	echo $sqlInsert;
}

?>