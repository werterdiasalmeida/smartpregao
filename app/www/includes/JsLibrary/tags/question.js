window.QuestionOption = function QuestionOption( strTextOption , funOption )
{
	this.strTextOption = null;
	this.functOption = null;
	
	this.__construct = function __construct( strTextOption, funOption )
	{
		this.strTextOption = strTextOption;
		this.funOption = funOption;
	}
	
	this.__construct( strTextOption, funOption );
}

window.Question = function Question()
{
	this.arrOptions = Array();
	
	this.id = null;
	
	this.strMessage = ' Are You Sure?';
	
	this.strTitle = '';
	
	this.__construct = function __construct()
	{
		if( IE )
		{
			if( isUndefined( window.Question.instances ) )
			{
				window.Question.instances = new Array();
			}
		}
		this.id = window.Question.instances.length;
		window.Question.instances.push( this );
	}
	
	this.appendQuestionOption = function addQuestionOption( objQuestionOption )
	{
		this.arrOptions.push( objQuestionOption );
	}
	
	this.hide = function hide()
	{
		try
		{
			if( document.getElementById( "QuestionContainerId" ) )
			{
				document.getElementById( "QuestionContainerId" ).parentNode.removeChild( document.getElementById( "QuestionContainerId" ) );
			}			
			if( document.getElementById( "ClickableDraw" ) )
			{
				document.getElementById( "ClickableDraw" ).parentNode.removeChild( document.getElementById( "ClickableDraw" ) );
			}
			
			document.body.style.overflow = "inherit";
		}
		catch( e )
		{}
	}
	
	this.show = function show()
	{
		if( document.getElementById( "QuestionContainerId" ) )
		{
			// nao ha ainda pilha de questoes //
			this.hide();
		}
		
		importCssStyle( "tags/question.css" );
		
		document.body.style.overflow = "hidden";
		
		var intBodyScrollLeft		= document.body.scrollLeft;
		var intBodyScrollTop		= document.body.scrollTop;
		var objDivQuestionContainer = document.createElement( "div" );
		var objSpanButtons 			= document.createElement( "span" );
		var objDivQuestion 			= document.createElement( "div" );
		var objDivQuestionPos		= document.createElement( "div" );
		var objDivQuestionDraw		= document.createElement( "div" );
		var objDivQuestionTitle		= document.createElement( "div" );
		var objDivClickableDraw		= document.createElement( "div" );
		var objSpanDescription		= document.createElement( "span" );
		var objCenter				= document.createElement( "center" );
		
		objDivQuestionContainer.id	= "QuestionContainerId";
		objDivClickableDraw.id		= "ClickableDraw";
		
		objDivQuestionContainer.className 	= 'divQuestionContainer';
		objSpanButtons.className 			= 'spanButtons';
		objDivQuestionPos.className 		= 'divQuestionPos';
		objDivQuestion.className 			= 'divQuestion';
		objDivQuestionDraw.className 		= 'divQuestionDraw';
		objSpanDescription.className		= 'spanDescription';
		objDivClickableDraw.className		= 'divUnclickableDraw'
		objDivQuestionTitle.className		= 'divQuestionTitle';
		
		objDivQuestionDraw.appendChild( objDivQuestionTitle );
		objDivQuestionDraw.appendChild( objSpanDescription );
		objDivQuestionDraw.appendChild( objSpanButtons );
		
		objCenter.appendChild( objDivQuestionDraw );
		objDivQuestion.appendChild( objCenter );
		objDivQuestionPos.appendChild( objDivQuestion );
		objDivQuestionContainer.appendChild( objDivQuestionPos );
		
		objDivQuestionTitle.innerHTML = this.strTitle; //'Alo mundo';
		objSpanDescription.innerHTML = this.strMessage + '<br/>';//'Voce quer andar no parque ? ';
		
		for( var i = 0; i < this.arrOptions.length; ++i )
		{
			objQuestionOption = this.arrOptions[ i ];
			
			objInputButton = document.createElement( "input" );
			objInputButton.type = "button";
			objInputButton.className = "inputQuestionButton";
			objInputButton.value = loop_unxmlentities( objQuestionOption.strTextOption );
			addEvent( objInputButton  , 'onclick' , "window.Question.getQuestion("+this.id+").hide();("+objQuestionOption.funOption+")()" );
			
			objSpanButtons.appendChild( objInputButton );
		}
				
		document.body.appendChild( objDivClickableDraw );
		document.body.appendChild( objDivQuestionContainer );
		objDivQuestionDraw.style.width = ( Math.max( objSpanDescription.offsetWidth, objSpanButtons.offsetWidth + this.arrOptions.length * 10 ) ) + 'px';
		objDivQuestionDraw.style.width = ( Math.max( forceInt( objDivQuestionDraw.style.width ) , 300 ) ) + 'px';
		objDivQuestionPos.style.position = "absolute";
		objDivQuestionPos.style.top = intBodyScrollTop + ( document.body.clientHeight / 2 );
		objDivQuestion.style.width = document.body.clientWidth;
		objDivClickableDraw.style.height = Math.max( document.body.offsetHeight , document.body.clientHeight );
		document.body.scrollLeft = intBodyScrollLeft;
		document.body.scrollTop = intBodyScrollTop;
		objInputButton.focus();
	}
	
	this.__construct();
}

window.Question.instances = Array();

window.Question.getQuestion = function getQuestion( intQuestionId )
{
	return window.Question.instances[ parseInt( intQuestionId ) ];
}
window.Question.questionAlert = function questionAlert( strMessage , funAfter_)
{
	if( funAfter_ == undefined )
	{
		funAfter_ = function(){};
	}
	objOk = new QuestionOption( 'Ok', funAfter_ );
	objQuestion = new Question();
	objQuestion.strMessage = strMessage;
	objQuestion.appendQuestionOption( objOk );
	objQuestion.show();
}