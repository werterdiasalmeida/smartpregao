<?php
//
// $Id$
//

require_once "base/ConteudoPPPBase.php";

class ConteudoPPP extends ConteudoPPPBase {
    public $arquivo = null;


    /**
     * 
     */
    static public function carregarPorItemPPPEntidade(ItemPPP $itemPPP, $entid)
    {
        $sql = "SELECT
                    c.cppid
                FROM
                    cte.conteudoppp c
                LEFT JOIN
                    cte.itensppp i
                ON
                    c.ippid = i.ippid
                WHERE
                    c.ippid = ? AND
                    c.entid = ?";

        $rs = ActiveRecord::ExecSQL($sql, array($itemPPP->getPrimaryKey(), $entid));

        return new ConteudoPPP($rs->fields['cppid']);
    }


    /**
     * 
     */
    static public function carregarPorEntidade($entid)
    {
        $item   = new ItemPPP();
        $arvore = $item->carregarArvore();
        $result = array();

        foreach ($arvore as $item) {
            $result[] = array('ipptitulo'       => $item->ipptitulo,
                              'ippdsc'          => $item->ippdsc,
                              'ipppai'          => $item->ipppai,
                              'ipptiporesposta' => $item->ipptiporesposta,
                              'cpptexto'        => self::carregarPorItemPPPEntidade($item, $entid)->cpptexto);
        }

        return $result;
    }


    public function carregarArquivo($arqid = null)
    {
        if ($arqid === null)
            $arqid = $this->arqid;

        $this->arquivo = new Arquivo();

        if ($arqid != null)
            $this->arquivo->carregar($arqid);
    }
}





