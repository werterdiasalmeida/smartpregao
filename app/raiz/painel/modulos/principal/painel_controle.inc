<?
//Funções
include_once (APPRAIZ.'www/painel/_funcoes_painel_controle.php');

if(count($_POST['agrupador']) != 0 && $_POST['cxcid_arvore']){
	header('content-type: text/html; charset=ISO-8859-1');
	addArvore($_POST['cxcid_arvore'],$_POST['agrupador']);
}

if($_POST['filtraEstado']){
	header('content-type: text/html; charset=ISO-8859-1');
	filtraEstado($_POST['regcod']);
	exit;
}

if($_POST['listaMunicipiosSelecionados']){
	header('content-type: text/html; charset=ISO-8859-1');
	listaMunicipiosSelecionados($_POST['cxcid'],$_POST['usucpf']);
	exit;
}

if($_POST['ajaxAddMeusMuncod']){
	header('content-type: text/html; charset=ISO-8859-1');
	AddMeusMuncod($_POST['acao'],$_POST['cxcid'],$_POST['muncod']);
	exit;
}

if($_POST['marcarTodosMunicipios']){
	header('content-type: text/html; charset=ISO-8859-1');
	AddMeusMuncod("adiciona",$_POST['cxcid'],$_POST['arrMuncod']);
	exit;
}

if($_POST['desmarcarTodosMunicipios']){
	header('content-type: text/html; charset=ISO-8859-1');
	AddMeusMuncod("remove",$_POST['cxcid'],$_POST['arrMuncod']);
	exit;
}

if($_POST['filtraGrupoMunicipios']){
	header('content-type: text/html; charset=ISO-8859-1');
	filtraGrupoMunicipios($_POST['gtmid']);
	exit;
}

