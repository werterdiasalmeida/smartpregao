window.MenuSelect = function MenuSelect( arrElements, arrElementsLink , arrElementsShortcuts, objParent  )
{
	this.intAlpha = 0;
	
	this.intAlphaCount = 5;
	
	this.boolInLoop = false;
	
	this.intActualElement = -1;
	
	this.arrElements = new Array();
	
	this.arrElemenstLink = new Array();
	
	this.arrElementsShortcuts = new Array();
	
	this.parent = null;
	
	this.objContainer = null;
	
	this.__construct = function __construct( arrElements, arrElementsLink , arrElementsShortcuts , objParent )
	{
//		objDebug = new debuggerClass( this , false );
		this.parent = objParent;
		this.arrElements = arrElements;
		this.arrElementsLink = arrElementsLink;
		this.arrElementsShortcuts = arrElementsShortcuts;
		this.arrElementsUnderline = new Array();
		
		intOffsetHeight = 18;
		objDivMenuSelect = document.createElement( 'div' );
		objDivMenuSelect.className = 'menuSelect';
		strHtml = '<table>';
		for( var  i = 0 ; i < arrElementsShortcuts.length; ++i )
		{
			
			intPosUpper = arrElements[ i ].indexOf( arrElementsShortcuts[ i ].toUpperCase() );
			intPosLower = arrElements[ i ].indexOf( arrElementsShortcuts[ i ].toLowerCase() );
			if( intPosUpper != -1 && intPosLower != -1 )
			{
				intFirst = Math.min( intPosUpper, intPosLower );
			}
			else if ( intPosUpper != -1 )
			{
				intFirst = intPosUpper;
			}
			else
			{
				intFirst = intPosLower;
			}
			
			if( intFirst != -1 )
			{
				this.arrElementsUnderline[ i ] = arrElements[ i ].substr( 0 , intFirst ) +  '<u>' + arrElements[ i ].charAt( intFirst  ) + '</u>' + arrElements[ i ].substr( intFirst + 1); 
			}
		}
		for( var  i = 0 ; i < arrElements.length; ++i )
		{
			strTdOnMouseOverEvent	= 'this.className = \'menuSelectOver\'';
			strTdOnMouseOutEvent	= 'this.className = \'menuSelectOut\'';
			strHtml += ''
				+''
				+'<tr>'
				+'	<td '
				+		' class="menuSelectOut" ' 
				+		'onmouseover="' + strTdOnMouseOverEvent + '" '
				+		'onmouseout="' + strTdOnMouseOutEvent + '" '
				+		'onclick="abreElementoMenu( \'' + this.arrElementsLink[ i ] + '\' , window.MenuSelect.getElementById( ' + this.id + ' ).parent )" '
				+	'>' 
				+'		' + this.arrElementsUnderline[ i ];
				+'	</td>'
				+'</tr>'
				+'';
		}
		objDivMenuSelect.innerHTML = strHtml;
		objDivSombra = document.createElement( 'div' );
		objDivSombra.innerHTML = '<input type="text" length="0" style="visibility: hidden; position: absolute; top: 0px; left: 0px"/>';
		objDivMenuSelect.appendChild( objDivSombra );
		objDivSombra.style.height = arrElements.length * intOffsetHeight;
		objContainer = document.createElement( 'div' );
		objContainer.id = 'container';
		objContainer.appendChild( objDivMenuSelect );
		objContainer.style.position = "absolute";
		objContainer.style.top	= mouseY - 5;
		objContainer.style.left = mouseX - 20;
		objTable = objContainer.getElementsByTagName( 'table' )[0];
		
		if ( !IE )
		{
			objTable.onmouseout		= Function( 'window.MenuSelect.getElementById(' + this.id + ').fadeOut()' );
			objTable.onmouseover	= Function( 'window.MenuSelect.getElementById(' + this.id +').fadeIn()' );
		}
		else
		{
			objTable.onmouseout		= Function( 'setTimeout( "try{ window.MenuSelect.getElementById(' + this.id +').fadeOut() }catch( e ){ }" , 2000 )' );
//			objContainer.onmouseover	= Function( 'window.MenuSelect.getElementById(' + this.id + ').fadeIn()' );
//			objContainer.onmouseover	= Function( 'setTimeout( alert( "window.MenuSelect.getElementById(' + this.id +').fadeIn()" ) , 100)' );
		}
		
		this.objContainer = objContainer;
//		document.write( objContainer.innerHTML );
//		 document.body.innerHTML += objContainer.innerHTML;
//		document.body.appendChild( objContainer );
		this.parent.parentNode.appendChild( objContainer );
		
		window.MenuSelect.removeOtherInstances( this.id );
		this.fadeIn();
	}
	
	this.__destruct = function destruct()
	{
		objMenuSelect.objContainer.parentNode.removeChild( objMenuSelect.objContainer );
		window.MenuSelect.arrObjParent[ this.id ] = null;
		window.MenuSelect.arrInstances[ this.id ] = null;
	}
	
	this.fadeIn = function fadeIn()
	{
		this.intAlphaCount = 5;
		this.singletonLoop();
	}
	
	this.fadeOut = function fadeOut()
	{
		this.intAlphaCount = -5;
		this.singletonLoop();
	}

	this.singletonLoop = function singletonLoop()
	{
		if( !this.boolInLoop )
		{
			this.boolInLoop = true;
			this.fadeLoop();
		}
	}
	
	this.fadeLoop = function fadeLoop()
	{
		this.intAlpha += this.intAlphaCount;
		if ( this.intAlpha <= 0 )
		{
			this.intAlpha = 0;
			this.boolInLoop = false;
			this.__destruct();
		}
		else if ( this.intAlpha >= 100 )
		{
			this.intAlpha = 100;
			this.boolInLoop = false;
		}
		
		if( !IE )
		{
			Alpha( this.objContainer , this.intAlpha );
		}
		else
		{
			
			// nenhum funcionou // 			
//			window.Alpha( this.objContainer , this.intAlpha );
//			var objContainer = document.getElementById( 'container' );
//			window.Alpha( objContainer , 80 );
//			this.objContainer.style.filter  = 'alpha(opacity=75);-moz-opacity:.75;opacity:.75';
		}
		
		if( this.boolInLoop )
		{
			strCommand = ( 'try{ window.MenuSelect.getElementById(' + this.id + ').fadeLoop() }catch( e ){ }' );
			setTimeout( strCommand , 10 );
		}
	}
	
	this.beforeElement = function beforeElement()
	{
		--this.intActualElement;
		if( this.intActualElement < 0 )
		{
			this.intActualElement = this.arrElements.length - 1 ;
		}
		this.selectActualElement();
	}
	
	this.nextElement = function afterElement()
	{
		++this.intActualElement;
		if( this.intActualElement >= this.arrElements.length )
		{
			this.intActualElement = 0 ;
		}
		this.selectActualElement();
	}
	
	this.selectActualElement = function selectActualElement()
	{
		var arrTrElements = this.objContainer.getElementsByTagName( 'td' );
		for( var  i = 0 ; i < arrTrElements.length; ++i )
		{
			if( this.intActualElement == i )
				arrTrElements[ i ].className = 'menuSelectOver';
			else
				arrTrElements[ i ].className = 'menuSelectOut';
		}
		try
		{
			this.objContainer.getElementsByTagName( 'input' )[0].focus();
		}
		catch( e )
		{
			
		}
	}

	this.keyPress = function keyPress( chrKey )
	{
		for( var  i = 0 ; i < arrElementsShortcuts.length; ++i )
		{
			if( this.arrElementsShortcuts[ i ].toUpperCase() == chrKey )
			{
				return abreElementoMenu( this.arrElementsLink[ i ] , this.parent );
			}
		}
	}
	
	this.enterPress = function enterPress()
	{
		if( this.intActualElement != -1 )
		{
				return abreElementoMenu( this.arrElementsLink[ this.intActualElement ] , this.parent );
		}
	}
	
	this.id = window.MenuSelect.arrInstances.length;
	window.MenuSelect.arrInstances[ this.id ] = this;
	window.MenuSelect.objActualInstance = this;
	this.__construct( arrElements, arrElementsLink , arrElementsShortcuts , objParent );
}

