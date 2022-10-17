
/**************************************************************

	Script		: Overlay
	Version		: 1.1
	Authors		: Samuel birch
	Desc		: Covers the window with a semi-transparent layer.
	Licence		: Open Source MIT Licence

**************************************************************/

var Overlay = new Class({
	
	getOptions: function(){
		return {
			colour: '#000',
			opacity: 0.7,
			zIndex: 1,
			container: document.body,
			onClick: Class.empty
		};
	},

	initialize: function(options){
		this.setOptions(this.getOptions(), options);
		
		this.options.container = $(this.options.container);
		
		this.overlay = new Element('div').setProperty('id', 'Overlay').setStyles({
			position: 'absolute',
			left: '0px',
			top: '0px',
			width: '100%',
			zIndex: this.options.zIndex,
			backgroundColor: this.options.colour
		}).injectInside(this.options.container);
		
		this.overlay.addEvent('click', function(){
			this.options.onClick();
		}.bind(this));
		
		this.fade = new Fx.Style(this.overlay, 'opacity').set(0);
		this.position();
		
		window.addEvent('resize', this.position.bind(this));
	},
	
	position: function(){ 
		if(this.options.container == document.body){ 
			var h = window.getScrollHeight()+'px'; 
			this.overlay.setStyles({top: '0px', height: h}); 
		}else{ 
			var myCoords = this.options.container.getCoordinates(); 
			this.overlay.setStyles({
				top: myCoords.top+'px', 
				height: myCoords.height+'px', 
				left: myCoords.left+'px', 
				width: myCoords.width+'px'
			}); 
		} 
	},
	
	show: function(){
		this.fade.start(0,this.options.opacity);
	},
	
	hide: function(){
		this.fade.start(this.options.opacity,0);
	}
	
});
Overlay.implement(new Options);

/*************************************************************/
