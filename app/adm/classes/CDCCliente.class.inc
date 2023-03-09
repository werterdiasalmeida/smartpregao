<?php

class CDCCliente extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "edital.cotacao_dispensa_credenciamentoempresa";

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
        'cdc_id' => null,
        'estabelecimento_id' => null,
        'docid' => null,
        'comentarios' => null,
        'valor_estimado' => null,

    );

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "edi.id IS NOT NULL";

        $sql = "SELECT
                    edi.tipo,
                    edi.orgao || ' [' || edi.portal || ']' as orgao,
                    TO_CHAR(edi.data_limite, 'DD/MM/YYYY') AS data_limite_formatada,
                    data_limite - current_date || ' (' || COALESCE(temperatura, '-') || ')' as dias,
                    substring(edi.objeto, 0, 150) || ' ...' as objeto,
                    mun.municipio || '/' || uf.uf_sigla AS municipio,
                    COALESCE(esd.esddsc, 'Selecionado') as situacao,
                    edc.id,
                    edi.id as id_edital
                FROM
                    edital.cotacao_dispensa_credenciamento edi
                LEFT JOIN
                  corporativo.tb_municipio mun ON edi.municipio_id = mun.id 
                LEFT JOIN
                  corporativo.tb_unidade_federacao uf ON mun.uf_id = uf.id
                LEFT JOIN 
                    edital.cotacao_dispensa_credenciamentoempresa edc on edc.cdc_id = edi.id
                LEFT JOIN 
                    workflow.documento doc ON doc.docid = edc.docid
                LEFT JOIN 
                    workflow.estadodocumento esd ON esd.esdid = doc.esdid
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

    /**
     * @param $params
     * @throws Exception
     */
    public function salvarCDCCliente($params)
    {
        // tratamento dos dados

        $params['valor_estimado'] = formata_valor_sql($params['valor_estimado']);
        if (!$params['valor_estimado'] OR $params['valor_estimado']=='NULL') $params['valor_estimado'] = 0;
        $id = $this->popularDadosObjeto($params)->salvar();
        return $id;
    }

    public function excluirEditalCliente($id)
    {
        $this->excluirLogicamente($id);
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
        $saida['dispensas_enviadas'] = 0;
        $saida['credenciamentos_cadastrados'] = 0;
        $saida['credenciamentos_pendentes'] = 0;

        return $saida;
    }
    public function retornaListaStatusWF()
    {

        $sql = "select esdid as codigo,
        esddsc as descricao
        from workflow.estadodocumento where tpdid = 18
        order by 2";

        return $this->carregarArray($sql);
    }
}
