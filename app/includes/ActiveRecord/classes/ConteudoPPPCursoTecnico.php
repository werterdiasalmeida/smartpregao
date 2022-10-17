<?php
//
// $Id$
//

require_once "base/ConteudoPPPCursoTecnicoBase.php";

class ConteudoPPPCursoTecnico extends ConteudoPPPCursoTecnicoBase {
    static public function carregarPorConteudoPPP(ConteudoPPP $conteudoPPP)
    {
        $sql = "SELECT
                    cc.crsid,
                    cp.cppid,
                    cr.areid
                FROM
                    cte.conteudopppcursotecnico cc
                INNER JOIN
                    cte.conteudoppp cp
                ON
                    cc.cppid = cp.cppid
                INNER JOIN
                    cte.cursotecnico cr
                ON
                    cc.crsid = cr.crsid
                WHERE
                    cp.cppid = ?
                GROUP BY
                    cr.areid,
                    cp.cppid,
                    cc.crsid
                ORDER BY
                    cp.cppid";

        $arr = array();
        $res = ActiveRecord::ExecSQL($sql, array($conteudoPPP->getPrimaryKey()));

        while (!$res->EOF) {
            $arr[] = array($res->fields['crsid'], $res->fields['cppid']);
            $res->MoveNext();
        }

        return $arr;
    }


    static public function excluirTodosPorConteudoPPP(ConteudoPPP $conteudoPPP)
    {
        $sql = "DELETE FROM
                    cte.conteudopppcursotecnico
                WHERE
                    cppid = ?";

        return ActiveRecord::ExecSQL($sql, array($conteudoPPP->getPrimaryKey()));
    }
}





