<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

error_reporting( E_ALL );



$db = new cls_banco();

header( 'Content-Type: text/plain; charset=utf-8' );

define( 'ANO', 2007 );

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

update elabrev.ppaacao_orcamento
set tdecod = '1'
where acaid in ( 9806, 8329, 9819 );

";
*/

$handle = fopen ( "detalhamento.csv", "r" );
$nomesCampo = fgetcsv( $handle, 1000, "," );

$dadosTotal = array();

$quantidade = 0;
while ( ( $linha = fgetcsv( $handle, 1000, "," ) ) !== FALSE)
{
	$dados = array_combine( $nomesCampo, $linha );
	if ( !$dados['COD_LOCG'] )
	{
		continue;
	}
	if ( !array_key_exists( $dados['COD_REFERENCIA'], $dadosTotal ) )
	{
		$dadosTotal[$dados['COD_REFERENCIA']] = array();
	}
	array_push(
		$dadosTotal[$dados['COD_REFERENCIA']],
			$dados['COD_MOMENTO']
	);
}
fclose( $handle );

$sqlBase_insert = "
update elabrev.ppaacao_orcamento set tdecod = '%s' where acacodreferenciasof = '%s' and prgano = '2007'; 
";

foreach ( $dadosTotal as $codsof => $dados )
{
	$qtd = count( $dados );
	switch ( $qtd )
	{
		case 1:
			/*
			if ( $dados[0] == '1' )
			{
				continue 2;
			}
*/
			$sql = sprintf(
				$sqlBase_insert,
					$dados[0],
					$codsof
			);
			break;
		case 2:
			if ( !in_array( '1', $dados ) && !in_array( '5', $dados ) )
			{
				echo "\n--ERRO! acao possui 3 tipo de detalhamento";
				echo "\nrollback;";
				exit();
			}
			$sql = sprintf(
				$sqlBase_insert,
					'5',
					$codsof
			);
			break;
		default:
			echo "\n--ERRO! acao possui 3 tipo de detalhamento";
			echo "\nrollback;";
			exit();
	}
	echo $sql;
}


