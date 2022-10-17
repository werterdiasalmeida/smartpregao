<?
//Funções
include (APPRAIZ.'www/painel/_funcoes_painel_controle.php');
include_once (APPRAIZ.'www/painel/_funcoesdetalhamentoindicador.php');

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


// verificando a validade do indicador
if(is_numeric($_REQUEST['indid'])) {
	$indid = $db->pegaUm("SELECT indid FROM painel.indicador WHERE indid='".$_REQUEST['indid']."'");
	// testando se existe
	if($indid) {
		$_SESSION['indid'] = $indid;
	} else {
		echo "<script>alert('Indicador informado não existe');window.location='?modulo=inicio&acao=C';</script>";
		exit;
	}
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

if($_SESSION['indid']):
	if (!$cabecalho_painel) $db->cria_aba($abacod_tela,$url,'');
	!$_SESSION['indid']? $_SESSION['indid'] = $_REQUEST['indid']: $_SESSION['indid'] = $_SESSION['indid'];
	
	/*
	 * VALIDANDO SE USUÁRIO POSSUI PERMISSÃO PARA ACESSAR
	 * O INDICADOR
	 */
	$permissoes = verificaPerfilPainel();
	if($permissoes['verindicadores'] == 'vertodos') {
		$acesso = true;
	} else {
		$acesso = validaAcessoIndicadores($permissoes['verindicadores'], $_SESSION['indid']);
	}
	/*
	 * FIM
	 * VALIDANDO SE USUÁRIO POSSUI PERMISSÃO PARA ACESSAR
	 * O INDICADOR
	 */
endif;

if($_REQUEST['relxls']){
	include "geraxls_resultado.inc";
	exit;	
}

$titulo = "Painel do Indicador";
monta_titulo( $titulo, '' );

if($_SESSION['indid'] && !$_REQUEST['evento']){
	$_REQUEST['evento'] = 'A';
}

if( (isset($_REQUEST[evento]) && ($_REQUEST[evento] != '')) || $_SESSION['indid']){
	
	if($_SESSION['indid']){
		$_REQUEST['indid'] = $_SESSION['indid'];
	}
	switch($_REQUEST[evento]) {
	
		case 'A':
			$grafico = "Entra gráfico!"; 
		break;	
	}
}


//Dados do indicador para utilização na tela
if($_SESSION['indid']){
	$sql = "select indqtdevalor from painel.indicador where indid = {$_SESSION['indid']}";
	$dadosInd = $db->pegaLinha($sql);
	$indqtdevalor = $dadosInd['indqtdevalor'];
} 

?>
<script language="javascript" type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
<script type="text/javascript" src="/includes/JQuery/interface.js"></script>
<script language="JavaScript" src="./js/painel.js"></script>
<script language="JavaScript" src="./js/detalhamentoindicador.js"></script>

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
<?if ($local[2]=="simec-sdh"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAA5oufaSx1PgyKsR79BVQ9MhQ11pDnvwvLkkpC_LAV6dkUcsfjXxR5Fd0F2rKLhvlK_dwkTI1Q2fbw_w" type="text/javascript"></script>
	<? $GKey = "ABQIAAAA5oufaSx1PgyKsR79BVQ9MhQ11pDnvwvLkkpC_LAV6dkUcsfjXxR5Fd0F2rKLhvlK_dwkTI1Q2fbw_w"; ?> 	
<? } ?>
<?if ($local[2]=="sig.sdh.gov.br"){ ?>
	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAA5oufaSx1PgyKsR79BVQ9MhR4i7N7ksIAYgqL8VHrtx8huyb0ZxRDJ3fl3exO65DtxDbDxyJ9SRsuvA" type="text/javascript"></script>
	<? $GKey = "ABQIAAAA5oufaSx1PgyKsR79BVQ9MhR4i7N7ksIAYgqL8VHrtx8huyb0ZxRDJ3fl3exO65DtxDbDxyJ9SRsuvA"; ?> 	
<? } ?>

<script type="text/javascript">

/* FUNÇÕES DO MAPA */

var marcadores = new Array();
var detalhesMarcadores = new Array();

function createMarker(posn, title, icon, html,muncod) {
	var marker = new GMarker(posn, {title: title, icon: icon, draggable:false });
	if(html != false){
		GEvent.addListener(marker, "click", function() {
		marker.openInfoWindowHtml(html);
		});
	}
	return marker;
}

function exibePontos(obj){
	
	if(obj.checked == true){
		if(markerGroups[obj.value]){
			for (var i = 0; i < markerGroups[obj.value].length; i++) {
		        var marker = markerGroups[obj.value][i];
		        marker.show();
			}
		}	
	}else{
		if(markerGroups[obj.value]){
			for (var i = 0; i < markerGroups[obj.value].length; i++) {
		        var marker = markerGroups[obj.value][i];
		        marker.hide();
			}
		}
	}		

}

var markerGroups = { '1': []};

function initialize(indid) {
	if (GBrowserIsCompatible()) { // verifica se o navegador é compatível
			document.getElementById('linha_mapa_'+indid).style.display='';
			map = new GMap2(document.getElementById('div_exibe_mapa_'+indid)); // inicila com a div mapa
			var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;	//Brasil	
			map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom)); //Centraliza e aplica o zoom
			
			// Início Controles
			map.addControl(new GMapTypeControl());
			map.addControl(new GLargeMapControl3D());
//	        map.addControl(new GOverviewMapControl());
	        map.enableScrollWheelZoom();
	        map.addMapType(G_PHYSICAL_MAP);
	        // Fim Controles
	
	}
}

function fecharGlobo(indid) {
	document.getElementById('linha_mapa_'+indid).style.display = 'none';
	document.getElementById('linha_mapa_'+indid+'_').style.display = 'none';
	document.getElementById('div_exibe_mapa_'+indid).innerHTML = '';
	document.getElementById('info_mapa_'+indid).innerHTML = '';
}

function exibeGrafico(indid, params) {

	initialize(indid);
	divCarregando(document.getElementById('linha_mapa_'+indid));	
	var baseIcon2 = new GIcon();
	baseIcon2.iconSize = new GSize(15, 17);
	baseIcon2.iconAnchor = new GPoint(11, 13);
	baseIcon2.infoWindowAnchor = new GPoint(9, 2);
	baseIcon2.image='/imagens/icones/tb.png';
	
	xml_filtro = 'painel.php?modulo=principal/detalhamentoIndicador&acao=A&requisicao=exibeMapaDetalhamento&indid='+indid+'&filtro='+params;
		
	// Criando os Marcadores com o resultado
	GDownloadUrl(xml_filtro, function(data) {
		var xml = GXml.parse(data);
		
		var markers = xml.documentElement.getElementsByTagName("marker");
		
		if(markers.length > 0) {
			var lat_ant=0;
			var lng_ant=0;
				
			for (var i = 0; i < markers.length; i++) {
						
				var mundsc = markers[i].getAttribute("mundsc");
				var estuf = markers[i].getAttribute("estuf");
				var muncod = markers[i].getAttribute("muncod");
				var qtde = markers[i].getAttribute("qtde");
				var valor = markers[i].getAttribute("valor");
				var descricao = markers[i].getAttribute("descricao");
				var endereco = markers[i].getAttribute("endereco");
				var telefone = markers[i].getAttribute("telefone");
				title = mundsc; 
										
				icon = baseIcon2;
				
				var lat = markers[i].getAttribute("lat");
				var lng = markers[i].getAttribute("lng");
				
				var html;
						
				html = "<div style=\"font-family:verdana;font-size:11px;padding:10px\" >";
				html += "<b>Localização:</b> " + mundsc + "/" + estuf + "<br /><br />";
				html += "<b>Quantidade:</b> " + qtde + "<br /><br />";
				if(valor != '') {
					html += "<b>Valor R$:</b> " + valor + "<br /><br />";
				}
				html += "</div>";

				// Verifica pontos em um mesmo lugar e move o seguinte para a direita
				if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
					var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
				else
					var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
	
				lat_ant=markers[i].getAttribute("lat");
				lng_ant=markers[i].getAttribute("lng");
				
				// Cria o marcador na tela
				var marker = createMarker(point,title,icon,html);
				
				markerGroups['1'].push(marker);
				
				marcadores[muncod]=marker;
				detalhesMarcadores[muncod]=html;
				
				map.addOverlay(marker);
				
		}
		}else{
			alert("Não existem municipios neste indicador")
		}
	});
	

	var myAjax = new Ajax.Request(
		'painel.php?modulo=principal/detalhamentoIndicador&acao=A',
		{
			method: 'post',
			parameters: 'requisicao=exibeInformacoesMapa&indid='+indid+'&filtro='+params,
			asynchronous: false,
			onComplete: function(resp) {
				extrairScript(resp.responseText);
				document.getElementById('info_mapa_'+indid).innerHTML = resp.responseText;
			},
			onLoading: function(){
				document.getElementById(iddestinatario).innerHTML = 'Carregando...';
			}
		});
	
	
	divCarregado(document.getElementById('linha_mapa_'+indid));


}

