/**
 * classe Calendar
 */
window.Calendar = function Calendar( objCalendarDisplay )
{
	/**
	 * @var string name of the class
	 * @access public read only
	 */
	this.className		= 'Calendar';

	/**
	 * @var string of the css class of the selected date
	 * @access public read only
	 */
	this.strCssClassSelectedDate = 'selectDate';

	/**
	 * @var string of the css class of the used date
	 * @access public read only
	 */
	this.strCssClassUsedDate = 'usedDate';

	/**
	 * @var integer actual year of the calendar
	 * @acess public
	 */
	this.intActualYear 	= null;

	/**
	 * @var integer actual month of the calendar
	 * @acess public
	 */
	this.intActualMonth	= null;

	/**
	 * @var integer actual day of the calendar
	 * @acess public
	 */
	this.intActualDay		= null;

	/**
	 * @var integer of id of this object
	 * @acess public read only
	 */
	this.id				= null;

	/**
	 * @var array of the actual selecteds dates of the calendar
	 * @acess private
	 */
	this.objCalendarDateGroupCollection = new CalendarDateGroupCollection();

	/**
	 * @var object to control the htmls changes into calendar
	 */
	this.objCalendarDisplay = null;
	
	/**
	 * @var integer max numbers of dates selecteds null for unlimited
	 * @acess private
	 */
	this.intMaxSelectedDates	=	1;

	/**
	 * @var integer max numbers of dates used null for unlimited
	 * @acess private
	 */
	this.intMaxUsedDates		=	null;

	/**
	 * Constructor of the class
	 * @acess public
	 * @return void
	 */
	this.__construct =	function construct( objCalendarDisplay_ )
	{
		if( IE )
		{
			if( isUndefined( Calendar.instances ) )
			{
				Calendar.instances = new Array();
			}
		}
		if( this.id == null )
		{
			this.id = Calendar.instances.length;
			Calendar.instances.push(this);
		}
		
		if( !isUndefined( objCalendarDisplay_ ) ) 
		{
			this.objCalendarDateGroupCollection.createCalendarDateGroup( 'selected' , this.strCssClassSelectedDate , this.intMaxSelectedDates );
			this.objCalendarDateGroupCollection.createCalendarDateGroup( 'used' , this.strCssClassUsedDate , this.intMaxUsedDates );
			this.objCalendarDisplay = objCalendarDisplay;
		}
	}

	/**
	 * Remove the body Calendar to redraw
	 * @acess private
	 * return void
	 */
	this.clearCalendar = function clearCalendar()
	{
		this.objCalendarDisplay.clearCalendar();
	}

	/**
	 * Build the html version of this object
	 * @acess public
	 */
	this.build	=	function build()
	{
		this.objCalendarDisplay.build();
		this.displaySpecialDates();
	}

	/**
	 * Select / Unselect some date following the rules of the object
	 * @param integer intDay
	 * @param integer intMonth
	 * @param integer intFullYear
	 * @return void
	 * @acess public
	 */
	this.selectDate = function selectDate( intDay, intMonth, intFullYear )
	{
		var objCalendarDateGroup = this.objCalendarDateGroupCollection.getCalendarDateGroupByName( 'selected' );
		objCalendarDateGroup.clickDate( intDay, intMonth, intFullYear );
		this.displaySpecialDates();
	}

	this.getCalendarDisplay = function()
	{
		return this.objCalendarDisplay;
	}
	
	/**
	 * Add / Remove some date of use following the rules of the object
	 * @param integer intDay
	 * @param integer intMonth
	 * @param integer intFullYear
	 * @return void
	 * @acess public
	 */
	this.useDate = function useDate( intDay, intMonth, intFullYear , strSendDate )
	{
		var objCalendarDateGroup = this.objCalendarDateGroupCollection.getCalendarDateGroupByName( 'used' );
		objCalendarDateGroup.clickDate( intDay, intMonth, intFullYear );
		this.displaySpecialDates();
	}

	/**
	 * Back One Month into Display
	 * @return void
	 * @see CalendarDisplay.backOneMonth()
	 * @acess public
	 */
	this.backOneMonth = function backOneMonth()
	{
		this.objCalendarDisplay.backOneMonth();
		this.displaySpecialDates();
	}

	/**
	 * Advance One Month into Display
	 * @return void
	 * @see CalendarDisplay.moreOneMonth()
	 * @acess public
	 */
	this.moreOneMonth = function moreOneMonth()
	{
		this.objCalendarDisplay.moreOneMonth();
		this.displaySpecialDates();
	}

	/**
	 * Refresh the classes of the dates inside the calendar
	 * @return void
	 * @acess public
	 */
	this.displaySpecialDates = function displaySpecialDates()
	{
		this.objCalendarDateGroupCollection.markDates( this.getCalendarDisplay() );
	}

	this.unselectAllDates = function unselectAllDates()
	{
		var objCalendarDateGroupSelected = this.objCalendarDateGroupCollection.getCalendarDateGroupByName( 'selected' );
		this.getCalendarDisplay().unmarkDates( objCalendarDateGroupSelected.arrObjDatesElement , this.strCssClassSelectedDate );
		objCalendarDateGroupSelected.arrObjDatesElement = new Array();
	}

	this.unuseAllDates = function unselectAllDates()
	{
		var objCalendarDateGroupUsed = this.objCalendarDateGroupCollection.getCalendarDateGroupByName( 'used' );
		this.getCalendarDisplay().unmarkDates( objCalendarDateGroupUsed.arrObjDatesElement , this.strCssClassUsedDate );
		objCalendarDateGroupUsed.arrObjDatesElement = new Array();
	}

	/**
	 * Set the array of selecteds dates
	 * @return void
	 * @acess public
	 */
	this.setSelectedDates = function setSelectedDates( arrDates )
	{
		var objCalendarDateGroupSelected = this.objCalendarDateGroupCollection.getCalendarDateGroupByName( 'selected' );
		objCalendarDateGroupSelected.arrObjDatesElement = arrDates;
		this.displaySpecialDates();
	}

	/**
	 * Set the array of selecteds dates
	 * @return void
	 * @acess public
	 */
	this.getSelectedDates = function getSelectedDates()
	{
		var objCalendarDateGroupSelected = this.objCalendarDateGroupCollection.getCalendarDateGroupByName( 'selected' );
		var arrResult = new Array();
		for( var i = 0; i < objCalendarDateGroupSelected.arrObjDatesElement.length; i++ )
		{
			arrResult.push( strDateToObjDate( objCalendarDateGroupSelected.arrObjDatesElement[ i ] , 'd-m-Y' ) );
		}
		return arrResult;
	}
	
	/**
	 * Set the array of the dates in use
	 * @return void
	 * @acess public
	 */
	this.setInUseDates = function setUsedDates( arrDates )
	{
		var objCalendarDateGroupUsed = this.objCalendarDateGroupCollection.getCalendarDateGroupByName( 'used' );
		this.getCalendarDisplay().unmarkDates( objCalendarDateGroupUsed.arrObjDatesElement , this.strCssClassUsedDate );
		objCalendarDateGroupUsed.arrObjDatesElement = arrDates;
	}

	/**
	 * Set the array of selecteds dates
	 * @return void
	 * @acess public
	 */
	this.getInUseDates = function getInUseDates()
	{
		var objCalendarDateGroupUsed = this.objCalendarDateGroupCollection.getCalendarDateGroupByName( 'used' );
		var arrResult = new Array();
		for( var i = 0; i < objCalendarDateGroupUsed.arrObjDatesElement.length; i++ )
		{
			arrResult.push( strDateToObjDate( objCalendarDateGroupUsed.arrObjDatesElement[ i ] , 'd-m-Y' ) );
		}
		return arrResult;
	}

	this.close = function close()
	{
		this.getCalendarDisplay().hide();
		this.afterClose();
	}

	this.afterClose = function afterClose()
	{
	}
		
	// call to the constructor method //
	this.__construct( objCalendarDisplay );
}

/**
 * Array of Instances of Calendar in Memory
 *
 * @acess public static
 */ 
window.Calendar.instances = new Array();

/**
 * Static Method to get some calendar by Id
 *
 * @acess public static
 */
window.Calendar.getCalendar = function getCalendar( intCalendarId )
{
	return Calendar.instances[ parseInt( intCalendarId ) ];
}
