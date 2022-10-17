<?php

class UnidadeFederacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.tb_unidade_federacao";

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
        'uf_sigla' => null,
        'uf_nome_estado' => null,
        'uf_codigo_ibge' => null,
        'municipio_ibge_id_capital' => null,
        'regiao_id' => null
    );

    public function getIdBySigla($sigla)
    {
        return $this->recuperarUm('id', array("uf_sigla = '{$sigla}'"));
    }

    public function getCodIbgeBySigla($sigla)
    {
        return $this->recuperarUm('uf_codigo_ibge', array("uf_sigla = '{$sigla}'"));
    }

    public function getSiglaByCodIbge ($codIbge)
    {
        return $this->recuperarUm('uf_sigla', array("uf_codigo_ibge = '{$codIbge}'"));
    }
}