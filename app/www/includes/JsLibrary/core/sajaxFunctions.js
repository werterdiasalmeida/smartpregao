///////////////////////////////////////////////////////////////
/**
* Constantes relacionadas ? configura??o da requisi??o
*
* Requisition Config Constants
*//////////////////////////////////////////////////////////////
    /**
    *
    * Metodo que sera utilizado na requisicao pelo xml http request
    *
    * Method what will be used in the xml http request
    */
	var XML_LOAD_TYPE = "POST";
	
/////////////////////////////////////////////////////////////
/**
 * Constantes das pastas
 * 
 * Folter constants
*//////////////////////////////////////////////////////////////
	/**
	* Pasta da Acao do Xml Request
	*
	* Xml Request Action Folder
	*/
	var FOLDER_ACTION_XMLREQUEST = "../../action/xmlrequest"
	
	/**
	* Nome da A????o do Xml Request
	*
	* Xml Request Action Name
	*/
	var ACTION_NAME_XMLREQUEST = "receiveXmlRequest.php"

/////////////////////////////////////////////////////////////
/**
* Constantes do acesso ao servidor
* 
* Server Acess Constants
*//////////////////////////////////////////////////////////////
	/**
	* Nome da fun??o que est? aguardando a chamada Xml Http Request
	*
	* Function Name how is waiting the Xml Http Request 
	*/
	var SERVER_FUNCTION_NAME = "call_class_action"
	

/**
 * Global Vertical Absolute Position of The Table
 * @date 	2005-12-26
 */
arrGlobalStackXmlRequest = Array();

/**
 * Global Singletoon XmlDoc Controler
 * @date 	2005-12-26
 */
globalBoolOnLoadXMLDoc = false;

/**
 * Fun??o que gera o objeto Xml Http Request conforme o sistema operacional do usu?rio
 *
 * @Author		CAIXA
 * @Date			01/11/2005
 * @Version		1.0
 * @package		Xml Request
 * @subpackage	Javascrit Xml Request
 * @return object XMLHttRequest
 */
function xmlRequest()
{
	// code for Mozilla, etc.
	if ( window.XMLHttpRequest )
	{
		xmlHttp = new XMLHttpRequest()
		return xmlHttp;
	}
	// code for IE
	else if ( window.ActiveXObject )
	{
		xmlHttp = new ActiveXObject( "Microsoft.XMLHTTP" )
		if ( xmlHttp )
		{
		return xmlHttp;
		}
	}
	return null;
}

function simpleLoadXmlDoc( strUrl, funcJavascriptAfterLoad, objectJavascriptAfterdo )
{
	loadXMLDoc( strUrl, "", Array( ), funcJavascriptAfterLoad, objectJavascriptAfterdo );
}


/**
 * Fun??o que efetua a requisi??o para o servidor, aguarda o xml de retorno e chama a funcao indicada 
 * ap?s o retorno enviando os dados adquiridos.
 * Os parametros a serem enviados e a ordem dos mesmo est?o de acordo com o esperado pelo SAJAX.
 *
 * @Author		CAIXA
 * @Date			01/11/2005
 * @Version		1.0
 * @package		Xml Request
 * @subpackage	Javascrit Xml Request
 * @return object XMLHttRequest
 */
