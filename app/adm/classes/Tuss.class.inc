<?php

class Tuss extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.tb_tuss";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("id");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'id' => null,
        'cod_tuss' => null,
        'procedimento' => null,
        'resolucao' => null,
        'sugrupo' => null,
        'grupo' => null,
        'capitulo' => null,
        'od' => null,
        'amb' => null,
        'hco' => null,
        'hso' => null,
        'pac' => null,
        'excluido' => null,
    );

    public function getList($where = [])
    {
        $where[] = 'TRUE';
        $sql = "SELECT
                    cod_tuss AS codigo,
                    procedimento AS descricao
                FROM
                    corporativo.vw_tuss
                WHERE
                    " . implode(' AND ', $where) . "
                ORDER BY
                    cod_tuss";

        return $this->carregarArray($sql);
    }

    public function getCodAssocDsc($where = [])
    {
        $rs = $this->getList($where);
        $assoc = [];

        foreach($rs as $tuss) {
            $assoc[$tuss['codigo']] = $tuss['descricao'];
        }

        return $assoc;
    }

}