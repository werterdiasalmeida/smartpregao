<?php

define( "HISTORICO_TABELA_SUBACAO", 1 );

function his_inserir( $hitid, $hisvalorchave, array $novosDados )
{
	global $db;
	$hitid = (integer) $hitid;
	$novosDados = array_map( "strval", $novosDados );
	$usucpf = $_SESSION['usucpf'];
	if ( !his_tabelaExiste( $hitid ) )
	{
		return;
	}
	$sql = "
		insert into historico.historico
		( hitid, hisvalorchave, hisdata, usucpf )
		values
		( " . $hitid . ", '" . $hisvalorchave . "', now(), '" . $usucpf . "' )
		returning hisid
	";
	$hisid = (integer) $db->pegaUm( $sql );
	if ( !$hisid )
	{
		return false;
	}
	$atuais = his_pegarDadosAtuais( $hitid, $hisvalorchave );
	foreach ( $novosDados as $campo => $novoValor )
	{
		if ( !array_key_exists( $campo, $atuais ) || $novoValor != $atuais[$campo] )
		{
			$sql = "
				insert into historico.alteracao
				( hisid, altcampo, altvalor )
				values
				( " . $hisid . ", '" . $campo . "', '" . $novoValor . "' )
			";
			if ( !$db->executar( $sql ) )
			{
				return false;
			}
		}
	}
	return true;
}

function his_pegarDadosAtuais( $hitid, $hisvalorchave )
{
	global $db;
	$hitid = (integer) $hitid;
	$sql = "
		select
			a.altcampo,
			a.altvalor
		from
		(
			select
				a.altcampo,
				max( h.hisdata ) as hisdata
			from historico.tabela t
				inner join historico.historico h on h.hitid = t.hitid
				inner join historico.alteracao a on a.hisid = h.hisid
			where
				t.hitid = " . $hitid . "
			group by
				a.altcampo
		) as fake
			inner join historico.alteracao a on
				a.altcampo = fake.altcampo
			inner join historico.historico h on
				h.hisid = a.hisid and
				h.hisdata = fake.hisdata
		where
			h.hitid = " . $hitid . "
	";
	$dados = $db->carregar( $sql );
	$dados = $dados ? $dados : array();
	$atuais = array();
	foreach ( $dados as $linha )
	{
		$atuais[$linha['altcampo']] = $linha['altvalor'];
	}
	return $atuais;
}

function his_tabelaExiste( $hitid )
{
	global $db;
	$hitid = (integer) $hitid;
	$existe = array();
	if ( !array_key_exists( $hitid, $existe ) )
	{
		$sql = "
			select
				count(*)
			from historico.tabela
			where
				hitid = " . $hitid . "
		";
		$existe[$hitid] = $db->pegaUm( $sql ) > 0;
	}
	return $existe[$hitid];
}

?>