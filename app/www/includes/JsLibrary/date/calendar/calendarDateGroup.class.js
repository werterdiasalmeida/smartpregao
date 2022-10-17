/**
 * Class CalendarDateGroup
 * @author Thiago Mata
 * @classDescription Date Group of Calendar
 */
window.CalendarDateGroup = function CalendarDateGroup( strDateGroupName , strCssStyleName_ )
{
	this.className = 'CalendarDateGroup';
	
	this.arrObjDatesElement = new Array();
	
	this.strGroupName = null;
	
	this.strCssStyleName = null;
	
	this.intMaxLength = null;
	
	this.arrLastAddedDates = new Array();
	
	this.arrLastRemovedDates = new Array();
	
	this.boolRemoveTheOldies = true;
	
	this.__construct = function __construct( strDateGroupName , strCssStyleName_ )
	{
		if( strCssStyleName_ == undefined )
		{
			strCssStyleName_ = strDateGroupName;
		}
		
		this.strGroupName = strDateGroupName;
		this.strCssStyleName = strCssStyleName_;
	}

	this.clearLastArrayDates = function clearLastArrayDates()
	{
		this.arrLastRemovedDates = new Array();
		this.arrLastAddedDates = new Array();
	}	
	
	/**
	 * Remove the first / last elements from the group if it
	 * be bigger of the maxlength  
	 */
	this.checkArrDatesLength = function checkArrDatesLength()
	{
		if ( this.intMaxLength != null )
		{
			while ( this.arrObjDatesElement.length > this.intMaxLength )
			{
				var objFirst = this.arrObjDatesElement[ 0 ];
				
				if( this.boolRemoveTheOldies )
				{
					this.arrObjDatesElement.splice( 0, 1 );
				}
				else
				{
					this.arrObjDatesElement.splice( this.arrObjDatesElement.length - 1 , 1 );
				}
				
				this.arrLastRemovedDates.push( objFirst );
				var intAddedPos = array_search( objFirst , this.arrLastAddedDates );
				if( intAddedPos != -1 )
				{
					this.arrLastAddedDates.splice( 0, 1 );
				}
			}
		}
	}
	
	/**
	 * Add date as item from this calendar group
	 * 
	 * @param {integer} intDay
	 * @param {integer} intMonth
	 * @param {integer} intFullYear
	 */
	this.addDate = function addDate( intDay , intMonth, intFullYear, boolClearLastArrayDates_ )
	{
		if( boolClearLastArrayDates_ == undefined )
		{
			boolClearLastArrayDates_ = true;
		}
		if( boolClearLastArrayDates_ )
		{
			this.clearLastArrayDates();
		}
		var strDateValue = intDay + "-" + intMonth + "-" + intFullYear;
		var intDatePos = array_search( strDateValue , this.arrObjDatesElement );
		if ( intDatePos == -1 )
		{
			this.arrObjDatesElement.push( strDateValue );
		}
		this.checkArrDatesLength();
	}
	
	/**
	 * Remove date from this calendar group if belong to it.
	 * 
	 * @param {integer} intDay
	 * @param {integer} intMonth
	 * @param {integer} intFullYear
	 */
	this.removeDate = function removeDate( intDay , intMonth, intFullYear , boolClearLastArrayDates_ )
	{
		if( boolClearLastArrayDates_ == undefined )
		{
			boolClearLastArrayDates_ = true;
		}
		if( boolClearLastArrayDates_ )
		{
			this.clearLastArrayDates();
		}
		
		var strDateValue = intDay + "-" + intMonth + "-" + intFullYear;
		var intDatePos = array_search ( strDateValue , this.arrObjDatesElement );
		if ( intDatePos != -1 )
		{
			this.arrObjDatesElement.splice( intDatePos , 1 );
		} 
		this.checkArrDatesLength();
	}
	
	/**
	 * Add a Date if not into array and remove if already is it.
	 * 
	 * @param {integer} intDay
	 * @param {integer} intMonth
	 * @param {integer} intFullYear
	 */
	this.clickDate = function clickDate( intDay , intMonth, intFullYear, boolClearLastArrayDates_ )
	{
		if( boolClearLastArrayDates_ == undefined )
		{
			boolClearLastArrayDates_ = true;
		}
		if( boolClearLastArrayDates_ )
		{
			this.clearLastArrayDates();
		}
		
		var strDateValue = intDay + "-" + intMonth + "-" + intFullYear;
		var intDatePos = array_search ( strDateValue , this.arrObjDatesElement );
		if ( intDatePos != -1 )
		{
			this.arrObjDatesElement.splice( intDatePos , 1 );
			this.arrLastRemovedDates.push( strDateValue );
		}
		else
		{
			this.arrObjDatesElement.push( strDateValue );
			this.arrLastAddedDates.push( strDateValue );
		}
		this.checkArrDatesLength();
	}
	
	this.__construct( strDateGroupName , strCssStyleName_ ); 
}