 /*
   Sistema Sistema Simec
   Setor responsável: SPO/MEC
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   Módulo: valida.js
   Finalidade: Funções de validação em Javascript
   Data de criação: 03/08/2005
   */
   

/*
objetivo: mascarar de acordo com a mascara passada
caracteres: # - caracter a ser mascarado
           | - separador de mascaras
modos (exemplos):
mascara simples: "###-####"	                 mascara utilizando a mascara passada
mascara composta: "###-####|####-####"       mascara de acordo com o tamanho (length) do valor passado
mascara dinâmica: "[###.]###,##"             multiplica o valor entre colchetes de acordo com o length do valor para que a mascara seja dinâmica ex: ###.###.###.###,##
utilizar no onkeyup do objeto
ex: onkeyup="this.value = mascara_global('#####-###',this.value);"
tratar o maxlength do objeto na página (a função não trata isso)
*/

function mascaraglobal(mascara, valor){

        var mascara_utilizar;
        var mascara_limpa;
        var temp;
        var i;
        var j;
        var caracter;
        var separador;
        var dif;
        var validar;
        var mult;
        var ret;
        var tam;
        var tvalor;
        var valorm;
        var masct;
        tvalor = "";
        ret = "";
        caracter = "#";
        separador = "|";
        mascara_utilizar = "";
        valor = trim(valor);
        if (valor == "")return valor;
        temp = mascara.split(separador);
        dif = 1000;

        valorm = valor;
        //tirando mascara do valor já existente
        for (i=0;i<valor.length;i++){
                if (!isNaN(valor.substr(i,1))){
                        tvalor = tvalor + valor.substr(i,1);
                }
        }
        valor = tvalor;

        //formatar mascara dinamica
        for (i = 0; i<temp.length;i++){
                mult = "";
                validar = 0;
                for (j=0;j<temp[i].length;j++){
                        if (temp[i].substr(j,1) == "]"){
                                temp[i] = temp[i].substr(j+1);
                                break;
                        }
                        if (validar == 1)mult = mult + temp[i].substr(j,1);
                        if (temp[i].substr(j,1) == "[")validar = 1;
                }
                for (j=0;j<valor.length;j++){
                        temp[i] = mult + temp[i];
                }
        }


        //verificar qual mascara utilizar
        if (temp.length == 1){
                mascara_utilizar = temp[0];
                mascara_limpa = "";
                for (j=0;j<mascara_utilizar.length;j++){
                        if (mascara_utilizar.substr(j,1) == caracter){
                                mascara_limpa = mascara_limpa + caracter;
                        }
                }
                tam = mascara_limpa.length;
        }else{
                //limpar caracteres diferente do caracter da máscara
                for (i=0;i<temp.length;i++){
                        mascara_limpa = "";
                        for (j=0;j<temp[i].length;j++){
                                if (temp[i].substr(j,1) == caracter){
                                        mascara_limpa = mascara_limpa + caracter;
                                }
                        }

                        if (valor.length > mascara_limpa.length){
                                if (dif > (valor.length - mascara_limpa.length)){
                                        dif = valor.length - mascara_limpa.length;
                                        mascara_utilizar = temp[i];
                                        tam = mascara_limpa.length;
                                }
                        }else if (valor.length < mascara_limpa.length){
                                if (dif > (mascara_limpa.length - valor.length)){
                                        dif = mascara_limpa.length - valor.length;
                                        mascara_utilizar = temp[i];
                                        tam = mascara_limpa.length;
                                }
                        }else{
                                mascara_utilizar = temp[i];
                                tam = mascara_limpa.length;
                                break;
                        }
                }
        }

        //validar tamanho da mascara de acordo com o tamanho do valor
        if (valor.length > tam){
                valor = valor.substr(0,tam);
        }else if (valor.length < tam){
                masct = "";
                j = valor.length;
                for (i = mascara_utilizar.length-1;i>=0;i--){
                        if (j == 0) break;
                        if (mascara_utilizar.substr(i,1) == caracter){
                                j--;
                        }
                        masct = mascara_utilizar.substr(i,1) + masct;
                }
                mascara_utilizar = masct;
        }

        //mascarar
        j = mascara_utilizar.length -1;
        for (i = valor.length - 1;i>=0;i--){
                if (mascara_utilizar.substr(j,1) != caracter){
                        ret = mascara_utilizar.substr(j,1) + ret;
                        j--;
                }
                ret = valor.substr(i,1) + ret;
                j--;
        }
        return ret;
}

