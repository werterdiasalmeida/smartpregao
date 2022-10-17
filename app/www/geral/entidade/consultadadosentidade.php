<?php
header("Content-Type: text/html; charset=ISO-8859-1",true);
function __autoload($nameClass) {
	$arPath[] = APPRAIZ . "includes/classes/modelo/agenda/";
	$arPath[] = APPRAIZ . "includes/classes/modelo/entidade/";
	$arPath[] = APPRAIZ . "includes/classes/modelo/obras/";
	$arPath[] = APPRAIZ . "includes/classes/modelo/public/";
	$arPath[] = APPRAIZ . "includes/classes/modelo/questionario/";
	$arPath[] = APPRAIZ . "includes/classes/modelo/seguranca/";
	$arPath[] = APPRAIZ . "includes/classes/modelo/spm/";
	$arPath[] = APPRAIZ . "includes/classes/modelo/territorios/";
	$arPath[] = APPRAIZ . "includes/classes/view/";
	$arPath[] = APPRAIZ . "includes/classes/html/";
	$arPath[] = APPRAIZ . "includes/classes/";
	
	$loop = true; 
	do{	
		$arquivo = current($arPath) . $nameClass . '.class.inc';
		if (is_file($arquivo)){
    		require_once $arquivo;
    		$loop = false;
    		break;
		}
	}while ($loop && next($arPath));
}

if (!defined('APPRAIZ')) {
    define('APPRAIZ', realpath('../../../') . "/");
}

require_once APPRAIZ . 'global/config.inc';
include_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . 'includes/classes_simec.inc';


