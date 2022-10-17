<?php
	
class VinculoFuncional extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.vinculofuncional";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "vifid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'vifid' 		=> null, 
									  	'vifdsc' 		=> null, 
									  	'vifstatus'	    => null
									  );
}
  