<?php

class VwCid10 extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cid.vw_cid10";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("codigo");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'codigo' => null,
        'descricao' => null,
        'descricao_abreviada' => null,
        'classificacao' => null,
        'referencia' => null,
        'excluidos' => null,
        'restricao_sexo' => null,
        'causa_obito' => null,
        'codigo_formatado' => null
    );

    public function getListByCampoDeBusca($busca, $retonrarJson = true) {
        $busca = trim(addslashes(utf8_decode($busca)));
        $pessoas = array();
        if($busca){
            $arWhere = array(
                "codigo ILIKE '%{$busca}%'",
                "descricao ILIKE '%{$busca}%'",
                "descricao_abreviada ILIKE '%{$busca}%'"
            );

            $cids = $this->getList(array('(' . implode(' OR ', $arWhere) . ')'), false, null, 10);
        }

        return $retonrarJson ? json_encode(utf8ArrayEncode(array('data' => $cids))): $cids;
    }

    public function getList($arWhere = array(), $paginacao = true, $order = null, $limit = null)
    {
        $arWhere = is_array($arWhere) ? $arWhere : array();

        $sql = "SELECT 
                  *
                FROM
                  {$this->stNomeTabela}
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY 1 ASC";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 15);
            return $dado;
        } else {
            if($limit && is_numeric($limit)){
                $sql .= " LIMIT {$limit}";
            }

            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }
}