<?
/*
 * VALIDANDO SE EXISTE O ID DO INDICADOR NA SESSÃO
 * É OBRIGATÓRIO A EXISTÊNCIA DESTA VARIAVEL NA SESSÃO
 */
if(!$_SESSION['indid']) {
	echo "<script>
			alert('Não existe indicador selecionado.');
			window.location='?modulo=inicio&acao=C';
		  <script>";
	exit;
}

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(300);
/* FIM configurações */


/* Segurança : Validando os acessos as funções autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('carregarComboMunicipioPorUF'=>true,
									 'carregarGridEscola'=>true,
									 'carregarGridIES'=>true,
									 'carregarGridMunicipio'=>true,
									 'carregarGridEstado'=>true,
									 'carregarGridBrasil'=>true,
									 'gravarGridDados'=>true,
									 'gravarGridDadosSemFiltros'=>true,
									 'atualizarDetalhesTipoDadosIndicador'=>true,
									 'removerDetalhesTipoIndicador'=>true,
									 'excluirDetalheTipoDadosIndicador'=>true,
									 'removerDetalhesTipoDadosIndicador'=>true,
									 'carregarComboUFPorPeriodo'=>true,
									 'carregarGridPosgraducao'=>true);
	break;
}
/* Fim Segurança : Validando os acessos as funções autorizadas */

if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}

// monta cabeçalho
include APPRAIZ . "includes/cabecalho.inc";
echo "<br />";

$db->cria_aba($abacod_tela,$url,'');
monta_titulo("Série Histórica", "&nbsp;");
cabecalhoIndicador($_SESSION['indid']);

$dadosindicador = $db->pegaLinha("SELECT * FROM painel.indicador WHERE indid='".$_SESSION['indid']."'");

// pegando o formato do campo
$formatoinput = pegarFormatoInput();


