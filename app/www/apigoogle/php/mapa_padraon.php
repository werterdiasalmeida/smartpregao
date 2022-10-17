<?
// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if(!$_SESSION['usucpf'])
	echo "<script>
			alert('Problemas com autenticação.');
			window.close();
		  </script>";


// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if($_REQUEST['requisicao'] == "validarlatlongmunicipio") {
	$sql = "SELECT distanciaPontosGPS(
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
										END,
										
										(SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1),
										'".$_REQUEST['lat']."',
										'".$_REQUEST['lng']."') as distancia,
										
				CAST(munmedraio as integer)*1000 as munmedraio
			FROM territorios.municipio mun
			WHERE mun.muncod='".$_REQUEST['muncod']."'";
	
	$dados = $db->pegaLinha($sql);
	
	if($dados['distancia'] > $dados['munmedraio']) {
		echo "FALSE";
	} else {
		echo "TRUE";
	}
	exit;
}

?>
<html>
  <head>
    <meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
	<meta http-equiv="Cache-Control" content="no-cache">
	<meta http-equiv="Pragma" content="no-cache">
	<meta http-equiv="Expires" content="-1">


    <title>Mapa</title>
        <link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
    <?
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
		$local= explode("/",curPageURL());
    ?>
	<?if ($local[2]=="simec.mec.gov.br" ){ ?>
    	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBQhVwj8ALbvbyVgNcB-R-H_S2MIRxSRw07TtkPv50s-khCgCjw1KhcuSw" type="text/javascript"></script>
  	<? } ?>
  	<?if ($local[2]=="simec-d.mec.gov.br"){ ?>
  		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBRYtD8tuHxswJ_J7IRZlgTxP-EUtxRD3aBmpKp7QQhM-oKEpi_q_Z6nzQ" type="text/javascript"></script> 
	<? } ?>
	<?if ($local[2]=="simec" ){ ?>
    	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBTNzTBk8zukZFuO3BxF29LAEN1D1xRrpthpVw0AZ6npV05I8JLIRtHtyQ" type="text/javascript"></script>
  	<? } ?>
  	<?if ($local[2]=="simec-d"){ ?>
  		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBTFm3qU4CVFuo3gZaqihEzC-0jfaRSyt8kdLeiWuocejyXXgeTtRztdYQ" type="text/javascript"></script> 
	<? } ?>
	<?if ($local[2]=="simec-local"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBRzjpIxsx3o6RYEdxEmCzeJMTc4zBRTBTWZ8zjwenwhN3CBw_oSZaFJjQ" type="text/javascript"></script> 	
    <? } ?>
    <?if ($local[2]=="sdh.sisgov.com.br"){ ?>
  		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;sensor=true&amp;key=ABQIAAAA5oufaSx1PgyKsR79BVQ9MhS_lGBxp67RqB18fxpv0Q0FWe0AlhTkpPNjPNyGQo-_d_ocmq-VaXVhIA" type="text/javascript"></script>
	<? } ?>
  	<?if ($local[2]=="simec-sdh"){ ?>
		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;sensor=true&amp;key=ABQIAAAA5oufaSx1PgyKsR79BVQ9MhQ11pDnvwvLkkpC_LAV6dkUcsfjXxR5Fd0F2rKLhvlK_dwkTI1Q2fbw_w" type="text/javascript"></script>
	<? } ?>
	<?if ($local[2]=="sig.sdh.gov.br"){ ?>
		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAA5oufaSx1PgyKsR79BVQ9MhR4i7N7ksIAYgqL8VHrtx8huyb0ZxRDJ3fl3exO65DtxDbDxyJ9SRsuvA" type="text/javascript"></script>
	<? } ?>
  	
    
    
    
    
    
    
    <script type="text/javascript" src="/includes/prototype.js"></script>    
