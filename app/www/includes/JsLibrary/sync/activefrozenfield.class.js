window.ActiveFrozenField = function ActiveFrozenField( objContainer_ , object_ , identifier_ )
{
	this.objContainer = null;
	
	this.objContainerId = null;
	
	this.mask = null;

	this.tagName = null;
			
	this.objectServerSide	= null;
	
	this.objectServerSideId	= null;
	
	this.attributeServerSide = null;
	
	this.action = null;
	
	this.checkMethod = null;
	
	this.originalValue = null;
	
	this.newValue = null;
	
	this.__construct = function __construct( objContainer_ , object_ , identifier_ )
	{
		if( IE )
		{
			if( isUndefined( window.ActiveFrozenField.instances ) )
			{
				window.ActiveFrozenField.instances = new Array();
			}
		}
		if( this.id == null )
		{
			this.id = window.ActiveFrozenField.instances.length;
			window.ActiveFrozenField.instances.push(this);
		}
		
		if( objContainer_ != undefined )
		{
			this.setObjContainer( objContainer_ );
			this.objectServerSide		= object_ 		? object_ 		: this.getObjContainer().getAttribute( 'object' );
			this.objectServerSideId		= identifier_ 	? identifier_ 	: this.getObjContainer().getAttribute( 'identifier' );
			
			this.tagName 				= ( this.getObjContainer().tagName + '' ).toLowerCase();
			this.mask 					= this.getObjContainer().getAttribute( 'mask' );
			this.attributeServerSide	= this.getObjContainer().getAttribute( 'attribute' );
			this.action					= this.getObjContainer().getAttribute( 'action' );
			this.checkMethod			= this.getObjContainer().getAttribute( 'checkMethod' );
			this.onLoad();
			objContainer_.className = 'activeFrozenFieldEnabled';
		}
	}
	
	this.getObjContainer  = function getObjContainer()
	{
		try
		{
			return document.getElementById( this.objContainerId );
		}
		catch( e )
		{
			print( e.message );
			throw e;
		}
	}
	
	this.setObjContainer = function setObjContainer( objContainer )
	{
		this.objContainer = objContainer;
		strId = setId( this.objContainer , "container" );
		this.objContainerId = strId;
		this.objContainer.objActiveFrozenField = this;
	}
	
	this.onLoad = function onLoad( )
	{
		switch( this.tagName )	
		{
			case 'span':
			{
				switch( this.mask  )
				{
					case 'string':
					case 'readonly':
					{
					
						strFullValue = this.getObjContainer().innerHTML;
						this.getObjContainer().innerHTML = "-";
						if( !IE )
						{
//							offsetHeight = 14; // altura de uma linha
							offsetHeight = 21; // altura de uma linha

						}
						else
						{
							offsetHeight = 26; // altura de uma linha
						}
						
//						document.title = offsetHeight;
		
						if( ( this.getObjContainer().parentNode.parentNode.offsetHeight > offsetHeight ) )
//						if( ( this.getObjContainer().parentNode.offsetHeight > offsetHeight ) )
						{
							this.getObjContainer().innerHTML = this.getObjContainer().innerHTML + "...";
						}
	
						if( this.id == 35 )
						{					
//							alert( 'atual: ' + this.getObjContainer().parentNode.parentNode.offsetHeight ) 
//							alert( 'meta: ' + offsetHeight  );
//							alert( 'tamanho ' + this.getObjContainer().innerHTML.length );
						}
						
//						while( this.getObjContainer().parentNode.offsetHeight > offsetHeight && this.getObjContainer().innerHTML.length > 7 )
						while	(
									( this.getObjContainer().parentNode.parentNode.offsetHeight > offsetHeight )
								&&
									( this.getObjContainer().innerHTML.length > 7 )
								)
						{
//							document.title += '*';
							strActualValue = "" + this.getObjContainer().innerHTML;
							strActualValue = strActualValue.substring( 0 , ( (strActualValue.length - 1 )  - 3 - 1) );
							strActualValue = strActualValue  + "...";
							this.getObjContainer().innerHTML = strActualValue;
						}
						
						if( this.getObjContainer().innerHTML.length <= 7 )
						{
							// nao apenas é este campo o restritor da linha
							this.getObjContainer().innerHTML = strFullValue;
							if( this.getObjContainer().innerHTML.length > 40 )
							{
								this.getObjContainer().innerHTML = this.getObjContainer().innerHTML.substring( 0 , 40 ) + '...';
							}
						}
						
						break;
					}
					case 'integer':
					{
						break;
					}
					case 'date':
					{
						break;
					}
					case 'check':
					{
						break;
					}
					default:
					{
						throw new Error( 'Inspectable Mask Type ' + this.mask );
						break;
					}
				}
				break;
			}
			case 'input':
			{
				switch( this.mask  )
				{
					case 'readonly':
					{
						break;
					}
					case 'checkbox':
					{
						this.getObjContainer().setAttribute( 'disabled' , 'false' );
						this.getObjContainer().disabled = false;
						this.getObjContainer().disabled = undefined;
						this.getObjContainer().value = this.getObjContainer().checked ? 'on' : 'off';
						break;
					}
					default:
					{
						this.getObjContainer().setAttribute( 'disabled' , 'false' );
						this.getObjContainer().disabled = false;
						this.getObjContainer().disabled = undefined;
						this.originalValue = this.getObjContainer().value;
						break;
					}
				}
			}
			case 'readonly':
			{
				break;
			}
			default:
			{
				throw new Error( 'Inspectable TagName ' + this.tagName );
				break;
			}
		}
		this.getObjContainer().className = 'activeFrozenFieldEnabled';
	}
	
	this._addInputField = function _addInputField( strMaskType, intMaxLenght )
	{
		// guardando as funcoes que se perderiam na alteracao //
		var arrFunctions = Array();
		
		
		
		// executando a alteracao //
		if( !IE )
		{
			intWidth = this.getObjContainer().parentNode.parentNode.offsetWidth * 0.9;
			intWidth -= forceInt( this.getObjContainer().parentNode.parentNode.style.paddingLeft );
			
			var objInput = document.createElement( 'input' );
			objInput.type = "text";
			objInput.className = this.getObjContainer().parentNode.className;
			objInput.style.width = intWidth + "px" ;
			
			if( this.getObjContainer().title != "" )
			{
				objInput.setAttribute( "value" , this.getObjContainer().title );
				objInput.value = this.getObjContainer().title;
			}
			else if( this.getObjContainer().innerHTML != '-' )
			{
				objInput.setAttribute( "value" , this.getObjContainer().innerHTML );
				objInput.value = trim( this.getObjContainer().innerHTML );
			}
			else
			{
				objInput.value = '';
			}
			this.getObjContainer().innerHTML = '';
			this.getObjContainer().appendChild( objInput );
		
			objInput.setAttribute('completeselectedindex', 	"off" );
			objInput.setAttribute('autocomplete', 			"off" );
			objInput.setAttribute('maxlength'	, intMaxLenght );
			objInput.setAttribute('onkeyup'		, "return false;" );
			objInput.setAttribute('class'		, "inputActiveFrozenField" );
			
			if( strMaskType != "String" )
			{
				objInput.setAttribute('onkeydown'	, "return delegateApplyMask( this , '" + strMaskType + "' , event , true , '.' , '' ,  0 , " + intMaxLenght + " )" );
				objInput.setAttribute('onkeypress'	, "return delegateApplyMask( this , '" + strMaskType + "' , event , true , '.' , '' ,  0 , " + intMaxLenght + " )" );
				objInput.setAttribute('onfocus'		, "return delegateApplyMask( this , '" + strMaskType + "' , event , true , '.' , '' ,  0 , " + intMaxLenght + " )" );
				objInput.setAttribute('onblur'		, "checkValue( this , '" + strMaskType + "' ); formatInput(this , '" + strMaskType + "' );" );
			}
		}
		if ( IE )
		{
			
			
			intWidth = forceInt( this.getObjContainer().parentNode.offsetWidth ) * 0.8;
			intWidth -= forceInt( this.getObjContainer().parentNode.style.paddingLeft );
			intWidth -= 5;
			var objInput = document.createElement( 'input' );
			objInput.type = "text";
			objInput.className = this.getObjContainer().parentNode.className;
			objInput.style.width = intWidth + "px" ;
			
			if( this.getObjContainer().title != "" )
			{
				objInput.setAttribute( "value" , this.getObjContainer().title );
				objInput.value = this.getObjContainer().title;
			}
			else if( this.getObjContainer().innerHTML != '-' )
			{
				objInput.value = trim( this.getObjContainer().innerHTML );
				objInput.setAttribute( "value" , this.getObjContainer().innerHTML );
			}
			else
			{
				objInput.value = '';
			}
			this.getObjContainer().innerHTML = '';
			this.getObjContainer().appendChild( objInput );
			
			objInput.maxlength 	=	intMaxLenght;
			objInput.className	=	"inputActiveFrozenField";
			objInput.onkeyup	=	( "return false;" );
			objInput.onkeydown	=	( "return delegateApplyMask( this , '" + strMaskType + "' , event , true , '.' , '' ,  0 , " + intMaxLenght + " )" );
			objInput.onkeypress	=	( "return delegateApplyMask( this , '" + strMaskType + "' , event , true , '.' , '' ,  0 , " + intMaxLenght + " )" );
			objInput.onfocus	=	( "return delegateApplyMask( this , '" + strMaskType + "' , event , true , '.' , '' ,  0 , " + intMaxLenght + " )" );
			objInput.onblur		=	( "checkValue( this , '" + strMaskType + "' );formatInput(this , '" + strMaskType + "' );" );
			objInput.parentNode.innerHTML += '' ;
		}

		// recuperando os dados perdidos na alteracao //
		this.getObjContainer().objActiveFrozenField = this;
		for( strAttribute in arrFunctions )
		{
			if( !isNative( arrFunctions[ strAttribute ] ) && !isUndefined( arrFunctions[ strAttribute ] ) )
			{
				try
				{
					this.getObjContainer()[ strAttribute ] = arrFunctions[ strAttribute ];				
				}
				catch( objError )
				{
					print( e.message );
				}
			}
		}
		// finalizando		
		this.doFocus();
	}
	
	this.addDateInputField = function addDateInputField()
	{
		this._addInputField( 'date' , 10 );
	}
	
	this.addIntegerInputField = function addIntegerInputField()
	{
		this._addInputField( 'integer' , 5 );
	}
	
	this.addStringInputField = function addStringInputField()
	{
		this._addInputField( 'string' , 255 );
	}

	this.toString = function toString()
	{
		return this.objContainerId 
		+ "(" 
		+ this.id 
		+ ") {" 
		+ "objectServerSide:(" 
		+ this.objectServerSide 
		+ ") ;objectServerSideId:(" 
		+ this.objectServerSideId 
		+ ") ;attributeServerSide:(" 
		+ this.attributeServerSide
		+ ") }";
		
	}
	this.removeOtherInstances = function removeOtherInstances()
	{
//		document.title = 'removing ' + window.ActiveFrozenField.actualElement;
		for( var  i = 0 ; i < window.ActiveFrozenField.instances.length ; ++i )
		{
			objActualElement = window.ActiveFrozenField.instances[ i ];
			if( objActualElement !=  this )
			{
				if( objActualElement == window.ActiveFrozenField.actualElement )
				{
					objActualElement.submit();
				}
				else
				{
					//objActualElement.cancel();
				}
			}
		}
	}
	
	this.edit = function edit()
	{
		this.removeOtherInstances();
		window.ActiveFrozenField.actualElement = this;

		if ( this.getObjContainer().getElementsByTagName( '*' ).length == 0 )
		{
			switch( this.tagName )
			{
				case 'span':
				{
					this.originalValue = trim( this.getObjContainer().innerHTML );
					switch( this.mask  )
					{
						case 'string':
						{
							this.addStringInputField();
							break;
						}
						case 'integer':
						{
							this.addIntegerInputField();
							break;
						}
						case 'date':
						{
							this.addDateInputField();
							break;
						}
						case 'check':
						{
							break;
						}
						case 'readonly':
						{
							break;
						}
						default:
						{
							throw new Error( 'Inspectable Mask Type ' + this.mask );
							break;
						}
					}
					break;
				}
				case 'input':
				{
					switch( this.mask )
					{
						case 'readonly':
						{
							break;
						}
						case 'checkbox':
						{
							this.getObjContainer().focus();
							break;
						}
						default:
						{
							throw new Error( 'Inspectable Mask Type ' + this.mask );
							break;
						}
					}
					break;
				}
				default:
				{
					throw new Error( 'Inspectable TagName ' + this.tagName );
					break;
				}
			}
		}
	}
	
	this.onclick = function onclick( event )
	{		
		switch( this.tagName )
		{
			case 'span':
			{
				this.originalValue = trim( this.getObjContainer().innerHTML );
				switch( this.mask  )
				{
					case 'string':
					case 'integer':
					case 'date':
					case 'readonly':
					{
//						if ( this.getObjContainer().getElementsByTagName( '*' ).length == 0 )
						{
							this.edit();
						}
						break;
					}
					case 'check':
					{
						this.submit();
						break;
					}
					default:
					{
						throw new Error( 'Inspectable Mask Type ' + this.mask );
						break;
					}
				}
				break;
			}
			case 'input':
			{
				switch( this.mask )
				{
					case 'readonly':
					{
						break;
					}
					case 'checkbox':
					{
						var boolOriginalValue = ( ( this.getObjContainer().value + '' ) == 'on' );
						var boolNewValue = !boolOriginalValue;
						this.originalValue = boolOriginalValue;
						this.newValue = boolNewValue;
						this.submit();
						break;
					}
					default:
					{
						throw new Error( 'Inspectable Mask Type ' + this.mask );
						break;
					}
				}
				break;
			}
			default:
			{
				throw new Error( 'Inspectable TagName ' + this.tagName );
				break;
			}
		}
	}
	
	this._removeInputField = function _removeInputField( strMaskType )
	{
//		alert( this );
//		alert( this.id );
		var arrSpan = this.getObjContainer().getElementsByTagName( '*' );
		if( arrSpan.length == 0 )
		{
			return;
		}
		objInput = arrSpan[0];
		var strNewValue = objInput.value;
		if( strNewValue == '-' ) strNewValue = '';
		if( this.originalValue == '-' ) this.originalValue = '';
		
		if( strNewValue != this.originalValue )
		{
			this.tryChangeFieldValue( strNewValue );
		}
		else
		{
			this.changeFieldValue( this.originalValue );
		}
	}
	
	this.changeFieldValueCheck = function changeFieldValueCheck( strNewValue )
	{
		this.newValue = ( strNewValue == '1'  ||  strNewValue == 'true' );
		var objImg = document.createElement( 'img' );
		objImg.src = this.newValue ? builImageLink( 'checkbox_checked.gif' ) : builImageLink( 'checkbox_unchecked.gif' );
		objImg.className = 'activeFrozenFieldEnabled';
		this.getObjContainer().innerHTML = '';
		this.getObjContainer().appendChild( objImg );
	}
	
	this.changeFieldValue = function changeFieldValue( strNewValue )
	{
		switch( this.tagName )
		{
			case 'span':
			{
				this.originalValue = trim( this.getObjContainer().innerHTML );
				switch( this.mask  )
				{
					case 'check':
					{
						this.changeFieldValueCheck( strNewValue );
						break;
					}
					case 'string':
					case 'integer':
					case 'date':
					case 'readonly':
					{
						strNewValue += '';
						try
						{
							this.originalValue = strNewValue;
							this.getObjContainer().title = unxmlentities( strNewValue );
							if( strNewValue != '')
							{
								this.getObjContainer().innerHTML = strNewValue;
								setId( this.getObjContainer() , "container" );
								this.getObjContainer().innerHTML = strNewValue;
							}
							else
							{
								this.getObjContainer().innerHTML = '-';
							}
						}
						catch( e )
						{
							/**
							 * @TODO Quando os elementos htmls que contem os elementos activefrozenfields
							 * forem removidos no banco os activefrozen fields deve ser destruidos
							 */
							print( 'changeFieldValue:' + e.message );
						}
						break;
					}
					default:
					{
						throw new Error( 'Inspectable Mask Type ' );
						break;
					}
				}
				break;
			}
			case 'input':
			{
				switch( this.mask )
				{
					case 'readonly':
					{
						break;
					}
					case 'checkbox':
					{
						strNewValue += ''
						if( strNewValue == '1' || strNewValue == 'true' || strNewValue == true || strNewValue == 'on' )
						{
							this.getObjContainer().checked = true;
							this.getObjContainer().setAttribute( 'checked' , 'true' );
							this.getObjContainer().setAttribute( 'value' , 'on' );
						}
						else
						{
							var objClone = document.createElement( this.tagName );
							objClone.className = 'activeFrozenFieldEnabled';
							
							objClone.objActiveFrozenField = this;
							objClone.setAttribute( 'value'		, 'off' );
							addEvent( objClone , 'onclick' , 'return this.objActiveFrozenField.onclick( event )' );
							addEvent( objClone , 'onblur' , 'return this.objActiveFrozenField.onblur( event )' );
							addEvent( objClone , 'onfocus' , 'return this.objActiveFrozenField.onfocus( event )' );
							
							var objContainerParent = this.getObjContainer().parentNode;
							objContainerParent.innerHTML = '';
							objContainerParent.appendChild( objClone );
							this.setObjContainer( objClone );
						}
						break;
					}
					default:
					{
						throw new Error( 'Inspectable Mask Type ' );
						break;
					}
				}
				break;
			}
			default:
			{
				throw new Error( 'Inspectable TagName ' );
				break;
			}
		}
		setTimeout( 'window.ActiveFrozenField.getActiveFrozenField("' + this.id + '").onLoad();' , 100 );
		this.objContainer.objActiveFrozenField = this;
	}
	
	this.tryChangeFieldValue = function tryChangeFieldValue( strNewValue )
	{
		this.getObjContainer().innerHTML = '';
		var objImg = document.createElement( 'img' );
		objImg.src = strSrcImgWait;
		this.getObjContainer().appendChild( objImg );
		
		var arrSendRequestParams = new Array();
		arrSendRequestParams.push( 'atualizarCampo' );
		arrSendRequestParams.push( xmlentities( this.objectServerSide ) );
		arrSendRequestParams.push( xmlentities( this.objectServerSideId ) );
		arrSendRequestParams.push( xmlentities( 'update' ) );
		arrSendRequestParams.push( xmlentities( this.attributeServerSide ) );
		arrSendRequestParams.push( xmlentities( this.originalValue ) );
		arrSendRequestParams.push( xmlentities( strNewValue ) );
		addRequest( window.ActiveFrozenField.Event , '' , 
			arrSendRequestParams , refreshActiveFrozenFields , null );
	}
	
	this.removeIntegerInputField = function removeIntegerInputField()
	{
		this._removeInputField( 'integer' );
	}				
	
	this.removeStringInputField = function removeStringInputField()
	{
		this._removeInputField( 'string' );
	}				
	
	this.removeDateInputField = function removeDateInputField()
	{
		this._removeInputField( 'date' );
	}				
	
	this.submitCheckboxField = function submitCheckboxField()
	{
		this.newValue		= this.newValue				? '1' : '0';
		this.originalValue	= this.originalValue		? '1' : '0';
		
		if( this.newValue != this.originalValue )
		{
			this.tryChangeFieldValue( this.newValue );
		}
		else
		{
			this.changeFieldValue( this.originalValue );
		}
	}				
	
	this.submitCheck = function submitCheck()
	{
		this.originalValue	= ( this.getObjContainer().innerHTML.indexOf( 'checkbox_checked.gif' ) != -1 );
		
		this.newValue = !this.originalValue;
		
		this.originalValue	= this.originalValue		? '1' : '0';
		this.newValue		= this.newValue				? '1' : '0';
		
		if( this.newValue != this.originalValue )
		{			
			this.tryChangeFieldValue( this.newValue );
		}
	}				
	
	this.onfocus = function onfocus()
	{
		this.removeOtherInstances();		
	}
	
	this.submit = function submit()
	{
		switch( this.tagName )
		{
			case 'span':
			{
				switch( this.mask )
				{
					case 'string':
					{
						this.removeStringInputField();
						break;
					}
					case 'integer':
					{
						this.removeIntegerInputField();
						break;
					}
					case 'date':
					{
						this.removeDateInputField();
						break;
					}
					case 'check':
					{
						this.submitCheck();
						break;
					}
					case 'readonly':
					{
						break;
					}
					default:
					{
						throw new Error( 'Inspectable Mask ' + this.mask );
						break;
					}
				}
				break;
			}
			case 'input':
			{
				switch( this.mask )
				{
					case 'checkbox':
					{
						this.submitCheckboxField();
						break;
					}
					default:
					{
						throw new Error( 'Inspectable Mask ' + this.mask );
						break;
					}
				}
				break;
			}
			default:
			{
				throw new Error( 'Inspectable TagName ' + this.tagName );
				break;
			}
		}
	}
	
	this.onblur = function onblur()
	{
		
	}
	
	this.doFocus = function doFocus()
	{
		
		this.onfocus();
		arrInput = this.getObjContainer().getElementsByTagName( "input" );
		if( arrInput.length > 0 )
		{
			try
			{
				if( arrInput[ 0 ].focus )
				{
					intInputId = setId( arrInput[ 0 ] , "inputElement" );
					setTimeout("if( document.getElementById('" + intInputId + "' ) ) document.getElementById('" + intInputId + "' ).focus();", 10);
				}
			}
			catch( e )
			{
				print( e.message );
			}
		}
		
	}
	
	this.cancel = function cancel()
	{
		this.changeFieldValue(this.originalValue );
		window.ActiveFrozenField.actualElement = null;
	}
	
	this.__construct( objContainer_ , object_ , identifier_ );
}

