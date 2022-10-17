<?php
	
class IdentidadeGenero extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.identidadegenero";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "idgid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'idgid' 			 => null, 
									  	'idgdsc' 			 => null, 
									  	'idgstatus' 		 => null
									  );
	
}
