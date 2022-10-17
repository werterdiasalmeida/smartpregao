<?php

header( 'Content-Type: text/plain; charset=iso-8859-1' );

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

error_reporting( E_ALL );

$db = new cls_banco();

$sqlBase = "
update cte.proposicaoacao
set ppadsc = '%s'
where ppaid = %d;
";

echo "
start transaction;
-- commit
-- rollback
";
$arquivo = fopen( "guia.csv", "r" );
fgetcsv( $arquivo );
while ( $linha = fgetcsv( $arquivo ) )
{
	$id   = $linha[4];
	$de   = trim( $linha[5] );
	$para = trim( $linha[6] );
	if ( !$id || !$de || !$para )
	{
		continue;
	}
	$sql = sprintf(
		$sqlBase,
			str_replace( "'", "\\'", $para ),
			$id
	);
	echo $sql;
}

echo "
update cte.acaoindicador ai
set acidsc = ( select ppadsc from cte.proposicaoacao ps where ps.ppaid = ai.ppaid )
where ai.ppaid is not null;
";
