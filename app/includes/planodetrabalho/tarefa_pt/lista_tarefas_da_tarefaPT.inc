<?php
$intTarefaId = @$_REQUEST[ 'rsargs' ][ 2 ];
$objTarefa = new TarefaPT();
$objTarefa = $objTarefa->getTarefaPeloId( $intTarefaId );
$arrTarefasQueContenho = $objTarefa->getArraydeTarefasqueContenho();
if ( sizeof( $arrTarefasQueContenho) > 0 ) 
{
	geraTabelaTarefas( $arrTarefasQueContenho , true );
}
?>
