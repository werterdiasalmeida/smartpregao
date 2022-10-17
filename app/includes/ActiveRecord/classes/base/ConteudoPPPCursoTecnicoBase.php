<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * 
 */
abstract class ConteudoPPPCursoTecnicoBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'cte.conteudopppcursotecnico';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array(null, null);

    protected $campos        = array('crsid' => null,
                                     'cppid' => null);


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





