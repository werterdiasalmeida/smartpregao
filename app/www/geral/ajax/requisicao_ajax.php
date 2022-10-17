<?php

set_time_limit( 0 );

//var_dump($_REQUEST);

if( !isset( $_REQUEST[ 'rsargs' ] ) )
{
//	$_REQUEST = Array( 'rsargs' => Array( 'atualizarCampo' , 'TarefaPT' , '1030' , 'insert' , 'container' , 35 ) );
}

include "config.inc";
include APPRAIZ."includes/funcoes.inc";
include APPRAIZ."includes/classes_simec.inc";

global $db;
$db = new cls_banco();
try
{
	switch( @$_REQUEST['rsargs'][1] )
	{
		case 'Atividade':
		{
			include 'requisicao_ajax_pde.php';
			break;
		}
		case 'TarefaPT':
		{
			include 'requisicao_ajax_pt.php';
			break;
		}
		case 'TarefaAcao':
		{
			include 'requisicao_ajax_acao.php';
			break;
		}
		default:
		{
			?>
				Acesso Ajax nao Permitido a Esta Classe <?= @$_REQUEST['rsargs'][1] ?>
				<?= print_r( $_REQUEST ) ?>
			<?php
			exit();
			break;
		}
	}
}
catch( Exception $objError )
{
	$arrError = array();
	$arrError[ 'className' ]		= get_class( $objError );
	$arrError[ 'message' ]			= xmlentities( $objError->getMessage() );
	$arrError[ 'code' ] 			= $objError->getCode();
	$arrReturn = $arrError;
	?>
	(<?= ( json_encode( $arrReturn ) ) ?>)
	<?
}
$db->commit();
?>