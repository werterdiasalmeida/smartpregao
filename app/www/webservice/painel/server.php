<?php
// Pull in the NuSOAP code
require_once('nusoap.php');

//$_REQUEST['baselogin'] = "simec_espelho_producao";

// Connects to basedata in simec
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "www/painel/_constantes.php";
include_once APPRAIZ . "www/painel/_funcoes.php";
include_once APPRAIZ . "www/painel/_funcoesagendamentoindicador.php";

define("SISID_PAINEL", 48);

/* configuraчѕes do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configuraчѕes - Memoria limite de 1024 Mbytes */


// 	abre conexуo com o servidor de banco de dados
$db = new cls_banco();


// Create the server instance
$server = new soapservidor();

// Initialize WSDL support
$server->configureWSDL('painelwsdl', 'urn:painelwsdl');


// Register the method to expose
$server->register('autenticarUsuario',                		// method name
    array('cpf' => 'xsd:string', 'senha' => 'xsd:string'),  // input parameters
    array('return' => 'xsd:string'),      					// output parameters
    'urn:autenticarUsuariowsdl',                      		// namespace
    'urn:autenticarUsuariowsdl#autenticarUsuario',          // soapaction
    'rpc',                                					// style
    'encoded',                            					// use
    'Autentica usuсrio no simec'            				// documentation
);

// Register the method to expose
$server->register('pegarFormatoIndicadorCSV',               	// method name
    array('PHPSESSID'=>'xsd:string','indid' => 'xsd:integer'),  // input parameters
    array('return' => 'xsd:string'),      						// output parameters
    'urn:autenticarUsuariowsdl',                      			// namespace
    'urn:autenticarUsuariowsdl#autenticarUsuario',          	// soapaction
    'rpc',                                						// style
    'encoded',                            						// use
    'Pegar formato de carga do indicador'      					// documentation
);


$server->wsdl->addComplexType(
        'csvs',
        'complexType',
        'array',
        '',
        'SOAP-ENC:Array',
        array(),
        array('csvs' => array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'xsd:string'))
);

$server->wsdl->addComplexType(
    'detalhesserihistorica',
    'complexType',
    'struct',
    'all',
    '',
    array(
        'agddataprocessamento' => array('name' => 'agddataprocessamento', 'type' => 'xsd:string'),
        'indid'                => array('name' => 'indid', 'type' => 'xsd:int'),
        'csvarray'             => array('name' => 'csvarray', 'type' => 'tns:csvs')
    )
);

// Register the method to expose
$server->register('inserirAgendamentoSerieHistorica',                // method name
    array('PHPSESSID' => 'xsd:string', 'dadosdetalhesserihistorica' => 'tns:detalhesserihistorica'),        // input parameters
    array('return' => 'xsd:string'),      // output parameters
    'urn:inserirAgendamentoSerieHistoricawsdl',                      // namespace
    'urn:inserirAgendamentoSerieHistoricawsdl#inserirAgendamentoSerieHistorica',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Inserir dados em serie historica'            // documentation
);

// Register the method to expose
$server->register('enviarArquivoSerieHistorica',                // method name
    array('PHPSESSID' => 'xsd:string', 'dadosdetalhesserihistorica' => 'tns:detalhesserihistorica'),        // input parameters
    array('return' => 'xsd:string'),      // output parameters
    'urn:enviarArquivoSerieHistoricawsdl',                      // namespace
    'urn:enviarArquivoSerieHistoricawsdl#enviarArquivoSerieHistorica',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Inserir dados em serie historica'            // documentation
);

// Register the method to expose
$server->register('acompanharLogArquivoSerieHistorica',                // method name
    array('PHPSESSID' => 'xsd:string', 'wbsid' => 'xsd:integer'),        // input parameters
    array('return' => 'xsd:string'),      // output parameters
    'urn:acompanharLogArquivoSerieHistorica',                      // namespace
    'urn:acompanharLogArquivoSerieHistoricawsdl#acompanharLogArquivoSerieHistorica',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Acompanhae o log do arquivo de serie historica'            // documentation
);