function abrebalao(marcador){
	marcadores[marcador].openInfoWindowHtml(detalhesMarcadores[marcador]);
}


/* FUNÇÕES DO MAPA */





function abrasil() {
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&indid=&paiid=1','Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function amun(muncod) {
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=municipio&indid=<? echo $_SESSION['indid'] ?>&muncod='+muncod,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aest(estuf) {
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=estado&indid=<? echo $_SESSION['indid'] ?>&estuf='+estuf,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aesc(escodinep){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=escola&indid=<? echo $_SESSION['indid'] ?>&esccodinep='+escodinep,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
}
function auni(unicod){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=universidade&indid=<? echo $_SESSION['indid'] ?>&unicod='+unicod,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
}
function aies(iesid) {
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=ies&indid=<? echo $_SESSION['indid'] ?>&dshcod='+iesid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aiep(iepid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=posgraduacao&indid=<? echo $_SESSION['indid'] ?>&iepid='+iepid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function acampusprof(entid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=campusprofissional&indid=<? echo $_SESSION['indid'] ?>&entid='+entid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function acampussup(entid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=campussuperior&indid=<? echo $_SESSION['indid'] ?>&entid='+entid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function ains(unicod){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=instituto&indid=<? echo $_SESSION['indid'] ?>&unicod='+unicod,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function ahosp(entid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=hospital&entid='+entid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function apol(polid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=polo&polid='+polid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function atip(tpmid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=tipomunicipio&tpmid='+tpmid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function agrp(gtmid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=grupotipomunicipio&gtmid='+gtmid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}

function aacao(acaid,estuf){
	if(estuf){
		var reg = "estado";
		var getEstuf = "&estuf=" + estuf;
	}else{
		var reg = "pais";
		var getEstuf = "";
	}
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=' + reg + '&acaid=' + acaid + getEstuf,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function asec(secid,estuf){
	if(estuf){
		var reg = "estado";
		var getEstuf = "&estuf=" + estuf;
	}else{
		var reg = "pais";
		var getEstuf = "";
	}
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=' + reg + '&secid=' + secid + getEstuf,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function ames(mescod){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=mesoregiao&mescod=' + mescod ,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}  
function areg(regcod){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=regiao&regcod=' + regcod ,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function amicro(miccod){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=microregiao&miccod='+miccod,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
} 
</script>

<style>
.boxindicador {
	width:400px;
	height:100px;
	position: absolute;
	background-color: #FCFCFC;
	border: solid 1px #000000;
	padding: 3px;
	margin-left:30px;
	display:none;
}
</style>

<?php 
//Verifica se o indicador possui série histórica, se nã oexistir o usário é redirecionado
if(!$_SESSION['indid']) die("<script>alert('Variavel do indicador não encontrado');window.location='painel.php?modulo=inicio&acao=C';</script>");
$sql = "select indid from painel.seriehistorica where indid = {$_SESSION['indid']} and sehstatus <> 'I'";
if(!$db->pegaUm($sql)){ ?>
	<script type="text/javascript">
		alert('Série Histórica inexistente!');
		history.back(-1);
	</script>
<?php }  ?>

<?
$indid = !$_REQUEST['indid'] ? $_SESSION['indid'] : $_REQUEST['indid'];

?>

<input type="hidden" name="evento" id="evento" value="I">
<input type="hidden" name="indid" id="indid" value="<?=$indid?>">

<table style="border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
    <td><? exibedashBoard($indid); ?></td>
</tr>
</table>

<script>

var globalVisualizaMapa = false;

//Exibe o Objeto
function exibeID(id){
	document.getElementById(id).style.display = '';
}

//Esconde o Objeto
function escondeID(id){
	if(document.getElementById(id)){
		document.getElementById(id).style.display = 'none';
	}
}
//Função que indica que a função está sendo carregada
function Carregando(div){
	if(document.getElementById(div)){
		document.getElementById(div).innerHTML = '<center><img src="../imagens/wait.gif" /> Carregado... </center>';
	}
}
//Função que muda o evento
function mudaEvento(id,evento,funcao){
	if(document.getElementById(id)){
		var obj = document.getElementById(id);
		obj.setAttribute(evento, funcao);
	}
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
				//Procura pelo ponto do nome da extensao do arquivo e marca para depois dele  
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

jQuery.noConflict();

jQuery(document).ready(function() {
        jQuery().click(function () {
        	jQuery('#abaid_<? echo $abaid ?>').hide();
        });
        jQuery("#exibe_edita_aba").click(function () {
        	jQuery('#abaid_<? echo $abaid ?>').show();
        	return false;
        });
});

jQuery(document).ready(function() {
        jQuery().click(function () {
        	jQuery('#abaid_<? echo $abaid ?>').hide();
        	jQuery('#exibe_escolhe_add_aba').hide();
        });
        jQuery("#exibe_edita_aba").click(function () {
        	jQuery('#abaid_<? echo $abaid ?>').show();
        	return false;
        });
        jQuery("#exibe_add_aba").click(function () {
        	jQuery('#exibe_escolhe_add_aba').show();
        	return false;
        });
});

function escondePeriodo(tipografico){
			
			var indqtdevalor = '<? echo trim($indqtdevalor) ?>'
			
			if(tipografico == "linha" && indqtdevalor == "t"){
				document.getElementById('grafico_linha_financeiro').style.display = "";
			}else{
				document.getElementById('grafico_linha_financeiro').style.display = "none";
			}
			
			//if(document.getElementById('opc_periodicidade'))
				//document.getElementById('opc_periodicidade').style.display = "";

			if(tipografico != "linha" && tipografico != "barra"){
				
//				if(document.getElementById('opc_periodicidade'))
//				document.getElementById('opc_periodicidade').style.display = "none";
			
				var arrTipoGrafico = tipografico.split('_');
				var tpGrafico = arrTipoGrafico[0];
				var fatia =  arrTipoGrafico[1];
				var idFatia = arrTipoGrafico[2];
			}else{
				document.getElementById('ate_periodo').style.display = "";
				document.getElementById('periodo_fim').style.display = "";
			}
}

function compartilharAba(abaid){
	if(confirm("Deseja compartilhar esta aba e seu conteudo com outros usuários?")){
		var myAjax = new Ajax.Request(
				'painel.php?modulo=principal/painel_controle&acao=A',
				{
					method: 'post',
					parameters: "compartilharAba=true&abaid=" + abaid,
					asynchronous: false,
					onComplete: function(resp){
						alert('Aba compartilhada com sucesso!');
					},
					onLoading: function(){
					}
			});
	}
}
		
function mudaTipoGrafico2(indid,divMapa){
			if(!document.getElementById('tipo_grafico')){
				var tipo_grafico = "null";
			}else{
				var tipo_grafico = document.getElementById('tipo_grafico').value;
			}
			mudaTipoGrafico(tipo_grafico,indid,divMapa)
}
function exibeMapaDashBoard(indid,estuf,muncod,gtmid,tpmid,visualizaMapa,DivMapa){
			var dpeid_inicio = document.getElementById('periodo_inicio').value;
			var dpeid_fim = document.getElementById('periodo_fim').value;
			var sehid = document.getElementById('ultima_sehid').value;
			
			if(document.getElementById('regcod'))
				var regcod = document.getElementById('regcod').value;
			else
				var regcod = "todos"
			
			if(DivMapa){
				var mapa = document.getElementById(DivMapa);
			}else{
				var mapa = document.getElementById('div_mapa');
			}
			if(document.getElementById('tidid1'))
				var tidid1 = document.getElementById('tidid1').value;
			else
				var tidid1 = "";
			if(document.getElementById('tidid2'))
				var tidid2 = document.getElementById('tidid2').value;
			else
				var tidid2 = "";
				
			if(visualizaMapa == true){
				globalVisualizaMapa = true;
			}
			
			if(document.getElementById('abaPadrao')){
				var ValAbaPadrao = document.getElementById('abaPadrao').value;
				
				if(ValAbaPadrao == "graficos"){
					mapa = document.getElementById('div_mapa2');
				}
				if(ValAbaPadrao == "mapa"){
					mapa = document.getElementById('div_mapa');
				}
				if(!ValAbaPadrao){
					mapa = document.getElementById('div_mapa2');
				}
				
			}
			
			if(mapa)
				mapa.innerHTML = "<div style=\"margin-top:15%;text-align:center;width:100%;\" ><img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\"><br />Aguarde...Carregando.</div>";
			
			var myAjax = new Ajax.Request(
				'painel.php?modulo=principal/painel_controle&acao=A',
				{
					method: 'post',
					parameters: "exibeMapaDashBoard=true&indid=" + indid + "&sehid=" + sehid + "&dpeid_inicio=" + dpeid_inicio + "&dpeid_fim=" + dpeid_fim + "&estuf=" + estuf + "&muncod=" + muncod + "&visualizaMapa=" + visualizaMapa + "&gtmid=" + gtmid + "&tpmid=" + tpmid + "&tidid1=" + tidid1 + "&tidid2=" + tidid2 + "&regcod=" + regcod + "&abaPadrao=" + ValAbaPadrao + "&DivMapa=" + DivMapa,
					asynchronous: false,
					onComplete: function(resp){
						mapa.innerHTML = resp.responseText;
						extraiScript(resp.responseText);
					},
					onLoading: function(){
						mapa.innerHTML = "<div style=\"margin-top:15%;text-align:center;width:100%;\" ><img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\"><br />Aguarde...Carregando.</div>";
					}
			});
			if(estuf && globalVisualizaMapa == true && document.getElementById('div_exibe_mapa')) {
				setTimeout("flashProxy.call('zoomTo', '"+estuf+"')", 3000);
			}
}

function mudaTipoGrafico(tipografico,indid,divMapa){
		var dpeid_inicio = document.getElementById('periodo_inicio').value;
		var dpeid_fim = document.getElementById('periodo_fim').value;
		var sehid = document.getElementById('ultima_sehid').value;
		var regid = document.getElementById('regid').value;
		if(document.getElementById('unidade_inteiro')){
			var unidade_inteiro = document.getElementById('unidade_inteiro').value;
		}else{
			var unidade_inteiro = "null";
		}
		if(document.getElementById('unidade_moeda')){
			var unidade_moeda = document.getElementById('unidade_moeda').value;
		}else{
			var unidade_moeda = "null";
		}
		if(document.getElementById('indice_moeda')){
			var indice_moeda = document.getElementById('indice_moeda').value;
		}else{
			var indice_moeda = "null";
		}
		if(document.getElementById('regcod')){
			var regcod = document.getElementById('regcod').value;
		}else{
			var regcod = "todos";
		}
		if(document.getElementById('estuf')){
			var estuf = document.getElementById('estuf').value;
		}else{
			var estuf = "todos";
		}
		if(document.getElementById('muncod')){
			var muncod = document.getElementById('muncod').value;
		}else{
			var muncod = "todos";
		}
		if(document.getElementById('gtmid')){
			var gtmid = document.getElementById('gtmid').value;
		}else{
			var gtmid = "todos";
		}
		if(document.getElementById('tpmid')){
			var tpmid = document.getElementById('tpmid').value;
		}else{
			var tpmid = "todos";
		}
		if(document.getElementById('tidid1')){
			var tidid1 = document.getElementById('tidid1').value;
		}else{
			var tidid1 = "todos";
		}
		if(document.getElementById('tidid2')){
			var tidid2 = document.getElementById('tidid2').value;
		}else{
			var tidid2 = "todos";
		}
		if(document.getElementById('finac_qtde')){
			if(document.getElementById('finac_qtde').checked == true)
				var finac_qtde = 1;
			else
				var finac_qtde = "";
		}else{
			var finac_qtde = "";
		}
		if(document.getElementById('finac_valor')){
			if(document.getElementById('finac_valor').checked == true)
				var finac_valor = 1;
			else
				var finac_valor = "";
		}else{
			var finac_valor = "";
		}
		
		if(document.getElementById('finac_ambos')){
			if(document.getElementById('finac_ambos').checked == true){
				var finac_valor = 1;
				var finac_qtde = 1;
			}
		}
		
		if(document.getElementById('sel_periodicidade')){
			var periodicidade = document.getElementById('sel_periodicidade').value;
		}else{
			var periodicidade = "";
		}
		if(document.getElementById('regvalue')){
			var regvalue = document.getElementById('regvalue').value;
		}else{
			var regvalue = "";
		}
		
		var erro = "false";
									
		if(tipografico == "barra"){
			var myAjax = new Ajax.Request(
								'painel.php?modulo=principal/painel_controle&acao=A',
								{
									method: 'post',
									parameters: "testaDatasDpeid=true&dpeid1=" + dpeid_inicio + "&dpeid2=" + dpeid_fim,
									asynchronous: false,
									onComplete: function(resp){
										erro = resp.responseText;
									},
									onLoading: function(){
									}
			});
			//geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda + ";" + estuf + ";" + muncod + ";" + gtmid + ";" + tpmid  + ";" + tidid1  + ";" + tidid2 + ";" + regcod + ";" + ";" + ";" + periodicidade;
			
			geraGrafico = "geraGrafico.php?tipo="  			 + tipografico     + 
										 ";indid=" 			 + indid 		   + 
										 ";dpeid=" 			 + dpeid_inicio    +
										 ";dpeid2=" 		 + dpeid_fim 	   +
										 ";unidade_inteiro=" + unidade_inteiro +
										 ";unidade_moeda="   + unidade_moeda   +
										 ";indice_moeda="    + indice_moeda    +
										 ";estuf="           + estuf    	   +
										 ";muncod="          + muncod    	   +
										 ";gtmid="           + gtmid    	   +
										 ";tpmid="           + tpmid    	   +
										 ";tidid1="          + tidid1    	   +
										 ";tidid2="          + tidid2    	   +
										 ";regcod="          + regcod    	   +
										 ";periodicidade="   + periodicidade   +
										 ";regvalue="   + regvalue
			
			var altura = "90%";
			var largura = "100%";
		}
		if(tipografico == "linha"){
			
			if(document.getElementById('tipo_grafico'))
				escondePeriodo(document.getElementById('tipo_grafico').value);
				
			var myAjax = new Ajax.Request(
								'painel.php?modulo=principal/painel_controle&acao=A',
								{
									method: 'post',
									parameters: "testaDatasDpeid=true&dpeid1=" + dpeid_inicio + "&dpeid2=" + dpeid_fim,
									asynchronous: false,
									onComplete: function(resp){
										erro = resp.responseText;
									},
									onLoading: function(){
									}
			});
			//geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + estuf + ";" + muncod  + ";" + gtmid + ";" + tpmid  + ";" + tidid1  + ";" + tidid2 + ";" + regcod + ";" + finac_qtde + ";" + finac_valor + ";" + periodicidade;
			
			geraGrafico = "geraGrafico.php?tipo="  			 + tipografico     + 
										 ";indid=" 			 + indid 		   + 
										 ";dpeid=" 			 + dpeid_inicio    +
										 ";dpeid2=" 		 + dpeid_fim 	   +
										 ";unidade_inteiro=" + unidade_inteiro +
										 ";unidade_moeda="   + unidade_moeda   +
										 ";indice_moeda="    + indice_moeda    +
										 ";estuf="           + estuf    	   +
										 ";muncod="          + muncod    	   +
										 ";gtmid="           + gtmid    	   +
										 ";tpmid="           + tpmid    	   +
										 ";tidid1="          + tidid1    	   +
										 ";tidid2="          + tidid2    	   +
										 ";regcod="          + regcod    	   +
										 ";finac_qtde="      + finac_qtde      +
										 ";finac_valor="     + finac_valor     +
										 ";periodicidade="   + periodicidade   +
										 ";regvalue="   + regvalue
			
			var altura = "90%";
			var largura = "100%";
		}
		if(tipografico != "linha" && tipografico != "barra" && tipografico != "null"){
			var arrTipoGrafico = tipografico.split('_');
			var tpGrafico = arrTipoGrafico[0];
			var fatia =  arrTipoGrafico[1];
			var idFatia = arrTipoGrafico[2];
			var altura = "80%";
			var largura = "100%";
			if(tpGrafico == "barra"){
				var myAjax = new Ajax.Request(
								'painel.php?modulo=principal/painel_controle&acao=A',
								{
									method: 'post',
									parameters: "testaDatasDpeid=true&dpeid1=" + dpeid_inicio + "&dpeid2=" + dpeid_fim,
									asynchronous: false,
									onComplete: function(resp){
										erro = resp.responseText;
									},
									onLoading: function(){
									}
				});
				//geraGrafico = "geraGrafico.php?tipo=" + indid + ";barra_fatia;" + sehid + ";tdiid" + fatia + "_" + idFatia + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + estuf + ";" + muncod  + ";" + gtmid + ";" + tpmid  + ";" + tidid1  + ";" + tidid2 + ";" + regcod;
				
				geraGrafico = "geraGrafico.php?tipo=barra_fatia"	           		  + 
										 ";indid=" 			 + indid 		   		  + 
										 ";sehid=" 			 + sehid 		   		  +
										 ";tidid=tdiid"		 + fatia + "_" +  idFatia +
										 ";dpeid=" 			 + dpeid_inicio    		  +
										 ";dpeid2=" 		 + dpeid_fim 	   		  +
										 ";unidade_inteiro=" + unidade_inteiro 		  +
										 ";unidade_moeda="   + unidade_moeda   		  +
										 ";indice_moeda="    + indice_moeda    		  +
										 ";estuf="           + estuf    	   		  +
										 ";muncod="          + muncod    	   		  +
										 ";gtmid="           + gtmid    	   		  +
										 ";tpmid="           + tpmid    	   		  +
										 ";tidid1="          + tidid1    	   		  +
										 ";tidid2="          + tidid2    	   		  +
										 ";regcod="          + regcod    	   		  +
										 ";finac_qtde="      + finac_qtde      		  +
										 ";finac_valor="     + finac_valor     		  +
										 ";periodicidade="   + periodicidade		  +
										 ";regvalue="   + regvalue
			}
			if(tpGrafico == "barracomp"){
				var myAjax = new Ajax.Request(
								'painel.php?modulo=principal/painel_controle&acao=A',
								{
									method: 'post',
									parameters: "testaDatasDpeid=true&dpeid1=" + dpeid_inicio + "&dpeid2=" + dpeid_fim,
									asynchronous: false,
									onComplete: function(resp){
										erro = resp.responseText;
									},
									onLoading: function(){
									}
				});
				//geraGrafico = "geraGrafico.php?tipo=" + indid + ";barra_comp;" + sehid + ";tdiid" + fatia + "_" + idFatia + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + estuf + ";" + muncod  + ";" + gtmid + ";" + tpmid  + ";" + tidid1  + ";" + tidid2 + ";" + regcod;
				
				geraGrafico = "geraGrafico.php?tipo=barra_comp"		           		  + 
										 ";indid=" 			 + indid 		   		  + 
										 ";sehid=" 			 + sehid 		   		  +
										 ";tidid=tdiid"		 + fatia + "_" +  idFatia +
										 ";dpeid=" 			 + dpeid_inicio    		  +
										 ";dpeid2=" 		 + dpeid_fim 	   		  +
										 ";unidade_inteiro=" + unidade_inteiro 		  +
										 ";unidade_moeda="   + unidade_moeda   		  +
										 ";indice_moeda="    + indice_moeda    		  +
										 ";estuf="           + estuf    	   		  +
										 ";muncod="          + muncod    	   		  +
										 ";gtmid="           + gtmid    	   		  +
										 ";tpmid="           + tpmid    	   		  +
										 ";tidid1="          + tidid1    	   		  +
										 ";tidid2="          + tidid2    	   		  +
										 ";regcod="          + regcod    	   		  +
										 ";finac_qtde="      + finac_qtde      		  +
										 ";finac_valor="     + finac_valor     		  +
										 ";periodicidade="   + periodicidade		  +
										 ";regvalue="   + regvalue
			}
			if(tpGrafico == "pizza"){
					var myAjax = new Ajax.Request(
							'painel.php?modulo=principal/painel_controle&acao=A',
							{
								method: 'post',
								parameters: "testaDatasDpeid=true&dpeid1=" + dpeid_inicio + "&dpeid2=" + dpeid_fim,
								asynchronous: false,
								onComplete: function(resp){
									erro = resp.responseText;
								},
								onLoading: function(){
								}
				});
				//geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tpGrafico + ";" + sehid + ";tdiid" + fatia + "_" + idFatia + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + estuf + ";" + muncod  + ";" + gtmid + ";" + tpmid  + ";" + tidid1  + ";" + tidid2 + ";" + regcod;
				
				geraGrafico = "geraGrafico.php?tipo="        + tpGrafico			  +
										 ";indid=" 			 + indid 		   		  + 
										 ";sehid=" 			 + sehid 		   		  +
										 ";tidid=tdiid"		 + fatia + "_" +  idFatia +
										 ";dpeid=" 			 + dpeid_inicio    		  +
										 ";dpeid2=" 		 + dpeid_fim 	   		  +
										 ";unidade_inteiro=" + unidade_inteiro 		  +
										 ";unidade_moeda="   + unidade_moeda   		  +
										 ";indice_moeda="    + indice_moeda    		  +
										 ";estuf="           + estuf    	   		  +
										 ";muncod="          + muncod    	   		  +
										 ";gtmid="           + gtmid    	   		  +
										 ";tpmid="           + tpmid    	   		  +
										 ";tidid1="          + tidid1    	   		  +
										 ";tidid2="          + tidid2    	   		  +
										 ";regcod="          + regcod    	   		  +
										 ";finac_qtde="      + finac_qtde      		  +
										 ";finac_valor="     + finac_valor     		  +
										 ";periodicidade="   + periodicidade		  +
										 ";regvalue="   + regvalue
			}
			

		}
		if(tipografico == "null"){
			var myAjax = new Ajax.Request(
					'painel.php?modulo=principal/painel_controle&acao=A',
					{
						method: 'post',
						parameters: "testaDatasDpeid=true&dpeid1=" + dpeid_inicio + "&dpeid2=" + dpeid_fim,
						asynchronous: false,
						onComplete: function(resp){
							erro = resp.responseText;
						},
						onLoading: function(){
						}
			});
			
			//geraGrafico = "geraGrafico.php?tipo=" + indid + ";linha;" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda + ";" + estuf + ";" + muncod + ";" + gtmid + ";" + tpmid  + ";" + tidid1  + ";" + tidid2 + ";" + regcod;
			
			geraGrafico = "geraGrafico.php?"        								  +
										 ";indid=" 			 + indid 		   		  + 
										 ";sehid=" 			 + sehid 		   		  +
										 ";tidid=tdiid"		 + fatia + "_" +  idFatia +
										 ";dpeid=" 			 + dpeid_inicio    		  +
										 ";dpeid2=" 		 + dpeid_fim 	   		  +
										 ";unidade_inteiro=" + unidade_inteiro 		  +
										 ";unidade_moeda="   + unidade_moeda   		  +
										 ";indice_moeda="    + indice_moeda    		  +
										 ";estuf="           + estuf    	   		  +
										 ";muncod="          + muncod    	   		  +
										 ";gtmid="           + gtmid    	   		  +
										 ";tpmid="           + tpmid    	   		  +
										 ";tidid1="          + tidid1    	   		  +
										 ";tidid2="          + tidid2    	   		  +
										 ";regcod="          + regcod    	   		  +
										 ";finac_qtde="      + finac_qtde      		  +
										 ";finac_valor="     + finac_valor     		  +
										 ";periodicidade="   + periodicidade		  +
										 ";regvalue="   + regvalue
			
		}
		if(erro.search("false") >= 0){
			if(tipografico != "null"){
				//Verifica se o grafico pode ser exibido
				var myAjax = new Ajax.Request(
						'painel.php?modulo=principal/painel_controle&acao=A',
						{
							method: 'post',
							parameters: "verificaExibeGrafico=true&indid=" + indid + "&estuf=" + estuf + "&muncod=" + muncod  + "&gtmid=" + gtmid + "&tpmid=" + tpmid + "&tidid1=" + tidid1  + "&tidid2=" + tidid2 + "&regcod=" + regcod,
							asynchronous: false,
							onComplete: function(resp){
								var exibe = resp.responseText;
								if(exibe.search("true") >= 0){
									swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_indicador", largura, altura, "9.0.0", "expressInstall.swf", {"data-file":geraGrafico,"loading":"Carregando gráfico..."} );
								}else{
									document.getElementById('div_graficos').innerHTML = '<fieldset style="height: 400px;"><legend>Gráficos</legend><div style="overflow: auto; width: 100%; height: 100%;"><div id="grafico_indicador">' + exibe + '</div><div id="legenda_grafico"/></div></fieldset>';
									document.getElementById('div_graficos').innerHTML+= '<div><fieldset><legend>Tabela do Indicador</legend><div id="tabela_indicador_db"></div>';
								}						
							},
							onLoading: function(){
								document.getElementById('grafico_indicador').innerHTML = "<center>Carregando...</center>";
							}
					});
				
				if(tipografico != "linha" && tipografico != "barra"){
					var myAjax = new Ajax.Request(
								'painel.php?modulo=principal/painel_controle&acao=A',
								{
									method: 'post',
									parameters: "exibeLegendaGrafico=true&indid=" + indid + "&sehid=" + sehid + "&tdiid=" + fatia + "&valorTdiid=" + idFatia + "&dpeid_inicio=" + dpeid_inicio + "&dpeid_fim=" + dpeid_fim + "&estuf=" + estuf + "&muncod=" + muncod + "&gtmid=" + gtmid  + "&tpmid=" + tpmid   + "&tidid1=" + tidid1  + "&tidid2=" + tidid2 + "&regcod=" + regcod,
									asynchronous: false,
									onComplete: function(resp){
										document.getElementById('legenda_grafico').innerHTML = resp.responseText;
									},
									onLoading: function(){
									}
								});
				}else{
					document.getElementById('legenda_grafico').innerHTML = "";
				}
			}
			
			if(document.getElementById('div_mapa') || document.getElementById('div_mapa2'))
				exibeMapaDashBoard(indid,estuf,muncod,gtmid,tpmid,globalVisualizaMapa,divMapa);
			
			if(document.getElementById('exibe_serie_historica_db'))
				exibeSerieHistoricaDashBoard(indid,geraGrafico);
			
			if(document.getElementById('tabela_indicador_db'))
				atualizaTabelaIndicador(indid,geraGrafico);
			
		}else{
			alert('A data inícial deve ser menor ou igual a data final!');
		}
			
}

function atualizaTabelaIndicador(indid,parametros){

	var arrParametros = parametros.split('geraGrafico.php?tipo=');
	var parametro1 = arrParametros[1];

	var myAjax = new Ajax.Request(
								'painel.php?modulo=principal/painel_controle&acao=A',
								{
									method: 'post',
									parameters: "atualizaTabelaIndicador=true&indid=" + indid + "&parametros=" + parametro1,
									asynchronous: false,
									onComplete: function(resp){
										document.getElementById('tabela_indicador_db').innerHTML = resp.responseText;
									},
									onLoading: function(){
									}
								});
}

function exibeSerieHistoricaDashBoard(indid,parametros){
		
			var arrParametros = parametros.split('geraGrafico.php?tipo=');
			var parametro1 = arrParametros[1];
			
			if(document.getElementById('exibe_serie_historica_db'))
				document.getElementById('exibe_serie_historica_db').innerHTML = "<div style=\"margin-top:15%;text-align:center;width:100%;\" ><img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\"><br />Aguarde...Carregando.</div>";
						
			var myAjax = new Ajax.Request(
									'painel.php?modulo=principal/painel_controle&acao=A',
									{
										method: 'post',
										parameters: "exibeSerieHistoricaDashBoard=true&indid=" + indid + "&parametros=" + parametro1,
										asynchronous: false,
										onComplete: function(resp){
											document.getElementById('exibe_serie_historica_db').innerHTML = resp.responseText;
										},
										onLoading: function(){
											document.getElementById('exibe_serie_historica_db').innerHTML = "<div style=\"margin-top:15%;text-align:center;width:100%;\" ><img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\"><br />Aguarde...Carregando.</div>";
										}
			});
}

function filtraMunicipio(estuf,muncod){
		var div = document.getElementById('exibe_municipio');
			if(estuf != "todos"){
				var myAjax = new Ajax.Request(
					'painel.php?modulo=principal/painel_controle&acao=A',
					{
						method: 'post',
						parameters: "filtraMunicipio=true&estuf=" + estuf,
						asynchronous: false,
						onComplete: function(resp){
							div.innerHTML = resp.responseText;
							if(muncod)
								document.getElementById('muncod').value = muncod;
						},
						onLoading: function(){
							div.innerHTML = '<select id="muncod" style="width:200px;"><option disabled="disabled" selected="selected" value="todos">Carregando...</option></select>';
						}
					});
			}else{
				div.innerHTML = '<select id="muncod" style="width:200px;"><option disabled="disabled" selected="selected" value="todos">Selecione...</option></select>';
			}
		
}

function filtraRegionalizador(muncod){
	if(document.getElementById('exibe_reg') && muncod && muncod != "todos"){
		var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
		        method:     'post',
		        parameters: 'filtraRegionalizador=true&indid=<? echo $indid ?>&muncod=' + muncod,
		        onComplete: function (res)
		        {	
					document.getElementById('exibe_reg').innerHTML = res.responseText;
		        }
		  });
	}
}

function filtraEstadoDB(regcod,estuf){
	var div = document.getElementById('exibeEstado');
		var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
				        method:     'post',
				        parameters: 'filtraEstado=true&regcod=' + regcod + '&filtro=true',
				        onComplete: function (res)
				        {	
							div.innerHTML = res.responseText;
							extraiScript(res.responseText);
							if(estuf)
								document.getElementById('estuf').value = estuf;
				        }
				  });
}

function filtraGrupoMunicipios(gtmid,tpmid){
	var div = document.getElementById('exibe_tipo_municipio');
		var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
				        method:     'post',
				        parameters: 'filtraGrupoMunicipios=true&gtmid=' + gtmid + '&filtro=true',
				        onComplete: function (res)
				        {	
							div.innerHTML = res.responseText;
							extraiScript(res.responseText);
							if(tpmid)
								document.getElementById('tpmid').value = tpmid;
				        }
				  });
}

function filtraMunicipioPorTipoGrupo(tpmid,muncod){
	var estuf = document.getElementById('estuf').value;
	var div = document.getElementById('exibe_municipio');
		var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
				        method:     'post',
				        parameters: 'filtraMunicipioPorTipoGrupo=true&tpmid=' + tpmid + '&estuf=' + estuf,
				        onComplete: function (res)
				        {	
							div.innerHTML = res.responseText;
							extraiScript(res.responseText);
							if(muncod)
								document.getElementById('muncod').value = muncod;
							
				        }
				  });
}

