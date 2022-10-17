//################################## Início - Parte 1

function amun(muncod) {
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=municipio&muncod='+muncod,'Indicador','scrollbars=yes,height=700,width=700,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aest(estuf) {
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=estado&estuf='+estuf,'Indicador','scrollbars=yes,height=700,width=700,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aesc(esccodinep) {
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=escola&esccodinep='+esccodinep,'Indicador','scrollbars=yes,height=700,width=700,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aies(iesid) {
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=ies&iesid='+iesid,'Indicador','scrollbars=yes,height=700,width=700,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function auni(unicod){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=universidade&unicod='+unicod,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function ains(unicod){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=instituto&unicod='+unicod,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aiep(iepid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=posgraduacao&iepid='+iepid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function acampusprof(entid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=campusprofissional&entid='+entid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function acampussup(entid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=campussuperior&entid='+entid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function ahosp(entid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=hospital&entid='+entid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
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




//################################## Fim - Parte 1

//################################## Início - Parte 2

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
	escondeID('button_addCaixa');
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

function pesquisarUniversidades(cxcid,ckeckbox){
	
	var destino = document.getElementById("resultado_pesquisa");
	Carregando('resultado_pesquisa');
	
	var regcod = document.getElementById('regcod').value;	
	var estuf = document.getElementById('estuf').value;
	var gtmid = document.getElementById('gtmid').value;
	var tpmid = document.getElementById('tpmid').value;
	var municipio = document.getElementById('municipio').value;
	var unidsc = document.getElementById('unidsc').value;
				
	var myAjax = new Ajax.Request(
		"painel.php?modulo=principal/painel_controle&acao=A",
		{
			method: 'post',
			parameters: "ckeckbox=" + ckeckbox + "&pesquisaUniversidade=true&cxcid=" + cxcid + "&regcod=" + regcod + "&estuf=" + estuf + "&gtmid=" + gtmid + "&tpmid=" + tpmid + "&municipio=" + municipio + "&unidsc=" + unidsc,
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

function pesquisarRegionalizadores(cxcid,ckeckbox){
	
	var destino = document.getElementById("resultado_pesquisa");
	Carregando('resultado_pesquisa');
	
	var regid = $('regid').value;
	var regcod = $('regcod').value;
	var estuf = $('estuf').value;
	var mundesc = $('mundesc').value;
	var escoladesc = $('escoladesc').value;
	var unidesc = $('unidesc').value;
	var iesdesc = $('iesdesc').value;
	var campusdesc = $('campusdesc').value;
	var iepgdesc = $('iepgdesc').value;
							
	var myAjax = new Ajax.Request(
		"painel.php?modulo=principal/painel_controle&acao=A",
		{
			method: 'post',
			parameters: "ckeckbox=" + ckeckbox + "&pesquisaRegionalizadores=true&cxcid=" + cxcid + "&regid=" + regid + "&regcod=" + regcod + "&estuf=" + estuf + "&mundesc=" + mundesc + "&escoladesc=" + escoladesc + "&unidesc=" + unidesc + "&iesdesc=" + iesdesc + "&campusdesc=" + campusdesc + "&iepgdesc=" + iepgdesc,
			asynchronous: false,
			onComplete: function(resp) {
				if(destino) {
					destino.innerHTML = resp.responseText;
					extraiScript(resp.responseText);
				} 
			},
			onLoading: function(){
				Carregando('resultado_pesquisa');
			}
		});
}

function limparRegionalizadores(){
	$('regcod').value = '';
	$('estuf').value = '';
	$('mundesc').value = '';
	$('escoladesc').value = '';
	$('unidesc').value = '';
	$('iesdesc').value = '';
	$('campusdesc').value = '';
	$('iepgdesc').value = '';
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

function limparUniversidades(){
	document.getElementById('regcod').value = '';
	document.getElementById('estuf').value = '';
	document.getElementById('gtmid').value = '';
	document.getElementById('tpmid').value = '';
	document.getElementById('municipio').value = '';
	document.getElementById('unidsc').value = '';
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
	
			
	swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_" + cxcid, "365", "200", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipo=pizza;indid=" + indid + ";sehid=" + sehid + ";tdiid=" + tdiid,"loading":"Carregando gráfico..."} );
	
}

function escondePeriodo(tipografico){
	if(tipografico != "linha" && tipografico != "barra"){
		var arrTipoGrafico = tipografico.split('_');
		var tpGrafico = arrTipoGrafico[0];
		var fatia =  arrTipoGrafico[1];
		var idFatia = arrTipoGrafico[2];
		if(tpGrafico == "pizza"){
			if(document.getElementById('indcumulativo')){
				document.getElementById('ate_periodo').style.display = "";
				document.getElementById('periodo_fim').style.display = "";
			}else{
				document.getElementById('ate_periodo').style.display = "none";
				document.getElementById('periodo_fim').style.display = "none";
			}
		}else{
			document.getElementById('ate_periodo').style.display = "";
			document.getElementById('periodo_fim').style.display = "";
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
			//geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda + ";" + estuf + ";" + muncod  + ";" + tidid1  + ";" + tidid2;
			
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
										 ";tidid2="          + tidid2
			
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
			//geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + estuf + ";" + muncod + ";" + tidid1  + ";" + tidid2;
			
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
										 ";tidid2="          + tidid2
			
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
				//geraGrafico = "geraGrafico.php?tipo=" + indid + ";barra_fatia;" + sehid + ";tdiid" + fatia + "_" + idFatia + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + estuf + ";" + muncod  + ";" + tidid1  + ";" + tidid2;
				
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
										 ";tidid2="          + tidid2
				
			}
			if(tpGrafico == "pizza"){
				//geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tpGrafico + ";" + sehid + ";tdiid" + fatia + "_" + idFatia + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + estuf + ";" + muncod  + ";" + tidid1  + ";" + tidid2;
				
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
										 ";tidid2="          + tidid2 
				
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
								parameters: "exibeLegendaGrafico=true&indid=" + indid + "&sehid=" + sehid + "&tdiid=" + fatia + "&valorTdiid=" + idFatia + "&dpeid_inicio=" + dpeid_inicio + "&dpeid_fim=" + dpeid_fim + "&estuf=" + estuf + "&muncod=" + muncod  + "&tidid1=" + tidid1  + "&tidid2=" + tidid2,
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
	return false;
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


//selecionaTipoGrafico(609,135,'barra_comp','1','10');
function selecionaTipoGrafico(cxcid,indid,tipo,tipoTidid,tidid){
	var div = document.getElementById('conteudo_flutuante');
	Carregando('conteudo_flutuante');
	escondeID('button_addCaixa');
	var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
			        method:     'post',
			        parameters: 'exibeFiltroGrafico=true&cxcid=' + cxcid + '&indid=' + indid + '&tipo=' + tipo + '&tipoTidid=' + tipoTidid + '&tidid=' + tidid,
			        onComplete: function (res){	
						div.innerHTML = res.responseText;
						extraiScript(res.responseText);
						jQuery(document).ready(function() {
							vizualizarGrafico(indid);
						});
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
				if (resp.responseText.search("ok") < 0){
					alert("Não foi possível remover o indicador!");
				}else{
					document.getElementById('tr_' + cxcid + '_' + indid).style.display = 'none';
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
				if (resp.responseText.search("ok") < 0){
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
function listaUniversidadesSelecionados(cxcid,usucpf){
	var div = document.getElementById('universidades_selecionadas');
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: 'listaUniversidadesSelecionados=true&cxcid=' + cxcid + '&usucpf=' + usucpf,
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
	}if(tipoConteudo == 13){
		addRegionalizadoresCaixa(cxcid);
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

function addUniversidadeCaixa(cxcid){
	var div = document.getElementById('conteudo_flutuante');
		Carregando('conteudo_flutuante');
		escondeID('button_addCaixa');
		var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
				        method:     'post',
				        parameters: 'checkbox=true&addUniversidadeCaixa=true&exibePesquisa=true&cxcid=' + cxcid,
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

function addRegionalizadoresCaixa(cxcid){
	var div = document.getElementById('conteudo_flutuante');
		Carregando('conteudo_flutuante');
		escondeID('button_addCaixa');
		var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
				        method:     'post',
				        parameters: 'checkbox=true&addRegionalizadoresCaixa=true&exibePesquisa=true&cxcid=' + cxcid,
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
				window.location.href,
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
				window.location.href,
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

function vizualizarGrafico(indid){
		var tipografico = document.getElementById('tipo_grafico').value;
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
			//geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda + ";" + estuf + ";" + muncod + ";" + gtmid + ";" + tpmid  + ";" + tidid1  + ";" + tidid2 + ";" + regcod;
			
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
										 ";regcod="          + regcod 
			
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
			//geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + estuf + ";" + muncod  + ";" + gtmid + ";" + tpmid  + ";" + tidid1  + ";" + tidid2 + ";" + regcod;
			
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
										 ";regcod="          + regcod
			
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
										 ";regcod="          + regcod
				
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
										 ";regcod="          + regcod
				
			}
			if(tpGrafico == "pizza"){
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
										 ";regcod="          + regcod
			}

		}
		if(erro.search("false") >= 0){
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
								document.getElementById('div_graficos').innerHTML = '<fieldset style="height: 400px;"><legend>Gráficos</legend><div style="overflow: auto; width: 100%; height: 100%;"><div id="grafico_indicador">Gráfico indisponível.</div><div id="legenda_grafico"/></div></fieldset>';
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
		}else{
			alert('A data incial deve ser menor que a data final!');
		}
			
}

function salvarGrafico(indid,cxcid){
		var tipografico = document.getElementById('tipo_grafico').value;
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
			//geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda + ";" + estuf + ";" + muncod + ";" + gtmid + ";" + tpmid  + ";" + tidid1  + ";" + tidid2 + ";" + regcod;
			
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
										 ";regcod="          + regcod
			
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
			//geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";" + unidade_inteiro + ";" + unidade_moeda + ";" + indice_moeda  + ";" + estuf + ";" + muncod  + ";" + gtmid + ";" + tpmid  + ";" + tidid1  + ";" + tidid2 + ";" + regcod;
			
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
										 ";regcod="          + regcod
			
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
										 ";regcod="          + regcod
				
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
										 ";regcod="          + regcod
				
			}
			if(tpGrafico == "pizza"){
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
										 ";regcod="          + regcod
				
			}

		}
		if(erro.search("false") >= 0){
			//Verifica se o grafico pode ser exibido
			var myAjax = new Ajax.Request(
					'painel.php?modulo=principal/painel_controle&acao=A',
					{
						method: 'post',
						parameters: "verificaExibeGrafico=true&indid=" + indid + "&estuf=" + estuf + "&muncod=" + muncod  + "&gtmid=" + gtmid + "&tpmid=" + tpmid + "&tidid1=" + tidid1  + "&tidid2=" + tidid2 + "&regcod=" + regcod,
						asynchronous: false,
						onComplete: function(resp){
							var exibe = resp.responseText;
							if (exibe.search("true") >= 0){
								//Salva as configurações de exibição do Gráfico em Banco
								var dados = geraGrafico.split("geraGrafico.php?");
																								
								var myAjax2 = new Ajax.Request(
									'painel.php?modulo=principal/painel_controle&acao=A',
									{
										method: 'post',
										parameters: "salvaGrafico=true&dados=" + dados[1] + "&cxcid=" + cxcid,
										asynchronous: false,
										onComplete: function(resp){
											//document.getElementById('erro').innerHTML = resp.responseText;
											window.location.reload();
										},
										onLoading: function(){
										}
									});
								//swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_indicador", largura, altura, "9.0.0", "expressInstall.swf", {"data-file":geraGrafico,"loading":"Carregando gráfico..."} );
							}else{
								alert("Não foi possível salvar o Gráfico!");
								window.location.reload();
								//document.getElementById('div_graficos').innerHTML = '<fieldset style="height: 400px;"><legend>Gráficos</legend><div style="overflow: auto; width: 100%; height: 100%;"><div id="grafico_indicador">Gráfico indisponível.</div><div id="legenda_grafico"/></div></fieldset>';
							}						
						},
						onLoading: function(){
							document.getElementById('grafico_indicador').innerHTML = "<center>Carregando...</center>";
						}
				});
		}else{
			alert('A data incial deve ser menor que a data final!');
		}			
}

function maximizaGrafico(parametros){
	inicialytebox('painel.php?modulo=principal/painel_controle&acao=A&maximizaGrafico=true&parametros=' + parametros);
}

function addRegionalizador(obj,cxcid,regid){
	
	if(obj.checked == false){
		var acao = "excluir";
	}else{
		var acao = "incluir";
	}
	
	var myAjax = new Ajax.Request(
		'painel.php?modulo=principal/painel_controle&acao=A',
		{
			method: 'post',
			parameters: "addRegionalizador=true&cxcid=" + cxcid + "&regid=" + regid + "&codigo=" + obj.value + "&acao=" + acao,
			asynchronous: false,
			onComplete: function(resp){
				$('erro').update(resp.responseText);
			},
			onLoading: function(){
			}
		});
}

function abaPadrao(aba){
	if(document.getElementById('abaPadrao')){
		document.getElementById('abaPadrao').value = aba;
	}
}

function exibeFiltrosPainel(obj){
	if(obj.title == "mais"){
		obj.title="menos";
		obj.src="../imagens/menos.gif";
		document.getElementById('filtros_painel').style.display = "";
	}else{
		obj.title="mais";
		obj.src="../imagens/mais.gif";
		document.getElementById('filtros_painel').style.display = "none";
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

function exibePaginaInicial(){
	Carregando('conteudo_flutuante');
	var myAjax = new Ajax.Request(
			'painel.php?modulo=principal/painel_controle&acao=A',
			{
				method: 'post',
				parameters: "paginaInicial=true",
				asynchronous: false,
				onComplete: function(resp){
					$('conteudo_flutuante').update(resp.responseText);
					extraiScript(resp.responseText);
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

function exibeBuscaRegionalizacaoEnter(e)
	{
	    if (e.keyCode == 13)
	    {
	        exibeBuscaRegionalizacao();
	    }
	}

function exibeBuscaRegionalizacao(){
	Carregando('td_exibe_resultado_busca');
	var myAjax = new Ajax.Request(
			'painel.php?modulo=principal/painel_controle&acao=A',
			{
				method: 'post',
				parameters: "exibeBuscaRegionalizacao=true&busca=" + document.getElementById('busca').value,
				asynchronous: false,
				onComplete: function(resp){
					$('td_exibe_resultado_busca').update(resp.responseText);
					extraiScript(resp.responseText);
				}
			});
}

function exibeResultadoBuscaRegionalizador(regid){
		if( jQuery('#img_seta_' + regid ).attr("src") == "../imagens/seta_cima.png" ){
			jQuery("#div_exibe_regid_" + regid ).slideUp(300);
			jQuery('#img_seta_' + regid ).attr("src","../imagens/seta_baixo.png");
			return false;
		}else{
			jQuery("#div_exibe_regid_" + regid ).slideDown(300);
			jQuery('#img_seta_' + regid ).attr("src","../imagens/seta_cima.png");
			return false;
		}
}

function exibeMapaRegionalizador(regid){
	Carregando('conteudo_flutuante');
	var myAjax = new Ajax.Request(
			'painel.php?modulo=principal/painel_controle&acao=A',
			{
				method: 'post',
				parameters: "exibeMapaRegionalizador=true&regid=" + regid,
				asynchronous: false,
				onComplete: function(resp){
					$('conteudo_flutuante').update(resp.responseText);
					extraiScript(resp.responseText);
				}
			});
}

function exibeRegionalizador(regiao,regid,letraInicial){
	Carregando('exibeBuscaRegionalizador');
		var buscakey = $('buscakey').value;
	if(!letraInicial || letraInicial == "undefined"){
		var letraInicial = $('hidden_letra_inicial').value;
	}
	$('hidden_regiao').setValue(regiao);
	var myAjax = new Ajax.Request(
			'painel.php?modulo=principal/painel_controle&acao=A',
			{
				method: 'post',
				parameters: "exibeBuscaRegionalizador=true&regid=" + regid + "&buscakey=" + buscakey + "&regiao=" + regiao + "&letraInicial=" + letraInicial,
				asynchronous: false,
				onComplete: function(resp){
					$('exibeBuscaRegionalizador').update(resp.responseText);
					extraiScript(resp.responseText);
				}
			});
}

function exibeRegionalizadorKeyUp(buscakey,regid){
	var regiao = $('hidden_regiao').value;
	$('hidden_letra_inicial').setValue("");
	setTimeout("verificaKeyUp('" + buscakey + "','" + regiao + "','" + regid + "');",1000);	 
}

function exibeRegionalizadorLetraInicial(regiao,regid,letraInicial){
	$('hidden_letra_inicial').setValue(letraInicial);
	$('buscakey').setValue("");
	exibeRegionalizador(regiao,regid,letraInicial);
}

function verificaKeyUp(busca,regiao,regid){
	var buscakey = $('buscakey').value;
	var letraInicial = $('hidden_letra_inicial').value;
	if(buscakey == busca){
		exibeRegionalizador(regiao,regid,letraInicial);
	}else{
		return false;
	}
}


//################################## Fim - Parte 2