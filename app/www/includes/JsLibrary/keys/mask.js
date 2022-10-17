/**
 * Check if the pressed key is a number or special key, and remove the not numerical elements of the content /
 *
 *
 *
 * @required forceInt
 * @required forceDouble
 * @param object objElement
 * @return boolean
 * @example <input type="text" onkeypress="return numberMask(this, event, false)"  />
 */
window.numberMask = function numberMask( objElement, Event, isDouble, Dot )
{
	isDouble = (isDouble == undefined) ? false : isDouble;
	Dot = (Dot == undefined) ? "." : Dot;

	var intKeyCode = getIntKeyCode( Event );
	var strKeyType = getKeyType( intKeyCode );

	switch( Dot )
	{
		case "." :
			intDotCode = 190;
		break;
		case "," :
			intDotCode = 188;
		break;
		case ";" :
			intDotCode = 191;
		break;
		case "/" :
			intDotCode = 191;
		break;
		default:
			intDotCode = -1;
		break;
	}

	if ( intDotCode == -1 )
	{
		return false;
	}

	if ( ( isDouble )
	  && ( intKeyCode == intDotCode )
	  && ( array_search( Dot , objElement.value ) == -1 ) )
	{
		// Dot //
		return true;
	}


	if( ( strKeyType == "number" )
	 || ( strKeyType == "position" )
	 || ( strKeyType == "Fn" )
	 || ( strKeyType == "backspace" )
	 || ( strKeyType == "delete" )
	 || ( strKeyType == "tab" ) )
	{
		return true;
	}

	return false;
}

/**
 * integer Mask /
 *
 *
  * @required numberMask
 * @param object objElement
 * @return boolean
 * @example <input type="text" onkeypress="return integerMask(this,event)"  />
 */
window.integerMask = function integerMask( objElement, Event, maxlenght )
{
	objElement.value = forceInt( objElement.value, true );
	if( maxlenght == undefined ){
		return( numberMask( objElement, Event, false ) );
	} else {
		return( numberMask( objElement, Event, false ) && maxLengthMask( objElement, Event, maxlenght ) );
	}
}

/**
 * double Mask 
 *
 *
 * @required numberMask
 * @param object objElement
 * @return boolean
 * @example <input type="text" onkeypress="return doubleMask(this, event, '.' )"  />
 */
window.doubleMask = function doubleMask( objElement, Event, Dot )
{
	return numberMask( objElement, Event, true, Dot );
}

/**
 * put the integerMask in some object
 *
 *
 * @required integerMask
 * @param object objElement
 * @return void
 * @example integerMaksObject( document.getElementById( 'myNode' ) );
 */
window.integerMaskObject = function integerMaskObject( objElement )
{
	if (objElement.onkeypress)
	{
		objElement.onkeypress = " return integerMask( this , event ) ";
	}
	else
	{
		objElement.setAttribute( "onkeypress" , " return integerMask( this , event ) " );
	}
}

/**
 * put the doubleMask in some object
 *
 *
 * @required doubleMask
 * @param object objElement
 * @return void
 * @example doubleMaskObject( document.getElementById( 'myNode' ) );
 */
window.doubleMaskObject = function doubleMaskObject( objElement )
{
	if (objElement.onkeypress)
	{
		objElement.onkeypress = " return doubleMask( this , event ) ";
	}
	else
	{
		objElement.setAttribute( "onkeypress" , " return doubleMask( this , event ) " );
	}
}
/**
 * return the substring from the text since the first while the chars be inside in the group
 *
 *
 * @param string Texto
 * @param string ConjuntoValidos
 * @return string Texto
 * @example alert( filtra( "ExAmPlE WiTh SoMe StRiNg", "abcdefghijklmnoprstuvxz" ) );
 */
window.filtra = function filtra(Texto,ConjuntoValidos)
{
	for(i=0;i<Texto.length;i++)
	{
		Letra = Texto.charAt(i);
		if (ConjuntoValidos.indexOf(Letra) == -1)
		{
			if (i == 0)
			{
				return "";
			}
			else
			{
				return Texto.substring(0,i);
			}
		}
	}
	return Texto;
}

/**
 * change the value of some object
 *
 *
 * @required filtra
 * @param string Texto
 * @param string ConjuntoValidos
 * @return void
 * @example filtraObjeto( document.getElementById( 'myNode' , "123456789-" ) );
 */
