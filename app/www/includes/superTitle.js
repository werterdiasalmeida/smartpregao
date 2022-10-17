	/**
	 *
	 @date 2006-01-17
	 */
	FADE_IN_FINISHED = "fade in finished";
	 
	/**
	 *
	 @date 2006-01-17
	 */
	FADE_OUT_FINISHED = "fade out finished";
	
	/**
	 *
	 @date 2006-01-17
	 */
	mouseX = 0;	

	/**
	 *
	 @date 2006-01-17
	 */
	mouseY = 0;	

	/**
	 *
	 @date 2006-01-17
	 */
	globalBoolOnFade = false;
	
	/**
	 *
	 @date 2006-01-17
	 */
	globalBoolUseFade = false;
	
	/**
	 *
	 @date 2006-01-17
	 */
	globalIntFadeDirection = 0;
	
	/**
	 *
	 @date 2006-01-17
	 */
	globalIntFadeSpeed		= 1;
	
	/**
	 *
	 @date 2006-01-17
	 */
	globalIntFadeIESpeed	= 100;
	
	/**
	 *
	 @date 2006-01-17
	 */
	globalIntFadeValue		= 15;
	
	/**
	 *
	 @date 2006-01-17
	 */
	globalIntFadeIEValue		= 15;
	
	/**
	 *
	 @date 2006-01-17
	 */
	globalIntFadeNotIEValue		= 5;
	
	/**
	 *
	 @date 2006-01-17
	 */
	globalIntFadeCount		= 0;
	
	/**
	 *
	 @date 2006-01-17
	 */
	globalIntFadeMax		= 100;
	
	/**
	 *
	 @date 2006-01-17
	 */
	globalIntFadeMin		= 0;
	
	/**
	 *
	 @date 2006-01-17
	 */
	globalStrIdTitleBox = "TitleBoxId";
	
	/**
	 *
	 @date 2006-01-17
	 */
	globalObjSelected	= null;
	
	/**
	 *
	 @date 2006-01-19
	 */
	globalIntMouseXAdd	= 10;
	
	/**
	 *
	 @date 2006-01-19
	 */
	globalIntMouseYAdd	= 10;
	
	globalIntFinalMouseX = null;
	
	globalIntFinalMouseY = null;
	
	globalIntDivWidth = 150;
	
	/**
	 *
	 @date 2006-01-17
	 */
	window.singletonFade = function singletonFade()
	{
		if ( globalBoolOnFade == true )
		{
			return;
		}
		globalBoolOnFade = true;
		loopFade();
	}
	 
	/**
	 *
	 @date 2006-01-17
	 */
	window.loopFade = function loopFade()
	{
		
		var objDivTitleBox = document.getElementById( globalStrIdTitleBox );
	
		globalIntFadeCount += globalIntFadeValue*globalIntFadeDirection ; 
		
		if ( globalIntFadeCount > globalIntFadeMax )
		{
			objDivTitleBox.style.visibility = 'visible';
			globalIntFadeCount = globalIntFadeMax;
			globalBoolOnFade = false;
			setTimeout( 'SuperTitleOff()' , 100 );
			return FADE_IN_FINISHED ;
		}
		if ( globalIntFadeCount < globalIntFadeMin )
		{
			globalIntFadeCount = globalIntFadeMin;
			globalBoolOnFade = false;
			RemoveSuperTitle();
			return FADE_OUT_FINISHED;
		}
	
		if( globalBoolUseFade )
		{
			Alpha( objDivTitleBox , globalIntFadeCount );
		}
		else
		{
			objDivTitleBox.style.visibility = 'hidden';
		}
		
		if ( !IE )
		{
			setTimeout( "loopFade()" , globalIntFadeSpeed );
		}
		else
		{
			setTimeout( "loopFade()" , globalIntFadeIESpeed );
		}
		
		return false;
	}
	
	/**
	 *
	 @date 2006-01-17
	 */	
	window.FadeIn = function FadeIn()
	{
		globalIntFadeDirection = 1;
		
		if ( !IE )
		{
			globalIntFadeValue = globalIntFadeNotIEValue;
		}
		else
		{
			globalIntFadeValue = globalIntFadeIEValue;
		}
		singletonFade();
	}
	
	/**
	 *
	 @date 2006-01-17
	 */	
	window.FadeOut = function FadeOut()
	{
		globalIntFadeDirection = -1;
		if ( !IE )
		{
			globalIntFadeValue = globalIntFadeNotIEValue;
		}
		else
		{
			globalIntFadeValue = globalIntFadeIEValue;
		}
		singletonFade();
	}
	
	/**
	 *
	 @date 2006-01-17
	 */
	window.SuperTitleOff = function SuperTitleOff(me)
	{
		if( Math.abs(  mouseX - globalIntFinalMouseX ) < 10 &&  Math.abs(  mouseY - globalIntFinalMouseY ) < 10  )
		{
			setTimeout( 'SuperTitleOff()' , 100 );
			return;
		}
		var objDivTitleBox = document.getElementById( globalStrIdTitleBox );
		
		if (objDivTitleBox == false)
		{
			FadeOut();
			// there is not a super title to be removed //
			return false;
		}
			
		FadeOut();
	}
	
	/**
	 *
	 @date 2006-01-17
	 */
	window.RemoveSuperTitle = function RemoveSuperTitle()
	{
	    
		var objDivTitleBox = document.getElementById( globalStrIdTitleBox );
		
		while ( objDivTitleBox )
		{
		    objParent = objDivTitleBox.parentNode;
			objDivTitleBox.style.display = 'none';
			objParent.removeChild( objDivTitleBox );
			objDivTitleBox = document.getElementById( globalStrIdTitleBox );
		}
	}
	
	window.CreateSuperTitle = function CreateSuperTitle( strValue )
	{
		if ( strValue == "" )
		{	
			RemoveSuperTitle();
			return;
		}
		
		var objChildBox						=	document.createElement( 'div' );
		objChildBox.className 				=	"TitleClass";
		objChildBox.innerHTML 				=	strValue;

	    objDivTitleBox 						=	document.createElement('div');
		objDivTitleBox.appendChild( objChildBox );
		
		objDivTitleBox.id					=	globalStrIdTitleBox;
		objDivTitleBox.style.position		=	"absolute";
		objDivTitleBox.style.top			=	( mouseY + globalIntMouseYAdd ) + "px";
		objDivTitleBox.style.left			=	( mouseX + globalIntMouseXAdd ) + "px";
		objDivTitleBox.style.zIndex			=	"200";
		objDivTitleBox.onmouseout			=	SuperTitleOff;
		
		document.body.appendChild( objDivTitleBox );
		window.RefreshSuperTitlePos( objDivTitleBox );
		return objDivTitleBox;
	}

	window.RefreshSuperTitlePos = function RefreshSuperTitlePos( objDivTitleBox )
	{
		objDivTitleBox.style.top			=	( mouseY + globalIntMouseYAdd ) + "px";
		objDivTitleBox.style.left			=	( mouseX + globalIntMouseXAdd ) + "px";
		
		var intDivWidth 	= 0 + forceInt( objDivTitleBox.offsetWidth );
		
		if( intDivWidth < globalIntDivWidth )
		{
			if( !IE )
			{
				objDivTitleBox.style.width = globalIntDivWidth + 'px';
			}
			else
			{
				objDivTitleBox.style.width = ( 25 + globalIntDivWidth )+ 'px';
			}
		}
		
		
		var boolOk = false;
		var boolChanged = false;
		var intCountControl = 0;
		
		while( boolOk == false )
		{
			++intCountControl;
			
			var intDivWidth 	= 0 + forceInt( objDivTitleBox.offsetWidth );
			
			if( intDivWidth < globalIntDivWidth )
			{
				if( !IE )
				{
					objDivTitleBox.style.width = globalIntDivWidth + 'px';
				}
				else
				{
					//objDivTitleBox.style.width = ( 50 + globalIntDivWidth )+ 'px';
				}
			}
			
			var intDivLeft		= 0 + forceInt( objDivTitleBox.style.left );
			var intEndLeft		= 0 + intDivWidth + intDivLeft;
			var intBodyLeft 	= 0 + window.document.body.offsetWidth - 10;
	
			var intDivHeight	= 0 + forceInt( objDivTitleBox.offsetHeight );
			var intDivTop		= 0 + forceInt( objDivTitleBox.style.top );
			var intEndTop		= 0 + intDivHeight + intDivTop;
			var intBodyTop		= 0 + window.document.body.offsetHeight;
	
			if( IE )
			{
				intBodyLeft	= 0 + window.document.body.offsetWidth - 42;
				intBodyTop	= 0 + window.document.body.offsetHeight + 110;
			}
			
			
			boolOk = true;
			if ( intEndLeft > intBodyLeft )
			{
				boolChanged = true;
				boolOk = false;
//				var intNewLeft = 0 + intBodyLeft - ( intDivWidth ) - intCountControl;
				var intNewLeft = mouseX - ( intDivWidth ) - intCountControl - globalIntMouseXAdd ;
				objDivTitleBox.style.left =  intNewLeft + 'px';
			}
			else
			{
				var intNewLeft = 0;
			}
			
			if ( intEndTop > intBodyTop )
			{
				boolChanged = true;
				boolOk = false;
				if( !IE )
				{
//					var intNewTop = 0 + intBodyTop - ( intDivHeight ) - intCountControl;
					var intNewTop = mouseY  - ( intDivHeight ) - intCountControl - globalIntMouseYAdd;
				}
				else
				{
					var intNewTop = mouseY  - ( intDivHeight ) - intCountControl
				}
				objDivTitleBox.style.top =  intNewTop + 'px';
			}
			else
			{
				var intNewTop = 0;
			}
			
			if( intCountControl > 16 )
			{
				if ( IE )
				{
					break;
				}
			}		
		}		
		if( boolChanged )
		{
			globalIntFinalMouseX = mouseX;
			globalIntFinalMouseY = mouseY;
		}
		else
		{
			globalIntFinalMouseX = mouseX;
			globalIntFinalMouseY = mouseY;
		}
	}
	/**
	 *
	 @date 2006-01-17
	 */
	window.SuperTitleOn = function SuperTitleOn( objSelected , strValue )
	{
		objDivTitleBox = document.getElementById( globalStrIdTitleBox );
	
		/// caso o div do title ja exista //
		if ( objDivTitleBox && ( strValue != "" ) ) 
	    {
	    	// caso o objeto pai seja diferente do objeto pai atual //
	    	if ( objSelected != globalObjSelected )
			{
				if ( globalObjSelected )
				{
					if (objSelected.onmouseover != globalObjSelected.onmouseover )
					
					var arrChilds = objSelected.getElementsByTagName( "*" );
					var intPos	= array_search( globalObjSelected, arrChilds );
					if ( intPos != -1 )
					{
						// o novo elemento de title ? pai do elemento de title anterior //
						strValue = objDivTitleBox.getElementsByTagName( "div" )[0].innerHTML;
					}
				}
				globalObjSelected = objSelected;
				
				objDivTitleBox.style.top 		= ( mouseY + globalIntMouseYAdd ) + "px";
				objDivTitleBox.style.left 		= ( mouseX + globalIntMouseXAdd ) + "px";
				
				objDivTitleBox.style.display 	= "block";
				objDivTitleBox.getElementsByTagName("div")[0].innerHTML = strValue;
				window.RefreshSuperTitlePos( objDivTitleBox );
			}
			// caso o objeto pai seja o mesmo do pai atual
			else
			{
				objDivTitleBox.getElementsByTagName("div")[0].innerHTML = strValue;
				window.RefreshSuperTitlePos( objDivTitleBox );
			}
		}
		else
		{
			CreateSuperTitle( strValue );
		}
		FadeIn();
		return false;
	}

	
	/**
	 *
	 @date 2006-01-17
	 */	
	window.ShowUp = function ShowUp(Percents,Passo)
	    {
		if (!(self.TitleBox))
			return false;
		Percents += Passo;
		if (Percents > 100)
		    {
	        Making = false;
		    return false;
		    }
		if (Percents < 0)
		    {
	        Alpha(TitleBox,0);
	        Making = false;
		    return false;
		    }
		Alpha(TitleBox,Percents);
		setTimeout( "ShowUp( " + Percents + "," + Passo + " ) " , 40 );
	    }

	/**
	 * @see superTitle.css
	 * @date 13-12-2006
	 */
	window.initializeSuperTitle = function initializeSuperTitle()
	{
		activeMouseGetPos();
	}
	
	var SuperTitleAjaxCache = new Array();
	
	/**
	 *
	 @date 2006-12-15
	 */	
	window.SuperTitleAjax = function SuperTitleAjax( strUrl, objSelected )
	{
		if ( SuperTitleAjaxCache[strUrl] )
		{
			SuperTitleOn( objSelected, SuperTitleAjaxCache[strUrl] );
			return;
		}
		SuperTitleOn( objSelected, '<div style="background-color: white; color: black; width: ' + globalIntDivWidth + 'px; height: 100%"> Carregando.. <img src="/imagens/wait.gif"/></div>' );
		var objRequest = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
		objRequest.onreadystatechange = function()
		{
			if ( objRequest.readyState == 4 )
			{
				if ( objRequest.status == 200 )
				{
					SuperTitleAjaxCache[strUrl] = objRequest.responseText;
					SuperTitleOn( objSelected, SuperTitleAjaxCache[strUrl] );
				}
			}
		}
		objRequest.open( 'GET', strUrl, true );
		objRequest.send( null );
	}
	
initializeSuperTitle();