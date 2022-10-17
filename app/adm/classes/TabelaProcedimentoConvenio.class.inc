<?php

class TabelaProcedimentoConvenio extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_tabela_procedimento_convenio";

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
        'descricao' => null,
        'inicio_vigencia' => null,
        'fim_vigencia' => null,
        'convenio_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
        'percentual_global' => null,
        'tabela_preco_id' => null
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null, $filtrarResponsabilidade = true)
    {
        $arWhere[] = "tpc.excluido IS FALSE";

        if (!$this->testa_superuser() && $filtrarResponsabilidade) {
            $arWhere[] = UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento();
        }

        $sql = "SELECT
                  tpc.inicio_vigencia,
                  tpc.fim_vigencia,
                  array_to_string(array_agg(cet.nome), ',') AS convenios,
                  cet.estabelecimento_id,
                  tpc.descricao,
                  tpc.percentual_global
                FROM
                  estabelecimentosaude.tb_tabela_procedimento_convenio tpc
                INNER JOIN
                  estabelecimentosaude.tb_convenio_estabelecimento cet ON tpc.convenio_id = cet.id
                                                                      AND cet.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento est ON cet.estabelecimento_id = est.id
                WHERE
                  " . implode(' AND ', $arWhere) . "
                GROUP BY
                  tpc.inicio_vigencia,
                  tpc.fim_vigencia,
                  cet.estabelecimento_id,
                  tpc.descricao,
                  tpc.percentual_global";

        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY tpc.inicio_vigencia DESC, tpc.fim_vigencia DESC";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 15);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public function salvarTabelaIndividual($params, $salvarValor = true, $salvarValorProfissional = false)
    {
        $tabelaPrecoId = $params['tabela_preco_id'];
        $convenioId = $params['convenio'];
        $procedimentoId = $params['procedimento'];

        $tabelaPreco = new TabelaPreco();
        $dadosTabelaPreco = $tabelaPreco->carregarPorId($tabelaPrecoId)->getDados();

        $tabelaProcedimentoId = $this->recuperarUm('id', array("tabela_preco_id = '{$tabelaPrecoId}'", "convenio_id = {$convenioId}"));
        $tabelaProcedimentoId = $this->manter(array('id' => $tabelaProcedimentoId), array(
            'id' => $tabelaProcedimentoId,
            'descricao' => "Tabela de preços de " . formata_data($dadosTabelaPreco['inicio_vigencia']) . " a " . formata_data($dadosTabelaPreco['fim_vigencia']) . ".",
            'convenio_id' => $convenioId,
            'tabela_preco_id' => $params['tabela_preco_id'],
            'excluido' => 'false'
        ));
        $this->setDadosNull();

        $mValorProcedimento = new ValorProcedimento();
        $mValorCodigo = new ValorProcedimentoCodigo();

        if($salvarValor){
            $valorProcedimentoId = $mValorProcedimento->salvarValorIndividual($procedimentoId,$tabelaProcedimentoId, $params['valor'], $params['valorProfissional'], $params['percentualProfissional']);
            $mValorCodigo->salvarValor($params['valor_codigo'], $valorProcedimentoId);
        }

        if($salvarValorProfissional){
            $mProfissional = new ProfissionalEstabelecimentoProcedimento();
            $profissionalEstabelecimentoProcedimentoId = $mProfissional->recuperarUm('id', array(
                "profissional_id = {$params['profissional']}",
                "estabelecimento_id = {$params['estabelecimento_id']}",
                "procedimento_id = {$params['procedimento']}",
            ));
            $valorProcedimentoId = $mValorProcedimento->salvarValorProfissionalIndividual($procedimentoId,$profissionalEstabelecimentoProcedimentoId,$tabelaProcedimentoId, $params['valor'], $params['valorProfissional'], $params['percentualProfissional'],$params['executaProcedimento']);
            $mValorCodigo->salvarValor($params['valor_codigo'], $valorProcedimentoId);
        }
    }

    public function salvarTabelas($params)
    {
        $dataInicio = formata_data_sql($params['inicio']);
        $dataFim = formata_data_sql($params['fim']);
        $dataFimOriginal = formata_data_sql($params['fim_original'] ? $params['fim_original'] : $params['fim']);
        $estabelecimentoId = $params['estabelecimento_id'];
        $mValorProcedimento = new ValorProcedimento();
        $this->excluirPorEstabelecimentoData($estabelecimentoId, $dataInicio, $dataFimOriginal);

        foreach ($params['valor'] as $convenioId => $procedimentoValor) {
            $procedimentoValor = array_filter($procedimentoValor, function ($valor) {
                return !empty($valor) && $valor !== '0,00';
            });

            $procedimentoValorProfissional = array_filter($params['valores_profissionais'], function ($valorProfissional) use ($convenioId) {
                $valorPorConvenio = false;
                foreach ($valorProfissional as $procedimentoId => $profissionais) {

                    if ((isset($profissionais['valor'][$convenioId]) && !empty($profissionais['valor'][$convenioId])
                            && $profissionais['valor'][$convenioId] !== '0,00')
                        || (isset($profissionais['atende'][$convenioId]))) {
                        $valorPorConvenio = true;
                        break;
                    }
                }

                return $valorPorConvenio;
            });

            if (count($procedimentoValor) === 0 && count($procedimentoValorProfissional) === 0) {
                continue;
            }

            $tabelaProcedimentoId = $this->recuperarUm('id', array("inicio_vigencia = '{$dataInicio}'", "fim_vigencia = '{$dataFimOriginal}'", "convenio_id = {$convenioId}"));
            $tabelaProcedimentoId = $this->manter(array('id' => $tabelaProcedimentoId), array(
                'id' => $tabelaProcedimentoId,
                'descricao' => "Tabela de preços de {$params['inicio']} a {$params['fim']}",
                'inicio_vigencia' => $dataInicio,
                'fim_vigencia' => $dataFim,
                'convenio_id' => $convenioId,
                'excluido' => 'false',
                'percentual_global' => desformata_valor($params['percentualGlobal'])
            ));
            $this->setDadosNull();

            $mValorProcedimento->salvarValores($tabelaProcedimentoId, $procedimentoValor);
            $mValorProcedimento->salvarValoresProfissionais($tabelaProcedimentoId, $procedimentoValorProfissional);
        }
    }

    public function getValoresTabelaAgrupadosByTabelaPreco($tabelaPrecoId)
    {
        $valoresAgrupados = array();
        $sql = "SELECT
                    tpc.convenio_id,
                    tvp.procedimento_id,
                    tvp.valor,
                    tvp.valor_profissional,
                    tvp.percentual_profissional
                FROM
                    estabelecimentosaude.tb_tabela_procedimento_convenio tpc
                INNER JOIN
                    estabelecimentosaude.tb_valor_procedimento tvp ON tpc.id = tvp.tabela_procedimento_convenio_id
                                                                  AND tvp.excluido IS FALSE
                                                                  and tvp.profissional_estabelecimento_procedimento_id IS NULL
                INNER JOIN
                    estabelecimentosaude.tb_convenio_estabelecimento coe ON tpc.convenio_id = coe.id
                WHERE
                    tpc.excluido IS FALSE AND
                    tpc.tabela_preco_id = '{$tabelaPrecoId}';";

        foreach ($this->carregarArray($sql) as $valor) {
            if (!isset($valoresAgrupados[$valor['convenio_id']])) {
                $valoresAgrupados[$valor['convenio_id']] = array();
            }

            $valoresAgrupados[$valor['convenio_id']][$valor['procedimento_id']] = $valor;
        }

        return $valoresAgrupados;
    }

    public function getValoresTabelaAgrupados($inicio, $fim, $estabelecimentoId)
    {
        $valoresAgrupados = array();
        $sql = "SELECT
                    tpc.convenio_id,
                    tvp.procedimento_id,
                    tvp.valor,
                    tvp.valor_profissional,
                    tvp.percentual_profissional
                FROM
                    estabelecimentosaude.tb_tabela_procedimento_convenio tpc
                INNER JOIN
                    estabelecimentosaude.tb_valor_procedimento tvp ON tpc.id = tvp.tabela_procedimento_convenio_id
                                                                  AND tvp.excluido IS FALSE
                                                                  and tvp.profissional_estabelecimento_procedimento_id IS NULL
                INNER JOIN
                    estabelecimentosaude.tb_convenio_estabelecimento coe ON tpc.convenio_id = coe.id
                WHERE
                    tpc.excluido IS FALSE
                    AND
                    tpc.inicio_vigencia = '{$inicio}'
                    AND
                    tpc.fim_vigencia = '{$fim}'
                    AND
                    coe.estabelecimento_id = {$estabelecimentoId}";

        foreach ($this->carregarArray($sql) as $valor) {
            if (!isset($valoresAgrupados[$valor['convenio_id']])) {
                $valoresAgrupados[$valor['convenio_id']] = array();
            }

            $valoresAgrupados[$valor['convenio_id']][$valor['procedimento_id']] = $valor;
        }

        return $valoresAgrupados;
    }

    public function excluirPorEstabelecimentoData($estabelecimentoId, $inicioVigencia, $fimVigencia)
    {
        $sql = "UPDATE estabelecimentosaude.tb_tabela_procedimento_convenio SET excluido = TRUE 
                  WHERE inicio_vigencia = '{$inicioVigencia}' AND fim_vigencia = '{$fimVigencia}'
                    AND convenio_id IN (SELECT id FROM estabelecimentosaude.tb_convenio_estabelecimento 
                                         WHERE excluido IS FALSE AND estabelecimento_id = {$estabelecimentoId})";

        $this->executar($sql);
    }

    public function excluirPorConvenio($convenioId)
    {
        $sql = "UPDATE estabelecimentosaude.tb_tabela_procedimento_convenio SET excluido = TRUE 
                  WHERE inicio_vigencia = '{$inicioVigencia}' AND fim_vigencia = '{$fimVigencia}'
                    AND convenio_id =   {$convenioId}";

        $this->executar($sql);
    }

    public function getValoresProcedimentosProfissionaisAgrupados($profissionaisValores)
    {
        $profissionaisValoresAgrupados = array();
        foreach ($profissionaisValores as $profissionalValor) {
            if (!isset($profissionaisValoresAgrupados[$profissionalValor['procedimento_id']])) {
                $profissionaisValoresAgrupados[$profissionalValor['procedimento_id']] = array();
            }

            if (!isset($profissionaisValoresAgrupados[$profissionalValor['procedimento_id']][$profissionalValor['profissional_id']])) {
                $profissionaisValoresAgrupados[$profissionalValor['procedimento_id']][$profissionalValor['profissional_id']] = array(
                    'profissional_id' => $profissionalValor['profissional_id'],
                    'profissional_estabelecimento_procedimento_id' => $profissionalValor['profissional_estabelecimento_procedimento_id'],
                    'profissional' => $profissionalValor['nome_completo'],
                    'tipo' => $profissionalValor['tipo'],
                );
            }

            if ($profissionalValor['convenio_id']) {
                $profissionaisValoresAgrupados[$profissionalValor['procedimento_id']]
                                                                 [$profissionalValor['profissional_id']]
                                                                 ["valor"]
                                                                 [$profissionalValor['convenio_id']] = $profissionalValor['valor'];

                $profissionaisValoresAgrupados[$profissionalValor['procedimento_id']]
                                                                 [$profissionalValor['profissional_id']]
                                                                 ["percentual_profissional"]
                                                                 [$profissionalValor['convenio_id']] = $profissionalValor['percentual_profissional'];

                $profissionaisValoresAgrupados[$profissionalValor['procedimento_id']]
                                                                 [$profissionalValor['profissional_id']]
                                                                 ["valor_profissional"]
                                                                 [$profissionalValor['convenio_id']] = $profissionalValor['valor_profissional'];

                if($profissionalValor['atende'] === 'f'){
                    $profissionaisValoresAgrupados[$profissionalValor['procedimento_id']]
                                                                     [$profissionalValor['profissional_id']]
                                                                     ["atende"]
                                                                     [$profissionalValor['convenio_id']] = false;
                }
            }
        }

        return $profissionaisValoresAgrupados;
    }


    public function getValoresTabelaProcedimentosProfissionaisByTabelaPreco($estabelecimentoId,$tabelaPrecoId)
    {
        $sql = "SELECT
                    CASE 
                      WHEN pes.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                      THEN tp.pron_trat_masc || ' ' || pes.nome_completo
                      WHEN pes.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                      THEN tp.pron_trat_fem || ' ' || pes.nome_completo
                      ELSE pes.nome_completo
                      END AS nome_completo,
                    tp.descricao AS tipo,
                    proc.descricao AS procedimento,
                    vap.valor,
                    vap.valor_profissional,
                    vap.percentual_profissional,
                    vap.profissional_atende AS atende,
                    pep.profissional_id, 
                    proc.id AS procedimento_id,
                    tpc.convenio_id,
                    pep.id AS profissional_estabelecimento_procedimento_id
                FROM
                    estabelecimentosaude.tb_profissional_estabelecimento_procedimento pep
                INNER JOIN
                    profissionalsaude.tb_profissional pro ON pep.profissional_id = pro.id
                                                         AND pep.excluido IS FALSE
                INNER JOIN
                    profissionalsaude.tb_especialidade_profissional ep ON pro.id = ep.profissional_id
                                                                      AND ep.excluido IS FALSE
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional esp ON ep.id = esp.especialidade_profissional_id
                                                                                          AND esp.excluido IS FALSE
                                                                                          AND esp.estabelecimento_id = pep.estabelecimento_id
                INNER JOIN
                    corporativo.tb_pessoafisica pes ON pro.pessoafisica_id = pes.id
                INNER JOIN
                    profissionalsaude.tb_tipo_profissional tp ON pro.tipo_profissional_id = tp.id
                INNER JOIN
                  estabelecimentosaude.tb_procedimento proc ON pep.procedimento_id = proc.id
                                                          AND pro.excluido IS FALSE
                LEFT JOIN
                  estabelecimentosaude.tb_valor_procedimento vap ON proc.id = vap.procedimento_id
                                                                AND vap.excluido IS FALSE
                                                                AND vap.profissional_estabelecimento_procedimento_id = pep.id
                LEFT JOIN
                  estabelecimentosaude.tb_tabela_procedimento_convenio tpc ON vap.tabela_procedimento_convenio_id = tpc.id
                                                                          AND tpc.excluido IS FALSE
                                                                          AND tpc.tabela_preco_id = {$tabelaPrecoId}
                LEFT JOIN
                    estabelecimentosaude.tb_convenio_estabelecimento coe ON tpc.convenio_id = coe.id
                WHERE
                    pro.excluido IS FALSE
                    AND
                    pep.estabelecimento_id = {$estabelecimentoId}
               ORDER BY
                   nome_completo";

        return $this->carregarArray($sql);
    }

    public function getValoresTabelaProcedimentosProfissionais($inicio, $fim, $estabelecimentoId)
    {
        $sql = "SELECT
                    CASE 
                      WHEN pes.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                      THEN tp.pron_trat_masc || ' ' || pes.nome_completo
                      WHEN pes.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                      THEN tp.pron_trat_fem || ' ' || pes.nome_completo
                      ELSE pes.nome_completo
                      END AS nome_completo,
                    tp.descricao AS tipo,
                    proc.descricao AS procedimento,
                    vap.valor,
                    vap.valor_profissional,
                    vap.percentual_profissional,
                    vap.profissional_atende AS atende,
                    pep.profissional_id, 
                    proc.id AS procedimento_id,
                    tpc.convenio_id,
                    pep.id AS profissional_estabelecimento_procedimento_id
                FROM
                    estabelecimentosaude.tb_profissional_estabelecimento_procedimento pep
                INNER JOIN
                    profissionalsaude.tb_profissional pro ON pep.profissional_id = pro.id
                                                         AND pep.excluido IS FALSE
                INNER JOIN
                    profissionalsaude.tb_especialidade_profissional ep ON pro.id = ep.profissional_id
                                                                      AND ep.excluido IS FALSE
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional esp ON ep.id = esp.especialidade_profissional_id
                                                                                          AND esp.excluido IS FALSE
                                                                                          AND esp.estabelecimento_id = pep.estabelecimento_id
                INNER JOIN
                    corporativo.tb_pessoafisica pes ON pro.pessoafisica_id = pes.id
                INNER JOIN
                    profissionalsaude.tb_tipo_profissional tp ON pro.tipo_profissional_id = tp.id
                INNER JOIN
                  estabelecimentosaude.tb_procedimento proc ON pep.procedimento_id = proc.id
                                                          AND pro.excluido IS FALSE
                LEFT JOIN
                  estabelecimentosaude.tb_valor_procedimento vap ON proc.id = vap.procedimento_id
                                                                AND vap.excluido IS FALSE
                                                                AND vap.profissional_estabelecimento_procedimento_id = pep.id
                LEFT JOIN
                  estabelecimentosaude.tb_tabela_procedimento_convenio tpc ON vap.tabela_procedimento_convenio_id = tpc.id
                                                                          AND tpc.excluido IS FALSE
                                                                          AND tpc.inicio_vigencia = '{$inicio}'
                                                                          AND tpc.fim_vigencia = '{$fim}'
                LEFT JOIN
                    estabelecimentosaude.tb_convenio_estabelecimento coe ON tpc.convenio_id = coe.id
                WHERE
                    pro.excluido IS FALSE
                    AND
                    pep.estabelecimento_id = {$estabelecimentoId}
               ORDER BY
                   nome_completo";

        return $this->carregarArray($sql);
    }

    public static function getWhereTabelaVigente($prefixoTabelaPreco = 'tbp', $prefixoTabelaConvenio = 'tpc'){
        return "AND ({$prefixoTabelaPreco}.fim_vigencia = (SELECT 
                                                               MAX(tbps.fim_vigencia) 
                                                           FROM 
                                                               estabelecimentosaude.tb_tabela_preco tbps 
                                                           INNER JOIN
                                                               estabelecimentosaude.tb_tabela_procedimento_convenio tpcs ON tbps.id = tpcs.tabela_preco_id
                                                                                                                        AND tpcs.excluido IS FALSE
                                                           WHERE 
                                                               tbps.excluido IS FALSE 
                                                               AND 
                                                               {$prefixoTabelaConvenio}.convenio_id = tpcs.convenio_id 
                                                               AND 
                                                               tbps.inicio_vigencia <= CURRENT_DATE
                                                           ))";
    }

    public function getIdByTabelaPrecoConvenio($tabelaPrecoId, $convenioId)
    {
        return $this->recuperarUm('id', array("tabela_preco_id = '{$tabelaPrecoId}'", "convenio_id = {$convenioId}"));
    }
}