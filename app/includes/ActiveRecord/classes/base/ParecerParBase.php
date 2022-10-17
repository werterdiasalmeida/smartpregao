<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * 
 */
class ParecerParBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'cte.parecerpar';

    /**
     * Sequence pertencente a tabela entidade.funcao
     * @access protected
     */
    protected $sequence      = 'cte.parecerpar_parid_seq';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('parid', null);

    protected $campos        = array('partexto' => null,
                                     'arqid'    => null,
                                     'usucpf'   => null,
                                     'tppid'    => null,
                                     'pardata'  => null);


    /**
     * Carrega os dados recuperados do banco de dados no objeto.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    public function carregar($id = null)
    {
        $sql = "SELECT
                    parid,
                    partexto,
                    arqid,
                    usucpf,
                    tppid,
                    pardata
                FROM
                    cte.parecerpar
                WHERE
                    parid = ?";

        $rs                     = $this->Execute($sql, array($id));

        $this->partexto         = $rs->fields['partexto'];
        $this->arqid            = $rs->fields['arqid'];
        $this->usucpf           = $rs->fields['usucpf'];
        $this->tppid            = $rs->fields['tppid'];
        $this->pardata          = $rs->fields['pardata'];

        $this->chavePrimaria[1] = $rs->fields['parid'];

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





