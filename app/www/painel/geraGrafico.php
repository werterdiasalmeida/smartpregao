<?php
// carrega as funções gerais
include_once "config.inc";
include_once "_constantes.php";
include ("../../includes/funcoes.inc");
include ("../../includes/classes_simec.inc");

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if($_REQUEST['tipo']){
	
	/* INÍCIO - Recupera os parametros via $_GET e cria variáveis com os nomes e valores */
	$parametros = "tipo=".$_REQUEST['tipo'];
	$arrPar = explode(";",$parametros);
	foreach($arrPar as $dado){
		$d = explode("=",$dado);
		$arrparametros[ $d[0] ] = $d[1]; 
	}
	
	extract($arrparametros);
	/* FIM - Recupera os parametros via $_GET e cria variáveis com os nomes e valores */
		
	//dbg($arrparametros);
	/* ********* *  INICIO QUERY PARA PEGAR OS DADOS DO INDICADOR * ********* */
	if($indid) {
		$sql = "select 
					ind.indid,
					ind.perid,
					ume.umedesc,
					ind.indcumulativo,
					ind.indcumulativovalor,
					ind.unmid,
					ind.regid,
					ind.indqtdevalor
				from
					painel.indicador ind
				inner join
					painel.periodicidade per ON per.perid = ind.perid
				inner join
					painel.unidademeta ume ON ind.umeid = ume.umeid
				inner join
					painel.regionalizacao reg ON reg.regid = ind.regid
				where
					ind.indid = $indid
				and
					ind.indstatus = 'A'";
		$arrDadosIndicador = $db->pegaLinha($sql);
	} else {
		criaGraficoIndicadorNaoAtendido();
		exit;
	}
	
	/* ********* *  INICIO QUERY PARA PEGAR OS DADOS DO INDICADOR * ********* */

	
	/* INÍCIO - REGRAS DE CONSULTA */
	
	/* Início - Adiciona exibição de quantidade e valor quando não houver nenhuma delas selecionada */
	if(!$finac_valor && !$finac_qtde){
		$finac_valor  = 1;
		$finac_qtde  = 1;
	}
	/* Fim - Adiciona exibição de quantidade e valor quando não houver nenhuma delas selecionada */
	
	/* Início - Limita a 12 períodos caso não exista data inicial e final selecionada */
	if(!$dpeid && !$dpeid2){
		$limit = " limit 12";
	}
	/* Fim - Limita a 12 períodos caso não exista data inicial e final selecionada */
	
	/* Início - Indica o nome do índice aplicado aos gráficos */
	switch($indice_moeda){
		case "ipca":
			$nome_indice = "IPCA";
		break;
		default:
			$nome_indice = "Indice";
	}
	/* Fim - Indica o nome do índice aplicado aos gráficos */
	
	/* Início - Se o indicador for monetário, a quantidade é a representação financeira */
	$arrDadosIndicador['unmid'] = (int)$arrDadosIndicador['unmid'];
	if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA)
		$arrQtdVlr = array("quantidade");
	/* Fim - Se o indicador for monetário, a quantidade é a representação financeira */
	
	/* Início - Se o indicador for inteiro, deve-se pegar apenas valores antes da ',' */
	$arrDadosIndicador['unmid'] = (int)$arrDadosIndicador['unmid'];
	if($arrDadosIndicador['unmid'] == UNIDADEMEDICAO_NUM_INTEIRO)
		$tipoUnmid = "::integer ";
	/* Fim - Se o indicador for inteiro, deve-se pegar apenas valores antes da ',' */
		
	
	/* Início - Campo para coleta de quantidade */
	if($finac_qtde)
		$arrCampos[] = "sum(d.qtde)$tipoUnmid as dshqtde";
	/* Fim - Campo para coleta de quantidade */
	
	/* Início - Campo para coleta de valor */
	if($finac_valor)
		$arrCampos[] = "sum(d.valor) as dshvalor";
	/* Fim - Campo para coleta de valor */
	
	
	/* Início - Quando a exibição não for em gráfico de Pizza */
	if($tipo != "pizza"){
		$wherePeriodo = "d1.dpedatainicio>=dp.dpedatainicio 
							and 
						d1.dpedatafim<=dp.dpedatafim 
							and ";
	}
	/* Fim - Quando a exibição não for em gráfico de Pizza */
	
	/* FIM - REGRAS DE CONSULTA */
		
	
	/* INÍCIO - FILTROS POR PARAMETRO */
	
	/* Início - Filtro por detalhe (entid,polid,etc) e valor do detalhe */
	if($detalhe && $detalhe != "" && $valorDetalhe && $valorDetalhe != ""){
		if($detalhe == "muncod"){
			$arrFiltros[] = "d.dshcodmunicipio = '$valorDetalhe'";
		}elseif($detalhe == "estuf"){
			$arrFiltros[] = "d.dshuf = '$valorDetalhe'";
		}elseif($detalhe == "estado"){
			$arrFiltros[] = "d.dshuf = '$valorDetalhe'";
		}elseif($detalhe == "esccodinep"){
			$arrFiltros[] = "d.dshcod = '$valorDetalhe'";
		}elseif($detalhe == "paiid"){
			
		}elseif($detalhe == "tpmid"){
			$arrFiltros[] = "d.dshcodmunicipio in (	select muncod from territorios.muntipomunicipio where tpmid = '$valorDetalhe' )";
		}elseif($detalhe == "miccod"){
			$arrFiltros[] = "d.dshcodmunicipio in (	select muncod from territorios.municipio where miccod = '$valorDetalhe' )";
		}elseif($detalhe == "regcod"){
			$arrFiltros[] = "d.dshuf in ( select estuf from territorios.estado where regcod = '$valorDetalhe' ) ";
		}elseif($detalhe == "mescod"){
			$arrFiltros[] = "d.dshcodmunicipio in (select distinct muncod from territorios.municipio where mescod  = '$valorDetalhe') ";
		}elseif($detalhe == "iesid"){
			$arrFiltros[] = "d.dshcod = '$valorDetalhe'";
		}else{
			$arrFiltros[] = "d.$detalhe = '$valorDetalhe'";
		}
	}
	/* Fim - Filtro por detalhe (entid,polid,etc) e valor do detalhe */
	
	/* Início - Filtro por tidid*/
	if($tidid && $tidid != "todos"){
		$td = explode("_",$tidid);
		$tdiid = $td[1];
		
		$sqlTdiNumero = "select tdinumero from painel.detalhetipoindicador where tdiid = $tdiid";
		$tdinumero = $db->pegaUm($sqlTdiNumero);
		
		$arrCampos[] = "d.tidid$tdinumero";
		$arrGroup[] = "d.tidid$tdinumero";
	}
	/* Fim - Filtro por nível 1 de detalhe */
	
	/* Início - Filtro por nível 1 de detalhe*/
	if($tidid1 && $tidid1 != "todos"){
		$sql = "select 
					tdinumero 
				from 
					painel.detalhetipoindicador dti 
				inner join
					painel.detalhetipodadosindicador dtdi ON dtdi.tdiid = dti.tdiid 
				where 
					dtdi.tidid = $tidid1";
		$tdinumero1 = $db->pegaUm($sql);
		$arrFiltros[] ="d.tidid$tdinumero1 = $tidid1";
		$arrGroup[] = "d.tidid$tdinumero1";
	}
	/* Fim - Filtro por nível 1 de detalhe */
	
	/* Início - Filtro por nível 2 de detalhe*/
	if($tidid2 && $tidid2 != "todos"){
		$sql = "select 
					tdinumero 
				from 
					painel.detalhetipoindicador dti 
				inner join
					painel.detalhetipodadosindicador dtdi ON dtdi.tdiid = dti.tdiid 
				where 
					dtdi.tidid = $tidid2";
		$tdinumero2 = $db->pegaUm($sql);
		$arrFiltros[] ="d.tidid$tdinumero2 = $tidid2";
		$arrGroup[] = "d.tidid$tdinumero2";
	}
	/* Fim - Filtro por nível 2 de detalhe */
	
	/* Início - Filtro por período inicial*/
	if($dpeid){
		$arrFiltros[] = "d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpeid)";
		$andQntValor = " AND d1.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpeid) ";	
	}
	/* Fim - Filtro por período inicial*/
	
	/* Início - Filtro por período final*/
	if($dpeid2){
		$arrFiltros[] = "d.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = $dpeid2)";
		$andQntValor.= " AND d1.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = $dpeid2) ";	
	}
	/* Fim - Filtro por período final*/
	
	/* Início - Filtro por região*/
	if($regcod && $regcod != "" && $regcod != "todos"){
		$arrFiltros[] = "d.dshuf in ( select estuf from territorios.estado where regcod = '$regcod' )";
	}
	/* Fim - Filtro por região*/
	
	/* Início - Filtro por mesoregião*/
	if($mescod && $mescod != "" && $mescod != "todos"){
		$arrFiltros[] = "d.dshcodmunicipio in (select distinct muncod from territorios.municipio where mescod  = '$mescod') ";
	}
	/* Fim - Filtro por mesoregião*/
	
	/* Início - Filtro por grupo de municípios*/
	if($gtmid && $gtmid != "todos"){
		$arrFiltros[] = "d.dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid in (select tpmid from territorios.tipomunicipio where gtmid = $gtmid) )";
	}
	/* Fim - Filtro por grupo de municípios*/
	
	/* Início - Filtro por tipo de municípios*/
	if($tpmid && $tpmid != "todos"){
		$arrFiltros[] = "d.dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid = $tpmid )";
	}
	/* Fim - Filtro por tipo de municípios*/
	
	/* Início - Filtro por estado*/
	if($estuf && $estuf != "" && $estuf != "todos"){
		$arrFiltros[] = "d.dshuf = '$estuf'";
	}
	/* Fim - Filtro por estado*/
	
	/* Início - Filtro por municipio*/
	if($muncod && $muncod != "" && $muncod != "todos"){
		$arrFiltros[] = "d.dshcodmunicipio = '$muncod'";
	}
	/* Fim - Filtro por municipio*/
	//Filtro por entid
	if($entid && $entid != ""){
		$arrFiltros[] = "d.entid = '$entid' ";
	}
	
	/* Início - Filtro por regvalue*/
	//Filtro por regvalue
	if($regvalue && $regvalue != "" && $regvalue != "todos"){
		$sql = "select rgaidentificador,rgafiltro from painel.regagreg where regid = {$arrDadosIndicador['regid']}";
		$campoReg = $db->pegaLinha($sql);
		$arrFiltros[] = str_replace(array("AND","and","{".$campoReg['rgaidentificador']."}"),array("","",$regvalue),$campoReg['rgafiltro']);
	}
	/* Fim - Filtro por regvalue*/
	
	/* FIM - FILTROS POR PARAMETRO */
	
	
	/* INÍCIO - QUERY PARA CONSULTA DOS PERIODOS */
	
	/* Início - Filtro por periodicidade*/
	$periodicidade =  strtoupper($periodicidade) == "ANUAL" || strtoupper($periodicidade) == "ANO" ? PERIODO_ANUAL : $periodicidade;
	$perid = !$periodicidade ? $arrDadosIndicador['perid'] : $periodicidade;
	/* Fim - Filtro por periodicidade*/
	
	/* Início - SQL para criação dos períodos*/
	$sql = "select 
				dpe.dpeid,
				dpe.dpedsc,
				dpe.dpedatainicio,
				dpe.dpedatafim
			from
				painel.detalheperiodicidade dpe
			where
				dpe.perid = $perid
			and
				dpe.dpestatus = 'A'
			order by
				dpe.dpedatainicio asc";
	/* Início - SQL para criação dos períodos*/
	
	//Array para armazenamento dos periodos
	$arrPeriodos = $db->carregar($sql);
	
	/* FIM - QUERY PARA CONSULTA DOS PERIODOS */
	
		
	/* INÍCIO - QUERY PARA CONSULTA DOS VALORES E QUANTIDADES POR PERÍODO */
	//Se houver períodos para consulta
	if(is_array($arrPeriodos)){
		//percorre os dados de período
		foreach($arrPeriodos as $arrDetPer){
			
			/* Início SQL Novo */
			
			$sql = "select 
						dpeid,
						dpedsc,
						dpedatainicio,
						dpedatafim
						".(count($arrCampos) ? "," . implode(" , ",$arrCampos) : "" )."
					from (
							select 
								dp.dpeid,
								dp.dpedatainicio,
								dp.dpedatafim,
								d.indid,
								dp.dpedsc,
								".( in_array("d.tidid1",$arrCampos) ? "tidid1," : "" )."
								".( in_array("d.tidid2",$arrCampos) ? "tidid2," : "" )."
								case when d.indcumulativo = 'N' then
					        			case when (
							                        select 
							                        	count(d1.dpeid) 
							                        from 
							                        	painel.detalheperiodicidade d1
													inner join 
														painel.seriehistorica sh on sh.dpeid=d1.dpeid
													where 
														$wherePeriodo
														sh.indid=d.indid
														$andQntValor
													 
													limit 
														1
					                				) > 0 then sum(d.qtde) 
					                	else 0 end
									else sum(d.qtde)
								end as qtde,
								case when d.indcumulativovalor = 'N' then
					        			case when (
					                        		select 
					                        			count(d1.dpeid) 
					                        		from 
					                        			painel.detalheperiodicidade d1
					                                inner join 
					                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
					                                where 
					                                	$wherePeriodo
					                                	sh.indid=d.indid
					                                	$andQntValor
					                                 
					                                limit 
					                                	1
					                				) > 0 then sum(d.valor) 
					                			else 0 end
										else sum(d.valor)
								end as valor
							from 
								painel.v_detalheindicadorsh d
							inner join 
								painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
							-- periodo que vc quer exibir
							where 
								dp.perid = $perid
							-- indicador que vc quer exibir
							and 
								d.indid = $indid
							".(count($arrFiltros) ? " and ".implode(" and ",$arrFiltros) : "" )."
							--range de data compreendida no periodo
							and 
								d.dpedatainicio >= '".$arrDetPer['dpedatainicio']."'
							and
								d.dpedatafim <= '".$arrDetPer['dpedatafim']."'
							group by 
								d.indid,
								d.dpeid,
								dp.dpedsc,
								dp.dpeid,
								dp.dpedatainicio,
								dp.dpedatafim,
								d.indcumulativo,
								d.indcumulativovalor
								".( in_array("d.tidid1",$arrCampos) ? ",tidid1" : "" )."
								".( in_array("d.tidid2",$arrCampos) ? ",tidid2" : "" )."
								".(count($arrGroup) ? ",".implode(",",$arrGroup) : "" )."
						) d
					group by 
						dpedatainicio,
						dpedatafim,
						indid,
						dpeid,
						dpedsc
						".( in_array("d.tidid1",$arrCampos) ? ",tidid1" : "" )."
						".( in_array("d.tidid2",$arrCampos) ? ",tidid2" : "" )."
					order by 
						dpedatainicio
						".( in_array("d.tidid1",$arrCampos) ? ",tidid1" : "" )."
						".( in_array("d.tidid2",$arrCampos) ? ",tidid2" : "" )."";
//dbg($sql);
			/* Fim SQL Novo */

			if($tidid && $tidid != "todos"){
				
				//Array para armazenamento dos valores e quantidades do indicador no período
				$arrDados = $db->carregar($sql);
				//dbg($sql);
				if($arrDados[0]){
					
					foreach($arrDados as $dados){
						
						//Array para armazenamento dos valores e quantidades do indicador no período com chave no periodo
						$arrValor[ $arrDetPer['dpeid'] ] [ $dados["tidid$tdinumero"] ] = array( "dpedatainicio" => $arrDetPer['dpedatainicio'] , "dpedatafim" => $arrDetPer['dpedatafim'], "periodo" => $arrDetPer['dpedsc'] , "qtde" => $dados['dshqtde'], "valor" => $dados['dshvalor'] );
						//Array para armazenamento dos períodos com chave no periodo
						$arrPeriodo[ $arrDetPer['dpeid'] ] = $arrDetPer['dpedsc'];
						//Array para armazenamento dos valores e quantidades para verificação
						$arrVerificaQtde[] = $dados['dshqtde'];
						$arrVerificaValor[] = $dados['dshvalor'];
					}
					
				}

			}else{
				
				//Array para armazenamento dos valores e quantidades do indicador no período
				$arrDados = $db->pegaLinha($sql);
				//dbg($sql);
				//Se houver quantidade ou valor adicionamos os dados no array que irá compor o gráfico
				if($arrDados['dshvalor'] || $arrDados['dshqtde']){
				
					//Array para armazenamento dos valores e quantidades do indicador no período com chave no periodo
					$arrValor[ $arrDetPer['dpeid'] ] = array( "dpedatainicio" => $arrDetPer['dpedatainicio'], "dpedatafim" => $arrDetPer['dpedatafim'] , "periodo" => $arrDetPer['dpedsc'] , "qtde" => $arrDados['dshqtde'], "valor" => $arrDados['dshvalor'] );
					//Array para armazenamento dos períodos com chave no periodo
					$arrPeriodo[ $arrDetPer['dpeid'] ] = $arrDetPer['dpedsc'];
					//Array para armazenamento dos valores e quantidades para verificação
					$arrVerificaQtde[] = $arrDados['dshqtde'];
					$arrVerificaValor[] = $arrDados['dshvalor'];
				}
			}
		}
	}
	/* FIM - QUERY PARA CONSULTA DOS VALORES E QUANTIDADES POR PERÍODO */
	
	//dbg($sql);
	
	//Filtro por regvalue
	$sql = "select 
				regdescricao, 
				rgaidentificador,
				rgafiltro 
			from 
				painel.regagreg reg1
			inner join
				painel.regionalizacao reg2 ON reg1.regid = reg2.regid 
			where 
				reg1.regid = {$arrDadosIndicador['regid']} 
			and 
				regsqlcombo is not null";
	$campoReg = $db->pegaLinha($sql);
	/* Fim - Filtro por regvalue*/
	
	if($campoReg && $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_RAZAO && (!$valorDetalhe && !$regvalue)){
		criaGraficoIndicadorRefinePesquisa("Indicador de razão! Por favor, selecione o(a) {$campoReg['regdescricao']} e tente novamente!");
		die;
	}
	
	/* INÍCIO - CHAMADA DA FUNÇÃO PARA CRIAÇÃO DO GRÁFICO COM OS PARAMETROS DE TIPO DE GRÁFICO, PERÍODO E VALORES / QUANTIDADES */
	if($arrValor && ( array_sum($arrVerificaQtde) != 0 || array_sum($arrVerificaValor) != 0 ))
		criaGraficoIndicador($tipo,$arrDadosIndicador,$arrValor,$arrparametros,$arrPeriodo);
	else
		criaGraficoIndicadorNaoAtendido();
	/* FIM - CHAMADA DA FUNÇÃO PARA CRIAÇÃO DO GRÁFICO COM OS PARAMETROS DE TIPO DE GRÁFICO, PERÍODO E VALORES / QUANTIDADES */
}

