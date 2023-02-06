<?php

class EditalCliente extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "edital.editalcliente";

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
        'edital_id' => null,
        'docid' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
        
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "edi.excluido IS FALSE";

        $sql = "SELECT
                    edi.categoria,
                    edi.orgao || ' [' || edi.portal || ']' as orgao,
                    TO_CHAR(edi.data_limite, 'DD/MM/YYYY') AS data_limite_formatada,
                    data_limite - current_date || ' (' || COALESCE(temperatura, '-') || ')' as dias,
                    '<b>' || valor_estimado || '</b> <br/> ' || substring(edi.objeto, 0, 150) || ' ...' as objeto,
                    mun.municipio || '/' || uf.uf_sigla AS municipio,
                    COALESCE(esd.esddsc, 'Selecionado') as situacao,
                    edc.id,
                    edi.id as id_edital
                FROM
                    edital.edital edi
                LEFT JOIN
                  corporativo.tb_municipio mun ON edi.municipio_id = mun.id 
                LEFT JOIN
                  corporativo.tb_unidade_federacao uf ON mun.uf_id = uf.id
                LEFT JOIN 
                    edital.editalcliente edc on edc.edital_id = edi.id
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
    public function salvarEditalCliente($params)
    {
       
       unset($params['id']);

        list($params, $camposNulos) = $this->limparDadosVazios($params);
        $dados = $params;
        // Carimbo de alteração
        //$dados = $this->getDadosCarimbo($params, $dados);
        $id = $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);

        return $id;
       
    }

    public function excluirEditalCliente($id)
    {
        $this->excluirLogicamente($id);
    }
}