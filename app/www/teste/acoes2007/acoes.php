<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

error_reporting( E_ALL );


$db = new cls_banco();

pg_set_client_encoding( $db->link, 'UNICODE' );

header( 'Content-Type: text/plain; charset=utf-8' );

define( 'ANO', 2007 );

$sqlBase_insert = "
insert into elabrev.ppaacao_orcamento
(
	acacod, loccod, esfcod, unicod, unitpocod, funcod,
	prgano, prgcod, acadsc, sacdsc, acacodreferenciasof,
	sfucod, acastatus, prgid, acacodprosof, acadscprosof,
	acacodunmsof, acadscunmsof
)
values
(
	'%s',          -- acacod
	'%s',          -- loccod
	%d,            -- esfcod
	'%s',          -- unicod
	'%s',           -- unitpocod
	'%s',          -- funcod
	'" . ANO . "', -- prgano
	'%s',          -- prgcod
	'%s',          -- acadsc
	'%s',          -- sacdsc
	'%s',          -- acacodreferenciasof
	'%s',          -- sfucod
	'%s',           -- acastatus
	(
		select
			prgid
		from elabrev.ppaprograma_orcamento
		where
			prgcod = '%s' and
			prgstatus = 'A' and
			prgano = '" . ANO . "'
	),             -- prgid (entrada prgcod)
	'%s',          -- acacodprosof
	'%s',          -- acadscprosof
	'%s',          -- acacodunmsof
	'%s'           -- acadscunmsof
);
";


$sqlBase_update = "
update elabrev.ppaacao_orcamento
set
	acacod       = '%s',
	loccod       = '%s',
	esfcod       = '%s',
	unicod       = '%s',
	unitpocod    = '%s',
	funcod       = '%s',
	prgano       = '" . ANO . "',
	prgcod       = '%s',
	acadsc       = '%s',
	sacdsc       = '%s',
	sfucod       = '%s',
	acastatus    = '%s',
	prgid        = (
						select
							prgid
						from elabrev.ppaprograma_orcamento
						where
							prgcod = '%s' and
							prgstatus = 'A' and
							prgano = '" . ANO . "'
					),
	acacodprosof = '%s',
	acadscprosof = '%s',
	acacodunmsof = '%s',
	acadscunmsof = '%s'
where
	acacodreferenciasof = '%s';
";

$sqlBase_delete = "
delete from elabrev.ppaacao_orcamento
where acacodreferenciasof = '%s';
";

$sqlBase_count = "
select count(*)
from elabrev.ppaacao_orcamento
where acacodreferenciasof = '%s'
";

echo "

start transaction;
-- commit;
-- rollback;

-- servidor  " . $servidor_bd . "
-- banco     " . $nome_bd . "
-- data      " . date( "Y-m-d H:i:s" ) . "

";

/*
echo "

alter table elabrev.ppaacao_orcamento alter acacodprosof type varchar;
alter table elabrev.ppaacao_orcamento alter acacodunmsof type varchar;
alter table elabrev.ppaacao_orcamento alter column acacodprosof set statistics -1;
alter table elabrev.ppaacao_orcamento alter column acacodunmsof set statistics -1;

";
*/

$codigos_csv      = array();
$codigos_insercao = array();

$handle = fopen ( "acoes.csv", "r" );
$nomesCampo = fgetcsv( $handle, 1000, "," );

