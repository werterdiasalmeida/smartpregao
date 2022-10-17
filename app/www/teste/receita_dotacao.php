<?php

phpinfo();
exit;

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$sql = <<<EOT
	select *
	from public.naturezareceita
	--where nrcstatus = 'A'
EOT;
$linhas = $db->carregar( $sql );
$linhas = $linhas ? $linhas : array();
$nat = array();
foreach ( $linhas as $linha )
{
	$nat[$linha['nrccod']] = $linha['nrcid'];
}

/*
print '<pre>';
var_dump( $nat );
exit();
*/

header( 'Content-Type: text/plain;' );

$sql_base = <<<EOT
insert into elabrev.receitaunidadedotacao ( rcuano, unicod, unitpocod, foncod, nrcid, rcdvalor )
values ( '%s', '%s', '%s', '%s', %d, %d );
EOT;
$linhas = file( 'receita_dotacao' );
foreach ( $linhas as $linha )
{
	$dados = explode( "\t", trim( $linha ) );
	$registro = array(
		'rcuano'    => '2007',
		'unicod'    => $dados[0],
		'unitpocod' => $dados[1],
		'foncod'    => $dados[2],
		'nrcid'     => (integer) $nat[$dados[3]],
		'rcdvalor'  => (integer) str_replace( '.', '', $dados[4] )
	);
	if ( $registro['rcdvalor'] <= 0 )
	{
		continue;
	}
	if ( $registro['nrcid'] == 0 )
	{
		print 'natureza de receita ' . $dados[3] . ' não existe';
		exit();
	}
	$sql = sprintf(
		$sql_base,
			$registro['rcuano'],
			$registro['unicod'],
			$registro['unitpocod'],
			$registro['foncod'],
			$registro['nrcid'],
			$registro['rcdvalor']
	);
	print $sql . "\n\n";
}











