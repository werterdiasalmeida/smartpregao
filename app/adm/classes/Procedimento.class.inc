<?php

class Procedimento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_procedimento";

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
        'codigo' => null,
        'codigo_ans' => null,
        'codigo_sus' => null,
        'estabelecimento_id' => null,
        'descricao' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
        'tipo_guia_id' => null,
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null, $filtrarResponsabilidade = true)
    {
        $arWhere[] = "pro.excluido IS FALSE";

        if(!$this->testa_superuser() && $filtrarResponsabilidade){
            $arWhere[] = "(" . UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento(). " OR pro.estabelecimento_id IS NULL)";
        }

        $sql = "SELECT
                  pro.codigo,
                  pro.descricao,
                  CASE 
                    WHEN pj.id IS NOT NULL THEN pj.nome_fantasia
                    ELSE 'Padrão'
                  END AS estabelecimento,
                  pro.codigo_ans,
                  pro.codigo_sus,
                  tgu.descricao AS tipo_guia,
                  pro.estabelecimento_id,
                  pro.id AS procedimento_id,
                  pro.id
                FROM
                  estabelecimentosaude.tb_procedimento pro
               LEFT JOIN
                  estabelecimentosaude.tb_estabelecimento est ON pro.estabelecimento_id = est.id
               LEFT JOIN
                  corporativo.tb_pessoajuridica pj ON est.pessoajuridica_id = pj.id
               LEFT JOIN
                  estabelecimentosaude.tb_tipo_guia tgu ON pro.tipo_guia_id = tgu.id
               WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY estabelecimento ASC, pro.descricao ASC";
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

    public function salvarProcedimento($params)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);
        $dados = array(
            'id' => $params['id'],
            'descricao' => $params['descricao'],
            'codigo' => $params['codigo'],
            'codigo_ans' => $params['codigo_ans'],
            'codigo_sus' => $params['codigo_sus'],
            'tipo_guia_id' => $params['tipo_guia_id'],
            'estabelecimento_id' => $params['estabelecimento_id'],
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    /**
     * @param $estabelecimentoId
     * @throws Exception
     */
    public function excluirPorEstabelecimento($estabelecimentoId)
    {
        $sql = "UPDATE {$this->stNomeTabela} SET excluido = TRUE WHERE estabelecimento_id = {$estabelecimentoId}";
        $this->executar($sql);
    }

    public function copiarDados($origem,$destino, $desabilita = false)
    {
        $mProfissionalEstabelecimentoProcedimento = new ProfissionalEstabelecimentoProcedimento();

        if($desabilita){
            $this->excluirPorEstabelecimento($destino);
        }

        $sql = "SELECT
                 p.*
               FROM
                 estabelecimentosaude.tb_procedimento p
               WHERE
                 p.excluido IS FALSE AND
                 p.estabelecimento_id = '{$origem}';";

        $dados = $this->carregar($sql);

        $arReferenciaChaves = array();
        if (is_array($dados) && count($dados) > 0) {
            foreach ($dados as $index => $item) {
                $valor = $this->manter(array(
                    'estabelecimento_id' => $destino,
                    'codigo' => $item['codigo'],
                    'codigo_ans' => $item['codigo_ans'],
                    'codigo_sus' => $item['codigo_sus'],
                    'descricao' => $item['descricao'],
                    'criado_em' => 'now()',
                    'criado_por' => $_SESSION['usuario_id'],
                    'excluido' => 'false'
                ));
                $this->setDadosNull();

                $arReferenciaChaves[$item['id']]['valor'] = $valor;
                $arReferenciaChaves[$item['id']]['itens'] = $mProfissionalEstabelecimentoProcedimento->copiarDados($item['id'],$valor,$destino);
            }
        }

        return $arReferenciaChaves;
    }
}