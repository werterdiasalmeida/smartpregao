<?php
//
// $Id$
//

require_once "base/EntidadeDetalheBase.php";

class EntidadeDetalhe extends EntidadeDetalheBase {
    /**
     * 
     */
    static public function atualizar($entidade, array $entidadedetalhe)
    {
        try {
            $detalhe = new EntidadeDetalhe();
            $detalhe->BeginTransaction();

            $sql = 'DELETE FROM
                        entidade.entidadedetalhe
                    WHERE
                        entcodent = ?
                        or entid  = ?';

            $res = EntidadeDetalhe::ExecSQL($sql, array($entidade->entcodent,
                                                        $entidade->getPrimaryKey()));

            $sql = 'INSERT INTO entidade.entidadedetalhe
                    (
                        entid,
                        entcodent,
                        entdreg_infantil_preescola,
                        entdreg_fund_8_anos,
                        entdreg_fund_9_anos,
                        entdreg_medio_medio,
                        entdreg_medio_integrado,
                        entdreg_medio_normal,
                        entdreg_medio_prof
                    ) VALUES (?, ?, 1, 1, 1, 1, 1, 1, 1)';

            $res = EntidadeDetalhe::ExecSQL($sql, array($entidade->getPrimaryKey(),
                                                        $entidade->entcodent));

            $detalhe->Commit();
        } catch (Exception $e) {
            $detalhe->Rollback();
        }
    }
}





