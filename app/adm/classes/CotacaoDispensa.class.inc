<?php

class CotacaoDispensa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "edital.cotacao_dispensa_credenciamento";

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
        'portal' => null,
        'orgao' => null,
        'edital' => null,
        'data_limite' => null,
        'hora_limite' => null,
        'objeto' => null,
        'temperatura' => null,
        'id_externo' => null,
        'id_origem' => null,                
        'categoria' => null,  
        'municipio_id' => null,                
        'tipo' => null,
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "edi.id IS not null";

        $sql = "SELECT
                    edi.tipo,
                    edi.portal,
                    edi.orgao,
                    TO_CHAR(edi.data_limite, 'DD/MM/YYYY') AS data_limite_formatada,
                    data_limite - current_date as dias,
                    substring(edi.objeto, 0, 300) || ' ...' as objeto,
                    mun.municipio || '/' || uf.uf_sigla AS municipio,
                    edi.id
                FROM
                    edital.cotacao_dispensa_credenciamento edi
                LEFT JOIN
                  corporativo.tb_municipio mun ON edi.municipio_id = mun.id 
                LEFT JOIN
                  corporativo.tb_unidade_federacao uf ON mun.uf_id = uf.id   
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY edi.data_limite ";
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

    /**
     * @param $params
     * @throws Exception
     */
    public function salvarCD($params)
    {
        //list($params, $camposNulos) = $this->limparDadosVazios($params);
        $dados = $params;

        // Tratamento dos dados
        $dados['data_limite'] = formata_data_sql($params['data_limite']);

        // Carimbo de alteração
        $dados = $this->getDadosCarimbo($params, $dados);
       
        $id = $this->popularDadosObjeto($dados)->salvar();

        return $id;
    }

    public function excluirCD($id)
    {
        $this->excluirLogicamente($id);
    }
    public function getDadosCompletos($id)
    {
        $sql = "SELECT
                    edi.*,
                    TO_CHAR(edi.data_limite, 'DD/MM/YYYY') AS data_limite_formatada,
                    data_limite - current_date as dias,
                    mun.municipio AS municipio,
                    uf.uf_sigla as uf,
                    COALESCE(esd.esddsc, 'Selecionado') as situacao,
                    edc.id,
                    edi.id as id_edital,
                    categoria,
                    edc.*
                FROM
                    edital.cotacao_dispensa_credenciamento edi
                LEFT JOIN
                    corporativo.tb_municipio mun ON edi.municipio_id = mun.id 
                LEFT JOIN
                    corporativo.tb_unidade_federacao uf ON mun.uf_id = uf.id   
                INNER JOIN 
                    edital.cotacao_dispensa_credenciamentoempresa edc on edc.cdc_id = edi.id
                LEFT JOIN 
                    workflow.documento doc ON doc.docid = edc.docid
                LEFT JOIN 
                    workflow.estadodocumento esd ON esd.esdid = doc.esdid
                 WHERE
                 edc.id = {$id}";
        $dados = $this->carregar($sql);
        return $dados[0];
    }
}
