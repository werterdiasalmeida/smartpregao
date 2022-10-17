<?php
/**
 * Rotina que controla o acesso рs pсginas do mѓdulo. Carrega as bibliotecas
 * padrѕes do sistema e executa tarefas de inicializaчуo. 
 *
 * @author Cristiano Cabral (cristiano.cabral@gmail.com)
 * @since 25/08/2008
 */
$_SERVER['REQUEST_URI'] = $bodytag = str_replace("painel2.php", "painel.php", $_SERVER['REQUEST_URI']);
date_default_timezone_set ('America/Sao_Paulo');
$cabecalho_painel=true;
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
include_once APPRAIZ . "includes/classes/Modelo.class.inc";


// carrega as funчѕes especэficas do mѓdulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_funcoesseriehistorica.php';
include_once '_funcoesagendamentoindicador.php';
include_once '_componentes.php';


// abre conexуo com o servidor de banco de dados
$db = new cls_banco();

// carrega a pсgina solicitada pelo usuсrio
$sql = sprintf( "select u.usuchaveativacao from seguranca.usuario u where u.usucpf = '%s'", $_SESSION['usucpf'] );
$chave = $db->pegaUm( $sql );
if ( $chave == 'f' ) {
	// leva o usuсrio para o formulсrio de troca de senha
	include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/sistema/usuario/altsenha.inc";
	include APPRAIZ . "includes/rodape.inc";
} else if ( $_REQUEST['modulo'] ) {
	// leva o usuсrio para a pсgina solicitada
	
	include APPRAIZ . 'includes/testa_acesso.inc';
	$_SERVER['REQUEST_URI'] = $bodytag = str_replace("painel.php", "painel2.php", $_SERVER['REQUEST_URI']);
	include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc";
	if(!$_REQUEST["AJAX"]){
	include APPRAIZ . "includes/rodape.inc";
	}
} else {
	// leva o usuсrio para o formulсrio de login
	header( "Location: login.php" );
}
?>