<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * Classe de representação da tabela tiposituacao do banco de dados.
 *
 * $Id$
 */
abstract class TiposituacaoBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'entidade.tiposituacao';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('tpsid', null);

    protected $campos        = array('tpscod'    => null,
                                     'tpsdsc'    => null,
                                     'tpsdesc'   => null,
                                     'tpsstatus' => null,
                                     'tpscor'    => null);

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
                   tpscod,
                   tpsid,
                   tpsdsc,
                   tpsdesc,
                   tpsstatus,
                   tpscor
                FROM
                    entidade.tiposituacao
                WHERE
                    tpsid = ?";

        // 9
        $rs              = $this->Execute($sql, array($id));
        $this->tpscod    = $rs->fields['tpscod'];
        $this->tpsdsc    = $rs->fields['tpsdsc'];
        $this->tpsdesc   = $rs->fields['tpsdesc'];
        $this->tpsstatus = $rs->fields['tpsstatus'];
        $this->tpscor    = $rs->fields['tpscor'];
        $this->tpsid     = $this->chavePrimaria[1];

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





