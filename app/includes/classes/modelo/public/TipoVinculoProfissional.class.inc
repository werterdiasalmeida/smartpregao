<?php
	
class TipoVinculoProfissional extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "public.tipovinculoprofissional";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tvpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tvpid' => null, 
									  	'tvpdsc' => null, 
									  	'tvpstatus' => null, 
									  );
}