function Questionario(param){
	this.obPerg  	 	= new Pergunta();	   
	var qrpid 	   	 	= param.qrpid;
	var peridAtual 	 	= param.peridAtual;
	var url	  	   	 	= param.url;
	var obDadosParam 	= "";

	var bntProx	= jQuery('[value="Próximo"]').attr('disabled');
	var bntAnt	= jQuery('[value="Anterior"]').attr('disabled');

	// Obj tela
	var tela = jQuery('#telacentral');
	// Obj tela da árvore
	var telaA = jQuery('#telaarvore');
	// Obj tela do questionário
	var telaQ = jQuery('#telaquestionario');

	this.atualizaTela = function (perid){
		perid = perid ? perid : peridAtual;
		var urlParam = [{'name' : "ajax", 'value' : true}, 
						{'name' : "perid", 'value' : perid}, 
						{'name' : "qrpid", 'value' : qrpid}]; 
		
		urlParam = concatenaArray(urlParam, obDadosParam);
		
		desabilitaForm(true);
		
		telaQ.html('<center>carregando...</center>');
		
		jQuery.ajax({
			    type:	 "POST",
			    url: 	 url,
			    data: 	 urlParam,
			    async: 	 false,
			    success: function(html){
					peridAtual = perid;
					//extrairScript( trataRetornoAjax(html) );			   
			      	//alert( trataRetornoAjax(html) );
			      	//alert( html );
			      	//alert( peridAtual );
			      	tela.html( trataRetornoAjax(html) );
			      	verificaButtons();
			    }
			   });
	}
	
	this.buscaSubPergunta = function (param, obj){
	
		var div, arrVal, z, valOption;
		var perid = param.perid;
		var itpid = param.itpid;


		if ( !jQuery('#linha_' + perid + '_' + itpid )[0] ){
			this.closeSubPerguntas(perid);
//			jQuery('#tr_subpergunta_' + perid).hide();
			return;
		}

		arrVal = jQuery(obj).val();
	
		if ( jQuery(obj).attr("type") == 'radio' || (jQuery(obj).attr('tagName') == 'SELECT' && typeof(arrVal) != 'object' ) ){
			this.closeSubPerguntas(perid);
		}
		
		if (typeof(arrVal) == 'object'){
			jQuery('option:not(:selected)', obj).each(function (){
				valOption = jQuery(this).val();
				jQuery('#linha_' + perid + '_' + valOption ).hide();
			});
		}else{
			arrVal = new Array();
			arrVal.push( itpid );
		}

		desabilitaForm(true);
		for (z=0; z < arrVal.length; z++){
			itpid = arrVal[z];

			var urlParam = [{'name' : 'ajax',  'value' : true}, 
							{'name' : 'perid', 'value' : perid}, 
							{'name' : 'qrpid', 'value' : qrpid}, 
							{'name' : 'itpid', 'value' : itpid}]; 
							
			urlParam = concatenaArray(urlParam, obDadosParam);
			 
			if ( obj.checked || jQuery(obj).attr('tagName') == 'SELECT' ){			
	//			jQuery('#tr_subpergunta_' + perid).show();
				div = jQuery('#linha_' + perid + '_' + itpid ).html();
				jQuery('#linha_' + perid + '_' + itpid ).show();
				if (div.length > 10){
					continue;
				}	
				jQuery('#linha_' + perid + '_' + itpid ).html('carregando...');
			}else{
				jQuery('#linha_' + perid + '_' + itpid ).hide();
				continue;
			}
			
			
//			desabilitaForm(true);
			
			jQuery.ajax({
				    type:	 "POST",
				    url: 	 url,
				    data: 	 urlParam,
				    async: 	 false,
				    success: function(html){
				     	peridAtual = perid;
				     	jQuery('#linha_' + perid + '_' + itpid ).show();
				     	trataDiv = '<div>' + trataRetornoAjax(html);
				     	jQuery('#linha_' + perid + '_' + itpid ).html( trataDiv );
//						desabilitaForm(false);			    
						}
				   });	   
			   
		}
		desabilitaForm(false);			    
		
	}
	
	this.salvar = function (perid){
		
		divCarregando();
		
		var condicao, msgValidacao;

		if( jQuery(':file').length > 0 ){
			if(jQuery(':file').val()){
				peridAtual = perid;
				document.getElementById('perid').value = perid;
				document.formulario.submit();
				return true;
			}
		}

		jQuery("[name^='perg_']").each(function (){
			jQuery(this).attr("name", "perg[" + jQuery(this).attr('name').substring(5) + "]");
		});

/*		for (var i in dados ){
			if(dados[i].name.indexOf('perg_') == 0){
				jQuery("[name='" + dados[i].name + "']").attr('name', 'perg[' + dados[i].name.substring(5, dados[i].name.length) + ']');
				dados[i].name = 'perg[' + dados[i].name.substring(5, dados[i].name.length) + ']';
			}
		}
*/		
		msgValidacao = this.obPerg.validaCampoObrig();
		if ( msgValidacao != "" ){
			alert(msgValidacao);
			divCarregado();
			return false;
		}
		
/*		if (dados.length == 0 || !condicao){
			alert('Responda a pergunta antes de salvar!');
			return;
		}*/
		dados = new Array();
		if( jQuery('#idTabela').val() || jQuery('#identExterno').val() ){
			dados = jQuery('#formulario').serializeArray();
		} else {
			dados = this.obPerg.pegaDadosValido();
		}
		
		
		functionPosAcao = this.obPerg.getFuncaoPergunta();
		functionPosAcao = functionPosAcao.replace("qrpid", qrpid);
		functionPosAcao = functionPosAcao.replace("perid", perid);
		

		desabilitaForm(true);
		
		dados.push({
					name  : "salvar_questionario",
					value : true
					});
		/*
		 * Após salvar, busca a pergunta ANTERIOR | ATUAL | PRÓXIMA	
		 */			
		dados.push({
					name  : "ajax",
					value : true
					});
					
		dados.push({
					name  : "perid",
					value : perid
					});
					
		dados.push({
					name  : "qrpid",
					value : qrpid
					});
		

		
		dados = concatenaArray(dados, obDadosParam);
		/*
		 * FIM - Após salvar, busca a pergunta ANTERIOR | ATUAL | PRÓXIMA	
		 */			

		jQuery.ajax({
			    type:	 "POST",
			    url: 	 url,
			    data: 	 dados,
			    async: 	 false,
			    success: function(html){
			    	peridAtual = perid;
			    	//alert('Operação realizada com sucesso!');
			    	//alert( html );
			    	tela.html( trataRetornoAjax(html) );
			    	verificaButtons();
			    	eval( functionPosAcao );
			    	divCarregado();
			    }
			   });				
			   
	}
	
	this.closeSubPerguntas =  function (perid){
		jQuery('[id*="linha_' + perid + '_"]').each(function (){
						jQuery(this).hide();
					})				
/*	
		var linha = jQuery('#tr_subpergunta_' + perid);
		if (linha[0]){
			jQuery('#tr_subpergunta_' + perid + ' td div').each(function (){
				jQuery(this).hide();
			})		
		}		
*/		
	}	

	this.carregaParamUrl = function (obParam){
		obDadosParam = eval( obParam );
//		alert( obDadosParam );
	}
	
	function concatenaArray(arrElemento, arrConcatena){
		for (indArr=0; indArr < arrConcatena.length; indArr++){
			if( arrConcatena[indArr].name != 'perid' ){
				arrElemento.push(arrConcatena[indArr]);
			}
		}
//		for (indArr=0; indArr < arrElemento.length; indArr++){
//			alert( arrElemento[indArr].name + ' - ' + arrElemento[indArr].value )
//		}
		return arrElemento;
	}	

	function trataRetornoAjax(html){
	   	var iniReturn = html.indexOf('<table');
//	   	iniReturn = iniReturn > -1 ? iniReturn : html.indexOf('<table');; 
	   	iniReturn = iniReturn > -1 ? iniReturn : 0; 
	   	
	   	var fimReturn = html.lastIndexOf('</table>');
//		fimReturn = fimReturn ? fimReturn : html.indexOf('</table>'); 
		fimReturn = fimReturn ? fimReturn : html.length; 
		
		var retorno = html.substr(iniReturn, fimReturn);
		return retorno;
	}
	
	function desabilitaForm(param){
		jQuery('#formulario select,textarea,input').each(function (){
			if(jQuery(this).attr("name") != "csFiltroArvore"){
				if ( (jQuery(this).attr("value").indexOf('Próximo') > -1 && !bntProx) 
					 || 
					 (jQuery(this).attr("value").indexOf('Anterior') > -1 && !bntAnt) 
				   	 || 
				   	 (jQuery(this).attr("value").indexOf('Anterior') == -1 && jQuery(this).attr("value").indexOf('Próximo') == -1) ){
					jQuery(this).attr("disabled", param);
				}
			}
		});
	}
	
	function verificaButtons(){
		bntProx	= jQuery('[value="Próximo"]').attr('disabled');
		bntAnt	= jQuery('[value="Anterior"]').attr('disabled');
	}
	
}

