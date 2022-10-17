<?php

class FormaPagamento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_forma_pagamento";

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
        'estabelecimento_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null, $filtrarResponsabilidade = true)
    {
        $arWhere[] = "tc.excluido IS FALSE";

        if(!$this->testa_superuser() && $filtrarResponsabilidade){
            $arWhere[] = "(" . UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento(). " OR tc.estabelecimento_id IS NULL)";
        }

        $sql = "SELECT
                  tc.descricao,
                  CASE 
                    WHEN pj.id IS NOT NULL THEN pj.nome_fantasia
                    ELSE 'Padrão'
                  END AS estabelecimento,
                  tc.estabelecimento_id,
                  tc.id,
                  tc.id AS forma_pagamento_id
                FROM
                  estabelecimentosaude.tb_forma_pagamento tc
               LEFT JOIN
                  estabelecimentosaude.tb_estabelecimento est ON tc.estabelecimento_id = est.id
               LEFT JOIN
                  corporativo.tb_pessoajuridica pj ON est.pessoajuridica_id = pj.id
               WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY estabelecimento ASC, tc.descricao ASC";
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

    public function excluir($id = null, $retorno = null )
    {
        $this->excluirLogicamente($id);
    }

    public function salvarFormaPagamento($params)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);
        $dados = array(
            'id' => $params['id'],
            'descricao' => $params['descricao'],
            'sigla_tipo_registro' => $params['sigla_tipo_registro'],
            'estabelecimento_id' => $params['estabelecimento_id'],
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    public function getComboPorEstabelecimento($estabelecimentoId)
    {
        $arWhereEstabelecimento = array('tc.estabelecimento_id IS NULL');
        if ($estabelecimentoId) {
            $arWhereEstabelecimento[] = "
                (
                    tc.estabelecimento_id = {$estabelecimentoId}
                    AND
                    " . UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento() . "
                )";
        }
        $arWhere[] = '(' . implode(' OR ', $arWhereEstabelecimento) . ')';

        $rs = $this->getList($arWhere, false, null, false);
        return array_map(function ($row) {
            return array(
                'codigo' => $row['id'],
                'descricao' => $row['descricao']
            );
        }, $rs);
    }

   
}