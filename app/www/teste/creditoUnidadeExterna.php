<?php

$creditos = array(
	array(
		'unicod'   => '74902',
		'prgcod'   => '1073',
		'acacod'   => '4556',
		'loccod'   => '0001',
		'ndpcod'   => '33900000',
		'iducod'   => '0',
		'foncod'   => '100',
		'dpavalor' => '56700000'
	),
	array(
		'unicod'   => '74902',
		'prgcod'   => '1073',
		'acacod'   => '0579',
		'loccod'   => '0001',
		'ndpcod'   => '33900000',
		'iducod'   => '0',
		'foncod'   => '100',
		'dpavalor' => '25139451'
	),
	array(
		'unicod'   => '74902',
		'prgcod'   => '1073',
		'acacod'   => '0579',
		'loccod'   => '0001',
		'ndpcod'   => '33900000',
		'iducod'   => '0',
		'foncod'   => '118',
		'dpavalor' => '343074575'
	),
	array(
		'unicod'   => '74902',
		'prgcod'   => '1073',
		'acacod'   => '0579',
		'loccod'   => '0001',
		'ndpcod'   => '33900000',
		'iducod'   => '0',
		'foncod'   => '180',
		'dpavalor' => '555348761'
	),
);

header( 'Content-Type: text/plain;' );

require_once "config.inc";

$nome_bd     = 'simec_espelho_producao';
$servidor_bd = 'simec-d';
$porta_bd    = '5432';
$usuario_db  = 'seguranca';
$senha_bd    = 'phpseguranca';

include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

define( 'EXERCICIO', 2007 );

// pega ppoid
$sql = sprintf(
"
	select
		ppoid
	from elabrev.propostaorcamento
	where
		ppoano = '%s' and
		tppid = %d and
		ppostatus = 'A'
",
	EXERCICIO,
	3
);
$ppoid = $db->pegaUm( $sql );

echo "start transaction;\n";

foreach ( $creditos as $credito )
{
	// pega acaid
	$sql = sprintf(
"
		select acaid
		from monitora.acao
		where
			unicod = '%s' and
			prgcod = '%s' and
			acacod = '%s' and
			loccod = '%s' and
			acasnrap = 'f' and
			prgano = '%s'
",
			$credito['unicod'],
			$credito['prgcod'],
			$credito['acacod'],
			$credito['loccod'],
			EXERCICIO
	);
	$acaid = $db->pegaUm( $sql );
	// pega ndpid
	$sql = sprintf(
"
		select ndpid
		from public.naturezadespesa
		where
			ndpcod = '%s' and
			ndpstatus = 'A'
",
			$credito['ndpcod']
	);
	$ndpid = $db->pegaUm( $sql );
	echo sprintf(
"
insert into elabrev.despesaacao ( acaidloa, ndpid, iducod, foncod, ppoid, idoid, dpavalor )
values ( %s, %s, '%s', '%s', %s, %s, %s );
",
			$acaid,
			$ndpid,
			$credito['iducod'],
			$credito['foncod'],
			$ppoid,
			1,
			$credito['dpavalor']
	);
}

echo "\n--rollback;\n";
echo "--commit;\n";





















