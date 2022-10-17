/**
 * Variável do tipo objeto, instanciada como global
 * 
 * @access public
 * @name objs
 * @var object
 */
var objs = new Object;

/**
 * dCNPJ classe que estrutura a busca/tratamento dos dados vindo do WebService da Receita Federal;
 * 
 * @author FelipeChiavicatti
 */
function dCNPJ (){

    /**
     * Atributo que receberá o índice e valor retornado do WebService
     * 
     * @see dCNPJ()
     * @access public
     * @name dadosAjax
     * @var atribute
     * @attribute array indice
     * @attribute array valor
     */
	this.dadosAjax = {
						indice : new Array(), 
						valor  : new Array()
				  	 };	

    /**
     * Atributo que receberá o índice e valor retornado do WebService
     * 
     * @see dCNPJ()
     * @access public
     * @name dados
     * @var atribute 
     * @attribute string nu_cnpj_rf "Contém o número de inscrição no CNPJ (cadastro nacional de pessoa jurídica)"
     * @attribute string tp_estabelecimento_rf "Indicador de matriz/filial" 1:Matriz | 2:Filial
     * @attribute string no_igrejarial_rf "Corresponde ao nome da razão social e/ou nome igrejarial da pessoa jurídica"
     * @attribute string no_fantasia_rf "Corresponde ao nome fantasia"
     * @attribute string st_cadastral_rf "Situação cadastral do CNPJ" 1:Nula | 2:Ativa | 3:Suspensa | 4:Inapta | 8:Baixada
     * @attribute string dt_st_cadastral_rf "Data da situação cadastral do CNPJ, formato de retorno é AAAAMMDD"
     * @attribute string no_cidade_exterior_rf "A cidade no exterior, caso o estabelecimento seja domiciliado no exterior"
     * @attribute string co_codigo_pais_rf "Código do país, caso o estabelecimento seja domiciliado no exterior"
     * @attribute string no_pais_rf "Nome do país, caso o estabelecimento seja domiciliado no exterior"
     * @attribute string co_natureza_juridica_rf "O código da natureza jurídica do estabelecimento consultado"
     * @attribute string dt_abertura_rf "Data abertura CNPJ (início das atividades do estabelecimento)"
     * @attribute string co_cnae_principal_rf "Descrição do código da atividade econômica principal do estabelecimento"
     * @attribute string nu_cpf_responsavel_rf "O CPF do responsável pelo estabelecimento"
     * @attribute string no_responsavel_rf "Nome da pessoa física responsável pela igreja perante o ministério da fazenda"
     * @attribute string nu_capital_social_rf "Capital social da igreja"
     * @attribute string tp_crc_contador_pj_rf "Código do tipo de CRC do contador pessoa jurídica" O:Originário | T:Transferido | S:Secundário | P:Provisório | F:Filiais
     * @attribute string nu_classificacao_crc_contador_pj_rf "Número de classificação do CRC do contador pessoa jurídica" 1:Profissional | 2:Escritório Sociedade | 3:Escritório Individual
     * @attribute string nu_crc_contador_pj_rf "Número do CRC do contador pessoa jurídica"
     * @attribute string sg_uf_crc_contador_pj_rf "UF CRC do contador pessoa jurídica"
     * @attribute string nu_cnpj_contador_rf "Número do CNPJ do contador"
     * @attribute string ds_porte_rf "O porte do estabelecimento consultado" 1:Microigreja | 2:Igreja de pequeno porte | 5:demais
     * @attribute string ds_opcao_simples_rf "Indicador de opção pelo simples" S:Sim | N:Não
     * @attribute string dt_opcao_simples_rf "Data opção simples (INCLUSÃO)"
     * @attribute string dt_exclusao_simples_rf "Data opção simples (Exclusão)"
     * @attribute string nu_cnpj_sucedida_rf "Número do CNPJ da sucedida. Este campo será sempre preenchido com o CNPJ consultado"
     * @attribute string dt_cadastro "Data do cadastro"
     * @attribute string CNAESecundario "Armazena as informações do CNAE (classificação Nacional de Atividades Econômicas) secundário, até 10 ocorrências do estabelecimento consultado"
     * @attribute string CNPJSucessora "Número dos CNPJ's das sucessoras caso haja uma operação de sucessão. Ocorre até 8 vezes"
     * @attribute string co_cidade "Código IBGE do município onde o endereço se encontra"
     * @attribute string co_tipo_endereco_pessoa "Código do tipo de endereço" 1:Endereço Receita | 2:Endereço Residencial | 3:Endereço Comercial
     * @attribute string sg_uf "Sigla da unidade federativa do endereço"
     * @attribute string ds_localidade "Localidade geográfica dos correios"
     * @attribute string ds_bairro "Bairro endereço"
     * @attribute string ds_logradouro "Logradouro endereço"
     * @attribute string ds_logradouro_comp "Complemento do logradouro"
     * @attribute string ds_numero "Número do endereço"
     * @attribute string nu_cep "Código de endereçamento postal, sem máscara"
     * @attribute string ds_ponto_referencia "Ponto de referência que sirva de auxílio no endereço"
     * @attribute string ds_tipo_logradouro "Descrição do tipo de logradouro"
     * @attribute array|string nu_socio_rf "Número do CNPJ ou CPF do sócio do estabelecimento consultado, sendo que, se o sócio for estrangeiro, este campo será 9999999999999"
     * @attribute array|string tp_socio_rf "Indicador do tipo de sócio" 1:Sócio pessoa júridica | 2:Sócio pessoa física | 3:Sócio estrangeiro
     * @attribute array|string no_socio_rf "Corresponde ao nome do sócio pessoa física, razão social e/ou nome igrejarial da pessoa jurídica e nome do sócio/razão social do sócio estrangeiro"
     * @attribute array|string nu_percentual_participacao_rf "Percentual de participação de um sócio na igreja"
     * @attribute array|string co_pais_origem_rf "Código do país do sócio estrangeiro"
     * @attribute array|string no_pais_origem_rf "Corresponde ao nome do país do sócio"
     */	
	this.dados = { 
					nu_cnpj_rf:				 			 '',
					tp_estabelecimento_rf:				 '',
					no_igrejarial_rf: 		 			 '',
					no_fantasia_rf: 		 			 '',
					st_cadastral_rf: 		 			 '',
					dt_st_cadastral_rf: 	 			 '',
					no_cidade_exterior_rf:   			 '',
					co_codigo_pais_rf: 		 			 '',
					no_pais_rf: 			 			 '',
					co_natureza_juridica_rf: 			 '',
					dt_abertura_rf: 					 '',
					co_cnae_principal_rf: 				 '',
					nu_cpf_responsavel_rf: 				 '',
					no_responsavel_rf: 					 '',
					nu_capital_social_rf: 				 '',
					tp_crc_contador_pj_rf: 				 '',
					nu_classificacao_crc_contador_pj_rf: '',
					nu_crc_contador_pj_rf: 				 '',
					sg_uf_crc_contador_pj_rf: 			 '',
					nu_cnpj_contador_rf: 				 '',
					tp_crc_contador_pf_rf: 				 '',
					nu_classificacao_crc_contador_pf_rf: '',
					nu_crc_contador_pf_rf: 				 '',
					sg_uf_crc_contador_pf_rf: 			 '',
					nu_cpf_contador_rf: 				 '',
					ds_porte_rf: 						 '',
					ds_opcao_simples_rf: 				 '',
					dt_opcao_simples_rf: 				 '',
					dt_exclusao_simples_rf: 			 '',
					nu_cnpj_sucedida_rf: 				 '',
					dt_cadastro: 						 '',
					CNAESecundario: 					 '',
					CNPJSucessora: 						 '',
					nu_cnpj_rf: 						 '',
					co_cidade: 							 '',
					co_tipo_endereco_pessoa: 			 '',
					sg_uf: 								 '',
					ds_localidade: 						 '',
					ds_bairro: 							 '',
					ds_logradouro:  					 '',
					ds_logradouro_comp: 				 '',
					ds_numero: 							 '',
					nu_cep: 							 '',
					ds_ponto_referencia: 				 '',
					ds_tipo_logradouro: 				 '',
					nu_socio_rf: 						 [],
					tp_socio_rf: 						 [],
					no_socio_rf:						 [],
					nu_percentual_participacao_rf:		 [],
					co_pais_origem_rf: 					 [],
					no_pais_origem_rf: 					 []			
		 		};
	/**
	 * Faz requisição AJAX ao arquivo PHP que faz a requisição do WebService
	 * 
	 * @param string cpf com máscara ou não;
	 */
	this.buscarDados = function (cnpj){

										// Faz uma requisição ajax							
										var req = new Ajax.Request('/includes/webservice/pj.php', {
																	method:      'post',
																	parameters:  '&ajaxPJ=' + cnpj,
																	asynchronous: false,
																	onComplete:   function (res)
																	{	
		
										        							if (res.responseText) {
										        								objs.formatarDados(res.responseText);
										        							}	
										        								  
																	  }
																});
											
									  }




	


	/**
	 * Formata os dados passados no parametro e seta nos atributos da classe "dadosAjax.indice e dadosAjax.valor"
	 * 
	 * @param string text no formato " indice#valor| ";
	 */
	this.formatarDados = function (text){
		
											var tipoItem    = [];
											var linhaItem   = [];
										    var linhaSocio  = [];
										    var colunaItem  = [];
										    var colunaSocio = [];
										
											tipoItem   = text.split('$$');	
											linhaItem  = tipoItem[0].split('|');
											linhaSocio = tipoItem[1].split('|');
											
											for (i=0; i < linhaItem.length; i++){
												colunaItem = linhaItem[i].split('#');
												objs.dadosAjax.indice[i] = colunaItem[0];
												objs.dadosAjax.valor[i]  = colunaItem[1];												
											}
	
											for (a=0; a < linhaSocio.length; a++){
												colunaSocio = linhaSocio[a].split('#');
												
												switch (colunaSocio[0]) {	
													case 'nu_socio_rf':
														objs.dadosAjax.indice[i+1] = colunaSocio[0];
														objs.dadosAjax.valor[i+1]  = objs.dadosAjax.valor[i+1] ? objs.dadosAjax.valor[i+1] +'#'+ colunaSocio[1] : colunaSocio[1];
														continue;
														break;	
													case 'tp_socio_rf':
														objs.dadosAjax.indice[i+2] = colunaSocio[0];
														objs.dadosAjax.valor[i+2]  = objs.dadosAjax.valor[i+2] ? objs.dadosAjax.valor[i+2] +'#'+ colunaSocio[1] : colunaSocio[1];
														continue;
														break;	
													case 'no_socio_rf':
														objs.dadosAjax.indice[i+3] = colunaSocio[0];
														objs.dadosAjax.valor[i+3]  = objs.dadosAjax.valor[i+3] ? objs.dadosAjax.valor[i+3] +'#'+ colunaSocio[1] : colunaSocio[1];
														continue;
														break;	
													case 'nu_percentual_participacao_rf':
														objs.dadosAjax.indice[i+4] = colunaSocio[0];
														objs.dadosAjax.valor[i+4]  = objs.dadosAjax.valor[i+4] ? objs.dadosAjax.valor[i+4] +'#'+ colunaSocio[1] : colunaSocio[1];
														continue;
														break;	
													case 'co_pais_origem_rf':
														objs.dadosAjax.indice[i+5] = colunaSocio[0];
														objs.dadosAjax.valor[i+5]  = objs.dadosAjax.valor[i+5] ? objs.dadosAjax.valor[i+5] +'#'+ colunaSocio[1] : colunaSocio[1];
														continue;
														break;	
													case 'no_pais_origem_rf':
														objs.dadosAjax.indice[i+6] = colunaSocio[0];
														objs.dadosAjax.valor[i+6]  = objs.dadosAjax.valor[i+6] ? objs.dadosAjax.valor[i+6] +'#'+ colunaSocio[1] : colunaSocio[1];
														continue;
														break;	
												}
												
											}
											
											objs.carregarDados();
											
										}



	/**
	 * Seta nos atributos da classe os valores, relacionando índices e valores;
	 */
	this.carregarDados = function () {
										var ajaxCampoName  = objs.dadosAjax.indice;
										var ajaxCampoValue = objs.dadosAjax.valor;
													
										for (i=0; i < ajaxCampoName.length; i++){
											switch (ajaxCampoName[i]) {
												case 'nu_cnpj_rf': 
													objs.dados.nu_cnpj_rf = ajaxCampoValue[i]; 
													continue; 
													break;
												case 'tp_estabelecimento_rf': 
													objs.dados.tp_estabelecimento_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'no_igrejarial_rf':
													objs.dados.no_igrejarial_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'no_fantasia_rf':
													objs.dados.no_fantasia_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'st_cadastral_rf':
													objs.dados.st_cadastral_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'no_cidade_exterior_rf':
													objs.dados.no_cidade_exterior_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'co_codigo_pais_rf':
													objs.dados.co_codigo_pais_rf = ajaxCampoValue[i];
													continue;
													break;	
												case 'no_pais_rf':
													objs.dados.no_pais_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'co_natureza_juridica_rf':
													objs.dados.co_natureza_juridica_rf = ajaxCampoValue[i];
													continue;
													break;												
												case 'dt_abertura_rf':
													objs.dados.dt_abertura_rf  = ajaxCampoValue[i];
													continue;
													break;
												case 'co_cnae_principal_rf':
													objs.dados.co_cnae_principal_rf  = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_cpf_responsavel_rf':
													objs.dados.nu_cpf_responsavel_rf  = ajaxCampoValue[i];
													continue;
													break;
												case 'no_responsavel_rf':
													objs.dados.no_responsavel_rf  = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_capital_social_rf':
													objs.dados.nu_capital_social_rf  = ajaxCampoValue[i];
													continue;
													break;
												case 'tp_crc_contador_pj_rf':
													objs.dados.tp_crc_contador_pj_rf  = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_classificacao_crc_contador_pj_rf':
													objs.dados.nu_classificacao_crc_contador_pj_rf  = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_crc_contador_pj_rf':
													objs.dados.nu_crc_contador_pj_rf  = ajaxCampoValue[i];
													continue;
													break;
												case 'sg_uf_crc_contador_pj_rf':
													objs.dados.sg_uf_crc_contador_pj_rf  = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_cnpj_contador_rf':
													objs.dados.nu_cnpj_contador_rf  = ajaxCampoValue[i];
													continue;
													break;
												case 'tp_crc_contador_pf_rf':
													objs.dados.tp_crc_contador_pf_rf  = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_classificacao_crc_contador_pf_rf':
													objs.dados.nu_classificacao_crc_contador_pf_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_crc_contador_pf_rf':
													objs.dados.nu_crc_contador_pf_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'sg_uf_crc_contador_pf_rf':
													objs.dados.sg_uf_crc_contador_pf_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_cpf_contador_rf':
													objs.dados.nu_cpf_contador_rf = ajaxCampoValue[i];
													continue;
													break;													
												case 'ds_porte_rf':
													objs.dados.ds_porte_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'ds_opcao_simples_rf':
													objs.dados.ds_opcao_simples_rf  = ajaxCampoValue[i];
													continue;
													break;													
												case 'dt_opcao_simples_rf':
													objs.dados.dt_opcao_simples_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'dt_exclusao_simples_rf':
													objs.dados.dt_exclusao_simples_rf = ajaxCampoValue[i];
													continue;
													break;													
												case 'dt_exclusao_simples_rf':
													objs.dados.dt_exclusao_simples_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_cnpj_sucedida_rf':
													objs.dados.nu_cnpj_sucedida_rf = ajaxCampoValue[i];
													continue;
													break;													
												case 'dt_cadastro':
													objs.dados.dt_cadastro = ajaxCampoValue[i];
													continue;
													break;													
												case 'CNAESecundario':
													objs.dados.CNAESecundario = ajaxCampoValue[i];
													continue;
													break;													
												case 'CNPJSucessora':
													objs.dados.CNPJSucessora  = ajaxCampoValue[i];
													continue;
													break;													
												case 'co_cidade':
													objs.dados.co_cidade  = ajaxCampoValue[i];
													continue;
													break;	
												case 'co_tipo_endereco_pessoa':
													objs.dados.co_tipo_endereco_pessoa = ajaxCampoValue[i];
													continue;
													break;	
												case 'sg_uf':
													objs.dados.sg_uf = ajaxCampoValue[i];
													continue;
													break;	
												case 'ds_localidade':
													objs.dados.ds_localidade = ajaxCampoValue[i];
													continue;
													break;	
												case 'ds_bairro':
													objs.dados.ds_bairro = ajaxCampoValue[i];
													continue;
													break;	
												case 'ds_logradouro':
													objs.dados.ds_logradouro  = ajaxCampoValue[i];
													continue;
													break;	
												case 'ds_logradouro_comp':
													objs.dados.ds_logradouro_comp = ajaxCampoValue[i];
													continue;
													break;	
												case 'ds_numero':
													objs.dados.ds_numero = ajaxCampoValue[i];
													continue;
													break;	
												case 'nu_cep':
													objs.dados.nu_cep = ajaxCampoValue[i];
													continue;
													break;	
												case 'ds_ponto_referencia':
													objs.dados.ds_ponto_referencia = ajaxCampoValue[i];
													continue;
													break;	
												case 'ds_tipo_logradouro':
													objs.dados.ds_tipo_logradouro = ajaxCampoValue[i];
													continue;
													break;	
												case 'nu_socio_rf':
													objs.dados.nu_socio_rf = ajaxCampoValue[i].split('#');
													continue;
													break;	
												case 'tp_socio_rf':
													objs.dados.tp_socio_rf = ajaxCampoValue[i].split('#');
													continue;
													break;	
												case 'no_socio_rf':
													objs.dados.no_socio_rf = ajaxCampoValue[i].split('#');
													continue;
													break;	
												case 'nu_percentual_participacao_rf':
													objs.dados.nu_percentual_participacao_rf = ajaxCampoValue[i].split('#');
													continue;
													break;	
												case 'co_pais_origem_rf':
													objs.dados.co_pais_origem_rf = ajaxCampoValue[i].split('#');
													continue;
													break;	
												case 'no_pais_origem_rf':
													objs.dados.no_pais_origem_rf = ajaxCampoValue[i].split('#');
													continue;
													break;	
											}
																
												
										}
		
									}
    /**
     * Atributo que receberá a instância do objeto "this"
     * 
     * @see dCNPJ()
     * @access public
     * @name objs
     * @var object
     */
	objs = this;
	
}