function Pergunta(){
	var pergunta 	    = new Array();
	var msgValObrig	    = '';
	var functionPosAcao = '';
	 
	this.add = function (obParam){
		var nivelProx;
		
		if ( pergunta.length == 0 ){
			pergunta[0] 		   		= new Array();
			pergunta[0]["id"] 	   		= obParam.id;
			pergunta[0]["obrig"]   		= obParam.obrig;
			pergunta[0]["tipo"]    		= obParam.tipo;
			pergunta[0]["idPai"]   		= obParam.idPai;
			pergunta[0]["itemPai"] 		= obParam.itemPai;
			pergunta[0]["txt"] 	   		= obParam.txt;
			pergunta[0]["perposacao"] 	= obParam.perposacao;
			pergunta[0]["new"]     = new Array();
		}else{
			elementPergunta = this.buscaElemento( obParam.idPai );
			nivelProx 		= (elementPergunta["new"].length == 0 ? 0 : elementPergunta["new"].length);
			
			elementPergunta["new"][nivelProx] 		     	= new Array();
			elementPergunta["new"][nivelProx]["id"]      	= obParam.id; 
			elementPergunta["new"][nivelProx]["obrig"]   	= obParam.obrig; 
			elementPergunta["new"][nivelProx]["tipo"]    	= obParam.tipo; 
			elementPergunta["new"][nivelProx]["idPai"] 	 	= obParam.idPai; 
			elementPergunta["new"][nivelProx]["itemPai"] 	= obParam.itemPai; 
			elementPergunta["new"][nivelProx]["txt"]	 	= obParam.txt; 
			elementPergunta["new"][nivelProx]["perposacao"]	= obParam.perposacao; 
			elementPergunta["new"][nivelProx]["new"]     	= new Array();
		}
	
	}
	
	this.clean = function (){
		pergunta = new Array();
	}
/*	
	this.buscaElementoPai = function (elementPergunta, idPai){
		var i;
		
		if (elementPergunta["id"] == idPai){
			return elementPergunta; 
		}else{
			for(i=0; i < elementPergunta["new"].length; i++){
				return this.buscaElementoPai( elementPergunta["new"][i], idPai );
			}
		}
	}
*/	
	this.buscaElemento = function (id, elementPergunta){
		var i;
		elementPergunta = elementPergunta ? elementPergunta : pergunta[0];
		
		if (elementPergunta["id"] == id){
			return elementPergunta; 
		}else{
			for(i=0; i < elementPergunta["new"].length; i++){
				elemento = this.buscaElemento( id, elementPergunta["new"][i] );
		
				if (elemento){
					return elemento;
				}				
			}
		}
	}
	
	this.buscaArrElementoPorItem = function ( itemPai, elementPergunta, arrElementPergunta ){
		var i;
		
		arrElementPergunta = arrElementPergunta ? arrElementPergunta : new Array()
		elementPergunta    = elementPergunta ? elementPergunta : pergunta[0];

		if (elementPergunta["itemPai"] == itemPai){
			arrElementPergunta.push( elementPergunta );
		}

		for(i=0; i < elementPergunta["new"].length; i++){
			arrElementPergunta = this.buscaArrElementoPorItem( itemPai, elementPergunta["new"][i], arrElementPergunta );
		}
		
		return arrElementPergunta;
	}
	


	this.validaCampoObrig = function (perg){
		var i, ii, b, valPerg, subPerg, msgRetorno; 
		var msg = "";
		
		perg = perg ? perg : pergunta[0];		

		if ( jQuery('[name*="perg[' + perg.id + ']"]').attr('type') == 'radio' || jQuery('[name*="perg[' + perg.id + ']"]').attr('type') == 'checkbox' ){
			valPerg = new Array();
			jQuery('[name*="perg[' + perg.id + ']"]:checked').each(function (){
				valPerg.push(jQuery(this).val());
			});
		}else if ( jQuery('[name*="perg[' + perg.id + ']"]').attr('tagName') == "SELECT" ){
			valPerg = new Array();
			jQuery('[name*="perg[' + perg.id + ']"] option:selected').each(function (){
				if (jQuery(this).val().trim()){
					valPerg.push(jQuery(this).val());
				}
			});
		}else{
			valPerg = jQuery('[name*="perg[' + perg.id + ']"]').val();
		}
		valPerg = (typeof(valPerg) != 'object' ? jQuery.trim( valPerg ) : valPerg);
		if ( perg.obrig == true && valPerg == "" ){
			msg += 'O campo "'+ perg.txt + '" é obrigatório!\n';
		}else if ( typeof(valPerg) == 'object' ){
			for (ii=0; ii < valPerg.length; ii++){
				subPerg = this.buscaArrElementoPorItem( valPerg[ii] );
				for(b=0; b < subPerg.length; b++){
					msgRetorno = this.validaCampoObrig( subPerg[b] );
					if ( msgRetorno ){
						msg += msgRetorno;
					}
				}
			}
		}
		
		return ( msg );
	}
	
	this.pegaDadosValido = function (perg, ArrObDados){
		var ii, bbb;
		var valPerg 	= new Array();
		functionPosAcao = ArrObDados ? functionPosAcao : ""; 
		ArrObDados  	= ArrObDados ? ArrObDados : new Array(); 
		perg 			= perg ? perg : pergunta[0];
		
		if ( jQuery('[name*="perg[' + perg.id + ']"]').attr('type') == 'radio' || jQuery('[name*="perg[' + perg.id + ']"]').attr('type') == 'checkbox' ){
			valPerg = new Array();
			jQuery('[name*="perg[' + perg.id + ']"]:checked').each(function (){
				valPerg.push(jQuery(this).val());
			});
		}else if ( jQuery('[name*="perg[' + perg.id + ']"]').attr('tagName') == "SELECT" ){
			valPerg = new Array();
			jQuery('[name*="perg[' + perg.id + ']"] option:selected').each(function (){
				if (jQuery(this).val().trim()){
					valPerg.push(jQuery(this).val());
				}
			});
		}else{
			valPerg = jQuery('[name*="perg[' + perg.id + ']"]').val();
		}
						
		if ( typeof(valPerg) == 'object' ){
			for (ii=0; ii < valPerg.length; ii++){
				ArrObDados.push({
								 "name"  : 'perg[' + perg.id + '][]',
								 "value" : valPerg[ii]
								});

				// Concatena funções
				functionPosAcao += perg.perposacao + ';';
//				functionPosAcao.replace("perid", perg.id);
				
				var subPerg = this.buscaArrElementoPorItem( valPerg[ii] );
			
				for(bbb=0; bbb < subPerg.length; bbb++){
					ArrObDados = this.pegaDadosValido( subPerg[bbb], ArrObDados );
				}
			}		
		}else{
			ArrObDados.push({
							 "name"  : 'perg[' + perg.id + ']',
							 "value" : valPerg
							}); 
			
			// Concatena funções
			functionPosAcao += perg.perposacao + ';';
//			functionPosAcao.replace("perid", perg.id);
		}
		
		return ArrObDados;
	}

	this.getFuncaoPergunta = function(){
		return functionPosAcao;
	}
}