window.prepareActiveFrozenFields = function prepareActiveFrozenFields( funcAfterPrepare_ )
{
	try
	{
		var objTag;
		var objActiveFrozenTag;
		var arrActiveFrozenTag	= document.body.getElementsByTagName( 'element' );
		for( var i = 0; i <  arrActiveFrozenTag.length ; i++ )
		{
			objActiveFrozenTag = arrActiveFrozenTag[ i ];
			if( !IE )
			{
				strObjectServerSide			= objActiveFrozenTag.className;
				intObjectServerSideId		= objActiveFrozenTag.getAttribute( 'identifier' ) + '';
				var arrSpan		= objActiveFrozenTag.getElementsByTagName( 'span' );
				var arrInput	= objActiveFrozenTag.getElementsByTagName( 'input' );
			}
			else
			{
				strObjectServerSide			= objActiveFrozenTag.className;
				intObjectServerSideId		= objActiveFrozenTag.getAttribute( 'identifier' ) + '';
				var arrSpan		= objActiveFrozenTag.parentNode.getElementsByTagName( 'span' );
				var arrInput	= objActiveFrozenTag.parentNode.getElementsByTagName( 'input' );
			}
			if( arrSpan.length == 0)
			{
				arrTags = arrInput;
			}
			else if( arrInput.length == 0 )
			{
				arrTags = arrSpan;
			}
			else
			{
				arrTags 	= array_order_by_field( array_merge( arrSpan, arrInput ) , "tabindex" );
			}
			for( var j = 0; j <  arrTags.length ; j++ )
			{
				var objTag = arrTags[ j ];
				if( objTag.tagName != undefined )
				{
					switch( objTag.getAttribute( 'attribute' ) + '' )
					{
						case 'undefined':
						case 'null':
						case '':
						{
							break;
						}
						default:
						{
							if( objTag.objActiveFrozenField == undefined )
							{
								objTag.objActiveFrozenField = new ActiveFrozenField( objTag , strObjectServerSide , intObjectServerSideId );
								var strTagId = setId( objTag , 'tag' );
								addEvent( objTag , 'onclick'	, 'return window.ActiveFrozenField.getActiveFrozenField("' + objTag.objActiveFrozenField.id + '").onclick( event )' );
								addEvent( objTag , 'onblur'		, 'return window.ActiveFrozenField.getActiveFrozenField("' + objTag.objActiveFrozenField.id + '").onblur( event )' );
								addEvent( objTag , 'onfocus'	, 'return window.ActiveFrozenField.getActiveFrozenField("' + objTag.objActiveFrozenField.id + '").onfocus( event )' );
							}
							break;
						}
					}
				}
			}
		}
		if( funcAfterPrepare_ != undefined )
		{
			funcAfterPrepare_();
		}
	}
	 catch( e )
	{
		print( 'prepareActiveFrozenFields:' + e.message );
	}
	
	if( IE )
	{
		setTimeout( 'if( document.getElementById( "divProtecao" ) )document.getElementById( "divProtecao" ).parentNode.removeChild( document.getElementById( "divProtecao" ) )' , 10 );
	}
	else
	{
		removeDivProtection();
	}
	
}
window.ActiveFrozenField.Event = "/geral/ajax/requisicao_ajax.php";

