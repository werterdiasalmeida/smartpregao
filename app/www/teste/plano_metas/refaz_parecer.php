<?php
set_time_limit( 0 );

function msg( $msg )
{
	//return;
	echo $msg . "\n";
	flush();
	ob_flush();
}

header( "Content-Type: text/plain;" );

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
//include APPRAIZ . "www/cte/_funcoes.php";
///////////////////////////////////////////////
define( "SUBACAO_ASSISTENCIA_TECNICA", 8 );
define( "FUNCAO_PREFEITURA", 1 );
define( "FUNCAO_PREFEITO", 2 );

//Sistema
define( "CTE_SISTEMA", 13 );

//Produção

// Perfil
define( "CTE_PERFIL_EQUIPE_MUNICIPAL", 130 );
define( "CTE_PERFIL_EQUIPE_LOCAL", 124 );
define( "CTE_PERFIL_EQUIPE_LOCAL_APROVACAO", 128 );
define( "CTE_PERFIL_EQUIPE_TECNICA", 125 );
define( "CTE_PERFIL_CONSULTORES", 126 );
define( "CTE_PERFIL_ALTA_GESTAO", 129 );
define( "CTE_PERFIL_CONSULTA_GERAL", 127 );

define( "CTE_PERFIL_SUPER_USUARIO", 121 );
define( "CTE_PERFIL_ADMINISTRADOR", 147 );

// Estado Documento
define( "CTE_ESTADO_DIAGNOSTICO", 1 );
define( "CTE_ESTADO_PAR", 2 );
define( "CTE_ESTADO_ANALISE", 10 ); // assistência técnica
define( "CTE_ESTADO_ANALISE_FIN", 13 ); // assistência financeira
define( "CTE_ESTADO_FINALIZADO", 11 );
define( "CTE_ESTADO_PARECER", 14 );
define( "CTE_ESTADO_FNDE", 15 );

// Instrumentos
define( "INSTRUMENTO_DIAGNOSTICO_ESTADUAL", 1 );
define( "INSTRUMENTO_DIAGNOSTICO_MUNICIPAL", 2 );

define( "CTE_OBRA_CONSTRUCAO", 1 );
define( "CTE_OBRA_REFORMA", 2 );
define( "CTE_OBRA_AMPLIACAO", 3 );

// Formas de execucao

define( "FORMA_EXECUCAO_ASS_TEC", 4 );

function pegarPagina( $endereco, $mensagem ) {
		$snoopy = new Snoopy();
	$resultado = $snoopy->fetch( $endereco );
	if( $snoopy->status == "200" ) {
		return $snoopy->results;
	} else {
		throw new Exception( $mensagem );
	}
}

