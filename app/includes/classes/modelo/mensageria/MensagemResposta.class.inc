<?php
class MensagemResposta extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "mensageria.tb_mensagem_resposta";

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
		'mensagem_id'           => null,
		'resposta'              => null,
		'criado_em'             => null,
		'criado_por'            => null,
		'atualizado_em'         => null,
		'atualizado_por'        => null,
		'excluido'              => null
	);
}

