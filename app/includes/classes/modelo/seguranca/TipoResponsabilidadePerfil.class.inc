<?php

class TipoResponsabilidadePerfil extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.tprperfil";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("prfid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'prfid' => null,
        'tprcod' => null,
        'pflcod' => null,
    );

    public function isPerfisPossuemResponsabilidade($perfis)
    {
        $perfis = is_array($perfis) ? $perfis : array($perfis);

        $sql = "SELECT
                    COUNT(*)
                FROM
                    seguranca.tprperfil tp
                WHERE
                    tp.pflcod IN (". implode(', ', $perfis) . ")";

        return $this->pegaUm($sql) > 0;
    }
}