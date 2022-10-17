<?php
/**
 * Função que mostra a lista dos agendamentos do indicador
 * 
 * @author Alexandre Dourado
 * @return void htmlcode (ajax)
 * @param integer $dados[indid] ID do indicador
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function carregaragendamento($dados) {
	global $db;
	$arrPflcod = pegaPerfil();
	
	if(!$arrPflcod[0]) $arrPflcod = array();
	
	if(!in_array(PAINEL_PERFIL_CONSULTA,$arrPflcod) && !in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod) && !in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod)){
		$sql = "SELECT 
					'<center>
						<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirAgendamento('||agdid||');\">
						' ||
						CASE WHEN agdprocessado=true 
							THEN '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;' 
							ELSE '<img src=\"/imagens/par_amarelo.gif\" border=0 title=\"Processar agora\" style=\"cursor:pointer;\" onclick=\"janela(\'./processaragendamento.php?fecharjanela=true&agora=true&agdid=' || agdid || '\', 50, 50,\'Agendamento\');\">' 
						END
						  || ' 
					 </center>' AS acao, 
						'#'||agdid, 
						to_char(agd.agddata, 'dd/mm/YYYY HH24:MI')||' por '||usu.usunome as responsavel, 
						to_char(agd.agddataprocessamento, 'dd/mm/YYYY') as agddataprocessamento, 
						CASE WHEN agdprocessado=true THEN 'PROCESSADO' ELSE 'NÃO PROCESSADO' END 
				FROM painel.agendamentocarga agd 
				LEFT JOIN seguranca.usuario usu ON usu.usucpf = agd.usucpf 
				WHERE 
					indid='".$_SESSION['indid']."' 
					AND agdstatus='A'
				ORDER BY agddata desc";
	}else{		
		$sql = "SELECT 
					'<center><img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"alert(\'Operação Não Permitida!\');\">' AS acao, 
					'#'||agdid, 
					to_char(agd.agddata, 'dd/mm/YYYY HH24:MI')||' por '||usu.usunome as responsavel, 
					to_char(agd.agddataprocessamento, 'dd/mm/YYYY') as agddataprocessamento, 
					CASE WHEN agdprocessado=true THEN 'PROCESSADO' ELSE 'NÃO PROCESSADO' END 
				FROM painel.agendamentocarga agd 
				LEFT JOIN seguranca.usuario usu ON usu.usucpf = agd.usucpf 
				WHERE 
					indid='".$_SESSION['indid']."' 
					AND agdstatus='A'
				ORDER BY agddata desc";
	}
	
	if(in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod) && !$db->testa_superuser()){
		
		$sql2 = "SELECT 
					indid
				FROM 
					painel.indicador
				WHERE
					acaid in (
								select 
									distinct acaid
								from
									painel.usuarioresponsabilidade
								where
									usucpf = '{$_SESSION['usucpf']}'
								and
									rpustatus = 'A'
								)";
		$arrIndicadores = $db->carregar($sql2);
	}
	if(in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) && !$db->testa_superuser()){
		
		$sql2 = "SELECT 
					indid
				FROM 
					painel.indicador
				WHERE
					exoid in (
								select 
									distinct exoid
								from
									painel.usuarioresponsabilidade
								where
									usucpf = '{$_SESSION['usucpf']}'
								and
									rpustatus = 'A'
								)";
		$arrIndicadores = $db->carregar($sql2);
	}
		
	if($arrIndicadores){
		foreach($arrIndicadores as $Ind){
			$arrInd[] = $Ind['indid'];
		}
	}else{
		$arrIndicadores = array();
		$arrInd = array();
	}
	
	if((in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) || in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod)) && !in_array($dados['indid'],$arrInd)){
		$sql = "SELECT '<center><img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"alert(\'Operação Não Permitida!\');\">' AS acao, '#'||agdid, to_char(agd.agddata, 'dd/mm/YYYY HH24:MI')||' por '||usu.usunome as responsavel, to_char(agd.agddataprocessamento, 'dd/mm/YYYY') as agddataprocessamento, CASE WHEN agdprocessado=true THEN 'PROCESSADO' ELSE 'NÃO PROCESSADO' END FROM painel.agendamentocarga agd 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = agd.usucpf 
			WHERE indid='".$_SESSION['indid']."' AND agdstatus='A'
			ORDER BY agddata desc";
	}
	if((in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) || in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod)) && in_array($dados['indid'],$arrInd)){
		$sql = "SELECT '<center><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirAgendamento('||agdid||');\">' AS acao, '#'||agdid, to_char(agd.agddata, 'dd/mm/YYYY HH24:MI')||' por '||usu.usunome as responsavel, to_char(agd.agddataprocessamento, 'dd/mm/YYYY') as agddataprocessamento, CASE WHEN agdprocessado=true THEN 'PROCESSADO' ELSE 'NÃO PROCESSADO' END FROM painel.agendamentocarga agd 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = agd.usucpf 
			WHERE indid='".$_SESSION['indid']."' AND agdstatus='A'
			ORDER BY agddata desc";
	}
	
	$cabecalho = array("","Agendamento","Responsável","Agendado para","Processado");
	$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
	exit;
}

function excluirAgendamento($dados) {
	global $db;
	// verificando se o agpid existe e ele faz parte do indicador da sessão
	$sql = "SELECT agdid FROM painel.agendamentocarga WHERE agdid='".$dados['agdid']."' AND indid='".$_SESSION['indid']."'";
	$agpid = $db->pegaUm($sql);
	if($agpid) {
		$sql = "DELETE FROM painel.agendamentocargadados WHERE agdid='".$agpid."'";
		$db->executar($sql);
		$sql = "DELETE FROM painel.agendamentocarga WHERE agdid='".$agpid."'";
		$db->executar($sql);
		$db->commit();
	}
	exit;
}

function formatopadrao($valor) {
	return (($valor)?"'".$valor."'":"NULL");
}

function formatovalor($valor) {
	return (($valor)?"'".str_replace(array(".",","),array("","."),$valor)."'":"NULL");
}


function formatodata($valor) {
	$dta = explode("/",$valor);
	return str_pad($dta[2], 4, "0", STR_PAD_LEFT)."-".str_pad($dta[1], 2, "0", STR_PAD_LEFT)."-".str_pad($dta[0], 2, "0", STR_PAD_LEFT);
}

/**
 * Função que valida se o estado existe
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param string $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function validarsiglauf($valor) {
	$erro=false;
	$existe = $db->pegaUm("SELECT estuf FROM territorios.estado WHERE estuf='".$valor."'");
	if(!$existe) {
		$erro[] = "UF não existe";			
	}
	return $erro;
}
/**
 * Função que valida se o municipio existe, utilizando o código do IBGE
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param string $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function validarmunicipioibge($valor) {
	$erro=false;
	if(!is_numeric($valor)) {
		$erro[] = "Código do municipio não é númerico";
	} else {
		$existe = $db->pegaUm("SELECT muncod FROM territorios.municipio WHERE muncod='".$valor."'");
		if(!$existe) {
			$erro[] = "ID do municipio não existe(".$valor.")";			
		}
	}
	return $erro;
}
/**
 * Função que valida se o valor do indicador é númerico e igual ao indicador selecionado
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param integer $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function validarindicadorid($valor) {
	$erro=false;
	if(!is_numeric($valor)) {
		$erro[] = "ID do indicador não é númerico";
	}
	if($valor != $_SESSION['indid']) { 
		$erro[] = "Indicador da planilha não confere com sistema";
	}
	return $erro;
}
/**
 * Função que valida se o valor da periodicidade do indicador é númerico e existe no banco de dados
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param integer $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function validarperiodicidadeid($valor) {
	global $db, $_OTIMIZAR_ACOES_DPEID;
	$erro=false;
	if(!is_numeric($valor)) {
		$erro[] = "ID da Periodicidade não é númerico";
	} else {
		if(!$_OTIMIZAR_ACOES_DPEID[$_SESSION['indid']][$valor]) {
			$erro[] = "ID da periodicidade não existe";			
		}
	}
	return $erro;
}

function validarpolo($valor) {
	global $db;
	$erro=false;
	if(!is_numeric($valor)) {
		$erro[] = "Código do polo não é númerico";
	} else {
		$existe = $db->pegaUm("SELECT muncod FROM painel.polo WHERE polid='".$valor."'");
		if(!$existe) {
			$erro[] = "ID do polo não existe (".$valor.")";			
		}
	}
	return $erro;
}

function validartidid($valor) {
	global $db;
	$erro=false;
	if(!is_numeric($valor)) {
		$erro[] = "ID do Detalhe não é númerico";
	} else {
		$existe = $db->pegaUm("SELECT tidid FROM painel.detalhetipodadosindicador WHERE tidid='".$valor."'");
		if(!$existe) {
			$erro[] = "ID do Detalhe não existe";			
		}
	}
	return $erro;
}
/**
 * Função que valida se o valor do detalhes do indicador é númerico e existe no banco de dados
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param integer $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function validarddiid($valor) {
	global $db;
	$erro=false;
	if($valor) {
		if(!is_numeric($valor)) {
			$erro[] = "ID do detalhe não é númerico";
		} else {
			$existe = $db->pegaUm("SELECT ddiid FROM painel.detalhedadoindicador WHERE ddiid='".$valor."'");
			if(!$existe) {
				$erro[] = "ID do detalhe não existe";			
			}
		}
	}
	return $erro;
}
/**
 * Função que valida se a quantidade informada é númerica
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param integer $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function validaracdqtde($valor) {
	$erro=false;
	if(!is_numeric(str_replace(array(".",","), array("","."), $valor))) {
		$erro[] = "Quantidade não é inteiro";
	}
	return $erro;
}
/**
 * Função que valida se o valor monetário é númerico
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param integer $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function validaracdvalor($valor) {
	$erro=false;
	if(!is_numeric(str_replace(array(".",","),array("","."),$valor))) {
		$erro[] = "Valor não é válido";
	}
	return $erro;
}
/**
 * Função que valida se o municipio existe, utilizando o código do IBGE
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param string $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function validarmuncod($valor) {
	global $db;
	$erro=false;
	if(!is_numeric($valor)) {
		$erro[] = "Código do município não é númerico";
	} else {
		$existe = $db->pegaUm("SELECT muncod FROM territorios.municipio WHERE muncod='".$valor."'");
		if(!$existe) {
			$erro[] = "ID do municipio não existe (".$valor.")";			
		}
	}
	return $erro;
}
/**
 * Função que valida se o estado existe
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param string $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function validaruf($valor) {
	global $db;
	$erro=false;
	$existe = $db->pegaUm("SELECT estuf FROM territorios.estado WHERE estuf='".$valor."'");
	if(!$existe) {
		$erro[] = "UF não existe";			
	}
	return $erro;
}

/**
 * Função que valida se a região existe
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param string $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function validarregiao($valor) {
	global $db;
	$erro=false;
	$existe = $db->pegaUm("SELECT regdescricao FROM territorios.regiao WHERE regcod='".$valor."'");
	if(!$existe) {
		$erro[] = "Região não existe";			
	}
	return $erro;
}
/**
 * Função se o id da escola é numerico e se encontra no banco de dados
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param integer $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function validarcodescola($valor) {
	global $db, $_OTIMIZAR_ACOES_TERRITORIO;
	$erro=false;
	if(!$_OTIMIZAR_ACOES_TERRITORIO[trim($valor)]) {
		$erro[] = "Código do INEP não existe";			
	}
	return $erro;
}
/**
 * Função se o id da escola é numerico e se encontra no banco de dados
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param integer $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function validarcodies($valor) {
	global $db;
	$erro=false;
	if(!is_numeric($valor)) {
		$erro[] = "Código do IES não é númerico";
	} else {
		$existe = $db->pegaUm("SELECT iesid FROM painel.ies WHERE iesid='".$valor."'");
		if(!$existe) {
			$erro[] = "Código do IES não existe";			
		}
	}
	return $erro;
}
/**
 * Função se o id do Sistema Nacional de Pós-graduação é numerico e se encontra no banco de dados
 * 
 * @author Juliano Meinen
 * @return array $erros (contendo lista de erros)
 * @param integer $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 10/09/2009
 */
