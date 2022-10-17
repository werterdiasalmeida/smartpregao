<?php
/*
global $db;
$db = new Database();
*/
function in_array_object( $objElement , $arrObjects )
{
	foreach( $arrObjects as $objElementArray )	
	{
		if( $objElement === $objElementArray )
		{
			return true;
		}
	}
	return false;
}

function array_intersect_object( $arrObject1 , $arrObject2 )
{
	$arrResult = array();
	foreach( $arrObject1 as $objElement1 )	
	{
		foreach( $arrObject2 as $objElement2 )	
		{
//			dbg( $objElement1->getId() . ' x ' . $objElement2->getId() );
			if( $objElement1 === $objElement2 )
			{
				$arrResult[] = $objElement1;
			}
		}
	}
	return $arrResult;
}

function array_unique_object( $arrObject1 )
{
	$arrResult = array();
	foreach( $arrObject1 as $objElement1 )	
	{
		if( !in_array_object( $objElement1 , $arrResult ) )
		{
			$arrResult[] = $objElement1;
		}
	}
	return $arrResult;
}

function orderArrayOfObjectsByMethod( $arrObjects , $strMethodName )
{
	$arrElements = array();
	if( is_array( $arrObjects ) )
	{
		foreach( $arrObjects as $objElement )
		{
			if( !isset( $arrElements[ $objElement->$strMethodName() ] ) )
			{
				$arrElements[ $objElement->$strMethodName() ] = array();
			}
			$arrElements[ $objElement->$strMethodName() ][] = $objElement;
		}
	}
	
	ksort( $arrElements );
	
	$arrResult = array();
	foreach( $arrElements as $arrSameKey )
	{
		foreach( $arrSameKey as $objElement )
		{
			$arrResult[] = $objElement;
		}
	}
	return $arrResult;
}

/**
 * Converte os valores dos tipos nativos do php para o formato adequado ao
 * banco de dados
 * 
 * @return string
 */
function escape( $mixValue , $strType = 'string' )
{
	if( $mixValue === null )
	{
		return 'null';
	}
	if( is_string( $mixValue ) && $strType == 'string' )
	{
		$mixValue = str_replace( '\'' , '\\\'' , $mixValue );
		return '\'' . $mixValue . '\'';
	}
	if( is_string( $mixValue ) && $strType == 'date')
	{
		$mixValue = date( 'Y-m-d' , convertToTimestamp( $mixValue ) );
		return '\'' . $mixValue . '\'';
	}
	if( is_numeric( $mixValue ) )
	{
		return $mixValue;
	}
	if( is_bool( $mixValue ) )
	{
		if( $mixValue )
		{
//					return '\'t\'';
			return 'TRUE';
		}
//				else
		{
//					return '\'f\'';
			return 'FALSE';
		}
	}
	return null;
}

/**
 * Converte os valores dos tipos nativos do banco para o formato adequado
 * do php.
 * 
 * @return undefined
 */
function unescape( $mixValue , $strType )
{
	if( $mixValue === null )
	{
		return null;
	}
	
	switch( $strType )
	{
		case 'string':
		{
			return (string) $mixValue;
			break;
		}
		case 'integer':
		{
			return (integer) $mixValue;
			break;
		}
		case 'date':
		{
			$arrData = explode( '-' , $mixValue );
			$intAno = $arrData[ 0 ];
			$intMes = $arrData[ 1 ];
			$intDia = $arrData[ 2 ];
			$strData = $intDia . '/' . $intMes . '/' . $intAno;
			return $strData;
			break;
		}
		case 'bool':
		{
			return ( $mixValue === 't' || $mixValue === TRUE );
			break;
		}
		case 'double':
		{
			return (double) $mixValue;	
		}
		default:
		{
			throw new Exception( 'Tipo de variavel desconhecida ' . $mixValue  );
		}
	}
}

/**
 * Converte uma data no formato dd/mm/YYYY ou d/m/Y para o formato timestamp
 * 
 * @param string
 * @return integer
 */
function convertToTimestamp( $strDataDiaMesAno )
{
	if( $strDataDiaMesAno == null )
	{
		return null;
	}
	$arrData = explode( '/' , $strDataDiaMesAno );
	$intDia = (integer) $arrData[0];
	$intMes = (integer) $arrData[1];
	$intAno = (integer) $arrData[2];
	return mktime( 0, 0, 0, $intMes , $intDia, $intAno );
}

