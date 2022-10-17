		activeMouseGetPos();

		/**
		 * Drag Drop Item
		 */
		 
		window.dragDropItem = function dragDropItem()
		{
			throw new Error( 'dragDropItem is a Abstract class and should not ne instanced' );
		}

		window.dragDropItem.classDragIsPossible		= "opcaoArrastadoPossivel";
		window.dragDropItem.classDragIsImpossible	= "opcaoArrastadoInpossivel";
		window.dragDropItem.classDragIsHappening	= "opcaoArrastadoSelecionado";
		window.dragDropItem.classDropIsPossible		= "opcaoArrastaSoltaPossivel";
		window.dragDropItem.classDropIsImpossible	= "opcaoArrastaSoltaImpossivel";

		window.dragDropItem.getElementToCssStyle = function getElementToCssStyle( objTagElement )
		{
			return objTagElement;
		}
		
		window.dragDropItem.clearCssStyle = function clearCssStyle( objTagElement )
		{
			var objCssElement = window.dragDropItem.getElementToCssStyle( objTagElement );
			var arrCssClasses = Array();
			arrCssClasses.push( window.dragDropItem.classDragIsPossible );
			arrCssClasses.push( window.dragDropItem.classDragIsHappening );
			arrCssClasses.push( window.dragDropItem.classDropIsImpossible );
			removeClassesName( objCssElement , arrCssClasses );		
		}
		
		window.dragDropItem.onmouseover = function onmouseover( objTagElement , strValue_ , strId_ , boolPossible_ )
		{
			if( !window.dragDropColumn.actualDragDropElement )
			{
				window.dragDropColumn.actualDragDropElement = null;
			}
			if( boolPossible_ == undefined )
			{
				boolPossible_ = true;
			}
		
			window.dragDropItem.clearCssStyle( objTagElement );
			var objCssElement = window.dragDropItem.getElementToCssStyle( objTagElement );
			
			if( isObject( window.dragDropColumn.actualDragDropElement ) )
			{
				switch( window.dragDropColumn.actualDragDropElement.strValueFrom )
				{
					case null:
					{
						objTagElement.style.cursor = "move";
						addClassName( objCssElement , window.dragDropItem.classDragIsPossible );
						break;
					}
					case strValue_:
					{
						objTagElement.style.cursor = "no-drop";
						addClassName( objCssElement , window.dragDropItem.classDragIsHappening );
						break;
					}
					default:
					{
						if( boolPossible_ )
						{
							try
							{
								if	(
										( window.dragDropColumn.actualDragDropElement.strValueFrom.length <= strValue_.length ) 
										&&
										( strValue_.indexOf( window.dragDropColumn.actualDragDropElement.strValueFrom ) == 0 )
									)
								{
									boolPossible_ = false;
								}
							}
							catch ( e )
							{
							}
								
						}
						
						if( boolPossible_ )
						{
							
							addClassName( objCssElement , window.dragDropItem.classDropIsPossible );
							objTagElement.style.cursor = "move";
							
//							window.dragDropItem.getElementToCssStyle( objTagElement ).className = 
//								window.dragDropItem.classDropIsPossible;
						}
						else
						{
							addClassName( objCssElement , window.dragDropItem.classDropIsImpossible );
							objTagElement.style.cursor = "no-drop";

//							window.dragDropItem.getElementToCssStyle( objTagElement ).className = 
//								window.dragDropItem.classDropIsImpossible;

						}
						break;
					}
				}
			}
			else
			{
				
				
				objTagElement.style.cursor = "move";		
				
				addClassName( objTagElement , window.dragDropItem.classDragIsPossible );
			
//				window.dragDropItem.getElementToCssStyle( objTagElement ).className = 
//					window.dragDropItem.classDragIsPossible;
			}
		}
		
		window.dragDropItem.onmousedown = function onmousedown( objTagElement , strText , strValue_ , strId_ )
		{
			dragdropElement( objTagElement , strText, strValue_ , strId_ );
		}

		window.dragDropItem.onmouseout = function onmouseout( objTagElement ,  strValue_ , strId_ )
		{ 
			window.dragDropItem.clearCssStyle( objTagElement );
			window.dragDropColumn.unsetValue( strValue_ );
			window.dragDropColumn.unsetId( strId_ );
		}		
		
		/**
		 * Drag Drop Column
		 */
		 
		window.dragDropColumn = function dragDropColumn( objTagElement , objTagElementContainer , strValueFrom , strIdFrom )
		{
			this.objTagElement = null ;
			this.objTagElementContainer = null;
			this.id = null;
			this.cursorRelativeTop = null;
			this.cursorRelativeLeft = null;
			this.strValueFrom = null;
			this.strValueTo = null;
			this.strIdFrom = null;
			this.strIdTo = null;

			this.__construct = function __construct( objTagElement , objTagElementContainer , strValueFrom , strIdFrom )
			{
				window.globalFadeActive = false;
				
				// gerando a instancia //
				this.id = window.dragDropColumn.instances.length;
				window.dragDropColumn.instances[ this.id ] = this;

				// inicializando os atributos do objeto //
				this.objTagElement = objTagElement;
				this.objTagElementContainer = objTagElementContainer;
				this.objTagElement.style.display = 'block';	
				this.strValueFrom = strValueFrom;
				this.strIdFrom = strIdFrom;
				
				// instanciando este objeto como instancia em uso //
				window.dragDropColumn.actualDragDropElement = this;

				// adicionando eventos com este objeto nos elementos de interacao html //
				addEvent( objTagElement , "onmousedown" , "window.dragDropColumn.getElementById(" + this.id + ").onmousedown()" );
				addEvent( objTagElement , "onmouseup"	, "window.dragDropColumn.getElementById(" + this.id + ").onmouseup()" );
				addEvent( objTagElement , "onmouseover" , "window.dragDropColumn.getElementById(" + this.id + ").onmouseover()" );
				addEvent( objTagElementContainer, "onmousemove" , window.dragDropColumn.refresh );
				addEvent( objTagElementContainer, "onmouseup" , window.dragDropColumn.clear );
				addEvent( objTagElementContainer, "onmouseout" , window.dragDropColumn.out );
				
				// posicionando este objeto na posicao inicial correta //
				this.refresh();

			}

			this.onmousedown = function onmousedown()
			{
			}

			this.onmouseover = function onmouseover()
			{
			}

			this.onmouseup = function onmouseup()
			{
			}

			this.refresh = function refresh()
			{
				if( !window.dragDropColumn.actualDragDropElement )	return;
				
				this.objTagElement.style.top = ( document.MouseY.value ) + "px";
				this.objTagElement.style.left = ( document.MouseX.value + 30 ) + "px";
			}

			this.__construct( objTagElement , objTagElementContainer , strValueFrom , strIdFrom );
		}
		
		window.dragDropColumn.classDragIsPossible		= 'podesoltar';
		window.dragDropColumn.classDragIsNotPossible	= 'naopodesoltar';
		window.dragDropColumn.classDragAddChild			= 'linhaAberta';
		window.dragDropColumn.classDragAddBrother		= 'linhaFechada';
		
		window.dragDropColumn.instances = Array();
		window.dragDropColumn.actualDragDropElement = null;
		window.dragDropColumn.getElementById = function getElementById( intId )
		{
			return window.dragDropColumn.instances[ intId ];
		}

		window.dragDropColumn.clearCssStyle = function clearCssStyle( objTagElement )
		{
			var arrCssClasses = Array();
			arrCssClasses.push( window.dragDropColumn.classDragIsPossible );
			arrCssClasses.push( window.dragDropColumn.classDragIsNotPossible );
			removeClassesName( objTagElement , arrCssClasses );		
		}
		
		window.dragDropColumn.out = function out()
		{
//			if( !window.dragDropColumn.actualDragDropElement ) return;
//			window.dragDropColumn.clear();
		}
		
		window.dragDropColumn.clear = function clear()
		{		
			if( !window.dragDropColumn.actualDragDropElement ) return;

			window.dragDropColumn.actualDragDropElement.objTagElement.style.display = 'none';
			window.dragDropColumn.actualDragDropElement = null;
			window.document.body.style.cursor = "default";
			
			window.globalFadeActive = true;
			
		}

		window.dragDropColumn.refresh = function refresh()
		{
			try
			{
				if( !window.dragDropColumn ) return;

				if( !window.dragDropColumn.actualDragDropElement ) return;

				var objActual = window.dragDropColumn.actualDragDropElement;

				if( !objActual.refresh ) return;			
				
				if( !window.dragDropColumn.actualDragDropElement ) return;

				objActual.refresh();
			}
			catch( e )
			{
			}
		}

		window.dragDropColumn.checkFamilyLink = function checkFamilyLink()
		{
			if( !window.dragDropColumn.actualDragDropElement )	return;
			
			try
			{
				if	(
						(
							window.dragDropColumn.actualDragDropElement.strValueFrom.length 
							<= 
							window.dragDropColumn.actualDragDropElement.strValueTo.length 
						) 
						&&
						(
							window.dragDropColumn.actualDragDropElement.strValueTo.indexOf( 
								window.dragDropColumn.actualDragDropElement.strValueFrom 
							) 
							== 
							0 
						)
					)
				{
					boolPossible_ = false;
				}
				else
				{
					boolPossible_ = true;
				}
				return boolPossible_;	
				
			}
			catch ( e )
			{
				return true;
			}
		}
		
		window.dragDropColumn.setValue = function setValue( strToValue )
		{
			if( !window.dragDropColumn.actualDragDropElement )	return;
			window.dragDropColumn.actualDragDropElement.strValueTo = strToValue;
			
			var boolPossible_ = window.dragDropColumn.checkFamilyLink();
			
			if( boolPossible_ )
			{
				window.document.body.style.cursor = "move";
			}
			else
			{
				window.document.body.style.cursor = "no-drop";
			}
			return boolPossible_;
		}
		
		window.dragDropColumn.unsetValue = function unsetValue( strToValue )
		{
			if( !window.dragDropColumn.actualDragDropElement )	return;
			if( window.dragDropColumn.actualDragDropElement.strValueTo == strToValue )
			{
				window.dragDropColumn.actualDragDropElement.strValueTo = null;
			}
			
			window.document.body.style.cursor = "no-drop";
		}
		

		window.dragDropColumn.setId = function setId( strToId )
		{
			if( !window.dragDropColumn.actualDragDropElement )	return;
			window.dragDropColumn.actualDragDropElement.strIdTo = strToId;
		}
		
		window.dragDropColumn.unsetId = function unsetId( strToId )
		{
			if( !window.dragDropColumn.actualDragDropElement )	return;
			if( window.dragDropColumn.actualDragDropElement.strIdTo == strToId )
			{
				window.dragDropColumn.actualDragDropElement.strIdTo = null;
			}
		}
		
		/** 
		 *	DragDropReceiver
		 */
		 
		window.dragDropReceiver = function dragDropReceiver()
		{
			throw new Error( 'dragDropReceiver is a Abstract class and should not ne instanced' );
		}

		window.dragDropReceiver.objActualTagElement = null;
		
		window.dragDropReceiver.onmouseup = function onmouseup( objTagReceiver )
		{
			if( !window.dragDropColumn.actualDragDropElement ) return;
			
			var boolTranferAsChild = haveClassName( objTagReceiver , window.dragDropReceiver.classDragAddChild );
			
//			document.title = boolTranferAsChild;
			
			window.dragDropColumn.clearCssStyle( objTagReceiver);
						
			var boolPossible = window.dragDropColumn.checkFamilyLink();
			
			if( boolPossible )
			{
				mudar_posicao_atividade(
					window.dragDropColumn.actualDragDropElement.strValueFrom,
					window.dragDropColumn.actualDragDropElement.strIdFrom,
					window.dragDropColumn.actualDragDropElement.strValueTo,
					window.dragDropColumn.actualDragDropElement.strIdTo,
					boolTranferAsChild 
				);
			}
			else
			{
				// alert( 'Você não pode mover um elemento para um de seus descendentes.' );
			}

//			alert( 
//			    ' => ' +
//				window.dragDropColumn.actualDragDropElement.strValueFrom + 
//				' , ' +
//				window.dragDropColumn.actualDragDropElement.strIdFrom + 
//				' eeee  ' + 
//				window.dragDropColumn.actualDragDropElement.strValueTo +
//				' , ' +
//				window.dragDropColumn.actualDragDropElement.strIdTo +
//				' <= '
//			);

		}
		
		window.dragDropReceiver.onmouseover = function onmouseover( objTagReceiver , strValue_ , strId_ )
		{
			
			if( strValue_ == undefined )
			{
				strValue_ = objTagReceiver.id;
			}

			window.dragDropReceiver.objActualTagElement = objTagReceiver;
			var boolPossible = window.dragDropColumn.setValue( strValue_ );
			window.dragDropColumn.setId( strId_ );
			
			if( !window.dragDropColumn.actualDragDropElement )	return;

			window.dragDropColumn.clearCssStyle( objTagReceiver );
			if( boolPossible )
			{
				addClassName( objTagReceiver , window.dragDropColumn.classDragIsPossible );
			}
		}
		
		window.dragDropReceiver.onmouseout = function onmouseout( objTagReceiver , strValue_ , strId_ )
		{
			
			if( strValue_ == undefined )
			{
				strValue_ = objTagReceiver.id;
			}
			
			if(	window.dragDropReceiver.objActualTagElement == objTagReceiver )
			{
				window.dragDropColumn.unsetValue( strValue_ );
				window.dragDropColumn.unsetId( strId_ );
			}
			
			if( !window.dragDropColumn.actualDragDropElement )	return;
			
			window.dragDropColumn.clearCssStyle( objTagReceiver );
			addClassName( objTagReceiver , window.dragDropColumn.classDragIsNotPossible );
		}
		
		function dragdropElement( objTagElement , strDragText_ , strValueFrom_ , strIdFrom_ , objTagElementContainer_	 )
		{
			
			objTagElement.innerHTML += '';
			objTagElement.unselectable = "on";			
			
			if( objTagElementContainer_ == undefined )
			{
				objElementoContainer_ = document.body;
			}

			var strDragId = 'dragDropColumn';
			var objElemento = document.getElementById( strDragId );
				
			if( strDragText_ == undefined )
			{
				strDragText_ = IE ? objTagElement.parentNode.innerText : objTagElement.parentNode.textContent ;
			} 
			if( strValueFrom_ == undefined )
			{
				strValueFrom_ = objTagElement.id;
			} 
			objElemento.innerHTML = strDragText_;
			var objDragDrop = new dragDropColumn( objElemento , objElementoContainer_  , strValueFrom_ , strIdFrom_ );
		}

	addEvent( document.body  , 'onmouseup' , window.dragDropColumn.clear );

