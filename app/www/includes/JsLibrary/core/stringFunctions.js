
/**
 * Muda alguns caracters para o modo html
 * Change some chars to html mode
 *
 @param string strText
 @return string
 */
function entityify( strText )
{
	if (isUndefined(strText))
	{
		return "undefined";
	}

	strText = strText + "";

	strText = strText
	.replace( /&/g , "&amp;" )
	.replace( /</g , "&lt;" )
	.replace( />/g , "&gt;" );

	strText = strText;
	strText = replaceAll( strText, "\n", "<br/>\n" );
	strText = replaceAll( strText, " ", "&nbsp;" );

	return strText;
};

function quote( strText )
{
	var c, i, l = strText.length, o = '"';
	for (i = 0; i < l; i += 1)
	{
		c = strText.charAt(i);
		if (c >= ' ')
		{
			if (c == '\\' || c == " \"" )
			{
				o += '\\';
			}
		o += c;
		}
		else
		{
			switch (c)
			{
				case '\b':
					o += '\\b';
					break;
				case '\f':
					o += '\\f';
					break;
				case '\n':
					o += '\\n';
					break;
				case '\r':
					o += '\\r';
					break;
				case '\t':
					o += '\\t';
					break;
				default:
					c = c.charCodeAt();
					o += '\\u00' + Math.floor(c / 16).toString(16) +
						(c % 16).toString(16);
			}
		}
	}
	return o + '"';
};

/**
 * Remove os espacos em branco dos extremos de um string
 * Remove the white spaces os the limits of some string
 *
 @param string strText
 @return string
 */
function trim( strText )
{
	if (! strText)
	{
		return "";
	}
	strText += '';
	return strText.replace(/^\s*(\S*(\s+\S+)*)\s*$/, "$1");
};


/**
 * remove as ocorrencias de um texto nos extremos do outro
 *@param string strText
 *@param sttring strPeace
 *@param bool boolLeft
 *@param bool boolRigth
 *@return string
 */
function trimString ( strText, strPeace, boolLeft, boolRight )
{
	strText += "";

	if (boolLeft == undefined)
	{
		boolLeft = true;
	}

	if (boolRight == undefined)
	{
		boolRight = false;
	}

	intPos = strText.indexOf( strPeace ) ;

	if ( boolLeft )
	while ( intPos == 0 )
	{
		strText = strText.substring( strPeace.length );
		intPos = strText.indexOf( strPeace ) ;
	}

	if ( boolRight )
	while ( intPos == strText.length - strPeace.length )
	{
		strText = strText.substring( 0 , intPos ) ;
		intPos = strText.indexOf( strPeace ) ;
	}

	return strText;
}

/**
 * adiciona texto ao final do corpo do html se existir ou antes caso nao exista
 * add text into end of the body or the html element if that not exist
 * 
 *@param string strText
 *@param object Div objDivPlace
 *@return void
 */
function print( strText , objDivPlace_ )
{
	if (objDivPlace_ == undefined)
	{
		objDivPlace_ = document.body;
	}

	if (objDivPlace_ != undefined)
	{
		objDivPlace_.innerHTML += strText;
	}
	else
	{
		document.write( strText );
	}
}


/**
 * substitui todas as ocorrencias de um string por outro
 * replace all the ocurrence of some string to another
 *
 *@param  string strText
 *@param  string strFinder
 *@param  string strReplacer
 *@return bool
 */