/**
	 * Altera os caracteres especiais para o formato aceito em
	 * html, xhtml, xml, xslt, etc.
	 *
	 * @param string $strText
	 * @param string $strQuoteStype
	 * @return string
	 */
function xmlentities2( $strText, $strQuoteStyle = ENT_QUOTES, $strCharSet = 'ISO' )
{

	if ( $strCharSet == 'UTF8' )
	{
		$strText = utf8_decode( $strText );
	}

	static $trans;
	if ( !isset( $trans ) )
	{
		$trans = get_html_translation_table( HTML_ENTITIES, $strQuoteStyle );
		foreach ( $trans as $key => $value )
		{
			$trans[ $key ] = '&#' . ord( $key ) . ';';
		}
		$trans[ chr(38) ] = '&';
	}
	return preg_replace( "/&(?![A-Za-z]{0,4}\w{2,3};|#[0-9]{2,5};)/" , "&#38;" , strtr( $strText, $trans ) );
}

function xmlentities( $strText )
{
	$strText .= '';
	$strResult = '';
	
	$strText = loop_unxmlentities( $strText );
	
	$arrRegularValues = array(
	'a',	'b',	'c',	'd',	'e',	'f',	'g', 
	'h',	'i',	'j',	'k',	'l',	'm',	'n', 
	'o',	'p',	'q',	'r',	's',	't',	'u', 
	'v',	'x',	'z',	'0',	'1',	'2',	'3', 
	'4',	'5',	'6',	'7',	'8',	'9',	'w', 
	'(',	')',	'{',	'}',	'-',	'+',	'_',
	'&', 	'#',	';' , 	'.',	' '
	);	
	
	$arrReplaced = array(	"\"",		"'",		"<",		">",		"\\",		"/"		);
	
	$arrReplacer = array(	"&#34;",	"&#39;",	"&#60;",   	"&#62;",	"&#92;",	"&#47;"	);
	
	$strText = str_replace( $arrReplaced , $arrReplacer , $strText );
	      	
	for( $i = 0 ; $i < strlen( $strText ); ++$i )
	{
		$strChar = $strText[ $i ];
		if( ( array_search( $strChar , $arrRegularValues ) === FALSE ) && ( array_search( strtolower( $strChar ) , $arrRegularValues ) === FALSE ) )
		{
			$intCod = ord( $strChar);
			$strResult .= '&#' . $intCod . ';';
		}
		else
		{
			$strResult .= $strChar;
		}
	}
	
	return $strResult;
}

function loop_unxmlentities( $strText )
{
	$strText .= '';
	$strNewText = unxmlentities( $strText );
	while( ( $strText ) != ( $strNewText ) )
	{
		$strText = $strNewText; 
		$strNewText = unxmlentities( $strText );
	}	
	return $strText;
}

function unxmlentities( $strText )
{
	$strText .= '';
    $strResult = '';
    
	$strBeginSpecial = "&#";
	$strEndSpecial	= ";";
	for( $i = 0 ; $i < strlen( $strText ); ++$i )
    {
       	$strChar = $strText[ $i ];
       	if( $strChar == $strBeginSpecial[ 0 ] )
        {
        	$strCodeSpecial = "";
			$k = $i;
			$boolSpecial = true;
        	$j = 0;
        	while( ( $strText[ $i ] != $strEndSpecial ) &&( $j < 10 ) )
        	{
        		$strCharSpecial = $strText[ $i ];
        		if( $j < strlen( $strBeginSpecial ) )
	 	      	{
					if( $strCharSpecial == $strBeginSpecial[ $j ] )
					{
						$strCodeSpecial .= $strCharSpecial;
						$i++;
						$j++;
					}
					else
					{
						$boolSpecial = false;
						break;
					}
				}
				else
				{
					$strCodeSpecial .= $strCharSpecial;
					$i++;
					$j++;
	 	      	}				
        	}
        	
        	if( $j < 10 && $boolSpecial)
        	{
        		// remove the &# from the begin
				$intStart = strlen( $strBeginSpecial );
				$strCodeSpecial = substr( $strCodeSpecial , $intStart );
				$intCharCode = (integer) $strCodeSpecial;
				$strCharSpecial = chr( $intCharCode );
				$strResult .= $strCharSpecial;
        	}
        	else
        	{
        		$i = $k;
        		$strResult .= $strBeginSpecial;
        	}
        }
        else
        {
            $strResult .= $strChar;
        }
    }
    return $strResult;
}

?>