function loadXMLDoc( strUrl, strFunctionServerName, arrServerFunctionArguments, funcJavascriptAfterLoad, objectJavascriptAfterdo, arrParameters )
{
    var xmlResult;
    var txtResult;
    
	var objectDo;
	if (objectJavascriptAfterdo)
		objectDo = objectJavascriptAfterdo;
		
	funcLoading = function() 
	{ 
		if( xmlHttp && xmlHttp.readyState == 4 )
		{
			// loaded //
			if ( xmlHttp.status == 200 || xmlHttp.status == 0)
			{
				// case OK
				try
				{
					xmlResult = xmlHttp.responseXML;
                }
                catch( e )
                {
					print( e.message );
                	xmlResult = "" ;
                }
				try
				{
					txtResult = xmlHttp.responseText;
               }
                catch( e )
                {
					print( e.message );
                	txtResult = "";
                }
                
				if( window.last == undefined ) window.last = Array();
				
				window.last.http = xmlHttp;
				window.last.txtResult = txtResult;
				window.last.xmlResult = xmlResult;
				window.last.funcJavascriptAfterLoad = funcJavascriptAfterLoad;
				
				if ( funcJavascriptAfterLoad == undefined )
				{
					strFuncJavascriptAfterLoad = "afterLoad" + strUrl.substring( 0, strUrl.indexOf( "." ) );
					strCommand = (strFuncJavascriptAfterLoad + "(txtResult,xmlResult,arrParameters);" );
					
					eval(strCommand);
					
					loopLoadXMLDoc();
				}
				else
				{               
					if (objectDo)
					{
						if ( !isFunction( objectDo[ funcJavascriptAfterLoad ] ) )
						{
							throw new Error( "Error after receive the xmlHttpResponse" +
					   			 " unknow method " + funcJavascriptAfterLoad + " in " + objectDo ) ;
						}
						
						objectDo[ funcJavascriptAfterLoad ]( txtResult, xmlResult, arrParameters );
						loopLoadXMLDoc();
					}
	                else if ( !isFunction(funcJavascriptAfterLoad) )
	                {
						throw new Error( "Error after receive the xmlHttpResponse" +
				   			 " unknow function " + funcJavascriptAfterLoad + " in " + objectDo ) ;
				   		
					    loopLoadXMLDoc();
					    return false;
	                }
					else
					{
						try
						{
           					funcJavascriptAfterLoad( txtResult, xmlResult, arrParameters );
           					loopLoadXMLDoc();
           				}
         				catch( e )
           				{
							print( e.message );
           				}
   	            		
                	}
				}
			}
			else
			{
				// case error
				throw new Error( "Problem retrieving XML data " + strUrl );
				loopLoadXMLDoc();
				return true;
			}
		}
		else
		{
			// loading //
		}
	};
	
	
	// 1. MAKING THE COMMAND LINE TO SEND THE ARGUMENTS TO THE SERVER FUNCTION
	
	strUrlParameters = "";
	if( strFunctionServerName == '' )
	{
		strFunctionServerName = 'none';
	}
	
	if ( XML_LOAD_TYPE == "GET" || 1 ) 
	{
		if ( strUrlParameters.indexOf( "?" ) == -1 ) 
		{
			strUrlParameters = strUrlParameters + "?rs=" + escape( strFunctionServerName );
		}
		else
		{
			strUrlParameters = strUrlParameters + "&rs=" + escape( strFunctionServerName );
		}
			
		for ( var i = 0; i < arrServerFunctionArguments.length; ++i ) 
		{
			strUrlParameters = strUrlParameters + "&rsargs[" + i + "]=" + escape( arrServerFunctionArguments[ i ] );
		}
			
		strUrlParameters = strUrlParameters + "&rsrnd=" + new Date( ).getTime( );
		
		strPostData = null;
	} 
//	else 
	{
		strPostData = "rs=" + escape( strFunctionServerName );
		for ( var i = 0; i < arrServerFunctionArguments.length; i++ ) 
		{
			strPostData = strPostData + "&rsargs[]=" + escape( arrServerFunctionArguments[ i ] );
		}
	}
	
	// 2. URL OF THE ACTION //
	
	if ( ( strUrl == undefined ) || ( strUrl == "" ) )
	{
		strUrl = window.location;
	}
	
	// N. CREATING THE XML HTTP REQUEST

	if( window.last == undefined ) window.last = Array();
	window.last.url = strUrl + strUrlParameters;
	window.last.args = arrServerFunctionArguments;

	var objXmlRequest = xmlRequest( );
	xmlHttp.open( XML_LOAD_TYPE , strUrl , true )
	
	if ( XML_LOAD_TYPE == "POST" ) 
	{
		objXmlRequest.setRequestHeader( "Method", "POST " + strUrl + strUrlParameters + " HTTP/1.1" );
		objXmlRequest.setRequestHeader( "Content-Type", "application/x-www-form-urlencoded" );
	}
	else
	{
		objXmlRequest.setRequestHeader( "Method", "GET " + strUrl + strUrlParameters + " HTTP/1.1" );
		objXmlRequest.setRequestHeader( "Content-Type", "text/html" );
	}
	xmlHttp.onreadystatechange = funcLoading;
	xmlHttp.send( strPostData );	
}

