<?php

class ModeloDocumento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_modelo_documento";

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
        'titulo' => null,
        'descricao' => null,
        'estabelecimento_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
        'tipo_prontuario_id' => null,
        'privado' => null,
        'quantidade_uso' => null
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null, $filtrarResponsabilidade = true, $arrColumns = [])
    {
        $arWhere[] = "md.excluido IS FALSE";

        if (!$this->testa_superuser() && $filtrarResponsabilidade) {
            $arWhere[] = "(" . UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento() . " OR md.estabelecimento_id IS NULL)";
        }

        if (empty($arrColumns)) {
            $arrColumns = [
                'md.titulo',
                "CASE 
                    WHEN pj.id IS NOT NULL THEN pj.nome_fantasia
                    ELSE 'Padrão'
                   END AS estabelecimento",
                'tp.descricao AS tipo_prontuario',
                'md.descricao',
                'md.estabelecimento_id',
                'md.id',
                'md.id AS modelo_documento_id',
                'md.privado'
            ];
        }

        $sql = "SELECT
                  " . implode(', ', $arrColumns) . "
                FROM
                  estabelecimentosaude.tb_modelo_documento md
               LEFT JOIN
                  estabelecimentosaude.tb_estabelecimento est ON md.estabelecimento_id = est.id
               LEFT JOIN
                  corporativo.tb_pessoajuridica pj ON est.pessoajuridica_id = pj.id
               LEFT JOIN
                  paciente.tb_tipo_prontuario tp ON md.tipo_prontuario_id = tp.id
               WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY estabelecimento ASC, md.titulo ASC";
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

    public function salvarModeloDocumento($params)
    {
        $dados = array(
            'id' => $params['id'],
            'titulo' => $params['titulo'],
            'descricao' => $params['descricao'],
            'tipo_pedido_id' => $params['tipo_pedido_id'],
            'estabelecimento_id' => $params['estabelecimento_id'],
        );

        return $this->manter($params, $dados);
    }

    public function getComboPorEstabelecimento($estabelecimentoId, $tipoId = null, $agendaId = null)
    {
        $variaveis = $this->getValoresDeVariavelPadrao();

        if ($tipoId) {
            $mTipo = new TipoProntuario($tipoId);
            $variaveis['PROCEDIMENTO'] = $mTipo->descricao;
        }

        if ($agendaId) {
            $mAgenda = new EstabelecimentoProfissionalAgenda();
            $agenda = $mAgenda->getTodosDados($agendaId);

            $variaveis['NOME_MEDICO'] = $agenda['profissional'];
            $variaveis['NOME_PACIENTE'] = $agenda['paciente'];
            $variaveis['CPF_PACIENTE'] = $agenda['cpf_paciente'];
        }

        $arWhere = array(
            "(md.estabelecimento_id IS NULL AND md.privado IS FALSE)",
            "(md.criado_por = {$_SESSION['usuario_id']} AND md.privado IS TRUE)"
        );
        if ($estabelecimentoId) {
            $arWhere[] = "(md.estabelecimento_id = {$estabelecimentoId})";
        }

        $arWhere = array('(' . implode(' OR ', $arWhere) . ')');

        if ($tipoId) {
            $arWhere[] = "md.tipo_prontuario_id = {$tipoId}";
        }

        $order = "md.quantidade_uso DESC, estabelecimento ASC, md.titulo ASC";
        $rs = $this->getList(array('(' . implode(' AND ', $arWhere) . ')'), false, $order, false);

        return array_map(function ($row) use ($variaveis) {
            return array(
                'codigo' => $row['id'],
                'descricao' => $row['titulo'],
                'modelo' => $this->substituirVariaveis($row['descricao'], $variaveis),
                'modelo_original' => $row['descricao']
            );
        }, $rs);
    }

    public function getModelosAssociativo($valorCombo, $jsonEncode = false)
    {
        $modelos = array();
        foreach ($valorCombo as $modelo) {
            $modelos[$modelo['codigo']] = $modelo['modelo'];
        }

        return $jsonEncode ? json_encode(utf8ArrayEncode($modelos)) : $modelos;
    }

    public function excluirPorEstabelecimento($estabelecimentoId)
    {
        $sql = "UPDATE 
                    {$this->stNomeTabela}
                SET 
                  excluido = TRUE,
                  atualizado_em = now(),
                  atualizado_por = {$_SESSION['usuario_id']}
                WHERE 
                  estabelecimento_id = {$estabelecimentoId};";

        $this->executar($sql);
    }

    public function copiarDados($origem, $destino, $desabilita = false)
    {
        if ($desabilita) {
            $this->excluirPorEstabelecimento($destino);
        }

        $sql = "SELECT
                 *
               FROM
                 {$this->stNomeTabela}
               WHERE
                 excluido IS FALSE AND
                 estabelecimento_id = '{$origem}';";

        $dados = $this->carregar($sql);

        $arReferenciaChaves = array();
        if (is_array($dados) && count($dados) > 0) {
            foreach ($dados as $index => $item) {
                $arReferenciaChaves[$item['id']] = $this->manter(array(
                    'estabelecimento_id' => $destino,
                    'titulo' => $item['titulo'],
                    'tipo_prontuario_id' => $item['tipo_prontuario_id'],
                    'descricao' => $item['descricao'],
                    'criado_em' => 'now()',
                    'criado_por' => $_SESSION['usuario_id'],
                    'excluido' => 'false'
                ));
                $this->setDadosNull();
            }
        }

        return $arReferenciaChaves;
    }

    public function getVariaveis()
    {
        return [
            'NOME_PACIENTE' => 'Nome do paciente',
            'CPF_PACIENTE' => 'CPF do paciente',
            'NOME_MEDICO' => 'Nome do médico',
            'PROCEDIMENTO' => 'Procedimento',
            'DATA_ATUAL' => 'Data atual',
            'DATA_ATUAL_POR_EXTENSO' => 'Data atual por extenso',
            'TURNO_ATUAL' => 'Turno',
        ];
    }

    public function substituirVariaveis($descricao, $valoresVariavel)
    {
        $variaveis = $this->getVariaveis();
        foreach ($variaveis as $tagVariavel => $dscVariavel) {
            if (isset($valoresVariavel[$tagVariavel])) {
                $descricao = str_replace("[{$tagVariavel}]", $valoresVariavel[$tagVariavel], $descricao);
            }
        }

        return $descricao;
    }

    public function getValoresDeVariavelPadrao()
    {
        $variaveis = [
            'DATA_ATUAL' => date('d/m/Y'),
            'TURNO_ATUAL' => getDescricaoTurnoByHora(),
            'DATA_ATUAL_POR_EXTENSO' => getDataPorExtenso()
        ];

        return $variaveis;
    }

    public function adicionarUsoDoModelo($id)
    {
        $sql = "UPDATE {$this->stNomeTabela} SET quantidade_uso = quantidade_uso + 1 WHERE id = {$id};";
        $this->executar($sql);
    }
}