function validarcodiepg($valor) {
	global $db;
	$erro=false;
	if(!is_numeric($valor)) {
		$erro[] = "Código do IEPG não é númerico";
	} else {
		$existe = $db->pegaUm("SELECT iepid FROM painel.iepg WHERE iepid='".$valor."'");
		if(!is_numeric($existe)) {
			$erro[] = "Código do IEPG não existe";			
		}
	}
	return $erro;
}

/**
 * Função se o id do unidade é numerico e se encontra no banco de dados
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param integer $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 10/09/2009
 */
function validarcodunidade($valor) {
	global $db;
	$erro=false;
	if(!is_numeric($valor)) {
		$erro[] = "Código da Unidade não é númerico";
	} else {
		$existe = $db->pegaUm("SELECT unicod FROM public.unidade WHERE unicod='".$valor."'");
		if(!$existe) {
			$erro[] = "Código da Unidade não existe";			
		}
	}
	return $erro;
}

/**
 * Função se o id do Campus é numerico e se encontra no banco de dados
 * 
 * @author Alexandre Dourado
 * @return array $erros (contendo lista de erros)
 * @param integer $valor
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 10/09/2009
 */
function validarcodentidade($valor) {
	global $db;
	$erro=false;
	if(!is_numeric($valor)) {
		$erro[] = "Código do Campus não é númerico";
	} else {
		$existe = $db->pegaUm("SELECT entid FROM entidade.entidade WHERE entid='".$valor."'");
		if(!$existe) {
			$erro[] = "Código do Campus não existe";			
		}
	}
	return $erro;
}


function validariecid($valor) {
	global $db;
	$erro=false;
	if(!is_numeric($valor)) {
		$erro[] = "Código do IES CDC não é númerico";
	} else {
		$existe = $db->pegaUm("SELECT iecid FROM painel.iescpc WHERE iecid='".$valor."'");
		if(!$existe) {
			$erro[] = "Código do IES CDC não existe";			
		}
	}
	return $erro;
}

