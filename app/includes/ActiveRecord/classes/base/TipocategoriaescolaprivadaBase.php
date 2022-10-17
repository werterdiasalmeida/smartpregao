<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * Classe de representação da tabela tipocategoriaescolaprivada do banco de dados.
 *
 * $Id$
 */
abstract class TipocategoriaescolaprivadaBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'entidade.tipocategoriaescolaprivada';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('', null);

    protected $campos        = array('tpctgid'     => null,
                                     'tpctgdesc'   => null,
                                     'tpctgstatus' => null);

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
                   tpctgid,
                   tpctgdesc,
                   tpctgstatus
                FROM
                    entidade.tipocategoriaescolaprivada
                --WHERE
                --     = ?";

        // 11
        $rs        = $this->Execute($sql, array($id));
        $this->tpctgid     = $rs->fields['tpctgid'];
        $this->tpctgdesc   = $rs->fields['tpctgdesc'];
        $this->tpctgstatus = $rs->fields['tpctgstatus'];

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





