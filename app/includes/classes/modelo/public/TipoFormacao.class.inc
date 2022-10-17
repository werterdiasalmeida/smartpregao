<?php
	
class TipoFormacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "public.tipoformacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tfoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tfoid' => null, 
									  	'tfodsc' => null, 
									  	'tfostatus' => null, 
									  );
}