$quantidade_insert = 0;
$quantidade_update = 0;
while ( ( $linha = fgetcsv( $handle, 1000, "," ) ) !== FALSE)
{
    $dados = array_combine( $nomesCampo, $linha );
	if ( !$dados['COD_LOCG'] )
	{
		continue;
	}
    foreach ( $dados as &$item )
    {
    	$item = str_replace( "'", "\"", $item );
    }
    $sql_existe = sprintf(
    	$sqlBase_count,
    		$dados['COD_REFERENCIA']
    );
    $existe = $db->pegaUm( $sql_existe );
    
    if ( $existe != 0 && $existe != 1 )
    {
    	echo "
-- ERRO o codigo sof " . $dados['COD_REFERENCIA'] . " esta duplicado na base de dados
rollback;
";
    	exit();
    }
    
    array_push( $codigos_csv, $dados['COD_REFERENCIA'] );
    
    $dados['COD_ACAO']   = substr( $dados['COD_ACAO'], 0, 4 );
    $dados['COD_LOCG']   = substr( $dados['COD_LOCG'], -4 );
    $dados['COD_ORUN_U'] = substr( $dados['COD_ORUN_U'], 0, 5 );
    if ( $existe )
    {
    	$sql = sprintf(
	    	$sqlBase_update,
	    		$dados['COD_ACAO'],        // acacod
	    		$dados['COD_LOCG'],        // loccod
	    		$dados['COD_ESFE'],        // esfcod
	    		$dados['COD_ORUN_U'],      // unicod
	    		'U',                       // unitpocod
	    		$dados['COD_FUNC'],        // funcod
	    		//$dados['xxx'],           // prgano
	    		$dados['COD_PROG'],        // prgcod
	    		$dados['NOM_ACAO'],        // acadsc
	    		$dados['NOM_LOCG'],        // sacdsc
	    		$dados['COD_SBFU'],        // sfucod
	    		'A',                       // acastatus
	    		$dados['COD_PROG'],        // prgid (entrada prgcod)
	    		$dados['COD_BEMS'],        // acacodprosof
	    		$dados['DES_BEM_SERVICO'], // acadscprosof
	    		$dados['COD_UNIDADE_MED'], // acacodunmsof
	    		$dados['TIT_UNIDADE_MED'],  // acadscunmsof
				$dados['COD_REFERENCIA']   // acacodreferenciasof
	    );
	    $quantidade_update++;
    }
    else
    {
	    $sql = sprintf(
	    	$sqlBase_insert,
	    		$dados['COD_ACAO'],        // acacod
	    		$dados['COD_LOCG'],        // loccod
	    		$dados['COD_ESFE'],        // esfcod
	    		$dados['COD_ORUN_U'],      // unicod
	    		'U',                       // unitpocod
	    		$dados['COD_FUNC'],        // funcod
	    		//$dados['xxx'],           // prgano
	    		$dados['COD_PROG'],        // prgcod
	    		$dados['NOM_ACAO'],        // acadsc
	    		$dados['NOM_LOCG'],        // sacdsc
	    		$dados['COD_REFERENCIA'],  // acacodreferenciasof
	    		$dados['COD_SBFU'],        // sfucod
	    		'A',                       // acastatus
	    		$dados['COD_PROG'],        // prgid (entrada prgcod)
	    		$dados['COD_BEMS'],        // acacodprosof
	    		$dados['DES_BEM_SERVICO'], // acadscprosof
	    		$dados['COD_UNIDADE_MED'], // acacodunmsof
	    		$dados['TIT_UNIDADE_MED']  // acadscunmsof
	    );
	    $codigos_insercao[$dados['COD_REFERENCIA']] = $dados; 
	    $quantidade_insert++;
    }
    echo $sql . "\n";
    //var_dump( $dados );
    //exit();
}

$sqlCodigos = "
select acacodreferenciasof
from elabrev.ppaacao_orcamento
where prgano = '" . ANO . "'
group by acacodreferenciasof";
$codigos_base_cru = $db->carregar( $sqlCodigos );
$codigos_base_cru = $codigos_base_cru ? $codigos_base_cru : array();
$codigos_base = array();
foreach ( $codigos_base_cru as $codigo )
{
	array_push( $codigos_base, $codigo['acacodreferenciasof'] );
}

// captura os códigos diferentes
$codigos_diferenca = array_diff( $codigos_base, $codigos_csv );

// dos códigos diferentes, pega os que estão na base
$codigos_removidos = array_intersect( $codigos_base, $codigos_diferenca );

foreach ( $codigos_removidos as $codigo )
{
	$sql = sprintf( $sqlBase_delete, $codigo );
	echo $sql . "\n";
}


echo "\n\n\n-- INSERCAO ( " . $quantidade_insert . " ) ";
if ( count( $quantidade_insert ) )
{
	echo "\n\n\t-- codigo sof\tacacod\tloccod\tprgcod\tunicod\n";
	foreach ( $codigos_insercao as $codigo => $dados )
	{
		echo "\n\t-- " . $codigo . "\t" .
			$dados['COD_ACAO'] . "\t" .
    		$dados['COD_LOCG'] . "\t" .
    		$dados['COD_PROG'] . "\t" .
    		$dados['COD_ORUN_U'];
	}
}
echo "\n\n\n-- ATUALIZACAO ( " . $quantidade_update . " ) ";
echo "\n\n\n-- DELECAO ( " . count( $codigos_removidos ) . " ) ";
if ( count( $codigos_removidos ) )
{
	echo "\n\n\t-- codigo sof\tacacod\tloccod\tprgcod\tunicod\n";
	foreach ( $codigos_removidos as $codigo )
	{
		$sql = "
			select
				acacod,
				loccod,
				prgcod,
				unicod
			from elabrev.ppaacao_orcamento
			where
				acacodreferenciasof = '" . $codigo . "'
		";
		$dados = $db->recuperar( $sql );
		echo "\n\t-- " . $codigo . "\t" .
			$dados['acacod'] . "\t" .
    		$dados['loccod'] . "\t" .
    		$dados['prgcod'] . "\t" .
    		$dados['unicod'];
	}
}

fclose( $handle );