window.ActiveFrozenField.actualElement = null;
/**
 * Array of Instances of Calendar in Memory
 *
 * @acess public static
 */ 
window.ActiveFrozenField.instances = Array();

/**
 * Static Method to get some calendar by Id
 *
 * @acess public static
 */
window.ActiveFrozenField.getActiveFrozenField = function getActiveFrozenField( intActiveFrozenFieldId )
{
	return window.ActiveFrozenField.instances[ parseInt( intActiveFrozenFieldId ) ];
}

window.ActiveFrozenField.next = function next()
{
	if( window.ActiveFrozenField.actualElement == null )	
	{
		return true;
	}
	if( ( window.ActiveFrozenField.instances.length - 1 ) > window.ActiveFrozenField.actualElement.id )
	{
		window.ActiveFrozenField.instances[ window.ActiveFrozenField.actualElement.id + 1 ].edit();
	}
	else
	{
		window.ActiveFrozenField.instances[ 0 ].edit();
	}
}

window.ActiveFrozenField.back = function back()
{
	if( window.ActiveFrozenField.actualElement == null )	
	{
		return true;
	}
	if( window.ActiveFrozenField.actualElement.id > 0 )
	{
		window.ActiveFrozenField.instances[ window.ActiveFrozenField.actualElement.id - 1 ].edit();
	}
	else
	{
		window.ActiveFrozenField.instances[ ActiveFrozenField.instances.length - 1 ].edit();
	}
}

