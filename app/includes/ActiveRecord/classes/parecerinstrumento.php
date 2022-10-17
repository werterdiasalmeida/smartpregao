<?php
//
// $Id$
//

require_once 'base/parecerinstrumentobase.php';

/**
 * 
 */
class ParecerInstrumento extends ParecerInstrumentoBase {
    static public function adicionar($parecer, $inuid)
    {
        $pi = new ParecerInstrumento();

        if ($parecer instanceof ParecerPar) {
            $pi->parid = $parecer->getPrimaryKey();
        } else {
            $pi->parid = $parecer;
        }

        $pi->inuid = $inuid;
        return $pi->salvar();
    }


    static public function remover($parecer, $inuid)
    {
        $pi = new ParecerInstrumento();

        if ($parecer instanceof ParecerPar) {
            $parid = $parecer->getPrimaryKey();
        } else {
            $parid = $parecer;
        }

        $sql = 'DELETE FROM
                    cte.parecerinstrumento
                WHERE
                    parid = ?
                    AND
                    inuid = ?';

        return ParecerInstrumento::ExecSQL($sql, array($parid, $inuid));
    }
}





