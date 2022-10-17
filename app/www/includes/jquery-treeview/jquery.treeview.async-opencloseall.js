jQuery(document).ready(function(){
	
	var expandable 				 = 'expandable';
	var collapsable  			 = 'collapsable';
	var lastExpandable 			 = 'lastExpandable';
	var lastCollapsable  		 = 'lastCollapsable';
	var expandable_hitarea 		 = 'expandable-hitarea';
	var collapsable_hitarea  	 = 'collapsable-hitarea';
	var lastExpandable_hitarea 	 = 'lastExpandable-hitarea';
	var lastCollapsable_hitarea  = 'lastCollapsable-hitarea';
	
	jQuery('.abrirTodos').click(function(){
		var idArvore = jQuery(this).parent().parent().find('~ div').find('ul:first').attr('id')
		jQuery("#"+idArvore).find("li").each(function() {
			var class_this = $(this).attr('class');
			// Se não tem class hasChildren então grava o cokkie, são os que carrega com ajax
			if(class_this.search('hasChildren') == '-1'){
				var div = jQuery(this).find('div:first');
				var span = $(this).find('span:first');
				var tag_cokkie = '';
				var tag_a = span.find('a:first');
				if(tag_a.attr('tagName') == 'A' && tag_a.attr('cokkieGuia')){
					tag_cokkie = tag_a.attr('cokkieGuia');
					$.cookie(tag_cokkie,'show', {expires: 1});						
				}
				
				if(jQuery(this).attr('class')){
					jQuery(this).replaceClass(expandable, collapsable)
								.replaceClass(lastExpandable,lastCollapsable)
								.replaceClass(expandable_hitarea,collapsable_hitarea)
								.replaceClass(lastExpandable_hitarea,lastCollapsable_hitarea);
				}
		
				if(div.attr('class')){
					div.replaceClass(expandable, collapsable)
					   .replaceClass(lastExpandable,lastCollapsable)
					   .replaceClass(expandable_hitarea,collapsable_hitarea)
					   .replaceClass(lastExpandable_hitarea,lastCollapsable_hitarea);				
				}
				
				var ul = jQuery(this).find('ul:first');
				if(ul.is(":hidden")){
					ul.show('slow');
				}
			}
	    });
	    return false;
	});
	
	jQuery('.fecharTodos').click(function(){
		var idArvore = jQuery(this).parent().parent().find('~ div').find('ul:first').attr('id')
		jQuery("#"+idArvore).find("li").each(function() {
			var class_this = $(this).attr('class');
			// Se não tem class hasChildren então grava o cokkie, são os que carrega com ajax
			if(class_this.search('hasChildren') == '-1'){
				var div = jQuery(this).find('div:first');
				var span = $(this).find('span:first');
				var tag_cokkie = '';
				var tag_a = span.find('a:first');
				if(tag_a.attr('tagName') == 'A' && tag_a.attr('cokkieGuia')){
					tag_cokkie = tag_a.attr('cokkieGuia');
					//$.cookie(tag_cokkie,'hide', {expires: 1});						
					$.cookie(tag_cokkie,null);
				}
				
				if(jQuery(this).attr('class')){
					jQuery(this).replaceClass(collapsable,expandable)
								.replaceClass(lastCollapsable,lastExpandable)
								.replaceClass(collapsable_hitarea,expandable_hitarea)
								.replaceClass(lastCollapsable_hitarea,lastExpandable_hitarea);
				}
		
				if(div.attr('class')){
					div.replaceClass(collapsable,expandable)
					   .replaceClass(lastCollapsable,lastExpandable)
					   .replaceClass(collapsable_hitarea,expandable_hitarea)
					   .replaceClass(lastCollapsable_hitarea,lastExpandable_hitarea);				
				}
				
				var ul = jQuery(this).find('ul:first');
				if(!ul.is(":hidden")){
					ul.hide('slow');
				}
			}
	    });
	    return false;
	});

});