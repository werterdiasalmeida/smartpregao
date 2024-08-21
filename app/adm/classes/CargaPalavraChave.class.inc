<?php

class CargaPalavraChave extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cargas.tb_carga_palavra_chave";

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
        'tipo_carga' => null,
        'palavra' => null
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {

        if (!$arWhere)
            $arWhere[] = 'TRUE';

        $sql = "SELECT
                  *
                FROM
                 cargas.tb_carga_palavra_chave
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY id DESC";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 20);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }
}
