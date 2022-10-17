/**
 * Return a integer that identify a pressed key
 *
 * @param handler de evento de tecla
 * @return integer
 * @date 2005-02-18 18:00
 */
window.getIntKeyCode = function getIntKeyCode( keyEvent )
{
	try
	{
		var intKeyCode = ( (keyEvent.which) ? keyEvent.which : keyEvent.keyCode );
		if ( keyEvent.shiftKey )
		{
			intKeyCode += 1000;
		}
	}
	catch( e )
	{
		var intKeyCode = -1;
	}
	return intKeyCode;
}

/**
 * Return the "type" of the pressed key
 *
 * @require getIntKeyCode
 * @param integer
 * @return string
 * @date 2005-02-18 18:00
 */
staticIntLastTypeKeyCode 	= "unknown";
staticIntLastKeyCode		= -1;
window.getKeyType = function getKeyType( intKeyCode )
{
	var strType = "unknown";

	if ( !IE )
	{
		if ( staticIntLastTypeKeyCode == "special" )
		{
			strType = "letter";
		}
	 	else if( intKeyCode == 8 )
		{
			strType = "backspace";
		}
		else if( ( intKeyCode == 9 ) || ( intKeyCode == 1009 ) )
		{
			strType = "tab";
		}
	 	else if( intKeyCode == 13 )
		{
			strType = "enter";
		}
	 	else if( intKeyCode == 27 )
		{
			strType = "esc";
		}
		else if( intKeyCode == 32 )
		{
			strType = "space";
		}
		else if( ( ( intKeyCode >= 33 ) && ( intKeyCode <= 40 ) )  || ( ( intKeyCode >= 1033 ) && ( intKeyCode <= 1040 ) ) )
		{
			strType = "position";
		}
		else if( intKeyCode == 46 )
		{
			strType = "delete";
		}
//		else if( ( ( intKeyCode >= 48 ) && ( intKeyCode <= 57 ) ) || ( ( intKeyCode >= 96 ) && ( intKeyCode <= 105 ) ) )
		else if( ( intKeyCode >= 48 ) && ( intKeyCode <= 57 ) )
		{
			strType = "number";
		}
		else if( ( intKeyCode >= 96 ) && ( intKeyCode <= 107 ) )
		{
			strType = "number";
		}
		else if( ( ( intKeyCode >= 59 ) && ( intKeyCode <= 90 ) ) || ( ( intKeyCode >= 1059 ) && ( intKeyCode <= 1090 ) ) )
		{
			strType = "letter";
		}
		else if( ( intKeyCode >= 112 ) && ( intKeyCode <= 123 ) )
		{
			strType = "Fn";
		}
		else if( ( intKeyCode == 219 ) || ( intKeyCode == 1219 ) || ( intKeyCode == 222 ) || ( intKeyCode == 1222 ) )
		{
			strType = "special";
		}
	}
	else
	{
		if ( staticIntLastTypeKeyCode == "special" )
		{
			strType = "letter";
		}
	 	else if( intKeyCode == 8 )
		{
			strType = "backspace";
		}
		else if( ( intKeyCode == 9 ) || ( intKeyCode == 1009 ) )
		{
			strType = "tab";
		}
	 	else if( intKeyCode == 13 )
		{
			strType = "enter";
		}
	 	else if( intKeyCode == 27 )
		{
			strType = "esc";
		}
		else if( intKeyCode == 32 )
		{
			strType = "space";
		}
		else if( ( ( intKeyCode >= 33 ) && ( intKeyCode <= 40 ) )  || ( ( intKeyCode >= 1033 ) && ( intKeyCode <= 1040 ) ) )
		{
			strType = "position";
		}
		else if( intKeyCode == 46 )
		{
			strType = "delete";
		}
		else if( ( ( intKeyCode >= 48 ) && ( intKeyCode <= 57 ) ) || ( ( intKeyCode >= 96 ) && ( intKeyCode <= 105 ) ) )
//		else if( ( intKeyCode >= 48 ) && ( intKeyCode <= 57 ) )
		{
			strType = "number";
		}
		else if( ( ( intKeyCode >= 59 ) && ( intKeyCode <= 90 ) ) || ( ( intKeyCode >= 1059 ) && ( intKeyCode <= 1090 ) ) )
//		else if( ( ( intKeyCode >= 97 ) && ( intKeyCode <= 122 ) ) || ( ( intKeyCode >= 1097 ) && ( intKeyCode <= 1122 ) ) )
		{
			strType = "letter";
		}
		else if( ( intKeyCode >= 112 ) && ( intKeyCode <= 123 ) )
		{
			strType = "Fn";
		}
		else if( ( intKeyCode == 219 ) || ( intKeyCode == 1219 ) || ( intKeyCode == 222 ) || ( intKeyCode == 1222 ) )
		{
			strType = "special";
		}
	}
	staticIntLastKeyCode		= intKeyCode;
	staticIntLastTypeKeyCode	= strType;

	return( strType );
}

/**
 * Funcao para ser substituida conforme o contexto
 * 
 * Function to be replaced as you wish
 */
window.KeyPressed = function KeyPressed( intKeyCode , strTypeCode )
{
//	document.title = intKeyCode + ' ' + strTypeCode;	
	return true;
}

window.onBodyKeyDown = function onBodyKeyDown( event )
{
	if( event == undefined ) event = e ;
	intKeyCode = window.getIntKeyCode( event );
	strTypeCode = window.getKeyType( intKeyCode );
	return window.KeyPressed( intKeyCode, strTypeCode );
}	

/**
 * Active the function to get the mouse position
 * 
 *@return void
 */
window.activeBodyGetKey = function activeBodyGetKey()
{
	if (!IE)
	{
	//	document.captureEvents(Event.KEYDOWN);
		document.body.setAttribute( 'onkeydown' ,  'return onBodyKeyDown( event )' );
		document.body.onkeydown = 'return onBodyKeyDown( event )';
	}
	else
	{
		//document.captureEvents(Event.KEYDOWN);
		// Set-up to use getMouseXY function onkeydown
		document.body.onkeydown = function( e )
		{ 
			return onBodyKeyDown( event );
		};
	}
}
