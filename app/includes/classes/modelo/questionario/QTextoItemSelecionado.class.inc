<?php
	
class QTextoItemSelecionado extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.textoitemselecionado";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tisid' => null, 
									  	'isuid' => null, 
									  	'tistexto' => null, 
									  	'tisquantescola' => null, 
									  	'tisquantesf' => null, 
									  );
}