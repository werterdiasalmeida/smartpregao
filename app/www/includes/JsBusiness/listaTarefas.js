strSrcImgMore	= builImageLink( "more.gif" );
strSrcImgLess	= builImageLink( "less.gif" );
strSrcImgWait	= builImageLink( "wait.gif" );


function main_tarefas()
{	
	window.activeMouseGetPos();
	window.activeBodyGetKey();	
}

function showHideMyChildren( idTarefa , boolShow )
{
	arrTrTarefas = document.getElementById( 'tabelaTarefas' ).getElementsByTagName( 'tr' );
	for( var i = 0; i < arrTrTarefas.length ; ++i )
	{
		if( arrTrTarefas[ i ].getAttribute( 'parent' ) == idTarefa )
		{
			strChild =  arrTrTarefas[ i ].id;
			if( ! boolShow )
			{
				showHideMyChildren( strChild.substr( 2 ) , boolShow );
			}
			if( ! boolShow )
			{
				arrTrTarefas[ i ].style.display = "none";
			}
			else
			{
				if( !IE )
				{
					arrTrTarefas[ i ].style.display = "table-row";
				}
				else
				{
					arrTrTarefas[ i ].style.display = "block";
				}
				closeImgPlus( arrTrTarefas[ i ] );;
			}
		}
	}
}

function removeMyChildren( idTarefa )
{
	arrTrTarefas = document.getElementById( 'tabelaTarefas' ).getElementsByTagName( 'tr' );
	for( var i = 0; i < arrTrTarefas.length ; ++i )
	{
		if( arrTrTarefas[ i ].getAttribute( 'parent' ) == idTarefa )
		{
			strChild =  arrTrTarefas[ i ].id;
			removeMyChildren( strChild.substr( 2 ) );
			arrTrTarefas[ i ].parentNode.removeChild( arrTrTarefas[ i ] );
			i  = 0;
		}
	}
}

function getImgPlus( objTr )
{
	return objTr.getElementsByTagName( "img" )[ 5 ];
}

function closeImgPlus( objTr )
{
	var objImgPlus = getImgPlus( objTr );
	objImgPlus.src = strSrcImgMore;	
	try
	{
		objImgPlus.style.visibility = "show";
	}
	catch(e){}
}

function waitImgPlus( objTr )
{
	var objImgPlus = getImgPlus( objTr );
	objImgPlus.src = strSrcImgWait;
	try
	{
		objImgPlus.style.visibility = "show";
	}
	catch(e){}
}

function openImgPlus( objTr )
{
	var objImgPlus = getImgPlus( objTr );
	objImgPlus.src = strSrcImgLess;
	try
	{
		objImgPlus.style.visibility = "show";
	}
	catch(e){}
}

function incluirTarefaRaiz( )
{
	cod = 0;
	e = "monitora.php?modulo=principal/projespec/plantrabpje_insercao&acao=I&id="+cod;
	window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=300'");
}

function incluirTarefaFilha( idTarefa , idProjeto, objImg )
{
	// pegando os atributos da imagem
	var strSrcOriginal	= objImg.src;
	
	// pegando o td
	var objContainerImg = objImg;
	while( objContainerImg.tagName != 'TD' )
	{
		objContainerImg = objContainerImg.parentNode;
	}
	var objTdElement 	= objContainerImg;
	
	// lendo status do atributo loaded
	var boolLoaded		= objTdElement.parentNode.getAttribute( 'loaded' ) == 'true';
	
	// caso o conteudo interno ja esteja em processo de carga //
	if	( 
			( objTdElement.parentNode.getAttribute( 'loaded' ) == 'process' ) 
			|| 
			( objTdElement.parentNode.getAttribute( 'loaded' ) == 'inserting' ) 
		)
	{
		return;
	}
	
	// caso o conteudo interno ja tenha ainda sido carregado //
	if( boolLoaded )
	{
		// limpa o conteudo carregado
		var strParentId = 	( objTdElement.parentNode.id + '').substring( 2 );
		removeMyChildren(  strParentId );
		closeImgPlus( objTdElement.parentNode );
	}
	
	// o conteudo interno a partir de agora estara em processo de carga //
	objTdElement.parentNode.setAttribute( 'loaded' , 'inserting' );
	objImg.src = strSrcImgWait;
	
	// gerando o array de parametros que irao voltar a funcao de resposta //
	var arrLoopParams = new Array();
	arrLoopParams.push( idTarefa );
	arrLoopParams.push( objTdElement );
	arrLoopParams.push( objImg );
	arrLoopParams.push( strSrcOriginal );
	
	objImg.src = strSrcImgWait;
	insertElement( objImg ,idTarefa, idProjeto, window.serverSideClassName , aposInserirTarefasFilhas , arrLoopParams )
	
}