function replaceAll (strText , strFinder, strReplacer)
{
	strText += "";
	strFinder += "";
	strReplacer += "";
	var strSpecials = /(\.|\*|\^|\?|\&|\$|\+|\-|\#|\!|\(|\)|\[|\]|\{|\}|\|)/gi; // :D
	strFinder = strFinder.replace(strSpecials, "\\$1")

	var objRe = new RegExp(strFinder, "gi");
	return strText.replace(objRe, strReplacer);
}

function strCount( strSearch, strText )
{
	strText += "";
	var intCount = 0;

	var intPos = strText.indexOf( strSearch );

	while ( intPos != -1 )
	{
		intCount++;
		strText	= strText.substr( intPos + 1 );
		intPos = strText.indexOf( strSearch );
	}

	return intCount;
}


function str_repeat( strText, intRepeat )
{
	strReturn = "";
	for ( var i = 0; i < intRepeat; ++i)
	{
		strReturn += strText;
	}
	return strReturn;
}

function str_slice_line( strText )
{
	var strReturn = strText;
	strReturn = replaceAll( strText, "\"" , "\\\'\\'" );
//	strReturn = replaceAll( strReturn, "\'" , "\\\'" );
//	strReturn = replaceAll( strReturn, "\n" , "\\n " );
	strReturn = replaceAll( strReturn, "\n" , " " );
	return strReturn;
}

function str_slice( strText )
{
	var strReturn = strText;
	strReturn = replaceAll( strText, "\"" , "\\\\\"" );
	strReturn = replaceAll( strReturn, "\'" , "\\\'" );
	return strReturn;
}

/**
 * Retorna o Html da Tag ( incluindo a mesma )
 * Return the outer Html of the Tag
 *
 */
function getHtmlString( objDomHtml )
{
	if (!objDomHtml)
		return "";

	if ( isString(objDomHtml) || isNumber(objDomHtml) )
	{
		return objDomHtml;
	}

	if ( objDomHtml.outerHTML )
	{
		return( objDomHtml.outerHTML );
	}

	var parentDiv = document.createElement("div");
	parentDiv.appendChild( objDomHtml );
	return parentDiv.innerHTML;
}

function ucfirst( strText )
{
	var strFirstChar = strText.substr( 0 , 1 );
	return strFirstChar.toUpperCase() + strText.substr( 1 );
}

function string_reverse( strText )
{
	strNewText = '';
	for( var i = strText.length - 1 ; i >= 0 ; --i )
	{
		strNewText += strText.charAt( i );
	}
	return strNewText;
}

function strToDate( strDate )
{
    var arrDate = explode( "/" , strDate  );
    var objDate = new Date( arrDate[2] , arrDate[1] - 1 , arrDate[0] );
    return objDate;
}

/**
 * remove from one string all the not numerical (integer) elements
 *
 *
 *
 * @param string Text
 * @return integer
 * @example alert(forceInt("1a2b3c4"))
 */
function forceInt( Text, leftZeros )
{
    Text += '';
	leftZeros = (leftZeros == undefined) ? false : leftZeros;

	Numbers = "1234567890\n";
	NewText = "";
	for( i = 0 ; i < Text.length ; i++ ){
		if( Numbers.indexOf( Text.charAt(i) ) != -1 ){
			NewText += Text.charAt(i);
		}
	}
	if (NewText == ""){
		return "";
	}

	if(!leftZeros){
		// a base tem de ser forcada para 10
		// porque o default ? base 8 caso NewText comece com 0
		NewText = parseInt(NewText,10);
	}

	return( NewText );
}

/**
 * remove from one string all the not numirical (double) elements
 *
 *
 *
 * @param string Text
 * @param string Dot
 * @return double
 * @example alert(forceInt("1a2b3c4.5aX"))
 */
function forceDouble(Text, Dot)
{
	Text = Text.replace( Dot , "." );
	Numbers = "1234567890.\n";
	NewText = "";
	var i = 0;
	for(i=0;i<Text.length;i++)
		{
		if(Numbers.indexOf(Text.charAt(i)) != -1)
			{
			NewText += Text.charAt(i);
			}
		}
	if (NewText == ""){
		return "";
	}
	// a base tem de ser forcada para 10
	// porque o default ? base 8 caso NewText comece com 0
	floatNew = parseFloat(NewText,10);
	NewText = floatNew + "";
	Text = Text.replace( ".", Dot );
	return Text;
}

function replaceWord( strText, strFinder , strReplacer )
{
	strText += '';
	if( strText.indexOf( strFinder ) == 0 )
	{
		strText = strText.replace( strFinder, strReplacer );
	}
	strText = replaceAll( strText , ' '		+ strFinder, ' ' + strReplacer );
	strText = replaceAll( strText , '	'	+ strFinder, '	' + strReplacer );
	strText = replaceAll( strText ,  "\n" + strFinder, "\n" + strReplacer );
	return strText;
}

function replaceWordColor( strText , strFinder, strColor )
{
	return replaceWord( strText , strFinder , '<font style="color: ' + strColor + '">' + strFinder + '</font>' );
}


/**
 * Muda alguns caracters para o modo html
 * Change some chars to html mode
 *
 @param string strText
 @return string
 */
function entityify( strText )
{
	if (isUndefined(strText))
	{
		return "undefined";
	}

	strText = strText + "";

	strText = strText
	.replace( /&/g , "&amp;" )
	.replace( /</g , "&lt;" )
	.replace( />/g , "&gt;" );

	strText = strText;
	strText = replaceAll( strText, "\n", "<br/>\n" );
	strText = replaceAll( strText, " ", "&nbsp;" );

	return strText;
};

function quote( strText )
{
	var c, i, l = strText.length, o = '"';
	for (i = 0; i < l; i += 1)
	{
		c = strText.charAt(i);
		if (c >= ' ')
		{
			if (c == '\\' || c == " \"" )
			{
				o += '\\';
			}
		o += c;
		}
		else
		{
			switch (c)
			{
				case '\b':
					o += '\\b';
					break;
				case '\f':
					o += '\\f';
					break;
				case '\n':
					o += '\\n';
					break;
				case '\r':
					o += '\\r';
					break;
				case '\t':
					o += '\\t';
					break;
				default:
					c = c.charCodeAt();
					o += '\\u00' + Math.floor(c / 16).toString(16) +
						(c % 16).toString(16);
			}
		}
	}
	return o + '"';
};

/**
 * Remove os espacos em branco dos extremos de um string
 * Remove the white spaces os the limits of some string
 *
 @param string strText
 @return string
 */
function trim( strText )
{
	if (! strText)
	{
		return "";
	}
	strText += '';
	return strText.replace(/^\s*(\S*(\s+\S+)*)\s*$/, "$1");
};


/**
 * remove as ocorrencias de um texto nos extremos do outro
 *@param string strText
 *@param sttring strPeace
 *@param bool boolLeft
 *@param bool boolRigth
 *@return string
 */
function trimString ( strText, strPeace, boolLeft, boolRight )
{
	strText += "";

	if (boolLeft == undefined)
	{
		boolLeft = true;
	}

	if (boolRight == undefined)
	{
		boolRight = false;
	}

	intPos = strText.indexOf( strPeace ) ;

	if ( boolLeft )
	while ( intPos == 0 )
	{
		strText = strText.substring( strPeace.length );
		intPos = strText.indexOf( strPeace ) ;
	}

	if ( boolRight )
	while ( intPos == strText.length - strPeace.length )
	{
		strText = strText.substring( 0 , intPos ) ;
		intPos = strText.indexOf( strPeace ) ;
	}

	return strText;
}

/**
 * adiciona texto ao final do corpo do html se existir ou antes caso nao exista
 * add text into end of the body or the html element if that not exist
 * 
 *@param string strText
 *@param object Div objDivPlace
 *@return void
 */
function print( strText , objDivPlace_ )
{
	if (objDivPlace_ == undefined)
	{
		objDivPlace_ = document.body;
	}

	if (objDivPlace_ != undefined)
	{
		objDivPlace_.innerHTML += strText;
	}
	else
	{
		document.write( strText );
	}
}


/**
 * substitui todas as ocorrencias de um string por outro
 * replace all the ocurrence of some string to another
 *
 *@param  string strText
 *@param  string strFinder
 *@param  string strReplacer
 *@return bool
 */
function replaceAll (strText , strFinder, strReplacer)
{
	strText += "";
	strFinder += "";
	strReplacer += "";
	var strSpecials = /(\.|\*|\^|\?|\&|\$|\+|\-|\#|\!|\(|\)|\[|\]|\{|\}|\|)/gi; // :D
	strFinder = strFinder.replace(strSpecials, "\\$1")

	var objRe = new RegExp(strFinder, "gi");
	return strText.replace(objRe, strReplacer);
}

function strCount( strSearch, strText )
{
	strText += "";
	var intCount = 0;

	var intPos = strText.indexOf( strSearch );

	while ( intPos != -1 )
	{
		intCount++;
		strText	= strText.substr( intPos + 1 );
		intPos = strText.indexOf( strSearch );
	}

	return intCount;
}


function str_repeat( strText, intRepeat )
{
	strReturn = "";
	for ( var i = 0; i < intRepeat; ++i)
	{
		strReturn += strText;
	}
	return strReturn;
}

function str_slice_line( strText )
{
	var strReturn = strText;
	strReturn = replaceAll( strText, "\"" , "\\\'\\'" );
//	strReturn = replaceAll( strReturn, "\'" , "\\\'" );
//	strReturn = replaceAll( strReturn, "\n" , "\\n " );
	strReturn = replaceAll( strReturn, "\n" , " " );
	return strReturn;
}

function str_slice( strText )
{
	var strReturn = strText;
	strReturn = replaceAll( strText, "\"" , "\\\\\"" );
	strReturn = replaceAll( strReturn, "\'" , "\\\'" );
	return strReturn;
}

/**
 * Retorna o Html da Tag ( incluindo a mesma )
 * Return the outer Html of the Tag
 *
 */
function getHtmlString( objDomHtml )
{
	if (!objDomHtml)
		return "";

	if ( isString(objDomHtml) || isNumber(objDomHtml) )
	{
		return objDomHtml;
	}

	if ( objDomHtml.outerHTML )
	{
		return( objDomHtml.outerHTML );
	}

	var parentDiv = document.createElement("div");
	parentDiv.appendChild( objDomHtml );
	return parentDiv.innerHTML;
}

function ucfirst( strText )
{
	var strFirstChar = strText.substr( 0 , 1 );
	return strFirstChar.toUpperCase() + strText.substr( 1 );
}

function string_reverse( strText )
{
	strNewText = '';
	for( var i = strText.length - 1 ; i >= 0 ; --i )
	{
		strNewText += strText.charAt( i );
	}
	return strNewText;
}

function strToDate( strDate )
{
    var arrDate = explode( "/" , strDate  );
    var objDate = new Date( arrDate[2] , arrDate[1] - 1 , arrDate[0] );
    return objDate;
}

/**
 * remove from one string all the not numerical (integer) elements
 *
 *
 *
 * @param string Text
 * @return integer
 * @example alert(forceInt("1a2b3c4"))
 */
function forceInt( Text, leftZeros )
{
    Text += '';
	leftZeros = (leftZeros == undefined) ? false : leftZeros;

	Numbers = "1234567890\n";
	NewText = "";
	for( i = 0 ; i < Text.length ; i++ ){
		if( Numbers.indexOf( Text.charAt(i) ) != -1 ){
			NewText += Text.charAt(i);
		}
	}
	if (NewText == ""){
		return "";
	}

	if(!leftZeros){
		// a base tem de ser forcada para 10
		// porque o default ? base 8 caso NewText comece com 0
		NewText = parseInt(NewText,10);
	}

	return( NewText );
}

/**
 * remove from one string all the not numirical (double) elements
 *
 *
 *
 * @param string Text
 * @param string Dot
 * @return double
 * @example alert(forceInt("1a2b3c4.5aX"))
 */
function forceDouble(Text, Dot)
{
	Text = Text.replace( Dot , "." );
	Numbers = "1234567890.\n";
	NewText = "";
	var i = 0;
	for(i=0;i<Text.length;i++)
		{
		if(Numbers.indexOf(Text.charAt(i)) != -1)
			{
			NewText += Text.charAt(i);
			}
		}
	if (NewText == ""){
		return "";
	}
	// a base tem de ser forcada para 10
	// porque o default ? base 8 caso NewText comece com 0
	floatNew = parseFloat(NewText,10);
	NewText = floatNew + "";
	Text = Text.replace( ".", Dot );
	return Text;
}

function replaceWord( strText, strFinder , strReplacer )
{
	strText += '';
	if( strText.indexOf( strFinder ) == 0 )
	{
		strText = strText.replace( strFinder, strReplacer );
	}
	strText = replaceAll( strText , ' '		+ strFinder, ' ' + strReplacer );
	strText = replaceAll( strText , '	'	+ strFinder, '	' + strReplacer );
	strText = replaceAll( strText ,  "\n" + strFinder, "\n" + strReplacer );
	return strText;
}

function replaceWordColor( strText , strFinder, strColor )
{
	return replaceWord( strText , strFinder , '<font style="color: ' + strColor + '">' + strFinder + '</font>' );
}

function xmlentities( strText )
{
	strText += '';
    var strResult = '';
    
    loop_unxmlentities( strText );
    var arrRegularValues = Array(
	'a',	'b',	'c',	'd',	'e',	'f',	'g', 
	'h',	'i',	'j',	'k',	'l',	'm',	'n', 
	'o',	'p',	'q',	'r',	's',	't',	'u', 
	'v',	'x',	'z',	'0',	'1',	'2',	'3', 
	'4',	'5',	'6',	'7',	'8',	'9',	'.', 
	'(',	')',	'{',	'}',	'-',	'+',	'_',
	' '
	);
    for( var i = 0 ; i < strText.length; ++i )
    {
    	if( !IE )
    	{
        	var strChar = strText[ i ];
        }
        else
        {
        	var strChar = strText.charAt( i );
        }
        if	( 
        		( array_search( strChar , arrRegularValues ) == -1 ) 
        	&& 
        		( array_search( strChar.toLowerCase() , arrRegularValues ) == -1 ) 
        	)
        {
            var intCod = strChar.charCodeAt( 0 );
            strResult += '&#' + intCod + ';';
        }
        else
        {
            strResult += strChar;
        }
    }
    return strResult;
}

function loop_unxmlentities( strText )
{
	strText += '';
	var strNewText = unxmlentities( strText );
	while( ( strText ) != ( strNewText ) )
	{
		strText = strNewText; 
		strNewText = unxmlentities( strText );
	}	
	return strText;
}

function unxmlentities( strText )
{
	strText += '';
    var strResult = '';
    
	var strBeginSpecial = "&#";
	var strEndSpecial	= ";";
    for( var i = 0 ; i < strText.length; ++i )
    {
       	var strChar = strText.charAt( i );
        if( strChar == strBeginSpecial.charAt( 0 ) )
        {
        	strCodeSpecial = "";
        	j = 0;
			boolSpecial = true;
        	k = i;
        	while( ( strText.charAt( i ) != strEndSpecial ) && ( j < 10 ) )
        	{
	 	      	var strCharSpecial = strText.charAt( i );
	 	      	if( j < strBeginSpecial.length )
	 	      	{
					if( strCharSpecial == strBeginSpecial.charAt( j ) )
					{
						strCodeSpecial += strCharSpecial;
						i++;
						j++;
					}
					else
					{
						boolSpecial = false;
						break;
					}
				}
				else
				{
					strCodeSpecial += strCharSpecial;
					i++;
					j++;
	 	      	}
        	}
        	if( j < 10 && boolSpecial )
        	{
	        	// remove the &# from the begin
				intStart = strBeginSpecial.length;
				strCodeSpecial = strCodeSpecial.substring( intStart );
				intCharCode = parseInt( forceInt( strCodeSpecial ) );
				strCharSpecial = String.fromCharCode( intCharCode );
	        	strResult += strCharSpecial;
        	}
        	else
        	{
        		i = k;
        		strResult += strCodeSpecial;
        	}
        }
        else
        {
            strResult += strChar;
        }
    }
    return strResult;
}