window.filtraObjeto = function filtraObjeto(Obj,ConjuntoValidos)
{
	if (Obj.value)
	{
		TextoNovo = filtra(Obj.value,ConjuntoValidos);
		Obj.value = TextoNovo;
	}
	else if (Obj.text)
	{
		TextoNovo = filtra(Obj.text,ConjuntoValidos);
		Obj.text = TextoNovo;
	}
	else if (Obj.textContent)
	{
		TextoNovo = filtra(Obj.textContent,ConjuntoValidos);
		Obj.textContent = TextoNovo;
	}
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
window.forceInt = function forceInt( Text, leftZeros )
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
window.forceDouble = function forceDouble(Text, Dot)
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

/**
 * Check if the pressed key is a number or special key, and remove the not numerical elements of the content /
 *
 *
 *
 * @required forceInt
 * @required forceDouble
 * @param object objElement
 * @return boolean
 * @example <input type="text" onkeypress="return numberMask(this, event, false)"  />
 */
window.numberMask = function numberMask( objElement, Event, isDouble, Dot )
{
	isDouble = (isDouble == undefined) ? false : isDouble;
	Dot = (Dot == undefined) ? "." : Dot;

	var intKeyCode = getIntKeyCode( Event );
	var strKeyType = getKeyType( intKeyCode );

	switch( Dot )
	{
		case "." :
			intDotCode = 190;
		break;
		case "," :
			intDotCode = 188;
		break;
		case ";" :
			intDotCode = 191;
		break;
		case "/" :
			intDotCode = 191;
		break;
		default:
			intDotCode = -1;
		break;
	}

	if ( intDotCode == -1 )
	{
		return false;
	}

	if ( ( isDouble )
	  && ( intKeyCode == intDotCode )
	  && ( array_search( Dot , objElement.value ) == -1 ) )
	{
		// Dot //
		return true;
	}


	if( ( strKeyType == "number" )
	 || ( strKeyType == "position" )
	 || ( strKeyType == "Fn" )
	 || ( strKeyType == "backspace" )
	 || ( strKeyType == "delete" )
	 || ( strKeyType == "tab" ) )
	{
		return true;
	}

	return false;
}

/**
 * integer Mask /
 *
 *
  * @required numberMask
 * @param object objElement
 * @return boolean
 * @example <input type="text" onkeypress="return integerMask(this,event)"  />
 */
window.integerMask = function integerMask( objElement, Event, maxlenght )
{
	objElement.value = forceInt( objElement.value, true );
	if( maxlenght == undefined ){
		return( numberMask( objElement, Event, false ) );
	} else {
		return( numberMask( objElement, Event, false ) && maxLengthMask( objElement, Event, maxlenght ) );
	}
}

/**
 * double Mask /
 *
 *
 * @required numberMask
 * @param object objElement
 * @return boolean
 * @example <input type="text" />
 */
window.doubleMask = function doubleMask( objElement, Event, Dot )
{
	return numberMask( objElement, Event, true, Dot );
}

/**
 * put the integerMask in some object
 *
 *
 * @required integerMask
 * @param object objElement
 * @return void
 * @example integerMaksObject( document.getElementById( 'myNode' ) );
 */
window.integerMaskObject = function integerMaskObject( objElement )
{
	if (objElement.onkeypress)
	{
		objElement.onkeypress = " return integerMask( this , event ) ";
	}
	else
	{
		objElement.setAttribute( "onkeypress" , " return integerMask( this , event ) " );
	}
}

/**
 * put the doubleMask in some object
 *
 *
 * @required doubleMask
 * @param object objElement
 * @return void
 * @example doubleMaskObject( document.getElementById( 'myNode' ) );
 */
window.doubleMaskObject = function doubleMaskObject( objElement )
{
	if (objElement.onkeypress)
	{
		objElement.onkeypress = " return doubleMask( this , event ) ";
	}
	else
	{
		objElement.setAttribute( "onkeypress" , " return doubleMask( this , event ) " );
	}
}
/**
 * return the substring from the text since the first while the chars be inside in the group
 *
 *
 * @param string Texto
 * @param string ConjuntoValidos
 * @return string Texto
 * @example alert( filtra( "ExAmPlE WiTh SoMe StRiNg", "abcdefghijklmnoprstuvxz" ) );
 */
window.filtra = function filtra(Texto,ConjuntoValidos)
{
	for(i=0;i<Texto.length;i++)
	{
		Letra = Texto.charAt(i);
		if (ConjuntoValidos.indexOf(Letra) == -1)
		{
			if (i == 0)
			{
				return "";
			}
			else
			{
				return Texto.substring(0,i);
			}
		}
	}
	return Texto;
}


/**
 * change the value of some object
 *
 *
 * @required filtra
 * @param string Texto
 * @param string ConjuntoValidos
 * @return void
 * @example filtraObjeto( document.getElementById( 'myNode' , "123456789-" ) );
 */
window.filtraObjeto = function filtraObjeto(Obj,ConjuntoValidos)
{
	if (Obj.value)
	{
		TextoNovo = filtra(Obj.value,ConjuntoValidos);
		Obj.value = TextoNovo;
	}
	else if (Obj.text)
	{
		TextoNovo = filtra(Obj.text,ConjuntoValidos);
		Obj.text = TextoNovo;
	}
	else if (Obj.textContent)
	{
		TextoNovo = filtra(Obj.textContent,ConjuntoValidos);
		Obj.textContent = TextoNovo;
	}
}


/**
 * mask to phone [(numbers)]0..1 + [ numbers + ["-"]0..1 ]* to some value of object
 *
 *
 * @required implode
 * @required explode
 * @required filtraObjeto
 * @required strCount
 * @param object <input> Obj
 * @return void
 * @example <input type="text" onkeyup="return ChecaTeclasTelefone(this)"  />
 */
window.ChecaTeclasTelefone = function ChecaTeclasTelefone(Obj)
{
	filtraObjeto(Obj,"01234567890-()");
	if ((strCount(Obj.value,"(")) > 1)
	{
		NovoArray = Explode(Obj.value,"(");
		Obj.value = Implode(NovoArray,"(",2);
	}
	if ((strCount(Obj.value,")")) > 1)
	{
		NovoArray = Explode(Obj.value,")");
		Obj.value = Implode(NovoArray,")",2);
	}
	PosAbre		= Obj.value.indexOf("(");
	PosFecha         = Obj.value.indexOf(")");

	// Parenteses Invalidos {...)(..}  {...)..} //
	if          (((PosAbre == -1) && (PosFecha != -1)) || ((PosAbre != -1) && (PosFecha != -1) && (PosAbre > PosFecha)))
	{
		Obj.value = Obj.value.substring(0,PosFecha) + Obj.value.substring(PosFecha+1,Obj.value.length);
	}
}

 /*
 * Put the phone mask and a max length in the sended object.
 *
 *
 * @required ChecaTeclasTelefone
 * @param object <input> Obj
 * @param integer Tamanho
 * @return void
 * @example MascaraParaTelefone( document.getElementById( 'myNode' ) , 10 );
 */
window.MascaraParaTelefone = function MascaraParaTelefone(Obj,Tamanho)
{
	if (!Obj)
	{
		return;
	}
	Obj.maxlength   = Tamanho;
	Obj.onkeypress = ChecaTeclasTelefone;
	Obj.onkeyup	     = ChecaTeclasTelefone;
}

/**
 * Double numbers mask with limit of size before and after the separator
 *
 *
 *
 * @required explode
 * @param object <input> Obj
 * @param integer AntesDaVirgula
 * @param integer DepoisDaVirgula
 * @param string Separador
 * @example <input type="text" onkeyup="MascaraReal( this , 3 , 4 )" />
 */
window.MascaraReal = function MascaraReal(Obj, AntesDaVirgula, DepoisDaVirgula, Separador)
{
	if (Separador == undefined )
	{
		Separador = ".";
	}

	var ConjuntoValidos = "0123456789" + Separador;

	if (Obj.value == "")
	{
		return false;
	}

	var Pedacos = Explode(Obj.value,Separador);

	if (Pedacos.length > 1)
	{
		var Texto = Pedacos[0] + Separador + Pedacos[1];
		var TemSeparador = true;
	}
	else
	{
		var TemSeparador = false;
		var Texto = Obj.value;
	}

	for( var i = 0 ; i < Texto.length ; ++i)
	{
		var Letra = Texto.charAt(i);
		if (ConjuntoValidos.indexOf(Letra) == -1)
		{
			if (i == 0)
			{
				Texto = "";
			}
			else
			{
				Texto = Texto.substring(0,i);
				break;
			}
		}
	}

	var Pedacos = Explode(Texto,Separador);
	var Texto0 = "";
	var Texto1 = "";

	if (Pedacos[0] != undefined)
	{
		Texto0 = Pedacos[0];
		if (Texto0.length > AntesDaVirgula)
		{
			Texto1 = Texto0.substring(AntesDaVirgula,Texto0.length) + Texto1;
			Texto0 = Texto0.substring(0,AntesDaVirgula);
		}
		if (Pedacos[1] != undefined)
		{
			Pedacos[1] = Texto1 + Pedacos[1];
		}
		else
		{
			Pedacos[1] = Texto1;
		}
	}

	if (Pedacos[1] != undefined)
	{
		Texto1 = Pedacos[1];
		if (Texto1.length >= DepoisDaVirgula)
		{
			Texto1 = Texto1.substring(0,DepoisDaVirgula);
		}
	}

	if (TemSeparador||(Texto1 != ""))
	{
		Obj.value = Texto0 + Separador + Texto1;
	}
	else
	{
		Obj.value = Texto0;
	}

	if (Obj.value.length > DepoisDaVirgula + AntesDaVirgula)
	{
		Obj.value = Obj.value.substring(0,DepoisDaVirgula + AntesDaVirgula);
	}

    return true;
}


/**
 * Apply the double mask in some object
 *
 *
 *
 * @required floatMask
 * @param object <input> Obj
 * @param integer AntesDaVirgula
 * @param integer DepoisDaVirgula
 * @param string Separador
 * @example AplicaMascaraParaReais( document.getElementById( 'MyNode' , 10, 3 , "." ) );
 */
window.AplicaMascaraParaReais = function AplicaMascaraParaReais(Obj,AntesDaVirgula,DepoisDaVirgula,Separador)
{
	if (Separador == undefined )
	{
		Separador = ".";
	}

	Obj.onkeypress = "return FloatMask( this , " +  parseInt( AntesDaVirgula ) + " , " + parseInt(DepoisDaVirgula) + ", '" + Separador + "' );";
}

/**
 * Apply the double mask in some object in one event
 *
 *
 *
 * @required explode
 * @param object <input> Obj
 * @param event Event
 * @param integer intBefore
 * @param integer intAfter
 * @param string Dot
 * @example <input type="text" onkeypress="return floatMask(this, event, 3, 2, '.' )"  />
 */
window.floatMask = function floatMask( objElement, event, intBefore, intAfter, Dot )
{
    if (Dot == undefined)
    {
        Dot = ".";
    }

    if (intAfter == undefined)
    {
        intAfter = 2;
    }

    if ( doubleMask(objElement,event, Dot) )
    {
        MascaraReal( objElement , intBefore , intAfter, Dot ); return true;
    }
    else
    {
        return false;
    }
}

/**
 * Check if the text has value in most of then sended length
 *
 *
 * @param object objElement
 * @return boolean
 * @example <input type="text" onkeypress="return maxLengthMask(this, event, 10 )"  />
 */
window.maxLengthMask = function maxLengthMask( objElement, Event, intLength )
{
	var textContent = "";

	if (objElement.value)
	{
		if ( objElement.value.length > intLength )
		{
			objElement.value = objElement.value.substring( 0 , intLength );
			return false;
		}
		textContent = objElement.value;
	}

	if (objElement.textContent)
	{
		if ( objElement.textContent.length > intLength )
		{
			objElement.textContent = objElement.textContent.substring( 0 , intLength );
			return false;
		}
		textContent = objElement.textContent;
	}

	if (objElement.text)
	{
		if ( objElement.text.length > intLength )
		{
			objElement.text = objElement.text.substring( 0 , intLength );
			return false;
		}
		textContent = objElement.text;
	}

	if (textContent.length < intLength)
	{
		return true;
	}

	var intKeyCode = getIntKeyCode( Event );
	var strKeyType = getKeyType( intKeyCode );

	if(
		( strKeyType == "position" )
	 || ( strKeyType == "Fn" )
	 || ( strKeyType == "backspace" )
	 || ( strKeyType == "delete" )
	 || ( strKeyType == "tab" ) )
	{
		return true;
	}

	Event = false;
	return false;
}

window.setCaretToEnd = function setCaretToEnd(control)
{
    if (control.createTextRange){
        var range = control.createTextRange();
        range.collapse(false);
        range.select();
    }
    else if(control.setSelectionRange){
        control.focus();
        var len = control.value.length;
        control.setSelectionRange(len,len);
    }
}

window.unFormatInput = function unFormatInput( objElement, format )
{
	switch(format)
	{
		case "percent":
		case "money":
			strOut = "" + forceInt( objElement.value );
			objElement.value = strOut;
		break;
		case "date":
			objElement.value = forceInt( objElement.value, true );
		break;
		case "hour":
			objElement.value = forceInt( objElement.value, true );
		break;
	}

	// o IE mistura alteracoes de valor com alteracoes de foco
	if(IE) setCaretToEnd(objElement);

}

window.formatInput = function formatInput( mixElement, format )
{
	debuggerEnterFunction('formatInput');
	if( isObject( mixElement ) )
	{
		var strOut = mixElement.value;
	}
	else
	{
		var strOut = mixElement;
	}

	if( strOut + "" != "" )
	{
		switch(format)
		{
			case "money":
				// strOut vale a string sem zeros a esquerda
				strOut = "" + strOut;
				strOut = "" + forceInt(strOut);

				// coloca virgula
				var regex = /([^,])(\d{2})$/
				if( regex.test( strOut ) ){
					strOut = strOut.replace( regex, "$1,$2" );
				} else {
					strOut = "000".concat(strOut);
					strOut = strOut.replace( /.*(\d)(\d{2})$/, "$1,$2");
				}

				// coloca ponto
				regex = /(\d+)(\d{3})(.|,)/
				while( regex.test( strOut ) ){
					strOut = strOut.replace( regex, "$1.$2$3" );
				}

				// coloca R$
				strOut = "R$ " + strOut;
			break;
			case "date":
//				strOut = ( strOut.length != 8 ) ? "" : strOut.replace( /(\d{2})(\d{2})(\d{4})/, "$1/$2/$3" );
			break;
			case "hour":
				if (
						( strOut.length != 5  )
						||
						( forceInt( strOut.substring(0,2) ) > 23 )
						||
						( forceInt( strOut.substring(3,5) ) > 59 )
					)
				{
					strOut = "";
				}
				else
				{
					strOut = strOut.replace( /(\d{2})(\d{2})/, "$1:$2" );
				}
			break;
			case "percent":

				strOut = "" + strOut;
				strOut = "" + forceInt(strOut);

				if( strOut.length < 3 )
				{
					for( var n = 3 ; n >= strOut.length; --n )
					{
						strOut = "0" + strOut;
					}
				}


				strOut = strOut.replace( /(\d+)(\d{2})$/, "$1,$2");
				strOut += "%";

				/*// strOut vale a string sem zeros a esquerda
				strOut = "" + strOut;
				strOut = "" + forceInt(strOut);

				// coloca virgula
				var regex = /([^,]),(\d{2})$/
				if( regex.test( strOut ) ){
					strOut = strOut.replace( regex, "$1,$2" );

				} else {
					strOut = "000".concat(strOut);
					strOut = strOut.replace( /.*(\d)(\d{2})$/, "$1,$2");

				}

				// coloca %
				strOut = strOut + "%";*/

			break;
		}
	}

	debuggerOutFunction('formatInput');

	if( isObject( mixElement ) )
	{
		mixElement.value = strOut;
	}
	else
	{
		return( strOut );
	}

}


window.formatKey = function formatKey( objElement, Event, maxlenght, format )
{
	boolCtrl = Event.ctrlKey;
	if( boolCtrl )
	{
		return true;
	}
	switch(format)
	{
		case "percent":
			return percentMask( objElement, Event, maxlenght);
		case "money":
			return moneyMask( objElement, Event, maxlenght );
		break;
		case "date":
		case "hour":
		default:
			return integerMask( objElement, Event, maxlenght );
		break;
	}
}

window.percentToFloat = function percentToFloat( strPercent , nanReturn )
{
	nanReturn = (nanReturn == undefined) ? 0 : nanReturn;

	floPercent = forceInt( strPercent );

	if( floPercent == "" ){
		floPercent = nanReturn;
	} else {
		floPercent = floPercent/100;
	}

	return( floPercent );
}

window.moneyToFloat = function moneyToFloat( strMoney , nanReturn )
{
	nanReturn = (nanReturn == undefined) ? 0 : nanReturn;

	floMoney = forceInt( strMoney );

	if( floMoney == "" ){
		floMoney = nanReturn;
	} else {
		floMoney = floMoney/100;
	}

	return( floMoney );
}

window.floatToPercent = function floatToPercent( floPercent )
{
	floPercent *= 10000;
	floPercent = parseInt( floPercent ) / 100;
	strPercent = formatInput( floPercent, "percent" );
	return( strPercent );
}

window.floatToMoney = function floatToMoney( floMoney )
{
	strMoney = formatInput( Math.round( floMoney*100 ), "money" );

	return( strMoney );
}


/**
 * Class UpdateInterdependentFields
 */
window.UpdateInterdependentFields = function UpdateInterdependentFields()
{
	/**
	 * Money mask relative to the percent value
	 * @var object Input
	 * @acess private
	 */
	this.objInputMoney = null;

	/**
	 * Id of the Object
	 * @var string
	 * @acess private
	 */
	this.strId = null;

	/**
	 * Input with total value ( with ou without money mask )
	 * @var object Input
	 * @acess private
	 */
	this.objInputTotal = null;

	/**
	 * Input with percent mask relative to the total value
	 * @var object Input
	 * @acess private
	 */
	this.objInputPercent = null;

	/**
	 * Percent value of the objInputPercent
	 * @var float
	 * @acess private
	 */
	this.floPercentValue = 0;

	/**
	 * Money value of the objInputMoney
	 * @var float
	 * @acess private
	 */
	this.floMoneyValue = 0;

	/**
	 * Total value of the objInputTotal
	 * @var float
	 * @acess private
	 */
	this.floTotalValue = 0;

	/**
	 * Old Percent value of the objInputPercent
	 * @var float
	 * @acess private
	 */
	this.floOldPercentValue = 0;

	/**
	 * Old Money value of the objInputMoney
	 * @var float
	 * @acess private
	 */
	this.floOldMoneyValue = 0;

	/**
	 * Old Total value of the objInputTotal
	 * @var float
	 * @acess private
	 */
	this.floOldTotalValue = 0;

	/**
	 * Percent value of the objInputPercent has change
	 * @var bool
	 * @acess private
	 */
	this.boolChangePercentValue = false;

	/**
	 * Money value of the objInputMoney has change
	 * @var bool
	 * @acess private
	 */
	this.boolChangeMoneyValue = false;

	/**
	 * Total value of the objInputTotal has change
	 * @var bool
	 * @acess private
	 */
	this.boolChangeTotalValue = false;

	/**
	 * Percentual is not about the total but about the descount off it
	 * @var bool
	 * @acess private
	 */
	this.boolPercentIsOff = false;

	/**
	 * Update the floernal float values with the values of the objects
	 * @return void
	 * @acess private
	 */
	this.updateFloatValues = function updateFloatValues()
	{
		debuggerEnterFunction( 'UpdateInterdependentFields.updateFloatValues' );
		if ( this.objInputMoney != null )
		{
			var intNewValue = moneyToFloat( this.objInputMoney.value , 0 );
			this.boolChangeMoneyValue = ( this.floMoneyValue != intNewValue );

			if ( this.boolChangeMoneyValue )
			{
				this.floMoneyValue	= parseInt( intNewValue * 100) / 100;
			}
		}
		if ( this.objInputPercent != null )
		{
			var intNewValue = percentToFloat( this.objInputPercent.value , 0 );
			this.boolChangePercentValue = ( this.floPercentValue != intNewValue );

			if ( this.boolChangePercentValue )
			{
				this.floPercentValue = intNewValue;
			}
		}
		if ( this.objInputTotal != null )
		{
			if ( this.objInputTotal.value.indexOf("$") != -1 )
			{
				// is with the money mask //
				var intNewValue = moneyToFloat( this.objInputTotal.value , 0 );
			}
			else
			{
				// it is just the number value //
				var intNewValue = parseFloat( this.objInputTotal.value );
			}

			this.boolChangeTotalValue = ( this.floTotalValue != intNewValue );

			if ( this.boolChangeTotalValue )
			{
				this.floTotalValue	= parseInt( intNewValue * 100) / 100;
			}
		}
		debuggerOutFunction();
	}

	/**
	 * Change the id of Object
	 *
	 * @param string strId
	 * @return void
	 * @acess public
	 */
	this.setId = function setId( strId )
	{
		debuggerEnterFunction( 'UpdateInterdependentFields.setId' );
		this.strId = strId;
		debuggerOutFunction();
	}

	/**
	 * Get the actual value of the id of Object
	 * @return object Input
	 * @acess public
	 */
	this.getId = function getId()
	{
		debuggerEnterFunction( 'UpdateInterdependentFields.getId' );
		debuggerOutFunction();
		return this.strId;
	}


	/**
	 * Change the Discount Mode
	 *
	 * @param string strId
	 * @return void
	 * @acess public
	 */
	this.setDiscountMode = function setDiscountMode( boolDiscountMode )
	{
		debuggerEnterFunction( 'UpdateInterdependentFields.setDiscountMode' );
		this.boolPercentIsOff = ( boolDiscountMode == true );
		debuggerOutFunction();
	}

	/**
	 * Get if the Discount Mode is active
	 * @return object Input
	 * @acess public
	 */
	this.getDiscountMode = function getDiscountMode()
	{
		debuggerEnterFunction( 'UpdateInterdependentFields.getId' );
		debuggerOutFunction();
		return this.boolPercentIsOff;
	}

	/**
	 * Change the objInputMoney to the object sended by id
	 * return true if successful and false other wise
	 *
	 * @param string Object Input Money Id
	 * @return bool
	 * @acess public
	 */
	this.setInputMoney = function setInputMoney( strMoneyId )
	{
		debuggerEnterFunction( 'UpdateInterdependentFields.setInputMoney' );
		var objInputMoney	= document.getElementById( strMoneyId );

		if ( objInputMoney == null )
		{
			// error //
			debuggerAlert( 'error' );
			debuggerOutFunction();
			return false;
		}

		this.objInputMoney = objInputMoney;

		debuggerOutFunction();
		return true;
	}

	/**
	 * Get the actual value of the object input money
	 * @return object Input
	 * @acess public
	 */
	this.getInputMoney = function getInputMoney()
	{
		debuggerEnterFunction( 'UpdateInterdependentFields.getInputMoney' );
		debuggerOutFunction();
		return this.objInputMoney;
	}

	/**
	 * Change the objInputMoney to the object sended by id
	 * return true if successful and false other wise
	 *
	 * @param string Object Input Money Id
	 * @return bool
	 * @acess public
	 */
	this.setInputPercent = function setInputPercent( strPercentId )
	{
		debuggerEnterFunction( 'UpdateInterdependentFields.setInputPercent' );
		var objInputPercent	= document.getElementById( strPercentId );

		if ( objInputPercent == null )
		{
			// error //
			debuggerAlert( 'error' );
			debuggerOutFunction();
			return false;
		}

		this.objInputPercent = objInputPercent;

		debuggerOutFunction();
		return true;
	}

	/**
	 * Get the actual value of the object input money
	 * @return object Input
	 * @acess public
	 */
	this.getInputPercent = function getInputPercent()
	{
		debuggerEnterFunction( 'UpdateInterdependentFields.getInputPercent' );
		debuggerOutFunction();
		return this.objInputPercent;
	}

	/**
	 * Change the objInputMoney to the object sended by id
	 * return true if successful and false other wise
	 *
	 * @param string Object Input Money Id
	 * @return bool
	 * @acess public
	 */
	this.setInputTotal = function setInputTotal( strTotalId )
	{
		debuggerEnterFunction( 'UpdateInterdependentFields.setInputTotal' );
		var objInputTotal	= document.getElementById( strTotalId );
		if ( objInputTotal == null )
		{
			// error //
			debuggerAlert( 'error' );
			debuggerOutFunction();
			return false;
		}

		this.objInputTotal = objInputTotal;

		debuggerOutFunction();
		return true;
	}

	/**
	 * Get the actual value of the object input money
	 * @return object Input
	 * @acess public
	 */
	this.getInputTotal = function getInputTotal()
	{
		debuggerEnterFunction( 'UpdateInterdependentFields.getInputTotal' );
		debuggerOutFunction();
		return this.objInputTotal;
	}

	/**
	 * Update Money Field From the Percent Field and Total
	 * @return void
	 * @acess public
	 */
	this.updateMoneyFromPercentField = function updateMoneyFromPercentField()
	{
		debuggerEnterFunction( 'UpdateInterdependentFields.updateMoneyFromPercentField' );
		this.updateFloatValues();

		if ( ! this.boolChangePercentValue && ! this.boolChangeTotalValue )
		{
			// not changed //
			this.clearChanges();
			debuggerOutFunction();
			return;
		}

		if ( ! this.boolPercentIsOff )
		{
			var floNewMoneyValue = parseInt( this.floTotalValue * this.floPercentValue ) / 100;
		}
		else
		{
			var floNewMoneyValue = parseInt( this.floTotalValue * ( 100 - this.floPercentValue )  ) / 100;
		}

		this.objInputMoney.value = floatToMoney( floNewMoneyValue );

		this.floMoneyValue = floNewMoneyValue;

		this.clearChanges();
		debuggerOutFunction();
	}

	/**
	 * Update Percent Field From the Money Field and Total
	 * @return void
	 * @acess public
	 */
	this.updatePercentFromMoneyField = function updatePercentFromMoneyField()
	{
		debuggerEnterFunction( 'UpdateInterdependentFields.updatePercentFromMoneyField' );
		this.updateFloatValues();

		if ( ! this.boolChangeMoneyValue && ! this.boolChangeTotalValue )
		{
			// not changed //
			this.clearChanges();
			debuggerOutFunction();
			return;
		}

		if ( ! this.boolPercentIsOff )
		{
			// convert 0,12345.. to 12,34% using		//
			// #1 0,12345 turn to 1234,56.. ( * 10000 )	//
			// #2 1234,56 turn to 1234 ( parseInt )		//
			// #3 1234 turn to 12,34 ( / 100 )			//
			var floNewPercentValue = parseInt ( ( this.floMoneyValue / this.floTotalValue ) * 10000 ) / 100;
		}
		else
		{
			var floNewPercentValue = 100 - parseInt ( ( this.floMoneyValue / this.floTotalValue ) * 10000 ) / 100;
		}

		this.floPercentValue = floNewPercentValue;

		this.objInputPercent.value = floatToPercent( floNewPercentValue );


		this.clearChanges();
		debuggerOutFunction();

	}

	this.clearChanges = function clearChanges()
	{
		this.boolChangeMoneyValue	= false;
		this.boolChangePercentValue	= false;
		this.boolChangeTotalValue	= false;
		this.floOldMoneyValue	= parseInt( this.floMoneyValue		* 100) / 100;;
		this.floOldPercentValue	= parseInt( this.floPercentValue	* 100) / 100;
		this.floOldTotalValue	= parseInt( this.floTotalValue		* 100) / 100;
	}
}

/**
 * Abstract Methods
 */

window.UpdateInterdependentFields.instances = Array();
window.UpdateInterdependentFields.arrIds = Array();

window.UpdateInterdependentFields.getSingleton = function getSingleton( strMoneyId  , strPercentId , strTotalId, boolDiscountMode )
{
	debuggerEnterFunction( 'UpdateInterdependentFields.getSingleton' );

	if ( boolDiscountMode == undefined )
	{
		boolDiscountMode = false;
	}
	var strId = strMoneyId + "-" + strPercentId + "-" + strTotalId;

	// o m?todo indexOf n?o funcionou direito no Internet Explorer //

	var intIdPos = array_search( strId , UpdateInterdependentFields.arrIds );

	if ( intIdPos == -1 )
	{
		// not created yey //
		var objUpdater = new UpdateInterdependentFields();

		// setting the input values //
		objUpdater.setId( strId );
		objUpdater.setInputMoney( strMoneyId );
		objUpdater.setInputPercent( strPercentId );
		objUpdater.setInputTotal( strTotalId );
		objUpdater.setDiscountMode( boolDiscountMode );
		// add the id to the array of ids //
		UpdateInterdependentFields.arrIds.push( strId );

		// add the objUpdater to the array of UpdateInterdependentFields //
		UpdateInterdependentFields.instances.push( objUpdater );
		debuggerOutFunction();
		return objUpdater;
	}
	else
	{
		// allready created //
		debuggerOutFunction();
		return UpdateInterdependentFields.instances[ intIdPos ];
	}
}

/**
 * Simple Singleton function to call the method of the UpdateInterdependentFields Object
 * @param string strPercentId
 * @param string strMoneyId
 * @param string strTotalId
 * @return void
 */
window.updateMoneyFromPercentField = function updateMoneyFromPercentField( strPercentId , strMoneyId , strTotalId )
{
	debuggerEnterFunction( 'updateMoneyFromPercentField' );
	var objUpdater = UpdateInterdependentFields.getSingleton( strMoneyId , strPercentId, strTotalId );
	objUpdater.updateMoneyFromPercentField();
	debuggerOutFunction();
}


/**
 * Simple Singleton function to call the method of the UpdateInterdependentFields Object
 * @param string strPercentId
 * @param string strMoneyId
 * @param string strTotalId
 * @return void
 */
window.updatePercentFromMoneyField = function updatePercentFromMoneyField( strMoneyId , strPercentId , strTotalId )
{
	debuggerEnterFunction( 'updatePercentFromMoneyField' );
	var objUpdater = UpdateInterdependentFields.getSingleton( strMoneyId , strPercentId, strTotalId );
	objUpdater.updatePercentFromMoneyField();
	debuggerOutFunction();
}

/**
 * Simple Singleton function to call the method of the UpdateInterdependentFields Object in Discount Mode
 * @param string strPercentDiscountId
 * @param string strMoneyId
 * @param string strTotalId
 * @return void
 */
window.updateMoneyFromDiscountField = function updateMoneyFromDiscountField( strPercentDiscountId , strMoneyId , strTotalId )
{
	debuggerEnterFunction( 'updateMoneyFromDiscountField' );
	var objUpdater = UpdateInterdependentFields.getSingleton( strMoneyId , strPercentDiscountId, strTotalId, true );
	objUpdater.updateMoneyFromPercentField();
	debuggerOutFunction();
}

/**
 * Simple Singleton function to call the method of the UpdateInterdependentFields Object in Discount Mode
 * @param string strPercentDiscountId
 * @param string strMoneyId
 * @param string strTotalId
 * @return void
 */
window.updateDiscountFromMoneyField = function updateDiscountFromMoneyField( strMoneyId , strPercentDiscountId , strTotalId )
{
	debuggerEnterFunction( 'updateDiscountFromMoneyField' );
	var objUpdater = UpdateInterdependentFields.getSingleton( strMoneyId , strPercentDiscountId, strTotalId , true );
	objUpdater.updatePercentFromMoneyField();
	debuggerOutFunction();
}



window.delegateApplyMask = function delegateApplyMask( objField, strType, Event, boolAfterWrite, strDecimalSeparator, strThousandSeparator, maxDecimal , intMaxLength )
{
	var boolMaskAfter = false;
	var boolLeftZeros = true;

	switch( strType )
	{
		case "percent":
		{
			strMask = '###' + strDecimalSeparator + '##$';
			boolMaskAfter = true;
			boolLeftZeros = false;
			break;
		}
		case "hour":
		{
			strMask = '^##:##';
			break;
		}
		case "money":
		{
			strMask = '###' + strThousandSeparator + '###' + strThousandSeparator + '###' + strThousandSeparator + '###' + strThousandSeparator + '###' + strThousandSeparator + '###' + strDecimalSeparator + '##$';
			boolMaskAfter = true;
			boolLeftZeros = false;
			break;
		}
		case "date":
		{
			strMask = '^##/##/####';
			break;
		}
		case "cpf":
		{
			strMask = '^###.###.###-##';
			break;
		}
		case "cnpj":
		{
			strMask = '^##.###.###/####-##';
			break;
		}
		case "cep":
		{
			strMask = '^#####-###';
			break;
		}
		case "integer":
		case "phone":
		case "cpfcnpj":

		{
			strMask = '^##############################';
			break;
		}
		default:
		{
			strMask = '';
			break;
		}
	}

	if (strMask == '')
	{
		return;
	}
	else
	{
		return applyMask( objField, strMask , Event, boolAfterWrite, boolMaskAfter, maxDecimal, boolLeftZeros, intMaxLength );
	}
}

/**
 * Funcao que aplica a mascara ao input.
 * @param objectHTMLInput objField
 * @param string          strMask
 * @param event           evtKeyPress
 * @param bool           boolAfterWrite
 * @param bool           boolMaskAfter
 */
window.applyMask = function applyMask( objField, strMask, Event, boolAfterWrite, boolMaskAfter, intMaxDec, boolLeftZeros, intMaxLength )
{
	var i, intCount, strValue, intFldLen, intMskLen, bolMask, strCod, intTecla, strDirection;
	var intMaskAfter = ( boolMaskAfter ) ? 1 : 0 ;
	if ( Event != undefined )
	{
		var intKeyCode = getIntKeyCode( Event );
		var strKeyType = getKeyType( intKeyCode );
	}
	else
	{
		var strKeyType = "number";
	}

	if( ( strKeyType == "backspace" )
	 || ( strKeyType == "position" )
	 || ( strKeyType == "Fn" )
	 || ( strKeyType == "delete" )
	 || ( strKeyType == "tab" )
	 || ( intKeyCode == 116 ) )
	{
		return true;
	}

	if ( ( boolAfterWrite == false ) && ( Event.ctrlKey ) )
	{
		return true;
	}


	// detects in which direction the mask will be applied //
	if ( strMask.match( /\$$/ ) )
	{
		strDirection = 'rightToLeft';
	}
	else
	{
		strDirection = 'leftToRight';
	}
	var strOriginalValue = objField.value;

	// stripping mask characters from input value //
	var strValue = forceInt( objField.value, boolLeftZeros ) + "";
	// inserindo os zeros necess?rios para se demonstrar as casas decimais //
	if( ( intMaxDec > 0 ) && ( intMaxDec >= strValue.length ) )
	{
	 	var intNumRepeat = ( intMaxDec + 1 ) - strValue.length;
	 	var strZeros = str_repeat( "0" , intNumRepeat );
	 	strValue = strZeros + strValue;
	}

	// stripping beggining and end delimiters from mask //
	strMask = replaceAll( strMask, "^" , "" );
	strMask = replaceAll( strMask, "$" , "" );


	var arrChars = Array();

	for( var i = 0 ; i < strMask.length; ++i )
	{
		arrChars[ i ] = strMask.charAt( i );
	}

	strResultValue = "";

	if ( strDirection == 'leftToRight' )
	{
		var intActualCharValue = 0;
		var strKey;
		for( var i = 0 ; i < arrChars.length; ++i )
		{
			if ( intActualCharValue > strValue.length - intMaskAfter )
			{
				break;
			}
			else
			{
				strKey = strValue.charAt( intActualCharValue );
			}
			if ( arrChars[ i ] == "#" )
			{
				strResultValue += strKey;
				++intActualCharValue;
			}
			else
			{
				strResultValue += arrChars[ i ];
			}
		}
	}
	else
	{
		var intActualCharValue = 0;
		var strKey;
		arrChars = arrChars.reverse();

		var strValueOld = strValue;
		var strValue = "";
		for(var i = strValueOld.length - 1 ; i >= 0; --i )
		{
			strValue += strValueOld.charAt( i );
		}

		for( var i = 0 ; i < arrChars.length; ++i )
		{
			if ( intActualCharValue > strValue.length - intMaskAfter )
			{
				break;
			}
			else
			{
				strKey = strValue.charAt( intActualCharValue );
			}
			debuggerAlert( 'strkey = ' + strKey);
			if ( arrChars[ i ] == "#" )
			{
				strResultValue = strKey + strResultValue;
				++intActualCharValue;
			}
			else
			{
				strResultValue = arrChars[ i ] + strResultValue ;
			}
		}
	}

	// controle de tamanho maximo l?gico //

	if ( strValue.length >= intMaxLength )
	{
		if ( boolAfterWrite == true )
		{
			objField.value = strResultValue;
		}

		if ( Event == undefined )
		{
			return strResultValue;
		}
		return false;
	}

	// caso haja uma altera??o no value //
	// no evento onkeypress, deixa para //
	// o evento onkeyup cuidar			//
	if ( ( boolAfterWrite == false ) && (strOriginalValue != strResultValue ) )
	{
		// nao altera o campo
	}
	else if ( strOriginalValue != strResultValue )
	{
		objField.value = strResultValue;
	}

	if( strKeyType == "number" )
	{
		return true;
	}
	else
	{
		return false;
	}
}

strMasks = 'aa,dd';

i = strMasks.length - 1;
while ( i >= 0 )
{
	//alert ( strMasks.charAt(i) );
	i--;
}

/**
 * Testa se o valor de um input do formulario atende ao regex passado como parametro
 * Caso n?o atenda, o valor do objeto ? setado para "".
 *
 * @param objectHTMLInput objField
 * @param string          strType
 */
window.checkValue = function checkValue( objField, strType )
{
	var mixReturn = objField.onkeyup();

	if ( isString( mixReturn ) )
	{
		objField.value = mixReturn;
	}

	switch( strType )
	{
		case "percent":
		{
			strRegex = /^(?:(?:\d{1,2},)?(\d{1,2})|(100,00))$/;
			break;
		}
		case "hour":
		{
			strRegex = /^\d{2}:\d{2}/;
			break;
		}
		case "date":
		{
			//strRegex = /^(?:(?:[0-2]\d|3[0-1])\/(?:01|03|05|07|08|10|12)|(?:[0-2]\d|30)\/(?:04|06|09|11)|(?:[0-1]\d|2[0-9])\/02)\/\d{4}/;
			strRegex = /^(?:(?:[0-2]\d|3[0-1])\/(?:01|03|05|07|08|10|12)|(?:[0-2]\d|30)\/(?:04|06|09|11)|(?:[0-1]\d|2[0-9])\/02)\/(?:19[7-9]\d|20\d{2})/;
			break;
		}
		case "cpf":
		{
			strRegex = /\d{3}.\d{3}.\d{3}-\d{2}/;
			break;
		}
		case "cnpj":
		{
			strRegex = /\d\d\.\d\d\d\.\d\d\d\/\d\d\d\d\-\d\d/;
			break;
		}
		case "cpfCnpj":
		{
			strRegex = /^\d+$/;
			break;
		}
		case "cep":
		{
			strRegex = /\d\d\d\d\d\-\d\d\d/;
			break;
		}
		case "integer":
		case "phone":
		{
			strRegex = /^\d+$/;
			break;
		}
		default:
		{
			return;
		}
	}

	if ( objField.value.match( strRegex ) )
	{
		return;
	}
	else
	{
		objField.value = "";
	}
}

/**
 * Testa se o valor de um input do tipo money atende ao regex passado como parametro
 * Caso n?o atenda, o valor do objeto ? setado para "".
 *
 * @param objectHTMLInput objField
 * @param string          strRegex
 */
window.checkMoneyValue = function checkMoneyValue( objField, strRegex )
{
	if ( objField.value.match( strRegex ) )
	{
		objField.value = "R$ " + objField.value;
	}
	else
	{
		objField.value = "R$ 0,00";
	}
}

/**
 * Cria m?scara de moeda em campo passado no tipo 000.000.000,00
 *
 * @param objectHTMLInput objCampo
 * @param Event
 * @param int maxlength
 * @return bool/void
 */
window.moneyMask = function moneyMask(objCampo, event, maxlength)
{

	var strValor = objCampo.value;
	strValor = strValor.replace("R$ ","");
	objCampo.value = strValor;

	var intKeyCode = getIntKeyCode( event );
	var strKeyType = getKeyType( intKeyCode );

	if( numberMask( objCampo, event ) && maxLengthMask( objCampo, event, maxlength ) )
	{

		if( strKeyType == "number" ){

			var intTecla = getIntKeyCode(event);

			strValue = objCampo.value;
			strTxt  = "";

			strValue = forceInt( strValue ) + '';

			//deixa com no m?nimo 3 n?meros o algorismo: 0,00
			if( strValue.length <= 2 )
			{
				zeros = 3 - strValue.length;
				for( n = 0; n < zeros; n++ )
				{
					strValue = '0' + strValue;
				}
			}

			//divide string em inteiro e decimo
			pattern = /^(\d*)(\d{1})$/;
			arrSaida = pattern.exec( strValue );
			strInteiro = arrSaida[1];
			strDecimal = arrSaida[2];

			//prepara para colocar pontos
			pattern = /^([^\.]+)(\d{3})/;
			boolTemPonto = true;

			//coloca pontos
			while( boolTemPonto )
			{
				if(	pattern.test( strInteiro ) )
				{
					strInteiro = strInteiro.replace( pattern , '$1.$2' );
					boolTemPonto = true;
				}
				else
				{
					boolTemPonto = false;
				}
			}

			//junta inteiro de decimal com ","
			strSaida = strInteiro + "," + strDecimal;

			//escreve numero formatado
			objCampo.value = strSaida;

		} else {

			return true;
		}


	} else {
		return false;
	}

}

/**
 * Aplica m?scara de percentual
 * Caso o campo seja maior que 100,00 retorna ""
 *
 * @param objectHTMLInput objCampo
 * @param Event
 * @param int maxlength
 * return bool/void
 */
window.percentMask = function percentMask(objCampo, event, maxlength)
{
	integerMask( objCampo, event, 4 );
	var strValor = objCampo.value;

	var intKeyCode = getIntKeyCode( event );
	var strKeyType = getKeyType( intKeyCode );

	if( numberMask( objCampo, event ) && maxLengthMask( objCampo, event, maxlength ) )
	{

		if( strKeyType == "number" )
		{

			//se tiver s? decimais acrescentar um zero na frente
			if( strValor.length < 3 )
			{
				intZero = 3 - strValor.length;
				for( intCount = 0; intCount < intZero; intCount++ )
				{
					strValor = '0' + strValor;
				}
			}

			if(strValor.length < 5)
			{
				strRegex = /^(\d{1,3})(\d{1})$/;
				strValor = strValor.replace(strRegex,'$1,$2');
				strValor = trimString( strValor, '0', true, false);
				if( strValor.charAt(0) == ',')
				{
					strValor = '0' + strValor;
				}

				objCampo.value = strValor;
			}
			else
			{
				return false;
			}
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
$arrHtmlParameter[ "onkeydown" ] 	= ( $arrInternalParameters[ "submit" ] == "false" ) ? "return goToNextHtmlElement( event , this );" : "return submitThisHtmlElement( event , this );";
$arrHtmlParameter[ "onkeyup" ]		= "return delegateApplyMask( this , '" . $strType . "' , event , true 	, '" . $strDec . "' , '" . $strThousand . "' , " . $intMaxDec . " , " . $intMaxLength . " )";
$arrHtmlParameter[ "onkeypress" ] 	= "return delegateApplyMask( this , '" . $strType . "' , event , false 	, '" . $strDec . "' , '" . $strThousand . "' , " . $intMaxDec . " , " . $intMaxLength . " )";
$arrHtmlParameter[ "onfocus" ] 		= "return delegateApplyMask( this , '" . $strType . "' , event , true	, '" . $strDec . "' , '" . $strThousand . "' , " . $intMaxDec . " , " . $intMaxLength . " )";
$arrHtmlParameter[ "onblur" ] 		= "checkValue( this , '" . $arrInternalParameters[ "type" ] . "' ); formatInput(this , '" . $arrInternalParameters[ "type" ] . "');";
*/
