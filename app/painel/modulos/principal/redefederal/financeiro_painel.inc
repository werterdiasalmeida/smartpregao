<?php

header('content-type: text/html; charset=ISO-8859-1');

$ano = $_REQUEST['ano'] ? $_REQUEST['ano'] : '2009';

if ( $_REQUEST["relatorioajax"] ){
	
	$orgid = $_SESSION["redefederal"]["orgid"];
	
	// vefirica se ï¿½ campus/uned ou unidade pai
	switch ( $orgid ){
		case '1':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$_REQUEST['entid']} AND funid = " . 18);
			break;
		case '2':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$_REQUEST['entid']} AND funid = " . 17);
			break;
	}
	
	// verifica as requisiï¿½ï¿½es e cria os $_REQUEST de acordo com o relatï¿½rio selecionado
	switch ($_REQUEST["relatorioajax"]) {
		case 'execucao':
			
			// pega o unicod da unidade
			if ( !empty($campus) ){
				$entid = $db->pegaUm("SELECT entid FROM entidade.funentassoc WHERE fueid = {$campus}");
				if ( !empty($entid) ){
					$unicod = $db->pegaUm("SELECT entunicod FROM entidade.entidade WHERE entid = {$entid}"); 
				}
			}else{
				$unicod = $db->pegaUm("SELECT entunicod FROM entidade.entidade WHERE entid = {$_REQUEST['entid']}");
			}
			
			// cria os $_REQUEST do relatï¿½rio
			$_SESSION["redefederal"]['financeiro'] = array( "agrupador" 	   	   => array( 0 => "uo",
																		 	 1 => "acacod"),
															"agrupadorColunas" => array( 0 => "19", 	
																			 1 => "6", 
																			 2 => "12", 
																			 3 => "7" ),
															"uo"			   => array( 0 => $unicod ),
															"gnd"			   => array( 0 => "1", 
																				  		 1 => "3", 
																				  		 2 => "4", 
																				  		 3 => "5" ) );
		
		break;
			
	}
	
}

$_REQUEST = $_SESSION["redefederal"]['financeiro'];

ini_set( "memory_limit", "2048M" );
set_time_limit(0);

// Parï¿½metros para a nova conexï¿½o com o banco do SIAFI
$servidor_bd = $servidor_bd_siafi;
$porta_bd    = $porta_bd_siafi;
$nome_bd     = $nome_bd_siafi;
$usuario_db  = $usuario_db_siafi;
$senha_bd    = $senha_bd_siafi;

// Cria o novo objeto de conexï¿½o
$db2 = new cls_banco();

