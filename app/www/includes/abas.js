
var abas = new Array();

var aba_selecionada = '';

function aba_iniciar( id, itens, id_aba_aberta )
{
	var janela = document.getElementById( id );
	if ( !janela )
	{
		return;
	}
	abas[id] = new Array();
	var div = null;
	var div_menu = null;
	var html_menu = '<ul id="listaAbas">';
	var divs = janela.getElementsByTagName( 'div' );
	var tamanho_div = divs.length;
	var tamanho_itens = itens.length;
	var indice_itens = 0;
	for ( var indice_div = 0; indice_div < tamanho_div; indice_div++ )
	{
		div = divs[indice_div];
		if ( div.parentNode != janela )
		{
			continue;
		}
		if ( div.id == 'menu' )
		{
			div_menu = div;
			continue;
		}
		if ( indice_itens >= tamanho_itens )
		{
			break;
		}
		html_menu += '<li id="menu_' + div.id + '" class="abaItemMenu">';
		html_menu += '<a href="javascript:aba_mostrar( \'' + div.id + '\', abas[\'' + id + '\'] );" >' + itens[indice_itens] + '</a>';
		html_menu += '</li>';
		abas[id][abas[id].length] = div.id;
		indice_itens++;
	}
	html_menu += '</ul>';
	if ( div_menu )
	{
		div_menu.className = 'abaMenu';
		div_menu.style.display = 'block';
		div_menu.innerHTML += html_menu;
		if ( id_aba_aberta == '' )
		{
			aba_mostrar( abas[id][0], abas[id] );
		}
		else
		{
			aba_mostrar( id_aba_aberta, abas[id] );
		}
	}
}

/**
 * 
 * 
 * @param string
 * @param string[]
 * @return void
 */
function aba_mostrar( id_aba, id_abas )
{
	var j = id_abas.length;
	
	for ( var i = 0; i < j; i++ )
	{
		if ( id_aba != id_abas[i] )
		{
			document.getElementById( 'menu_' + id_abas[i] ).style.background = '#dcdcdc';
			document.getElementById( id_abas[i] ).style.display = 'none';
		}
	}
	document.getElementById( id_aba ).style.display = 'block';
	document.getElementById( 'menu_' + id_aba ).style.background = '#e9e9e9';
	aba_selecionada = id_aba;
}
