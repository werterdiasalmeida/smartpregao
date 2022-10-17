<?php

class TipoProfissional extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "profissionalsaude.tb_tipo_profissional";

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
        'sigla_tipo_registro' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
        'pron_trat_masc' => null,
        'pron_trat_fem' => null,
        'ordem_pronome' => null,
        'codigo_conselho' => null,
    );


    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "tp.excluido IS FALSE";

        $sql = "SELECT
                  tp.descricao,
                  tp.sigla_tipo_registro,
                  tp.pron_trat_masc,
                  tp.pron_trat_fem,
                  tp.codigo_conselho,
                  tp.id
                FROM
                  {$this->stNomeTabela} tp
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY tp.descricao ASC";
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

    public function salvarTipoProfissional($dados, $camposNulos = array()){
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }
}