window.ActiveFrozenField.submit = function submit()
{
	if( window.ActiveFrozenField.actualElement == null )	
	{
		return true;
	}
	window.ActiveFrozenField.actualElement.submit();
}

window.ActiveFrozenField.cancel = function cancel()
{
	if( window.ActiveFrozenField.actualElement == null )	
	{
		return true;
	}
	window.ActiveFrozenField.actualElement.cancel();
}

window.ActiveFrozenField.sendAnyWay = function sendAnyWay()
{ 
	arrObjActiveFrozenFields	= window.ActiveFrozenField.question.arrObjActiveFrozenFields;
	arrElements 				= window.ActiveFrozenField.question.arrElements;
	
	for( var i = 0 ; i < arrObjActiveFrozenFields.length; ++i )
	{
		objActiveFrozenField = arrObjActiveFrozenFields[ i ];
	
		if( arrElements.attributeName == objActiveFrozenField.attributeServerSide )
		{
			objActiveFrozenField.originalValue	= arrElements.originalValue;
			objActiveFrozenField.tryChangeFieldValue( arrElements.newValue );
			return;
		}
	}
}

window.ActiveFrozenField.sendOldValue = function sendOldValue()
{
	arrObjActiveFrozenFields	= window.ActiveFrozenField.question.arrObjActiveFrozenFields;
	arrElements 				= window.ActiveFrozenField.question.arrElements;
	
	objActiveFrozenField = arrObjActiveFrozenFields.pop();
	objActiveFrozenField.newValue = objActiveFrozenField.originalValue;
	objActiveFrozenField.originalValue	= arrElements.originalValue;
	objActiveFrozenField.tryChangeFieldValue( arrElements.newValue );
}

