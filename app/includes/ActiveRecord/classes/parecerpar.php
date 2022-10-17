<?php
//
// $Id$
//

require_once 'base/ParecerParBase.php';

/**
 * 
 */
class ParecerPar extends ParecerParBase
{
    /**
     * 
     */
    static public function carregarParecerParPorInuid($inuid, $tppid)
    {
        $sql = 'SELECT
                    pp.parid
                FROM
                    cte.parecerpar pp
                INNER JOIN
                    cte.parecerinstrumento pi
                ON
                    pp.parid = pi.parid
                WHERE
                    pi.inuid = ?
                    and
                    pp.tppid = ?';

        $rs = ParecerPar::ExecSQL($sql, array($inuid, $tppid));
        return new ParecerPar($rs->fields['parid']);
    }


    /**
     * 
     */
    static public function carregarArquivoParecerParPorInuid($inuid, $tppid)
    {
        require_once 'Arquivo.php';
        return new Arquivo(ParecerPar::carregarParecerParPorInuid($inuid, $tppid)->arqid);
    }


    /**
     * 
     */
    public function carregarArquivo()
    {
        require_once 'Arquivo.php';
        return new Arquivo($this->arqid);
    }


    /**
     * 
     */
    final public function salvarParecerInstrumento()
    {
    }
}





