<?php
class DispositivoNotificacaoApp extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "app.dispositivonotificacaoapp";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("dinid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
		'dinid'	        => null,
		'disid'	        => null,
		'naeid'	        => null,
		'dindtrecebida'	=> null,
		'diniprecebida'	=> null,
		'dindtlida'	    => null,
		'diniplida'	    => null
	);
}

