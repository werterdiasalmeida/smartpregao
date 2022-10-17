<?php

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "/includes/planodetrabalho/pde/arquivos_requeridos.inc";

switch( @$_REQUEST['rsargs' ][0] ) {
	case 'inserirTarefaFilhas':
	{
		// insere o registro
		$atividade = new Atividade();
		$atividade->setNome( 'Nova Tarefa' );
		$atividade->setDescricao( 'Nova Descrição');
		if ( $_REQUEST['rsargs'][3] ) {
			$atividade->setContainerId( $_REQUEST['rsargs'][3] );
			$atividade->inserirTarefa();
		}
		AbstractEntity::updateAllChangedEntities();
		$db->commit();
		// exibe o novo registro na tabela
		$atividades = $atividade->getArraydeTarefasqueContenho();
		$atividades = orderArrayOfObjectsByMethod( $atividades , 'getCodigoUnico' );
		geraTabelaAtividades( $atividades, true );
		break;
	}
	case 'carregarTarefasFilhas':
	{
		$atividade = new Atividade();
		$atividade = $atividade->getTarefaPeloId( $_REQUEST[ 'rsargs' ][2] );
		$atividades = $atividade->getArraydeTarefasqueContenho();
		$atividades = orderArrayOfObjectsByMethod( $atividades , 'getCodigoUnico' );
		geraTabelaAtividades( $atividades, true );
		break;
	}
	case 'atualizarCampo':
	{
		$arrArgumentsRequest = (array) $_REQUEST[ 'rsargs' ];
		$strClassName		= loop_unxmlentities( @$arrArgumentsRequest[ 1 ] );
		$intIdObject		= loop_unxmlentities( @$arrArgumentsRequest[ 2 ] );
		$strMethod			= loop_unxmlentities( @$arrArgumentsRequest[ 3 ] );
		$strAttribute		= loop_unxmlentities( @$arrArgumentsRequest[ 4 ] );
		$strOriginalValue	= loop_unxmlentities( @$arrArgumentsRequest[ 5 ] );
		$strNewValue		= loop_unxmlentities( @$arrArgumentsRequest[ 6 ] );
		
		$objActiveFrozenField = new ActiveFrozenField();
		$objActiveFrozenField->setContainerClassName( $strClassName );
		$objActiveFrozenField->setAttributeName( $strAttribute );
		$objActiveFrozenField->setMethod( $strMethod );
		$objActiveFrozenField->setId( $intIdObject );
		$objActiveFrozenField->setOriginalValue( $strOriginalValue );
		$objActiveFrozenField->setNewValue( $strNewValue );
		$arrReturn = $objActiveFrozenField->apply();
		?>(<?= ( json_encode( $arrReturn ) ) ?>)<?
		break;
	}
	default:
	{
		print_r( $_REQUEST );
		?>
			Tipo de Requisicao Desconhecido
		<?
		break;
	}	
}

?>