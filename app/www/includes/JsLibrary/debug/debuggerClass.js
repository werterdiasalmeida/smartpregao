window.debuggerClass = function debuggerClass( objElement , boolShowAll , arrNotDebugThisFunctions )
	{
		this.constructor = function constructor( objElement , boolShowAll )
		{
			if( boolShowAll == undefined ) boolShowAll = false;
			if( arrNotDebugThisFunctions == undefined ) arrNotDebugThisFunctions = Array();
			for( var intKey in objElement )
			{
				objFunction = objElement[ intKey ];
				// caso seja uma funcao //
				if( isFunction( objFunction ) )
				{
				 	// convertendo a funcao em string					//
				 	strFunction = objFunction + ' ';
				 	// pegando o cabecalho da funcao					//
				 	strFunctionHeader = this.getFunctionHeader( strFunction );
				 	// pegando o conteudo da funcao						//
				 	strInside = this.getFunctionInside( strFunction );
				 	
					// caso nao seja uma funcao nativa					//
					if( strInside.indexOf( '[native code]' ) == -1 )
					{
					 	// pegando o nome da funcao										//
					 	strFunctionName = this.getFunctionName( strFunctionHeader );
						// caso nao seja uma funcao do proprio debugger					//
						switch( strFunctionName )
						{
							case 'debuggerEnterFunction':
							case 'debuggerOutFunction':
							case 'debuggerAlert':
							case 'debuggerFunction':
							case 'string_reverse':
							{
								// nao devem ter o debugger adicionado //
								break;
							}
							default:
							{
								if( array_search( strFunctionName , arrNotDebugThisFunctions ) == -1 )
								{
								 	// montando a nova funcao										//
								 	strNewFunction = this.createNewFunction( strFunctionHeader , strFunctionName, strInside );
								 	// substituindo a funcao pela nova								//
									
									try
									{
										objElement[ intKey ] = ( eval( 'temp = ' + strNewFunction  ) );
									}
									catch( e )
									{
										throw Error( 'Nao foi possivel gerar o debug do metodo ' + strFunctionName + "\n" +
												e.message );	
									 	this.showFunctionCode( 'depois' , strNewFunction );
										return false;
									}
									
								 	if( boolShowAll )
								 	{
								 		this.showFunctionCode( intKey , objElement[ intKey] );
								 	}
									break;
								}
							}
						}
					}
				}
			}
		}
		
		this.getFunctionHeader = function getFunctionHeader( strFunction )
		{
			return strFunction.substr( 0 , strFunction.indexOf( '{' ) );
		}
		
		this.getFunctionInside = function getFunctionInside( strFunction )
		{
		 	var strReverse;
		 	var intBeginPos;
		 	var intEndPos;
		 	var strInside;
		 	
			// gerando o texto invertido para buscas no final	//
			strReverse = string_reverse( strFunction );
			// pegando a posicao do {							//
			intBeginPos = strFunction.indexOf( '{' );
			// procurando a posicao do } no string invertido	//
			intEndReversePos = strReverse.indexOf( '}' );
			// pegando a posicao do { 							//
			intEndPos = strReverse.length - intEndReversePos;
			// removendo o {									//
			++intBeginPos; 
			 // removendo a diferenca para pegar o substring	//
			intEndPos -= intBeginPos;
			// removendo o }									//
			--intEndPos;
			// pega a substring e removendo os espacos brancos	//
			strInside = trim( strFunction.substr( intBeginPos, intEndPos ) );  
			return strInside;
		}
		
		this.getFunctionName = function getFunctionName( strFunctionHeader )
		{
		 	var intBeginPos;
		 	var intEndPos;
		 	
			// strFunctionHeader = 'function functionName()'				//
			// pegando a posicao no comeco do nome da funcao pelo espaco	//
			intBeginPos = strFunctionHeader.indexOf( ' ' );
			// pegando a posicao de termino do nome da funcao pelo (		//
			intEndPos = strFunctionHeader.indexOf( '(' );
			// estraindo o nome da funcao									//
			strFunctionName = trim( strFunctionHeader.substr( intBeginPos , intEndPos - intBeginPos ) );
			return strFunctionName;
		}
		
		this.createNewFunction = function createNewFunction( strFunctionHeader , strFunctionName, strInside )
		{
			strInside = replaceAll( strInside , 'return' , 'debuggerOutFunction( "' + strFunctionName + '" ); ' + "\n" + 'return' );
		 	strNewFunction = strFunctionHeader +
		 	'{' + "\n" +
		 	'	window.debuggerClass.debuggerEnterFunction( "' + strFunctionName + '" ); ' + "\n" +
		 	'	' + strInside + "\n" +
		 	'	window.debuggerClass.debuggerOutFunction( "' + strFunctionName + '" ); ' + "\n" +
		 	'}' + "\n" +
		 	'';
			return strNewFunction;
		}
		
		this.showFunctionCode = function showFunctionCode( strKeyName , strFunction )
		{
			strFunction = replaceAll( strFunction , '<' 		, 'OPENTAG' );
			strFunction = replaceAll( strFunction , '>' 		, 'CLOSETAG' );
			strFunction = replaceAll( strFunction , 'OPENTAG' 	, '<font color="red">&lt;</font>');
			strFunction = replaceAll( strFunction , 'CLOSETAG' 	, '<font color="red">&gt;</font>');
			strFunction = replaceWordColor( strFunction , 'function' 				, '1111AA');
			strFunction = replaceWordColor( strFunction , 'case' 					, 'green');
			strFunction = replaceWordColor( strFunction , 'break' 					, 'green');
			strFunction = replaceWordColor( strFunction , 'default'					, 'green');
			strFunction = replaceWordColor( strFunction , 'if' 						, 'green');
			strFunction = replaceWordColor( strFunction , 'else' 					, 'green');
			strFunction = replaceWordColor( strFunction , 'for' 					, 'green');
			strFunction = replaceWordColor( strFunction , 'var' 					, 'green');
			strFunction = replaceWordColor( strFunction , 'null' 					, 'blue');
			strFunction = replaceWordColor( strFunction , 'return' 					, 'blue');
			strFunction = replaceWordColor( strFunction , 'debuggerEnterFunction' 	, '#4444FF');
			strFunction = replaceWordColor( strFunction , 'debuggerOutFunction' 	, '#FF4444');
			
			document.body.innerHTML += '<pre>' + strKeyName + ':' + "\n" + strFunction + '</pre>' ;
		}
		
		this.constructor( objElement , boolShowAll );
	}
	
	window.debuggerClass.printMessage = function printMessage( strMessage )
	{
		objDivDebugger = document.getElementById( 'debugger' );
		if( ! objDivDebugger )
		{
			objDivDebugger = document.createElement( 'div' );
			objDivDebugger.id = 'debugger';
			document.body.appendChild( objDivDebugger );
		}
		objDivDebugger.innerHTML += strMessage;
	}
	
	window.debuggerClass.debuggerEnterFunction = function debuggerEnterFunction( strFunctionName )
	{
		window.debuggerClass.printMessage( strFunctionName + '<br/>' );
	}
	
	window.debuggerClass.debuggerOutFunction = function debuggerOutFunction( strFunctionName )
	{
		
	}
	window.debuggerClass.debuggerAlert = function debuggerAlert( strMessage )
	{
		window.debuggerClass.printMessage( strMessage + '<br/>' );
	}
	
	
	/**
	 * @see superTitle.css
	 * @date 13-12-2006
	 */
	window.initializeDebuggerClass = function initializeDebuggerClass()
	{
		importCssStyle( 'debug/debuggerClass.css' );
	}

