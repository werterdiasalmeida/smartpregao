<?php
require_once "config.inc";

$nome_bd     = 'simec_espelho_producao';
$servidor_bd = 'simec-d';
$porta_bd    = '5432';
$usuario_db  = 'seguranca';
$senha_bd    = 'phpseguranca';

include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$xml = simplexml_load_file( APPRAIZ . "arquivos/SIGPLAN/importacao/sigplan-2007-20070516.xml" );

header( 'Content-Type: text/plain;' );

if ( $xml->ArrayOfAcao->Acao )
{
	$sql_base_atualizacao = <<<EOT

update monitora.acao
set procod = '%s', unmcod = '%s'
where
	prgano    = '%s' and
	prgcod    = '%s' and
	acacod    = '%s' and
	loccod    = '%s' and
	unicod    = '%s' and
	unitpocod = '%s' and
	acasnrap  =  %s;

EOT;
	
	$sql_base_select = <<<EOT

select procod, unmcod
from monitora.acao
where
	prgano    = '%s' and
	prgcod    = '%s' and
	acacod    = '%s' and
	loccod    = '%s' and
	unicod    = '%s' and
	unitpocod = '%s' and
	acasnrap  =  %s;

EOT;
/*
				print <<<EOT
acao	programa	unidade		localizador	rap	|	produto				unidade medida
-----------------------------------------------------------------------------------------------------------------------------

EOT;
*/
	$alteracoes = array();
	foreach ( $xml->ArrayOfAcao->Acao as $acao )
	{
		// identificação
		$prgano    = (string) $acao->PRGAno;
		$prgcod    = (string) $acao->PRGCod;
		$acacod    = (string) $acao->ACACod;
		$loccod    = (string) $acao->LOCCod;
		$unicod    = (string) $acao->UNICod;
		$unitpocod = (string) $acao->UNITPOCod;
		$acasnrap  = (string) $acao->ACASNRap; // string "true" ou "false"
		
		// novos valores
		$procod = $acao->PROCod;
		$unmcod = $acao->UNMCod;
		
		$chave = $procod . '.' . $unmcod;
		if ( !array_key_exists( $chave, $alteracoes ) )
		{
			$alteracoes[$chave] = array();
		}
		
		$alteracao = array(
			'prgano'    => $prgano,
			'prgcod'    => $prgcod,
			'acacod'    => $acacod,
			'loccod'    => $loccod,
			'unicod'    => $unicod,
			'unitpocod' => $unitpocod,
			'acasnrap'  => $acasnrap,
		);
		array_push( $alteracoes[$chave], $alteracao );
		/*
		print sprintf(
			$sql_base_atualizacao,
				
				$procod,
				$unmcod,
				
				$prgano,
				$prgcod,
				$acacod,
				$loccod,
				$unicod,
				$unitpocod,
				$acasnrap
		);
		*/
		/*
		// verifica se item está diferente da base
		$sql = sprintf(
			$sql_base_select,
				$prgano,
				$prgcod,
				$acacod,
				$loccod,
				$unicod,
				$unitpocod,
				$acasnrap
		);
		$registro = $db->recuperar( $sql );
		if ( $registro['procod'] != $procod || $registro['unmcod'] != $unmcod )
		{
			print "$acacod\t$prgcod\t\t$unicod ($unitpocod)\t$loccod\t\t$acasnrap\t|";
			if ( $registro['procod'] != $procod )
			{
				print "\t" . sprintf( "%4s", $registro['procod'] ) . "    >    $procod\t\t"; 
			}
			else
			{
				print "\t\t\t\t\t";
			}
			if ( $registro['unmcod'] != $unmcod )
			{
				print sprintf( "%4s", $registro['unmcod'] ) . "    >    $unmcod"; 
			}
			print "\n";
		}
		*/
		
	}
	//dbg( $alteracoes );
	
	foreach ( $alteracoes as $chave => $alteracoes_item )
	{
		$dados = explode( '.', $chave );
		$procod = $dados[0];
		$unmcod = $dados[1];
		$sql = <<<EOT
update monitora.acao
set procod = '$procod', unmcod = '$unmcod'
where 
EOT;
		$where = array();
		foreach ( $alteracoes_item as $alteracao )
		{
			$condicao = <<<EOT
(
	prgano    = '{$alteracao['prgano']}' and
	prgcod    = '{$alteracao['prgcod']}' and
	acacod    = '{$alteracao['acacod']}' and
	loccod    = '{$alteracao['loccod']}' and
	unicod    = '{$alteracao['unicod']}' and
	unitpocod = '{$alteracao['unitpocod']}' and
	acasnrap  =  {$alteracao['acasnrap']}
)
EOT;
			array_push( $where, $condicao );
		}
		$sql .= implode( ' or ', $where ) . ";\n\n";
		print $sql;
	}
	
}









