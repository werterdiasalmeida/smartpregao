<?php

class TabelaPreco extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_tabela_preco";

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
        'percentual_global' => null,
        'inicio_vigencia' => null,
        'fim_vigencia' => null,
        'estabelecimento_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null, $filtrarResponsabilidade = true)
    {
        $arWhere[] = "tp.excluido = FALSE";

        if (!$this->testa_superuser() && $filtrarResponsabilidade) {
            $arWhere[] = UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento();
        }

        $sql = "SELECT
                  tp.inicio_vigencia,
                  tp.fim_vigencia,
                  tp.estabelecimento_id,
                  tp.percentual_global,
                  array_to_string(ARRAY(SELECT 
                                            cet.nome 
                                           FROM
                                            estabelecimentosaude.tb_tabela_procedimento_convenio tpc
                                           INNER JOIN
                                            estabelecimentosaude.tb_convenio_estabelecimento cet ON tpc.convenio_id = cet.id AND cet.excluido IS FALSE
                                           WHERE
                                                tpc.excluido = FALSE AND tpc.tabela_preco_id = tp.id
                                            ORDER BY 
                                              1),',') as convenios,
                  tp.id
                FROM
                  estabelecimentosaude.tb_tabela_preco tp
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento est ON tp.estabelecimento_id = est.id
                WHERE
                 " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY tp.inicio_vigencia DESC, tp.fim_vigencia DESC";
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

    public function getQtdRegistroEntreDatas($estabelecimento, $inicio,$fim,$excecao){

        $sql = "SELECT
                  COUNT(id)
                FROM
                  estabelecimentosaude.tb_tabela_preco tp
                WHERE
                  tp.excluido IS FALSE AND
                  tp.estabelecimento_id = '{$estabelecimento}' AND
                  ('{$inicio}' BETWEEN tp.inicio_vigencia AND tp.fim_vigencia OR '{$fim}' BETWEEN tp.inicio_vigencia AND tp.fim_vigencia 
                    OR tp.inicio_vigencia BETWEEN '{$inicio}' AND '{$fim}' OR tp.fim_vigencia BETWEEN '{$inicio}' AND '{$fim}' )";

        if($excecao){
            $sql .= " AND id != {$excecao};";
        }

        return $this->pegaUm($sql);
    }

    public function salvarTabelaPreco($params)
    {
        $tabelaPrecoId = $this->manter(array('id' => $params['tabela_preco_id']), array(
            'id' => $params['tabela_preco_id'],
            'percentual_global' => desformata_valor($params['percentualGlobal']),
            'inicio_vigencia' => formata_data_sql($params['inicio']),
            'fim_vigencia' => formata_data_sql($params['fim']),
            'estabelecimento_id' => $params['estabelecimento_id']
        ));


        return $tabelaPrecoId;
    }

    public function getRegistrosTabelaVigente($tabelaParametro)
    {
        $sql = "SELECT
                  tec.*
                FROM
                  estabelecimentosaude.tb_tabela_preco tp
                INNER JOIN
                  estabelecimentosaude.tb_tabela_procedimento_convenio tec ON tec.tabela_preco_id = tp.id AND tec.excluido IS FALSE
                WHERE
                  tp.excluido IS FALSE AND
                  tp.estabelecimento_id = '{$tabelaParametro['estabelecimento_id']}' AND 
                  (tp.fim_vigencia = (SELECT MAX(tpi.fim_vigencia) FROM estabelecimentosaude.tb_tabela_preco tpi WHERE tp.estabelecimento_id = tpi.estabelecimento_id AND tpi.excluido IS FALSE AND tpi.fim_vigencia < '{$tabelaParametro['inicio_vigencia']}'))";

        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : array();
    }

    public function atualizaTabelaPrecoInfoVigente($id)
    {
        $mTabelaProcedimentoConvenio = new TabelaProcedimentoConvenio();
        $mValorProcedimento          = new ValorProcedimento();

        $tabelaDestino = $this->carregarPorId($id)->getDados();
        $registrosTabelaVigente = $this->getRegistrosTabelaVigente($tabelaDestino);

        if (is_array($registrosTabelaVigente) && count($registrosTabelaVigente) > 0) {
            foreach ($registrosTabelaVigente as $item) {

                //Gravando o registro de tb_procedimento_convenio
                $tabelaProcedimentoIdNovo = $mTabelaProcedimentoConvenio->manter(array(
                    'descricao' => "Tabela de preços de " . formata_data($tabelaDestino['inicio_vigencia']) . " a " . formata_data($tabelaDestino['fim_vigencia']) . ".",
                    'convenio_id' => $item['convenio_id'],
                    'tabela_preco_id' => $id,
                    'excluido' => 'false'
                ));
                $mTabelaProcedimentoConvenio->setDadosNull();

                //Gravando os valores (do proprio registro e por profissional)
                $valores = $mValorProcedimento->recuperarTodos('*', array("tabela_procedimento_convenio_id = '{$item['id']}'", 'excluido = false'));
                if (is_array($valores) && count($valores) > 0) {
                    foreach ($valores as $valor) {
                        //Gravando o registro de tb_procedimento_convenio
                        $valorIdNovo = $mValorProcedimento->manter(array(
                            'codigo' => $valor['codigo'],
                            'valor' => $valor['valor'],
                            'procedimento_id' => $valor['procedimento_id'],
                            'tabela_procedimento_convenio_id' => $tabelaProcedimentoIdNovo,
                            'profissional_estabelecimento_procedimento_id' => $valor['profissional_estabelecimento_procedimento_id'],
                            'profissional_atende' => $valor['profissional_atende'],
                            'valor_profissional' => $valor['valor_profissional'],
                            'percentual_profissional' => $valor['percentual_profissional'],
                            'excluido' => 'false'
                        ));
                        $mValorProcedimento->setDadosNull();
                    }
                }
            }
        }
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

    public function copiarDados($origem,$destino, $desabilita = false, $arComparacaoConvenio = array(),$arComparacaoProcedimento = array())
    {
        if($desabilita){
            $this->excluirPorEstabelecimento($destino);
        }

        $sql = "SELECT
                 tp.*
               FROM
                 estabelecimentosaude.tb_tabela_preco tp
               WHERE
                 tp.excluido IS FALSE AND
                 tp.estabelecimento_id = '{$origem}';";

        $dados = $this->carregar($sql);

        $arReferenciaChaves = array();
        if (is_array($dados) && count($dados) > 0) {
            foreach ($dados as $index => $item) {

                $arReferenciaChaves[$item['id']] = $this->manter(array(
                    'estabelecimento_id' => $destino,
                    'percentual_global' => $item['percentual_global'],
                    'inicio_vigencia' => $item['inicio_vigencia'],
                    'fim_vigencia' => $item['fim_vigencia'],
                    'criado_em' => 'now()',
                    'criado_por' => $_SESSION['usuario_id'],
                    'excluido' => 'false'
                ));
                $this->copiarDadosRelacionados($item['id'],$arReferenciaChaves[$item['id']],$destino,$arComparacaoConvenio,$arComparacaoProcedimento);
                $this->setDadosNull();
            }
        }

        return $arReferenciaChaves;
    }

    public function copiarDadosRelacionados($origem,$destino,$estabelecimento_id,$arComparacaoConvenio = array(),$arComparacaoProcedimento = array())
    {
        $mTabelaProcedimentoConvenio = new TabelaProcedimentoConvenio();
        $mValorProcedimento          = new ValorProcedimento();

        $tabelaDestino = $this->carregarPorId($destino)->getDados();

        //recuperar os profissionais do estebelecimento
        $mEstabelecimentoEspecialidadeProfissional = new EstabelecimentoEspecialidadeProfissional();
        $arIdsProfissionais = $mEstabelecimentoEspecialidadeProfissional->getIdsProfissionaisPorEstabelecimento($estabelecimento_id);

        //Recuperando os valores referentes a tabela atual
        $sql = "SELECT
                  tec.*
                FROM
                  estabelecimentosaude.tb_tabela_preco tp
                INNER JOIN
                  estabelecimentosaude.tb_tabela_procedimento_convenio tec ON tec.tabela_preco_id = tp.id AND tec.excluido IS FALSE
                WHERE
                  tp.excluido IS FALSE AND
                  tp.id = '{$origem}'";
        $registrosTabelaVigente = $this->carregar($sql);

        if (is_array($registrosTabelaVigente) && count($registrosTabelaVigente) > 0) {
            foreach ($registrosTabelaVigente as $item) {

                //Gravando o registro de tb_procedimento_convenio
                $tabelaProcedimentoIdNovo = $mTabelaProcedimentoConvenio->manter(array(
                    'descricao' => "Tabela de preços de " . formata_data($tabelaDestino['inicio_vigencia']) . " a " . formata_data($tabelaDestino['fim_vigencia']) . ".",
                    'convenio_id' => isset($arComparacaoConvenio[$item['convenio_id']]) ? $arComparacaoConvenio[$item['convenio_id']] : $item['convenio_id'],
                    'tabela_preco_id' => $destino,
                    'excluido' => 'false'
                ));
                $mTabelaProcedimentoConvenio->setDadosNull();

                //Seletando todos os valores, tanto do proprio registro como de profissional
                $sql = "SELECT
                          vp.*,
                          pep.profissional_id
                        FROM
                          estabelecimentosaude.tb_valor_procedimento vp
                        LEFT JOIN
                          estabelecimentosaude.tb_profissional_estabelecimento_procedimento pep ON pep.id = vp.profissional_estabelecimento_procedimento_id AND pep.excluido IS FALSE
                        WHERE
                          vp.excluido IS FALSE AND
                          vp.tabela_procedimento_convenio_id = '{$item['id']}'";
                $valores = $this->carregar($sql);

                //Gravando os valores (do proprio registro e por profissional)
                if (is_array($valores) && count($valores) > 0) {
                    foreach ($valores as $valor) {
                        if((empty($valor['profissional_id']) || in_array($valor['profissional_id'],$arIdsProfissionais)) && !empty($arComparacaoProcedimento[$valor['procedimento_id']])){
                            //Gravando o registro de tb_procedimento_convenio
                            $valorIdNovo = $mValorProcedimento->manter(array(
                                'codigo' => $valor['codigo'],
                                'valor' => $valor['valor'],
                                'procedimento_id' => isset($arComparacaoProcedimento[$valor['procedimento_id']]) ? $arComparacaoProcedimento[$valor['procedimento_id']]['valor'] : null,
                                'tabela_procedimento_convenio_id' => $tabelaProcedimentoIdNovo,
                                'profissional_estabelecimento_procedimento_id' => isset($arComparacaoProcedimento[$valor['procedimento_id']]) ? $arComparacaoProcedimento[$valor['procedimento_id']]['itens'][$valor['profissional_estabelecimento_procedimento_id']] : null,
                                'profissional_atende' => $valor['profissional_atende'],
                                'valor_profissional' => $valor['valor_profissional'],
                                'percentual_profissional' => $valor['percentual_profissional'],
                                'excluido' => 'false'
                            ));
                            $mValorProcedimento->setDadosNull();
                        }
                    }
                }
            }
        }
    }





}