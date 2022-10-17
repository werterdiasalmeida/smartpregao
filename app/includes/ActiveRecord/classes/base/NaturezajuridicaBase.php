<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * Classe de representação da tabela naturezajuridica do banco de dados.
 *
 * $Id$
 */
abstract class NaturezajuridicaBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'entidade.naturezajuridica';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('', null);

    protected $campos        = array('njuid'    => null,
                                     'njudsc'   => null,
                                     'njugrupo' => null);

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
                   njuid,
                   njudsc,
                   njugrupo
                FROM
                    entidade.naturezajuridica
                --WHERE
                --     = ?";

        // 8
        $rs     = $this->Execute($sql, array($id));
        $this->njuid    = $rs->fields['njuid'];
        $this->njudsc   = $rs->fields['njudsc'];
        $this->njugrupo = $rs->fields['njugrupo'];

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