window.ActiveFrozenField.sync = function sync()
{
	arrObjActiveFrozenFields	= window.ActiveFrozenField.question.arrObjActiveFrozenFields;
	arrElements 				= window.ActiveFrozenField.question.arrElements;

	for( var i = 0 ; i < arrObjActiveFrozenFields.length; ++i )
	{
		objActiveFrozenField = arrObjActiveFrozenFields[ i ];
		if( arrElements.attributeName == objActiveFrozenField.attributeServerSide )
		{
			objActiveFrozenField.changeFieldValue( arrElements.originalValue );
		}
	}
	if( window.ActiveFrozenField.actualElement != null )
	{
//		window.ActiveFrozenField.actualElement.onclick();
	}
}

/**
 *
 */
window.ActiveFrozenField.getActiveFrozenFieldByDescription = function getActiveFrozenFieldByDescription( objServerSideDescription )
{
	var arrActiveFrozenFields = Array();
	var objActiveFrozenField;
	for( var i = 0 ; i < window.ActiveFrozenField.instances.length ; ++i )
	{
		objActiveFrozenField =  window.ActiveFrozenField.instances[ i ];
		if(
			( objActiveFrozenField.objectServerSide == objServerSideDescription.className )
			&&
			( objActiveFrozenField.objectServerSideId == objServerSideDescription.id )
			&&
			( !isUndefined( objServerSideDescription[ objActiveFrozenField.attributeServerSide ] ) )
		  )
		{
			arrActiveFrozenFields.push( objActiveFrozenField );
		}
	}
	return arrActiveFrozenFields;
}

