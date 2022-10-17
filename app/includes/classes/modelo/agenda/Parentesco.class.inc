<?php
	
class Parentesco extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.parentesco";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "parid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'parid' 			 => null, 
									  	'pardsc' 			 => null, 
									  	'parstatus' 		 => null
									  );
	
}
