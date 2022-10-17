<?php

class Perfil extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.perfil";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("pflcod");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pflcod' => null,
        'pfldsc' => null,
        'pfldatainicio' => null,
        'pfldatafim' => null,
        'pflstatus' => null,
        'pflresponsabilidade' => null,
        'pflsncumulativo' => null,
        'pflfinalidade' => null,
        'pflnivel' => null,
        'pfldescricao' => null,
        'sisid' => null,
        'pflsuperuser' => null,
        'pflsuporte' => null,
        'pflinddelegar' => null,
        'pflpadrao' => null
    );
}