window.refreshActiveFrozenFields = function refreshActiveFrozenFields( strXml, objXml , arrLoopParams_ )
{
	window.last.activeCommand = strXml;
///	alert( 'refreshActiveFrozenFields:' + strXml );
	if( strXml == "" )
	{
		return;
	}
	
	if( arrLoopParams_ != undefined )
	{
		var objImg					= document.getElementById( arrLoopParams_[ 0 ] );
		var strSrcOriginal			= arrLoopParams_[ 1 ];
		var strClassServerSide		= arrLoopParams_[ 2 ];
		var intObjectServerSideId	= arrLoopParams_[ 3 ];
		var intContainerId			= arrLoopParams_[ 4 ];
		objImg.src = strSrcOriginal;
	}
	
	try
	{
		arrElements = eval( strXml );
	}catch( e )
	{
		arrElements = Array();
	}
	if( arrElements == undefined )
	{
		arrElements = Array();
	}
	
	if( arrElements.length != undefined )
	{
		for( var i = 0; i < arrElements.length ; ++i )
		{
			objElement = arrElements[ i ];
			if( objElement.removed == undefined )
			{
				arrObjActiveFrozenFields = window.ActiveFrozenField.getActiveFrozenFieldByDescription( objElement );
				for( var j = 0 ; j < arrObjActiveFrozenFields.length ; ++j )
				{
					objActiveFrozenField = arrObjActiveFrozenFields[ j ];
					objActiveFrozenField.changeFieldValue( objElement[ objActiveFrozenField.attributeServerSide ] );
				}
			}
			else
			{
				strClass = objElement.className;
				strFunctionName = 'remove' + strClass + '(' + objElement.id + ')';
				try
				{
					eval( strFunctionName );
				}
				catch( e )
				{
				}
			}
		}
		if( window.ActiveFrozenField.actualElement != null )
		{
//			window.ActiveFrozenField.actualElement.edit();
		}		
	}
	else if ( arrElements.className = 'Exception' )
	{
		switch( arrElements.code )
		{
			case 1: // invalid params // paremetros invalidos //
			{
				window.Question.questionAlert( arrElements.message );
				break;
			}
			case 2: // the class could not be founded // a classe nao pode ser encontrada //
			case 3: // the class is not a entity of the system // a classe nao eh uma entidade do sistema //
			case 5: // attribute not founded // campo nao encontrado //
			{
				window.Question.questionAlert( 'Ocorreu um erro interno, favor tente novamente');
				
				break;
			}
			case 6: // error of simultaneos change // erro de alteracao simultanea //
			{
				objElement = Array();
				objElement[ "className" ]				= arrElements.classChanged;
				objElement[ "id" ] 						= arrElements.id;
				objElement[ arrElements.attributeName ]	= arrElements.newValue;
				arrObjActiveFrozenFields = window.ActiveFrozenField.getActiveFrozenFieldByDescription( objElement );
				
				window.ActiveFrozenField.question = Array();
				window.ActiveFrozenField.question.arrObjActiveFrozenFields = arrObjActiveFrozenFields;
				window.ActiveFrozenField.question.arrElements = arrElements;
				
				objOp1 = new QuestionOption( 'Enviar Mesmo Assim' , window.ActiveFrozenField.sendAnyWay	);
				
				objOp2 = new QuestionOption( 'Enviar Valor Antigo' ,  window.ActiveFrozenField.sendOldValue );
				
				objOp3 = new QuestionOption( 'Aceitar o Novo Valor e Cancelar Alteracao' , window.ActiveFrozenField.sync ); 
				
				objQuestion = new Question();
				objQuestion.strMessage = 'Ocorreu em erro de sincronia ao tentar alterar registro.' +
					'<br/> Provalmente outro usuario alterou o valor deste campo antes.' +
					'<br/> O que voce deseja fazer ?';
				objQuestion.appendQuestionOption( objOp1 );
				objQuestion.appendQuestionOption( objOp2 );
				objQuestion.appendQuestionOption( objOp3 );
				objQuestion.show();
				
				break;
			}
			case 4: // element not founded // elemento nao encontrado //
			case 473: // element not finded in this class // elemento nao encontrado //
			{
				window.Question.questionAlert( 'O elemento o qual voce deseja alterar nao foi encontrado no sistema. O mesmo provalemente foi removido recentemente por outro usuario.' );
				break;
			}
			default:
			{
				objElement = Array();
				objElement[ "className" ]				= arrElements.classChanged;
				objElement[ "id" ] 						= arrElements.id;
				objElement[ arrElements.attributeName ]	= arrElements.newValue;
				arrObjActiveFrozenFields = window.ActiveFrozenField.getActiveFrozenFieldByDescription( objElement );
				
				window.ActiveFrozenField.question = Array();
				window.ActiveFrozenField.question.arrObjActiveFrozenFields = arrObjActiveFrozenFields;
				window.ActiveFrozenField.question.arrElements = arrElements;

				window.Question.questionAlert( arrElements.message , window.ActiveFrozenField.sync );
				break;
			}
		}
	}
	else
	{
		window.Question.questionAlert( 'O servidor retornou uma resposta desconhecida' );
		throw new Error( 'The server returned a unexpected answer' , 1001 );
	}

		
	if( arrLoopParams_ && arrLoopParams_[ 5 ] )
	{
		 arrLoopParams_[ 5 ]( arrLoopParams_[ 6 ] );
	}

}

