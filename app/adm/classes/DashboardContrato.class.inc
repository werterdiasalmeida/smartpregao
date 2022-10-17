<?php

class DashboardContrato extends Modelo
{
    public function getDadosDashboard($arrDados)
    {
        $dadosContrato = $this->getDadosContrato($arrDados);
        $dadosFaturamento = $this->getDadosFaturamento($arrDados);
        $dadosGraficoFaturamento = $this->getDadosParaGraficoDeFaturamento($dadosFaturamento);

        return [
            'dados_contrato' => $dadosContrato,
            'dados_faturamento' => $dadosFaturamento,
            'dados_grafico_faturamento' => $dadosGraficoFaturamento
        ];
    }

    private function getDadosContrato($arrDados)
    {
        $arrWhere[] = "con.excluido IS FALSE";

        if (!empty($arrDados['inicio']) && !empty($arrDados['fim'])) {
            $arrWherePeriodo[] = "con.inicio_vigencia BETWEEN '{$arrDados['inicio']}'::date AND '{$arrDados['fim']}'::date";
            $arrWherePeriodo[] = "con.fim_vigencia BETWEEN '{$arrDados['inicio']}'::date AND '{$arrDados['fim']}'::date";
            $arrWhere[] = "(" . implode(" OR ", $arrWherePeriodo) . ")";
        }

        if (!empty($arrDados['estabelecimento_id'])) {
            $arrWhere[] = "esc.estabelecimento_id IN (" . implode(", ", $arrDados['estabelecimento_id']) . ")";
        }

        $sql = "
            SELECT 
                count(con.id) FILTER (WHERE con.situacao = '" . Contrato::SITUACAO_CONTRATO_ATIVO . "') AS qtd_ativo,
                count(con.id) FILTER (WHERE con.situacao = '" . Contrato::SITUACAO_CONTRATO_PENDENTE_PAGAMENTO . "') AS qtd_ativo_pendente_pagamento,
                count(con.id) FILTER (WHERE con.situacao = '" . Contrato::SITUACAO_CONTRATO_CANCELADO . "') AS qtd_cancelado
            FROM
                financeiro.tb_contrato con
            INNER JOIN 
                estabelecimentosaude.tb_estabelecimento_contrato esc ON esc.contrato_id = con.id
            WHERE
                " . implode(' AND ', $arrWhere) . "
        ";

        $rs = $this->pegaLinha($sql);
        $rs = $rs ? $rs : [];

        return $rs;
    }

    private function getDadosFaturamento($arrDados)
    {
        $arrWhere[] = "con.excluido IS FALSE";

        if (!empty($arrDados['inicio']) && !empty($arrDados['fim'])) {
            $arrWhere[] = "fat.data_vencimento BETWEEN '{$arrDados['inicio']}'::date AND '{$arrDados['fim']}'::date";
        }

        if (!empty($arrDados['estabelecimento_id'])) {
            $arrWhere[] = "esc.estabelecimento_id IN (" . implode(", ", $arrDados['estabelecimento_id']) . ")";
        }


        $sql = "
            SELECT 
                SUM(fat.valor) FILTER (WHERE fat.situacao = '" . Fatura::SITUACAO_FATURA_PAGA . "') AS vlr_parcelas_pagas,
                SUM(fat.valor) FILTER (WHERE fat.situacao = '" . Fatura::SITUACAO_FATURA_VENCIDA . "') AS vlr_parcelas_vencidas,
                SUM(fat.valor) FILTER (WHERE fat.situacao = '" . Fatura::SITUACAO_FATURA_GERADA . "' OR fat.situacao = '" . Fatura::SITUACAO_FATURA_PENDENTE . "') AS vlr_parcelas_a_vencer,
                SUM(fat.valor) AS vlr_total
            FROM
                financeiro.tb_contrato con
            INNER JOIN 
                estabelecimentosaude.tb_estabelecimento_contrato esc ON esc.contrato_id = con.id AND esc.excluido IS FALSE
            INNER JOIN
                financeiro.tb_contrato_fatura cnf ON con.id = cnf.contrato_id AND cnf.excluido IS FALSE
            INNER JOIN
                financeiro.tb_fatura fat ON fat.id = cnf.fatura_id AND fat.excluido IS FALSE
            WHERE
                " . implode(' AND ', $arrWhere) . "
        ";

        $rs = $this->pegaLinha($sql);
        $rs = $rs ? $rs : [];

        return $rs;
    }

    private function getDadosParaGraficoDeFaturamento($dadosFaturamento)
    {
        $arrSeries = [];

        if ($dadosFaturamento['vlr_total']) {
            $percParcelaAVencer = ($dadosFaturamento['vlr_parcelas_a_vencer'] * 100) / $dadosFaturamento['vlr_total'];
            $percParcelaVencidas = ($dadosFaturamento['vlr_parcelas_vencidas'] * 100) / $dadosFaturamento['vlr_total'];
            $percParcelaPaga = ($dadosFaturamento['vlr_parcelas_pagas'] * 100) / $dadosFaturamento['vlr_total'];
            $arrSeries = [
                [
                    'data' => [
                        [
                            'name' => 'Valor pago',
                            'y' => $percParcelaPaga,
                            'valor' => formata_valor($dadosFaturamento['vlr_parcelas_pagas']),
                            'color' => '#5F9CCF'
                        ],
                        [
                            'name' => 'Valor vencido',
                            'y' => $percParcelaVencidas,
                            'color' => '#E0686C',
                            'valor' => formata_valor($dadosFaturamento['vlr_parcelas_vencidas'])
                        ],
                        [
                            'name' => 'Valor a receber',
                            'y' => $percParcelaAVencer,
                            'color' => '#BFBFBF',
                            'valor' => formata_valor($dadosFaturamento['vlr_parcelas_a_vencer'])
                        ]
                    ]
                ]
            ];
        }


        return json_encode($arrSeries);
    }
}