<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * Classe de representação da tabela entidade do banco de dados.
 *
 * $Id$
 */
abstract class EntidadeBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'entidade.entidade';

    /**
     * Sequence pertencente a tabela entidade.entidade
     * @access protected
     */
    protected $sequence      = 'entidade.entidade_entid_seq';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('entid', null);

    protected $campos        = array('njuid'                => null,
                                     'entnumcpfcnpj'        => null,
                                     'entnome'              => null,
                                     'entemail'             => null,
                                     'entnuninsest'         => null,
                                     'entobs'               => null,
                                     'entstatus'            => null,
                                     'entnumrg'             => null,
                                     'entorgaoexpedidor'    => null,
                                     'entsexo'              => null,
                                     'entdatanasc'          => null,
                                     'entdatainiass'        => null,
                                     'entdatafimass'        => null,
                                     'entnumdddresidencial' => null,
                                     'entnumresidencial'    => null,
    								 'entnumdddcelular' 	=> null,
                                     'entnumcelular'	    => null,
                                     'entnumdddcomercial'   => null,
                                     'entnumramalcomercial' => null,
                                     'entnumcomercial'      => null,
                                     'entnumdddfax'         => null,
                                     'entnumramalfax'       => null,
                                     'entnumfax'            => null,
                                     'tpctgid'              => null,
                                     'tpcid'                => null,
                                     'tplid'                => null,
                                     'tpsid'                => null,
                                     'entcodentsup'         => null,
                                     'entcodent'            => null,
                                     'entunicod'            => null,
                                     'entsig'               => null,
                                     'entdatainclusao'      => null,
                                     'entescolanova'        => null,
                                     'entproep'        		=> null);

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
                   entid,
                   njuid,
                   entnumcpfcnpj,
                   entnome,
                   entemail,
                   entnuninsest,
                   entobs,
                   entstatus,
                   entnumrg,
                   entorgaoexpedidor,
                   entsexo,
                   entdatanasc,
                   entdatainiass,
                   entdatafimass,
                   entnumdddresidencial,
                   entnumresidencial,
                   entnumdddcelular,
                   entnumcelular,
                   entnumdddcomercial,
                   entnumramalcomercial,
                   entnumcomercial,
                   entnumdddfax,
                   entnumramalfax,
                   entnumfax,
                   tpctgid,
                   tpcid,
                   tplid,
                   tpsid,
                   entcodentsup,
                   entcodent,
                   entunicod,
                   entsig,
                   entdatainclusao,
                   entescolanova,
                   entproep
                FROM
                    entidade.entidade
                WHERE
                    entid = ?";

        // 22
        $rs                         = $this->Execute($sql, array($id));
        $this->njuid                = $rs->fields['njuid'];
        $this->entnumcpfcnpj        = $rs->fields['entnumcpfcnpj'];
        $this->entnome              = $rs->fields['entnome'];
        $this->entemail             = $rs->fields['entemail'];
        $this->entnuninsest         = $rs->fields['entnuninsest'];
        $this->entobs               = $rs->fields['entobs'];
        $this->entstatus            = $rs->fields['entstatus'];
        $this->entnumrg             = $rs->fields['entnumrg'];
        $this->entorgaoexpedidor    = $rs->fields['entorgaoexpedidor'];
        $this->entsexo              = $rs->fields['entsexo'];
        $this->entdatanasc          = $rs->fields['entdatanasc'];
        $this->entdatainiass        = $rs->fields['entdatainiass'];
        $this->entdatafimass        = $rs->fields['entdatafimass'];
        $this->entnumdddresidencial = $rs->fields['entnumdddresidencial'];
        $this->entnumresidencial    = $rs->fields['entnumresidencial'];
        $this->entnumdddcelular     = $rs->fields['entnumdddcelular'];
        $this->entnumcelular	    = $rs->fields['entnumcelular'];
        $this->entnumdddcomercial   = $rs->fields['entnumdddcomercial'];
        $this->entnumramalcomercial = $rs->fields['entnumramalcomercial'];
        $this->entnumcomercial      = $rs->fields['entnumcomercial'];
        $this->entnumdddfax         = $rs->fields['entnumdddfax'];
        $this->entnumramalfax       = $rs->fields['entnumramalfax'];
        $this->entnumfax            = $rs->fields['entnumfax'];
        $this->tpctgid              = $rs->fields['tpctgid'];
        $this->tpcid                = $rs->fields['tpcid'];
        $this->tplid                = $rs->fields['tplid'];
        $this->tpsid                = $rs->fields['tpsid'];
        $this->entcodentsup         = $rs->fields['entcodentsup'];
        $this->entcodent            = $rs->fields['entcodent'];
        $this->entunicod            = $rs->fields['entunicod'];
        $this->entsig               = $rs->fields['entsig'];
        $this->entdatainclusao      = $rs->fields['entdatainclusao'];
        $this->entescolanova        = $rs->fields['entescolanova'];
        $this->entproep        		= $rs->fields['entproep'];
        $this->chavePrimaria[1]     = $rs->fields['entid'];

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





