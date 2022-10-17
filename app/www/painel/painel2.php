<?php
/**
 * Rotina que controla o acesso �s p�ginas do m�dulo. Carrega as bibliotecas
 * padr�es do sistema e executa tarefas de inicializa��o. 
 *
 * @author Cristiano Cabral (cristiano.cabral@gmail.com)
 * @since 25/08/2008
 */
$_SERVER['REQUEST_URI'] = $bodytag = str_replace("painel2.php", "painel.php", $_SERVER['REQUEST_URI']);
date_default_timezone_set ('America/Sao_Paulo');
$cabecalho_painel=true;
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
include_once APPRAIZ . "includes/classes/Modelo.class.inc";


// carrega as fun��es espec�ficas do m�dulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_funcoesseriehistorica.php';
include_once '_funcoesagendamentoindicador.php';
include_once '_componentes.php';


// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

// carrega a p�gina solicitada pelo usu�rio
$sql = sprintf( "select u.usuchaveativacao from seguranca.usuario u where u.usucpf = '%s'", $_SESSION['usucpf'] );
$chave = $db->pegaUm( $sql );
if ( $chave == 'f' ) {
	// leva o usu�rio para o formul�rio de troca de senha
	include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/sistema/usuario/altsenha.inc";
	include APPRAIZ . "includes/rodape.inc";
} else if ( $_REQUEST['modulo'] ) {
	// leva o usu�rio para a p�gina solicitada
	
	include APPRAIZ . 'includes/testa_acesso.inc';
	$_SERVER['REQUEST_URI'] = $bodytag = str_replace("painel.php", "painel2.php", $_SERVER['REQUEST_URI']);
	include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc";
	if(!$_REQUEST["AJAX"]){
	include APPRAIZ . "includes/rodape.inc";
	}
} else {
	// leva o usu�rio para o formul�rio de login
	header( "Location: login.php" );
}
?>