function abrirMun(obj, estuf, filtro) {
	var acao = obj.title;
	var linha  = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	if(document.getElementById('muncod')){
		var muncod = document.getElementById('muncod').value;
		if(muncod == "todos"){
			muncod = false;
		}
	}else{
		var muncod = false;
	}

	if(acao=="mais") {
		var linhan = tabela.insertRow(linha.rowIndex);
		obj.title="menos";
		obj.src="../imagens/menos.gif";
		var col0 = linhan.insertCell(0);
		col0.colSpan=3;
		col0.innerHTML="Carregando...";
		var myAjax = new Ajax.Request(
			'painel.php?modulo=principal/painel_controle&acao=A',
			{
				method: 'post',
				parameters: "abrirMunicipios=true&estuf=" + estuf + "&muncod=" + muncod + "&filtro=" + filtro,
				asynchronous: false,
				onComplete: function(resp){
					col0.innerHTML = resp.responseText;
				},
				onLoading: function(){
				}
			});
	} else {
		tabela.deleteRow(linha.rowIndex);
		obj.title="mais";
		obj.src="../imagens/mais.gif";
	}
}

function visualizaMapaDashBoard(indid){

	if(document.getElementById('estuf')){
		var estuf = document.getElementById('estuf').value
	}else{
		var estuf = "todos";
	}	
	if(document.getElementById('muncod')){
		var muncod = document.getElementById('muncod').value
	}else{
		var muncod = "todos";
	}
	if(document.getElementById('gtmid')){
		var gtmid = document.getElementById('gtmid').value
	}else{
		var gtmid = "todos";
	}
	if(document.getElementById('tpmid')){
		var tpmid = document.getElementById('tpmid').value
	}else{
		var tpmid = "todos";
	}
	//Variavel global recebe true
	globalVisualizaMapa = true;

	exibeMapaDashBoard(indid,estuf,muncod,gtmid,tpmid,true,'div_mapa');
}

