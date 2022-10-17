/**
 * Get the name of the month based on the month position order
 * @param integer monthNumber
 * @return string
 */
window.getPortugueseMonth = function getPortugueseMonth( monthNumber )
{
	portugueseMonth = new Array();
	portugueseMonth[0]  = "JANEIRO";
	portugueseMonth[1]  = "FEVEREIRO";
	portugueseMonth[2]  = "MAR&Ccedil;O";
	portugueseMonth[3]  = "ABRIL";
	portugueseMonth[4]  = "MAIO";
	portugueseMonth[5]  = "JUNHO";
	portugueseMonth[6]  = "JULHO";
	portugueseMonth[7]  = "AGOSTO";
	portugueseMonth[8]  = "SETEMBRO";
	portugueseMonth[9]  = "OUTUBRO";
	portugueseMonth[10] = "NOVEMBRO";
	portugueseMonth[11] = "DEZEMBRO";

	return( portugueseMonth[monthNumber] );
}

window.moreOneDay = function moreOneDay( objDate )
{
	var intDay		= objDate.getDate();
	var intMonth	= objDate.getMonth();
	var intYear		= objDate.getFullYear();

	var objDate = new Date( intYear , intMonth, intDay );
	
	++intDay;
	objDate.setDate( intDay );
	// month change //
	if( objDate.getMonth() != intMonth )
	{
		++intMonth;
		intDay = 1;
		// year change //
		if ( intMonth == 12 )
		{
			intMonth = 0;
			++intYear;
		}
		objDate = new Date( intYear , intMonth, intDay );
	}
	return objDate;
}

window.moreManyDays = function moreManyDays( objDate , intAddDays )
{
	for( var i = 0 ; i < intAddDays; ++i )
	{
		objDate = moreOneDay( objDate );
	}
	return objDate;
}

window.strDateToObjDate = function strDateToObjDate( strDate , strFormat_ , strSeparator_ )
{
	if( isUndefined( strFormat_ ) )
	{
		strFormat_ = 'Y-m-d';
	}
	if( isUndefined( strSeparator_ ) )
	{
		strSeparator_ = '-' ;
	}
	
	
	var arrMaskValue	= explode( strSeparator_ , strFormat_ );
	var arrActualValue	= explode( strSeparator_ , strDate );
	
	// -1 no mes pos no date do javascript o primeiro mes eh 0 //
	var objDate	= new Date
	(
		arrActualValue[ array_search( 'Y' , arrMaskValue ) ] , 
		arrActualValue[ array_search( 'm' , arrMaskValue ) ] - 1 , 
		arrActualValue[ array_search( 'd' , arrMaskValue ) ]
	);
	
	return objDate;
}

window.objDateToStrDate = function objDateToStrDate( objDate , strFormat_ , strSeparator_ )
{
	if( isUndefined( strFormat_ ) )
	{
		strFormat_ = 'Y-m-d';
	}
	if( isUndefined( strSeparator_ ) )
	{
		strSeparator_ = '-' ;
	}
	
	var arrDate		= new Array();
	arrDate['d']	= objDate.getDate();
	arrDate['m']	= objDate.getMonth();
	arrDate['Y']	= objDate.getFullYear();
	
	// +1 no mes pos no date do javascript o primeiro mes eh 0 //
	++arrDate['m'];
	
	strResult = '';
	var arrMaskValue = explode( strSeparator_ , strFormat_ );
	for( var i = 0; i < arrMaskValue.length; ++i )
	{
		if( i != 0 )
		{
			strResult += strSeparator_;
		}
		strResult += ( arrDate[ arrMaskValue[ i ] ] < 10 ) ? '0' + arrDate[ arrMaskValue[ i ] ] : arrDate[ arrMaskValue[ i ] ] ;
	}
	return strResult;
}