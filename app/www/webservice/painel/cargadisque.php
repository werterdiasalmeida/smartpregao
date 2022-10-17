<?php
set_time_limit(0);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

require_once('nusoap.php');
require_once 'configDisque.php';


$db = new cls_banco();

$cfgPainel = array();
$cfgPainel['login'] = '03700155689';
$cfgPainel['senha'] = '12345';
$cfgPainel['url'] = "http://" . $_SERVER['SERVER_NAME'] . "/webservice/painel/server.php?wsdl";

$indice = $_REQUEST['indice'] ? $_REQUEST['indice'] : 0;
// grupo de violacao uf - 01/2011
$indid = $cfgIndicador[$indice]['id'];
$dataproc = $_REQUEST['dataproc'] ? $_REQUEST['dataproc'] : date('d-m-Y');
//$periodicidade = 1153;
$sql = $cfgIndicador[$indice]['sql'];


if ( $_REQUEST['cargageral'] )
{
	$parametros = array(
		array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>1150
			, '%DATA_INICIAL%'=>'2011-01-01'
			, '%DATA_FINAL%'=>"'%DATA_INICIAL%'::date + interval '1 month' - interval '1 day'"
		)
		, array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>1151
			, '%DATA_INICIAL%'=>'2011-02-01'
			, '%DATA_FINAL%'=>"'%DATA_INICIAL%'::date + interval '1 month' - interval '1 day'"
		)
		, array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>1152
			, '%DATA_INICIAL%'=>'2011-03-01'
			, '%DATA_FINAL%'=>"'%DATA_INICIAL%'::date + interval '1 month' - interval '1 day'"
		)
		, array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>1153
			, '%DATA_INICIAL%'=>'2011-04-01'
			, '%DATA_FINAL%'=>"'%DATA_INICIAL%'::date + interval '1 month' - interval '1 day'"
		)
		, array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>1154
			, '%DATA_INICIAL%'=>'2011-05-01'
			, '%DATA_FINAL%'=>"'%DATA_INICIAL%'::date + interval '1 month' - interval '1 day'"
		)
		, array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>1155
			, '%DATA_INICIAL%'=>'2011-06-01'
			, '%DATA_FINAL%'=>"'%DATA_INICIAL%'::date + interval '1 month' - interval '1 day'"
		)
		, array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>1156
			, '%DATA_INICIAL%'=>'2011-07-01'
			, '%DATA_FINAL%'=>"'%DATA_INICIAL%'::date + interval '1 month' - interval '1 day'"
		)
		, array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>1157
			, '%DATA_INICIAL%'=>'2011-08-01'
			, '%DATA_FINAL%'=>"'%DATA_INICIAL%'::date + interval '1 month' - interval '1 day'"
		)
		, array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>1158
			, '%DATA_INICIAL%'=>'2011-09-01'
			, '%DATA_FINAL%'=>"'%DATA_INICIAL%'::date + interval '1 month' - interval '1 day'"
		)
		, array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>1159
			, '%DATA_INICIAL%'=>'2011-10-01'
			, '%DATA_FINAL%'=>"'2011-10-01'::date + interval '1 month' - interval '1 day'"
		)
		, array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>1160
			, '%DATA_INICIAL%'=>'2011-11-01'
			, '%DATA_FINAL%'=>"'2011-11-01'::date + interval '1 month' - interval '1 day'"
		)
		, array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>1161
			, '%DATA_INICIAL%'=>'2011-12-01'
			, '%DATA_FINAL%'=>"'2011-12-01'::date + interval '1 month' - interval '1 day'"
		)
		, array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>1162
			, '%DATA_INICIAL%'=>'2012-01-01'
			, '%DATA_FINAL%'=>"'2012-01-01'::date + interval '1 month' - interval '1 day'"
		)
	);
}
else
{
	$linha = $db->pegaLinha("select dpeid, dpedatainicio, dpedatafim from painel.detalheperiodicidade where perid = 1 and dpestatus = 'A' and now() between dpedatainicio and dpedatafim");

	$parametros = array(
		array(
			'%IDINDICADOR%'=>$indid
			, '%PERIODICIDADE%'=>$linha['dpeid']
			, '%DATA_INICIAL%'=>$linha['dpedatainicio']
			, '%DATA_FINAL%'=>"'" . $linha['dpedatafim'] . "'"
		)
	);
}

