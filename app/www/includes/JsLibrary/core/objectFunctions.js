/**
 * Replace a mask in a strHtml for the element
 *@param string strHTML
 *@param string Key
 *@param mix objReplacer
 *@param string Key
 *@param mix objReplacer
 *@param string Key
 *@param mix objReplacer
 *@param ...
 *@return string
 */
window.createHtmlByElements = function createHtmlByElements( strHTML )
{
	arrArguments = createHtmlByElements.arguments;
	if (arrArguments.length == 0)
	{
		return strHTML;
	}

	if (arrArguments.length % 2 == 0)
	{
		debuggerAlert( 'invalid paramaters numbers ' );
		return strHTML;
	}

	for (var i = 1; i < arrArguments.length; i += 2)
	{
		strSearch = arrArguments[ i ];
		mixReplace = arrArguments[ i + 1 ];
		if ( !isString(mixReplace) && !isNumber(mixReplace) )
		{
			mixReplace = getHtmlString( mixReplace );
		}

		strHTML = replaceAll( strHTML, strSearch, mixReplace );
	}

	return strHTML;
}


var autoId = 0;

var globalArrAutoId = new Array();
/**
 * Set a unique and valid Id for some object
 *@param object objElement
 *@return string strHeader
 */
window.setId = function setId( objElement, strHeader_ )
{
	if ( (objElement.id == undefined) || (objElement.id == "") )
	{
		if( isUndefined( strHeader_ ) )
		{
			objElement.id = strHeader_ + autoId++;
		}
		else
		{
			if( isUndefined( globalArrAutoId[ strHeader_ ] ) )
			{
				globalArrAutoId[ strHeader_ ] = 0;
			}
			objElement.id = strHeader_ + globalArrAutoId[ strHeader_ ]++;
		}
	}
	return objElement.id;
}


window.removeChildNodes = function removeChildNodes( object )
{
	if( object )
	{
		while ( object.childNodes.length > 0 )
		{
			object.removeChild( object.childNodes[0] , true );
		}
	}
}
	
window.getAnonymousElementByAttribute = function getAnonymousElementByAttribute(Element, attr, value, intDeeper, onlyOne)
{
	if( Element == undefined )
	{
		return Array();
	}
	
	if (intDeeper == undefined)
	{
		intDeeper = 1;
	}
	
	if (onlyOne == undefined)
	{
		onlyOne = true;
	}
		
	var Elements = new Array();
	var children = Element.childNodes;
	var i;
	for (i = 0; i < children.length; i++)
	{
		if ( children[i].getAttribute )
		{
			switch( attr )
			{
				case 'class':
				{
					childAttribute = children[i].className;
					break;
				}
				default:
				{
				    childAttribute = children[i].getAttribute(attr);
				    break;
				}
			}
		    
		}
		else
		    childAttribute = null;
		    
		if (childAttribute == value)
		{
		    Elements[Elements.length] = children[i];
			if (onlyOne)
			{
			    return Elements;
			}
		}
		if ((intDeeper != 1)&&(children[i].tagName)&&(children[i].childNodes.length > 1))
		{
			Elements = Elements.concat(getAnonymousElementByAttribute(children[i],attr,value,intDeeper - 1,onlyOne));
		}
		if (Elements.length > 1 && onlyOne)
		{
		    return Elements;
		}
	}
	return Elements;
}

window.createButton = function createButton( strButtonUp, strButtonDown, strClassName, strFuncOnPress , arrCssAttributes_ )
{
	if( isUndefined( arrCssAttributes_ ) )
	{
		arrCssAttributes_ = Array();
	}
	
	strHtmlButton =  '' +
		'<img ' +
			'class="'+ strClassName + '" ' + 
			'src="' + builImageLink( 'btn/' + strButtonUp ) + '" ' + 
			'style="' + implode( ';' , arrCssAttributes_ ) + '" ' + 
			'onmouseup="'	+ 'this.src = \'' + builImageLink( 'btn/' + strButtonUp ) + '\';" ' +
			'onmouseout="'	+ 'this.src = \'' + builImageLink( 'btn/' + strButtonUp ) + '\';" ' +
			'onmousedown="'	+ 'this.src = \'' + builImageLink( 'btn/' + strButtonDown ) + '\';" ' +
			'onclick="'	+ strFuncOnPress + '" ' +
		'/>'
		
	if( IE )
	{
		// The Internet Explorer don't support  change of innerHTML of some objects
		var objDiv = document.createElement( 'div' );
		objDiv.innerHTML = strHtmlButton;
		var objImg = objDiv.getElementsByTagName( 'img' )[0];
		objImg.onclick = strFuncOnPress;
		strHtmlButton = getHtmlString( objImg );
	}
	return strHtmlButton;
}

function haveClassName( objElement, strClassName )
{
	var strClasses = objElement.className;
	var arrClasses = explode( ' ' , strClasses );
	return( array_search( strClassName , arrClasses ) != -1 );
}

function addClassName( objElement, strClassName )
{
	var strClasses = objElement.className;
	var arrClasses = explode( ' ' , strClasses );
	if( array_search( strClassName , arrClasses )  == -1 )
	{
		arrClasses.push( strClassName );
	}
	strClasses = implode( ' ' , arrClasses );
	objElement.className = strClasses;
}

function addClassesName( objElement, arrClassesToAdd )
{
	var strClasses = objElement.className;
	var arrClasses = explode( ' ' , strClasses );
	for( var i = 0 ; i < arrClassesToAdd.length; ++i )
	{
		var strClassName = arrClassesToAdd[ i ];
		if( array_search( strClassName , arrClasses )  == -1 )
		{
			arrClasses.push( strClassName );
		}
	}
	strClasses = implode( ' ' , arrClasses );
	objElement.className = strClasses;
}

function removeClassName( objElement, strClassName )
{
	var strClasses = objElement.className;
	var arrClasses = explode( ' ' , strClasses );
	var intPosition = array_search( strClassName , arrClasses );
	if( intPosition != -1 )
	{
		arrClasses.splice( intPosition , 1 );		
//		arrClasses[ intPosition ] = undefined;
	}
	strClasses = implode( ' ' , arrClasses );
	objElement.className = strClasses;
}


function removeClassesName( objElement, arrClassesToRemove )
{
	var strClasses = objElement.className;
	var arrClasses = explode( ' ' , strClasses );
	
	for( var i = 0 ; i < arrClassesToRemove.length; ++i )
	{
		var strClassName = arrClassesToRemove[ i ];
		var intPosition = array_search( strClassName , arrClasses );
		if( intPosition != -1 )
		{
			arrClasses.splice( intPosition , 1 );
//			arrClasses[ intPosition ] = undefined;
		}
	}
	strClasses = implode( ' ' , arrClasses );
	objElement.className = strClasses;
}