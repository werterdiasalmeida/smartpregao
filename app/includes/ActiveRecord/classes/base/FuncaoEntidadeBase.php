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
abstract class FuncaoEntidadeBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'entidade.funcaoentidade';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array(null, null);

    protected $campos        = array('funid'     => null,
                                     'entid'     => null,
                                     'fuedata'   => null,
                                     'fuestatus' => null);


    /**
     * Carrega os dados recuperados do banco de dados no objeto.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    public function carregar($id = null)
    {
        if (!is_array($id) || sizeof($id) != 2)
            return $this;

        $sql = "SELECT
                   funid,
                   entid,
                   fuedata,
                   funstatus
                FROM
                    entidade.funcaoentidade
                WHERE
                    entid = ? AND funid = ?";

        // 22
        $rs              = $this->Execute($sql, $id);
        $this->funid     = $rs->fields['funid'];
        $this->entid     = $rs->fields['entid'];
        $this->fuedata   = $rs->fields['fuedata'];
        $this->funstatus = $rs->fields['fuestatus'];

        return clone $this;
    }


    /**
     * 
     */
    public function setPrimaryKey($valor)
    {
        return null;
    }


    /**
     * 
     */
    public function getPrimaryKey()
    {
        return null;
    }
}





