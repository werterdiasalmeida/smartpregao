/**
 * @author Thiago Mata
 */
/**
 * class CalendarPeriodic extends Calendar
 */
function CalendarPeriodic( strBodyId, strTitleId, strPreviousId, strNextId )
{
	/**
	 * Original Class Calendar
	 */
	that = new Calendar( strBodyId, strTitleId, strPreviousId, strNextId );

	/**
	 * Parent SelectDate Function Link
	 */
	that.parentSelectDate	= that.selectDate;

	/**
	 * Parente UseDate Function Link
	 */
	that.parentUseDate		= that.useDate;

	/**
	 * Day of start of the periodic
	 *
	 * 0 => sunday
	 * 1 => monday
	 * 2 => tuesday
	 * 3 => wednesday
	 * 4 => thursday
	 * 5 => friday
	 * 6 => saturday
	 *
	 */
	that.intStartLimitDate	= 5;

	/**
	 * Day of end of the periodic
	 */
	that.intEndLimitDate	= 4;

	/**
	 * Max of Periodics Selecteds Simultaneous
	 */
	that.intMaxPeriodics	= -1;

	/**
	 * Max of Fields Selecteds
	 */
	that.intMaxSelectedDates = -1;

	/**
	 * Array of Periodic End Dates
	 */
	that.arrEndDates 		= Array();

	/**
	 * Array of Periodic Start Date
	 */
	that.arrStartDates 		= Array();

	/**
	 * string of the css class of the start date
	 */
	that.strCssClassStartDate =  "startPeriodicDate";

	/**
	 * string of the css class of the end date
	 */
	that.strCssClassEndDate =  "endPeriodicDate";

	/**
	 * Select / Unselect some date following the rules of the object
	 * @param integer intDay
	 * @param integer intMonth
	 * @param integer intFullYear
	 * @return void
	 * @acess public
	 */
	that.selectDate = function selectDate( intDay, intMonth, intFullYear )
	{
		this.unselectAllDates();
		// datas no javascrit comecam com 0 //
		// dates in javascrit start with 0 //
		intMonth--;

		var objDate = new Date( intFullYear, intMonth , intDay );

		var objStartDate = this.getStartDate( objDate );
		var objEndDate = this.getEndDate( objDate );

		var intDay 		= objStartDate.getDate();
		var intMonth	= objStartDate.getMonth();
		var intFullYear	= objStartDate.getFullYear();

		this.addStartPeriodicDate( intDay, intMonth + 1, intFullYear );

		var intDay 		= objEndDate.getDate();
		var intMonth	= objEndDate.getMonth();
		var intFullYear	= objEndDate.getFullYear();

		this.addEndPeriodicDate( intDay, intMonth + 1, intFullYear );

		arrDates = this.getArrayOfDates( objStartDate, objEndDate );

		for( var i = 0; i < arrDates.length; ++i )
		{
			var objDateArray = arrDates[ i ] ;
			this.parentSelectDate( objDateArray[0] , objDateArray[1] + 1 , objDateArray[2] );
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

	/**
	 * Get the must close periodic start date of some other date
	 * @param Date objDate
	 * @return Date resultDate
	 */
	that.getStartDate = function getStartDate( objDate )
	{
		var intWeekDay = ( objDate.getDay() );

		var intDay 		= objDate.getDate();
		var intMonth	= objDate.getMonth();
		var intFullYear	= objDate.getFullYear();

		// to allways be the big number //
		intWeekDay += 35;

		var intDif = intWeekDay - this.intStartLimitDate;

		// back to the one week space //
		intDif %= 7;

		intDay -= intDif;

		if ( intDay < 1 )
		{
			// back one month //
			--intMonth;

			if ( intMonth < 0 )
			{
				// back one year //
				intMonth = 11;
				--intFullYear;
			}
			// min days of some month //
			var objBeforeMonthDate = new Date( intFullYear, intMonth , 27 );

			// min days of some month //
			var intMaxDate = 27;
			while ( intMaxDate <= objBeforeMonthDate.getDate() )
			{
				intMaxDate++;
				objBeforeMonthDate.setDate( intMaxDate );
			}
			intMaxDate--;

			intDay = intMaxDate + intDay;
		}
		var objDate = new Date( intFullYear, intMonth , intDay );
		return objDate;
	}

	/**
	 * Get the must close periodic end date of some other date
	 * @param Date objDate
	 * @return Date resultDate
	 */
	that.getEndDate = function getEndDate( objDate )
	{
		var intWeekDay = ( objDate.getDay() );

		var intDay 		= objDate.getDate();
		var intMonth	= objDate.getMonth();
		var intFullYear	= objDate.getFullYear();

		var intDif = this.intEndLimitDate + 35 - intWeekDay;

		intDif %= 7;

		intDay += intDif;


		// get the max of days of this month //

		var objMonthDate = new Date( intFullYear, intMonth , 27 );

		// min days of some month //
		var intMaxDate = 27;

		while ( intMaxDate <= objMonthDate.getDate() )
		{
			intMaxDate++;
			objMonthDate.setDate( intMaxDate );
		}
		intMaxDate--;

		if ( intDay > intMaxDate )
		{
			// in the next month //
			++intMonth;

			intDay -= intMaxDate;

			// next year //
			if ( intMonth > 11 )
			{
				intMonth = 0;
				++intFullYear;
			}

		}

		var objDate = new Date( intFullYear, intMonth , intDay );

		return objDate;
	}

	/**
	 * Add / Remove some date of Periodic Group following the rules of the object
	 * @param integer intDay
	 * @param integer intMonth
	 * @param integer intFullYear
	 * @return void
	 * @acess public
	 */
	that.addStartPeriodicDate = function addStartPeriodicDate( intDay, intMonth, intFullYear )
	{
		var strDate = intDay + "/" + intMonth + "/" + intFullYear;

		var intPos = array_search( strDate , this.arrStartDates );
		var arrRemovedStartDates = Array();
		var objCalendarDisplay = this.getCalendarDisplay();

		if ( intPos == -1 )
		{
			// add to array //
			this.arrStartDates[ this.arrStartDates.length ] = strDate;
			// the array could not pass the max size if this != -1 )
			if ( this.intMaxPeriodics > 0 )
			{
				while ( this.arrStartDates.length > this.intMaxPeriodics )
				{
					var objFirst = this.arrStartDates[ 0 ];
					this.arrStartDates.splice( 0, 1 );
					arrRemovedStartDates.push( objFirst );
				}
			}

		}
		else
		{
			// add to the removed array //
			arrRemovedStartDates[ arrRemovedStartDates.length ] = strDate;
			// remove from array //
			this.arrStartDates.splice( intPos , 1 );
		}

		objCalendarDisplay.unmarkDates( arrRemovedStartDates	, this.strCssClassStartDate );
		objCalendarDisplay.markDates( 	this.arrStartDates		, this.strCssClassStartDate );
	}

	/**
	 * Add / Remove some date of Periodic Group following the rules of the object
	 * @param integer intDay
	 * @param integer intMonth
	 * @param integer intFullYear
	 * @return void
	 * @acess public
	 */
	that.addEndPeriodicDate = function addEndPeriodicDate( intDay, intMonth, intFullYear )
	{
		var strDate = intDay + "/" + intMonth + "/" + intFullYear;

		var intPos = array_search( strDate , this.arrEndDates );
		var arrRemovedEndDates = Array();
		var objCalendarDisplay = this.getCalendarDisplay();

		if ( intPos == -1 )
		{
			// add to array //
			this.arrEndDates[ this.arrEndDates.length ] = strDate;
			// the array could not pass the max size if this != -1 )
			if ( this.intMaxPeriodics > 0 )
			{
				while ( this.arrEndDates.length > this.intMaxPeriodics )
				{
					var objFirst = this.arrEndDates[ 0 ];
					this.arrEndDates.splice( 0, 1 );
					arrRemovedEndDates.push( objFirst );
				}
			}

		}
		else
		{
			// add to the removed array //
			arrRemovedEndDates[ arrRemovedEndDates.length ] = strDate;
			// remove from array //
			this.arrEndDates.splice( intPos , 1 );
		}

		objCalendarDisplay.unmarkDates( arrRemovedEndDates	, this.strCssClassEndDate );
		objCalendarDisplay.markDates( 	this.arrEndDates		, this.strCssClassEndDate );
	}

	/**
	 * Get the array of the dates in use
	 * @return void
	 * @acess public
	 */
	that.getPeriodics = function getPeriodics()
	{
		var arrPeriodics = Array();

		for ( var i = 0; i < this.arrStartDates.length ; ++i )
		{
			var arrItem = Array();
			arrItem[0] = this.arrStartDates[ i ];
			arrItem[1] = this.arrEndDates[ i ];
			
			arrPeriodics.push( arrItem );
		}

		return arrPeriodics;
	}

	/**
	 * Set the array of the dates in use
	 * @return void
	 * @param Array of Array of Integer
	 * @acess public
	 */
	that.setPeriodics = function setPeriodics( arrPeriodics )
	{
		this.arrStartDates = Array();
		this.arrEndDates = Array();
		this.arrSelectedDates = Array();
		
		for ( var i = 0; i <  arrPeriodics.length; ++i )
		{
			var arrItem = arrPeriodics[ i ];
			this.arrStartDates.push( arrItem[0] );
			this.arrEndDates.push(   arrItem[1] );
			var arrSelectedDates = this.getArrayOfDates( strToDate( arrItem[0] ) , strToDate( arrItem[1] ) );

            for ( var j = 0; j < arrSelectedDates.length ; j++ )
            {
                // o mes no javascript comeca com 0 mas no string de data nao //
                arrSelectedDates[j][1]++;
                this.arrSelectedDates.push( implode( "/" , arrSelectedDates[j] ) );
            }
		}

		this.displaySpecialDates();
	}

	/**
	 * Set the array of the dates in use
	 * @return void
	 * @param Array of Array of Integer
	 * @acess public
	 */
	that.setUsedPeriodics = function setUsedPeriodics( arrPeriodics )
	{
		this.arrInUseDates = Array();
		
		for ( var i = 0; i <  arrPeriodics.length; ++i )
		{
			var arrItem = arrPeriodics[ i ];
			var arrInUseDates = this.getArrayOfDates( strToDate( arrItem[0] ) , strToDate( arrItem[1] ) );

            for ( var j = 0; j < arrInUseDates.length ; j++ )
            {
                // o mes no javascript comeca com 0 mas no string de data nao //
                arrInUseDates[j][1]++;
                this.arrInUseDates.push( implode( "/" , arrInUseDates[j] ) );
            }
		}

		this.displaySpecialDates();
	}

	/**
	 * Add to the array of the dates in use one new Periodic
	 * @return void
	 * @param Array of Integer
	 * @acess public
	 */
	that.addPeriodic = function addPeriodics( arrPeriodic )
	{

		this.arrStartDates.push(	arrPeriodic[0] );
		this.arrEndDates.push(		arrPeriodic[1] );
		
		var arrSelectedDates = this.getArrayOfDates( strToDate( arrPeriodic[0] ) , strToDate( arrPeriodic[1] ) );

        for ( var j = 0; j < arrSelectedDates.length ; j++ )
        {
            // o mes no javascript comeca com 0 mas no string de data nao //
            arrSelectedDates[j][1]++;
            this.arrSelectedDates.push( implode( "/" , arrSelectedDates[j] ) );
        }
        
		this.displaySpecialDates();
	}

	/**
	 * Refresh the classes of the dates inside the calendar
	 * @return void
	 * @acess public
	 */
	that.displaySpecialDates = function displaySpecialDates()
	{
		this.getCalendarDisplay().markDates( this.arrSelectedDates	, this.strCssClassSelectedDate );
		this.getCalendarDisplay().markDates( this.arrInUseDates		, this.strCssClassUsedDate );
		this.getCalendarDisplay().markDates( this.arrEndDates		, this.strCssClassEndDate );
		this.getCalendarDisplay().markDates( this.arrStartDates		, this.strCssClassStartDate );
	}

	return that;
}
