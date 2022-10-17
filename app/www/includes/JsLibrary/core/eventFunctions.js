/**
 *# Funcoes para lidarem com eventos do sistema.
 *#
 *# Functions to deal with the system events
 *
 *@link http://www.crockford.com/javascript/remedial.html
 *@date 01/11/2005
 *@version 2.0
 * with many changes
 */



/**
 *
 @date 2006-01-17
 */
window.Alpha = function Alpha( objField , Percents)
    {
    if ( objField )
    	{
    		try
    		{
		   	objField.style.filter		= "alpha(opacity=" + Percents + ")";
			objField.style.MozOpacity	= "" + Percents/100 + "";
			}
			catch( e )
			{
			}
		}
	}
		 
/**
 * Controle padrao para objetos que aparecem e somem alternadamente.
 * 
 * Default control function to objects how show and hide.
 * @param object object
 * @return boolean
 */
function showHide( object )
{
  if (object.style.display == "none")
  {
    object.style.display = "block";
    return true;
  }
  else
  {
    object.style.display = "none";
    return false;
  }
}

var dav = navigator.appVersion;
IE = document.all ? true : false;
IE6 = dav.indexOf("MSIE 6.0") >= 0;
IE7 = dav.indexOf("MSIE 7.0") >= 0;
var mouseX = 0;
var mouseY = 0;

/**
 * Active the function to get the mouse position
 * 
 *@return void
 */
function activeMouseGetPos()
{
	if (!IE)
	{
		// document.captureEvents(Event.MOUSEMOVE);
		document.onmousemove = getMouseXY;
		return;
	}
	// Set-up to use getMouseXY function onMouseMove
	document.onmousemove = getMouseXY;

	// Temporary variables to hold mouse x-y pos.s

	// Main function to retrieve mouse x-y pos.s
}

/**
 * Called function when the mouse move
 * 
 * THIS FUNCTION MAY BE OVERWRITE
 *
 *@version 1.0
 *@param intX
 *@param intY
 *@return bool
 */
function mouseMove( intX , intY)
{
	return true;
}

/**
 * Event to se the mouse position into global variables
 * and dispare the mouseMove function. To be used into
 * on body mouse move event.
 * 
 * @param event e
 * @return boolean
 */
function getMouseXY(e)
{
	if (IE)
	{
		mouseX = event.clientX + document.body.scrollLeft;
		mouseY = event.clientY + document.body.scrollTop;
	}
	else
	{
		mouseX = e.pageX;
		mouseY = e.pageY;
	}
	if (mouseX < 0){mouseX = 0}
	if (mouseY < 0){mouseY = 0}

	document.MouseX = new Object();
	document.MouseY = new Object();
	document.MouseX.value = mouseX;
	document.MouseY.value = mouseY;

	return mouseMove( mouseX, mouseY);
}

function debuggerOutFunction()
{
}

function debuggerEnterFunction( strFunction )
{
}

function debuggerFunction( funcEnter )
{
	return funcEnter;
}

