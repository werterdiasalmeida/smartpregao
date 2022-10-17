<?
/**
 * Fun��o que carrega os detalhes dos indicadores
 * 
 * @author Alexandre Dourado
 * @return htmlcode (AJAX)
 * @param integer $indid
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarDetalhesTipoIndicador($dados) {
	global $db, $permissoes;
	
	if($permissoes['verindicadores'] == 'vertodos') {
		$acesso = true;
	} else {
		$acesso = validaAcessoIndicadores($permissoes['verindicadores'], $_SESSION['indid']);
	}
	
	// verificando se possui permiss�o para gerenciar detalhe indicador
	if($acesso) {
		$acoes = "<img src=\"../imagens/gif_inclui.gif\" align=\"absmiddle\" onclick=\"inserirDetalheTipoDadosIndicador('||tdiid||', this);\"> <img src=\"/imagens/alterar.gif\" align=\"absmiddle\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"editarDetalheTipoIndicador('||tdiid||', this);\"> <img src=\"/imagens/excluir.gif\" align=\"absmiddle\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"excluirDetalheTipoIndicador('||tdiid||');\">";
	} else {
		$acoes = "&nbsp;";
	}
	
	$sql = "SELECT '<center><img src=\"../imagens/mais.gif\" align=\"absmiddle\" title=\"mais\" id=\"imagem'||tdiid||'\" onclick=\"carregarDetalheTipoDadosIndicador('||tdiid||', this);\"></center>' as mais, 
	        	   '<center>".$acoes."</center>' as acoes,
	 			   'N�vel '||COALESCE(tdiordem,0) as nivel, 
				   tdidsc, 
				   '<img id=\"setasubir\" src=\"../imagens/seta_cima.gif\" onclick=\"ordenar('||tdiid||',\'subir\');\"> <img id=\"setadescer\" src=\"../imagens/seta_baixo.gif\" onclick=\"ordenar('||tdiid||',\'descer\');\">' 
			FROM painel.detalhetipoindicador 
			WHERE indid='".$_SESSION['indid']."' AND tdistatus='A' ORDER BY tdiordem";
	
	// Op��o para retornar o n�mero de registros
	if($dados['numeroregistro']) {
		$registros = $db->carregar($sql);
		echo count($registros);
		exit;
	}
	$cabecalho = array('&nbsp;','A��es', 'N�vel', 'Descri��o','Ordena��o');
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%',$par2);
	exit;
}
/**
 * Fun��o que verifica se o indicador possui dados inseridos
 * 
 * @author Alexandre Dourado
 * @return bolean
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function verificarExisteDados($dados) {
	global $db;
	echo $db->pegaUm("SELECT round(sum(qtde)) FROM painel.v_detalheindicadorsh 
			 		  WHERE indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	exit;
}

/**
 * Fun��o que insere os detalhes indicador (painel.detalhetipoindicador)
 * 
 * @author Alexandre Dourado
 * @return void
 * @param string $dados['tdidsc']
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function inserirDetalhesTipoIndicador($dados) {
	global $db;
	$ordem = $db->pegaUm("SELECT tdiordem FROM painel.detalhetipoindicador WHERE indid='".$_SESSION['indid']."' ORDER BY tdiordem DESC LIMIT 1");
	$numero = $db->pegaUm("SELECT tdinumero FROM painel.detalhetipoindicador WHERE indid='".$_SESSION['indid']."' ORDER BY tdinumero DESC LIMIT 1");
	// processando logica do n�mero
	switch($numero) {
		case '1':
			$numero = '2';
			break;
		case '2':
		default:
			$numero = '1';
			break;
	}
	
	$sql = "INSERT INTO painel.detalhetipoindicador(
            indid, tdidsc, tdistatus, tdiordem, tdinumero)
    		VALUES ('".$_SESSION['indid']."', '".utf8_decode($dados['tdidsc'])."', 'A', '".(($ordem)?($ordem+1):'1')."', '".$numero."');";
	
	$db->executar($sql);
	$db->commit();
	exit;
}
/**
 * Fun��o que insere os dados dos detalhes indicador (painel.detalhetipodadosindicador)
 * 
 * @author Alexandre Dourado
 * @return htmlcode (AJAX)
 * @param integer $indid
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function inserirDetalhesTipoDadosIndicador($dados) {
	global $db;
	$sql = "INSERT INTO painel.detalhetipodadosindicador(
            tdiid, tiddsc, tidstatus)
    		VALUES ('".$dados['tdiid']."', '".utf8_decode($dados['tiddsc'])."', 'A');";
	$db->executar($sql);
	$db->commit();
	exit;
}
/**
 * Fun��o que remover registro os tipos de detalhes dos indicadores
 * Esta fun��o preve apenas 2 detalhes (reajustando manualmente a ordem para um)
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['tdiid']
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function removerDetalhesTipoIndicador($dados) {
	global $db;
	$db->executar("DELETE FROM painel.detalhetipodadosindicador WHERE tdiid='".$dados['tdiid']."'");
	$db->executar("DELETE FROM painel.detalhetipoindicador WHERE tdiid='".$dados['tdiid']."'");
	// ajustando a ordem quando um detalhe for deletado
	$db->executar("UPDATE painel.detalhetipoindicador SET tdiordem='1' WHERE indid='".$_SESSION['indid']."'");
	$db->commit();
	exit;
}

function removerDetalhesTipoDadosIndicador($dados) {
	global $db;
	$db->executar("DELETE FROM painel.agendamentocargadados WHERE tidid1='".$dados['tidid']."' OR tidid2='".$dados['tidid']."'");
	$db->executar("DELETE FROM painel.detalhetipodadosindicador WHERE tidid='".$dados['tidid']."'");
	$db->commit();
	exit;
}

function ordenarDetalhesTipoIndicador($dados) {
	global $db;
	switch($dados['executar']) {
		case 'subir':
			$sql = "SELECT tdiid FROM painel.detalhetipoindicador 
					WHERE indid=(SELECT indid FROM painel.detalhetipoindicador WHERE tdiid='".$dados['tdiid']."') AND 
						  tdiordem<(SELECT tdiordem FROM painel.detalhetipoindicador WHERE tdiid='".$dados['tdiid']."') ORDER BY tdiordem DESC LIMIT 1";
			$tdiid_ir = $db->pegaUm($sql);
			break;
		case 'descer':
			$sql = "SELECT tdiid FROM painel.detalhetipoindicador 
					WHERE indid=(SELECT indid FROM painel.detalhetipoindicador WHERE tdiid='".$dados['tdiid']."') AND 
						  tdiordem>(SELECT tdiordem FROM painel.detalhetipoindicador WHERE tdiid='".$dados['tdiid']."') ORDER BY tdiordem ASC LIMIT 1";
			$tdiid_ir = $db->pegaUm($sql);
			break;
	}
	if($tdiid_ir) {
		$ordem_ir    = $db->pegaUm("SELECT tdiordem FROM painel.detalhetipoindicador WHERE tdiid='".$tdiid_ir."'");
		$ordem_atual = $db->pegaUm("SELECT tdiordem FROM painel.detalhetipoindicador WHERE tdiid='".$dados['tdiid']."'");
		$db->executar("UPDATE painel.detalhetipoindicador SET tdiordem='".$ordem_ir."' WHERE tdiid='".$dados['tdiid']."'");
		$db->executar("UPDATE painel.detalhetipoindicador SET tdiordem='".$ordem_atual."' WHERE tdiid='".$tdiid_ir."'");
		$db->commit();
	}
	exit;
}

function atualizarDetalhesTipoIndicador($dados) {
	global $db;
	$sql = "UPDATE painel.detalhetipoindicador
     		SET tdidsc='".utf8_decode($dados['tdidsc'])."'
     		WHERE tdiid='".$dados['tdiid']."'";
	$db->executar($sql);
	$db->commit();
	exit;
}
function atualizarDetalhesTipoDadosIndicador($dados) {
	global $db;
	$sql = "UPDATE painel.detalhetipodadosindicador
   			SET tiddsc='".utf8_decode($dados['tiddsc'])."'
 			WHERE tidid='".$dados['tidid']."'";
	$db->executar($sql);
	$db->commit();
	exit;
}

function carregarDetalhesTipoDadosIndicador($dados) {
	global $db, $permissoes;
	
	if($permissoes['verindicadores'] == 'vertodos') {
		$acesso = true;
	} else {
		$acesso = validaAcessoIndicadores($permissoes['verindicadores'], $_SESSION['indid']);
	}
	
	// verificando se possui permiss�o para gerenciar detalhe indicador
	if($acesso) {
		$acoes = "<img src=\"/imagens/alterar.gif\" align=\"absmiddle\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"editarDetalheTipoDadosIndicador('||tidid||', '||tdiid||', this);\"> <img src=\"/imagens/excluir.gif\" align=\"absmiddle\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"excluirDetalheTipoDadosIndicador('||tidid||','||tdiid||', this)\">";
	} else {
		$acoes = "&nbsp;";
	}
	
	$sql = "SELECT '<center>".$acoes."</center>' as acoes, tiddsc FROM painel.detalhetipodadosindicador WHERE tdiid='".$dados['tdiid']."' ORDER BY tiddsc";
	$tipodetalhe = $db->pegaUm("SELECT tdidsc FROM painel.detalhetipoindicador WHERE tdiid='".$dados['tdiid']."'");
	$cabecalho = array("A��es", $tipodetalhe);
	$db->monta_lista_simples($sql,$cabecalho,250,5,'N','100%',$par2);
	exit;
}

function carregarComboMunicipioPorUF($dados) {
	global $db;
	$sql = "SELECT muncod as codigo, mundescricao||'(0)' as descricao FROM territorios.municipio WHERE estuf='".$dados['estuf']."' ORDER BY mundescricao";
	$listamun = $db->carregar($sql);
	if($listamun[0]) {
		foreach($listamun as $mun) {
			$listafim[$mun['codigo']] = $mun; 
		}
	}
	
	$sql = str_replace(array("{condicao_dpeid}","{condicao_estuf}"),array("dpeid='".$dados['dpeid']."' AND","mun.estuf='".$dados['estuf']."' AND"),base64_decode($dados['consulta']));
	$lista = $db->carregar($sql);
	if($lista[0]) {
		foreach($lista as $op) {
			$listafim[$op['codigo']] = $op; 
		}
	}
	
	if($listafim) {
		$option = "<option value=''>Selecione</option>";
		foreach($listafim as $op) {
			$option .= "<option value='".$op['codigo']."'>".$op['descricao']."</option>"; 
		}
	}
	echo $option;
	exit;
}

function carregarComboUFPorPeriodo($dados) {
	global $db;
	$sql = "SELECT estuf as codigo, estdescricao||'(0)' as descricao FROM territorios.estado";
	$listaest = $db->carregar($sql);
	if($listaest[0]) {
		foreach($listaest as $est) {
			$listafim[$est['codigo']] = $est; 
		}
	}
	$sql = str_replace(array("{condicao_dpeid}"),array("dpeid='".$dados['dpeid']."' AND"),base64_decode($dados['consulta']));
	$lista = $db->carregar($sql);
	if($lista[0]) {
		foreach($lista as $op) {
			$listafim[$op['codigo']] = $op; 
		}
	}
	if($listafim) {
		$option = "<option value=''>Selecione</option>";
		foreach($listafim as $lst) {
			$option .= "<option value='".$lst['codigo']."'>".$lst['descricao']."</option>"; 
		}
	}
	echo $option;
	exit;
}


function detalhetipoindicador() {
	global $db;
	$sql = "SELECT tdi.*, dti.tdinumero FROM painel.detalhetipoindicador dti 
			INNER JOIN painel.detalhetipodadosindicador tdi ON tdi.tdiid = dti.tdiid
			WHERE indid='".$_SESSION['indid']."' AND tdiordem='1'";
	$itens['nivel1'] = $db->carregar($sql);
	if(!$itens['nivel1'][0]) {
	 
		unset($itens['nivel1']);
		
	} else {
	
		$sql = "SELECT tdi.*, dti.tdinumero FROM painel.detalhetipoindicador dti 
				INNER JOIN painel.detalhetipodadosindicador tdi ON tdi.tdiid = dti.tdiid
				WHERE indid='".$_SESSION['indid']."' AND tdiordem='2'";
		$itens['nivel2'] = $db->carregar($sql);
		if(!$itens['nivel2'][0]) unset($itens['nivel2']);
		
	}
	
	return $itens;
}

function pegarFormatoInput($indid = null) {
	global $db;
	/*
	 * Verificando o tipo de unidade de medi��o do indicador
	 * regra 1: se for moeda (unmid=5), o formato dos campos devem ser ###.###.###,##
	 * regra 2: se for Inteiro (unmid=3), verificar se indqtdevalor == true, caso sim, mostrar os dois campos
	 */
	$indid = !$indid ? $_SESSION['indid'] : $indid;
	if($indid) {
		$ind = $db->pegaLinha("SELECT unmid, indqtdevalor FROM painel.indicador WHERE indid='".$indid."'");
		switch($ind['unmid']) {
			case '1':
				$formatoinput = array('mascara'             => '###,##',
									  'size'                => '7',
									  'maxlength'           => '6',
									  'label'               => 'Porcentagem',
									  'unmid'				=> $ind['unmid']);
				break;
			case '2':
				$formatoinput = array('mascara'             => '###.###,##',
									  'size'                => '11',
									  'maxlength'           => '10',
									  'label'               => 'Raz�o',
									  'unmid'				=> $ind['unmid']);
				break;
			case '4':
				$formatoinput = array('mascara'             => '###,##',
									  'size'                => '7',
									  'maxlength'           => '6',
									  'label'               => 'Ind�ce',
									  'unmid'				=> $ind['unmid']);
				break;
			case '5':
				$formatoinput = array('mascara'             => '###.###.###.###,##',
									  'size'                => '18',
									  'maxlength'           => '17',
									  'label'               => 'Valor',
									  'unmid'				=> $ind['unmid']);
				break;
			case '3':
				$formatoinput = array('mascara'             => '##########',
									  'size'                => '11',
									  'maxlength'           => '10',
									  'label'               => 'Quantidade',
									  'unmid'				=> $ind['unmid']);
				
				if($ind['indqtdevalor'] == "t") {
					// mostar os dois campos (quantidade e valor)
					$formatoinput['campovalor'] = array('mascara'             => '###.###.###.###,##',
									  					'size'                => '18',
									  					'maxlength'           => '17',
									  					'label'               => 'Valor',
									  					'unmid'				  => $ind['unmid']);
				}
				break;
			default:
				$formatoinput = array('mascara'             => '##########',
									  'size'                => '11',
									  'maxlength'           => '10',
									  'label'               => 'Quantidade',
									  'unmid'				=> $ind['unmid']);
		}
		return $formatoinput;
	} else {
		echo "<p align='center'>Problemas na identifica��o do indicador. <b><a href=\"?modulo=inicio&acao=C\">Clique aqui</a></b> e refa�a os procedimentos.</p>";
		exit;
	}

}

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
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}
function processarLinhasTabelaSemFiltros($registros, $detalhes, $variaveis = false) {
	global $db, $_CONFIGS;
	
	// pegando o formato do campo
	$formatoinput = pegarFormatoInput();
	
	if($registros[0]) {
		foreach($registros as $key => $reg) {
			// pegando a serie historica
			$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE indid='".$_SESSION['indid']."' AND dpeid='".$reg['codigo']."' AND (sehstatus='A' OR sehstatus='H')");
			$html .= "<tr bgcolor='".((fmod($key,2) == 0)?'#F7F7F7':'')."' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".((fmod($key,2) == 0)?'#F7F7F7':'')."';\">";
			$html .= "<td nowrap>".$reg['descricao']."</td>";
			if($detalhes['nivel2']) {
				foreach($detalhes['nivel1'] as $de1) {
					foreach($detalhes['nivel2'] as $de2) {
						unset($valor,$dinheiro);
						if($seriehistorica) {
							$sql = "SELECT trim(to_char(dshqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as dshqtde FROM painel.detalheseriehistorica WHERE sehid='".$seriehistorica."' AND tidid".$de1['tdinumero']."='".$de1['tidid']."' AND tidid".$de2['tdinumero']."='".$de2['tidid']."'";
							$valor = $db->pegaUm($sql);
						}
						if($formatoinput['campovalor']) {
							$html .="<td><font size=1>Quantidade:</font> <input type='text' class='normal' name='item[".$variaveis['tipotabela']."][".$reg['codigo']."][".$de1['tidid']."][".$de2['tidid']."]' value='".$valor."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['size']."\" maxlength=\"".$formatoinput['maxlength']."\">";
							if($seriehistorica) {
								$sql = "SELECT trim(to_char(dshvalor, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['campovalor']['mascara'])."')) as dshvalor FROM painel.detalheseriehistorica WHERE sehid='".$seriehistorica."' AND tidid".$de1['tdinumero']."='".$de1['tidid']."' AND tidid".$de2['tdinumero']."='".$de2['tidid']."'";
								$dinheiro = $db->pegaUm($sql);
							}
							$html .= "<br />&nbsp;&nbsp;&nbsp;<font size=1>Valor (R$):</font> <input type='text' class='normal' name='valor[".$variaveis['tipotabela']."][".$reg['codigo']."][".$de1['tidid']."][".$de2['tidid']."]' value='".$dinheiro."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['campovalor']['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['campovalor']['size']."\" maxlength=\"".$formatoinput['campovalor']['maxlength']."\">";
							$html .="</td>";
						} else {
							$html .="<td align='center'><input type='text' class='normal' name='item[".$variaveis['tipotabela']."][".$reg['codigo']."][".$de1['tidid']."][".$de2['tidid']."]' value='".$valor."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['size']."\" maxlength=\"".$formatoinput['maxlength']."\"></td>";
						}
						
					}
				}
			} elseif($detalhes['nivel1']) {
				foreach($detalhes['nivel1'] as $de1) {
					unset($valor,$dinheiro);
					if($seriehistorica) {
						$sql = "SELECT trim(to_char(dshqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as dshqtde FROM painel.detalheseriehistorica WHERE sehid='".$seriehistorica."' AND tidid".$de1['tdinumero']."='".$de1['tidid']."'";
						$valor = $db->pegaUm($sql);
					}
					if($formatoinput['campovalor']) {
						$html .="<td><font size=1>Quantidade:</font> <input type='text' class='normal' name='item[".$variaveis['tipotabela']."][".$reg['codigo']."][".$de1['tidid']."]' value='".$valor."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['size']."\" maxlength=\"".$formatoinput['maxlength']."\">";
						if($seriehistorica) {
								$sql = "SELECT trim(to_char(dshvalor, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['campovalor']['mascara'])."')) as dshvalor FROM painel.detalheseriehistorica WHERE sehid='".$seriehistorica."' AND tidid".$de1['tdinumero']."='".$de1['tidid']."'";
								$dinheiro = $db->pegaUm($sql);
						}
						$html .= "<br />&nbsp;&nbsp;&nbsp;<font size=1>Valor (R$):</font> <input type='text' class='normal' name='valor[".$variaveis['tipotabela']."][".$reg['codigo']."][".$de1['tidid']."]' value='".$dinheiro."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['campovalor']['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['campovalor']['size']."\" maxlength=\"".$formatoinput['campovalor']['maxlength']."\">";
						$html .="</td>";
					} else {
						$html .="<td align='center'><input type='text' class='normal' name='item[".$variaveis['tipotabela']."][".$reg['codigo']."][".$de1['tidid']."]' value='".$valor."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['size']."\" maxlength=\"".$formatoinput['maxlength']."\"></td>";
					}
				}
			} else {
				unset($valor,$dinheiro);
				if($seriehistorica) {
					$sql = "SELECT trim(to_char(dshqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as dshqtde FROM painel.detalheseriehistorica WHERE sehid='".$seriehistorica."'";
					$valor = $db->pegaUm($sql);
				}
				
				if($formatoinput['campovalor']) {
					$html .="<td><font size=1>Quantidade:</font> <input type='text' class='normal' name='item[".$variaveis['tipotabela']."][".$reg['codigo']."]' value='".$valor."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['size']."\" maxlength=\"".$formatoinput['maxlength']."\">";
					if($seriehistorica) {
						$sql = "SELECT trim(to_char(dshvalor, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['campovalor']['mascara'])."')) as dshvalor FROM painel.detalheseriehistorica WHERE sehid='".$variaveis['seriehistorica']."'";
						$dinheiro = $db->pegaUm($sql);
					}
					$html .= "<br />&nbsp;&nbsp;&nbsp;<font size=1>Valor (R$):</font> <input type='text' class='normal' name='valor[".$variaveis['tipotabela']."][".$reg['codigo']."]' value='".$dinheiro."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['campovalor']['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['campovalor']['size']."\" maxlength=\"".$formatoinput['campovalor']['maxlength']."\">";
					$html .="</td>";
				} else {
					$html .="<td align='center'><input type='text' class='normal' name='item[".$variaveis['tipotabela']."][".$reg['codigo']."]' value='".$valor."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['size']."\" maxlength=\"".$formatoinput['maxlength']."\"></td>";
				}

			}
			$html .= "</tr>";
		}
	} else {
		$html .= "<tr>";
		$html .= "<td colspan='2'>N�o existem estados cadastrados.</td>";
		$html .= "</tr>";
	}
	
	return $html;	
}
/**
 * Fun��o que processa as linhas da tabela
 * 
 * @author Alexandre Dourado
 * @return htmlcode
 * @param array $registros Registros contendo inform��es das linhas
 * @param array $detalhes Lista de detalhes 
 * @param array $variaveis Contem variaveis alternativas, utilizado para enviar alguma variavel fixa 
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function processarLinhasTabela($registros, $detalhes, $variaveis = false) {
	global $db, $_CONFIGS;
	
	// pegando o formato do campo
	$formatoinput = pegarFormatoInput();
	
	if($registros[0]) {
		for($key = (($variaveis['paginainicial'])?$variaveis['paginainicial']:0);$key < (($variaveis['paginafinal'])?$variaveis['paginafinal']:count($registros));$key++) {
			$reg = $registros[$key];
			if($reg) {
				$html .= "<tr bgcolor='".((fmod($key,2) == 0)?'#F7F7F7':'')."' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='".((fmod($key,2) == 0)?'#F7F7F7':'')."';\">";
				$html .= "<td nowrap>".$reg['descricao']."</td>";
				if($detalhes['nivel2']) {
					foreach($detalhes['nivel1'] as $de1) {
						foreach($detalhes['nivel2'] as $de2) {
							if($variaveis['seriehistorica']) {
								$sql = "SELECT trim(to_char(dshqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as valor FROM painel.detalheseriehistorica WHERE ".$_CONFIGS[$variaveis['tipotabela']]['campo']."='".$reg['codigo']."' AND sehid='".$variaveis['seriehistorica']."' AND tidid".$de1['tdinumero']."='".$de1['tidid']."' AND tidid".$de2['tdinumero']."='".$de2['tidid']."'";
								$valor = $db->pegaUm($sql);
							}
							
							if($formatoinput['campovalor']) {
								$html .="<td><font size=1>Quantidade:</font> <input type='text' class='normal' name='item[".$variaveis['tipotabela']."][".$reg['codigo']."][".$de1['tidid']."][".$de2['tidid']."]' value='".$valor."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['size']."\" maxlength=\"".$formatoinput['maxlength']."\">";
								if($variaveis['seriehistorica']) {
									$sql = "SELECT dshvalor as dshvalor FROM painel.detalheseriehistorica WHERE ".$_CONFIGS[$variaveis['tipotabela']]['campo']."='".$reg['codigo']."' AND sehid='".$variaveis['seriehistorica']."' AND tidid".$de1['tdinumero']."='".$de1['tidid']."' AND tidid".$de2['tdinumero']."='".$de2['tidid']."'";
									$dinheiro = $db->pegaUm($sql);
								}
								$html .= "<br />&nbsp;&nbsp;&nbsp;<font size=1>Valor (R$):</font> <input type='text' class='normal' name='valor[".$variaveis['tipotabela']."][".$reg['codigo']."][".$de1['tidid']."][".$de2['tidid']."]' value='".(($dinheiro)?mascaraglobal($dinheiro, $formatoinput['campovalor']['mascara']):"")."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['campovalor']['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['campovalor']['size']."\" maxlength=\"".$formatoinput['campovalor']['maxlength']."\">";
								$html .="</td>";
							} else {
								$html .="<td align='center'><input type='text' class='normal' name='item[".$variaveis['tipotabela']."][".$reg['codigo']."][".$de1['tidid']."][".$de2['tidid']."]' value='".$valor."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);\" size=\"".$formatoinput['size']."\" maxlength=\"".$formatoinput['maxlength']."\"></td>";
							}
							
						}
					}
				} elseif($detalhes['nivel1']) {
					foreach($detalhes['nivel1'] as $de1) {
						if($variaveis['seriehistorica']) {
								$sql = "SELECT trim(to_char(dshqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as dshqtde FROM painel.detalheseriehistorica WHERE ".$_CONFIGS[$variaveis['tipotabela']]['campo']."='".$reg['codigo']."' AND sehid='".$variaveis['seriehistorica']."' AND tidid".$de1['tdinumero']."='".$de1['tidid']."'";
								$valor = $db->pegaUm($sql);
						}
						
						if($formatoinput['campovalor']) {
							$html .="<td><font size=1>Quantidade:</font> <input type='text' class='normal' name='item[".$variaveis['tipotabela']."][".$reg['codigo']."][".$de1['tidid']."]' value='".$valor."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['size']."\" maxlength=\"".$formatoinput['maxlength']."\">";
							if($variaveis['seriehistorica']) {
									$sql = "SELECT dshvalor as dshvalor FROM painel.detalheseriehistorica WHERE ".$_CONFIGS[$variaveis['tipotabela']]['campo']."='".$reg['codigo']."' AND sehid='".$variaveis['seriehistorica']."' AND tidid".$de1['tdinumero']."='".$de1['tidid']."'";
									$dinheiro = $db->pegaUm($sql);
							}
							$html .= "<br />&nbsp;&nbsp;&nbsp;<font size=1>Valor (R$):</font> <input type='text' class='normal' name='valor[".$variaveis['tipotabela']."][".$reg['codigo']."][".$de1['tidid']."]' value='".(($dinheiro)?mascaraglobal($dinheiro, $formatoinput['campovalor']['mascara']):"")."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['campovalor']['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['campovalor']['size']."\" maxlength=\"".$formatoinput['campovalor']['maxlength']."\">";
							$html .="</td>";
						} else {
							$html .="<td align='center'><input type='text' class='normal' name='item[".$variaveis['tipotabela']."][".$reg['codigo']."][".$de1['tidid']."]' value='".$valor."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);\" size=\"".$formatoinput['size']."\" maxlength=\"".$formatoinput['maxlength']."\"></td>";
						}
					}
				} else {
					// Quando n�o tiver detalhes
					if($variaveis['seriehistorica']) {
						$sql = "SELECT trim(to_char(dshqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as dshqtde FROM painel.detalheseriehistorica WHERE ".$_CONFIGS[$variaveis['tipotabela']]['campo']."='".$reg['codigo']."' AND sehid='".$variaveis['seriehistorica']."'";
						$valor = $db->pegaUm($sql);
					}
					if($formatoinput['campovalor']) {
						$html .="<td><font size=1>Quantidade:</font> <input type='text' class='normal' name='item[".$variaveis['tipotabela']."][".$reg['codigo']."]' value='".$valor."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['size']."\" maxlength=\"".$formatoinput['maxlength']."\">";
						if($variaveis['seriehistorica']) {
							$sql = "SELECT dshvalor as dshvalor FROM painel.detalheseriehistorica WHERE ".$_CONFIGS[$variaveis['tipotabela']]['campo']."='".$reg['codigo']."' AND sehid='".$variaveis['seriehistorica']."'";
							$dinheiro = $db->pegaUm($sql);
						}
						$html .= "<br />&nbsp;&nbsp;&nbsp;<font size=1>Valor (R$):</font> <input type='text' class='normal' name='valor[".$variaveis['tipotabela']."][".$reg['codigo']."]' value='".(($dinheiro)?mascaraglobal($dinheiro, $formatoinput['campovalor']['mascara']):"")."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['campovalor']['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['campovalor']['size']."\" maxlength=\"".$formatoinput['campovalor']['maxlength']."\">";
						$html .="</td>";
					} else {
						$html .="<td align='center'><input type='text' class='normal' name='item[".$variaveis['tipotabela']."][".$reg['codigo']."]' value='".$valor."' onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);somarcoluna(this);\" size=\"".$formatoinput['size']."\" maxlength=\"".$formatoinput['maxlength']."\"></td>";
					}
	
				}
				$html .= "</tr>";
			}
		}
	} else {
		$html .= "<tr>";
		$html .= "<td colspan='2'>N�o existem estados cadastrados.</td>";
		$html .= "</tr>";
	}
	
	return $html;
}
/**
 * Fun��o que processa o rodape contendo os totalizadores
 * 
 * @author Alexandre Dourado
 * @return htmlcode
 * @param array $detalhes Lista de detalhes 
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function montarSubRodapeDetalhes($detalhes = false) {
	global $db;
	
	// pegando o formato do campo
	$formatoinput = pegarFormatoInput();
	
	if($detalhes['nivel1']) {
		$html_preview .="<tr ".($formatoinput['unmid'] == 1 ? "style='display:none'" : "")." id='linhatotais'>";
		$html_preview .="<td class='SubTituloDireita'><strong>TOTAL:</strong></td>";
		foreach($detalhes['nivel1'] as $de1) {
			if($formatoinput['campovalor']) {
				$html_preview .="<td><font size=1>Quantidade:</font> <input type='text' class='disabled' size='".$formatoinput['size']."' maxlength='".$formatoinput['maxlength']."' onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);\" readonly>";
				$html_preview .= "<br />&nbsp;&nbsp;&nbsp;<font size=1>Valor (R$):</font> <input type='text' class='disabled' size='".$formatoinput['campovalor']['size']."' maxlength='".$formatoinput['campovalor']['maxlength']."' onKeyUp=\"this.value=mascaraglobal('".$formatoinput['campovalor']['mascara']."',this.value);\" readonly>";
				$html_preview .="</td>";
			} else {
				$html_preview .="<td align='center'><input type='text' class='disabled' size='".$formatoinput['size']."' maxlength='".$formatoinput['maxlength']."' value='' id='total_".$de1['tidid']."' onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);\" readonly></td>";
			}
		}
		$html_preview .="</tr>";
		
		if($detalhes['nivel2']) {
			unset($html_preview);
			$html_preview .="<tr ".($formatoinput['unmid'] == 1 ? "style='display:none'" : "")." id='linhatotais'>";
			$html_preview .="<td class='SubTituloDireita'><strong>TOTAL:</strong></td>";
			foreach($detalhes['nivel1'] as $de1) {
				foreach($detalhes['nivel2'] as $de2) {
					if($formatoinput['campovalor']) {
						$html_preview .="<td><font size=1>Quantidade:</font> <input type='text' class='disabled' size='".$formatoinput['size']."' maxlength='".$formatoinput['maxlength']."' onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);\" value='' readonly>";
						$html_preview .= "<br />&nbsp;&nbsp;&nbsp;<font size=1>Valor (R$):</font> <input type='text' class='disabled' size='".$formatoinput['campovalor']['size']."' maxlength='".$formatoinput['campovalor']['maxlength']."' onKeyUp=\"this.value=mascaraglobal('".$formatoinput['campovalor']['mascara']."',this.value);\" readonly>";
						$html_preview .="</td>";
					} else {
						$html_preview .="<td class='SubTituloCentro'><input type='text' class='disabled' size='".$formatoinput['size']."' maxlength='".$formatoinput['maxlength']."' value='' id='total_".$de1['tidid']."_".$de2['tidid']."' onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);\" readonly></td>";
					}
				}
			}
			$html_preview .="</tr>";
		}
	} else {
		$html_preview .="<tr ".($formatoinput['unmid'] == 1 ? "style='display:none'" : "")."  id='linhatotais'>";
		$html_preview .="<td class='SubTituloDireita'><strong>TOTAL:</strong></td>";
		if($formatoinput['campovalor']) {
			$html_preview .="<td><font size=1>Quantidade:</font> <input type='text' class='disabled' size='".$formatoinput['size']."' maxlength='".$formatoinput['maxlength']."' onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);\" readonly>";
			$html_preview .= "<br />&nbsp;&nbsp;&nbsp;<font size=1>Valor (R$):</font> <input type='text' class='disabled' size='".$formatoinput['campovalor']['size']."' maxlength='".$formatoinput['campovalor']['maxlength']."' onKeyUp=\"this.value=mascaraglobal('".$formatoinput['campovalor']['mascara']."',this.value);\" readonly>";
			$html_preview .="</td>";
		} else {
			$html_preview .="<td align='center'><input type='text' class='disabled' size='".$formatoinput['size']."' maxlength='".$formatoinput['maxlength']."' onKeyUp=\"this.value=mascaraglobal('".$formatoinput['mascara']."',this.value);\" readonly></td>";
		}
		$html_preview .="</tr>";
	}
		
	return $html_preview;
}
/**
 * Fun��o que processa o cabe�alho contendo niveis e os subniveis
 * 
 * @author Alexandre Dourado
 * @return htmlcode
 * @param array $detalhes Lista de detalhes 
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function montarSubCabecalhoDetalhes($detalhes = false) {
	if($detalhes['nivel1']) {
		$html .="<tr>";
		foreach($detalhes['nivel1'] as $de1) {
			$html .="<td class='SubTituloCentro' ".((count($detalhes['nivel2']))?"colspan='".count($detalhes['nivel2'])."'":"").">".$de1['tiddsc']."</td>";
		}
		$html .="</tr>";
		
		if($detalhes['nivel2']) {
			$html .="<tr>";
			foreach($detalhes['nivel1'] as $de1) {
				foreach($detalhes['nivel2'] as $de2) {
					$html .="<td class='SubTituloCentro'>".$de2['tiddsc']."</td>";
				}
			}
			$html .="</tr>";
		}
	}
	return $html;
}



/**
 * Fun��o que carrega o GRID de informa��o por HOSPITAL
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarGridHospital($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	// carregando lista de per�odos a serem apresentados (colunas), atualmente poder� utilizar apenas uma per�odo,
	// por�m pode ser atualizado para coletar de v�rios per�odos
	$sql = "SELECT dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe  
			WHERE dpeid='".$dados['dpeid']."' AND dpestatus='A'";
	$colunasperiodos = $db->carregar($sql);
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();

	// iniciando o o c�digo html (abrindo o formulario e tabela) 
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDados'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">Campus</td>";
	if($colunasperiodos[0]) {
		foreach($colunasperiodos as $per) {
			$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">".$per['descricao']."</td>";
		}
	}
	$html .= "</tr>";
	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// fim do cabe�alho
	
	// carregando serie historica
	$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$dados['dpeid']."' AND indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	// carregando as linhas da tabela
	$ent = $db->carregar("SELECT ent.entid as codigo, UPPER(ent.entnome)||' ('||COALESCE(ent2.entsig,'')||')' as descricao FROM entidade.entidade ent  
						  INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
					  	  INNER JOIN entidade.endereco ende ON ende.entid = ent.entid 
					  	  INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
					  	  INNER JOIN entidade.entidade ent2 ON ent2.entid = fea.entid 
					  	  INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = ent2.entid 
						  WHERE ende.muncod='".$dados['muncod']."' AND fen.funid='16' AND (fen2.funid='12' OR fen2.funid='16')
						  ORDER BY descricao");
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabela($ent, $detalhes, array('tipotabela'=>'hospital',
														  'seriehistorica'=>$seriehistorica));
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	// Bot�o para gravar o formulario de estado
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "<input type='hidden' name='muncod' value='".$dados['muncod']."'>";
	$html .= "<input type='hidden' name='estuf' value='".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$dados['muncod']."'")."'>";
	$html .= "<input type='hidden' name='dpeid' value='".$dados['dpeid']."'>";
	$html .= "<input type='hidden' name='sehid' value='".$seriehistorica."'>";
	$html .= "</form>";
	echo $html;
	exit;
}




/**
 * Fun��o que carrega o GRID de informa��o por UNIVERSIDADE
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarGridInstituto($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	// carregando lista de per�odos a serem apresentados (colunas), atualmente poder� utilizar apenas uma per�odo,
	// por�m pode ser atualizado para coletar de v�rios per�odos
	$sql = "SELECT dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe  
			WHERE dpeid='".$dados['dpeid']."' AND dpestatus='A'";
	$colunasperiodos = $db->carregar($sql);
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();

	// iniciando o o c�digo html (abrindo o formulario e tabela) 
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDados'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">Campus</td>";
	if($colunasperiodos[0]) {
		foreach($colunasperiodos as $per) {
			$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">".$per['descricao']."</td>";
		}
	}
	$html .= "</tr>";
	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// fim do cabe�alho
	
	// carregando serie historica
	$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$dados['dpeid']."' AND indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	// carregando as linhas da tabela
	$ent = $db->carregar("SELECT uni.unicod as codigo, UPPER(uni.unidsc) as descricao FROM public.unidade uni 
						  INNER JOIN entidade.entidade ent ON ent.entunicod = uni.unicod 
					  	  INNER JOIN entidade.endereco ende ON ende.entid = ent.entid 
						  WHERE ende.muncod='".$dados['muncod']."' 
						  AND uni.orgcod='26000' AND gunid=2 AND unistatus='A' ORDER BY descricao");
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabela($ent, $detalhes, array('tipotabela'=>'instituto',
														  'seriehistorica'=>$seriehistorica));
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	// Bot�o para gravar o formulario de estado
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "<input type='hidden' name='muncod' value='".$dados['muncod']."'>";
	$html .= "<input type='hidden' name='estuf' value='".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$dados['muncod']."'")."'>";
	$html .= "<input type='hidden' name='dpeid' value='".$dados['dpeid']."'>";
	$html .= "<input type='hidden' name='sehid' value='".$seriehistorica."'>";
	$html .= "</form>";
	echo $html;
	exit;
}

/**
 * Fun��o que carrega o GRID de informa��o por UNIVERSIDADE
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarGridUniversidade($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	// carregando lista de per�odos a serem apresentados (colunas), atualmente poder� utilizar apenas uma per�odo,
	// por�m pode ser atualizado para coletar de v�rios per�odos
	$sql = "SELECT dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe  
			WHERE dpeid='".$dados['dpeid']."' AND dpestatus='A'";
	$colunasperiodos = $db->carregar($sql);
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();

	// iniciando o o c�digo html (abrindo o formulario e tabela) 
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDados'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">Campus</td>";
	if($colunasperiodos[0]) {
		foreach($colunasperiodos as $per) {
			$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">".$per['descricao']."</td>";
		}
	}
	$html .= "</tr>";
	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// fim do cabe�alho
	
	// carregando serie historica
	$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$dados['dpeid']."' AND indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	// carregando as linhas da tabela
	$ies = $db->carregar("SELECT uni.unicod as codigo, UPPER(uni.unidsc) as descricao FROM public.unidade uni 
						  INNER JOIN entidade.entidade ent ON ent.entunicod = uni.unicod 
					  	  INNER JOIN entidade.endereco ende ON ende.entid = ent.entid 
						  WHERE ende.muncod='".$dados['muncod']."' 
						  AND uni.orgcod='26000' AND gunid=3 AND unistatus='A' ORDER BY descricao");
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabela($ies, $detalhes, array('tipotabela'=>'universidade',
														  'seriehistorica'=>$seriehistorica));
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	// Bot�o para gravar o formulario de estado
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "<input type='hidden' name='muncod' value='".$dados['muncod']."'>";
	$html .= "<input type='hidden' name='estuf' value='".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$dados['muncod']."'")."'>";
	$html .= "<input type='hidden' name='dpeid' value='".$dados['dpeid']."'>";
	$html .= "<input type='hidden' name='sehid' value='".$seriehistorica."'>";
	$html .= "</form>";
	echo $html;
	exit;
}

/**
 * Fun��o que carrega o GRID de informa��o por CAMPUS PROFISSIONAL (quando regionaliza��o for por estado)
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarGridCampusProfissional($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	// carregando lista de per�odos a serem apresentados (colunas), atualmente poder� utilizar apenas uma per�odo,
	// por�m pode ser atualizado para coletar de v�rios per�odos
	$sql = "SELECT dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe  
			WHERE dpeid='".$dados['dpeid']."' AND dpestatus='A'";
	$colunasperiodos = $db->carregar($sql);
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();

	// iniciando o o c�digo html (abrindo o formulario e tabela) 
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDados'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">Campus</td>";
	if($colunasperiodos[0]) {
		foreach($colunasperiodos as $per) {
			$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">".$per['descricao']."</td>";
		}
	}
	$html .= "</tr>";
	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// fim do cabe�alho
	
	// carregando serie historica
	$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$dados['dpeid']."' AND indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	// carregando as linhas da tabela
	$ies = $db->carregar("SELECT ent.entid as codigo, UPPER(ent.entnome) as descricao FROM entidade.entidade ent
						  INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid   
					  	  INNER JOIN entidade.endereco ende ON ende.entid = ent.entid 
						  WHERE ende.muncod='".$dados['muncod']."' AND fen.funid=17 ORDER BY descricao");
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabela($ies, $detalhes, array('tipotabela'=>'campusprofissional',
														  'seriehistorica'=>$seriehistorica));
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	// Bot�o para gravar o formulario de estado
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "<input type='hidden' name='muncod' value='".$dados['muncod']."'>";
	$html .= "<input type='hidden' name='estuf' value='".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$dados['muncod']."'")."'>";
	$html .= "<input type='hidden' name='dpeid' value='".$dados['dpeid']."'>";
	$html .= "<input type='hidden' name='sehid' value='".$seriehistorica."'>";
	$html .= "</form>";
	echo $html;
	exit;
}

/**
 * Fun��o que carrega o GRID de informa��o por CAMPUS SUPERIOR (quando regionaliza��o for por estado)
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarGridCampusSuperior($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	// carregando lista de per�odos a serem apresentados (colunas), atualmente poder� utilizar apenas uma per�odo,
	// por�m pode ser atualizado para coletar de v�rios per�odos
	$sql = "SELECT dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe  
			WHERE dpeid='".$dados['dpeid']."' AND dpestatus='A'";
	$colunasperiodos = $db->carregar($sql);
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();

	// iniciando o o c�digo html (abrindo o formulario e tabela) 
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDados'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">Campus</td>";
	if($colunasperiodos[0]) {
		foreach($colunasperiodos as $per) {
			$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">".$per['descricao']."</td>";
		}
	}
	$html .= "</tr>";
	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// fim do cabe�alho
	
	// carregando serie historica
	$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$dados['dpeid']."' AND indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	// carregando as linhas da tabela
	$ies = $db->carregar("SELECT ent.entid as codigo, UPPER(ent.entnome) as descricao FROM entidade.entidade ent
						  INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid   
					  	  INNER JOIN entidade.endereco ende ON ende.entid = ent.entid 
						  WHERE ende.muncod='".$dados['muncod']."' AND fen.funid=18 ORDER BY descricao");
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabela($ies, $detalhes, array('tipotabela'=>'campussuperior',
														  'seriehistorica'=>$seriehistorica));
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	// Bot�o para gravar o formulario de estado
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "<input type='hidden' name='muncod' value='".$dados['muncod']."'>";
	$html .= "<input type='hidden' name='estuf' value='".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$dados['muncod']."'")."'>";
	$html .= "<input type='hidden' name='dpeid' value='".$dados['dpeid']."'>";
	$html .= "<input type='hidden' name='sehid' value='".$seriehistorica."'>";
	$html .= "</form>";
	echo $html;
	exit;
}
/**
 * Fun��o que carrega o GRID de informa��o por ESTADO (quando regionaliza��o for por estado)
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarGridEstado($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	// carregando lista de per�odos a serem apresentados (colunas), atualmente poder� utilizar apenas uma per�odo,
	// por�m pode ser atualizado para coletar de v�rios per�odos
	$sql = "SELECT dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe  
			WHERE dpeid='".$dados['dpeid']."' AND dpestatus='A'";
	$colunasperiodos = $db->carregar($sql);
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();

	// iniciando o o c�digo html (abrindo o formulario e tabela) 
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDados'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" id=\"data\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">Estados</td>";
	if($colunasperiodos[0]) {
		foreach($colunasperiodos as $per) {
			$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">".$per['descricao']."</td>";
		}
	}
	$html .= "</tr>";
	// fim do cabe�alho

	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// carregando serie historica
	$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$dados['dpeid']."' AND indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	// carregando as linhas da tabela
	$estados = $db->carregar("SELECT DISTINCT estuf AS codigo, UPPER(estdescricao) as descricao FROM territorios.estado ORDER BY descricao");
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabela($estados, $detalhes, array('tipotabela'=>'estado',
															  'seriehistorica'=>$seriehistorica));
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	
	// Bot�o para gravar o formulario de estado
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "<input type='hidden' name='dpeid' value='".$dados['dpeid']."'>";
	$html .= "<input type='hidden' name='sehid' value='".$seriehistorica."'>";
	$html .= "</form>";
	echo $html;
	exit;
}
/**
 * Fun��o que carrega o GRID de informa��o por REGI�O (quando regionaliza��o for por regi�o)
 * 
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 */
function carregarGridRegiao($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	// carregando lista de per�odos a serem apresentados (colunas), atualmente poder� utilizar apenas uma per�odo,
	// por�m pode ser atualizado para coletar de v�rios per�odos
	$sql = "SELECT dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe  
			WHERE dpeid='".$dados['dpeid']."' AND dpestatus='A'";
	$colunasperiodos = $db->carregar($sql);
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();

	// iniciando o o c�digo html (abrindo o formulario e tabela) 
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDados'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" id=\"data\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">Regi�es</td>";
	if($colunasperiodos[0]) {
		foreach($colunasperiodos as $per) {
			$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">".$per['descricao']."</td>";
		}
	}
	$html .= "</tr>";
	// fim do cabe�alho

	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// carregando serie historica
	$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$dados['dpeid']."' AND indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	// carregando as linhas da tabela
	$estados = $db->carregar("SELECT DISTINCT regcod AS codigo, UPPER(regdescricao) as descricao FROM territorios.regiao ORDER BY descricao");
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabela($estados, $detalhes, array('tipotabela'=>'regiao',
															  'seriehistorica'=>$seriehistorica));
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	
	// Bot�o para gravar o formulario de regiao
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "<input type='hidden' name='dpeid' value='".$dados['dpeid']."'>";
	$html .= "<input type='hidden' name='sehid' value='".$seriehistorica."'>";
	$html .= "</form>";
	echo $html;
	exit;
}