function escondeMapaDashBoard(indid){

	if(document.getElementById('estuf')){
		var estuf = document.getElementById('estuf').value
	}else{
		var estuf = "todos";
	}	
	if(document.getElementById('muncod')){
		var muncod = document.getElementById('muncod').value
	}else{
		var muncod = "todos";
	}
	if(document.getElementById('gtmid')){
		var gtmid = document.getElementById('gtmid').value
	}else{
		var gtmid = "todos";
	}
	if(document.getElementById('tpmid')){
		var tpmid = document.getElementById('tpmid').value
	}else{
		var tpmid = "todos";
	}
	//Variavel global recebe true
	globalVisualizaMapa = false;
	
	exibeMapaDashBoard(indid,estuf,muncod,gtmid,tpmid,false,'div_mapa');
}


function exibeAddNovaAba(){
			
	}
	
function addAbaCompartilhada(){
		var div = document.getElementById('conteudo_flutuante');
			Carregando('conteudo_flutuante');
			escondeID('button_addCaixa');
			var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: 'addAbaCompartilhada=true',
					        onComplete: function (res)
					        {	
								div.innerHTML = res.responseText;
								extraiScript(res.responseText);
								document.getElementById('botoes').style.display = 'none';
								//Função que exibe a lista incial de caixas
								jQuery(function () {
									jQuery('#button_cancelar').click( function() {
										window.location.reload();
									});
									
									jQuery('#button_voltar').click( function() {
										window.location.reload();
									});
									
									jQuery('#salvar_cima').click( function() {
										window.location.reload();
									});
									
								});
					        }
					  });
	}
	
