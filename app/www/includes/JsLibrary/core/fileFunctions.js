window.globalArrOnExecution = Array();

window.checkIfSomebodyIsOnExecution = function checkIfSomebodyIsOnExecution()
{
	for( strUrlName in window.globalArrOnExecution )
	{
		if(  window.globalArrOnExecution[ strUrlName ]  == false )
		{
			return true;
		}  
	} 
	return false; 
}
 
window.makeHappend = function makeHappend( strJsCommand , strUrlName )
{
	strUrlName = replaceAll( strUrlName , '/' , '_' );
	strUrlName = replaceAll( strUrlName , ' ' , '_' );
	
	window.globalArrOnExecution[ strUrlName ] = false;
	
	strJsCommand += "; \n" + ' window.globalArrOnExecution["' + strUrlName + '"] = true; ';

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
			with( window )
			{		
				eval( strJsCommand );
			}
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
			if( window.debuggerClass )
			{
				if ( window.debuggerClass.lastMessage != '' )
				{
					throw new Error( window.debuggerClass.lastMessage );
				}
			}
		}
		throw e;
	}
}


window.include_js = function include_js( strJsCommand , objXml , arrReceivedParam )
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
	
	window.makeHappend( strJsCommand , strUrlName );
	
	if( isFunction( funcAfterLoad ) )
	{
		loop_include_js( funcAfterLoad , strUrlName);
	}
}

window.globalBoolXmlRequestTreeActive = true;

window.test = 0;
window.loop_include_js = function loop_include_js( funcAfterLoad , strUrlName )
{

//	document.title = globalArrStandBy + ' ' + strUrlName + '? ' + arrGlobalStackXmlRequest.length;
	
	if( array_search( strUrlName , globalArrStandBy ) == -1 )
	{
		return;
	}
	
	if( arrGlobalStackXmlRequest.length > 0 || AJAX_LOADED == false)
	{
		setTimeout( 'loop_include_js( ' + funcAfterLoad + ' , "' + strUrlName +'" ) ' , 10 );
	}
	else
	{
		if( globalArrStandBy[ globalArrStandBy.length - 1 ] == strUrlName )
		{
			globalArrStandBy.splice( array_search( strUrlName , globalArrStandBy ) , 1 );
			funcAfterLoad();
		}
		else
		{
			setTimeout( 'loop_include_js( ' + funcAfterLoad + ' , "' + strUrlName +'" ) ' , 10 );
		}
	}
	window.globalBoolXmlRequestTreeActive = ( globalArrStandBy.length != 0 || ( window.checkIfSomebodyIsOnExecution()  ) )
}

globalArrUrls = new Array();

globalArrStandBy = new Array();

/**
 * Metodo de se adicionar um script como requerido para a execuo de outro.
 *
 * Atuamente, este mtodo de chamada deve ser feito apenas nos scripts que no so carregados
 * dinamicamente.
 */
window.require_once = function require_once( arrUrls , funcAfterLoad_ )
{
	
	if( isUndefined( window.globalIntCountBlank ) )
	{
		window.globalIntCountBlank = 0;
	}
	
	if( funcAfterLoad_ == undefined )
	{
		funcAfterLoad_ = function(){};
	}
	
	if( globalArrUrls == undefined ) globalArrUrls = Array();
	if( isString( arrUrls ) )
	{
		var arrUrls = new Array( arrUrls );
	}
	
	var intLast = arrUrls.length - 1 ;
	for( var  i = 0 ; i < arrUrls.length ; ++i )	
	{
		strUrl = arrUrls[ i ];
		
		if( array_search( strUrl , globalArrUrls ) != -1 )
		{
			arrUrls.splice( i , 1 );
		}
	}
		
	if( arrUrls.length > 0 )
	{
		for( var  i = 0 ; i < arrUrls.length ; ++i )	
		{			
			strUrl = arrUrls[ i ];
			if( array_search( strUrl , globalArrUrls ) == -1 )
			{
				strFunctionServerName = '';
				arrParam = Array();
				
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
				addRequest( PATH_JS_LIBRARY + strUrl , '' , arrParam , include_js , null , arrLoopParams );
			}
			else if( i == ( arrUrls.length - 1 ) )
			{
				//strUrlName = '_blank' + window.globalIntCountBlank++;
				//var arrLoopParams = Array( funcAfterLoad_ , strUrl );
				//globalArrStandBy.push( strUrl );
				//include_js( '' , '' , arrLoopParams);
				//makeHappend( funcAfterLoad_ , strUrlName );
				funcAfterLoad_();
			}
		}
	}
	else if( ! isUndefined( funcAfterLoad_ ) )
	{
		//strUrlName = '_blank' + window.globalIntCountBlank++;
		//var arrLoopParams = Array( funcAfterLoad_ , strUrl );
		//globalArrStandBy.push( strUrl );
		//include_js( '' , '' , arrLoopParams);
		//makeHappend( funcAfterLoad_ , strUrlName );
		funcAfterLoad_();
	}
}

function importPackage( strPackage )
{
	strPackage = replaceAll( strPackage , '.' , '/' );
	strPackage = replaceAll( strPackage , '*' , '_start' );
	strPackage += '.js';
	require_once( strPackage );
}

window.onBodyLoad = function onBodyLoad()
{
	globalBoolBodyLoad = true;	
}

window.importCssStyle = function importCssStyle( strCssFileName )
{
	if( array_search( strCssFileName , globalArrUrls ) == -1 )
	{
		print
		(
			'<LINK REL="StyleSheet" HREF=' +  '"' + PATH_JS_LIBRARY + strCssFileName + '" >' + "\n" +
			'</LINK>'
		);
		globalArrUrls.push( strCssFileName );
	}
	
}	

window.builImageLink = function builImageLink( strImageLink )
{
	return PATH_IMAGES + strImageLink;
}

window.buildImage = function( strImageLink , strAlt_ , strTitle_ )
{
	if( strAlt_ == undefined )
	{
		strAlt_ = '';
	}
	if( strTitle_ == undefined )
	{
		strTitle_ = '';
	}
	if( !document.body )
	{
		document.write
		(
			'<img src=' +  '"' + window.builImageLink( strImageLink ) + '" />' + "\n"
		);
	}
	else
	{
		print
		(
			'<img src=' +  '"' + window.builImageLink( strImageLink ) + '" />' + "\n"
		);
	}
}	