function aposInserirTarefasFilhas( arrLoopParams )
{
//	aler( 'aposInserirTarefasFilhas' );
	var idTarefa		= arrLoopParams[ 0 ];
	var objTdElement	= arrLoopParams[ 1 ];
	var objImg			= arrLoopParams[ 2 ];
	var strSrcOriginal	= arrLoopParams[ 3 ];
	
	objImg.src = strSrcOriginal;
	var objImgPlus		= getImgPlus( objTdElement.parentNode );
	
	carregaTarefasFilhas( idTarefa , objImg , function(){ buscaNovasTarefas(); });
}

function carregaTarefasFilhas( idTarefa , objImg , funcAposCarregar_ )
{
//	alert( 'carregaTarefasFilhas' );
	// pegndo os atributos da imagem
	var strSrcOriginal	= objImg.src;
	
	// pegando o td
	var objContainerImg = objImg;
	while( objContainerImg.tagName != 'TD' )
	{
		objContainerImg = objContainerImg.parentNode;
	}
	var objTdElement 	= objContainerImg;
	
	// lendo status do atributo loaded
	var boolLoaded		= objTdElement.parentNode.getAttribute( 'loaded' ) == 'true';
	
	// caso o conteudo interno esteja em processo de carga //
	if( objTdElement.parentNode.getAttribute( 'loaded' ) == 'process' )
	{
		return;
	}
	
	// caso o conteudo interno nao tenha ainda sido carregado //
	if( ! boolLoaded )
	{
		waitImgPlus( objTdElement.parentNode );
		
		// o conteudo interno a partir de agora estara em processo de carga //
		objTdElement.parentNode.setAttribute( 'loaded' , 'process' );
		objImg.src = strSrcImgWait;
		
		// gerando o array de parametros para enviar a requisicao //
		var arrParams = new Array();
		arrParams.push( 'carregarTarefasFilhas' );
		arrParams.push( window.serverSideClassName );
		arrParams.push( idTarefa );
		
		// gerando o array de parametros que irao voltar a funcao de resposta //
		var arrLoopParams = new Array();
		arrLoopParams.push( idTarefa );
		arrLoopParams.push( objTdElement );
		arrLoopParams.push( objImg );
		arrLoopParams.push( strSrcOriginal );
		arrLoopParams.push( funcAposCarregar_ );
		
		objImg.src = strSrcImgWait;
		
		addRequest( '/geral/ajax/requisicao_ajax.php' , '' , arrParams , aposCarregarTarefasFilhas , null , arrLoopParams );
	}
	// caso o conteudo interno ja tenha sido carregado //
	else
	{
		var strSrc = objImg.src + '';
		if( strSrc.indexOf( strSrcImgMore ) != -1 )
		{
			// objImg.src = strSrcImgLess;
			openImgPlus( objTdElement.parentNode );
			showHideMyChildren( idTarefa , true );
		}
		else
		{
			//objImg.src = strSrcImgMore;
			closeImgPlus( objTdElement.parentNode );
			showHideMyChildren( idTarefa , false );
		}
	}
}

function aposCarregarTarefasFilhas( strXml , objXml , arrLoopParams )
{
	var arrDegradeCor			= Array( 255 , 255 , 255 );
	var idTarefa				= arrLoopParams[0];
	var objTdElement			= arrLoopParams[1];
	var objImgElementLoading	= arrLoopParams[2];
	var strIgmSrc				= arrLoopParams[3];
	var funcAposCarregar_		= arrLoopParams[4];
	
	if( funcAposCarregar_ == undefined )
	{
		funcAposCarregar_ = function(){};
	}
	
	var intColunaTarefaNomePosicao = 2;
	
	var objTrElement		= new DomElement( objTdElement.parentNode );
	
	var objTrActual = objTdElement.parentNode;

	var objTarefaNome = objTrActual.getElementsByTagName( "td" )[ intColunaTarefaNomePosicao ];
	
	var intTrElementPaddingLeft =  0 + 
	forceInt( 
		objTarefaNome.style.paddingLeft 
	);
	var objDomDivObject = DomDocument.createElement( 'div' );
	objDomDivObject.setInnerHTML( strXml );
	
	var arrTrElements	= objDomDivObject.getElementsByTagName( 'tr' );
	
	while( 0 + arrTrElements.length > 0 )
	{
		var objActualTrElement = arrTrElements.pop();
		objActualTrElement.objHtmlElement.getElementsByTagName( "td" )[ intColunaTarefaNomePosicao ].style.paddingLeft = intTrElementPaddingLeft + 30;
		objTrElement.insertAfter( objActualTrElement );
	}

	try
	{
		prepareActiveFrozenFields();
		
	}catch(e){};
	
	objImgElementLoading.src = strIgmSrc;
	openImgPlus( objTdElement.parentNode );
	
	objTrElement.setAttribute( 'loaded' , 'true' );
	funcAposCarregar_();
}



