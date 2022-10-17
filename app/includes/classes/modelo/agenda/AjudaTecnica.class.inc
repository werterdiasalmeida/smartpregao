<?php
	
class AjudaTecnica extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.ajudatecnica";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ajtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ajtid' 		=> null, 
									  	'ajtdsc' 		=> null, 
									  	'ajtstatus'	    => null
									  );
}
