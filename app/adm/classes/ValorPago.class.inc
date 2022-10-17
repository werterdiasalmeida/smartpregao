<?php

class ValorPago extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_valor_pago";

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
        'estabelecimento_id' => null,
        'profissional_id' => null,
        'arquivo_comprovante_id' => null,
        'descricao' => null,
        'valor_pagamento' => null,
        'data_operacao' => null,
        'responsavel_operacao' => null,
        'excluido' => null
    );

    public function salvarRegistro($params)
    {
        $id = $this->manter($params);
        return $id;
    }
}