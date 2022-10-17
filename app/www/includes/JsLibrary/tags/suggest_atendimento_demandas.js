
window.Suggest = function Suggest( objTagInputElement, objDivSuggest, strSuggestAction, objHiddenElement )
{
	this.id;
	this.objTagInputElement;
	this.objHiddenElement;
	this.objDivSuggest;
	this.arrSuggestElements;
	this.strLastSuggestElement;
	this.intActualSuggestSelected;
	this.strActionSuggest;
		
	this.__construct = function __construct( objTagInputElement, objDivSuggest, strSuggestAction, objHiddenElement )
	{
		this.id = window.Suggest.arrInstances.length;
		window.Suggest.arrInstances.push( this );
		
		this.objTagInputElement = objTagInputElement;
		this.objHiddenElement = objHiddenElement;
		this.objDivSuggest = objDivSuggest;
		
		this.arrSuggestElements = Array();
		this.intActualSuggestSelected = -1;
	
		this.strActionSuggest = strSuggestAction;

		addEvent( objTagInputElement	, 'onchange'	, new Function( "event" , 'return window.Suggest.arrInstances[ ' + this.id + ' ].onChange( event )' ) );
		addEvent( objTagInputElement	, 'onkeydown'	, new Function( "event" , 'return window.Suggest.arrInstances[ ' + this.id + ' ].onKeyDown( event )' ) );
		addEvent( objTagInputElement	, 'onkeyup'		, new Function( "event" , 'return window.Suggest.arrInstances[ ' + this.id + ' ].onKeyUp( event )' ) );
	}
	
	this.onChange = function onChange( event )
	{
		this.hideSuggest();
	}
	
	this.onKeyDown = function onKeyDown( event )
	{
		var intKeyCode = window.getIntKeyCode( event );
		var strKeyType = window.getKeyType( intKeyCode );
		
		switch( strKeyType )
		{
			case 'position':
			{
				this.changePosition( intKeyCode );
				return false;
				break;
			}
			case 'enter':
			{
				this.acceptSugget();
				return false;
				break;
			}
			case 'tab':
			{
				this.hideSuggest();
				break;
			}
			case 'esc':
			{
				this.hideSuggest();
				return false;
				break;
			}
		}
		return true;
	}
	
	this.onKeyUp = function onKeyUp( event )
	{
		var intKeyCode = window.getIntKeyCode( event );
		var strKeyType = window.getKeyType( intKeyCode );
		
		switch( strKeyType )
		{
			case 'position':
			{
				return false;
				break;
			}
			case 'enter':
			{
				return false;
				break;
			}
			case 'esc':
			case 'tab':
			{
				this.hideSuggest();
				break;
			}
			case 'letter':
			case 'number':
			case 'unknown':
			default:
			{
				this.refreshSuggestElements();
				var strLastSuggestElement = this.getLastSuggestElement();
				if( strLastSuggestElement != false )
				{
					this.getSuggestList( strLastSuggestElement );
				}
				break;
			}
		}
		return true;
	}
	
	this.hideSuggest = function hideSuggest()
	{
		this.objDivSuggest.innerHTML = '';
		this.intActualSuggestSelected = -1;
	}
	
	this.changePosition = function changePosition( intKeyCode )
	{
		switch( intKeyCode )
		{
			case 40:
			{
				++this.intActualSuggestSelected;
				if( this.intActualSuggestSelected >= this.getDrawSuggestions().length )
				{
					this.intActualSuggestSelected = 0;
				}
				break;
			}
			case 38:
			{
				--this.intActualSuggestSelected;
				if( this.intActualSuggestSelected < 0 )
				{
					this.intActualSuggestSelected = this.getDrawSuggestions().length - 1;
				}
				break;
			}
			case 37:
			{
				// <= //
				break;
			}
			case 39:
			{
				// <= //
				break;
			}
		}
		this.drawSelectedElement();
	}
	
	this.cleanSelectedElement = function cleanSelectedElement()
	{
		var arrObjDivSuggesteds = this.objDivSuggest.getElementsByTagName( "div" );
		for( i = 0; i < arrObjDivSuggesteds.length; i++ )
		{
			arrObjDivSuggesteds[ this.intActualSuggestSelected ].style.backgroundColor = 'red' ;
		}
	}
	
	this.getDrawSuggestions = function getDrawSuggestions()
	{
		var objDivSuggest = this.objDivSuggest;
		if( objDivSuggest.getElementsByTagName( "div" ).length < 1 )
		{
			return Array();
		}
		var i = 0;
		while( objDivSuggest.childNodes[ i ].tagName == undefined )
		{
			i++;
		} 
		var objDivContainer = objDivSuggest.childNodes[ i ];
		var arrObjDivSuggesteds = objDivContainer.getElementsByTagName( "div" );
		return arrObjDivSuggesteds;
	}
	 
	this.drawSelectedElement = function drawSelectedElement()
	{
		var arrObjDivSuggesteds = this.getDrawSuggestions();
		for( i = 0; i < arrObjDivSuggesteds.length; i++ )
		{
			if( i == this.intActualSuggestSelected  )
			{
				arrObjDivSuggesteds[ i ].className = 'suggestMarked' ;
			}
			else
			{
				arrObjDivSuggesteds[ i ].className = 'suggestUnmarked' ;
			}
		}
	} 
	
	this.clickSuggest = function clickSuggest( intActualSuggest )
	{
		this.intActualSuggestSelected = intActualSuggest;
		this.acceptSugget();
		this.objTagInputElement.focus();
	}
		
	this.mouseOverSuggest = function mouseOverSuggest( intActualSuggest )
	{
		this.intActualSuggestSelected = intActualSuggest;
		this.drawSelectedElement();
	}
		
	this.getTagElementValue = function getTagElementValue()
	{
		var strValue;
		switch( this.objTagInputElement.tagName.toLowerCase() )
		{
			case 'input':
			{
				strValue = this.objTagInputElement.value;
				break;
			}
			case 'textarea':
			{
				strValue = this.objTagInputElement.value;
				break;
			}
			default:
			{
				throw new Error( 'Unknow Suggest Tag Type (' + this.objTagInputElement.tagName  + ')' );
			}
		}
		return trim( strValue );
	}
	
	this.setTagElementValue = function setTagElementValue( strNewValue )
	{
		switch( this.objTagInputElement.tagName.toLowerCase() )
		{
			case 'input':
			{
				this.objTagInputElement.value = strNewValue;
				break;
			}
			case 'textarea':
			{
				this.objTagInputElement.value = strNewValue;
				break;
			}
			default:
			{
				throw new Error( 'Unknow Suggest Tag Type (' + this.objTagInputElement.tagName  + ')' );
			}
		}
	}
	
	this.setValueHiddenElement = function setValueHiddenElement( valueHidden )
	{
		this.objHiddenElement.value = valueHidden;
	}
	
	this.acceptSugget = function acceptSugget()
	{
		try
		{
			if( this.intActualSuggestSelected == -1 )
			{
				return false;
			}
			var arrObjDivSuggesteds = this.getDrawSuggestions();
			var strActualSuggestion = trim( arrObjDivSuggesteds[ this.intActualSuggestSelected ].innerHTML );
			
			// Seta o value do campo hidden com o id da DIV, que contém o CPF do usuário selecionado.
			var id = trim( arrObjDivSuggesteds[ this.intActualSuggestSelected ].id );
			this.setValueHiddenElement( id );
			
			this.arrSuggestElements[ this.arrSuggestElements.length - 1 ] = strActualSuggestion;
			var strNewValue = implode( ',' , this.arrSuggestElements );
			strNewValue = replaceAll( strNewValue , '<b>' , '' );
			strNewValue = replaceAll( strNewValue , '</b>' , '' );
			strNewValue = replaceAll( strNewValue , '&gt;' , '>' );
			strNewValue = replaceAll( strNewValue , '&lt;' , '<' );
			strNewValue = replaceAll( strNewValue , '\t' , '' );
			strNewValue = replaceAll( strNewValue , '\n' , '' );
			this.setTagElementValue( strNewValue );
			this.hideSuggest();
		}
		catch( e )
		{
			
		}
	}
	
	this.getSuggestList = function getSuggestList( strWordSuggest )
	{		
		var arrSendRequestParams = Array();
		var arrLoopParams = Array();
		
		arrSendRequestParams[0] = strWordSuggest;
		arrSendRequestParams[1] = this.id;
		
		addRequest(
			this.strActionSuggest , 
			"SuggestsUsuario" , 
			arrSendRequestParams , 
			"afterGetSuggestList" , 
			this , 
			arrLoopParams 
		);
	}
	
	this.afterGetSuggestList = function afterGetSuggestList( strXml , objXml , arrLoopParams )
	{
		if( strXml != '' )
		{
			this.objDivSuggest.innerHTML = strXml;
			this.intActualSuggestSelected = 0;
			this.drawSelectedElement();
		}
		else
		{
			this.hideSuggest();
		}
	}
	 
	this.refreshSuggestElements = function refreshSuggestElements()
	{
		var strValue = this.getTagElementValue();
		var arrSuggestElements = explode( "," , strValue );
		this.arrSuggestElements = arrSuggestElements;
	}
	
	this.getLastSuggestElement = function getLastSuggestElement()
	{
		if( this.strLastSuggestElement != this.arrSuggestElements[ this.arrSuggestElements.length - 1 ] )
		{
			this.strLastSuggestElement = this.arrSuggestElements[ this.arrSuggestElements.length - 1 ]; 
			return this.strLastSuggestElement;
		}
		else
		{
			return false;
		}
	} 
	
	this.toString = function toString()
	{
		return '(' + this.id + ')';
	} 
	this.__construct( objTagInputElement , objDivSuggest , strSuggestAction, objHiddenElement );
}
window.Suggest.arrInstances = Array();
