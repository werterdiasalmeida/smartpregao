<?php
$acaoId = $_SESSION['acaid'];
$intTarefaId = @$_REQUEST[ 'rsargs' ][ 2 ];
$objTarefa = new TarefaAcao();
$objTarefa->setAcaoId( $acaoId );
$objTarefa = $objTarefa->getTarefaPeloId( $intTarefaId );
$arrTarefasQueContenho = $objTarefa->getArraydeTarefasqueContenho();
if ( sizeof( $arrTarefasQueContenho) > 0 ) 
{
	geraTabelaTarefas( $arrTarefasQueContenho , true );
}
?>