// Pegar a data do 'Acumulado Atï¿½'
$valorAcumulado = $db2->carregar("SELECT it_da_transacao AS data, 
									  count(it_da_transacao) AS qtd
								FROM siafi{$ano}.saldocontabil
								GROUP BY it_da_transacao
								ORDER BY it_da_transacao DESC
								LIMIT 2");

// Se o valor do campo "qtd" do primeiro registro for MAIOR que 100, usar a data do primeiro registro.
// Se o valor do campo "qtd" do primeiro registro for MENOR que 100, usar a data do segundo registro.
$valorAcumulado = ($valorAcumulado[0]["qtd"] > 100) ? $valorAcumulado[0]["data"] : $valorAcumulado[1]["data"];


$arrayTituloAgrupadores = array();
$tituloAgrupadores = "";
$arraySelectCod = array();
$selectCod = "";
$arraySelectDsc = array();
$selectDsc = "";
$arrayCase = array();
$case = "";
$arrayJoin = array();
$join = "";
$arrayWhere = array();
//$where = " sld.ano = '".$_SESSION['exercicio']."' ";
$where = "";
$arrayGroupBy = array();
$groupBy = "";

$arrayAuxiliar = array();

for($i=0; $i<count($_REQUEST["agrupador"]); $i++) {
	switch($_REQUEST["agrupador"][$i]) {
		case "acacod":
			array_push($arrayTituloAgrupadores, "Ação");
			
			array_push($arraySelectCod, "sld.acacod");
			array_push($arraySelectDsc, "aca.acadsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.acao aca ON aca.acacod = sld.acacod");
			
			array_push($arrayGroupBy, "sld.acacod");
			array_push($arrayGroupBy, "aca.acadsc");
			
			break;
			
		case "catecon":
			array_push($arrayTituloAgrupadores, "Categoria Econômica");
			
			array_push($arraySelectCod, "sld.ctecod");
			array_push($arraySelectDsc, "cte.ctedsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.categoriaeconomica cte ON CAST(cte.ctecod AS text) = sld.ctecod");
			
			array_push($arrayGroupBy, "sld.ctecod");
			array_push($arrayGroupBy, "cte.ctedsc");
			
			break;
			
		case "elemento":
			array_push($arrayTituloAgrupadores, "Elemento de Despesa");
			
			array_push($arraySelectCod, "sld.edpcod");
			array_push($arraySelectDsc, "edp.edpdsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.elementodespesa edp ON edp.edpcod = sld.edpcod");
			
			array_push($arrayGroupBy, "sld.edpcod");
			array_push($arrayGroupBy, "edp.edpdsc");
			
			break;
			
		case "subelemento":
			array_push($arrayTituloAgrupadores, "Sub-Elemento de Despesa");
			
			array_push($arraySelectCod, "sbe.sbecod");
			array_push($arraySelectDsc, "sbe.ndpdsc");
				
			array_push($arrayJoin, "LEFT JOIN ( select distinct ctecod || '.' || gndcod || '.' || mapcod || '.' || edpcod || '.' || sbecod as sbecod, ndpdsc from dw.naturezadespesa ) as sbe ON sbe.sbecod = sld.ctecod || '.' || sld.gndcod || '.' || coalesce(sld.mapcod,'00') || '.' || coalesce(sld.edpcod,'00') || '.' || coalesce(sld.sbecod,'00')");
				
			array_push($arrayGroupBy, "sbe.sbecod");
			array_push($arrayGroupBy, "sbe.ndpdsc");
						
			
			break;

		
		case "esfera": 
			array_push($arrayTituloAgrupadores, "Esfera");
			
			array_push($arraySelectCod, "sld.esfcod");
			array_push($arraySelectDsc, "esf.esfdsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.esfera esf ON substr(CAST(esf.esfcod AS text),1,1) = sld.esfcod");
			
			array_push($arrayGroupBy, "sld.esfcod");
			array_push($arrayGroupBy, "esf.esfdsc");
			
			break;
			
		case "fontesiafi":
			array_push($arrayTituloAgrupadores, "Fonte Detalhada");
			
			array_push($arraySelectCod, "fsf.foscod");
			array_push($arraySelectDsc, "fsf.fosdsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.fontesiafi fsf ON fsf.foscod = sld.foncod");
			
			array_push($arrayGroupBy, "fsf.foscod");
			array_push($arrayGroupBy, "fsf.fosdsc");
			
			break;

		case "fonte":
			array_push($arrayTituloAgrupadores, "Fonte SOF");
			
			array_push($arraySelectCod, "fon.foncod");
			array_push($arraySelectDsc, "fon.fondsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.fontesof fon ON fon.foncod = substr(sld.foncod,2,3)");
			
			array_push($arrayGroupBy, "fon.foncod");
			array_push($arrayGroupBy, "fon.fondsc");
			
			break;
			
		case "funcao":
			array_push($arrayTituloAgrupadores, "Função");
			
			array_push($arraySelectCod, "sld.funcod");
			array_push($arraySelectDsc, "fun.fundsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.funcao fun ON fun.funcod = sld.funcod");
			
			array_push($arrayGroupBy, "sld.funcod");
			array_push($arrayGroupBy, "fun.fundsc");
			
			break;
			
		case "gnd":
			array_push($arrayTituloAgrupadores, "GND");
			
			array_push($arraySelectCod, "sld.gndcod");
			array_push($arraySelectDsc, "gnd.gnddsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.gnd gnd ON cast(gnd.gndcod AS text) = sld.gndcod");
			
			array_push($arrayGroupBy, "sld.gndcod");
			array_push($arrayGroupBy, "gnd.gnddsc");
			
			break;
			
		case "grf":
			array_push($arrayTituloAgrupadores, "Grupo de Fonte");
			
			array_push($arraySelectCod, "grf.grfid");
			array_push($arraySelectDsc, "grf.grfdsc");
			
			array_push($arrayJoin, "LEFT JOIN (select fs.foscod, gf.grfid, gf.grfdsc from dw.fontesiafi fs inner join dw.grupofonte gf on gf.grfid = fs.grfid) as grf ON grf.foscod = sld.foncod");
			
			array_push($arrayGroupBy, "grf.grfid");
			array_push($arrayGroupBy, "grf.grfdsc");
			
			break;
			
		case "grupouo":
			array_push($arrayTituloAgrupadores, "Grupo de Unidade Orçamentária");
			
			array_push($arraySelectCod, "gun.guoid");
			array_push($arraySelectDsc, "gun.guodsc");
			
			array_push($arrayJoin, "LEFT JOIN (select uo.unicod, gu.guoid, gu.guodsc from dw.uguo uo inner join dw.grupouo gu on gu.guoid = uo.guoid group by uo.unicod, gu.guoid, gu.guodsc) as gun ON gun.unicod = sld.unicod");
			
			array_push($arrayGroupBy, "gun.guoid");
			array_push($arrayGroupBy, "gun.guodsc");
			
			break;


		case "orgaouo":
			array_push($arrayTituloAgrupadores, "Órgão da UO");
			
			array_push($arraySelectCod, "gun.unicod");
			array_push($arraySelectDsc, "gun.unidsc");
			
			array_push($arrayJoin, "LEFT JOIN (select uo.orgcoduo as unicod, uo.ugonome||' ('||uo.ugonomeabrev||')' as unidsc from dw.uguo uo group by uo.orgcoduo, uo.ugonome, uo.ugonomeabrev) as gun ON gun.unicod = sld.unicod");
			
			array_push($arrayGroupBy, "gun.unicod");
			array_push($arrayGroupBy, "gun.unidsc");
			
			break;
			
		case "mapcod":
			array_push($arrayTituloAgrupadores, "Modalidade de Aplicativo");
			
			array_push($arraySelectCod, "sld.mapcod");
			array_push($arraySelectDsc, "map.mapdsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.modalidadeaplicacao map ON map.mapcod = sld.mapcod");
			
			array_push($arrayGroupBy, "sld.mapcod");
			array_push($arrayGroupBy, "map.mapdsc");
			
			break;
			
		case "natureza":
			array_push($arrayTituloAgrupadores, "Natureza de Despesa");
			
			array_push($arraySelectCod, "sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod AS natureza");
			array_push($arraySelectDsc, "ndp.ndpdsc AS natureza_desc");
			
			array_push($arrayJoin, "LEFT JOIN ( select distinct ctecod, gndcod, mapcod, edpcod, ndpdsc from dw.naturezadespesa where sbecod = '00' ) as ndp ON cast(ndp.ctecod AS text) = sld.ctecod AND cast(ndp.gndcod AS text) = sld.gndcod AND cast(ndp.mapcod AS text) = sld.mapcod AND cast(ndp.edpcod AS text) = sld.edpcod");
			
			array_push($arrayGroupBy, "natureza");
			array_push($arrayGroupBy, "natureza_desc");
			
			break;
			
		case "programa":
			array_push($arrayTituloAgrupadores, "Programa");
			
			array_push($arraySelectCod, "sld.prgcod");
			array_push($arraySelectDsc, "prg.prgdsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.programa prg ON prg.prgcod = sld.prgcod");
			
			array_push($arrayGroupBy, "sld.prgcod");
			array_push($arrayGroupBy, "prg.prgdsc");
			
			break;
			
		case "subfuncao":
			array_push($arrayTituloAgrupadores, "Sub-função");
			
			array_push($arraySelectCod, "sld.sfucod");
			array_push($arraySelectDsc, "sfu.sfudsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.subfuncao sfu ON sfu.sfucod = sld.sfucod");
			
			array_push($arrayGroupBy, "sld.sfucod");
			array_push($arrayGroupBy, "sfu.sfudsc");
			
			break;
			
		case "ug":
			array_push($arrayTituloAgrupadores, "Unidade Gestora");
			
			array_push($arraySelectCod, "sld.ungcod");
			array_push($arraySelectDsc, "ung.ungdsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.ug ung ON ung.ungcod = sld.ungcod");
			
			array_push($arrayGroupBy, "sld.ungcod");
			array_push($arrayGroupBy, "ung.ungdsc");
			
			break;
			
		case "uo":
			array_push($arrayTituloAgrupadores, "Unidade Orçamentária");
			
			array_push($arraySelectCod, "sld.unicod");
			array_push($arraySelectDsc, "uni.unidsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.uo uni ON uni.unicod = sld.unicod");
			
			array_push($arrayGroupBy, "sld.unicod");
			array_push($arrayGroupBy, "uni.unidsc");
			
			break;
		
		case "orgao":
			array_push($arrayTituloAgrupadores, "Ação");
			
			array_push($arraySelectCod, "ors.orscod");
			array_push($arraySelectDsc, "ors.orsdsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.orgaosuperior ors ON substr(ors.orscod, 1, 2) = substr(sld.orgcod, 1, 2)");
			
			array_push($arrayGroupBy, "ors.orscod");
			array_push($arrayGroupBy, "ors.orsdsc");
			
			break;
			
		case "ptres":
			array_push($arrayTituloAgrupadores, "Ptres");
			
			array_push($arraySelectCod, "sld.ptres AS ptres");
			array_push($arraySelectDsc, "sld.ptres AS ptres_desc");
			
			array_push($arrayGroupBy, "ptres");
			array_push($arrayGroupBy, "ptres_desc");
			
			break;
			
		case "funcional":
			array_push($arrayTituloAgrupadores, "Funcional");
			
			array_push($arraySelectCod, "sld.prgcod || '.' || sld.acacod || '.' || sld.unicod || '.' || sld.loccod || ' - ' || aca2.acadsc || '(' || sld.loccod || ')' AS funcional");
			array_push($arraySelectDsc, "sld.prgcod || '.' || sld.acacod || '.' || sld.unicod || '.' || sld.loccod || ' - ' || aca2.acadsc || '(' || sld.loccod || ')' AS funcional_desc");
			
			array_push($arrayJoin, "LEFT JOIN dw.acao aca2 ON sld.acacod = aca2.acacod");
			
			array_push($arrayGroupBy, "funcional");
			array_push($arrayGroupBy, "funcional_desc");
			
			break;
			
		case "planointerno":
			array_push($arrayTituloAgrupadores, "Plano Interno");
			
			array_push($arraySelectCod, "sld.plicod");
			array_push($arraySelectDsc, "pli.plidsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.planointerno pli ON pli.plicod = sld.plicod");
			array_push($arrayWhere, "length(sld.plicod) = 11");
			
			array_push($arrayGroupBy, "sld.plicod");
			array_push($arrayGroupBy, "pli.plidsc");
			
			break;
			
		case "sldcontacorrente":
			array_push($arrayTituloAgrupadores, "Conta Corrente");
			
			array_push($arraySelectCod, "sld.sldcontacorrente AS conta_corrente");
			array_push($arraySelectDsc, "sld.sldcontacorrente AS conta_corrente_desc");
			
			array_push($arrayGroupBy, "conta_corrente");
			array_push($arrayGroupBy, "conta_corrente_desc");
			
			break;
		
		case "recurso":
			array_push($arrayTituloAgrupadores, "Recurso");
			
			array_push($arraySelectCod, "sld.trrcod");
			array_push($arraySelectDsc, "trr.trrdsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.tiporecurso trr ON cast(trr.trrcod AS text) = sld.trrcod");
			
			array_push($arrayGroupBy, "sld.trrcod");
			array_push($arrayGroupBy, "trr.trrdsc");
			
			break;
			
		case "vincod":
			array_push($arrayTituloAgrupadores, "Vinculaçãoo de Pagamento");
			
			array_push($arraySelectCod, "sld.vincod AS vincod");
			array_push($arraySelectDsc, "sld.vincod AS vincod_desc");
			
			array_push($arrayGroupBy, "vincod");
			array_push($arrayGroupBy, "vincod_desc");
			
			break;
			
		case "cagcod":
			array_push($arrayTituloAgrupadores, "Categoria de Gasto");
			
			array_push($arraySelectCod, "sld.cagcod");
			array_push($arraySelectDsc, "cag.cagdsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.categoriagasto cag ON cag.cagcod = sld.cagcod");
			
			array_push($arrayGroupBy, "sld.cagcod");
			array_push($arrayGroupBy, "cag.cagdsc");
			
			break;
			
		case "loccod":
			array_push($arrayTituloAgrupadores, "Subtítulo");
			
			array_push($arraySelectCod, "sld.loccod AS loccod");
			array_push($arraySelectDsc, "sld.loccod AS loccod_desc");
			
			array_push($arrayGroupBy, "loccod");
			array_push($arrayGroupBy, "loccod_desc");
			
			break;
			
		case "enquadramento":
			array_push($arrayTituloAgrupadores, "Enquadramento da Despesa");
			
			array_push($arraySelectCod, "ct1.cdtcod AS enquadramento_cod");
			array_push($arraySelectDsc, "ct1.cdtdsc AS enquadramento_dsc");
			
			array_push($arrayJoin, "LEFT JOIN public.combodadostabela ct1 ON ct1.ctbid = 5 AND ct1.cdtstatus = 'A' AND ct1.cdtcod = substr(sld.plicod, 1, 1)");
			
			array_push($arrayGroupBy, "enquadramento_cod");
			array_push($arrayGroupBy, "enquadramento_dsc");
			
			break;
			
		case "executor":
			array_push($arrayTituloAgrupadores, "Executor Orçamentário e Financeiro");
			
			array_push($arraySelectCod, "ct2.cdtcod AS executor_cod");
			array_push($arraySelectDsc, "ct2.cdtdsc AS executor_dsc");
			
			array_push($arrayJoin, "LEFT JOIN public.combodadostabela ct2 ON ct2.ctbid = 3 AND ct2.cdtstatus = 'A' AND ct2.cdtcod = substr(sld.plicod, 2, 1)");
			
			array_push($arrayGroupBy, "executor_cod");
			array_push($arrayGroupBy, "executor_dsc");
			
			break;
			
		case "gestor":
			array_push($arrayTituloAgrupadores, "Gestor da Subação");
			
			array_push($arraySelectCod, "ct3.cdtcod AS gestor_cod");
			array_push($arraySelectDsc, "ct3.cdtdsc AS gestor_dsc");
			
			array_push($arrayJoin, "LEFT JOIN public.combodadostabela ct3 ON ct3.ctbid = 4 AND ct3.cdtstatus = 'A' AND ct3.cdtcod = substr(sld.plicod, 3, 1)");
			
			array_push($arrayGroupBy, "gestor_cod");
			array_push($arrayGroupBy, "gestor_dsc");
			
			break;
			
		case "nivel":
			array_push($arrayTituloAgrupadores, "Nível/Etapa de Ensino");
			
			array_push($arraySelectCod, "ct4.cdtcod AS nivel_cod");
			array_push($arraySelectDsc, "ct4.cdtdsc AS nivel_dsc");
			
			array_push($arrayJoin, "LEFT JOIN public.combodadostabela ct4 ON ct4.ctbid = 6 AND ct4.cdtstatus = 'A' AND ct4.cdtcod = substr(sld.plicod, 6, 1)");
			
			array_push($arrayGroupBy, "nivel_cod");
			array_push($arrayGroupBy, "nivel_dsc");
			
			break;
			
		case "apropriacao":
			array_push($arrayTituloAgrupadores, "Categoria de Apropriação");
			
			array_push($arraySelectCod, "ct5.cdtcod AS apropriacao_cod");
			array_push($arraySelectDsc, "ct5.cdtdsc AS apropriacao_dsc");
			
			array_push($arrayJoin, "LEFT JOIN public.combodadostabela ct5 ON ct5.ctbid = 7 AND ct5.cdtstatus = 'A' AND ct5.cdtcod = substr(sld.plicod, 7, 2)");
			
			array_push($arrayGroupBy, "apropriacao_cod");
			array_push($arrayGroupBy, "apropriacao_dsc");
			
			break;
			
		case "modalidade":
			array_push($arrayTituloAgrupadores, "Modalidade de Ensino");
			
			array_push($arraySelectCod, "ct6.cdtcod AS modalidade_cod");
			array_push($arraySelectDsc, "ct6.cdtdsc AS modalidade_dsc");
			
			array_push($arrayJoin, "LEFT JOIN public.combodadostabela ct6 ON ct6.ctbid = 8 AND ct6.cdtstatus = 'A' AND ct6.cdtcod = substr(sld.plicod, 11, 1)");
			
			array_push($arrayGroupBy, "modalidade_cod");
			array_push($arrayGroupBy, "modalidade_dsc");
			
			break;
			
		case "subacao":
			array_push($arrayTituloAgrupadores, "Subação");
			
			array_push($arraySelectCod, "sac.sbacod");
			array_push($arraySelectDsc, "sac.sbatitulo");
			
			array_push($arrayJoin, "INNER JOIN financeiro.subacao sac ON sac.sbastatus = 'A' AND sac.sbacod = substr(sld.plicod, 2, 4)");
			
			array_push($arrayGroupBy, "sac.sbacod");
			array_push($arrayGroupBy, "sac.sbatitulo");
			
			break;
			
		case "orgaougexecutora":
				
			array_push($arrayTituloAgrupadores, "Orgão da UG Executora");
				
			array_push($arraySelectCod, "ogu.codigo");
			array_push($arraySelectDsc, "ogu.descricao");
				
			array_push($arrayJoin, "LEFT JOIN dw.orgaoug ogu ON ogu.codigo = sld.orgcodug");
				
			array_push($arrayGroupBy, "ogu.codigo");
			array_push($arrayGroupBy, "ogu.descricao");
			
			break;
			
		case "gestaoexecutora":
			
			array_push($arrayTituloAgrupadores, "Gestão Executora");
				
			array_push($arraySelectCod, "gse.gstcod");
			array_push($arraySelectDsc, "gse.gstdsc");
				
			array_push($arrayJoin, "LEFT JOIN dw.gestao gse ON gse.gstcod = sld.gescod");
				
			array_push($arrayGroupBy, "gse.gstcod");
			array_push($arrayGroupBy, "gse.gstdsc");
				
			break;
			
		case 'fonterecurso':
			
			array_push($arrayTituloAgrupadores, "Fonte de Recurso");
				
			array_push($arraySelectCod, "ftr.codigo");
			array_push($arraySelectDsc, "ftr.descricao");
			
			array_push($arrayJoin, "LEFT JOIN dw.fonterecursos ftr ON ftr.codigo = substr(sld.foncod,3,2)");
			
			array_push($arrayGroupBy, "ftr.codigo");
			array_push($arrayGroupBy, "ftr.descricao");
			
			break;	
		
		case 'iduso':
			
			array_push($arrayTituloAgrupadores, "IDUSO");
				
			array_push($arraySelectCod, "idf.iducod");
			array_push($arraySelectDsc, "idf.idudsc");
			
			array_push($arrayJoin, "LEFT JOIN dw.identifuso idf ON idf.iducod = substr(sld.foncod,1,1)");
			
			array_push($arrayGroupBy, "idf.iducod");
			array_push($arrayGroupBy, "idf.idudsc");
			
			break;
	}
}

/************************************************************/
/*** INï¿½CIO - Monta a clï¿½usula WHERE ï¿½ partir dos filtros ***/
/************************************************************/

/*** [ï¿½rgï¿½o] ***/
if($_REQUEST["orgao"][0] != "") {
	if($_REQUEST["orgao_campo_excludente"] != "1")
		array_push($arrayWhere, "substr(sld.orgcod, 1, 2) in (substr('".implode("', 1, 2), substr('", $_REQUEST["orgao"])."', 1, 2))");
	else
		array_push($arrayWhere, "substr(sld.orgcod, 1, 2) not in (substr('".implode("', 1, 2), substr('", $_REQUEST["orgao"])."', 1, 2))");
}
/*** [/ï¿½rgï¿½o] ***/

/*** [ï¿½rgï¿½o da UG Executora] ***/
if($_REQUEST["orgaougexecutora"][0] != "") {
	if($_REQUEST["orgaougexecutora_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.orgcodug in ('".implode("','", $_REQUEST["orgaougexecutora"])."')");
	else
		array_push($arrayWhere, "sld.orgcodug not in ('".implode("','", $_REQUEST["orgaougexecutora"])."')");
}
/*** [/ï¿½rgï¿½o da UG Executora] ***/

/*** [Gestï¿½o executora] ***/
if($_REQUEST["gestaoexecutora"][0] != "") {
	if($_REQUEST["gestaoexecutora_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.gescod in ('".implode("','", $_REQUEST["gestaoexecutora"])."')");
	else
		array_push($arrayWhere, "sld.gescod not in ('".implode("','", $_REQUEST["gestaoexecutora"])."')");
}
/*** [/ï¿½rgï¿½o da UG Executora] ***/


/*** [Unidades Gestoras] ***/
if($_REQUEST["ug"][0] != "") {
	if($_REQUEST["ug_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.ungcod in ('".implode("','", $_REQUEST["ug"])."')");
	else
		array_push($arrayWhere, "sld.ungcod not in ('".implode("','", $_REQUEST["ug"])."')");
}
/*** [/Unidades Gestoras] ***/

/*** [Unidades Orï¿½amentï¿½rias do perfil "Consulta Unidade"] ***/
if($unicods_CONSULTAUNIDADE) {
	array_push($arrayWhere, "sld.unicod in ('".implode("','", $unicods_CONSULTAUNIDADE)."')");
}
/*** [/Unidades Orï¿½amentï¿½rias do perfil "Consulta Unidade"] ***/

/*** [Unidades Orï¿½amentï¿½rias] ***/
if($_REQUEST["uo"][0] != "") {
	if($_REQUEST["uo_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.unicod in ('".implode("','", $_REQUEST["uo"])."')");
	else
		array_push($arrayWhere, "sld.unicod not in ('".implode("','", $_REQUEST["uo"])."')");
}
/*** [/Unidades Orï¿½amentï¿½rias] ***/

/*** [Grupo UO] ***/
if($_REQUEST["grupouo"][0] != "") {
	if($_REQUEST["grupouo_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.unicod in (select unicod from dw.uguo where guoid in (".implode(",", $_REQUEST["grupouo"])."))");
	else
		array_push($arrayWhere, "sld.unicod not in (select unicod from dw.uguo where guoid in (".implode(",", $_REQUEST["grupouo"])."))");
}
/*** [/Grupo UO] ***/

/*** [Esfera] ***/
if($_REQUEST["esfera"][0] != "") {
	if($_REQUEST["esfera_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.esfcod in ('".implode("','", $_REQUEST["esfera"])."')");
	else
		array_push($arrayWhere, "sld.esfcod not in ('".implode("','", $_REQUEST["esfera"])."')");
}
/*** [/Esfera] ***/


/*** [Funï¿½ï¿½o] ***/
if($_REQUEST["funcao"][0] != "") {
	if($_REQUEST["funcao_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.funcod in ('".implode("','", $_REQUEST["funcao"])."')");
	else
		array_push($arrayWhere, "sld.funcod not in ('".implode("','", $_REQUEST["funcao"])."')");
}
/*** [/Funï¿½ï¿½o] ***/


/*** [Sub-Funï¿½ï¿½o] ***/
if($_REQUEST["subfuncao"][0] != "") {
	if($_REQUEST["subfuncao_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.sfucod in ('".implode("','", $_REQUEST["subfuncao"])."')");
	else
		array_push($arrayWhere, "sld.sfucod not in ('".implode("','", $_REQUEST["subfuncao"])."')");
}
/*** [/Sub-Funï¿½ï¿½o] ***/


/*** [Programa] ***/
if($_REQUEST["programa"][0] != "") {
	if($_REQUEST["programa_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.prgcod in ('".implode("','", $_REQUEST["programa"])."')");
	else
		array_push($arrayWhere, "sld.prgcod not in ('".implode("','", $_REQUEST["programa"])."')");
	
	$arrayAuxiliar = array();
}
/*** [/Programa] ***/


/*** [Aï¿½ï¿½o] ***/
if($_REQUEST["acacod"][0] != "") {
	if($_REQUEST["acacod_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.acacod in ('".implode("','", $_REQUEST["acacod"])."')");
	else
		array_push($arrayWhere, "sld.acacod not in ('".implode("','", $_REQUEST["acacod"])."')");
}
/*** [/Aï¿½ï¿½o] ***/


/*** [Ptres] ***/
if($_REQUEST["ptres"][0] != "") {
	if($_REQUEST["ptres_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.ptres in ('".implode("','", $_REQUEST["ptres"])."')");
	else
		array_push($arrayWhere, "sld.ptres not in ('".implode("','", $_REQUEST["ptres"])."')");
}
/*** [/Ptres] ***/


/*** [Plano Interno] ***/
if($_REQUEST["planointerno"][0] != "") {
	if($_REQUEST["planointerno_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.plicod in ('".implode("','", $_REQUEST["planointerno"])."')");
	else
		array_push($arrayWhere, "sld.plicod not in ('".implode("','", $_REQUEST["planointerno"])."')");
}
/*** [/Plano Interno] ***/


/*** [Grupo Fonte] ***/
if($_REQUEST["grf"][0] != "") {
	if($_REQUEST["grf_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.foncod in (select foscod from dw.fontesiafi where grfid in (".implode(",", $_REQUEST["grf"])."))");
	else
		array_push($arrayWhere, "sld.foncod not in (select foscod from dw.fontesiafi where grfid in (".implode(",", $_REQUEST["grf"])."))");
}
/*** [/Grupo Fonte] ***/


/*** [Fonte SOF] ***/
if($_REQUEST["fonte"][0] != "") {
	if($_REQUEST["fonte_campo_excludente"] != "1")
		array_push($arrayWhere, "substr(sld.foncod, 2, 3) in ('".implode("','", $_REQUEST["fonte"])."')");
	else
		array_push($arrayWhere, "substr(sld.foncod, 2, 3) not in ('".implode("','", $_REQUEST["fonte"])."')");
}
/*** [/Fonte SOF] ***/


/*** [Fonte de Recurso] ***/
if($_REQUEST["fonterecurso"][0] != "") {
	if($_REQUEST["fonterecurso_campo_excludente"] != "1")
		array_push($arrayWhere, "substr(sld.foncod, 3, 2) in ('".implode("','", $_REQUEST["fonterecurso"])."')");
	else
		array_push($arrayWhere, "substr(sld.foncod, 3, 2) not in ('".implode("','", $_REQUEST["fonterecurso"])."')");
}
/*** [/Fonte de Recurso] ***/


/*** [Categoria Econï¿½mica] ***/
if($_REQUEST["catecon"][0] != "") {
	if($_REQUEST["catecon_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.ctecod in ('".implode("','", $_REQUEST["catecon"])."')");
	else
		array_push($arrayWhere, "sld.ctecod not in ('".implode("','", $_REQUEST["catecon"])."')");
}
/*** [/Categoria Econï¿½mica] ***/


/*** [GND] ***/
if($_REQUEST["gnd"][0] != "") {
	if($_REQUEST["gnd_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.gndcod in ('".implode("','", $_REQUEST["gnd"])."')");
	else
		array_push($arrayWhere, "sld.gndcod not in ('".implode("','", $_REQUEST["gnd"])."')");
}
/*** [/GND] ***/


/*** [Modalidade de Aplicaï¿½ï¿½o] ***/
if($_REQUEST["mapcod"][0] != "") {
	if($_REQUEST["mapcod_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.mapcod in ('".implode("','", $_REQUEST["mapcod"])."')");
	else
		array_push($arrayWhere, "sld.mapcod not in ('".implode("','", $_REQUEST["mapcod"])."')");
}
/*** [/Modalidade de Aplicaï¿½ï¿½o] ***/


/*** [Elemento de Despesa] ***/
if($_REQUEST["elemento"][0] != "") {
	if($_REQUEST["elemento_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.edpcod in ('".implode("','", $_REQUEST["elemento"])."')");
	else
		array_push($arrayWhere, "sld.edpcod not in ('".implode("','", $_REQUEST["elemento"])."')");
}
/*** [/Elemento de Despesa] ***/


/*** [Natureza de Despesa] ***/
if($_REQUEST["natureza"][0] != "") {
	if($_REQUEST["natureza_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.ctecod || sld.gndcod || sld.mapcod || sld.edpcod in ('".implode("','", $_REQUEST["natureza"])."')");
	else
		array_push($arrayWhere, "sld.ctecod || sld.gndcod || sld.mapcod || sld.edpcod not in ('".implode("','", $_REQUEST["natureza"])."')");
}
/*** [/Natureza de Despesa] ***/


/*** [IDUSO] ***/
if($_REQUEST["iduso"][0] != "") {
	if($_REQUEST["iduso_campo_excludente"] != "1")
		array_push($arrayWhere, "substr(sld.foncod,1,1) in ('".implode("','", $_REQUEST["iduso"])."')");
	else
		array_push($arrayWhere, "substr(sld.foncod,1,1) not in ('".implode("','", $_REQUEST["iduso"])."')");
}
/*** [/IDUSO] ***/


/*** [Fonte Detalhada] ***/
if($_REQUEST["fontesiafi"][0] != "") {
	if($_REQUEST["fontesiafi_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.foncod in ('".implode("','", $_REQUEST["fontesiafi"])."')");
	else
		array_push($arrayWhere, "sld.foncod not in ('".implode("','", $_REQUEST["fontesiafi"])."')");
}
/*** [/Fonte Detalhada] ***/


/*** [Conta Corrente] ***/
if($_REQUEST["sldcontacorrente"][0] != "") {
	array_push($arrayWhere, "trim(sld.sldcontacorrente) in ('".implode("','", $_REQUEST["sldcontacorrente"])."')");
}
/*** [/Conta Corrente] ***/


/*** [Recurso] ***/
if($_REQUEST["recurso"][0] != "") {
	if($_REQUEST["recurso_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.trrcod in ('".implode("','", $_REQUEST["recurso"])."')");
	else
		array_push($arrayWhere, "sld.trrcod not in ('".implode("','", $_REQUEST["recurso"])."')");
}
/*** [/Recurso] ***/


/*** [Vinculaï¿½ï¿½o de Pagamento] ***/
if($_REQUEST["vincod"][0] != "") {
	if($_REQUEST["vincod_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.vincod in ('".implode("','", $_REQUEST["vincod"])."')");
	else
		array_push($arrayWhere, "sld.vincod not in ('".implode("','", $_REQUEST["vincod"])."')");
}
/*** [/Vinculaï¿½ï¿½o de Pagamento] ***/


/*** [Categoria de Gasto] ***/
if($_REQUEST["cagcod"][0] != "") {
	if($_REQUEST["cagcod_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.cagcod in ('".implode("','", $_REQUEST["cagcod"])."')");
	else
		array_push($arrayWhere, "sld.cagcod not in ('".implode("','", $_REQUEST["cagcod"])."')");
}
/*** [/Categoria de Gasto] ***/


/*** [Subtï¿½tulo] ***/
if($_REQUEST["loccod"][0] != "") {
	if($_REQUEST["loccod_campo_excludente"] != "1")
		array_push($arrayWhere, "sld.loccod in ('".implode("','", $_REQUEST["loccod"])."')");
	else
		array_push($arrayWhere, "sld.loccod not in ('".implode("','", $_REQUEST["loccod"])."')");
}
/*** [/Subtï¿½tulo] ***/


/*** [Enquadramento da Despesa] ***/
if($_REQUEST["enquadramento"][0] != "") {
	if($_REQUEST["enquadramento_campo_excludente"] != "1")
		array_push($arrayWhere, "substr(sld.plicod, 1, 1) in ('".implode("','", $_REQUEST["enquadramento"])."')");
	else
		array_push($arrayWhere, "substr(sld.plicod, 1, 1) not in ('".implode("','", $_REQUEST["enquadramento"])."')");
}
/*** [/Enquadramento da Despesa] ***/


/*** [Executor Orï¿½amentï¿½rio e Financeiro] ***/
if($_REQUEST["executor"][0] != "") {
	if($_REQUEST["executor_campo_excludente"] != "1")
		array_push($arrayWhere, "substr(sld.plicod, 2, 1) in ('".implode("','", $_REQUEST["executor"])."')");
	else
		array_push($arrayWhere, "substr(sld.plicod, 2, 1) not in ('".implode("','", $_REQUEST["executor"])."')");
}
/*** [/Executor Orï¿½amentï¿½rio e Financeiro] ***/


/*** [Gestor da Subaï¿½ï¿½o] ***/
if($_REQUEST["gestor"][0] != "") {
	if($_REQUEST["gestor_campo_excludente"] != "1")
		array_push($arrayWhere, "substr(sld.plicod, 3, 1) in ('".implode("','", $_REQUEST["gestor"])."')");
	else
		array_push($arrayWhere, "substr(sld.plicod, 3, 1) not in ('".implode("','", $_REQUEST["gestor"])."')");
}
/*** [/Gestor da Subaï¿½ï¿½o] ***/


/*** [Nï¿½vel/Etapa de Ensino] ***/
if($_REQUEST["nivel"][0] != "") {
	if($_REQUEST["nivel_campo_excludente"] != "1")
		array_push($arrayWhere, "substr(sld.plicod, 6, 1) in ('".implode("','", $_REQUEST["nivel"])."')");
	else
		array_push($arrayWhere, "substr(sld.plicod, 6, 1) not in ('".implode("','", $_REQUEST["nivel"])."')");
}
/*** [/Nï¿½vel/Etapa de Ensino] ***/


/*** [Categoria de Apropriaï¿½ï¿½o] ***/
if($_REQUEST["apropriacao"][0] != "") {
	if($_REQUEST["apropriacao_campo_excludente"] != "1")
		array_push($arrayWhere, "substr(sld.plicod, 7, 2) in ('".implode("','", $_REQUEST["apropriacao"])."')");
	else
		array_push($arrayWhere, "substr(sld.plicod, 7, 2) not in ('".implode("','", $_REQUEST["apropriacao"])."')");
}
/*** [/Categoria de Apropriaï¿½ï¿½o] ***/


/*** [Modalidade de Ensino] ***/
if($_REQUEST["modalidade"][0] != "") {
	if($_REQUEST["modalidade_campo_excludente"] != "1")
		array_push($arrayWhere, "substr(sld.plicod, 11, 1) in ('".implode("','", $_REQUEST["modalidade"])."')");
	else
		array_push($arrayWhere, "substr(sld.plicod, 11, 1) not in ('".implode("','", $_REQUEST["modalidade"])."')");
}
/*** [/Modalidade de Ensino] ***/


/*** [Subaï¿½ï¿½o] ***/
if($_REQUEST["subacao"][0] != "") {
	if($_REQUEST["subacao_campo_excludente"] != "1")
		array_push($arrayWhere, "substr(sld.plicod, 2, 4) in ('".implode("','", $_REQUEST["subacao"])."')");
	else
		array_push($arrayWhere, "substr(sld.plicod, 2, 4) not in ('".implode("','", $_REQUEST["subacao"])."')");
}
/*** [/Subaï¿½ï¿½o] ***/

/*********************************************************/
/*** FIM - Monta a clï¿½usula WHERE ï¿½ partir dos filtros ***/
/*********************************************************/

$arraySum = array();
$arraySumWhere = array();
$inContasTodos = array();

for($i=0; $i<count($_REQUEST["agrupadorColunas"]); $i++) {
	$inContas = array();
	
	$arrayContas = $db2->carregar("SELECT conconta FROM financeiro.informacaoconta WHERE icbcod = ".$_REQUEST["agrupadorColunas"][$i]);
	
	for($j=0; $j<count($arrayContas); $j++) {
		array_push($inContas, "'".$arrayContas[$j]["conconta"]."'");
		array_push($inContasTodos, "'".$arrayContas[$j]["conconta"]."'");
	}
	
	$case = "CASE WHEN sld.sldcontacontabil in (".implode(',', $inContas).") THEN sld.sldvalor ELSE 0 END AS valor".($i+1);
	array_push($arrayCase, $case);
	array_push($arraySum, 'sum(valor'.($i+1).') AS coluna'.($i+1));
	array_push($arraySumWhere, 'valor'.($i+1).' <> 0');
}

array_push($arrayWhere, "sld.sldcontacontabil in (".implode(',', $inContasTodos).")");

/************************************************************************
 * Criaï¿½ï¿½o das Sessï¿½es para utilizar com o Agrupador Ajax
 ************************************************************************/
$_SESSION['arrayWhere'] = $arrayWhere;
$_SESSION['arrayCase']  = $arrayCase;
$_SESSION['arrayJoin']  = $arrayJoin;
$_SESSION['arrayGroupBy']  = $arrayGroupBy;

/*** Variï¿½vel $where vai conter todos os filtros incluï¿½dos ***/
if(!empty($arrayWhere))
	$where = "WHERE ".implode(" AND ",$arrayWhere);
	
	
$tituloAgrupadores = implode("/", $arrayTituloAgrupadores);
$case = implode(",", $arrayCase);
$join = implode(" ", $arrayJoin);
$groupBy = "GROUP BY " . implode(",", $arrayGroupBy) . ",sldcontacontabil, sld.sldvalor ";

$arrayUnionSelects = array();
$arrayAuxCod = array();
$arrayAuxDsc = array();

for($i=0; $i<count($_REQUEST["agrupador"]); $i++) {
	$arrayAuxCod = $arraySelectCod;
	$arrayAuxDsc = $arraySelectDsc;
	
	$cont = (count($arrayAuxCod) - 1);
	
	for($k=1; $k<=$i; $k++) {
		$arrayAuxCod[$cont] = 'null';
		$arrayAuxDsc[$cont] = 'null';
		
		$cont--;
	}
	
	$selectCod = implode(",", $arrayAuxCod);
	$selectDsc = implode(",", $arrayAuxDsc);
	
	$sqlInterno = "SELECT 
				   ".$selectCod.",
				   ".$selectDsc.",
				   ".$case."
			       FROM
				   dw.saldo sld
				   ".$join."
				   ".$where;
				   //".$groupBy;
	
	array_push($arrayUnionSelects, $sqlInterno);
}

/************************************************************************
 * Criaï¿½ï¿½o das Sessï¿½es para utilizar com o Agrupador Ajax
 ************************************************************************/
$_SESSION['arrayUnionSelects'] = $arrayUnionSelects;

$unionSelects = implode(" UNION ALL	", $arrayUnionSelects);

$sum =implode(",", $arraySum);
$sumWhere = implode(" OR ", $arraySumWhere);


for($i=0; $i<count($arraySelectCod); $i++) {
	if($arraySelectCod[$i] == "sld.ptres AS ptres") {
		$arraySelectCod[$i] = "ptres";
		$arraySelectDsc[$i] = "ptres_desc";
	}
	else if($arraySelectCod[$i] == "sld.prgcod || '.' || sld.acacod || '.' || sld.unicod || '.' || sld.loccod || ' - ' || aca2.acadsc || '(' || sld.loccod || ')' AS funcional") {
		$arraySelectCod[$i] = "funcional";
		$arraySelectDsc[$i] = "funcional_desc";
	}
	else if($arraySelectCod[$i] == "sld.ctecod || '.' || sld.gndcod || '.' || sld.mapcod || '.' || sld.edpcod AS natureza") {
		$arraySelectCod[$i] = "natureza";
		$arraySelectDsc[$i] = "natureza_desc";
	}
	else if($arraySelectCod[$i] == "sld.sldcontacorrente AS conta_corrente") {
		$arraySelectCod[$i] = "conta_corrente";
		$arraySelectDsc[$i] = "conta_corrente_desc";
	}
	else if($arraySelectCod[$i] == "sld.vincod AS vincod") {
		$arraySelectCod[$i] = "vincod";
		$arraySelectDsc[$i] = "vincod_desc";
	}
	else if($arraySelectCod[$i] == "sld.loccod AS loccod") {
		$arraySelectCod[$i] = "loccod";
		$arraySelectDsc[$i] = "loccod_desc";
	}
	else if($arraySelectCod[$i] == "ct1.cdtcod AS enquadramento_cod") {
		$arraySelectCod[$i] = "enquadramento_cod";
		$arraySelectDsc[$i] = "enquadramento_dsc";
	}
	else if($arraySelectCod[$i] == "ct2.cdtcod AS executor_cod") {
		$arraySelectCod[$i] = "executor_cod";
		$arraySelectDsc[$i] = "executor_dsc";
	}
	else if($arraySelectCod[$i] == "ct3.cdtcod AS gestor_cod") {
		$arraySelectCod[$i] = "gestor_cod";
		$arraySelectDsc[$i] = "gestor_dsc";
	}
	else if($arraySelectCod[$i] == "ct4.cdtcod AS nivel_cod") {
		$arraySelectCod[$i] = "nivel_cod";
		$arraySelectDsc[$i] = "nivel_dsc";
	}
	else if($arraySelectCod[$i] == "ct5.cdtcod AS apropriacao_cod") {
		$arraySelectCod[$i] = "apropriacao_cod";
		$arraySelectDsc[$i] = "apropriacao_dsc";
	}
	else if($arraySelectCod[$i] == "ct6.cdtcod AS modalidade_cod") {
		$arraySelectCod[$i] = "modalidade_cod";
		$arraySelectDsc[$i] = "modalidade_dsc";
	}
	else {
		$arraySelectCod[$i] = substr($arraySelectCod[$i], 4);
		$arraySelectDsc[$i] = substr($arraySelectDsc[$i], 4);
	}
}

/************************************************************************
 * Criaï¿½ï¿½o das Sessï¿½es para utilizar com o Agrupador Ajax
 ************************************************************************/
$_SESSION['arraySelectCod'] = $arraySelectCod;
$_SESSION['arraySelectDsc']  = $arraySelectDsc;

$orderByCodExterno = $groupByCodExterno = implode(",", $arraySelectCod);
$orderByDscExterno = $groupByDscExterno = implode(",", $arraySelectDsc);

for($i=0; $i<count($arraySelectCod); $i++) {
	if($arraySelectCod[$i] == 'ptres' || $arraySelectCod[$i] == 'funcional' || $arraySelectCod[$i] == 'conta_corrente' || $arraySelectCod[$i] == 'vincod' || $arraySelectCod[$i] == 'loccod') {
		$arraySelectCod[$i] .= " AS cod_agrupador".($i+1);
		$arraySelectDsc[$i] = " '' AS dsc_agrupador".($i+1);
	} else {
		$arraySelectCod[$i] .= " AS cod_agrupador".($i+1);
		//$arraySelectCod[$i] = "coalesce(".$arraySelectCod[$i].", 'Nï¿½o Aplicï¿½vel') AS cod_agrupador".($i+1);
		$arraySelectDsc[$i] .= " AS dsc_agrupador".($i+1);
		//$arraySelectDsc[$i] = "coalesce(".$arraySelectDsc[$i].", 'Nï¿½o Aplicï¿½vel') AS dsc_agrupador".($i+1);
	}
}

$selectCodExterno = implode(",", $arraySelectCod);
$selectDscExterno = implode(",", $arraySelectDsc);

$sqlCompleto = "SELECT
				".$selectCodExterno.",
				".$selectDscExterno.",
				".$sum."
				FROM
				(".$unionSelects.") as foo
				WHERE
				".$sumWhere."
				GROUP BY
				".$groupByCodExterno.",
				".$groupByDscExterno."
				ORDER BY
				".$orderByCodExterno.",
				".$orderByDscExterno;

$db2->executar("INSERT INTO financeiro.logrel (lgrdata, lgrdsc, usucpf) VALUES (now(), '".pg_escape_string($sqlCompleto)."', '".$_SESSION["usucpf"]."')");
$db2->commit();


//dbg($sqlCompleto);
$resultado = $db2->carregar($sqlCompleto);

if(count($_REQUEST["agrupadorColunas"])%2){
	$mul = 3;
}else{
	$mul = 2.5;
}

$larguraRel = 10 + (count($_REQUEST["agrupadorColunas"]) * $mul);
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>Acompanhamento da Execução Orçamentária e Financeiro</title>
		<style type="text/css">
			
			@media print {.notprint { display: none }}
			
			@media screen {
			.notscreen { display: none;  }
			.div_rolagem{ overflow-x: auto; overflow-y: auto; width:<?=$larguraRel?>.5cm;height:450px;}
			.topo { position: absolute; top: 0px; margin: 0; padding: 5px; position: fixed; background-color: #ffffff;}
			}
			
			*{margin:0; padding:0; border:none; font-size:10px;font-family:Arial;}
			.alignRight{text-align:right !important;width:100px;}
			.alignRightTit{text-align:right !important;padding:4px;}
			.colunaValor{width:80px;text-align:right;border-right: 1px solid #CCCCCC;padding: 2px 3px 2px 3px;}
			.alignCenter{ text-align:center !important;}
			.alignLeft{text-align:left !important;}
			.bold{font-weight:bold !important;}
			.italic{font-style:italic !important;}
			.noPadding{padding:0;}
			.titulo{width:52px;}
			.tituloagrup{font-size:9px;}
			.titulolinha{font-size:9px;}
			
			#tabelaTitulos th{border:2px solid black;}
			
			#orgao{margin:3px 0 0 0;}
			#orgao tr td{border:1px solid black;border-left:none;border-right:none;font-size:11px;}
			
			div.filtro { page-break-after: always; text-align: center; }
			
			table{width:<?=$larguraRel?>cm !important;border-collapse:saparate;}
			th, td{font-weight:normal;vertical-align:top;}
			thead{display:table-header-group;}
			table, tr{page-break-inside:avoid;}
			a{text-decoration:none;color:#3030aa;}
			a:hover{text-decoration:underline;color:#aa3030;}
			span.topo { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.topo:hover { background-color: #d0d0d0; }
			
			#formulario select {width:220px}
			#formulario table {width:100% !important;}
			#formulario th, td {font-weight:normal;}
			#formulario thead {display:table-header-group;}
			#formulario table, tr {page-break-inside:avoid;}
			
			#formulario1 select {width:220px}
			#formulario1 table {width:100% !important;}
			#formulario1 th, td {font-weight:normal;}
			#formulario1 thead {display:table-header-group;}
			#formulario1 table, tr {page-break-inside:avoid;}
			
			.exibe_simples {width:300px;padding: 5px;margin-left:50px;position:absolute;border:solid 2px #000000;background-color:#cccccc;text-align:left}
			.exibe_avancado {width:550px;padding: 5px;margin-left:50px;position:absolute;border:solid 2px #000000;background-color:#cccccc;text-align:left}
			
			.aba_s {background-color:#fcfcfc;border:solid 1px;color:#990000;margin-left:3px;margin-bottom:2px;cursor:pointer;padding:2px;float:left}
			.aba {background-color:#fcfcfc;border:solid 1px;margin-bottom:2px;cursor:pointer;margin-left:3px;padding:2px;float:left}
		
			

		</style>
		<!-- 
			border-collapse:collapse;
			#tabelaTitulos tr td, #tabelaTitulos tr th{border:2px solid black;border-left:none; border-right:none;}			
		 -->		 
	</head>
	<body>
	<?php 
	
	$hidden_agrupadorColunas = implode(";",$_REQUEST["agrupadorColunas"]);
	$hidden_agrupadores = implode(";",$_REQUEST["agrupador"]);
	
	?>
	<input type="hidden" id="hidden_agrupadorColunas" name="hidden_agrupadorColunas" value="<?php echo $hidden_agrupadorColunas ;?>" />
	<input type="hidden" id="hidden_agrupadores" name="hidden_agrupadores" value="<?php echo $hidden_agrupadores ;?>" />
	<input type="hidden" id="hidden_agrupador_corrente" name="hidden_agrupador_corrente" value="" />
	<input type="hidden" id="hidden_valor_agrupador_corrente" name="hidden_valor_agrupador_corrente" value="" />

		<div id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300px;">
			<center style="font-size:12px;"><br><img src="../imagens/wait.gif" border="0" align="absmiddle"> Aguarde! Gerando Relatï¿½rio...<br><br></center>
		</div>
		<script type="text/javascript">
			self.focus();
		</script>
		<div id="filtros" class="notscreen filtro">
			<b><font style="font-size:12px;">Filtros</font></b>
<?
	$dados = array('orgao' => array('descricao'=>'Orgï¿½o','campocod'=>'orscod','campodsc'=>"orscod || ' - ' || orsdsc",'tabela'=>'dw.orgaosuperior'),
				   'ug' => array('descricao'=>'Unidades Gestoras','campocod'=>'ungcod','campodsc'=>"ungcod || ' - ' || ungdsc",'tabela'=>'dw.ug'),
				   'uo' => array('descricao'=>'Unidades Orçamentárias','campocod'=>'unicod','campodsc'=>"unicod || ' - ' || unidsc",'tabela'=>'dw.uo'),
				   'grupouo' => array('descricao'=>'Grupo UO','campocod'=>'guoid','campodsc'=>"guoid || ' - ' || guodsc",'tabela'=>'dw.grupouo'),
				   'funcao' => array('descricao'=>'Função','campocod'=>'funcod','campodsc'=>"funcod || ' - ' || fundsc",'tabela'=>'dw.funcao'),
				   'subfuncao' => array('descricao'=>'Sub-Função','campocod'=>'sfucod','campodsc'=>"sfucod || ' - ' || sfudsc",'tabela'=>'dw.subfuncao'),
				   'programa' => array('descricao'=>'Programa','campocod'=>'prgcod','campodsc'=>"prgcod || ' - ' || prgdsc",'tabela'=>'dw.programa'),
				   'acacod' => array('descricao'=>'Ação','campocod'=>'acacod','campodsc'=>"acacod || ' - ' || acadsc",'tabela'=>'dw.acao'),
				   'ptres' => array('descricao'=>'PTRES','campocod'=>'pt.ptres','campodsc'=>'pt.ptres','tabela'=>'dw.ptres'),
				   'planointerno' => array('descricao'=>'Plano Interno','campocod'=>'plicod','campodsc'=>"plicod || ' - ' || plidsc",'tabela'=>'dw.planointerno'),
				   'grf' => array('descricao'=>'Grupo Fonte','campocod'=>'grfid','campodsc'=>"grfid || ' - ' || grfdsc",'tabela'=>'dw.grupofonte'),
				   'fonte' => array('descricao'=>'Fonte de Recurso','campocod'=>'foncod','campodsc'=>"foncod || ' - ' || fondsc",'tabela'=>'dw.fonterecurso'),
  				   'catecon' => array('descricao'=>'Categoria Econï¿½mica','campocod'=>'ctecod','campodsc'=>"ctecod || ' - ' || ctedsc",'tabela'=>'dw.categoriaeconomica'),
				   'gnd' => array('descricao'=>'GND','campocod'=>'gndcod','campodsc'=>"gndcod || ' - ' || gnddsc",'tabela'=>'dw.gnd'),
				   'mapcod' => array('descricao'=>'Modalidade de Aplicaï¿½ï¿½o','campocod'=>'mapcod','campodsc'=>"mapcod || ' - ' || mapdsc",'tabela'=>'dw.modalidadeaplicacao'),
				   'elemento' => array('descricao'=>'Elemento de Despesa','campocod'=>'edpcod','campodsc'=>"edpcod || ' - ' || edpdsc",'tabela'=>'dw.elementodespesa'),
				   'natureza' => array('descricao'=>'Natureza de Despesa','campocod'=>'cast(ctecod AS varchar) || cast(gndcod AS varchar) || mapcod || edpcod','campodsc'=>"ctecod || '.' || gndcod || '.' || mapcod || '.' || edpcod || ' - ' || ndpdsc",'tabela'=>'dw.naturezadespesa'),
				   'fontesiafi' => array('descricao'=>'Fonte Detalhada','campocod'=>'foscod','campodsc'=>'foscod','tabela'=>'dw.fontesiafi'),
				   'recurso' => array('descricao'=>'Recurso','campocod'=>'trrcod','campodsc'=>"trrcod || ' - ' || trrdsc",'tabela'=>'dw.tiporecurso'),
				   'vincod' => array('descricao'=>'Vinculação de Pagamento','campocod'=>'vincod','campodsc'=>'vincod','tabela'=>'dw.saldo'),
				   'cagcod' => array('descricao'=>'Categoria de Gasto','campocod'=>'cagcod','campodsc'=>"cagcod || ' - ' || cagdsc",'tabela'=>'dw.categoriagasto'),
				   'loccod' => array('descricao'=>'Subtítulo','campocod'=>'loccod','campodsc'=>'loccod','tabela'=>'dw.saldo'),
				   'enquadramento' => array('descricao'=>'Enquadramento da Despesa','campocod'=>'cdtcod','campodsc'=>"cdtcod || ' - ' || cdtdsc",'tabela'=>'public.combodadostabela'),
				   'executor' => array('descricao'=>'Executor Orï¿½amentï¿½rio e Financeiro','campocod'=>'cdtcod','campodsc'=>"cdtcod || ' - ' || cdtdsc",'tabela'=>'public.combodadostabela'),
				   'gestor' => array('descricao'=>'Gestor da Subaï¿½ï¿½o','campocod'=>'cdtcod','campodsc'=>"cdtcod || ' - ' || cdtdsc",'tabela'=>'public.combodadostabela'),
				   'nivel' => array('descricao'=>'Nível/Etapa de Ensino','campocod'=>'cdtcod','campodsc'=>"cdtcod || ' - ' || cdtdsc",'tabela'=>'public.combodadostabela'),
  				   'apropriacao' => array('descricao'=>'Categoria de Apropriaï¿½ï¿½o','campocod'=>'cdtcod','campodsc'=>"cdtcod || ' - ' || cdtdsc",'tabela'=>'public.combodadostabela'),
				   'modalidade' => array('descricao'=>'Modalidade de Ensino','campocod'=>'cdtcod','campodsc'=>"cdtcod || ' - ' || cdtdsc",'tabela'=>'public.combodadostabela'));
									  												   		
	function mostraFiltro($campo) {
			global $db2, $dados;
			
			$sql = "SELECT ".$dados[$campo]['campocod']." as codigo, ".$dados[$campo]['campodsc']." as descricao 
					FROM ".$dados[$campo]['tabela']." 
					WHERE ".$dados[$campo]['campocod']." IN('".implode("','",$_REQUEST[$campo])."')";
			$info = $db2->carregar($sql);
			echo "<br><b>".$dados[$campo]['descricao']."</b><br>";
			if($info) {
				foreach($info as $in) {
					echo $in['descricao']."<br>";
				}
			}
	}
	
	foreach(array_keys($dados) as $campo) {
		if($_REQUEST[$campo][0])mostraFiltro($campo);
	}
	
?>
		</div>
		<table>
			<thead>
				<tr>
					<th class="noPadding" align="left">
						<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
							<tr bgcolor="#ffffff">
								<td valign="top" width="50" rowspan="2"></td>
								<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
									<?= $GLOBALS['parametros_sistema_tela']['sigla']; ?><br/>
									<?= $GLOBALS['parametros_sistema_tela']['unidade']; ?><br/>
								</td>
								<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
									Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
									Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?><br />
									Orçamento Fiscal e Seg.Social - EM R$ <?= ($_REQUEST["escala"] == 1) ? "1,00" : "1.000,00" ?><br />
									Acumulado até: <?= formata_data($valorAcumulado) ?>
								</td>
							</tr>
							<tr>
								<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">
									<b><font style="font-size:14px;"><?= $_REQUEST["titulo"] ?></font></b>
								</td>
							</tr>
						</table>
						<table id="tabelaTitulos" align="left" cellspacing="0">
							<thead>
								<tr bgcolor="#DEDEDE">
								<?
								if(isset($_REQUEST["tipo"]) && $_REQUEST["tipo"] == 'xls') {
									require(APPRAIZ . 'includes/export-xls.class.php');
									
									$filename = 'relatorioSIAFI.xls';
									$xls = new ExportXLS($filename);
											
									$header[] = $tituloAgrupadores;
									
									for($i=0; $i<count($_REQUEST["agrupadorColunas"]); $i++) {
										$coluna = $db2->pegaLinha("SELECT icbdsc, icbdscresumida FROM financeiro.informacaocontabil WHERE icbcod = ".$_REQUEST["agrupadorColunas"][$i]);
										
										$header[] = $coluna["icbdscresumida"];
									}
									
									$xls->addHeader($header);
								}
								else {
								?>
									<th class="bold alignLeft" style="border-right: 1px solid #AAAAAA;border-left: 1px solid #FFFFFF;"><?=$tituloAgrupadores?></th>
								<?
										for($i=0; $i<count($_REQUEST["agrupadorColunas"]); $i++) {
											$coluna = $db2->pegaLinha("SELECT icbdsc, icbdscresumida FROM financeiro.informacaocontabil WHERE icbcod = ".$_REQUEST["agrupadorColunas"][$i]);

											echo "<th class=\"alignRightTit \" style=\"padding: 2px 2px 2px 3px;width:80px;border-right: 1px solid #AAAAAA;border-left: 1px solid #ffffff;\" onmouseover=\"return escape('".$coluna["icbdsc"]."');\">".$coluna["icbdscresumida"]."</th>\n";
										}
								}
								?>
								</tr>
							</thead>
						</table>
					</th>
					</tr>
					</thead>
						<tbody>
								<?
									// Se retornou algum valor, executa a lï¿½gica.
									if($resultado) {
										echo '<tr>
												<td class="noPadding" align="left">
													<div class="div_rolagem">';
										
										$totalGeral = array();
										
										// Cria um array que conterï¿½ todos os valores dos 
										// agrupadores na ordem correta.
										$arrValorAgrupadores = array();
										
										// Contador para controlar o array com os valores dos agrupadores.
										$contAgrupador = 0;
										
										// 
										$arrayAuxAgrupadores = array();
										
										// Percorre todos os registros do resultado.
										for($i=0; $i<count($resultado); $i++) {
											// Percorre cada agrupador selecionado.
											// Comeï¿½a em 1, pois o padrï¿½o ï¿½ cod_agrupador1, cod_agrupador2, cod_agrupador3...
											for($j=1; $j<=count($_REQUEST["agrupador"]); $j++) {
												// Alterna a cor das linhas.
												$cor = ($j%2) ? "#f8f8f8" : "";
												
												//Inclui a opï¿½ï¿½o de detalhar o ï¿½ltimo nï¿½vel
												if($j == count($_REQUEST["agrupador"]) && count($_REQUEST["agrupador"]) == 1){ 
													$maisDetalheAjax = " <a id=\"img_detalhe_agrupador_mais_{$_REQUEST["agrupador"][($j-1)]}_{$resultado[$i]["cod_agrupador".$j]}\" title=\"Detalhar\" style=\"cursor:pointer;\" onclick=\"detalheAgrupador('{$resultado[$i]["cod_agrupador".$j]}','{$_REQUEST["agrupador"][($j-1)]}');\" >[+]</a> <a id=\"img_detalhe_agrupador_menos_{$_REQUEST["agrupador"][($j-1)]}_{$resultado[$i]["cod_agrupador".$j]}\" title=\"Fechar Detalhe\" style=\"display:none;cursor:pointer;\" onclick=\"fechaDetalhe('{$resultado[$i]["cod_agrupador".$j]}','{$_REQUEST["agrupador"][($j-1)]}');\" >[ - ]</a> <a id=\"img_detalhe_agrupador_reset_{$_REQUEST["agrupador"][($j-1)]}_{$resultado[$i]["cod_agrupador".$j]}\" title=\"Limpar Detalhe\" style=\"display:none;cursor:pointer;\" onclick=\"resetarDetalhe('{$resultado[$i]["cod_agrupador".$j]}','{$_REQUEST["agrupador"][($j-1)]}');\" >[ x ]</a>";
												}else{
													$maisDetalheAjax = "";
												}
												
												// Se nï¿½o for o primeiro agrupador, coloca a seta para representar a hierarquia.
												$seta = ($j==1) ? $maisDetalheAjax : " <img src='../imagens/seta_filho.gif' align='absmiddle' /> $maisDetalheAjax";
												
												// Testa se ï¿½ o ï¿½ltimo agrupador.
												if($j == count($_REQUEST["agrupador"])) {
													if($resultado[$i]["cod_agrupador".$j] != NULL) {
														$arrValorAgrupadores[$contAgrupador]["codigo"] 		= $resultado[$i]["cod_agrupador".$j];
														$arrValorAgrupadores[$contAgrupador]["descricao"] 	= $resultado[$i]["dsc_agrupador".$j];
														$arrValorAgrupadores[$contAgrupador]["cor"] 		= $cor;
														$arrValorAgrupadores[$contAgrupador]["padding"] 	= (($j-1) * 10);
														$arrValorAgrupadores[$contAgrupador]["seta"] 		= $seta;
														$arrValorAgrupadores[$contAgrupador]["filhos"] 		= 'N';
														$arrValorAgrupadores[$contAgrupador]["nivel"] 		= $j;
														$arrValorAgrupadores[$contAgrupador]["agrupador"]	= $_REQUEST["agrupador"][($j-1)];
														
														for($k=1; $k<=count($_REQUEST["agrupadorColunas"]); $k++) {
															if($resultado[$i]["coluna".$k] != NULL) {
																$valor = number_format($resultado[$i]["coluna".$k], 0, ',', '.' );
																$totalGeral[$k] += $resultado[$i]["coluna".$k];
															}
															else {
																$valor = " - ";
															}
																
															$arrValorAgrupadores[$contAgrupador]["colunas"][$k] = $valor;
														}
														$contAgrupador++;
													}
												}
												// Testa se o prï¿½ximo agrupador tem valor nï¿½o nulo.
												else if($resultado[$i]["cod_agrupador".($j+1)] != NULL) {
													if($arrValorAgrupadores[$arrayAuxAgrupadores[$j]]["codigo"] == $resultado[$i]["cod_agrupador".$j]) {
														continue;
													}
													
													$arrValorAgrupadores[$contAgrupador]["codigo"] 		= $resultado[$i]["cod_agrupador".$j];
													$arrValorAgrupadores[$contAgrupador]["descricao"] 	= $resultado[$i]["dsc_agrupador".$j];
													$arrValorAgrupadores[$contAgrupador]["cor"] 		= $cor;
													$arrValorAgrupadores[$contAgrupador]["padding"] 	= (($j-1) * 10);
													$arrValorAgrupadores[$contAgrupador]["seta"] 		= $seta;
													$arrValorAgrupadores[$contAgrupador]["filhos"] 		= 'S';
													$arrValorAgrupadores[$contAgrupador]["nivel"] 		= $j;
													$arrValorAgrupadores[$contAgrupador]["agrupador"]	= $_REQUEST["agrupador"][($j-1)];
													
													$arrayAuxAgrupadores[$j] = $contAgrupador;
													$contAgrupador++;
												}
												// Se o prï¿½ximo agrupador tiver valor nulo.
												else {
													if($resultado[$i]["cod_agrupador".$j] != NULL) {
														if($arrValorAgrupadores[$arrayAuxAgrupadores[$j]]["codigo"] == $resultado[$i]["cod_agrupador".$j]) {
															for($k=1; $k<=count($_REQUEST["agrupadorColunas"]); $k++) {
																if($resultado[$i]["coluna".$k] != NULL) {
																	$valor = number_format($resultado[$i]["coluna".$k], 0, ',', '.' );
																}
																else {
																	$valor = " - ";
																}
																	
																$arrValorAgrupadores[$arrayAuxAgrupadores[$j]]["colunas"][$k] = $valor;
															}
														}
														else {
															$arrValorAgrupadores[$contAgrupador]["codigo"] 		= $resultado[$i]["cod_agrupador".$j];
															$arrValorAgrupadores[$contAgrupador]["descricao"] 	= $resultado[$i]["dsc_agrupador".$j];
															$arrValorAgrupadores[$contAgrupador]["cor"] 		= $cor;
															$arrValorAgrupadores[$contAgrupador]["padding"] 	= (($j-1) * 10);
															$arrValorAgrupadores[$contAgrupador]["seta"] 		= $seta;
															$arrValorAgrupadores[$contAgrupador]["filhos"] 		= 'N';
															$arrValorAgrupadores[$contAgrupador]["nivel"] 		= $j;
															$arrValorAgrupadores[$contAgrupador]["agrupador"]	= $_REQUEST["agrupador"][($j-1)];
															
															for($k=1; $k<=count($_REQUEST["agrupadorColunas"]); $k++) {
																if($resultado[$i]["coluna".$k] != NULL) {
																	$valor = number_format($resultado[$i]["coluna".$k], 0, ',', '.' );
																	$totalGeral[$k] += $resultado[$i]["coluna".$k];
																}
																else {
																	$valor = " - ";
																}
																	
																$arrValorAgrupadores[$contAgrupador]["colunas"][$k] = $valor;
															}
															$arrayAuxAgrupadores[$j] = $contAgrupador;
															$contAgrupador++;
														}
													}
												}
											}
										}
										
									if(isset($_REQUEST["tipo"]) && $_REQUEST["tipo"] == 'xls') {
										for($i=0; $i<count($arrValorAgrupadores); $i++) {
											$row = array();
											$espacoBranco = "";
											
											for($j=0;$j<((integer)$arrValorAgrupadores[$i]["padding"]);$j++) { $espacoBranco .= " "; }
											
											array_push($row, $espacoBranco.$arrValorAgrupadores[$i]["codigo"]." ".$arrValorAgrupadores[$i]["descricao"]);
											
											for($k=1; $k<=count($arrValorAgrupadores[$i]["colunas"]); $k++) {
												array_push($row, $arrValorAgrupadores[$i]["colunas"][$k]);
											}
											
											$xls->addRow($row);
										}
										
										ob_clean();
										$xls->sendFile();
										die;
									}
									else {	
										$arrNivel 	 = array();
										$arrNivel[1] = false;
										$arrNivel[2] = false;
										$arrNivel[3] = false;
										$arrNivel[4] = false;
										for($i=0; $i<count($arrValorAgrupadores); $i++) {
											if($arrNivel[$arrValorAgrupadores[$i]["nivel"]] == true) {
												for($t=(count($_REQUEST["agrupador"]) - 1); $t>$arrValorAgrupadores[$i]["nivel"]; $t--) {
													if($arrNivel[$t] == true) {
														echo "</div>";
														$arrNivel[$t] = false;
													}
												}
												
												echo "</div>";
												$arrNivel[$arrValorAgrupadores[$i]["nivel"]] = false;
											}
												
											if($arrValorAgrupadores[$i]["filhos"] == 'S') {
												$img = '<img id="imagem'.$i.'" style="cursor:pointer;" src="'.(($_REQUEST['agrupadores_escondidos']? '../imagens/mais.gif' : '../imagens/menos.gif')).'" onclick="abreFechaDiv('.$i.')" /> <input type="hidden" name="hidden_mais['.$i.'] id="hidden_mais_'.$i.'" value="'.$i.'">';
											} else {
												$img = '';
											}
											
											//Inclui a opï¿½ï¿½o de detalhar o ï¿½ltimo nï¿½vel
											if($arrValorAgrupadores[$i]["nivel"] == count($_REQUEST["agrupador"])){
												$tabelaDetalheAgrupador = "<div style=\"display:\" id=\"div_detalhe_agrupador_{$arrValorAgrupadores[$i]["agrupador"]}_{$arrValorAgrupadores[$i]["codigo"]}\" ></div>";
											}else{
												$tabelaDetalheAgrupador = "";
											}
											
																						
											
											echo '<table cellspacing="0" border="0">';
											echo '<tr bgcolor="'.$arrValorAgrupadores[$i]["cor"].'" onmouseover="this.style.backgroundColor = \'#ffffcc\';" onmouseout="this.style.backgroundColor = \''.$arrValorAgrupadores[$i]["cor"].'\';">';
											echo '<td style="text-align:left; padding: 2px 3px 2px '.$arrValorAgrupadores[$i]["padding"].'px;border-right: 1px solid #CCCCCC;">'.$arrValorAgrupadores[$i]["seta"].' '.$img.' '.$arrValorAgrupadores[$i]["codigo"].' '.$arrValorAgrupadores[$i]["descricao"].'</td>';
											
											for($k=1; $k<=count($arrValorAgrupadores[$i]["colunas"]); $k++) {
												echo '<td class="colunaValor">'.$arrValorAgrupadores[$i]["colunas"][$k].'</td>';
											}
											
											echo '</tr></table>';
											
											//Adiciona a Tabela para exibiï¿½ï¿½o do Detalhe do Agurpador de ï¿½limo Nï¿½vel
											echo $tabelaDetalheAgrupador;
											
											
											if($arrValorAgrupadores[$i]["filhos"] == 'S') {
												echo '<div '.(($_REQUEST['agrupadores_escondidos']? 'style="display:none"' : '')).'  id="div'.$i.'">';
												$arrNivel[$arrValorAgrupadores[$i]["nivel"]] = true;
											}
										}
										
										echo "</div>";
										?>
										</td>
										</tr>
										<tr>
										<td class="noPadding" align="left">
										
										<table id="tabelaTitulos" align="left" cellspacing="0">
											<thead>
											<tr bgcolor="#DEDEDE">
										<?
										
										echo '<th class="bold alignLeft" style="border-bottom:1px solid #CCCCCC;border-right: 1px solid #AAAAAA;border-left: 1px solid #FFFFFF;">TOTAL GERAL</th>';
										for($k=1; $k<=count($_REQUEST["agrupadorColunas"]); $k++) {
											echo '<th class=\"alignRightTit \" style="width:80px;text-align:right;border-bottom:1px solid #CCCCCC;border-right: 1px solid #AAAAAA;border-left: 1px solid #FFFFFF;padding: 2px 2px 2px 3px;">'.number_format($totalGeral[$k], 0, ',', '.' ).'</th>';
										}
										echo '</tr></thead></table></td></tr>';
									}
									}
									else { ?>
										<tr><td>
										<br/><br/><p style="color: #ff2020;">Nenhum resultado para os parï¿½metros indicados.</p>
										</td></tr>
									<? } 
								?>
						</tbody>
					</table>
		<script type="text/javascript" language="javascript">
			document.getElementById('aguarde').style.visibility = 'hidden';
			document.getElementById('aguarde').style.display = 'none';
			
		function abreFechaDiv(id) {
			var div = document.getElementById('div'+id);
			
			mudaImagem(id);
			
			if(div.style.display == "none")
				 div.style.display = "block";
			else
				div.style.display = "none";
		}
		
		function mudaImagem(id) {
  			var imagem = document.getElementById('imagem'+id);
  			
  			var nomeImagem = imagem.src.substr(imagem.src.search("mais"));
  			
  			if(nomeImagem == "mais.gif")
  				imagem.src = "../imagens/menos.gif";
  			else
  				imagem.src = "../imagens/mais.gif";
  		}

		function extraiScript(texto){  
			//desenvolvido por Skywalker.to, Micox e Pita.  
			//http://forum.imasters.uol.com.br/index.php?showtopic=165277  
			var ini, pos_src, fim, codigo;  
			var objScript = null;  
			ini = texto.indexOf('<script', 0)  
			while (ini!=-1){  
				var objScript = document.createElement("script");  
				//Busca se tem algum src a partir do inicio do script  
				pos_src = texto.indexOf(' src', ini)  
				ini = texto.indexOf('>', ini) + 1;
		
				//Verifica se este e um bloco de script ou include para um arquivo de scripts  
				if (pos_src < ini && pos_src >=0){//Se encontrou um "src" dentro da tag script, esta e um include de um arquivo script  
					//Marca como sendo o inicio do nome do arquivo para depois do src  
					ini = pos_src + 4;  
					//Procura pelo ponto do nome da extencao do arquivo e marca para depois dele  
					fim = texto.indexOf('.', ini)+4;  
					//Pega o nome do arquivo  
					codigo = texto.substring(ini,fim);  
					//Elimina do nome do arquivo os caracteres que possam ter sido pegos por engano  
					codigo = codigo.replace("=","").replace(" ","").replace("\"","").replace("\"","").replace("\'","").replace("\'","").replace(">","");  
					// Adiciona o arquivo de script ao objeto que sera adicionado ao documento  
					objScript.src = codigo;  
				}else{
				//Se nao encontrou um "src" dentro da tag script, esta e um bloco de codigo script  
					// Procura o final do script
					fim = texto.indexOf('</script', ini);  
					// Extrai apenas o script
					codigo = texto.substring(ini,fim);  
					// Adiciona o bloco de script ao objeto que sera adicionado ao documento  
					objScript.text = codigo;
				}
				
				//Adiciona o script ao documento  
				document.body.appendChild(objScript);
				
				// Procura a proxima tag de <script>  
				ini = texto.indexOf('<script', fim);
				
				//Limpa o objeto de script  
				objScript = null;  
			}  
		}  

  		function detalheAgrupador(codigo,agrupadorCorrente,agrupadorAntigo,valorAgrupadorAntigo){
			//Verifica se existe um agrupdaor sendo editado, se houver, fecha.
			verificaEdicaoCorrente(agrupadorCorrente,codigo);

			var div = document.getElementById('div_detalhe_agrupador_' + agrupadorCorrente + '_' + codigo);
			var img_mais = document.getElementById('img_detalhe_agrupador_mais_'  + agrupadorCorrente + '_' + codigo);
			var img_menos = document.getElementById('img_detalhe_agrupador_menos_'  + agrupadorCorrente + '_' + codigo);
			var agrupadores = $('hidden_agrupadores').serialize();
			var agrupadorCorrente = agrupadorCorrente;
			var agrupadorColunas = $('hidden_agrupadorColunas').serialize();

			if(div.innerHTML){
				div.style.display = "";
				img_mais.style.display = "none";
				img_menos.style.display = "";
				document.getElementById('hidden_agrupador_corrente').value = "";
				document.getElementById('hidden_valor_agrupador_corrente').value = "";
			}
			else{
				var req = new Ajax.Request('financeiro.php?modulo=relatorio/geral_teste&acao=R', {
			        method:     'post',
			        parameters: '&AjaxDetalhaAgrupador=true&' + agrupadores + '&' + agrupadorColunas + '&codigoAgrupador=' + codigo + '&agrupadorCorrente=' + agrupadorCorrente + '&agrupadorAntigo=' + agrupadorAntigo + '&valorAgrupadorAntigo=' + valorAgrupadorAntigo,
			        onComplete: function (res)
			        {	
						div.innerHTML = res.responseText;
						extraiScript(res.responseText);
						div.style.display = "";
						img_mais.style.display = "none";
						img_menos.style.display = "";
			        }
			  	});
			}
  		}

  		function fechaDetalhe(codigo,agrupadorCorrente,clear){
			var AgrupadorCorrente = document.getElementById('hidden_agrupador_corrente');
			var ValorAgrupadorCorrente = document.getElementById('hidden_valor_agrupador_corrente');
			var div = document.getElementById('div_detalhe_agrupador_'  + agrupadorCorrente + '_' + codigo);
			var img_mais = document.getElementById('img_detalhe_agrupador_mais_'  + agrupadorCorrente + '_' + codigo);
			var img_menos = document.getElementById('img_detalhe_agrupador_menos_'  + agrupadorCorrente + '_' + codigo);

			if(clear == true){
				div.innerHTML = "";
			}
			
			div.style.display = "none";
			img_mais.style.display = "";
			img_menos.style.display = "none";
			AgrupadorCorrente.value = "";
			ValorAgrupadorCorrente.value = "";
  		}
  		
  		function resetarDetalhe(codigo,agrupadorCorrente){

			var AgrupadorCorrente = document.getElementById('hidden_agrupador_corrente');
			var ValorAgrupadorCorrente = document.getElementById('hidden_valor_agrupador_corrente');
			var div = document.getElementById('div_detalhe_agrupador_'  + agrupadorCorrente + '_' + codigo);
			var img_mais = document.getElementById('img_detalhe_agrupador_mais_'  + agrupadorCorrente + '_' + codigo);
			var img_menos = document.getElementById('img_detalhe_agrupador_menos_'  + agrupadorCorrente + '_' + codigo);
			var img_reset = document.getElementById('img_detalhe_agrupador_reset_'  + agrupadorCorrente + '_' + codigo);

			
			div.innerHTML = "";
			div.style.display = "none";
			img_mais.style.display = "";
			img_menos.style.display = "none";
			img_reset.style.display = "none";
			AgrupadorCorrente.value = "";
			ValorAgrupadorCorrente.value = "";
  		}
  		

  		function enviaDetalhe(codigo,agrupadorCorrente,agrupadorAntigo,valorAgrupadorAntigo){

			var AgrupadorCorrente = document.getElementById('hidden_agrupador_corrente');
			var ValorAgrupadorCorrente = document.getElementById('hidden_valor_agrupador_corrente');  			
  			var div = document.getElementById('div_detalhe_agrupador_' + agrupadorCorrente + '_' + codigo);
			var cor = $('cor').serialize();
  			var img_mais = document.getElementById('img_detalhe_agrupador_mais_' + agrupadorCorrente + '_' + codigo);
			var img_menos = document.getElementById('img_detalhe_agrupador_menos_' + agrupadorCorrente + '_' + codigo);
			var img_reset = document.getElementById('img_detalhe_agrupador_reset_' + agrupadorCorrente + '_' + codigo);

  			if ( !document.getElementById('agrupador_simples').value && document.getElementById('agrupadorColunas').options.length == 0 ) {
  				alert( 'Escolha pelo menos um agrupador.' );
  			}else{
  			
  				if(document.getElementById('agrupador_simples').value){
  					agrupadoresAgrupador = "agrupadorColunas=" + document.getElementById('agrupador_simples').value; 
  				}
  				if(document.getElementById('agrupadorColunas').options.length != 0){
  					selectAllOptions( document.getElementById('agrupadorColunas') );
  	  				var agrupadoresAgrupador = $('agrupadorColunas').serialize();
  				}
  	  			
  				var agrupadores = $('hidden_agrupadores').serialize();
  				var agrupadorColunas = $('hidden_agrupadorColunas').serialize();
  				
  				div.innerHTML = "<center><img src=\"/imagens/wait.gif\" > Aguarde! Carregando Dados...</center>";
  				var req = new Ajax.Request('financeiro.php?modulo=relatorio/geral_teste&acao=R', {
			        method:     'post',
			        parameters: '&AjaxExibeDadosAgrupados=true&' + agrupadoresAgrupador + '&' + agrupadores + '&' + agrupadorColunas + '&codigoAgrupador=' + codigo + '&agrupadorCorrente=' + agrupadorCorrente + '&' + cor + '&agrupadorAntigo=' + agrupadorAntigo + '&valorAgrupadorAntigo=' + valorAgrupadorAntigo,
			        onComplete: function (res)
			        {					
						div.innerHTML = res.responseText;
						extraiScript(res.responseText);
						div.style.display = "";
						img_mais.style.display = "none";
						img_menos.style.display = "";
						img_reset.style.display = "";
						AgrupadorCorrente.value = "";
						ValorAgrupadorCorrente.value = "";
			        }
			  	});
  			}
  			
  		}

  		function exibeAba(aba){
			if(aba == "simples"){
				document.getElementById('aba_simples').setAttribute('class','aba_s');
				document.getElementById('aba_avancado').setAttribute('class','aba');
				document.getElementById('div_exibicao').setAttribute('class','exibe_simples');
				document.getElementById('div_avancado').style.display = "none";
				document.getElementById('div_simples').style.display = "";
			}
			if(aba == "avancado"){
				document.getElementById('aba_simples').setAttribute('class','aba');
				document.getElementById('aba_avancado').setAttribute('class','aba_s');
				document.getElementById('div_exibicao').setAttribute('class','exibe_avancado');
				document.getElementById('div_avancado').style.display = "";
				document.getElementById('div_simples').style.display = "none";
			}
  		}
  		
  		function carregaCores(){
			document.getElementById('exibe_cores').style.display = "";
  		}
  		
  		function selecionaCor(cor){
			document.getElementById('exibe_cores').style.display = "none";
			document.getElementById('cor').value = cor;
			document.getElementById('aba_cor').style.background = cor;
  		}


		function verificaEdicaoCorrente(codigo,agrupadorCorrente){
			var AgrupadorCorrente = document.getElementById('hidden_agrupador_corrente');
			var ValorAgrupadorCorrente = document.getElementById('hidden_valor_agrupador_corrente');
			if(AgrupadorCorrente.value && ValorAgrupadorCorrente.value){
				document.getElementById('div_detalhe_agrupador_' + ValorAgrupadorCorrente.value + '_' + AgrupadorCorrente.value).style.display = "none";
				document.getElementById('div_detalhe_agrupador_' + ValorAgrupadorCorrente.value + '_' + AgrupadorCorrente.value).innerHTML = "";
				document.getElementById('img_detalhe_agrupador_mais_' + ValorAgrupadorCorrente.value + '_' + AgrupadorCorrente.value).style.display = "";
				document.getElementById('img_detalhe_agrupador_menos_' + ValorAgrupadorCorrente.value + '_' + AgrupadorCorrente.value).style.display = "none";

			}
			AgrupadorCorrente.value = agrupadorCorrente;
			ValorAgrupadorCorrente.value = codigo;
		}
  		

  		
		</script>
	</body>
</html>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/agrupador.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
