<?
/*
 * VALIDAÇÃO SIMPLES
 */
session_start();

if(!$_SESSION['usucpf'])
	echo "<script>
			alert('Problemas com autenticação.');
			window.close();
		  </script>";
?>
<html>
  <head>
    <meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
	<meta http-equiv="Cache-Control" content="no-cache">
	<meta http-equiv="Pragma" content="no-cache">
	<meta http-equiv="Expires" content="-1">


    <title>Mapa</title>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
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
    </table>
    </form>
  </body>
    <script type="text/javascript">	
    document.getElementById("mapa").innerHTML = "Carregando...";
    function initialize() {
      if (GBrowserIsCompatible()) {
        //var map = new GMap2(document.getElementById("mapa"));
        var options = {showOnLoad : true, suppressInitialResultSelection : true};

        var map = new GMap2(document.getElementById("mapa"),{googleBarOptions:options}) 
       	 map.enableGoogleBar();
       	
       	// Preenchendo o mapa
		<?php if ($_REQUEST["latitude"]!='0'){ ?>
			var zoom = window.opener.document.getElementById("endzoom<? echo $_REQUEST['tipoendereco']; ?>").value;
			var lat = <? echo (($_REQUEST["polo"]=="S")?"-".$_REQUEST["latitude"]:$_REQUEST["latitude"]); ?>;
	        var lng = <? echo (($_REQUEST["polo"]=="S")?"-".$_REQUEST["longitude"]:$_REQUEST["longitude"]); ?>;
			if (zoom=='') zoom = 14;
			map.setCenter(new GLatLng(lat,lng), parseInt(zoom));

		    // Criando o ícone do mapa
			var baseIcon = new GIcon();
			baseIcon.shadow = "http://www.google.com/mapfiles/shadow50.png";
			baseIcon.iconSize = new GSize(18, 28);
			baseIcon.shadowSize = new GSize(18, 28);
			baseIcon.iconAnchor = new GPoint(9, 34);
			baseIcon.infoWindowAnchor = new GPoint(9, 2);
			baseIcon.infoShadowAnchor = new GPoint(18, 25);
			
			// Criando os Marcadores com o resultado
	        baseIcon.image='/imagens/icone_capacete_2.png'
			
			var posn = new GLatLng(lat, lng);
			var icon = baseIcon;
			var title = 'Clique para ver os detalhes';
			//var html = "não implementado";
			var html = "<iframe src='/apigoogle/php/mapa_balao.php?entid=<? echo $_REQUEST['entid']; ?>' width=350 frameborder=0 ></iframe>";
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

    function copiar(lat, lng, z){
		alert(lat);
		alert(lng);
		ddLat=lat+"";

		if (ddLat.substr(0,1) == "-") {
			ddLatVal = ddLat.substr(1,ddLat.length-1);
		} else {
			ddLatVal = ddLat;
		}
		
		// Graus Lat 
		ddLatVals = ddLatVal.split(".");
		dmsLatDeg = ddLatVals[0];
		window.opener.document.getElementById("graulatitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatDeg;
		
		// * 60 = mins
		ddLatRemainder  = ("0." + ddLatVals[1]) * 60;
		dmsLatMinVals   = ddLatRemainder.toString().split(".");
		dmsLatMin = dmsLatMinVals[0];
		window.opener.document.getElementById("minlatitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatMin;
			
		// * 60 novamente = secs
		ddLatMinRemainder = ("0." + dmsLatMinVals[1]) * 60;
		dmsLatSec  = Math.round(ddLatMinRemainder);
		window.opener.document.getElementById("seglatitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatSec;
		
		ddLat=lng+"";
		if (ddLat.substr(0,1) == "-") {
			ddLatVal = ddLat.substr(1,ddLat.length-1);
			window.opener.document.getElementById("pololatitude<? echo $_REQUEST['tipoendereco']; ?>").value = "S";
		} else {
			ddLatVal = ddLat;
			window.opener.document.getElementById("pololatitude<? echo $_REQUEST['tipoendereco']; ?>").value = "N";
		}
		// Graus Long 
		ddLatVals = ddLatVal.split(".");
		dmsLatDeg = ddLatVals[0];
		window.opener.document.getElementById("graulongitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatDeg;
		
		// * 60 = mins
		ddLatRemainder  = ("0." + ddLatVals[1]) * 60;
		dmsLatMinVals   = ddLatRemainder.toString().split(".");
		dmsLatMin = dmsLatMinVals[0];
		window.opener.document.getElementById("minlongitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatMin;
			
		// * 60 novamente = secs
		ddLatMinRemainder = ("0." + dmsLatMinVals[1]) * 60;
		dmsLatSec  = Math.round(ddLatMinRemainder);
		window.opener.document.getElementById("seglongitude<? echo $_REQUEST['tipoendereco']; ?>").value=dmsLatSec;

		if (ddLat.substr(0,1) == "-") {
			ddLatVal = ddLat.substr(1,ddLat.length-1);
			window.opener.document.getElementById("pololongitude<? echo $_REQUEST['tipoendereco']; ?>").value = "W";
		} else {
			ddLatVal = ddLat;
			window.opener.document.getElementById("pololongitude<? echo $_REQUEST['tipoendereco']; ?>").value = "E";
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