/* INÍCIO - FUNÇÃO PARA CRIAÇÃO DO GRÁFICO */
function criaGraficoIndicador($tipoGrafico,$arrDadosIndicador = array(),$arrValor = array(),$arrparametros = array(),$arrPeriodo = array()){
	global $db;
	
	include ("../../includes/open_flash_chart/open-flash-chart.php");
	include ("../../includes/open_flash_chart/ofc_sugar.php");
	 
	if(is_array($arrparametros)) //Se houver parametros cria-se as variaveis com seus valores através de extract
		extract($arrparametros); // executa extract para disponibilizar os parametros
	
	if($tidid && $tidid != "todos"){
		$td = explode("_",$tidid);
		$tdiid = $td[1];
		$sql = "select distinct 
					tidid
				from
					painel.detalhetipodadosindicador
				where
					tdiid = $tdiid
				and
					tidstatus = 'A'
				order by
					tidid";
		$arrDetalhes = $db->carregar($sql);
	}
		
	/* Início - Adiciona exibição de quantidade e valor quando não houver nenhuma delas selecionada */
	if(!$finac_valor && !$finac_qtde){
		$finac_valor  = 1;
		$finac_qtde  = 1;
	}
	
	if($arrDadosIndicador['indqtdevalor'] == "f")
		$finac_valor  = false;
	
	/* Fim - Adiciona exibição de quantidade e valor quando não houver nenhuma delas selecionada */
	
	/* Início - Aplica a escala */
	$escala = $unidade_inteiro && $unidade_inteiro != "null" ? $unidade_inteiro : 1;
	$escala = $unidade_moeda && $unidade_moeda != "null" ? $unidade_moeda : $escala;
	switch($escala){
		case 1:
			$EscalaTooltip = "";
		break;
		case 1000:
			$EscalaTooltip = " (Milhares)";
		break;
		case 1000000:
			$EscalaTooltip = " (Milhões)";
		break;
		case 1000000000:
			$EscalaTooltip = " (Bilhões)";
		break;
		default:
			$EscalaTooltip = "";
	}
	/* Início - Aplica a escala */ 
	
	/* Início - Aplicação de índices */
	$indice_moeda = $indice_moeda && $indice_moeda != "null" ? $indice_moeda : false;
	if($indice_moeda){
		switch($indice_moeda){
			case "ipca":
				$tooltipIndice = " Índice IPCA";
			break;
			default:
				$tooltipIndice = "";
		}
	}
	/* Fim - Aplicação de índices */
	
	switch($tipoGrafico)
	{
		/* Início - Gráfico Tipo = Linha */
		case "linha":
			
			$tipoLinha1 = new dot(); //instância um novo tipo de linha do gráfico
			$tipoLinha1->size(3)->halo_size(3)->colour('#6495ED'); //atribui parametros de estilo a linha (tamanho,ponto,cor)
			
			/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"){
				
				$tipoLinhaCumulativo1 = new dot(); //instância um novo tipo de linha do gráfico
				$tipoLinhaCumulativo1->size(3)->halo_size(3)->colour('#191970')->tooltip("#val# (valor acumulado)"); //atribui parametros de estilo a linha (tamanho,ponto,cor)
				
				/* Início - Criação da linha do gráfico */
				$line_cumulativo_1 = new line(); //instância a linha
				$line_cumulativo_1->set_default_dot_style($tipoLinhaCumulativo1); //atribui o tipo de linha criado
				$line_cumulativo_1->set_width( 2 ); //indica a largura
				$line_cumulativo_1->set_colour("#191970"); //indica a cor
				/* Fim - Criação da linha do gráfico */
				
			}
			/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			
			/* Início - Se o indicador possibilitar aplicação de índices*/
			if($indice_moeda){
				
				$tipoLinhaIndice1 = new hollow_dot(); //instância um novo tipo de linha do gráfico
				$tipoLinhaIndice1->size(3)->halo_size(3)->colour('#191970')->tooltip("#val# ".removeAcentosGrafico($EscalaTooltip." ".$tooltipIndice).""); //atribui parametros de estilo a linha (tamanho,ponto,cor)
				
				/* Início - Criação da linha do gráfico */
				$line_indice_1 = new line(); //instância a linha
				$line_indice_1->set_default_dot_style($tipoLinhaIndice1); //atribui o tipo de linha criado
				$line_indice_1->set_width( 2 ); //indica a largura
				$line_indice_1->set_colour("#191970"); //indica a cor
				/* Fim - Criação da linha do gráfico */
				
			}
			/* Fim - Se o indicador possibilitar aplicação de índices*/
			
			
			/* Início - Criação do tipo de linha e linha para valor monetário*/
			if($finac_valor){
				
				$tipoLinha2 = new dot(); //instância um novo tipo de linha do gráfico
				$tipoLinha2->size(3)->halo_size(3)->colour('#3CB371')->tooltip("R$ #val#"); //atribui parametros de estilo a linha (tamanho,ponto,cor)
				
				/* Início - Criação da linha do gráfico */
				$line_2 = new line(); //instância a linha
				$line_2->set_default_dot_style($tipoLinha2); //atribui o tipo de linha criado
				$line_2->set_width( 2 ); //indica a largura
				$line_2->set_colour("#3CB371"); //indica a cor
				$line_2->attach_to_right_y_axis(); // adiciona ao eixo y do lado direito
				/* Fim - Criação da linha do gráfico */
				
				/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A"){
					
					$tipoLinhaCumulativo2 = new dot(); //instância um novo tipo de linha do gráfico
					$tipoLinhaCumulativo2->size(3)->halo_size(3)->colour('#006400')->tooltip("R$ #val# (valor acumulado)"); //atribui parametros de estilo a linha (tamanho,ponto,cor)
					
					/* Início - Criação da linha do gráfico */
					$line_cumulativo_2 = new line(); //instância a linha
					$line_cumulativo_2->set_default_dot_style($tipoLinhaCumulativo2); //atribui o tipo de linha criado
					$line_cumulativo_2->set_width( 2 ); //indica a largura
					$line_cumulativo_2->set_colour("#006400"); //indica a cor
					$line_cumulativo_2->attach_to_right_y_axis(); // adiciona ao eixo y do lado direito
					/* Fim - Criação da linha do gráfico */
					
				}
				/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				
				/* Início - Se o indicador possibilitar aplicação de índices*/
				if($indice_moeda){
					
					$tipoLinhaIndice2 = new hollow_dot(); //instância um novo tipo de linha do gráfico
					$tipoLinhaIndice2->size(3)->halo_size(3)->colour('#3CB371')->tooltip("R$ #val# (".removeAcentosGrafico($EscalaTooltip." ".$tooltipIndice).")"); //atribui parametros de estilo a linha (tamanho,ponto,cor)
					
					/* Início - Criação da linha do gráfico */
					$line_indice_2 = new line(); //instância a linha
					$line_indice_2->set_default_dot_style($tipoLinhaIndice2); //atribui o tipo de linha criado
					$line_indice_2->set_width( 2 ); //indica a largura
					$line_indice_2->set_colour("#3CB371"); //indica a cor
					$line_indice_2->attach_to_right_y_axis();
					/* Fim - Criação da linha do gráfico */
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
				
			}
			/* Fim - Criação do tipo de linha e linha para valor monetário*/
			
			
			/* Início - Aplica a exibição de 'R$' para dados monetários */
			if($unidade_moeda && $unidade_moeda != "null"){
				$tipoLinha1->tooltip("R$ #val#");
			}
			/* Fim - Aplica a exibição de 'R$' para dados monetários */
			
			/* Início - Criação da linha do gráfico */
			$line_1 = new line(); //instância a linha
			$line_1->set_default_dot_style($tipoLinha1); //atribui o tipo de linha criado
			$line_1->set_width( 2 ); //indica a largura
			$line_1->set_colour("#6495ED"); //indica a cor
			/* Fim - Criação da linha do gráfico */
			
			/* Início - cria as variáveis usadas no foreach com valor zero*/
			$valorAcumulado = 0;
			$valorMonetarioAcumulado = 0;
			$valorIndiceAcumulado = 0;
			$valorIndiceMonetarioAcumulado = 0;
			/* Fim - cria as variáveis usadas no foreach com valor zero*/
			
			/* Início - Percorre o array de dados para criar os valores para o gráfico */
			foreach($arrValor as $arrV){
				
				//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
				$arrQtdeIndicador[] = round( (float)$arrV['qtde'] / $escala , 2) ;
				
				/* Início - Aplica cumulatividade Anual */
				if($arrDadosIndicador['indcumulativo'] == "A"){
					if(!$anoCorrente){
						$data = explode("-",$arrV['dpedatainicio']);
						$anoCorrente = $data[0];
					}else{
						$data = explode("-",$arrV['dpedatainicio']);
						if($anoCorrente != $data[0]){
							$valorAcumulado = 0;
							$anoCorrente = $data[0];
						}
					}
				}
				/* Fim - Aplica cumulatividade Anual */
				
				//variável para acumular as quantidades
				$valorAcumulado += round( (float)$arrV['qtde'] / $escala , 2) ;
				
				//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
				$arrQtdeAcumuladoIndicador[] = round( $valorAcumulado , 2) ;
				
				/* Início - Aplica cumulatividade Anual */
				if($arrDadosIndicador['indcumulativovalor'] == "A"){
					if(!$anoCorrenteValor){
						$data = explode("-",$arrV['dpedatainicio']);
						$anoCorrenteValor = $data[0];
					}else{
						$data = explode("-",$arrV['dpedatainicio']);
						if($anoCorrenteValor != $data[0]){
							$valorMonetarioAcumulado = 0;
							$anoCorrenteValor = $data[0];
						}
					}
				}
				/* Fim - Aplica cumulatividade Anual */
				
				//variável para acumular as quantidades	
				$valorMonetarioAcumulado += round( (float)$arrV['valor'] / $escala , 2) ;
				
				//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
				$arrValorAcumuladoIndicador[] = round( $valorMonetarioAcumulado , 2) ;
				
				//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
				$arrValorIndicador[] = round( (float)$arrV['valor']  / $escala , 2);
				
				//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
				$arrPeriodos[] = $arrV['periodo'];
				
				/* Início - Se o indicador possibilitar aplicação de índices*/
				if($indice_moeda){
					$sql = "select ipcindice from painel.ipca where ipcstatus = 'A' and ipcano = '".date("Y",strtotime($arrV['dpedatainicio']))."'";
					$ipca = $db->pegaUm($sql);
					$ipca = !$ipca ? 1 : $ipca;
					
					$arrQtdeIndiceIndicador[] = round( (float)$arrV['qtde'] * $ipca / $escala , 2) ;
					
					/* Início - Aplica cumulatividade Anual */
					if($arrDadosIndicador['indcumulativo'] == "A"){
						if(!$anoCorrente){
							$data = explode("-",$arrV['dpedatainicio']);
							$anoCorrente = $data[0];
						}else{
							$data = explode("-",$arrV['dpedatainicio']);
							if($anoCorrente != $data[0]){
								$valorIndiceAcumulado = 0;
								$anoCorrente = $data[0];
							}
						}
					}
					/* Fim - Aplica cumulatividade Anual */
					
					//variável para acumular as quantidades
					$valorIndiceAcumulado += round( (float)$arrV['qtde'] * $ipca  / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrQtdeIndiceAcumuladoIndicador[] = round( $valorIndiceAcumulado , 2) ;
					
					/* Início - Aplica cumulatividade Anual */
					if($arrDadosIndicador['indcumulativovalor'] == "A"){
						if(!$anoCorrente){
							$data = explode("-",$arrV['dpedatainicio']);
							$anoCorrente = $data[0];
						}else{
							$data = explode("-",$arrV['dpedatainicio']);
							if($anoCorrente != $data[0]){
								$valorMonetarioIndiceAcumulado = 0;
								$anoCorrente = $data[0];
							}
						}
					}
					/* Fim - Aplica cumulatividade Anual */
					
					//variável para acumular as quantidades
					$valorMonetarioIndiceAcumulado += round( (float)$arrV['valor'] * $ipca  / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrValorAcumuladoIndiceIndicador[] = round( $valorMonetarioIndiceAcumulado , 2) ;
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					$arrValorIndiceIndicador[] = round( (float)$arrV['valor'] * $ipca   / $escala , 2);
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
				
			}
			/* Fim - Percorre o array de dados para criar os valores para o gráfico */
			
			if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A")
				$max = getMaxValue($arrQtdeAcumuladoIndicador,$arrQtdeIndiceAcumuladoIndicador); //retorna o maior valor para criação do range do eixo y do gráfico
			else
				$max = getMaxValue($arrQtdeIndicador,$arrQtdeIndiceIndicador); //retorna o maior valor para criação do range do eixo y do gráfico
			
			//Instância do Eixo Y
			$eixo_y = new y_axis();
			//Range do Eixo Y
			$range = count($arrQtdeIndicador) == 0 ? 1 : ($max / 4);
			//Atribui os valores ao Eixo y
			$eixo_y->set_range( 0, $max , $range);
			
			if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A")
				$max2 = getMaxValue($arrValorAcumuladoIndicador,$arrValorAcumuladoIndiceIndicador); //retorna o maior valor para criação do range do eixo y do gráfico
			else
				$max2 = getMaxValue($arrValorIndicador,$arrValorIndiceIndicador); //retorna o maior valor para criação do range do eixo y do gráfico
			 
			$yr = new y_axis_right();
			$range2 = count($arrValorIndicador) == 0 ? 1 : ($max2 / 4);
			$yr->set_range( 0, $max2 , $range2 );
			$yr->set_tick_length( 12 );
			$yr->set_steps( 2 );
			
			//Definindo Label do Eixo X
			$x_labels = new x_axis_labels();
			//adiciona a cor dos elementos do eixo x
			$x_labels->set_colour( '#990000' );
			//adiciona os elementos do eixo x
			$x_labels->set_labels( removeAcentosGrafico($arrPeriodos) );
			//rotaciona o eixo x
			$x_labels->rotate(30);
			
			//Instância do Eixo X
			$eixo_x = new x_axis();
			//Adiciona a definição de label do eixo x
			$eixo_x->set_labels($x_labels) ;
			
			//Instância de novo gráfico
			$chart = new open_flash_chart();
			//adiciona o eixo x ao grafico
			$chart->set_x_axis( $eixo_x );
			//adiciona o eixo y ao grafico
			$chart->set_y_axis( $eixo_y );
												
			/* INÍCIO - ATRIBUI OS VALORES DA LINHA 1 */
			if($finac_qtde){
				
				$line_1->set_values( $arrQtdeIndicador );
				$line_1->set_key( removeacentosGrafico($arrDadosIndicador['umedesc'].$EscalaTooltip) ,10);
				//adiciona o elemento linha 1 no gráfico 
				$chart->add_element( $line_1 );
				
				if($indice_moeda && $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA){
					$line_indice_1->set_values( $arrQtdeIndiceIndicador );
					$line_indice_1->set_key( removeacentosGrafico($arrDadosIndicador['umedesc'].$EscalaTooltip." ".$tooltipIndice) ,10);
					//adiciona o elemento linha 1 no gráfico
					$chart->add_element( $line_indice_1 );
				}
				
				/* Início - Se o indicador possibilitar cumulatividade (S ou A) atribui-se os valores ao gráfico */
				if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"){
					
					$line_cumulativo_1->set_values( $arrQtdeAcumuladoIndicador );
					$line_cumulativo_1->set_key( removeacentosGrafico($arrDadosIndicador['umedesc']." Acumulado(s)".$EscalaTooltip) ,10);
					//adiciona o elemento linha 1 no gráfico 
					$chart->add_element( $line_cumulativo_1 );
				}
				/* Fim - Se o indicador possibilitar cumulatividade (S ou A) atribui-se os valores ao gráfico */
				
			}
			/* FIM - ATRIBUI OS VALORES DA LINHA 1 */
			
			
			/* INÍCIO - ATRIBUI OS VALORES DA LINHA 2 */
			if($finac_valor && $arrDadosIndicador['unmid'] != UNIDADEMEDICAO_MOEDA){
				
				//adiciona o eixo y direito
				$chart->set_y_axis_right( $yr );
				
				$line_2->set_values( $arrValorIndicador );
				$line_2->set_key( removeacentosGrafico("Valor Monetário R$".$EscalaTooltip) ,10);
				//adiciona o elemento linha 2 no gráfico 
				$chart->add_element( $line_2 );
				
				/* Início - Se o indicador possibilitar cumulatividade (S ou A) atribui-se os valores ao gráfico */
				if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A"){
					$line_cumulativo_2->set_values( $arrValorAcumuladoIndicador );
					$line_cumulativo_2->set_key( removeacentosGrafico("Valor Monetário Acumulado R$".$EscalaTooltip) ,10);
					//adiciona o elemento linha 1 no gráfico 
					$chart->add_element( $line_cumulativo_2 );
				}
				/* Fim - Se o indicador possibilitar cumulatividade (S ou A) atribui-se os valores ao gráfico */
				
				/* Início - Índice aplicado ao valor do indicador */
				if($indice_moeda && $arrDadosIndicador['indqtdevalor'] == "t" && $arrDadosIndicador['unmid'] != UNIDADEMEDICAO_MOEDA){
					$line_indice_2->set_values( $arrValorIndiceIndicador );
					$line_indice_2->set_key( removeacentosGrafico("Valor Monetário R$ ".$EscalaTooltip." (".$tooltipIndice.")") ,10);
					//adiciona o elemento linha 1 no gráfico 
					$chart->add_element( $line_indice_2 );
				}
				/* Fim - Índice aplicado ao valor do indicador */
				
			}
			/* FIM - ATRIBUI OS VALORES DA LINHA 2 */
			
			
			//informa a cor de background do grafico
			$chart->set_bg_colour( '#ffffff' );
			
			$y_legend = new y_legend( removeacentosGrafico($arrDadosIndicador['umedesc']) );
			$y_legend->set_style( '{font-size: 22px; color: #00000}' );
			
			$chart->set_y_legend( $y_legend );
			
			echo $chart->toPrettyString();
			
		break;
		/* Fim - Gráfico Tipo = Linha */
		
		/* Início - Gráfico Tipo = Meta */
		case "meta":
			
			$sql = "select 
						dpeid,
						dpedsc,
						dpedatainicio,
						dpedatafim
					from
						painel.detalheperiodicidade
					where
						perid = {$arrparametros['periodicidade']}
					and
						dpedatainicio >= (
									select 
										dpedatainicio
									from
										painel.detalheperiodicidade
									where
										perid = {$arrparametros['periodicidade']}
									and
										(
											select  
												dpedatainicio
											from
												painel.detalheperiodicidade
											where
												dpeid = {$arrparametros['dpeid']}
										) between dpedatainicio and dpedatafim limit 1
								)
					and
						dpedatafim <= (
								select 
									dpedatafim
								from
									painel.detalheperiodicidade
								where
									perid = {$arrparametros['periodicidade']}
								and
									(
										select  
											dpedatafim
										from
											painel.detalheperiodicidade
										where
											dpeid = {$arrparametros['dpeid2']}
									) between dpedatainicio and dpedatafim limit 1
								)
					order by
						dpedatainicio";

			$arrPeriodos = $db->carregar($sql);
			
			if($arrPeriodos){
				unset($arrPeriodo);
				foreach($arrPeriodos as $periodo){
					$arrPeriodo[$periodo['dpeid']] = $periodo['dpedsc'];
				}
			
				foreach($arrPeriodos as $periodo){
					if($arrValor[$periodo['dpeid']]){
						$arrDados[$periodo['dpeid']] = $arrValor[$periodo['dpeid']];
					}else{
						$arrDados[$periodo['dpeid']] = array("dpedatainicio" => $periodo['dpedatainicio'],
															 "dpedatafim" => $periodo['dpedatafim'], 
															 "periodo" => $periodo['dpedsc'] , 
															 "qtde" => null, 
															 "valor" => null);
					}
				}
				unset($arrPeriodos);
				unset($arrValor);
				$arrValor = $arrDados;
			}
			$bool_exibe = false;

			//Pega data inicial da projeção
			$sql = "select 
						dpedatainicio,
						dpedatafim
					from
						painel.detalheperiodicidade
					where
						dpeid = {$arrparametros['projecao']}";
			$arrDataProjecao = $db->pegaLinha($sql);
			
			foreach($arrValor as $dpeid => $valor){
				
				$dtinicio = (int)str_replace("-","",$valor['dpedatainicio']);
				$dtfim 	  = (int)str_replace("-","",$valor['dpedatafim']);
				$dtproj   = (int)str_replace("-","",$arrDataProjecao['dpedatainicio']);
				
				if( ( ($dtproj >= $dtinicio) && ( $dtproj <= $dtfim) ) || $bool_exibe == true){
					$bool_exibe = true;
					if( ($dtproj >= $dtinicio) && ( $dtproj <= $dtfim) ){
						$arrMetasQtdeIndicador[] = round((float)$valor['qtde'] / $escala ,2);
						$arrMetasvalorIndicador[] = round((float)$valor['valor'] / $escala ,2);
					}else{
						$sql = "select
									sum(dmivalor) as valor,
									sum(dmiqtde) as qtde
								from
									painel.detalhemetaindicador dmi
								inner join
									painel.metaindicador met ON met.metid = dmi.metid
								inner join
									painel.detalheperiodicidade dpe ON dmi.dpeid = dpe.dpeid
								where
									met.indid = {$arrDadosIndicador['indid']}
								and
									dmi.dmistatus = 'A'
								and
									met.metstatus = 'A'
								and
									dpedatainicio >= '{$valor['dpedatainicio']}'
								and
									dpedatafim <= '{$valor['dpedatafim']}'
								/*and
									dpe.perid = {$arrparametros['periodicidade']}*/";
						$arrMetaValor = $db->pegaLinha($sql);
						
						$arrMetasQtdeIndicador[]  = $arrMetaValor['qtde']  ? round((float)$arrMetaValor['qtde'] / $escala ,2)  : "num";
						$arrMetasvalorIndicador[] = $arrMetaValor['valor'] ? round((float)$arrMetaValor['valor'] / $escala ,2) : "num";
						
					}
				}else{
					$arrMetasQtdeIndicador[] = null;
					$arrMetasvalorIndicador[] = null;
				}
			}
//			dbg($arrMetasQtdeIndicador);
//			dbg($arrMetasvalorIndicador);
			if($arrMetasQtdeIndicador){
				foreach($arrMetasQtdeIndicador as $chave => $qtde){
					if($qtde != null){
						if($qtde == "num"){
							$arrChavesQ[] = $chave;
						}else{
							$arrChavesQtde[] = array("valor" => $qtde, "chave" => $chave, "array" => $arrChavesQ);
							unset($arrChavesQ);
						}
					}
				}
			}
			
			if($arrMetasvalorIndicador){
				foreach($arrMetasvalorIndicador as $chave => $valor){
					if($valor != null){
						if($valor == "num"){
							$arrChavesV[] = $chave;
						}else{
							$arrChavesValor[] = array("valor" => $qtde, "chave" => $chave, "array" => $arrChavesV);
							unset($arrChavesV);
						}
					}
				}
			}
			
			$x = 0;
			if($arrChavesQtde){
				foreach($arrChavesQtde as $key => $qtde){
					$arrQ[$x] = $qtde['valor'];
					if($qtde['array']){
						foreach($qtde['array'] as $chave){
							if($arrQ[$x - 1]){
								if($arrQ[$x] >= $arrQ[$x -1]){
									$qtdeQ = ($arrQ[$x] - $arrQ[$x -1]) / (count($qtde['array']) + 1); //aumenta
									$y = 1;
									foreach($qtde['array'] as $q){
										$arrMetasQtdeIndicador[$q] = round((float)$arrQ[$x -1] + ($qtdeQ * $y) / $escala ,2);
										$y++;
									}
								}else{
									$qtdeQ = ($arrQ[$x - 1] - $arrQ[$x]) / (count($qtde['array']) + 1); //diminui
									$y = 1;
									foreach($qtde['array'] as $q){
										$arrMetasQtdeIndicador[$q] = round((float)$arrQ[$x -1] - ($qtdeQ * $y) / $escala ,2);
										$y++;
									}
								}
							}else{
								$qtdeQ = $arrQ[$x] / (count($qtde['array']) + 1); //diminui
								$y = 1;
								foreach($qtde['array'] as $q){
									$arrMetasQtdeIndicador[$q] = round((float)$qtdeQ*$y / $escala ,2);
									$y++;
								}
							}
						}
					}
					$x++;
				}
			}
			
			$x = 0;
			if($arrChavesValor){
				foreach($arrChavesValor as $key => $qtde){
					$arrQ[$x] = $qtde['valor'];
					if($qtde['array']){
						foreach($qtde['array'] as $chave){
							if($arrQ[$x - 1]){
								if($arrQ[$x] >= $arrQ[$x -1]){
									$qtdeQ = ($arrQ[$x] - $arrQ[$x -1]) / (count($qtde['array']) + 1); //aumenta
									$y = 1;
									foreach($qtde['array'] as $q){
										$arrMetasvalorIndicador[$q] = round((float)$arrQ[$x -1] + ($qtdeQ * $y) / $escala ,2);
										$y++;
									}
								}else{
									$qtdeQ = ($arrQ[$x - 1] - $arrQ[$x]) / (count($qtde['array']) + 1); //diminui
									$y = 1;
									foreach($qtde['array'] as $q){
										$arrMetasvalorIndicador[$q] = round((float)$arrQ[$x -1] - ($qtdeQ * $y) / $escala ,2);
										$y++;
									}
								}
							}else{
								$qtdeQ = $arrQ[$x] / (count($qtde['array']) + 1); //diminui
								$y = 1;
								foreach($qtde['array'] as $q){
									$arrMetasvalorIndicador[$q] = round((float)$qtdeQ*$y / $escala ,2);
									$y++;
								}
							}
						}
					}
					$x++;
				}
			}
			
			//dbg($arrMetasQtdeIndicador);
			//dbg($arrMetasvalorIndicador);
			$n = 0;
			if($arrChavesQtde){
				foreach($arrChavesQtde as $chave){
					if($n != 0){
						$d2 = new solid_dot($arrMetasQtdeIndicador[$chave['chave']]);
						$arrMetasQtdeIndicador[$chave['chave']] = $d2->colour('#FF0000')->tooltip('#val#<br>Qtde de Meta');
					}
					$n++;
				}
			}
			$n = 0;
			if($arrChavesValor){
				foreach($arrChavesValor as $chave){
					if($n != 0){
						$d = new solid_dot($arrMetasvalorIndicador[$chave['chave']]);
						$arrMetasvalorIndicador[$chave['chave']] = $d->colour('#A52A2A')->tooltip('#val#<br>Valor de Meta');
					}
					$n++;
				}
			}
			
			$tipoLinha1 = new dot(); //instância um novo tipo de linha do gráfico
			$tipoLinha1->size(3)->halo_size(3)->colour('#6495ED'); //atribui parametros de estilo a linha (tamanho,ponto,cor)
			
			$tipoMetaValor = new hollow_dot(); //instância um novo tipo de linha do gráfico p/ a meta do valor
			$tipoMetaValor->size(5)->halo_size(0)->colour('#FF0000'); //atribui parametros de estilo a linha (tamanho,ponto,cor) p/ a meta do valor
			
			/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"){
				
				$tipoLinhaCumulativo1 = new dot(); //instância um novo tipo de linha do gráfico
				$tipoLinhaCumulativo1->size(3)->halo_size(3)->colour('#191970')->tooltip("#val# (valor acumulado)"); //atribui parametros de estilo a linha (tamanho,ponto,cor)
				
				/* Início - Criação da linha do gráfico */
				$line_cumulativo_1 = new line(); //instância a linha
				$line_cumulativo_1->set_default_dot_style($tipoLinhaCumulativo1); //atribui o tipo de linha criado
				$line_cumulativo_1->set_width( 2 ); //indica a largura
				$line_cumulativo_1->set_colour("#191970"); //indica a cor
				/* Fim - Criação da linha do gráfico */
				
			}
			/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
			
			/* Início - Se o indicador possibilitar aplicação de índices*/
			if($indice_moeda){
				
				$tipoLinhaIndice1 = new hollow_dot(); //instância um novo tipo de linha do gráfico
				$tipoLinhaIndice1->size(3)->halo_size(3)->colour('#191970')->tooltip("#val# ".removeAcentosGrafico($EscalaTooltip." ".$tooltipIndice).""); //atribui parametros de estilo a linha (tamanho,ponto,cor)
				
				/* Início - Criação da linha do gráfico */
				$line_indice_1 = new line(); //instância a linha
				$line_indice_1->set_default_dot_style($tipoLinhaIndice1); //atribui o tipo de linha criado
				$line_indice_1->set_width( 2 ); //indica a largura
				$line_indice_1->set_colour("#191970"); //indica a cor
				/* Fim - Criação da linha do gráfico */
				
			}
			/* Fim - Se o indicador possibilitar aplicação de índices*/
			
			
			/* Início - Criação do tipo de linha e linha para valor monetário*/
			if($finac_valor){
				
				$tipoLinha2 = new dot(); //instância um novo tipo de linha do gráfico
				$tipoLinha2->size(3)->halo_size(3)->colour('#3CB371')->tooltip("R$ #val#"); //atribui parametros de estilo a linha (tamanho,ponto,cor)
				
				/* Início - Criação da linha do gráfico */
				$line_2 = new line(); //instância a linha
				$line_2->set_default_dot_style($tipoLinha2); //atribui o tipo de linha criado
				$line_2->set_width( 2 ); //indica a largura
				$line_2->set_colour("#3CB371"); //indica a cor
				$line_2->attach_to_right_y_axis(); // adiciona ao eixo y do lado direito
				/* Fim - Criação da linha do gráfico */
				
				$tipoMetaValor2 = new hollow_dot(); //instância um novo tipo de linha do gráfico p/ valor monetário da meta
				$tipoMetaValor2->size(5)->halo_size(0)->colour('#A52A2A'); //atribui parametros de estilo a linha (tamanho,ponto,cor) p/ valor monetário da meta
				
				/* Início - Criação da linha do gráfico p/ valor monetário da meta */
				$line_meta_valor2 = new line(); //instância a linha
				$line_meta_valor2->set_default_dot_style($tipoMetaValor2); //atribui o tipo de linha criado
				$line_meta_valor2->set_width( 1 ); //indica a largura
				$line_meta_valor2->set_colour("#A52A2A"); //indica a cor
				$line_meta_valor2->attach_to_right_y_axis();
				/* Fim - Criação da linha do gráfico p/ valor monetário da meta */
				
				/* Início - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A"){
					
					$tipoLinhaCumulativo2 = new dot(); //instância um novo tipo de linha do gráfico
					$tipoLinhaCumulativo2->size(3)->halo_size(3)->colour('#006400')->tooltip("R$ #val# (valor acumulado)"); //atribui parametros de estilo a linha (tamanho,ponto,cor)
					
					/* Início - Criação da linha do gráfico */
					$line_cumulativo_2 = new line(); //instância a linha
					$line_cumulativo_2->set_default_dot_style($tipoLinhaCumulativo2); //atribui o tipo de linha criado
					$line_cumulativo_2->set_width( 2 ); //indica a largura
					$line_cumulativo_2->set_colour("#006400"); //indica a cor
					$line_cumulativo_2->attach_to_right_y_axis(); // adiciona ao eixo y do lado direito
					/* Fim - Criação da linha do gráfico */
					
				}
				/* Fim - Se o indicador possibilitar cumulatividade (S ou A) cria-se mais uma linha*/
				
				/* Início - Se o indicador possibilitar aplicação de índices*/
				if($indice_moeda){
					
					$tipoLinhaIndice2 = new hollow_dot(); //instância um novo tipo de linha do gráfico
					$tipoLinhaIndice2->size(3)->halo_size(3)->colour('#3CB371')->tooltip("R$ #val# (".removeAcentosGrafico($EscalaTooltip." ".$tooltipIndice).")"); //atribui parametros de estilo a linha (tamanho,ponto,cor)
					
					/* Início - Criação da linha do gráfico */
					$line_indice_2 = new line(); //instância a linha
					$line_indice_2->set_default_dot_style($tipoLinhaIndice2); //atribui o tipo de linha criado
					$line_indice_2->set_width( 2 ); //indica a largura
					$line_indice_2->set_colour("#3CB371"); //indica a cor
					$line_indice_2->attach_to_right_y_axis();
					/* Fim - Criação da linha do gráfico */
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
				
			}
			/* Fim - Criação do tipo de linha e linha para valor monetário*/
			
			
			/* Início - Aplica a exibição de 'R$' para dados monetários */
			if($unidade_moeda && $unidade_moeda != "null"){
				$tipoLinha1->tooltip("R$ #val#");
			}
			/* Fim - Aplica a exibição de 'R$' para dados monetários */
			
			/* Início - Criação da linha do gráfico */
			$line_1 = new line(); //instância a linha
			$line_1->set_default_dot_style($tipoLinha1); //atribui o tipo de linha criado
			$line_1->set_width( 2 ); //indica a largura
			$line_1->set_colour("#6495ED"); //indica a cor
			/* Fim - Criação da linha do gráfico */
			
			/* Início - Criação da linha do gráfico p/ valor da meta */
			$line_meta_valor = new line(); //instância a linha
			$line_meta_valor->set_default_dot_style($tipoMetaValor); //atribui o tipo de linha criado
			$line_meta_valor->set_width( 2 ); //indica a largura
			$line_meta_valor->set_colour("#FF0000"); //indica a cor
			/* Fim - Criação da linha do gráfico p/ valor da meta */
			
			/* Início - cria as variáveis usadas no foreach com valor zero*/
			$valorAcumulado = 0;
			$valorMonetarioAcumulado = 0;
			$valorIndiceAcumulado = 0;
			$valorIndiceMonetarioAcumulado = 0;
			/* Fim - cria as variáveis usadas no foreach com valor zero*/
			
			/* Início - Percorre o array de dados para criar os valores para o gráfico */
			foreach($arrValor as $arrV){
				
				//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
				if($arrV['qtde']){
					$arrQtdeIndicador[] = round( (float)$arrV['qtde'] / $escala , 2) ;
				}else{
					$arrQtdeIndicador[] = null;
				}
				
				/* Início - Aplica cumulatividade Anual */
				if($arrDadosIndicador['indcumulativo'] == "A"){
					if(!$anoCorrente){
						$data = explode("-",$arrV['dpedatainicio']);
						$anoCorrente = $data[0];
					}else{
						$data = explode("-",$arrV['dpedatainicio']);
						if($anoCorrente != $data[0]){
							$valorAcumulado = 0;
							$anoCorrente = $data[0];
						}
					}
				}
				/* Fim - Aplica cumulatividade Anual */
				
				//variável para acumular as quantidades
				if($arrV['qtde']){
					$valorAcumulado += round( (float)$arrV['qtde'] / $escala , 2) ;
				}else{
					$valorAcumulado = null;
				}
				
				//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
				if($arrV['qtde']){
					$arrQtdeAcumuladoIndicador[] = round( $valorAcumulado , 2) ;
				}else{
					$arrQtdeAcumuladoIndicador[] = null;
				}
				
				/* Início - Aplica cumulatividade Anual */
				if($arrDadosIndicador['indcumulativovalor'] == "A"){
					if(!$anoCorrenteValor){
						$data = explode("-",$arrV['dpedatainicio']);
						$anoCorrenteValor = $data[0];
					}else{
						$data = explode("-",$arrV['dpedatainicio']);
						if($anoCorrenteValor != $data[0]){
							$valorMonetarioAcumulado = 0;
							$anoCorrenteValor = $data[0];
						}
					}
				}
				/* Fim - Aplica cumulatividade Anual */
				
				//variável para acumular as quantidades	
				if($arrV['valor']){
					$valorMonetarioAcumulado += round( (float)$arrV['valor'] / $escala , 2) ;
				}else{
					$valorMonetarioAcumulado = null;
				}
				
				//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
				if($arrV['valor']){
					$arrValorAcumuladoIndicador[] = round( $valorMonetarioAcumulado , 2) ;
				}else{
					$arrValorAcumuladoIndicador[] = null;
				}
				
				//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
				if($arrV['valor']){
					$arrValorIndicador[] = round( (float)$arrV['valor']  / $escala , 2);
				}else{
					$arrValorIndicador[] = null;
				}
								
				//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
				$arrPeriodos[] = $arrV['periodo'];
				
				/* Início - Se o indicador possibilitar aplicação de índices*/
				if($indice_moeda){
					$sql = "select ipcindice from painel.ipca where ipcstatus = 'A' and ipcano = '".date("Y",strtotime($arrV['dpedatainicio']))."'";
					$ipca = $db->pegaUm($sql);
					$ipca = !$ipca ? 1 : $ipca;
					
					if($arrV['qtde']){
						$arrQtdeIndiceIndicador[] = round( (float)$arrV['qtde'] * $ipca / $escala , 2) ;
					}else{
						$arrQtdeIndiceIndicador[] = null;
					}
					
					/* Início - Aplica cumulatividade Anual */
					if($arrDadosIndicador['indcumulativo'] == "A"){
						if(!$anoCorrente){
							$data = explode("-",$arrV['dpedatainicio']);
							$anoCorrente = $data[0];
						}else{
							$data = explode("-",$arrV['dpedatainicio']);
							if($anoCorrente != $data[0]){
								$valorIndiceAcumulado = 0;
								$anoCorrente = $data[0];
							}
						}
					}
					/* Fim - Aplica cumulatividade Anual */
					
					//variável para acumular as quantidades
					if($arrV['qtde']){
						$valorIndiceAcumulado += round( (float)$arrV['qtde'] * $ipca  / $escala , 2) ;
					}else{
						$valorIndiceAcumulado = null;
					}
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					if($arrV['qtde']){
						$arrQtdeIndiceAcumuladoIndicador[] = round( $valorIndiceAcumulado , 2) ;
					}else{
						$arrQtdeIndiceAcumuladoIndicador[] = null;
					}
					
					/* Início - Aplica cumulatividade Anual */
					if($arrDadosIndicador['indcumulativovalor'] == "A"){
						if(!$anoCorrente){
							$data = explode("-",$arrV['dpedatainicio']);
							$anoCorrente = $data[0];
						}else{
							$data = explode("-",$arrV['dpedatainicio']);
							if($anoCorrente != $data[0]){
								$valorMonetarioIndiceAcumulado = 0;
								$anoCorrente = $data[0];
							}
						}
					}
					/* Fim - Aplica cumulatividade Anual */
					
					//variável para acumular as quantidades
					if($arrV['valor']){
						$valorMonetarioIndiceAcumulado += round( (float)$arrV['valor'] * $ipca  / $escala , 2) ;
					}else{
						$valorMonetarioIndiceAcumulado = null;
					}
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					if($arrV['valor']){
						$arrValorAcumuladoIndiceIndicador[] = round( $valorMonetarioIndiceAcumulado , 2) ;
					}else{
						$arrValorAcumuladoIndiceIndicador[] = null;
					}
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					if($arrV['valor']){
						$arrValorIndiceIndicador[] = round( (float)$arrV['valor'] * $ipca   / $escala , 2);
					}else{
						$arrValorIndiceIndicador[] = null;
					}
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/

			}
			/* Fim - Percorre o array de dados para criar os valores para o gráfico */
			
			if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"){
				$max = getMaxValue($arrQtdeAcumuladoIndicador,$arrQtdeIndiceAcumuladoIndicador); //retorna o maior valor para criação do range do eixo y do gráfico
			}else{
				$max = getMaxValue($arrQtdeIndicador,$arrQtdeIndiceIndicador); //retorna o maior valor para criação do range do eixo y do gráfico
			}
			
			$max = getMaxValue($arrMetasQtdeIndicador,array($max));
								
			//Instância do Eixo Y
			$eixo_y = new y_axis();
			//Range do Eixo Y
			$range = count($arrQtdeIndicador) == 0 ? 1 : ($max / 4);
			//Atribui os valores ao Eixo y
			$eixo_y->set_range( 0, $max , $range);
			
			if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A"){
				$max2 = getMaxValue($arrValorAcumuladoIndicador,$arrValorAcumuladoIndiceIndicador); //retorna o maior valor para criação do range do eixo y do gráfico
			}else{
				$max2 = getMaxValue($arrValorIndicador,$arrValorIndiceIndicador); //retorna o maior valor para criação do range do eixo y do gráfico
			}
			
			$max2 = getMaxValue($arrMetasvalorIndicador,array($max2));
			
			$yr = new y_axis_right();
			$range2 = count($arrValorIndicador) == 0 ? 1 : ($max2 / 4);
			$yr->set_range( 0, $max2 , $range2 );
			$yr->set_tick_length( 12 );
			$yr->set_steps( 2 );
			
			//Definindo Label do Eixo X
			$x_labels = new x_axis_labels();
			//adiciona a cor dos elementos do eixo x
			$x_labels->set_colour( '#990000' );
			//adiciona os elementos do eixo x
			$x_labels->set_labels( removeAcentosGrafico($arrPeriodos) );
			//rotaciona o eixo x
			$x_labels->rotate(30);
			
			//Instância do Eixo X
			$eixo_x = new x_axis();
			//Adiciona a definição de label do eixo x
			$eixo_x->set_labels($x_labels) ;
			
			//Instância de novo gráfico
			$chart = new open_flash_chart();
			//adiciona o eixo x ao grafico
			$chart->set_x_axis( $eixo_x );
			//adiciona o eixo y ao grafico
			$chart->set_y_axis( $eixo_y );
												
			/* INÍCIO - ATRIBUI OS VALORES DA LINHA 1 */
			if($finac_qtde){
				
				if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"){
					$line_cumulativo_1->set_values( $arrQtdeAcumuladoIndicador );
					$line_cumulativo_1->set_key( removeacentosGrafico($arrDadosIndicador['umedesc']." Acumulado(s)".$EscalaTooltip) ,10);
					//adiciona o elemento linha 1 no gráfico 
					$chart->add_element( $line_cumulativo_1 );
				}

				$line_1->set_values( $arrQtdeIndicador );
				$line_1->set_key( removeacentosGrafico($arrDadosIndicador['umedesc'].$EscalaTooltip) ,10);
				//adiciona o elemento linha 1 no gráfico 
				$chart->add_element( $line_1 );
								
				$line_meta_valor->set_values( $arrMetasQtdeIndicador );
				$line_meta_valor->set_key( "Meta de ".removeacentosGrafico($arrDadosIndicador['umedesc'].$EscalaTooltip) ,10);
				//adiciona o elemento linha 1 no gráfico 
				$chart->add_element( $line_meta_valor );
				
				if($indice_moeda && $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA){
					$line_indice_1->set_values( $arrQtdeIndiceIndicador );
					$line_indice_1->set_key( removeacentosGrafico($arrDadosIndicador['umedesc'].$EscalaTooltip." ".$tooltipIndice) ,10);
					//adiciona o elemento linha 1 no gráfico
					$chart->add_element( $line_indice_1 );
				}
				
			}
			/* FIM - ATRIBUI OS VALORES DA LINHA 1 */
			
			
			/* INÍCIO - ATRIBUI OS VALORES DA LINHA 2 */
			if($finac_valor && $arrDadosIndicador['unmid'] != UNIDADEMEDICAO_MOEDA){
				
				//adiciona o eixo y direito
				$chart->set_y_axis_right( $yr );
				
				/* Início - Se o indicador possibilitar cumulatividade (S ou A) atribui-se os valores ao gráfico */
				if($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A"){
					$line_cumulativo_2->set_values( $arrValorAcumuladoIndicador );
					$line_cumulativo_2->set_key( removeacentosGrafico("Valor Monetário Acumulado R$".$EscalaTooltip) ,10);
					//adiciona o elemento linha 1 no gráfico 
					$chart->add_element( $line_cumulativo_2 );
				}

				$line_2->set_values( $arrValorIndicador );
				$line_2->set_key( removeacentosGrafico("Valor Monetário R$".$EscalaTooltip) ,10);
				//adiciona o elemento linha 2 no gráfico 
				$chart->add_element( $line_2 );	
				
				/* Fim - Se o indicador possibilitar cumulatividade (S ou A) atribui-se os valores ao gráfico */
				
				$line_meta_valor2->set_values( $arrMetasvalorIndicador );
				$line_meta_valor2->set_key( "Meta de ".removeacentosGrafico("Valor Monetário R$".$EscalaTooltip) ,10);
				//adiciona o elemento linha 2 de meta no gráfico 
				$chart->add_element( $line_meta_valor2 );
				
				/* Início - Índice aplicado ao valor do indicador */
				if($indice_moeda && $arrDadosIndicador['indqtdevalor'] == "t" && $arrDadosIndicador['unmid'] != UNIDADEMEDICAO_MOEDA){
					$line_indice_2->set_values( $arrValorIndiceIndicador );
					$line_indice_2->set_key( removeacentosGrafico("Valor Monetário R$ ".$EscalaTooltip." (".$tooltipIndice.")") ,10);
					//adiciona o elemento linha 1 no gráfico 
					$chart->add_element( $line_indice_2 );
				}
				/* Fim - Índice aplicado ao valor do indicador */
				
			}
			/* FIM - ATRIBUI OS VALORES DA LINHA 2 */
			
			
			//informa a cor de background do grafico
			$chart->set_bg_colour( '#ffffff' );
			
			$y_legend = new y_legend( removeacentosGrafico($arrDadosIndicador['umedesc']) );
			$y_legend->set_style( '{font-size: 22px; color: #00000}' );
			
			$var = $chart->toPrettyString();
			$var = trataNC($var);
			echo $var;
			
			
		break;
		/* Fim - Gráfico Tipo = Meta */
		
		/* Início - Gráfico Tipo = Barra */
		case "barra":
			
			/* Início - Se o indicador possibilitar cumulatividade (S ou A) */
			if( ($arrDadosIndicador['indcumulativovalor'] == "S" || $arrDadosIndicador['indcumulativovalor'] == "A") && ($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A")){
				
				$bar_cumulativo_1 = new bar(); //Instância de barra
				
				$line_1_default_dot = new dot();
				$line_1_default_dot->colour('#006400');
				$line_1 = new line();
				$line_1->set_default_dot_style($line_1_default_dot);
				$line_1->set_width( 3 );
				$line_1->set_colour("#006400");
				
			}else{
				$line_1 = new bar(); //Instância de barra
			}
			/* Fim - Se o indicador possibilitar cumulatividade (S ou A) */
			
			/* Início - Se o indicador possibilitar aplicação de índices*/
			if($indice_moeda && $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA){
				
				$tipoLinhaIndice1 = new hollow_dot(); //instância um novo tipo de linha do gráfico
				$tipoLinhaIndice1->size(3)->halo_size(3)->colour('#191970')->tooltip("#val# ".removeAcentosGrafico($EscalaTooltip." ".$tooltipIndice).""); //atribui parametros de estilo a linha (tamanho,ponto,cor)
				
				/* Início - Criação da linha do gráfico */
				$line_indice_1 = new line(); //instância a linha
				$line_indice_1->set_default_dot_style($tipoLinhaIndice1); //atribui o tipo de linha criado
				$line_indice_1->set_width( 2 ); //indica a largura
				$line_indice_1->set_colour("#191970"); //indica a cor
				/* Fim - Criação da linha do gráfico */
				
			}
			/* Fim - Se o indicador possibilitar aplicação de índices*/
			
			
			/* Início - cria as variáveis usadas no foreach com valor zero*/
			$valorAcumulado = 0;
			$valorMonetarioAcumulado = 0;
			$valorIndiceAcumulado = 0;
			$valorIndiceMonetarioAcumulado = 0;
			/* Fim - cria as variáveis usadas no foreach com valor zero*/
			
			/* Início - Percorre o array de dados para criar os valores para o gráfico */
			foreach($arrValor as $arrV){
				
				//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
				$arrQtdeIndicador[] = round( (float)$arrV['qtde'] / $escala , 2) ;
				
				/* Início - Aplica cumulatividade Anual */
				if($arrDadosIndicador['indcumulativo'] == "A"){
					if(!$anoCorrente){
						$data = explode("-",$arrV['dpedatainicio']);
						$anoCorrente = $data[0];
					}else{
						$data = explode("-",$arrV['dpedatainicio']);
						if($anoCorrente != $data[0]){
							$valorAcumulado = 0;
							$anoCorrente = $data[0];
						}
					}
				}
				/* Fim - Aplica cumulatividade Anual */
				
				//variável para acumular as quantidades
				$valorAcumulado += round( (float)$arrV['qtde'] / $escala , 2) ;
				
				//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
				$arrQtdeAcumuladoIndicador[] = round( $valorAcumulado , 2) ;
								
				//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
				$arrPeriodos[] = $arrV['periodo'];
				
				/* Início - Se o indicador possibilitar aplicação de índices*/
				if($indice_moeda && $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA){
					$sql = "select ipcindice from painel.ipca where ipcstatus = 'A' and ipcano = '".date("Y",strtotime($arrV['dpedatainicio']))."'";
					$ipca = $db->pegaUm($sql);
					$ipca = !$ipca ? 1 : $ipca;
					
					//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
					$arrQtdeIndiceIndicador[] = round( (float)$arrV['qtde'] * $ipca / $escala , 2) ;
					
					/* Início - Aplica cumulatividade Anual */
					if($arrDadosIndicador['indcumulativo'] == "A"){
						if(!$anoCorrente){
							$data = explode("-",$arrV['dpedatainicio']);
							$anoCorrente = $data[0];
						}else{
							$data = explode("-",$arrV['dpedatainicio']);
							if($anoCorrente != $data[0]){
								$valorIndiceAcumulado = 0;
								$anoCorrente = $data[0];
							}
						}
					}
					/* Fim - Aplica cumulatividade Anual */
					
					//variável para acumular as quantidades
					$valorIndiceAcumulado += round( (float)$arrV['qtde'] * $ipca  / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrQtdeIndiceAcumuladoIndicador[] = round( $valorIndiceAcumulado , 2) ;
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
				
			}
			/* Fim - Percorre o array de dados para criar os valores para o gráfico */
			
			if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A")
				$max = getMaxValue($arrQtdeAcumuladoIndicador,$arrQtdeIndiceAcumuladoIndicador); //retorna o maior valor para criação do range do eixo y do gráfico
			else
				$max = getMaxValue($arrQtdeIndicador,$arrQtdeIndiceIndicador); //retorna o maior valor para criação do range do eixo y do gráfico
			
			//Instância do Eixo Y
			$eixo_y = new y_axis();
			//Range do Eixo Y
			$range = count($arrQtdeIndicador) == 0 ? 1 : ($max / 4);
			//Atribui os valores ao Eixo y
			$eixo_y->set_range( 0, $max , $range);
			
			//Definindo Label do Eixo X
			$x_labels = new x_axis_labels();
			//adiciona a cor dos elementos do eixo x
			$x_labels->set_colour( '#990000' );
			//adiciona os elementos do eixo x
			$x_labels->set_labels( removeAcentosGrafico($arrPeriodos) );
			//rotaciona o eixo x
			$x_labels->rotate(30);
			
			//Instância do Eixo X
			$eixo_x = new x_axis();
			//Adiciona a definição de label do eixo x
			$eixo_x->set_labels($x_labels) ;
			
			//Instância de novo gráfico
			$chart = new open_flash_chart();
			//adiciona o eixo x ao grafico
			$chart->set_x_axis( $eixo_x );
			//adiciona o eixo y ao grafico
			$chart->set_y_axis( $eixo_y );
												
			/* INÍCIO - ATRIBUI OS VALORES DA LINHA 1 */
			if($finac_qtde){
								
				/* Início - Se o indicador possibilitar cumulatividade (S ou A) atribui-se os valores ao gráfico */
				if($arrDadosIndicador['indcumulativo'] == "S" || $arrDadosIndicador['indcumulativo'] == "A"){
					
					if($bar_cumulativo_1){
						$bar_cumulativo_1->set_values( $arrQtdeAcumuladoIndicador );
						$bar_cumulativo_1->set_key( removeacentosGrafico($arrDadosIndicador['umedesc']." Acumulado(s)".$EscalaTooltip) ,10);
						//adiciona o elemento linha 1 no gráfico 
						$chart->add_element( $bar_cumulativo_1 );
					}
				}
				/* Fim - Se o indicador possibilitar cumulatividade (S ou A) atribui-se os valores ao gráfico */
				
				$line_1->set_values( $arrQtdeIndicador );
				$line_1->set_key( removeacentosGrafico($arrDadosIndicador['umedesc'].$EscalaTooltip) ,10);
				//adiciona o elemento linha 1 no gráfico 
				$chart->add_element( $line_1 );
				
				if($indice_moeda && $arrDadosIndicador['unmid'] == UNIDADEMEDICAO_MOEDA){
					$line_indice_1->set_values( $arrQtdeIndiceIndicador );
					$line_indice_1->set_key( removeacentosGrafico($arrDadosIndicador['umedesc'].$EscalaTooltip." ".$tooltipIndice) ,10);
					//adiciona o elemento linha 1 no gráfico 
					$chart->add_element( $line_indice_1 );
				}
				
			}
			/* FIM - ATRIBUI OS VALORES DA LINHA 1 */
			
			//informa a cor de background do grafico
			$chart->set_bg_colour( '#ffffff' );
			
			$y_legend = new y_legend( removeacentosGrafico($arrDadosIndicador['umedesc']) );
			$y_legend->set_style( '{font-size: 22px; color: #00000}' );
			
			$chart->set_y_legend( $y_legend );
			
			echo $chart->toPrettyString();
			
			
		break;
		/* Fim - Gráfico Tipo = Barra */
		
		/* Início - Gráfico Tipo = Barra_Fatia */
		case "barra_fatia":
			
			//Novo elemento barra
			$bar_stack = new bar_stack();
			
			$arrPeriodos = array();
			
			/* Início - Percorre o array de dados para criar os valores para o gráfico */
			foreach($arrValor as $dados){
				
				/* Início - cria as variáveis usadas no foreach com valor zero*/
				$valorAcumulado = 0;
				$valorMonetarioAcumulado = 0;
				$valorIndiceAcumulado = 0;
				$valorIndiceMonetarioAcumulado = 0;
				/* Fim - cria as variáveis usadas no foreach com valor zero*/
				
				foreach($dados as $tdiid => $arrV){
					
					//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
					$arrQtdeIndicador[ $arrV['periodo'] ][] = array("qtde" => (round( (float)$arrV['qtde'] / $escala , 2)),"tdiid" => $tdiid) ;
					
					//variável para acumular as quantidades
					$valorAcumulado += round( (float)$arrV['qtde'] / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrQtdeAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorAcumulado , 2) ;
					
					//variável para acumular as quantidades
					$valorMonetarioAcumulado += round( (float)$arrV['valor'] / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrValorAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorMonetarioAcumulado , 2) ;
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					$arrValorIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['valor']  / $escala , 2);
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					if(!in_array($arrV['periodo'],$arrPeriodos))
						$arrPeriodos[] = $arrV['periodo'];
					
					/* Início - Se o indicador possibilitar aplicação de índices*/
					if($indice_moeda){
						$sql = "select ipcindice from painel.ipca where ipcstatus = 'A' and ipcano = '".date("Y",strtotime($arrV['dpedatainicio']))."'";
						$ipca = $db->pegaUm($sql);
						$ipca = !$ipca ? 1 : $ipca;
						
						//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
						$arrQtdeIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['qtde'] * $ipca / $escala , 2) ;
						
						//variável para acumular as quantidades
						$valorIndiceAcumulado += round( (float)$arrV['qtde'] * $ipca  / $escala , 2) ;
						
						//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
						$arrQtdeIndiceAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorIndiceAcumulado , 2) ;
						
						//variável para acumular as quantidades
						$valorMonetarioIndiceAcumulado += round( (float)$arrV['valor'] * $ipca  / $escala , 2) ;
						
						//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
						$arrValorAcumuladoIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorMonetarioIndiceAcumulado , 2) ;
						
						//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
						$arrValorIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['valor'] * $ipca   / $escala , 2);
						
					}
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
			}
			/* Fim - Percorre o array de dados para criar os valores para o gráfico */
			
			foreach($arrQtdeIndicador as $arrQtde){
				$arrQtd = array();
				$soma = 0;
				foreach($arrQtde as $qtde){
					$arrQtd[] = new bar_stack_value( $qtde['qtde'] , getCorDetalhe($arrDetalhes,$qtde['tdiid']));
					$soma += $qtde['qtde'];
				}
				$arrValores[] = $soma;
				$bar_stack->append_stack( $arrQtd );
			}
			
			$bar_stack->set_tooltip( '#val# em #x_label# de um total de #total#' );
			
			$max = getMaxValue($arrValores); //retorna o maior valor para criação do range do eixo y do gráfico
			
			//Instância do Eixo Y
			$eixo_y = new y_axis();
			//Range do Eixo Y
			$range = ($max / 4);
			//Atribui os valores ao Eixo y
			$eixo_y->set_range( 0, $max , $range);
			
			//Definindo Eixo X
			$x_labels = new x_axis_labels();
			//adiciona a cor dos elementos do eixo x
			$x_labels->set_colour( '#990000' );
			//adiciona os elementos do eixo x
			$x_labels->set_labels( removeacentosGrafico($arrPeriodos) );
			//rotaciona o eixo x
			$x_labels->rotate(30);
			
			//Eixo x
			$x = new x_axis();
			//Adiciona a definição do eixo x
			$x->set_labels($x_labels) ;
			
			$tooltip = new tooltip();
			$tooltip->set_hover();
			
			$chart = new open_flash_chart();
			$chart->add_element( $bar_stack );
			$chart->set_x_axis( $x );
			$chart->add_y_axis( $eixo_y );
			$chart->set_bg_colour( '#ffffff' );
			$chart->set_tooltip( $tooltip );
			
			echo $chart->toPrettyString();
			
			
		break;
		/* Fim - Gráfico Tipo = Barra_Fatia */
		
		/* Início - Gráfico Tipo = Barra_Fatia */
		case "barra_comp":
						
			$arrPeriodos = array();
			
			/* Início - Percorre o array de dados para criar os valores para o gráfico */
			foreach($arrValor as $dados){
				
				/* Início - cria as variáveis usadas no foreach com valor zero*/
				$valorAcumulado = 0;
				$valorMonetarioAcumulado = 0;
				$valorIndiceAcumulado = 0;
				$valorIndiceMonetarioAcumulado = 0;
				/* Fim - cria as variáveis usadas no foreach com valor zero*/
				
				foreach($dados as $tdiid => $arrV){
					
					//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
					$arrQtdeIndicador[ $tdiid ] [ $arrV['periodo'] ] = array("qtde" => (round( (float)$arrV['qtde'] / $escala , 2)),"tdiid" => $tdiid) ;
					
					//variável para acumular as quantidades
					$valorAcumulado += round( (float)$arrV['qtde'] / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrQtdeAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorAcumulado , 2) ;
					
					//variável para acumular as quantidades
					$valorMonetarioAcumulado += round( (float)$arrV['valor'] / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrValorAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorMonetarioAcumulado , 2) ;
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					$arrValorIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['valor']  / $escala , 2);
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					if(!in_array($arrV['periodo'],$arrPeriodos))
						$arrPeriodos[] = $arrV['periodo'];
					
					/* Início - Se o indicador possibilitar aplicação de índices*/
					if($indice_moeda){
						$sql = "select ipcindice from painel.ipca where ipcstatus = 'A' and ipcano = '".date("Y",strtotime($arrV['dpedatainicio']))."'";
						$ipca = $db->pegaUm($sql);
						$ipca = !$ipca ? 1 : $ipca;
						
						//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
						$arrQtdeIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['qtde'] * $ipca / $escala , 2) ;
						
						//variável para acumular as quantidades
						$valorIndiceAcumulado += round( (float)$arrV['qtde'] * $ipca  / $escala , 2) ;
						
						//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
						$arrQtdeIndiceAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorIndiceAcumulado , 2) ;
						
						//variável para acumular as quantidades
						$valorMonetarioIndiceAcumulado += round( (float)$arrV['valor'] * $ipca  / $escala , 2) ;
						
						//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
						$arrValorAcumuladoIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorMonetarioIndiceAcumulado , 2) ;
						
						//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
						$arrValorIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['valor'] * $ipca   / $escala , 2);
						
					}
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
			}
			/* Fim - Percorre o array de dados para criar os valores para o gráfico */
			
			$chart = new open_flash_chart();
			
			$n = 0;
			foreach($arrQtdeIndicador as $chave => $detalhe){
				
				foreach($arrPeriodo as $periodo){
					$arrQtd[$chave][] = !$arrQtdeIndicador[$chave][$periodo]['qtde'] ? 0 : $arrQtdeIndicador[$chave][$periodo]['qtde'];
					$arrValores[] = !$arrQtdeIndicador[$chave][$periodo]['qtde'] ? 0 : $arrQtdeIndicador[$chave][$periodo]['qtde'];
				}
				
				$bar[ $chave ] = new bar();
				$bar[ $chave ]->set_colour( getCorDetalhe($arrDetalhes,$chave) );
				$bar[ $chave ]->set_values( $arrQtd[$chave] );
				$chart->add_element( $bar[ $chave ] );
				
				
			}
			
			
			$max = getMaxValue($arrValores); //retorna o maior valor para criação do range do eixo y do gráfico
			
			//Instância do Eixo Y
			$eixo_y = new y_axis();
			//Range do Eixo Y
			$range = ($max / 4);
			//Atribui os valores ao Eixo y
			$eixo_y->set_range( 0, $max , $range);
						
			//Definindo Eixo X
			$x_labels = new x_axis_labels();
			//adiciona a cor dos elementos do eixo x
			$x_labels->set_colour( '#990000' );
			//adiciona os elementos do eixo x
			$x_labels->set_labels( removeacentosGrafico($arrPeriodos) );
			//rotaciona o eixo x
			$x_labels->rotate(30);
			
			//Eixo x
			$x = new x_axis();
			//Adiciona a definição do eixo x
			$x->set_labels($x_labels) ;
			
			$tooltip = new tooltip();
			$tooltip->set_hover();
			
			$chart->set_x_axis( $x );
			$chart->add_y_axis( $eixo_y );
			$chart->set_bg_colour( '#ffffff' );
			$chart->set_tooltip( $tooltip );
			
			echo $chart->toPrettyString();
			
			
		break;
		/* Fim - Gráfico Tipo = Barra_Fatia */
		
		/* Início - Gráfico Tipo = Pizza */
		case "pizza":
		
			$arrPeriodos = array();
			
			/* Início - Percorre o array de dados para criar os valores para o gráfico */
			foreach($arrValor as $dados){
				
				/* Início - cria as variáveis usadas no foreach com valor zero*/
				$valorAcumulado = 0;
				$valorMonetarioAcumulado = 0;
				$valorIndiceAcumulado = 0;
				$valorIndiceMonetarioAcumulado = 0;
				/* Fim - cria as variáveis usadas no foreach com valor zero*/
				
				foreach($dados as $tdiid => $arrV){
					
					//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
					$arrQtdeIndicador[ $tdiid ] [ $arrV['periodo'] ] = array("qtde" => (round( (float)$arrV['qtde'] / $escala , 2)),"tdiid" => $tdiid) ;
					
					//variável para acumular as quantidades
					$valorAcumulado += round( (float)$arrV['qtde'] / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrQtdeAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorAcumulado , 2) ;
					
					//variável para acumular as quantidades
					$valorMonetarioAcumulado += round( (float)$arrV['valor'] / $escala , 2) ;
					
					//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
					$arrValorAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorMonetarioAcumulado , 2) ;
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					$arrValorIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['valor']  / $escala , 2);
					
					//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
					if(!in_array($arrV['periodo'],$arrPeriodos))
						$arrPeriodos[] = $arrV['periodo'];
					
					/* Início - Se o indicador possibilitar aplicação de índices*/
					if($indice_moeda){
						$sql = "select ipcindice from painel.ipca where ipcstatus = 'A' and ipcano = '".date("Y",strtotime($arrV['dpedatainicio']))."'";
						$ipca = $db->pegaUm($sql);
						$ipca = !$ipca ? 1 : $ipca;
						
						//array para armazenar os valores do indicador e atribuí-los a 1ª linha do gráfico
						$arrQtdeIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['qtde'] * $ipca / $escala , 2) ;
						
						//variável para acumular as quantidades
						$valorIndiceAcumulado += round( (float)$arrV['qtde'] * $ipca  / $escala , 2) ;
						
						//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
						$arrQtdeIndiceAcumuladoIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorIndiceAcumulado , 2) ;
						
						//variável para acumular as quantidades
						$valorMonetarioIndiceAcumulado += round( (float)$arrV['valor'] * $ipca  / $escala , 2) ;
						
						//array para armazenar os valores acumulados do indicador e atribuí-los a linha do gráfico
						$arrValorAcumuladoIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( $valorMonetarioIndiceAcumulado , 2) ;
						
						//array para armazenar os valores monetários do indicador e atribuí-los a 2ª linha do gráfico
						$arrValorIndiceIndicador[ $tdiid ] [ $arrV['periodo'] ][] = round( (float)$arrV['valor'] * $ipca   / $escala , 2);
						
					}
					
				}
				/* Fim - Se o indicador possibilitar aplicação de índices*/
				
			}
			/* Fim - Percorre o array de dados para criar os valores para o gráfico */
			
			$chart = new open_flash_chart();
			
			foreach($arrQtdeIndicador as $chave => $detalhe){
				
				foreach($arrPeriodo as $periodo){
					$arrQtd[$chave][] = !$arrQtdeIndicador[$chave][$periodo]['qtde'] ? 0 : $arrQtdeIndicador[$chave][$periodo]['qtde'];
					$arrValores[] = !$arrQtdeIndicador[$chave][$periodo]['qtde'] ? 0 : $arrQtdeIndicador[$chave][$periodo]['qtde'];
				}
				
			}
			foreach($arrQtdeIndicador as $chave => $detalhe){
				$arrCores[] = getCorDetalhe($arrDetalhes,$chave);
				$arrPizza[] = new pie_value( array_sum($arrQtd[$chave]) , round((array_sum($arrQtd[$chave]) * 100) / array_sum($arrValores),2)."%" );
			}
			
			
			$pie = new pie();
			$pie->set_alpha(1.0);
			
			$pie->alpha(1.0);
    		$pie->add_animation( new pie_fade() );
    		$pie->add_animation( new pie_bounce(5) );
			
			$pie->set_start_angle( 35 );
			$pie->add_animation( new pie_fade() );
			$pie->set_tooltip( '#val# de #total#<br>#percent# de 100%' );
			$pie->set_colours( $arrCores );
			$pie->set_values( $arrPizza );
			
			$chart = new open_flash_chart();
			
			$chart->add_element( $pie );
						
			$chart->set_bg_colour( '#ffffff' );
			
			$chart->x_axis = null;
			
			echo $chart->toPrettyString();
			
		break;
		/* Fim - Gráfico Tipo = Pizza */
		
		
	}
	
}
/* FIM - FUNÇÃO PARA CRIAÇÃO DO GRÁFICO */

