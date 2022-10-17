<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * Classe de representação da tabela tipoclassificacao do banco de dados.
 *
 * $Id$
 */
abstract class TipoclassificacaoBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'entidade.tipoclassificacao';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('tpcid', null);

    protected $campos        = array('tpcdesc'   => null,
                                     'tpcstatus' => null);

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
                   tpcid,
                   tpcdesc,
                   tpcstatus
                FROM
                    entidade.tipoclassificacao
                WHERE
                    tpcid = ?";

        // 9
        $rs                     = $this->Execute($sql, array($id));
        $this->tpcdesc          = $rs->fields['tpcdesc'];
        $this->tpcstatus        = $rs->fields['tpcstatus'];
        $this->chavePrimaria[1] = $rs->fields['tpcid'];

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





