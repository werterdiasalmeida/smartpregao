/* 
// +----------------------------------------------------------------------+
// | Copyright (c) 2005 Adonias Malosso                                   |
// +----------------------------------------------------------------------+
// | Licensed under the Apache License, Version 2.0 (the "License");      |
// | you may not use this file except in compliance with the License.     |
// | You may obtain a copy of the License at                              |
// | http://www.apache.org/licenses/LICENSE-2.0                           |
// | Unless required by applicable law or agreed to in writing, software  |
// | distributed under the License is distributed on an "AS IS" BASIS,    |
// | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or      |
// | implied. See the License for the specific language governing         |
// | permissions and limitations under the License.                       |
// +----------------------------------------------------------------------+
// | Author: Adonias Malosso <malosso@gmail.com>                          |
// +----------------------------------------------------------------------+
*/

var liveSearchRoot = "";
var lsScrollAmount = 18;
var t = false;
var i = 0;
var xmlDoc;
var isIE = false;

// campos devem ser adicionados à submissão do formulário
// cada chave é o id do campo no html
var lsExtraFieldParameter = new Array();

// valores dos campos de acordo com o valor do código
// utilizado para adicionar aos labels dos campos
var lsLabel = new Array();

var lsValida = new Array();

function liveSearchHide( idobj )
{
	var input = document.getElementById( idobj );
	// esconde div de listagem
	var result = document.getElementById( "LSResult_" + idobj );
	if ( result.style.display == "none" )
	{
		lsVerificaValor( input );
		return;
	}
	result.innerHTML = '';
	result.style.display = "none";
	// FIM esconde div de listagem
	// atualiza valor do label
	var label = document.getElementById( 'LSLabel_' + idobj );
	var hidden_input = document.getElementById( 'hidden_' + input.name );
	if ( label )
	{
		if ( input.value == '' )
		{
			label.innerHTML = '';
		}
		else
		{
			var valor_label = lsLabel[input.id][input.value];
			label.innerHTML = valor_label ? valor_label : '' ;
			if ( hidden_input )
			{
				hidden_input.value = valor_label ? valor_label : '' ;
			}
		}
	}
	lsVerificaValor( input );
}

/**
 * verifica se o valor está presente na lista de valores possíveis
 * 
 * @param object
 * @return void
 */
function lsVerificaValor( input )
{
	if ( !input.valida )
	{
		return;
	}
	var hidden_input = document.getElementById( 'hidden_' + input.name );
	if ( ( hidden_input.value == '' && input.value != '' ) || ( hidden_input.value != '' && input.value == '' ) )
	{
		var valorValido = false;
		for ( var i = 0, j = input.valores.length; i < j; i++ )
		{
			if ( input.valores[i] == input.value )
			{
				valorValido = true;
				break;
			}
		}
		if ( !valorValido )
		{
			input.value = '';
			alert( 'Valor inválido!' );
		}
	}
}

function liveSearchShow(idobj) {
	document.getElementById("LSResult_"+idobj).style.display = "";
}

function liveSearchHideDelayed(idobj) {
	window.setTimeout("liveSearchHide(\"" + idobj + "\")",150);
}




/**
 * Inicia a busca de um campo ajax, caso não tenha sucesso
 * por algum motivo (campos dependentes ou exceções) a função
 * retorna falso.
 * 
 * @return boolean
 */
function liveSearchStart( objetocampo, campos_extra )
{
	// define campos extra a serem enviados

	lsExtraFieldParameter[objetocampo.id] = campos_extra;
	jsNode = objetocampo;
	var j = campos_extra.length;
	var valor_campo_extra = '';
	for ( var i = 0; i < j; i++ )
	{
		campo_extra = document.getElementById( campos_extra[i] );
		if ( campo_extra )
		{
			if ( campo_extra.value == '' )
			{
				jsNode.blur();
				campo_extra.focus();
				alert( 'Escolha primeiro o campo ' + campo_extra.title + '.' );
				return false;
			}
			else // if ( !lsLabel[campo_extra.id] )
			{
				if ( !document.getElementById( campo_extra.id ).value )
				{
					jsNode.blur();
					campo_extra.value = '';
					document.getElementById( 'LSLabel_' + campo_extra.id ).innerHTML = '';
					campo_extra.focus();
					alert( 'Campo ' + campo_extra.title + ' inválido.' );
					return false;
				}
			}
		}
	}
	try {
		
		if (navigator.userAgent.indexOf("Safari") > 0) {
			jsNode.addEventListener( "keyup", lsKeyPress, false );
		} else if ( navigator.product == "Gecko") {
			jsNode.addEventListener( "keyup", lsKeyPress, false );
		} else {
			jsNode.onkeyup = new Function( '' );
			jsNode.attachEvent( 'onkeyup', lsKeyPress );
			isIE = true;
		}
		
		jsNode.setAttribute("autocomplete","off");
		jsNode.posicao = i++;
		jsNode.highlight = 0;
		jsNode.valores = new Array(1);
		jsNode.valor = '';
		jsNode.onblur = new Function( "MouseBlur(this);liveSearchHideDelayed(\"" + jsNode.id + "\");");
		
		var onchange = '';
		if ( campos_extra.length )
		{
			onchange += "lsLimpaDependentes( '" + jsNode.id + "' );";
		}
		jsNode.onchange = new Function( onchange );
	}
	catch(e)
	{
		return false;
	}
	return true;
}


