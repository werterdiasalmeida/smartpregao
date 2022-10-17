<?php 
$sql = "select 
			arqid,
			arqextensao,
			arqtipo
		from 
			public.arquivo 
		where 
			arqnome = '".str_replace("&atualizar_arquivo=1","",trim($_SERVER['REQUEST_URI']))."'
		and
			sisid = {$_SESSION['sisid']}
		and
			arqstatus = 'A'
		order by
			arqid desc
		limit 1";
$arq = $db->pegaLinha($sql);
if($arq && !$_GET['atualizar_arquivo']){
	$caminho = APPRAIZ."arquivos/painel/".floor($arq['arqid']/1000).'/'.$arq['arqid'];
	if(is_file($caminho)){
		$fp = fopen($caminho,'r');
		$texto = fread($fp, filesize($caminho));
		echo $texto;
		exit;
	}
}

?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<STYLE>
    thead { display: table-header-group; }
    tfoot { display: table-footer-group; }
</STYLE>

<table border=0 align="left" width="100%">

<thead>
  <tr>
     <th align="left"  width=100%>
       <?php echo monta_cabecalho_relatorio_painel(100);?>
     </th>
  </tr>
</thead>

<tbody>
<tr>
   <td align="left"  width="100%">
<?

//Função para aplicar a chave do Google Maps
function curPageURL() {
		 $pageURL = 'http';
		 if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
		 $pageURL .= "://";
		 if ($_SERVER["SERVER_PORT"] != "80") {
		  $pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
		 } else {
		  $pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
		 }
		 return $pageURL;
		}
if($_GET['muncod'] || $_GET['estuf']){
	$url = explode("/",curPageURL());
	?>
	<?if ($url[2]=="simec.mec.gov.br" ){ ?>
	    <script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBQhVwj8ALbvbyVgNcB-R-H_S2MIRxSRw07TtkPv50s-khCgCjw1KhcuSw" type="text/javascript"></script>
	  <? } ?>
	  <?if ($url[2]=="simec-d.mec.gov.br"){ ?>
	  	<script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBRYtD8tuHxswJ_J7IRZlgTxP-EUtxRD3aBmpKp7QQhM-oKEpi_q_Z6nzQ" type="text/javascript"></script>
	<? } ?>
	<?if ($url[2]=="simec" ){ ?>
	    <script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBTNzTBk8zukZFuO3BxF29LAEN1D1xRrpthpVw0AZ6npV05I8JLIRtHtyQ" type="text/javascript"></script>
	  <? } ?>
	  <?if ($url[2]=="simec-d"){ ?>
	  	<script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBTFm3qU4CVFuo3gZaqihEzC-0jfaRSyt8kdLeiWuocejyXXgeTtRztdYQ" type="text/javascript"></script>
	<? } ?>
	<?if ($url[2]=="simec-local"){ ?>
		<script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAiAikFUnGncRW1bW0qcdmBxRzjpIxsx3o6RYEdxEmCzeJMTc4zBRASslYk1IYbFjzsDflWVzHgfQ3gQ" type="text/javascript"></script>
	<? } 
}?>

<?php 
echo "<div class=\"notprint\" style=\"width:100%;text-align:right\" ><input type=\"button\" value=\"Imprimir\" onclick=\"self.print();\" /><input type=\"button\" value=\"Atualizar Relatório\" onclick=\"window.location.href='{$_SERVER["REQUEST_URI"]}&atualizar_arquivo=1'\" /></div><br/><br/>";

		$sql = "select 
					indid
				from
					painel.indicador
				where
					indrelatorio is true
				and
					indstatus = 'A'
				and
					indpublicado is true";
		
		$arrIndicadores = $db->carregar($sql);
		$arrIndicadores = !$arrIndicadores ? array() : $arrIndicadores;
		foreach($arrIndicadores as $indid){
			$arrIndicador[] = $indid['indid'];
		}
		$arrIndicador = !$arrIndicador ? array(1) : $arrIndicador;


$arrIndiid = $_GET['indid'] ? explode(",",$_GET['indid']) : $arrIndicador;

if($_REQUEST['muncod'])
	$regionalizacao = " and regid <> ".REGIONALIZACAO_UF." ";


