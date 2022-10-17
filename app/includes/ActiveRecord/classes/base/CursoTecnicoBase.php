<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * 
 */
abstract class CursoTecnicoBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protecrsd
     */
    protected $tabela        = 'cte.cursotecnico';

    /**
     * Sequence pertencente a tabela entidade.endereco
     * @access protecrsd
     */
    protected $sequence      = 'cte.cursotecnico_crsid_seq';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('crsid', null);

    protected $campos        = array('areid'             => null,
                                     'crstitulo'         => null,
                                     'crscargahoraria'   => null,
                                     'crsdsc'            => null,
                                     'crstema'           => null,
                                     'crsatuacao'        => null,
                                     'crsinfraestrutura' => null);


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
                    crsid,
                    areid,
                    crscargahoraria,
                    crstitulo,
                    crsdsc,
                    crstema,
                    crsatuacao,
                    crsinfraestrutura
                FROM
                    cte.cursotecnico
                WHERE
                    crsid = ?";

        $rs                      = $this->Execute($sql, array($id));
        $this->areid             = $rs->fields['areid'];
        $this->crscargahoraria   = $rs->fields['crscargahoraria'];
        $this->crstitulo         = $rs->fields['crstitulo'];
        $this->crsdsc            = $rs->fields['crsdsc'];
        $this->crstema           = $rs->fields['crstema'];
        $this->crsatuacao        = $rs->fields['crsatuacao'];
        $this->crsinfraestrutura = $rs->fields['crsinfraestrutura'];
        $this->chavePrimaria[1]  = $rs->fields['crsid'];

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





