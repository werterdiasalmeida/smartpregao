<?php

class CartaoAderenciaHistorico extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "clubvidacard.tb_cartao_aderente_historico";

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
        'id_cartao_aderente' => null,
        'motivo' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );


    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "h.excluido IS FALSE";

        $sql = "SELECT
                    h.criado_em,
                    u.usunome,
                    h.motivo
                FROM clubvidacard.tb_cartao_aderente_historico h
                JOIN seguranca.usuario u ON h.criado_por = u.id
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY criado_em DESC";
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

    public function salvarCartaoAderenciaHistorico($dados, $camposNulos = array())
    {
        $dados = $this->getDadosCarimbo($dados, $dados);

        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    public function excluir($id = null, $retorno = null)
    {
        $this->excluirLogicamente($id);
    }


}
