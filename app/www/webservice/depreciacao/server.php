<?php
// Pull in the NuSOAP code
require_once('nusoap.php');

// Connects to basedata in simec
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

$_CONTAS['14212.04.00'] = array("codigo" => "14212.04.00", "descricao" => "APARELHOS DE MEDICAO E ORIENTACAO", 				"porcentagem" => "0.1",  "vidautil" => "15");
$_CONTAS['14212.06.00'] = array("codigo" => "14212.06.00", "descricao" => "APARELHOS E EQUIPAMENTO DE COMUNICAÇÃO", 		"porcentagem" => "0.2",  "vidautil" => "10");
$_CONTAS['14212.08.00'] = array("codigo" => "14212.08.00", "descricao" => "APAR., EQUIP.E UTENS.MED.,ODONT.,LABOR.E HOSP.", "porcentagem" => "0.2",  "vidautil" => "15");
$_CONTAS['14212.10.00'] = array("codigo" => "14212.10.00", "descricao" => "APARELHOS E EQUIP. P/ESPORTES E DIVERSOES", 		"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.12.00'] = array("codigo" => "14212.12.00", "descricao" => "APARELHOS E UTENSILIOS DOMESTICOS", 				"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.13.00'] = array("codigo" => "14212.13.00", "descricao" => "ARMAZENS ESTRUTURAIS - COBERTURA DE LONA", 		"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.14.00'] = array("codigo" => "14212.14.00", "descricao" => "ARMAMENTOS", 									"porcentagem" => "0.15", "vidautil" => "20");
$_CONTAS['14212.18.00'] = array("codigo" => "14212.18.00", "descricao" => "COLECOES E MATERIAIS BIBLIOGRAFICOS",			"porcentagem" => "0",    "vidautil" => "10");
$_CONTAS['14212.19.00'] = array("codigo" => "14212.14.00", "descricao" => "DISCOTECAS E FILMOTECAS", 						"porcentagem" => "0.1",  "vidautil" => "5");
$_CONTAS['14212.22.00'] = array("codigo" => "14212.22.00", "descricao" => "EQUIPAMENTOS DE MANOBRAS E PATRULHAMENTO",		"porcentagem" => "0.1",  "vidautil" => "20");
$_CONTAS['14212.24.00'] = array("codigo" => "14212.24.00", "descricao" => "EQUIPAMENTO DE PROTEÇÃO, SEGURANÇA E SOCORRO",	"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.26.00'] = array("codigo" => "14212.26.00", "descricao" => "INSTRUMENTOS MUSICAIS E ARTISTICOS",				"porcentagem" => "0.1",  "vidautil" => "20");
$_CONTAS['14212.28.00'] = array("codigo" => "14212.28.00", "descricao" => "MAQUINAS E EQUIPAM. DE NATUREZA INDUSTRIAL",		"porcentagem" => "0.1",  "vidautil" => "20");
$_CONTAS['14212.30.00'] = array("codigo" => "14212.30.00", "descricao" => "MAQUINAS E EQUIPAMENTOS ENEGERTICOS",			"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.32.00'] = array("codigo" => "14212.32.00", "descricao" => "MAQUINAS E EQUIPAMENTOS GRAFICOS",				"porcentagem" => "0.1",  "vidautil" => "15");
$_CONTAS['14212.33.00'] = array("codigo" => "14212.33.00", "descricao" => "EQUIPAMENTOS PARA AUDIO, VIDEO E FOTO",			"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.34.00'] = array("codigo" => "14212.34.00", "descricao" => "MAQUINAS, UTENSILIOS E EQUIPAMENTOS DIVERSOS",	"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.35.00'] = array("codigo" => "14212.35.00", "descricao" => "EQUIPAMENTOS DE PROCESSAMENTOS DE DADOS",		"porcentagem" => "0.1",  "vidautil" => "5");
$_CONTAS['14212.36.00'] = array("codigo" => "14212.36.00", "descricao" => "MAQUINAS, INSTALACOES E UTENS. DE ESCRITORIO",	"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.38.00'] = array("codigo" => "14212.38.00", "descricao" => "MAQUINAS, FERRAMENTAS E UTENSILIOS DE OFICINA",	"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.39.00'] = array("codigo" => "14212.39.00", "descricao" => "EQUIPAMENTOS HIDRAULICOS E ELETRICOS",			"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.40.00'] = array("codigo" => "14212.40.00", "descricao" => "MAQ.EQUIP.UTENSILIOS AGRI/AGROP.E RODOVIARIOS",	"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.42.00'] = array("codigo" => "14212.42.00", "descricao" => "MOBILIARIO EM GERAL",							"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.46.00'] = array("codigo" => "14212.46.00", "descricao" => "SEMOVENTES E EQUIPAMENTOS DE MONTARIA",			"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.48.00'] = array("codigo" => "14212.48.00", "descricao" => "VEICULOS DIVERSOS",								"porcentagem" => "0.1",  "vidautil" => "15");
$_CONTAS['14212.49.00'] = array("codigo" => "14212.49.00", "descricao" => "EQUIPAMENTOS E MATERIAL SIGILOSO E RESERVADO",	"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.50.00'] = array("codigo" => "14212.50.00", "descricao" => "VEICULOS FERROVIARIOS",							"porcentagem" => "0.1",  "vidautil" => "30");
$_CONTAS['14212.51.00'] = array("codigo" => "14212.51.00", "descricao" => "PECAS NÃO INCORPORAVEIS A IMOVEIS",				"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.52.00'] = array("codigo" => "14212.52.00", "descricao" => "VEICULOS DE TRACAO MECANICA",					"porcentagem" => "0.1",  "vidautil" => "15");
$_CONTAS['14212.53.00'] = array("codigo" => "14212.53.00", "descricao" => "CARROS DE COMBATE",								"porcentagem" => "0.1",  "vidautil" => "30");
$_CONTAS['14212.54.00'] = array("codigo" => "14212.54.00", "descricao" => "EQUIPAMENTOS, PEÇAS E ACESSORIOS AERONAUTICOS",	"porcentagem" => "0.1",  "vidautil" => "30");
$_CONTAS['14212.56.00'] = array("codigo" => "14212.56.00", "descricao" => "EQUIPAMENTOS, PEÇAS E ACES.DE PROTECAO AO VOO",	"porcentagem" => "0.1",  "vidautil" => "30");
$_CONTAS['14212.57.00'] = array("codigo" => "14212.57.00", "descricao" => "ACESSORIOS PARA AUTOMOVEIS",						"porcentagem" => "0.1",  "vidautil" => "5");
$_CONTAS['14212.58.00'] = array("codigo" => "14212.58.00", "descricao" => "EQUIPAMENTO DE MERGULHO E SALVAMENTO",			"porcentagem" => "0.1",  "vidautil" => "15");
$_CONTAS['14212.60.00'] = array("codigo" => "14212.60.00", "descricao" => "EQUIPAMENTOS,PECAS E ACESSORIOS MARITIMOS",		"porcentagem" => "0.1",  "vidautil" => "15");
$_CONTAS['14212.83.00'] = array("codigo" => "14212.83.00", "descricao" => "EQUIPAMENTOS E SISTEMA DE PROT.VIG. AMBIENTAL",	"porcentagem" => "0.1",  "vidautil" => "10");





// Create the server instance
$server = new soapservidor();

// Initialize WSDL support
$server->configureWSDL('depreciacaowsdl', 'urn:depreciacaowsdl');


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
    'dadosdepreciacao',
    'complexType',
    'struct',
    'all',
    '',
    array('csvarray' => array('name' => 'csvarray', 'type' => 'tns:csvs'))
);

// Register the method to expose
$server->register('aplicardepreciacao',                // method name
    array('informacoes' => 'tns:dadosdepreciacao'),        // input parameters
    array('return' => 'tns:dadosdepreciacao'),      // output parameters
    'urn:aplicardepreciacaowsdl',                      // namespace
    'urn:aplicardepreciacaowsdl#aplicardepreciacao',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'REAVALIAÇÃO, REDUÇÃO A VALOR RECUPERÁVEL, DEPRECIAÇÃO, AMORTIZAÇÃO E EXAUSTÃO NA ADMINISTRAÇÃO DIRETA DA UNIÃO, SUAS AUTARQUIAS E FUNDAÇÕES'            // documentation
);

function aplicardepreciacao($informacoes) {
	
	global $_CONTAS;
	
	if(count($informacoes['csvarray']) > 0) {
		
		
		foreach($informacoes['csvarray'] as $key => $info) {
			
			$dadosdep = explode(";", $info);
			
			$retorno['csvarray'][$key] = trim(str_replace(" ","",$info));
			
			$validacao = true;
			
			// validando unicod
			if(!trim($dadosdep[0])) {
				$erro .= "Unidade orçamentária não existe.";
				$validacao = false;
			}
			// validando ungcod
			if(!trim($dadosdep[1])) {
				$erro .= "Unidade gestora não existe.";
				$validacao = false;
			}
			
			if($_AN[trim($dadosdep[3])]) {
				$erro .= "Número de tombamento duplicado.";
				$validacao = false;
			} else $_AN[trim($dadosdep[3])] = true;
			
			// $dadosdep[3] => número do tombamento não é obrigatório
			
			// validando conta contábil
			if(!$_CONTAS[trim($dadosdep[4])]) {
				$erro .= "Conta contábil inexistente.";
				$validacao = false;
			}
			// validando data de tombamento
			if(!verifica_data(trim($dadosdep[5]))) {
				$erro .= "Data de tombamento não existe e/ou inválida.";
				$validacao = false;
			}
			if(formata_data_sql(trim($dadosdep[5])) > date("Y-m-d")) {
				$erro .= "Data de tombamento maior que data de depreciação";
				$validacao = false;
			}
			// validando quantidade
			if(!is_numeric(trim($dadosdep[6])) || !trim($dadosdep[6])) {
				$erro .= "Quantidade não númerica e/ou vazia.";
				$validacao = false;
			}
			// validando quantidade, seé maior do que zero
			if(trim($dadosdep[6]) < 1) {
				$erro .= "Quantidade não deve ser menor do que 1(um)";
				$validacao = false;
			}
			
			// validando valor de entrada
			$dadosdep[7] = str_replace(array(".",","),array("","."), $dadosdep[7]);
			if(!is_numeric(trim($dadosdep[7]))) {
				$erro .= "Valor de entrada no formato não numérico.";
				$validacao = false;
			}
			
			if(trim($dadosdep[7]) < 0) {
				$erro .= "Valor de entrada não pode ser negativo";
				$validacao = false;
			}
			
			
			// regra 1 : se tiver número de tombamento, a quantidade deve ser maior do que 1(um)
			if(trim($dadosdep[3]) && trim($dadosdep[6]) > 1) {
				$erro .= "Quando existir um RGP a quantidade deve ser 1(um)";
				$validacao = false;
			}
			// regra 2 : se tiver a quantidade for 1(um), deverá existir o número de tombamento
			if(trim($dadosdep[6]) < 2 && !trim($dadosdep[3])) {
				$erro .= "RGP é obrigatório quando quantidade igual 1(um).";
				$validacao = false;
			}
			
			// regra 3: verificando se a primeira linha possui ou não cabeçalho
			if(!$key) {
				
				$d = explode("/",$dadosdep[5]);
				if(!is_numeric($d[1]) || !is_numeric($d[0]) || !is_numeric($d[2])) {
					unset($retorno['csvarray'][$key], $erro);
					$validacao = false;	
				}
				
			}
			
			if($validacao) {
				
				$Ve = trim($dadosdep[7]);
				$Vaa = trim($dadosdep[7]);
				$i  = $_CONTAS[trim($dadosdep[4])]['porcentagem'];
				$Vr = ($Ve*$i);
				$Vd = $Ve-$Vr;
				$t  = $_CONTAS[trim($dadosdep[4])]['vidautil'];
				$Da = $Vd/$t;
				$Dm = $Da/12;
				$dataE = trim($dadosdep[5]);
				$dataF = date("d/m/Y");
				
				$data = new Data();
				$difdata = $data->diferencaEntreDatas(trim($dadosdep[5]), date("d/m/Y"), 'tempoEntreDadas','array','dd/mm/yyyy');
				$nmeses = ($difdata['anos']*12)+$difdata['mes'];
				if(($nmeses) <= $t*12) {
					$Valorfd = $Ve - ($Dm*$nmeses);
					$Depref  = ($Dm*$nmeses);
				} else {
					$Valorfd = $Vr;
					$Depref  = ($Dm*$t*12);
				}
				$retorno['csvarray'][$key] .= ";".number_format($Vaa,2,",","").";".number_format($Vr,2,",","").";".number_format($Vd,2,",","").";".number_format($Dm,2,",","").";".number_format(($Depref),2,",","").";".number_format($Valorfd,2,",","").";".date("d/m/Y").";".($nmeses).";".($t*12);
				
			} else {
				
				if($erro) {
					$retorno['csvarray'][$key] .= ";;;;;;;;;;".$erro;
					unset($erro);
				}
				
			}
			
			
			
		}
		
		$retorno['csvarray'] = array_pad($retorno['csvarray'], -(count($retorno['csvarray']) + 1), "UNIDADE ORÇAMENTÁRIA(CÓDIGO);UNIDADE GESTORA(CÓDIGO);CÓDIGO DO PRODUTO;NÚMERO DE TOMBAMENTO*;CONTA PATRIMONIAL;DATA DE TOMBAMENTO;QUANTIDADE;VALOR DE ENTRADA;VALOR ATUAL ACUMULADO;VALOR RESIDUAL;VALOR DEPRECIÁVEL;DEPRECIAÇÃO DO MÊS CORRENTE;DEPRECIAÇÃO, AMORTIZAÇÃO OU EXAUSTÃO ACUMULADA;VALOR LÍQUIDO CONTÁBIL;DATA DE DEPRECIAÇÃO;MESES UTILIZAÇÃO;LIMITE MESES DEPRECIAÇÃO;ERROS");
		
	}
	
	return $retorno;
	

}


// Use the request to (try to) invoke the service
$HTTP_RAW_POST_DATA = isset($HTTP_RAW_POST_DATA) ? $HTTP_RAW_POST_DATA : '';
$server->service($HTTP_RAW_POST_DATA);
?>