
/**
 * Exibe ou oculta um elemento HTML.
 * 
 * It show or hide an HTML element.
 * 
 * Caso o elemento esteja visível o oculta, e caso contrário o exibi. O nome
 * _TRACE_FUNCTION_ é utilizado para que a classe responsável por gerar o HTML
 * de detalhamento crie uma função javascript por rastramento. O que garante a
 * existência e a não redeclaração dessa função não importando quantas vezes o
 * detalhamento é acionado e impresso.
 * 
 * @param string identificador do item a ser exibido ou ocultado
 * @return void
 */
function _TRACE_FUNCTION_( sId )
{
	var oDiv = document.getElementById( sId );
	var sDisplay = '';
	if ( oDiv.currentStyle )
	{
		sDisplay = oDiv.currentStyle['display'];
	}
	else if ( window.getComputedStyle )
	{
		sDisplay = document.defaultView.getComputedStyle( oDiv, null ).getPropertyValue( 'display' );
	}
	oDiv.style.display = sDisplay == 'none' ? 'block' : 'none';
}
