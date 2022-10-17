<?php

class TipoContato extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.tb_tipo_contato";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("id");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'id' => null,
        'sigla' => null,
        'descricao' => null,
        'observacao' => null
    );

    const TIPO_CELULAR = 'CL';
    const TIPO_TELEFONE_FIXO = 'TF';
    const TIPO_EMAIL = 'EM';

    public function getIdPorSigla($sigla)
    {
        return $this->recuperarUm('id', array("sigla = '{$sigla}'"));
    }
}