/**
 * Class CalendarDateGroupCollection
 * @author Thiago Mata
 * @classDescription
 */
window.CalendarDateGroupCollection = function CalendarDateGroupCollection()
{
	this.arrCalendarDateGroup = new Array();
	
	this.getCalendarDateGroupByName = function getCalendarDateGroupByName( strDateGroupName )
	{
		return this.arrCalendarDateGroup[ strDateGroupName ];
	}
	
	this.createCalendarDateGroup = function createCalendarDateGroup( strDateGroupName , strCssStyleName_ , intMaxLength_ )
	{
		var objCalendarDateGroup = new CalendarDateGroup( strDateGroupName , strCssStyleName_ );
		
		if( intMaxLength_ != undefined )
		{
			objCalendarDateGroup.intMaxLength = intMaxLength_;
		}
		this.arrCalendarDateGroup[ strDateGroupName ] = objCalendarDateGroup;
		
		
	}
	
	this.markDates = function markDates( objCalendarDisplay )
	{
		if( isUndefined( this.arrCalendarDateGroup ) )
		{
			return;
		}
		
		for( i in this.arrCalendarDateGroup )
		{
			if( IE )
			{
				if( isUndefined( this.arrCalendarDateGroup[ i ] ) )
				{
					continue;
				}
			}
			
			objCalendarDisplay.markDates(
				this.arrCalendarDateGroup[ i ].arrObjDatesElement , 
				this.arrCalendarDateGroup[ i ].strCssStyleName
			);
			
			objCalendarDisplay.unmarkDates( 
				this.arrCalendarDateGroup[ i ].arrLastRemovedDates	, 
				this.arrCalendarDateGroup[ i ].strCssStyleName 
			);
			
		}
	}
}