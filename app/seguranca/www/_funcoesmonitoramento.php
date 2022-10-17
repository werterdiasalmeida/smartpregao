<?
/*
 * Definições
 */
define("NU", 4);
define("NE", 2);
define("TM", 1);
define("NR", 3);
define("PE", 5);
define("MU", 6);
define("UA", 7);
define("UP", 8);

/*
 * Funções
 */

/**
 * Função utilizada para montar o painel de monitoramento
 * 
 * @author Alexandre Dourado
 * @return void função chamada por ajax
 * @param integer $dados[ano] Ano do monitoramento
 * @param integer $dados[mes] Mês do monitoramento 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 11/11/2009
 */
function monitoramentoGRID($dados) {
	global $db;
	
	// se for filtro por perído (aplicar regras) 
	if($dados['diaini'] && $dados['diafim']) {
		
		$sql = "SELECT m.sisid, m.tmoid, m.monvalor::numeric as monvalor, tp.tmoacao FROM seguranca.monitoramento m 
				LEFT JOIN seguranca.tipomonitoramento tp ON m.tmoid=tp.tmoid  
				WHERE monano='".$dados['ano']."' AND monmes='".$dados['mes']."' AND mondia>=".$dados['diaini']." AND mondia<=".$dados['diafim'];
		$respostas = $db->carregar($sql);
		
		if($respostas[0]) {
			foreach($respostas as $rsp) {
				$_ACAO[$rsp['tmoid']] = trim($rsp['tmoacao']);
				$_GRID[$rsp['sisid']][$rsp['tmoid']]+= $rsp['monvalor'];
			}
			
			foreach($_GRID as $sisid => $da) {
				foreach($da as $tmoid => $valor) {
					switch($_ACAO[$tmoid]) {
						case 'media':
							$_GRID[$sisid][$tmoid] = round($valor/((integer)$dados['diafim']-(integer)$dados['diaini']+1), 4);
							break;
					}
				}
			}
		}
	} else {
		$sql = "SELECT sisid, tmoid, monvalor::numeric as monvalor FROM seguranca.monitoramento WHERE monano='".$dados['ano']."' AND monmes='".$dados['mes']."' AND mondia IS NULL";
		$respostas = $db->carregar($sql);
		
		if($respostas[0]) {
			foreach($respostas as $rsp) {
				$_GRID[$rsp['sisid']][$rsp['tmoid']]= $rsp['monvalor'];
			}
		}
	}
	
	$_HTML .= "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";

	$sql = "SELECT sisid, sisdsc FROM seguranca.sistema WHERE sisstatus='A'";
	$sistemas = $db->carregar($sql);

	$sql = "SELECT * FROM seguranca.tipomonitoramento WHERE tmoativo='A' ORDER BY tmoordem";
	$tipomonitoramento = $db->carregar($sql);

	if($sistemas[0]) {
		
		$_HTML .= "<tr>";
		$_HTML .= "<td class='SubTituloCentro'>Ranking</td>";
		$_HTML .= "<td class='SubTituloCentro'>Módulo</td>";
		if($tipomonitoramento[0]) {
			foreach($tipomonitoramento as $tpm) {
				$_HTML .= "<td class='SubTituloCentro' title=\"".$tpm['tmodescricao']."\">".$tpm['tmosiglatipo']."</td>";
			}
		} else {
			$_HTML .= "<td>Não existem tipos de monitoramento</td>";
		}
		$_HTML .= "</tr>";
		
		unset($HTML);
		
		foreach($sistemas as $sis) {
			$HTML[$sis['sisid']] .= "<tr>";
			$HTML[$sis['sisid']] .= "<td class='SubTituloCentro'>{rankingplace}</td>";
			$HTML[$sis['sisid']] .= "<td class='SubTituloDireita'><a style=\"cursor:pointer;\" onclick=\"window.open('seguranca.php?modulo=principal/monitoramentopaineldetalhes&acao=A&sisid=".$sis['sisid']."&ano=".$dados['ano']."&mes=".$dados['mes']."','Detalhes','scrollbars=yes,height=700,width=1000,status=no,toolbar=no,menubar=no,location=no');\">".$sis['sisdsc'].":</a></td>";
			if($tipomonitoramento[0]) {
				foreach($tipomonitoramento as $tpm) {
					$_ORDEM[$tpm['tmoid']][$sis['sisid']] = $_GRID[$sis['sisid']][$tpm['tmoid']];
					$_TOTAL[$tpm['tmoid']][] = array('valor' => $_GRID[$sis['sisid']][$tpm['tmoid']], 'acao' => $tpm['tmoacao']);
					$HTML[$sis['sisid']] .= "<td align='right'>".(($_GRID[$sis['sisid']][$tpm['tmoid']])?"<b>".$_GRID[$sis['sisid']][$tpm['tmoid']]."</b>":"0")."</td>";		
				}
			} else {
				$HTML[$sis['sisid']] .= "<td>&nbsp;</td>";
			}
			$HTML[$sis['sisid']] .= "</tr>";
		}
		
		asort($_ORDEM[PE]);
		
		foreach($_ORDEM[PE] as $sisid => $indice) {
			$_ORDEM['MERGE'][(($indice)?$indice:"N")][$_ORDEM[NR][$sisid]] = $sisid;
		}
		
		$_ORDEM['FINAL'] = array();
		foreach($_ORDEM['MERGE'] as $ar) {
			krsort($ar);
			foreach($ar as $si) {
				$_ORDEM['FINAL'][] = $si;
			}
		}
		
		$rank=1;
		foreach($_ORDEM['FINAL'] as $sisid) {
			$_HTML .= str_replace("{rankingplace}", $rank."º", $HTML[$sisid]);
			$rank++; 
		}
		if($_TOTAL) {
			$_HTML .= "<tr>";
			$_HTML .= "<td class='SubTituloDireita' colspan='2'><b>Média Mensal:</b></td>";
			
			foreach($_TOTAL as $tot) {
				$totau=0;
				if($tot) {
					foreach($tot as $t) {
						$totau += $t['valor'];
					}
					
					switch(trim($t['acao'])) {
						case 'media':
							$_HTML .= "<td class='SubTituloDireita'><b>".number_format($totau/count($tot),4,',','.')."</b></td>";
							break;
						case 'soma':
							$_HTML .= "<td class='SubTituloDireita'><b>".number_format($totau,0,',','.')."</b></td>";
							break;
					}
					
				} else {
					$_HTML .= "<td class='SubTituloDireita'><b>0</b></td>";
				}
			}
			$_HTML .= "</tr>";
		}
		
	}
	$_HTML .= "</table>"; 
	
	echo $_HTML;
}