window.ActiveFrozenFields_Main = function ActiveFrozenFields_Main()
{
	if( !globalBoolAfterOnLoad )
	{
		addEvent( document.body  , 'onload' , prepareActiveFrozenFields );
	}
	else
	{
		prepareActiveFrozenFields();
	}
	
	activeBodyGetKey();
	
	window.KeyPressed = function KeyPressed( intKeyCode , strTypeCode )
	{
		switch( strTypeCode )
		{
			case 'enter':
			{
				window.ActiveFrozenField.submit();
				break;
			}
			case 'esc':
			{
				window.ActiveFrozenField.cancel();
				break;
			}
			case 'tab':
			{
				if( intKeyCode < 1000 )
				{
					window.ActiveFrozenField.next();
				}
				else
				{
					window.ActiveFrozenField.back();
				}
				break;
			}
			default:
			{
				return true;
				break;
			}
		}
		return false;
	}
	
}

function removeDivProtection()
{
	var objDivProtection = document.getElementById( "divProtecao" );
	
	if( objDivProtection )
	{
		objDivProtection.parentNode.removeChild( objDivProtection  );	
	}
}

globalBoolAfterOnLoad = false;
if( document.body )
{
	addEvent(  document.body  , 'onload', 'globalBoolAfterOnLoad = true' );
}

window.insertElement = function insertElement( objImg , intObjectServerSideId, intObjectContainerServerSideId, strClassServerSide , funcLoopBack_ , paramsLoopBack_ )
{
	strSrcOriginal = objImg.src;
	objImg.src = strSrcImgWait;
	
	// gerando o array de parametros para enviar a requisicao //
	
	var arrSendRequestParams = new Array();
	arrSendRequestParams.push( 'atualizarCampo' );
	arrSendRequestParams.push( xmlentities( strClassServerSide ) );
	arrSendRequestParams.push( xmlentities( intObjectServerSideId ) );
	arrSendRequestParams.push( xmlentities( 'insert' ) );
	arrSendRequestParams.push( xmlentities( 'container' ) );
	arrSendRequestParams.push( xmlentities( intObjectContainerServerSideId ) );
	
	// gerando o array de parametros que irao voltar a funcao de resposta //
	
	setId( objImg , "img" );
	var arrLoopParams = new Array();
	arrLoopParams.push( objImg.id );
	arrLoopParams.push( strSrcOriginal );
	arrLoopParams.push( strClassServerSide );
	arrLoopParams.push( intObjectServerSideId );
	arrLoopParams.push( intObjectContainerServerSideId );
	arrLoopParams.push( funcLoopBack_ );
	arrLoopParams.push( paramsLoopBack_ );
//	alert( arrSendRequestParams );
	addRequest( window.ActiveFrozenField.Event , '' , arrSendRequestParams , refreshActiveFrozenFields , null , arrLoopParams );
}

