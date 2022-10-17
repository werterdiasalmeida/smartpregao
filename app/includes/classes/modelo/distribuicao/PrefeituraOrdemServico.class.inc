<?php

class PrefeituraOrdemServico extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "distribuicao.prefeituraordemservico";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "proid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
                                        'proid' => null,
                                        'preid' => null,
                                        'ordid' => null,
                                        'proqtdcarros' => null
									  );

    public function removeByOs($ordid){
        $sql = "DELETE FROM {$this->stNomeTabela} WHERE ordid = {$ordid}";
        $this->executar($sql);
    }

    public function getPrefeiturasByOs($ordid){
        $sql = "SELECT
                    pre.preid,
                    pre.prenome,
                    pro.proqtdcarros
                FROM
                    distribuicao.prefeitura pre
                INNER JOIN
                    {$this->stNomeTabela} pro ON pro.preid = pre.preid
                 WHERE
                    pro.ordid = {$ordid}";

        $dados = $this->carregar($sql);

        return $dados ? $dados : array();

    }

}