/**
 * Função utilizada para carregar as informações
 * 
 * @author Alexandre Dourado
 * @return void função chamada por ajax
 * @param integer $dados[ano] Ano do monitoramento
 * @param integer $dados[mes] Mês do monitoramento
 * @param integer $dados[tmoid] Tipo do monitoramento
 * @param integer $dados[sisid] ID do sistema 
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 11/11/2009
 */
function pegarDados($dados) {
	global $db;
	$sql = "SELECT * FROM seguranca.tipomonitoramento WHERE tmoativo='A' ORDER BY tmoordem";
	$dadostp = $db->carregar($sql);
	
	if($dadostp[0]) {
		foreach($dadostp as $tp) {
			$dadosc[$tp['tmoid']] = array();
			
			$sql = "SELECT m.tmoid, m.mondia, m.monvalor::numeric as monvalor FROM seguranca.monitoramento m 
					LEFT JOIN seguranca.tipomonitoramento tm ON tm.tmoid=m.tmoid
					WHERE m.sisid='".$dados['sisid']."' AND tm.tmoid='".$tp['tmoid']."' AND m.monano='".$dados['ano']."' AND m.monmes='".$dados['mes']."' AND m.mondia IS NOT NULL";
			
			$dadosfn = $db->carregar($sql);
			if($dadosfn[0]) {
				foreach($dadosfn as $d) {
					$dadosc[$tp['tmoid']][$d['mondia']] = $d['monvalor'];
				}
			}
		}
	}
	
	return $dadosc;
}

function pegarDiasMes($dados) {
	echo cal_days_in_month(CAL_GREGORIAN, $dados['mes'], $dados['ano']);	
}