window.eventGroup = function eventGroup( objContainer )
{
	this.objContainer = objContainer;
	
	this.arrFunctions = Array();
	
	this.arrTempFunctions = Array();
	
	this.addPermantFunction = function addPermantFunction( funNewEventFunction )
	{
		this.arrFunctions.push( funNewEventFunction );
	}
	
	this.addTempFunction = function addTempFunction( funNewEventFunction )
	{
		this.arrTempFunctions.push( funNewEventFunction );
	}
	
	this.clearFunctions = function clearFunctions()
	{
	}
	
	this.runFunctions = function runFunctions( event )
	{
		var boolReturning = true;
		var i;
		for( i = 0; i < this.arrFunctions.length ; i++ )
		{
			if ( !IE )
			{
				with( objContainer )
				{
					if( isFunction( this.arrFunctions[ i ] ) )
					{ 
						boolReturning = boolReturning && this.arrFunctions[ i ]( event );
					}
					else
					{
						objContainer.__dinamicMethod = new Function( "event" , this.arrFunctions[ i ] );
						boolReturning = boolReturning && objContainer.__dinamicMethod( event );
					}
				}
			}
			else
			{
				if( isFunction( this.arrFunctions[ i ] ) )
				{ 
					boolReturning = boolReturning &&  this.arrFunctions[ i ]( event );
				}
				else
				{
					objContainer.__dinamicMethod = new Function( "event" , this.arrFunctions[ i ] );
					boolReturning = boolReturning &&  objContainer.__dinamicMethod( event );
				}
			}
		}
		for( i = 0; i < this.arrTempFunctions.length ; i++ )
		{
			if ( !IE )
			{
				with( objContainer )
				{
					if( isFunction( this.arrTempFunctions[ i ] ) )
					{ 
						boolReturning = boolReturning && this.arrTempFunctions[ i ]( event );
					}
					else
					{
						objContainer.__dinamicMethod = new Function( "event" , this.arrTempFunctions[ i ] );
						boolReturning = boolReturning && objContainer.__dinamicMethod( event );
					}
				}
			}
			else
			{
				if( isFunction( this.arrTempFunctions[ i ] ) )
				{ 
					this.arrTempFunctions[ i ]( event );
				}
				else
				{
					objContainer.__dinamicMethod = new Function( "event" , this.arrTempFunctions[ i ] );
					boolReturning = boolReturning && objContainer.__dinamicMethod( event );
				}
			}
		}
		this.arrTempFunctions = Array();
		return boolReturning;
	}
}

window.eventsPlacer = Array();

window.eventsController = function eventsController( objContainer )
{
	this.objContainer = objContainer;
	
//	window.eventsPlacer[ objContainer.id ] = this;
	
	this.arrEvents = Array();
	
	this.addEvent = function adEvent( strNewEvent )
	{
		this.arrEvent.push( strEvent );
	}
	
	this.getEventGroup = function getEventGroup( strEventName )
	{
		return this.arrEvents[ strEventName ];
	}
	
	this.addFunctionIntoEvent = function addFunctionIntoEvent( strEventName, funNewEventFunction , boolOnlyOneExecution )
	{
		if( ! this.objContainer ) return;
	
		if( !IE )
		{
			if( isUndefined( this.objContainer[ strEventName ] ) )
			{
				this[ strEventName ] = new Function( "event" , "return window.eventsPlacer['" + this.objContainer.id +  "'].getEventGroup('" + strEventName + "').runFunctions( event )" );
				this.objContainer.setAttribute( strEventName , "(" + this[ strEventName ] + ")" + "( event )" );
				this.objContainer[ strEventName ] = this[ strEventName ];
			}
		}
		else
		{
//			var strText = "return window.eventsPlacer['" + objContainer.id +  "'].getEventGroup('" + strEventName + "').runFunctions( event )";
			this[ strEventName ] = new Function( "event" , "return window.eventsPlacer['" + objContainer.id +  "'].getEventGroup('" + strEventName + "').runFunctions( event )" );
			this.objContainer[ strEventName ] = this[ strEventName ];
			this.objContainer.attachEvent(  strEventName , this[ strEventName ]	 );

//			this[ strEventName ] = new Function( "event" , "return window.eventsPlacer['" + objContainer.id +  "'].getEventGroup('" + strEventName + "').runFunctions( event )" );
//			this.objContainer[ strEventName ] = strText;
//			this.objContainer.attachEvent(  strEventName , strText );

		}
		if( isUndefined( this.arrEvents[ strEventName ] ) )
		{
			this.arrEvents[ strEventName ] = new eventGroup( this.objContainer );
		}
		if( boolOnlyOneExecution )
		{
			this.arrEvents[ strEventName ].addTempFunction( funNewEventFunction );
		}
		else
		{
			this.arrEvents[ strEventName ].addPermantFunction( funNewEventFunction );
		}
	}
	this.requireFunctionIntoEvent = function requireFunctionIntoEvent( strEventName, funNewEventFunction , boolOnlyOneExecution )
	{
	
		if( !IE )
		{
			if( isUndefined( this.objContainer[ strEventName ] ) )
			{
				this[ strEventName ] = new Function( "event" , "return window.eventsPlacer['" + objContainer.id +  "'].getEventGroup('" + strEventName + "').runFunctions( event )" );
				this.objContainer.setAttribute( strEventName , "(" + this[ strEventName ] + ")" + "( event )" );
				this.objContainer[ strEventName ] = this[ strEventName ];
			}
		}
		else
		{
			this[ strEventName ] = new Function( "event" , "return window.eventsPlacer['" + objContainer.id +  "'].getEventGroup('" + strEventName + "').runFunctions( event )" );
			this.objContainer[ strEventName ] = this[ strEventName ];
			this.objContainer.attachEvent(  strEventName , this[ strEventName ]	 );
		}
		if( isUndefined( this.arrEvents[ strEventName ] ) )
		{
			this.arrEvents[ strEventName ] = new eventGroup( this.objContainer );
		}
		if( boolOnlyOneExecution )
		{
			if( this.arrEvents[ strEventName ].arrTempFunctions.indexOf( funNewEventFunction ) == -1 )
			{
				this.arrEvents[ strEventName ].addTempFunction( funNewEventFunction );
			}
		}
		else
		{
			if( this.arrEvents[ strEventName ].arrFunctions.indexOf( funNewEventFunction ) == -1 )
			{
				this.arrEvents[ strEventName ].addPermantFunction( funNewEventFunction );
			}
		}
	}
}

