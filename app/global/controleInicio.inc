<?
/**
 * Rotina que controla o acesso �s p�ginas do m�dulo. Carrega as bibliotecas
 * padr�es do sistema e executa tarefas de inicializa��o.
 *
 * @author Cristiano Cabral (cristiano.cabral@gmail.com)
 * @since 25/08/2008
 */
date_default_timezone_set ('America/Sao_Paulo');

/**
 * Obt�m o tempo com precis�o de microsegundos. Essa informa��o � utilizada para
 * calcular o tempo de execu��o da p�gina.
 *
 * @return float
 * @see /includes/rodape.inc
 */
function getmicrotime(){
	list( $usec, $sec ) = explode( ' ', microtime() );
	return (float) $usec + (float) $sec;
}

// obt�m o tempo inicial da execu��o
$Tinicio = getmicrotime();

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as fun��es gerais
include_once "config.inc";
include "verificasistema.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// abre conex�o com o servidor de banco de dados
$db      = new cls_banco();
// $dbSipia = new cls_banco_sipia();

//Verifica permiss�es de acesso ao m�dulo requisitado via URL
include APPRAIZ . 'includes/testa_acesso.inc';