//Função para pesquisar os tipos de Caixas no evento onkeypress
function pesquisaAbas(){
	var div = document.getElementById('TipoAbas');
	var pesquisa = document.getElementById('pesquisa').value;
	var TodosTipos = document.getElementById('buttonListaTodosTipos');
	mudaEvento('pesquisa','onfocus','');
	if(pesquisa.length > 2){
		TodosTipos.style.display = '';
		var myAjax = new Ajax.Request(
				'painel.php?modulo=principal/painel_controle&acao=A',
				{
					method: 'post',
					parameters: "pesquisaAbas=true&pesquisa=" + pesquisa,
					asynchronous: false,
					onComplete: function(resp){
						div.innerHTML = resp.responseText;
					},
					onLoading: function(){
						Carregando('TipoAbas');
					}
				});
	}
}
//Função para listar todos os tipos de Caixas
function listaAbas(){
	var div = document.getElementById('TipoAbas');
	var TodosTipos = document.getElementById('buttonListaTodosTipos');
	var myAjax = new Ajax.Request(
				'painel.php?modulo=principal/painel_controle&acao=A',
				{
					method: 'post',
					parameters: 'listaAbas=true',
					asynchronous: false,
					onComplete: function(resp){
						document.getElementById('pesquisa').value = '';
						mudaEvento('pesquisa','onfocus','this.value=""');
						TodosTipos.style.display = 'none';
						div.innerHTML = resp.responseText;
					},
					onLoading: function(){
						Carregando('TipoAbas');
					}
				});
}

