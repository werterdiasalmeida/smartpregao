<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * Classe de representação da tabela endereco do banco de dados.
 *
 * $Id$
 */
abstract class EnderecoBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'entidade.endereco';

    /**
     * Sequence pertencente a tabela entidade.endereco
     * @access protected
     */
    protected $sequence      = 'entidade.endereco_endid_seq';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('endid', null);

    protected $campos        = array('endcep'       => null,
                                     'endlog'       => null,
                                     'endcom'       => null,
                                     'endbai'       => null,
                                     'muncod'       => null,
                                     'estuf'        => null,
                                     'endnum'       => null,
                                     'endstatus'    => null,
                                     'medlatitude'  => null,
                                     'medlongitude' => null,
    								 'endzoom' 		=> null,
                                     'endcomunidade' => null);

    protected $mundescricao  = null;

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
                   endid,
                   endcep,
                   endlog,
                   endcom,
                   endbai,
                   muncod,
                   estuf,
                   endnum,
                   endstatus,
                   medlatitude,
                   medlongitude,
                   endcomunidade,
                   endzoom
                FROM
                    entidade.endereco
                WHERE
                    endid = ?";

        // 22
        $rs                     = $this->Execute($sql, array($id));
        $this->endcep           = $rs->fields['endcep'];
        $this->endlog           = $rs->fields['endlog'];
        $this->endcom           = $rs->fields['endcom'];
        $this->endbai           = $rs->fields['endbai'];
        $this->muncod           = $rs->fields['muncod'];
        $this->estuf            = $rs->fields['estuf'];
        $this->endnum           = $rs->fields['endnum'];
        $this->endstatus        = $rs->fields['endstatus'];
        $this->medlatitude      = $rs->fields['medlatitude'];
        $this->medlongitude     = $rs->fields['medlongitude'];
        $this->endzoom			= $rs->fields['endzoom'];
        $this->chavePrimaria[1] = $rs->fields['endid'];
        $this->endcomunidade    = $rs->fields['endcomunidade'];

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


    /**
     * 
     */
    public function getMunDescricao()
    {
        if ($this->mundescricao === null)
            $this->_carregarMunDescricao();

        return $this->mundescricao;
    }


    /**
     * 
     */
    private function _carregarMunDescricao()
    {
        if ($this->muncod == null)
            return null;

        $this->mundescricao = $this->Execute("SELECT
                                                mundescricao
                                              FROM
                                                territorios.municipio
                                              WHERE
                                                muncod = '" . $this->muncod . "' ")->fields['mundescricao'];
    }
}





