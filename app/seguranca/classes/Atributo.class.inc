<?php
	
class Atributo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "formulario.atributo";	

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
									  	'atrid' => null, 
									  	'atrnome' => null, 
									  	'atrdescricao' => null, 
									  	'atrtipodado' => null, 
									  	'tiaid' => null, 
									  	'atrtamanhomax' => null, 
									  	'atrcasasdecimais' => null, 
									  	'atrmascara' => null, 
									  	'atridrelacionado' => null, 
									  	'ratid' => null, 
									  	'atrsqlopcoes' => null, 
									  	'atrstatus' => null, 
									  );
}