/**
 * Fun��o que carrega o GRID de informa��o por MUNICIPIO (quando regionaliza��o for por municipio)
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarGridMunicipio($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	// carregando lista de per�odos a serem apresentados (colunas), atualmente poder� utilizar apenas uma per�odo,
	// por�m pode ser atualizado para coletar de v�rios per�odos
	$sql = "SELECT dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe  
			WHERE dpeid='".$dados['dpeid']."' AND dpestatus='A'";
	$colunasperiodos = $db->carregar($sql);
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();
	// carregando as linhas da tabela
	$municipios = $db->carregar("SELECT DISTINCT muncod AS codigo, UPPER(mundescricao) as descricao FROM territorios.municipio WHERE estuf='".$dados['estuf']."' ORDER BY descricao");
	if(count($municipios) > 100) {
		// iniciando o o c�digo html (abrindo o formulario e tabela) 
		$html .= "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
		$html .= "<tr>";
		$html .= "<td><b>Pagina��o:</b> ";
		for($c = 0;$c < ceil(count($municipios)/100);$c++) {
			if($dados['pagina']==$c) $page = "<b>".($c+1)."</b>";
			else $page = $c+1;
			$html .= "<a style=\"cursor:pointer\" onclick=\"document.getElementById('gridinfo').innerHTML='<p align=center>Carregando...</p>';ajaxatualizar('requisicao=carregarGridMunicipio&dpeid=".$dados['dpeid']."&estuf=".$dados['estuf']."&pagina=".$c."','gridinfo');redimencionarBodyData();somarTodasColunas();\">".$page."</a>&nbsp;";
		}
		$html .= "</td></tr>";
		$html .= "</table>";
	}
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDados'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">Munic�pios</td>";
	if($colunasperiodos[0]) {
		foreach($colunasperiodos as $per) {
			$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">".$per['descricao']."</td>";
		}
	}
	$html .= "</tr>";
	// fim do cabe�alho
	
	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// carregando serie historica
	$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$dados['dpeid']."' AND indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabela($municipios, $detalhes, array('tipotabela'=>'municipio',
															     'seriehistorica'=> $seriehistorica,
																 'paginainicial' => (($dados['pagina'])?$dados['pagina']*100:0),
																 'paginafinal'   => (($dados['pagina'])?($dados['pagina']*100)+100:100)));
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	// Bot�o para gravar o formulario de estado
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "<input type='hidden' name='dshuf' value='".$dados['estuf']."'>";
	$html .= "<input type='hidden' name='dpeid' value='".$dados['dpeid']."'>";
	$html .= "<input type='hidden' name='sehid' value='".$seriehistorica."'>";
	$html .= "</form>";
	echo $html;
	exit;
}

/**
 * Fun��o que carrega o GRID de informa��o por IES (quando regionaliza��o for por ies)
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarGridIES($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	// carregando lista de per�odos a serem apresentados (colunas), atualmente poder� utilizar apenas uma per�odo,
	// por�m pode ser atualizado para coletar de v�rios per�odos
	$sql = "SELECT dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe  
			WHERE dpeid='".$dados['dpeid']."' AND dpestatus='A'";
	$colunasperiodos = $db->carregar($sql);
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();

	// iniciando o o c�digo html (abrindo o formulario e tabela) 
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDados'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">IES</td>";
	if($colunasperiodos[0]) {
		foreach($colunasperiodos as $per) {
			$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">".$per['descricao']."</td>";
		}
	}
	$html .= "</tr>";
	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// fim do cabe�alho
	
	// carregando serie historica
	$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$dados['dpeid']."' AND indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	// carregando as linhas da tabela
	$ies = $db->carregar("SELECT iesid as codigo, UPPER(iesdsc) as descricao FROM painel.ies WHERE iesmuncod='".$dados['muncod']."' ORDER BY descricao");
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabela($ies, $detalhes, array('tipotabela'=>'ies',
														  'seriehistorica'=>$seriehistorica));
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	// Bot�o para gravar o formulario de estado
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "<input type='hidden' name='muncod' value='".$dados['muncod']."'>";
	$html .= "<input type='hidden' name='estuf' value='".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$dados['muncod']."'")."'>";
	$html .= "<input type='hidden' name='dpeid' value='".$dados['dpeid']."'>";
	$html .= "<input type='hidden' name='sehid' value='".$seriehistorica."'>";
	$html .= "</form>";
	echo $html;
	exit;
}


/**
 * Fun��o que carrega o GRID de informa��o por IES (quando regionaliza��o for por ies)
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarGridIESCPC($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	// carregando lista de per�odos a serem apresentados (colunas), atualmente poder� utilizar apenas uma per�odo,
	// por�m pode ser atualizado para coletar de v�rios per�odos
	$sql = "SELECT dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe  
			WHERE dpeid='".$dados['dpeid']."' AND dpestatus='A'";
	$colunasperiodos = $db->carregar($sql);
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();

	// iniciando o o c�digo html (abrindo o formulario e tabela) 
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDados'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">IES</td>";
	if($colunasperiodos[0]) {
		foreach($colunasperiodos as $per) {
			$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">".$per['descricao']."</td>";
		}
	}
	$html .= "</tr>";
	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// fim do cabe�alho
	
	// carregando serie historica
	$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$dados['dpeid']."' AND indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	// carregando as linhas da tabela
	$iescdc = $db->carregar("SELECT iecid as codigo, UPPER(iecdsc) as descricao FROM painel.iescpc WHERE muncod='".$dados['muncod']."' ORDER BY descricao");
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabela($iescdc, $detalhes, array('tipotabela'=>'iescpc',
														     'seriehistorica'=>$seriehistorica));
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	// Bot�o para gravar o formulario de estado
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "<input type='hidden' name='muncod' value='".$dados['muncod']."'>";
	$html .= "<input type='hidden' name='estuf' value='".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$dados['muncod']."'")."'>";
	$html .= "<input type='hidden' name='dpeid' value='".$dados['dpeid']."'>";
	$html .= "<input type='hidden' name='sehid' value='".$seriehistorica."'>";
	$html .= "</form>";
	echo $html;
	exit;
}


/**
 * Fun��o que carrega o GRID de informa��o por BRASIL (quando regionaliza��o for por escola)
 * Neste caso, o GRID � diferente dos demais, n�o possui filtro (GRID criada pelo Cristiano)
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarGridBrasil($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	$sql = "SELECT ind.unmid, dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe 
			INNER JOIN painel.indicador ind ON ind.perid = dpe.perid   
			WHERE dpestatus='A' AND indid='".$_SESSION['indid']."' ORDER BY dpe.dpedatainicio";
	$linhasperiodos = $db->carregar($sql);
	
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();

	// iniciando o o c�digo html (abrindo o formulario e tabela) 
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDadosSemFiltros'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">Per�odos</td>";
	$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">Valor / Quantidade ".($linhasperiodos[0]['unmid'] == 1 ? " (%)" : "")."</td>";
	$html .= "</tr>";

	
	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// fim do cabe�alho
	
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabelaSemFiltros($linhasperiodos, $detalhes, array('tipotabela'=>'brasil'));
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	// Bot�o para gravar o formulario de estado
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "</form>";
	echo $html;
	exit;
}
/**
 * Fun��o que carrega o GRID de informa��o por POS GRADUA��O (quando regionaliza��o for por posgraduacao)
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarGridPosgraducao($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	// carregando lista de per�odos a serem apresentados (colunas), atualmente poder� utilizar apenas uma per�odo,
	// por�m pode ser atualizado para coletar de v�rios per�odos
	$sql = "SELECT dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe  
			WHERE dpeid='".$dados['dpeid']."' AND dpestatus='A'";
	$colunasperiodos = $db->carregar($sql);
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();

	// iniciando o o c�digo html (abrindo o formulario e tabela) 
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDados'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">Munic�pios</td>";
	if($colunasperiodos[0]) {
		foreach($colunasperiodos as $per) {
			$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">".$per['descricao']."</td>";
		}
	}
	$html .= "</tr>";
	
	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// fim do cabe�alho
	
	// carregando serie historica
	$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$dados['dpeid']."' AND indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	// carregando as linhas da tabela
	$posgraducao = $db->carregar("SELECT DISTINCT iepid as codigo, iepdsc as descricao FROM painel.iepg WHERE muncod='".$dados['muncod']."' ORDER BY descricao");
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabela($posgraducao, $detalhes, array('tipotabela'=>'posgraduacao',
															  'seriehistorica'=>$seriehistorica));
	
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	// Bot�o para gravar o formulario de estado
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "<input type='hidden' name='muncod' value='".$dados['muncod']."'>";
	$html .= "<input type='hidden' name='estuf' value='".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$dados['muncod']."'")."'>";
	$html .= "<input type='hidden' name='dpeid' value='".$dados['dpeid']."'>";
	$html .= "<input type='hidden' name='sehid' value='".$seriehistorica."'>";
	$html .= "</form>";
	echo $html;
	exit;
}
/**
 * Fun��o que carrega o GRID de informa��o por ESCOLA (quando regionaliza��o for por escola)
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarGridEscola($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	// carregando lista de per�odos a serem apresentados (colunas), atualmente poder� utilizar apenas uma per�odo,
	// por�m pode ser atualizado para coletar de v�rios per�odos
	$sql = "SELECT dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe  
			WHERE dpeid='".$dados['dpeid']."' AND dpestatus='A'";
	$colunasperiodos = $db->carregar($sql);
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();
	
	// iniciando o o c�digo html (abrindo o formulario e tabela) 
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDados'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">Munic�pios</td>";
	if($colunasperiodos[0]) {
		foreach($colunasperiodos as $per) {
			$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">".$per['descricao']."</td>";
		}
	}
	$html .= "</tr>";
	
	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// fim do cabe�alho
	
	// carregando serie historica
	$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$dados['dpeid']."' AND indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	// carregando as linhas da tabela
	$escolas = $db->carregar("SELECT DISTINCT esccodinep as codigo, escdsc as descricao FROM painel.escola WHERE escmuncod='".$dados['muncod']."' ORDER BY descricao");
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabela($escolas, $detalhes, array('tipotabela'=>'escola',
															  'seriehistorica'=>$seriehistorica));
	
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	// Bot�o para gravar o formulario de estado
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "<input type='hidden' name='muncod' value='".$dados['muncod']."'>";
	$html .= "<input type='hidden' name='estuf' value='".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$dados['muncod']."'")."'>";
	$html .= "<input type='hidden' name='dpeid' value='".$dados['dpeid']."'>";
	$html .= "<input type='hidden' name='sehid' value='".$seriehistorica."'>";
	$html .= "</form>";
	echo $html;
	exit;
}

/**
 * Fun��o que carrega o GRID de informa��o por POLO (quando regionaliza��o for por escola)
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function carregarGridPolo($dados) {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	// carregando lista de per�odos a serem apresentados (colunas), atualmente poder� utilizar apenas uma per�odo,
	// por�m pode ser atualizado para coletar de v�rios per�odos
	$sql = "SELECT dpe.dpeid as codigo, dpe.dpedsc as descricao FROM painel.detalheperiodicidade dpe  
			WHERE dpeid='".$dados['dpeid']."' AND dpestatus='A'";
	$colunasperiodos = $db->carregar($sql);
	// carregando os detalhes do indicador
	$detalhes = detalhetipoindicador();
	
	// iniciando o o c�digo html (abrindo o formulario e tabela) 
	$html .= "<form method='post'>";
	$html .= "<input type='hidden' name='requisicao' value='gravarGridDados'>";
	$html .= "<table class=\"tabela\" style=\"color:333333;\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$html .= "<thead>";
	// imprimindo o cabe�alho
	$html .= "<tr>";
	
	// realizando o controle do cabe�alho em rela��o aos detalhes (controle de rowspan e colspan das coluna no cabe�alho)
	$rowspancabecalho = 1;
	$colspancabecalho = 1;
	if($detalhes['nivel1']) {
		$rowspancabecalho = 2;
		$colspancabecalho = count($detalhes['nivel1']); 
	}
	if($detalhes['nivel2']) { 
		$rowspancabecalho = 3;
		$colspancabecalho = count($detalhes['nivel1'])*count($detalhes['nivel2']);
	}
	$html .= "<td class='SubTituloCentro' ".(($rowspancabecalho)?"rowspan='".$rowspancabecalho."'":"").">Munic�pios</td>";
	if($colunasperiodos[0]) {
		foreach($colunasperiodos as $per) {
			$html .= "<td class='SubTituloCentro' ".(($colspancabecalho)?"colspan='".$colspancabecalho."'":"").">".$per['descricao']."</td>";
		}
	}
	$html .= "</tr>";
	
	// verificando se tem detalhes da serie historica, se tiver monta a estrutura de detalhes
	if($detalhes) {
		$html .= montarSubCabecalhoDetalhes($detalhes);
	}
	
	$html .= "</thead>";
	$html .= "<tbody style='height:200px;overflow-y:scroll;overflow-x:hidden;' id='bodydata'>";
	// fim do cabe�alho
	
	// carregando serie historica
	$seriehistorica = $db->pegaUm("SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$dados['dpeid']."' AND indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H')");
	// carregando as linhas da tabela
	$polos = $db->carregar("SELECT DISTINCT polid as codigo, poldsc as descricao FROM painel.polo WHERE muncod='".$dados['muncod']."' ORDER BY descricao");
	// Imprimindo as linhas da tabela	
	$html .= processarLinhasTabela($polos, $detalhes, array('tipotabela'=>'polo',
															  'seriehistorica'=>$seriehistorica));
	
	$html .= "</tbody>";
	$html .= "<tfoot>";
	// Criando Rodape
	$html .= montarSubRodapeDetalhes($detalhes);
	// Bot�o para gravar o formulario de estado
	$html .= "<tr>";
	$html .= "<td colspan='".($colspancabecalho+1)."' class='SubTituloDireita'><input type='submit' value='Gravar'> <input type='button' value='Voltar' name='voltar' onclick=\"window.location='?modulo=principal/listaSerieHistorica&acao=A'\"></td>";
	$html .= "</tr>";
	
	$html .= "</tfoot>";
	// fechando o o c�digo html (fechando o formulario e tabela)
	$html .= "</table>";
	$html .= "<input type='hidden' name='muncod' value='".$dados['muncod']."'>";
	$html .= "<input type='hidden' name='estuf' value='".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$dados['muncod']."'")."'>";
	$html .= "<input type='hidden' name='dpeid' value='".$dados['dpeid']."'>";
	$html .= "<input type='hidden' name='sehid' value='".$seriehistorica."'>";
	$html .= "</form>";
	echo $html;
	exit;
}

function verificarStatusSerieHistorica($dpeid) {
	global $db;
	
	$sql = "SELECT MAX(dpedatainicio) FROM painel.seriehistorica seh 
			LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
			WHERE indid='".$_SESSION['indid']."' LIMIT 1";
	$periodomaior = $db->pegaUm($sql);
	$sql = "SELECT dpedatainicio FROM painel.detalheperiodicidade dpe 
			WHERE dpeid='".$dpeid."' LIMIT 1";
	$periodonovo = $db->pegaUm($sql);
	if($periodonovo > $periodomaior) return "A";
	else return "H";

}
/**
 * Fun��o que grava o GRID de informa��o (geral)
 * 
 * @author Alexandre Dourado
 * @return void 
 * @param integer $dados
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function gravarGridDados($dados) {
	global $db, $_CONFIGS;

	if(!$dados['sehid']) {
		$status = verificarStatusSerieHistorica($dados['dpeid']);
		switch($status) {
			case 'A':
				$db->executar("UPDATE painel.seriehistorica SET sehstatus='H' WHERE sehstatus='A' AND indid='".$_SESSION['indid']."'", false);
				break;
			case 'H':
				break;
			default:
				echo "<script>
						alert('Problemas na cria��o da s�rie historica.');
						window.location='?modulo=inicio&acao=C';
					  </script>";
				exit;
		}
		$regidseh = $db->pegaUm("SELECT regid FROM painel.indicador WHERE indid='".$_SESSION['indid']."'");
		
		$dados['sehid'] = $db->pegaUm("INSERT INTO painel.seriehistorica(indid, sehvalor, sehstatus, sehqtde, dpeid, regid)
    				 				   VALUES ('".$_SESSION['indid']."', NULL, '".$status."', '0', '".$dados['dpeid']."', ".(($regidseh)?"'".$regidseh."'":"NULL").") RETURNING sehid;", false);
	}
	$formatoinput = pegarFormatoInput();
	if($dados['item']) {
		foreach($dados['item'] as $indice1 => $dados1) {
			foreach($dados1 as $indice2 => $dados2) {
				$db->executar("DELETE FROM painel.detalheseriehistorica WHERE sehid='".$dados['sehid']."' AND ".$_CONFIGS[$indice1]['campo']."='".$indice2."'", false);
				if(is_array($dados2)) {
					foreach($dados2 as $tdiidnivel1 => $dados3) {
						$tdinumnivel1 =  'tidid'.$db->pegaUm("SELECT tdinumero FROM painel.detalhetipoindicador tdi 
										 	 	   		   	  INNER JOIN painel.detalhetipodadosindicador tid ON tid.tdiid = tdi.tdiid 
									 		 	   		   	  WHERE tidid='".$tdiidnivel1."'");
						if(is_array($dados3)) {
							foreach($dados3 as $tdiidnivel2 => $dados4) {
								$tdinumnivel2 =  'tidid'.$db->pegaUm("SELECT tdinumero FROM painel.detalhetipoindicador tdi 
												 	 	   		   	  INNER JOIN painel.detalhetipodadosindicador tid ON tid.tdiid = tdi.tdiid 
									 				 	   		   	  WHERE tidid='".$tdiidnivel2."'");
								if(is_numeric(str_replace(array(".",","), array("","."), $dados4))) {
									$sql = "INSERT INTO painel.detalheseriehistorica(sehid, dshqtde, ".$_CONFIGS[$indice1]['campo'].", ".$tdinumnivel1.", ".$tdinumnivel2." ".(($formatoinput['campovalor'] && $dados['valor'][$indice1][$indice2][$tdiidnivel1][$tdiidnivel2])?", dshvalor":"").")
								    		VALUES ('".$dados['sehid']."', '".str_replace(array(".",","), array("","."), $dados4)."', '".$indice2."', '".$tdiidnivel1."', '".$tdiidnivel2."' ".(($formatoinput['campovalor'] && $dados['valor'][$indice1][$indice2][$tdiidnivel1][$tdiidnivel2])?", '".str_replace(array(".",","), array("","."), $dados['valor'][$indice1][$indice2][$tdiidnivel1][$tdiidnivel2])."'":"").") RETURNING dshid;";
									$dtlid[] = $db->pegaUm($sql);
								}
							}
						} else {
							if(is_numeric(str_replace(array(".",","), array("","."), $dados3))) {
								$dtlid[] = $db->pegaUm("INSERT INTO painel.detalheseriehistorica(
				            							sehid, dshqtde, ".$_CONFIGS[$indice1]['campo'].", ".$tdinumnivel1." ".(($formatoinput['campovalor'] && $dados['valor'][$indice1][$indice2][$tdiidnivel1])?", dshvalor":"").")
	    												VALUES ('".$dados['sehid']."', '".str_replace(array(".",","), array("","."), $dados3)."', '".$indice2."', '".$tdiidnivel1."' ".(($formatoinput['campovalor'] && $dados['valor'][$indice1][$indice2][$tdiidnivel1])?", '".str_replace(array(".",","), array("","."), $dados['valor'][$indice1][$indice2][$tdiidnivel1])."'":"").") RETURNING dshid;");
							}
						}
					}
				} else {
					if(is_numeric(str_replace(array(".",","), array("","."), $dados2))) {
						$dtlid[] = $db->pegaUm("INSERT INTO painel.detalheseriehistorica(
           							   			sehid, dshqtde, ".$_CONFIGS[$indice1]['campo']." ".(($formatoinput['campovalor'] && $dados['valor'][$indice1][$indice2])?", dshvalor":"").")
   									   			VALUES ('".$dados['sehid']."', '".str_replace(array(".",","), array("","."), $dados2)."', '".$indice2."' ".(($formatoinput['campovalor'] && $dados['valor'][$indice1][$indice2])?", '".str_replace(array(".",","), array("","."), $dados['valor'][$indice1][$indice2])."'":"").") RETURNING dshid;");
					}
				}
		}
		$db->commit();
	}
	$db->executar("UPDATE painel.seriehistorica 
				   SET sehqtde=COALESCE((SELECT sum(qtde) FROM painel.v_detalheindicadorsh WHERE sehid='".$dados['sehid']."'),0) 
				   ".(($formatoinput['campovalor'])?", sehvalor=(SELECT sum(valor) FROM painel.v_detalheindicadorsh WHERE sehid='".$dados['sehid']."')":"")." 
				   WHERE sehid='".$dados['sehid']."'", false);
	
	switch($indice1) {
		case 'estado':
			$enderecoredi = "?modulo=principal/preenchimentoSerieHistorica&acao=A&dpeid=".$db->pegaUm("SELECT dpeid FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."'");
			break;
		case 'municipio':
			if($dados['dshuf'] && $dtlid) $db->executar("UPDATE painel.detalheseriehistorica SET dshuf='".$dados['dshuf']."' WHERE sehid='".$dados['sehid']."' AND dshid IN ('".implode("','",$dtlid)."')");
			$enderecoredi = "?modulo=principal/preenchimentoSerieHistorica&acao=A&dpeid=".$db->pegaUm("SELECT dpeid FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."'")."&estuf=".$dados['dshuf'];
			break;
		case 'posgraduacao':
			if($dados['muncod'] && $dados['estuf'] && $dtlid) $db->executar("UPDATE painel.detalheseriehistorica SET dshuf='".$dados['estuf']."', dshcodmunicipio='".$dados['muncod']."' WHERE sehid='".$dados['sehid']."' AND dshid IN ('".implode("','",$dtlid)."')");
			$enderecoredi = "?modulo=principal/preenchimentoSerieHistorica&acao=A&dpeid=".$db->pegaUm("SELECT dpeid FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."'")."&estuf=".$dados['estuf']."&muncod=".$dados['muncod'];
			break;
		case 'campusprofissional':
			if($dados['muncod'] && $dados['estuf'] && $dtlid) {
				$sql = "UPDATE 
						   painel.detalheseriehistorica e
						SET 
						   unicod = ent2.entunicod, dshuf='".$dados['estuf']."', dshcodmunicipio='".$dados['muncod']."'
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						INNER JOIN entidade.funcaoentidade fen ON fen.entid = dsh.entid
						INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
						INNER JOIN entidade.entidade ent2 ON ent2.entid = fea.entid
						INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = ent2.entid
						where i.regid in (9)
						and i.indstatus = 'A'
						and sh.sehstatus <> 'I'
						and dsh.entid is not null
						and fen2.funid in (11)
						AND e.dshid =dsh.dshid 
						AND e.dshid IN ('".implode("','",$dtlid)."')";
				$db->executar($sql, false);
				//$db->executar("UPDATE painel.detalheseriehistorica SET dshuf='".$dados['estuf']."', dshcodmunicipio='".$dados['muncod']."' WHERE sehid='".$dados['sehid']."' AND dshid IN ('".implode("','",$dtlid)."')");
			}
			$enderecoredi = "?modulo=principal/preenchimentoSerieHistorica&acao=A&dpeid=".$db->pegaUm("SELECT dpeid FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."'")."&estuf=".$dados['estuf']."&muncod=".$dados['muncod'];
			
			break;
		case 'campussuperior':

			if($dados['muncod'] && $dados['estuf'] && $dtlid) {
				$sql = "UPDATE 
						   painel.detalheseriehistorica e
						SET 
						   unicod = ent2.entunicod, dshuf='".$dados['estuf']."', dshcodmunicipio='".$dados['muncod']."' 
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						INNER JOIN entidade.funcaoentidade fen ON fen.entid = dsh.entid
						INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
						INNER JOIN entidade.entidade ent2 ON ent2.entid = fea.entid
						INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = ent2.entid
						where i.regid in (8)
						and i.indstatus = 'A'
						and sh.sehstatus <> 'I'
						and dsh.entid is not null
						and fen2.funid in (12)
						AND e.dshid =dsh.dshid 
						AND e.dshid IN ('".implode("','",$dtlid)."')";
				$db->executar($sql, false);
				//$db->executar("UPDATE painel.detalheseriehistorica SET dshuf='".$dados['estuf']."', dshcodmunicipio='".$dados['muncod']."' WHERE sehid='".$dados['sehid']."' AND dshid IN ('".implode("','",$dtlid)."')");
			}
			$enderecoredi = "?modulo=principal/preenchimentoSerieHistorica&acao=A&dpeid=".$db->pegaUm("SELECT dpeid FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."'")."&estuf=".$dados['estuf']."&muncod=".$dados['muncod'];
			
			break;

		case 'polo':
		case 'hospital':
		case 'instituto':
		case 'universidade':
			if($dados['muncod'] && $dados['estuf'] && $dtlid) $db->executar("UPDATE painel.detalheseriehistorica SET dshuf='".$dados['estuf']."', dshcodmunicipio='".$dados['muncod']."' WHERE sehid='".$dados['sehid']."' AND dshid IN ('".implode("','",$dtlid)."')");
			$enderecoredi = "?modulo=principal/preenchimentoSerieHistorica&acao=A&dpeid=".$db->pegaUm("SELECT dpeid FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."'")."&estuf=".$dados['estuf']."&muncod=".$dados['muncod'];
			break;
		case 'escola':
			if($dados['muncod'] && $dados['estuf'] && $dtlid) $db->executar("UPDATE painel.detalheseriehistorica SET dshuf='".$dados['estuf']."', dshcodmunicipio='".$dados['muncod']."' WHERE sehid='".$dados['sehid']."' AND dshid IN ('".implode("','",$dtlid)."')");
			$enderecoredi = "?modulo=principal/preenchimentoSerieHistorica&acao=A&dpeid=".$db->pegaUm("SELECT dpeid FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."'")."&estuf=".$dados['estuf']."&muncod=".$dados['muncod'];
			break;
		case 'iescpc':
			if($dados['muncod'] && $dados['estuf'] && $dtlid) $db->executar("UPDATE painel.detalheseriehistorica SET dshuf='".$dados['estuf']."', dshcodmunicipio='".$dados['muncod']."' WHERE sehid='".$dados['sehid']."' AND dshid IN ('".implode("','",$dtlid)."')");
			$enderecoredi = "?modulo=principal/preenchimentoSerieHistorica&acao=A&dpeid=".$db->pegaUm("SELECT dpeid FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."'")."&estuf=".$dados['estuf']."&muncod=".$dados['muncod'];
			break;
			
	}
	$db->commit();
	echo "<script>
			alert('Os dados foram salvos com sucesso');
			window.location='".$enderecoredi."';
	  	  </script>";
	exit;
	}
}

/**
 * Fun��o que grava o GRID de informa��o (geral)
 * 
 * @author Alexandre Dourado
 * @return void 
 * @param integer $dados
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function gravarGridDadosSemFiltros($dados) {
	global $db, $_CONFIGS;
	// limpando todas series historicas
	$db->executar("UPDATE painel.seriehistorica SET sehstatus='I' WHERE indid='".$_SESSION['indid']."'", false);
	$formatoinput = pegarFormatoInput();
	// verificando se existe item
	if($dados['item']) {
		// verificando de qual grid � o item ($indice1)
		foreach($dados['item'] as $indice1 => $dados1) {
			// verificando o dpeid ($indice2)
			foreach($dados1 as $indice2 => $dados2) {
				$sehid = $db->pegaUm("INSERT INTO painel.seriehistorica(indid, sehvalor, sehstatus, sehqtde, dpeid)
    								  VALUES ('".$_SESSION['indid']."', NULL, 'H', '0', '".$indice2."') RETURNING sehid;");
				$possuiregistro = false;
				if(is_array($dados2)) {
					foreach($dados2 as $tdiidnivel1 => $dados3) {
						$tdinumnivel1 =  'tidid'.$db->pegaUm("SELECT tdinumero FROM painel.detalhetipoindicador tdi 
										 	 	   		   	  INNER JOIN painel.detalhetipodadosindicador tid ON tid.tdiid = tdi.tdiid 
									 		 	   		   	  WHERE tidid='".$tdiidnivel1."'");
						if(is_array($dados3)) {
							foreach($dados3 as $tdiidnivel2 => $dados4) {
								$tdinumnivel2 =  'tidid'.$db->pegaUm("SELECT tdinumero FROM painel.detalhetipoindicador tdi 
												 	 	   		   	  INNER JOIN painel.detalhetipodadosindicador tid ON tid.tdiid = tdi.tdiid 
									 				 	   		   	  WHERE tidid='".$tdiidnivel2."'");
								if(is_numeric(str_replace(array(".",","), array("","."), $dados4))) {
									$sql = "INSERT INTO painel.detalheseriehistorica(sehid, dshqtde, ".$tdinumnivel1.", ".$tdinumnivel2." ".(($formatoinput['campovalor'] && $dados['valor'][$indice1][$indice2][$tdiidnivel1][$tdiidnivel2])?", dshvalor":"").")
								    		VALUES ('".$sehid."', '".str_replace(array(".",","), array("","."), $dados4)."', '".$tdiidnivel1."', '".$tdiidnivel2."' ".(($formatoinput['campovalor'] && $dados['valor'][$indice1][$indice2][$tdiidnivel1][$tdiidnivel2])?", '".str_replace(array(".",","), array("","."), $dados['valor'][$indice1][$indice2][$tdiidnivel1][$tdiidnivel2])."'":"").");";
									$db->executar($sql, false);
									$possuiregistro = true;
								}
							}
						} else {
							if(is_numeric(str_replace(array(".",","), array("","."), $dados3))) {
								$db->executar("INSERT INTO painel.detalheseriehistorica(
	            							   sehid, dshqtde, ".$tdinumnivel1." ".(($formatoinput['campovalor'] && $dados['valor'][$indice1][$indice2][$tdiidnivel1])?", dshvalor":"").")
	    									   VALUES ('".$sehid."', '".str_replace(array(".",","), array("","."), $dados3)."', '".$tdiidnivel1."' ".(($formatoinput['campovalor'] && $dados['valor'][$indice1][$indice2][$tdiidnivel1])?", '".str_replace(array(".",","), array("","."), $dados['valor'][$indice1][$indice2][$tdiidnivel1])."'":"").");", false);
								$possuiregistro = true;
							}
						}
					}
				} else {
					// adicionando a quantidade quando n�o houve detalhamento
					if(is_numeric(str_replace(array(".",","), array("","."), $dados2))) {
						$db->executar("INSERT INTO painel.detalheseriehistorica(
           							   sehid, dshqtde ".(($formatoinput['campovalor'] && $dados['valor'][$indice1][$indice2])?", dshvalor":"").")
   									   VALUES ('".$sehid."', '".str_replace(array(".",","), array("","."), $dados2)."' ".(($formatoinput['campovalor'] && $dados['valor'][$indice1][$indice2])?", '".str_replace(array(".",","), array("","."), $dados['valor'][$indice1][$indice2])."'":"").");", false);
						$possuiregistro = true;
					}
				}
				if($possuiregistro) {
					$db->executar("UPDATE painel.seriehistorica 
								   SET sehqtde=COALESCE((SELECT sum(qtde) FROM painel.v_detalheindicadorsh WHERE sehid='".$sehid."'),0) 
						   		   ".(($formatoinput['campovalor'])?", sehvalor=(SELECT sum(valor) FROM painel.v_detalheindicadorsh WHERE sehid='".$sehid."')":"")." 
						   		   WHERE sehid='".$sehid."'", false);
				} else {
					$db->executar("DELETE FROM painel.seriehistorica WHERE sehid='".$sehid."'", false);
				}
				
			}
			$sql = "UPDATE painel.seriehistorica SET sehstatus='A' WHERE sehstatus='H' AND sehid IN( SELECT sehid FROM painel.seriehistorica s
																				 				   INNER JOIN painel.detalheperiodicidade d ON s.dpeid = d.dpeid 
																				 				   WHERE indid='".$_SESSION['indid']."' ORDER BY dpedatainicio DESC LIMIT 1)";
			$db->executar($sql, false);
		}
		
	
		switch($indice1) {
			case 'brasil':
				$enderecoredi = "?modulo=principal/preenchimentoSerieHistorica&acao=A";
				break;
		}
		
		$db->commit();
		
		echo "<script>
				alert('Os dados foram salvos com sucesso');
				window.location='".$enderecoredi."';
		  	  </script>";
		exit;
	}
}

/**
 * Fun��o que lista todas as s�ries historicas cadastradas no indicador
 * 
 * @author Alexandre Dourado
 * @return void (AJAX)
 * @param integer $dados['dpeid'] ID do per�odo
 * @global class $db classe que inst�ncia o banco de dados 
 * @version v1.0 29/06/2009
 */
