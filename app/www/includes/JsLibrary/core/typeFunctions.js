/**
 * Confere se o parametro ? externo do javascript
 * Check is the parameter is alien of the javascript
 *
 *@param  objElement
 *@return bool
 */
function isAlien( objElement )
{
	return isObject( objElement ) && typeof objElement.constructor != 'function';
}

/**
 * Confere se o parametro enviado ? um array
 * Check if the parameter sended is one array
 *
 @param mixElement
 @return bool
 */
function isArray( objMix )
{
	return isObject( objMix ) && objMix.constructor == Array;
}

/**
 * Confere se o objeto enviado ? um boleano
 * Check if the parameter sended is one bool
 *
 @param mixElement
 @return bool
 */
function isBoolean( objMix )
{
	return typeof objMix == 'boolean';
}

/**
 * Confere se o objeto enviado ? vazio
 * Check if the object sended is empty
 *
 @param objElement
 @return bool
 */
function isEmpty( objElement )
{
	var strProperty, mixValue;
	if ( isObject( objElement ) )
	{
		for ( strProperty in objElement )
		{
			mixValue = o[ strProperty ];
			if ( isUndefined( mixValue ) && isFunction( mixValue ) )
			{
				return false;
			}
		}
	}
	return true;
}


/**
 * Confere se o parametro enviado ? uma fun??o
 * Check if the parameter sended is one function
 *
 @param mixElement
 @return bool
 */
function isFunction( mixElement )
{
	return typeof mixElement == 'function';
}


function existFunction( strNameFunction )
{
	if ( window[ strNameFunction ] != undefined )
	{
		return isFunction( window[ strNameFunction ] ); 
	}
	else
	{
		return false;
	}  
}

/**
 * Confere se o parametro enviado ? nulo
 * Check if the parameter sended is null
 *
 @param mixElement
 @return bool
 */
function isNull( mixElement )
{
	return typeof mixElement == 'object' && !mixElement;
}


/**
 * Confere se o parametro enviado ? um n?mero
 * Check if the parameter sended is one number
 *
 @param mixElement
 @return bool
 */
function isNumber( mixElement )
{
	return typeof mixElement == 'number' && isFinite( mixElement );
}

/**
 * Confere se o elemento enviado ? um objeto
 * Check if the element sended is one object
 *
 @param mixElement
 @return bool
 */
function isObject( mixElement )
{
	return ( mixElement && typeof mixElement == 'object' ) || isFunction( mixElement );
}

/**
 * Check if the object is native or not
 @param mixElement
 @return bool
 */
function isNative( mixElement )
{
	if( isFunction( mixElement ) )
	{
		var strCodeFunction = mixElement + ' ';
		if ( array_search( 'native' , strCodeFunction ) == -1 )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	} 
}

/**
 * Confere se o elemento enviado ? um texto
 * Check if the element sended is one string
 *
 @param mixElement
 @return bool
 */
function isString( mixElement )
{
	return typeof mixElement == 'string';
}

/**
 * Confere se o elemento enviado ? indefinido
 * Check if the element sended is undefined
 *
 @param mixElement
 @return bool
 */
function isUndefined( mixElement )
{
	return typeof mixElement == 'undefined';
}