<?php
	
class OrgaoOrigem extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.orgaoorigem";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "oroid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'oroid' 		=> null, 
									  	'orodsc' 		=> null, 
									  	'orostatus'	    => null
									  );
}
  