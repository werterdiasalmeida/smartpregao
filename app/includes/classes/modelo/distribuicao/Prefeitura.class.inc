<?php

class Prefeitura extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "distribuicao.prefeitura";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "preid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'preid' => null,
                                        'prenome' => null,
                                        'prerazaosocial' => null,
                                        'preendereco' => null,
                                        'pretelefone' => null,
                                        'preemail' => null,
                                        'precnpj' => null,
                                        'preprefeitonome' => null,
                                        'preprefeitorg' => null,
                                        'preprefeitocpf' => null,
                                        'preprefeitoemail' => null,
                                        'arqidprefeitorg' => null,
                                        'arqidprefeitocpf' => null,
                                        'arqidprefeitocomprovante' => null,
                                        'prerepresentantenome' => null,
                                        'prerepresentanterg' => null,
                                        'prerepresentantecpf' => null,
                                        'prerepresentantecargo' => null,
                                        'prerepresentanteemail' => null,
                                        'prerepresentantetelefoneres' => null,
                                        'prerepresentantetelefonefun' => null,
                                        'prerepresentantetelefonecel' => null,
                                        'arqidrepresentanterg' => null,
                                        'arqidrepresentantecpf' => null,
                                        'arqidrepresentantenomeacao' => null,
                                        'arqidrepresentantedesignacao' => null,
                                        'docid' => null,
                                        'usucpf' => null,
                                        'predatacadastro' => null,
                                        'prestatus' => null
									  );


    public function listar($where = array()){
        global $db;

        $where[] = "pre.prestatus = 'A'";

        if(!$db->testa_superuser()){
            $where[] = "pre.usucpf = '{$_SESSION['usucpf']}'";
        }

        $sql = "SELECT
                    pre.precnpj,
                    pre.prenome,
                    pre.pretelefone,
                    pre.preemail,
                    pre.preprefeitonome,
                    pre.preprefeitoemail,
                    pre.prerepresentantenome,
                    pre.prerepresentantetelefonecel,
                    pre.prerepresentanteemail,
                    esd.esddsc,
                    pre.preid
                FROM
                    {$this->stNomeTabela} pre
                LEFT JOIN
                    workflow.documento doc ON doc.docid = pre.docid
                LEFT JOIN
                    workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE "
                . implode(" AND ", $where);

        $rs = $this->carregar($sql);
        return $rs ? $rs : array();
    }

    public function getByCnpj($cnpj){

        $where[] = "pre.precnpj = '{$cnpj}'";
        $where[] = "pre.prestatus = 'A'";
        $where[] = "doc.esdid    != " . WF_PREFEITURA_REPROVADO;

        $sql = "SELECT
                    pre.precnpj,
                    pre.prenome,
                    pre.preid
                FROM
                    {$this->stNomeTabela} pre
                LEFT JOIN
                    workflow.documento doc ON doc.docid = pre.docid
                LEFT JOIN
                    workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE "
                . implode(" AND ", $where);

        return $this->pegaLinha($sql);
    }

    public function getComboPrefeiturasAprovadasSemOs(){
        $sql = "SELECT
                    pre.preid AS codigo,
                    pre.prenome AS descricao
                FROM
                    {$this->stNomeTabela} pre
                LEFT JOIN
                    workflow.documento doc ON doc.docid = pre.docid
                WHERE
                    pre.prestatus = 'A'
                    AND
                    pre.preid NOT IN (SELECT
                                          preid
                                      FROM
                                          distribuicao.prefeituraordemservico pro
                                      INNER JOIN
                                          distribuicao.ordemservico ord ON ord.ordid = pro.ordid
                                      WHERE
                                          ord.ordstatus = 'A')
                    AND
                    doc.esdid = " . WF_PREFEITURA_APROVADO;

        $rs = $this->carregar($sql);
        return $rs ? $rs : array();
    }

    public function getComboPrefeiturasAprovadasComOs(){
        $sql = "SELECT
                    pre.preid AS codigo,
                    pre.prenome AS descricao
                FROM
                    {$this->stNomeTabela} pre
                LEFT JOIN
                    workflow.documento doc ON doc.docid = pre.docid
                WHERE
                    pre.prestatus = 'A'
                    AND
                    pre.preid IN (SELECT
                                      preid
                                  FROM
                                      distribuicao.prefeituraordemservico pro
                                  INNER JOIN
                                      distribuicao.ordemservico ord ON ord.ordid = pro.ordid
                                  WHERE
                                      ord.ordstatus = 'A')
                    AND
                    doc.esdid = " . WF_PREFEITURA_APROVADO;

        $rs = $this->carregar($sql);
        return $rs ? $rs : array();
    }

}