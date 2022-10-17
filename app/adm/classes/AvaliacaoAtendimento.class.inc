<?php

class AvaliacaoAtendimento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_avaliacao_atendimento";

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
        'id'                => null,
        'agenda_id'         => null,
        'nota'              => null,
        'melhorias'         => null,
        'comentario'        => null,
        'criado_em'         => null,
        'criado_por'        => null,
        'atualizado_em'     => null,
        'atualizado_por'    => null,
        'excluido'          => null
    );

    const AVALIACAO_MELHORIA_ATENDIMENTO_MEDICO = 'AM';
    const AVALIACAO_MELHORIA_ATENDIMENTO_RECEPCAO = 'AR';
    const AVALIACAO_MELHORIA_MARCACAO = 'MC';
    const AVALIACAO_MELHORIA_TEMPO_ESPERA = 'TE';
    const AVALIACAO_MELHORIA_OUTROS = 'OT';

}