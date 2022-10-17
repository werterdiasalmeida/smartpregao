/**
 *# Funcoes para lidarem com operacoes de arrays.
 *#
 *# Functions to deal with arrays operations.
 *
 *@link http://www.crockford.com/javascript/remedial.html some of that
 *@date 01/11/2005
 *@version 2.0
 * with many changes
 */

/**
 * Search for a item in the array and return his position if founded or -1
 * in other case
 *@date 2005-11-06 17:35
 *@param mixObject mixElement
 *@param array arrConteiner
 *@return integer
 */
window.array_search = function array_search( mixElement, arrConteiner )
{
	if (!arrConteiner)
		return -1;

	for(var i = 0; i < arrConteiner.length; ++i)
	{
		if (arrConteiner[i] == mixElement)
			return i;
	}
	return -1;
}

/**
 * Merge one or more arrays
 * 
 * @date 07-03-2007 11:44
 */
function array_merge( arrElement1 , arrElement2 )
{
	if( !arrElement1.length )
	{
		arrElement1 = Array( arrElement1 );
	}
	if( !arrElement2.length )
	{
		arrElement2 = Array( arrElement2 );
	}
	
	var arrResult = Array();
	for( var i = 0 ; i < arrElement1.length ; ++i )
	{
		if( forceInt( i ) == ( i ) )
		{
			arrResult[ arrResult.length] = ( arrElement1[ i ] );
		}
		else
		{
			arrResult[ i ] = arrElement1[ i ];
		}		
	}
	for( var i = 0 ; i < arrElement2.length ; ++i )
	{
		if( forceInt( i ) == ( i ) )
		{
			arrResult[ arrResult.length] = ( arrElement2[ i ] );
		}
		else
		{
			arrResult[ i ] = arrElement2[ i ];
		}		
	}
	if( array_merge.arguments.length > 2 )
	{
		var arrArguments = array_merge.arguments;
		for( var i = 2 ; i < arrArguments.length ; ++i )
		{
			arrResult = array_merge( arrResult , arrArguments[ i ] )
		}
	}
	return arrResult;
}


/**
 *  Funcao que realiza um cast array, ou seja, converte um objeto em array //
 * 
 * @param object objElement
 * @return Array
 */
function parseArray( objElement )
{
    // Criando array //
    var arrNew = new Array();
    
    // Caso seja uma colecao //
    if( objElement.length )
    {
        for( var i = 0; i < objElement.length; ++i )
        {
            arrNew[ i ] = objElement[ i ];
        }
    }
    // Caso nao seja uma colecao //
    else
    {
        for( strAttribute in objElement )
        {
            if( isFunction( objElement[ strAttribute ] ) )
            {
                arrNew[ arrNew.length ] = objElement[ strAttribute ];
            }
        }
    }            
    // Retorno da funcao //
    return( arrNew );
}

function showArray( arrElement )
{
	var strText = '';
	for( i in arrElement )
	{
		if( isArray( arrElement[ i ] ) )
		{
			strText += showArray( arrElement[ i ] );
		}
		else
		{
			strText = i + ': ' + arrElement[ i ];
		}
	}
	return strText;
}

/**
 * concatena todos os itens de um array ligando-os por um string cola
 *
 * join all the array elements concatened with one glue string
 * 
 *@param array
 *@param string $str
 *@return strings
 */
function implode( strGlue, arrPieces)
{
	if ( arrPieces == undefined )
		return "undefined";
	var strResult = "";
	for(var i = 0; i < arrPieces.length; ++i)
	{
		if (strResult != "")
		{
			strResult += strGlue;
		}

		strResult += arrPieces[i];
	}
	return strResult;
}

/**
 * funcao implode com palavras especiais como %line %count %row
 * para possibilitar a formação especial da string.
 * 
 * @see implode
 * @see explode
 * @param string strGlue
 * @param arrPieces array
 * @param integer intLineCount
 * @return string
 */
function implode_complex( strGlue, arrPieces , intLineCount_ )
{
	if ( arrPieces == undefined )
		return "undefined";
	if ( intLineCount_ == undefined )
	{
		var intLineCount_ = 0;
	}

	var intLineGlue = strCount( "\n" , strGlue );
	var strResult = "";
	for(var i = 0; i < arrPieces.length; ++i)
	{
		if (strResult != "")
		{
			var strPeace = strGlue;
			strPeace = replaceAll( strPeace , "%line" , intLineCount_ );
			strPeace = replaceAll( strPeace , "%count" , i );
			strPeace = replaceAll( strPeace , "%row" , str_slice_line( arrPieces[i] ) );
			strResult += strPeace;
		}

		intLineCount_ += strCount( "\n" , arrPieces[i] );
		intLineCount_ += intLineGlue;

		strResult += arrPieces[i];
	}
	return strResult;
}

/**
 * Convert a string into a array by the separator.
 * 
 * @param string strSeparator
 * @param string strText
 * @return Array
 */
function explode( strSeparator, strText )
{
	strText += "";
	strSeparator + "";
	var arrResult = Array();
	var intSeparatorPos = strText.indexOf( strSeparator );

	while( intSeparatorPos != -1 )
	{
		var strBefore 	= strText.substr( 0 , intSeparatorPos );
		var strAfter	= strText.substr( intSeparatorPos + 1 );

		arrResult.push( strBefore );
		strText = strAfter;

		var intSeparatorPos = strText.indexOf( strSeparator );

	}
	arrResult.push( strText );

	return arrResult;
}

function array_order_by_value( arrElement )
{
	function sort_it(a,b)
	{
		return(a-b)
	}
	arrElement.sort( sort_it );
	return arrElement;
}

function array_order_by_field( arrElement , strField )
{
	function sort_it_field(a,b)
	{
		return( ( 0 + forceInt( a[ strField ] ) ) - ( 0 + forceInt( b[ strField ] ) )  );
	}
	arrElement.sort( sort_it_field );
	return arrElement;
}

function geraCorPelaMistura( arrRgbCor1 , arrRgbCor2 , intProporcaoCor1 )
{
	var intProporcaoCor2 = 100 - intProporcaoCor1;
	
	var intCorNovaRed 	= Math.round( ( ( intProporcaoCor1 * arrRgbCor1[0] ) + ( intProporcaoCor2 * arrRgbCor2[0] ) ) / 100 );
	var intCorNovaGreen = Math.round( ( ( intProporcaoCor1 * arrRgbCor1[1] ) + ( intProporcaoCor2 * arrRgbCor2[1] ) ) / 100 );
	var intCorNovaBlue	= Math.round( ( ( intProporcaoCor1 * arrRgbCor1[2] ) + ( intProporcaoCor2 * arrRgbCor2[2] ) )  / 100 );
	
	var arrCorNova = new Array( intCorNovaRed , intCorNovaGreen, intCorNovaBlue );

	return arrCorNova;
}

