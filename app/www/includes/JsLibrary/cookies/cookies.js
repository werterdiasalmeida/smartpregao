//alert( 'inicial: ' + document.cookie );
window.cookieController = function cookieController()
{
	this.intExpiredDays = 100;
	
	this.arrCookiesAttributes = Array();
	
	this._construct = function _construct()
	{
		this.arrCookiesAttributes = Array();
		var arrCookies = explode( '; ' , document.cookie );
		for( var  i = 0 ; i < arrCookies.length; i++ )
		{
			var strCookie	= arrCookies[ i ];
			var arrSeparated = explode( '=' , strCookie );
			this.arrCookiesAttributes[ arrSeparated[ 0 ] ] = unescape( arrSeparated[ 1 ] );
		}
	}
	
	this._getExpireHeader = function _getExpireHeader()
	{
		var strExpiresHeader;
		if( this.intExpiredDays != 0 )
		{
			var date = new Date();
			date.setTime( date.getTime() + ( this.intExpiredDays * 24 * 60 * 60 * 1000 ) );
			strExpiresHeader = "; expires=" + date.toGMTString();
		}
		else
		{
			strExpiresHeader = '';
		}
		return strExpiresHeader;
	}
	
	this.setCookieAttribute = function setCookieAttribute( strAttributeName , strAttributeValue )
	{
		this.arrCookiesAttributes[ strAttributeName ] = strAttributeValue;
		this._saveCookiesAttributes();
	}
	
	this.getCookieAttribute = function getCookieAttribute( strAttributeName )
	{
		return this.arrCookiesAttributes[ strAttributeName ];
	}
	
	this._saveCookiesAttributes = function _saveCookiesAttributes()
	{
		var strCookies = '';
		for( var strAttribute in this.arrCookiesAttributes )
		{
			if(  this.arrCookiesAttributes[ strAttribute ] == undefined )
			{
				this.arrCookiesAttributes[ strAttribute ] = '';
			}
			strCookies += strAttribute + '=' + escape( this.arrCookiesAttributes[ strAttribute ] ) + '; ';
		}
		strCookies += this._getExpireHeader();
//		strCookies += "; path=/";
		alert( strCookies );
		document.cookie = strCookies;
	}
	
	this._construct();
}

window.objCookieControler = new cookieController();

function eraseCookie(name)
{
	createCookie(name,"",-1);
}

window.cookieElement = function cookieElement( id )
{
	this.id = null;
	this.mixValue = null;
	
	this._getCookieValue = function _getCookieValue()
	{
		this.mixValue = window.objCookieControler.getCookieAttribute( 'cookieElement' + this.id );
	}
	
	this._setCookieValue = function _setCookieValue()
	{
		window.objCookieControler.setCookieAttribute( 
			'cookieElement' + this.id , 
			window.cookieElement.instances[ this.id ].mixValue
		);
	}
	
	this._construct = function construct( id )
	{
		if( id == undefined )
		{
			this.id = window.cookieElement.instances.length;
		}
		else
		{
			this.id = id;
		}
		if( window.cookieElement.instances[ this.id ] == undefined )
		{
			window.cookieElement.instances[ this.id ] = this;
		}
		else
		{
			this.mixValue = window.cookieElement.instances[ this.id ].mixValue;
		}
		this._getCookieValue();
	}
	
	this.setValue = function setValue( mixValue )
	{
		if( mixValue == undefined )
		{
			mixValue  = '';
		}
		window.cookieElement.instances[ this.id ].mixValue = mixValue;	
		this._setCookieValue();
	}
	
	this.getValue = function getValue()
	{
		return  window.cookieElement.instances[ this.id ].mixValue;
	}
	
	this._construct( id );
}
window.cookieElement.instances = Array();