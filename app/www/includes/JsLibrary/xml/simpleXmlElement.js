// requer remedial.js //

var SPECIAL_PROPERTY_NAMES = Array( "class" , "type" );
var MICROSOT_PROPERTY_NAMES = Array( "hideFocus", "contentEditable", "disabled", "tabIndex" );
/*
* Vers?o do SimpleXmlElement do PHP para Javascript. Converte um Xml num objeto simples ondes as
* propriedades da tag s?o acessadas como propriedades p?blicas do objeto e os elementos filhos 
* s?o acessaveis por arrays de SimpleXmlElement em propriedades publicas do objeto com o nome da tag
* dos mesmos.
*
* Version to the SimpleXmlElement of PHP para Javascript. Convert some Xml in one object simple
* where the properties of the tag will become public properties of the object and the child nodes
* can be acess by arrays of SimpleXmlElements in public properties of the object with the tag name
* of then.
*
* @Author		CAIXA
* @Date			01/11/2005
* @Version		1.0
* @package		Xml Request
* @package		Serializer
* @subpackage	Javascrit Xml Request
* @return object SimpleXmlElement
*/
window.SimpleXmlElement = function SimpleXmlElement()
{
	/**
	* dom Object original
	*
	* parent dom Object 
	*/
	this.domXml = undefined;
	
	/**
	* Tipo do objeto - constante
	*
	* Type of object - constant
	*/
	this.type 	= "SimpleXmlElement";
	
	/**
	* Nome da tag do objeto xml
	*
	* Name of the tag of the xml object
	*/
    this.tagName 	= "";
    this.childTagNames = Array();
    this.properties = Array();
    this.ignoredProperties = MICROSOT_PROPERTY_NAMES;
    
    this.clearDom = function clearDom()
    {
    	this.domXml = "";
    	for( var i = 0; i < this.childTagNames.length; ++i)
    	{
    		for (var count = 0; count < this[ this.childTagNames[ i ] ].length; ++count)
    		{
    			this[ this.childTagNames[ i ] ][ count ].clearDom();
    		}
    	}
    }
    
    this.getXml = function getXml( xmlElement )
    {
    	this.domXml = xmlElement;
    	this.tagName 	= xmlElement.tagName;
    	
    	this.textContent = undefined;
    	    	
		if ( xmlElement.attributes )
		{
			for ( var count = 0; count < xmlElement.attributes.length ; ++count )
	    	{
    			objActualAttribute = xmlElement.attributes[ count ];
    			var strName = objActualAttribute.name;
    			var mixValue = objActualAttribute.value;
				
				if ( array_search( strName, SPECIAL_PROPERTY_NAMES ) != -1)
				{
					strName += "Name";
				}
				
				
				if ((mixValue != "") && (mixValue != null) && (mixValue != "null") && (mixValue != undefined))
				if ( array_search( strName, this.ignoredProperties ) == -1)
				{
					this[ strName ] = mixValue;
					this.properties[ this.properties.length ] = strName;
				}
	    	}
    	}

    	// existe um bug que o text content dos filhos propaga nos pais
    	
    	for (var count = 0; count < xmlElement.childNodes.length; ++count )
		if (xmlElement.childNodes[ count ].tagName != undefined)
    	{
			var xmlChild = xmlElement.childNodes[ count ];
			var strChildName = xmlChild.tagName;
			
			var strPropertyName = strChildName  ;

			// alterando o nome da propriedade caso seja um nome especial //
					
			if ( array_search( strPropertyName, SPECIAL_PROPERTY_NAMES ) != -1)
			{
				strPropertyName += "Tag";
			}
			
			if (array_search( strPropertyName , this.childTagNames ) == -1)
			{
				this.childTagNames[ this.childTagNames.length ] = strPropertyName;
			}
			
			if 	(
					( !this[ strPropertyName ] )
				||
					( this[ strPropertyName ] == undefined )
				)
			{
				var MyArray = Array( );
			}
			else
			{
				var MyArray = this[ strChildName ];
			}
			
			var intLength = MyArray.length;
			var objSimpleXmlElementChild = new SimpleXmlElement( );
			
			objSimpleXmlElementChild.getXml( xmlChild );
    	
			MyArray[ intLength ] = objSimpleXmlElementChild;
			
			this[ strPropertyName] = MyArray;
			
    	}
    	else
    	{
        	if (xmlElement.textContent)
	    	{
	    		// mozilla
	    		this.textContent = xmlElement.textContent;
	    	}
	    	
	    	if (xmlElement.text)
	    	{
	    		// IE
	    		this.textContent = xmlElement.text;
	    	}

    	}
  
    }

    this.getTextContent = function getTextContent()
    {
    	return this.textContent;	
    }
    
    this.getAttribute = function getAttribute( strAttribute )   
    {
    	return this[ strAttribute ]	;
    }
    
    this.getFirstChild = function getFirstChild()
    {
    	return this[ this.childTagNames[ 0 ] ][0];	
    }
    
    this.getTagName = function getTagName( strTab )
    {
    	
    	if (strTab ==  undefined)
    		strTab = "";
    		
    	var strXml = "\n" + strTab + "<" + this.tagName ;
    	
    	return strXml;
    }
    
    this.asXml = function asXml( strTab )
    {
    	
    	if (strTab ==  undefined)
    		strTab = "";
    		
    	var strXml = "\n" + strTab + "<" + this.tagName ;
    	for (var i = 0; i < this.properties.length; ++i)
    	{
    		strXml += " " + this.properties[i] + ' = "' + this[ this.properties[i] ] + '" ';
    	}
    	
    	if ( this.childTagNames.length > 0 )
    	{
	   		strXml += ">";
	   		
    		if (this.textContent)
			{
				strXml += "\n" + strTab + this.textContent;
			}
	
    	
	    	for( var i = 0; i < this.childTagNames.length; ++i)
	    	{
	    		for (var count = 0; count < this[ this.childTagNames[ i ] ].length; ++count)
	    		{
   					strXml += this[ this.childTagNames[ i ] ][ count ].asXml( strTab + "      " );
	    		}
	    	}
	    
	    	strXml += "\n" + strTab + "</" + this.tagName + ">" ;
    	}	
    	else
    	{
    		if (this.textContent)
			{
		   		strXml += ">";
		   		
				strXml += "\n" + strTab + this.textContent;
		    	strXml += "\n" + strTab + "</" + this.tagName + ">" ;
			}
			else
			{
				strXml += "/>";
			}
    	}
    	return strXml;
    }
    
    SimpleXmlElement.prototype.toString = function()
    {
	   	this.clearDom();
	    return this.asXml();
    }

}
