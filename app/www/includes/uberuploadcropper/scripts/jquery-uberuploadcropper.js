/*
 * jQuery UberUploadCropper
 * By: Trent Richardson [http://trentrichardson.com]
 * Version 0.2
 * Last Modified: 4/10/2010
 * 
 * Copyright 2009 Trent Richardson
 * Dual licensed under the MIT and GPL licenses.
 * http://trentrichardson.com/Impromptu/GPL-LICENSE.txt
 * http://trentrichardson.com/Impromptu/MIT-LICENSE.txt
 * 
 */
(function($){

	$.fn.uberuploadcropper = function(options){
		options = $.extend({},{ multi:false, auto: true, buttonText:'BROWSE', onComplete:function(){} },options);
		
		var onComplete = options.onComplete;
		
		var imgdata = [];
		
		var cropPromptSubmit = function(v,m,f){
		
			if(v == 'Previous'){
				$.prompt.getCurrentState().find('.ubercropimage').Jcrop('disable');
				$.prompt.prevState();
				$.prompt.getCurrentState().find('.ubercropimage').Jcrop('enable');
				return false;
			}
			if(v == 'Next'){
				$.prompt.getCurrentState().find('.ubercropimage').Jcrop('disable');
				$.prompt.nextState();
				$.prompt.getCurrentState().find('.ubercropimage').Jcrop('enable');
				return false;
			}
			if(v == 'Cancel'){
				$.prompt.close();
				imgdata = [];
				return false;
			}
			if(v == 'Done'){ //process crop..
				$.prompt.goToState('waitState');
				
				$.post(options.cropScript,f,function(data){
					$.prompt.close();
					/*
					var str = "";
					$.each(f,function(i,obj){
						str += i +" : "+ obj +"\n";
					});
					alert(str);
					*/
					
					//call user callback func..
					onComplete(imgdata,data);					
					
					imgdata = [];
				});
				
				return false;
			}

	
		};
		
		var cropPromptCallback = function(v,m,f){
			imgdata = [];
		};
		
		//keep our coords up to date
		var jcropOnChange = function(c){
			$currstate = $.prompt.getCurrentState();
			
			$currstate.find('.imgcrop_x').val(c.x);
			$currstate.find('.imgcrop_y').val(c.y);
			$currstate.find('.imgcrop_w').val(c.w);
			$currstate.find('.imgcrop_h').val(c.h);
		}
		
		// add the uploaded image to our queue
		var singleUploadComplete = function(event,queueId,fileObj,response,data){
		
			imgdata.push(fileObj);
		};
		
		// upload is done.. crop these puppies!
		var allUploadsComplete = function(){
			var states = {};
						
			$.each(imgdata,function(i,obj){
				var btn = {};
				var str = '<div style="text-align: center;"><img src="'+ options.folder +'/'+ obj.name +'?d='+ (new Date()).getTime() +'" class="ubercropimage" id="ubercropimage_'+ i +'" style="margin: 0 auto;" /></div>'+
					'<input type="hidden" name="imgcrop['+ i +'][name]" id="imgcrop_'+ i +'_name" value="'+obj.name+'" class="imgcrop_name" />'+
					'<input type="hidden" name="imgcrop['+ i +'][x]" id="imgcrop_'+ i +'_x" value="0" class="imgcrop_x" />'+
					'<input type="hidden" name="imgcrop['+ i +'][y]" id="imgcrop_'+ i +'_y" value="0" class="imgcrop_y" />'+
					'<input type="hidden" name="imgcrop['+ i +'][w]" id="imgcrop_'+ i +'_w" value="'+ options.width +'" class="imgcrop_w" />'+
					'<input type="hidden" name="imgcrop['+ i +'][h]" id="imgcrop_'+ i +'_h" value="'+ options.height +'" class="imgcrop_h" />'+
					'<input type="hidden" name="imgcrop['+ i +'][folder]" id="imgcrop_'+ i +'_folder" value="'+ options.folder +'" class="imgcrop_folder" />';
				
				if(imgdata.length == 1)
					btn = { Done:'Done' };
				else if(i == imgdata.length-1)
					btn = { Previous:'Previous', Done:'Done' };
				else if(i > 0)
					btn = { Previous:'Previous', Next:'Next' };
				else btn = { Next:'Next' };
				
				
				states['state'+ i] = {
					html: str,
					buttons: btn,
					submit: cropPromptSubmit
				};
				
			});
			
			states['waitState'] = { html: 'Processing...', buttons:{} };
			
			var imp = $.prompt(states,{ callback: cropPromptCallback });
			
			var biggestwidth = 0;
			
			//we obviously need to destroy and create each time we enter and leave a state..
			//$('.ubercropimage').imgAreaSelect({ keys: { arrows: 15, ctrl: 5, shift: 'resize' } });
			$('.ubercropimage').each(function(i){ 
				$(this).Jcrop(options);
				if(i > 0)
					$(this).Jcrop('disable');
					
				// we might get a huge image, so resize the prompt..
				$(this).load(function(){
					if($(this).width() > biggestwidth){
						biggestwidth = $(this).width() + 40;
						imp.find('.jqi').width(biggestwidth+40).css('marginLeft',((imp.find('.jqi').outerWidth()/2)*-1));
					}
				});
			});
			//imgdata = [];
		};
		
		options = $.extend(options,{ 'onComplete': singleUploadComplete, 'onAllComplete': allUploadsComplete, 'onChange': jcropOnChange, 'onSelect': jcropOnChange });
		
		this.uploadify(options);
		
	}

})(jQuery);