function listaSerieHistorica() {
	header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	
	define("UNIDADEMEDICAO_PERCENTUAL", 1);
	
	// pegando o formato do indicador
	$formatoinput = pegarFormatoInput();
	
	//Se o indicador for Percentual, n�o exibe o valor/quantidade
	$sql = "select unmid from painel.indicador where indid = {$_SESSION['indid']}";
	$unmid = $db->pegaUm($sql);
	if($unmid == UNIDADEMEDICAO_PERCENTUAL || $unmid == UNIDADEMEDICAO_RAZAO){
		$qtde = "'<center><span style=\"color:#990000\" >N/A</span></center>'";
	}else{
		$qtde = "'<div style=\"width:100%;text-align:right;color:#0066CC\">' || to_char(sehqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."') || '</div>' as qtde 
				   ".(($formatoinput['campovalor'])?", '<div style=\"width:100%;text-align:right;color:#0066CC\">' || to_char(sehvalor, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['campovalor']['mascara'])."') || '</div>' as valor":"");
	}
	
	// liberar bloqueio apenas para Super Usu�rio e Equipe de Apoio ao Gestor do PDE (Solicitado pelo Vitor Sad)
	$permissoes = verificaPerfilPainel();
	
	if($permissoes['bloquearseriehistorica']) {
		$actsim = "onclick=alterarBloqueio('|| seh.sehid ||',\'nao\'); border=0 style=cursor:pointer;width:16;height:16";
		$actnao = "onclick=alterarBloqueio('|| seh.sehid ||',\'sim\'); border=0 style=cursor:pointer;width:16;height:16";
	} else {
		$actsim = "border=0 style=width:16;height:16";
		$actnao = "border=0 style=width:16;height:16";
		$caddetalhe = "_p";		
	}
	
	// fim permissao
	$sql = "SELECT '<center>'|| CASE WHEN (sehbloqueado = true) THEN '<img src=../imagens/cadiado".$caddetalhe.".png ".$actsim."> <img src=../imagens/excel.gif style=cursor:pointer; onclick=exportarsehcsv('|| seh.sehid ||');>' ELSE '<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/preenchimentoSerieHistorica&acao=A&dpeid='||seh.dpeid||'\'\"> ".(($permissoes['removerseriehistorica'])?"<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirSerieHistorica('||sehid||');\">":"")." <img src=../imagens/excel.gif style=cursor:pointer; onclick=exportarsehcsv('|| seh.sehid ||');> <img src=../imagens/cadeadoAberto".$caddetalhe.".png ".$actnao.">' END ||'</center>' as acoes,
				   to_char(seh.sehdtcoleta,'DD/MM/YYYY') as data, 
				   dpe.dpedsc,
				   $qtde
			FROM painel.seriehistorica seh 
			LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
			WHERE seh.indid = '".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H') 
			ORDER BY dpedatainicio";
	
	$cabecalho = array("A��es", "Data de Coleta","Refer�ncia", $formatoinput['label']);
	if($formatoinput['campovalor'])$cabecalho[] = $formatoinput['campovalor']['label'];
	$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
}