window.removeElement = function removeElement( objImg , intObjectServerSideId, intObjectContainerServerSideId, strClassServerSide , boolConfirmBefore_ )
{
	if( boolConfirmBefore_ == undefined )
	{
		boolConfirmBefore_ = true;
	}
	if( objImg.src != strSrcImgWait )
	{
		if( boolConfirmBefore_ )
		{
			strSrcOriginal = objImg.src;
			objImg.src = strSrcImgWait;
			
			// gerando o array de parametros para enviar a requisicao //
			
			var arrSendRequestParams = new Array();
			arrSendRequestParams.push( 'atualizarCampo' );
			arrSendRequestParams.push( xmlentities( strClassServerSide ) );
			arrSendRequestParams.push( xmlentities( intObjectServerSideId ) );
			arrSendRequestParams.push( xmlentities( 'analiseRemotionImpact' ) );
			arrSendRequestParams.push( xmlentities( 'container' ) );
			arrSendRequestParams.push( xmlentities( intObjectContainerServerSideId ) );
			
			// gerando o array de parametros que irao voltar a funcao de resposta //

			setId( objImg , "img" );
			var arrLoopParams = new Array();
			arrLoopParams.push( objImg.id );
			arrLoopParams.push( strSrcOriginal );
			arrLoopParams.push( strClassServerSide );
			arrLoopParams.push( intObjectServerSideId );
			arrLoopParams.push( intObjectContainerServerSideId );
//			alert( arrSendRequestParams );
			addRequest( window.ActiveFrozenField.Event , '' , 
			arrSendRequestParams , confirmRemoveElement , null , arrLoopParams );
		}
		else
		{
			strSrcOriginal = objImg.src;
			objImg.src = strSrcImgWait;
			
			// gerando o array de parametros para enviar a requisicao //
			
			var arrSendRequestParams = new Array();
			arrSendRequestParams.push( 'atualizarCampo' );
			arrSendRequestParams.push( xmlentities( strClassServerSide ) );
			arrSendRequestParams.push( xmlentities( intObjectServerSideId ) );
			arrSendRequestParams.push( xmlentities( 'remove' ) );
			arrSendRequestParams.push( xmlentities( 'container' ) );
			arrSendRequestParams.push( xmlentities( intObjectContainerServerSideId ) );
			
			// gerando o array de parametros que irao voltar a funcao de resposta //
			
			var arrLoopParams = new Array();
			arrLoopParams.push( objImg.id );
			arrLoopParams.push( strSrcOriginal );
			arrLoopParams.push( strClassServerSide );
			arrLoopParams.push( intObjectServerSideId );
			arrLoopParams.push( intObjectContainerServerSideId );
//			alert( arrSendRequestParams );
			addRequest( window.ActiveFrozenField.Event , '' , arrSendRequestParams , refreshActiveFrozenFields , null , arrLoopParams );
		}
	}
}

window.confirmRemoveElement = function confirmRemoveElement( strXml , objXml , arrLoopParams )
{
	var objImg							= document.getElementById( arrLoopParams[ 0 ] );
	var strSrcOriginal					= arrLoopParams[ 1 ];
	var strClassServerSide				= arrLoopParams[ 2 ];
	var intObjectServerSideId			= arrLoopParams[ 3 ];
	var intObjectContainerServerSideId	= arrLoopParams[ 4 ];
	objImg.src = strSrcOriginal;
	
	strIdImg = setId( objImg, 'img' );
	
	arrElements = Array();
	try
	{
		arrElements = eval( strXml );
	}
	catch( e )
	{
		print( e.message );
		print( strXml );
	}
	
	if( arrElements == undefined )
	{
		arrElements = Array();
	}
	if( arrElements.length != undefined )
	{
		if(arrElements.length > 0 )
		{
			var objOp1 = new QuestionOption( 'Excluir Mesmo Assim.' , '' +
				'function(){ ' + 
					'removeElement( ' + 
						'document.getElementById( "' + strIdImg + '" ) ' + 
						' , ' + 
						intObjectServerSideId + 
						' , ' + 
						intObjectContainerServerSideId + 
						' , ' + 
						'"' + strClassServerSide + '"' + 
						' , ' + 
						'false ' +
						') ' +
					' } '
				);
			
			var objOp2 = new QuestionOption( xmlentities( 'Cancelar Exclus&#227;o.' ) ,  function(){} );
			
			var objQuestion = new Question();
			objQuestion.strMessage = implode( "\n" + '<br/>' , arrElements );
			
			objQuestion.appendQuestionOption( objOp1 );
			objQuestion.appendQuestionOption( objOp2 );
			objQuestion.show();
		}
		else
		{
			var objOp1 = new QuestionOption( 'Confirmar Exclus&#227;o.' , '' +
				'function(){ ' + 
					'removeElement( ' + 
						'document.getElementById( "' + strIdImg + '" ) ' + 
						' , ' + 
						intObjectServerSideId + 
						' , ' + 
						intObjectContainerServerSideId + 
						' , ' + 
						'"' + strClassServerSide + '"' + 
						' , ' + 
						'false ' +
						') ' +
					' } '
				);
			
			var objOp2 = new QuestionOption( xmlentities( 'Cancelar Exclus&#227;o.' ) ,  function(){} );
			
			var objQuestion = new Question();
			objQuestion.strMessage = 'Tem Certeza que deseja excluir esta Tarefa ?';
			
			objQuestion.appendQuestionOption( objOp1 );
			objQuestion.appendQuestionOption( objOp2 );
			objQuestion.show();
//			removeElement( objImg , intObjectServerSideId , intObjectContainerServerSideId, strClassServerSide , false );
		}	
	}	
}


require_once( 
	Array( 
		'tags/question.js' ,  
		'keys/_start.js' ,
		'xml/_start.js'
	) 
	,
	ActiveFrozenFields_Main
);
