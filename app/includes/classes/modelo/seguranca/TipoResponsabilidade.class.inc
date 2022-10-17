<?php

class TipoResponsabilidade extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.tiporesponsabilidade";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("tprcod");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'tprcod' => null,
        'tprdsc' => null,
        'tprsnvisivelperfil' => null,
        'tprsigla' => null,
        'tprurl' => null,
        'sisid' => null
    );
}