/**
 * Abre a janela de interação do combo_popup. Veja restante dos
 * comentário do arquivo www/geral/combopopup.php
 * 
 * @param string nome
 * @param integer height
 * @param integer width
 * @return void
 */
function combo_popup_abre_janela( nome, height, width )
{
	var campo_select = document.getElementById( nome );
	for ( var i = 0; i < campo_select.options.length; i++ )
	{
		campo_select.options[i].selected = false;
	}
	//window.open( '../geral/combopopup.php?nome=' + nome, nome, "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
	window.open( '../geral/combopopup.php?nome='+nome, 'Combopopup', "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
}

function combo_popup_keypress_buscar_codigo( event, nome, valor )
{
	if ( event.keyCode == 13 )
	{
		combo_popup_buscar_codigo( nome, valor );
	}
}

function combo_popup_recebe_buscar_codigo( input, nome )
{
	httpRequest = input.httpRequest;
	if ( httpRequest.readyState == 4 )
	{
		if ( httpRequest.status == 200 )
		{
			var dados = httpRequest.responseText.split( "\n" );
			if ( dados.length == 2 )
			{
				combo_popup_adicionar_item( nome, dados[0], dados[1], true );
				input.value = '';
			}
			else
			{
				alert( 'Nenhum registro encontrado com o código indicado.' );
			}
			input.focus();
		}
	}
}

function combo_popup_codigo_selecionado( nome, cod )
{
	var campo_select = document.getElementById( nome );
	var j = campo_select.length;
	for( var i = 0; i < j; i++ )
	{
		if ( cod == campo_select.options[i].value )
		{
			return true;
		}
	}
	return false;
}

function combo_popup_buscar_codigo( nome, cod )
{
	var input = document.getElementById( 'combopopup_campo_busca_' + nome );
	if ( combo_popup_codigo_selecionado( nome, cod ) == true )
	{
		input.value = '';
		input.focus();
		return;
	}
	input.httpRequest = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );
	input.httpRequest.onreadystatechange = function(){ combo_popup_recebe_buscar_codigo( input, nome ); };
	input.httpRequest.open( 'GET', '../geral/combopopup.php?nome=' + escape( nome ) + '&pegar_dados_item=1&codigo_busca=' + escape( cod ), true );
	input.httpRequest.send( null );
}

/**
 * Abre a janela de interação do texto_popup. Veja restante dos
 * comentário do arquivo www/geral/textopopup.php
 * 
 * @param string nome
 * @param integer height
 * @param integer width
 * @return void
 */
