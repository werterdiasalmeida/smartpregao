<?php
//
// $Id$
//

require_once "base/ItemPPPBase.php";

class ItemPPP extends ItemPPPBase {
    //------------------------------------------------------------- properties
    /**
     * 
     */
    protected $arvore = array();


    //----------------------------------------------------------------- public
    /**
     * 
     */
    public function carregarArvore($ipppai = null, array &$node = array())
    {
        if ($ipppai === null) {
            $sql = 'SELECT
                        *
                    FROM
                        cte.itensppp
                    WHERE
                        ipppai IS NULL
                    ORDER BY
                        ippordem';

            $res = $this->Execute($sql);
            while (!$res->EOF) {
                $item                                = new ItemPPP($res->fields['ippid']);
                $this->arvore[$res->fields['ippid']] = $item;

                $item->carregarArvore($item->ippid, $this->arvore);
                $res->movenext();
            }
        } else {
            $sql = 'SELECT
                        *
                    FROM
                        cte.itensppp
                    WHERE
                        ipppai = ?
                    ORDER BY
                        ippordem';

            $res = $this->Execute($sql, array($ipppai));
            while (!$res->EOF) {
                $item                                = new ItemPPP($res->fields['ippid']);
                $this->arvore[$res->fields['ippid']] = $item;

                $item->carregarArvore($item->ippid, $this->arvore);
                $res->movenext();
            }
        }

        return $this->arvore;
    }


    /**
     * 
     */
    static public function carregarFilhos($ipppai = null)
    {
        if ($ipppai === null) {
            $sql = "SELECT
                        ippid
                    FROM
                        cte.itensppp
                    WHERE
                        ipppai is null
                    ORDER BY
                        ippordem";

            $rs  = ActiveRecord::ExecSQL($sql);
        } else {
            $sql = "SELECT
                        ippid
                    FROM
                        cte.itensppp
                    WHERE
                        ipppai = ?
                    ORDER BY
                        ippordem";

            $rs  = ActiveRecord::ExecSQL($sql, array($ipppai));
        }

        $filhos = array();

        while (!$rs->EOF) {
            $filhos[] = new ItemPPP($rs->fields['ippid']);
            $rs->movenext();
        }

        return $filhos;
    }


    final public function getArvore()
    {
        return $this->arvore;
    }


    /**
     * 
     */
    public function anterior()
    {
        $sql = "SELECT
                    ippid
                FROM
                    " . $this->tabela . "
                WHERE
                    ippid = ?";

        $res = $this->Execute($sql, array($this->getPrimaryKey() - 1));

        if ($res->numRows() == 0) {
            return -1;
        }

        $itm = new ItemPPP($res->fields['ippid']);

        if ($itm->ippconteudo == 1)
            return $itm;
        else
            return $itm->anterior();
    }


    public function proximo()
    {
        $sql = "SELECT
                    ippid
                FROM
                    " . $this->tabela . "
                WHERE
                    ippid = ?";

        $res = $this->Execute($sql, array($this->getPrimaryKey() + 1));

        if ($res->numRows() == 0) {
            return -1;
        }

        $itm = new ItemPPP($res->fields['ippid']);

        if ($itm->ippconteudo == 1)
            return $itm;
        else
            return $itm->proximo();
    }


    private function getRoot($ippid, ItemPPP $itemPPP = null)
    {
        static $root = null;

        if ($itemPPP === null)
            $itemPPP = $this;

        $arvore = $itemPPP->getArvore();

        foreach ($arvore as $item) {
            if ($ippid == $item->getPrimaryKey()) {
                if ($root === null)
                    $root = $item->getArvore();

                break;
            } else {
                $this->getRoot($ippid, $item);
            }
        }

        return $root;
    }
}





