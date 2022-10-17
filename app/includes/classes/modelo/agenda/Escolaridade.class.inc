<?php
	
class Escolaridade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.escolaridade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "escid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'escid' 			 => null, 
									  	'escdsc' 			 => null, 
									  	'escstatus' 		 => null
									  );
	
}