function getCorDetalhe($arrDetalhes,$chave){
	
	$arrCores = array('#6495ED','#66CDAA','#990000','#FFD700','#CDC8B1',' #000000','#FF0000','#008B45','#8B008B','#FFE4E1','#0000FF',' #7CFC00','#8B4513','#FF1493','#00FF00','#00008B','#7FFFD4','#8B8B00','#FF6A6A','#8B1A1A','#8B0A50','#828282');
	$n = 0;
	foreach($arrDetalhes as $detalhe){
		$arrCoresDetalhe[$detalhe['tidid']] = $arrCores[$n];
		$n++;
	}
	return $arrCoresDetalhe[$chave];
	
}

function getMaxValue($arrValores1 = array(),$arrValores2 = array()){
	
	if(is_array($arrValores1)){
		foreach($arrValores1 as $val1){
			if(is_object($val1)){
				$arrV1[] = $val1->value;
			}else{
				$arrV1[] = $val1;
			}
		}
	}else{
		$arrV1 = array(0);
	}
	
	if(is_array($arrValores2)){
		foreach($arrValores2 as $val2){
			if(is_object($val2)){
				$arrV2[] = $val2->value;
			}else{
				$arrV2[] = $val2;
			}
		}
	}else{
		$arrV2 = array(0);
	}
	$arrValores1 = $arrV1;
	$arrValores2 = $arrV2;
	
	if(is_array($arrValores1)){
		$max = max($arrValores1);
	}else{
		$arrValores1 = array(0);
		$max = 1;
	}
	
	if($arrValores2 && is_array($arrValores2)){
		$max2 = max($arrValores2);
	}else{
		$arrValores2 = array(0);
		$max2 = 1;
	}
	
	if($max >= $max2)
		$maxVal = $max;
	else
		 $maxVal = $max2;
	
	$arrMax = explode(".",$maxVal);
	$MaxValue = substr($arrMax[0],0,1);
	$MaxValue = $MaxValue + 1;
	for($i = 1; $i < strlen($arrMax[0]);$i++){
		$MaxValue .= "0";
	}
	
	return $MaxValue;
	
	
}

