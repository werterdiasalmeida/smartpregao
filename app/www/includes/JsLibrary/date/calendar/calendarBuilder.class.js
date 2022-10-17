/**
 * classe CalendarBuilder
 */
window.CalendarBuilder = function CalendarBuilder( strPlaceId_, intFullYear_ , intMonth_ , intDay_ , intYearMinValue_ , intYearMaxValue_ , strCalendarClass_ )
{
	this.className = 'CalendarBuilder' ;
	
	this.strPlaceId = null;
	
	this.objCalendar = null;
	
	this.intFullYearMinLimit = null;
	
	this.intFullYearMaxLimit = null;
	
	this.strTemplateTagName = 'div';
	
	this.strTemplateClassName = 'calendar';
	
	this.strTemplate = ''
	+ '' 
	+ '<div class="title">'
	+	'<%MONTH%>'
	+	' '
	+	'<%YEAR%>'
	+	' '
	+ 	'<%CLOSE%>'
	+ '</div>'
	+ '<div class="tableContainer">'
	+ '<%TABLE%>'
	+ '</div>'
	+ '<div class="navigatorCalendar">'
	+	'<%BACK%>'
	+ 	'<%NEXT%>'
	+ '</div>'
	+ '';
	
	this.strTemplateId = null;
	
	this.strMonthTagName = 'span';
	
	this.strMonthClassName = 'month';
	
	this.strMonthTemplate = ''
	+ ''
	+ '<strong>'
	+	'<%MONTH NAME%>'
	+ '</strong>'
	+ '';
	
	this.strMonthId = null;
	
	this.strYearTagName = 'span';
	
	this.strYearClassName = 'year';
	
	this.strYearTemplate = ''
	+ ''
	+ '<%YEAR NAME%>'
	+ '';

	this.strYearId = null;
	
	this.strTableTagName = 'table';
	
	this.strTableClassName = 'tableMonth';
	
	this.strTableTemplate = ''
	+ ''
	+ '<thead>'
	+ 	'<tr>'
	+ 		'<th>'
	+ 			'D'
	+ 		'</th>'
	+ 		'<th>'
	+ 			'S'
	+ 		'</th>'
	+ 		'<th>'
	+ 			'T'
	+ 		'</th>'
	+ 		'<th>'
	+ 			'Q'
	+ 		'</th>'
	+ 		'<th>'
	+ 			'Q'
	+ 		'</th>'
	+ 		'<th>'
	+ 			'S'
	+ 		'</th>'
	+ 		'<th>'
	+ 			'S'
	+ 		'</th>'
	+ 	'</tr>'
	+ '</thead>'
	+ '<tbody>'
		+ '<%WEEKS%>'
	+ '</tbody>'
	+ '';
	
	this.strTableId = null;
	
	this.strWeekTagName = 'tr';
	
	this.strWeekClassName = 'tableWeek';
	
	this.strWeekTemplate = ''
	+ ''
	+	'<td class="primeira" >'
	+		'<%SUNDAY%>'
	+	'</td>'
	+	'<td>'
	+		'<%MONDAY%>'
	+	'</td>'
	+	'<td>'
	+		'<%TUESDAY%>'
	+	'</td>'
	+	'<td>'
	+		'<%WEDNESDAY%>'
	+	'</td>'
	+	'<td>'
	+		'<%THURSDAY%>'
	+	'</td>'
	+	'<td>'
	+		'<%FRIDAY%>'
	+	'</td>'
	+	'<td class="ultima">'
	+		'<%SATURDAY%>'
	+	'</td>'
	+ '';
	
	this.strWeekId = null;
	
	this.strDayTagName = 'div';
	
	this.strDayClassName = 'tableDay';
	
	this.strDayTemplate = ''
	+ '<%DAY NAME%>'
	+ '';

	this.strBackTagName = 'div';
	
	this.strBackClassName = 'pastMonth';
	
	this.strBackTemplate = ''
	+ '<img src="' + builImageLink( 'btn/calendarioMesAnterior.jpg' ) + '" />'
	+ '';
	
	this.strNextTagName = 'div';
	
	this.strNextClassName = 'nextMonth';
	
	this.strNextTemplate = ''
	+ '<img src="' + builImageLink( 'btn/calendarioMesProximo.jpg' ) + '" />'
	+ '';
	
	this.intActualFullYear = null;
	
	this.intActualMonth = null;
	
	this.boolExists = false;
	
	this.buildBackMonthLink = function buildBackMonthLink()
	{
		var objBack		= document.createElement( this.strBackTagName );
		var strBackMonth = 'javascript:' + " Calendar.getCalendar( " + this.objCalendar.id + " ).backOneMonth(); ";
		if ( objBack.tagName == 'a' )
		{
			objBack.href = 		strBackMonth;
			objBack.setAttribute( "href" , strBackMonth );
		}
		else
		{
			objBack.onclick = 	strBackMonth;
			objBack.setAttribute( "onclick" , strBackMonth );
		}
		if( this.strBackClassName )
		{
			objBack.className = this.strBackClassName;
		}
		objBack.innerHTML = this.strBackTemplate;
		return objBack;
	}
	
	this.buildNextMonthLink = function buildNextMonthLink()
	{
		var objNext		= document.createElement( this.strNextTagName );
		var strNextMonth = 'javascript:' + " Calendar.getCalendar( " + this.objCalendar.id + " ).moreOneMonth(); ";
		if ( objNext.tagName == 'a' )
		{
			objNext.href = 	strNextMonth;	
			objNext.setAttribute( "href" , strNextMonth );
		}
		else
		{
			objNext.onclick = strNextMonth;
			objNext.setAttribute( "onclick" , strNextMonth );
		}
		if( this.strNextClassName )
		{
			objNext.className = this.strNextClassName;
		}
		
		objNext.innerHTML = this.strNextTemplate;
		return objNext;
	}
	
	this.buildCalendarDay = function buildCalendarDay( intDay_ , intMonth_ , intFullYear_ )
	{
		var objToday = new Date();
		var intDay = 1;

		if ( intFullYear_ == undefined && this.intActualFullYear != null )
		{
			intFullYear_ = this.intActualFullYear;
		}

		if ( intMonth_ == undefined && this.intActualMonth != null )
		{
			intMonth_ = this.intActualMonth;
		}
		else
		{
			// mes no javascript comeca com 0 //
			--intMonth_;
		}

		if ( intDay_ == undefined && this.intActualDay != null )
		{
			intDay_ = this.intActualDay;
		}
		else if ( intDay_ == undefined )
		{
			intDay_ = 1;
		}

		// instancia a data //
		if( intFullYear_ != undefined && intMonth_ != undefined )
		{
			// get the date as the received values //
			var objDate = new Date( intFullYear_, intMonth_ , intDay_ );
		}
		else
		{
			// the default value is this year and this month //
			var objDate = new Date();

			// just in case change the mode who the day is sended to here //
			if ( intDay_  )
			{
				var objDate = new Date( objDate.getFullYear() , objDate.getMonth() , intDay_ );
			}
		}

		// redefined the month and year to the case a change of year //
		// redefinicao de mes e ano por caso de uma transicao de anos //

		var intMonth_		= objDate.getMonth();
		var intFullYear_	= objDate.getFullYear();
		var intDay_			= objDate.getDate();

		this.intActualMonth 	= intMonth_;
		this.intActualFullYear 	= intFullYear_;
		this.intActualDay 		= intDay_;

	}
	
	this.buildMonth = function buildMonth()
	{
		if( this.strMonthId == null )
		{
			var objMonth	= document.createElement( this.strMonthTagName );
			this.strMonthId = setId( objMonth , 'month' );
		}
		else
		{
			var objMonth = document.getElementById( this.strMonthId );
		}
		
		var strMonthInnerHTML = createHtmlByElements
		(
			this.strMonthTemplate ,
			'<%MONTH NAME%>' , 
			getPortugueseMonth( this.intActualMonth )
		);
		
		if( this.strMonthClassName != '' )
		{
			objMonth.className = this.strMonthClassName;
		}
		
		objMonth.innerHTML = strMonthInnerHTML;
		return objMonth;
	}
	
	this.buildYear = function buildYear()
	{
		if( this.strYearId == null )
		{
			var objYear		= document.createElement( this.strYearTagName );
			this.strYearId = setId( objYear , 'year' );
		}
		else
		{
			var objYear = document.getElementById( this.strYearId );
		}
		var strYearInnerHTML = createHtmlByElements
		(
			this.strYearTemplate ,
			'<%YEAR NAME%>' , 
			this.intActualFullYear
		);
		
		if( this.strYearClassName != '' )
		{
			objYear.className = this.strYearClassName;
		}
		objYear.innerHTML = strYearInnerHTML;
		return objYear;
	}
	
	this.buildCalendarDiv = function buildCalendar( objMonth, objYear, objTable, objBack, objNext )
	{
		if( this.strTemplateId == null )
		{
			var objCalendarDiv = document.createElement( this.strTemplateTagName );
			this.strTemplateId = setId( objCalendarDiv , 'calendarDiv' );
		}
		else
		{
			var objCalendarDiv = document.getElementById( this.strTemplateId );
		}
				
		if( this.strTemplateClassName != '' )
		{
			objCalendarDiv.className = this.strTemplateClassName;
		}
		
		strHtmlButton = createButton( 'fechar.gif' , 'fechar_pressionado.gif', 'botaoExtremoDireito', 
			'Calendar.getCalendar( ' + this.id + ' ).close()' );
			
		if ( !IE )
		{
			var strCalendarInnerHTML = createHtmlByElements
			(
				this.strTemplate	,
				'<%CLOSE%>'			,
				strHtmlButton 		,
				'<%MONTH%>'	,
				objMonth 	,
				'<%YEAR%>' 	,
				objYear 	,
				'<%TABLE%>' ,
				objTable	,
				'<%BACK%>'	,
				objBack		,
				'<%NEXT%>'	,
				objNext
			);
		}
		else
		{
			var strCalendarInnerHTML = createHtmlByElements
			(
				this.strTemplate	,
				'<%CLOSE%>'			,
				strHtmlButton		,
				'<%MONTH%>'	,
				objMonth 	,
				'<%YEAR%>' 	,
				objYear 	,
				'<%TABLE%>' ,
				objTable.outerHTML + '<table><tr><td></td></tr></table>'	,
				'<%BACK%>'	,
				objBack		,
				'<%NEXT%>'	,
				objNext
			);
		}
		
		objCalendarDiv.innerHTML = strCalendarInnerHTML;
		return objCalendarDiv;
	}
	
	this.build = function build()
	{
		var objMonth		= this.buildMonth();
		var objYear			= this.buildYear();
		var objBack			= this.buildBackMonthLink();
		var objNext			= this.buildNextMonthLink();
		var objTable		= this.buildTableCalendar( this.intActualDay , this.intActualMonth, this.intActualFullYear );
		
		var objPlace;
		strPlaceId_ = this.strPlaceId;
		
		if( strPlaceId_ != null )
		{
			objPlace = document.getElementById( strPlaceId_ );
		}
		else
		{
			objPlace = undefined;
		}
		
		if( this.boolExists == false )
		{
			var objCalendarDiv	= this.buildCalendarDiv( objMonth, objYear, objTable, objBack, objNext );
			if( !IE )
			{
				print( getHtmlString( objCalendarDiv ) , objPlace );
			}
			else
			{
				print( objCalendarDiv.outerHTML , objPlace );
			}
		}
		this.boolExists = true;
	}
	/**
	 * Constructor of the class
	 * @acess public
	 * @return void
	 */
	this.__construct =	function __construct( strPlaceId_, intFullYear_ , intMonth_ , intDay_ , intYearMinValue_ , intYearMaxValue_, strCalendarClass_ )
	{
		if( intYearMinValue_ == undefined )
		{
			intYearMinValue_ = 0;
		}
		if( intYearMaxValue_ == undefined )
		{
			intYearMaxValue_ = 9999;
		}
		if( strCalendarClass_ == undefined )
		{
			strCalendarClass_ = 'Calendar';
		}
		
		if( strPlaceId_ != undefined )
		{
			this.strPlaceId = strPlaceId_;
		}
		
		// set this year min value to the const of the system //
		this.intFullYearMaxLimit = intYearMaxValue_;
		
		if( IE )
		{
			if( isUndefined( CalendarBuilder.instances ) )
			{
				CalendarBuilder.instances = new Array();
			}
		}
		this.id = CalendarBuilder.instances.length;
		CalendarBuilder.instances.push( this );
	
		this.buildCalendarDay( intDay_ , intMonth_ , intFullYear_ );

		this.objCalendar = new window[ strCalendarClass_ ]( this );
		
		this.objCalendar.build();
		this.markToday();
	}
	
	/**
	 * Build the Body Calendar inside of the BodyCalendar Element ( should be a table )
	 * @param integer intDay
	 * @param integer intMonth
	 * @param integer intFullYear
	 * @param object Date objDate
	 * @return void
 	 * @acess private
	 */
	this.buildTableCalendar = function buildTableCalendar( intDay, intMonth, intFullYear, objDate_ )
	{
		if( objDate_ == undefined )
		{
			var objDate_ = new Date( intFullYear , intMonth, intDay );
		}

		if( this.strTableId == null )
		{
			var objTable	= document.createElement( this.strTableTagName );
			this.strTableId = setId( objTable , 'calendarTable' ); 
		}
		else
		{
			var objTable	= document.getElementById( this.strTableId );
		}
		
		arrWeekDays = Array( '<%SUNDAY%>','<%MONDAY%>','<%TUESDAY%>','<%WEDNESDAY%>','<%THURSDAY%>','<%FRIDAY%>','<%SATURDAY%>' );
		arrWeeks = Array();
		
		while( intMonth == objDate_.getMonth() && intDay < 31 )
		{
			strWeekInnerHTML = this.strWeekTemplate;
			
			for( var intWeek = 0 ; intWeek <= 6 ; intWeek++ )
			{
				// cria a celula
				objDay = document.createElement( this.strDayTagName );


				// dias antes do dia 1 nesta semana
				if( intDay == 1 && intWeek < objDate_.getDay() )
				{
					objDay.innerHTML = "&nbsp;";
				}
				// dias depois do final do mes nesta semana
				else if( intMonth != objDate_.getMonth() )
				{
					objDay.innerHTML = "&nbsp;";
					intDay++;
					objDate_.setDate( intDay );
					
				}
				else
				{
					objDay.setAttribute( "id" , this.objCalendar.id  + "-" + objDate_.getDate() + "," + ( intMonth + 1 ) + "," + intFullYear );
					var strCellLink = "Calendar.getCalendar( " + this.objCalendar.id + " ).selectDate( " + objDate_.getDate() + "," + ( intMonth + 1 ) + "," + intFullYear + ");";

					if( this.strDayClassName != '' )
					{
						objDay.className = this.strDayClassName;
					}
					
					if( objDay.tagName == 'a' )
					{
						objDay.href = "javascript:" + strCellLink ;
						objDay.setAttribute( "href" , "javascript:" + strCellLink );
					}
					else
					{
						objDay.onclick = "javascript:" + strCellLink ;
						objDay.setAttribute( "onclick" , "javascript:" + strCellLink );
					}
					
					var strDayInnerHTML = createHtmlByElements
					(
						this.strDayTemplate ,
						'<%DAY NAME%>' , 
						objDate_.getDate()
					);
					
					
					objDay.innerHTML = strDayInnerHTML;
					intDay++;
//					objDate_.setDate( intDay );
					var objDate_ = new Date( intFullYear , intMonth, intDay );
				}

				
				strWeekInnerHTML  = createHtmlByElements
				(
					strWeekInnerHTML ,
					arrWeekDays[ intWeek ] , 
					objDay
				)
			}
			arrWeeks.push( '<' + this.strWeekTagName + '>' + strWeekInnerHTML + '<' + this.strWeekTagName + '/>' );
		}
		var strTableInnerHTML  = createHtmlByElements
		(
			this.strTableTemplate ,
			'<%WEEKS%>' , 
			implode( '' , arrWeeks )
		)
		if( this.strTableClassName != '' )
		{
			objTable.className 	= this.strTableClassName;
		}
		if( !IE )
		{
			objTable.innerHTML 	= strTableInnerHTML;
		}
		else
		{
			if( this.strTableTagName == 'table' )
			{
				/**
				 * No Iternet Explorera propriedade InnerHTML dos elementos 
				 * TABLE, TFOOT, THEAD e TR são somente leitura.
				 * @see http://support.microsoft.com/kb/239832/pt-br
				 */
				
				var objDivTemp = document.createElement( "div" );
				objDivTemp.innerHTML = '<' + this.strTableTagName + '>' + strTableInnerHTML + '<' + this.strTableTagName + '/>' ;
				var objNewTable = objDivTemp.getElementsByTagName( "table" )[ 0 ];
				
				objNewTable.id = objTable.id;
				objNewTable.className = this.strTableClassName;
				
				if( objTable.innerHTML == '' )
				{
					objTable = objNewTable;
				}
				else
				{
					objTable.parentNode.replaceChild( objNewTable , objTable );
				}
			}
			else
			{
				objTable.innerHTML 	= strTableInnerHTML;
			}
		}
		
		return objTable;
	}	
	
	/**
	 * Get Some object HTML from the calendar based in his id by date
	 * @param string strDate
	 * @return object TR
	 * @acess public
	 */
	this.getDate = function getDate( strDate )
	{
		var objDate = document.getElementById( this.objCalendar.id + "-" + replaceAll( strDate , "-" , "," ) );
		return objDate;
	}
	
	/**
	 * Make the changes of years when allowed
	 */
	this.validateMonthChange = function validateMonthChange()
	{
		if ( this.intActualMonth < 0 )
		{
			--this.intActualFullYear;
			this.intActualMonth = 11;
		}
		if ( this.intActualMonth > 11 )
		{
			++this.intActualFullYear;
			this.intActualMonth = 0;
		}
		
		if ( ( this.intActualFullYear < this.intFullYearMin ) || ( this.intActualFullYear > this.intFullYearMax ) )
		{
			if ( this.intActualFullYear < this.intFullYearMin )
			{
				this.intActualFullYear = this.intFullYearMin;
				this.intActualMonth = 0;
			}
			else if ( this.intActualFullYear > this.intFullYearMax )
			{
				this.intActualFullYear = this.intFullYearMax;
				this.intActualMonth = 11;
			}
		}
		if( this.objCalendar != null )
		{
			this.objCalendar.intActualDay	= 1;
			this.objCalendar.intActualMonth = this.intActualMonth + 1;
			this.objCalendar.intActualYear	= this.intActualFullYear;
		}
	}
	/**
	 * Back the calendar one month
	 * @return void
	 * @acess public
	 */
	this.backOneMonth = function backOneMonth()
	{
		if ( this.intActualMonth != undefined )
		{
			--this.intActualMonth;
		}
		this.validateMonthChange();
		this.objCalendar.build();
	}	
		
	/**
	 * Advance the calendar one month
	 * @return void
	 * @acess public
	 */
	this.moreOneMonth = function moreOneMonth()
	{
		if ( this.intActualMonth != undefined )
		{
			++this.intActualMonth;
		}
		this.validateMonthChange();
		this.objCalendar.build();
	}
		
	/**
	 * Mark the dates of the calendar with some class name
	 * @param array of string arrDates
	 * @param string strClassName
	 * @param bool boolClear_
	 * @return void
	 * @acess public
	 */
	this.markDates = function markDates( arrDates, strClassName, boolClear_ )
	{
		if ( boolClear_ == undefined )
		{
			boolClear_ = false;
		}

		if ( ! isArray( arrDates ) )
		{
			// error //
			return false;
		}

		if (  arrDates.length == 0 )
		{
			return false;
		}

		for( var i = 0 ; i < arrDates.length; i++ )
		{
			var strDate = arrDates[ i ];

			var objDate = this.getDate( strDate );

			if ( objDate != null )
			{
				if ( objDate.className == undefined )
				{
					objDate.className = "";
				}

				if ( boolClear_ )
				{
					objDate.className = " " + strClassName ;
				}
				else
				{

					// remove if already exist //
					objDate.className = replaceAll( objDate.className , " " + strClassName	, "" );
					objDate.className = replaceAll( objDate.className , strClassName		, "" );
					// insert in the end the class name //
					objDate.className = objDate.className + " " + strClassName ;

				}

			}
		}
		//this.doDraw( this.getBodyId() );
		
		return true;
	}

	/**
	 * Remove some class css for some dates of the calendar
	 * @param array of string arrDates
	 * @param string strClassName
	 * @param bool boolClear_
	 * @return void
	 * @acess public
	 */
	this.unmarkDates = function unmarkDates( arrDates, strClassName, boolClear_ )
	{
		if ( boolClear_ == undefined )
		{
			boolClear_ = false;
		}

		if ( ! isArray( arrDates ) )
		{
			// error //
			return false;
		}

		for( var i = 0; i < arrDates.length ; i++ )
		{
			var strDate = arrDates[ i ];
			var objDate = this.getDate( strDate );
			if ( objDate != null)
			{
				if ( !boolClear_ && objDate.className )
				{
					objDate.className = replaceAll( objDate.className , " " + strClassName	, "" );
					objDate.className = replaceAll( objDate.className , strClassName		, "" );
				}
				else
				{
					objDate.className = "";
				}
			}
		}

		//this.doDraw( this.getBodyId() );
		
		return true;
	}

	/**
	 * Mark the actual date of the calendar
	 * @param date objToday
	 * @return void
	 * @acess private
	 */
	this.markToday = function markToday()
	{
		objToday = new Date();
		var intMonth	= objToday.getMonth();
		var intFullYear	= objToday.getFullYear();
		var intDay		= objToday.getDate();

		var objToday = document.getElementById( this.objCalendar.id + "-" + intDay + "," + ( intMonth + 1 ) + "," + intFullYear );
		if ( objToday != null )
		{
			objToday.className += " " + this.strCssTodayDate ;
		}
	}

	this.hide = function hide()
	{
		document.getElementById( strPlaceId_ ).style.display = 'none';
	}
	
	this.__construct( strPlaceId_, intFullYear_ , intMonth_ , intDay_ , intYearMinValue_ , intYearMaxValue_ , strCalendarClass_);
}
/**
 * Array of Instances of CalendarBuilder in Memory
 *
 * @acess public static
 */
window.CalendarBuilder.instances = new Array();
/**
 * Static Method to get some calendar by Id
 *
 * @acess public static
 */
window.CalendarBuilder.getCalendar = function getCalendar( intCalendarId )
{
	return CalendarBuilder.instances[ parseInt( intCalendarId ) ];
}

/**
 * @see superTitle.css
 * @date 13-12-2006
 */
window.initializeCalendar = function initializeCalendar()
{
	importCssStyle( 'date/calendar/calendar.css' );
}


initializeCalendar();