function atualizardatacoleta($dados) {
	global $db;
	$sql = "SELECT sehid FROM painel.seriehistorica WHERE indid='".$_SESSION['indid']."' AND sehstatus='A'";
	$ultimaseh = $db->pegaUm($sql);
	
	if($ultimaseh) {
		$sql = "UPDATE painel.seriehistorica SET sehdtcoleta = NOW() WHERE sehid = '".$ultimaseh."'";
		$db->executar($sql);
		$db->commit();
	}
}



function bloquearSerieHistorica($dados) {
	global $db;
	if($dados['situacao'] == 'sim') {
		$sit = "sehbloqueado=TRUE";
	} elseif($dados['situacao'] == 'nao') {
		$sit = "sehbloqueado=FALSE";
	}
	
	if($dados['sehid'] && $sit) {
		$sql = "UPDATE painel.seriehistorica SET ".$sit." WHERE sehid='".$dados['sehid']."'";
		$db->executar($sql);
		$db->commit();
	}
}


function removerSerieHistorica($dados) {
	global $db;
	$sql = "UPDATE painel.seriehistorica SET sehstatus='I'  WHERE sehid='".$dados['sehid']."'";
	$db->executar($sql);
	
	// ativando a ultima serie historica
	$sql = "SELECT seh.sehid, seh.dpeid FROM painel.seriehistorica seh 
			LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
			WHERE indid=(SELECT indid FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."') AND (sehstatus='A' OR sehstatus='H') ORDER BY dpedatainicio DESC, sehid DESC LIMIT 1";
	
	$seriemaior = $db->pegaLinha($sql);
	if($seriemaior) {
		$sql = "UPDATE painel.seriehistorica SET sehstatus='A' WHERE sehid='".$seriemaior['sehid']."'";
		$db->executar($sql);
	}

	$db->commit();
}



function exportarsehcsv($dados) {
	global $db;
	
	$_VALIDACAO = criarestruturavalidacao($_SESSION['indid']);
	
	$CONV = array('acdqtde'          => 'dshqtde',
				  'acdmuncod'        => 'dshcodmunicipio',
				  'acduf'            => 'dshuf',
				  'acdvalor'         => 'dshvalor',
				  'acdesciescod'	 => 'dshcod',
				  'acdregiao'		 => 'dshregiao');
	
	if($_VALIDACAO) {
		$selalias = '';
		foreach($_VALIDACAO as $key => $val) {
			
			if($key == (count($_VALIDACAO)-1)) 
				$sep = "||'\n'";
			else 
				$sep = "||';'||";
			
				
			switch($val['funcaovalidacao']) {
				case 'validaracdvalor':
					if($CONV[$val['campo']]) $selalias .= "COALESCE(trim(to_char(".$CONV[$val['campo']].", '999g999g999g999d99')),'')".$sep;
					else $selalias .= "COALESCE(trim(to_char(".$val['campo'].", '999g999g999g999d99')),'')".$sep;
					break;
				case 'validaracdqtde':
					if($CONV[$val['campo']]) $selalias .= "trim(to_char(".$CONV[$val['campo']].", '99999999999'))".$sep;
					else $selalias .= "trim(to_char(".$val['campo'].", '99999999999'))".$sep;
					break;
				default:
					// se o campo correto da estrutura estiver vazio (possivelmente foi alterado a regionaliza��o)
					// ser� necessario encontrar qual o campo era carregado...(testando cada um)
					if($CONV[$val['campo']]) $selalias .= "CASE WHEN COALESCE(".$CONV[$val['campo']]."::varchar, '') != '' THEN ".$CONV[$val['campo']]." ELSE 
																																								CASE WHEN iepid IS NOT NULL THEN iepid::varchar 
																																								 	 WHEN entid IS NOT NULL THEN entid::varchar
																																								 	 WHEN unicod IS NOT NULL THEN unicod 
																																								 	 WHEN polid IS NOT NULL THEN polid::varchar 
																																								 	 WHEN iecid IS NOT NULL THEN iecid::varchar
																																								 	 WHEN dshuf IS NOT NULL THEN dshuf
																																								 END
																																						END".$sep;
					else $selalias .= $val['campo'].$sep;
					
					
			}
		}
	}
	
	$sql = "SELECT {$selalias} as line FROM painel.detalheseriehistorica d
			LEFT JOIN painel.seriehistorica s ON s.sehid = d.sehid 
			WHERE s.sehid='".$dados['sehid']."'";
	
	$linhas = $db->carregar($sql);
	
	if($linhas[0]) {
		foreach($linhas as $l) {
			$arqcsv .= $l['line'];
		}
	}
	
	header("Content-Type: text/html; charset=ISO-8859-1");
	header("Content-type: application/octet-stream");
	header("Content-Disposition: attachment; filename=\"indicador_".$_SESSION['indid']."_seriehistorica_".$dados['sehid'].".csv\"");
	echo $arqcsv;
	exit;

}

function sincronizarPainelPublico($dados) {
	global $db, $db2;
	
	$inicioSinc = getmicrotime();
	
	/* configura��es */
	ini_set("memory_limit", "3000M");
	set_time_limit(0);
	/* FIM configura��es */
	
	// CARREGANDO DADOS NOVOS
	
	$sql = "SELECT * FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."'";
	$dadosseh = $db->pegaLinha($sql);
	
	$sql = "SELECT * FROM painel.seriehistorica WHERE indid in (select indid from painel.seriehistorica where sehid='".$dados['sehid']."')";
	$serieshistoricas = $db->carregar($sql);
	
	if($serieshistoricas[0]) {
		foreach($serieshistoricas as $sh) {
			$_serieshistoricas_status[$sh['sehid']] = $sh['sehstatus'];
		}
	}
	

	$sql = "SELECT * FROM painel.detalheseriehistorica WHERE sehid='".$dados['sehid']."'";
	$dadosdtseh = $db->carregar($sql);

	$sql = "SELECT * FROM painel.detalhetipoindicador WHERE indid in (select indid from painel.seriehistorica where sehid='".$dados['sehid']."')";
	$detalhetipoindicador = $db->carregar($sql);
	
	if($detalhetipoindicador[0]) {
		foreach($detalhetipoindicador as $dti) {
			$sql = "SELECT * FROM painel.detalhetipodadosindicador WHERE tdiid='".$dti['tdiid']."'";
			$detalhetipodadosindicador[$dti['tdiid']] = $db->carregar($sql);
		}
	}

		
	// CARREGANDO DADOS NOVOS - FIM
	
	// APAGANDO PAINEL PUBLICO
	
	$arrSerie = $db->pegaLinha("SELECT dpeid, indid FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."'");
	
	$db2->executar("DELETE FROM painel.detalheseriehistorica WHERE sehid IN(SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$arrSerie['dpeid']."' AND indid='".$arrSerie['indid']."')", false);
	$db2->executar("DELETE FROM painel.seriehistorica WHERE sehid IN(SELECT sehid FROM painel.seriehistorica WHERE dpeid='".$arrSerie['dpeid']."' AND indid='".$arrSerie['indid']."')", false);
	
	// APAGANDO PAINEL PUBLICO
	
	// INSERINDO PAINEL PUBLICO
	
	$db2->executar("INSERT INTO painel.seriehistorica(
            		sehid, indid, sehvalor, sehstatus, sehqtde, dpeid, sehdtcoleta, regid, sehbloqueado)
    			   VALUES (".(($dadosseh['sehid'])?"'".$dadosseh['sehid']."'":"NULL").", 
    			   		   ".(($dadosseh['indid'])?"'".$dadosseh['indid']."'":"NULL").", 
    			   		   ".(($dadosseh['sehvalor'])?"'".$dadosseh['sehvalor']."'":"NULL").", 
    			   		   ".(($dadosseh['sehstatus'])?"'".$dadosseh['sehstatus']."'":"NULL").", 
    			   		   ".(($dadosseh['sehqtde'])?"'".$dadosseh['sehqtde']."'":"NULL").", 
    			   		   ".(($dadosseh['dpeid'])?"'".$dadosseh['dpeid']."'":"NULL").", 
    			   		   ".(($dadosseh['sehdtcoleta'])?"'".$dadosseh['sehdtcoleta']."'":"NULL").", 
    			   		   ".(($dadosseh['regid'])?"'".$dadosseh['regid']."'":"NULL").", 
    			   		   ".(($dadosseh['sehbloqueado']=="t")?"TRUE":"FALSE").");", false);
	
	if($dadosdtseh[0]) {
		foreach($dadosdtseh as $dt) {
			$sql = "INSERT INTO painel.detalheseriehistorica(
		            	dshid, ddiid, sehid, dshvalor, dshcod, dshcodmunicipio, dshuf, 
		            	dshqtde, tidid1, tidid2, iepid, entid, unicod, polid, iecid)
				      VALUES (".(($dt['dshid'])?"'".$dt['dshid']."'":"NULL").", 
				      		  ".(($dt['ddiid'])?"'".$dt['ddiid']."'":"NULL").", 
				      		  ".(($dt['sehid'])?"'".$dt['sehid']."'":"NULL").", 
				      		  ".(($dt['dshvalor'])?"'".$dt['dshvalor']."'":"NULL").", 
				      		  ".(($dt['dshcod'])?"'".$dt['dshcod']."'":"NULL").", 
				      		  ".(($dt['dshcodmunicipio'])?"'".$dt['dshcodmunicipio']."'":"NULL").", 
				      		  ".(($dt['dshuf'])?"'".$dt['dshuf']."'":"NULL").",
				      		  ".(($dt['dshqtde'])?"'".$dt['dshqtde']."'":"NULL").", 
				              ".(($dt['tidid1'])?"'".$dt['tidid1']."'":"NULL").", 
				              ".(($dt['tidid2'])?"'".$dt['tidid2']."'":"NULL").", 
				              ".(($dt['iepid'])?"'".$dt['iepid']."'":"NULL").", 
				              ".(($dt['entid'])?"'".$dt['entid']."'":"NULL").", 
				              ".(($dt['unicod'])?"'".$dt['unicod']."'":"NULL").", 
				              ".(($dt['polid'])?"'".$dt['polid']."'":"NULL").", 
				              ".(($dt['iecid'])?"'".$dt['iecid']."'":"NULL")." 
				              )";

			$db2->executar($sql, false);
		}
		

	}
	
	// atualizando status serie historica
	$sql = "SELECT * FROM painel.seriehistorica WHERE indid in (select indid from painel.seriehistorica where sehid='".$dados['sehid']."')";
	$serieshistoricas = $db2->carregar($sql);
	
	if($serieshistoricas[0]) {
		foreach($serieshistoricas as $sh) {
			if($_serieshistoricas_status[$sh['sehid']] != $sh['sehstatus'] && $_serieshistoricas_status[$sh['sehid']] != "") {
				$db2->executar("UPDATE painel.seriehistorica SET sehstatus='".$_serieshitoricas_status[$sh['sehid']]."' WHERE sehid='".$sh['sehid']."'", false);
			}
		}
	}
	
	
	// INSERINDO PAINEL PUBLICO - FIM
	
	$db2->commit();
	
	echo "Sincroniza��o efetuada com sucesso em ".(getmicrotime() - $inicioSinc)." segundos";	
	
}

function dadosSincronizacao() {
	global $db, $db2;
	
	$existeIndPP = $db2->pegaUm("SELECT indid FROM painel.indicador WHERE indid='".$_SESSION['indid']."'");
	
	// pegando o formato do indicador
	$formatoinput = pegarFormatoInput();
	if($formatoinput['unmid'] == UNIDADEMEDICAO_PERCENTUAL || $formatoinput['unmid'] == UNIDADEMEDICAO_RAZAO){
		$qtde = "'<center><span style=\"color:#990000\" >N/A</span></center>'";
	}else{
		$qtde = "'<div style=\"width:100%;text-align:right;color:#0066CC\">' || to_char(sehqtde, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."') || '</div>' as qtde 
				   ".(($formatoinput['campovalor'])?", '<div style=\"width:100%;text-align:right;color:#0066CC\">' || to_char(sehvalor, '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['campovalor']['mascara'])."') || '</div>' as valor":"");
	}

	$sql = "SELECT '<center><img src=../imagens/excel.gif title=\'Exportar CSV\' style=cursor:pointer; onclick=exportarsehcsv('|| seh.sehid ||',\'nao\');> ".(($existeIndPP)?"<img src=\"/imagens/reject.png\" border=0 title=\"Sincronizar\" style=\"cursor:pointer;\" onclick=\"sincronizarPainelPublico('||sehid||');\">":"")."</center>' as acoes,
				   to_char(seh.sehdtcoleta,'DD/MM/YYYY') as data, 
				   dpe.dpedsc,
				   $qtde
			FROM painel.seriehistorica seh 
			LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
			WHERE seh.indid = '".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H') 
			ORDER BY dpedatainicio";
				   
	$cabecalho = array("A��es", "Data de Coleta","Refer�ncia", $formatoinput['label']);	
	if($formatoinput['campovalor'])$cabecalho[] = $formatoinput['campovalor']['label'];
	
	echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>
		  <tr>
		  <td class=SubtituloCentro>Painel de Controle</td>
		  </tr>
		  </table>";
	
	$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
	
	echo "<br>";
	echo "<br>";
	
	if($existeIndPP) {
		$sql = "SELECT '<center><img src=../imagens/excel.gif style=cursor:pointer; title=\'Exportar CSV\'  onclick=exportarsehcsv('|| seh.sehid ||',\'sim\');> <img border=0 onclick=excluirSerieHistoricaPainelPublico('||sehid||'); style=cursor: pointer; title=Excluir src=/imagens/excluir.gif></center>' as acoes,
					   to_char(seh.sehdtcoleta,'DD/MM/YYYY') as data, 
					   dpe.dpedsc,
					   $qtde
				FROM painel.seriehistorica seh 
				LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
				WHERE seh.indid = '".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H') 
				ORDER BY dpedatainicio";
					   
		echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>
			  <tr>
			  <td class=SubtituloCentro>Painel P�blico</td>
			  </tr>
			  </table>";
		
		$db2->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
		
		$sql = "SELECT seh.sehid
				FROM painel.seriehistorica seh 
				LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
				WHERE seh.indid = '".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H') 
				ORDER BY dpedatainicio";
		
		$sehids = $db->carregar($sql);
		
		if($sehids[0]) {
			foreach($sehids as $seh) {
				$vlr[] = $seh['sehid'];
			}
			echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
			echo "<tr><td align=center><input type=button value='Sincronizar todas s�ries historicas' onclick=sincronizarTodasSeh('".implode(";",$vlr)."');></td></tr>";
			echo "</table>";
		}
	}
	
	
	
	
}