function editarTarefa( intIdTarefa )
{
	switch( window.serverSideClassName )
	{
		case 'TarefaPT':
		{
			e = "monitora.php?modulo=principal/projespec/plantrabpje_edicao&acao=A&id="+intIdTarefa;
			window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=600'");
			break;
		}
		case 'TarefaAcao':
		{
			e = "monitora.php?modulo=principal/acao/plantrabacao_edicao&acao=A&id="+intIdTarefa;
			window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=600'");
			break;
		}
	}
}


function aposRemoverTarefa( strXml , objXml , arrLoopParams )
{
	var objImg			= arrLoopParams[ 0 ];
	var strSrcOriginal	= arrLoopParams[ 1 ];
	var intIdTarefa		= arrLoopParams[ 2 ];
	
	var objTr = document.getElementById( "tr" + intIdTarefa );
	var intIdParentTarefa = objTr.getAttribute( "parent" );
	objImg.src = strSrcOriginal;
	removeMyChildren( intIdTarefa );
		
}

require_once( 
	Array( 
		'dom/_start.js' 
		,
		'sync/_start.js'
		) , 
	main_tarefas 
);

function alterartarefa( cod )
{
	strLink = "monitora.php?modulo=principal/projespec/plantrabpje_edicao&acao=A&id="+cod;
	window.open( 
		strLink , 
		"janela" , 	
		"menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=600'"
	);
}

function removeTarefaPT( intIdTarefa )
{
	removeTarefa( intIdTarefa);	
}

function removeTarefaAcao( intIdTarefa )
{
	removeTarefa( intIdTarefa);	
}

function removeTarefa( intIdTarefa )
{
	objTr = document.getElementById( "tr" + intIdTarefa );
	if( objTr )
	{
		objTr.parentNode.removeChild( objTr );
	}
	try
	{
		objImg.src = strSrcOriginal;
		removeMyChildren( intIdTarefa );
		objTr.parentNode.removeChild( objTr );
	}
	catch( e )
	{
		// o elemento foi removido
	}
	
	/**
	 * @TODO Ao se removerem todos os elementos de um pai remover dele o 
	 * mais e o estilo caracteristico.
	 */
}

function removeTarefaPDE( intIdTarefa )
{
	objTr = document.getElementById( "tr" + intIdTarefa );
	if( objTr )
	{
		objTr.parentNode.removeChild( objTr );
	}
	objImg.src = strSrcOriginal;
	removeMyChildren( intIdTarefa );
	objTr.parentNode.removeChild( objTr );
}


function exibe_grafico( strContainer, intContainerId , intTarefaId_ , intNivel_ )
{
	intTarefaId_ = ( forceInt( intTarefaId_ ) );
//	alert( intTarefaId_ );
	if( intNivel_ == undefined  )
	{
		var objSelect = document.getElementById( "profunidadeNivel" );
		var intNivel_ = objSelect.value;
	}
	var strLink = "../geral/gantt.php?container=" + strContainer + "&containerId=" + intContainerId + "&nivel=" + intNivel_ +"&tarefa=" + intTarefaId_;
    window.open( 
    	strLink ,
    	"janela",
    	"menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes" );
}

function buscaNovasTarefas()
{
	var arrSpan = document.getElementsByTagName( "span" );
	var objFirstSpan = null;
	
	for( var i = 0 ; i < arrSpan.length ; ++i )
	{
		var objSpan = arrSpan[ i ];
		if( trim( objSpan.innerHTML ) == "Nova Tarefa" )
		{
			objSpan.parentNode.className += " novaTarefa ";
			if( objFirstSpan == null )
			{
				objFirstSpan = objSpan;
			}
		}
		else
		{
			var strClassName = objSpan.parentNode.className + '';
			objSpan.parentNode.className =  replaceAll( strClassName, " novaTarefa ", "" );
		}
	}
	if( objFirstSpan != null )
	{
		objFirstSpan.onclick();
	}
}

function dumpmenusdoblaine()
{
	var mt = "<script language=javascript>"; 
	for ( var a = 1; a < menus.length; a++ )
	{ 
		mt += " menu" + a + "=menus[" + a + "];"; 
	}
	mt += "</script>"; 
//	_d.write(mt); 
	print( mt );
}