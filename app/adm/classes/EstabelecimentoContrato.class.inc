<?php

class EstabelecimentoContrato extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_estabelecimento_contrato";

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
        'contrato_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    /**
     * Verifica se o contrato vigente é ativo
     * @param $estabelecimentoId
     * @return bool
     */
    public function isContratoEmVigenciaAtivoPorEstabelecimento($estabelecimentoId)
    {
        $contratoAtivo = true;

        $mContrato = new Contrato();
        $dadosContratoVigente = $mContrato->getDadosContratoVigentePorEstabelecimento($estabelecimentoId);

        if($dadosContratoVigente) {
            $contratoAtivo = $dadosContratoVigente['situacao'] == Contrato::SITUACAO_CONTRATO_ATIVO;
        }

        return $contratoAtivo;
    }
}