/**
 * GerenciaTabEntidade => Gerencia a inserção de novas linhas/conteúdos na tabela que contém o formEntidade 
 * 
 * @author  Felipe Tarchiani Cerávolo Chiavicatti
 * @since   27/08/2009
 * @exemple /simec/academico/modulos/principal/dadosentidade.inc
 * @link    http://simec-local/academico/academico.php?modulo=principal/dadosentidade&acao=A
 */
function GerenciaTabEntidade(){
	// ID da tabela onde será manipulada as linhas
	this.idTabela = 'tblentidade';
	var d		  = document;
	
	/**
     * Função que adicionará a nova linha com seu respectivo conteúdo.
     * 
     * @see GerenciaTabEntidade()
     * @access public
     * @name addTR
	 * @param string trRef => ID da TR de referência, será inserida a nova linha logo abaixo.
	 * @param string trId  => ID da TR que será adicionada na tabela.
	 * @param string label => Texto que será o label do campo, dentro na nova linha. 
	 * @param string campoHTML => Texto HTML que contém o campo a ser adicionado na nova linha.
	 * @param boolean tipoLinhaTitulo (default = false) => Se "true" monta linha do tipo título, senão do tipo campo.  
     */
	this.addTR = function (trRef, trId, label, campoHTML, tipoLinhaTitulo){
		try {
			var trRow, nTR, nTD1, nTD2;
			// Carrega o elemento "TR" de referência
			var eRef 	  	= d.getElementById(trRef);
			// Carrega o elemento "tabela"
			var tabela    	= d.getElementById(this.idTabela);			
			tipoLinhaTitulo = tipoLinhaTitulo ? tipoLinhaTitulo : false;
			
			// Dispara exceção
			if (!trRef || !eRef){
				throw new Error('A TR de referência não é um elemento.');							
			}
			// Dispara exceção
			if (!tabela){
				throw new Error('A TABELA que foi setada para manipulação, não existe.');										
			}
			
			// Pega posição da TR de referência na tabela
			trRow = eRef.rowIndex;
			// Cria novo elemento TR
			nTR = tabela.insertRow(trRow+1);
			nTR.setAttribute('id', trId);
			
			// Verifica de qual tipo será a nova linha a ser adicionada
			if (tipoLinhaTitulo){
				// Cria TD do elemento
				nTD1 = d.createElement('td');
				nTD1.setAttribute('class',   'SubTituloCentro');
				nTD1.setAttribute('colspan', '2');
				nTD1.innerHTML = label;		
				// Add TD na TR
				nTR.appendChild(nTD1);			
			}else{
				// Cria o 1ª elemento TD
				nTD1 = d.createElement('td');
				nTD1.setAttribute('class',   'SubTituloDireita');
				nTD1.innerHTML = label;		
				// Cria o 2ª elemento TD
				nTD2 = d.createElement('td');
				nTD2.innerHTML = campoHTML;						
				
				// Add TD na TR
				nTR.appendChild(nTD1);		
				nTR.appendChild(nTD2);										
			}
			
		// Tratamento de erro	
		}catch (err){
			alert(err);
		}
	}

	/**
     * Função que bloqueia campos.
     * 
     * @see GerenciaTabEntidade()
     * @access public
     * @name blockCampo
	 * @param string|array idCampo => ID's dos campos que seram bloqueados.
	 * @tutorial (string) idCampo = 'iddocampo'
				 (array) idCampo = ['iddocampo1','iddocampo2','iddocampo3'];
	 * @param string|array notBlock  => Deve ser passado "atributo/eventos (pré-definidos pelo método)" que NÃO devem ser bloqueados.
	 * @tutorial (string) notBlock = 'class'
				 (array) notBlock = ['class','readOnly','onblur'];
     */	
	this.blockCampo = function (idCampo, notBlock){
		try{
			if (typeof(idCampo) != 'object'){
				idCampo = new Array(idCampo);
			}
			notBlock   = notBlock ? notBlock : '';		
			for (i=0; i<idCampo.length; i++){
				var eCampo = d.getElementById(idCampo[i]);
				// Dispara exceção
				if (!eCampo){
					alert('felppe');
					throw new Error('O campo setado para ser bloqueado não existe!');										
				}
				// Seta a classe para que fique com aparência de desabilitado.
				if (notBlock.indexOf('class') == -1){
					eCampo.className = 'disabled';
				}
				// Seta para true o atributo
				if (notBlock.indexOf('readOnly') == -1){
					eCampo.readOnly = true;
				}
				// Seta para vazio o evento
				if (notBlock.indexOf('onfocus') == -1){
					eCampo.setAttribute('onfocus', '');
				}			
 				// Seta para vazio o evento
				if (notBlock.indexOf('onmouseout') == -1){
					eCampo.setAttribute('onmouseout', '');
				}
 				// Seta para vazio o evento
				if (notBlock.indexOf('onblur') == -1){
					eCampo.setAttribute('onblur', '');
				}
 				// Seta para vazio o evento
				if (notBlock.indexOf('onkeyup') == -1){
					eCampo.setAttribute('onkeyup', '');
				}
			}
		// Tratamento de erro
		}catch (err){
			alert(err);
		}
	}
}


