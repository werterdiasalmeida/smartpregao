<?php
class PeriodoContrato extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "financeiro.tb_periodo_contrato";

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
		'quantidade_parcelas' => null,
		'criado_em' => null,
		'criado_por' => null,
		'atualizado_em' => null,
		'atualizado_por' => null,
		'excluido' => null
	);
}