/**
 * Função que cria o código contendo o modelo de carga (incluindo exemplos, tabelas de apoio, etc)
 * 
 * @author Alexandre Dourado
 * @return htmlcode (Tutorial)
 * @param array $dados (atualmente vazio)
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function verdicionariocargawebservice($dados) {
	global $db;
	
	$sql = "SELECT indnome FROM painel.indicador WHERE indid = '{$_SESSION['indid']}'";
	$indnome = $db->pegaUm($sql); 

	$HTML .= "<table bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$HTML .= "<tr><td>Agendamento de valores por meio Upload/Download de arquivos</td></tr>
		  	  <tr><td><b>1. Descrição</b>
		          <span style=\"text-align:justify\" >
		  		  <p>O Agendamento de valores através do Download/Upload foi criado para viabilizar um meio mais rápido de alteração de valores por meio de transferência de arquivos. Esta opção permite que você faça o download de um arquivo padronizado, onde as séries históricas das informações do Painel possam ser inseridas e atualizadas por meio do upload de arquivo.</p>
			      <p>A transferência de dados entre o usuário e o sistema Painel deve ser feita através de um arquivo em formato CSV (ver exemplo abaixo). O conteúdo dos arquivos deverá obedecer a estrutura definida para cada informação do Painel, ou seja, dependendo da regionalização, periodicidade e detalhamento, a estrutura dos dados deverá mudar. Em seguida, será possível realizar o processo de upload para que as informações contidas no arquivo, sejam devidamente atualizadas no Painel.</p>
			      <p style=\"color:#0000CD\" ><b>Dica:</b> Caso o seu órgão tenha uma equipe de suporte à Tecnologia da Informação - TI, entre em contato com eles para obter ajuda na geração do arquivo em formato CSV.</p>	
				  </span>
		  		  <b>2. Exemplo do arquivo CSV</b>
				  <p>Com objetivo de facilitar o entendimento da estrutura do arquivo CSV, exemplificamos abaixo uma estrutura simples do arquivo esperado pelo sistema <span style=\"color:#0000CD\" >Painel</span> para atualização da série histórica. Cada coluna do arquivo está separada por cores que representam as seguintes informações:</p>
			  	  </td>
			  </tr>";
	
	$_VALIDACAO = criarestruturavalidacao($_SESSION['indid']);
	if($_VALIDACAO) {
		foreach($_VALIDACAO as $key => $valid) {
			if($key==(count($_VALIDACAO)-1)) $sep="";
			else $sep=";";
			$csvlabel     .= "<font color='".$valid['cor']."'><b>".$valid['label']."</b></font>".$sep;
			$arrEx[] = array("cor" => $valid['cor'], "valor" => $valid['valor'], "sep" => $sep);
			$arrTabela[] = array("label" => $valid['label'],"tabela" => $valid['tabela'], "desc_tabela" => $valid['desc_tabela'], "cor" => $valid['cor']);
		}
	}
	$csvlabel .= "<br>";
	$i = 0;
	while( $i < 10){
		$i++;
		foreach ($arrEx as $arr){
			if($arr['valor'] != $_SESSION['indid'] && !is_string($arr['valor'])){
				$arr['valor'] = $arr['valor'] + rand(1,100);
			}
			$csvlabel .= "<font color='".$arr['cor']."'><b>x</b></font>".$arr['sep'];
		}
		$csvlabel .= "<br>";
	}
	
	$HTML .= "<tr><td>".$csvlabel."<br>
		          <b>Atenção:</b></br>
			      1) A primeira linha do arquivo deve conter o título dos campos.</br>
			      2) Todos os campos deverão ser separados por ponto e vírgula \";\".</br>
			      3) Os valores deverão ser escritos no padrão brasileiro (###.###.###,##).<br>
			      4) Para visualizar um exemplo do arquivo CSV especificamente para este indicador, <a href='?modulo=principal/cargaporindicador&acao=A&requisicao=downloadmodelocarga'>clique aqui</a>.
		      </td></tr>
		      <tr><td>
		      <b>3. Dicionário do arquivo CSV</b>
			  <span style=\"text-align:justify\" > 
			  <p>Segue abaixo uma relação dos campos que compõem o arquivo CSV especificamente para a informação <span style=\"color:#0000CD\">$indnome</span> . As linhas marcadas em <span style=\"color:#FF0000\">vermelho</span> indicam os campos informativos e que não devem ser alterados em hipótese alguma. As demais linhas indicam os campos que podem ser alterados.</p> 
			  </span>
			  </td></tr>
		  <tr><td>ESTRUTURA DO ARQUIVO CSV</td></tr>
		  <tr><td>
		  <table cellspacing=\"0\" cellpadding=\"2\" border=\"0\" align=\"center\" width=\"100%\" style=\"color: rgb(51, 51, 51);\" >
			<tr bgcolor=#F5F5F5 ><td style=\"text-align:center;font-weight:bold;border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\" class=\"title\" >Campo</td>
			<td style=\"text-align:center;font-weight:bold;border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\" class=\"title\" >Conteúdo / Formato</td>
			<td style=\"text-align:center;font-weight:bold;border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\" class=\"title\" >Descrição</td></tr>";
	
	foreach($arrTabela as $tabela){
	
		$HTML .= "<tr><td align=center><span style=\"color:{$tabela['cor']}\">{$tabela['label']}</span></td>";
		
		//Verifica qual o tipo de exibição
		if(strstr($tabela['tabela'],";desc;")) {
			$HTML .= "<td align=center><span style=\"color:{$tabela['cor']}\">".str_replace(";desc;","",$tabela['tabela'])."</span></td>";
		} elseif(strstr($tabela['tabela'],";link;")) {
			$HTML .= "<td align=center><span style=\"color:{$tabela['cor']}\"><a style=\"color:#133368;font-weight:bold\" href=\"?modulo=principal/cargaporindicador&acao=A&arquivoCodigo=".str_replace(";link;","",$tabela['tabela'])."\" >CLIQUE AQUI</a> para obter os códigos.</span></td>";
		}
		else{
			$dados = $db->carregar($tabela['tabela']);
			
			$HTML .= "<td align=center><span style=\"color:{$tabela['cor']}\">
				  <table cellspacing=\"0\" cellpadding=\"2\" border=\"0\" align=\"center\" width=100% ><tr><td><b>Código</b></td><td><b>Descrição</b></td></tr>";
			$c = 0; 
			if($dados[0]) {
				foreach($dados as $d){
					($c % 2)? $cor = "#f9f9f9" : $cor = "#f5f5f5"; 
					$HTML .= "<tr onmouseout=\"this.bgColor='$cor';\" onmouseover=\"this.bgColor='#ffffcc';\"  bgcolor=\"$cor\" ><td>{$d['codigo']}</td><td>{$d['descricao']}</td></tr>";
					$c++;
				}
			}
			$HTML .= "</table>";
			$HTML .= "</span></td>";
		}
		$HTML .= "<td align=center ><span style=\"color:{$tabela['cor']}\">{$tabela['desc_tabela']}</span></td></tr>";
	}
	$HTML .= "</table>
		  </td></tr>
		  </table>";
	
	return $HTML;
}



/**
 * Função que cria o código contendo o modelo de carga (incluindo exemplos, tabelas de apoio, etc)
 * 
 * @author Alexandre Dourado
 * @return htmlcode (Tutorial)
 * @param array $dados (atualmente vazio)
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function verdicionariocarga($dados) {
	global $db;
	
	$sql = "SELECT indnome FROM painel.indicador WHERE indid = '{$_SESSION['indid']}'";
	$indnome = $db->pegaUm($sql); 

	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>
		  <link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
		  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";

	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	echo "<tr><td class='SubTituloCentro'>Agendamento de valores por meio Upload/Download de arquivos</td></tr>
		  <tr><td><b>1. Descrição</b>
		          <span style=\"text-align:justify\" >
		  		  <p>O Agendamento de valores através do Download/Upload foi criado para viabilizar um meio mais rápido de alteração de valores por meio de transferência de arquivos. Esta opção permite que você faça o download de um arquivo padronizado, onde as séries históricas das informações do Painel possam ser inseridas e atualizadas por meio do upload de arquivo.</p>
			      <p>A transferência de dados entre o usuário e o sistema Painel deve ser feita através de um arquivo em formato CSV (ver exemplo abaixo). O conteúdo dos arquivos deverá obedecer a estrutura definida para cada informação do Painel, ou seja, dependendo da regionalização, periodicidade e detalhamento, a estrutura dos dados deverá mudar. Em seguida, será possível realizar o processo de upload para que as informações contidas no arquivo, sejam devidamente atualizadas no Painel.</p>
			      <p style=\"color:#0000CD\" ><b>Dica:</b> Caso o seu órgão tenha uma equipe de suporte à Tecnologia da Informação - TI, entre em contato com eles para obter ajuda na geração do arquivo em formato CSV.</p>	
				  </span>
		  		  <b>2. Exemplo do arquivo CSV</b>
				  <p>Com objetivo de facilitar o entendimento da estrutura do arquivo CSV, exemplificamos abaixo uma estrutura simples do arquivo esperado pelo sistema <span style=\"color:#0000CD\" >Painel</span> para atualização da série histórica. Cada coluna do arquivo está separada por cores que representam as seguintes informações:</p>
		  	  </td>
		  </tr>";
	
	$_VALIDACAO = criarestruturavalidacao($_SESSION['indid']);
	if($_VALIDACAO) {
		foreach($_VALIDACAO as $key => $valid) {
			if($key==(count($_VALIDACAO)-1)) $sep="";
			else $sep=";";
			$csvlabel     .= "<font color='".$valid['cor']."'><b>".$valid['label']."</b></font>".$sep;
			$arrEx[] = array("cor" => $valid['cor'], "valor" => $valid['valor'], "sep" => $sep);
			$arrTabela[] = array("label" => $valid['label'],"tabela" => $valid['tabela'], "desc_tabela" => $valid['desc_tabela'], "cor" => $valid['cor']);
		}
	}
	$csvlabel .= "<br>";
	$i = 0;
	while( $i < 10){
		$i++;
		foreach ($arrEx as $arr){
			if($arr['valor'] != $_SESSION['indid'] && !is_string($arr['valor'])){
				$arr['valor'] = $arr['valor'] + rand(1,100);
			}
			//$csvlabel .= "<font color='".$arr['cor']."'><b>".$arr['valor']."</b></font>".$arr['sep'];
			$csvlabel .= "<font color='".$arr['cor']."'><b>x</b></font>".$arr['sep'];
		}
		$csvlabel .= "<br>";
	}
	
	echo "<tr><td>".$csvlabel."<br>
		          <b>Atenção:</b></br>
			      1) A primeira linha do arquivo deve conter o título dos campos.</br>
			      2) Todos os campos deverão ser separados por ponto e vírgula \";\".</br>
			      3) Os valores deverão ser escritos no padrão brasileiro (###.###.###,##).<br>
			      4) Para visualizar um exemplo do arquivo CSV especificamente para este indicador, <a href='?modulo=principal/cargaporindicador&acao=A&requisicao=escolherconfigcarga'>clique aqui</a>.
		      </td></tr>
		      <tr><td>
		      <b>3. Dicionário do arquivo CSV</b>
			  <span style=\"text-align:justify\" > 
			  <p>Segue abaixo uma relação dos campos que compõem o arquivo CSV especificamente para a informação <span style=\"color:#0000CD\">$indnome</span> . As linhas marcadas em <span style=\"color:#FF0000\">vermelho</span> indicam os campos informativos e que não devem ser alterados em hipótese alguma. As demais linhas indicam os campos que podem ser alterados.</p> 
			  </span>
			  </td></tr>
		  <tr><td class='SubTituloCentro'>ESTRUTURA DO ARQUIVO CSV</td></tr>
		  <tr><td>
		  <table cellspacing=\"0\" cellpadding=\"2\" border=\"0\" align=\"center\" width=\"100%\" class=\"listagem\" style=\"color: rgb(51, 51, 51);\" >
			<tr bgcolor=#F5F5F5 ><td style=\"text-align:center;font-weight:bold;border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\" class=\"title\" >Campo</td>
			<td style=\"text-align:center;font-weight:bold;border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\" class=\"title\" >Conteúdo / Formato</td>
			<td style=\"text-align:center;font-weight:bold;border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);\" class=\"title\" >Descrição</td></tr>";
	
	foreach($arrTabela as $tabela){
	
		echo "<tr><td align=center><span style=\"color:{$tabela['cor']}\">{$tabela['label']}</span></td>";
		
		//Verifica qual o tipo de exibição
		if(strstr($tabela['tabela'],";desc;")) {
			echo "<td align=center><span style=\"color:{$tabela['cor']}\">".str_replace(";desc;","",$tabela['tabela'])."</span></td>";
		} elseif(strstr($tabela['tabela'],";link;")) {
			echo "<td align=center><span style=\"color:{$tabela['cor']}\"><a style=\"color:#133368;font-weight:bold\" href=\"?modulo=principal/cargaporindicador&acao=A&arquivoCodigo=".str_replace(";link;","",$tabela['tabela'])."\" >CLIQUE AQUI</a> para obter os códigos.</span></td>";
		}
		else{
			$dados = $db->carregar($tabela['tabela']);
			
			echo "<td align=center><span style=\"color:{$tabela['cor']}\">
				  <table cellspacing=\"0\" cellpadding=\"2\" border=\"0\" align=\"center\" width=100% ><tr><td><b>Código</b></td><td><b>Descrição</b></td></tr>";
			$c = 0; 
			if($dados[0]) {
				foreach($dados as $d){
					($c % 2)? $cor = "#f9f9f9" : $cor = "#f5f5f5"; 
					echo "<tr onmouseout=\"this.bgColor='$cor';\" onmouseover=\"this.bgColor='#ffffcc';\"  bgcolor=\"$cor\" ><td>{$d['codigo']}</td><td>{$d['descricao']}</td></tr>";
					$c++;
				}
			}
			echo "</table>";
			echo"</span></td>";
		}
		echo "<td align=center ><span style=\"color:{$tabela['cor']}\">{$tabela['desc_tabela']}</span></td></tr>";
	}
	echo "</table>
		  </td></tr>
		  </table>";
	exit;
}

/**
 * Função que cria a estrutura de validação, informando os campos que devem ser validados e o formato como devem ser validados
 * 
 * @author Alexandre Dourado
 * @return array $_VALIDACAO
 * @param integer $indid
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function criarestruturavalidacao($indid) {
	global $db;
	//carregando informações do indicador
	$indicador = $db->pegaLinha("SELECT * FROM painel.indicador WHERE indid='".$indid."'");
	// se existir indicador, criar estrutura de validação
	if($indicador) {
		// a estrutura basica consiste em id do indicador, período referente e quantidade (onde esta pode ser inteiro ou valor)
		$_VALIDACAO = array(0=>array("campo"           => "indid",
								     "funcaovalidacao" => "validarindicadorid",
									 "tipo"            => "formatopadrao",
									 "label"		   => "id do indicador",
									 "tabela"		   => ";desc;$indid",
									 "desc_tabela"	   => "Utilizar o Indicador ao lado.",
									 "valor"		   => $indid,
									 "cor"             => "#B22222"),
							1=>array("campo"           => "dpeid",
								     "funcaovalidacao" => "validarperiodicidadeid",
									 "tipo"            => "formatopadrao",
									 "tabela"		   => "SELECT dpeid as codigo, dpedsc as descricao FROM painel.detalheperiodicidade dsh	INNER JOIN painel.indicador ind ON ind.perid = dsh.perid WHERE dsh.dpestatus = 'A' AND ind.indid = {$_SESSION['indid']} ORDER BY dpeanoref ASC, dpemesref ASC",									 "desc_tabela"	   => "Utilizar o identificador ao lado conforme a periodicidade em questão.",
									 "label"           => "id da periodicidade",
									 "valor"		   => $indicador['perid'],
									 "cor"       	   => "#436EEE"),
							2=>array("campo"           =>"acdqtde",
							         "funcaovalidacao" => "validaracdqtde",
								     "tipo"            => "formatovalor",
									 "tabela"		   => ";desc;999999",
									 "desc_tabela"	   => "Informar a quantidade de(a) {$indicador['indnome']}.",
									 "label"           => "quantidade",
									 "valor"		   => 1,
									 "cor"             => "#2E8B57",
									 "descricao"       => "Quantidade dever ser inteiro sem casas decimais"));
		// verifica a unidade de medição do indicador e aplicar as regras especificas para as condições
		switch($indicador['unmid']) {
			// se for porcentagem, apagar os detalhes da quantidade e inserir as informações de porcentagem (sendo gravada no acdqtde)
			case 1:
				unset($_VALIDACAO[2]);
				$_VALIDACAO[2] = array("campo"           =>"acdqtde",
								         "funcaovalidacao" => "validaracdqtde",
									     "tipo"            => "formatovalor",
										 "tabela"		   => ";desc;9999,99",
										 "desc_tabela"	   => "Informar a porcentagem de(a) {$indicador['indnome']}.",
										 "label"           => "porcentagem",
										 "valor"		   => 1.5,
										 "cor"             => "#2E8B57",
										 "descricao"       => "Valor dever ser númerico e as casas decimais devem ser separadas por ponto(.) Ex.: 1001.23");
				break;
			// se for inteiro, e a opção coletar valores monetarios for verdadeiro, inserir a coleta do valoe
			case 3:
				if($indicador['indqtdevalor']=="t") {
					$_VALIDACAO[] = array("campo"           => "acdvalor",
										  "funcaovalidacao" => "validaracdvalor",
										  "tabela"		    => ";desc;9.999,99",
										  "desc_tabela"	    => "Informar o valor de(a) {$indicador['indnome']}.",
										  "tipo"            => "formatovalor",
										  "label"           => "valor monetário",
										  "cor"       	    => "#8B7E66",
										  "valor"		    => 1050.55,
										  "descricao"       => "Valor dever ser númerico e as casas decimais devem ser separadas por ponto(.) Ex.: 1001.23");
				}
				break;
			case 5:
				unset($_VALIDACAO[2]);
				$_VALIDACAO[2] = array("campo"           =>"acdqtde",
								         "funcaovalidacao" => "validaracdvalor",
									     "tipo"            => "formatovalor",
										 "tabela"		   => ";desc;9.999,99",
										 "desc_tabela"	   => "Informar o valor de(a) {$indicador['indnome']}.",
										 "label"           => "valor monetário",
										 "valor"		   => 1050.55,
										 "cor"             => "#8B7E66",
										 "descricao"       => "Valor dever ser númerico e as casas decimais devem ser separadas por ponto(.) Ex.: 1001.23");
				break;
		
		}
							
		$sql = "SELECT * FROM painel.detalhetipoindicador WHERE indid='".$_SESSION['indid']."' ORDER BY tdiordem";
		$detalhetipoindicador = $db->carregar($sql);
		if($detalhetipoindicador[0]) {
			foreach($detalhetipoindicador as $tpi) {
				$_VALIDACAO[] = array("campo"           =>"tidid".$tpi['tdinumero'],
								      "funcaovalidacao" => "validartidid",
									  "tipo"            => "formatopadrao",
									  "tabela"		    => "SELECT tidid as codigo, tiddsc as descricao from painel.detalhetipodadosindicador where tdiid='".$tpi['tdiid']."'",
									  "desc_tabela"	    => "Utilizar o identificador ao lado conforme o detalhe em questão.",
									  "valor"		    => 1,
									  "label"           => "id detalhe <font size=1>(".$tpi['tdidsc'].")</font> ",
									  "cor"       	    => "#CD6839");
			
			}
		}
							
		
		switch($indicador['regid']) {
			case REGIONALIZACAO_MUN:
				$_VALIDACAO[] = array("campo"           => "acdmuncod",
									  "funcaovalidacao" => "validarmuncod",
									  "tipo"            => "formatopadrao",
									  "tabela"		    => ";link;territorios.municipio",
				 					  "desc_tabela"	    => "Utilizar o arquivo de municípios.",
									  "label"           => "código do munícipio",
				 					  "cor"       	    => "#CDAD00",
									  "valor"		    => 1,
								  	  "descricao"       => "Código do munícipio no IBGE",
									  "regionalizacao"  => $indicador['regid']);
				break;
			case REGIONALIZACAO_UF:
				$_VALIDACAO[] = array("campo"           => "acduf",
									  "funcaovalidacao" => "validaruf",
									  "tipo"            => "formatopadrao",
									  "label"           => "uf",
									  "tabela"		    => "select estuf as codigo,	estdescricao as descricao from territorios.estado order by estdescricao",
									  "desc_tabela"	    => "Utilizar o identificador ao lado conforme o estado em questão.",
									  "valor"		    => "DF",
				 					  "cor"       	    => "#2F4F4F",
								  	  "descricao"       => "Sigla do estado");
				break;
			case REGIONALIZACAO_REGIAO:
				$_VALIDACAO[] = array("campo"           => "acdregiao",
									  "funcaovalidacao" => "validarregiao",
									  "tipo"            => "formatopadrao",
									  "label"           => "Região",
									  "tabela"		    => "select regcod as codigo,regdescricao as descricao from territorios.regiao order by regdescricao",
									  "desc_tabela"	    => "Utilizar o identificador ao lado conforme o estado em questão.",
									  "valor"		    => 1,
				 					  "cor"       	    => "#2F4F4F",
								  	  "descricao"       => "Nome da Região");
				break;
			case REGIONALIZACAO_POLO:
				$_VALIDACAO[] = array("campo"           => "polid",
									  "funcaovalidacao" => "validarpolo",
									  "tipo"            => "formatopadrao",
									  "label"           => "código do polo",
									  "tabela"		    => ";link;painel.polo",
									  "desc_tabela"	    => "Utilizar o arquivo dos polos.",
									  "cor"       	    => "#D15FEE",
									  "valor"		    => 1,
								  	  "descricao"       => "Código do polo");
				break;
			case REGIONALIZACAO_ESCOLA:
				$_VALIDACAO[] = array("campo"           => "acdesciescod",
									  "funcaovalidacao" => "validarcodescola",
									  "tipo"            => "formatopadrao",
									  "label"           => "código INEP",
									  "tabela"		    => ";link;painel.escola",
									  "desc_tabela"	    => "Utilizar o arquivo de escolas.",
									  "cor"       	    => "#D15FEE",
									  "valor"		    => 1,
								  	  "descricao"       => "Código INEP");
				break;
			case REGIONALIZACAO_IES:
				$_VALIDACAO[] = array("campo"           => "acdesciescod",
									  "funcaovalidacao" => "validarcodies",
									  "tipo"            => "formatopadrao",
									  "label"           => "código IES",
									  "tabela"		    => ";link;painel.ies",
									  "desc_tabela"	    => "Utilizar o arquivo de código IES.",
									  "cor"       	    => "#000000",
									  "valor"       	    => 1,
								  	  "descricao"       => "Código IES");
				break;
			case REGIONALIZACAO_IESCPC:
				$_VALIDACAO[] = array("campo"           => "iecid",
									  "funcaovalidacao" => "validariecid",
									  "tipo"            => "formatopadrao",
									  "label"           => "código IES CPC",
									  "tabela"		    => ";link;painel.iescpc",
									  "desc_tabela"	    => "Utilizar o arquivo de código IES CPC.",
									  "cor"       	    => "#000000",
									  "valor"       	    => 1,
								  	  "descricao"       => "Código IES CPC");
				break;
			case REGIONALIZACAO_POSGRADUACAO:
				$_VALIDACAO[] = array("campo"           => "iepid",
									  "funcaovalidacao" => "validarcodiepg",
									  "tipo"            => "formatopadrao",
									  "label"           => "código IEPG",
									  "tabela"		    => ";link;painel.iepg",
									  "desc_tabela"	    => "Utilizar o arquivo de código IEPG.",
									  "cor"       	    => "#000000",
									  "valor"       	    => 1,
								  	  "descricao"       => "Código IEPG");
				break;
			case REGIONALIZACAO_INSTITUTO:
				$_VALIDACAO[] = array("campo"           => "unicod",
									  "funcaovalidacao" => "validarcodunidade",
									  "tipo"            => "formatopadrao",
									  "label"           => "código Instituto",
									  "tabela"		    => ";link;entidade.instituto",
									  "desc_tabela"	    => "Utilizar o arquivo de código Instituto.",
									  "cor"       	    => "#000000",
									  "valor"       	    => 1,
								  	  "descricao"       => "Código Instituto");
				break;	
			case REGIONALIZACAO_UNIVERSIDADE:
				$_VALIDACAO[] = array("campo"           => "unicod",
									  "funcaovalidacao" => "validarcodunidade",
									  "tipo"            => "formatopadrao",
									  "label"           => "código Universidade",
									  "tabela"		    => ";link;entidade.universidade",
									  "desc_tabela"	    => "Utilizar o arquivo de código Universidade.",
									  "cor"       	    => "#000000",
									  "valor"       	    => 1,
								  	  "descricao"       => "Código Universidade");
				break;	
			case REGIONALIZACAO_CAMPUS_PROFISSIONAL:
				$_VALIDACAO[] = array("campo"           => "entid",
									  "funcaovalidacao" => "validarcodentidade",
									  "tipo"            => "formatopadrao",
									  "label"           => "código Campus(Educação Profissional)",
									  "tabela"		    => ";link;entidade.campusprofissional",
									  "desc_tabela"	    => "Utilizar o arquivo de código Campus(Educação Profissional).",
									  "cor"       	    => "#000000",
									  "valor"       	    => 1,
								  	  "descricao"       => "Código Campus(Educação Profissional)");
				break;	
			case REGIONALIZACAO_CAMPUS_SUPERIOR:
				$_VALIDACAO[] = array("campo"           => "entid",
									  "funcaovalidacao" => "validarcodentidade",
									  "tipo"            => "formatopadrao",
									  "label"           => "código Campus(Educação Superior)",
									  "tabela"		    => ";link;entidade.campussuperior",
									  "desc_tabela"	    => "Utilizar o arquivo de código Campus(Educação Superior).",
									  "cor"       	    => "#000000",
									  "valor"       	    => 1,
								  	  "descricao"       => "Código Campus(Educação Superior)");
				break;	
			case REGIONALIZACAO_HOSPITAL:
				$_VALIDACAO[] = array("campo"           => "entid",
									  "funcaovalidacao" => "validarcodentidade",
									  "tipo"            => "formatopadrao",
									  "label"           => "código hospital",
									  "tabela"		    => ";link;entidade.hospitais",
									  "desc_tabela"	    => "Utilizar o arquivo de código dos Hospitais Universitários.",
									  "cor"       	    => "#000000",
									  "valor"       	    => 1,
								  	  "descricao"       => "Código Hospitais Universitários");
				break;	
				

		}
		return $_VALIDACAO;
	}
}
/**
 * Função que cria e faz o download da lista de escolas cadastradas, informando o codigo e a descrição
 * 
 * @author Alexandre Dourado
 * @return array $_VALIDACAO
 * @param integer $indid
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function arquivoCodigo($tabela) {
	global $db;
	ini_set('memory_limit','1024M');
	switch($tabela) {
		case 'painel.polo':
			$sql = "select 
						pol.polid as codigo, 
						pol.poldsc as descricao,
						mun.estuf as uf
					from painel.polo pol 
					inner join territorios.municipio mun on mun.muncod = pol.muncod 
					order by 
						mun.estuf, descricao";
			break;
		case 'entidade.campusprofissional':
			$sql = "select 
						ent.entid as codigo, 
						ent.entnome as descricao,
						ent2.entsig as uf
					from entidade.entidade ent 
					inner join entidade.funcaoentidade fen on fen.entid = ent.entid 
					inner join entidade.funentassoc fea on fea.fueid = fen.fueid 
					inner join entidade.entidade ent2 on ent2.entid = fea.entid 
					inner join entidade.funcaoentidade fen2 on fen2.entid = ent2.entid   
					inner join entidade.endereco ende on ende.entid = ent.entid 
					where fen.funid=17 and fen2.funid=11
					order by 
						ent2.entsig, descricao";
			break;
		case 'entidade.campussuperior':
			$sql = "select 
						ent.entid as codigo, 
						ent.entnome as descricao,
						ent2.entsig as uf
					from entidade.entidade ent 
					inner join entidade.funcaoentidade fen on fen.entid = ent.entid 
					inner join entidade.funentassoc fea on fea.fueid = fen.fueid 
					inner join entidade.entidade ent2 on ent2.entid = fea.entid 
					inner join entidade.funcaoentidade fen2 on fen2.entid = ent2.entid   
					inner join entidade.endereco ende on ende.entid = ent.entid 
					where fen.funid=18 and fen2.funid=12
					order by 
						ent2.entsig, descricao";
			break;
		case 'entidade.hospitais':
			$sql = "select 
						ent.entid as codigo, 
						ent.entnome||' ('||ent2.entsig||')' as descricao,
						ende.estuf as uf
					from entidade.entidade ent 
					inner join entidade.funcaoentidade fen on fen.entid = ent.entid 
					inner join entidade.endereco ende on ende.entid = ent.entid 
					INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
					INNER JOIN entidade.entidade ent2 ON ent2.entid = fea.entid 
					INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = ent2.entid 
					where fen.funid='16' AND (fen2.funid='12' OR fen2.funid='16')
					order by descricao";
			break;
		case 'entidade.universidade':
			$sql = "select distinct unicod as codigo, unidsc as descricao, ende.estuf as uf from public.unidade uni
					inner join entidade.entidade ent on ent.entunicod=uni.unicod 
					inner join entidade.endereco ende on ende.entid=ent.entid   
					where orgcod='26000' and gunid=3 and unistatus='A'";
			break;
		case 'entidade.instituto':
			$sql = "select distinct unicod as codigo, unidsc as descricao, ende.estuf as uf from public.unidade uni
					inner join entidade.entidade ent on ent.entunicod=uni.unicod 
					inner join entidade.endereco ende on ende.entid=ent.entid   
					where orgcod='26000' and gunid=2 and unistatus='A'";
			break;
		case 'painel.ies':
			$sql = "select 
						iesid as codigo, 
						iesdsc as descricao,
						mun.estuf as uf
					from 
						painel.ies as ies
					inner join
						territorios.municipio mun on ies.iesmuncod = mun.muncod 
					order by 
						estuf, descricao";
			break;
		case 'painel.iescpc':
			$sql = "select 
						iecid as codigo, 
						iecdsc as descricao,
						mun.estuf as uf
					from 
						painel.iescpc as iescpc
					inner join
						territorios.municipio mun on iescpc.muncod = mun.muncod 
					order by 
						iescpc.estuf, descricao";
			break;
			
		case 'painel.iepg':
			$sql = "select 
						iepg.iepid as codigo, 
						iepg.iepdsc as descricao,
						mun.estuf as uf
					from 
						painel.iepg as iepg
					inner join
						territorios.municipio mun on iepg.muncod = mun.muncod 
					order by 
						iepg.estuf, descricao";
			break;
		case 'painel.escola':
			$sql = "select 
						esc.esccodinep as codigo, 
						esc.escdsc as descricao,
						mun.estuf as uf
					from 
						painel.escola as esc
					inner join
						territorios.municipio mun on esc.escmuncod = mun.muncod 
					order by 
						estuf, descricao";
			break;
		case 'territorios.municipio':
			$sql = "select muncod as codigo, mundescricao as descricao , estuf as uf from territorios.municipio order by estuf, mundescricao";
			break;
	}
	$dados = $db->carregar($sql);
		
	
	$txt = "Código - Descrição - UF\n";
	foreach($dados as $d){
		$txt .= "{$d['codigo']} - {$d['descricao']} - {$d['uf']} \n";		
	}
	
	$nomeArq = explode(".",$tabela);
	header("Content-Type: text/html; charset=ISO-8859-1");
	header("Content-type: application/octet-stream");
	header("Content-Disposition: attachment; filename=\"codigo_".$nomeArq[1].".txt\"");
	echo $txt;
	exit;	
}



function acompanharArquivoAgendamento($wbsid) {
	global $db;
	
	$sql = "SELECT wbslog FROM painel.webservicefiles WHERE wbsid='".$wbsid."' AND usucpf='".$_SESSION['usucpf']."'";
	return $db->pegaUm($sql);
	
}

/**
 * Função principal que executa o processamento de enviar arquivo para a lista de agendamentos
 * 
 * @author Alexandre Dourado
 * @return htmlcode (código sera impresso no iframe)
 * @param date $dados[agddataprocessamento] (data que sera processado o agendamento)
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function enviarAgendamentoWebService($dados) {
	global $db, $_OTIMIZAR_ACOES_DPEID, $_OTIMIZAR_ACOES_TERRITORIO;

	$_VALIDACAO = criarestruturavalidacao($_SESSION['indid']);
    $csvarray = $dados['csvarray'];
    
    /*
     * Trecho de otimização que cacheia todo o conteúdo para que não seja acessado novamente
     * Regionalizadores implementados o esquema de cache
     *  - Escola
     */
    
    $sql = "SELECT tidid FROM painel.detalhetipoindicador dti 
	 		LEFT JOIN painel.detalhetipodadosindicador tid ON tid.tdiid = dti.tdiid 
	 		WHERE tdinumero = '1' AND indid = '".$_SESSION['indid']."'";
    
	$detalhes1 = $db->carregar($sql);
	if($detalhes1[0]) {
		foreach($detalhes1 as $dt1) {
			$_OTIMIZAR_ACOES_TID1[$_SESSION['indid']][$dt1['tidid']] = true;
		}
	}
	
    $sql = "SELECT tidid FROM painel.detalhetipoindicador dti 
	 		LEFT JOIN painel.detalhetipodadosindicador tid ON tid.tdiid = dti.tdiid 
	 		WHERE tdinumero = '2' AND indid = '".$_SESSION['indid']."'";
    
	$detalhes2 = $db->carregar($sql);
	if($detalhes2[0]) {
		foreach($detalhes2 as $dt2) {
			$_OTIMIZAR_ACOES_TID2[$_SESSION['indid']][$dt2['tidid']] = true;
		}
	}

	
	unset($detalhes1,$detalhes1);
	
	$sql = "SELECT dpeid FROM painel.detalheperiodicidade d 
			INNER JOIN painel.indicador i ON d.perid = i.perid 
			WHERE indid = '".$_SESSION['indid']."'";
	
	$periodicidades = $db->carregar($sql);
	
	if($periodicidades[0]) {
		foreach($periodicidades as $peri) {
			$_OTIMIZAR_ACOES_DPEID[$_SESSION['indid']][$peri['dpeid']] = true;
		}
	}
	
	unset($periodicidades);
    
   	$regidc = $db->pegaUm("SELECT regid FROM painel.indicador WHERE indid='".$_SESSION['indid']."'");
   	switch($regidc) {
   		case REGIONALIZACAO_ESCOLA:
   			$dadosotim = $db->carregar("SELECT escuf, escmuncod, esccodinep FROM painel.escola");
   			if($dadosotim[0]) {
   				foreach($dadosotim as $dotim) {
   					$_OTIMIZAR_ACOES_TERRITORIO[$dotim['esccodinep']] = array("estuf" => $dotim['escuf'], "muncod" => $dotim['escmuncod']);
   				}
   			}
   			break;
   	}
   	unset($dadosotim, $regidc);
   	
   	/*
   	 * Fim do trecho de cache
   	 */
    
    
    $sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid)
    		VALUES (NOW(), '".formata_data_sql($dados['agddataprocessamento'])."', '".$_SESSION['usucpf']."','".$_SESSION['indid']."') RETURNING agdid;";
    $agdid = $db->pegaUm($sql);
   	$handle = fopen(APPRAIZ . "arquivos/painel/logs/".date("Y-m-d").".log", "a+");
    fwrite($handle, "----------------- IMPORTANDO AGENDAMENTO nº".$agdid." ---------------  (CPF_".$_SESSION['usucpf'].", ".date("d/m/Y h:i:s").")\n");
    
	$validacaogeral = true;
	// verifica se o arquivo começa com o cabeçalho, ou ja com as informações para fazer a carga (id do indicador)
    for($i=((is_numeric(substr($csvarray[0],0,1)))?0:1);$i<count($csvarray);$i++) {
    	unset($insertinto,$insertvalue,$bloqueado);
    	$insertinto[]  = "agdid";
    	$insertvalue[] = "'".$agdid."'";
    	$dadoscsv = explode(";",$csvarray[$i]);
    	foreach($_VALIDACAO as $key => $value) {
    		
    		unset($err);
    		
    		if(!$_OTIMIZAR_ACOES[$value['campo']][trim($dadoscsv[$key])])
	    		$err = $value['funcaovalidacao'](trim($dadoscsv[$key]));
	    		
    		if(!$err) {
    			
    			$_OTIMIZAR_ACOES[$value['campo']][trim($dadoscsv[$key])] = true;
    			
    			switch($value['funcaovalidacao']) {
    				case 'validarpolo':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));

    					$polo = $db->pegaLinha("SELECT mun.muncod, mun.estuf FROM painel.polo pol 
    										    LEFT JOIN territorios.municipio mun ON mun.muncod=pol.muncod
    										    WHERE polid='".trim($dadoscsv[$key])."'");
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$polo['estuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$polo['muncod']."'";
    					
    					break;
    				
    				case 'validarcodunidade':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));

    					$uni = $db->pegaLinha("SELECT muncod, estuf FROM entidade.entidade ent 
    										   LEFT JOIN entidade.endereco ende ON ende.entid=ent.entid
    										   WHERE entunicod='".trim($dadoscsv[$key])."'");
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$uni['estuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$uni['muncod']."'";
    					
    					break;
    				case 'validarcodentidade':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					
    					$ent = $db->pegaLinha("SELECT muncod, estuf FROM entidade.endereco WHERE entid='".trim($dadoscsv[$key])."'");
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$ent['estuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$ent['muncod']."'";
    					break;
    				case 'validariescpc':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					
    					$iescpc = $db->pegaLinha("SELECT muncod, estuf FROM painel.iescpc WHERE iecid='".trim($dadoscsv[$key])."'");
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$iescpc['estuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$iescpc['muncod']."'";
    					break;
    				case 'validarcodies':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					
    					$ies = $db->pegaLinha("SELECT iesmuncod, iesestuf FROM painel.ies WHERE iesid='".trim($dadoscsv[$key])."'");
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$ies['iesestuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$ies['iesmuncod']."'";
    					break;
    				case 'validarcodescola':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					
    					if($_OTIMIZAR_ACOES_TERRITORIO[trim($dadoscsv[$key])]) {
	    					$esc['escuf']  = $_OTIMIZAR_ACOES_TERRITORIO[trim($dadoscsv[$key])]['estuf'];
	    					$esc['escmuncod'] = $_OTIMIZAR_ACOES_TERRITORIO[trim($dadoscsv[$key])]['muncod'];
    					}
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$esc['escuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$esc['escmuncod']."'";
    					break;
    				case 'validarcodiepg':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					
    					$iepg = $db->pegaLinha("SELECT muncod, estuf FROM painel.iepg WHERE iepid='".trim($dadoscsv[$key])."'");
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$iepg['estuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$iepg['muncod']."'";
    					break;
    				case 'validarmuncod':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".trim($dadoscsv[$key])."'")."'";
    					break;
    				default:
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					
    			    	switch($value['campo']) {
    						case 'tidid2':
    							$tidid2val = false;
    							
    							if($_OTIMIZAR_ACOES_TID2[$_SESSION['indid']][trim($dadoscsv[$key])]) {
									$tidid2val = true;
    							}
    							
    							if(!$tidid2val) {
    								$erros[$i][$key] = array("Detalhe informado não condiz com a estrutura do indicador, releia o manual de carga e faça os ajustes na configuração do csv");
    							}
    							
    							break;
    						
    						case 'tidid1':
    							$tidid1val = false;
    							
    							if($_OTIMIZAR_ACOES_TID1[$_SESSION['indid']][trim($dadoscsv[$key])]) {
 									$tidid1val = true;
    							}
    							
    							if(!$tidid1val) {
    								$erros[$i][$key] = array("Detalhe informado não condiz com a estrutura do indicador, releia o manual de carga e faça os ajustes na configuração do csv");
    							}
    							
    							break;
    						case 'dpeid':
    							if(!$_OTIMIZAR_ACOES_SEHBLOQ[$_SESSION['indid'][$value['tipo'](trim($dadoscsv[$key]))]]) {
	    							$sehbloqueado = $db->pegaUm("SELECT sehbloqueado FROM painel.seriehistorica WHERE indid='".$_SESSION['indid']."' AND dpeid=".$value['tipo'](trim($dadoscsv[$key]))." AND (sehstatus='A' OR sehstatus='H')");
	    							$_OTIMIZAR_ACOES_SEHBLOQ[$_SESSION['indid'][$value['tipo'](trim($dadoscsv[$key]))]] = $sehbloqueado;
    							} else {
    								$sehbloqueado = $_OTIMIZAR_ACOES_SEHBLOQ[$_SESSION['indid'][$value['tipo'](trim($dadoscsv[$key]))]];
    							}
						    	if($sehbloqueado == "t") {
						    		$bloqueado = true;
						    	}
    							break;
    					}  					
    			}
 
    		} else {
    			if(!$erros[$i][$key]) $erros[$i][$key] = array();
    			$erros[$i][$key] = $err;
    		}
    	}
    	if(!$bloqueado) $sqls[] = "INSERT INTO painel.agendamentocargadados(".implode(",",$insertinto).")
			      					VALUES (".implode(",",$insertvalue).")";
    	
   }
   if($sqls) $db->executar("UPDATE painel.agendamentocarga SET agdlog='".base64_encode(implode(";",$sqls))."' WHERE agdid='".$agdid."'", false);
   
   // verificando se existe erros, caso sim não efetuar a carga e exibir os erros
   if(count($erros) > 0) {
    	$HTML .= "<span style='font-family:Arial,verdana;font-size:8pt;'>Foram encontrados erros em <b>".count($erros)."</b> linhas</span><br>";
    	foreach($erros as $linnum => $erroLin) {
    		foreach($erroLin as $colnum => $erroCol) {
    			foreach($erroCol as $erroo) {
    				$HTML .= "<span style='font-family:Arial,verdana;font-size:8pt;color:red;'>Linha #".$linnum." Coluna #".$colnum." : ".$erroo."</span><br>";
    				fwrite($handle, "-- ERRO LINHA ".$linnum." COLUNA ".$colnum." : ".strtoupper($erroo)."\n");
    			}
    		}
    	}
    	$db->executar("UPDATE painel.agendamentocarga SET agdstatus='I' WHERE agdid='".$agdid."'", false);
    	
    } else {
    	// executando todos os selects de carga
    	if(count($sqls)>0) $db->executar(implode(";",$sqls), false);
		fwrite($handle, "-- SUCESSO ".count($sqls)." LINHAS PROCESSADAS\n");
		$HTML .= "Foram processadas <b>".count($sqls)."</b> linhas com sucesso";
    }
    
    $db->commit();
    

    fwrite($handle, "----------------- FIM IMPORTANDO AGENDAMENTO nº".$agdid." --------------- (CPF_".$_SESSION['usucpf'].", ".date("d/m/Y h:i:s").")\n");
    fclose($handle);
    return $HTML;
}


function enviarArquivoAgendamento($dados) {
	global $db;
	
	if(!$dados['agddataprocessamento']) {
		return "Data de processamento não enviado";
	}
	if(!$dados['indid']) {
		return "ID do indicador não enviado";
	}
	if(!$dados['csvarray']) {
		return "Dados não enviados";
	}
	
	if(!is_dir( APPRAIZ .'/arquivos/painel/webservice_files')) {
		mkdir( APPRAIZ . '/arquivos/painel/webservice_files', 0777);
	}
	

	$file = formata_data_sql($dados['agddataprocessamento']).'_'.$dados['indid'].'_'.date("YmdHis");
	
	// Append the contents of $person to the file named by $file.
	// Let's make sure the file exists and is writable first.
	
    // In our example we're opening $filename in append mode.
    // The file pointer is at the bottom of the file hence
    // that's where $somecontent will go when we fwrite() it.
    if (!$handle = fopen(APPRAIZ . 'arquivos/painel/webservice_files/'.$file.'.csv', 'w')) {
         echo "Cannot open file ($filename)";
         exit;
    }
	
    foreach($dados['csvarray'] as $arr) {
	    // Write $somecontent to our opened file.
	    if (fwrite($handle, $arr."\n") === FALSE) {
	        echo "Cannot write to file ($filename)";
	        exit;
	    }
    }
	
	$wbslog = date("d/m/Y H:i:s")." :: Registros enviados com sucesso. Aguardando processamento.<br>";
	
	$sql = "INSERT INTO painel.webservicefiles(
            wbsdsc, wbslog, usucpf, wbsstatus)
    		VALUES ('".$file."', '".$wbslog."', '".$_SESSION['usucpf']."', 'A') RETURNING wbsid;";
	$wbsid = $db->pegaUm($sql);
	$db->commit();
	return $wbsid; 
	
}


/**
 * Função principal que executa o processamento de enviar arquivo para a lista de agendamentos
 * 
 * @author Alexandre Dourado
 * @return htmlcode (código sera impresso no iframe)
 * @param date $dados[agddataprocessamento] (data que sera processado o agendamento)
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function enviarAgendamento($dados) {
	global $db, $_OTIMIZAR_ACOES_DPEID, $_OTIMIZAR_ACOES_TERRITORIO;
	
	$TInicio = getmicrotime();
	$_VALIDACAO = criarestruturavalidacao($_SESSION['indid']);
	
	if($_FILES["arquivo"]["tmp_name"]) {
	    $csvarray = file($_FILES["arquivo"]["tmp_name"]);
	} else {
		$erros[0][0] = "Arquivo não foi enviado corretamente";
	}
    
    /*
     * Trecho de otimização que cacheia todo o conteúdo para que não seja acessado novamente
     * Regionalizadores implementados o esquema de cache
     *  - Escola
     */
    
    $sql = "SELECT tidid FROM painel.detalhetipoindicador dti 
	 		LEFT JOIN painel.detalhetipodadosindicador tid ON tid.tdiid = dti.tdiid 
	 		WHERE tdinumero = '1' AND indid = '".$_SESSION['indid']."'";
    
	$detalhes1 = $db->carregar($sql);
	if($detalhes1[0]) {
		foreach($detalhes1 as $dt1) {
			$_OTIMIZAR_ACOES_TID1[$_SESSION['indid']][$dt1['tidid']] = true;
		}
	}
	
    $sql = "SELECT tidid FROM painel.detalhetipoindicador dti 
	 		LEFT JOIN painel.detalhetipodadosindicador tid ON tid.tdiid = dti.tdiid 
	 		WHERE tdinumero = '2' AND indid = '".$_SESSION['indid']."'";
    
	$detalhes2 = $db->carregar($sql);
	if($detalhes2[0]) {
		foreach($detalhes2 as $dt2) {
			$_OTIMIZAR_ACOES_TID2[$_SESSION['indid']][$dt2['tidid']] = true;
		}
	}

	
	unset($detalhes1,$detalhes1);
	
	$sql = "SELECT dpeid FROM painel.detalheperiodicidade d 
			INNER JOIN painel.indicador i ON d.perid = i.perid 
			WHERE indid = '".$_SESSION['indid']."'";
	
	$periodicidades = $db->carregar($sql);
	
	if($periodicidades[0]) {
		foreach($periodicidades as $peri) {
			$_OTIMIZAR_ACOES_DPEID[$_SESSION['indid']][$peri['dpeid']] = true;
		}
	}
	
	unset($periodicidades);
    
   	$regidc = $db->pegaUm("SELECT regid FROM painel.indicador WHERE indid='".$_SESSION['indid']."'");
   	switch($regidc) {
   		case REGIONALIZACAO_ESCOLA:
   			$dadosotim = $db->carregar("SELECT escuf, escmuncod, esccodinep FROM painel.escola");
   			if($dadosotim[0]) {
   				foreach($dadosotim as $dotim) {
   					$_OTIMIZAR_ACOES_TERRITORIO[$dotim['esccodinep']] = array("estuf" => $dotim['escuf'], "muncod" => $dotim['escmuncod']);
   				}
   			}
   			break;
   	}
   	unset($dadosotim, $regidc);
   	
   	/*
   	 * Fim do trecho de cache
   	 */
    
    $sql = "INSERT INTO painel.agendamentocarga(agddata, agddataprocessamento, usucpf, indid)
    		VALUES (NOW(), '".formata_data_sql($dados['agddataprocessamento'])."', '".$_SESSION['usucpf']."','".$_SESSION['indid']."') RETURNING agdid;";
    $agdid = $db->pegaUm($sql);
   	$handle = fopen(APPRAIZ . "arquivos/painel/logs/".date("Y-m-d").".log", "a+");
    fwrite($handle, "----------------- IMPORTANDO AGENDAMENTO nº".$agdid." ---------------  (CPF_".$_SESSION['usucpf'].", ".date("d/m/Y h:i:s").")\n");
    
	$validacaogeral = true;
	// verifica se o arquivo começa com o cabeçalho, ou ja com as informações para fazer a carga (id do indicador)
    for($i=((is_numeric(substr($csvarray[0],0,1)))?0:1);$i<count($csvarray);$i++) {
    	
    	unset($insertinto,$insertvalue, $bloqueado);
    	$insertinto[]  = "agdid";
    	$insertvalue[] = "'".$agdid."'";
    	$dadoscsv = explode(";",$csvarray[$i]);
    	
    	foreach($_VALIDACAO as $key => $value) {
    		
    		unset($err);
    		
    		if(!$_OTIMIZAR_ACOES[$value['campo']][trim($dadoscsv[$key])])
    			$err = $value['funcaovalidacao'](trim($dadoscsv[$key]));

    		if(!$err) {
    			
    			$_OTIMIZAR_ACOES[$value['campo']][trim($dadoscsv[$key])] = true;
    			
    			switch($value['funcaovalidacao']) {
    				case 'validarpolo':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));

    					$polo = $db->pegaLinha("SELECT mun.muncod, mun.estuf FROM painel.polo pol 
    										    LEFT JOIN territorios.municipio mun ON mun.muncod=pol.muncod
    										    WHERE polid='".trim($dadoscsv[$key])."'");
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$polo['estuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$polo['muncod']."'";
    					
    					break;
    				
    				case 'validarcodunidade':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));

    					$uni = $db->pegaLinha("SELECT muncod, estuf FROM entidade.entidade ent 
    										   LEFT JOIN entidade.endereco ende ON ende.entid=ent.entid
    										   WHERE entunicod='".trim($dadoscsv[$key])."'");
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$uni['estuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$uni['muncod']."'";
    					
    					break;
    				case 'validarcodentidade':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					
    					$ent = $db->pegaLinha("SELECT ende.muncod, ende.estuf, ent.entunicod FROM entidade.endereco ende
												LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ende.entid 
												LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
												LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid 
    										   WHERE ende.entid='".trim($dadoscsv[$key])."' AND fen.funid IN(17,18)");
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$ent['estuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$ent['muncod']."'";
    					
    					$insertinto[]  = 'unicod';
    					$insertvalue[] = "'".$ent['entunicod']."'";
    					
    					break;
    				case 'validariecid':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					
    					$iescpc = $db->pegaLinha("SELECT muncod, estuf FROM painel.iescpc WHERE iecid='".trim($dadoscsv[$key])."'");
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$iescpc['estuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$iescpc['muncod']."'";
    					break;
    				case 'validarcodies':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					
    					if($_OTIMIZAR_ACOES_TERRITORIO[trim($dadoscsv[$key])]) {
	    					$ies['iesestuf']  = $_OTIMIZAR_ACOES_TERRITORIO[trim($dadoscsv[$key])]['estuf'];
	    					$ies['iesmuncod'] = $_OTIMIZAR_ACOES_TERRITORIO[trim($dadoscsv[$key])]['muncod'];
    					} else {
	    					$ies = $db->pegaLinha("SELECT iesmuncod, iesestuf FROM painel.ies WHERE iesid='".trim($dadoscsv[$key])."'");
	    					$_OTIMIZAR_ACOES_TERRITORIO[trim($dadoscsv[$key])] = array("estuf" => $ies['iesestuf'], "muncod" => $ies['iesmuncod']);
    					}
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$ies['iesestuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$ies['iesmuncod']."'";
    					break;
    				case 'validarcodescola':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					
    					if($_OTIMIZAR_ACOES_TERRITORIO[trim($dadoscsv[$key])]) {
	    					$esc['escuf']  = $_OTIMIZAR_ACOES_TERRITORIO[trim($dadoscsv[$key])]['estuf'];
	    					$esc['escmuncod'] = $_OTIMIZAR_ACOES_TERRITORIO[trim($dadoscsv[$key])]['muncod'];
    					}
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$esc['escuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$esc['escmuncod']."'";
    					break;
    				case 'validarcodiepg':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					
    					$iepg = $db->pegaLinha("SELECT muncod, estuf FROM painel.iepg WHERE iepid='".trim($dadoscsv[$key])."'");
    					
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$iepg['estuf']."'";
    					
    					$insertinto[]  = 'acdmuncod';
    					$insertvalue[] = "'".$iepg['muncod']."'";
    					break;
    				case 'validarmuncod':
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					$insertinto[]  = 'acduf';
    					$insertvalue[] = "'".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".trim($dadoscsv[$key])."'")."'";
    					break;
    				default:
    					$insertinto[]  = $value['campo'];
    					$insertvalue[] = $value['tipo'](trim($dadoscsv[$key]));
    					
    					switch($value['campo']) {
    						case 'tidid2':
    							$tidid2val = false;
    							
    							if($_OTIMIZAR_ACOES_TID2[$_SESSION['indid']][trim($dadoscsv[$key])]) {
									$tidid2val = true;
    							}
    							
    							if(!$tidid2val) {
    								$erros[$i][$key] = array("Detalhe informado não condiz com a estrutura do indicador, releia o manual de carga e faça os ajustes na configuração do csv");
    							}
    							
    							break;
    						
    						case 'tidid1':
    							$tidid1val = false;
    							
    							if($_OTIMIZAR_ACOES_TID1[$_SESSION['indid']][trim($dadoscsv[$key])]) {
 									$tidid1val = true;
    							}
    							
    							if(!$tidid1val) {
    								$erros[$i][$key] = array("Detalhe informado não condiz com a estrutura do indicador, releia o manual de carga e faça os ajustes na configuração do csv");
    							}
    							
    							break;
    						case 'dpeid':
    							if(!$_OTIMIZAR_ACOES_SEHBLOQ[$_SESSION['indid'][$value['tipo'](trim($dadoscsv[$key]))]]) {
	    							$sehbloqueado = $db->pegaUm("SELECT sehbloqueado FROM painel.seriehistorica WHERE indid='".$_SESSION['indid']."' AND dpeid=".$value['tipo'](trim($dadoscsv[$key]))." AND (sehstatus='A' OR sehstatus='H')");
	    							$_OTIMIZAR_ACOES_SEHBLOQ[$_SESSION['indid'][$value['tipo'](trim($dadoscsv[$key]))]] = $sehbloqueado;
    							} else {
    								$sehbloqueado = $_OTIMIZAR_ACOES_SEHBLOQ[$_SESSION['indid'][$value['tipo'](trim($dadoscsv[$key]))]];
    							}
						    	if($sehbloqueado == "t") {
						    		$bloqueado = true;
						    	}
    							break;
    					}
    			}
 
    		} else {
    			if(!$erros[$i][$key]) $erros[$i][$key] = array();
    			$erros[$i][$key] = $err;
    		}
    	}
		if(!$bloqueado) $sqls[] = "INSERT INTO painel.agendamentocargadados(".implode(",",$insertinto).")
				      			   VALUES (".implode(",",$insertvalue).")";

   }
   
   // verificando se existe erros, caso sim não efetuar a carga e exibir os erros
   if(count($erros) > 0) {
   		unset($csvarray,$_OTIMIZAR_ACOES_DPEID,$_OTIMIZAR_ACOES_SEHBLOQ,$_OTIMIZAR_ACOES_TID1,$_OTIMIZAR_ACOES_TID2,$_OTIMIZAR_ACOES_TERRITORIO,$sqls);
    	echo "<span style='font-family:Arial,verdana;font-size:8pt;'><img src='../imagens/seta_filho.gif' align='absmiddle'> Foram encontrados erros em <b>".count($erros)."</b> linhas (no detalhamento serão exibidos 1000 erros)</span><br>";
    	$errnum=0;
    	foreach($erros as $linnum => $erroLin) {
    		foreach($erroLin as $colnum => $erroCol) {
    			foreach($erroCol as $erroo) {
    				$erroHTML[] = "<span style='font-family:Arial,verdana;font-size:8pt;color:red;'><img src='../imagens/seta_filho.gif' align='absmiddle'> Linha #".$linnum." Coluna #".$colnum." : ".$erroo."</span>";
    			}
    			if($errnum ==1000) break;
    			else $errnum++;
    			
    		}
    	}
    	
    	echo implode("<br>",$erroHTML)."<br>";
    	echo "<span style='font-family:Arial,verdana;font-size:8pt;color:red;'><img src='../imagens/seta_filho.gif' align='absmiddle'> Tempo de processamento: ".number_format( ( getmicrotime() - $TInicio ), 2, ',', '.' )." segundos</span><br/>";
    	echo "<span style='font-family:Arial,verdana;font-size:8pt;color:red;'><img src='../imagens/seta_filho.gif' align='absmiddle'> Carga não efetuada</span>";
    	$db->executar("UPDATE painel.agendamentocarga SET agdstatus='I' WHERE agdid='".$agdid."'", false);
    	
    } else {
    	// executando todos os selects de carga
    	if(count($sqls)>0) $db->executar(implode(";",$sqls), false);
		fwrite($handle, "-- SUCESSO ".count($sqls)." LINHAS PROCESSADAS\n");
    	echo "<span style='font-family:Arial,verdana;font-size:8pt;'><img src='../imagens/seta_filho.gif' align='absmiddle'> Foram processadas <b>".count($sqls)."</b> linhas em ".(number_format( ( getmicrotime() - $TInicio ), 4, ',', '.' ))." segundos</span><br>";
    	echo "<span style='font-family:Arial,verdana;font-size:8pt;'><img src='../imagens/seta_filho.gif' align='absmiddle'> Carga efetuada com sucesso!</span>";
    }
    
    // limpando o formulario pai e atualizando a lista de agendamento do indicador
    echo "<script>
    		parent.document.getElementById('1click').innerHTML='';
    		parent.document.getElementById('arquivocsv').value='';
    		parent.document.getElementById('agddataprocessamento').value='';
    		parent.ajaxatualizar('requisicao=carregaragendamento&indid=".$_SESSION['indid']."','listaagendamento');
    	  </script>";
    
    $db->commit();
    
    /*
     * se for super usuario, verificar se deseja efetuar o processamento do agendamento no exato momento
     * MUDANÇA DE REGRA SOLICITADA PELO WESLEY LIRA 23/07/09 : PERMITIR QUE TODOS OS USUÁRIOS POSSAM CARREGAR A AGENDAMENTO NO EXATO MOMENTO
     * DA CARGA
     */
    if(count($erros)==0) {
    	echo "<script>
    			parent.efetuarprocessamento('".formata_data_sql($dados['agddataprocessamento'])."','".date("Y-m-d")."','".$agdid."');
	    		parent.ajaxatualizar('requisicao=carregaragendamento&indid=".$_SESSION['indid']."','listaagendamento');
    		  </script>";
	    fwrite($handle, "-- SOLICITANDO CONFIRMAÇÃO PARA USUARIO DA EFETUAÇÃO DA CARGA  --------------- (CPF_".$_SESSION['usucpf'].", ".date("d/m/Y h:i:s").")\n");
    }
    fwrite($handle, "----------------- FIM IMPORTANDO AGENDAMENTO nº".$agdid." --------------- (CPF_".$_SESSION['usucpf'].", ".date("d/m/Y h:i:s").")\n");
    fclose($handle);
    exit;
}

/**
 * Função dá a opção de escolha das configurações de carga.
 * 
 * @author Felipe Chiavicatti
 * @return void
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/09/2011
 */
function escolherconfigcarga(){
	global $db;
	
	$arrRegionalizacao = array(REGIONALIZACAO_MUN, REGIONALIZACAO_UF,REGIONALIZACAO_REGIAO);
		
//	if (!in_array($_SESSION['painel']['regid'], $arrRegionalizacao)){
	if (!in_array($_SESSION['regid'], $arrRegionalizacao)){
		die("<script>
				window.location = '?modulo=principal/cargaporindicador&acao=A&requisicao=downloadmodelocarga';
			 </script>");
	}
	
	$sql = "(
				SELECT
					ci.cgidsc,
					dc.total,
					u.usunome,
					ci.cgiid
				FROM
					painel.cargaindicador ci
				JOIN (SELECT COUNT(*) as total, cgiid FROM painel.dadocargaindicador GROUP BY cgiid) dc ON dc.cgiid = ci.cgiid
				JOIN seguranca.usuario u ON u.usucpf = ci.usucpf
				WHERE
					cgipublico = true AND
					regid = {$_SESSION['regid']}
			)UNION(
				SELECT
					ci.cgidsc,
					dc.total,
					u.usunome,
					ci.cgiid
				FROM
					painel.cargaindicador ci
				JOIN (SELECT COUNT(*) as total, cgiid FROM painel.dadocargaindicador GROUP BY cgiid) dc ON dc.cgiid = ci.cgiid
				JOIN seguranca.usuario u ON u.usucpf = ci.usucpf AND
											u.usucpf = '{$_SESSION['usucpf']}'
				WHERE
					cgipublico = false AND
					regid = {$_SESSION['regid']}
			)";
	$rs = $db->carregar($sql);

	$arCabecalho = array("Ação","Descrição","Qtd. Regionalização", "Publicado por");
$acao = "<center>
			<input type='radio' name='cgiid' value='{cgiid}'>
		 </center>";
// ARRAY de parametros de configuração da tabela
$arConfig = array("style" => "width:95%;",
				  "totalLinha" => false,
				  "totalRegistro" => true);

$oLista = new Lista($arConfig);
$oLista->setCabecalho( $arCabecalho );
$oLista->setCorpo( $rs, $arParamCol );
$oLista->setAcao( $acao );
?>
<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<script>
		function gerar()
		{
			var dpeid = document.getElementsByName('dpeid')[0].value;		
			if (dpeid){
				document.formulario.submit();
			}else{
				alert('Escolha uma periodicidade!');
			}
		}
		</script>
	</head>
	<body>	
		<?php monta_titulo('Escolha de configuração', '')?>
		<form name="formulario" action="?modulo=principal/cargaporindicador&acao=A&requisicao=downloadmodelocarga" method="post">
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloDireita">Periodicidade:</td>
				<td>
			    <?
			    	$sql = "SELECT
						      	dpeid as codigo,
						       	dpedsc as descricao
							FROM
						       	painel.detalheperiodicidade dsh        
							JOIN 
								painel.indicador ind ON ind.perid = dsh.perid
							WHERE
						       	dsh.dpestatus = 'A' AND
						       	ind.indid = {$_SESSION['indid']}
							ORDER BY
						       	dpeanoref ASC, dpemesref ASC";
			    	
			  		echo $db->monta_combo('dpeid',$sql,'S','Selecione...',"",'','',150,'S','','',$dpeid);
			    ?>
				</td>
			</tr>
			<tr>
				<td colspan="2">
					<center>
						<fieldset style="width:100x'%;">
							<legend>Opções de configurações (opcional)</legend>
							<?php $oLista->show();?>
						</fieldset>
					</center>
				</td>
			</tr>
		<tr bgcolor="#cccccc">
			<td>&nbsp;</td>
			<td>
				<input type="button" class="botao" name="btnnovo" value="Gerar CSV" onclick="gerar();">	  	    	  	     
			</td>
		</tr>
	</table>	
	</form>	
	</body>
</html>
<?php 
die;
}

function recuperaDadosDetalhe()
{
	global $db;
	// Detalhe do indicador
	$sql = "SELECT 
				tdiid,
				tdidsc 
			FROM 
				painel.detalhetipoindicador 
			WHERE 
				tdistatus = 'A' AND
				indid='".$_SESSION['indid']."' 
			ORDER BY 
				tdiordem";
	$detalheIndicador = $db->carregar($sql);
//	$detalheindicador = $detalheindicador ? $detalheindicador : array();
	$arDado 		   = array();
	$csvlabelCabecalho = array(); 
	if ($detalheIndicador){
		
		$tdiidIni = $detalheIndicador[0]['tdiid'];
		foreach ($detalheIndicador as $detalheIndicador){
			$csvlabelCabecalho[] = "id detalhe <font size=1>({$detalheIndicador['tdidsc']})</font>"; 
			// Detalhe do indicador
			$sql = "SELECT 
						tidid
					FROM 
						painel.detalhetipodadosindicador 
					WHERE 
						tidstatus = 'A' AND
						tdiid='".$detalheIndicador['tdiid']."' 
					ORDER BY 
						tidid";
			$detalheDadoIndicador = $db->carregarColuna($sql);
			$arDado[] = $detalheDadoIndicador; 
		}
	}
	
	return array($csvlabelCabecalho ,$arDado);
}

function montaDadosCSV($reg, $dados)
{
	$csvlabel = 'id do indicador;id da periodicidade;quantidade;';
	
	$arDadosDetalhe = recuperaDadosDetalhe();
	$csvlabel 		.= $arDadosDetalhe[0] ? implode(";", $arDadosDetalhe[0]).";" : "";
	$dadosDetalhe 	= $arDadosDetalhe[1];
	
	switch ($reg){
		case REGIONALIZACAO_MUN:
			$csvlabel .= 'código do município' . "\n";
			break;
		case REGIONALIZACAO_UF:
			$csvlabel .= 'uf' . "\n";
			break;
		case REGIONALIZACAO_REGIAO:
			$csvlabel .= 'Região' . "\n";
			break;
	}	

	$indid = $_SESSION['indid'];
	$dpeid = $_POST['dpeid'];
	
	switch (count($dadosDetalhe)){
		case 1:
			foreach ($dadosDetalhe[0] as $detalhe){
				foreach ($dados as $dado){
					$csvlabel .= "{$indid};{$dpeid};x;{$detalhe};{$dado}\n";
				}
			}
			break;
		case 2:
			foreach ($dadosDetalhe[0] as $detalhe){
				foreach ($dadosDetalhe[1] as $detalhe1){
//			dbg($dadosDetalhe[1], d);
					foreach ($dados as $dado){
						$csvlabel .= "{$indid};{$dpeid};x;{$detalhe};{$detalhe1};{$dado}\n";
					}
				}	
			}
			break;
		default:
			foreach ($dados as $dado){
				$csvlabel .= "{$indid};{$dpeid};x;{$dado}\n";
			}
			break;
	}
	
	return $csvlabel;
}

/**
 * Função que cria e faz o download da do modelo dos dados.
 * 
 * @author Alexandre Dourado
 * @return array $_VALIDACAO
 * @param integer $indid
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function downloadmodelocarga(){
	global $db;
	
//	$sql 	   = "SELECT * FROM painel.indicador WHERE indid = {$_SESSION['indid']}";
//	$indicador = $db->pegaLinha($sql);
	
	switch ($_SESSION['regid']){
		case REGIONALIZACAO_MUN:
//			$indid 		= $_SESSION['indid'];
			$cgiid 		= $_POST['cgiid'];
//			$dpeid 		= $_POST['dpeid'];
//			$muncod 	= false;
			if ($cgiid){
				$sql = "SELECT 
							dcidsc
						FROM 
							painel.dadocargaindicador
						WHERE
							cgiid = {$cgiid}
						ORDER BY
							dcidsc";
				
				$muncod = (array) $db->carregarColuna($sql);
			}else{
				$sql = "SELECT
							muncod
						FROM
							territorios.municipio
						ORDER BY
							muncod";
				$muncod = (array) $db->carregarColuna($sql);
			}	
			
			$csvlabel = montaDadosCSV(REGIONALIZACAO_MUN, $muncod);
//			$csvlabel 	= 'id do indicador;id da periodicidade;quantidade;código do município' . "\n";
//			foreach ($muncod as $muncod){
//				$csvlabel .= "{$indid};{$dpeid};x;{$muncod}\n";
//			}			
		break;
		case REGIONALIZACAO_UF:
//			$indid 		= $_SESSION['indid'];
			$cgiid 		= $_POST['cgiid'];
//			$dpeid 		= $_POST['dpeid'];
//			$muncod 	= false;
			if ($cgiid){
				$sql = "SELECT 
							dcidsc
						FROM 
							painel.dadocargaindicador
						WHERE
							cgiid = {$cgiid}
						ORDER BY
							dcidsc";
				
				$estuf = (array) $db->carregarColuna($sql);
			}else{
				$sql = "SELECT
							estuf
						FROM
							territorios.estado
						ORDER BY
							estuf";
				$estuf = (array) $db->carregarColuna($sql);
			}	
			
//			$csvlabel 	= 'id do indicador;id da periodicidade;quantidade;id detalhe <font size=1>(Tipo)</font> ;uf' . "\n";
//			foreach ($estuf as $estuf){
//				$csvlabel .= "{$indid};{$dpeid};x;x;{$estuf}\n";
//			}			
			$csvlabel = montaDadosCSV(REGIONALIZACAO_UF, $estuf);
		break;
		case REGIONALIZACAO_REGIAO:
//			$indid 		= $_SESSION['indid'];
			$cgiid 		= $_POST['cgiid'];
//			$dpeid 		= $_POST['dpeid'];
//			$muncod 	= false;
			if ($cgiid){
				$sql = "SELECT 
							dcidsc
						FROM 
							painel.dadocargaindicador
						WHERE
							cgiid = {$cgiid}
						ORDER BY
							dcidsc";
				
				$regcod = (array) $db->carregarColuna($sql);
			}else{
				$sql = "SELECT
							regcod
						FROM
							territorios.regiao
						ORDER BY
							regcod";
				$regcod = (array) $db->carregarColuna($sql);
			}	
			
//			$csvlabel 	= 'id do indicador;id da periodicidade;quantidade;id detalhe <font size=1>(Tipo)</font> ;uf' . "\n";
//			foreach ($estuf as $estuf){
//				$csvlabel .= "{$indid};{$dpeid};x;x;{$estuf}\n";
//			}
			$csvlabel = montaDadosCSV(REGIONALIZACAO_REGIAO, $regcod);
		break;
		default:
			$_VALIDACAO = criarestruturavalidacao($_SESSION['indid']);
			
			if($_VALIDACAO) {
				foreach($_VALIDACAO as $key => $valid) {
					if($key==(count($_VALIDACAO)-1)) $sep="";
					else $sep=";";
					$csvlabel     .= $valid['label'].$sep;
					$arrEx[] = array("valor" => $valid['valor'], "sep" => $sep);
				}
			}
			$csvlabel .= "\n";
			$i = 0;
			while( $i < 10){
				$i++;
				foreach ($arrEx as $arr){
					/*
					 * COMENTADO POR ALEXANDRE DOURADO
					 * SOLICITADO PELO TERZIO
					 * IMPRIMIR SOMENTE O CÓDIGO DO INDICADOR, O RESTANTE IMPRIMIR "X"
					 * if($arr['valor'] != $_SESSION['indid'] && !is_string($arr['valor'])){
					 */
					if($arr['valor'] != $_SESSION['indid']){
						$arr['valor'] = "x";
					}
					$csvlabel .= $arr['valor'].$arr['sep'];
				}
				$csvlabel .= "\n";
			}
		break;	
	}
	header("Content-Type: text/html; charset=ISO-8859-1");
	header("Content-type: application/octet-stream");
	header("Content-Disposition: attachment; filename=\"codigo_modelo.csv\"");
	echo $csvlabel;
	exit;
}

function carregarlistacargaautomatica($dados) {
	global $db;
	$sql = "SELECT '<center>' || CASE WHEN c.cauid IS NULL THEN '<img src=\"../imagens/gif_inclui.gif\" border=\"0\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/cargaautomatica&acao=A&evento=detalhe&indid='||i.indid||'\';\">' ELSE ' <img src=../imagens/reject.png style=cursor:pointer; onclick=rodarcaragaautomatica('|| i.indid ||');> <img src=\"../imagens/alterar.gif\" border=\"0\"  style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/cargaautomatica&acao=A&evento=detalhe&indid='||i.indid||'\';\"> <img src=\"../imagens/excluir.gif\" border=\"0\" style=\"cursor:pointer\" onclick=\"deletarcargaautomatica('||c.cauid||')\">' END || '</center>' as acoes, i.indid, i.indnome, CASE WHEN c.cauid IS NULL THEN '<center><font color=#FF0000><b>Consulta não detalhada</b></font></center>' ELSE '<center><b>Consulta detalhado</b></center>' END as status FROM painel.indicador i 
			LEFT JOIN painel.cargaautomatica c ON i.indid = c.indid 
			WHERE i.colid='".COLETA_AUTOMATICA."' AND i.indstatus='A' ORDER BY i.indid";
	$cabecalho = array("Ações", "Identificador", "Indicador", "Status");
	$db->monta_lista_simples( $sql, $cabecalho, 500, 5, 'N', '100%','N');
	exit;
}

function inserircargaautomatica($dados) {
	global $db;
	$sql = "INSERT INTO painel.cargaautomatica(
            indid, causql, cautipoproc, caudiasem, caudiames)
    		VALUES ('".$dados['indid']."', '".$dados['causql']."', '".$dados['cautipoproc']."', ".(($dados['caudiasem'])?"'".implode(";", $dados['caudiasem'])."'":"NULL").", ".(($dados['caudiames'])?"'".$dados['caudiames']."'":"NULL").");";
	
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Detalhe da carga inserida com sucesso');
			window.location='?modulo=principal/cargaautomatica&acao=A';
		  </script>";
	exit;
}

function atualizarcargaautomatica($dados) {
	global $db;
	switch($dados['cautipoproc']) {
		case '1':
			unset($dados['caudiasem'],$dados['caudiames']);
			break;
		case '2':
			unset($dados['caudiames']);
			break;
		case '3':
			unset($dados['caudiasem']);
			break;
	}
	
	$sql = "UPDATE painel.cargaautomatica SET
            causql = '".$dados['causql']."',
            cautipoproc = '".$dados['cautipoproc']."',
            caudiasem = ".(($dados['caudiasem'])?"'".implode(";", $dados['caudiasem'])."'":'NULL').", 
            caudiames = ".(($dados['caudiames'])?"'".$dados['caudiames']."'":"NULL")." 
            WHERE cauid='".$dados['cauid']."'";
	
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Detalhe da carga atualizado com sucesso');
			window.location='?modulo=principal/cargaautomatica&acao=A';
		  </script>";
	exit;
}
function removercargaautomatica($dados) {
	global $db;
	$sql = "DELETE FROM painel.cargaautomatica WHERE cauid='".$dados['cauid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Detalhe da carga removido com sucesso');
			window.location='?modulo=principal/cargaautomatica&acao=A';
		  </script>";
	exit;
}

?>