<?php
//
// $Id$
//

require_once "base/EntidadeBase.php";

class Entidade extends EntidadeBase {
    public $enderecos = array();
    public $funcoes   = array();


    /**
     * 
     */
    public function carregar($id = null)
    {
        if (parent::carregar($id) === false || $this->getPrimaryKey() === null)
            return clone $this;

        $sql = "SELECT
                   to_char(entdatanasc, 'dd/mm/YYYY') as entdatanasc,
                   to_char(entdatainiass, 'dd/mm/YYYY') as entdatainiass,
                   to_char(entdatafimass, 'dd/mm/YYYY') as entdatafimass,
                   to_char(entdatainclusao, 'dd/mm/YYYY') as entdatainclusao
                FROM
                    entidade.entidade
                WHERE
                    entid = ?";

        // 22
        $rs                    = $this->Execute($sql, array($this->getPrimaryKey()));
        $this->entdatanasc     = $rs->fields['entdatanasc'];
        $this->entdatainiass   = $rs->fields['entdatainiass'];
        $this->entdatafimass   = $rs->fields['entdatafimass'];
        $this->entdatainclusao = $rs->fields['entdatainclusao'];

        return clone $this;
    }


    /**
     * 
     */
    public function carregarEnderecos()
    {
        $this->enderecos = Endereco::carregarEnderecosPorEntidade($this->getPrimaryKey());
        return $this->enderecos;
    }


    /**
     * 
     */
    public function carregarFuncoes()
    {
        $this->funcoes   = FuncaoEntidade::carregarPorEntidade($this->getPrimaryKey());
        return $this->funcoes;
    }


    /**
     * 
     */
    static public function carregarEntidadesPorFuncao($fundsc, $estuf)
    {
        $sql = "SELECT
                    funid
                FROM
                    entidade.funcao
                WHERE
                    fundsc = ?";

        $fun = ActiveRecord::ExecSQL($sql, array($fundsc));

        $sql = "SELECT DISTINCT
                    fe.entid
                FROM
                    entidade.funcaoentidade fe
                INNER JOIN
                    entidade.endereco en
                ON
                    fe.entid = en.entid
                INNER JOIN
                    entidade.funcao fu
                ON
                    fe.funid = fu.funid
                WHERE
                    fe.funid         = ?
                    AND fe.fuestatus = 'A'
                    AND en.estuf     = ?
                GROUP BY
                    fe.entid";

        $arr = array();
        while (!$fun->EOF) {
            $set = ActiveRecord::ExecSQL($sql, array($fun->fields['funid'], $estuf));

            while (!$set->EOF) {
                $arr[] = new Entidade($set->fields['entid']);
                $set->MoveNext();
            }

            $fun->MoveNext();
        }

        return $arr;
    }


    /**
     * 
     */
    static public function carregarEntidadePorCnpjCpf($entnumcpfcnpj, $inativo = false)
    {
        $num = (string) str_replace(array('.', '/', '-'), '', $entnumcpfcnpj);
        $sql = 'SELECT entid FROM entidade.entidade WHERE entnumcpfcnpj = ?'
             . ($inativo === false ? ' AND entstatus = \'A\'' : '');

        $res = ActiveRecord::ExecSQL($sql, array($num));

        if ($res->numRows() > 0) {
            return new Entidade($res->fields['entid']);
        } else {
            return new Entidade();
        }
    }


    /**
     * 
     */
    static public function carregarEntidadePorEntcodent($entcodent, $inativo = false)
    {
        $sql = 'SELECT entid FROM entidade.entidade WHERE entcodent = ?'
             . ($inativo === false ? ' AND entstatus = \'A\'' : '');

        $res = ActiveRecord::ExecSQL($sql, array(strtoupper($entcodent)));

        if ($res->numRows() > 0) {
            return new Entidade($res->fields['entid']);
        } else {
            return new Entidade();
        }
    }


    /**
     * 
     */
    static public function carregarEntidadePorEntunicod($entunicod, $inativo = false)
    {
        $sql = 'SELECT entid FROM entidade.entidade WHERE entunicod = ?'
             . ($inativo === false ? ' AND entstatus = \'A\'' : '');

        $res = ActiveRecord::ExecSQL($sql, array(strtoupper($entunicod)));

        if ($res->numRows() > 0) {
            return new Entidade($res->fields['entid']);
        } else {
            return new Entidade();
        }
    }


    /**
     * 
     */
    static public function carregarEntidadePorEntidassociado($entidassociado, $inativo = false)
    {
        $sql = 'SELECT entid FROM entidade.entidade WHERE entid = ?'
             . ($inativo === false ? ' AND entstatus = \'A\'' : '');

        $res = ActiveRecord::ExecSQL($sql, array($entidassociado));

        if ($res->numRows() > 0) {
            return new Entidade($res->fields['entid']);
        } else {
            return new Entidade();
        }
    }

    /**
     * 
     */
    static public function carregarEntidassociadoPorEntidade($entid, $funid = false, $inativo = false)
    {
        $sql = 'SELECT ent.entid FROM entidade.entidade ent 
        		LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
        		LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
        		WHERE fea.entid = ?'
             . ($inativo === false ? ' AND ent.entstatus = \'A\'' : '') . ($funid !== false ? ' AND fen.funid = \''.$funid.'\'' : '');
        $res = ActiveRecord::ExecSQL($sql, array($entid));

        if ($res->numRows() > 0) {
            return new Entidade($res->fields['entid']);
        } else {
            return new Entidade();
        }
    }
    

    /**
     * 
     */
    static public function carregarPrefeito(Entidade $prefeitura)
    {
        $sql = 'SELECT entid FROM entidade.entidade WHERE entidassociado = ?';

        $res = ActiveRecord::ExecSQL($sql, array($prefeitura->getPrimaryKey()));

        if ($res->numRows() > 0) {
            return new Entidade($res->fields['entid']);
        } else {
            return new Entidade();
        }
    }
}





