<?php

define( "APP_PLANO_TRABALHO_ACAO" , APPRAIZ . "includes/planodetrabalho/tarefa_acao/" );
define( "APP_PLANO_TRABALHO" , APPRAIZ . "includes/planodetrabalho/tarefa_pt/" );

require_once( APP_PLANO_TRABALHO_ACAO . 'arquivos_requeridosAcao.inc' );

switch( @$_REQUEST[ 'rsargs' ][0] )
{
	case 'inserirTarefaFilhas':
	{
		require_once( APP_PLANO_TRABALHO_ACAO . "adiciona_tarefa_filhaAcao.inc" );
		require_once( APP_PLANO_TRABALHO_ACAO . "lista_tarefas_da_tarefaAcao.inc" );
		break;
	}	
	case 'carregarTarefasFilhas':
	{
		require_once( APP_PLANO_TRABALHO_ACAO . "lista_tarefas_da_tarefaAcao.inc" );
		break;
	}
	case 'atualizarCampo':
	{
		$acaoId = $_SESSION['acaid'];
		$objAcao = Acao::getAcaoPeloId( $acaoId );
		require_once( APP_PLANO_TRABALHO_ACAO . "../comuns/ActiveFrozenFieldServerSide.inc" );
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