/*
var p = new Pergunta();
p.add({"id" : 1 , "obrig" : true, "tipo" : "CK" ,"idPai" : "", "itemPai" : ""});
p.add({"id" : 2 , "obrig" : true, "tipo" : "CK" ,"idPai" : 1, "itemPai" : "224"});
p.add({"id" : 3 , "obrig" : false,"tipo" : "CK" ,"idPai" : 2, "itemPai" : "224"});
p.add({"id" : 4 , "obrig" : true, "tipo" : "CK" ,"idPai" : 2, "itemPai" : "224"});
p.add({"id" : 5 , "obrig" : false,"tipo" : "CK" ,"idPai" : 4, "itemPai" : "224"});
p.add({"id" : 6 , "obrig" : false,"tipo" : "CK" ,"idPai" : 4, "itemPai" : "224"});
p.add({"id" : 7 , "obrig" : false,"tipo" : "CK" ,"idPai" : 1, "itemPai" : "224"});
p.add({"id" : 8 , "obrig" : true, "tipo" : "CK","idPai" : 5, "itemPai" : "224"});
p.add({"id" : 9 , "obrig" : true, "tipo" : "CK","idPai" : 8, "itemPai" : "224"});

alert( p.validaCampoObrig() );
*/
//alert( p.validaCampoObrig() );

/*
ele = p.buscaElemento(9);
alert(ele['idPai']);

ele = p.buscaArrElementoPorItem(224);
alert(ele[0]['id'] + '\n' 
		+ ele[1]['id'] + '\n' 
		+ ele[2]['id'] + '\n' 
		+ ele[3]['id'] + '\n' 
		+ ele[4]['id'] + '\n' 
		+ ele[5]['id'] + '\n' 
		+ ele[6]['id'] + '\n' 
		+ ele[7]['id']);
*/
		
		