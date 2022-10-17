<?
//Funções
include (APPRAIZ.'www/painel/_funcoes_painel_controle.php');

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

?>
<script type="text/javascript" src="../includes/DIYMap/js/JavaScriptFlashGateway.js"></script>
<script type="text/javascript" src="../includes/DIYMap/js/Exception.js"></script>
<script type="text/javascript" src="../includes/DIYMap/js/FlashTag.js"></script>
<script type="text/javascript" src="../includes/DIYMap/js/FlashSerializer.js"></script>
<script type="text/javascript" src="../includes/DIYMap/js/FlashProxy.js"></script>

<script language="javascript" type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
<script type="text/javascript" src="/includes/JQuery/interface.js"></script>

<script type="text/javascript">
function amun(muncod) {
	window.open('detalhamentoIndicador.php?detalhes=municipio&indid=<? echo $_SESSION['indid'] ?>&muncod='+muncod,'Indicador','scrollbars=yes,height=700,width=800,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aest(estuf) {
	window.open('detalhamentoIndicador.php?detalhes=estado&indid=<? echo $_SESSION['indid'] ?>&estuf='+estuf,'Indicador','scrollbars=yes,height=700,width=800,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
</script>

<?php 
//Verifica se o indicador possui série histórica, se nã oexistir o usário é redirecionado

$sql = "select indid from  painel.v_detalheindicadorsh where indid = {$_SESSION['indid']} limit 1";
if(!$db->pegaUm($sql)){ ?>
	<script type="text/javascript">
		alert('Série Histórica inexistente!');
		history.back(-1);
	</script>
<?php } ?>

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

jQuery.noConflict();

jQuery(document).ready(
	function () {
	jQuery('#button_cancelar').click( function() {
				window.history.back(-1);
			});
			
			jQuery('#button_voltar').click( function() {
				window.history.back(-1);
			});
			
			//executa as funções
			mudaTipoGrafico2(<? echo $indid ?>);
			
	}
);

function escondePeriodo(tipografico){
			if(tipografico != "linha" && tipografico != "barra"){
				var arrTipoGrafico = tipografico.split('_');
				var tpGrafico = arrTipoGrafico[0];
				var fatia =  arrTipoGrafico[1];
				var idFatia = arrTipoGrafico[2];
				if(tpGrafico == "barra"){
					document.getElementById('ate_periodo').style.display = "";
					document.getElementById('periodo_fim').style.display = "";
				}
				if(tpGrafico == "pizza"){
					document.getElementById('ate_periodo').style.display = "none";
					document.getElementById('periodo_fim').style.display = "none";
				}
			}else{
				document.getElementById('ate_periodo').style.display = "";
				document.getElementById('periodo_fim').style.display = "";
			}
}
		
function mudaTipoGrafico2(indid){
			var tipo_grafico = document.getElementById('tipo_grafico').value;
			mudaTipoGrafico(tipo_grafico,indid)
}

function exibeMapaDashBoard(indid,estuf,muncod,visualizaMapa){
			var dpeid_inicio = document.getElementById('periodo_inicio').value;
			var dpeid_fim = document.getElementById('periodo_fim').value;
			var sehid = document.getElementById('ultima_sehid').value;
			var mapa = document.getElementById('div_mapa');
			
			if(visualizaMapa == true){
				globalVisualizaMapa = true;
			}
			
			var myAjax = new Ajax.Request(
				'painel.php?modulo=principal/painel_controle&acao=A',
				{
					method: 'post',
					parameters: "exibeMapaDashBoard=true&indid=" + indid + "&sehid=" + sehid + "&dpeid_inicio=" + dpeid_inicio + "&dpeid_fim=" + dpeid_fim + "&estuf=" + estuf + "&muncod=" + muncod + "&visualizaMapa=" + visualizaMapa,
					asynchronous: false,
					onComplete: function(resp){
						mapa.innerHTML = resp.responseText;
						extraiScript(resp.responseText);
					},
					onLoading: function(){
						mapa.innerHTML = "<center>Carregando...</center>";
					}
			});
			if(estuf && globalVisualizaMapa == true) {
				setTimeout("flashProxy.call('zoomTo', '"+estuf+"')", 3000);
			}
}

function mudaTipoGrafico(tipografico,indid){
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
			geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda + ";" + estuf + ";" + muncod;
			document.getElementById('ate_periodo').style.display = "";
			document.getElementById('periodo_fim').style.display = "";
			var altura = "90%";
			var largura = "100%";
		}
		if(tipografico == "linha"){
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
			geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + estuf + ";" + muncod;
			document.getElementById('ate_periodo').style.display = "";
			document.getElementById('periodo_fim').style.display = "";
			var altura = "90%";
			var largura = "100%";
		}
		if(tipografico != "linha" && tipografico != "barra"){
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
				geraGrafico = "geraGrafico.php?tipo=" + indid + ";barra_fatia;" + sehid + ";tdiid" + fatia + "_" + idFatia + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + estuf + ";" + muncod;
				document.getElementById('ate_periodo').style.display = "";
				document.getElementById('periodo_fim').style.display = "";
			}
			if(tpGrafico == "pizza"){
				dpeid_fim = "";
				geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tpGrafico + ";" + sehid + ";tdiid" + fatia + "_" + idFatia + ";" + dpeid_inicio + ";;" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + estuf + ";" + muncod;
				document.getElementById('ate_periodo').style.display = "none";
				document.getElementById('periodo_fim').style.display = "none";
			}

		}
		if(erro == "false"){
			//Verifica se o grafico pode ser exibido
			var myAjax = new Ajax.Request(
					'painel.php?modulo=principal/painel_controle&acao=A',
					{
						method: 'post',
						parameters: "verificaExibeGrafico=true&indid=" + indid + "&estuf=" + estuf + "&muncod=" + muncod,
						asynchronous: false,
						onComplete: function(resp){
							var exibe = resp.responseText;
							if(exibe == "true"){
								swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_indicador", largura, altura, "9.0.0", "expressInstall.swf", {"data-file":geraGrafico,"loading":"Carregando gráfico..."} );
							}else{
								document.getElementById('div_graficos').innerHTML = '<fieldset style="height: 400px;"><legend>Gráficos</legend><div style="overflow: auto; width: 100%; height: 100%;"><div id="grafico_indicador">Gráfico indisponível.</div><div id="legenda_grafico"/></div></fieldset>';
							}						
						},
						onLoading: function(){
							document.getElementById('grafico_indicador').innerHTML = "<center>Carregando...</center>";
						}
				});
			exibeMapaDashBoard(indid,estuf,muncod,globalVisualizaMapa);
			exibeSerieHistoricaDashBoard(indid,geraGrafico);
			if(tipografico != "linha" && tipografico != "barra"){
				var myAjax = new Ajax.Request(
							'painel.php?modulo=principal/painel_controle&acao=A',
							{
								method: 'post',
								parameters: "exibeLegendaGrafico=true&indid=" + indid + "&sehid=" + sehid + "&tdiid=" + fatia + "&valorTdiid=" + idFatia + "&dpeid_inicio=" + dpeid_inicio + "&dpeid_fim=" + dpeid_fim + "&estuf=" + estuf + "&muncod=" + muncod,
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
		}else{
			alert('A data incial deve ser menor que a data final!');
		}
			
}

function exibeSerieHistoricaDashBoard(indid,parametros){
		
			var arrParametros = parametros.split('geraGrafico.php?tipo=');
			var parametro1 = arrParametros[1];
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
										}
			});
}

function filtraMunicipio(estuf){
		var div = document.getElementById('exibe_municipio');
			if(estuf != "null"){
				var myAjax = new Ajax.Request(
					'painel.php?modulo=principal/painel_controle&acao=A',
					{
						method: 'post',
						parameters: "filtraMunicipio=true&estuf=" + estuf,
						asynchronous: false,
						onComplete: function(resp){
							div.innerHTML = resp.responseText;
						},
						onLoading: function(){
							div.innerHTML = '<select id="muncod" style="width:200px;"><option disabled="disabled" selected="selected" value="null">Carregando...</option></select>';
						}
					});
			}else{
				div.innerHTML = '<select id="muncod" style="width:200px;"><option disabled="disabled" selected="selected" value="null">Selecione o Estado...</option></select>';
			}
		
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
		col0.colSpan=2;
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
		var estuf = "null";
	}	
	if(document.getElementById('muncod')){
		var muncod = document.getElementById('muncod').value
	}else{
		var muncod = "null";
	}
	
	exibeMapaDashBoard(indid,estuf,muncod,true);
	
}
</script>