<?php
//
// $Id$
//

require_once "base/EntidadeEnderecoBase.php";

class EntidadeEndereco extends EntidadeEnderecoBase {
    /**
     * 
     */
    public function carregar($id = null)
    {
        $arg = func_get_args();

        $sql = "SELECT
                   entid,
                   endid,
                   tpeid
                FROM
                    entidade.entidadeendereco
                WHERE
                    entid = ?
                    AND
                    endid = ?";

        $res = $this->Execute($sql, array($arg[0]->getPrimaryKey(),
                                          $arg[1]->getPrimaryKey()));

        $this->tpeid = $res->fields['tpeid'];
        $this->endid = $res->fields['endid'];
        $this->entid = $res->fields['entid'];

        return clone $this;
    }


    /**
     * 
     */
    static public function adicionar(Entidade $entidade, Endereco $endereco)
    {
        $ee = new EntidadeEndereco();
        $ee->carregar($entidade, $endereco);

        if (trim($ee->endid) == '') {
            $sql = "DELETE FROM
                        entidade.entidadeendereco
                    WHERE
                        entid = ?
                        AND
                        endid = ?";

            ActiveRecord::execSQL($sql, array($entidade->getPrimaryKey(),
                                              $endereco->getPrimaryKey()));

            $ee->endid = $endereco->getPrimaryKey();
            $ee->entid = $entidade->getPrimaryKey();
            $ee->tpeid = 1;

            return $ee->salvar();
        }
    }
}