foreach( $parametros as $params )
{
	var_dump($params);exit;
	$dados = json_decode(getData( $cfgIndicador[$indice]['sql'], $params ), true);
	switch($indid)
	{
		case INDICADOR_GRUPO_VIOLACAO:
			$csv = getCSVGrupoViolacao($dados);
		break;
		case INDICADOR_VIOLACAO_TIPO_VIOLACAO:
			$csv = getCSVViolacaoPorTipoViolacao($dados);
		break;

	}

	// Create the client instance
	$client = new soapcliente($cfgPainel['url'], true);

	// Check for an error
	$err = $client->getError();

	if ($err) {
		// Display the error
		echo '<h2>Constructor error</h2><pre>' . $err . '</pre>';
		// At this point, you know the call that follows will fail
	}

	// Call the SOAP method
	$autenticacao = $client->call('autenticarUsuario', array('cpf' => $cfgPainel['login'],'senha' => $cfgPainel['senha']));

	// Check for a fault
	if ($client->fault) {
		echo '<h2>Fault</h2><pre>';
		print_r($result);
		echo '</pre>';
	} else {
		// Check for errors
		$err = $client->getError();
		if ($err) {
			// Display the error
			echo '<h2>Error</h2><pre>' . $err . '</pre>';
		} else {
			// Display the result
			echo '<h2>Result</h2><pre>';
			print_r($autenticacao);
		echo '</pre>';
		}
	}
	// Display the request and response
	echo '<h2>Request</h2>';
	echo '<pre>' . htmlspecialchars($client->request, ENT_QUOTES) . '</pre>';
	echo '<h2>Response</h2>';
	echo '<pre>' . htmlspecialchars($client->response, ENT_QUOTES) . '</pre>';
	// Display the debug messages
	echo '<h2>Debug</h2>';
	echo '<pre>' . htmlspecialchars($client->debug_str, ENT_QUOTES) . '</pre>';

	echo '----------------------------------------------------------------------';

	$dadosdetalhesserihistorica['agddataprocessamento'] = $dataproc;
	$dadosdetalhesserihistorica['indid'] = $indid;
	$dadosdetalhesserihistorica['csvarray']= $csv;

	// Call the SOAP method
	$xx = $client->call('inserirAgendamentoSerieHistorica', array('PHPSESSID'=> $autenticacao,'dadosdetalhesserihistorica' => $dadosdetalhesserihistorica));

	// Check for a fault
	if ($client->fault) {
		echo '<h2>Fault</h2><pre>';
		print_r($xx);
		echo '</pre>';
	} else {
		// Check for errors
		$err = $client->getError();
		if ($err) {
			// Display the error
			echo '<h2>Error</h2><pre>' . $err . '</pre>';
		} else {
			// Display the result
			echo '<h2>Result</h2><pre>';
			print_r($xx);
		echo '</pre>';
		}
	}
	// Display the request and response
	echo '<h2>Request</h2>';
	echo '<pre>' . htmlspecialchars($client->request, ENT_QUOTES) . '</pre>';
	echo '<h2>Response</h2>';
	echo '<pre>' . htmlspecialchars($client->response, ENT_QUOTES) . '</pre>';
	// Display the debug messages
	echo '<h2>Debug</h2>';
	echo '<pre>' . htmlspecialchars($client->debug_str, ENT_QUOTES) . '</pre>';

	echo '----------------------------------------------------------------------';
}
?>