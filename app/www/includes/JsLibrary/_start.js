/**
 * path from the html until the javascript library
 * 
 * caminho do html até a biblioteca javascript
 */

//PATH_JS_LIBRARY = '../../javascript/'
//PATH_IMAGES		= '../../img/';

PATH_ROOT_NODE				= '../';
//PATH_ROOT_NODE				= 'http://simec.mec.gov.br/';
PATH_JS_LIBRARY			 	= PATH_ROOT_NODE + 'includes/JsLibrary/';
PATH_SERVER_SIDE_LIBRARY 	= PATH_ROOT_NODE + './';
PATH_IMAGES					= PATH_ROOT_NODE + 'includes/JsLibrary/img/';

function writeJsScript( strScript )
{
	document.write
		( 
		'<script language="javascript" src="'+ PATH_JS_LIBRARY + strScript + '" >' +
		'</script>' + "\n" 
		);
}

writeJsScript( 'core/_start.js' );
