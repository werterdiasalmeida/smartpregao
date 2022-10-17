<?php

/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */
 
$_REQUEST['baselogin'] = "simec_espelho_producao";
//$_REQUEST['baselogin'] = "simec_desenvolvimento";

// carrega as funções gerais
if (!defined('APPRAIZ')) {
    define('APPRAIZ', realpath('../../') . "/");
}

require_once APPRAIZ . 'global/config.inc';
//include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once '_constantes.php';
include_once '_funcoesagendamentoindicador.php';

date_default_timezone_set ('America/Sao_Paulo');

//referente painel.coleta
define("COLETA_AUTOMATICA", 2);// tipo automatica

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$HTML .=  "Início do processamento (".date("d/m/Y h:i:s").")<br>";

if(!$_REQUEST['fecharjanela']) {
	/*
	 * 
	 */
	$sql = "SELECT * FROM painel.webservicefiles WHERE wbsstatus='A'";
	$agendwbs = $db->carregar($sql);
	
	if($agendwbs[0]) {
		foreach($agendwbs as $ag) {
			$cc = explode("_",$ag['wbsdsc']);
			$dd_wbs['indid'] = $cc[1];
			$_SESSION['indid'] = $cc[1];
			$_SESSION['usucpf'] = $ag['usucpf'];
			$_SESSION['usucpforigem'] = $ag['usucpf'];
			$dd_wbs['agddataprocessamento'] = formata_data($cc[0]);
			$dd_wbs['csvarray'] = file("../../arquivos/painel/webservice_files/".$ag['wbsdsc'].".csv");
			$html = date("d/m/Y H:i:s")." :: ".enviarAgendamentoWebService($dd_wbs);
			$sql = "UPDATE painel.webservicefiles SET wbslog=wbslog||'".addslashes($html)."', wbsstatus='P'
 					WHERE wbsid='".$ag['wbsid']."'";
			$db->executar($sql);
			$db->commit();
			$_SESSION['usucpf']='00000000191';
			$_SESSION['usucpforigem']='00000000191';
			unset($_SESSION['indid']);
			$HTML .= "Arquivo carregado do webservice '".$ag['wbsdsc'].".csv' com sucesso<br>";
			
		}
	} else {
		$HTML .= "Não existem arquivos do webservice para serem carregados<br>";
	}

	/*
	 * CRIANDO AGENDAMENTOS COLETANDO DADOS DO PROPRIO SIMEC
	 */
	if($_REQUEST['indidat']) {
		$indidat_sql = "AND i.indid='".$_REQUEST['indidat']."'";
	}
	
	$inds = $db->carregar("SELECT i.indid, c.causql, c.cautipoproc, c.caudiames, c.caudiasem FROM painel.indicador i 
						   INNER JOIN painel.cargaautomatica c ON i.indid = c.indid 
				   		   WHERE i.colid='".COLETA_AUTOMATICA."' AND i.indstatus='A' ".$indidat_sql." AND c.causql IS NOT NULL ORDER BY i.indid");
	
	if($inds[0]) {
		foreach($inds as $in) {
			if($_REQUEST['indidat']) {
				$indicadoresanalisados[] = $in['indid'];
				$consultasql[$in['indid']] = $in['causql'];
			} else {
				
				switch($in['cautipoproc']) {
					case '1':
						$indicadoresanalisados[] = $in['indid'];
						$consultasql[$in['indid']] = $in['causql'];
						break;
					case '2':
						$caudiasem = explode(";", $in['caudiasem']);
						if(in_array(date("w"), $caudiasem)) {
							$indicadoresanalisados[] = $in['indid'];
							$consultasql[$in['indid']] = $in['causql'];
						}
						break;
					case '3':
						if($in['caudiames'] == date("d")) {
							$indicadoresanalisados[] = $in['indid'];
							$consultasql[$in['indid']] = $in['causql'];
						}
						break;				
				}
				
			}
			
		}
	}
	$HTML .=  "Indicadores/consultas a serem processadas carregadas com sucesso (".date("d/m/Y h:i:s").")<br>";
	
	if($indicadoresanalisados) {
		$sql = "SELECT indid, dpeid, regid FROM painel.indicador ind 
				LEFT JOIN painel.detalheperiodicidade dpe ON dpe.perid = ind.perid 
				WHERE dpedatainicio <= '".date("Y-m-d")."' AND dpedatafim >= '".date("Y-m-d")."' 
				AND ind.indid IN('".implode("','",$indicadoresanalisados)."') AND ind.colid='".COLETA_AUTOMATICA."' ORDER BY ind.indid";
		
		$dados = $db->carregar($sql);
		if($dados[0]) {
			foreach($dados as $d) {
				$agendamentos[$d['indid']] = array("dpeid" => $d['dpeid'], "regid" => $d['regid'], "qry" => $db->carregar($consultasql[$d['indid']])); 
			}
		}
		$HTML .=  "Agendamentos automaticos carregados com sucesso (".date("d/m/Y h:i:s").")<br>";
		if($agendamentos) {
			foreach($agendamentos as $indid => $agp) {
				unset($_CACHE_PAINEL);
				switch($agp['regid']) {
					case REGIONALIZACAO_INSTITUTO:
					case REGIONALIZACAO_UNIVERSIDADE:
						if($agp['qry'][0]) {
							$sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid, agdstatus, agdprocessado)
					    			VALUES (NOW(), '".date("Y-m-d")."', '".$_SESSION['usucpforigem']."', '".$indid."', 'A', FALSE) RETURNING agdid;";
							$agdid = $db->pegaUm($sql);
							foreach($agp['qry'] as $con) {
								if($con['unicod'] && !is_null($con['dshqtde'])) {
									
									if(!$_CACHE_PAINEL[REGIONALIZACAO_UNIVERSIDADE][$con['unicod']]) {
										
										// pegando informações do polo
										$dadosuni = $db->pegaLinha("SELECT mun.muncod, mun.estuf FROM entidade.entidade ent
																	  INNER JOIN entidade.endereco ende ON ende.entid = ent.entid 
																	  INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod 
										 							  WHERE ent.entunicod='".$con['unicod']."'");
										$_CACHE_PAINEL[REGIONALIZACAO_UNIVERSIDADE][$con['unicod']] = $dadospolo;
										
									} else {
										$dadosuni = $_CACHE_PAINEL[REGIONALIZACAO_UNIVERSIDADE][$con['unicod']];
									}
									
									$sql = "INSERT INTO painel.agendamentocargadados(agdid, indid, dpeid, acdqtde, acdvalor, acdmuncod, acduf, unicod, tidid1, tidid2)
							    	 		VALUES ('".$agdid."', '".$indid."', '".(($con['dpeid'])?$con['dpeid']:$agp['dpeid'])."', '".(($con['dshqtde'])?$con['dshqtde']:"0")."', ".(($con['dshvalor'])?"'".$con['dshvalor']."'":"NULL").", ".(($dadosuni['muncod'])?"'".$dadosuni['muncod']."'":"NULL").", ".(($dadosuni['estuf'])?"'".$dadosuni['estuf']."'":"NULL").", '".$con['unicod']."', ".(($con['tidid1'])?"'".$con['tidid1']."'":"NULL").", ".(($con['tidid2'])?"'".$con['tidid2']."'":"NULL").");";
									$db->executar($sql);
								}
							}
						}
						break;
					// Regionalização POLO
					case REGIONALIZACAO_POLO:
						if($agp['qry'][0]) {
							$sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid, agdstatus, agdprocessado)
					    			VALUES (NOW(), '".date("Y-m-d")."', '".$_SESSION['usucpforigem']."', '".$indid."', 'A', FALSE) RETURNING agdid;";
							$agdid = $db->pegaUm($sql);
							foreach($agp['qry'] as $con) {
								if($con['polid'] && !is_null($con['dshqtde'])) {
									
									if(!$_CACHE_PAINEL[REGIONALIZACAO_POLO][$con['polid']]) {
										
										// pegando informações do polo
										$dadospolo = $db->pegaLinha("SELECT mun.muncod, mun.estuf FROM painel.polo pol
																	  LEFT JOIN territorios.municipio mun ON pol.muncod = mun.muncod
										 							  WHERE polid='".$con['polid']."'");
										$_CACHE_PAINEL[REGIONALIZACAO_POLO][$con['polid']] = $dadospolo;
										
									} else {
										$dadospolo = $_CACHE_PAINEL[REGIONALIZACAO_POLO][$con['polid']];
									}
									
									$sql = "INSERT INTO painel.agendamentocargadados(agdid, indid, dpeid, acdqtde, acdvalor, acdmuncod, acduf, polid, tidid1, tidid2)
							    	 		VALUES ('".$agdid."', '".$indid."', '".(($con['dpeid'])?$con['dpeid']:$agp['dpeid'])."', '".(($con['dshqtde'])?$con['dshqtde']:"0")."', ".(($con['dshvalor'])?"'".$con['dshvalor']."'":"NULL").", ".(($dadospolo['muncod'])?"'".$dadospolo['muncod']."'":"NULL").", ".(($dadospolo['estuf'])?"'".$dadospolo['estuf']."'":"NULL").", '".$con['polid']."', ".(($con['tidid1'])?"'".$con['tidid1']."'":"NULL").", ".(($con['tidid2'])?"'".$con['tidid2']."'":"NULL").");";
									$db->executar($sql);
								}
							}
						}
						break;
					// Regionalização CAMPUS SUPERIOR
					case REGIONALIZACAO_CAMPUS_SUPERIOR:
						if($agp['qry'][0]) {
							$sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid, agdstatus, agdprocessado)
					    			VALUES (NOW(), '".date("Y-m-d")."', '".$_SESSION['usucpforigem']."', '".$indid."', 'A', FALSE) RETURNING agdid;";
							$agdid = $db->pegaUm($sql);
							foreach($agp['qry'] as $con) {
								if($con['entid'] && !is_null($con['dshqtde'])) {
									
									if(!$_CACHE_PAINEL[REGIONALIZACAO_CAMPUS_SUPERIOR][$con['entid']]) {
										
										// pegando informações do campus
										$dadosentid = $db->pegaLinha("SELECT ende.muncod, ende.estuf, ent.entunicod FROM entidade.endereco ende 
																	LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ende.entid 
																	LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
																	LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid 
																	WHERE ende.entid='".$con['entid']."' AND fen.funid=18");
										
										$_CACHE_PAINEL[REGIONALIZACAO_CAMPUS_SUPERIOR][$con['entid']] = $dadosentid;
										
									} else {
										$dadosentid = $_CACHE_PAINEL[REGIONALIZACAO_CAMPUS_SUPERIOR][$con['entid']];
									}
									
									$sql = "INSERT INTO painel.agendamentocargadados(agdid, indid, dpeid, acdqtde, acdvalor, acdmuncod, acduf, entid, unicod, tidid1, tidid2)
							    	 		VALUES ('".$agdid."', '".$indid."', '".(($con['dpeid'])?$con['dpeid']:$agp['dpeid'])."', '".(($con['dshqtde'])?$con['dshqtde']:"0")."', ".(($con['dshvalor'])?"'".$con['dshvalor']."'":"NULL").", ".(($dadosentid['muncod'])?"'".$dadosentid['muncod']."'":"NULL").", ".(($dadosentid['estuf'])?"'".$dadosentid['estuf']."'":"NULL").", '".$con['entid']."', ".(($dadosentid['entunicod'])?"'".$dadosentid['entunicod']."'":"NULL").", ".(($con['tidid1'])?"'".$con['tidid1']."'":"NULL").", ".(($con['tidid2'])?"'".$con['tidid2']."'":"NULL").");";
									$db->executar($sql);
								}
							}
						}
						break;
					// Regionalização CAMPUS PROFISSIONAL
					case REGIONALIZACAO_CAMPUS_PROFISSIONAL:
						if($agp['qry'][0]) {
							$sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid, agdstatus, agdprocessado)
					    			VALUES (NOW(), '".date("Y-m-d")."', '".$_SESSION['usucpforigem']."', '".$indid."', 'A', FALSE) RETURNING agdid;";
							$agdid = $db->pegaUm($sql);
							foreach($agp['qry'] as $con) {
								if($con['entid'] && !is_null($con['dshqtde'])) {
									
									if(!$_CACHE_PAINEL[REGIONALIZACAO_CAMPUS_PROFISSIONAL][$con['entid']]) {
										
										// pegando informações da escola
										$dadosentid = $db->pegaLinha("SELECT ende.muncod, ende.estuf, ent.entunicod FROM entidade.endereco ende 
																	LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ende.entid 
																	LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
																	LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid 
																	WHERE ende.entid='".$con['entid']."' AND fen.funid=17");
										
										$_CACHE_PAINEL[REGIONALIZACAO_CAMPUS_PROFISSIONAL][$con['entid']] = $dadosentid;
										
									} else {
										$dadosentid = $_CACHE_PAINEL[REGIONALIZACAO_CAMPUS_PROFISSIONAL][$con['entid']];
									}
									
									$sql = "INSERT INTO painel.agendamentocargadados(agdid, indid, dpeid, acdqtde, acdvalor, acdmuncod, acduf, entid, unicod, tidid1, tidid2)
							    	 		VALUES ('".$agdid."', '".$indid."', '".(($con['dpeid'])?$con['dpeid']:$agp['dpeid'])."', '".(($con['dshqtde'])?$con['dshqtde']:"0")."', ".(($con['dshvalor'])?"'".$con['dshvalor']."'":"NULL").", ".(($dadosentid['muncod'])?"'".$dadosentid['muncod']."'":"NULL").", ".(($dadosentid['estuf'])?"'".$dadosentid['estuf']."'":"NULL").", '".$con['entid']."', ".(($dadosentid['entunicod'])?"'".$dadosentid['entunicod']."'":"NULL").", ".(($con['tidid1'])?"'".$con['tidid1']."'":"NULL").", ".(($con['tidid2'])?"'".$con['tidid2']."'":"NULL").");";
									$db->executar($sql);
								}
							}
						}
						break;
					// Regionalização BRASIL
					case REGIONALIZACAO_BRASIL:
						if($agp['qry'][0]) {
							$sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid, agdstatus, agdprocessado)
					    			VALUES (NOW(), '".date("Y-m-d")."', '".$_SESSION['usucpforigem']."', '".$indid."', 'A', FALSE) RETURNING agdid;";
							$agdid = $db->pegaUm($sql);
							foreach($agp['qry'] as $con) {
								if(!is_null($con['dshqtde'])) {	
									$sql = "INSERT INTO painel.agendamentocargadados(agdid, indid, dpeid, acdqtde, acdvalor, tidid1, tidid2)
							    	 		VALUES ('".$agdid."', '".$indid."', '".(($con['dpeid'])?$con['dpeid']:$agp['dpeid'])."', '".(($con['dshqtde'])?$con['dshqtde']:"0")."', ".(($con['dshvalor'])?"'".$con['dshvalor']."'":"NULL").", ".(($con['tidid1'])?"'".$con['tidid1']."'":"NULL").", ".(($con['tidid2'])?"'".$con['tidid2']."'":"NULL").");";
									$db->executar($sql);
								}
							}
						}
						break;
					// Regionalização ESCOLA
					case REGIONALIZACAO_ESCOLA:
						if($agp['qry'][0]) {
							$sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid, agdstatus, agdprocessado)
					    			VALUES (NOW(), '".date("Y-m-d")."', '".$_SESSION['usucpforigem']."', '".$indid."', 'A', FALSE) RETURNING agdid;";
							$agdid = $db->pegaUm($sql);
							
							$escolas = $db->carregar("SELECT mun.muncod, mun.estuf, esc.esccodinep FROM painel.escola esc
													  INNER JOIN territorios.municipio mun ON mun.muncod=esc.escmuncod");
							
							if($escolas[0]) {
								foreach($escolas as $escola) {
									$_CACHE_PAINEL[REGIONALIZACAO_ESCOLA][$escola['esccodinep']] = $escola;
								}
								
							}
							
							foreach($agp['qry'] as $con) {
								if($con['dshcod'] && !is_null($con['dshqtde'])) {
									
									if($_CACHE_PAINEL[REGIONALIZACAO_ESCOLA][trim($con['dshcod'])]) {
										$dadosescola = $_CACHE_PAINEL[REGIONALIZACAO_ESCOLA][trim($con['dshcod'])];
									}
									
									$sql = "INSERT INTO painel.agendamentocargadados(agdid, indid, dpeid, acdqtde, acdvalor, acdesciescod, acdmuncod, acduf, tidid1, tidid2)
							    	 		VALUES ('".$agdid."', '".$indid."', '".(($con['dpeid'])?$con['dpeid']:$agp['dpeid'])."', '".(($con['dshqtde'])?$con['dshqtde']:"0")."', ".(($con['dshvalor'])?"'".$con['dshvalor']."'":"NULL").", '".$con['dshcod']."', '".$dadosescola['muncod']."', '".$dadosescola['estuf']."', ".(($con['tidid1'])?"'".$con['tidid1']."'":"NULL").", ".(($con['tidid2'])?"'".$con['tidid2']."'":"NULL").");";
									$db->executar($sql);
								}
							}
							unset($_CACHE_PAINEL);
						}
						break;
					// Regionalização MUNICIPIO
					case REGIONALIZACAO_MUN:
						if($agp['qry'][0]) {
							$sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid, agdstatus, agdprocessado)
					    			VALUES (NOW(), '".date("Y-m-d")."', '".$_SESSION['usucpforigem']."', '".$indid."', 'A', FALSE) RETURNING agdid;";
							$agdid = $db->pegaUm($sql);
							foreach($agp['qry'] as $con) {
								if($con['dshcodmunicipio'] && !is_null($con['dshqtde'])) {
									
									if(!$_CACHE_PAINEL[REGIONALIZACAO_MUN][$con['dshcodmunicipio']]) {
										// pegando informações do municipio
										$estuf = $db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$con['dshcodmunicipio']."'");
										$_CACHE_PAINEL[REGIONALIZACAO_MUN][$con['dshcodmunicipio']] = $estuf;
									} else {
										$estuf = $_CACHE_PAINEL[REGIONALIZACAO_MUN][$con['dshcodmunicipio']];
									}
									$sql = "INSERT INTO painel.agendamentocargadados(agdid, indid, dpeid, acdqtde, acdvalor, acdmuncod, acduf, tidid1, tidid2)
							    	 		VALUES ('".$agdid."', '".$indid."', '".(($con['dpeid'])?$con['dpeid']:$agp['dpeid'])."', '".(($con['dshqtde'])?$con['dshqtde']:"0")."', ".(($con['dshvalor'])?"'".$con['dshvalor']."'":"NULL").", '".$con['dshcodmunicipio']."', '".$estuf."', ".(($con['tidid1'])?"'".$con['tidid1']."'":"NULL").", ".(($con['tidid2'])?"'".$con['tidid2']."'":"NULL").");";
									$db->executar($sql);
								}
							}
						}
						break;
					// Regionalização ESTADO
					case REGIONALIZACAO_UF:
						if($agp['qry'][0]) {
							$sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid, agdstatus, agdprocessado)
							 		VALUES (NOW(), '".date("Y-m-d")."', '".$_SESSION['usucpforigem']."', '".$indid."', 'A', FALSE) RETURNING agdid;";
							$agdid = $db->pegaUm($sql);
							foreach($agp['qry'] as $con) {
								if($con['dshuf'] && !is_null($con['dshqtde'])) {
									$sql = "INSERT INTO painel.agendamentocargadados(agdid, indid, dpeid, acdqtde, acdvalor, acduf, tidid1, tidid2)
							    	 		VALUES ('".$agdid."', '".$indid."', '".(($con['dpeid'])?$con['dpeid']:$agp['dpeid'])."', '".(($con['dshqtde'])?$con['dshqtde']:"0")."', ".(($con['dshvalor'])?"'".$con['dshvalor']."'":"NULL").", '".$con['dshuf']."', ".(($con['tidid1'])?"'".$con['tidid1']."'":"NULL").", ".(($con['tidid2'])?"'".$con['tidid2']."'":"NULL").");";
									$db->executar($sql);
								}
							}
						}
						break;
					// Regionalização REGIÃO
					case REGIONALIZACAO_REGIAO:
						if($agp['qry'][0]) {
							$sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid, agdstatus, agdprocessado)
							 		VALUES (NOW(), '".date("Y-m-d")."', '".$_SESSION['usucpforigem']."', '".$indid."', 'A', FALSE) RETURNING agdid;";
							$agdid = $db->pegaUm($sql);
							foreach($agp['qry'] as $con) {
								if($con['dshregiao'] && !is_null($con['dshqtde'])) {
									$sql = "INSERT INTO painel.agendamentocargadados(agdid, indid, dpeid, acdqtde, acdvalor, acdregiao, tidid1, tidid2)
							    	 		VALUES ('".$agdid."', '".$indid."', '".(($con['dpeid'])?$con['dpeid']:$agp['dpeid'])."', '".(($con['dshqtde'])?$con['dshqtde']:"0")."', ".(($con['dshvalor'])?"'".$con['dshvalor']."'":"NULL").", '".$con['dshregiao']."', ".(($con['tidid1'])?"'".$con['tidid1']."'":"NULL").", ".(($con['tidid2'])?"'".$con['tidid2']."'":"NULL").");";
									$db->executar($sql);
								}
							}
						}
						break;
					// Regionalização IES
					case REGIONALIZACAO_IES:
						if($agp['qry'][0]) {
							$sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid, agdstatus, agdprocessado)
						   			VALUES (NOW(), '".date("Y-m-d")."', '".$_SESSION['usucpforigem']."', '".$indid."', 'A', FALSE) RETURNING agdid;";
							$agdid = $db->pegaUm($sql);
							foreach($agp['qry'] as $con) {
								if($con['dshcod'] && !is_null($con['dshqtde'])) {
									if(!$_CACHE_PAINEL[REGIONALIZACAO_IES][$con['dshcod']]) {
										
										// pegando informações da escola
										$dadosinstitui = $db->pegaLinha("SELECT mun.muncod, mun.estuf FROM painel.ies ies
										 							     INNER JOIN territorios.municipio mun ON mun.muncod=ies.iesmuncod 
										 							     WHERE iesid='".$con['dshcod']."'");
										
										$_CACHE_PAINEL[REGIONALIZACAO_IES][$con['dshcod']] = $dadosinstitui;
										
									} else {
										$dadosinstitui = $_CACHE_PAINEL[REGIONALIZACAO_IES][$con['dshcod']];
									}
									
									
									$sql = "INSERT INTO painel.agendamentocargadados(agdid, indid, dpeid, acdqtde, acdvalor, acdesciescod, acdmuncod, acduf, tidid1, tidid2)
							    	 		VALUES ('".$agdid."', '".$indid."', '".(($con['dpeid'])?$con['dpeid']:$agp['dpeid'])."', '".(($con['dshqtde'])?$con['dshqtde']:"0")."', ".(($con['dshvalor'])?"'".$con['dshvalor']."'":"NULL").", '".$con['dshcod']."', '".$dadosinstitui['muncod']."', '".$dadosinstitui['estuf']."', ".(($con['tidid1'])?"'".$con['tidid1']."'":"NULL").", ".(($con['tidid2'])?"'".$con['tidid2']."'":"NULL").");";
									$db->executar($sql);
								}
							}
						}
						break;
						
					// Regionalização IES CPC
					case REGIONALIZACAO_IESCPC:
						if($agp['qry'][0]) {
							$sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid, agdstatus, agdprocessado)
						   			VALUES (NOW(), '".date("Y-m-d")."', '".$_SESSION['usucpforigem']."', '".$indid."', 'A', FALSE) RETURNING agdid;";
							$agdid = $db->pegaUm($sql);
							foreach($agp['qry'] as $con) {
								if($con['iecid'] && !is_null($con['dshqtde'])) {
									if(!$_CACHE_PAINEL[REGIONALIZACAO_IESCPC][$con['iecid']]) {
										// pegando informações da escola
										$dadosinstitui = $db->pegaLinha("SELECT mun.muncod, mun.estuf FROM painel.iescpc iescpc
										 							     INNER JOIN territorios.municipio mun ON mun.muncod=iescpc.muncod 
										 							     WHERE iecid='".$con['iecid']."'");
										
										$_CACHE_PAINEL[REGIONALIZACAO_IESCPC][$con['iecid']] = $dadosinstitui;
										
									} else {
										$dadosinstitui = $_CACHE_PAINEL[REGIONALIZACAO_IESCPC][$con['iecid']];
									}
									
									
									$sql = "INSERT INTO painel.agendamentocargadados(agdid, indid, dpeid, acdqtde, acdvalor, iecid, acdmuncod, acduf, tidid1, tidid2)
							    	 		VALUES ('".$agdid."', '".$indid."', '".(($con['dpeid'])?$con['dpeid']:$agp['dpeid'])."', '".(($con['dshqtde'])?$con['dshqtde']:"0")."', ".(($con['dshvalor'])?"'".$con['dshvalor']."'":"NULL").", '".$con['iecid']."', '".$dadosinstitui['muncod']."', '".$dadosinstitui['estuf']."', ".(($con['tidid1'])?"'".$con['tidid1']."'":"NULL").", ".(($con['tidid2'])?"'".$con['tidid2']."'":"NULL").");";
									$db->executar($sql);
								}
							}
						}
						break;
						
					case REGIONALIZACAO_HOSPITAL:
						if($agp['qry'][0]) {
							$sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid, agdstatus, agdprocessado)
						   			VALUES (NOW(), '".date("Y-m-d")."', '".$_SESSION['usucpforigem']."', '".$indid."', 'A', FALSE) RETURNING agdid;";
							$agdid = $db->pegaUm($sql);
							foreach($agp['qry'] as $con) {
								if($con['entid'] && !is_null($con['dshqtde'])) {
									if(!$_CACHE_PAINEL[REGIONALIZACAO_HOSPITAL][$con['entid']]) {
										
										// pegando informações da escola
										$dadoshospital = $db->pegaLinha("SELECT ende.estuf, ende.muncod FROM entidade.endereco ende WHERE entid='".$con['entid']."'");
										$_CACHE_PAINEL[REGIONALIZACAO_HOSPITAL][$con['entid']] = $dadoshospital;
										
									} else {
										$dadoshospital = $_CACHE_PAINEL[REGIONALIZACAO_HOSPITAL][$con['entid']];
										
									}
									
									
									$sql = "INSERT INTO painel.agendamentocargadados(agdid, indid, dpeid, acdqtde, acdvalor, entid, acdmuncod, acduf, tidid1, tidid2)
							    	 		VALUES ('".$agdid."', '".$indid."', '".(($con['dpeid'])?$con['dpeid']:$agp['dpeid'])."', '".(($con['dshqtde'])?$con['dshqtde']:"0")."', ".(($con['dshvalor'])?"'".$con['dshvalor']."'":"NULL").", '".$con['entid']."', '".$dadoshospital['muncod']."', '".$dadoshospital['estuf']."', ".(($con['tidid1'])?"'".$con['tidid1']."'":"NULL").", ".(($con['tidid2'])?"'".$con['tidid2']."'":"NULL").");";
									$db->executar($sql);
								}
							}
						}
						break;
						
				}
			}
			$db->commit();
		}
	}
	$HTML .=  "Agendamentos automaticos processados com sucesso (".date("d/m/Y h:i:s").")<br>";
	/*
	 * FIM
	 * CRIANDO AGENDAMENTOS COLETANDO DADOS DO PROPRIO SIMEC
	 */
}