function criaGraficoIndicadorNaoAtendido(){
	
	include ("../../includes/open_flash_chart/open-flash-chart.php");
	include ("../../includes/open_flash_chart/ofc_sugar.php");
	
	$data_1 = array(0);
	
	$title = new title( "Nao Atendido" );
	
	$line_1_default_dot = new dot();
	$line_1_default_dot->colour('#f00000');
	
	$line_1 = new line();
	$line_1->set_default_dot_style($line_1_default_dot);
	$line_1->set_values( $data_1 );
	$line_1->set_width( 1 );
	
	$y = new y_axis();
	$y->set_range( 0, 10, 4 );
	
	$chart = new open_flash_chart();
	$chart->set_title( $title );
	$chart->add_element( $line_1 );
	$chart->set_y_axis( $y );
	$chart->set_bg_colour( '#ffffff' );
	
	echo $chart->toPrettyString();
	
}


function criaGraficoIndicadorRefinePesquisa($msg = "Por favor, refine sua busca e tente novamente!"){
	
	include ("../../includes/open_flash_chart/open-flash-chart.php");
	include ("../../includes/open_flash_chart/ofc_sugar.php");
	
	$data_1 = array(0);
	
	$title = new title( removeAcentosGrafico($msg) );
	
	$line_1_default_dot = new dot();
	$line_1_default_dot->colour('#f00000');
	
	$line_1 = new line();
	$line_1->set_default_dot_style($line_1_default_dot);
	$line_1->set_values( $data_1 );
	$line_1->set_width( 1 );
	
	$y = new y_axis();
	$y->set_range( 0, 10, 4 );
	
	$chart = new open_flash_chart();
	$chart->set_title( $title );
	$chart->add_element( $line_1 );
	$chart->set_y_axis( $y );
	$chart->set_bg_colour( '#ffffff' );
	
	echo $chart->toPrettyString();
	
}

