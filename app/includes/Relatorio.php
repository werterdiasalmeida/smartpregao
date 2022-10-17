<?php

class Relatorio extends Modelo {

	function gravarParametrosBanco( $stTituloRelatorio ){
		global $db;
		$sql = sprintf( "SELECT prtid FROM public.parametros_tela WHERE prtdsc = '%s'", $stTituloRelatorio );
		if ( $prtid = $db->pegaUm( $sql ) ) {
			$sql = sprintf( "UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			$_SESSION['usucpf'],
			$_SESSION['mnuid'],
			$prtid );
		}
		else
		{
			$sql = sprintf( "INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
			$stTituloRelatorio,
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			'FALSE',
			$_SESSION['usucpf'],
			$_SESSION['mnuid'] );
		}
		$db->executar( $sql );
		return $db->commit();

	}
	
	public function carregarParametrosBanco( $prtid ){
		global $db;
		$sql = "select prtobj from public.parametros_tela where prtid = " . $prtid;
		$itens = $db->pegaUm( $sql );
		return unserialize( stripslashes( stripslashes( $itens ) ) );
	}
	
	public function montaListaParametrosBanco( $mnuid = null, $usucpf = null ){
		// Se não passar valor por parametro recupera da asessão
		$mnuid  = ( empty( $mnuid ) )  ? $_SESSION['mnuid']  : $mnuid;
		$usucpf = ( empty( $usucpf ) ) ? $_SESSION['usucpf'] : $usucpf;
		
		$sql = "SELECT Case when prtpublico = false then '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir agenda \" onclick=\"excluir_relatorio(' || prtid || ');\">' else '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar agenda \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir agenda \" onclick=\"excluir_relatorio(' || prtid || ');\">' end as acao, '<a href=\"javascript: carregar_consulta(' || prtid || ')\">' || prtdsc || '</a>' as descricao FROM public.parametros_tela 
				WHERE mnuid = " . $mnuid . " AND usucpf = '" . $usucpf . "'";
		
		$cabecalho = array( 'Ação', 'Nome' );
		$db->monta_lista_simples( $sql, null, 50, 50, null, null, null );
	}

	public function excluirParametroBanco( $prtid ){
		global $db;
		$sql = 'DELETE from public.parametros_tela WHERE prtid = ' . $prtid;
		$db->executar( $sql );
		return $db->commit();
	}

	public function parametroBancoTornarPublico( $prtid ){
		global $db;
		$sql = "UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = " . $prtid;
		$db->executar( $sql );
		$db->commit();
	}
}