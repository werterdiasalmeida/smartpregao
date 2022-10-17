

/**************************************************************

	Script		: History
	Version		: 1.0
	Authors		: Samuel Birch
	Desc		: Enables browser back/forward buttons when using Javascript or AJAX.
	Licence		: Open Source MIT Licence

**************************************************************/

var History = new Class({
							  
	getOptions: function(){
		return {
			links: '.loadMe'
		};
	},

	initialize: function(options){
	
		this.setOptions(this.getOptions(), options);
		
		this.url = window.location.href.toString();
		this.checkURL.periodical(500,this);
		
		if(window.ie){
			this.iframe = new Element('iframe').setProperties({id: 'HistoryIframe', src: this.url}).setStyles({display: 'none'}).injectInside(document.body);
		}
		
		var url = this.formatHash(this.url);
		
		this.links = $$(this.options.links);
		this.links.each(function(el,i){			 
			el.addEvent('click',function(e){
				if(e != undefined){
					new Event(e).stop();
				}
				this.set(el);
			}.bind(this));
			
			if(url != ''){
				if(el.href == url){
					el.fireEvent('click','',50);
					this.set(el.href);
				}
			}
		}.bind(this));
		
	},
	
	formatHash: function(str){
		str = str.toString();
		var index = str.indexOf('#');
		if(index > -1){
			str = str.substr(index+1);
		}
		return str;		
	},
	
	formatURL: function(str){
		str = str.toString();
		var index = str.indexOf('#');
		if(index > -1){
			str = str.substring(0, index);
		}
		return str;
	},
	
	set: function(str){
		var url = this.formatURL(this.url);
		str = this.formatHash(str);
		this.url = url+'#'+str;
		window.location.href = this.url;
		if(window.ie){
			this.iframe.setProperty('src', str);
		}
	},
	
	checkURL: function(){
		
		if(window.ie){
			var url = this.iframe.contentWindow.location.href;
			if(url != this.formatHash(this.url)){
				this.url = this.formatURL(this.url)+'#'+url;
				window.location.href = this.url;
				this.iframe.setProperty('src', url);
				this.setContent();
			}
		}else{
			var url = window.location.href.toString();
			if(url != this.url){
				this.url = url;
				window.location.href = this.url;
				this.setContent();
			}
		}
	},
	
	setContent: function(){
		var url = this.formatHash(this.url)
		this.links.each(function(el,i){	
			if(el.href == url){
				el.fireEvent('click','',50);
			}
		});
	}

});

History.implement(new Events);
History.implement(new Options);


/*************************************************************/