function visualizaAba(abaid){
	//inicialytebox('painel.php?modulo=principal/painel_controle&acao=A&abaid=' + abaid + '&visualizar=true');
}

function addAbaComp(abaid){
	var div = document.getElementById('TipoAbas');
	var myAjax = new Ajax.Request(
	'painel.php?modulo=principal/painel_controle&acao=A',
	{
		method: 'post',
		parameters: 'copiarAba=true&abaid=' + abaid,
		asynchronous: false,
		onComplete: function(resp){
			var NovaAba = resp.responseText;
			if(NovaAba == "false"){
				listaAbas();
				return false;
			}else{
				<?if(!$cabecalho_painel){ ?>
					window.location.href = "painel.php?modulo=principal/painel_controle&acao=A&abaid=" + NovaAba;
				<?}else{ ?>
					window.location.href = "painel2.php?modulo=principal/painel_controle&acao=A&abaid=" + NovaAba;
				<?}?>
			}
		},
		onLoading: function(){
			Carregando('Aguarde... Copiando Conteudo.');
		}
	});
}

<?php 
	$sql = "select indvispadrao from painel.indicador where indid = $indid";
	$indvispadrao = $db->pegaUm($sql);
	if($indvispadrao){
		$dados = explode(";",$indvispadrao);
		$tipoGrafico = $dados[0] ? $dados[0] : '';
		$periodo_inicio = $dados[1] ? $dados[1] : '';
		$periodo_fim = $dados[2] ? $dados[2] : '';
		$unidade_inteiro = $dados[3] && $dados[3] != "null" && $dados[3] != "todos" ? $dados[3] : '';
		$unidade_moeda = $dados[4] && $dados[4] != "null" && $dados[4] != "todos" ? $dados[4] : '';
		$indice_moeda = $dados[5] && $dados[5] != "null" && $dados[5] != "todos" ? $dados[5] : '';
		$regcod = $dados[6] && $dados[6] != "null" && $dados[6] != "todos" ? $dados[6] : '';
		$estuf = $dados[7] && $dados[7] != "null" && $dados[7] != "todos" ? $dados[7] : '';
		$muncod = $dados[8] && $dados[8] != "null" && $dados[8] != "todos" ? $dados[8] : '';
		$gtmid = $dados[9] && $dados[9] != "null" && $dados[9] != "todos" ? $dados[9] : '';
		$tpmid = $dados[10] && $dados[10] != "null" && $dados[10] != "todos" ? $dados[10] : '';
		$tidid1 = $dados[11] && $dados[11] != "null" && $dados[11] != "todos" ? $dados[11] : '';
		$tidid2 = $dados[12] && $dados[12] != "null" && $dados[12] != "todos" ? $dados[12] : '';
		$mapa = $dados[13] && $dados[13] != "null" && $dados[13] != "todos" ? $dados[13] : '';
		$periodicidade = $dados[15] && $dados[15] != "null" && $dados[15] != "todos" ? $dados[15] : '';
	}
		
		?>