if($_GET['indid'] && $_GET['indid'] != "todos"){
	$sql = "select 
				ind.indid 
			from 
				painel.indicador ind
			inner join
				painel.acao aca ON aca.acaid = ind.acaid 
			where 
				indid in (".implode(",",$arrIndiid).")
			and
				indstatus = 'A'
			and
				indpublicado is true
			and
				indrelatorio is true
			$regionalizacao 
			order by
				acadsc,indnome";
}else{
	$sql = "select 
				ind.indid 
			from 
				painel.indicador ind
			inner join
				painel.acao aca ON aca.acaid = ind.acaid
			where
				1 = 1
			and
				indstatus = 'A'
			and
				indpublicado is true
			and
				indrelatorio is true
			order by
				acadsc,indnome";
}

$dados = $db->carregar($sql);
foreach($dados as $d){
	$arrI[] = $d['indid'];
}
if($_GET['muncod']){

	$sql = "select 
				mun.mundescricao as municipio,
				substr(mun.muncod,1,6) as muncod,
				est.estdescricao as estado,
				est.estuf as uf,
				est.estuf as estuf,
				reg.regcod as regcod,
				reg.regdescricao as regiao,
				munviz.mundescricao || ' / ' ||  munviz.estuf as vizinho,
				munviz.muncod as muncodviz,
				meso.mescod,
				mesdsc,
					 --############### LATITUDE ###################### --
						CASE WHEN (SPLIT_PART(mun.munmedlat, '.', 1) <>'' AND SPLIT_PART(mun.munmedlat, '.', 2) <>'' AND split_part(mun.munmedlat, '.', 3) <>'') THEN
			               CASE WHEN split_part(mun.munmedlat, '.', 4) <>'N' THEN
			                   (((split_part(mun.munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(mun.munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(mun.munmedlat, '.', 1)::int)))*(-1)
			                ELSE
			                   ((SPLIT_PART(mun.munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(mun.munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(mun.munmedlat, '.', 1)::int))
			               END
			            ELSE
			            -- Valores do IBGE convertidos em  decimal
			            CASE WHEN (length (mun.munmedlat)=8) THEN
			                CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
			                    ((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                    (SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
			                END
			            ELSE
			                CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
			                   ((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                  0--((SUBSTR(REPLACE(mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),1,2)::double precision))
			                END
			            END
			            END as latitude,
			            --############### FIM LATITUDE ###################### --

			            --############### LONGITUDE ###################### --
			            CASE WHEN (SPLIT_PART(mun.munmedlog, '.', 1) <>'' AND SPLIT_PART(mun.munmedlog, '.', 2) <>'' AND split_part(mun.munmedlog, '.', 3) <>'') THEN
			               ((split_part(mun.munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(mun.munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(mun.munmedlog, '.', 1)::int))*(-1)
			            ELSE
			                -- Valores do IBGE convertidos em  decimal
			               (SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1)
			            END as longitude
			            --############### FIM LONGITUDE ###################### --
			         
			from
				territorios.municipio mun
			inner join
				territorios.estado est ON est.estuf = mun.estuf
			inner join
				territorios.regiao reg ON reg.regcod = est.regcod
			inner join
				territorios.mesoregiao meso ON meso.mescod = mun.mescod
			left join
				territorios.municipiosvizinhos viz ON mun.muncod = viz.muncod
			left join
				territorios.municipio munviz ON munviz.muncod = viz.muncodvizinho
			where
				mun.muncod = '{$_GET['muncod']}'
			order by
				mun.mundescricao,munviz.mundescricao ";
	$mun = $db->carregar($sql);
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
		    <td align='right' width=25% class="SubTituloDireita">Município:</td>
		    <td align='right' colspan="2" style="font-size:16px;color:#000066;" class="SubTituloEsquerda"><?=$mun[0]['municipio']?> / <?=$mun[0]['estuf']?></td>
		</tr>
		<tr>
		    <td align='right' width=25% class="SubTituloDireita">Estado:</td>
		    <td><a href="painel.php?modulo=principal/popupTabelaIndicador&acao=A&estuf=<?=$mun[0]['uf']?>&indid=<?=$_REQUEST['indid']?>"><?=$mun[0]['estado']?></a></td>
		    <td align='right' width="330px" rowspan="5" ><div style="border: solid 1px #000000 ; width: 330px; height: 300px;" id="mapa_indicadores"><?=$mun[0]['latitude']?><?=$mun[0]['longitude']?></div></td>
		</tr>
		<tr>
		    <td align='right' width=25% class="SubTituloDireita">Região:</td>
		    <td><a href="painel.php?modulo=principal/popupTabelaIndicador&acao=A&regcod=<?=$mun[0]['regcod']?>&indid=<?=$_REQUEST['indid']?>"><?=$mun[0]['regiao']?></a></td>
		</tr>
		<tr>
		    <td align='right' width=25% class="SubTituloDireita">Mesoregião:</td>
		    <td><a href="painel.php?modulo=principal/popupTabelaIndicador&acao=A&mescod=<?=$mun[0]['mescod']?>&indid=<?=$_REQUEST['indid']?>"><?=$mun[0]['mesdsc']?></a></td>
		</tr>
		<tr>
		    <td align='right' width=25% class="SubTituloDireita">Municípios Limítrofes:</td>
		    <td>
		    <?php foreach($mun as $m){$vizinhos[] = "<a href=\"javascript:window.location.href='painel.php?modulo=principal/popupTabelaIndicador&acao=A&muncod={$m['muncodviz']}&indid={$_REQUEST['indid']}'\" >".$m['vizinho']."</a>";}
		    echo $viz = implode(" <br/> ", $vizinhos);?>
		    </td>
		</tr>
		<tr>
		    <td align='right' width=25% class="SubTituloDireita"><span class="notprint">Dados IBGE:</span></td>
		    <td>
		    <a class="notprint" href="javascript:popupIbge('<?=$mun[0]['muncod']?>','<?=$mun[0]['municipio']?>');">Clique para ver mais informações sobre o município.</a>
		    </td>
		</tr>
	</table>
	<br/><br/>
	<script>

			function popupIbge(codmun,nomemun){
				window.open('http://www.ibge.gov.br/cidadesat/xtras/perfilwindow.php?nomemun='+nomemun+'&codmun='+codmun+'&r=2','IBGE','scrollbars=yes,height=400,width=400,status=no,toolbar=no,menubar=no,location=no');
				}
			var map; //variável do Google Maps

			function initialize() {
				if (GBrowserIsCompatible()) { // verifica se o navegador é compatível
					
					map = new GMap2(document.getElementById("mapa_indicadores"));
					map.setCenter(new GLatLng(<?=$mun[0]['latitude']?>, <?=$mun[0]['longitude']?>), 13);

					// Controles
					map.addControl(new GMapTypeControl());
					map.addControl(new GLargeMapControl3D());
			        //map.addControl(new GOverviewMapControl());
			        map.enableScrollWheelZoom();
			        //map.addMapType(G_PHYSICAL_MAP);
				    
					// Valor inicial do mapa e zoom
					var zoom = 8;
					var title = 'Clique para ver os detalhes.';
					var lat_i = <?=$mun[0]['latitude']?>; var lng_i = <?=$mun[0]['longitude']?>;
					map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom));

					// Cria o marcador na tela
					var point = new GLatLng(<?=$mun[0]['latitude']?>,<?=$mun[0]['longitude']?>);
			        map.addOverlay(new GMarker(point));
			        
					
				} // fim do if do JS
			       
		     } // fim da função initialize

		     function createMarker(posn, title, icon) {
		         var marker = new GMarker(posn, {title: title, icon: icon, draggable:false });
		         return marker;
		       }
	</script>
<?php }

if($_GET['estuf'] && !$_GET['muncod']){

	$sql = "select muncodcapital from territorios.estado where estuf = '{$_GET['estuf']}'";
	
	$muncapital = $db->pegaUm($sql);
	
	$sql = "select 
				mun.mundescricao as municipio,
				substr(mun.muncod,1,6) as muncod,
				est.estdescricao as estado,
				est.estuf as uf,
				est.estuf as estuf,
				reg.regcod as regcod,
				reg.regdescricao as regiao,
				munviz.mundescricao || ' / ' ||  munviz.estuf as vizinho,
				munviz.muncod as muncodviz,
				meso.mescod,
				mesdsc,
					 --############### LATITUDE ###################### --
						CASE WHEN (SPLIT_PART(mun.munmedlat, '.', 1) <>'' AND SPLIT_PART(mun.munmedlat, '.', 2) <>'' AND split_part(mun.munmedlat, '.', 3) <>'') THEN
			               CASE WHEN split_part(mun.munmedlat, '.', 4) <>'N' THEN
			                   (((split_part(mun.munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(mun.munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(mun.munmedlat, '.', 1)::int)))*(-1)
			                ELSE
			                   ((SPLIT_PART(mun.munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(mun.munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(mun.munmedlat, '.', 1)::int))
			               END
			            ELSE
			            -- Valores do IBGE convertidos em  decimal
			            CASE WHEN (length (mun.munmedlat)=8) THEN
			                CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
			                    ((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                    (SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
			                END
			            ELSE
			                CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
			                   ((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                  0--((SUBSTR(REPLACE(mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),1,2)::double precision))
			                END
			            END
			            END as latitude,
			            --############### FIM LATITUDE ###################### --

			            --############### LONGITUDE ###################### --
			            CASE WHEN (SPLIT_PART(mun.munmedlog, '.', 1) <>'' AND SPLIT_PART(mun.munmedlog, '.', 2) <>'' AND split_part(mun.munmedlog, '.', 3) <>'') THEN
			               ((split_part(mun.munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(mun.munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(mun.munmedlog, '.', 1)::int))*(-1)
			            ELSE
			                -- Valores do IBGE convertidos em  decimal
			               (SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1)
			            END as longitude
			            --############### FIM LONGITUDE ###################### --
			         
			from
				territorios.municipio mun
			inner join
				territorios.estado est ON est.estuf = mun.estuf
			inner join
				territorios.regiao reg ON reg.regcod = est.regcod
			inner join
				territorios.mesoregiao meso ON meso.mescod = mun.mescod
			left join
				territorios.municipiosvizinhos viz ON mun.muncod = viz.muncod
			left join
				territorios.municipio munviz ON munviz.muncod = viz.muncodvizinho
			where
				mun.muncod = '$muncapital'
			order by
				mun.mundescricao,munviz.mundescricao ";
	$mun = $db->carregar($sql);
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
		    <td align='right' width=25% class="SubTituloDireita">Estado:</td>
		    <td align='right' colspan="2" style="font-size:16px;color:#000066;" class="SubTituloEsquerda"><?=$mun[0]['estado']?></td>
		    <td align='right' width="330px" rowspan="5" ><div style="border: solid 1px #000000 ; width: 330px; height: 300px;" id="mapa_indicadores"><?=$mun[0]['latitude']?><?=$mun[0]['longitude']?></div></td>
		</tr>
	</table>
	<br/><br/>
	<script>
			
			var map; //variável do Google Maps

			function initialize() {
				if (GBrowserIsCompatible()) { // verifica se o navegador é compatível
					
					map = new GMap2(document.getElementById("mapa_indicadores"));
					map.setCenter(new GLatLng(<?=$mun[0]['latitude']?>, <?=$mun[0]['longitude']?>), 13);

					// Controles
					map.addControl(new GMapTypeControl());
					map.addControl(new GLargeMapControl3D());
			        //map.addControl(new GOverviewMapControl());
			        map.enableScrollWheelZoom();
			        //map.addMapType(G_PHYSICAL_MAP);
				    
					// Valor inicial do mapa e zoom
					var zoom = 5;
					var title = 'Clique para ver os detalhes.';
					var lat_i = <?=$mun[0]['latitude']?>; var lng_i = <?=$mun[0]['longitude']?>;
					map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom));
					
				} // fim do if do JS
			       
		     } // fim da função initialize
	</script>
<?php }

if($_GET['regcod']){

	$sql = "select regdescricao from territorios.regiao where regcod = '{$_GET['regcod']}'";$regdescricao = $db->pegaUm($sql);?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
		    <td align='right' width=25% class="SubTituloDireita">Região:</td>
		    <td align='right' colspan="2" style="font-size:16px;color:#000066;" class="SubTituloEsquerda"><?=$regdescricao?></td>
		</tr>
	</table>
	<br/><br/>	
<?php }

if($_GET['mescod']){

	$sql = "select mesdsc from territorios.mesoregiao where mescod = '{$_GET['mescod']}'";$mesdsc = $db->pegaUm($sql);?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
		    <td align='right' width=25% class="SubTituloDireita">Mesoregião:</td>
		    <td align='right' colspan="2" style="font-size:16px;color:#000066;" class="SubTituloEsquerda"><?=$mesdsc?></td>
		</tr>
	</table>
	<br/><br/>	
<?php }

foreach($arrI as $indid){
	
	$sql = "select 
				acadsc,
				aca.acaid,
				indnome,
				sec.entid,
				sec.secid,
				indobservacao,
				indobjetivo,
				indfontetermo
			from
				painel.acao aca
			inner join
				painel.indicador ind ON ind.acaid = aca.acaid
			inner join
				painel.secretaria sec ON ind.secid = sec.secid
			and
				ind.indid = $indid";
	$ind = $db->pegaLinha($sql);
	
	
	$sql = "select
			prtobj
		from
			public.parametros_tela
		where 
			mnuid = '6171'
		and
			usucpf is null 
		and
			prtdsc = '$indid' ";
	$arrPOST = $db->pegaUm($sql);
	
	$_POST = $arrPOST ? unserialize($arrPOST) : array();
		
	$semDetalhe = false;
	
	$TpDetalhe = !$_POST['sel_det'] ? array("naoexibir") : explode(",",$_POST['sel_det']);
	
	switch($_POST['rbt_valor']){
		case 2:
			$arrQtdVlr = array("valor");
		break;
		case 1:
			$arrQtdVlr = array("quantidade");
		break;
		default:
			$arrQtdVlr = array("quantidade","valor");
		break;
	}
	
	switch($_POST['rbt_total']){
		case 1:
			$arrTotalizadores = array(1,0);
		break;
		case 2:
			$arrTotalizadores = array(0,1);
		break;
		case 4:
			$arrTotalizadores = array(0,0);
		break;
		default:
			$arrTotalizadores = array(1,1);
		break;
	}
	
	$exibirRegionalizador = $_POST['rbt_reg'] == 1 || !$_POST['rbt_reg'] ? true : false;
	
	if($_POST['sel_per_ini']){
		$arrFiltros = array( "dpeid" => array($_POST['sel_per_ini'],$_POST['sel_per_fim']) );
	}else{
		$arrFiltros = array( "dpeid" => array() );
	}
	
	$arrFiltros["estuf"] = $_POST['estuf'];
	$arrFiltros["regcod"] = $_POST['regcod'];
	$arrFiltros["gtmid"] = $_POST['gtmid'];
	$arrFiltros["tpmid"] = $_POST['tpmid'];
	$arrFiltros["muncod"] = $_POST['muncod'];
	
	$texto = "Brasil";
	
	if($_GET['muncod']){
		$sql = "select estuf,mundescricao from territorios.municipio where muncod = '{$_GET['muncod']}'";
		$mun = $db->pegaLinha($sql);
		$texto = $mun['mundescricao']." / ".$mun['estuf']; 
		$arrFiltros["estuf"] = "";
		$arrFiltros["regcod"] = "";
		$arrFiltros["gtmid"] = "";
		$arrFiltros["tpmid"] = "";
		$arrFiltros["muncod"] = $_GET['muncod'];
	}
	
	if($_GET['regcod']){
		$sql = "select regdescricao from territorios.regiao where regcod = '{$_GET['regcod']}'";
		$texto = $db->pegaUm($sql);
		$arrFiltros["estuf"] = "";
		$arrFiltros["regcod"] = $_GET['regcod'];
		$arrFiltros["gtmid"] = "";
		$arrFiltros["tpmid"] = "";
		$arrFiltros["muncod"] = "";
	}
	
	if($_GET['estuf']){
		$sql = "select estdescricao from territorios.estado where estuf = '{$_GET['estuf']}'";
		$estdescricao = $db->pegaUm($sql);
		$texto = $estdescricao;
		$arrFiltros["estuf"] = $_GET['estuf'];
		$arrFiltros["regcod"] = "";
		$arrFiltros["gtmid"] = "";
		$arrFiltros["tpmid"] = "";
		$arrFiltros["muncod"] = "";
	}
	
	//Filtro por mescod
	if($_GET['mescod']){
		$arrFiltros["mescod"] = $_GET['mescod'];
		$sql = "select mesdsc from territorios.mesoregiao where mescod = '{$_GET['mescod']}'";
		$texto = $db->pegaUm($sql);
	}
	
	$sql = "select 
				tdi.tdiid
			from
				painel.detalhetipoindicador tdi
			where
				tdi.indid = $indid
			and
				tdi.tdistatus = 'A'
			order by
				tdinumero";
	$arrDetalhes1 = $db->carregar($sql);
	
	
	if($_POST['detalhe_'.$arrDetalhes1[0]['tdiid'].'_campo_flag'] == 1){
		$arrFiltros['tidid_1'] = array( $arrDetalhes1[0]['tdiid'] => $_POST['detalhe_'.$arrDetalhes1[0]['tdiid'] ]);
	}
	if($_POST['detalhe_'.$arrDetalhes1[1]['tdiid'].'_campo_flag'] == 1){
		$arrFiltros['tidid_2'] = array( $arrDetalhes1[1]['tdiid'] => $_POST['detalhe_'.$arrDetalhes1[1]['tdiid'] ]);
	}
	$ind['indid'] = $indid;
	$ind['TpDetalhe'] = $TpDetalhe;
	$ind['arrQtdVlr'] = $arrQtdVlr;
	$ind['arrFiltros'] = $arrFiltros;
	$ind['arrTotalizadores'] = $arrTotalizadores;
	$ind['exibirRegionalizador'] = $exibirRegionalizador;
	$ind['semDetalhe'] = $semDetalhe;
	$arrAcao[$ind['acaid']][] = $ind;
}
$ind = array();
foreach($arrAcao as $acao => $indicador){
	//dbg($indicador);
	echo '<table class="tabela" align="center" bgcolor="#e9e9e9"><tr>';
	echo "<td align=\"right\"><span style=\"text-align:center;font-size:16px;font-weight:bold;color:#005500;\">{$indicador[0]['acadsc']}</span></br><span style=\"text-align:center;font-size:12px;font-weight:bold;color:#CC8800;\">$texto</span></td></tr>";
	echo "</table>";
	foreach($indicador as $ind){
		$responsaveis = "";
		if($ind['secid']){
			$sql = "select 
				respnome,
				respemail,
				(case when respdddcelular is not null
					then '(' || respdddcelular || ') ' || respcelular
					else 'N/A'
				end) as  celular,
				(case when respdddtelefone is not null
					then '(' || respdddtelefone || ') ' || resptelefone
					else 'N/A'
				end) as  telefone
			from
				painel.responsavelsecretaria
			where
				secid = {$ind['secid']}
			and
				respstatus = 'A'";
			$resp = $db->carregar($sql);
			if($resp){
					$responsaveis = "<table cellspacing=\"0\" cellpadding=\"0\" >";
				foreach($resp as $rs){
					$responsaveis .= "<tr><td style=\"color:#888888;padding-left: 10px;\"><b>Responsável:</b> <span style=\"padding-right:30px\" >{$rs['respnome']}</span> </td><td style=\"color:#888888;\"><b>E-mail:</b></span> {$rs['respemail']}</td></tr>";
					$responsaveis .= "<tr><td style=\"color:#888888;padding-left: 10px;\" ><b>Telefone:</b> {$rs['telefone']} </td><td style=\"color:#888888\" ><b>Celular:</b></span> {$rs['celular']}</td></tr>";
				}
				$responsaveis .= "</table>";
			}
		}
		
		echo '<table width=100% class="tabela" align="center" bgcolor="#e9e9e9"><tr>';
		echo "<td style=\"padding-left:10px;color:#888888\" ><span style=\"text-align:right;font-size:16px;font-weight:bold;color:#444444;\">{$ind['indnome']}</span><span style=\"font-size:12px;\"><br>{$ind['indobjetivo']}".($ind['indobservacao'] ? "<br /><b>Observação:</b> {$ind['indobservacao']}</span>" : "");
		echo "</td></tr>";
		echo "<tr><td>$responsaveis</td></tr>";
		echo "</table>";
		exibeTabelaIndicador($ind['indid'],$ind['TpDetalhe'],"Ano",$ind['arrQtdVlr'],$ind['arrFiltros'],$ind['arrTotalizadores'],$ind['exibirRegionalizador'],$ind['semDetalhe'],false);
		echo '<table class="tabela" align="center" bgcolor="#e9e9e9"><tr><td style="text-align:right;font-size:10px;color:#888888" >';
		echo "FONTE: ".($ind['indfontetermo'] ? $ind['indfontetermo'] : "N/A");
		echo "</td></tr></table>";
		
	}
	echo "<br/>&nbsp;";
	//echo "<br style=\"page-break-before:always\">";
	
}
$responsaveis = "";
$sql = "select 
			respnome,
			respemail,
			(case when respdddcelular is not null
				then '(' || respdddcelular || ') ' || respcelular
				else 'N/A'
			end) as  celular,
			(case when respdddtelefone is not null
				then '(' || respdddtelefone || ') ' || resptelefone
				else 'N/A'
			end) as  telefone
		from
			painel.responsavelsecretaria
		where
			secid = 3
		and
			respstatus = 'A'";
$resp = $db->carregar($sql);
if($resp){
		$responsaveis = "<table cellspacing=\"0\" cellpadding=\"0\" >";
	foreach($resp as $rs){
		$responsaveis .= "<tr><td style=\"color:#888888;padding-left: 10px;\"><b>Responsável:</b> <span style=\"padding-right:30px\" >{$rs['respnome']}</span> </td><td style=\"color:#888888;\"><b>E-mail:</b></span> {$rs['respemail']}</td></tr>";
		$responsaveis .= "<tr><td style=\"color:#888888;padding-left: 10px;\" ><b>Telefone:</b> {$rs['telefone']} </td><td style=\"color:#888888\" ><b>Celular:</b></span> {$rs['celular']}</td></tr>";
	}
	$responsaveis .= "</table>";
}

$observacoes = "";
$where = array();
if($_GET['estuf']){
	array_push($where, " estuf =  '".$_GET['estuf']."'");
}
if($_GET['muncod']){
	array_push($where, " muncod =  '".$_GET['muncod']."'");
}
if($_GET['regcod']){
	array_push($where, " regcod =  '".$_GET['regcod']."'");
}
if(!$_GET['estuf'] && !$_GET['muncod'] && !$_GET['regcod']){
	array_push($where, " regcod = '0'");
}
$sql = "select 
			obpobservacao
		from
			academico.observacaopainel
		where
			obptipoensino = 'S'
		" . ( is_array($where) && count($where) ? ' AND' . implode(' AND ', $where) : '' ) ."";

$observ = $db->carregar($sql);
if($observ){
		$observacoes = "<table cellspacing=\"0\" cellpadding=\"0\" >";
	foreach($observ as $rs){
		$observacoes .= "<tr><td style=\"color:#888888;padding-left: 10px;\"><span style=\"padding-right:30px;font-size:12px;\" ><b>Observação:</b> {$rs['obpobservacao']}</span> </td></tr>";
	}
	$observacoes .= "</table>";
}

echo '<table class="tabela" align="center" bgcolor="#e9e9e9"><tr>';
echo "<td style=\"padding-left:10px;color:#888888\" ><span style=\"text-align:center;font-size:16px;font-weight:bold;color:#444444;\">Rede Federal</span><br></td>";
echo "<td align=\"right\"><span style=\"text-align:center;font-size:16px;font-weight:bold;color:#005500;\">Educação Superior</span><br>";
echo "<span style=\"text-align:center;font-size:12px;font-weight:bold;color:#CC8800;\">$texto</span></td>";
echo "<tr><td style=\"color:#888888\" colspan=2>$observacoes</td></tr>";
echo "<tr><td style=\"color:#888888\" colspan=2>$responsaveis</td></tr>";
echo "</tr></table>";
exibeRelatorioRedeFederalEducacaoSuperior($arrFiltros);
echo '<table class="tabela" align="center" bgcolor="#e9e9e9"><tr><td style="text-align:right;font-size:10px;color:#888888" >';
echo "FONTE: N/A";
echo "</td></tr></table>";
echo "<br/>&nbsp;";
//echo "<br style=\"page-break-before:always\">";
$responsaveis = "";
$sql = "select 
			respnome,
			respemail,
			(case when respdddcelular is not null
				then '(' || respdddcelular || ') ' || respcelular
				else 'N/A'
			end) as  celular,
			(case when respdddtelefone is not null
				then '(' || respdddtelefone || ') ' || resptelefone
				else 'N/A'
			end) as  telefone
		from
			painel.responsavelsecretaria
		where
			secid = 10
		and
			respstatus = 'A'";
$resp = $db->carregar($sql);
if($resp){
		$responsaveis = "<table cellspacing=\"0\" cellpadding=\"0\" >";
	foreach($resp as $rs){
		$responsaveis .= "<tr><td style=\"color:#888888;padding-left: 10px;\"><b>Responsável:</b> <span style=\"padding-right:30px\" >{$rs['respnome']}</span> </td><td style=\"color:#888888;\"><b>E-mail:</b></span> {$rs['respemail']}</td></tr>";
		$responsaveis .= "<tr><td style=\"color:#888888;padding-left: 10px;\" ><b>Telefone:</b> {$rs['telefone']} </td><td style=\"color:#888888\" ><b>Celular:</b></span> {$rs['celular']}</td></tr>";
	}
	$responsaveis .= "</table>";
}

$observacoes = "";
$sql = "select 
			obpobservacao
		from
			academico.observacaopainel
		where
			obptipoensino = 'P'
		" . ( is_array($where) && count($where) ? ' AND' . implode(' AND ', $where) : '' ) ."";

$observ = $db->carregar($sql);
if($observ){
		$observacoes = "<table cellspacing=\"0\" cellpadding=\"0\" >";
	foreach($observ as $rs){
		$observacoes .= "<tr><td style=\"color:#888888;padding-left: 10px;\"><span style=\"padding-right:30px;font-size:12px;\" ><b>Observação:</b> {$rs['obpobservacao']}</span> </td></tr>";
	}
	$observacoes .= "</table>";
}

echo '<table class="tabela" align="center" bgcolor="#e9e9e9"><tr>';
echo "<td style=\"padding-left:10px;color:#888888\" ><span style=\"text-align:center;font-size:16px;font-weight:bold;color:#444444;\">Rede Federal</span><br></td>";
echo "<td align=\"right\"><span style=\"text-align:center;font-size:16px;font-weight:bold;color:#005500;\">Educação Profissional</span><br>";
echo "<span style=\"text-align:center;font-size:12px;font-weight:bold;color:#CC8800;\">$texto</span></td>";
echo "<tr><td style=\"color:#888888\" colspan=2><span style=\"font-size:12px;\">$observacoes</span></td></tr>";
echo "<tr><td style=\"color:#888888\" colspan=2>$responsaveis</td></tr>";
echo "</tr></table>";
exibeRelatorioRedeFederalEducacaoProfissional($arrFiltros);
echo '<table class="tabela" align="center" bgcolor="#e9e9e9"><tr><td style="text-align:right;font-size:10px;color:#888888" >';
echo "FONTE: N/A";
echo "</td></tr></table>";
echo "<br/>&nbsp;";

?>
</td>
</tr>
</tbody>
</table>

<script>
<?php if($_GET['muncod'] || $_GET['estuf']){ ?>
	initialize();
<?php } ?>
</script>

<?php 
$html = ob_get_contents();

if(!$_GET['atualizar_arquivo']){
	$sql = "INSERT INTO public.arquivo (arqnome,
										arqextensao,
										arqdescricao,
										arqtipo,
										arqtamanho,
										arqdata,
										arqhora,
										usucpf,
										sisid,
										arqstatus)
			VALUES( '".trim($_SERVER['REQUEST_URI'])."',
					'html',
					'Relatório Painel de Controle',
					'application/html',
					'0',
					'".date('Y-m-d')."',
					'".date('H:i:s')."',
					'".$_SESSION["usucpf"]."',
					'".$_SESSION["sisid"]."',
					'A') 
			RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
}else{
	$sql = "update 
				public.arquivo 
			set 
				usucpf = '{$_SESSION['usucpf']}', 
				arqdata = '".date('Y-m-d')."', 
				arqhora = '".date('H:i:s')."'
			where
				arqid = {$arq['arqid']}";
	$db->executar($sql);
	$arqid = $arq['arqid'];
	if(is_file(APPRAIZ."arquivos/painel/".floor($arqid/1000)."/".$arqid))
		@unlink(APPRAIZ."arquivos/painel/".floor($arqid/1000)."/".$arqid);
}


if(!is_dir(APPRAIZ."arquivos/painel/".floor($arqid/1000))) {
	if(!is_dir(APPRAIZ."arquivos/painel"))
		mkdir(APPRAIZ."arquivos/painel", 0777);
	mkdir(APPRAIZ."arquivos/painel/".floor($arqid/1000), 0777);
}

$fp = fopen(APPRAIZ."arquivos/painel/".floor($arqid/1000)."/".$arqid, "a");
if(fwrite($fp, $html)){
	$db->commit($sql);
	fclose($fp);
}

if($_GET['atualizar_arquivo']){
	echo "<script>window.location.href='".str_replace("&atualizar_arquivo=1","",trim($_SERVER['REQUEST_URI']))."'</script>";
}
?>