if($_REQUEST['verificaExibeGrafico'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	verificaExibeGrafico($_REQUEST['indid'], $_REQUEST['estuf'],$_REQUEST['muncod']);
	exit;
}

if($_REQUEST['abrirMunicipios'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	abrirMunicipios($_REQUEST['estuf'], $_REQUEST['muncod'] , $_REQUEST['filtro']);
	exit;
}

if($_REQUEST['filtraMunicipio'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	filtraMunicipio($_REQUEST['estuf']);
	exit;
}

if($_REQUEST['exibeMapaDashBoard'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	exibeMapaDashBoard($_REQUEST['visualizaMapa'],$_REQUEST['indid'],$_REQUEST['sehid'],$_REQUEST['dpeid_inicio'],$_REQUEST['dpeid_fim'],$_REQUEST['estuf'],$_REQUEST['muncod']);
	exit;
}
if($_REQUEST['exibeSerieHistoricaDashBoard'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	db_exibeSerieHistorica($_REQUEST['indid'],$_REQUEST['parametros']);
	exit;
}

if($_REQUEST['exibeLegendaGrafico'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	exibeLegendaGrafico($_REQUEST['indid'],$_REQUEST['sehid'],$_REQUEST['tdiid'],$_REQUEST['valorTdiid'],$_REQUEST['dpeid_inicio'],$_REQUEST['dpeid_fim'],$_REQUEST['estuf'],$_REQUEST['muncod']);
	exit;
}

if($_REQUEST['testaDatasDpeid'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	testaDatasDpeid($_REQUEST['dpeid1'],$_REQUEST['dpeid2']);
	exit;
}

if($_REQUEST['excluirAba'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	excluirAba($_REQUEST['abaid']);
	exit;
}

if($_REQUEST['marcarTodos'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	marcarTodos($_REQUEST['cxcid'],$_REQUEST['usucpf'],$_REQUEST['arrIndid']);
	exit;
}

if($_REQUEST['desmarcarTodos'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	desmarcarTodos($_REQUEST['cxcid'],$_REQUEST['usucpf'],$_REQUEST['arrIndid']);
	exit;
}


if($_REQUEST['listaIndicadoresSelecionados'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	listaIndicadoresSelecionados($_REQUEST['cxcid'],utf8_decode($_REQUEST['usucpf']));
	exit;
}

if($_REQUEST['editarNomeAba'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	editarNomeAba($_REQUEST['abaid'],utf8_decode($_REQUEST['abadsc']));
	exit;
}

if($_REQUEST['novaAba'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	addNovaAba(utf8_decode($_REQUEST['abadsc']));
	exit;
}

if($_REQUEST['EditaSerieHistoricaGrafico'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	EditaSerieHistoricaGrafico($_REQUEST['tcgid'],$_REQUEST['sehid']);
	exit;
}

if($_REQUEST['exibeArvore'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	exibeArvore($_REQUEST['cxcid']);
	exit;
}

if($_REQUEST['addCaixa'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	addCaixa();
	exit;
}

if($_REQUEST['editarNomecaixa'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	editarNomecaixa(utf8_decode($_REQUEST['cxcid']),utf8_decode($_REQUEST['cxcdescricao']));
	exit;
}

if($_REQUEST['addArvoreIndicador'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	addArvoreIndicador($_REQUEST['cxcid']);
	exit;
}

if($_REQUEST['buscaIndicador'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	exibeBuscaIndicador(utf8_decode($_REQUEST['texto']));
	gravaBuscaIndicador($_REQUEST['cxcid'],utf8_decode($_REQUEST['texto']));
	exit;
}

if($_REQUEST['exibedashBoard'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	exibedashBoard($_REQUEST['indid']);
	exit;
}

if ($_REQUEST['excluirBox']) {
	header('content-type: text/html; charset=ISO-8859-1');
	excluirBox($_REQUEST['cxcid']);
	exit;
}

if ($_REQUEST['ajaxAddMeusIndicadores']) {
	header('content-type: text/html; charset=ISO-8859-1');
	AddMeusIndicadores($_REQUEST['acao'],$_REQUEST['indid'],$_REQUEST['cxcid']);
	exit;
}

if($_REQUEST['geraGrafico'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	geraGraficoIndicador($_REQUEST['cxcid'],$_REQUEST['indid'],$_REQUEST['tipoGraficoIndicador'],$_REQUEST['dpeid'],$_REQUEST['sehid'],$_REQUEST['tidid']);
	exit;
}

if($_REQUEST['selecionaGraficoIndicador'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	selecionaGraficoIndicador($_REQUEST['cxcid'],$_REQUEST['indid']);
	exit;
}

if($_REQUEST['pesquisaMunicipio'] == 'true' && $_REQUEST['ckeckbox'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	pesquisaMunicipio($_REQUEST['cxcid'],"true");
	exit;
}

if($_REQUEST['pesquisaIndicador'] == 'true' && $_REQUEST['ckeckbox'] != 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	pesquisaIndicador($_REQUEST['cxcid'],"false");
	exit;
}

if($_REQUEST['pesquisaIndicador'] == 'true' && $_REQUEST['ckeckbox'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	pesquisaIndicador($_REQUEST['cxcid'],"true");
	exit;
}


if($_REQUEST['AddGrafico'] == 'true' && $_REQUEST['exibePesquisa'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	exibePesquisa( $_REQUEST['cxcid'],"false"); //checkbox = false
	exit;
}

if($_REQUEST['addIndicadorCaixa'] == 'true' && $_REQUEST['exibePesquisa'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	exibePesquisa( $_REQUEST['cxcid'],"true"); //checkbox = true
	exit;
}

if($_REQUEST['addMunicipioCaixa'] == 'true' && $_REQUEST['exibePesquisa'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	exibePesquisaMunicipio( $_REQUEST['cxcid'],"true"); //checkbox = true
	exit;
}


if($_REQUEST['index'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	index($_REQUEST['abaid']);
	exit;
}

if($_REQUEST['pequisaTipocaixa']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['pequisaTipocaixa'] != 'false' ? exibeTipos(utf8_decode($_REQUEST['pequisaTipocaixa'])) : exibeTipos();
	exit;
}

if($_REQUEST['executaAddCaixa'] == 'true' && $_REQUEST['cxcdescricao'] && $_REQUEST['tcoid']){
	header('content-type: text/html; charset=ISO-8859-1');
	$cxcid = executaAddCaixa(utf8_decode($_REQUEST['cxcdescricao']),utf8_decode($_REQUEST['tcoid']),$_REQUEST['abaid']);
	echo "<script>exibeConteudoCaixa($cxcid,{$_REQUEST['tcoid']},{$_REQUEST['abaid']})</script>";
	exit;
}

if($_REQUEST['exibeConteudoCaixa'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	exibeConteudoCaixa($_REQUEST['cxcid'],$_REQUEST['tcoid'],$_REQUEST['abaid']);
	exit;
}

//Salva os Parametros da Tela
if($_REQUEST['salvaParametros']){
	header('content-type: text/html; charset=ISO-8859-1');
	salvaParametros($_REQUEST['sort1'],$_REQUEST['sort2'],$_REQUEST['sort3'],$_REQUEST['estados'],$_REQUEST['scroll'],$_SESSION['usucpf']);
	exit;
}

//Função para exibir / esconder a função de salvar as Cxs
$abaid = !$_GET['abaid'] ? 0 : $_GET['abaid'];
$style_buttons = exibeButtons($abaid);

if($_REQUEST['exibeButtons']){
	header('content-type: text/html; charset=ISO-8859-1');
	$style_buttons = exibeButtons($_REQUEST['abaid']);
	echo "<script>document.getElementById('botoes').style.display = '$style_buttons'</script>";
	exit;
}


// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
//Monta as abas da tela
if(!$cabecalho_painel) $db->cria_aba($abacod_tela,$url,$parametros);
//Exibe as Abas do Usuário
$abaid = !$_GET['abaid'] ? false : $_GET['abaid'];
!$cabecalho_painel ? exibeAbaUsuario($_SESSION['usucpf'],$abaid) : exibeAbaUsuario($_SESSION['usucpf'],$abaid,2);
$addcaixa = "<div style=\"width:100%;text-align:right\" > <input type=\"button\" onclick=\"buscarMapa();\" value=\"Buscar no Mapa\" />&nbsp;&nbsp;&nbsp;<input style=\"display:none\" id=\"button_voltar\" type=\"button\" value=\"Voltar\" /> <input id=\"button_addCaixa\" type=\"button\" value=\"Adicionar Caixa\" onclick=\"addCaixa();\"  /></div>";
if(!$cabecalho_painel) monta_titulo( $titulo_modulo, $addcaixa);

?>
<style type="text/css">	
	/*Drag adn Drop Google*/
	
		.groupWrapper
	{
		width: 32%;
		float: left;
		margin-right: 5px;
		border:1px solid #FFFFFF;
	}
		
	.groupItem
	{
		margin-bottom: 20px;
		border: #e0e0e0 solid 2px;
	}
	.groupItem .itemHeader
	{
		line-height: 28px;
		background-color: #e0e0e0;
		color: #000;
		cursor: move;
		font-weight: bold;
		font-size: 14px;
		height: 28px;
		position: relative;
		width: 100%;
	}
	
	.groupItem .itemHeader a
	{
		position: absolute;
		right: 10px;
		top: 0px;
		font-weight: normal;
		font-size: 11px;
		text-decoration: none;
	}
	.sortHelper
	{
		border: 3px dashed #666;
		width: auto !important;
	}
	.groupWrapper p
	{
		height: 1px;
		overflow: hidden;
		margin: 0;
		padding: 0;
		width: 100%;
	}
	.itemContent{
		overflow: auto;
		width: 100%;
		background-color: #FFFFFF;
	}
	.edit{
	cursor: pointer;
	float:right;
	font-size:12px;
	font-weight:normal;
	padding-right: 30px;
	padding-top: 5px;
	width:30px;
	}
	.edit img{width:12px ; height:10px; border: #000000 solid 1px}
	
	.editar{
		font-size:12px;
		width: 100%;
		border-top: #000000 solid 1px;
		background-color: #DFDFDF;
		cursor: default;
	}
	
	.excluir {
		padding-top: 5px;
	}
	
	.excluir img{
		width:12px ; height:10px; border: #000000 solid 1px;background-color: #FFFFFF;
	}
	
	.closeEl{
		padding-top: 5px;
		margin-right:20px;
	}
	.closeEl img{
		width:12px ; height:12px; border: none;
	}
	
	.painel_button{
		border: #000000 dashed 1px;
		font-weight: bold;
		cursor: pointer;
		padding: 3px;
		color: #000000; 
	}
	.titulo_coluna{
		float:left;
		padding-left: 10px
	}
</style>



<link rel="StyleSheet" href="/includes/dtree/dtree.css" type="text/css" />
<script type="text/javascript" src="/includes/dtree/dtree.js">window.open('/www/includes/dtree/dtree.js');</script>
<link rel="stylesheet" href="/includes/LyteBox/lytebox.css" type="text/css" media="screen" />
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script language="javascript" type="text/javascript" src="/includes/JsLibrary/tags/superTitle.js"></script>
<script language="javascript" type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
<script type="text/javascript" src="/includes/LyteBox/lytebox.js" ></script>
<script type="text/javascript" src="/includes/prototype.js"></script>

<script type="text/javascript">
function amun(muncod) {
	window.open('detalhamentoIndicador.php?detalhes=municipio&indid=<? echo $_SESSION['indid'] ?>&muncod='+muncod,'Indicador','scrollbars=yes,height=700,width=700,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aest(estuf) {
	window.open('detalhamentoIndicador.php?detalhes=estado&indid=<? echo $_SESSION['indid'] ?>&estuf='+estuf,'Indicador','scrollbars=yes,height=700,width=700,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aesc(muncod) {
	window.open('detalhamentoIndicador.php?detalhes=escola&indid=<? echo $_SESSION['indid'] ?>&muncod='+muncod,'Indicador','scrollbars=yes,height=700,width=700,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
</script>

<script type="text/javascript">

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
	
	//Função que indica que a função está sendo carregada
	function Carregando(div){
		document.getElementById(div).innerHTML = '<center><img src="../imagens/wait.gif" /> Carregado... </center>';
	}
	//Função que muda o evento
	function mudaEvento(id,evento,funcao){
		var obj = document.getElementById(id);
		obj.setAttribute(evento, funcao);
	}
	
	//Exibe o Objeto
	function exibeID(id){
		document.getElementById(id).style.display = '';
	}
	
	//Esconde o Objeto
	function escondeID(id){
		document.getElementById(id).style.display = 'none';
	}
	
	//Focus no objeto colocando borda vermelha
	function focusId(id){
		if(document.getElementById('colid_' + id)){
			var obj1 = document.getElementById('colid_' + id);
			var obj2 = document.getElementById('titulo_' + id);
			obj1.setAttribute('style', 'border:#AA0000 solid 2px');
			obj2.setAttribute('style', 'color:#AA0000');
		}
	}
	
	//Função para pesquisar os tipos de Caixas no evento onkeypress
	function pesquisaTipos(){
		var div = document.getElementById('TipoCaixa');
		var pesquisa = document.getElementById('pesquisa').value;
		var TodosTipos = document.getElementById('buttonListaTodosTipos');
		mudaEvento('pesquisa','onfocus','');
		if(pesquisa.length >= 2){
			Carregando('TipoCaixa');
			TodosTipos.style.display = '';
			var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
						        method:     'post',
						        parameters: '&pequisaTipocaixa=' + pesquisa,
						        onComplete: function (res)
						        {	
									div.innerHTML = res.responseText;
						        }
						  });
		}
	}
	//Função para listar todos os tipos de Caixas
	function listaTipos(){
		var div = document.getElementById('TipoCaixa');
		var TodosTipos = document.getElementById('buttonListaTodosTipos');
		Carregando('TipoCaixa');
		var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: '&pequisaTipocaixa=false',
					        onComplete: function (res)
					        {	
								document.getElementById('pesquisa').value = 'Digite o tipo';
								mudaEvento('pesquisa','onfocus','this.value=""');
								TodosTipos.style.display = 'none';
								div.innerHTML = res.responseText;
					        }
					  });
	}
	
	//Função para adicionar o Tipo da Caixa
	function addTipo(tcoid){
		var tcoid_antigo = document.getElementById('tcoid').value;
		if(document.getElementById('check_' + tcoid_antigo)){
			document.getElementById('check_' + tcoid_antigo).style.display = 'none';
		}
		document.getElementById('check_' + tcoid).style.display = '';
		document.getElementById('tcoid').value = tcoid;
		//setTimeout('escondeID("check_' + tcoid + '")',2000);
	}
	
	
	function executarEditar(id,display){
		var eixoid = id;
		if(document.getElementById( 'autoscroll_' + eixoid)){
			if(document.getElementById( 'autoscroll_' + eixoid).checked == true){
				document.getElementById( 'itemContent_' + eixoid ).style.height = '300px';
				document.getElementById( 'itemContent_' + eixoid ).style.overflow = 'auto';
				document.getElementById( 'scroll_' + eixoid ).value = 'true';
			}
			if(document.getElementById( 'autoscroll_' + eixoid).checked == false){
				document.getElementById( 'itemContent_' + eixoid ).style.height = 'auto';
				document.getElementById( 'itemContent_' + eixoid ).style.overflow = 'hidden';
				document.getElementById( 'scroll_' + eixoid ).value = 'false';
			}
		}
		if(display == true){
			document.getElementById( 'editar_' + eixoid ).style.display = 'none';
		}
		document.getElementById( 'edit_' + eixoid ).innerHTML = '<img src="../imagens/ico_config.gif" onclick="exibeEditar(\'' + eixoid + '\');" />';
	}
	
</script>

<table onclick="escondeID('abaid_<? echo $abaid ?>')" class="tabela" width="95%" bgcolor="FFFFFF" cellSpacing="1" border=0 cellPadding="3" align="center">
<tr>
	<td valign="top" >
		<!-- Início do Drag and Drop  das Caixas-->
		<div id="conteudo_flutuante" style="width: 100%;min-height:400px;"  >
		
		<?
		$abaid = !$_GET['abaid'] ? 0 : $_GET['abaid'];
		index($abaid);
		?>
				
		<!-- Fim do Drag and Drop  das Caixas-->
		</div>
	</td>
</tr>
<tr id="botoes" style="background-color: #e9e9e9;display:<?=$style_buttons;?>">
	<td align="center" ><input onclick="salvaColunas();" type="button" value="Salvar" /></td>
</tr>
</table>
<div id="erro">
</div>
<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
<script type="text/javascript" src="/includes/JQuery/interface.js"></script>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(
	function () {
		jQuery('a.closeEl').bind('click', toggleContent);
		jQuery('div.groupWrapper').Sortable(
			{
				accept: 'groupItem',
				helperclass: 'sortHelper',
				activeclass : 	'sortableactive',
				hoverclass : 	'sortablehover',
				handle: 'div.itemHeader',
				tolerance: 'pointer',
				onChange : function(ser)
				{
				},
				onStart : function()
				{
					jQuery.iAutoscroller.start(this, document.getElementsByTagName('body'));
				},
				onStop : function()
				{
					jQuery.iAutoscroller.stop();
				}
			}
		);
	}
);

//Função que chama a execução o SQL p/ inserir a Caixa
	function excutaAddCaixa(cxcdescricao,tcoid){
		var div = document.getElementById('conteudo_flutuante');
		var erro = document.getElementById('erro');
				
		Carregando('conteudo_flutuante');
		var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: '&executaAddCaixa=true&cxcdescricao=' + cxcdescricao + '&tcoid=' + tcoid + '&abaid=<? echo $abaid ?>',
					        onComplete: function (res)
					        {	
								//document.getElementById('erro').innerHTML = res.responseText;
								extraiScript(res.responseText);
					        }
					  });
	}

function exibeConteudoCaixa(cxcid,tcoid,abaid){
	var tipo = tcoid;
	var div = document.getElementById('conteudo_flutuante');
	var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: '&exibeConteudoCaixa=true&tcoid=' + tcoid + '&cxcid=' + cxcid + '&abaid=' + abaid,
					        onComplete: function (res)
					        {	
								div.innerHTML = res.responseText;
								extraiScript(res.responseText);
								
								if(tipo == 1 || tipo == 2 || tipo == 10){
									//mudaEvento('addCaixa','onclick','alert(\'Já existe uma caixa sendo adicionada!\')');
									escondeID('button_addCaixa');
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
									
								}else{
									//inicio
									window.location.reload();
								}
								
					        }
					  });
}


//Função que verifica e adiciona uma caixa nova
	function adicionaCaixa(cxcdescricao,tcoid){
		excutaAddCaixa(cxcdescricao,tcoid);
	}

//Função que exibe a lista incial de caixas
function index(executaJS){
	window.location.reload();
}


//Função que chama os objetos para incluir uma nova caixa
	function addCaixa()
	{
		var div = document.getElementById('conteudo_flutuante');
		Carregando('conteudo_flutuante');
		//mudaEvento('addCaixa','onclick','alert(\'Já existe uma caixa sendo adicionada!\')');
		escondeID('button_addCaixa');
		var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: '&addCaixa=true',
					        onComplete: function (res)
					        {	
								div.innerHTML = res.responseText;
								document.getElementById('botoes').style.display = 'none';
								//Função que exibe a lista incial de caixas
								jQuery(function () {
									jQuery('#button_cancelar').click( function() {
										window.location.reload();
									});
									
									jQuery('#button_voltar').click( function() {
										window.location.reload();
									});
									
								});
					        }
					  });
	}

var toggleContent = function(e)
{
	var targetContent = jQuery('div.itemContent', this.parentNode.parentNode);
	if (targetContent.css('display') == 'none') {
		
		if(document.getElementById( 'autoscroll_' + this.id).checked == true){
			targetContent.css({'display' : ''});
			jQuery(this).html('<img src="../imagens/menos.gif" border=0 />');
			mudaEstado(this.id,'max');
			executarEditar(this.id,true);
		}
		if(document.getElementById( 'autoscroll_' + this.id).checked == false){
			targetContent.slideDown(300);
			jQuery(this).html('<img src="../imagens/menos.gif" border=0 />');
			mudaEstado(this.id,'max');
		}		
	} else {
		targetContent.slideUp(300);
		executarEditar(this.id,true);
		mudaEstado(this.id,'min');
		jQuery(this).html('<img src="../imagens/mais.gif" border=0 />');
	}
	return false;
};

function mudaEstado(estid,str){
	var id = estid;
	var estado = str;
	document.getElementById( 'estado_' + id ).value = estado; 
}

function mundaScroll(estid,str){
	var id = estid;
	var scroll = str;
	document.getElementById( 'scroll_' + id ).value = scroll; 
}

function exibeEditar(id){
	var eixoid = id;
	document.getElementById( 'editar_' + eixoid ).style.display = '';
	document.getElementById( 'edit_' + eixoid ).innerHTML = '<img src="../imagens/ico_config.gif" onclick="escondeEditar(\'' + eixoid + '\');executarEditar(\'' + eixoid + '\',true)" />';
}

function escondeEditar(id){
	var eixoid = id;
	document.getElementById( 'editar_' + eixoid ).style.display = 'none';
	document.getElementById( 'edit_' + eixoid ).innerHTML = '<img src="../imagens/ico_config.gif" onclick="exibeEditar(\'' + eixoid + '\');executarEditar(\'' + eixoid + '\',true)" />';
}

function editaIndicador(id){
	var eixoid = id;
	//alert('Selecione os Indicadores: para eixoid = ' + eixoid);
	
}

function desmarcaRadio(id){
	var eixoid = id;
	document.getElementById( 'radio_' + eixoid ).selected = false;
}

function exibeButtons(){
	if(document.getElementById('button_voltar')){
		document.getElementById('button_voltar').style.display = 'none';
	}
	if(document.getElementById('button_addCaixa')){
		document.getElementById('button_addCaixa').style.display = '';
	}

	var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
			        method:     'post',
			        parameters: '&exibeButtons=true&abaid=<? echo $abaid ?>',
			        onComplete: function (res)
			        {	
						//document.getElementById('erro').innerHTML = res.responseText;
						extraiScript(res.responseText);
			        }
			  });
}

function addNovaAba(){
	var abadsc = prompt("Descrição:");
	if(abadsc){
		var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
				        method:     'post',
				        parameters: '&novaAba=true&abadsc=' + abadsc,
				        onComplete: function (res)
				        {	
							<?if(!$cabecalho_painel){ ?>
								window.location.href="painel.php?modulo=principal/painel_controle&acao=A&abaid=" + res.responseText;
							<?}else{ ?>
								window.location.href="painel2.php?modulo=principal/painel_controle&acao=A&abaid=" + res.responseText;
							<?} ?>
				        }
		});
	}
}


function inserirCaixa(tcoid,cxcdescricao) {
	//var cxcdescricao = prompt("Descrição:");
	if(tcoid && cxcdescricao){
		adicionaCaixa(cxcdescricao,tcoid);
	}else{
		return false;
	}
}


function salvaColunas(s){
			
			//Estado das CXs
		 	var estados = document.getElementsByTagName("input");
			var num = estados.length;
		 	var ckeckados = 0;
		 	var arrID = new Array();
		 	
		 	for(var i=0; i < num; i++){
				if(document.getElementsByTagName('input')[i].type == 'hidden'){
		        	if(document.getElementsByTagName('input')[i].value == 'max' || document.getElementsByTagName('input')[i].value == 'min'){
		        		arrID[ckeckados] = document.getElementsByTagName('input')[i].value;
		        		ckeckados ++;
		       		}
		       	}
		     }
		     
		     //Estado das CXs em Scroll
		 	var scroll = document.getElementsByTagName("input");
			var num2 = scroll.length;
		 	var ckeckados2 = 0;
		 	var arrID2 = new Array();
		 	
		 	for(var i=0; i < num2; i++){
				if(document.getElementsByTagName('input')[i].type == 'hidden'){
		        	if(document.getElementsByTagName('input')[i].value == 'true' || document.getElementsByTagName('input')[i].value == 'false'){
		        		arrID2[ckeckados2] = document.getElementsByTagName('input')[i].value;
		        		ckeckados2 ++;
		       		}
		       	}
		     }
		     
			
			var div = document.getElementById('erro'); 
			
			serial = jQuery.SortSerialize(s);
			
			if(ckeckados != 0){
				// Faz uma requisição ajax, passando o parametro 'ordid', via POST
				var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
						        method:     'post',
						        parameters: '&salvaParametros=1&' + serial.hash + '&estados=' + arrID + '&scroll=' + arrID2,
						        onComplete: function (res)
						        {	
									alert('Operação Realizada com Sucesso!');
									div.innerHTML = res.responseText;
						        }
						  });
			}else{
				alert('Não existem Caixas de Conteudo.');
			}
		}
		
		function addGrafico(cxcid){
			var div = document.getElementById('conteudo_flutuante');
			Carregando('conteudo_flutuante');
			//mudaEvento('addCaixa','onclick','alert(\'Existe um conteudo sendo adicionado!\')');
			escondeID('button_addCaixa')
			var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: 'ckeckbox=false&&AddGrafico=true&exibePesquisa=true&cxcid=' + cxcid,
					        onComplete: function (res)
					        {	
								div.innerHTML = res.responseText;
								document.getElementById('botoes').style.display = 'none';
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
		
		function carregaMapa(){
			var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: '&AjaxCarregaMapa=1&',
					        onComplete: function (res)
					        {	
								montaMapa();
								disponibilizaMapa();
					        }
					  });
		}
		
		function pesquisar(cxcid,ckeckbox){
			
			var destino = document.getElementById("resultado_pesquisa");
			Carregando('resultado_pesquisa');
			
			var indid = document.getElementById('indid').value;	
			var exoid = document.getElementById('exoid').value;
			var secid = document.getElementById('secid').value;
			var acaid = document.getElementById('acaid').value;
			var indnome = document.getElementById('indnome').value;
			var tpiid = document.getElementById('tpiid').value;
			var regid = document.getElementById('regid').value;
			
			var myAjax = new Ajax.Request(
				"painel.php?modulo=principal/painel_controle&acao=A",
				{
					method: 'post',
					parameters: "ckeckbox=" + ckeckbox + "&pesquisaIndicador=true&cxcid=" + cxcid + "&indid=" + indid + "&exoid=" + exoid + "&secid=" + secid + "&acaid=" + acaid + "&indnome=" + indnome + "&tpiid=" + tpiid + "&regid=" + regid,
					asynchronous: false,
					onComplete: function(resp) {
						if(destino) {
							destino.innerHTML = resp.responseText;
						} 
					},
					onLoading: function(){
						Carregando('resultado_pesquisa');
					}
				});
		}
		
		function pesquisarMunicipio(cxcid,ckeckbox){
			
			var destino = document.getElementById("resultado_pesquisa");
			Carregando('resultado_pesquisa');
			
			var regcod = document.getElementById('regcod').value;	
			var estuf = document.getElementById('estuf').value;
			var gtmid = document.getElementById('gtmid').value;
			var tpmid = document.getElementById('tpmid').value;
			var municipio = document.getElementById('municipio').value;
						
			var myAjax = new Ajax.Request(
				"painel.php?modulo=principal/painel_controle&acao=A",
				{
					method: 'post',
					parameters: "ckeckbox=" + ckeckbox + "&pesquisaMunicipio=true&cxcid=" + cxcid + "&regcod=" + regcod + "&estuf=" + estuf + "&gtmid=" + gtmid + "&tpmid=" + tpmid + "&municipio=" + municipio,
					asynchronous: false,
					onComplete: function(resp) {
						if(destino) {
							destino.innerHTML = resp.responseText;
						} 
					},
					onLoading: function(){
						Carregando('resultado_pesquisa');
					}
				});
		}
		
		
		function limpar(){
			document.getElementById('indid').value = '';	
			document.getElementById('exoid').value = '';
			document.getElementById('secid').value = '';
			document.getElementById('acaid').value = '';
			document.getElementById('indnome').value = '';
			document.getElementById('tpiid').value = '';
			document.getElementById('regid').value = '';	
			document.getElementById('resultado_pesquisa').innerHTML = '';	
		}
		
		function filtraAcao(secid) {
			if(!secid){
				return false;
			}
			var destino = document.getElementById("td_acao");
			var myAjax = new Ajax.Request(
				"painel.php?modulo=principal/lista&acao=A",
				{
					method: 'post',
					parameters: "filtraAcaoAjax=true&" + "secid=" + secid,
					asynchronous: false,
					onComplete: function(resp) {
						if(destino) {
							destino.innerHTML = resp.responseText;
						} 
					},
					onLoading: function(){
						destino.innerHTML = 'Carregando...';
					}
				});
		}
		
		function selecionaGraficoIndicador(cxcid,indid){
			var div = document.getElementById('conteudo_flutuante');
			Carregando('conteudo_flutuante');
			//mudaEvento('addCaixa','onclick','alert(\'Existe um conteudo sendo adicionado!\')');
			escondeID('button_addCaixa');
			var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: '&selecionaGraficoIndicador=true&indid=' + indid + '&cxcid=' + cxcid,
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
									
								});
					        }
					  });
		}
		
		function selecionaTipoGrafico(cxcid,indid,tipo,sehid,dpeid,tidid){
			var div = document.getElementById('conteudo_flutuante');
			Carregando('conteudo_flutuante');
			//mudaEvento('addCaixa','onclick','alert(\'Já existe uma caixa sendo adicionada!\')');
			escondeID('button_addCaixa');
			var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: 'geraGrafico=true&tipoGraficoIndicador=' + tipo + '&indid=' + indid + '&cxcid=' + cxcid + '&sehid=' + sehid + '&tidid=' + tidid + '&dpeid=' + dpeid,
					        onComplete: function (res)
					        {	
								window.location.reload();
					        }
			});
		}
		
		function removerMeuIndicador(cxcid,indid){
			var myAjax = new Ajax.Request(
				window.location.href,
				{
					method: 'post',
					parameters: "ajaxAddMeusIndicadores=true&acao=remove&indid=" + indid + "&cxcid=" + cxcid,
					asynchronous: false,
					onComplete: function(resp){
						document.getElementById('tr_' + cxcid + '_' + indid).style.display = 'none';
						if(resp.responseText != "ok"){
							alert("Não foi possível remover o indicador!");
						}
					},
					onLoading: function(){
					}
			});
		}
		
		function excluirBox(cxcid){
			
			if(confirm("Deseja realmente excluir?")){
				var div = document.getElementById('conteudo_flutuante');
				Carregando('conteudo_flutuante');
				var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
						        method:     'post',
						        parameters: 'excluirBox=true&cxcid=' + cxcid,
						        onComplete: function (res)
						        {	
									window.location.reload();
						        }
				});
			}
		}
		

		function addMeusIndicadores(id,indid,cxcid){
			if(document.getElementById(id)){
				if(document.getElementById(id).checked == true){
					var acao = "adiciona";
				}
				if(document.getElementById(id).checked == false){
					var acao = "remove";
				}
				var myAjax = new Ajax.Request(
				window.location.href,
				{
					method: 'post',
					parameters: "ajaxAddMeusIndicadores=true&acao=" + acao + "&indid=" + indid + "&cxcid=" + cxcid,
					asynchronous: false,
					onComplete: function(resp){
						//document.getElementById('erro').innerHTML = resp.responseText;
						if(resp.responseText != "ok"){
							if(acao == "adiciona"){
								alert("Não foi possível adicionar o indicador!");
								document.getElementById(id).checked = false;
							}else{
								alert("Não foi possível remover o indicador!");
								document.getElementById(id).checked = true;
							}
						}
					},
					onLoading: function(){
					}
				});
			}else{
				alert("Operação não realizada!");
			}
		}
		
		function addMeusMuncod(id,indid,cxcid){
			if(document.getElementById(id)){
				if(document.getElementById(id).checked == true){
					var acao = "adiciona";
				}
				if(document.getElementById(id).checked == false){
					var acao = "remove";
				}
				var myAjax = new Ajax.Request(
				window.location.href,
				{
					method: 'post',
					parameters: "ajaxAddMeusMuncod=true&acao=" + acao + "&muncod=" + indid + "&cxcid=" + cxcid,
					asynchronous: false,
					onComplete: function(resp){
						//document.getElementById('erro').innerHTML = resp.responseText;
						if(resp.responseText != "ok"){
							alert("Não foi possível adicionar o Município!");
						}
					},
					onLoading: function(){
					}
				});
			}else{
				alert("Não foi possível adicionar o Município!");
			}
		}
		
		function addIndicadoresCaixa(cxcid){
			var div = document.getElementById('conteudo_flutuante');
			Carregando('conteudo_flutuante');
			//mudaEvento('addCaixa','onclick','alert(\'Existe um conteudo sendo adicionado!\')');
			escondeID('button_addCaixa');
			var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: 'checkbox=true&addIndicadorCaixa=true&exibePesquisa=true&cxcid=' + cxcid,
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
		
		function exibeDashBoard(indid){
			<?if ($cabecalho_painel==true){?>
			window.location.href = 'painel2.php?modulo=principal/pc_indicador&acao=A&indid=' + indid;
			<?}else{?>
			window.location.href = 'painel.php?modulo=principal/pc_indicador&acao=A&indid=' + indid;
			<?}?>

		}
		
		function BuscaIndicadorEnter(e,cxcid)
		{
		    if (e.keyCode == 13)
		    {
		        buscarIndicador(cxcid);
		    }
		}
		
		
		function buscarIndicador(cxcid){
			var div = document.getElementById('resultadoBusca_' + cxcid);
			var texto = document.getElementById('busca_' + cxcid);
			
			if(texto.value){
				Carregando('resultadoBusca_' + cxcid);
				var myAjax = new Ajax.Request(
				window.location.href,
				{
					method: 'post',
					parameters: "buscaIndicador=true&cxcid=" + cxcid + "&texto=" + texto.value,
					asynchronous: false,
					onComplete: function(resp){
						div.innerHTML = resp.responseText;
					},
					onLoading: function(){
					}
				});
			}
			
		}
		
		function limparBusca(cxcid){
			var div = document.getElementById('resultadoBusca_' + cxcid);
			var texto = document.getElementById('busca_' + cxcid);
			var myAjax = new Ajax.Request(
				window.location.href,
				{
					method: 'post',
					parameters: "buscaIndicador=true&cxcid=" + cxcid + "&texto=''",
					asynchronous: false,
					onComplete: function(resp){
						div.innerHTML = "";
						texto.value = "";
					},
					onLoading: function(){
					}
				});
			
		}
		
		function limparMunicipios(){
			document.getElementById('regcod').value = '';
			document.getElementById('estuf').value = '';
			document.getElementById('gtmid').value = '';
			document.getElementById('tpmid').value = '';
			document.getElementById('municipio').value = '';
			document.getElementById('resultado_pesquisa').innerHTML = '';
		}
		
		function addArvoreIndicadores(cxcid){
			var div = document.getElementById('conteudo_flutuante');
			Carregando('conteudo_flutuante');
			//mudaEvento('addCaixa','onclick','alert(\'Existe um conteudo sendo adicionado!\')');
			escondeID('button_addCaixa');
			var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: '&addArvoreIndicador=true&cxcid=' + cxcid,
					        onComplete: function (res)
					        {	
								div.innerHTML = res.responseText;
								extraiScript(res.responseText);
								document.getElementById('botoes').style.display = 'none';
								
								jQuery(document).ready(
												function () {
													jQuery('a.closeEl').bind('click', toggleContent);
													jQuery('div.groupWrapper').Sortable(
														{
															accept: 'groupItem',
															helperclass: 'sortHelper',
															activeclass : 	'sortableactive',
															hoverclass : 	'sortablehover',
															handle: 'div.itemHeader',
															tolerance: 'pointer',
															onChange : function(ser)
															{
															},
															onStart : function()
															{
																jQuery.iAutoscroller.start(this, document.getElementsByTagName('body'));
															},
															onStop : function()
															{
																jQuery.iAutoscroller.stop();
															}
														}
													);
												}
								);
								
								
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
		
		function editarNomecaixa(cxcid){
			var cxcdescricao = prompt("Descrição:");
			if(cxcdescricao){
				var myAjax = new Ajax.Request(
					window.location.href,
					{
						method: 'post',
						parameters: "editarNomecaixa=true&cxcdescricao=" + cxcdescricao + "&cxcid=" + cxcid,
						asynchronous: false,
						onComplete: function(resp){
							document.getElementById('titulo_' + cxcid).innerHTML = '<img style="cursor:pointer" onclick="editarNomecaixa(' + cxcid + ')"  src="../imagens/editar_nome.gif" border=0 /> ' + cxcdescricao;
						},
						onLoading: function(){
						}
				});
			}
		}
		
		function salvarArvore(){
			// Agrupadores
			selectAllOptions( formulario.agrupador );
			if ( formulario.agrupador.options.length == 0 ) {
				alert( 'Escolha pelo menos um agrupador.' );
				return false;
			}else{
				formulario.submit();
			}
			
		}
		function mudaPeriodoGrafico(tcgid,cxcid,indid,sehid,tdiid,tipoTidid,idFatia){
			var myAjax = new Ajax.Request(
					window.location.href,
					{
						method: 'post',
						parameters: "EditaSerieHistoricaGrafico=true&sehid=" + sehid + "&tcgid=" + tcgid,
						asynchronous: false,
						onComplete: function(resp){

						},
						onLoading: function(){
						}
				});
			
			var myAjax = new Ajax.Request(
							'painel.php?modulo=principal/painel_controle&acao=A',
							{
								method: 'post',
								parameters: "exibeLegendaGrafico=true&indid=" + indid + "&sehid=" + sehid + "&tdiid=" + tipoTidid + "&valorTdiid=" + idFatia,
								asynchronous: false,
								onComplete: function(resp){
									document.getElementById(cxcid + '_legenda_grafico').innerHTML = resp.responseText;
								},
								onLoading: function(){
								}
							});
			
					
			swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_" + cxcid, "365", "200", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipo=" + indid + ";pizza;" + sehid + ";" + tdiid,"loading":"Carregando gráfico..."} );
			
		}
		
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
		
		function exibeMapaDashBoard(indid){
			var dpeid_inicio = document.getElementById('periodo_inicio').value;
			var dpeid_fim = document.getElementById('periodo_fim').value;
			var sehid = document.getElementById('ultima_sehid').value;
			var estuf = document.getElementById('estuf').value;
			var muncod = document.getElementById('muncod').value;
			var mapa = document.getElementById('div_mapa');
			
			var myAjax = new Ajax.Request(
				'painel.php?modulo=principal/painel_controle&acao=A',
				{
					method: 'post',
					parameters: "exibeMapaDashBoard=true&indid=" + indid + "&sehid=" + sehid + "&dpeid_inicio=" + dpeid_inicio + "&dpeid_fim=" + dpeid_fim + "&estuf=" + estuf + "&muncod=" + muncod,
					asynchronous: false,
					onComplete: function(resp){
						mapa.innerHTML = resp.responseText;
					},
					onLoading: function(){
						mapa.innerHTML = "<center>Carregando...</center>";
					}
			});
		}
		
		function mudaTipoGrafico(tipografico,indid){
				var dpeid_inicio = document.getElementById('periodo_inicio').value;
				var dpeid_fim = document.getElementById('periodo_fim').value;
				var sehid = document.getElementById('ultima_sehid').value;
				
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
						dpeid_fim = false;
						geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tpGrafico + ";" + sehid + ";tdiid" + fatia + "_" + idFatia + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + estuf + ";" + muncod;
						document.getElementById('ate_periodo').style.display = "none";
						document.getElementById('periodo_fim').style.display = "none";
					}
		
				}
				if(erro == "false"){
					swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_indicador", largura, altura, "9.0.0", "expressInstall.swf", {"data-file":geraGrafico,"loading":"Carregando gráfico..."} );
					exibeMapaDashBoard(indid);
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
		
		function editarAba(abaid){
			document.getElementById('abaid_' + abaid).style.display = "";
		}
		
		function alterarNomeAba(abaid,descricao){
			document.getElementById('abaid_' + abaid).style.display = "none";
			var abadsc = prompt("Descrição:",descricao);
			if(abadsc && abaid){
				var myAjax = new Ajax.Request(
					window.location.href,
					{
						method: 'post',
						parameters: "editarNomeAba=true&abadsc=" + abadsc + "&abaid=" + abaid,
						asynchronous: false,
						onComplete: function(resp){
							window.location.reload();
						},
						onLoading: function(){
						}
				});
			}
		}
		
		function excluirAba(abaid){
			document.getElementById('abaid_' + abaid).style.display = "none";
			if(confirm("Deseja realmente excluir a aba e todo seu conteúdo?")){
				var myAjax = new Ajax.Request(
					window.location.href,
					{
						method: 'post',
						parameters: "excluirAba=true&abaid=" + abaid,
						asynchronous: false,
						onComplete: function(resp){
							<?if(!$cabecalho_painel){ ?>
								window.location.href="painel.php?modulo=principal/painel_controle&acao=A";
							<?}else{ ?>
								window.location.href="painel2.php?modulo=principal/painel_controle&acao=A";
							<?} ?>
						},
						onLoading: function(){
						}
				});
			}
		}
		
		function listaIndicadoresSelecionados(cxcid,usucpf){
			var div = document.getElementById('indicadores_selecionados');
			var myAjax = new Ajax.Request(
				window.location.href,
				{
					method: 'post',
					parameters: 'listaIndicadoresSelecionados=true&cxcid=' + cxcid + '&usucpf=' + usucpf,
					asynchronous: false,
					onComplete: function(resp){
						div.innerHTML = resp.responseText;
					},
					onLoading: function(){
						div.innerHTML = "Carregando...";
					}
				});
			
		}
		function listaMunicipiosSelecionados(cxcid,usucpf){
			var div = document.getElementById('municipios_selecionados');
			var myAjax = new Ajax.Request(
				window.location.href,
				{
					method: 'post',
					parameters: 'listaMunicipiosSelecionados=true&cxcid=' + cxcid + '&usucpf=' + usucpf,
					asynchronous: false,
					onComplete: function(resp){
						div.innerHTML = resp.responseText;
					},
					onLoading: function(){
						div.innerHTML = "Carregando...";
					}
				});
			
		}
				
		function marcarTodos(cxcid,usucpf){
			var indid = document.getElementById('arrIndid').value;
			var check = document.getElementById('marca_todos');
			var arrIndid = indid.split(',');
			if(check.checked == true){
				for(i=0;i<arrIndid.length;i++){  
					document.getElementById('meu_indicador_' + arrIndid[i]).checked = true;;
				}
				var myAjax = new Ajax.Request(
					window.location.href,
					{
						method: 'post',
						parameters: 'marcarTodos=true&arrIndid=' + arrIndid + '&cxcid=' + cxcid + '&usucpf=' + usucpf,
						asynchronous: false,
						onComplete: function(resp){
							listaIndicadoresSelecionados(cxcid,usucpf)
						},
						onLoading: function(){
						}
					});
			}else{
				for(i=0;i<arrIndid.length;i++){  
					document.getElementById('meu_indicador_' + arrIndid[i]).checked = false;;
				}
				var myAjax = new Ajax.Request(
					window.location.href,
					{
						method: 'post',
						parameters: 'desmarcarTodos=true&arrIndid=' + arrIndid + '&cxcid=' + cxcid + '&usucpf=' + usucpf,
						asynchronous: false,
						onComplete: function(resp){
							listaIndicadoresSelecionados(cxcid,usucpf)
						},
						onLoading: function(){
						}
					});
			}
			
		}
		
		function marcarTodosMunicipios(cxcid,usucpf){
			var indid = document.getElementById('arrMuncod').value;
			var check = document.getElementById('marca_todos');
			var arrIndid = indid.split(',');
			if(check.checked == true){
				for(i=0;i<arrIndid.length;i++){  
					document.getElementById('meu_muncod_' + arrIndid[i]).checked = true;;
				}
				var myAjax = new Ajax.Request(
					window.location.href,
					{
						method: 'post',
						parameters: 'marcarTodosMunicipios=true&arrMuncod=' + arrIndid + '&cxcid=' + cxcid + '&usucpf=' + usucpf,
						asynchronous: false,
						onComplete: function(resp){
							listaIndicadoresSelecionados(cxcid,usucpf)
						},
						onLoading: function(){
						}
					});
			}else{
				for(i=0;i<arrIndid.length;i++){  
					document.getElementById('meu_muncod_' + arrIndid[i]).checked = false;;
				}
				var myAjax = new Ajax.Request(
					window.location.href,
					{
						method: 'post',
						parameters: 'desmarcarTodosMunicipios=true&arrMuncod=' + arrIndid + '&cxcid=' + cxcid + '&usucpf=' + usucpf,
						asynchronous: false,
						onComplete: function(resp){
							listaIndicadoresSelecionados(cxcid,usucpf)
						},
						onLoading: function(){
						}
					});
			}
			
		}
		
		
		function filtraMunicipio(estuf){
			var div = document.getElementById('exibe_municipio');
			if(estuf != "todos"){
				var myAjax = new Ajax.Request(
					window.location.href,
					{
						method: 'post',
						parameters: "filtraMunicipio=true&estuf=" + estuf,
						asynchronous: false,
						onComplete: function(resp){
							div.innerHTML = resp.responseText;
						},
						onLoading: function(){
							div.innerHTML = '<select id="muncod" style="width:200px;"><option disabled="disabled" selected="selected" value="todos">Carregando...</option></select>';
						}
					});
			}else{
				div.innerHTML = '<select id="muncod" style="width:200px;"><option disabled="disabled" selected="selected" value="todos">Todos os Municípios...</option></select>';
			}
		
		}
		
		function buscarMapa(muncod){
			
			if(muncod)
				var municipio = '&municipio='+ muncod;
			else
				var municipio = '';
				
			inicialytebox('_funcoes_mapa_painel_controle.php?monta_mapa=true' + municipio);
		}
		 function inicialytebox(url) {
			var anchor = this.document.createElement('a'); // cria um elemento de link
		    anchor.setAttribute('rev', 'width: 800px; height: 600px; scrolling: auto;');
		    anchor.setAttribute('title', '');
		    anchor.setAttribute('href', url);
		    anchor.setAttribute('rel', 'lyteframe');
		    // paramentros para do lytebox //
		    myLytebox.maxOpacity = 50;
		    myLytebox.outerBorder = true;
		    myLytebox.doAnimations = false;
		    myLytebox.start(anchor, false, true);
		    return false;
		}
		
		function editarConteudoCaixa(cxcid,tipoConteudo){
			if(tipoConteudo == 1){
				addIndicadoresCaixa(cxcid);
				return true;
			}if(tipoConteudo == 2){
				addGrafico(cxcid);
				return true;
			}if(tipoConteudo == 10){
				addArvoreIndicadores(cxcid);
				return true;
			}if(tipoConteudo == 12){
				addMunicipiosCaixa(cxcid);
				return true;
			}
			else{
				alert('Operação não disponível.');
				return false;
			}
		}
		
		function addMunicipiosCaixa(cxcid){
			var div = document.getElementById('conteudo_flutuante');
				Carregando('conteudo_flutuante');
				escondeID('button_addCaixa');
				var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
						        method:     'post',
						        parameters: 'checkbox=true&addMunicipioCaixa=true&exibePesquisa=true&cxcid=' + cxcid,
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
		
		function filtraEstado(regcod){
			var div = document.getElementById('exibeEstado');
				var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
						        method:     'post',
						        parameters: 'filtraEstado=true&regcod=' + regcod,
						        onComplete: function (res)
						        {	
									div.innerHTML = res.responseText;
									extraiScript(res.responseText);
						        }
						  });
		}
		function filtraGrupoMunicipios(gtmid){
			var div = document.getElementById('exibeTipoMunicipio');
				var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
						        method:     'post',
						        parameters: 'filtraGrupoMunicipios=true&gtmid=' + gtmid,
						        onComplete: function (res)
						        {	
									div.innerHTML = res.responseText;
									extraiScript(res.responseText);
						        }
						  });
		}
		
		function exibeLocalizacaoMapa(muncod){
			buscarMapa(muncod);
		}
		
</script>