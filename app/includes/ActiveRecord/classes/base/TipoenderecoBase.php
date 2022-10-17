<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * Classe de representação da tabela tipoendereco do banco de dados.
 *
 * $Id$
 */
abstract class TipoenderecoBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'entidade.tipoendereco';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('', null);

    protected $campos        = array('tpeid'     => null,
                                     'tpedsc'    => null,
                                     'tpestatus' => null);

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
                   tpeid,
                   tpedsc,
                   tpestatus
                FROM
                    entidade.tipoendereco
                --WHERE
                --     = ?";

        // 9
        $rs      = $this->Execute($sql, array($id));
        $this->tpeid     = $rs->fields['tpeid'];
        $this->tpedsc    = $rs->fields['tpedsc'];
        $this->tpestatus = $rs->fields['tpestatus'];

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





