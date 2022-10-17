function Conteudo (){

	var d = document;
	var controleSpancao;
	var indNivel = 0;
	// Atributo guarda os elementos da tabela.
	var elemento = new Array();								

	this.debug = function (obj){  
	       var janela = window.open()
	       for(prop in obj){
	         janela.document.write(prop + ' = '+ obj[prop]+'<BR>');
	       }
	   
	 }

	function findPosY(obj){
		var curtop = 0;
	    if(obj.offsetParent)
	        while(1)
	        {
	          curtop += obj.offsetTop;
	          if(!obj.offsetParent)
	            break;
	          obj = obj.offsetParent;
	        }
	    else if(obj.y)
	        curtop += obj.y;
	    return curtop;
	}

	this.conteudoCarregado = function (){
		d.getElementById('temporizador').style.display = 'none';
	}
	
	this.retornaElemento = function (){
		return elemento;
	}
	
	this.conteudoCarregando = function (id){
		var div;
		var h;
		var w;
		var topImg;
		
		h = d.body.scrollHeight;
		w = d.body.scrollWidth;

		elementRefe = d.getElementById(id);
		h = h < screen.height ? screen.height : h;
		
		if (elementRefe){
			topImg = findPosY(elementRefe);
		}else{
			topImg = (h/4);
		}

		if (!d.getElementById('temporizador')){
			div = d.createElement("div");
			div.setAttribute('id', 'temporizador');
		}else{
			div = d.getElementById('temporizador');		
		}

		// Monta Imagem

		if (div.getElementsByTagName('img').length == 0){
			img = d.createElement("img");
		}else{
			img = div.getElementsByTagName('img')[0];
		}

		img.setAttribute('src', '/imagens/carregando.gif');
		img.style.cssText = 'position:relative; top:' + topImg + 'px;';
		div.appendChild(img);
		
		d.body.appendChild(div);
		d.getElementById('temporizador').style.cssText = '-moz-opacity:0.8; filter: alpha(opacity=80); background:#ffffff; text-align:center; position:absolute; top:0px; left:0px; width:' + w + 'px; height:' + h + 'px; z-index:1000;';
		return true;
	}	

	this.carregaElemento = function (arrParent, id, visivel, profundidade)
							{
								try{							
									var elementoNivel = new Array();
									var ind;
									
									// Verifica a profundidade 
									if (profundidade == 0){
										// Cria o elemento de primeiro nível
										elemento[indNivel] 			   = new Array();
										elemento[indNivel]['id'] 	   = id;
										elemento[indNivel]['visivel']  = visivel;
										elemento[indNivel]['elemento'] = new Array();
										
										indNivel++;								
									}else{
										elementoNivel = elemento;
										for (i=0; i < arrParent.length; i++){
											for(a=0; a < elementoNivel.length; a++){
												if (arrParent[i].id == elementoNivel[a].id){
													elementoNivel = elementoNivel[a]['elemento'];
													a = 0;
													break;
												}
											}
										}
										ind = elementoNivel.length == 0 ? 0 : elementoNivel.length;

										elementoNivel[ind] = new Array();
										elementoNivel[ind]['id'] 	   = id;
										elementoNivel[ind]['visivel']  = visivel;
										elementoNivel[ind]['elemento'] = new Array();
									}	
								}catch(err){
									alert('erro no mapeamento da tabela!');
								}
								return;
							}

	this.controle = function(id, idPaiTx, imgId)
					{
						imgObj = d.getElementById(imgId);
						if( !imgObj ){
							alert(imgId);
						}
						//this.conteudoCarregando(id);
						var elementoId;	
						var visibilidade   = '';
						var visibilidadeId = '';
						var idPai 		   = new Array();
						
						// Transforma string com pais em array.		
						if ((idPaiTx.indexOf(':') > -1)){
							idPai = idPaiTx.split(':');	
						}else if (idPaiTx){
							idPai.push(idPaiTx);
						}
						elementoId = elemento;						
						// Verifica se o elemento está no primeiro nível.
						if (idPaiTx != id){
							// Desce em níveis, até chegar no elemento.
							for (i=0; i < idPai.length; i++){
								for(a=0; a < elementoId.length; a++){
									//alert(elementoId[a].id);
									if (idPai[i] == elementoId[a].id){
										elementoId = elementoId[a]['elemento'];
										a = 0;
										break;
									}
								}
							}
							elementoId = elementoId;
						}

						for (z=0; z < elementoId.length; z++){
							if (elementoId[z].id == id){
								elementoId = elementoId[z];
								break;
							}
						}
						
						visibilidadeId = elementoId.visivel;
						
						// Modifica a imagem e seta o atributo de visibilidade.
						if (visibilidadeId == 'S'){
							imgObj.src 		   = '/imagens/mais.gif';
							imgObj.title 	   = 'Clique para expandir';		
							elementoId.visivel = 'N';
						}else{
							imgObj.src 	 	   = '/imagens/menos.gif';
							imgObj.title 	   = 'Clique para minimizar';		
							elementoId.visivel = 'S';		
						}
						// Faz maximização ou minimização dos elementos
						controleSpancao(elementoId, elementoId.visivel);
						this.conteudoCarregado();
					}
					
	controleSpancao = function (elementoId, visibilidade)
					  {
						var elementoAtual;
						var elementoAtualHtml;
						var display;
												
						// Seta ao atributo o valor de maximização ou minimização 
						display = visibilidade == 'N' ? 'none' : ( navigator.appName.indexOf('Explorer') > -1 ? 'block' : 'table-row');
						
						// Varre nos elementos filhos	
						for (var i in elementoId.elemento){						
							elementoAtual = elementoId.elemento[i];
							if (!elementoAtual.id){
								continue;
							}
							elementoAtualHtml 				= d.getElementById(elementoAtual.id);
							elementoAtualHtml.style.display = display;
							
							// Varre os elementos filhos, aplicando a recursividade.
							for (var a in elementoAtual.elemento){	
								if (!elementoAtual.elemento[a].id){
									continue;
								}
								
								if ( elementoId.elemento[i].visivel == 'S' ){
									elementoAtualHtml 				= d.getElementById(elementoAtual.elemento[a].id);
									elementoAtualHtml.style.display = display;
								}
									  
								if (elementoAtual.elemento[a].visivel == 'S' && ((visibilidade == 'N' && elementoId.elemento[i].visivel == 'S') || (visibilidade == 'S' && elementoId.elemento[i].visivel == 'S')) ){
									controleSpancao(elementoAtual.elemento[a], visibilidade);									
								}
							}
						} 
						return;							
					  }
}
/*
function Conteudo (){

	var d = document;
	var controleSpancao;
	// Atributo guarda os elementos da tabela.
	elemento = new Array();								

	this.carregaElemento = function (arrParent, id, visivel, profundidade)
							{
								try{							
									var txObj    = '';
									var tx		 = '';
									var txObjNew = '';
									var ponto    = '';
									
									// Verifica a profundidade 
									if (profundidade == 0){
										// Cria o elemento de primeiro nível
										txObj = 'elemento[\'' + id + '\'] = new Array();' + 
												'elemento[\'' + id + '\'][\'id\'] 		= \'' + id + '\';' + 
												'elemento[\'' + id + '\'][\'visivel\']  = \'' + visivel + '\';' +
												'elemento[\'' + id + '\'][\'elemento\'] = new Array();';
									}else{
										tx += 'elemento[\'' + arrParent[0].id + '\']';
										// Desce os níveis, para adicionar o novo elemento
										for (i=1; i < arrParent.length; i++){
											idParent 	= arrParent[i].id;
											idProParent = arrParent[i+1] ? arrParent[i+1].id : 1; 
											tx += '[\'elemento\'][\'' + idParent + '\']';
										}
										// Adiciona o novo elemento
										txObj += tx + '[\'elemento\'][\'' + id + '\'] = new Array();' + 
												 tx + '[\'elemento\'][\'' + id + '\'][\'id\'] 		 = \'' + id + '\';' + 
												 tx + '[\'elemento\'][\'' + id + '\'][\'visivel\']  = \'' + visivel + '\';' +
												 tx + '[\'elemento\'][\'' + id + '\'][\'elemento\'] = new Array();';
										
									}	
	
									// Executa o texto JS
									eval(txObj);	
								}catch(err){
									alert('erro no mapeamento da tabela!');
								}								
								return;
							}

	this.controle = function(id, idPaiTx, imgObj)
					{						
						var elementoId;	
						var visibilidade   = '';
						var visibilidadeId = '';
						var idPai 		   = new Array();
						
						// Transforma string com pais em array.		
						if ((idPaiTx.indexOf(':') > -1)){
							idPai = idPaiTx.split(':');	
						}else if (idPaiTx){
							idPai.push(idPaiTx);
						}	
						// Verifica se o elemento está no primeiro nível.
						if (idPaiTx != id){
							// Desce em níveis, até chegar no elemento.
							for(i=0; i<idPai.length; i++){
								elementoId = elementoId ? elementoId.elemento[idPai[i]] : elemento[idPai[i]];
							}
							elementoId = elementoId.elemento[id];
						}else{
							elementoId = elemento[id];		
						}
						
						visibilidadeId = elementoId.visivel;
						
						// Modifica a imagem e seta o atributo de visibilidade.
						if (visibilidadeId == 'S'){
							imgObj.src = '/imagens/mais.gif';		
							elementoId.visivel = 'N';
						}else{
							imgObj.src = '/imagens/menos.gif';
							elementoId.visivel = 'S';		
						}
						// Faz maximização ou minimização dos elementos
						controleSpancao(elementoId, elementoId.visivel);
					}
					
	controleSpancao = function (elementoId, visibilidade)
					  {
						var elementoFilho;
						var elementoAtual;
						var display;
						
						// Seta ao atributo o valor de maximização ou minimização 
						display = visibilidade == 'N' ? 'none' : ( navigator.appName.indexOf('Explorer') > -1 ? 'block' : 'table-row');
						
						// Varre nos elementos filhos	
						for (var i in elementoId.elemento){
							elementoAtual = elementoId.elemento[i];
							if (!elementoAtual.id){
								continue;
							}
							elementoFilho = d.getElementById(elementoId.elemento[i].id);
							if (elementoFilho){
								elementoFilho.style.display = display;
								// Varre os elementos filhos, aplicando a recursividade. 
								for (var a in elementoAtual.elemento){
									if (!d.getElementById(elementoAtual.elemento[a].id))
										continue;
										
									if (visibilidade == 'N' && elementoAtual.visivel == 'S'){
										controleSpancao(elementoAtual, visibilidade);
									}else if (visibilidade == 'S' && elementoId.elemento[i].visivel == 'S'){
										controleSpancao(elementoAtual, visibilidade);											
									}
								}	
							}
						} 
						return;							
					  }
	
}
*/