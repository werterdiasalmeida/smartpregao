<?php
//
// $Id$
//

require_once "base/AreaCursoBase.php";

class AreaCurso extends AreaCursoBase {
    /**
     * 
     */
    public $cursos = array();


    /**
     * 
     */
    public function carregarCursosTecnicos()
    {
        $sql = "SELECT
                    crsid
                FROM
                    cte.cursotecnico c
                INNER JOIN
                    cte.areacurso a
                ON
                    c.areid = a.areid
                WHERE
                    c.areid = ?";

        $res = $this->Execute($sql, array($this->getPrimaryKey()));

        while (!$res->EOF) {
            $this->cursos[] = new CursoTecnico($res->fields['crsid']);
            $res->movenext();
        }

        return $this->cursos;
    }


    /**
     * 
     */
    public function buscarPorDescricao($actdsc)
    {
        $arr = array();
        $dsc = str_replace(" ", "%", $actdsc);
        $sql = "SELECT
                    areid,
                    aredsc
                FROM
                    cte.areacurso
                WHERE
                    aredsc LIKE ?";

        $res = $this->Execute($sql, array($dsc));

        while (!$res->EOF) {
            $arr = new AreaCurso($res->fields['areid']);
            $res->movenext();
        }

        return $arr;
    }
}





