<?php

header( "Content-Type: text/plain;" );

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

error_reporting( E_ALL );

$sqlBase = "
update cte.subacaoindicador
	set sbaprm = '%s'
	where trim( sbaprm ) in (
		'%s'
	);
";

echo "

-- gerado em " . date( "Y-m-d H:i:s" ) . "

start transaction;

-- commit
-- rollback

";

$toFrom = array();

$handle = fopen ( "de_para.csv", "r" );
while ( ( $linha = fgetcsv( $handle, 1000, "," ) ) !== FALSE)
{
	$from = trim( $linha[0] );
	$to = trim( $linha[1] );
	if ( !$to || $from == $to )
	{
		continue;
	}
	if ( !array_key_exists( $to, $toFrom ) )
	{
		$toFrom[$to] = array();
	}
	array_push( $toFrom[$to], str_replace( "'", "\\'", $from ) );
}

foreach ( $toFrom as $to => $from )
{
	$sql = sprintf(
		$sqlBase,
			$to,
			implode( "',\n\t\t'", $from )
	);
	echo $sql;
}

?>