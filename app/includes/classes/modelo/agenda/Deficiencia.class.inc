<?php
	
class Deficiencia extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.deficiencia";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "defid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'defid' 	=> null, 
									  	'tdeid' 	=> null, 
									  	'defdsc' 	=> null,
    									'defstatus' => null
									  );
}