function lsProcessaResultado(idobj, stringXML) {
	var  jsNode = document.getElementById(idobj);
	var  divRes = document.getElementById("LSResult_" + jsNode.id);
	var  strSaida = '<div class="LSResultInner"><table class="lsLista">';	
/*
	if (document.implementation && document.implementation.createDocument) {
		xmlDoc = (new DOMParser()).parseFromString(stringXML,"text/xml");
	}
	else if (window.ActiveXObject) {
		xmlDoc = new ActiveXObject("Microsoft.XMLDOM");
		xmlDoc.async = "false";
		xmlDoc.loadXML(stringXML);
 	}
	else {
		alert('Your browser can\'t handle this script');
		return;
	}*/
	try {
		eval(stringXML);
	} catch ( e )
	{
		alert( 'Não foi possível carregar dados.' );
	}
	//alert(typeof(resultado));
	//alert(resultado.length);
	//var resultado = xmlDoc.getElementsByTagName("linha");

	//jsNode.valores = new Array(resultado.length);

	// armazena o valor do campo
	var valor_codigo = 0;
	var valor_id = '';
	
	lsLabel[idobj] = new Array();
	for(i = 0; i < resultado.length; i++) {
		if(i==0) {
			//lsCompletarCampo(idobj, resultado[i].childNodes[0].firstChild.nodeValue);
			lsCompletarCampo(idobj, resultado[i][0]);
		}
		jsNode.valores[i] = resultado[i][0];
		
		valor_codigo = resultado[i][0];
		valor_descricao = resultado[i][1];
		
		// armazena o descrição de acordo com o valor para os labels
		lsLabel[idobj][valor_codigo] = valor_descricao;
		
		var onclick = ' onclick="document.getElementById(\'' + idobj + '\').value=\'' + valor_codigo + '\';lsLimpaDependentes( \'' + idobj + '\' );" ';
		
		if(isIE) {
			strSaida += "<tr bgcolor=\"#FFFFFF\" onmouseover=\"this.bgColor='#ffffcc'; this.style.cursor='hand'\" onmouseout=\"this.bgColor='#ffffff';\">";
			strSaida += '<td height=\"'+lsScrollAmount+'\" nowrap="nowrap" class="' + (i==0 ? 'lsResultado_hover' : '') + '" ' + onclick + '>' + valor_descricao + '</td>';
		}
		else {
			strSaida += '<tr>';
			strSaida += '<td ' + valor_id + ' height=\"'+lsScrollAmount+'\" nowrap="nowrap" class="' + (i==0 ? 'lsResultado_hover' : 'lsResultado')+ '" ' + onclick + '>' + valor_descricao + '</td>';
		}
		strSaida += '</tr>';
	}
	
	strSaida += '</table></div>';
	divRes.style.display = "block";
	divRes.innerHTML = strSaida;
}

