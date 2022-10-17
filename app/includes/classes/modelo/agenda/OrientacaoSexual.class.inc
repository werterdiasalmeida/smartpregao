<?php
	
class OrientacaoSexual extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.orientacaosexual";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "orsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'orsid' 			 => null, 
									  	'orsdsc' 			 => null, 
									  	'orsstatus' 		 => null
									  );
	
}
