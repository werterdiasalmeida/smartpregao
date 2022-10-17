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
abstract class FuncaoBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'entidade.funcao';

    /**
     * Sequence pertencente a tabela entidade.funcao
     * @access protected
     */
    protected $sequence      = 'entidade.funcao_funid_seq';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('funid', null);

    protected $campos        = array('funtipo'   => null,
                                     'fundsc'    => null,
                                     'funstatus' => null);

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
                   funtipo,
                   funid,
                   fundsc,
                   funstatus
                FROM
                    entidade.funcao
                WHERE
                    funid = ?";

        // 22
        $rs                     = $this->Execute($sql, array($id));
        $this->funtipo          = $rs->fields['funtipo'];
        $this->fundsc           = $rs->fields['fundsc'];
        $this->funstatus        = $rs->fields['funstatus'];
        $this->chavePrimaria[1] = $rs->fields['funid'];

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