function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(substr($mask,$masklen,1) == "#") {
				$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
				$valuelen--;
			} else {
				if(trim(substr($value,$valuelen,1)) != "") {
					$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}



global $db;
$db = new cls_banco();

if($_REQUEST['requisicao'])
	$_REQUEST['requisicao']($_REQUEST);

function buscarEntidadePorEntid($dado)
{
	global $db;
	
	$dados = '{}';
	$entid = $_REQUEST['entid'];
	if ($entid){
		$entidade = new Entidade();
		$dados = $entidade->recuperarDados($entid);
	}
	$dados = array_map('tratarRetorno', $dados);
	
	die(json_encode($dados));
}	

function buscarEntidadePorCPF($dado)
{
	global $db;
	
	$dados = '{}';
	$cpf = pega_numero($dado['entcpfcnpj']);
	if ($cpf){
		$entidade = new Entidade();
		$dados = $entidade->recuperarDados(null, array('tpeid = 4', "entcpfcnpj = '{$cpf}'"));
	}
	$dados = is_array($dados) ? $dados : array();
	$dados = array_map('tratarRetorno', $dados);
	
	die(json_encode($dados));
}	


function buscarEntidadePorCNPJ($dado)
{
	global $db;
	
	$dados = '{}';
	$cnpj = pega_numero($dado['entcpfcnpj']);
	if ($cnpj){
		$entidade = new Entidade();
		$dados = $entidade->recuperarDados(null, array('tpeid != 4', "entcpfcnpj = '{$cnpj}'"));
	}
	$dados = is_array($dados) ? $dados : array();
	$dados = array_map('tratarRetorno', $dados);
	
	die(json_encode($dados));
}	

//function tratarRetorno($elemento)
//{
//	$elemento = utf8_encode($elemento);
//	$elemento = htmlspecialchars($elemento, ENT_QUOTES);
//	$elemento = trim($elemento);
//	
//	return $elemento;
//}
	
//function pegarenderecoPorCEP($dados) {
//	global $db;
//	$cp = str_replace(array('.', '-'), '', $_REQUEST['endcep']);
//	$rs = $db->pegaLinha("SELECT * FROM cep.v_endereco WHERE cep='".$cp."' ORDER BY cidade ASC");
//	echo $rs['logradouro']."||".$rs['bairro']."||".$rs['cidade']."||".$rs['estado']."||".$rs['muncod'];
//	exit;
//}
//
//function pegarentidadePorentid($dados) {
//	global $db;
//	$rs = $db->pegaLinha("SELECT * FROM entidade.entidade WHERE entid='".$dados['entid']."'");
//	
//	if($rs) {
//		/*
//		 * AJUSTANDO DADOS
//		 */
//		if($rs['entdatanasc']) {
//			$d = explode("-",$rs['entdatanasc']);
//			$rs['entdatanasc'] = $d[2]."/".$d[1]."/".$d[0];
//		}
//		/*
//		 * CARREGANDO AS FUNCOES 
//		 */
//		$fu = $db->carregar("SELECT fun.fundsc, ent.entnome FROM entidade.funcaoentidade fen 
//							 LEFT JOIN entidade.funcao fun ON fen.funid = fun.funid 
//							 LEFT JOIN entidade.funentassoc fea ON fen.fueid = fea.fueid  
//							 LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid 
//						 	 WHERE fen.entid='".$rs['entid']."'");
//		if($fu[0]) {
//			foreach($fu as $f) {
//				$funcoes[] = $f['fundsc']." ".(($f['entnome'])?"(".$f['entnome'].")":"");
//			}
//		}
//		
//		/*
//		 * CARREGANDO OS ENDEREÇOS
//		 */
//		$en = $db->carregar("SELECT een.*, mun.mundescricao FROM entidade.endereco een
//							 LEFT JOIN territorios.municipio mun ON mun.muncod = een.muncod 
//							 WHERE een.entid='".$rs['entid']."'");
//		
//		if($en[0]) {
//			foreach($en as $e) {
//				$enderecos[] = $e['endid']."##".$e['tpeid']."##".mascaraglobal($e['endcep'],"#####-###")."##".$e['endlog']."##".
//							   $e['endcom']."##".$e['endbai']."##".$e['muncod']."##".$e['estuf']."##".$e['endnum']."##".
//							   $e['medlatitude']."##".$e['medlongitude']."##".$e['endcomunidade']."##".$e['endzoom']."##".$e['mundescricao'];
//			}
//		}
//		
//		echo $rs['entid']."||".$rs['njuid']."||".$rs['entnumcpfcnpj']."||".$rs['entnome']."||".$rs['entemail']."||".$rs['entnuninsest']."||".
//			 $rs['entobs']."||".$rs['entnumrg']."||".$rs['entorgaoexpedidor']."||".$rs['entsexo']."||".$rs['entdatanasc']."||".
//			 $rs['entdatainiass']."||".$rs['entdatafimass']."||".$rs['entnumdddresidencial']."||".$rs['entnumresidencial']."||".
//			 $rs['entnumdddcomercial']."||".$rs['entnumramalcomercial']."||".$rs['entnumcomercial']."||".$rs['entnumdddfax']."||".
//			 $rs['entnumramalfax']."||".$rs['entnumfax']."||".$rs['tpctgid']."||".$rs['tpcid']."||".$rs['tplid']."||".$rs['tpsid']."||".
//			 $rs['entcodentsup']."||".$rs['entcodent']."||".$rs['entescolanova']."||".$rs['entsig']."||".$rs['entunicod']."||".$rs['entungcod']."||".
//			 $rs['entproep']."||".$rs['entnumdddcelular']."||".$rs['entnumcelular']."||".$rs['entorgcod']."||".$rs['entsede']."||".(($funcoes)?implode("%%",$funcoes):"naofuncoes")."||".(($enderecos)?implode("%%",$enderecos):"naoendereco");
//	} else {
//		echo "naoexiste";
//	}
//	exit;
//}
//
//function pegarentidadePorentcodent($dados) {
//	global $db;
//	$rs = $db->carregar("SELECT * FROM entidade.entidade WHERE entcodent='".$dados['entcodent']."'");
//	/*
//	 * Quando existem varias entidades na consulta, avisar que o filtro não esta preciso
//	 */
//	if(count($rs) > 1) {
//		echo "existemuitos";
//		exit;
//	} else {
//		if($rs[0]) $rs = current($rs);
//	}
//	
//	if($rs) {
//		/*
//		 * AJUSTANDO DADOS
//		 */
//		if($rs['entdatanasc']) {
//			$d = explode("-",$rs['entdatanasc']);
//			$rs['entdatanasc'] = $d[2]."/".$d[1]."/".$d[0];
//		}
//		/*
//		 * CARREGANDO AS FUNCOES 
//		 */
//		$fu = $db->carregar("SELECT fun.fundsc, ent.entnome FROM entidade.funcaoentidade fen 
//							 LEFT JOIN entidade.funcao fun ON fen.funid = fun.funid 
//							 LEFT JOIN entidade.funentassoc fea ON fen.fueid = fea.fueid  
//							 LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid 
//						 	 WHERE fen.entid='".$rs['entid']."'");
//		if($fu[0]) {
//			foreach($fu as $f) {
//				$funcoes[] = $f['fundsc']." ".(($f['entnome'])?"(".$f['entnome'].")":"");
//			}
//		}
//		
//		/*
//		 * CARREGANDO OS ENDEREÇOS
//		 */
//		$en = $db->carregar("SELECT een.*, mun.mundescricao FROM entidade.endereco een
//							 LEFT JOIN territorios.municipio mun ON mun.muncod = een.muncod 
//							 WHERE een.entid='".$rs['entid']."'");
//		
//		if($en[0]) {
//			foreach($en as $e) {
//				$enderecos[] = $e['endid']."##".$e['tpeid']."##".mascaraglobal($e['endcep'],"#####-###")."##".$e['endlog']."##".
//							   $e['endcom']."##".$e['endbai']."##".$e['muncod']."##".$e['estuf']."##".$e['endnum']."##".
//							   $e['medlatitude']."##".$e['medlongitude']."##".$e['endcomunidade']."##".$e['endzoom']."##".$e['mundescricao'];
//			}
//		}
//		
//		echo $rs['entid']."||".$rs['njuid']."||".$rs['entnumcpfcnpj']."||".$rs['entnome']."||".$rs['entemail']."||".$rs['entnuninsest']."||".
//			 $rs['entobs']."||".$rs['entnumrg']."||".$rs['entorgaoexpedidor']."||".$rs['entsexo']."||".$rs['entdatanasc']."||".
//			 $rs['entdatainiass']."||".$rs['entdatafimass']."||".$rs['entnumdddresidencial']."||".$rs['entnumresidencial']."||".
//			 $rs['entnumdddcomercial']."||".$rs['entnumramalcomercial']."||".$rs['entnumcomercial']."||".$rs['entnumdddfax']."||".
//			 $rs['entnumramalfax']."||".$rs['entnumfax']."||".$rs['tpctgid']."||".$rs['tpcid']."||".$rs['tplid']."||".$rs['tpsid']."||".
//			 $rs['entcodentsup']."||".$rs['entcodent']."||".$rs['entescolanova']."||".$rs['entsig']."||".$rs['entunicod']."||".$rs['entungcod']."||".
//			 $rs['entproep']."||".$rs['entnumdddcelular']."||".$rs['entnumcelular']."||".$rs['entorgcod']."||".$rs['entsede']."||".(($funcoes)?implode("%%",$funcoes):"naofuncoes")."||".(($enderecos)?implode("%%",$enderecos):"naoendereco");
//	} else {
//		echo "naoexiste";
//	}
//	exit;
//}
//
//function pegarentidadePorentnome($dados) {
//	global $db;
//	
//	$rs = $db->carregar("SELECT * FROM entidade.entidade WHERE entnome ilike '%".$dados['entnome']."%'");
//	
//	if(count($rs) > 50) {
//		echo "filtroruim";
//		exit;
//	}
//	/*
//	 * Quando existem varias entidades na consulta, avisar que o filtro não esta preciso
//	 */
//	if(count($rs) > 1) {
//		foreach($rs as $l) {
//			/*
//			 * AJUSTANDO DADOS
//			 */
//			if($l['entdatanasc']) {
//				$d = explode("-",$l['entdatanasc']);
//				$l['entdatanasc'] = $d[2]."/".$d[1]."/".$d[0];
//			}
//			/*
//			 * CARREGANDO AS FUNCOES 
//			 */
//			$fu = $db->carregar("SELECT fun.fundsc, ent.entnome FROM entidade.funcaoentidade fen 
//								 LEFT JOIN entidade.funcao fun ON fen.funid = fun.funid 
//								 LEFT JOIN entidade.funentassoc fea ON fen.fueid = fea.fueid  
//								 LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid 
//							 	 WHERE fen.entid='".$l['entid']."'");
//			if($fu[0]) {
//				foreach($fu as $f) {
//					$funcoes[] = $f['fundsc']." ".(($f['entnome'])?"(".$f['entnome'].")":"");
//				}
//			}
//			
//			/*
//			 * CARREGANDO OS ENDEREÇOS
//			 */
//			$en = $db->carregar("SELECT een.*, mun.mundescricao FROM entidade.endereco een
//								 LEFT JOIN territorios.municipio mun ON mun.muncod = een.muncod 
//								 WHERE een.entid='".$l['entid']."'");
//			
//			if($en[0]) {
//				foreach($en as $e) {
//					$enderecos[] = $e['endid']."##".$e['tpeid']."##".mascaraglobal($e['endcep'],"#####-###")."##".$e['endlog']."##".
//								   $e['endcom']."##".$e['endbai']."##".$e['muncod']."##".$e['estuf']."##".$e['endnum']."##".
//								   $e['medlatitude']."##".$e['medlongitude']."##".$e['endcomunidade']."##".$e['endzoom']."##".$e['mundescricao'];
//				}
//			}
//			
//			$ajaxresp .=  $l['entid']."||".$l['njuid']."||".$l['entnumcpfcnpj']."||".$l['entnome']."||".$l['entemail']."||".$l['entnuninsest']."||".
//				 		  $l['entobs']."||".$l['entnumrg']."||".$l['entorgaoexpedidor']."||".$l['entsexo']."||".$l['entdatanasc']."||".
//				 		  $l['entdatainiass']."||".$l['entdatafimass']."||".$l['entnumdddresidencial']."||".$l['entnumresidencial']."||".
//				 		  $l['entnumdddcomercial']."||".$l['entnumramalcomercial']."||".$l['entnumcomercial']."||".$l['entnumdddfax']."||".
//				 		  $l['entnumramalfax']."||".$l['entnumfax']."||".$l['tpctgid']."||".$l['tpcid']."||".$l['tplid']."||".$l['tpsid']."||".
//				 		  $l['entcodentsup']."||".$l['entcodent']."||".$l['entescolanova']."||".$l['entsig']."||".$l['entunicod']."||".$l['entungcod']."||".
//				 		  $l['entproep']."||".$l['entnumdddcelular']."||".$l['entnumcelular']."||".$l['entorgcod']."||".$l['entsede']."||".(($funcoes)?implode("%%",$funcoes):"naofuncoes")."||".(($enderecos)?implode("%%",$enderecos):"naoendereco")."$$"; 
//		}
//		echo "existemuitos$$".$ajaxresp;
//		exit;
//	} elseif($rs[0]) {
//		$rs = current($rs);
//	}
//	
//	if($rs) {
//		/*
//		 * AJUSTANDO DADOS
//		 */
//		if($rs['entdatanasc']) {
//			$d = explode("-",$rs['entdatanasc']);
//			$rs['entdatanasc'] = $d[2]."/".$d[1]."/".$d[0];
//		}
//		/*
//		 * CARREGANDO AS FUNCOES 
//		 */
//		$fu = $db->carregar("SELECT fun.fundsc, ent.entnome FROM entidade.funcaoentidade fen 
//							 LEFT JOIN entidade.funcao fun ON fen.funid = fun.funid 
//							 LEFT JOIN entidade.funentassoc fea ON fen.fueid = fea.fueid  
//							 LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid 
//						 	 WHERE fen.entid='".$rs['entid']."'");
//		if($fu[0]) {
//			foreach($fu as $f) {
//				$funcoes[] = $f['fundsc']." ".(($f['entnome'])?"(".$f['entnome'].")":"");
//			}
//		}
//		
//		/*
//		 * CARREGANDO OS ENDEREÇOS
//		 */
//		$en = $db->carregar("SELECT een.*, mun.mundescricao FROM entidade.endereco een
//							 LEFT JOIN territorios.municipio mun ON mun.muncod = een.muncod 
//							 WHERE een.entid='".$rs['entid']."'");
//		
//		if($en[0]) {
//			foreach($en as $e) {
//				$enderecos[] = $e['endid']."##".$e['tpeid']."##".mascaraglobal($e['endcep'],"#####-###")."##".$e['endlog']."##".
//							   $e['endcom']."##".$e['endbai']."##".$e['muncod']."##".$e['estuf']."##".$e['endnum']."##".
//							   $e['medlatitude']."##".$e['medlongitude']."##".$e['endcomunidade']."##".$e['endzoom']."##".$e['mundescricao'];
//			}
//		}
//		
//		echo $rs['entid']."||".$rs['njuid']."||".$rs['entnumcpfcnpj']."||".$rs['entnome']."||".$rs['entemail']."||".$rs['entnuninsest']."||".
//			 $rs['entobs']."||".$rs['entnumrg']."||".$rs['entorgaoexpedidor']."||".$rs['entsexo']."||".$rs['entdatanasc']."||".
//			 $rs['entdatainiass']."||".$rs['entdatafimass']."||".$rs['entnumdddresidencial']."||".$rs['entnumresidencial']."||".
//			 $rs['entnumdddcomercial']."||".$rs['entnumramalcomercial']."||".$rs['entnumcomercial']."||".$rs['entnumdddfax']."||".
//			 $rs['entnumramalfax']."||".$rs['entnumfax']."||".$rs['tpctgid']."||".$rs['tpcid']."||".$rs['tplid']."||".$rs['tpsid']."||".
//			 $rs['entcodentsup']."||".$rs['entcodent']."||".$rs['entescolanova']."||".$rs['entsig']."||".$rs['entunicod']."||".$rs['entungcod']."||".
//			 $rs['entproep']."||".$rs['entnumdddcelular']."||".$rs['entnumcelular']."||".$rs['entorgcod']."||".$rs['entsede']."||".(($funcoes)?implode("%%",$funcoes):"naofuncoes")."||".(($enderecos)?implode("%%",$enderecos):"naoendereco");
//	} else {
//		echo "naoexiste";
//	}
//	exit;
//}
//
//function pegarentidadePorunicod($dados) {
//	global $db;
//	$rs = $db->carregar("SELECT * FROM entidade.entidade WHERE entunicod='".$dados['entunicod']."'");
//	/*
//	 * Quando existem varias entidades na consulta, avisar que o filtro não esta preciso
//	 */
//	if(count($rs) > 1) {
//		foreach($rs as $l) {
//			/*
//			 * AJUSTANDO DADOS
//			 */
//			if($l['entdatanasc']) {
//				$d = explode("-",$l['entdatanasc']);
//				$l['entdatanasc'] = $d[2]."/".$d[1]."/".$d[0];
//			}
//			/*
//			 * CARREGANDO AS FUNCOES 
//			 */
//			$fu = $db->carregar("SELECT fun.fundsc, ent.entnome FROM entidade.funcaoentidade fen 
//								 LEFT JOIN entidade.funcao fun ON fen.funid = fun.funid 
//								 LEFT JOIN entidade.funentassoc fea ON fen.fueid = fea.fueid  
//								 LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid 
//							 	 WHERE fen.entid='".$l['entid']."'");
//			if($fu[0]) {
//				foreach($fu as $f) {
//					$funcoes[] = $f['fundsc']." ".(($f['entnome'])?"(".$f['entnome'].")":"");
//				}
//			}
//			
//			/*
//			 * CARREGANDO OS ENDEREÇOS
//			 */
//			$en = $db->carregar("SELECT een.*, mun.mundescricao FROM entidade.endereco een
//								 LEFT JOIN territorios.municipio mun ON mun.muncod = een.muncod 
//								 WHERE een.entid='".$l['entid']."'");
//			
//			if($en[0]) {
//				foreach($en as $e) {
//					$enderecos[] = $e['endid']."##".$e['tpeid']."##".mascaraglobal($e['endcep'],"#####-###")."##".$e['endlog']."##".
//								   $e['endcom']."##".$e['endbai']."##".$e['muncod']."##".$e['estuf']."##".$e['endnum']."##".
//								   $e['medlatitude']."##".$e['medlongitude']."##".$e['endcomunidade']."##".$e['endzoom']."##".$e['mundescricao'];
//				}
//			}
//			
//			$ajaxresp .=  $l['entid']."||".$l['njuid']."||".$l['entnumcpfcnpj']."||".$l['entnome']."||".$l['entemail']."||".$l['entnuninsest']."||".
//				 		  $l['entobs']."||".$l['entnumrg']."||".$l['entorgaoexpedidor']."||".$l['entsexo']."||".$l['entdatanasc']."||".
//				 		  $l['entdatainiass']."||".$l['entdatafimass']."||".$l['entnumdddresidencial']."||".$l['entnumresidencial']."||".
//				 		  $l['entnumdddcomercial']."||".$l['entnumramalcomercial']."||".$l['entnumcomercial']."||".$l['entnumdddfax']."||".
//				 		  $l['entnumramalfax']."||".$l['entnumfax']."||".$l['tpctgid']."||".$l['tpcid']."||".$l['tplid']."||".$l['tpsid']."||".
//				 		  $l['entcodentsup']."||".$l['entcodent']."||".$l['entescolanova']."||".$l['entsig']."||".$l['entunicod']."||".$l['entungcod']."||".
//				 		  $l['entproep']."||".$l['entnumdddcelular']."||".$l['entnumcelular']."||".$l['entorgcod']."||".$l['entsede']."||".(($funcoes)?implode("%%",$funcoes):"naofuncoes")."||".(($enderecos)?implode("%%",$enderecos):"naoendereco")."$$"; 
//		}
//		echo "existemuitos$$".$ajaxresp;
//		exit;
//	} elseif($rs[0]) {
//		$rs = current($rs);
//	}
//	
//	if($rs) {
//		/*
//		 * AJUSTANDO DADOS
//		 */
//		if($rs['entdatanasc']) {
//			$d = explode("-",$rs['entdatanasc']);
//			$rs['entdatanasc'] = $d[2]."/".$d[1]."/".$d[0];
//		}
//		/*
//		 * CARREGANDO AS FUNCOES 
//		 */
//		$fu = $db->carregar("SELECT fun.fundsc, ent.entnome FROM entidade.funcaoentidade fen 
//							 LEFT JOIN entidade.funcao fun ON fen.funid = fun.funid 
//							 LEFT JOIN entidade.funentassoc fea ON fen.fueid = fea.fueid  
//							 LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid 
//						 	 WHERE fen.entid='".$rs['entid']."'");
//		if($fu[0]) {
//			foreach($fu as $f) {
//				$funcoes[] = $f['fundsc']." ".(($f['entnome'])?"(".$f['entnome'].")":"");
//			}
//		}
//		
//		/*
//		 * CARREGANDO OS ENDEREÇOS
//		 */
//		$en = $db->carregar("SELECT een.*, mun.mundescricao FROM entidade.endereco een
//							 LEFT JOIN territorios.municipio mun ON mun.muncod = een.muncod 
//							 WHERE een.entid='".$rs['entid']."'");
//		
//		if($en[0]) {
//			foreach($en as $e) {
//				$enderecos[] = $e['endid']."##".$e['tpeid']."##".mascaraglobal($e['endcep'],"#####-###")."##".$e['endlog']."##".
//							   $e['endcom']."##".$e['endbai']."##".$e['muncod']."##".$e['estuf']."##".$e['endnum']."##".
//							   $e['medlatitude']."##".$e['medlongitude']."##".$e['endcomunidade']."##".$e['endzoom']."##".$e['mundescricao'];
//			}
//		}
//		
//		echo $rs['entid']."||".$rs['njuid']."||".$rs['entnumcpfcnpj']."||".$rs['entnome']."||".$rs['entemail']."||".$rs['entnuninsest']."||".
//			 $rs['entobs']."||".$rs['entnumrg']."||".$rs['entorgaoexpedidor']."||".$rs['entsexo']."||".$rs['entdatanasc']."||".
//			 $rs['entdatainiass']."||".$rs['entdatafimass']."||".$rs['entnumdddresidencial']."||".$rs['entnumresidencial']."||".
//			 $rs['entnumdddcomercial']."||".$rs['entnumramalcomercial']."||".$rs['entnumcomercial']."||".$rs['entnumdddfax']."||".
//			 $rs['entnumramalfax']."||".$rs['entnumfax']."||".$rs['tpctgid']."||".$rs['tpcid']."||".$rs['tplid']."||".$rs['tpsid']."||".
//			 $rs['entcodentsup']."||".$rs['entcodent']."||".$rs['entescolanova']."||".$rs['entsig']."||".$rs['entunicod']."||".$rs['entungcod']."||".
//			 $rs['entproep']."||".$rs['entnumdddcelular']."||".$rs['entnumcelular']."||".$rs['entorgcod']."||".$rs['entsede']."||".(($funcoes)?implode("%%",$funcoes):"naofuncoes")."||".(($enderecos)?implode("%%",$enderecos):"naoendereco");
//	} else {
//		echo "naoexiste";
//	}
//	exit;
//}
//
//
//function pegarentidadePorcpfcnpj($dados) {
//	global $db;
//	$cp = str_replace(array('.', '-', '/'), '', $dados['entnumcpfcnpj']);
//	$rs = $db->carregar("SELECT * FROM entidade.entidade WHERE entnumcpfcnpj='".$cp."'");
//	
//	/*
//	 * Quando existem varias entidades na consulta, avisar que o filtro não esta preciso
//	 */
//	if(count($rs) > 1) {
//		foreach($rs as $l) {
//			/*
//			 * AJUSTANDO DADOS
//			 */
//			if($l['entdatanasc']) {
//				$d = explode("-",$l['entdatanasc']);
//				$l['entdatanasc'] = $d[2]."/".$d[1]."/".$d[0];
//			}
//			/*
//			 * CARREGANDO AS FUNCOES 
//			 */
//			$fu = $db->carregar("SELECT fun.fundsc, ent.entnome FROM entidade.funcaoentidade fen 
//								 LEFT JOIN entidade.funcao fun ON fen.funid = fun.funid 
//								 LEFT JOIN entidade.funentassoc fea ON fen.fueid = fea.fueid  
//								 LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid 
//							 	 WHERE fen.entid='".$l['entid']."'");
//			if($fu[0]) {
//				foreach($fu as $f) {
//					$funcoes[] = $f['fundsc']." ".(($f['entnome'])?"(".$f['entnome'].")":"");
//				}
//			}
//			
//			/*
//			 * CARREGANDO OS ENDEREÇOS
//			 */
//			$en = $db->carregar("SELECT een.*, mun.mundescricao FROM entidade.endereco een
//								 LEFT JOIN territorios.municipio mun ON mun.muncod = een.muncod 
//								 WHERE een.entid='".$l['entid']."'");
//			
//			if($en[0]) {
//				foreach($en as $e) {
//					$enderecos[] = $e['endid']."##".$e['tpeid']."##".mascaraglobal($e['endcep'],"#####-###")."##".$e['endlog']."##".
//								   $e['endcom']."##".$e['endbai']."##".$e['muncod']."##".$e['estuf']."##".$e['endnum']."##".
//								   $e['medlatitude']."##".$e['medlongitude']."##".$e['endcomunidade']."##".$e['endzoom']."##".$e['mundescricao'];
//				}
//			}
//			
//			$ajaxresp .=  $l['entid']."||".$l['njuid']."||".$l['entnumcpfcnpj']."||".$l['entnome']."||".$l['entemail']."||".$l['entnuninsest']."||".
//				 		  $l['entobs']."||".$l['entnumrg']."||".$l['entorgaoexpedidor']."||".$l['entsexo']."||".$l['entdatanasc']."||".
//				 		  $l['entdatainiass']."||".$l['entdatafimass']."||".$l['entnumdddresidencial']."||".$l['entnumresidencial']."||".
//				 		  $l['entnumdddcomercial']."||".$l['entnumramalcomercial']."||".$l['entnumcomercial']."||".$l['entnumdddfax']."||".
//				 		  $l['entnumramalfax']."||".$l['entnumfax']."||".$l['tpctgid']."||".$l['tpcid']."||".$l['tplid']."||".$l['tpsid']."||".
//				 		  $l['entcodentsup']."||".$l['entcodent']."||".$l['entescolanova']."||".$l['entsig']."||".$l['entunicod']."||".$l['entungcod']."||".
//				 		  $l['entproep']."||".$l['entnumdddcelular']."||".$l['entnumcelular']."||".$l['entorgcod']."||".$l['entsede']."||".(($funcoes)?implode("%%",$funcoes):"naofuncoes")."||".(($enderecos)?implode("%%",$enderecos):"naoendereco")."$$"; 
//		}
//		echo "existemuitos$$".$ajaxresp;
//		exit;
//		
//	} elseif($rs[0]) {
//		$rs = current($rs);
//	}
//	
//	
//	if($rs) {
//		/*
//		 * AJUSTANDO DADOS
//		 */
//		if($rs['entdatanasc']) {
//			$d = explode("-",$rs['entdatanasc']);
//			$rs['entdatanasc'] = $d[2]."/".$d[1]."/".$d[0];
//		}
//		/*
//		 * CARREGANDO AS FUNCOES 
//		 */
//		$fu = $db->carregar("SELECT fun.fundsc, ent.entnome FROM entidade.funcaoentidade fen 
//							 LEFT JOIN entidade.funcao fun ON fen.funid = fun.funid 
//							 LEFT JOIN entidade.funentassoc fea ON fen.fueid = fea.fueid  
//							 LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid 
//						 	 WHERE fen.entid='".$rs['entid']."'");
//		if($fu[0]) {
//			foreach($fu as $f) {
//				$funcoes[] = $f['fundsc']." ".(($f['entnome'])?"(".$f['entnome'].")":"");
//			}
//		}
//		
//		/*
//		 * CARREGANDO OS ENDEREÇOS
//		 */
//		$en = $db->carregar("SELECT een.*, mun.mundescricao FROM entidade.endereco een
//							 LEFT JOIN territorios.municipio mun ON mun.muncod = een.muncod 
//							 WHERE een.entid='".$rs['entid']."'");
//		
//		if($en[0]) {
//			foreach($en as $e) {
//				$enderecos[] = $e['endid']."##".$e['tpeid']."##".mascaraglobal($e['endcep'],"#####-###")."##".$e['endlog']."##".
//							   $e['endcom']."##".$e['endbai']."##".$e['muncod']."##".$e['estuf']."##".$e['endnum']."##".
//							   $e['medlatitude']."##".$e['medlongitude']."##".$e['endcomunidade']."##".$e['endzoom']."##".$e['mundescricao'];
//			}
//		}
//		
//		echo $rs['entid']."||".$rs['njuid']."||".$rs['entnumcpfcnpj']."||".$rs['entnome']."||".$rs['entemail']."||".$rs['entnuninsest']."||".
//			 $rs['entobs']."||".$rs['entnumrg']."||".$rs['entorgaoexpedidor']."||".$rs['entsexo']."||".$rs['entdatanasc']."||".
//			 $rs['entdatainiass']."||".$rs['entdatafimass']."||".$rs['entnumdddresidencial']."||".$rs['entnumresidencial']."||".
//			 $rs['entnumdddcomercial']."||".$rs['entnumramalcomercial']."||".$rs['entnumcomercial']."||".$rs['entnumdddfax']."||".
//			 $rs['entnumramalfax']."||".$rs['entnumfax']."||".$rs['tpctgid']."||".$rs['tpcid']."||".$rs['tplid']."||".$rs['tpsid']."||".
//			 $rs['entcodentsup']."||".$rs['entcodent']."||".$rs['entescolanova']."||".$rs['entsig']."||".$rs['entunicod']."||".$rs['entungcod']."||".
//			 $rs['entproep']."||".$rs['entnumdddcelular']."||".$rs['entnumcelular']."||".$rs['entorgcod']."||".$rs['entsede']."||".(($funcoes)?implode("%%",$funcoes):"naofuncoes")."||".(($enderecos)?implode("%%",$enderecos):"naoendereco");
//	} else {
//		echo "naoexiste";
//	}
//	exit;
//}
//	
//	
//function cepUF() {
//    global $db;
//
//    try{
//        $cp = str_replace(array('.', '-'), '', $_REQUEST['endcep']);
//        $rs = $db->carregar('SELECT ufesg as estado FROM cep.logfaixauf WHERE \'' . $cp . '\' BETWEEN ufecepini and ufecepfim');
//
//        //$ws  = new SoapClient('http://dne.mec.gov.br/server/service.php?WSDL');
//        //$res = $ws->getDNE(null, 0, 0, 0, str_replace(array('.', '-'), '', $_REQUEST['endcep']));
//        //$xml = simplexml_load_string(utf8_encode(html_entity_decode($res))); // returns UTF-8
//
//      //  echo "var DNE = new Array();" , "\n";
//
//        if (!is_array($rs)) {
//            echo 'DNE.push({ \'cep\'           : \'' , $cp , '\',
//                             \'cidade\'        : \'\',
//                             \'estado\'        : \'\',
//                             \'latitude\'      : \'\',
//                             \'hemisferio\'    : \'\',
//                             \'longitude\'     : \'\',
//                             \'meridiano\'     : \'\',
//                             \'altitude\'      : \'\',
//                             \'medidaarea\'    : \'\',
//                             \'medidaraio\'    : \'\',
//                             \'muncod\'        : \'\',
//                             \'muncodcompleto\': \'\'
//        					});';
//        } else {
//            foreach ($rs as $cepUF) {
//                echo 'DNE.push({';
//                foreach ($cepUF as $node => $value) {
//                    echo "'" , $node , "': '" , addslashes(trim($value)) , "',";
//                }
//                echo "'time':'" , time() , "'});\n";
//            }
//        }
//    } catch (Exception $e) {
//        echo ($e->getMessage());
//    }
//}
//
//
//
//
//function dne()
//{
//    global $db;
//
//    try{
//        $cp = str_replace(array('.', '-'), '', $_REQUEST['endcep']);
//        $rs = $db->carregar('select * from cep.v_endereco where cep = \'' . $cp . '\' order by cidade asc');
//
//        //$ws  = new SoapClient('http://dne.mec.gov.br/server/service.php?WSDL');
//        //$res = $ws->getDNE(null, 0, 0, 0, str_replace(array('.', '-'), '', $_REQUEST['endcep']));
//        //$xml = simplexml_load_string(utf8_encode(html_entity_decode($res))); // returns UTF-8
//
//        echo "var DNE = new Array();" , "\n";
//
//        if (!is_array($rs)) {
//            echo 'DNE.push({\'cep\'           : \'' , $cp , '\',
//                            \'logradouro\'    : \'\',
//                            \'bairro\'        : \'\',
//                            \'cidade\'        : \'\',
//                            \'estado\'        : \'\',
//                            \'latitude\'      : \'\',
//                            \'hemisferio\'    : \'\',
//                            \'longitude\'     : \'\',
//                            \'meridiano\'     : \'\',
//                            \'altitude\'      : \'\',
//                            \'medidaarea\'    : \'\',
//                            \'medidaraio\'    : \'\',
//                            \'muncod\'        : \'\',
//                            \'muncodcompleto\': \'\'});';
//        } else {
//            foreach ($rs as $dne) {
//                echo 'DNE.push({';
//                foreach ($dne as $node => $value) {
//                    echo "'" , $node , "': '" , addslashes(trim($value)) , "',";
//                }
//                echo "'time':'" , time() , "'});\n";
//            }
//        }
//    } catch (Exception $e) {
//        echo ($e->getMessage());
//    }
//}
//
//
//function municipio()
//{
//    global $db;
//
//    if (array_key_exists('complete', $_REQUEST))
//        die();
//
//    if (trim($_REQUEST['regcod'] == ''))
//        die();
//
//    $res       = $db->carregar("SELECT estuf, muncod, mundescricao as mundsc FROM territorios.municipio WHERE estuf = '" . $_REQUEST['regcod'] . "' ORDER BY mundescricao");
//    $ultimoCod = null;
//
//    echo "var listaMunicipios = new Array();\n";
//
//    foreach ($res as $unidade) {
//        if ($ultimoCod != $unidade['estuf']) {
//            echo "listaMunicipios['" , $unidade['estuf'] , "'] = new Array();";
//            $ultimoCod = $unidade['estuf'];
//        }
//
//        echo "listaMunicipios['" , $unidade['estuf'] , "'].push(new Array('" , $unidade['muncod'] , "', '" , addslashes(trim($unidade['mundsc'])) , "'));";
//    }
//}
//
//
//function pegarentidadePorCnpj($dados) {
//	global $db;
//	
//	$cp = str_replace(array('.', '-', '/'), '', $dados['entnumcpfcnpj']);
//	$sql = "SELECT * FROM entidade.entidade WHERE entnumcpfcnpj='".$cp."' and entnome = '{$dados['entnome']}' order by entid limit 1";
//	
//	$rs = $db->carregar($sql);
//	/*
//	 * Quando existem varias entidades na consulta, avisar que o filtro não esta preciso
//	 */
//	if(is_array($rs) && $rs[0]) {
//		$rs = current($rs);
//		/*
//		 * AJUSTANDO DADOS
//		 */
//		if($rs['entdatanasc']) {
//			$d = explode("-",$rs['entdatanasc']);
//			$rs['entdatanasc'] = $d[2]."/".$d[1]."/".$d[0];
//		}
//		/*
//		 * CARREGANDO AS FUNCOES 
//		 */
//		$fu = $db->carregar("SELECT fun.fundsc, ent.entnome FROM entidade.funcaoentidade fen 
//							 LEFT JOIN entidade.funcao fun ON fen.funid = fun.funid 
//							 LEFT JOIN entidade.funentassoc fea ON fen.fueid = fea.fueid  
//							 LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid 
//						 	 WHERE fen.entid='".$rs['entid']."'");
//		if($fu[0]) {
//			foreach($fu as $f) {
//				$funcoes[] = $f['fundsc']." ".(($f['entnome'])?"(".$f['entnome'].")":"");
//			}
//		}
//		
//		/*
//		 * CARREGANDO OS ENDEREÇOS
//		 */
//		$en = $db->carregar("SELECT een.*, mun.mundescricao FROM entidade.endereco een
//							 LEFT JOIN territorios.municipio mun ON mun.muncod = een.muncod 
//							 WHERE een.entid='".$rs['entid']."'");
//		
//		if($en[0]) {
//			foreach($en as $e) {
//				$enderecos[] = $e['endid']."##".$e['tpeid']."##".mascaraglobal($e['endcep'],"#####-###")."##".$e['endlog']."##".
//							   $e['endcom']."##".$e['endbai']."##".$e['muncod']."##".$e['estuf']."##".$e['endnum']."##".
//							   $e['medlatitude']."##".$e['medlongitude']."##".$e['endcomunidade']."##".$e['endzoom']."##".$e['mundescricao'];
//			}
//		}
//		
//		echo $rs['entid']."||".$rs['njuid']."||".$rs['entnumcpfcnpj']."||".$rs['entnome']."||".$rs['entemail']."||".$rs['entnuninsest']."||".
//			 $rs['entobs']."||".$rs['entnumrg']."||".$rs['entorgaoexpedidor']."||".$rs['entsexo']."||".$rs['entdatanasc']."||".
//			 $rs['entdatainiass']."||".$rs['entdatafimass']."||".$rs['entnumdddresidencial']."||".$rs['entnumresidencial']."||".
//			 $rs['entnumdddcomercial']."||".$rs['entnumramalcomercial']."||".$rs['entnumcomercial']."||".$rs['entnumdddfax']."||".
//			 $rs['entnumramalfax']."||".$rs['entnumfax']."||".$rs['tpctgid']."||".$rs['tpcid']."||".$rs['tplid']."||".$rs['tpsid']."||".
//			 $rs['entcodentsup']."||".$rs['entcodent']."||".$rs['entescolanova']."||".$rs['entsig']."||".$rs['entunicod']."||".$rs['entungcod']."||".
//			 $rs['entproep']."||".$rs['entnumdddcelular']."||".$rs['entnumcelular']."||".$rs['entorgcod']."||".$rs['entsede']."||".(($funcoes)?implode("%%",$funcoes):"naofuncoes")."||".(($enderecos)?implode("%%",$enderecos):"naoendereco");
//	} else {
//		echo "naoexiste";
//	}
//	exit;
//}