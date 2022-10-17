<html>
<head>
<style>
 .tp_grafico{background-color: #FCFFCC; border: solid 1px #FCCFFF;width: 100%;height: 100%;text-align: center;cursor: pointer;}
    
    @media print {
      .noprint { display: none; }
    }

</style>
<?php

function gerarCor() {
	$p = sprintf("%02s",dechex(mt_rand(0,255)));
	$s = sprintf("%02s",dechex(mt_rand(0,255)));
	$t = sprintf("%02s",dechex(mt_rand(0,255)));
	return $p.$s.$t;
}


/*Funções para montar os graficos*/
function geraGrafico($arrIndid){	
	
	//comparacao_+ arrIndid
	foreach($arrIndid as $indid){
		$xml .= trim($indid);
	}
	
	$arquivo_xml = $xml."_".$indid.".xml";
	
	$caminho = '/graficos/painel/xml/';
	$script = ("<script type=\"text/javascript\">
	var grafico_$ind = new FusionCharts(\"/includes/FusionCharts/FusionChartsFree/Charts/FCF_MSLine.swf\", \"grafico\", \"750\", \"420\", \"0\", \"0\");
	grafico_$ind.setDataURL('{$caminho}$arquivo_xml');
	grafico_$ind.render(\"grafico\");
   </script>");
   
   return $script;
}

function geraXML_Grafico($arrIndid){
	global $db;
	if(!is_dir(APPRAIZ."www/graficos")) {
		mkdir(APPRAIZ."www/graficos", 0777);
	}
	if(!is_dir(APPRAIZ."www/graficos/painel")) {
		mkdir(APPRAIZ."www/graficos/painel", 0777);
		mkdir(APPRAIZ."www/graficos/painel/xml", 0777);
	}
	if(!is_dir(APPRAIZ."www/graficos/painel/xml")) {
		mkdir(APPRAIZ."www/graficos/painel/xml", 0777);
	}
	$caminho = APPRAIZ."www/graficos/painel/xml/"; //
	
	//comparacao_+ arrIndid
	foreach($arrIndid as $indid){
		$xml .= trim($indid);
	}
	$arquivo_xml = $xml."_".$indid.".xml";
	
	//formação dos selects com in(...)
	$indid = implode(',',$arrIndid);
	
	$sql = "select min(sehvalor) as valor from painel.seriehistorica where indid in ($indid) and sehstatus = 'A'";
	$min = $db->pegaUm($sql); 
	
	($min == "")? $min = 0 : $min = $min;
	
	$sql = "select max(sehvalor) as valor from painel.seriehistorica where indid in ($indid) and sehstatus = 'A'";
	$min = $db->pegaUm($sql);
	
	($max == "")? $max = 100 : $max = $max;
	
	$divlines = count($arrIndid) * count($arrIndid);
	
	//Subtítulo (Tipo, de data1 até data2
	$sql = "select 
				max(data) 
			from
				(
					(	select 
							max((to_char(sehdtcoleta, 'YYYY-MM-DD'))) as data 
						from 
							painel.seriehistorica 
						where 
							indid in ($indid) 
						and 
							sehstatus <> 'I'
						and
							dpeid is null
					)
					union 
					(
						select 
							max((to_char(dpe.dpedatainicio, 'YYYY-MM-DD'))) as data 
						from 
							painel.detalheperiodicidade dpe
						inner join
							painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
						where 
							seh.indid in ($indid) 
						and 
							seh.sehstatus <> 'I'
					)
				) as maxData";
	$maxData = $db->pegaUm($sql); 
	
	$sql = "select 
				min(data) 
			from
				(
					(	select 
							min((to_char(sehdtcoleta, 'YYYY-MM-DD'))) as data 
						from 
							painel.seriehistorica 
						where 
							indid in ($indid) 
						and 
							sehstatus <> 'I'
						and
							dpeid is null
					)
					union 
					(
						select 
							min((to_char(dpe.dpedatainicio, 'YYYY-MM-DD'))) as data 
						from 
							painel.detalheperiodicidade dpe
						inner join
							painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
						where 
							seh.indid in ($indid) 
						and 
							seh.sehstatus <> 'I'
					)
				) as minData";
	$minData = $db->pegaUm($sql);
		
	$sql = "select 
				distinct(umedesc) 
			from 
				painel.indicador i
			left join 
				painel.unidademeta u on i.umeid = u.umeid
			where 
				indid in ($indid)";
	$umedesc = $db->pegaUm($sql);
	
	($umedesc == "")? $umedesc = "Valor / Quatidade" : $umedesc = $umedesc;
	
	$subtitulo = "$umedesc de ".date('d/m/Y',strtotime($minData))." a ".date('d/m/Y',strtotime($maxData));
	
	$sql = "select 
				distinct(data) 
			from
				(
					(	select 
							distinct(to_char(sehdtcoleta, 'YYYY-MM-DD')) as data 
						from 
							painel.seriehistorica 
						where 
							indid in ($indid) 
						and 
							sehstatus <> 'I'
						and
							dpeid is null
					)
					union 
					(
						select 
							distinct(to_char(dpe.dpedatainicio, 'YYYY-MM-DD')) as data 
						from 
							painel.detalheperiodicidade dpe
						inner join
							painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
						where 
							seh.indid in ($indid) 
						and 
							seh.sehstatus <> 'I'
					)
				) as datas
			order by
				data asc;";
	$arrDatas = $db->carregar($sql);
		
	$tituloGrafico = "GRÁFICO COMPARATIVO";
		
	$conteudo_xml = ("	
						<graph caption='$tituloGrafico' subcaption='$subtitulo' hovercapbg='FFECAA' hovercapborder='F47E00' formatNumberScale='0' decimalPrecision='0' showvalues='0' numdivlines='$divlines' numVdivlines='0' yaxisminvalue='0' yaxismaxvalue='$max'  rotateNames='1'>
			<categories > ");
			
			foreach($arrDatas as $referencia){
				$conteudo_xml .=  "<category name='".date('d/m/Y',strtotime($referencia['data']))."' />" ;
			}
			
			$conteudo_xml .=  "</categories>";
			foreach($arrIndid as $indid){
				
				$sql = "select 
							indnome,
							perid 
						from 
							painel.indicador 
						where 
							indid = $indid 
						and 
							indstatus <> 'I'";
							
				$ind = $db->pegaLinha($sql);
				
				$cor = gerarCor();
				
				$conteudo_xml .=  ("
				<dataset seriesName='{$ind['indnome']}' color='$cor' anchorBorderColor='$cor' anchorBgColor='$cor'> ");
				
				if($ind['perid'] != 5){
					$sql = "select
								to_char(dpe.dpedatainicio, 'YYYY-MM-DD') as data,
								seh.sehqtde
							from
								painel.seriehistorica seh
							inner join
								painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
							where
								seh.indid = $indid
							and
								sehstatus <> 'I'";
				}else{
					$sql = "select 
							sehqtde,
							to_char(sehdtcoleta, 'YYYY-MM-DD') as data
						from 
							painel.seriehistorica 
						where 
							indid = $indid
						and
							dpeid is null 
						and 
							sehstatus <> 'I'";
				}						
				$seh = $db->carregar($sql);
				if($seh){	
					foreach($seh as $seh2){
						$conteudo_xml .=  "<set value='{$seh2['sehqtde']}' />";
						//corrigir os pontos respectivos às datas de cada indicador.
					}
				}
				$conteudo_xml .=  ("</dataset>") ;
			}
	$conteudo_xml.= "</graph>";
	$xml = fopen($caminho.$arquivo_xml, 'w');
	fwrite($xml, $conteudo_xml);
	fclose($xml);
}


function criaGraficos($arrIndid){
	
	geraXML_Grafico($arrIndid);
	$graficos = geraGrafico($arrIndid);
	echo $graficos;	
}


function montaGraficos($arrIndid){
	
	$caminho = "/graficos/painel/xml/"; //
	
	echo ("
		<script type=\"text/javascript\" src=\"/includes/FusionCharts/FusionCharts.js\"></script>
		<script type=\"text/javascript\">
		<!--
			var d = document;
		
			function escondeGraficos(){
				var div = d.getElementById('exibe_graficos');
				d.getElementById('mais_graficos').style.display = \"\";
				d.getElementById('menos_graficos').style.display = \"none\";
				div.style.display = \"none\";
			}
			
			function exibeGraficos(){
				var div = d.getElementById('exibe_graficos');
				window.location.reload();
			}
		//-->
		</script>");
			
	echo ("<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
			<tr>
				<td>
					<div id=\"exibe_graficos\"></div>
				</td>
			</tr>
		</table>
		");
	
	if(!$arrIndid){
		echo "<script>document.getElementById('grafico').innerHTML = 'Indicadores não informados.';</script>";
		return;
	}
	
	criaGraficos($arrIndid);
	
}

?>

<title>Série Histórica</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

</head>
<body>
<?
if(!$_GET['indid']){
	echo "<div style=\"text-align:center;color:#AA0000\" >Indicadores Inexistentes.</div>";
	die;
}
?>
<div id="grafico" style="float:left;border:1px solid #AAAAAA;padding: 5px;text-align: center" >
<?
	$arrID = explode(',',$_GET['indid']);
	montaGraficos($arrID);
?>
</div>
</body>
</html>