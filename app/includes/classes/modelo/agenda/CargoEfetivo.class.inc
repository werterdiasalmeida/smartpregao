<?php
	
class CargoEfetivo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.cargoefetivo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "cefid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'cefid' 		=> null, 
									  	'cefdsc' 		=> null, 
									  	'cefstatus'	    => null
									  );
}
  