window.addEvent = function addEvent( objElement, strEventName , funNewEventFunction , boolOnlyOneExecution_ )
{
	strId = setId( objElement , 'element' );
	
	if( boolOnlyOneExecution_ == undefined )
	{
		boolOnlyOneExecution_ = false;
	}
	
	if( IE )
	{
		if( isUndefined( window.eventsPlacer ) )
		{
			window.eventsPlacer = new Object();
		}
	}
	
	if( isUndefined( window.eventsPlacer[ objElement.id ] ) )
	{
		window.eventsPlacer[ objElement.id ] = new eventsController( objElement );
	}

	window.eventsPlacer[ objElement.id ].addFunctionIntoEvent( strEventName , funNewEventFunction , boolOnlyOneExecution_ );
}

window.requireEvent = function requireEvent( objElement, strEventName , funNewEventFunction , boolOnlyOneExecution_ )
{
	strId = setId( objElement , 'element' );
	
	if( boolOnlyOneExecution_ == undefined )
	{
		boolOnlyOneExecution_ = false;
	}
	
	if( IE )
	{
		if( isUndefined( window.eventsPlacer ) )
		{
			window.eventsPlacer = new Object();
		}
	}
	
	if( isUndefined( window.eventsPlacer[ objElement.id ] ) )
	{
		window.eventsPlacer[ objElement.id ] = new eventsController( objElement );
	}

	window.eventsPlacer[ objElement.id ].requireFunctionIntoEvent( strEventName , funNewEventFunction , boolOnlyOneExecution_ );
}

window.applyFinish = function applyFinish()
{
	if( !IE )
	{
		document.body.innerHTML += '';
	}
	
	try
	{
		if( IE )
		{
			x = document.body.onload;
			x();
		}
		else
		{
			document.body.onload();
		}
	}
	catch( e )
	{
//		alert( document.body.onload );
//		alert( e.message );	
	}
} 

window.finishing = function finishing()
{
	if( window.globalBoolXmlRequestTreeActive )
	{
		setTimeout( 'window.finishing()' , 10 );
	}
	else
	{
		window.applyFinish();
	}
}