function salvaView(indid){

	if(document.getElementById('tipo_grafico')){
		var tpGrafico = document.getElementById('tipo_grafico').value;
	}else{
		var tpGrafico = '<?php echo $tipoGrafico ?>';
	}
	var dpeid_inicio = document.getElementById('periodo_inicio').value;
	var dpeid_fim = document.getElementById('periodo_fim').value;
	var abaPadrao = document.getElementById('abaPadrao').value;
	
	if(document.getElementById('unidade_inteiro')){
		var unidade_inteiro = document.getElementById('unidade_inteiro').value;
	}else{
		var unidade_inteiro = "null";
	}
	if(document.getElementById('unidade_moeda')){
		var unidade_moeda = document.getElementById('unidade_moeda').value;
	}else{
		var unidade_moeda = "null";
	}
	if(document.getElementById('indice_moeda')){
		var indice_moeda = document.getElementById('indice_moeda').value;
	}else{
		var indice_moeda = "null";
	}
	if(document.getElementById('regcod')){
		var regcod = document.getElementById('regcod').value;
	}else{
		var regcod = "todos";
	}
	if(document.getElementById('estuf')){
		var estuf = document.getElementById('estuf').value;
	}else{
		var estuf = "todos";
	}
	if(document.getElementById('muncod')){
		var muncod = document.getElementById('muncod').value;
	}else{
		var muncod = "todos";
	}
	if(document.getElementById('gtmid')){
		var gtmid = document.getElementById('gtmid').value;
	}else{
		var gtmid = "todos";
	}
	if(document.getElementById('tpmid')){
		var tpmid = document.getElementById('tpmid').value;
	}else{
		var tpmid = "todos";
	}
	if(document.getElementById('tidid1')){
		var tidid1 = document.getElementById('tidid1').value;
	}else{
		var tidid1 = "todos";
	}
	if(document.getElementById('tidid2')){
		var tidid2 = document.getElementById('tidid2').value;
	}else{
		var tidid2 = "todos";
	}
	if(document.getElementById('sel_periodicidade')){
		var periodicidade = document.getElementById('sel_periodicidade').value;
	}else{
		var periodicidade = "null";
	}
	if(globalVisualizaMapa == true){
		var mapa = 'exibe';
	}else{
		var mapa = 'naoExibe';
	}
	
	var indvispadrao = tpGrafico + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + regcod + ";" + estuf + ";" + muncod  + ";" + gtmid + ";" + tpmid  + ";" + tidid1  + ";" + tidid2 + ";" + mapa + ";" + abaPadrao + ";" + periodicidade;
	var myAjax = new Ajax.Request(
		'painel.php?modulo=principal/painel_controle&acao=A',
		{
			method: 'post',
			parameters: 'salvaView=true&indid=' + indid + '&indvispadrao=' + indvispadrao,
			asynchronous: false,
			onComplete: function(resp){
				var resposta = resp.responseText;
				if(resposta.search("ok") >= 0){
					alert('Visualização padrão definida com sucesso!');
				}else{
					alert('Não foi possível definir a visualização!');
				}
				
			},
			onLoading: function(){
			}
	});
}

<?php if($indvispadrao){ ?>
	//tpGrafico + ";" + dpeid_inicio + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + regcod + ";" + estuf + ";" + muncod  + ";" + gtmid + ";" + tpmid  + ";" + tidid1  + ";" + tidid2 + ";" + mapa;
	//linha;530;null;null;null;todos;todos;todos;todos;todos;todos;todos;naoExibe;8
	jQuery(document).ready(function() {
		jQuery("#tipo_grafico").val("<?php echo $tipoGrafico ?>");
		jQuery("#periodo_inicio").val("<?php echo $periodo_inicio ?>");
		/*jQuery("#periodo_fim").val("<?php echo $periodo_fim ?>");*/
		jQuery("#unidade_inteiro").val("<?php echo $unidade_inteiro ?>");
		jQuery("#unidade_moeda").val("<?php echo $unidade_moeda ?>");
		jQuery("#indice_moeda").val("<?php echo $indice_moeda ?>");
		jQuery("#regcod").val("<?php echo $regcod ?>");
		jQuery("#sel_periodicidade").val("<?php echo $periodicidade ?>");
		<?php if($regcod && $regcod != ''){ ?>
			filtraEstadoDB('<?php echo $regcod ?>','<?php echo $estuf ?>');
		<?php } ?>
		<?php if($estuf && $estuf != ''){ ?>
			filtraMunicipio('<?php echo $estuf ?>','<?php echo $muncod ?>');
		<?php } ?>
		jQuery("#gtmid").val("<?php echo $gtmid ?>");
		<?php if($gtmid && $gtmid != ''){ ?>
			filtraGrupoMunicipios('<?php echo $gtmid ?>','<?php echo $tpmid ?>');
		<?php } ?>
		<?php if($tpmid && $tpmid != ''){ ?>
			filtraMunicipioPorTipoGrupo('<?php echo $tpmid ?>','<?php echo $muncod ?>');
		<?php } ?>
		jQuery("#tidid1").val("<?php echo $tidid1 ?>");
		jQuery("#tidid2").val("<?php echo $tidid2 ?>");
		<?php if($mapa == "exibe"){ ?>
			globalVisualizaMapa = true;
		<?php } ?>
	});
	
<?php } ?>

function abrirEscolas(obj,muncod,filtro){

	var linha  = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	if(obj.id == "img_mais_" + muncod) {
		var linhan = tabela.insertRow(linha.rowIndex);
		obj.src = "../imagens/menos.gif";
		obj.id = "img_menos_" + muncod;
		var col0 = linhan.insertCell(0);
		col0.colSpan=3;
		col0.innerHTML = "Carregando...";
		var myAjax = new Ajax.Request(
			'painel.php?modulo=principal/painel_controle&acao=A',
			{
				method: 'post',
				parameters: "abrirEscola=true&muncod=" + muncod + "&filtro=" + filtro,
				asynchronous: false,
				onComplete: function(resp){
					col0.innerHTML = resp.responseText;
				},
				onLoading: function(){
					col0.innerHTML = "Carregando...";
				}
			});
	} else {
		tabela.deleteRow(linha.rowIndex);
		obj.src="../imagens/mais.gif";
		obj.id = "img_mais_" + muncod;
	}
}

function abrirIES(obj,muncod,filtro){

	var linha  = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	if(obj.id == "img_mais_" + muncod) {
		var linhan = tabela.insertRow(linha.rowIndex);
		obj.src = "../imagens/menos.gif";
		obj.id = "img_menos_" + muncod;
		var col0 = linhan.insertCell(0);
		col0.colSpan=3;
		col0.innerHTML = "Carregando...";
		var myAjax = new Ajax.Request(
			'painel.php?modulo=principal/painel_controle&acao=A',
			{
				method: 'post',
				parameters: "abrirIES=true&muncod=" + muncod + "&filtro=" + filtro,
				asynchronous: false,
				onComplete: function(resp){
					col0.innerHTML = resp.responseText;
				},
				onLoading: function(){
					col0.innerHTML = "Carregando...";
				}
			});
	} else {
		tabela.deleteRow(linha.rowIndex);
		obj.src="../imagens/mais.gif";
		obj.id = "img_mais_" + muncod;
	}
}

function abrirPOS(obj,muncod,filtro){

	var linha  = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	if(obj.id == "img_mais_" + muncod) {
		var linhan = tabela.insertRow(linha.rowIndex);
		obj.src = "../imagens/menos.gif";
		obj.id = "img_menos_" + muncod;
		var col0 = linhan.insertCell(0);
		col0.colSpan=3;
		col0.innerHTML = "Carregando...";
		var myAjax = new Ajax.Request(
			'painel.php?modulo=principal/painel_controle&acao=A',
			{
				method: 'post',
				parameters: "abrirPOS=true&muncod=" + muncod + "&filtro=" + filtro,
				asynchronous: false,
				onComplete: function(resp){
					col0.innerHTML = resp.responseText;
				},
				onLoading: function(){
					col0.innerHTML = "Carregando...";
				}
			});
	} else {
		tabela.deleteRow(linha.rowIndex);
		obj.src="../imagens/mais.gif";
		obj.id = "img_mais_" + muncod;
	}
}

function abrirCAM(obj,muncod,entid,filtro) {
	var linha  = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	if(obj.id == "img_maiscampus_" + entid) {
		var linhan = tabela.insertRow(linha.rowIndex);
		obj.src = "../imagens/menos.gif";
		obj.id = "img_menoscampus_" + entid;
		var col0 = linhan.insertCell(0);
		col0.colSpan=3;
		col0.innerHTML = "Carregando...";
		var myAjax = new Ajax.Request(
			'painel.php?modulo=principal/painel_controle&acao=A',
			{
				method: 'post',
				parameters: "abrirCAM=true&muncod=" + muncod + "&entid="+entid+"&filtro=" + filtro,
				asynchronous: false,
				onComplete: function(resp){
					col0.innerHTML = resp.responseText;
				},
				onLoading: function(){
					col0.innerHTML = "Carregando...";
				}
			});
	} else {
		tabela.deleteRow(linha.rowIndex);
		obj.src="../imagens/mais.gif";
		obj.id = "img_maiscampus_" + entid;
	}
}

function abrirUNIVCAM(obj,muncod,filtro){

	var linha  = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	if(obj.id == "img_mais_" + muncod) {
		var linhan = tabela.insertRow(linha.rowIndex);
		obj.src = "../imagens/menos.gif";
		obj.id = "img_menos_" + muncod;
		var col0 = linhan.insertCell(0);
		col0.colSpan=3;
		col0.innerHTML = "Carregando...";
		var myAjax = new Ajax.Request(
			'painel.php?modulo=principal/painel_controle&acao=A',
			{
				method: 'post',
				parameters: "abrirUNIVCAM=true&muncod=" + muncod + "&filtro=" + filtro,
				asynchronous: false,
				onComplete: function(resp){
					col0.innerHTML = resp.responseText;
				},
				onLoading: function(){
					col0.innerHTML = "Carregando...";
				}
			});
	} else {
		tabela.deleteRow(linha.rowIndex);
		obj.src="../imagens/mais.gif";
		obj.id = "img_mais_" + muncod;
	}
}

