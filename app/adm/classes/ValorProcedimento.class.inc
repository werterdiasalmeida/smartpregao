<?php

class ValorProcedimento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_valor_procedimento";

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
        'procedimento_id' => null,
        'tabela_procedimento_convenio_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
        'profissional_estabelecimento_procedimento_id' => null,
        'profissional_atende' => null,
        'valor_profissional' => null,
        'percentual_profissional' => null
    );

    public function getTodosDados($id)
    {
        $sql = "SELECT
                    vap.id,
                    vap.profissional_estabelecimento_procedimento_id,
                    vap.tabela_procedimento_convenio_id,
                    Coalesce(vap.valor, va.valor) as valor_procedimento,
                    Coalesce(vap.valor_profissional, va.valor_profissional) as valor_profissional,
                    Coalesce(vap.percentual_profissional, va.percentual_profissional, tpr.percentual_global) as percentual_profissional,
                    vap.profissional_atende,
                    vap.procedimento_id,
                    tpc.convenio_id
                FROM
                    estabelecimentosaude.tb_valor_procedimento vap
                INNER JOIN
                    estabelecimentosaude.tb_tabela_procedimento_convenio tpc ON vap.tabela_procedimento_convenio_id = tpc.id
                INNER JOIN
                    estabelecimentosaude.tb_tabela_preco tpr ON tpc.tabela_preco_id = tpr.id
                INNER JOIN
                    estabelecimentosaude.tb_convenio_estabelecimento coe ON tpc.convenio_id = coe.id
                LEFT JOIN
                    estabelecimentosaude.tb_valor_procedimento va ON va.tabela_procedimento_convenio_id = vap.tabela_procedimento_convenio_id
                                                                 AND va.procedimento_id = vap.procedimento_id
                                                                 AND va.profissional_estabelecimento_procedimento_id IS NULL
                                                                 AND va.excluido IS FALSE
                WHERE
                    vap.id = {$id}";

        return $this->pegaLinha($sql);
    }

    public function excluirPorConvenioProcedimento($convenioId,$procedimentoId,$tabelaPrecoId)
    {

        $sql = "UPDATE estabelecimentosaude.tb_valor_procedimento SET excluido = TRUE where procedimento_id = {$procedimentoId} and 
                                                    tabela_procedimento_convenio_id = 
                    (SELECT id FROM estabelecimentosaude.tb_tabela_procedimento_convenio where convenio_id = {$convenioId} and tabela_preco_id = {$tabelaPrecoId});";

        $this->executar($sql);
    }

    public function excluirPorTabelaProcedimento($tabelaProcedimentoId)
    {
        $sql = "UPDATE estabelecimentosaude.tb_valor_procedimento SET excluido = TRUE 
                  WHERE tabela_procedimento_convenio_id = {$tabelaProcedimentoId} 
                  AND profissional_estabelecimento_procedimento_id IS NULL";
        $this->executar($sql);
    }

    public function excluirPorTabelaProcedimentoProfissional($tabelaProcedimentoId)
    {
        $sql = "UPDATE estabelecimentosaude.tb_valor_procedimento SET excluido = TRUE 
                  WHERE tabela_procedimento_convenio_id = {$tabelaProcedimentoId} 
                  AND profissional_estabelecimento_procedimento_id IS NOT NULL";
        $this->executar($sql);
    }

    public function salvarValorIndividual($procedimentoId, $tabelaProcedimentoId, $valor, $valorProfissional, $percentualProfissional)
    {
        $valorProcedimentoId = $this->recuperarUm('id', array("procedimento_id = {$procedimentoId}", "tabela_procedimento_convenio_id = {$tabelaProcedimentoId}", "profissional_estabelecimento_procedimento_id IS NULL"));

        $excluido = 'false';
        if ($valor === '') {
            $valorProfissional = '';
            $percentualProfissional = '';
            $excluido = 'true';
        }

        $dados = array(
            'id' => $valorProcedimentoId,
            'codigo' => null,
            'valor' => $valor ? (strpos($valor, ',') === false ? $valor : desformata_valor($valor)) : 0,
            'valor_profissional' => $valorProfissional !== '' ? (strpos($valorProfissional, ',') === false ? $valorProfissional : desformata_valor($valorProfissional)) : '',
            'percentual_profissional' => $percentualProfissional !== '' ? (strpos($percentualProfissional, ',') === false ? $percentualProfissional : desformata_valor($percentualProfissional)) : '',
            'procedimento_id' => $procedimentoId,
            'tabela_procedimento_convenio_id' => $tabelaProcedimentoId,
            'excluido' => $excluido
        );

        return $this->manter($dados);
    }

    public function salvarValorProfissionalIndividual($procedimentoId, $profissionalEstabelecimentoProcedimentoId, $tabelaProcedimentoId, $valor, $valorProfissional, $percentualProfissional, $atende)
    {
        $valorProcedimentoId = $this->recuperarUm('id', array(
            "procedimento_id = {$procedimentoId}",
            "tabela_procedimento_convenio_id = {$tabelaProcedimentoId}",
            "profissional_estabelecimento_procedimento_id = {$profissionalEstabelecimentoProcedimentoId}"));

        $excluido = 'false';
        if ($valor === '' && $atende == 'S') {
            $valorProfissional = '';
            $percentualProfissional = '';
            $excluido = 'true';
        }

        return $this->manter(array(
            'id' => $valorProcedimentoId,
            'codigo' => null,
            'valor' => $valor ? (strpos($valor, ',') === false ? $valor : desformata_valor($valor)) : 0,
            'valor_profissional' => $valorProfissional !== '' ? (strpos($valorProfissional, ',') === false ? $valorProfissional : desformata_valor($valorProfissional)) : '',
            'percentual_profissional' => $percentualProfissional !== '' ? (strpos($percentualProfissional, ',') === false ? $percentualProfissional : desformata_valor($percentualProfissional)) : '',
            'procedimento_id' => $procedimentoId,
            'tabela_procedimento_convenio_id' => $tabelaProcedimentoId,
            'profissional_estabelecimento_procedimento_id' => $profissionalEstabelecimentoProcedimentoId,
            'profissional_atende' => $atende == 'N' ? 'false' : 'true',
            'excluido' => $excluido
        ));
    }

    public function salvarValores($tabelaProcedimentoId, $procedimentoValor)
    {
        $this->excluirPorTabelaProcedimento($tabelaProcedimentoId);

        foreach ($procedimentoValor as $procedimentoId => $valor) {
            $valorProcedimentoId = $this->recuperarUm('id', array("procedimento_id = {$procedimentoId}", "tabela_procedimento_convenio_id = {$tabelaProcedimentoId}", "profissional_estabelecimento_procedimento_id IS NULL"));
            $this->manter(array('id' => $valorProcedimentoId), array(
                'id' => $valorProcedimentoId,
                'codigo' => null,
                'valor' => desformata_valor($valor),
                'procedimento_id' => $procedimentoId,
                'tabela_procedimento_convenio_id' => $tabelaProcedimentoId,
                'excluido' => 'false'
            ));

            $this->setDadosNull();
        }
    }

    public function salvarValoresProfissionais($tabelaProcedimentoId, $valoresProfissionais)
    {
        $this->excluirPorTabelaProcedimentoProfissional($tabelaProcedimentoId);
        $mTabela = new TabelaProcedimentoConvenio($tabelaProcedimentoId);
        $convenioId = $mTabela->convenio_id;

        $valoresProfissionais = is_array($valoresProfissionais) ? $valoresProfissionais : array();
        foreach ($valoresProfissionais as $procedimentoId => $profissionais) {
            foreach ($profissionais as $profissionalId => $profissional) {
                $valor = isset($profissional['valor'][$convenioId]) ? $profissional['valor'][$convenioId] : null;

                if (empty($valor) && !isset($profissional['atende'][$convenioId])) {
                    continue;
                }

                $valorProcedimentoId = $this->recuperarUm('id', array(
                    "procedimento_id = {$procedimentoId}",
                    "tabela_procedimento_convenio_id = {$tabelaProcedimentoId}",
                    "profissional_estabelecimento_procedimento_id = {$profissional['profissional_estabelecimento_procedimento_id']}"));

                $this->manter(array('id' => $valorProcedimentoId), array(
                    'id' => $valorProcedimentoId,
                    'codigo' => null,
                    'valor' => $valor ? (strpos($valor, ',') === false ? $valor : desformata_valor($valor)) : 0,
                    'procedimento_id' => $procedimentoId,
                    'tabela_procedimento_convenio_id' => $tabelaProcedimentoId,
                    'profissional_estabelecimento_procedimento_id' => $profissional['profissional_estabelecimento_procedimento_id'],
                    'profissional_atende' => isset($profissional['atende'][$convenioId]) && !$profissional['atende'][$convenioId] ? 'false' : 'true',
                    'excluido' => 'false'
                ));

                $this->setDadosNull();
            }
        }
    }

    public function getValoresVigentesPorProfissionalEstabelecimentoProcedimento($profissionalEstabelecimentoProcedimentoId)
    {

        $sql = "SELECT
                  coe.nome AS convenio,
                  COALESCE(vpr_pro.valor, vpr.valor) AS valor,
                  COALESCE(vpr_pro.id, vpr.id) AS id
                FROM
                  estabelecimentosaude.tb_tabela_preco tbp
                  INNER JOIN
                  estabelecimentosaude.tb_tabela_procedimento_convenio tpc ON tbp.id = tpc.tabela_preco_id
                                                                              AND tbp.excluido IS FALSE
                  INNER JOIN
                  estabelecimentosaude.tb_convenio_estabelecimento coe ON tpc.convenio_id = coe.id
                                                                          AND coe.excluido IS FALSE
                  LEFT JOIN
                  estabelecimentosaude.tb_valor_procedimento vpr_pro ON vpr_pro.tabela_procedimento_convenio_id = tpc.id
                                                                        AND vpr_pro.excluido IS FALSE
                                                                        AND vpr_pro.profissional_estabelecimento_procedimento_id = {$profissionalEstabelecimentoProcedimentoId}
                  LEFT JOIN
                  estabelecimentosaude.tb_valor_procedimento vpr ON vpr.tabela_procedimento_convenio_id = tpc.id
                                                                    AND vpr.excluido IS FALSE
                                                                    AND vpr.profissional_estabelecimento_procedimento_id IS NULL
                                                                    AND (CASE
                                                                         WHEN vpr_pro.id IS NOT NULL THEN vpr.procedimento_id = vpr_pro.procedimento_id
                                                                         ELSE true
                                                                     END)
                  INNER JOIN
                  estabelecimentosaude.tb_profissional_estabelecimento_procedimento pep ON pep.procedimento_id = COALESCE(vpr_pro.procedimento_id, vpr.procedimento_id)
                                                                                          AND pep.excluido IS FALSE
                
                WHERE
                  vpr_pro.profissional_atende IS NOT FALSE AND
                  tbp.excluido IS FALSE AND
                  pep.id = {$profissionalEstabelecimentoProcedimentoId}
                  " . TabelaProcedimentoConvenio::getWhereTabelaVigente() . "
                ORDER BY
                  coe.nome ASC";

        return $this->carregarArray($sql);
    }

    public function calcularValorCobranca($valorProcedimentoId)
    {
        $valor = $this->getTodosDados($valorProcedimentoId);
        $valorProcedimento = $valor['valor_procedimento'];
        $valorProfissional = $valor['valor_profissional'];
        $percentualProfissional =  $valor['percentual_profissional'];


        if(!$valorProfissional){
            $valorProfissional = ($valorProcedimento * $percentualProfissional) / 100;
        }

        return array(
            'valor' => $valorProcedimento,
            'valor_profissional' => $valorProfissional,
            'percentual_profissional' => $percentualProfissional,
            'convenio_id' => $valor['convenio_id']
        );
    }

    public static function isVisualizaValores($estabelecimentoId)
    {
        return UsuarioResponsabilidadeEstabelecimento::isPossuiPerfilEstabelecimento($estabelecimentoId, array(
            ADM_PERFIL_LOCAL,
            ADM_PERFIL_GERAL
        ));
    }

    public function getIdByProcedimentoTabela($procedimentoId, $tabelaProcedimentoConvenioId, $profissionalEstabelecimentoProcId = null)
    {
        $whereProfissional = "profissional_estabelecimento_procedimento_id IS NULL";
        if($profissionalEstabelecimentoProcId) {
            $whereProfissional = "profissional_estabelecimento_procedimento_id = {$profissionalEstabelecimentoProcId}";
        }

        return $this->recuperarUm('id', array(
            "procedimento_id = {$procedimentoId}",
            "tabela_procedimento_convenio_id = {$tabelaProcedimentoConvenioId}",
            $whereProfissional
        ));
    }
}