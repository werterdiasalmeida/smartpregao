<?php

class TipoProposto extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "administrativo.tipoproposto";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tppid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'tppid' 		   	=> null,
									  	'tppdsc'        	=> null,
									  	'tppstatus'         => null
									  );
}
