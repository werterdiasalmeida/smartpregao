// requer remedial js //

window.getType = function getType( mixObject )
{
	if (mixObject == null)
		return "null";
	if (mixObject == undefined)
		return "undefined";
		
	try
	{	
		if ( isString( mixObject) )
			return "string";	

		if ( isNumber( mixObject) )
			return "number";	

		if ( isArray( mixObject) )
			return "array";	
			
		if ( isObject( mixObject ) )
			return "object";
	}
	catch( e )
	{
		strType = "unknow";
	}
	return strType;
}

window.varDump = function varDump( mixObject, xmlObject, oldObjects )
{
	var SPECIAL_PROPERTIES = Array( "prototype", "domconfig" );
	
	if (mixObject.asXml)
	{
		strType = "group";
		var xmlObject = document.createElement( strType );
		xmlObject.setAttribute("type", getType( mixObject ) );
		xmlObject.innerHTML = mixObject.asXml();
		return xmlObject;
	}
	
	if (xmlObject == undefined)
	{
		strType = getType( mixObject );
		if (strType == "object")
		{
			strType = "group";
			var xmlObject = document.createElement( strType );
			xmlObject.setAttribute("type", getType( mixObject ) );
		}
		else
		{		
			var xmlObject = document.createElement( strType );		
		}
	}
		
	
	if (oldObjects == undefined)
	{
		oldObjects = Array();
	}
	
	if (isArray(mixObject))
	{
		for ( var i = 0; i < mixObject.length; ++i )
		{
			var xmlElement = document.createElement( "element");
			var xmlValue = document.createElement( "value");
			var objChild = mixObject[ i ];
			var xmlItem = varDump( objChild );
			xmlValue.appendChild( xmlItem );
			xmlElement.appendChild( xmlValue );
			
			xmlElement.setAttribute( "name" , i );
			xmlElement.setAttribute( "type" , "group" );
			
			xmlObject.appendChild( xmlElement );
		}
		
			
	}
	else if (! isObject( mixObject ) )
	{
		var xmlValue = document.createElement( "value" );
		xmlValue.innerHTML = mixObject;
		xmlObject.appendChild( xmlValue );
	}
	else
	{
		
		oldObjects[ oldObjects.length ] = mixObject;
		
		var strProperty;
		for( strProperty in mixObject )
		if ( array_search( strProperty, SPECIAL_PROPERTIES ) == -1)
		{
			
			mixProperty = mixObject[ strProperty ];
			var xmlProperty = document.createElement("element");
			xmlProperty.setAttribute("name", strProperty);

			if ( strProperty != "className" )
			{
				var strPropertyType = typeof mixProperty;
				
				if ( isArray( mixProperty ) )
				{
					
					var xmlChildObject = document.createElement( "array" );
					varDump( mixProperty , xmlChildObject, oldObjects );
					xmlProperty.appendChild(xmlChildObject);
				}
				else if ( isObject( mixProperty ) )
				{

					if (array_search( mixProperty, oldObjects ) == -1 )
					{
						var xmlChildObject = document.createElement( "group" );
						
						if ( mixProperty.asXml )
						{
							xmlProperty.innerHTML = mixProperty.asXml();
						}
						else
						{
							xmlChildObject.setAttribute("type","object");
							varDump( mixProperty , xmlChildObject, oldObjects );
							xmlProperty.appendChild(xmlChildObject);
						}
					}
				}
				else
				{
					if ((mixProperty != "") && (mixProperty != null))
					{
    					xmlProperty.setAttribute("type", getType( mixProperty ) );
	    				var xmlValue = document.createElement( "value" );
		    			xmlValue.innerHTML = mixProperty;
			    		xmlProperty.appendChild(xmlValue);
			    	}
				}
				xmlObject.appendChild( xmlProperty );				

			}
			else
			{
				xmlObject.setAttribute( "class" ,  mixProperty );
			}		
		}
	}

	
	xmlObject.asXml = function ()
	{
		var tempDiv = document.createElement("div");
		tempDiv.appendChild(this);
		var strXml = tempDiv.innerHTML;
		tempDiv.removeChild(this);
		return strXml;
	}
	

	return xmlObject;
}

window.beatifulXml = function beatifulXml( objDomXml )
{
	try	
	{
		var tempDiv = document.createElement("div");
		tempDiv.appendChild(objDomXml);
		var strXml = tempDiv.innerHTML;
		tempDiv.removeChild(objDomXml);
	}
	catch(e)
	{
			strXml = objDomXml;
	}	
	
	strXml = replaceAll(strXml, "<", "\n&lt;");
	strXml = replaceAll(strXml, ">", "&gt;\n");
	strXml = replaceAll(strXml, "\n", "ENTER");
	strXml = replaceAll(strXml, "ENTER", "</br>\n");
	return strXml;
}