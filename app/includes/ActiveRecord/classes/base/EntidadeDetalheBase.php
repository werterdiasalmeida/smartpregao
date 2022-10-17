<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * 
 */
abstract class EntidadeDetalheBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'entidade.entidadedetalhe';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array(null, null);

    protected $campos        = array('entid'                          => null,
                                     'entcodent'                      => null,
                                     'entdreg_infantil_creche'        => null,
                                     'entdreg_infantil_preescola'     => null,
                                     'entdreg_fund_8_anos'            => null,
                                     'entdreg_fund_9_anos'            => null,
                                     'entdreg_medio_medio'            => null,
                                     'entdreg_medio_integrado'        => null,
                                     'entdreg_medio_normal'           => null,
                                     'entdreg_medio_prof'             => null,
                                     'entdesp_infantil_creche'        => null,
                                     'entdesp_infantil_preescola'     => null,
                                     'entdesp_fund_8_anos'            => null,
                                     'entdesp_fund_9_anos'            => null,
                                     'entdesp_medio_medio'            => null,
                                     'entdesp_medio_integrado'        => null,
                                     'entdesp_medio_normal'           => null,
                                     'entdesp_eja_fundamental'        => null,
                                     'entdesp_eja_medio'              => null,
                                     'entdeja_fundamental'            => null,
                                     'entdeja_medio'                  => null,
                                     'entdmaterial_esp_etnico'        => null,
                                     'entdeducacao_indigena'          => null,
                                     'entdlingua_portuguesa'          => null,
                                     'entdnum_alunos_ed_comp_escola'  => null,
                                     'entdnum_alunos_ed_comp_outra'   => null,
                                     'entdnum_funcionarios'           => null,
                                     'entdnum_salas_existentes'       => null,
                                     'entdnum_salas_utilizadas'       => null,
                                     'entdnum_comp_administrativos'   => null,
                                     'num_comp_alunos'                => null,
                                     'num_alunos_atend_escola'        => null,
                                     'num_alunos_atend_outra_escola'  => null,
                                     'id_lingua_indigena'             => null,
                                     'id_mod_ens_regular'             => null,
                                     'id_esp_medio_profissional'      => null,
                                     'entdnum_enem_media_total_2006'  => null,
                                     'entdnum_medio_abandono_2005'    => null);


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
                    entcodent,
                    entdreg_infantil_creche,
                    entdreg_infantil_preescola,
                    entdreg_fund_8_anos,
                    entdreg_fund_9_anos,
                    entdreg_medio_medio,
                    entdreg_medio_integrado,
                    entdreg_medio_normal,
                    entdreg_medio_prof,
                    entdesp_infantil_creche,
                    entdesp_infantil_preescola,
                    entdesp_fund_8_anos,
                    entdesp_fund_9_anos,
                    entdesp_medio_medio,
                    entdesp_medio_integrado,
                    entdesp_medio_normal,
                    entdesp_eja_fundamental,
                    entdesp_eja_medio,
                    entdeja_fundamental,
                    entdeja_medio,
                    entdmaterial_esp_etnico,
                    entdeducacao_indigena,
                    entdlingua_portuguesa,
                    entdnum_alunos_ed_comp_escola,
                    entdnum_alunos_ed_comp_outra,
                    entdnum_funcionarios,
                    entdnum_salas_existentes,
                    entdnum_salas_utilizadas,
                    entdnum_comp_administrativos,
                    num_comp_alunos,
                    num_alunos_atend_escola,
                    num_alunos_atend_outra_escola,
                    id_lingua_indigena,
                    id_mod_ens_regular,
                    id_esp_medio_profissional,
                    entdnum_enem_media_total_2006,
                    entdnum_medio_abandono_2005
                FROM
                    entidade.entidadedetalhe
                WHERE
                    entcodent = ?";

        $rs                                  = $this->Execute($sql, array($id));

        $this->entid                         = $rs->fields['entid'];
        $this->entcodent                     = $rs->fields['entcodent'];
        $this->entdreg_infantil_creche       = $rs->fields['entdreg_infantil_creche'];
        $this->entdreg_infantil_preescola    = $rs->fields['entdreg_infantil_preescola'];
        $this->entdreg_fund_8_anos           = $rs->fields['entdreg_fund_8_anos'];
        $this->entdreg_fund_9_anos           = $rs->fields['entdreg_fund_9_anos'];
        $this->entdreg_medio_medio           = $rs->fields['entdreg_medio_medio'];
        $this->entdreg_medio_integrado       = $rs->fields['entdreg_medio_integrado'];
        $this->entdreg_medio_normal          = $rs->fields['entdreg_medio_normal'];
        $this->entdreg_medio_prof            = $rs->fields['entdreg_medio_prof'];
        $this->entdesp_infantil_creche       = $rs->fields['entdesp_infantil_creche'];
        $this->entdesp_infantil_preescola    = $rs->fields['entdesp_infantil_preescola'];
        $this->entdesp_fund_8_anos           = $rs->fields['entdesp_fund_8_anos'];
        $this->entdesp_fund_9_anos           = $rs->fields['entdesp_fund_9_anos'];
        $this->entdesp_medio_medio           = $rs->fields['entdesp_medio_medio'];
        $this->entdesp_medio_integrado       = $rs->fields['entdesp_medio_integrado'];
        $this->entdesp_medio_normal          = $rs->fields['entdesp_medio_normal'];
        $this->entdesp_eja_fundamental       = $rs->fields['entdesp_eja_fundamental'];
        $this->entdesp_eja_medio             = $rs->fields['entdesp_eja_medio'];
        $this->entdeja_fundamental           = $rs->fields['entdeja_fundamental'];
        $this->entdeja_medio                 = $rs->fields['entdeja_medio'];
        $this->entdmaterial_esp_etnico       = $rs->fields['entdmaterial_esp_etnico'];
        $this->entdeducacao_indigena         = $rs->fields['entdeducacao_indigena'];
        $this->entdlingua_portuguesa         = $rs->fields['entdlingua_portuguesa'];
        $this->entdnum_alunos_ed_comp_escola = $rs->fields['entdnum_alunos_ed_comp_escola'];
        $this->entdnum_alunos_ed_comp_outra  = $rs->fields['entdnum_alunos_ed_comp_outra'];
        $this->entdnum_funcionarios          = $rs->fields['entdnum_funcionarios'];
        $this->entdnum_salas_existentes      = $rs->fields['entdnum_salas_existentes'];
        $this->entdnum_salas_utilizadas      = $rs->fields['entdnum_salas_utilizadas'];
        $this->entdnum_comp_administrativos  = $rs->fields['entdnum_comp_administrativos'];
        $this->num_comp_alunos               = $rs->fields['num_comp_alunos'];
        $this->num_alunos_atend_escola       = $rs->fields['num_alunos_atend_escola'];
        $this->num_alunos_atend_outra_escola = $rs->fields['num_alunos_atend_outra_escola'];
        $this->id_lingua_indigena            = $rs->fields['id_lingua_indigena'];
        $this->id_mod_ens_regular            = $rs->fields['id_mod_ens_regular'];
        $this->id_esp_medio_profissional     = $rs->fields['id_esp_medio_profissional'];
        $this->entdnum_enem_media_total_2006 = $rs->fields['entdnum_enem_media_total_2006'];
        $this->entdnum_medio_abandono_2005   = $rs->fields['entdnum_medio_abandono_2005'];

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
        return $this->entcodent;
    }
}





