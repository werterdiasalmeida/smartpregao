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


    <title>Google Maps - Rotas</title>
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
	.body {BORDER-BOTTOM: 0px; BORDER-LEFT: 0px; BORDER-RIGHT: 0px; BORDER-TOP: 0px}
</style>
</head>
<body>
	<div id="mapa" style="width: 100%; height: 450px"></div>
	<div id="routes" style="margin-top:10px; width: 98%; height: 210px;overflow: auto;font-size:verdana;font-size:11px" ></div>
</body>
  
<script type="text/javascript">	
document.getElementById("mapa").innerHTML = "Carregando...";

function initialize() {
	if (GBrowserIsCompatible()) {
	var map = new GMap2(document.getElementById("mapa"));
	var routes = document.getElementById("routes");
	var options = {showOnLoad : true, suppressInitialResultSelection : true};
	
	var map = new GMap2(document.getElementById("mapa"),{googleBarOptions:options}) 
	map.enableGoogleBar();
	map.addControl(new GSmallMapControl());
	map.addControl(new GMapTypeControl());
	map.enableScrollWheelZoom();
	var zoom = 4;
	//var lat_i = -14.689881; var lng_i = -52.373047; //Brasil
	
	<? if($_REQUEST['rota'] == "off"){?>
	
		var rotas = new Array();
		var ponto = new Array();
		 
		<?if($_REQUEST['rotas']){
			$rotas = explode("::rota::",$_REQUEST['rotas']);
			$i = 0;
			foreach($rotas as $rota){
				$rot = explode(",",$rota);?>
				rotas[<?=$i?>] = new GLatLng(<?=$rot[0]?>,<?=$rot[1]?>);
				ponto[<?=$i?>] = new GMarker(rotas[<?=$i?>], {draggable:false })
				map.addOverlay(ponto[<?=$i?>]);
			<?$i++;}?>
			
			map.setCenter(rotas[<?=($i - 1)?>], parseInt(zoom));
			
			alert("Não foi possível traçar a roda.");
			
		<? }?>
	        
	<? }else{ ?>
	
		directions = new GDirections(map,routes);
		 
		var rotas = new Array();
			
		<?if($_REQUEST['rotas']){
			$rotas = explode("::rota::",$_REQUEST['rotas']);
			$i = 0;
			foreach($rotas as $rota){
			$rot = explode(",",$rota);?>
			rotas[<?=$i?>] = new GLatLng(<?=$rot[0]?>,<?=$rot[1]?>);<?$i++;}?>
		<? }?>
		
			directions.loadFromWaypoints(rotas);
			
			GEvent.addListener(directions, "error", function(){
	            window.location.href= 'mapa_rotas.php?rotas=<?=$_REQUEST['rotas']?>&rota=off'
	        });
	  
	<? } ?> 
		
	}
}

initialize();
</script>
</html>