function getDadosIndicador($indid,$db)
{
	$sql = "SELECT 
				ind.indid,
				unm.unmdesc,
				tip.tpidsc,
				sec.secdsc as secretaria,
				secgestora.secdsc as secdsc,
				aca.acadsc,
				per.perdsc,
				per2.perdsc as perdata,
				est.estdsc,
		        indnome, indobjetivo, indformula, indtermos, indfontetermo, 
		        ume.umedesc, reg.regdescricao, exo.exodsc, indobservacao,coldsc, 
		        case ind.indescala when 't' then 'Sim' when 'f' then 'N�o' end as escala, 
		        case ind.indcumulativo when 'S' then 'Sim' when 'N' then 'N�o' when 'A' then 'Anual' end as qndtacumulada, 
		        case ind.indqtdevalor when 't' then 'Sim' when 'f' then 'N�o' end as valormonetario, 
		        case ind.indcumulativovalor when 'S' then 'Sim' when 'N' then 'N�o' when 'A' then 'Anual' end as valoracumulado, 
		        case ind.indpublicado when 't' then 'Sim' when 'f' then 'N�o' end as publicado, 
		        case ind.indpublico when 't' then 'Sim' when 'f' then 'N�o' end as publico
		FROM painel.indicador ind
		INNER JOIN painel.unidademedicao unm ON unm.unmid = ind.unmid
		inner join painel.tipoindicador tip ON tip.tpiid = ind.tpiid
		inner join painel.secretaria sec on ind.secid = sec.secid
		inner join painel.secretaria secgestora on ind.secidgestora = secgestora.secid
		inner join painel.acao aca on ind.acaid = aca.acaid
		inner join painel.periodicidade per on ind.perid = per.perid
		inner join painel.periodicidade per2 on ind.peridatual = per2.perid
		inner join painel.estilo est on est.estid = ind.estid
		inner join painel.unidademeta ume on ume.umeid = ind.umeid
		inner join painel.regionalizacao reg on reg.regid = ind.regid
		inner join painel.eixo exo on exo.exoid = ind.exoid
		inner join painel.coleta col on col.colid = ind.colid
		where ind.indid = $indid";
	return $db->pegaLinha($sql);
}