function cte_emitirParecer( $inuid ){
	global $db;
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {
		return false;
	}
	try {
		$muncod = $db->pegaUm( sprintf( "select muncod from cte.instrumentounidade where inuid = %d", $inuid ) );
		# pega os dados da prefeitura
		$sql = sprintf(
			"select ent.*, entend.*, ende.*, mnu.*, f.*
			from entidade.entidade ent
			inner join entidade.funcao f on f.funid = ent.funid 
			inner join entidade.entidadeendereco entend on ent.entid = entend.entid
			inner join entidade.endereco ende on entend.endid = ende.endid
			inner join territorios.municipio mnu on ende.muncod = mnu.muncod
			where mnu.muncod = '%s'
			and ent.funid = %d",
			$muncod,
			FUNCAO_PREFEITURA
		);
		$prefeitura = $db->pegaLinha( $sql );
		$prefeitura = $prefeitura ? $prefeitura : null;
		if ( !$prefeitura ) {
			throw new Exception( "Não foi possível encontrar os dados da prefeitura." );
		}
		# pega os dados do prefeito
		$sql = sprintf(
			"select entpref.*, entend.*, ende.*, mnu.*, f.*
			from entidade.entidade ent
			inner join entidade.funcao f on f.funid = ent.funid 
			inner join entidade.entidadeendereco entend on ent.entid = entend.entid
			inner join entidade.endereco ende on entend.endid = ende.endid
			inner join territorios.municipio mnu on ende.muncod = mnu.muncod
			inner join entidade.entidade entpref on ent.entid = entpref.entidassociado and entpref.funid = %d and entpref.entstatus = 'A'
			where mnu.muncod = '%s'
			and ent.funid = %d",
			FUNCAO_PREFEITO,
			$muncod,
			FUNCAO_PREFEITURA
		);
		$prefeito = $db->pegaLinha( $sql );
		$prefeito = $prefeito ? $prefeito : null;
		if ( !$prefeito ) {
			throw new Exception( "Não foi possível encontrar os dados do prefeito." );
		}
		# cadastra o novo termo
		cte_excluirParecer( $inuid );
		$sql = sprintf( "insert into cte.parecer ( inuid, pardocumento ) values ( %d, '' ) returning parid", $inuid );
		$parid = $db->pegaUm( $sql );
		if ( !$parid ) {
			throw new Exception( "Ocorreu um erro ao cadastrar o novo Termo de Compromisso." );
		}
		# pega as ações
		$sql = sprintf(
			"select ai.aciid, i.indid, ad.ardid, d.dimid
			from cte.pontuacao p
			inner join cte.acaoindicador ai on ai.ptoid = p.ptoid
			inner join cte.indicador i on i.indid = p.indid and i.indstatus = 'A'
			inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
			inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
			where p.inuid = %d",
			$inuid
		);
		$acoes = $db->carregarColuna( $sql, "aciid" );
		$acoes = $acoes ? array_unique( $acoes ) : array();
		$indicadores = $db->carregarColuna( $sql, "indid" );
		$indicadores = $indicadores ? array_unique( $indicadores ) : array();
		$areas = $db->carregarColuna( $sql, "ardid" );
		$areas = $areas ? array_unique( $areas ) : array();
		$dimensoes = $db->carregarColuna( $sql, "dimid" );
		$dimensoes = $dimensoes ? array_unique( $dimensoes ) : array();
		ob_start();
		include APPRAIZ . "www/teste/plano_metas/parecer.php";
		$parecer = $db->escape( str_replace( "#PARECER#", sprintf( "%05d", $parid ), ob_get_clean() ) );
		$sql = sprintf( "update cte.parecer set pardocumento = %s where parid = %d", $parecer, $parid );
		
		if ( !$db->executar( $sql, false ) ) {
			throw new Exception( "Ocorreu um erro ao cadastrar o novo Termo de Compromisso." );
		}
		# relaciona as ações ao novo termo
		foreach ( $acoes as $acao ) {
			$sql = sprintf(
				"insert into cte.pareceracaoindicador ( parid, aciid ) values ( %d, '%s' )",
				$parid,
				$acao
			);
			if ( !$db->executar( $sql ) ) {
				throw new Exception( "Ocorreu um erro ao cadastrar o novo Termo de Compromisso." );
			}
		}
		return $parid;
	} catch ( Exception $erro ) {
		wf_registrarMensagem( $erro->getMessage() );
		$db->rollback();
		return false;
	}
}

function cte_excluirDocumentos( $inuid ){
	if ( cte_pegarItrid( $inuid ) != INSTRUMENTO_DIAGNOSTICO_MUNICIPAL )
	{
		return true;
	}
	$parecer = cte_excluirParecer( $inuid );
	$termo = cte_excluirTermo( $inuid );
	return $parecer && $termo;
}

function cte_pegarItrid( $inuid )
{
	global $db;
	$inuid = (integer) $inuid;
	$sql = "
		select
			itrid
		from cte.instrumentounidade
		where
			inuid = " . $inuid . "
	";
	return (integer) $db->pegaUm( $sql );
}

function cte_excluirParecer( $inuid ){
	global $db;
	try {
		$sql = sprintf(
			"delete from cte.pareceracaoindicador where parid in ( select parid from cte.termo where inuid = %d )",
			$inuid
		);
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "Ocorreu um erro ao excluir relação das subações com o Parecer antigo." );
		}
		$sql = sprintf(
			"delete from cte.parecer where inuid = %d",
			$inuid
		);
		if ( !$db->executar( $sql ) ) {
			throw new Exception( "Ocorreu um erro ao excluir o Parecer antigo." );
		}
		return true;
	} catch ( Exception $erro ) {
		$db->rollback();
		return false;
	}
}


///////////////////////////////////////////////

error_reporting( E_ALL );

$db = new cls_banco();

$sql = "
	select
		p.inuid,
		m.estuf,
		m.mundescricao
	from cte.parecer p
		inner join cte.instrumentounidade i on i.inuid = p.inuid
		inner join territorios.municipio m on m.muncod = i.muncod
	order by
		m.mundescricao,
		m.estuf
";
$pareceres = $db->carregar( $sql );
$pareceres = $pareceres ? $pareceres : array();

while ( ob_get_level() > 1 )
{
	ob_end_flush();
}

msg( "Atualização de parecer iniciada\n" );

msg( "Removendo pareceres atuais ...\n" );

$sql = "delete from cte.pareceracaoindicador";
$db->executar( $sql );

$sql = "delete from cte.parecer";

$db->executar( $sql );

foreach ( $pareceres as $parecer )
{
	msg( $parecer["mundescricao"] . "/" . $parecer["estuf"] . " ..." );
	cte_emitirParecer( $parecer["inuid"] );
}

//$db->rollback();
$db->commit();
msg( "\nAtualização de parecer finalizada!" );
