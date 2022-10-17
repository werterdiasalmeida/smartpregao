<?php

define( "APP_PLANO_TRABALHO" , APPRAIZ . "includes/planodetrabalho/tarefa_pt/" );
require_once( APP_PLANO_TRABALHO . 'arquivos_requeridosPT.inc' );

switch( @$_REQUEST[ 'rsargs' ][0] )
{
	case 'inserirTarefaFilhas':
	{
		require_once( APP_PLANO_TRABALHO . "adiciona_tarefa_filhaPT.inc" );
		require_once( APP_PLANO_TRABALHO . "lista_tarefas_da_tarefaPT.inc" );
		break;
	}	
	case 'carregarTarefasFilhas':
	{
		require_once( APP_PLANO_TRABALHO . "lista_tarefas_da_tarefaPT.inc" );
		break;
	}
	case 'atualizarCampo':
	{
		$intPjeId = $_SESSION['pjeid'];
		$objProjeto =  ProjetoPT::getProjetoPeloId( $intPjeId );
		if( $objProjeto->getProjetoAberto() )
		{
			//dbg( 'no if');
			require_once( APP_PLANO_TRABALHO . "../comuns/ActiveFrozenFieldServerSide.inc" );
		}
		else
		{
			throw new Exception( 'O projeto é fechado' );
		}
		break;
	}
	default:
	{
		print_r( $_REQUEST );
		?>
			Tipo de Requisicao Desconhecido
		<?
	}
}
$db->commit();
?>