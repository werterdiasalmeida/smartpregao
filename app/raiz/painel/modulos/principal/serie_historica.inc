<html>
<head>
<script language="javascript" type="text/javascript" src="/includes/JsLibrary/tags/superTitle.js"></script>
<script language="javascript" type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
<script type="text/javascript">
	
	function exibeDetalhes(){
		var img_mais = document.getElementById('img_mais_ind');
		var img_menos = document.getElementById('img_menos_ind');
		var div_ind = document.getElementById('detalhe_indicador');
		div_ind.style.display = '';
		img_mais.style.display = 'none';
		img_menos.style.display = '';
	}
	
	function escondeDetalhes(){
		var img_mais = document.getElementById('img_mais_ind');
		var img_menos = document.getElementById('img_menos_ind');
		var div_ind = document.getElementById('detalhe_indicador');
		div_ind.style.display = 'none';
		img_mais.style.display = '';
		img_menos.style.display = 'none';
	}
	
	
</script>
<style>
 .tp_grafico{background-color: #FCFFCC; border: solid 1px #FCCFFF;width: 100%;height: 100%;text-align: center;cursor: pointer;}
    
    @media print {
      .noprint { display: none; }
    }

</style>
<?php

/*Funções para montar os graficos*/
function geraGrafico($acaid,$ind){	
	global $db;
	$arquivo_xml = $acaid."_".$ind.".xml";
	
	###################### Verifica se a quantidade do indicadore é cumulativa, se for, utiliza o gráfico FCF_MSColumn3DLineDY / se não for utiliza FCF_Line
	$sql = "select indcumulativo from painel.indicador where indid = $ind";
	$cumulativo = $db->pegaUm($sql);
	if($cumulativo =="t"){
		$tipo_grafico = "FCF_MSColumn3DLineDY";
	}else{
		$tipo_grafico = "FCF_Line";
	}
		
	$caminho = '/graficos/painel/xml/';
	$script = ("<script type=\"text/javascript\">
	var grafico_$ind = new FusionCharts(\"/includes/FusionCharts/FusionChartsFree/Charts/$tipo_grafico.swf\", \"grafico\", \"570\", \"300\", \"0\", \"0\");
	grafico_$ind.setDataURL('{$caminho}$arquivo_xml');
	grafico_$ind.render(\"grafico\");
   </script>");
   
   return $script;
}

function geraXML_Grafico($acaid,$indid,$EixoXnome,$EixoYnome){
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
	//acaid + indicador + ano
	$arquivo_xml = $acaid."_".$indid.".xml";
	
	$sql = "select 
				min(sehvalor) as valor
			from 
				painel.seriehistorica 
			where 
				indid = $indid 
			and 
				sehstatus <> 'I'";
	$min = $db->pegaUm($sql);
	
	($min == "")? $min = 0 : $min=$min;
	
	$sql = "select 
				* 
			from 
				painel.indicador 
			where 
				indid = $indid 
			and 
				indstatus = 'A'";
	$indicador = $db->pegaLinha($sql);
	$tituloGrafico = strtoupper($indicador['indnome']);
	
	$sql = "select 
				umedesc 
			from 
				painel.indicador i
			left join 
				painel.unidademeta u on i.umeid = u.umeid
			where 
				indid = $indid";
	$umedesc = $db->pegaUm($sql);
	($umedesc == "")? $umedesc = "Valor / Quatidade" : $umedesc = $umedesc;
	
	$sql = "select 
				perdsc 
			from 
				painel.indicador i
			left join 
				painel.periodicidade p on i.perid = p.perid
			where 
				indid = $indid";
	$perdsc = $db->pegaUm($sql);
	
	switch($perdsc){
		case "Diária":
			$tp_data = "d/m/y";
			break;
		case "Mensal":
			$tp_data = "m/Y";
			break;
		case "Semestral":
			$tp_data = "m/Y";
			break;
		case "Anual":
			$tp_data = "Y";
			break;
		default:
			$tp_data = "d/m/y";
			break;
	}
	
	$sql = "select 
				unmdesc 
			from 
				painel.indicador i
			left join 
				painel.unidademedicao u on i.unmid = u.unmid
			where 
				indid = $indid";
	$unmedsc = $db->pegaUm($sql);
	
	switch($unmedsc){
		case "Número inteiro":
			$tp_valor = "sehqtde::integer ";
			$decimalPrecision = "0";
			break;
		case "Percentual":
			$tp_valor = "sehqtde";
			
			break;
		case "Razão":
			$tp_valor = "sehqtde";
			break;
		case "Número índice":
			$tp_valor = "sehqtde";
			$decimalPrecision = "1";
			break;
		case "Moeda":
			$tp_valor = "sehqtde";
			$decimalPrecision = "0";
			break;
		default:
			$tp_valor = "sehqtde";
			$decimalPrecision = "0";
			break;
	}
	
	$acentos = array("á", "é", "í","ó","ú","â","ê","ô","à","è","ì","ò","ù","Á","É","Í","Ó","Ú","À","È","Ì","Ò","Ù","ã","õ","Ã","Õ","ç","Ç");
	$sem_acentos = array("a", "e", "i","o","u","a","e","o","a","e","i","o","u","A","E","I","O","U","A","E","I","O","U","a","o","A","O","c","C");

	$umedesc = str_replace($acentos, $sem_acentos, $umedesc);
	
	###################### Verifica se a quantidade do indicadore é cumulativa, se for, utiliza o gráfico FCF_MSColumn3DLineDY / se não for utiliza FCF_Line
	$sql = "select indcumulativo from painel.indicador where indid = $indid";
	$cumulativo = $db->pegaUm($sql);
		
	if($cumulativo =="t"){
		
		$conteudo_xml = ("
				<graph caption='' PYAxisName='$umedesc Cumulativo(s)' SYAxisName='$umedesc Nao Cumulativo(s)'
						 numberPrefix='' showvalues='0'  numDivLines='4' formatNumberScale='0' decimalPrecision='$decimalPrecision'
						anchorSides='10' anchorRadius='3' anchorBorderColor='009900'>
				
				
			");
		
			if($perdsc == "Diária"){
					$sql = "select 
								sehid, 
								$tp_valor as sehqtde,
								(to_char(sehdtcoleta, 'YYYY-MM-DD')) as data 
							from 
								painel.seriehistorica 
							where 
								indid = $indid 
							and 
								sehstatus <> 'I' 
							order by 
								data";
				}else{
					$sql = "select 
								seh.sehid,
								$tp_valor as sehqtde,
								(to_char(dpe.dpedatainicio, 'YYYY-MM-DD')) as data
							from 
								painel.detalheperiodicidade dpe
							inner join
								painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
							where 
								seh.indid = $indid
							and 
								seh.sehstatus <> 'I'
							order by 
								data";
				}
					$dados = $db->carregar($sql);
					
			if($dados){
				
				//Criação das Datas
				$conteudo_xml.= "<categories>";
				foreach($dados as $seh){
					$data = date($tp_data,strtotime($seh['data']));
					$valor = (float)$seh['sehqtde'];
					$conteudo_xml.= "<category name='$data' />";
				}
				$conteudo_xml.= "</categories>";
				
				//Criação do valor cumulativo
				$conteudo_xml.= "<dataset seriesName='Quantidade Cumulativa' color='AFD8F8' showValues='0'>";
				foreach($dados as $seh){
					$data = date($tp_data,strtotime($seh['data']));
					$valor1 = (float)$seh['sehqtde'];
					$soma = $soma + $valor1;
					$conteudo_xml.= "<set value='$soma' />";
				}
				$conteudo_xml.= "</dataset>";
				
				//Criação do valor não cumulativo
				$conteudo_xml.= "<dataset seriesName='Quantidade Não Cumulativa' color='8BBA00' showValues='0' parentYAxis='S' >";
				foreach($dados as $seh){
					$data = date($tp_data,strtotime($seh['data']));
					$valor = (float)$seh['sehqtde'];
					$conteudo_xml.= "<set value='$valor' />";
				}
				$conteudo_xml.= "</dataset>";
				
				$conteudo_xml.= "</graph>";
								
			}
		
	}else{
		
		$conteudo_xml = ("	
				<graph caption='' subcaption='' xAxisName='Referencia' yAxisMinValue='0' yAxisName='$umedesc' decimalPrecision='$decimalPrecision' formatNumberScale='0' numberPrefix='' showNames='1' showValues='0' showAlternateHGridColor='1' AlternateHGridColor='ff5904' divLineColor='ff5904' divLineAlpha='20' alternateHGridAlpha='5'>
			");
			
				if($perdsc == "Diária"){
					$sql = "select 
								sehid, 
								$tp_valor as sehqtde,
								(to_char(sehdtcoleta, 'YYYY-MM-DD')) as data 
							from 
								painel.seriehistorica 
							where 
								indid = $indid 
							and 
								sehstatus <> 'I' 
							order by 
								data";
				}else{
					$sql = "select 
								seh.sehid,
								$tp_valor as sehqtde,
								(to_char(dpe.dpedatainicio, 'YYYY-MM-DD')) as data
							from 
								painel.detalheperiodicidade dpe
							inner join
								painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
							where 
								seh.indid = $indid
							and 
								seh.sehstatus <> 'I'
							order by 
								data";
				}
					$dados = $db->carregar($sql);
					
			if($dados){
				
				foreach($dados as $seh){
					$data = date($tp_data,strtotime($seh['data']));
					$valor = (float)$seh['sehqtde'];
					$conteudo_xml.= "<set name='$data' value='$valor' hoverText='$data'/>";
				}
			}
			else{
				echo "<script>document.getElementById('grafico').innerHTML = 'Indicadores não informados.';</script>";
				return;
			}
			
		$conteudo_xml.= "</graph>";
	}
		
	$xml = fopen($caminho.$arquivo_xml, 'w');
	fwrite($xml, $conteudo_xml);
	fclose($xml);
}


function criaGraficos($acaid,$indid){
	
	geraXML_Grafico($acaid,$indid,'Referência','Valor');
	$graficos = geraGrafico($acaid,$indid);
	echo $graficos;	
}


function montaGraficos($acaid,$indid){
	
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
			
	echo ("<table style=\"display:none\" class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
			<tr>
				<td>
					<div id=\"menu_graficos\" style=\"font-weight: bold;\"><img style=\"cursor:pointer;\" onclick=\"escondeGraficos();\" src=\"../imagens/menos.gif\" id=\"menos_graficos\" /><img style=\"cursor:pointer;display:none\" onclick=\"exibeGraficos();\" src=\"../imagens/mais.gif\" id=\"mais_graficos\" /> Gráficos</div>
					<div id=\"exibe_graficos\"></div>
				</td>
			</tr>
		</table>
		");
	
	if(!$acaid || !$indid){
		echo "<script>document.getElementById('grafico').innerHTML = 'Indicadores não informados.';</script>";
		return;
	}
	
	criaGraficos($acaid,$indid);
	
}

?>

<title>Série Histórica</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

</head>
<body>
<?
if($_GET['indid']){
	$sql = "select e.exodsc from painel.indicador i
		left join painel.eixo e on i.exoid = e.exoid
			where indid = {$_GET['indid']}";
	
	$eixo = $db->pegaUm($sql);
	
	$sql = "select a.acadsc from painel.indicador i
		left join painel.acao a on i.acaid = a.acaid
			where indid = {$_GET['indid']}";
	
	$acao = $db->pegaUm($sql);
	
	$sql = "select indnome from painel.indicador 
			where indid = {$_GET['indid']}";
	
	$indnome = $db->pegaUm($sql);
	
}else{
	echo "<div style=\"text-align:center;color:#AA0000\" >Indicadores Inexistentes.</div>";
	die;
}

?>
<div style='width:757;border-bottom:1px solid;font-size: 18px;font-weight: bold;margin-bottom: 5px' ><?=$eixo;?></div>
<div style='width:757;font-size: 14px;font-weight: bold;margin-bottom: 5px' ><?=$acao;?></div>
<div style='width:757;font-size: 12px;font-weight: bold;margin-bottom: 5px;text-align: center' ><?=$indnome;?></div>
<div>
<div style="float:left;border:1px solid #AAAAAA;width: 77%;height: 310px;">
<div id="grafico">
</div>
<?
if($_GET['indid']){
		$sql = "select 
					* 
				from 
					painel.seriehistorica 
				where 
					indid = {$_GET['indid']} 
				and 
					sehstatus <> 'I'";
		$dados = $db->carregar($sql);
	
		if(!$dados){
			echo "Indicadores não cadastrados.";
		}
		else{
			
			###################### Verifica se a quantidade do indicadore é cumulativa, se for, utiliza o gráfico FCF_MSColumn3DLineDY / se não for utiliza FCF_Line
			$sql = "select indcumulativo from painel.indicador where indid = {$_GET['indid']}";
			$cumulativo = $db->pegaUm($sql);
			if($cumulativo =="t"){?>
				<script type="text/javascript">
						swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico", "600", "310", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipo=<?php echo $_GET['indid'] ?>;barra","loading":"Carregando gráfico..."} );
				</script>
			<?php
			}else{
				montaGraficos($_GET['acaid'],$_GET['indid']);
			}
		}
}
?>
</div>
<div id="historico" style="float:right;border: 1px solid #AAAAAA;margin-left:5px;height:80%;width: 20%;padding:5px 5px 5px 5px;overflow: auto;" >
<table style="display: none" width="100%">
	<tr><td><div class="tp_grafico" >1d</div></td><td><div class="tp_grafico">1s</div></td><td><div class="tp_grafico">1m</div></td><td><div class="tp_grafico">3m</div></td><td><div class="tp_grafico">6m</div></td><td><div class="tp_grafico">1a</div></td></tr>
</table>
<?

if($dados){
	
	$sql = "select 
				umedesc 
			from 
				painel.indicador i
			left join 
				painel.unidademeta u on i.umeid = u.umeid
			where 
				indid = {$_GET['indid']}";
	$umedesc = $db->pegaUm($sql);
	($umedesc == "")? $umedesc = "Valor / Qtd." : $umedesc = $umedesc;
	
	$sql = "select 
				unmdesc 
			from 
				painel.indicador i
			left join 
				painel.unidademedicao u on i.unmid = u.unmid
			where 
				indid = {$_GET['indid']}";
	$unmdesc = $db->pegaUm($sql);
	($unmdesc == "")? $unmdesc = "Razão" : $unmdesc = $unmdesc;
	
	$sql = "select
				indqtdevalor,
				perid
			from
				painel.indicador
			where 
				indid = {$_GET['indid']}";
	
	$ind = $db->pegaLinha($sql);
	
	if($ind['perid'] != 5){
		$data = "dpe.dpedsc as data";
		$order = "order by 
						dpe.dpedatainicio desc";
	}else{
		$data = "(to_char(seh.sehdtcoleta, 'DD/MM/YYYY')) as data";
		$order = "order by
						data desc";
	}
	
	if($ind['indqtdevalor'] == 't' && $unmdesc != "Moeda"){
		$sql = "select
						$data,
						coalesce(sehqtde,0) as sehqtde,
						coalesce(sehvalor,0) as sehvalor
				from 
					painel.seriehistorica seh
				left join
					painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
				where
					seh.indid = {$_GET['indid']} 
				and 
					seh.sehstatus <> 'I'
				$order";
	
		$cabecalho = array("Referência",$umedesc,"Valor");

		$dados = $db->carregar($sql);
		
		if( $dados ){
			
			foreach($dados as $chave => $val){	
				
				//Verifica o tipo de medição e aplica as regras
				switch($unmdesc){
					case "Número inteiro":
						$val['sehqtde'] = str_replace(",0","",number_format( $val['sehqtde'], 1, ',', '.'))." ";					
						break;
					case "Percentual":
						$val['sehqtde'] = number_format($val['sehqtde'],2,'.','')."%";
						break;
					case "Razão":
						$val['sehqtde'] = number_format($val['sehqtde'],2,'.','');
						break;
					case "Número índice":
						$val['sehqtde'] = number_format($val['sehqtde'],2,'.','');
						break;
					case "Moeda":
						$val['sehqtde'] = number_format($val['sehqtde'],2,',','.');
						break;
					default:
						$val['sehqtde'] = $val['sehqtde'];
						break;
				}
				
				
				$dados_array[$chave] = array("data" => $val['data'],
											 "sehqtde" => "<div style=\"width:100%;text-align:right;color:#0066CC\" >".$val['sehqtde']."</div>",
											 "valor" => $val['sehvalor']);
			}
		}
		
	}
	else{
		$sql = "select
						$data,
						coalesce(sehqtde,0) as sehqtde 
				from 
					painel.seriehistorica seh
				left join
					painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
				where
					indid = {$_GET['indid']} 
				and
					sehstatus <> 'I'
				$order";
	
		$cabecalho = array("Referência",$umedesc);
		
		
		
		$dados = $db->carregar($sql);
		
		if( $dados ){
			
			foreach($dados as $chave => $val){

				//Verifica o tipo de medição e aplica as regras
				switch($unmdesc){
					case "Número inteiro":
						$val['sehqtde'] = str_replace(",0","",number_format( $val['sehqtde'], 1, ',', '.'))." ";
						break;
					case "Percentual":
						$val['sehqtde'] = number_format($val['sehqtde'],2,'.','')."%";
						break;
					case "Razão":
						$val['sehqtde'] = number_format($val['sehqtde'],2,'.','');
						break;
					case "Moeda":
						$val['sehqtde'] = number_format($val['sehqtde'],2,',','.');
						break;
					case "Número índice":
						$val['sehqtde'] = number_format($val['sehqtde'],2,'.','');
						break;
					default:
						$val['sehqtde'] = $val['sehqtde'];
						break;
				}
				
				$dados_array[$chave] = array("data" => $val['data'],
											 "sehqtde" => "<div style=\"width:100%;text-align:right;color:#0066CC\" >".$val['sehqtde']."</div>");
			}
		}
	}
	
	$db->monta_lista_array($dados_array, $cabecalho, 50, 20, '', 'center', '');
	?>
	
	
<?} ?>
</div>

<?
if($_GET['indid']){
	$sql = "select 
				i.indfontetermo,
				un.umedesc,
				tp.tpidsc,
				sec.secdsc,
				i.indobjetivo,
				i.indformula,
				i.indmetavalor,
				reg.regdescricao
			from 
				painel.indicador i 
			inner join 
				painel.unidademeta un ON i.umeid = un.umeid
			inner join 
				painel.tipoindicador tp ON i.tpiid = tp.tpiid
			inner join 
				painel.secretaria sec ON i.secid = sec.secid
			inner join
				painel.regionalizacao reg ON i.regid = reg.regid
			where 
				i.indid = {$_GET['indid']}";
		$ind = $db->pegaLinha($sql);
	(!$ind['indfontetermo'])? $ind['indfontetermo'] = "Não Informada." : $ind['indfontetermo'] = $ind['indfontetermo'];
}
?>
<div style="float:left;border:1px solid #AAAAAA;width: 77%;margin-top: 5px;margin-bottom: 5px;font-size: 12px" >
<span style="padding:3px" ><b>Fonte:</b> <?=$ind['indfontetermo'] ?></span>
</div>
<div class="noprint" style="float:left;border:1px solid #AAAAAA;width: 77%;margin-bottom: 5px;font-size: 12px" >
<div><span style="padding:3px" ><img id="img_menos_ind" src="../imagens/menos.gif" onclick="escondeDetalhes()"  style="display: none;cursor: pointer" ><img  id="img_mais_ind" onclick="exibeDetalhes()" style="cursor: pointer" src="../imagens/mais.gif"> <b>Detalhar Indicador</b></span></div>
<div id="detalhe_indicador" style="display: none;padding: 3px;"  >
<table class="tabela" width="95%" bgcolor="FFFFFF" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td  width=25% align='right' class="SubTituloDireita">Unidade:</td>
    <td><?=$ind['umedesc'] ?></td>
</tr>
<tr>
	<td  align='right' class="SubTituloDireita">Tipo:</td>
    <td><?=$ind['tpidsc'] ?></td>
</tr>
<tr>
	<td  align='right' class="SubTituloDireita">Secretaria:</td>
    <td><?=$ind['secdsc'] ?></td>
</tr>
<tr>
	<td  align='right' class="SubTituloDireita">Objetivo:</td>
    <td><?=$ind['indobjetivo'] ?></td>
</tr>
<tr>
	<td  align='right' class="SubTituloDireita">Fórmula:</td>
    <td><?=$ind['indformula'] ?></td>
</tr>
<tr>
	<td  align='right' class="SubTituloDireita">Meta:</td>
    <td><?=$ind['indmetavalor'] ?></td>
</tr>
<tr>
	<td  align='right' class="SubTituloDireita">Regionalização:</td>
    <td><?=$ind['regdescricao'] ?></td>
</tr>
</table>
</div>
</div>

</div>

</body>
</html>