window.MenuSelect.objActualInstance = null;
window.MenuSelect.arrInstances = new Array();
window.MenuSelect.arrObjParent = new Array();
window.MenuSelect.arrObjParentInstances = new Array();

window.MenuSelect.removeOtherInstances = function removeOtherInstances( intId )
{
	for( var i = 0; i <  window.MenuSelect.arrInstances.length ; ++i )
	{
		if( i != intId )
		{
			objMenuSelect = window.MenuSelect.getElementById( i );
			if( !isNull( objMenuSelect ) )
			{
				objMenuSelect.fadeLoop();
			}
		}
		else
		{
		}
	}
	objMenuSelectActual = window.MenuSelect.getElementById( intId );
}
window.MenuSelect.getElementById = function getElementById( intId )
{
	if( intId < window.MenuSelect.arrInstances.length )
	{
		return window.MenuSelect.arrInstances[ intId ];
	}
	else
	{
		return null;
	}
}
window.MenuSelect.getElementByParent = function getElementByParent( objElement , arrElements , arrElementsLink , arrElementsShortcuts)
{
	var intPos = array_search( objElement ,window.MenuSelect.arrObjParent );
	if( intPos == -1 )
	{
		objMenuSelect = new MenuSelect( arrElements , arrElementsLink , arrElementsShortcuts , objElement );
		window.MenuSelect.arrObjParent.push( objElement );
		window.MenuSelect.arrObjParentInstances.push( objMenuSelect );
	}
	else
	{
		objMenuSelect = window.MenuSelect.arrObjParentInstances[ intPos ];
	}
	return objMenuSelect;
}

window.MenuSelect.onKeyDown = function onKeyDown( intKey , strType )
{
	switch( intKey )
	{
		case 38:
		{
			if( !isNull( window.MenuSelect.objActualInstance ) )
			{
				window.MenuSelect.objActualInstance.beforeElement();
			}
			break;
		}
		case 40:
		{
			if( !isNull( window.MenuSelect.objActualInstance ) )
			{
				window.MenuSelect.objActualInstance.nextElement();
			}
			break;
		}
		default:
		{
			if( !isNull( window.MenuSelect.objActualInstance ) )
			{
				if( strType != 'enter' )
				{
					window.MenuSelect.objActualInstance.keyPress( String.fromCharCode( intKey % 1000 )  );
				}
				else
				{
					window.MenuSelect.objActualInstance.enterPress();
				}
			}
			break;
		}
	}
}
/**
 * @see menuSelect.css
 * @date 19-12-2006
 */
window.initializeMenuSelect = function initializeMenuSelect()
{
	importCssStyle( 'tags/menuSelect.css' );
	activeMouseGetPos();
}
	
require_once( 'keys/keyEvents.js' );
initializeMenuSelect();
