<?php
	
class DadosEmergencia extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.dadosemergencia";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "demid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'demid' 			 => null, 
									  	'demtiposanguinio' 	 => null, 
									  	'demrh' 	 		 => null, 
									  	'demobservacao' 	 => null, 
									  	'dempossuiplano' 	 => null, 
									  	'demplanosaude' 	 => null, 
									  	'demnomecontato1' 	 => null, 
									  	'demtelefone1' 	 	 => null, 
									  	'parid1' 	 		 => null, 
									  	'demnomecontato2' 	 => null, 
									  	'demtelefone2' 	 	 => null, 
									  	'parid2' 	 		 => null, 
									  	'entid' 		 	 => null
									  );
									  
}