function sicronizarIndicador()
{
	global $db, $db2;
	
	/* configura��es */
	ini_set("memory_limit", "3000M");
	set_time_limit(0);
	
	$indid = $_POST['indid'];
	
	//Dados
	$sql = "select * from painel.indicador where indid = $indid";
	$arrDados = $db->pegaLinha($sql);
	
	$sqlCampos = "	SELECT DISTINCT
						pg_attribute.attname AS coluna,
						pg_attribute.attnotnull as naonulo
					FROM 
						pg_class
					JOIN 
						pg_namespace ON pg_namespace.oid = pg_class.relnamespace AND pg_namespace.nspname NOT LIKE 'pg_%'
					JOIN 
						pg_attribute ON pg_attribute.attrelid = pg_class.oid AND pg_attribute.attisdropped = 'f'
					JOIN
						pg_type ON pg_type.oid = pg_attribute.atttypid
					JOIN 
						pg_index ON pg_index.indrelid=pg_class.oid
					LEFT JOIN
						pg_constraint ON (pg_attribute.attrelid = pg_constraint.conrelid AND pg_constraint.conkey[1] = pg_attribute.attnum AND pg_constraint.contype != 'u')
					WHERE 
						pg_class.relname = 'indicador'
					AND
						pg_namespace.nspname = 'painel'
					AND 
						pg_attribute.attnum > 0
					AND 
						pg_attribute.attrelid = pg_class.oid
					AND 
						pg_attribute.atttypid = pg_type.oid
					";
	$arrCampos = $db2->carregar($sqlCampos);
	
	if($arrCampos && is_array($arrCampos)):
		foreach($arrCampos as $campo):
			$arrSet[] = $campo['coluna']." = ".($arrDados[$campo['coluna']] && $arrDados[$campo['coluna']] != "" ? "'".$arrDados[$campo['coluna']]."'" : "NULL");
			$arrCamp[] = $campo['coluna'];
			$arrVal[] = ($arrDados[$campo['coluna']] && $arrDados[$campo['coluna']] != "" ? "'".$arrDados[$campo['coluna']]."'" : "NULL");
		endforeach;
	endif;
	
	if($arrSet):
			
		$sqlI = "select indid from painel.indicador where indid = $indid";
		$indid2 = $db2->pegaUm($sqlI);
		if($indid2){
			//in�cio - painel.indicador
			$sqlU = "update
					painel.indicador
				set
				".($arrSet ? implode(",",$arrSet) : "")."
				where
					indid = $indid2";
			$db2->executar($sqlU);
			//fim - painel.indicador
		}else{
			//in�cio - painel.indicador
			$sqlI = "insert into
					painel.indicador
					(".($arrCamp ? implode(",",$arrCamp) : "").")
				values
					(".($arrVal ? implode(",",$arrVal) : "").") 
				returning indid";
			$indid2 = $db2->pegaUm($sqlI);
			//fim - painel.indicador
		}
		
		//in�cio - painel.detalhetipoindicador
		$sql = "select 
					*
				from
					painel.detalhetipoindicador
				where
					indid = $indid";
		$arrDetalhes = $db->carregar($sql);
		if($arrDetalhes && is_array($arrDetalhes)):
			foreach($arrDetalhes as $detalhe):
				$sql = "select tdiid from painel.detalhetipoindicador where indid = $indid2 and tdiid = {$detalhe['tdiid']}";
				$tdiid = $db2->pegaUm($sql);
				if($tdiid):
					$sqlUD .= "update painel.detalhetipoindicador set tdistatus = '{$detalhe['tdistatus']}', tdiordem = '{$detalhe['tdiordem']}', tdidsc = '{$detalhe['tdidsc']}', tdinumero = '{$detalhe['tdinumero']}' where indid = $indid2 and tdiid = {$detalhe['tdiid']};";
				else:
					$sqlUD .= "insert into painel.detalhetipoindicador (tdiid,indid,tdistatus,tdiordem,tdidsc,tdinumero) values({$detalhe['tdiid']},$indid2,'{$detalhe['tdistatus']}','{$detalhe['tdiordem']}','{$detalhe['tdidsc']}','{$detalhe['tdinumero']}');";
				endif;
				$arrTdiid[] = $detalhe['tdiid'];
			endforeach;
		endif;
		if($sqlUD){
			$db2->executar($sqlUD, false);
		}
		//fim - painel.detalhetipoindicador
		
		//in�cio - painel.detalhetipodadosindicador
		if(is_array($arrTdiid)):
			$sql = "select 
						*
					from
						painel.detalhetipodadosindicador
					where
						tdiid in (".implode(",",$arrTdiid).")";
			$arrTipos = $db->carregar($sql);
			if($arrTipos && is_array($arrTipos)):
				foreach($arrTipos as $tipo):
					$sql = "select tidid from painel.detalhetipodadosindicador where tidid = {$tipo['tidid']} and tdiid = {$tipo['tdiid']}";
					$tidid = $db2->pegaUm($sql);
					if($tidid):
						$sqlUT .= "update painel.detalhetipodadosindicador set tiddsc = '{$tipo['tiddsc']}', tidstatus = '{$tipo['tidstatus']}' where tidid = {$tipo['tidid']} and tdiid = {$tipo['tdiid']};";
					else:
						$sqlUT .= "insert into painel.detalhetipodadosindicador (tidid,tdiid,tiddsc,tidstatus) values({$tipo['tidid']},'{$tipo['tdiid']}','{$tipo['tiddsc']}','{$tipo['tidstatus']}');";
					endif;
				endforeach;
			endif;
			if($sqlUT){
				$db2->executar($sqlUT, false);
			}
		endif;
		//fim - painel.detalhetipodadosindicador
		
		$db2->commit();
	endif;
	
	header("Location: http://painel.mec.gov.br/limparCache.php");
	
}