function removeAcentosGrafico ($var)
{
       $ACENTOS   = array("À","Á","Â","Ã","à","á","â","ã");
       $SEMACENTOS= array("A","A","A","A","a","a","a","a");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
      
       $ACENTOS   = array("È","É","Ê","Ë","è","é","ê","ë");
       $SEMACENTOS= array("E","E","E","E","e","e","e","e");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
       $ACENTOS   = array("Ì","Í","Î","Ï","ì","í","î","ï");
       $SEMACENTOS= array("I","I","I","I","i","i","i","i");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
      
       $ACENTOS   = array("Ò","Ó","Ô","Ö","Õ","ò","ó","ô","ö","õ");
       $SEMACENTOS= array("O","O","O","O","O","o","o","o","o","o");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
     
       $ACENTOS   = array("Ù","Ú","Û","Ü","ú","ù","ü","û");
       $SEMACENTOS= array("U","U","U","U","u","u","u","u");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);
       $ACENTOS   = array("Ç","ç","ª","º","°");
       $SEMACENTOS= array("C","c","a.","o.","o.");
       $var=str_replace($ACENTOS,$SEMACENTOS, $var);      

       return $var;
}

function trataNC($string){
	if(strstr($string,"e+")){
		$valor = trim(substr($string,strpos($string,"e+")-6,9));
		$d = explode("e+",$valor);
		$num = $d[0];
		$casas = $d[1];
		if(strstr($num,".")){ //se houver ponto
			$c = explode(".",$num);
			$sub_casas = strlen($c[1]);
			$num = str_replace(".","",$num);
			$casas = (int)$casas - (int)$sub_casas;
		}
		unset($valorFinal);
		$valorFinal = $num;
		for($i=0;$i<$casas;$i++){
			$valorFinal.="0";
		}
		$string = str_replace($valor,$valorFinal,$string);
		$string = trataNC($string);
	}
	return $string;
}
?>