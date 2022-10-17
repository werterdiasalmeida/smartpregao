<?php

//header( 'Content-Type: text/plain; charset=utf-8' );

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

error_reporting( E_ALL );

$db = new cls_banco();

function apagaAtividade( $atiid )
{
	global $db;
	$atiid = (integer) $atiid;
	// pega filhas
	$sql = "
		select
			atiid
		from pde.atividade
		where
			atiidpai = " . $atiid . "
	";
	$linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();
	$filhas = array();
	foreach ( $linhas as $linha )
	{
		array_push( $filhas, $linha["atiid"] );
	}
	foreach ( $filhas as $atiid_filha )
	{
		apagaAtividade( $atiid_filha );
	}
	
	// atualiza para nulo outra tabelas
	$sql = "
		update pde.anexoatividade
		set verid = null
		where atiid = " . $atiid . "
	";
	$db->executar( $sql );
	
	// apaga referencias
	$sql = "
		delete from pde.orcamentoatividade
		where atiid = " . $atiid . "
	";
	$db->executar( $sql );
	$sql = "
		delete from pde.versaoanexoatividade
		where aneid in (
			select aneid from pde.anexoatividade
			where atiid = " . $atiid . "
		)
	";
	$db->executar( $sql );
	$sql = "
		delete from pde.anexoatividade
		where atiid = " . $atiid . "
	";
	$db->executar( $sql );
	$sql = "
		delete from pde.observacaoatividade
		where atiid = " . $atiid . "
	";
	$db->executar( $sql );
	$sql = "
		delete from pde.notaatividade
		where atiid = " . $atiid . "
	";
	$db->executar( $sql );
	$sql = "
		delete from pde.usuarioresponsabilidade
		where atiid = " . $atiid . "
	";
	$db->executar( $sql );
	
	// delete atividade
	$sql = "
		delete from pde.atividade
		where atiid = " . $atiid . "
	";
	$db->executar( $sql );
	
}

function apagaFilhos( $atiid )
{

	global $db;
	$atiid = (integer) $atiid;
	// pega filhas
	$sql = "
		select
			atiid
		from pde.atividade
		where
			atiidpai = " . $atiid . "
	";
	$linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();
	$filhas = array();
	foreach ( $linhas as $linha )
	{
		array_push( $filhas, $linha["atiid"] );
	}
	foreach ( $filhas as $atiid_filha )
	{
		apagaAtividade( $atiid_filha );
	}
}



apagaFilhos( 57101 );
apagaAtividade( 57101 );

$db->commit();

//$db->rollback();