function getDetalhesIndicador($indid,$db)
{
			$sql = "select 
						*
					from
						painel.detalhetipoindicador det
					inner join
						painel.detalhetipodadosindicador tip ON det.tdiid = tip.tdiid
					where
						indid = $indid
					order by
						det.tdiid,tip.tidid";
			$arrDados = $db->carregar($sql);
			
			if($arrDados):
				foreach($arrDados as $dado):
					$arrD[$dado['tdiid']]['tdidsc']    = $dado['tdidsc'];
					$arrD[$dado['tdiid']]['tdistatus'] = $dado['tdistatus'];
					$arrD[$dado['tdiid']]['tdiordem']  = $dado['tdiordem'];
					$arrD[$dado['tdiid']]['tdinumero'] = $dado['tdinumero'];
					$arrD[$dado['tdiid']]['tipo'][]    = array("tidid" => $dado['tidid'], "tiddsc" => $dado['tiddsc'], "tidstatus" => $dado['tidstatus']);
				endforeach;
				return $arrD;
			else:
				return false;
			endif;
}

function removerSerieHistoricaPainelPublico($dados) {
	global $db2;
	$sql = "UPDATE painel.seriehistorica SET sehstatus='I'  WHERE sehid='".$dados['sehid']."'";
	$db2->executar($sql, false);
	
	// ativando a ultima serie historica
	$sql = "SELECT seh.sehid, seh.dpeid FROM painel.seriehistorica seh 
			LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
			WHERE indid=(SELECT indid FROM painel.seriehistorica WHERE sehid='".$dados['sehid']."') AND (sehstatus='A' OR sehstatus='H') ORDER BY dpedatainicio DESC, sehid DESC LIMIT 1";
	
	$seriemaior = $db2->pegaLinha($sql);
	if($seriemaior) {
		$sql = "UPDATE painel.seriehistorica SET sehstatus='A' WHERE sehid='".$seriemaior['sehid']."'";
		$db2->executar($sql, false);
	}

	$db2->commit();
}

?>