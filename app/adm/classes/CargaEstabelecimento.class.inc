<?php

class CargaEstabelecimento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "carga_clientes.tb_carga_estabelecimento";

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
        'cnpj' => null,
        'razao_social' => null,
        'email_1' => null,
        'email_2' => null,
        'telefone_1' => null,
        'telefone_2' => null,
        'situacao_carga' => null,
        'prgcod' => null,
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "cnpj IS NOT NULL";
        $sql = "SELECT
                  *
                FROM
                    carga_clientes.tb_carga_estabelecimento car
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY 1 DESC";
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

    public function salvarBasico($params)
    {
        $sql = "INSERT INTO carga_clientes.tb_carga_estabelecimento 
            (cnpj, razao_social, situacao_carga, prgcod) 
            VALUES ('{$params['cnpj']}','{$params['razao_social']}','{$params['situacao_carga']}',{$params['prgcod']})";
        $this->executar($sql);
        return $this->commit();
    }

    public function excluirEstabelecimento($id)
    {
        return $this->excluir($id);
    }
}