function pegarDadosPorPagina($dados) {
	global $db;
	
	$resultado = $db->carregar("SELECT COUNT(e.oid) as num,  to_char(estdata, 'DD') as dia  FROM seguranca.estatistica e 
								INNER JOIN seguranca.menu m ON m.mnuid=e.mnuid 
								WHERE e.sisid='".$dados['sisid']."' AND (date_part('year',estdata)::varchar||date_part('month',estdata)::varchar)::varchar='".$dados['ano'].(integer)$dados['mes']."' AND m.mnulink ILIKE '%".$dados['link']."%' 
								GROUP BY to_char(estdata, 'DD') ORDER BY to_char(estdata, 'DD')");
	
	if($resultado[0]) {
		foreach($resultado as $r) {
			$result[(integer)$r['dia']] = $r['num'];
		}
		$resul[NR] = $result;
		unset($result); 
	}
	
	$resultado = $db->carregar("SELECT COUNT(DISTINCT u.usucpf) as num,  to_char(estdata, 'DD') as dia FROM seguranca.estatistica e
								INNER JOIN seguranca.menu m ON m.mnuid=e.mnuid 
								LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf 
								WHERE e.sisid='".$dados['sisid']."' AND (date_part('year',estdata)::varchar||date_part('month',estdata)::varchar)::varchar='".$dados['ano'].(integer)$dados['mes']."' AND m.mnulink ILIKE '%".$dados['link']."%' 
								GROUP BY to_char(estdata, 'DD') ORDER BY to_char(estdata, 'DD')");
	
	if($resultado[0]) {
		foreach($resultado as $r) {
			$result[(integer)$r['dia']] = $r['num'];
		}
		$resul[NU] = $result;
		unset($result); 
	}
	
	$resultado = $db->carregar("SELECT COUNT(au.oid) as num, to_char(auddata, 'DD') as dia FROM seguranca.auditoria au 
							    INNER JOIN seguranca.menu me ON au.mnuid=me.mnuid 
							    LEFT JOIN seguranca.usuario u ON u.usucpf=au.usucpf 
								WHERE me.sisid='".$dados['sisid']."' AND au.audtipo='X' AND (date_part('year',auddata)::varchar||date_part('month',auddata)::varchar)::varchar='".$dados['ano'].(integer)$dados['mes']."' AND me.mnulink ILIKE '%".$dados['link']."%'
								GROUP BY to_char(auddata, 'DD') ORDER BY to_char(auddata, 'DD')");
	
	if($resultado[0]) {
		foreach($resultado as $r) {
			$result[(integer)$r['dia']] = $r['num'];
		}
		$resul[NE] = $result;
		unset($result); 
	}
	
	$resultado = $db->carregar("SELECT ROUND(CAST(AVG(estmemusa) as numeric),2) as num, to_char(estdata, 'DD') as dia FROM seguranca.estatistica e 
						 		INNER JOIN seguranca.menu m ON m.mnuid=e.mnuid
								WHERE e.estmemusa IS NOT NULL AND e.sisid='".$dados['sisid']."' AND (date_part('year',estdata)::varchar||date_part('month',estdata)::varchar)::varchar='".$dados['ano'].(integer)$dados['mes']."' AND m.mnulink ILIKE '%".$dados['link']."%' 
								GROUP BY to_char(estdata, 'DD') ORDER BY to_char(estdata, 'DD')");
	
	if($resultado[0]) {
		foreach($resultado as $r) {
			$result[(integer)$r['dia']] = $r['num'];
		}
		$resul[MU] = $result;
		unset($result); 
	}
	
	$resultado = $db->carregar("SELECT ROUND(CAST(AVG(esttempoexec) as numeric),2) as num, to_char(estdata, 'DD') as dia FROM seguranca.estatistica e 
						 		INNER JOIN seguranca.menu m ON m.mnuid=e.mnuid
								WHERE e.sisid='".$dados['sisid']."' AND (date_part('year',estdata)::varchar||date_part('month',estdata)::varchar)::varchar='".$dados['ano'].(integer)$dados['mes']."' AND m.mnulink ILIKE '%".$dados['link']."%' 
								GROUP BY to_char(estdata, 'DD') ORDER BY to_char(estdata, 'DD')");
	
	if($resultado[0]) {
		foreach($resultado as $r) {
			$result[(integer)$r['dia']] = $r['num'];
		}
		$resul[TM] = $result;
		unset($result); 
	}
	
	return $resul;
	
}
?>