$sql = $_REQUEST['agora'] 
		? 
		"SELECT * FROM painel.agendamentocarga WHERE agdstatus='A' AND agdprocessado=false AND agdid='".$_REQUEST['agdid']."'"
		:
		"SELECT * FROM painel.agendamentocarga WHERE (agddataprocessamento='".(($_REQUEST['dataprocessamento'])?$_REQUEST['dataprocessamento']:date("Y-m-d"))."' OR agddataprocessamento= DATE(NOW() - INTERVAL '1 DAY')) AND agdstatus='A' AND agdprocessado=false ".(($_REQUEST['agdid'])?"AND agdid='".$_REQUEST['agdid']."'":"");
$agendamentos = $db->carregar($sql);

/*
 * CARREGANDO TODOS OS AGENDAMENTOS A SEREM PROCESSADOS NAQUELE DIA
 *  
 */
if($agendamentos[0]) {
	foreach($agendamentos as $agen) {
		$sql = "SELECT DISTINCT COALESCE(agd.indid,0)||'.'||COALESCE(agd.dpeid,0)||'.'||COALESCE(agd.ddiid,0)||'.'||COALESCE(agd.acdmuncod,'0')||'.'||COALESCE(agd.acduf,'0')||'.'||COALESCE(agd.acdregiao,'0')||'.'||COALESCE(agd.acdesciescod,'0')||'.'||COALESCE(agd.tidid1,'0')||'.'||COALESCE(agd.tidid2,'0')||'.'||COALESCE(agd.iepid,'0')||'.'||COALESCE(agd.entid,'0')||'.'||COALESCE(agd.unicod,'0')||'.'||COALESCE(agd.polid,'0')||'.'||COALESCE(agd.iecid,'0') as identificador, * FROM painel.agendamentocargadados agd 
				LEFT JOIN painel.indicador ind ON ind.indid = agd.indid 
				WHERE agdid='".$agen['agdid']."'";
		
		$dadosagendamento = $db->carregar($sql);
		
		unset($dadosconsolidados, $dadosduplicados);
		/*
		 * CARREGANDO OS REGISTROS DO AGENDAMENTOS
		 */
		if($dadosagendamento[0]) {
			/*
			 * ARMAZENANDO OS DADOS DO AGENDAMENTO POR INDICADOR E PERÍODO DETALHES
			 */
			$dadosduplicados = false;
			foreach($dadosagendamento as $dadosagen) {
				$tot += $dadosagen['acdqtde'];
				if(!$dadosconsolidados[$dadosagen['indid']][$dadosagen['dpeid']][$dadosagen['identificador']]) {
					$dadosconsolidados[$dadosagen['indid']][$dadosagen['dpeid']][$dadosagen['identificador']] = $dadosagen;
				} else {
					$dadosduplicados[] = array('indid' => $dadosagen['indid'], 'dpeid' => $dadosagen['dpeid'], 'identificador' => $dadosagen['identificador']);
				}
			
			}

			/*
			 * VARRENDO OS DADOS E APLICANDO AS REGRAS
			 */
//$arrdados = array();
			if($dadosconsolidados) {
				foreach($dadosconsolidados as $indid => $valores1) {
//dbg($valores1);
					foreach($valores1 as $dpeid => $valores2) {
						
						$regidseh = $db->pegaUm("SELECT regid FROM painel.indicador WHERE indid='".$indid."'");
						
						$sql = "INSERT INTO painel.seriehistorica(indid, sehstatus, dpeid, sehdtcoleta, sehqtde, regid)
	    				 		VALUES ('".$indid."', 'H', '".$dpeid."', NOW(), '0',".(($regidseh)?"'".$regidseh."'":"NULL").") RETURNING sehid;";
						
						$sehid = $db->pegaUm($sql);
						
						if($valores2) {
							foreach($valores2 as $vlrdetalhe) {
								$valorcumulativo += $vlrdetalhe['acdvalor'];
								$qtdecumulativo += $vlrdetalhe['acdqtde'];
								$sqls = "INSERT INTO painel.detalheseriehistorica(
								            ddiid, sehid, dshvalor, dshcod, dshcodmunicipio, dshuf, dshqtde, tidid1, tidid2, iepid, entid, unicod, polid, iecid, dshregiao)
								    	VALUES (".(($vlrdetalhe['ddiid'])?"'".$vlrdetalhe['ddiid']."'":"NULL").", '".$sehid."', ".(($vlrdetalhe['acdvalor'])?"'".$vlrdetalhe['acdvalor']."'":"NULL").",
								    			".(($vlrdetalhe['acdesciescod'])?"'".$vlrdetalhe['acdesciescod']."'":"NULL").", ".(($vlrdetalhe['acdmuncod'])?"'".$vlrdetalhe['acdmuncod']."'":"NULL").", 
								    			".(($vlrdetalhe['acduf'])?"'".$vlrdetalhe['acduf']."'":"NULL").", ".((trim($vlrdetalhe['acdqtde']))?trim($vlrdetalhe['acdqtde']):"0").", 
								    			".(($vlrdetalhe['tidid1'])?"'".$vlrdetalhe['tidid1']."'":"NULL").", ".(($vlrdetalhe['tidid2'])?"'".$vlrdetalhe['tidid2']."'":"NULL").",
								    			".(($vlrdetalhe['iepid'])?"'".$vlrdetalhe['iepid']."'":"NULL").", ".(($vlrdetalhe['entid'])?"'".$vlrdetalhe['entid']."'":"NULL").",
								    			".(($vlrdetalhe['unicod'])?"'".$vlrdetalhe['unicod']."'":"NULL").", ".(($vlrdetalhe['polid'])?"'".$vlrdetalhe['polid']."'":"NULL").", 
								    			".(($vlrdetalhe['iecid'])?"'".$vlrdetalhe['iecid']."'":"NULL").", ".(($vlrdetalhe['acdregiao'])?"'".$vlrdetalhe['acdregiao']."'":"NULL").")";
//$arrdados[] = $sqls;
								$db->executar($sqls, false);
							}
						}
						$db->executar("UPDATE painel.seriehistorica SET sehqtde=(SELECT sum(qtde) FROM painel.v_detalheindicadorsh WHERE sehid='".$sehid."'), sehvalor=(SELECT sum(valor) FROM painel.v_detalheindicadorsh WHERE sehid='".$sehid."') WHERE sehid='".$sehid."'");
						
					}

					/*
					 * limpando series historicas
					 */
					$sql = "UPDATE painel.seriehistorica SET sehstatus='H' WHERE indid='".$indid."' AND sehstatus='A'";
					$db->executar($sql);
					$sql = "SELECT seh.sehid, seh.dpeid FROM painel.seriehistorica seh 
							LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
							WHERE indid='".$indid."' AND (sehstatus='A' OR sehstatus='H') ORDER BY dpedatainicio DESC, sehid DESC LIMIT 1";
					$seriemaior = $db->pegaLinha($sql);
					
					$sql = "UPDATE painel.seriehistorica SET sehstatus='A' WHERE sehid='".$seriemaior['sehid']."'";
					$db->executar($sql);
					
					// Apaga dados do detalhe serie historica
					$sql = "DELETE FROM painel.detalheseriehistorica WHERE sehid in (SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$seriemaior['dpeid']."' AND indid='".$indid."' AND sehid!='".$seriemaior['sehid']."')";
					$db->executar($sql);

					$sql = "DELETE FROM painel.seriehistorica WHERE dpeid='".$seriemaior['dpeid']."' AND indid='".$indid."' AND sehid!='".$seriemaior['sehid']."'";
					$db->executar($sql);

					$sql = "SELECT * FROM painel.seriehistorica WHERE indid='".$indid."' AND sehstatus='H' ORDER BY sehid DESC";
					$serieoutros = $db->carregar($sql);
					unset($dpe);
					if($serieoutros[0]) {
						foreach($serieoutros as $ser) {
							if($dpe[$ser['dpeid']]) {
								$sql = "DELETE FROM painel.tipoconteudografico WHERE sehid='".$ser['sehid']."'";
								$db->executar($sql);
								$sql = "DELETE FROM painel.detalheseriehistorica WHERE sehid='".$ser['sehid']."'";
								$db->executar($sql);
								$sql = "DELETE FROM painel.seriehistorica WHERE sehid='".$ser['sehid']."'";
								$db->executar($sql);
							} else {
								$dpe[$ser['dpeid']]=true;
							}
						}
					}
					/*
					 * limpando series historicas
					 */
				}
//dbg($arrdados,1);
			}
			$db->executar("UPDATE painel.agendamentocarga SET agdprocessado=true WHERE agdid='".$dadosagen['agdid']."'");
			$db->commit();
			
			$HTML .=  "Agendamento #".$dadosagen['agdid']."(ind.".$indid.") foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
		} else {
			$HTML .=  "O agendamento #".$agen['agdid']." não possui dados<br>";
			$db->executar("UPDATE painel.agendamentocarga SET agdprocessado=true WHERE agdid='".$agen['agdid']."'");
			$db->commit();
		}
	}
} else {
	$HTML .= "Não existem agendamentos ate a data '".date("d/m/Y h:i:s")."'<br>";
}