// Autenticar o usuсrio
function autenticarUsuario($login, $senha) {
	global $db;
	
	$sql = "SELECT * FROM seguranca.usuario WHERE usucpf='".$login."'";
	$usr = $db->pegaLinha($sql);
	
	if($senha == md5_decrypt_senha( $usr['ususenha'], '' )) {
		session_start();
		$_SESSION['usucpf'] = $usr['usucpf'];
		$_SESSION['sisid']  = SISID_PAINEL;
		return session_id();
	} else {
		return false;
	}
}

function pegarFormatoIndicadorCSV($PHPSESSID, $indid) {
	global $db;
	
	if (isset($PHPSESSID)) {
		session_id($PHPSESSID);
	}
	session_start();
	
	if($_SESSION['usucpf']) {
		$_SESSION['indid'] = $indid;
		$permissoes = verificaPerfilPainel();

		if($permissoes['verindicadores'] == 'vertodos') {
			$validacao = true;	
		} else {
			$validacao = validaAcessoIndicadores($permissoes['verindicadores'], $_SESSION['indid']);
		}

		if($validacao) {
			$manual = verdicionariocargawebservice(array());
			return $manual;		
		} else {
			return "Usuсrio nуo possui permissуo";
		}
	} else {
		return "Erro de autenticaчуo";
	}
}


function inserirAgendamentoSerieHistorica($PHPSESSID, $dadosdetalhesserihistorica) {
	
	if (isset($PHPSESSID)) {
		session_id($PHPSESSID);
	}
	session_start();
	
	if($_SESSION['usucpf']) {
		$_SESSION['usucpforigem']=$_SESSION['usucpf'];
		$_SESSION['indid'] = $dadosdetalhesserihistorica['indid'];
		$permissoes = verificaPerfilPainel();

		if($permissoes['verindicadores'] == 'vertodos') {
			$validacao = true;	
		} else {
			$validacao = validaAcessoIndicadores($permissoes['verindicadores'], $dadosdetalhesserihistorica['indid']);
		}

		if($validacao) {
			$result = enviarAgendamentoWebService($dadosdetalhesserihistorica);
			return $result;		
		} else {
			return "Usuсrio nуo possui permissуo";
		}
	} else {
		return "Erro de autenticaчуo";
	}
	
	

}


function enviarArquivoSerieHistorica($PHPSESSID, $dadosdetalhesserihistorica) {
	
	if (isset($PHPSESSID)) {
		session_id($PHPSESSID);
	}
	session_start();
	
	if($_SESSION['usucpf']) {
		$_SESSION['usucpforigem']=$_SESSION['usucpf'];
		$_SESSION['indid'] = $dadosdetalhesserihistorica['indid'];
		$permissoes = verificaPerfilPainel();

		if($permissoes['verindicadores'] == 'vertodos') {
			$validacao = true;	
		} else {
			$validacao = validaAcessoIndicadores($permissoes['verindicadores'], $dadosdetalhesserihistorica['indid']);
		}

		if($validacao) {
			$result = enviarArquivoAgendamento($dadosdetalhesserihistorica);
			return $result;		
		} else {
			return "Usuсrio nуo possui permissуo";
		}
	} else {
		return "Erro de autenticaчуo";
	}
	
	

}

function acompanharLogArquivoSerieHistorica($PHPSESSID, $wbsid) {
	
	if (isset($PHPSESSID)) {
		session_id($PHPSESSID);
	}
	session_start();
	
	if($_SESSION['usucpf']) {
		$_SESSION['usucpforigem']=$_SESSION['usucpf'];
		
		$result = acompanharArquivoAgendamento($wbsid);
		return $result;		
	} else {
		return "Erro de autenticaчуo";
	}
	
	

}


// Use the request to (try to) invoke the service
$HTTP_RAW_POST_DATA = isset($HTTP_RAW_POST_DATA) ? $HTTP_RAW_POST_DATA : '';
$server->service($HTTP_RAW_POST_DATA);
?>