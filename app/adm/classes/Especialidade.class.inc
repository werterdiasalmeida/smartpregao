<?php

class Especialidade extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "profissionalsaude.tb_especialidade";

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
        'tipo_profissional_id' => null,
        'descricao' => null,
        'cbos' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "e.excluido IS FALSE";

        $sql = "SELECT
                  e.descricao,
                  tp.descricao AS descricao_tipo_profissional,
                  e.cbos,
                  e.id,
                  e.tipo_profissional_id
                FROM
                  {$this->stNomeTabela} e
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON e.tipo_profissional_id = tp.id
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY e.descricao ASC";
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

    public function salvarEspecialidade($dados, $camposNulos = array()){
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }
}