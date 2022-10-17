/**
 * @author Thiago Mata
 */
/**
 * class CalendarPeriodic extends Calendar
 */
window.CalendarProject = function CalendarProject( objCalendarDisplay )
{
	/**
	 * Original Class Calendar
	 */
	that = new Calendar(  objCalendarDisplay );
	that.self = window.CalendarProject;

	that.className		= 'CalendarProject';
	
	/**
	 * Parent SelectDate Function Link
	 */
	that.parentSelectDate	= that.selectDate;

	/**
	 * Parente UseDate Function Link
	 */
	that.parentUseDate		= that.useDate;

	/**
	 * Max of Fields Selecteds
	 */
	that.intMaxSelectedDates = 1;

	/**
	 * string of the css class of one work day
	 */
	that.strCssClassWorkDay =  "workDay";

	/**
	 * string of the css class of one work day
	 */
	that.strCssClassHoliday =  "holiday";
	
	that.intDefaultDuration = 1;
	
	that.__construct = function __construct( objCalendarDisplay_ )
	{
		this.objCalendarDateGroupCollection.createCalendarDateGroup( 'work' , this.strCssClassWorkDay );
		this.objCalendarDateGroupCollection.createCalendarDateGroup( 'holiday' , this.strCssClassHoliday );
	}
	
	that._checkIfIsAWorkDay =  function _checkIfIsAWorkDay( intDay , intMonth, intYear )
	{
		var objDate = new Date( intYear , intMonth - 1 , intDay );
		if( array_search( objDate.getDay() , this.self.arrWorkDays ) == -1 )
		{
			return false;
		}
		for( var i = 0 ; i < this.self.arrHolidays.length; ++i )
		{
			strDayHoliday = this.self.arrHolidays[ i ];
			arrDayHoliday = explode( '-' , strDayHoliday );
			if(
					( ( arrDayHoliday[ 0 ] == intDay )		|| ( arrDayHoliday[ 0 ] == '%' ) )
					&&
					( ( arrDayHoliday[ 1 ] == intMonth )	|| ( arrDayHoliday[ 1 ] == '%' ) )
					&&
					( ( arrDayHoliday[ 2 ] == intYear )		|| ( arrDayHoliday[ 2 ] == '%' ) )
				)
			{
				
				return false;
			}		
		}
		
		return true;
	}
	
	that.getArrWorkDays = function getArrWorkDays( intDay , intMonth, intYear , intDuration )
	{
		--intMonth;
		var arrResult = new Array();
		var objDate = new Date( intYear , intMonth, intDay );
		var boolFirstDate = true;
		
		while( intDuration > 0 )
		{
			if( boolFirstDate )
			{
				boolFirstDate = false;
				if( this._checkIfIsAWorkDay( intDay , intMonth + 1 , intYear ) )
				{
					--intDuration;
				}
			}
			else
			if( this._checkIfIsAWorkDay( intDay , intMonth + 1 , intYear ) )
			{
				arrResult.push( intDay + "-" + ( intMonth + 1 ) + "-" + intYear );
				--intDuration;
			}
			var objDate = new Date( intYear, intMonth , intDay );
			objDate = moreOneDay( objDate );
			intDay		= objDate.getDate();
			intMonth	= objDate.getMonth();
			intYear		= objDate.getFullYear();

		}
		return arrResult;
	}
			
	/**
	 * Select / Unselect some date following the rules of the object
	 * @param integer intDay
	 * @param integer intMonth
	 * @param integer intFullYear
	 * @return void
	 * @acess public
	 */
	that.selectDate = function selectDate( intDay, intMonth, intFullYear, intDuration_ )
	{
		this.intActualDay	= intDay;
		this.intActualMonth	= intMonth;
		this.intActualYear	= intFullYear;
		
		if( ! isUndefined( intDuration_ ) )
		{
			this.intDefaultDuration = intDuration_;
		}

		this.unselectAllDates();
		// datas no javascrit comecam com 0 //
		// dates in javascrit start with 0 //
		var objDate = new Date( intFullYear, intMonth , intDay );
		if( this.intDefaultDuration > 1 )
		{
			var arrWorkDays = this.getArrWorkDays( intDay, intMonth, intFullYear, this.intDefaultDuration );
		}
		this.unWorkAllDates( true );
		this.setWorkDates( arrWorkDays , true );
		this.parentSelectDate( intDay, intMonth, intFullYear );
	}

	that.unWorkAllDates = function unselectAllDates( boolInternal_ )
	{
		if( boolInternal_ == undefined )
		{
			boolInternal_ = false;
		}
		var objCalendarDateGroupWork = this.objCalendarDateGroupCollection.getCalendarDateGroupByName( 'work' );
		this.getCalendarDisplay().unmarkDates( objCalendarDateGroupWork.arrObjDatesElement , this.strCssClassWorkDay );
		objCalendarDateGroupWork.arrObjDatesElement = new Array();
		if( ! boolInternal_ )
		{
			this.displaySpecialDates();
		}
	}

	/**
	 * Set the array of selecteds dates
	 * @return void
	 * @acess public
	 */
	that.setWorkDates = function setWorkDates( arrDates , boolInternal_ )
	{
		if( boolInternal_ == undefined )
		{
			boolInternal_ = false;
		}	
		var objCalendarDateGroupWork = this.objCalendarDateGroupCollection.getCalendarDateGroupByName( 'work' );
		this.getCalendarDisplay().unmarkDates( objCalendarDateGroupWork.arrObjDatesElement );
		objCalendarDateGroupWork.arrObjDatesElement = arrDates;
		if( ! boolInternal_ )
		{
			this.displaySpecialDates();
		}
	}


	/**
	 * Set the array of selecteds dates
	 * @return void
	 * @acess public
	 */
	that.setHolidays = function setHolidays( arrDates , boolInternal_ )
	{
		if( boolInternal_ == undefined )
		{
			boolInternal_ = false;
		}	
		var objCalendarDateGroupHoliday = this.objCalendarDateGroupCollection.getCalendarDateGroupByName( 'holiday' );
		this.getCalendarDisplay().unmarkDates( objCalendarDateGroupHoliday.arrObjDatesElement );
		objCalendarDateGroupHoliday.arrObjDatesElement = arrDates;
		if( ! boolInternal_ )
		{
			this.displaySpecialDates();
		}
	}
	
	/**
	 * Get one Array of Date Informations Based on 2 Dates
	 * @param Date objStartDate
	 * @param Date objEndDate
	 * @return Array of Array of Integer
	 */
	that.getArrayOfDates = function getArrayOfDates( objStartDate, objEndDate )
	{
		var arrDates = Array();
		var objActualDate = objStartDate;
		var intCount = 0;

		while ( objActualDate <= objEndDate )
		{
			var intDay 		= objActualDate.getDate();
			var intMonth	= objActualDate.getMonth();
			var intFullYear	= objActualDate.getFullYear();

			arrDates[ intCount ] = new Array();
			arrDates[ intCount ][0]	= intDay;
			arrDates[ intCount ][1]	= intMonth;
			arrDates[ intCount ][2]	= intFullYear;

			intCount++;

			objActualDate.setDate( objActualDate.getDate() + 1 );
		}

		return arrDates;
	}

	that.createCssHolidaysToClosedDate = function createCssHolidaysToClosedDate()
	{
		var arrDates = new Array();
		for( var i = 0 ; i < this.self.arrHolidays.length; ++i )
		{
			strDayHoliday = this.self.arrHolidays[ i ];
			arrDayHoliday = explode( '-' , strDayHoliday );
			if(
					( ( arrDayHoliday[ 1 ] == this.intActualMonth )	|| ( arrDayHoliday[ 1 ] == '%' ) )
					&&
					( ( arrDayHoliday[ 2 ] == this.intActualYear )	|| ( arrDayHoliday[ 2 ] == '%' ) )
				)
			{
				
				arrDates.push(  arrDayHoliday[ 0 ] + '-' +  this.intActualMonth + '-' +  this.intActualYear );
			}		
		}
		this.setHolidays( arrDates , true );
	}
	
	/**
	 * Back One Month into Display
	 * @return void
	 * @see CalendarDisplay.backOneMonth()
	 * @acess public
	 */
	that.backOneMonth = function backOneMonth()
	{
		this.objCalendarDisplay.backOneMonth();
		this.createCssHolidaysToClosedDate();
		this.displaySpecialDates();
	}

	/**
	 * Advance One Month into Display
	 * @return void
	 * @see CalendarDisplay.moreOneMonth()
	 * @acess public
	 */
	that.moreOneMonth = function moreOneMonth()
	{
		this.objCalendarDisplay.moreOneMonth();
		this.createCssHolidaysToClosedDate();
		this.displaySpecialDates();
	}
	
	/**
	 * Refresh the classes of the dates inside the calendar
	 * @return void
	 * @acess public
	 */
	that.displaySpecialDates = function displaySpecialDates()
	{
		this.createCssHolidaysToClosedDate();		
		this.objCalendarDateGroupCollection.markDates( this.getCalendarDisplay() );
	}

	that.__construct( objCalendarDisplay );
	return that;
}

/**
 * 0 => sunday
 * 1 => monday
 * 2 => tuesday
 * 3 => wednesday
 * 4 => thursday
 * 5 => friday
 * 6 => saturday
 * 
 */
window.CalendarProject.arrWorkDays = Array(  1 , 2 , 3 , 4 , 5);

/**
 * Array of Holidays. day:2-month:2-year:4 Use % to the date of any year
 * @example Array( '1-1-%' , '1-3-%' );
 */
//window.CalendarProject.arrHolidays = Array( '1-1-%' , '1-3-%' );
window.CalendarProject.arrHolidays = Array();
