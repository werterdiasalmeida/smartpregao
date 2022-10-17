<?php

/*
require_once( APP_PLANO_TRABALHO . 'AbstractEntity.class.inc' );
require_once( APP_PLANO_TRABALHO . 'ActiveFrozenField.class.inc' );
require_once( APP_PLANO_TRABALHO . 'ActiveFrozenFieldEntity.class.inc' );
require_once( APP_PLANO_TRABALHO . 'library.inc' );
*/
/*
	$objPeople = new PeopleEntity();
	$objPeople = $objPeople->getInstanceById( 2 );
	$_REQUEST[ 'rsargs' ] = array( get_class( $objPeople ) , $objPeople->getId() , "objSex" , $objPeople->objSex, 'xmale' );
*/
/** /
	$objTarefa =  new Tarefa();
	$objTarefa = $objTarefa->getInstanceById( 78 );
	$_REQUEST[ 'rsargs' ] = 
	array
	( 
		get_class( $objTarefa ) , 
		$objTarefa->getId() , 
		"boolDataFechada" ,	
		xmlentities( ( $objTarefa->getDataFechada() ? '1' : '0' ) ) , 
		'0' 
	);
/**/	
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
?>
(<?= ( json_encode( $arrReturn ) ) ?>)