liveSearchDoSearch = function (idobj) {
	var jsNode = document.getElementById(idobj);

	if (typeof jsNode == "undefined" ) {
		return false;
	}
	if (typeof liveSearchRoot == "undefined") {
		liveSearchRoot = "";
	}
	if (typeof liveSearchRootSubDir == "undefined") {
		liveSearchRootSubDir = "";
	}
	if (typeof liveSearchParams == "undefined") {
		liveSearchParams2 = "";
	} else {
		liveSearchParams2 = "&" + liveSearchParams;
	}
	
	// define parametros para o sql no servidor
	var q = '';
	var tamanho_extra = lsExtraFieldParameter[idobj] ? lsExtraFieldParameter[idobj].length : 0 ;
	var campo_extra = null;
	if ( tamanho_extra > 0 )
	{
		for ( var i = 0; i < tamanho_extra; i++ )
		{
			campo_extra = document.getElementById( lsExtraFieldParameter[idobj][i] );
			if ( campo_extra )
			{
				q += '&q[]=' + campo_extra.value;
			}
		}
		q += '&q[]=' + jsNode.value;
	}
	else
	{
		// compatibilidade com códigos antigos
		// ver arquivo que recebe requisição e observar a definição do sql
		q = '&q=' + jsNode.value;
	}
	// fim define parametros para o sql no servidor
	try {
		
		if ( jsNode._xmlHttp.last != jsNode.value ) {
			if (jsNode._xmlHttp && jsNode._xmlHttp.readyState < 4) {
				jsNode._xmlHttp.abort();
			}
			/*
			// comentado para permitir busca quando campo estiver em branco
			if ( jsNode.value == "") {			
				liveSearchHide(jsNode.id);
				return false;
			}
			*/
			try {
				jsNode._xmlHttp.open("GET", liveSearchRoot + "/livesearch.php?campo="+ jsNode.id + q + liveSearchParams2, true);	
				jsNode.last = jsNode.value;
				jsNode._xmlHttp.onreadystatechange = new Function("ls_func_onload(\"" + idobj + "\")");
			}
			catch (e) {
				//lsTravarCampo(jsNode.id);			
				liveSearchHide(jsNode.id);
				return false;			
			}
			// call in new thread to allow ui to update
			window.setTimeout("ls_func_ontimeout(\"" + idobj + "\")", 300);
		}
	}
	catch(e) {}

};
ls_func_ontimeout = function (idobj) {var jsNode = document.getElementById(idobj);
									try {
									if(typeof jsNode._xmlHttp != "undefined") jsNode._xmlHttp.send(null);	}
	catch(e) {}};

ls_func_onload = function (idobj) {
	var jsNode = document.getElementById(idobj);
	try {
	if (jsNode._xmlHttp.readyState == 4) {

		if (jsNode._xmlHttp.status==200)
		{
			if( jsNode._xmlHttp.responseText != "") {
				lsProcessaResultado(jsNode.id, jsNode._xmlHttp.responseText);
			}
			else {
				//lsTravarCampo(jsNode.id);
			}
		}
		else
		{
			alert("Erro! No foi possvel Recuperar os Dados!");
		}

		if (jsNode._xmlHttp.dispose)
			jsNode._xmlHttp.dispose();
		jsNode._xmlHttp = null;
	}
	}
	catch(e) {}
};

function lsCompletarCampo(idcampo, valor) {
	var campo = document.getElementById(idcampo);
	var oldval = campo.valor;
	
	if(typeof campo != "undefined") {
		campo.value = valor;
//		campo.select();
	}
	
	if( document.selection ){
		range = campo.createTextRange();
		range.moveStart('character', oldval.length);
		range.moveEnd('character', valor.length);
		range.select();
	}
	else if (document.getSelection) {
		campo.selectionStart = oldval.length;
	}
}

function lsTravarCampo(idcampo) {
	var campo = document.getElementById(idcampo);
	campo.value = campo.value.substr(0, campo.value.length-1);
}

function pegarLayerResultado(obj) {
	var lsLayer = document.getElementById("LSResult_"+obj.id);
	
	if(typeof(lsLayer) != "undefined")
		return lsLayer;
	else
		return null;
}

/**
 * Inicia busca de um campo ajax a partir do seu valor.
 * 
 * @param object
 * @return void
 */
function lsFazerBuscar( input )
{
	input.focus(); // para o caso dele vir da seta do "falso combo"
	input._xmlHttp = lsCriaHttpRequest();
	input.valor = input.value ? input.value : '' ;
	window.setTimeout( "liveSearchDoSearch(\"" + input.id + "\")", 200 );
}

/**
 * Criar um objeto para requisição ajax
 * 
 * @return XMLHttpRequest
 */
function lsCriaHttpRequest()
{
	return window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject("Microsoft.XmlHttp");
}

// ver comentário interno da função lsKeyPress(), onde essa variável é manipulada
var lsKeyPressPosIE = 0;

