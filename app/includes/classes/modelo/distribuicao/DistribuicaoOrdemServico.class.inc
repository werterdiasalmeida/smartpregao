<?php

class DistribuicaoOrdemServico extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "distribuicao.ordemservico";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ordid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
                                        'ordid' => null,
                                        'ordnum' => null,
                                        'orddata' => null,
                                        'ordqtdcarros' => null,
                                        'ordstatus' => null,
                                        'orddatacadastro' => null,
                                        'ordobservacao' => null,
                                        'usucpf' => null,
                                        'docid' => null
									  );

    public function listar($where = array()){
        global $db;

        $where[] = "ord.ordstatus = 'A'";
        if(!$db->testa_superuser()){
            $where[] = "ord.usucpf = '{$_SESSION['usucpf']}'";
        }

        $where = implode(' AND ', $where);

        $sql = "SELECT
                    ord.ordnum,
                    to_char(ord.orddata, 'DD/MM/YYYY') AS orddata,
                    ord.ordqtdcarros,
                    esd.esddsc,
                    ord.ordid
                FROM
                    {$this->stNomeTabela} ord
                INNER JOIN
                    workflow.documento doc ON doc.docid = ord.docid
                INNER JOIN
                    workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE
                    {$where}";
        $dados = $this->carregar($sql);
        return $dados ? $dados : array();
    }

}