initializeDebuggerClass();	
	
window.debuggerClass.lastMessage = '';
	
window.makeAsDebuggerCommand = function makeAsDebuggerCommand( strJsCommand , strUrlCommand )
{
	arrLines = explode( "\n" , strJsCommand );
	for( var i = 0 ; i < arrLines.length ; ++i )
	{
		strText = arrLines[ i ]
		strText = trim( strText );
		if( strText.charAt( strText.length - 1 ) == ';' )
		{
			strShow = strText;
			strShow = replaceAll( strShow , "\"" ,  "" );
			strShow = replaceAll( strShow , "\'" ,  "" );
			strShow = replaceAll( strShow , "\n" ,  "" );
			strShow = replaceAll( strShow , "\r" ,  "" );
			strShow = replaceAll( strShow , "<" ,  "&lt;" );
			strShow = replaceAll( strShow , ">" ,  "&gt;" );
			
			//strText = strText + ' window.debuggerClass.lastMessage = "' + strShow + '"; debuggerClass.debuggerAlert( "' + strUrlCommand + ' : " + "' + i + ' - Command: " + "' +  strShow + '" ); ';
			strText = strText + ' window.debuggerClass.lastMessage = "' + strShow + '"; debuggerClass.debuggerAlert( "' + strUrlCommand + ' : " + "' + i + ' - Command: " + "' +  strShow + '" ); ';
			
			arrLines[ i ] = strText;
		}
	}
	strJsCommand = implode( "\n" , arrLines );
	return strJsCommand;
}


