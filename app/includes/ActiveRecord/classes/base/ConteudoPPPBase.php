<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * Classe de representação da tabela funcao do banco de dados.
 *
 * $Id$
 */
abstract class ConteudoPPPBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'cte.conteudoppp';

    /**
     * Sequence pertencente a tabela entidade.funcao
     * @access protected
     */
    protected $sequence      = 'cte.conteudoppp_cppid_seq';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('cppid', null);

    protected $campos        = array('ippid'    => null,
                                     'entid'    => null,
                                     'cpptexto' => null,
                                     'arqid'    => null);

    /**
     * Carrega os dados recuperados do banco de dados no objeto.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    public function carregar($id = null)
    {
        if ($id === null)
            return false;

        $sql = "SELECT
                    cppid,
                    ippid,
                    entid,
                    cpptexto,
                    arqid
                FROM
                    cte.conteudoppp
                WHERE
                    cppid = ?";

        $rs                     = $this->Execute($sql, array($id));

        $this->ippid            = $rs->fields['ippid'];
        $this->entid            = $rs->fields['entid'];
        $this->cpptexto         = $rs->fields['cpptexto'];
        $this->arqid            = $rs->fields['arqid'];
        $this->chavePrimaria[1] = $rs->fields['cppid'];

        return clone $this;
    }


    /**
     * 
     */
    public function setPrimaryKey($valor)
    {
        return $this->chavePrimaria[1] = $valor;
    }


    /**
     * 
     */
    public function getPrimaryKey()
    {
        return $this->chavePrimaria[1];
    }
}





