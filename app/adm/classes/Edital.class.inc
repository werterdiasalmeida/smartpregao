<?php

class Edital extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "edital.edital";

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
        'comentarios' => null,
        'link_g_drive' => null,
        'municipio_id' => null,
        'habilitacao_tecnica' => null,
        'resumo_servico' => null,
        'valor_estimado' => null,
        'temperatura' => null,
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
                    edi.portal,
                    edi.orgao,
                    TO_CHAR(edi.data_limite, 'DD/MM/YYYY') AS data_limite_formatada,
                    data_limite - current_date as dias,
                    edi.objeto,
                    mun.municipio || '/' || uf.uf_sigla AS municipio,
                    edi.id
                FROM
                    edital.edital edi
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
    public function salvarEdital($params)
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

    public function excluirEdital($id)
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
                    edi.id as id_edital
                FROM
                    edital.edital edi
                LEFT JOIN
                    corporativo.tb_municipio mun ON edi.municipio_id = mun.id 
                LEFT JOIN
                    corporativo.tb_unidade_federacao uf ON mun.uf_id = uf.id   
                INNER JOIN 
                    edital.editalcliente edc on edc.edital_id = edi.id
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
