/**
 * Inicializa processamento para criação de textos editáveis com recursos
 * avançados.
 * 
 * @author Renan de Lima Barbosa <renandelima@gmail.com>
 * @author Renê de Lima Barbosa <renedelima@gmail.com>
 */
var TextEditor = ( function(){
	
	/**
	 * Língua utilizada pelos campos.
	 * 
	 * @var string
	 */
	var sLanguage = 'en';
	
	/**
	 * Inicia processamento para criação dos campos.
	 * 
	 * @return void
	 */
	this.init = function( sLanguage )
	{
		tinyMCE.init({
			plugins : 'paste',
			theme : 'advanced',
			mode: 'specific_textareas',
			editor_selector : 'text_editor_simple',
			theme_advanced_toolbar_location : 'top',
			theme_advanced_toolbar_align : 'left',
			theme_advanced_buttons1 : 'cut,copy,paste,separator,undo,redo,separator,bold,italic,underline,removeformat,separator,justifyleft,justifycenter,justifyright,justifyfull,separator,numlist,bullist,separator,outdent,indent,separator,help',
			theme_advanced_buttons2 : '',
			theme_advanced_buttons3 : '',
			language : sLanguage,
			docs_language : sLanguage,
			gecko_spellcheck : true,
			object_resizing : false,
			content_css: "/stdclass/public/stylesheet/screen.css",
			width : "465px"
		});
		tinyMCE.init({
			plugins : 'table,print,preview,paste,fullscreen',
			theme : 'advanced',
			mode: 'specific_textareas',
			editor_selector : 'text_editor_normal',
			theme_advanced_toolbar_location : 'top',
			theme_advanced_toolbar_align : 'left',
			theme_advanced_buttons1 : 'fullscreen,preview,print,separator,cut,copy,paste,pastetext,separator,undo,redo,separator,link,unlink,separator,tablecontrols,separator,help',
			theme_advanced_buttons2 : 'fontselect,fontsizeselect,separator,bold,italic,underline,strikethrough,removeformat,separator,justifyleft,justifycenter,justifyright,justifyfull,separator,numlist,bullist,separator,outdent,indent,separator,forecolor,backcolor',
			theme_advanced_buttons3 : '',
			language : sLanguage,
			docs_language : sLanguage,
			gecko_spellcheck : true,
			content_css: "/stdclass/public/stylesheet/screen.css",
			width : "600px"
		});
		tinyMCE.init({
			plugins : 'table,print,preview,paste,searchreplace,media,layer,insertdatetime,fullscreen,xhtmlxtras,visualchars',
			theme : 'advanced',
			mode: 'specific_textareas',
			editor_selector : 'text_editor_full',
			theme_advanced_toolbar_location : 'top',
			theme_advanced_toolbar_align : 'left',
			theme_advanced_buttons1 : 'fullscreen,preview,print,separator,selectall,cut,copy,paste,pastetext,separator,undo,redo,separator,link,unlink,separator,tablecontrols,separator,help',
			theme_advanced_buttons2 : 'fontselect,fontsizeselect,separator,bold,italic,underline,strikethrough,removeformat,separator,sub,sup,separator,justifyleft,justifycenter,justifyright,justifyfull,separator,numlist,bullist,separator,outdent,indent,separator,forecolor,backcolor',
			theme_advanced_buttons3 : 'search,replace,separator,image,media,charmap,insertdate,inserttime,separator,hr,cite,ins,del,abbr,acronym,code,separator,insertlayer,moveforward,movebackward,absolute,,separator,visualaid,visualchars',
			language : sLanguage,
			docs_language : sLanguage,
			gecko_spellcheck : true,
			object_resizing : false,
			content_css: "/stdclass/public/stylesheet/screen.css",
			width : "660px"
		});
		tinyMCE.init({
			mode: 'specific_textareas',
			editor_selector : 'cte',
			theme : "advanced",
			plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
			theme_advanced_buttons1 : "cut,copy,paste,pastetext,pasteword,separator,search,replace,separator,tablecontrols, separator, preview",
			theme_advanced_buttons2 : "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright,justifyfull,separator,outdent,indent,bullist,numlist,separator,forecolor,fontselect,fontsizeselect ",
			theme_advanced_buttons3 : "",
			theme_advanced_toolbar_location : "top",
			theme_advanced_toolbar_align : "left",
			language : "pt_br",
			entity_encoding : "raw",
			width : "670px",
			onchange_callback : "marcarAlteracao"
		});
	}
	
	/**
	 * Altera a lingua utilizada pelos campos.
	 * 
	 * @param string
	 * @return void
	 */
	this.setLanguage = function( sNewLanguage )
	{
		sLanguage = sNewLanguage + '';
	}
	
	return this;
	
} )();
