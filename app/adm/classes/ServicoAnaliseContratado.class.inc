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
                    COALESCE(aed.aeddscrealizada, 'Selecionado') || ' ('|| TO_CHAR(hst.htddata, 'DD/MM/YYYY HH24:MI:SS') ||')' as situacao,
                    edc.id,
                    doc.docid
                FROM
                    edital.edital edi
                INNER JOIN
                  corporativo.tb_municipio mun ON edi.municipio_id = mun.id 
                INNER JOIN
                  corporativo.tb_unidade_federacao uf ON mun.uf_id = uf.id
                INNER JOIN 
                    edital.editalcliente edc on edc.edital_id = edi.id
                INNER JOIN 
                    workflow.documento doc ON doc.docid = edc.docid
                INNER JOIN 
                    workflow.estadodocumento esd ON esd.esdid = doc.esdid
                INNER JOIN     
                    workflow.historicodocumento hst ON hst.docid = doc.docid
                INNER JOIN 
					workflow.acaoestadodoc aed ON aed.aedid = hst.aedid    
                 WHERE
                  " . implode(' AND ', $arWhere);
        //ver($sql);
        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY hst.htddata DESC";
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
