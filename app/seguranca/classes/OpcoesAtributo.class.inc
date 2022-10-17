<?php

class OpcoesAtributo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "formulario.opcoesatributo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "opaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'opaid' => null, 
									  	'atrid' => null, 
									  	'opavalor' => null, 
									  	'opadescricao' => null, 
									  	'opaordem' => null, 
									  	'opavalorrelacionado' => null, 
									  );
}