<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * Classe de representação da tabela entidadeendereco do banco de dados.
 *
 * $Id$
 */
abstract class EntidadeEnderecoBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'entidade.entidadeendereco';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array(null, null);

    protected $campos        = array('entid'   => null,
                                     'tpeid'   => null,
                                     'endid'   => null);

    /**
     * Carrega os dados recuperados do banco de dados no objeto.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    public function carregar($id = null)
    {
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





