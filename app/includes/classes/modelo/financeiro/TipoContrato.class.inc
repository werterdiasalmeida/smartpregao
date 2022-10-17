<?php
class TipoContrato extends Modelo {
    const TIPO_POR_VALOR_FIXO = '1';
    const TIPO_POR_ATENDIMENTO_REALIZADO = '2';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "financeiro.tb_tipo_contrato";

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
		'nome' => null,
		'valor' => null,
		'criado_em' => null,
		'criado_por' => null,
		'atualizado_em' => null,
		'atualizado_por' => null,
		'excluido' => null
	);
}

