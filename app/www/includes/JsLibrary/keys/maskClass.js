EM CONSTRUCAO
Class( "MaskClass" ,
{
	/**
	 * Check if the pressed key is a number or special key, and remove the not numerical elements of the content /
	 *
	 *
	 *
	 * @required forceInt
	 * @required forceDouble
	 * @param object objElement
	 * @return boolean
	 * @example <input type="text" onkeypress="return numberMask(this, event, false)"  />
	 */
	applyNumberMask: function applyNumberMask( objElement, event, isDouble_ , Dot_ )
	{
		isDouble_	= ( isDouble_	== undefined) ? false	: isDouble_ ;
		Dot_		= ( Dot_		== undefined) ? "."		: Dot_ ;
	
		var intKeyCode = getIntKeyCode( event );
		var strKeyType = getKeyType( intKeyCode );
	
		switch( Dot_ )
		{
			case "." :
				intDotCode = 190;
			break;
			case "," :
				intDotCode = 188;
			break;
			case ";" :
				intDotCode = 191;
			break;
			case "/" :
				intDotCode = 191;
			break;
			default:
				intDotCode = -1;
			break;
		}
	
		if ( intDotCode == -1 )
		{
			return false;
		}
	
		if ( ( isDouble_ )
		  && ( intKeyCode == intDotCode )
		  && ( array_search( Dot_ , objElement.value ) == -1 ) )
		{
			// Dot //
			return true;
		}
	
	
		if( ( strKeyType == "number" )
		 || ( strKeyType == "position" )
		 || ( strKeyType == "Fn" )
		 || ( strKeyType == "backspace" )
		 || ( strKeyType == "delete" )
		 || ( strKeyType == "tab" ) )
		{
			return true;
		}
	
		return false;
	},
	

	/**
	 * integer Mask /
	 *
	 *
	  * @required numberMask
	 * @param object objElement
	 * @return boolean
	 * @example <input type="text" onkeypress="return integerMask(this,event)"  />
	 */
	applyIntegerMask: function applyIntegerMask( objElement, event, maxlenght )
	{
		objElement.value = forceInt( objElement.value, true );
		if( maxlenght == undefined )
		{
			return( numberMask( objElement, event, false ) );
		}
		else
		{
			return( numberMask( objElement, event, false ) && maxLengthMask( objElement, event, maxlenght ) );
		}
	},
	
	/**
	 * double Mask 
	 *
	 *
	 * @required numberMask
	 * @param object objElement
	 * @return boolean
	 * @example <input type="text" onkeypress="return doubleMask(this, event, '.' )"  />
	 */
	applyDoubleMask: function applyDoubleMask( objElement, Event, Dot_ )
	{
		return numberMask( objElement, Event, true, Dot_ );
	}
	
	
});

<input type="text" onkeypress="MaskClass.Press" onkeydown="MaskClass.Down"  onkeyUp="MaskClass.Up" 
	onkeyfocus="MaskClass.Focus" onkeyBlur="MaskClass.Blur" class="Date Lalala" />
	
MaskClass.Press = function()
	