window.makeHappendDebugger = function( strJsCommand , strUrlName )
{
	strJsCommand = makeAsDebuggerCommand( strJsCommand , strUrlName );
	try
	{
		if( !IE )
		{
			with( window )
			{
				window.eval( strJsCommand );
			}	
		}
		else
		{
			eval( strJsCommand );
		}
	}
	catch( e )
	{
		strError = '';
		for( i in e )
		{
			strError += i + ' ' + e[ i ] + "\n";
		}
		if( confirm( 'Ocorreu um erro ao carregar o arquivo ' + strUrlName + '. Clique Ok para mais detalhes.' ) )
		{
			alert( strError );
		}
	}
}


window.include_js_debug = function include_js_debug( strJsCommand , objXml , arrReceivedParam )
{
	if( isArray( arrReceivedParam ) && arrReceivedParam.length > 0 )
	{
		funcAfterLoad = arrReceivedParam[ 0 ];
		strUrlName =  arrReceivedParam[ 1 ];
	}
	else
	{
		funcAfterLoad = function(){};
		strUrlName = 'void';
	}
	
	window.makeHappendDebugger( strJsCommand , strUrlName );
	
	if( isFunction( funcAfterLoad ) )
	{
		loop_include_js( funcAfterLoad , strUrlName);
	}
}


/**
 * Metodo de se adicionar um script como requerido para a execução de outro.
 *
 * Atuamente, este método de chamada deve ser feito apenas nos scripts que não são carregados
 * dinamicamente.
 */
window.require_once_debug = function require_once_debug( arrUrls , funcAfterLoad_ )
{
	window.require_once = window.require_once_debug;
	if( globalArrUrls == undefined ) globalArrUrls = Array();
	if( isString( arrUrls ) )
	{
		var arrUrls = new Array( arrUrls );
	}
	if( isUndefined( arrUrls ) )
	{
		throw new Error( 'arrUrls must have some value into require_once_debugger ' );
	}
	var intLast = arrUrls.length - 1 ;
	for( var  i = 0 ; i < arrUrls.length ; ++i )	
	{
		strUrl = arrUrls[ i ];
	
		if( array_search( strUrl , globalArrUrls ) == -1 )
		{
			strFunctionServerName = '';
			arrParam = Array();
			
			if( funcAfterLoad_ == undefined )
			{
				funcAfterLoad_ = function(){};
			}
			if( i == intLast )
			{
				var arrLoopParams = Array( funcAfterLoad_ , strUrl );
			}
			else
			{
				var arrLoopParams = Array( function(){} , strUrl );
			}
	
			globalArrStandBy.push( strUrl );
			globalArrUrls.push( strUrl );
			addRequest( PATH_JS_LIBRARY + strUrl , '' , arrParam , include_js_debug , null , arrLoopParams );
		}
		else if (  i == intLast )
		{
			if( ! isUndefined( funcAfterLoad ) )
			{
				funcAfterLoad();
			}
		}
	}
}