switch($dadosindicador['regid']) {
	case REGIONALIZACAO_POSGRADUACAO:
		
		//Se o indiacador for 'percentual' (unmid = 1), não pode somar
		$dpedsc = $dadosindicador['unmid'] != 1 ? "dpedsc||'('||trim(to_char(COALESCE((SELECT SUM(dshqtde) FROM painel.detalheseriehistorica d INNER JOIN painel.seriehistorica s ON s.sehid = d.sehid WHERE s.dpeid=dp.dpeid AND s.indid='".$_SESSION['indid']."'  AND (sehstatus='A' OR sehstatus='H')),0),'".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."'))||')' as descricao" : "dpedsc as descricao"; 
		
		$tabeladefiltros[] = array("descricao" 		  => "Período:",
								   "nome"  			  => "dpeid",
								   "funcaojavascript" => "executar_periodoestado",
								   "editavel"		  => "S",
								   "sql"   			  => "SELECT dp.dpeid as codigo,  $dpedsc FROM painel.detalheperiodicidade dp WHERE dp.perid='".$dadosindicador['perid']."' ORDER BY dpedatainicio");
	
		$tabeladefiltros[] = array("descricao" 		  => "Estado:",
								   "span_id"          => "spanestuf",
								   "nome"  			  => "estuf",
								   "funcaojavascript" => "executar_estadomunicipio",
								   "editavel"		  => "N",
								   "sqldin"		  	  => "SELECT e.estuf as codigo , e.estdescricao||'('||trim(to_char(COALESCE(sum(dshqtde),0),'".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."'))||')' as descricao
														 FROM territorios.estado e 
														 LEFT JOIN territorios.municipio mun ON mun.estuf = e.estuf 
														 LEFT JOIN painel.iepg iep ON iep.muncod = mun.muncod 
														 LEFT JOIN painel.detalheseriehistorica d ON d.iepid = iep.iepid  
														 LEFT JOIN painel.seriehistorica s ON s.sehid = d.sehid 
														 WHERE {condicao_dpeid} s.indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H') 
														 GROUP BY e.estuf, estdescricao ORDER BY descricao");
		
		$tabeladefiltros[] = array("descricao"		  => "Município:",
								   "span_id"          => "spanmuncod",
								   "nome"  			  => "muncod",
								   "funcaojavascript" => "montargrid_posgraducao",
								   "editavel"		  => "N",
								   "td_id"		  	  => "tdmuncod",
								   "sqldin"		  	  => "SELECT mun.muncod as codigo , mun.mundescricao||'('||trim(to_char(COALESCE(sum(dshqtde),0),'".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."'))||')' as descricao
														 FROM territorios.municipio mun  
														 LEFT JOIN painel.iepg iep ON iep.muncod = mun.muncod 
														 LEFT JOIN painel.detalheseriehistorica d ON d.iepid = iep.iepid 
														 LEFT JOIN painel.seriehistorica s ON s.sehid = d.sehid 
														 WHERE {condicao_dpeid} {condicao_estuf} s.indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H') 
														 GROUP BY mun.muncod, mun.mundescricao ORDER BY mun.mundescricao");
		
		break;
	case REGIONALIZACAO_BRASIL:
		break;
	case REGIONALIZACAO_ESCOLA:
		
		//Se o indiacador for 'percentual' (unmid = 1), não pode somar
		$dpedsc = $dadosindicador['unmid'] != 1 ? "dpedsc||'('||trim(to_char(COALESCE((SELECT SUM(dshqtde) FROM painel.detalheseriehistorica d INNER JOIN painel.seriehistorica s ON s.sehid = d.sehid WHERE s.dpeid=dp.dpeid AND s.indid='".$_SESSION['indid']."'  AND (sehstatus='A' OR sehstatus='H')),0),'".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."'))||')' as descricao" : "dpedsc as descricao"; 
		
		
		$tabeladefiltros[] = array("descricao" 		  => "Período:",
								   "nome"  			  => "dpeid",
								   "funcaojavascript" => "executar_periodoestado",
								   "editavel"		  => "S",
								   "sql"   			  => "SELECT dp.dpeid as codigo, $dpedsc FROM painel.detalheperiodicidade dp WHERE dp.perid='".$dadosindicador['perid']."' ORDER BY dpedatainicio");
	
		$tabeladefiltros[] = array("descricao" 		  => "Estado:",
								   "span_id"          => "spanestuf",
								   "nome"  			  => "estuf",
								   "funcaojavascript" => "executar_estadomunicipio",
								   "editavel"		  => "N",
								   "sqldin"		  	  => "SELECT e.estuf as codigo , e.estdescricao||'('||trim(to_char(COALESCE(sum(dshqtde),0),'".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."'))||')' as descricao
														 FROM territorios.estado e 
														 LEFT JOIN territorios.municipio mun ON mun.estuf = e.estuf 
														 LEFT JOIN painel.escola esc ON esc.escmuncod = mun.muncod 
														 LEFT JOIN painel.detalheseriehistorica d ON d.dshcod = esc.esccodinep 
														 LEFT JOIN painel.seriehistorica s ON s.sehid = d.sehid 
														 WHERE {condicao_dpeid} s.indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H') 
														 GROUP BY e.estuf, estdescricao ORDER BY descricao");
		
		$tabeladefiltros[] = array("descricao"		  => "Município:",
								   "span_id"          => "spanmuncod",
								   "nome"  			  => "muncod",
								   "funcaojavascript" => "montargrid_escola",
								   "editavel"		  => "N",
								   "td_id"		  	  => "tdmuncod",
								   "sqldin"		  	  => "SELECT mun.muncod as codigo , mun.mundescricao||'('||trim(to_char(COALESCE(sum(dshqtde),0),'".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."'))||')' as descricao
														 FROM territorios.municipio mun  
														 LEFT JOIN painel.escola esc ON esc.escmuncod = mun.muncod 
														 LEFT JOIN painel.detalheseriehistorica d ON d.dshcod = esc.esccodinep 
														 LEFT JOIN painel.seriehistorica s ON s.sehid = d.sehid 
														 WHERE {condicao_dpeid} {condicao_estuf} s.indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H') 
														 GROUP BY mun.muncod, mun.mundescricao ORDER BY descricao");
		
		break;
	case REGIONALIZACAO_IES:
		$tabeladefiltros[] = array("descricao" 		  => "Período:",
								   "nome"  			  => "dpeid",
								   "funcaojavascript" => "executar_periodoestado",
								   "editavel"		  => "S",
								   "sql"   			  => "SELECT dp.dpeid as codigo, dp.dpedsc||'('||trim(to_char(COALESCE((SELECT SUM(dshqtde) FROM painel.detalheseriehistorica d INNER JOIN painel.seriehistorica s ON s.sehid = d.sehid WHERE s.dpeid=dp.dpeid AND s.indid='".$_SESSION['indid']."'  AND (sehstatus='A' OR sehstatus='H')),0),'".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."'))||')' as descricao FROM painel.detalheperiodicidade dp WHERE dp.perid='".$dadosindicador['perid']."' ORDER BY dpedatainicio");
		
		$tabeladefiltros[] = array("descricao" 		  => "Estado:",
								   "span_id"          => "spanestuf",
								   "nome"  			  => "estuf",
								   "funcaojavascript" => "executar_estadomunicipio",
								   "editavel"		  => "N",
								   "sqldin"			  => "SELECT e.estuf as codigo , e.estdescricao||'('||trim(to_char(COALESCE(sum(dshqtde),0),'".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."'))||')' as descricao
														 FROM territorios.estado e 
														 LEFT JOIN territorios.municipio mun ON mun.estuf = e.estuf 
														 LEFT JOIN painel.ies ies ON ies.iesmuncod = mun.muncod 
														 LEFT JOIN painel.detalheseriehistorica d ON d.dshcod = cast(ies.iesid as varchar) 
														 LEFT JOIN painel.seriehistorica s ON s.sehid = d.sehid 
														 WHERE {condicao_dpeid} s.indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H') 
														 GROUP BY e.estuf, estdescricao ORDER BY descricao");
		
		$tabeladefiltros[] = array("descricao"		  => "Município:",
								   "span_id"          => "spanmuncod",
								   "nome"  			  => "muncod",
								   "funcaojavascript" => "montargrid_ies",
								   "editavel"		  => "N",
								   "td_id"		  	  => "tdmuncod",
								   "sqldin"		  	  => "SELECT mun.muncod as codigo , mun.mundescricao||'('||trim(to_char(COALESCE(sum(dshqtde),0),'".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."'))||')' as descricao
														 FROM territorios.municipio mun  
														 LEFT JOIN painel.ies ies ON ies.iesmuncod = mun.muncod 
														 LEFT JOIN painel.detalheseriehistorica d ON d.dshcod = cast(ies.iesid as varchar) 
														 LEFT JOIN painel.seriehistorica s ON s.sehid = d.sehid 
														 WHERE {condicao_dpeid} {condicao_estuf} s.indid='".$_SESSION['indid']."' AND (sehstatus='A' OR sehstatus='H') 
														 GROUP BY mun.muncod, mun.mundescricao ORDER BY descricao");
		
		
		break;
	case REGIONALIZACAO_MUN:
		$tabeladefiltros[] = array("descricao" 		  => "Período:",
								   "nome"  			  => "dpeid",
								   "funcaojavascript" => "executar_periodoestado",
								   "editavel"		  => "S",
								   "sql"   			  => "SELECT dp.dpeid as codigo, dpedsc||'('||trim(to_char(COALESCE((SELECT SUM(dshqtde) FROM painel.detalheseriehistorica d INNER JOIN painel.seriehistorica s ON s.sehid = d.sehid WHERE s.dpeid=dp.dpeid AND s.indid='".$_SESSION['indid']."'  AND (sehstatus='A' OR sehstatus='H')),0),'".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."'))||')' as descricao FROM painel.detalheperiodicidade dp WHERE dp.perid='".$dadosindicador['perid']."' ORDER BY dpedatainicio");
		
		$tabeladefiltros[] = array("descricao" 		  => "Estado:",
								   "span_id"          => "spanestuf",          
								   "nome"  			  => "estuf",
								   "funcaojavascript" => "montargrid_municipio",
								   "editavel"		  => "N",
								   "sqldin"		  	  => "SELECT estuf as codigo, estdescricao||'('||trim(to_char(COALESCE((SELECT SUM(dshqtde) FROM painel.detalheseriehistorica d 
														  INNER JOIN painel.seriehistorica s ON s.sehid = d.sehid 
														  INNER JOIN territorios.municipio mun ON mun.muncod = d.dshcodmunicipio 
														  WHERE {condicao_dpeid} s.indid='".$_SESSION['indid']."' AND mun.estuf=e.estuf AND (sehstatus='A' OR sehstatus='H')),0),'".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."'))||')' as descricao FROM territorios.estado e");
		break;
	case REGIONALIZACAO_UF:
		$tabeladefiltros[] = array("descricao" 		  => "Período:",
								   "nome"  			  => "dpeid",
								   "funcaojavascript" => "montargrid_estado",
								   "editavel"		  => "S",
								   "sql"   			  => "SELECT dp.dpeid as codigo, dpedsc||'('||trim(to_char(COALESCE((SELECT SUM(dshqtde) FROM painel.detalheseriehistorica d INNER JOIN painel.seriehistorica s ON s.sehid = d.sehid WHERE s.dpeid=dp.dpeid AND s.indid='".$_SESSION['indid']."'  AND (sehstatus='A' OR sehstatus='H')),0),'".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."'))||')' as descricao FROM painel.detalheperiodicidade dp WHERE dp.perid='".$dadosindicador['perid']."' ORDER BY dpedatainicio");
		break;
}

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="./js/painel.js"></script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloCentro' colspan='<? echo (count($tabeladefiltros)*2); ?>'>Filtros</td>
</tr>
<tr>
<?
if($tabeladefiltros) {
	foreach($tabeladefiltros as $tbl) {
		echo "<td class='SubTituloDireita' style='width:150px;'>".$tbl['descricao']."</td>";
		echo "<td ".(($tbl['td_id'])?"id='".$tbl['td_id']."'":"").">";
		echo "<span style='display:none' id='".$tbl['span_id']."'>".base64_encode($tbl['sqldin'])."</span>";
		$db->monta_combo($tbl['nome'], (($tbl['sql'])?$tbl['sql']:array()), $tbl['editavel'], 'Selecione', $tbl['funcaojavascript'], '', '', '200', 'S', $tbl['nome']);
		echo "</td>";
	}
} else {
	echo "<td class='SubTituloEsquerda'>Preencha os valores referentes aos períodos descritos.</td>";
}
?>
</tr>
</table>
<div id="gridinfo"></div>
<script>
<?
if($tabeladefiltros) {
	if($_REQUEST['dpeid']) {
		echo "document.getElementById('dpeid').value='".$_REQUEST['dpeid']."';";
		echo "document.getElementById('dpeid').onchange();";
	}
	if($_REQUEST['estuf']) {
		echo "document.getElementById('estuf').value='".$_REQUEST['estuf']."';";
		echo "document.getElementById('estuf').onchange();";
	}
	if($_REQUEST['muncod']) {
		echo "document.getElementById('muncod').value='".$_REQUEST['muncod']."';";
		echo "document.getElementById('muncod').onchange();";
	}
} else {
	echo "montargrid_brasil();";
}
?>
redimencionarBodyData();
</script>