<style>
.body {
BORDER-BOTTOM: 0px; BORDER-LEFT: 0px; BORDER-RIGHT: 0px; BORDER-TOP: 0px}
</style>
  </head>
  <body>
    <form action="#">
    <table bgcolor=#c3c3c3 width=530><tr><td align=center>
    <?php if($habilitado){ ?><font color=blue>Navegue pelo mapa e clique sobre o ponto desejado para definir as coordenadas.<? } ?></font>
    </td></tr></table>
    <div id="mapa" style="width: 530px; height: 550px">
    </div>
    <table bgcolor=#c3c3c3 width=530>
    <tr>
    <td>
    Latitude: <input type=text id=lat value="0" STYLE="border:none ; background-color:#c3c3c3" size=8 /> 
    Longitude: <input type=text id=lng value="0" STYLE="border:none; background-color:#c3c3c3" size=8 />
    <input type=text id=proximo value="" STYLE="border:none; background-color:#c3c3c3" size=40 />
    </td>
    </tr>
    <tr>
    <td>
    Latitude:  <input type="text" class="normal" size="3" maxlength="2" id="__graulatitude"> º <input type="text" class="normal" size="3" maxlength="2" id="__minlatitude"> ' <input type="text" class="normal" size="3" maxlength="2" id="__seglatitude"> " <select id="__pololatitude"><option value="S">S</option><option value="N">N</option></select>
    Longitude: <input type="text" class="normal" size="3" maxlength="2" id="__graulongitude"> º <input type="text" class="normal" size="3" maxlength="2" id="__minlongitude"> ' <input type="text" class="normal" size="3" maxlength="2" id="__seglongitude"> " <select id="__pololongitude"><option value="W">W</option><option value="E">E</option></select> <br />
    </td>
    </tr>
    <tr>
    <td>
    <input type="button" name="visualizar" value="Visualizar" onclick="visualizarPontosManuais();"> <input type="button" value="Ok" onclick="marcarPontosManuais();">
    </td>
    </tr>

    </table>
    </form>
  </body>
    <script type="text/javascript">	
    document.getElementById("mapa").innerHTML = "Carregando...";
    
    function marcarPontosManuais() {
	    var latitude;
    	var longitude;
    
	    if(document.getElementById("__seglatitude").value && 
	       document.getElementById("__minlatitude").value &&
	       document.getElementById("__graulatitude").value &&
	       document.getElementById("__seglongitude").value &&
	       document.getElementById("__seglongitude").value &&
	       document.getElementById("__seglongitude").value) {
	       
	    	var latitude  = ((( Number(document.getElementById("__seglatitude").value) / 60 ) + Number(document.getElementById("__minlatitude").value)) / 60 ) + Number(document.getElementById("__graulatitude").value);
			var longitude = ((( Number(document.getElementById("__seglongitude").value) / 60 ) + Number(document.getElementById("__minlongitude").value)) / 60 ) + Number(document.getElementById("__graulongitude").value);
		}
		
		if(document.getElementById("__pololatitude").value == "S") {
			latitude = latitude * -1;
		}
		
		if(document.getElementById("__pololongitude").value == "W") {
			longitude = longitude * -1;
		}
		
		
		if(!latitude || !longitude) {
			alert("Digite as coordenadas geograficas");
			return false;
		}
		
		var lat = latitude;
        var lng = longitude;

		copiar(lat,lng,window.opener.document.getElementById("endzoom<? echo $_REQUEST['tipoendereco']; ?>").value);

    }
    
    function visualizarPontosManuais() {
    
    var latitude;
    var longitude;
    
    if(document.getElementById("__seglatitude").value && 
       document.getElementById("__minlatitude").value &&
       document.getElementById("__graulatitude").value &&
       document.getElementById("__seglongitude").value &&
       document.getElementById("__seglongitude").value &&
       document.getElementById("__seglongitude").value) {
       
    	var latitude  = ((( Number(document.getElementById("__seglatitude").value) / 60 ) + Number(document.getElementById("__minlatitude").value)) / 60 ) + Number(document.getElementById("__graulatitude").value);
		var longitude = ((( Number(document.getElementById("__seglongitude").value) / 60 ) + Number(document.getElementById("__minlongitude").value)) / 60 ) + Number(document.getElementById("__graulongitude").value);
	}
	
	if(latitude=='' || longitude=='') {
		alert("Digite as coordenadas geograficas");
		return false;
	}
	
	if(document.getElementById("__pololatitude").value == "S") {
		latitude = latitude * -1;
	}
		
	if(document.getElementById("__pololongitude").value == "W") {
		longitude = longitude * -1;
	}
    
    if (GBrowserIsCompatible()) {
      
      
      	var zoom = window.opener.document.getElementById("endzoom<? echo $_REQUEST['tipoendereco']; ?>").value;
        //var map = new GMap2(document.getElementById("mapa"));
        var options = {showOnLoad : true, suppressInitialResultSelection : true};
        var map = new GMap2(document.getElementById("mapa"),{googleBarOptions:options}) 
      	map.enableGoogleBar();
		var lat = latitude;
        var lng = longitude;
		if (zoom=='') zoom = 14;
		map.setCenter(new GLatLng(lat,lng), parseInt(zoom));
		
	    // Criando o ícone do mapa
		var baseIcon = new GIcon();
		baseIcon.iconSize = new GSize(18, 28);
		baseIcon.shadowSize = new GSize(18, 28);
		baseIcon.iconAnchor = new GPoint(9, 34);
		baseIcon.infoWindowAnchor = new GPoint(9, 2);
		baseIcon.infoShadowAnchor = new GPoint(18, 25);
		
		// Criando os Marcadores com o resultado
        baseIcon.image='/imagens/tachinha_b.png';
		
		var posn = new GLatLng(lat, lng);
		var icon = baseIcon;
		var title = 'Clique para ver os detalhes';
		var entidTemp = '<? echo $_REQUEST['entid']; ?>';
		if( entidTemp == ""){
			var html = "Não implementado";
		} else {
			var html = "<iframe src='/apigoogle/php/mapa_balao.php?entid=<? echo $_REQUEST['entid']; ?>' width=350 frameborder=0 ></iframe>";
		}
		var marker = createMarker(posn,title,icon, html); 
		map.addOverlay(marker);
		
        map.addControl(new GSmallMapControl());
        map.addControl(new GMapTypeControl());
        map.enableScrollWheelZoom();
        map.setMapType(G_HYBRID_MAP);
        
        
        GEvent.addListener(map,"mousemove",function(latlng) {
	       	document.getElementById('lat').value=dec2grau(latlng.lat());
	        document.getElementById('lng').value=dec2grau(latlng.lng());
         });
         
        GEvent.addListener(map,"click", function(overlay,latlng) {
          var tileCoordinate = new GPoint();
          var tilePoint = new GPoint();
          var currentProjection = G_NORMAL_MAP.getProjection();
          tilePoint = currentProjection.fromLatLngToPixel(latlng, map.getZoom());
          var myHtml = "Latitude: " + dec2grau(latlng.lat()) +"<br/>Longitude: " + dec2grau(latlng.lng()) 
				+ "<br/> Zoom:  " + map.getZoom() + "<br> <a href=# onClick='javascript: copiar("+latlng.lat()+","+latlng.lng()+","+map.getZoom()+")'>[Definir local neste ponto]</a>";
          map.openInfoWindow(latlng, myHtml);
        });



	}

    }
    
    
    function initialize() {
      if (GBrowserIsCompatible()) {
        //var map = new GMap2(document.getElementById("mapa"));
        var options = {showOnLoad : true, suppressInitialResultSelection : true};

        var map = new GMap2(document.getElementById("mapa"),{googleBarOptions:options}) 
       	 map.enableGoogleBar();
       	
       	// Preenchendo o mapa
		<?php if ($_REQUEST["latitude"]!='0'){ ?>
			var zoom = window.opener.document.getElementById("endzoom<? echo $_REQUEST['tipoendereco']; ?>").value;
			var lat = <? echo ((trim($_REQUEST["polo"])=="S")?"-".$_REQUEST["latitude"]:$_REQUEST["latitude"]); ?>;
	        var lng = <? echo ((trim($_REQUEST["polo"])=="S")?"-".$_REQUEST["longitude"]:$_REQUEST["longitude"]); ?>;
			if (zoom=='') zoom = 14;
			map.setCenter(new GLatLng(lat,lng), parseInt(zoom));

		    // Criando o ícone do mapa
			var baseIcon = new GIcon();
			baseIcon.iconSize = new GSize(14, 14);
			baseIcon.shadowSize = new GSize(14, 14);
			baseIcon.iconAnchor = new GPoint(14, 14);
			baseIcon.infoWindowAnchor = new GPoint(9, 2);
			baseIcon.infoShadowAnchor = new GPoint(18, 25);
			
			// Criando os Marcadores com o resultado
	        baseIcon.image='/imagens/tachinha_b.png';
			
			var posn = new GLatLng(lat, lng);
			var icon = baseIcon;
			var title = 'Clique para ver os detalhes';
			var entidTemp = '<? echo $_REQUEST['entid']; ?>';
			if( entidTemp == ""){
				var html = "Não implementado";
			} else {
				var html = "<iframe src='/apigoogle/php/mapa_balao.php?entid=<? echo $_REQUEST['entid']; ?>' width=350 frameborder=0 ></iframe>";
			}
			var marker = createMarker(posn,title,icon, html); 
			map.addOverlay(marker);
				
		<?php } else { ?>
			var cep = window.opener.document.getElementById("endcep<? echo $_REQUEST['tipoendereco']; ?>").value;
			var cidade = window.opener.document.getElementById("mundescricao<? echo $_REQUEST['tipoendereco']; ?>").value;
			var estado = window.opener.document.getElementById("estuf<? echo $_REQUEST['tipoendereco']; ?>").value;
			var bairro=	window.opener.document.getElementById("endbai<? echo $_REQUEST['tipoendereco']; ?>").value;
			endereco= cep+", "+cidade+", "+estado+", Brasil";
			/*
			 * UTILIZAR O CEP PARA DEIXAR O BUSCA MAIS REFINADA
			if (cep.substr(5,8) == '000');
				endereco= cidade+", "+estado+", Brasil";
		    */
			
		    document.getElementById("proximo").value=endereco;
		    // vai para o local apxoximado
		    try{
		    var address = endereco;
		      var geocoder = new GClientGeocoder();
		      if (geocoder) {
		        	geocoder.getLatLng(
		          	address,
		          	function(point) {
			            if (!point) {
		              	alert(address + " Não encontrado"); return false;
		            	} else {
			              	map.setCenter(point, 13);
			              	var marker = new GMarker(point);
			              	map.addOverlay(marker);
			              	marker.openInfoWindowHtml(address);
			              	start = point;
		            	}
		          	}
		        );
		      }
		     } catch(e){}
			
		<? } ?>  
        map.addControl(new GSmallMapControl());
        map.addControl(new GMapTypeControl());
        map.enableScrollWheelZoom();
        map.setMapType(G_HYBRID_MAP);
        GEvent.addListener(map,"mousemove",function(latlng) {
	       	document.getElementById('lat').value=dec2grau(latlng.lat());
	        document.getElementById('lng').value=dec2grau(latlng.lng());
         });
         
        <? if(!$_REQUEST['redirectOFF']){ ?>
	        GEvent.addListener(map,"click", function(overlay,latlng) {
	          var tileCoordinate = new GPoint();
	          var tilePoint = new GPoint();
	          var currentProjection = G_NORMAL_MAP.getProjection();
	          tilePoint = currentProjection.fromLatLngToPixel(latlng, map.getZoom());
	          var myHtml = "Latitude: " + dec2grau(latlng.lat()) +"<br/>Longitude: " + dec2grau(latlng.lng()) 
					+ "<br/> Zoom:  " + map.getZoom() + "<br> <a href=# onClick='javascript: copiar("+latlng.lat()+","+latlng.lng()+","+map.getZoom()+")'>[Definir local neste ponto]</a>";
	          map.openInfoWindow(latlng, myHtml);
	        });
        <? } ?>
      }
      
    }

    function copiar(lat, lng, z){
		var ddLat=lat+"";
		var validadistancia = true;
		
		if(window.opener.document.getElementById("muncod<? echo $_REQUEST['tipoendereco']; ?>").value != "") {
			
			var myAjax = new Ajax.Request(
				window.location.href,
				{
					method: 'post',
					parameters: 'requisicao=validarlatlongmunicipio&lat='+lat+'&lng='+lng+'&muncod='+window.opener.document.getElementById("muncod<? echo $_REQUEST['tipoendereco']; ?>").value,
					asynchronous: false,
					onComplete: function(resp) {
						if(resp.responseText == "FALSE") {
							validadistancia = false;
						}
					},
					onLoading: function(){}
				});
			
			if(!validadistancia) {
				alert("As coordenadas não correspondem os limites do munícipio");
				return false;
			}
			
		}

		if (ddLat.substr(0,1) == "-") {
			ddLatVal = ddLat.substr(1,ddLat.length-1);
		} else {
			ddLatVal = ddLat;
		}
		
		// Graus Lat 
		var ddLatVals = ddLatVal.split(".");
		if(ddLatVals[0]) {
			var dmsLatDeg = ddLatVals[0];
			window.opener.document.getElementById("graulatitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatDeg;
			window.opener.document.getElementById("_graulatitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML=dmsLatDeg;
		}
		
		// * 60 = mins
		if(ddLatVals[1]) {
			var ddLatRemainder  = ("0." + ddLatVals[1]) * 60;
			var dmsLatMinVals   = ddLatRemainder.toString().split(".");
			var dmsLatMin = dmsLatMinVals[0];
			window.opener.document.getElementById("minlatitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatMin;
			window.opener.document.getElementById("_minlatitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML=dmsLatMin;
		}
			
		// * 60 novamente = secs
		if(dmsLatMinVals[1]) {
			var ddLatMinRemainder = ("0." + dmsLatMinVals[1]) * 60;
			var dmsLatSec  = Math.round(ddLatMinRemainder);
			window.opener.document.getElementById("seglatitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatSec;
			window.opener.document.getElementById("_seglatitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML=dmsLatSec;
		}
		
		ddLat=lng+"";
		if (ddLat.substr(0,1) == "-") {
			var ddLatVal = ddLat.substr(1,ddLat.length-1);
			window.opener.document.getElementById("pololatitude<? echo $_REQUEST['tipoendereco']; ?>").value = "S";
			window.opener.document.getElementById("_pololatitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML = "S";
		} else {
			var ddLatVal = ddLat;
			window.opener.document.getElementById("pololatitude<? echo $_REQUEST['tipoendereco']; ?>").value = "N";
			window.opener.document.getElementById("_pololatitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML = "N";
		}
		// Graus Long 
		ddLatVals = ddLatVal.split(".");
		if(ddLatVals[0]) {
			var dmsLatDeg = ddLatVals[0];
			window.opener.document.getElementById("graulongitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatDeg;
			window.opener.document.getElementById("_graulongitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML=dmsLatDeg;
		}
		
		// * 60 = mins
		if(ddLatVals[1]) {
			ddLatRemainder  = ("0." + ddLatVals[1]) * 60;
			dmsLatMinVals   = ddLatRemainder.toString().split(".");
			dmsLatMin = dmsLatMinVals[0];
			window.opener.document.getElementById("minlongitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatMin;
			window.opener.document.getElementById("_minlongitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML=dmsLatMin;
		}
			
		// * 60 novamente = secs
		if(dmsLatMinVals[1]) {
			ddLatMinRemainder = ("0." + dmsLatMinVals[1]) * 60;
			dmsLatSec  = Math.round(ddLatMinRemainder);
			window.opener.document.getElementById("seglongitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatSec;
			window.opener.document.getElementById("_seglongitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML=dmsLatSec;
		}

		if (ddLat.substr(0,1) == "-") {
			ddLatVal = ddLat.substr(1,ddLat.length-1);
			window.opener.document.getElementById("pololongitude<? echo $_REQUEST['tipoendereco']; ?>").value = "W";
			window.opener.document.getElementById("_pololongitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML = "W";
		} else {
			ddLatVal = ddLat;
			window.opener.document.getElementById("pololongitude<? echo $_REQUEST['tipoendereco']; ?>").value = "E";
			window.opener.document.getElementById("_pololongitude<? echo $_REQUEST['tipoendereco']; ?>").innerHTML = "E";
		}
		
		window.opener.document.getElementById("endzoom<? echo $_REQUEST['tipoendereco']; ?>").value=z;
		
	   	window.close();
    	
    }
	
	 function createMarker(posn, title, icon, html) {
      var marker = new GMarker(posn, {title: title, icon: icon, draggable:false });
      GEvent.addListener(marker, "click", function() {
      	marker.openInfoWindowHtml(html);
       });

      return marker;
      
    }
 
 	// Converte para corrdenada Gmaps
 	function grau2dec(valor){
 	
 	var valor=valor.split(".");	
 	valor=((((Number(valor[2]) / 60 ) + Number(valor[1])) / 60 ) + Number(valor[0]))*-1;

	return valor
 		
 	}
     // Converter em Graus
     function dec2grau(valor){
		ddLat=valor+"";

		if (ddLat.substr(0,1) == "-") {
			ddLatVal = ddLat.substr(1,ddLat.length-1);
		} else {
			ddLatVal = ddLat;
		}
		
		// Graus 
		ddLatVals = ddLatVal.split(".");
		dmsLatDeg = ddLatVals[0];
		
		// * 60 = mins
		ddLatRemainder  = ("0." + ddLatVals[1]) * 60;
		dmsLatMinVals   = ddLatRemainder.toString().split(".");
		dmsLatMin = dmsLatMinVals[0];
			
		// * 60 novamente = secs
		ddLatMinRemainder = ("0." + dmsLatMinVals[1]) * 60;
		dmsLatSec  = Math.round(ddLatMinRemainder);
		
		valor=dmsLatDeg+unescape('%B0')+" "+dmsLatMin+"' "+dmsLatSec+"''";

     	return valor;
     }
     initialize();
</script>
</html>