function relatorioXLS(){		

	var formulario = document.formulario;
	// verifica se tem algum agrupador selecionado
	agrupador = document.getElementById( 'agrupador' );
	if ( !agrupador.options.length ) {
		alert( 'Escolha ao menos um item para agrupar o resultado.' );
		return;
	}
	selectAllOptions( agrupador );
	//document.getElementById('divdashboard').innerHTML="<center><br /><img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\"><br />Carregando...<br /></center>";
	var params='';
	
	if (document.getElementById('periodo_inicio')){
		if(document.getElementById('periodo_inicio').value != '') {
			params += '&periodo_inicio='+document.getElementById('periodo_inicio').value; 
		}
	}
	if (document.getElementById('periodo_fim')){	
		if(document.getElementById('periodo_fim').value != '') {
			params += '&periodo_fim='+document.getElementById('periodo_fim').value; 
		}
	}
	if (document.getElementById('regcod')){
		if(document.getElementById('regcod').value != '') {
			params += '&regcod='+document.getElementById('regcod').value; 
		}
	}
	if (document.getElementById('estuf')){
		if(document.getElementById('estuf').value != '') {
			params += '&estuf='+document.getElementById('estuf').value; 
		}
	}
	if (document.getElementById('gtmid')){	
		if(document.getElementById('gtmid').value != '') {
			params += '&gtmid='+document.getElementById('gtmid').value; 
		}
	}
	if (document.getElementById('tpmid')){
		if(document.getElementById('tpmid').value != '') {
			params += '&tpmid='+document.getElementById('tpmid').value; 
		}
	}
	if (document.getElementById('muncod')){
		if(document.getElementById('muncod').value != '') {
			params += '&muncod='+document.getElementById('muncod').value; 
		}
	}
	if (document.getElementById('tidid1')){
		if(document.getElementById('tidid1').value != '') {
			params += '&tidid1='+document.getElementById('tidid1').value; 
		}
	}
	if (document.getElementById('tidid2')){
		if(document.getElementById('tidid2').value != '') {
			params += '&tidid2='+document.getElementById('tidid2').value; 
		}
	}
	if (document.getElementById('sel_periodicidade')){
		if(document.getElementById('sel_periodicidade').value != '') {
			params += '&perid='+document.getElementById('sel_periodicidade').value;
		}
	}
	if (document.getElementById('regvalue')){
		if(document.getElementById('regvalue').value != '') {
			params += '&regvalue='+document.getElementById('regvalue').value; 
		}
	}
	
	
	params += '&xls=1';
	formulario.btnsubmit.value = 2;
	formulario.action 	 = "painel.php?modulo=principal/relatorioDashBoard&acao=A" + params;
	formulario.submit();
}

function exibirRelatorioDashBoard() {
	var formulario = document.formulario;
	// verifica se tem algum agrupador selecionado
	agrupador = document.getElementById( 'agrupador' );
	if ( !agrupador.options.length ) {
		alert( 'Escolha ao menos um item para agrupar o resultado.' );
		return;
	}
	selectAllOptions( agrupador );
	document.getElementById('divdashboard').innerHTML="<center><br /><img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\"><br />Carregando...<br /></center>";
	var params='';
	
	if (document.getElementById('periodo_inicio')){
		if(document.getElementById('periodo_inicio').value != '') {
			params += '&periodo_inicio='+document.getElementById('periodo_inicio').value; 
		}
	}
	if (document.getElementById('periodo_fim')){	
		if(document.getElementById('periodo_fim').value != '') {
			params += '&periodo_fim='+document.getElementById('periodo_fim').value; 
		}
	}
	if (document.getElementById('regcod')){
		if(document.getElementById('regcod').value != '') {
			params += '&regcod='+document.getElementById('regcod').value; 
		}
	}
	if (document.getElementById('estuf')){
		if(document.getElementById('estuf').value != '') {
			params += '&estuf='+document.getElementById('estuf').value; 
		}
	}
	if (document.getElementById('gtmid')){	
		if(document.getElementById('gtmid').value != '') {
			params += '&gtmid='+document.getElementById('gtmid').value; 
		}
	}
	if (document.getElementById('tpmid')){
		if(document.getElementById('tpmid').value != '') {
			params += '&tpmid='+document.getElementById('tpmid').value; 
		}
	}
	if (document.getElementById('muncod')){
		if(document.getElementById('muncod').value != '') {
			params += '&muncod='+document.getElementById('muncod').value; 
		}
	}
	if (document.getElementById('tidid1')){
		if(document.getElementById('tidid1').value != '') {
			params += '&tidid1='+document.getElementById('tidid1').value; 
		}
	}
	if (document.getElementById('tidid2')){
		if(document.getElementById('tidid2').value != '') {
			params += '&tidid2='+document.getElementById('tidid2').value; 
		}
	}
	if (document.getElementById('sel_periodicidade')){
		if(document.getElementById('sel_periodicidade').value != '') {
			params += '&perid='+document.getElementById('sel_periodicidade').value; 
		}
	}
	if (document.getElementById('tidid1')){
		if(document.getElementById('tidid1').value != '') {
			params += '&tidid1='+document.getElementById('tidid1').value; 
		}
	}
	if (document.getElementById('tidid2')){
		if(document.getElementById('tidid2').value != '') {
			params += '&tidid2='+document.getElementById('tidid2').value; 
		}
	}
	if (document.getElementById('regvalue')){
		if(document.getElementById('regvalue').value != '') {
			params += '&regvalue='+document.getElementById('regvalue').value; 
		}
	}
	
	formulario.btnsubmit.value = 1;
	var myAjax = new Ajax.Request(
		'painel.php?modulo=principal/relatorioDashBoard&acao=A',
		{
				method: 'post',
				parameters: $('formdashboard').serialize()+params,
				asynchronous: false,
				onComplete: function(resp){
					document.getElementById('divdashboard').innerHTML = resp.responseText;
					extraiScript(resp.responseText);
				},
				onLoading: function(){}
		});

}

function abaPadrao(aba){
	if(document.getElementById('abaPadrao')){
		document.getElementById('abaPadrao').value = aba;
	}
}

function exibeFiltrosPainel(obj){

	if(obj.innerHTML.search("menos.gif") >= 0){
		obj.innerHTML = '<img title="mais" src="../imagens/mais.gif" align="absmiddle"> Filtros';
		document.getElementById('filtros_painel').style.display = "none";
	}else{
		obj.innerHTML = '<img title="menos" src="../imagens/menos.gif" align="absmiddle"> Filtros';
		document.getElementById('filtros_painel').style.display = "";
	}
	
}

function pesquisarIndicador(){
	Carregando('conteudo_flutuante');
	var myAjax = new Ajax.Request(
			'painel.php?modulo=principal/painel_controle&acao=A',
			{
				method: 'post',
				parameters: "pesquisaIndicadores=true",
				asynchronous: false,
				onComplete: function(resp){
					$('conteudo_flutuante').update(resp.responseText);
					
					jQuery(function () {
									jQuery('#button_cancelar').click( function() {
										window.location.reload();
									});
									
									jQuery('#button_voltar').click( function() {
										window.location.reload();
									});
									
								});
					
				},
				onLoading: function(){
				}
			});
}

function checkFinanceiro(obj){
	
	if(document.getElementById('finac_qtde').checked == false && document.getElementById('finac_valor').checked == false && document.getElementById('finac_ambos').checked == false){
		alert('É necessário selecionar pelo menos uma linha de exibição!');
		obj.checked = true;
		return false;
	}else{
		mudaTipoGrafico2(<? echo $_SESSION['indid'] ?>);
	}
	
}


//Executa a função quando a tela estiver carregada
jQuery(document).ready(function() {
		if(document.getElementById('tipo_grafico'))
			escondePeriodo(document.getElementById('tipo_grafico').value);
	});
</script>