/**
 * Fun??o que converte os campos n?o triviais (tais como objetos e array) 
 * do array de parametros a serem enviados em objetos xml e prepara a chamada
 * da fun??o call_class_action no servidor.
 *
 * @author		CAIXA
 * @date		01/11/2005
 * @version		1.0
 * @param       strClass 		Nome da Classe
 * @param       strAction		Nome da Action
 * @param		arrParameters 	Parametros Enviados ao Servidor
 * @param		funcAfterDo		Fun??o ou string do Nome Metodo que ser? chamado ap?s a requisi??o
 * @param		objectAfterDo	Objeto que cont?m o m?todo ( caso o par?metro anterior n?o seja uma fun??o )
 * @package		Xml Request
 * @subpackage	Javascrit Xml Request
 * @example 	do_call_class_action( "actionFolder", "actionName",  Array( 'param1', 'param2' ) , "afterSendedObjectMethod", objSended );
 * @example 	do_call_class_action( "actionFolder", "actionName",  Array( 'param1', 'param2' ) , funcAfterSended );
 * @return 		void
 */
function do_call_class_action( strClass, strAction, arrParameters, funcAfterDo, objectAfterDo )
{
	var arrParam = Array();
	arrParam[ arrParam.length ] = strClass;
	arrParam[ arrParam.length ] = strAction;
	for ( var i = 0 ; i < arrParameters.length ; ++i )
	{
		if ( isObject( arrParameters[ i ] ) )
		{
			if (!arrParameters[ i ].asXml )
			{
				var objXml = varDump( arrParameters[ i ] );
				arrParam[ arrParam.length ] = objXml.asXml();			
			}
			else
			{
				var objXml = arrParameters[ i ];
				arrParam[ arrParam.length ] = objXml.asXml();
			}
		}
		else
		{
			arrParam[ arrParam.length ] = arrParameters[ i ];
		}
	}
	
	var strUrl = FOLDER_ACTION_XMLREQUEST + "/" + ACTION_NAME_XMLREQUEST;
	
	var objRequest = Array();
	objRequest[0] = strUrl;
	objRequest[1] = SERVER_FUNCTION_NAME;		
	objRequest[2] = arrParam;
	objRequest[3] = funcAfterDo;
	objRequest[4] = objectAfterDo;
	objRequest[5] = arrParameters;
	
	arrGlobalStackXmlRequest[ arrGlobalStackXmlRequest.length ] = ( objRequest );
	singletonLoadXMLDoc();
	//loadXMLDoc( strUrl	 ,  SERVER_FUNCTION_NAME , arrParam, funcAfterDo, objectAfterDo, arrParameters );
	
}

function addRequest( strUrl , strFunctionServerName , arrParam , functionAfterLoad, 
	objectAfterLoad, arrParamatersAfterLoadFunction )
{
	var objRequest = Array();
	objRequest[0] = strUrl;
	objRequest[1] = strFunctionServerName;		
	objRequest[2] = arrParam;
	objRequest[3] = functionAfterLoad;
	objRequest[4] = objectAfterLoad;
	objRequest[5] = arrParamatersAfterLoadFunction;
	
	arrGlobalStackXmlRequest[ arrGlobalStackXmlRequest.length ] = ( objRequest );
	singletonLoadXMLDoc();
	
}

function singletonLoadXMLDoc()
{
	if ( globalBoolOnLoadXMLDoc == true )
	{
		return false;
	}
	globalBoolOnLoadXMLDoc = true;
	loopLoadXMLDoc();
	return true;
}

/**
 *
 @date 2006-01-17
 */
function loopLoadXMLDoc()
{
	if ( arrGlobalStackXmlRequest.length == 0 )
	{
		globalBoolOnLoadXMLDoc = false;
		AJAX_LOADED = true;
		return false;
	}
	
	AJAX_LOADED = false;
	var objRequest = arrGlobalStackXmlRequest.pop();
	loadXMLDoc( objRequest[0], objRequest[1] , objRequest[2], objRequest[3], objRequest[4], objRequest[5] );
	return false;
}
