<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * Classe de representação da tabela tipolocalizacao do banco de dados.
 *
 * $Id$
 */
abstract class TipolocalizacaoBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'entidade.tipolocalizacao';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('tplid', null);

    protected $campos        = array('tpldesc'   => null,
                                     'tplstatus' => null);

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
                   tplid,
                   tpldesc,
                   tplstatus
                FROM
                    entidade.tipolocalizacao
                WHERE
                    tplid = ?";

        // 9
        $rs                     = $this->Execute($sql, array($id));
        $this->tpldesc          = $rs->fields['tpldesc'];
        $this->tplstatus        = $rs->fields['tplstatus'];
        $this->chavePrimaria[1] = $rs->fields['tplid'];

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





