<?php

class ServicoAnaliseContratado extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "servicoanalise.servico_contratado";

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
       
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "edi.excluido IS FALSE and edc.docid is not null and esd.esdid <> 86";

        $sql = "SELECT
                    edi.orgao || '( '||edi.edital || ')' as edital,
                    COALESCE(esd.esddsc, 'Selecionado') as situacao,
                    edc.id
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
                  " . implode(' AND ', $arWhere);
        //ver($sql);
        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY edi.data_limite DESC";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 7);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    
}
