jQuery.fn.extend({
	//indica o nome do plugin que será criado
	cokkieasync: function(options){
	
		//seta as variáveis default
		var sDefaults = {
	        boCokkie: false
	    }
	
		//função do jquery que substitui os parametros que não foram informados pelos defaults
		var options = jQuery.extend(sDefaults, options);
	
		if(options.boCokkie){
			var idArvore = this.attr('id');
			
			var span_carregando = this.parent().find("span:first");
			    span_carregando.html("<img border=\"0\" src=\"../imagens/carregando.gif\" align=\"absmiddle\" width=\"15px\" />");
			    
			setTimeout(function(){
				jQuery("#"+idArvore).find("li").each(function(i) {
					var expandable 				 = 'expandable';
					var collapsable  			 = 'collapsable';
					var lastExpandable 			 = 'lastExpandable';
					var lastCollapsable  		 = 'lastCollapsable';
					var expandable_hitarea 		 = 'expandable-hitarea';
					var collapsable_hitarea  	 = 'collapsable-hitarea';
					var lastExpandable_hitarea 	 = 'lastExpandable-hitarea';
					var lastCollapsable_hitarea  = 'lastCollapsable-hitarea';
					
					var ul = jQuery(this).find('ul:first');
					var span = jQuery(this).find('span:first');
	
					var tag_cokkie = '';
	
					var tag_a = span.find('a:first');
					
					if(tag_a.attr('tagName') == 'A'){
						if(tag_a.attr('cokkieGuia')){
							tag_cokkie = tag_a.attr('cokkieGuia');
						}
									
						//jQuery.cookie(tag_cokkie,null);
						if(jQuery.cookie(tag_cokkie)){
							//alert(jQuery.cookie(tag_cokkie))
							if(jQuery.cookie(tag_cokkie) == 'show'){
								if(jQuery(this).attr('class')){
									jQuery(this).replaceClass(expandable, collapsable)
												.replaceClass(lastExpandable,lastCollapsable)
												.replaceClass(expandable_hitarea,collapsable_hitarea)
												.replaceClass(lastExpandable_hitarea,lastCollapsable_hitarea);
									
								}
			
								var div = jQuery(this).find('div:first');
								if(div.attr('class')){
									div.replaceClass(expandable, collapsable)
									   .replaceClass(lastExpandable,lastCollapsable)
									   .replaceClass(expandable_hitarea,collapsable_hitarea)
									   .replaceClass(lastExpandable_hitarea,lastCollapsable_hitarea);
								}
								ul.show('slow');
							} 
						} else {
							if(tag_cokkie){
								var class_this = jQuery(this).attr('class');
								// Se não tem class hasChildren então grava o cokkie, são os que carrega com ajax
								if(!jQuery(this).hasClass("hasChildren") && !jQuery(this).hasClass("hasChildren-hitarea")){									
									if(ul.is(":hidden")){
										jQuery.cookie(tag_cokkie,null);
									} else {
										jQuery.cookie(tag_cokkie,'show', {expires: 7});
									}
								}							
							}
	
						}
				    } // Fim da verificação se tem tag a
		        });
				span_carregando.html("");
			}, 1000 );
		}
	}
});