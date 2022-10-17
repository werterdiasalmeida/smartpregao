/**
 * Funcoes para aprimorarem e adicionarem funcionalidades aos objetos doom
 */

if( IE )
{
	HTMLElement = Object;
}

// O Firefox Aceita que vc trate os objetos DOM como extensao da classe			//
// HTMLElement podendo alterar os metodos da mesma sem problemas				//

HTMLElement.prototype.nativeInsertBefore = HTMLElement.prototype.insertBefore;

HTMLElement.prototype.insertBefore = function insertBefore( objNewElement, objElement_ )
{
	if( objElement_ == undefined )
	{
		objElement_ = this;
	}
	objElement_.parentNode.nativeInsertBefore( objNewElement , objElement_ );
}

HTMLElement.prototype.insertAfter = function insertAfter( objNewElement, objElement_ )
{
	if( objElement_ == undefined )
	{
		objElement_ = this;
	}
	if( objElement_.Class == 'DomElement' )
	{
		objElement_ = objElement_.objHtmlElement;
	}
	if( objNewElement.Class == 'DomElement' )
	{
		objNewElement = objNewElement.objHtmlElement;
	}
	
	var objNextElement = objElement_.nextSibling;
	if( objNextElement != undefined )
	{
		objElement_.parentNode.insertBefore( objNewElement ,  objNextElement );
	}
	else
	{
		objElement_.parentNode.appendChild( objNewElement );
	}
}

// O Internet Explorer trata os objetos resultados dos metodos DOM como da classe	//
// object o que impossibilita qualquer processo nativo de controle dos metodos.		//
// Para poder unificar o controle de objetos DOM esta classe DomElement foi criada	//
// que abstrai este processo internamente											//


window.DomDocument = function DomDocument()
{};

window.DomElement = function DomElement( objHtmlElement )
{
	this.Class = 'DomElement';
	
	this.__construct = function __construct( objHtmlElement )
	{
		this.objHtmlElement = objHtmlElement;
		for( strAttribute in objHtmlElement )
		{
			if( isFunction( objHtmlElement[ strAttribute ] ) )
			{
				if( this[ strAttribute ] == undefined )
				{
					if ( IE )
					{
						var strFunctionCommand = "{ return this.objHtmlElement." + strAttribute + ".apply( this.objHtlmElement , this." + strAttribute + ".arguments )}";
						this[ strAttribute ] = Function( strFunctionCommand );
					}
					else
					{
						var strFunctionCommand = "{ return this.objHtmlElement." + strAttribute + ".apply( this.objHtmlElement , this." + strAttribute + ".arguments )}";
						this[ strAttribute ] = Function( strFunctionCommand );
					}
				}
			}
			else
			{
				var strSetFunctionCommand = "this.objHtmlElement." + strAttribute + " = mixNewValue; return true; ";
				var strMethodName = ucfirst( strAttribute );
				this[ 'set' + strMethodName ] = Function( "mixNewValue" , strSetFunctionCommand );
				var strGetFunctionCommand = "return this.objHtmlElement." + strAttribute + ";";
				this[ 'get' + strMethodName ] = Function( strGetFunctionCommand );
			}
		}
		for( strMethod in HTMLElement.prototype )
		{
			if( this[ strMethod ] == undefined )
			{
				var strFunctionCommand = "return this.objHtmlElement." + strMethod + ".apply( this , this." + strMethod + ".arguments );";
				this[ strMethod ] = Function( strFunctionCommand );
			}
		}
		
	}
	
	this.getElementsByTagName = function getElementsByTagName( strTagName )
	{
		try
		{
			strTagName += '';
			strTagName = strTagName.toUpperCase();
			var arrChildElements = Array();
			var arrHtmlElements = this.objHtmlElement.getElementsByTagName( strTagName );
			for( var i = 0 ; i < arrHtmlElements.length; ++i )
			{
					if( !isUndefined( arrHtmlElements[ i ] ) )
					{
						var objDomElement = new window.DomElement( arrHtmlElements[ i ] );
						arrChildElements.push( objDomElement );
					}
			}
			return arrChildElements;
		}
		catch( e )
		{
			throw Error( ''+
				' Error into the received HTML string. ' + "\n" +
				' The Browser could not to make the DOM process ' +
				' into the received HTML because bad syntax into ' + 
				' the element ' +  "\n" +
				"\n" +
				'<code>' + "\n" +
					this.objHtmlElement.innerHTML  + "\n" +
				'</code>' + "\n" +
				"\n" +
				'Please check your HTML code specially into this part:' +
				"\n" +
				'<code>' + "\n" +
					getHtmlString( arrHtmlElements[ i ] ) + "\n" + 
				'</code>' + "\n" +
				''
			);
		}
	}

	if( IE )
	{
		this.setAttribute = function setAttribute( strAttributeName , strValueAttribute )
		{
			this.objHtmlElement.setAttribute( strAttributeName , strValueAttribute );
		}
		
		this.getAttribute = function getAttribute( strAttributeName )
		{
			return this.getAttribute( strAttributeName );
		}	
	}
	
	this.toString = function toString()
	{
		this.objHtmlElement = objHtmlElement;
		var strSerial = '';
		for( strAttribute in objHtmlElement )
		{
			if( !isFunction( objHtmlElement[ strAttribute ] ) )
			{
				if( ! isNull( this.objHtmlElement[ strAttribute ] ) &&  this.objHtmlElement[ strAttribute ] != '' )
				{
					strSerial += strAttribute + ': ' + trim( this.objHtmlElement[ strAttribute ] ) + " ";
				}
			}
		}
		return strSerial;
	}	
	
	this.__construct( objHtmlElement );
};

window.DomDocument.createElement = function( strElementName )
{
	var objHtmlElement = document.createElement( strElementName );
	var objDomElement = new DomElement( objHtmlElement );
	return objDomElement;
}