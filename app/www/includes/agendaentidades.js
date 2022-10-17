function Entidade()
{
	var atributos = ['entcep', 
					 'estuf',
					 'muncod',
					 'entlog',
					 'entcomplemento',
					 'entbairro',
					 'entnumero'];
	
	this.buscarEnderecoCEP = function (cep)
	{
		divCarregando();
		$.ajax({url		 : '/geral/dne.php?opt=dne&endcep=' + cep,
//				data	 : {cep},
			    type 	 : "POST",
			    async	 : false,
//			    dataType : 'json',
			    success  : function(data)
			    	{
						// Return DNE
						eval(data);
						if (typeof DNE[0] == 'object'){
										//    FORM	 	 	Ajax
							var dadosRefe = {'estuf' 	 : 'estado',
										 	 'cidade' 	 : 'cidade',
										 	 'muncod' 	 : 'muncod',
										 	 'entlog' 	 : 'logradouro',
										 	 'entbairro' : 'bairro'};
							atribuirForm(DNE[0], dadosRefe);
							divCarregado();
						}
					}
			});
	}
	
	this.buscarEntidadePorCPF = function (cpf)
	{
		cpf = cpf || '';
		if (cpf){
			if(!validar_cpf(cpf)) {
				alert('CPF inválido!');
				return false;		
			}
			divCarregando();
			$.ajax({url		 : '/geral/entidade/consultadadosentidade.php',
					data	 : {requisicao:'buscarEntidadePorCPF', entcpfcnpj:cpf},
				    type 	 : "POST",
				    async	 : false,
				    success  : function(data)
				    	{
//							document.body.innerHTML = data;
//							return;
//							alert(data);
							var data = eval('('+data+');');
							var dadosRefe = {"entid":"entid",
											 "tpeid":"tpeid",
											 "entidpai":"entidpai",
											 "estuf":"estuf",
											 "muncod":"muncod",
											 "arqid":"arqid",
											 "arqidRefe":"arqid",
//											 "entcpfcnpj":"entcpfcnpj",
											 "entnome":"entnome",
											 "entstatus":"entstatus",
											 "entcep":"entcep",
											 "entlog":"entlog",
											 "entcomplemento":"entcomplemento",
											 "entbairro":"entbairro",
											 "entnumero":"entnumero",
											 
											 "entnomesocial":"entnomesocial", 
											 "escid":"escid", 
											 "orsid":"orsid", 
											 "idgid":"idgid", 
											 "corid":"corid",
											 
											 "graulatitude":"graulatitude",
											 "minlatitude":"minlatitude",
											 "seglatitude":"seglatitude",
											 "pololatitude":"pololatitude",
											 
											 "graulongitude":"graulongitude",
											 "minlongitude":"minlongitude",
											 "seglongitude":"seglongitude",
											 
											 "modlongitude":"modlongitude",
											 "entzoom":"entzoom",
											 "entdtnascimento":"entdtnascimento",
											 "entrg":"entrg",
											 "entorgaoemissorrg":"entorgaoemissorrg",
											 "entdtemissaorg":"entdtemissaorg",
											 "enttituloeleitor":"enttituloeleitor",
											 "entfax":"entfax",
											 "enttelfone":"enttelfone",
											 "entcelular":"entcelular",
											 "entramal":"entramal",
											 "entemail":"entemail",
											 "entemail_confirmacao":"entemail",
											 "entobservacao":"entobservacao",
											 "entnomemae":"entnomemae",
											 "entnomepai":"entnomepai",
											 "cidade":"cidade",
											 
											 
											 "entsiape":"entsiape",
											 "vifid":"vifid",
											 "cefid":"cefid",
											 "oroid":"oroid",
											 "entpispasep":"entpispasep",
											 "entgraduacao":"entgraduacao"
											 };
							
							if(data.entsiape)
								$('#tr_siape').show();
							else
								$('#tr_siape').hide();
							
							if(data.cefid)
								$('#tr_cargo').show();
							else
								$('#tr_cargo').hide();
							
							if(data.oroid)
								$('#tr_origem').show();
							else
								$('#tr_origem').hide();
							
							atribuirForm(data, dadosRefe);
							montaImagem(data.arqid);
							
							divCarregado();
						}
				});			
		}
	}
	
	this.buscarEntidadePorCNPJ = function (cnpj)
	{
		cnpj = cnpj || '';
		if (cnpj){
			if(!validarCnpj(cnpj)) {
				alert('CNPJ inválido!');
				return false;		
			}
			divCarregando();
			$.ajax({url		 : '/geral/entidade/consultadadosentidade.php',
				data	 : {requisicao:'buscarEntidadePorCNPJ', entcpfcnpj:cnpj},
				type 	 : "POST",
				async	 : false,
				success  : function(data)
				{
//							document.body.innerHTML = data;
//							return;
//							alert(data);
					var data = eval('('+data+');');
					var dadosRefe = {	"entid":"entid",
										"tpeid":"tpeid",
										"entidpai":"entidpai",
										"estuf":"estuf",
										"muncod":"muncod",
			//		   				    "entcpfcnpj":"entcpfcnpj",
										"entnome":"entnome",
										"entstatus":"entstatus",
										"entcep":"entcep",
										"entlog":"entlog",
										"entcomplemento":"entcomplemento",
										"entbairro":"entbairro",
										"entnumero":"entnumero",
										
										"graulatitude":"graulatitude",
										"minlatitude":"minlatitude",
										"seglatitude":"seglatitude",
										"pololatitude":"pololatitude",
										
										"graulongitude":"graulongitude",
										"minlongitude":"minlongitude",
										"seglongitude":"seglongitude",
										
										"modlongitude":"modlongitude",
										"entzoom":"entzoom",
										"entdtnascimento":"entdtnascimento",
										"entrg":"entrg",
										"entorgaoemissorrg":"entorgaoemissorrg",
										"entdtemissaorg":"entdtemissaorg",
										"enttituloeleitor":"enttituloeleitor",
										"entfax":"entfax",
										"enttelfone":"enttelfone",
										"entcelular":"entcelular",
										"entramal":"entramal",
										"entemail":"entemail",
										"entemail_confirmacao":"entemail",
										"entobservacao":"entobservacao",
										"entnomemae":"entnomemae",
										"entnomepai":"entnomepai",
										"cidade":"cidade"};
					
					atribuirForm(data, dadosRefe);
					divCarregado();
				}
			});			
		}
	}
	
	/*
	 * MÉTODOS DE POPUP - INÍCIO
	 */
	this.popup = {
			abrePopUpEntidade : function (entid )
				{
					var param = '';
					if (entid){
						param = '&entid='+entid;
					}
					return windowOpen('?modulo=principal/popup/cadEntidade&acao=C&noheader=true'+param, 
									  'popupentidade', 
									  'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
				},
			atualizaInformacaoPai : function (dados)
				{
					if(window.opener.$('#entidcontato')[0]){
						window.opener.$('#entidcontato').val(dados.entid);
						window.opener.$('#entidcontato_dsc').val(dados.msg);
						window.opener.$('#entidcontato_retorno').val(dados.msg);
					}else{
						window.opener.$('#entidpessoa').val(dados.entid);
	//					window.opener.$('#texto_entidade').html(dados.msg);
						window.opener.$('#petemail').val(dados.entemail);
						window.opener.$('#pettelefone').val(dados.enttelfone);
						window.opener.$('#petramal').val(dados.entramal);
					}	
					window.opener.$('#idiid').val(dados.entid);
//					window.close();
				} 
	}
	/*
	 * MÉTODOS DE POPUP - FIM
	 */
	
//	this.abrePopUpEntidade = function (entid ){
//		var param = '';
//		if (entid){
//			param = '&entid='+entid;
//		}
//		return windowOpen('?modulo=principal/popup/cadEntidade&acao=A'+param, '_blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
////		else{
////				return windowOpen( caminho_atual + '?modulo=principal/inserir_entidade&acao=A&orgid=' + orgid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
////		}
//	}

	function montaImagem(arqid)
	{
		$('#PhotoPrev').attr('src', '../slideshow/slideshow/verimagem.php?newwidth=64&newheight=48&arqid=' + arqid)
		   			   .css({visibility:'visible'}); 
	}
	
	function atribuirForm(dado, refeParam)
	{
		if (typeof refeParam == 'object'){
			$.each(refeParam, function (nomeForm, nomeRefe)
				{
//					alert(dado[nomeRefe]);
					$('[name='+nomeForm+']').val(dado[nomeRefe] || "");
				});
		}else{
			$.each(dado, function (nomeForm, valor)
				{
//					alert(nomeForm+'\n'+valor);
					$('[name='+nomeForm+']').val(valor || "");
				});
		}
	}
	
}	
	
