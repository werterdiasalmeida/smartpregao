<?php
class Aplicativo extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "app.aplicativo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("appid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
		'appid' => null,
		'appnome' => null,
		'appdescricao' => null,
		'appstatus' => null,
		'appdtcadastro' => null,
		'usucpf' => null
	);
}

