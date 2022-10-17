<?php

class ValorProcedimentoCodigo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_valor_procedimento_codigo";

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
        'codigo' => null,
        'valor' => null,
        'valor_procedimento_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function getByProcedimentoValor($procedimentoId, $valorProcedimentoId)
    {
        $valorProcedimentoId = $valorProcedimentoId ?: -1;
        $sql = "SELECT
                   vpc.id,
                   proc.codigo_ans AS codigo,
                   tuss.procedimento,
                   vpc.valor
                FROM
                    estabelecimentosaude.vw_procedimento_codigo_ans proc
                LEFT JOIN
                    corporativo.vw_tuss tuss ON tuss.cod_tuss = proc.codigo_ans
                LEFT JOIN
                    estabelecimentosaude.tb_valor_procedimento_codigo vpc ON vpc.codigo = proc.codigo_ans
                                                                         AND (valor_procedimento_id = {$valorProcedimentoId} OR valor_procedimento_id IS NULL)
                WHERE
                    proc.id = {$procedimentoId}-- and (vpc.excluido is false or vpc.excluido is null)
                ORDER BY
                    proc.codigo_ans ASC";

        return $this->carregarArray($sql);
    }

    public function getAssocByProcedimentoValor($procedimentoId, $valorProcedimentoId)
    {
        $valoresProcedimento = $this->getByProcedimentoValor($procedimentoId, $valorProcedimentoId);
        $valorProcedimentoAssoc = [];
        foreach($valoresProcedimento as $valorProcedimento) {
            $valorProcedimentoAssoc[$valorProcedimento['codigo']] = $valorProcedimento;
        }

        return $valorProcedimentoAssoc;
    }

    public function salvarValor($valoresPorCodigo, $valorProcedimentoId)
    {
        if(!is_array($valoresPorCodigo)) {
            return false;
        }

        foreach($valoresPorCodigo as $codigo => $valor) {
            $valorCodigo = array(
                'id' => $this->recuperarUm('id', ["codigo = '{$codigo}'", "valor_procedimento_id = '{$valorProcedimentoId}'"]),
                'codigo' => $codigo,
                'valor' => desformata_valor($valor),
                'valor_procedimento_id' => $valorProcedimentoId,
            );

            $this->manter($valorCodigo);
            $this->setDadosNull();
        }
    }

    public function setValorNull($Id)
    {
        $valorCodigo = array(
            'id' => $Id,
            'valor' => null,
        );

        $this->popularDadosObjeto($valorCodigo)->salvar(true,true,['valor']);
        $this->setDadosNull();
    }
}