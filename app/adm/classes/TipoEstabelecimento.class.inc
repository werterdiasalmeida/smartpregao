<?php

class TipoEstabelecimento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_tipo_estabelecimento";

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
        'descricao' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );


    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "te.excluido IS FALSE";

        $sql = "SELECT
                  te.descricao,
                  te.id
                FROM
                  {$this->stNomeTabela} te
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY te.descricao ASC";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 15);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public function excluir($id = null, $retorno = null)
    {
        $this->excluirLogicamente($id);
    }

    public function salvarTipoEstabelecimento($dados, $camposNulos = array()){
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }
}