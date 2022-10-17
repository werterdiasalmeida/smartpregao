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
abstract class ItemPPPBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'cte.itensppp';

    /**
     * Sequence pertencente a tabela entidade.funcao
     * @access protected
     */
    protected $sequence      = 'cte.itensppp_ippid_seq';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('ippid', null);

    protected $campos        = array('ipppai'          => null,
                                     'ippdsc'          => null,
                                     'ippconteudo'     => null,
                                     'ippordem'        => null,
                                     'ipptiporesposta' => null,
                                     'ipptitulo'       => null,
    								 'ippbloqdifept'   => null);

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
                    ippid,
                    ipppai,
                    ippdsc,
                    ippconteudo,
                    ippordem,
                    ipptitulo,
                    ipptiporesposta,
                    ippbloqdifept
                FROM
                    cte.itensppp
                WHERE
                    ippid = ?";

        $rs                     = $this->Execute($sql, array($id));
        $this->ipppai           = $rs->fields['ipppai'];
        $this->ippdsc           = $rs->fields['ippdsc'];
        $this->ippconteudo      = $rs->fields['ippconteudo'];
        $this->ippordem         = $rs->fields['ippordem'];
        $this->ipptitulo        = $rs->fields['ipptitulo'];
        $this->ipptiporesposta  = $rs->fields['ipptiporesposta'];
        $this->ippbloqdifept  	= $rs->fields['ippbloqdifept'];
        $this->chavePrimaria[1] = $rs->fields['ippid'];

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





