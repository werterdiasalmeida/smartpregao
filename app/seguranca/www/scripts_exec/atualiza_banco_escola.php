<?php 

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

$db = new cls_banco();


if($_REQUEST['requisicao']=='inserirSQL') {
	if(is_file('update_coord_escolas.txt')) {
		$conteudo = file_get_contents('update_coord_escolas.txt');
	}
	$fp = fopen('update_coord_escolas.txt', 'w+');
	fwrite($fp, (($conteudo)?$conteudo."\n":$conteudo).$_REQUEST['sql']);
	fclose($fp);
	exit; 
}


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

$local= explode("/",curPageURL());?>
<?if ($local[2]=="simec.mec.gov.br" ){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxQhVwj8ALbvbyVgNcB-R-H_S2MIRxTIdhrqjcwTK3xxl_Nu_YMC5SdLWg" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxQhVwj8ALbvbyVgNcB-R-H_S2MIRxTIdhrqjcwTK3xxl_Nu_YMC5SdLWg"; ?>
<? } ?>
<?if ($local[2]=="simec-d.mec.gov.br"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRYtD8tuHxswJ_J7IRZlgTxP-EUtxT_Cz5IMSBe6d3M1dq-XAJNIvMcpg" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxRYtD8tuHxswJ_J7IRZlgTxP-EUtxT_Cz5IMSBe6d3M1dq-XAJNIvMcpg"; ?> 
<? } ?>
<?if ($local[2]=="simec" ){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTNzTBk8zukZFuO3BxF29LAEN1D1xSIcGWxF7HCjMwks0HURg6MTfdk1A" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTNzTBk8zukZFuO3BxF29LAEN1D1xSIcGWxF7HCjMwks0HURg6MTfdk1A"; ?>
<? } ?>
<?if ($local[2]=="simec-d"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTFm3qU4CVFuo3gZaqihEzC-0jfaRTY9Fe8UfzYeoYDxtThvI3nGbbZEw" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTFm3qU4CVFuo3gZaqihEzC-0jfaRTY9Fe8UfzYeoYDxtThvI3nGbbZEw"; ?> 
<? } ?>
<?if ($local[2]=="simec-local"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRzjpIxsx3o6RYEdxEmCzeJMTc4zBSMifny_dJtMKLfrwCcYh5B01Pq_g" type="text/javascript"></script>
	<? $Gkey = "ABQIAAAAwN0kvNsueYw8CBs704pusxRzjpIxsx3o6RYEdxEmCzeJMTc4zBSMifny_dJtMKLfrwCcYh5B01Pq_g"; ?> 	
<? } ?>
<?if ($local[2]=="painel.mec.gov.br"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTPkFYZwQy2nvpGvFj08HQmPOt9ZBT2EJmQsTms0WQqU_5GvEj7bMZd7g" type="text/javascript"></script>
	<? $GKey = "ABQIAAAAwN0kvNsueYw8CBs704pusxTPkFYZwQy2nvpGvFj08HQmPOt9ZBT2EJmQsTms0WQqU_5GvEj7bMZd7g"; ?> 	
<? } ?>

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript">

function initialize() {
	if (GBrowserIsCompatible()) { // verifica se o navegador é compatível
			map = new GMap2(document.getElementById("google_map")); // inicila com a div mapa
			var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;	//Brasil	
			map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom)); //Centraliza e aplica o zoom

			
			// Início Controles
			map.addControl(new GMapTypeControl());
			map.addControl(new GLargeMapControl3D());
	        map.addControl(new GOverviewMapControl());
	        map.enableScrollWheelZoom();
	        map.addMapType(G_PHYSICAL_MAP);
	        // Fim Controles
	
	}
}


function getLatLng(address,endid) {
	geocoder = new GClientGeocoder();
	if (geocoder) {
		geocoder.getLatLng(
		address,
		function(point) {
			if (point) {
				$.ajax({
			   		type: "POST",
			   		url: "atualiza_banco_escola.php",
   					async: false,
			   		data: "requisicao=inserirSQL&sql=UPDATE entidade.endereco set medlatitude = '" + formatoEnderecoCoordenada(point.lat(),'lat') + "', medlongitude = '" + formatoEnderecoCoordenada(point.lng(),'lng') + "' where endid = " + endid + ";",
			   		success: function(msg){}
			 		});
			}
		}
		);
	}
}


function formatoEnderecoCoordenada(coordenada, tipocoordenada) {
	
	var dmsCorDeg='';
	var dmsCorMin='';
	var dmsCorSec='';
	var dmsCorPolo='';
	var dmsCorMinVals='';
	var ddCorVal='';
	
	if(coordenada.toString().substr(0,1) == "-") {
		ddCorVal = coordenada.toString().substr(1,coordenada.toString().length-1);
	} else {
		ddCorVal = coordenada.toString();
	}
		
	// Graus Lat 
	ddCorVals = ddCorVal.split(".");
	if(ddCorVals[0]) {
		dmsCorDeg = ddCorVals[0];
	}
	
	// * 60 = mins
	if(ddCorVals[1]) {
		ddCorRemainder  = ("0." + ddCorVals[1]) * 60;
		dmsCorMinVals   = ddCorRemainder.toString().split(".");
		dmsCorMin = dmsCorMinVals[0];
	}
	
	// * 60 novamente = secs
	if(dmsCorMinVals[1]) {
		ddCorMinRemainder = ("0." + dmsCorMinVals[1]) * 60;
		dmsCorSec  = Math.round(ddCorMinRemainder);
	}
		
	if (coordenada.toString().substr(0,1) == "-") {
		if(tipocoordenada == 'lat') {
			dmsCorPolo = "S";
		} else {
			dmsCorPolo = "W";		
		}
	} else {
		if(tipocoordenada == 'lat') {
			dmsCorPolo = "N";
		} else {
			dmsCorPolo = "E";		
		}
	}
	
	return dmsCorDeg+'.'+dmsCorMin+'.'+dmsCorSec+'.'+dmsCorPolo;

}


</script>
</head>
<body>
<?php 

$sql = "select en.endcep, mun.mundescricao, en.estuf, en.endbai, en.endid from entidade.entidade e 
		left join entidade.endereco en on e.entid = en.entid 
		left join territorios.municipio mun on mun.muncod = en.muncod
		where entcodent is not null and (medlatitude is null or medlongitude is null)
		LIMIT 10000 OFFSET 0";

$endereco = $db->carregar($sql);

$endereco = !$endereco ? array() : $endereco;

echo "<script>";
foreach($endereco as $end){
	echo "getLatLng(\"CEP ".$end['endcep'].", Brasil\",".$end['endid'].");";
}
echo "</script>";

?>
<script>
var t=setTimeout("alertMsg()",4200000);
}
function alertMsg() {
	window.location='atualiza_banco_escola.php';
}
</script>
</body>
</html>