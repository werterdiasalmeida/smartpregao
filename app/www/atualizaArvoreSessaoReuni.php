<?php
require_once( "config.inc" );
require_once( APPRAIZ . "includes/classes_simec.inc" );
require_once( APPRAIZ . "includes/funcoes.inc" );

$strEvento = $_REQUEST[ "rs"] ;
$arrArgumentos = $_REQUEST[ "rsargs"] ;



if( !$_SESSION[ 'ArvoreSessaoReuni' ] )
{
	$_SESSION[ 'ArvoreSessaoReuni' ] = array();
}
function mostraNo( $strIdNo )
{
	$_SESSION[ 'ArvoreSessaoReuni' ][ $strIdNo ] = 1;
}

function fechaNo( $strIdNo )
{
	$_SESSION[ 'ArvoreSessaoReuni' ][ $strIdNo ] = 0;	
}
function mudaImg( $strIdNo , $strSrc )
{
	$_SESSION[ 'ArvoreSessaoReuniSrc' ][ $strIdNo ] = $strSrc;	
}

switch( $strEvento )
{
	case 'mostraNo':
		{
			if($arrArgumentos){
				foreach( $arrArgumentos as $strIdNo )
				{
					mostraNo( $strIdNo );
				}
			}
			
			break;
		}
	case 'fechaNo':
		{
			if($arrArgumentos){
				foreach( $arrArgumentos as $strIdNo )
				{
					fechaNo( $strIdNo );
				}
			}
			break;
		}
	case 'mudaImg':
		{
			$strIdNo = @$arrArgumentos[ 0 ];
			$strSrc =  @$arrArgumentos[ 1 ];
			mudaImg( $strIdNo , $strSrc );
			break;
		}
}

print_r( $_SESSION[ "ArvoreSessaoReuni"] );
print_r( $_SESSION[ "ArvoreSessaoReuniSrc"] );

?>