/*
 * Definições em javascript
 * ID endereço residencial (TPEEND_RESIDENCIAL)
 */
var TPEEND_RESIDENCIAL = 2;
var TPEEND_COMERCIAL   = 1;

// Função utilizada para abri o mapa na tela de entidade, na área de endereço
// recebe o tipo de endereço para que grave os dados no tipo correto
function abreMapaEntidade(tipoendereco){
	var graulatitude = window.document.getElementById("graulatitude"+tipoendereco).value;
	var minlatitude  = window.document.getElementById("minlatitude"+tipoendereco).value;
	var seglatitude  = window.document.getElementById("seglatitude"+tipoendereco).value;
	var pololatitude = window.document.getElementById("pololatitude"+tipoendereco).value;
	
	var graulongitude = window.document.getElementById("graulongitude"+tipoendereco).value;
	var minlongitude  = window.document.getElementById("minlongitude"+tipoendereco).value;
	var seglongitude  = window.document.getElementById("seglongitude"+tipoendereco).value;
	
	var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
	var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
	var entid = window.document.getElementById("entid").value;
	var janela=window.open('../apigoogle/php/mapa_padraon.php?tipoendereco='+tipoendereco+'&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude+'&entid='+entid, 'mapa','height=650,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();

}
// Função AJAX que pega os dados do endereço atraves do CEP
// as informações são retornadas com o separador "||"
function getEnderecoPeloCEP(endcep,tipoendereco) {
	var myAjax = new Ajax.Request('/geral/consultadadosentidade.php?requisicao=pegarenderecoPorCEP&endcep=' + endcep,
		{
			method: 'post',
			asynchronous: false,
			onComplete: function(resp) {
				var dados = resp.responseText.split("||");
				document.getElementById('endlog'+tipoendereco).value = dados[0];
				document.getElementById('endbai'+tipoendereco).value = dados[1];
				document.getElementById('mundescricao'+tipoendereco).value = dados[2];
				if(dados[3]) document.getElementById('estuf'+tipoendereco).value = dados[3];
				if(dados[4]) document.getElementById('muncod'+tipoendereco).value = dados[4];
			}
		});
}
// Carrega o formulario da entidade com dados da pessoa juridica
function carregaDadosEntidadeJuridica(dados) {
	if(dados[0]) document.getElementById('entid').value = dados[0];
	if(dados[1]) document.getElementById('njuid').value = dados[1];
	if(dados[3]) document.getElementById('entnome').value = dados[3];
	if(dados[4]) document.getElementById('entemail').value = dados[4];
	if(dados[6]) document.getElementById('entobs').value = dados[6];
	if(dados[28]) document.getElementById('entsig').value = dados[28];
	if(dados[15]) document.getElementById('entnumdddcomercial').value = dados[15];
	if(dados[17]) document.getElementById('entnumcomercial').value = dados[17];
	if(dados[16]) document.getElementById('entnumramalcomercial').value = dados[16];
	if(dados[18]) document.getElementById('entnumdddfax').value = dados[18];
	if(dados[20]) document.getElementById('entnumfax').value = dados[20];
	if(dados[19]) document.getElementById('entnumramalfax').value = dados[19];
	if(dados[21]) document.getElementById('tpctgid').value = dados[21];
	if(dados[22]) document.getElementById('tpcid').value = dados[22];
	if(dados[23]) document.getElementById('tplid').value = dados[23];
	if(dados[24]) document.getElementById('tpsid').value = dados[24];
	
	
	
	if(document.getElementById('funcoescadastradas').options.length > 0) {
		for (i = document.getElementById('funcoescadastradas').length - 1; i>=0; i--) {
		    document.getElementById('funcoescadastradas').remove(i);
		}
	}
	// verifica se possui funções
	if(dados[36] != "naofuncoes") {
		var funcoes = new Array();
		funcoes = dados[36].split("%%");
		if(funcoes[0]) {
			for(i=0;i<funcoes.length;i++) {
				var y=document.createElement('option');
				y.text=funcoes[i];
				y.value="";
				try {
					document.getElementById("funcoescadastradas").add(y,null);                        
				}catch(ex){
					document.getElementById("funcoescadastradas").add(y);
				}

			}
		}
	}else{
		document.getElementById('funcoescadastradas').options[0]= new Option('Não existem funções cadastradas',"");
	}
	// verifica se possui endereço
	if(dados[37] != "naoendereco") {
		var enderecos = new Array();
		enderecos = dados[37].split("%%");
		if(enderecos[0]) {
			for(i=0;i<enderecos.length;i++) {
				var dadosendereco = enderecos[i].split("##");
				if(document.getElementById('endid'+dadosendereco[1])) {
					document.getElementById('endid'+dadosendereco[1]).value = dadosendereco[0];
					document.getElementById('endcep'+dadosendereco[1]).value = dadosendereco[2];
					document.getElementById('endlog'+dadosendereco[1]).value = dadosendereco[3];
					document.getElementById('endcom'+dadosendereco[1]).value = dadosendereco[4];
					document.getElementById('endbai'+dadosendereco[1]).value = dadosendereco[5];
					if(dadosendereco[6]) document.getElementById('muncod'+dadosendereco[1]).value = dadosendereco[6];
					if(dadosendereco[7]) document.getElementById('estuf'+dadosendereco[1]).value = dadosendereco[7];
					if(dadosendereco[8]) document.getElementById('endnum'+dadosendereco[1]).value = dadosendereco[8];
					var latitude = dadosendereco[9].split(".");
					if(latitude != "") {
						document.getElementById('graulatitude'+dadosendereco[1]).value = latitude[0];
						document.getElementById('_graulatitude'+dadosendereco[1]).innerHTML = latitude[0];
						document.getElementById('minlatitude'+dadosendereco[1]).value = latitude[1];
						document.getElementById('_minlatitude'+dadosendereco[1]).innerHTML = latitude[1];
						document.getElementById('seglatitude'+dadosendereco[1]).value = latitude[2];
						document.getElementById('_seglatitude'+dadosendereco[1]).innerHTML = latitude[2];
						document.getElementById('pololatitude'+dadosendereco[1]).value = latitude[3];
						document.getElementById('_pololatitude'+dadosendereco[1]).innerHTML = latitude[3];
					}
					var longitude = dadosendereco[10].split(".");
					if(longitude != "") {
						document.getElementById('graulongitude'+dadosendereco[1]).value = longitude[0];
						document.getElementById('_graulongitude'+dadosendereco[1]).innerHTML = longitude[0];
						document.getElementById('minlongitude'+dadosendereco[1]).value = longitude[1];
						document.getElementById('_minlongitude'+dadosendereco[1]).innerHTML = longitude[1];
						document.getElementById('seglongitude'+dadosendereco[1]).value = longitude[2];
						document.getElementById('_seglongitude'+dadosendereco[1]).innerHTML = longitude[2];
						document.getElementById('pololongitude'+dadosendereco[1]).value = longitude[3];
						document.getElementById('_pololongitude'+dadosendereco[1]).innerHTML = longitude[3];
					}
					document.getElementById('endzoom'+dadosendereco[1]).value = dadosendereco[12];
					document.getElementById('mundescricao'+dadosendereco[1]).value = dadosendereco[13];
				}
			}
		}
	}
}
// Carrega o formulario da entidade com dados da pessoa física
function carregaDadosEntidadeFisica(dados) {
	if(dados[0]) document.getElementById('entid').value = dados[0];
	if(dados[3]) document.getElementById('entnome').value = dados[3];
	if(dados[4]) document.getElementById('entemail').value = dados[4];
	if(dados[6]) document.getElementById('entobs').value = dados[6];
	if(dados[7]) document.getElementById('entnumrg').value = dados[7];
	if(dados[8]) document.getElementById('entorgaoexpedidor').value = dados[8];
	if(dados[9]) document.getElementById('entsexo').value = dados[9];
	if(dados[10]) document.getElementById('entdatanasc').value = dados[10];
	if(dados[13]) document.getElementById('entnumdddresidencial').value = dados[13];
	if(dados[14]) document.getElementById('entnumresidencial').value = dados[14];
	if(dados[15]) document.getElementById('entnumdddcomercial').value = dados[15];
	if(dados[17]) document.getElementById('entnumcomercial').value = dados[17];
	if(dados[16]) document.getElementById('entnumramalcomercial').value = dados[16];
	if(dados[18]) document.getElementById('entnumdddfax').value = dados[18];
	if(dados[20]) document.getElementById('entnumfax').value = dados[20];
	if(dados[19]) document.getElementById('entnumramalfax').value = dados[19];
	if(dados[32]) document.getElementById('entnumdddcelular').value = dados[32];
	if(dados[33]) document.getElementById('entnumcelular').value = dados[33];
	if(document.getElementById('funcoescadastradas').options.length > 0) {
		for(i=0;i<document.getElementById('funcoescadastradas').options.length;i++) {
			document.getElementById('funcoescadastradas').remove(i);
		}
	}
	// verifica se possui funções
	if(dados[36] != "naofuncoes") {
		var funcoes = new Array();
		funcoes = dados[36].split("%%");
		if(funcoes[0]) {
			for(i=0;i<funcoes.length;i++) {
				document.getElementById('funcoescadastradas').options[i]= new Option(funcoes[i],"");
			}
		}
	}else{
		document.getElementById('funcoescadastradas').options[0]= new Option('Não existem funções cadastradas',"");
	}
	// verifica se possui endereço
	if(dados[37] != "naoendereco") {
		var enderecos = new Array();
		enderecos = dados[37].split("%%");
		if(enderecos[0]) {
			for(i=0;i<enderecos.length;i++) {
				var dadosendereco = enderecos[i].split("##");
					if(document.getElementById('endid'+dadosendereco[1])) {
					document.getElementById('endid'+dadosendereco[1]).value = dadosendereco[0];
					document.getElementById('endcep'+dadosendereco[1]).value = dadosendereco[2];
					document.getElementById('endlog'+dadosendereco[1]).value = dadosendereco[3];
					document.getElementById('endcom'+dadosendereco[1]).value = dadosendereco[4];
					document.getElementById('endbai'+dadosendereco[1]).value = dadosendereco[5];
					if(dadosendereco[6]) document.getElementById('muncod'+dadosendereco[1]).value = dadosendereco[6];
					if(dadosendereco[7]) document.getElementById('estuf'+dadosendereco[1]).value = dadosendereco[7];
					
					document.getElementById('endnum'+dadosendereco[1]).value = dadosendereco[8];
					var latitude = dadosendereco[9].split(".");
					if(latitude != "") {
						document.getElementById('graulatitude'+dadosendereco[1]).value = latitude[0];
						document.getElementById('_graulatitude'+dadosendereco[1]).innerHTML = latitude[0];
						document.getElementById('minlatitude'+dadosendereco[1]).value = latitude[1];
						document.getElementById('_minlatitude'+dadosendereco[1]).innerHTML = latitude[1];
						document.getElementById('seglatitude'+dadosendereco[1]).value = latitude[2];
						document.getElementById('_seglatitude'+dadosendereco[1]).innerHTML = latitude[2];
						document.getElementById('pololatitude'+dadosendereco[1]).value = latitude[3];
						document.getElementById('_pololatitude'+dadosendereco[1]).innerHTML = latitude[3];
					}
					var longitude = dadosendereco[10].split(".");
					if(longitude != "") {
						document.getElementById('graulongitude'+dadosendereco[1]).value = longitude[0];
						document.getElementById('_graulongitude'+dadosendereco[1]).innerHTML = longitude[0];
						document.getElementById('minlongitude'+dadosendereco[1]).value = longitude[1];
						document.getElementById('_minlongitude'+dadosendereco[1]).innerHTML = longitude[1];
						document.getElementById('seglongitude'+dadosendereco[1]).value = longitude[2];
						document.getElementById('_seglongitude'+dadosendereco[1]).innerHTML = longitude[2];
						document.getElementById('pololongitude'+dadosendereco[1]).value = longitude[3];
						document.getElementById('_pololongitude'+dadosendereco[1]).innerHTML = longitude[3];
					}
					document.getElementById('endzoom'+dadosendereco[1]).value = dadosendereco[12];
					document.getElementById('mundescricao'+dadosendereco[1]).value = dadosendereco[13];
				}
			}
		}
	} else {
		if(document.getElementById('endcep'+TPEEND_COMERCIAL)) {
			document.getElementById('endcep'+TPEEND_COMERCIAL).value="";
			document.getElementById('endcep'+TPEEND_COMERCIAL).onkeyup();
			document.getElementById('endcep'+TPEEND_COMERCIAL).onblur();
		}
	}
}
function ProcessaMuitasEntidades(texto, tpe) {
	var entidadesl = texto.split("$$");
	var html = "<div style='height:295px;overflow:auto;'><table class='tabela' align='center'>";
	html += "<tr><td colspan='2' style='background: rgb(252, 253, 219) none repeat scroll 0% 0%; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;'>";
	html += "<img src=\"../imagens/restricao.png\"> <b>A entidade que procura não está na lista?</b> <a href=\"#\" onclick=\"confirmaBuscaReceitaPeloCNPJ($('entnumcpfcnpj').value); closeMessage();\">Clique aqui</a> para consultar a base da Receita Federal.";
	html += "</td></tr>";
	html += "<tr><td colspan='2'>Existe mais de 1(um) registro. Selecione o registro referente:</td></tr>";
	for(i=1;i<entidadesl.length;i++){
		if(entidadesl[i]) {
			dados = entidadesl[i].split("||");
			html += "<tr><td class='SubTituloDireita'><strong>CPF/CNPJ:</strong></td><td>"+dados[2]+"</td>";
			html += "<tr><td class='SubTituloDireita'><strong>Nome:</strong></td><td><a onclick=\"getEntidadePeloEntid('"+dados[0]+"','"+tpe+"');closeMessage(); desabilitaEntnome();\">"+dados[3]+"</a></td></tr>";
			html += "<tr><td class='SubTituloDireita'><strong>Email:</strong></td><td>"+dados[4]+"</td></tr>";
			html += "<tr><td class='SubTituloDireita' nowrap><strong>Telefone Com.:</strong></td><td>("+dados[15]+")"+dados[17]+"</td></tr>";
			var enderecos = dados[37].split("%%");
			if(enderecos[0] != "naoendereco") {
				for(j=0;j<enderecos.length;j++) {
					var dadosendereco = enderecos[j].split("##");
					html += "<tr><td class='SubTituloDireita'><strong>Endereco"+(j+1)+":</strong></td><td>"+dadosendereco[7]+", "+dadosendereco[13]+", "+dadosendereco[2]+", "+dadosendereco[5]+", "+dadosendereco[3]+", Número "+dadosendereco[8]+"</td></tr>";
				}
			}
			html += "<tr><td colspan='2' class='SubTituloCentro'>&nbsp;</td></tr>";
		}
	}
	html += "<tr><td colspan='2' class='SubTituloDireita'><a onclick='closeMessage();'>Fechar</a></td></tr>";
	html += "</table></div>";
	displayStaticMessage(html,"");
}
// Função AJAX que pega a entidade atraves do ID
// e carrega os dados na pessoa física
function getEntidadePeloEntid(entid, tp) {
	if(entid) {
		var myAjax = new Ajax.Request('/geral/consultadadosentidade.php?requisicao=pegarentidadePorentid&entid=' + entid,
			{
				method: 'post',
				asynchronous: false,
				onComplete: function(resp) {
					if(resp.responseText == "naoexiste") {
						return false;
					} else {
						switch(tp) {
							case 'fisica':
								var dados = resp.responseText.split("||");
								carregaDadosEntidadeFisica(dados);
							break;
							case 'juridica':
							default:
								var dados = resp.responseText.split("||");
								carregaDadosEntidadeJuridica(dados);
						}
					}
				}
			});
	}
}
// Função AJAX que pega a entidade atraves do código INEP
function getEntidadePeloEntcodent(entcodent) {
	if(entcodent) {
		displayStaticMessage("<p align=center>Carregando...</p>","");
		var myAjax = new Ajax.Request('/geral/consultadadosentidade.php?requisicao=pegarentidadePorentcodent&entcodent=' + entcodent,
			{
				method: 'post',
				asynchronous: false,
				onComplete: function(resp) {
					if(resp.responseText == "naoexiste") {
						closeMessage();
						return false;
					} else if(resp.responseText.substr(0,12) == "existemuitos") {
						ProcessaMuitasEntidades(resp.responseText, 'juridica');
						return false;
					} else {
						var dados = resp.responseText.split("||");
						carregaDadosEntidadeJuridica(dados);
						closeMessage();
					}
				}
			});
	}
}
// Função AJAX que pega a entidade atraves do código da Unidades
function getEntidadePeloUnicod(entunicod) {
	if(entunicod) {
		displayStaticMessage("<p align=center>Carregando...</p>","");
		var myAjax = new Ajax.Request('/geral/consultadadosentidade.php?requisicao=pegarentidadePorunicod&entunicod=' + entunicod,
			{
				method: 'post',
				asynchronous: false,
				onComplete: function(resp) {
					if(resp.responseText == "naoexiste") {
						closeMessage();
						return false;
					} else if(resp.responseText.substr(0,12) == "existemuitos") {
						ProcessaMuitasEntidades(resp.responseText, 'juridica');
						return false;
					} else {
						var dados = resp.responseText.split("||");
						carregaDadosEntidadeJuridica(dados);
						closeMessage();
					}
				}
			});
	}
}
// Função AJAX que pega a entidade atraves do código da Unidades
function getEntidadePeloEntnome(entnome, tpessoa) {
	if(entnome) {
		displayStaticMessage("<p align=center>Carregando...</p>","");
		var myAjax = new Ajax.Request('/geral/consultadadosentidade.php?requisicao=pegarentidadePorentnome&tpessoa='+tpessoa+'&entnome=' + entnome,
			{
				method: 'post',
				asynchronous: false,
				onComplete: function(resp) {
					document.getElementById('test').innerHTML=resp.responseText;
					if(resp.responseText == "naoexiste") {
						closeMessage();
						return false;
					} else if(resp.responseText.substr(0,12) == "existemuitos") {
						ProcessaMuitasEntidades(resp.responseText, tpessoa);
						return false;
					} else if(resp.responseText == "filtroruim") {
						closeMessage();
						alert("Especifique melhor o nome da entidade, existem mais de 50 opções com este nome.");
						return false;
					} else {
						var dados = resp.responseText.split("||");
						switch(tpessoa) {
							case 'juridica':
								carregaDadosEntidadeJuridica(dados);
								break;
							case 'fisica':
								carregaDadosEntidadeFisica(dados);
								break;
						}
						closeMessage();

					}
				}
			});
	}
}

// Função AJAX que pega a entidade atraves do CPF
function getEntidadePeloCPF(cpf) {
	if(cpf) {
		if(!validar_cpf(cpf)) {
			alert('CPF inválido!');
			return false;		
		}
		displayStaticMessage("<p align=center>Carregando...</p>","");
		var myAjax = new Ajax.Request('/geral/consultadadosentidade.php?requisicao=pegarentidadePorcpfcnpj&entnumcpfcnpj=' + cpf + "&funidEspecifico=" + funidEspecifico,
			{
				method: 'post',
				asynchronous: false,
				onComplete: function(resp) {
					if(resp.responseText == "naoexiste") {
						getUsuarioReceitaPeloCPF(cpf);
						closeMessage();
					} else if(resp.responseText.substr(0,12) == "existemuitos") {
						ProcessaMuitasEntidades(resp.responseText, 'fisica');
						return false;
					} else {
						var dados = resp.responseText.split("||");
						carregaDadosEntidadeFisica(dados);
						closeMessage();
					}
				}
			});
	}
}
// Função que pega os dados do CPF na receita e carrega nos campos referentes a pessoa física
function getUsuarioReceitaPeloCPF(cpf) {
	// limpando todo formulario
	var frm = document.getElementById('frmEntidade');
	
	if(document.getElementById('funcoescadastradas').options.length > 0) {
		for(i=0;i<document.getElementById('funcoescadastradas').options.length;i++) {
			document.getElementById('funcoescadastradas').remove(i);
		}
		var x=document.getElementById("funcoescadastradas");
		x.options[0] = new Option("Não existem funções cadastradas","");
	}
	
	for(i=0;i<frm.elements.length;i++) {
		if(frm.elements[i].id != "entnumcpfcnpj" && 
		   frm.elements[i].id != "funid" &&
		   frm.elements[i].name != "secid" &&
		   frm.elements[i].name != "sbatitulo" &&
		   frm.elements[i].name != "sbaid" &&
		   frm.elements[i].name != "evento" && 
		   frm.elements[i].name != "dsc" && 
		   frm.elements[i].name != "ptrid" &&
		   frm.elements[i].className != "noclear" &&
		   frm.elements[i].id != "entidassociado" &&
		   (frm.elements[i].type == "hidden" ||
		    frm.elements[i].type == "text")) {
		
			frm.elements[i].value = "";
		}
		
	}
	
	var comp = new dCPF();
	comp.buscarDados(cpf);
	document.getElementById('entid').value = "";
	if(comp.dados.no_pessoa_rf) document.getElementById('entnome').value = comp.dados.no_pessoa_rf;
	if(comp.dados.nu_rg) document.getElementById('entnumrg').value = comp.dados.nu_rg;
	if(comp.dados.ds_orgao_expedidor_rg) document.getElementById('entorgaoexpedidor').value = comp.dados.ds_orgao_expedidor_rg;
	if(comp.dados.sg_sexo_rf) document.getElementById('entsexo').value = comp.dados.sg_sexo_rf;
	if(comp.dados.dt_nascimento_rf) document.getElementById('entdatanasc').value = comp.dados.dt_nascimento_rf.substr(6,2)+'/'+comp.dados.dt_nascimento_rf.substr(4,2)+'/'+comp.dados.dt_nascimento_rf.substr(0,4);
	if(comp.dados.ds_contato_pessoa) {
		var tel = comp.dados.ds_contato_pessoa.split('-');
		document.getElementById('entnumdddresidencial').value = tel[0];
		document.getElementById('entnumresidencial').value = tel[1];
		document.getElementById('entnumresidencial').onkeyup();
	}
	if(comp.dados.nu_cep) {
		if(document.getElementById('endcep'+TPEEND_COMERCIAL)) {
			document.getElementById('endcep'+TPEEND_COMERCIAL).value = comp.dados.nu_cep;
			document.getElementById('endcep'+TPEEND_COMERCIAL).onkeyup();
			document.getElementById('endcep'+TPEEND_COMERCIAL).onblur();
		}
		if(document.getElementById('endcep'+TPEEND_RESIDENCIAL)) {
			document.getElementById('endcep'+TPEEND_RESIDENCIAL).value = comp.dados.nu_cep;
			document.getElementById('endcep'+TPEEND_RESIDENCIAL).onkeyup();
			document.getElementById('endcep'+TPEEND_RESIDENCIAL).onblur();
		}

	}
}
// Função AJAX que pega a entidade atraves do CNPJ
function getEntidadePeloCNPJ(cnpj) {
	if(cnpj) {
		if(!validarCnpj(cnpj)) {
			alert('CNPJ inválido!');
			return false;		
		}
		
		var myAjax = new Ajax.Request('/geral/consultadadosentidade.php?requisicao=pegarentidadePorcpfcnpj&entnumcpfcnpj=' + cnpj + "&funidEspecifico=" + funidEspecifico,
			{
				method: 'post',
				asynchronous: false,
				onComplete: function(resp) {
					if(resp.responseText == "naoexiste") {
						$('msg_buscacnpj_receita').style.display = 'none';
						getUsuarioReceitaPeloCNPJ(cnpj);
						desabilitaEntnome();
					} else if(resp.responseText.substr(0,12) == "existemuitos") {
						$('msg_buscacnpj_receita').style.display = 'none';
						ProcessaMuitasEntidades(resp.responseText, 'juridica');
						return false;
					} else {
						var dados = resp.responseText.split("||");
						$('msg_buscacnpj_receita').style.display = '';
						carregaDadosEntidadeJuridica(dados);
						desabilitaEntnome();
					}
				}
			});
	}
}

/**
* Função que pega os dados do CNPJ na receita e carrega nos campos referentes a pessoa jurídica
* Caso ele clique no link para consultar a base da Receita Federal
*/
function confirmaBuscaReceitaPeloCNPJ(cnpj) {
	if(cnpj) {
		if(!validarCnpj(cnpj)) {
			alert('CNPJ inválido!');
			return false;		
		}
		if (confirm('Verifique se há diferença entre a Razão Social e o Nome Fantasia.\nDeseja prosseguir a consulta na base da Receita Federal?')) {
			getUsuarioReceitaPeloCNPJ(cnpj);
			$('msg_buscacnpj_receita').style.display = 'none';
			desabilitaEntnome();
		}
	}
}

// Função que pega os dados do CNPJ na receita e carrega nos campos referentes a pessoa jurídica
function getUsuarioReceitaPeloCNPJ(cnpj) {
	// limpando todo formulario
	var frm = document.getElementById('frmEntidade');
	if(document.getElementById('funcoescadastradas').options.length > 0) {
		for(i=0;i<document.getElementById('funcoescadastradas').options.length;i++) {
			document.getElementById('funcoescadastradas').remove(i);
		}
		var y=document.createElement('option');
		y.text='Não existem funções cadastradas'
		var x=document.getElementById("funcoescadastradas");
		x.add(y,null);
	}
	for(i=0;i<frm.elements.length;i++) {
		if(frm.elements[i].id != "entnumcpfcnpj" && 
		   frm.elements[i].id != "funid" &&
		   frm.elements[i].id != "entidassociado" &&
		   (frm.elements[i].type == "hidden" ||
		    frm.elements[i].type == "text")) {
		
			frm.elements[i].value = "";
		}
	}
	var comp = new dCNPJ();
	comp.buscarDados(cnpj);
	
	var myAjax = new Ajax.Request('/geral/consultadadosentidade.php?requisicao=pegarentidadePorCnpj&entnumcpfcnpj=' + cnpj +'&entnome='+comp.dados.no_igrejarial_rf,
			{
				method: 'post',
				asynchronous: false,
				onComplete: function(resp) {
					if(resp.responseText == "naoexiste") {
						document.getElementById('entnome').value = comp.dados.no_igrejarial_rf;
						document.getElementById('njuid').value = comp.dados.co_natureza_juridica_rf;
						if(comp.dados.nu_cep && document.getElementById('endcep'+TPEEND_COMERCIAL)) {
							document.getElementById('endcep'+TPEEND_COMERCIAL).value = comp.dados.nu_cep;
							document.getElementById('endcep'+TPEEND_COMERCIAL).onkeyup();
							document.getElementById('endcep'+TPEEND_COMERCIAL).onblur();
						}						
					} else {
						var dados = resp.responseText.split("||");
						$('msg_buscacnpj_receita').style.display = '';
						carregaDadosEntidadeJuridica(dados);
						desabilitaEntnome();
					}
					
				}
			});
	
}


// Função que pega os dados do CNPJ na receita e carrega nos campos referentes a pessoa jurídica
function getRazaoSocialReceitaPeloCNPJ(cnpj) {

	var comp = new dCNPJ();
	comp.buscarDados(cnpj);
	
	document.getElementById('entrazaosocial').value = comp.dados.no_igrejarial_rf;
	
}

function desabilitaEntnome(){
	if($('entnome').value){
		$('entnome').readOnly = true;
		$('entnome').className = 'disabled';
		$('entnome').onfocus = "";
		$('entnome').onmouseout = "";
		$('entnome').onblur = "";
		$('entnome').onkeyup = "";
	}
}

function carregarFotosEntidades(params){
	var a = window.open("../includes/fotosEntidades/component_foto_simples.php" + params,"inserir_fotos","scrollbars=yes,height=540,width=630");
	a.focus();
}

function UpdateListFotoSimples(indice) {
	for(i=indice;i<11;i++) {
		if(document.getElementById('imageBox'+i) && document.getElementById('imageBox'+(i+1))) {
			document.getElementById('imageBox'+i).innerHTML = document.getElementById('imageBox'+(i+1)).innerHTML;
		} 
	}
}