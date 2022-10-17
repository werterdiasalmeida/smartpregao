<?php
	
class BensMateriais extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.bensmateriais";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "bmaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'bmaid' 		=> null, 
									  	'bmadsc' 		=> null, 
									  	'bmastatus'	    => null
									  );

									  
}