function texto_popup_abre_janela( nome, height, width, param )
{
	window.open( '../geral/textopopup.php?nome=' + nome + param, nome, "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
}

/**
 * Adiciona um item ao combo popup.
 * 
 * @param string nome_combo
 * @param string codigo
 * @param string descricao
 * @param boolean ordenar
 * @return void
 */
function combo_popup_adicionar_item( nome_combo, codigo, descricao, ordenar )
{
	var campo_select = document.getElementById( nome_combo );
	if ( campo_select.options[0].value == '' )
	{
		campo_select.options[0] = null;
	}
	campo_select.options[campo_select.options.length] = new Option( descricao, codigo, false, false );
	if ( ordenar == true )
	{
		sortSelect( campo_select );
	}
}

/**
 * Remove um item do combo popup.
 * 
 * @param string nome_combo
 * @param string codigo
 * @param boolean ordenar
 * @return void
 */
function combo_popup_remover_item( nome_combo, codigo, ordenar )
{
	var campo_select = document.getElementById( nome_combo );
	for( var i = 0; i <= campo_select.length-1; i++ )
	{
		if ( codigo == campo_select.options[i].value )
		{
			campo_select.options[i] = null;
		}
	}
	if ( campo_select.options.length == 0 )
	{
		campo_select.options[0] = new Option( 'Duplo clique para selecionar da lista', '', false, false );
	}
	if ( ordenar == true )
	{
		sortSelect( campo_select );
	}
}

/**
 * Adiciona uma lista de itens ao combo popup.
 * 
 * @param string nome_combo
 * @param string[][] lista
 * @return void
 */
function combo_popup_adicionar_itens( nome_combo, lista )
{
	var j = lista.length;
	for ( var i =0; i < j; i ++ )
	{
		combo_popup_adicionar_item( nome_combo, lista[i][0], lista[i][1], false );
	}
	var campo_select = document.getElementById( nome_combo );
	sortSelect( campo_select );
}

/**
 * Remove uma lista de itens ao combo popup.
 * 
 * @param string nome_combo
 * @param string[][] lista
 * @return void
 */
function combo_popup_remover_itens( nome_combo, lista )
{
	var j = lista.length;
	for ( var i =0; i < j; i ++ )
	{
		combo_popup_remover_item( nome_combo, lista[i][0], false );
	}
	var campo_select = document.getElementById( nome_combo );
	sortSelect( campo_select );
}

function combo_popup_remove_selecionados( event, nome_combo )
{
	if( window.event ) // IE
	{
		var keynum = event.keyCode
	}
	else if( event.which ) // Netscape/Firefox/Opera
	{
		var keynum = event.which
	}
	if ( keynum != 46 )
	{
		return;
	}
	var campo_select = document.getElementById( nome_combo );
	for( var i = 0; i <= campo_select.length-1; )
	{
		if ( campo_select.options[i].selected )
		{
			combo_popup_remover_item( nome_combo, campo_select.options[i].value, false );
		}
		else
		{
			i++;
		}
	}
	sortSelect( campo_select );
}

/**
 * Seleciona todos os itens do campo select.
 * 
 * @param object campo_select
 * @return void
 */
function selectAllOptions( campo_select )
{
	if ( !campo_select )
	{
		return;
	}
	var j = campo_select.options.length;
	for ( var i = 0; i < j; i++ )
	{
		campo_select.options[i].selected = true;
	}
}


/**
 * Deseleciona todos os itens do campo select.
 * 
 * @param object campo_select
 * @return void
 */
function DeselectAllOptions( campo_select )
{
	if ( !campo_select )
	{
		return;
	}
	var j = campo_select.options.length;
	for ( var i = 0; i < j; i++ )
	{
		campo_select.options[i].selected = false;
	}
}

/**
 * Varre formulário em busca de campos que precisam de tratamento
 * especial. O campo do tipo 'combo_popup' tem seus itens todos
 * selecionados.
 * 
 * @return void
 */
function prepara_formulario()
{
	var quantidade = document.forms.length;
	var quantidade_elementos = 0;
	var elemento = null;
	var j = 0;
	for ( var i = 0; i < quantidade; i++  )
	{
		quantidade_elementos = document.forms[i].elements.length;
		for ( j = 0; j < quantidade_elementos; j++ )
		{
			elemento = document.forms[i].elements[j];
			if ( elemento.getAttribute( 'tipo' ) == 'combo_popup' )
			{
				selectAllOptions( elemento );
			}
		}
	}
}

/**
* Abre popup dos arquivos relacionados ao projeto especial de acordo com a acao (L - listar, I - inserir)
* 
* @param string acao
* @param string nome
* @param integer width
* @param integer height
*/
function popup_arquivo( acao, nome, width, height )
{
	window.open( '../geral/popup_arquivo.php?acao=' + acao + '&nome=' + nome, '', "height=" + height +  ",width=" + width +  ",scrollbars=yes,top=50,left=200" );
}

//tirar os espaços das extremidades do valor passado (utilizada pela mascaraglobal)
function trim(valor){
        for (i=0;i<valor.length;i++){
                if(valor.substr(i,1) != " "){
                        valor = valor.substr(i);
                        break;
                }
                if (i == valor.length-1){
                        valor = "";
                }
        }
        for (i=valor.length-1;i>=0;i--){
                if(valor.substr(i,1) != " "){
                        valor = valor.substr(0,i+1);
                        break;
                }
        }
        return valor;
}


function validaRadio(campo,label) {
   var radiogroup = campo; 
   var itemchecked = false;
   for(var j = 0 ; j < radiogroup.length ; ++j) {
    if(radiogroup[j].checked) {
	 itemchecked = true;
	 break;
	}
   }
   if(!itemchecked) { 
    alert("Escolha uma opção para o Campo: "+label+".");
    if(campo[0].focus)
     campo[0].focus();
	return false;
   }
 return true;
}

function validar_radio( campo,label ) {
	var radiogroup = campo;
	var itemchecked = false;
	for( var j = 0 ; j < radiogroup.length ; ++j ) {
		if( radiogroup[j].checked ) {
			itemchecked = true;
			break;
		}
	}
	return itemchecked;
} 

function validaEmail(Email) {
        var s = new String(Email);

        // { } ( ) < > [ ] | \ /
        if ((s.indexOf("{")>=0) || (s.indexOf("}")>=0) || (s.indexOf("(")>=0) || (s.indexOf(")")>=0) || (s.indexOf("<")>=0) || (s.indexOf(">")>=0) || (s.indexOf("[")>=0) || (s.indexOf("]")>=0) || (s.indexOf("|")>=0) || (s.indexOf("\"")>=0) || (s.indexOf("/")>=0) )
                return false;
        if (vogalAcentuada(Email))
                return false;
        // & * $ % ? ! ^ ~ ` ' "
        if ((s.indexOf("&")>=0) || (s.indexOf("*")>=0) || (s.indexOf("$")>=0) || (s.indexOf("%")>=0) || (s.indexOf("?")>=0) || (s.indexOf("!")>=0) || (s.indexOf("^")>=0) || (s.indexOf("~")>=0) || (s.indexOf("`")>=0) || (s.indexOf("'")>=0) )
                return false;
        // , ; : = #
        if ((s.indexOf(",")>=0) || (s.indexOf(";")>=0) || (s.indexOf(":")>=0) || (s.indexOf("=")>=0) || (s.indexOf("#")>=0) )
                return false;
        // procura se existe apenas um @
        if ( (s.indexOf("@") < 0) || (s.indexOf("@") != s.lastIndexOf("@")) )
                return false;
        // verifica se tem pelo menos um ponto ap\u00f3s o @
        if (s.lastIndexOf(".") < s.indexOf("@"))
                return false;
        // verifica se nao termina com um ponto
        if (s.substr(s.length-1,s.length) == ".")
        	return false;
        return true;
}

//verifica se tem vogais acentuadas
function vogalAcentuada(s) {
        ls = s.toLowerCase();
        if ((ls.indexOf("\u00e1")>=0) || (ls.indexOf("\u00e0")>=0) || (ls.indexOf("\u00e3")>=0) || (ls.indexOf("\u00e2")>=0) || (ls.indexOf("\u00e9")>=0) || (ls.indexOf("\u00ed")>=0) || (ls.indexOf("\u00f3")>=0) || (ls.indexOf("\u00f5")>=0) || (ls.indexOf("\u00f4")>=0) || (ls.indexOf("\u00fa")>=0) || (ls.indexOf("\u00fc")>=0))
                return true;
}


/**
 * Verifica campos não preenchidos.
 * 
 * O campo é dito não preenchido caso somente o caracter ' '. Verifica
 * também o campo do tipo select, neste caso deve haver pelo menos um
 * item selecionado e nenhum deles deve possuir valor vazio.
 * 
 * @param campo
 * @param label
 * @return void
 */
function validaBranco( campo, label ) 
{
	var i = 0;
	var teste_campo = "false"; //variavel para teste de espacos em branco
	// campo instanceof HTMLSelectElement  dá problema no Microsoft Internet Explorer
	if ( campo.tagName == 'SELECT' )
	{
		var tamanho_select = campo.options.length;
		var possui_selecionado = false;
		var possui_vazio = false;
		for ( i = 0; i < tamanho_select; i++ )
		{
			if ( campo.options[i].selected )
			{
				possui_selecionado = true;
				if ( campo.options[i].value == '' )
				{
					possui_vazio = true;
				}
			}
		}
		if ( possui_selecionado && !possui_vazio )
		{
			return true;
		}
		alert( "Campo obrigatório: " + label + "." );
		return false;
	}
	else
	{
		var tamanho_campo = campo.value.length;
		if ( tamanho_campo != 0 )
		{
			for ( i = 0; i < tamanho_campo; i++ )
			{
				if ( campo.value.charAt( i ) != " " )
				{
					teste_campo = "true"; // existe caracter diferente de branco
				}
			}
			if ( teste_campo == "false" ) // todos os caracteres digitados são brancos
			{
				alert( "Campo obrigatório: " + label + "." );
				campo.focus();
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			alert( "Campo obrigatório: " + label + "." );
			campo.focus();
			return false;
		}
	}
}




function janela(pagina,TW,TH,dest) {
        window.open(pagina, dest, "toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=yes"+",width="+TW+",height="+TH);
 }

function FiltraCampo(codigo) {
    var s = "";

        tam = codigo.length;
        for (i = 0; i < tam ; i++) {
                if (codigo.substring(i,i + 1) == "0" ||
                   codigo.substring(i,i + 1) == "1" ||
            codigo.substring(i,i + 1) == "2" ||
            codigo.substring(i,i + 1) == "3" ||
            codigo.substring(i,i + 1) == "4" ||
            codigo.substring(i,i + 1) == "5" ||
            codigo.substring(i,i + 1) == "6" ||
            codigo.substring(i,i + 1) == "7" ||
            codigo.substring(i,i + 1) == "8" ||
            codigo.substring(i,i + 1) == "9"  )
                                 s = s + codigo.substring(i,i + 1);
        }
        return s;
}



function DvCpfOk(e) {
    var dv = false;
    controle = "";
    s = FiltraCampo(e.value);
    tam = s.length;
    if ( tam == 11 ) {
        dv_cpf = s.substring(tam-2,tam);
        for ( i = 0; i < 2; i++ ) {
            soma = 0;
            for ( j = 0; j < 9; j++ )
                soma += s.substring(j,j+1)*(10+i-j);
            if ( i == 1 ) soma += digito * 2;
            digito = (soma * 10) % 11;
            if ( digito == 10 ) digito = 0;
            controle += digito;
        }
        if ( controle == dv_cpf )
            dv = true;
    }
	if ( s == "11111111111" ) dv = false;
	if ( s == "22222222222" ) dv = false;
 	if ( s == "33333333333" ) dv = false;
	if ( s == "44444444444" ) dv = false;
	if ( s == "55555555555" ) dv = false;
	if ( s == "66666666666" ) dv = false;
	if ( s == "77777777777" ) dv = false;
	if ( s == "88888888888" ) dv = false;
	if ( s == "99999999999" ) dv = false;
	if ( s == "00000000000" ) dv = false;
	if ( s == "00000000191" ) dv = false;
	if ( s == "22525837541" ) dv = false;
	if ( s == "23111478203" ) dv = false;
	if ( s == "28075746708" ) dv = false;
	if ( s == "91087662826" ) dv = false;

	if ( ! dv && tam > 0) {
         mensagem = "           Erro de digita\347\343o:\n";
         mensagem+= "          ===============\n\n";
         mensagem+= " O CPF: " + e.value + " n\343o: existe!\n";
         //mensagem+= " O DV: " + controle + "\n";
         alert(mensagem);
     }
    return dv;
}

function validaData(dataform)
{	//Funcionalidade:	Valida a Data retornando True se for uma Data
        //					válida e False se não for.
        //					Antes de se usar esta função deve-se garantir que os parâmetros
        //					passados sejam numéricos e inteiros.
        // PARÂMETROS:
        //		Dia = Dia da Data(caracteres numericos),
        //		Mes = Mes da Data(caracteres numericos),
        //		Ano = Ano da Data(caracteres numericos)

        //alert(dia +"/"+ mes +"/"+ ano);
		var dia = dataform.value.substring(0,2);
		var mes = dataform.value.substring(3,5);
		var ano = dataform.value.substring(6,10);
        var v_dia;
        var v_mes;
        var v_ano;

        if (!validaInteiro(dia)){
                return (false);
        }
        if (!validaInteiro(mes)){
                return (false);
        }
        if (!validaInteiro(ano)){
                return (false);
        }

        v_dia = dia;
        v_mes = mes;
        v_ano = ano;

        if (v_dia.length < 2)
        {
                return(false);
        }

        if (v_mes.length < 2)
        {
                return(false);
        }

        if (v_ano.length < 4)
        {
                return(false);
        }

        if (((v_ano < 1900) || (v_ano > 2079)) && (v_ano.length != 0))
        {
                return(false);
        }

        if (v_dia > 31 || v_dia < 1)
        {
                return(false);
        }

        if (v_mes > 12 || v_mes < 1)
        {
                return(false);
        }

        if (v_dia == "31")
        {
                if ((v_mes == "04") || (v_mes == "06") || (v_mes == "09") || (v_mes == "11"))
                {
                        return(false);
                }
        }

        //Validação de Ano Bissexto
        if (v_mes == "02")
        {
                if (!(v_ano%4))
                {
                        if (v_dia > 29)
                        {
                                return(false);
                        }
                }
                else if (v_dia > 28)
                {
                        return(false);
                }
        }

        //o -if- abaixo testa se algum campo foi preenchido e outro deixado em branco deixando a data incompleta

        if (((v_dia != "") || (v_mes != "") || (v_ano != "")) && ((v_dia == "") || (v_mes == "") || (v_ano == "")))
        {
                return(false);
        }

        return(true);
}



function validaInteiro(parametro)  //FUNCAO PARA VALIDACAO DE NÚMEROS INTEIROS, E ESPAÇOS EM BRANCO
{
        if (parametro.length != 0)
        {

                teste_ponto = "false";
                tamanho_parametro = parametro.length;

                if (isNaN(parametro)) //valor digitado não é numérico
                {
                        return false;
                }
                else //valor digitado é um numérico válido
                {

                        for (k = 0; k < tamanho_parametro; k++)
                        {if ((parametro.charAt(k) == '.') || (parametro.charAt(k) == '-') || (parametro.charAt(k) == '+'))
                                {
                                        teste_ponto = "true"; /*existe caracter ponto*/
                                }
                        }

                        if (teste_ponto == "true") //encontrou caracter ponto(numero real)
                        {
                                return false;
                        }
                        else
                        {
                                return true;
                        }
                }
        }
        else
        {
                return true;
        }

}

// Retorna true se a dataIni maior que a dataFin
function validaDataMaior(dataIni, dataFin)  {

	if ((dataIni.length != 0 && dataIni.length < 10) || (dataFin.length != 0 && dataFin.length < 10))
		return(true);
	else
		if ((dataIni.length != 0 && !validaData(dataIni)) || (dataFin.length != 0 && !validaData(dataFin)))
			return(true);

	dataIni = dataIni.value.substr(6,4) + dataIni.value.substr(3,2) + dataIni.value.substr(0,2);
	dataFin = dataFin.value.substr(6,4) + dataFin.value.substr(3,2) + dataFin.value.substr(0,2);

	if (dataIni <= dataFin)
		return(true);

	return(false);
}

function validaDataMaiorQueHoje(data)  {

		hoje = new Date();
        dia = hoje.getDate();        
        if (dia < 10) dia = "0"+dia;
        
        mes = hoje.getMonth()+1;
        
        if (mes < 10) mes = "0"+mes;

        ano=hoje.getFullYear();
        
        diadehoje = ano + '' + mes +''+ dia;
        // alert (diadehoje)  ;    
	data = data.value.substr(6,4) + data.value.substr(3,2) + data.value.substr(0,2);
	//alert(data);
	//alert(data + '\n'+diadehoje);
	if (data >= diadehoje)
		return(true);
	else return(false);
}

/**
 * substitui todas as ocorrencias de um string por outro
 * replace all the ocurrence of some string to another
 *
 *@param  string strText
 *@param  string strFinder
 *@param  string strReplacer
 *@return bool
 */
function replaceAll (strText , strFinder, strReplacer)
{
	strText += "";
	var strSpecials = /(\.|\*|\^|\?|\&|\$|\+|\-|\#|\!|\(|\)|\[|\]|\{|\}|\|)/gi; // :D
	strFinder = strFinder.replace(strSpecials, "\\$1")

	var objRe = new RegExp(strFinder, "gi");
	return strText.replace(objRe, strReplacer);
}

//Função para limitar o tamanho do Textarea 
function textCounter(field, countfield, maxlimit) {
if (field.value.length > maxlimit)
field.value = field.value.substring(0, maxlimit);
else 
countfield.value = maxlimit - field.value.length;
}

// Inicio funções para preencher conteúdos dinamicamente (sem refresh)
var xmlhttp
var idobjeto

abreconteudo = function (url, idobj) {
	var jsNode = document.getElementById('td'+idobj);
	if (document.getElementById('img'+idobj).name=='+')
	{
		try
		{
		strContent = document.getElementById('td'+idobj).innerHTML;
		}
		catch( e )
		{
			throw new Error( 'objeto nao encontrado ' + 'td' + idobj );
		}
		strContent = replaceAll( strContent , "\t" , " ");
		strContent = replaceAll( strContent , String.fromCharCode( 10 ) , " ");
		strContent = trim( strContent );

		if ( strContent == '' || strContent == ' ' ) {
			
		  	document.getElementById('td'+idobj).style.visibility = "visible";
		  	document.getElementById('td'+idobj).style.display  = "";
			document.getElementById('td'+idobj).innerHTML='<div align=center><img src="../imagens/wait.gif" border="0" align="middle"> <font color=blue>Carregando Dados...</font></div>'

			jsNode._xmlHttp = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject("Microsoft.XmlHttp");
			jsNode._xmlHttp.open("GET", url, true);	
			jsNode._xmlHttp.onreadystatechange = new Function("func_onload(\"" + idobj + "\")");

			// call in new thread to allow ui to update
			window.setTimeout("func_ontimeout(\"" + idobj + "\")", 300);

			document.getElementById('img'+idobj).name='-';
			document.getElementById('img'+idobj).src = document.getElementById('img'+idobj).src.replace('mais.gif', 'menos.gif');
		} else {
			document.getElementById('img'+idobj).name='-';
			document.getElementById('img'+idobj).src = document.getElementById('img'+idobj).src.replace('mais.gif', 'menos.gif');
			document.getElementById('td'+idobj).style.visibility = "visible";
			document.getElementById('td'+idobj).style.display  = "";
		}
	}
	else
	{
		document.getElementById('img'+idobj).name='+';
		document.getElementById('img'+idobj).src = document.getElementById('img'+idobj).src.replace('menos.gif', 'mais.gif');
		document.getElementById('td'+idobj).style.visibility = "hidden";
		document.getElementById('td'+idobj).style.display = "none";
	}


};

func_ontimeout = function (idobj) {var jsNode = document.getElementById('td'+idobj);
									contLoadQueue.add(jsNode);};

func_onload = function (idobj) {
	var jsNode = document.getElementById('td'+idobj);

	if (jsNode._xmlHttp.readyState == 4) {

		if (jsNode._xmlHttp.status==200)
		{
			jsNode.innerHTML='';
			var resp = jsNode._xmlHttp.responseText;
			var jsCode = '';
			if(resp.indexOf('<JSCode>')!=-1) {
				jsCode = resp.substring( ( resp.indexOf('<JSCode>') + 8 ), resp.indexOf('</JSCode>') );
				resp = resp.replace(/<JSCode>(?:\n|\r|.)*?<\/JSCode>/gm, "");
			}
			jsNode.innerHTML=resp;
			if (jsCode != '') {
				try	{
					eval( jsCode );
				}
				catch( e ) {
					alert( e );
				}
			}
		}
		else
		{
			alert("Erro! Não foi possível Recuperar os Dados!");
			document.getElementById('td'+idobjeto).innerHTML='';
		}

		if (jsNode._xmlHttp.dispose)
			jsNode._xmlHttp.dispose();
		jsNode._xmlHttp = null;
	}
};

var contLoadQueue = {add: function (jsNode) {jsNode._xmlHttp.send(null);}}


// Fim funções para preencher conteúdos dinamicamente (sem refresh)

/////////////////////////////
//Eventos
//Funções para controlar eventos de campos de formulário
////////////////////////////
function MouseOver(objeto)
{
	if (objeto.type == "text" || objeto.type == "password"){
		if(objeto.className != 'clsMouseFocus'){
				objeto.className = 'clsMouseOver';
		}

	}else if(objeto.type == "textarea"){
		if(objeto.className != 'txareaclsMouseFocus'){
				objeto.className = 'txareaclsMouseOver';
		}
	}
	return true;
}

function MouseOut(objeto)
{
	if (objeto.type == "text" || objeto.type == "password")
	{
		if(objeto.className != 'clsMouseFocus'){
					objeto.className = 'normal';
			}
	
	}else if(objeto.type == "textarea"){
		if(objeto.className != 'txareaclsMouseFocus'){
				objeto.className = 'txareanormal';
		}
	}
	return true;
}


function MouseClick(objeto){
	if (objeto.type == "text" || objeto.type == "password"){
		objeto.className = 'clsMouseFocus';	
	}else if(objeto.type == "textarea"){
		objeto.className = 'txareaclsMouseFocus';
	}
}


function MouseBlur( objeto )
{
	if ( objeto.type == "text" || objeto.type == "textarea" || objeto.type == "password" )
	{
		if ( objeto.type == "textarea")
		{
			objeto.className = 'txareanormal';
		}
		else
		{
			objeto.className = 'normal';
		}
	}
}

function sortSelect(obj) {
		var o = new Array();
		if (obj.options==null) { return; }
		for (var i=0; i<obj.options.length; i++) {
			o[o.length] = new Option( obj.options[i].text, obj.options[i].value, obj.options[i].defaultSelected, obj.options[i].selected) ;
			}
		if (o.length==0) { return; }
		o = o.sort( 
			function(a,b) { 
				if ((a.text+"") < (b.text+"")) { return -1; }
				if ((a.text+"") > (b.text+"")) { return 1; }
				return 0;
				} 
			);
	
		for (var i=0; i<o.length; i++) {
			obj.options[i] = new Option(o[i].text, o[i].value, o[i].defaultSelected, o[i].selected);
			}
}






/* ----------------------------------------------------------------------------
---------------------------------------------------------------------------- */
function Right(pcString,pnTamanho)
{
	var lcRight
	lcRigth = pcString.substr(pcString.length - pnTamanho, pnTamanho)
	return lcRigth
}

/* ----------------------------------------------------------------------------
---------------------------------------------------------------------------- */
function Left(pcString,pnTamanho)
{
	var lcRigth
	lcRigth = pcString.substr(0, pnTamanho)
	return lcRigth
}

/* ----------------------------------------------------------------------------
---------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
---------------------------------------------------------------------------- */
// máscara para valores monetários. Usar passando como paramentro uma string ele retorna 
// um string com a formatação padrão de valores com duas casa decimais.

function MascaraMonetario( objeto )
{
	var j = 1;
	var valorObjeto = objeto.split(".");
	var lcDecimal;
	
	
	if(valorObjeto.length > 1) //Possui parte decimal
	{	
		lcDecimal = valorObjeto[1];
		lcDecimal = Left(lcDecimal + "00",2);
		lcDecimal = Right("00" + lcDecimal,2);
	}
	else 
	{
		lcDecimal = "00";
	}
	
	var obj = valorObjeto[0] + lcDecimal;

	if ( obj.length > 2 ) obj = obj.substr(0,obj.length - 2) + "," + obj.substr(obj.length - 2,2);
	if ( obj.length > 6 ) 
		 for (i = obj.length - 1; i >= 0; i--) {
		 	j = obj.length - i;
		 	if (j > 3 && (j + 1)%4 == 0) obj = obj.substr(0,i + 1) + "." + obj.substr(i + 1,obj.length - 1);
		 }
	return obj;

}

/////////////////////////////
//Fim Eventos
//////////////////////////////



//////////////////////////////
// Prototypes
//////////////////////////////

/**
  *	Método adicionado ao objeto Array do javascript para ordenação de array de objetos
  *
  * @param string campo
  * @param string sentido
  * @param integer isNumeric
  * @return void
  */
  Array.prototype.sortObject = function( campo, sentido, isNumeric )
  {
  	var cmpString = sentido != 'dsc' ? 'jCampo > iCampo' : 'jCampo < iCampo';
	for( var i = 0 ; i < this.length ; i++ )
	{
		for( var j = 0 ; j < this.length ; j++ )
		{
			jCampo = isNumeric ? parseFloat( this[ j ][ campo ] ) : this[ j ][ campo ];
			iCampo = isNumeric ? parseFloat( this[ i ][ campo ] ) : this[ i ][ campo ];
			if( eval( cmpString ) )
			{
				temp = this[ i ];
				this[ i ] = this[ j ];
				this[ j ] = temp;
			}
		}
	}
  }
  /**
  * Método adicionado ao objeto Array do javascript para conversão de um array de objetos em string
  * Se campo for vazio então ele gerará um array contendo todos os campos do objeto no registro
  * separador de campo padrão '#'
  * separador de registro padrão ','
  *
  * @param string campo
  * @param string separadorReg
  * @param string separadorCampo
  * @return string
  */
  Array.prototype.objectToString = function( campo, separadorReg, separadorCampo )
  {
  	var string = '';
  	sepReg = separadorReg ? separadorReg : ',';
  	sepCampo = separadorCampo ? separadorCampo : '#';
	for( var i = 0 ; i < this.length ; i++ )
	{
		if( campo )
		{
			string += this[ i ][ campo ];
		}
		else
		{
			for( var j in this[ i ] )
			{
				string += this[ i ][ j ] + sepCampo;
			}
			string = string.substr( 0, string.length - 1 );//Eliminar o último separador de campo
		}
		string += sepReg;
	}
	string = string.substr( 0, string.length - 1 );//Elimina o último separador de registro
  	return string;
  }

function validar_cpf( cpf ){
    var dv = false;
    controle = "";
    s = FiltraCampo( cpf );
    tam = s.length;
    if ( tam == 11 ) {
        dv_cpf = s.substring( tam-2, tam );
        for ( i = 0; i < 2; i++ ) {
            soma = 0;
            for ( j = 0; j < 9; j++ ) {
                soma += s.substring( j, j + 1 ) * ( 10 + i - j );
            }
            if ( i == 1 ) {
            	soma += digito * 2;
            }
            digito = ( soma * 10 ) % 11;
            if ( digito == 10 ) {
            	digito = 0;
            }
            controle += digito;
        }
        if ( controle == dv_cpf ) {
            dv = true;
        }
    }
	if ( s == "11111111111" ) dv = false;
	if ( s == "22222222222" ) dv = false;
 	if ( s == "33333333333" ) dv = false;
	if ( s == "44444444444" ) dv = false;
	if ( s == "55555555555" ) dv = false;
	if ( s == "66666666666" ) dv = false;
	if ( s == "77777777777" ) dv = false;
	if ( s == "88888888888" ) dv = false;
	if ( s == "99999999999" ) dv = false;
	if ( s == "00000000000" ) dv = false;
	if ( s == "12345678909" ) dv = false;
	if ( s == "00000000191" ) dv = false;
	if ( s == "22525837541" ) dv = false;
	if ( s == "23111478203" ) dv = false;
	if ( s == "28075746708" ) dv = false;
	if ( s == "91087662826" ) dv = false;
	return dv;
}