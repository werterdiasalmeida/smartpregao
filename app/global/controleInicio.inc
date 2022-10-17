<?
/**
 * Rotina que controla o acesso рs pсginas do mѓdulo. Carrega as bibliotecas
 * padrѕes do sistema e executa tarefas de inicializaчуo.
 *
 * @author Cristiano Cabral (cristiano.cabral@gmail.com)
 * @since 25/08/2008
 */
date_default_timezone_set ('America/Sao_Paulo');

/**
 * Obtщm o tempo com precisуo de microsegundos. Essa informaчуo щ utilizada para
 * calcular o tempo de execuчуo da pсgina.
 *
 * @return float
 * @see /includes/rodape.inc
 */
function getmicrotime(){
	list( $usec, $sec ) = explode( ' ', microtime() );
	return (float) $usec + (float) $sec;
}

// obtщm o tempo inicial da execuчуo
$Tinicio = getmicrotime();

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as funчѕes gerais
include_once "config.inc";
include "verificasistema.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// abre conexуo com o servidor de banco de dados
$db      = new cls_banco();
// $dbSipia = new cls_banco_sipia();

//Verifica permissѕes de acesso ao mѓdulo requisitado via URL
include APPRAIZ . 'includes/testa_acesso.inc';