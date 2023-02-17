<?php

class EditalClienteContrato extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "edital.editalcliente_contrato";

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
        'editalcliente_id' => null,
        'numero_contrato' => null,
        'vencimento_contrato' => null,
        'processo_adminitrativo' => null,
        'responsavel_interno' => null,
        'valor_total' => null,
        'estabelecimento_id' => null,
        'cnpj_contratante' => null,
        'nome_contratante' => null,
        'objeto_contratante' => null,
    );

    public function salvarContrato($params, $arquivos = null)
    {
        //ver($params,d);
        // Tratamento dos dados
        $dados = array(
            'id' => $params['editalcliente_contrato_id'],
            'numero_contrato' => $params['numero_contrato'],
            'tipo_acompanhamento' => $params['tipo_acompanhamento'],
            'vencimento_contrato' => formata_data_sql($params['vencimento_contrato']),
            'inicio_contrato' => formata_data_sql($params['inicio_contrato']),
            'processo_adminitrativo' => $params['processo_adminitrativo'],
            'responsavel_interno' => $params['responsavel_interno'],
            'editalcliente_id' => $params['editalcliente_id'],
            'valor_total' => formata_valor_sql($params['valor_total']),
            'estabelecimento_id' => $params['id'],
            'cnpj_contratante' => $params['cnpj_contratante'],
            'nome_contratante' => $params['nome_contratante'],
            'objeto_contratante' => $params['objeto_contratante'],
        );
        $id = $this->popularDadosObjeto($dados)->salvar();
        return $id;
    }
    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "edi.excluido IS FALSE";

        $sql = "SELECT
                    CASE WHEN ecc.cnpj_contratante IS NOT NULL THEN
                        upper(coalesce(ecc.nome_contratante, edi.orgao)) || ' ('||ecc.cnpj_contratante||')'
                    ELSE
                        upper(coalesce(ecc.nome_contratante, edi.orgao))
                    END as empresa,
                    ecc.numero_contrato,
                    TO_CHAR(ecc.inicio_contrato, 'DD/MM/YYYY') as inicio,
                    TO_CHAR(ecc.vencimento_contrato, 'DD/MM/YYYY') as fim,
                    ecc.valor_total,
                    'Em execução',
                    ecc.id
                FROM
                    edital.editalcliente_contrato ecc
                LEFT JOIN edital.editalcliente edc ON ecc.editalcliente_id = edc.id
                LEFT JOIN edital.edital edi ON edc.edital_id = edi.id
                 WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY edi.data_limite";
            }
        }
        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 10);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }
    public function retornaPainelCliente($id)
    {

        $saida = array();

        $saida['cotacoes_recebidas'] = 0;
        $saida['cotacoes_enviadas'] = 0;
        $sql = "SELECT COUNT (0) 
            FROM edital.cotacao_dispensa_credenciamentoempresa ccl
            JOIN  edital.cotacao_dispensa_credenciamento  cdc ON cdc.id = ccl.cdc_id
            WHERE estabelecimento_id = {$id} AND tipo = 'Dispensa eletrônica'";
        $saida['dispensas_recebidas'] = $this->pegaUm($sql);
        $saida['dispensas_recebidas'] = 0;
        $saida['dispensas_enviadas'] = 0;
        $saida['credenciamentos_cadastrados'] = 0;
        $saida['credenciamentos_pendentes'] = 0;

        return $saida;
    }
}
