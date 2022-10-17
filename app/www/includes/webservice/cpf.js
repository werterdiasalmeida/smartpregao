/**
 * Variável do tipo objeto, instanciada como global
 * 
 * @access public
 * @name objs
 * @var object
 */
var objs = new Object;

/**
 * dCPF classe que estrutura a busca/tratamento dos dados vindo do WebService da Receita Federal;
 * 
 * @author FelipeChiavicatti
 */
function dCPF (){

    /**
     * Atributo que receberá o índice e valor retornado do WebService
     * 
     * @see dCPF()
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
     * @see dCPF()
     * @access public
     * @name dados
     * @var atribute
     * @attribute string no_pessoa_rf "Nome"
	 * @attribute string nu_cpf_rf "Número do CPF"
	 * @attribute string no_mae_rf "Nome da mãe"
	 * @attribute string dt_nascimento_rf "Data de nascimento"
	 * @attribute string sg_sexo_rf "Sexo" M:Masculino | F:Feminino | X:Não informado
	 * @attribute string nu_titulo_eleitor_rf "Número do título de eleitor"
	 * @attribute string st_indicador_residente_ext_rf "Indicativo de residente no exterior" 1:Residente | 2:Não residente
	 * @attribute string co_pais_residente_exterior_rf "Caso seja residente no exterior, o código do país onde reside"
	 * @attribute string no_pais_residente_exterior_rf "Caso seja residente no exterior, o nome do país onde reside"
	 * @attribute string st_cadastro_rf "Indicativo da situação cadastral do CPF" 0:Regular | 1:Cancelada por encerramento | 2:Suspensa | 3:Cancelada por óbito sem espólio | 4:Pendente de regularização | 5:Cancelada por multiplicidade | 8:Nula | 9:Cancelada de ofício 
	 * @attribute string nu_ano_obito_rf "Ano do óbito no formato AAAA"
	 * @attribute string co_natureza_ocupacao_rf "Código da natureza ocupação da pessoa física"
	 * @attribute string co_ocupacao_principal_rf "Código da natureza ocupação da pessoa física" 
	 * @attribute string nu_ano_exercicio_ocupacao_rf "Ano do exercício da informação de natureza de ocupação principal"
	 * @attribute string co_unidade_administrativa_rf "Código da unidade administrativa"
	 * @attribute string dt_inscricao_atualizacao_cpf_rf "Data de inscrição do CPF ou da última operação de atualização no formato AAAAMMDD"
	 * @attribute string nu_rg "Número do documento de identidade da pessoa física"
	 * @attribute string dt_emissao_rg "Data de emissão do RG da pessoa física no formato AAAAMMDD"
	 * @attribute string ds_orgao_expedidor_rg "Nome do órgão emissor do RG da pessoa física"
	 * @attribute string st_indicador_estrangeiro_rf "Indicativo de estrangeiro" 0:Não é estrangeiro | 1:É estrangeiro
	 * @attribute string dt_cadastro "Data e hora em que foi feito o cadastramento da pessoa física no formato AAAAMMDDHHMMSS"
	 * @attribute string co_cidade "Código IBGE do município onde o endereço se encontra"
	 * @attribute string co_tipo_endereco_pessoa "Código do tipo de endereço" 1:Endereço Receita | 2:Endereço Residencial | 3:Endereço Comercial 
	 * @attribute string sg_uf "Sigla da unidade federativa do endereço"
	 * @attribute string ds_localidade "Localidade geográfica dos correios"
	 * @attribute string ds_bairro "Bairro do endereço"
	 * @attribute string ds_logradouro "Logradouro do endereço"
	 * @attribute string ds_logradouro_comp "Complemento do logradouro"
	 * @attribute string ds_numero "Número do endereço"
	 * @attribute string nu_cep "Código de endereçamento postal sem máscara"
	 * @attribute string ds_ponto_referencia "Ponto de referência que sirva de auxílio no endereço"
	 * @attribute string ds_tipo_logradouro "Descrição do tipo de logradouro"
	 * @attribute string co_tipo_contato_pessoa "Tipo de contato" 1:Telefone receita | 2:Telefone Residencial | 3:Telefone comercial | 4:Correio eletrônico | 5:Fax | 6:Telefone celular
	 * @attribute string ds_contato_pessoa "Contém a descrição do contato, como o número de telefone ou e-mail da pessoa física"
     */	
	this.dados = { 
					no_pessoa_rf:					'',
					nu_cpf_rf:						'',
					no_mae_rf:						'',
					dt_nascimento_rf:				'',
					sg_sexo_rf:						'',
					nu_titulo_eleitor_rf:			'',
					st_indicador_estrangeiro_rf:	'',
					co_pais_residente_exterior_rf:	'',
					st_indicador_residente_ext_rf:	'',
					no_pais_residente_exterior_rf:	'',
					st_cadastro_rf:					'',
					nu_ano_obito_rf:				'',
					co_natureza_ocupacao_rf:		'',
					co_ocupacao_principal_rf:		'',
					co_unidade_administrativa_rf:	'',
					nu_ano_exercicio_ocupacao_rf:	'',
					dt_inscricao_atualizacao_cpf_rf:'',
					nu_rg:							'',
					dt_emissao_rg:					'',
					ds_orgao_expedidor_rg:			'',
					dt_cadastro:					'',
					nu_cpf_rf:						'',
					co_cidade:						'',
					co_tipo_endereco_pessoa:		'',
					sg_uf:							'',
					ds_localidade:					'',
					ds_bairro:						'',
					ds_logradouro:					'',
					ds_logradouro_comp:				'',
					ds_numero:						'',
					nu_cep:							'',
					ds_ponto_referencia:			'',
					ds_tipo_logradouro:				'',
					nu_cpf_rf:						'',
					co_tipo_contato_pessoa:			'',
					ds_contato_pessoa:				''				
		 		};
	/**
	 * Faz requisição AJAX ao arquivo PHP que faz a requisição do WebService
	 * 
	 * @param string cpf com máscara ou não;
	 */
	this.buscarDados = function (cpf){
		
										// Faz uma requisição ajax							
										var req = new Ajax.Request('/includes/webservice/cpf.php', {
																	method:      'post',
																	parameters:  '&ajaxCPF=' + cpf,
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
		
											var linhaItem  = [];
										    var colunaItem = []; 
											var indice 	   = [];
											var val		   = [];
										
											linhaItem = text.split('|');		
											for (i=0; i < linhaItem.length; i++){
												colunaItem = linhaItem[i].split('#');											
												indice[i]  = colunaItem[0];
												val[i]     = colunaItem[1];												
											}

											objs.dadosAjax.indice = indice.join('#').split('#');
											objs.dadosAjax.valor  = val.join('#').split('#');

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
												case 'no_pessoa_rf': 
													objs.dados.no_pessoa_rf = ajaxCampoValue[i]; 
													continue; 
													break;
												case 'nu_cpf_rf': 
													objs.dados.nu_cpf_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'no_mae_rf':
													objs.dados.no_mae_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'dt_nascimento_rf':
													objs.dados.dt_nascimento_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'sg_sexo_rf':
													objs.dados.sg_sexo_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_titulo_eleitor_rf':
													objs.dados.nu_titulo_eleitor_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'st_indicador_estrangeiro_rf':
													objs.dados.st_indicador_estrangeiro_rf = ajaxCampoValue[i];
													continue;
													break;	
												case 'co_pais_residente_exterior_rf':
													objs.dados.co_pais_residente_exterior_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'st_indicador_residente_ext_rf':
													objs.dados.st_indicador_residente_ext_rf = ajaxCampoValue[i];
													continue;
													break;												
												case 'st_cadastro_rf':
													objs.dados.st_cadastro_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_ano_obito_rf':
													objs.dados.nu_ano_obito_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'co_natureza_ocupacao_rf':
													objs.dados.co_natureza_ocupacao_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'co_ocupacao_principal_rf':
													objs.dados.co_ocupacao_principal_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'co_unidade_administrativa_rf':
													objs.dados.co_unidade_administrativa_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_ano_exercicio_ocupacao_rf':
													objs.dados.nu_ano_exercicio_ocupacao_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'dt_inscricao_atualizacao_cpf_rf':
													objs.dados.dt_inscricao_atualizacao_cpf_rf = ajaxCampoValue[i];
													continue;
													break;
												case 'nu_rg':
													objs.dados.nu_rg = ajaxCampoValue[i];
													continue;
													break;
												case 'dt_emissao_rg':
													objs.dados.dt_emissao_rg = ajaxCampoValue[i];
													continue;
													break;
												case 'ds_orgao_expedidor_rg':
													objs.dados.ds_orgao_expedidor_rg = ajaxCampoValue[i];
													continue;
													break;
												case 'dt_cadastro':
													objs.dados.dt_cadastro = ajaxCampoValue[i];
													continue;
													break;
												case 'co_cidade':
													objs.dados.co_cidade = ajaxCampoValue[i];
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
													objs.dados.ds_logradouro = ajaxCampoValue[i];
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
												case 'co_tipo_contato_pessoa':
													objs.dados.co_tipo_contato_pessoa = ajaxCampoValue[i];
													continue;
													break;													
												case 'ds_contato_pessoa':
													objs.dados.ds_contato_pessoa = ajaxCampoValue[i];
													continue;
													break;													
											}
																
												
										}
		
									}
    /**
     * Atributo que receberá a instância do objeto "this"
     * 
     * @see dCPF()
     * @access public
     * @name objs
     * @var object
     */
	objs = this;
	
}