function lsKeyPress(event) {
	try {	
			
		if( isIE )
			var origem = event.srcElement;
		else
			var origem = event.target;
		
		origem.setAttribute("autocomplete","off");
		
		/*
			Por algum motivo, no Microsoft Internet Explorer, o evento
			keydown atribuido ao input ajax é chamada duas vezes. A
			solução temporária que resolve o problema é a utilização de
			um contador para esse navegador.
		*/
		if ( isIE )
		{
			lsKeyPressPosIE++;
			if ( lsKeyPressPosIE > 1 )
			{
				lsKeyPressPosIE = 0;
				return;
			}
		}
		
		/*
		criação do objeto de requisição http substituído e centralizado pelo método lsCriaHttpRequest()
		este método é utilizado pela função lsFazerBuscar()
		origem._xmlHttp = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject("Microsoft.XmlHttp");
		*/
		
		var layer = pegarLayerResultado(origem);
		var itens = layer.getElementsByTagName("td");
	
		if( !origem.highlight )
		{
			//origem.highlight = 0;
		}
		
		if (event.keyCode == 40 )
		//KEY DOWN
		{
			// faz busca quando campo em branco
			if ( origem.value.length == 0 )
			{
				lsFazerBuscar( origem );
			}
			else
			{
				itens[origem.highlight].className = 'lsResultado';
				origem.highlight += 1;
				if ( origem.highlight >= itens.length )
				{
					origem.highlight = itens.length-1;
				}
				itens[origem.highlight].className = 'lsResultado_hover';
				lsCompletarCampo(origem.id, origem.valores[origem.highlight]);
				if ( !isIE )
				{
					event.preventDefault();
				}
				lsScroll(layer, (lsScrollAmount*origem.highlight)-(lsScrollAmount*2) );
				lsLimpaDependentes( origem.id ); // limpa os campos dependentes
			}
		}
		//KEY UP
		else if ( event.keyCode == 38 )
		{		
			itens[origem.highlight].className = 'lsResultado';
			origem.highlight -= 1;
			if ( origem.highlight <= 0 )
			{
				origem.highlight = 0;
			}
			itens[origem.highlight].className = 'lsResultado_hover';
			lsCompletarCampo(origem.id, origem.valores[origem.highlight]);		
			if ( !isIE )
			{
				event.preventDefault();
			}
			lsScroll( layer, (lsScrollAmount*origem.highlight)-(lsScrollAmount*2) );
			lsLimpaDependentes( origem.id ); // limpa os campos dependentes
		} 
		//ENTER
		else if (event.keyCode == 13) {
			origem.valor = origem.value;
			lsCompletarCampo(origem.id, origem.value);
			liveSearchHide(origem.id);
			lsLimpaDependentes( origem.id ); // limpa os campos dependentes
		}
		// DEL
		else if ( event.keyCode == 27 ) {
			liveSearchHide(origem.id);
		}
		/*// ESC
		else if ( event.keyCode == 46 ) {
			alert( 'g' );
		}*/
		else {
			if(event.keyCode == 8) {
				var txt = origem.valor.substr(0, origem.valor.length-1);
				origem.value = txt;
				origem.highlight = 0;
				lsScroll(layer,0);
			}
			origem.setAttribute( "autocomplete", "off" );
			lsFazerBuscar( origem );
			/*
			inicialização da busca por ajax substituído e centralizado pelo método acima
			origem.valor = origem.value;
			window.setTimeout("liveSearchDoSearch(\"" + origem.id + "\")",200);
			*/
		}
	} catch( e ) {}
	
	// limpa label caso o valor do campo seja apagado
	if ( origem.value == '' )
	{
		lsLimpaLabel( origem );
	}
	return true;
}

function lsScroll(camada, qtdepx) {
	camada.scrollTop = qtdepx;
}

/**
 * Estrutura de dependencia entre os campos ajax.
 * Observar função php campo_texto_ajax_cascata()
 * 
 * var object
 */
var lista_campo_cascata = new Array();

/**
 * Apaga o conteudo do label do campo.
 * 
 * @param object
 * @return void
 */
function lsLimpaLabel( input )
{
	if ( input )
	{
		var label = document.getElementById( 'LSLabel_' + input.id );
		if ( label )
		{
			label.innerHTML = '';
		}
	}
}

/**
 * Limpa os campos que dependem do valor do campo em questão. 
 * 
 * var string
 */
function lsLimpaDependentes( input_id )
{
	var input = document.getElementById( input_id );
	if ( !input )
	{
		return;
	}
	var j = lista_campo_cascata.length;
	var nome = input.name;
	var input_auxiliar = null;
	for ( var i = 0; i < j; i++ )
	{
		if ( lista_campo_cascata[i]['dependencia'] == nome )
		{
			input_auxiliar = document.getElementById( lista_campo_cascata[i]['nome'] );
			input_auxiliar.value = '';
			lista_campo_cascata[i]['valor'] = '';
			// limpa label caso o valor do campo seja apagado
			lsLimpaLabel( input_auxiliar );
			lsLimpaDependentes( input_auxiliar.id );
		}
	}
}

