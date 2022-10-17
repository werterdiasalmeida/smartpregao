<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * 
 */
abstract class AreaCursoBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'cte.areacurso';

    /**
     * Sequence pertencente a tabela entidade.endereco
     * @access protected
     */
    protected $sequence      = 'cte.areacurso_areid_seq';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('areid', null);

    protected $campos        = array('aretitulo' => null,
                                     'aredsc'    => null);


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
                   areid,
                   aretitulo,
                   aredsc
                FROM
                    cte.areacurso
                WHERE
                    areid = ?";

        $rs                     = $this->Execute($sql, array($id));
        $this->aretitulo        = $rs->fields['aretitulo'];
        $this->aredsc           = $rs->fields['aredsc'];
        $this->chavePrimaria[1] = $rs->fields['areid'];

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