if($_REQUEST['fecharjanela']==true) {
	echo "<script>
			alert('Processamento efetuado com sucesso.');
			".($_REQUEST['agora'] ? "window.opener.ajaxatualizar('requisicao=carregaragendamento','listaagendamento');" : "")."
			window.close();
		  </script>";
} else {
	$db->executar("delete from painel.agendamentocargadados where agdid in(select agdid from painel.agendamentocarga where agdstatus='I' or agdprocessado is true)");
	$HTML .=  "DELETE TBL_agendamentocargadados foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	$db->executar("delete from painel.detalheseriehistorica where dshid in(select d.dshid from painel.seriehistorica s inner join painel.detalheseriehistorica d on s.sehid=d.sehid where sehstatus='I')");
	$HTML .=  "DELETE TBL_detalheseriehistorica foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	$db->executar("delete from painel.tipoconteudografico where sehid in(select sehid from painel.seriehistorica where sehstatus='I')");
	$HTML .=  "DELETE TBL_tipoconteudografico foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	$db->executar("delete from painel.seriehistorica where sehstatus='I'");
	$HTML .=  "DELETE TBL_seriehistorica foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
	$db->commit();
	if(!$_REQUEST['indidat']) {
		$db->executar("REINDEX INDEX painel.idx_detalheseriehistorica_entid;");
		$HTML .=  "REINDEX idx_detalheseriehistorica_entid foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
		$db->executar("REINDEX INDEX painel.idx_dshsehid;");
		$HTML .=  "REINDEX idx_dshsehid foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
		$db->executar("REINDEX INDEX painel.idx_detalheseriehistorica_iepid;");
		$HTML .=  "REINDEX idx_detalheseriehistorica_iepid foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
		$db->executar("REINDEX INDEX painel.idx_detalheseriehistorica_unicod;");
		$HTML .=  "REINDEX idx_detalheseriehistorica_unicod foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
		$db->executar("REINDEX INDEX painel.ix_detalheserirhistorica_dshcod;");
		$HTML .=  "REINDEX ix_detalheserirhistorica_dshcod foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
		$db->executar("REINDEX INDEX painel.ix_detalheserirhistorica_dshcodmunicipio;");
		$HTML .=  "REINDEX ix_detalheserirhistorica_dshcodmunicipio foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
		$db->executar("REINDEX INDEX painel.ix_detalheserirhistorica_dshuf;");
		$HTML .=  "REINDEX ix_detalheserirhistorica_dshuf foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
		$db->executar("REINDEX INDEX painel.ix_detalheserirhistorica_sehiddshcodmunicipio;");
		$HTML .=  "REINDEX ix_detalheserirhistorica_sehiddshcodmunicipio foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
		$db->executar("REINDEX INDEX painel.ix_detalheserirhistorica_sehiddshuf;");
		$HTML .=  "REINDEX ix_detalheserirhistorica_sehiddshuf foi processado com sucesso (".date("d/m/Y h:i:s").")<br>";
		$db->commit();
	}
	
	/*
	 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
	 */
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= "SISTEMA DE PROCESSAMENTO DE AGENDAMENTOS";
	$mensagem->From 		= "simec@mec.gov.br";
	$mensagem->AddAddress("alexandre.dourado@mec.gov.br", "Alexandre Dourado");
	$mensagem->AddAddress("vitor.sad@mec.gov.br"        , "Vitor Nunes Sad");
	$mensagem->AddAddress("cristiano.cabral@mec.gov.br" , "Cristiano Cabral");
	$mensagem->Subject = "Processamento dos agendamentos";
	
	ob_start();
	echo "<pre>";
	print_r($_SERVER);
	$dadosserv = ob_get_contents();
	ob_end_clean();
	
	$mensagem->Body = $inicioscript."<br /><br />".$HTML."<br /><br />".$dadosserv;
	$mensagem->IsHTML( true );
	$mensagem->Send();
	/*
	 * FIM
	 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
	 */
}


?>