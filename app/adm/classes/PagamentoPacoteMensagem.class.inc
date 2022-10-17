<?php

class PagamentoPacoteMensagem extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_pagamento_pacote_mensagem";

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
        'id'                    => null,
        'fatura_id'             => null,
        'criado_em'             => null,
        'criado_por'            => null,
        'atualizado_em'         => null,
        'atualizado_por'        => null,
        'excluido'              => null
    );
}