<?php

header( 'Content-Type: text/plain;' );
set_time_limit( 0 );
ini_set( 'display_errors', E_ALL );

// carrega as bibliotecas
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

function msg( $msg )
{
	echo $msg . "\n";
}

function erro( $erro )
{
	global $db;
	msg( 'erro! ' . $erro );
	msg( 'operaчуo abortada' );
	$db->rollback();
	exit();
}

function importarPara( $dados, $para = 'normal' )
{
	global $db;
	$para = strtoupper( $para );
	if ( $para == 'RAP' )
	{
		$label = 'RAP';
		$acasnrap = 't';
	}
	else
	{
		$label = '';
		$acasnrap = 'f';
	}
	msg( 'iniciando importacao de dados financeiros ' . $label );
	$quantidade_acoes = 0;
	foreach( $dados as $registro )
	{
		// verifica se aчуo existe
		$registro = (array) $registro;
		$sql = sprintf(
			"select acaid, loccod from monitora.acao where prgcod = '%s' and acacod = '%s' and prgano = '%s' and saccod = '%s' and acasnrap = '%s'",
			$registro['PRGCod'],
			$registro['ACACod'],
			$registro['PRGAno'],
			$registro['SACCod'],
			$acasnrap
		);
		$dados_acao = $db->pegaLinha( $sql );
		$registro['acaid'] = (integer) $dados_acao['acaid'];
		$registro['loccod'] = $dados_acao['loccod'];
		if ( $registro['acaid'] == 0 )
		{
			continue;
		}
		//msg( 'importando acao ' . $registro['acaid'] );
		
		// remove o registro para evitar duplicaчуo
		$sql = sprintf(
			"delete from monitora.dadofinanceiro where acaid = %d and prgano = '%s'",
			$registro['acaid'],
			$registro['PRGAno']
		);
		if ( !$db->executar( $sql ) )
		{
			erro(
				'falha ao excluir dados financeiros da acao ' .
				$registro['PRGCod'] . '.' .
				$registro['ACACod'] . '.' .
				$registro['loccod'] . '-' .
				$registro['PRGAno']
			);
		}
		
		// insere o registro novo
		$sql = sprintf(
			"insert into monitora.dadofinanceiro ( %s ) values ( '%s' )",
			implode( ',', array_keys( $registro ) ),
			implode( "','", array_values( $registro ) )
		);
		if ( !$db->executar( $sql ) )
		{
			erro(
				'falha ao inserir dados financeiros da acao ' .
				$registro['PRGCod'] . '.' .
				$registro['ACACod'] . '.' .
				$registro['loccod'] . '-' .
				$registro['PRGAno']
			);
		}
		$quantidade_acoes++;
	}
	msg( 'importacao ' . $label . ' efetuada com sucesso' );
	msg( 'foram importados dados financeiros ' . $label . ' de ' . $quantidade_acoes . ' acoes' );
}

$db = new cls_banco();

// carrega arquivo a ser importado
$arquivos = glob( APPRAIZ . 'arquivos/SIGPLAN/importacao/sigplan-*.xml' );
arsort( $arquivos );
$arquivo = (string) array_shift( $arquivos );
if ( $arquivo == '' )
{
	erro( 'nуo existe arquivo a ser importado' );
}

// verifica se arquivo xml щ vсlido 
$xml = simplexml_load_file( $arquivo );
if ( !$xml )
{
	erro( 'arquivo invсlido ' . $arquivo );
}

$_SESSION['usucpforigem'] = '00000000191';

// realiza importaчуo de dados normais e RAP
msg( 'iniciando importacao de dados financeiros do arquivo ' . $arquivo );
// normal
$dados = (array) $xml->ArrayOfDadoFinanceiro;
importarPara( $dados['DadoFinanceiro'], 'normal' );
// RAP
$dados = (array) $xml->ArrayOfDadoFinanceiroRAP;
importarPara( $dados['DadoFinanceiroRAP'], 'RAP' );
// confirma ediчуo 
$db->commit();
?>