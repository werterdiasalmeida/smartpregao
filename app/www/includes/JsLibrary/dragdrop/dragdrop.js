		window.dragDrop = function dragDrop( objTagElement , objTagElementContainer )
		{
			this.objTagElement = null ;
			this.objTagElementContainer = null;
			this.id = null;
			this.cursorRelativeTop = null;
			this.cursorRelativeLeft = null;

			this.__construct = function __construct( objTagElement , objTagElementContainer )
			{
				activeMouseGetPos();
				
				requireEvent( document.body , "onmouseup"  , window.dragDrop.out );
				this.id = window.dragDrop.instances.length;
				window.dragDrop.instances[ this.id ] = this;

				this.objTagElement = objTagElement;
				this.objTagElementContainer = objTagElementContainer;

				addEvent( objTagElement , "onmousedown" , "window.dragDrop.getElementById(" + this.id + ").onmousedown()" );
				addEvent( objTagElement , "onmouseup"	, "window.dragDrop.getElementById(" + this.id + ").onmouseup()" );
				addEvent( objTagElement , "onmouseover" , "window.dragDrop.getElementById(" + this.id + ").onmouseover()" );
				addEvent( objTagElementContainer, "onmousemove" , window.dragDrop.refresh );
				addEvent( objTagElementContainer, "onmouseup" , window.dragDrop.clear );
				addEvent( objTagElementContainer, "onmouseout" , window.dragDrop.out );
			}

			this.onmousedown = function onmousedown()
			{
				if( isNull( window.dragDrop.actualDragDropElement ) )
				{
					window.dragDrop.actualDragDropElement = this;
				}
				document.title = 'onmousedown';
			}

			this.onmouseover = function onmouseover()
			{
				document.title = 'onmouseover';
			}

			this.onmouseup = function onmouseup()
			{
				if( window.dragDrop.actualDragDropElement == this )
				{
					window.dragDrop.clear();
				}
				document.title = 'onmouseup';
			}

			this.refresh = function refresh()
			{
				this.objTagElement.style.position = "relative";
				this.objTagElement.style.top = ( document.MouseY.value - this.objTagElementContainer.offsetLeft - ( this.objTagElement.scrollWidth / 2 ) ) + "px";
				this.objTagElement.style.left = ( document.MouseX.value - this.objTagElementContainer.offsetTop - ( this.objTagElement.scrollHeight / 2 ) ) + "px";
			}

			this.toString = function toString()
			{
				return ':D';
			}
			this.__construct( objTagElement , objTagElementContainer );
		}
		window.dragDrop.instances = Array();
		window.dragDrop.actualDragDropElement = null;
		window.dragDrop.getElementById = function getElementById( intId )
		{
			return window.dragDrop.instances[ intId ];
		}

		window.dragDrop.out = function out()
		{
			document.title = 'out';
			//window.dragDrop.actualDragDropElement = null;
		}
		
		window.dragDrop.clear = function clear()
		{
			document.title = 'clear';
			window.dragDrop.actualDragDropElement = null;
		}

		window.dragDrop.refresh = function refresh()
		{
			if( !isNull( window.dragDrop.actualDragDropElement ) )
			{
				document.title = window.dragDrop.actualDragDropElement;
				window.dragDrop.actualDragDropElement.refresh();
			}
		}