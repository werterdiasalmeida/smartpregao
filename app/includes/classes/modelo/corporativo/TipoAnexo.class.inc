<?php

class TipoAnexo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.tb_tipo_anexo";

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
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    const TIPO_CPF = 'CPF';
    const TIPO_CNS = 'CNS';
    const TIPO_COMP_RESIDENCIA = 'CRES';

    public function getIdBySigla($sigla)
    {
        return $this->recuperarUm('id', array("sigla = '{$sigla}'", "excluido IS FALSE"));
    }
}