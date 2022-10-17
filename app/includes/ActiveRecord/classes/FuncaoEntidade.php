<?php
//
// $Id$
//

require_once "base/FuncaoEntidadeBase.php";

class FuncaoEntidade extends FuncaoEntidadeBase {
    static public function carregarPorEntidade($entid)
    {
        $sql = "SELECT
                   funid,
                   entid,
                   fuedata,
                   funstatus
                FROM
                    entidade.funcaoentidade
                WHERE
                    entid = ?";

        $res = ActiveRecord::ExecSQL($sql, array($entid));
        $arr = array();

        while (!$res->EOF) {
            $arr = new FuncaoEntidade(array($rs->fields['entid'],
                                            $rs->fields['funid']));

            $res->MoveNext();
        }

        return $arr;
    }
}





