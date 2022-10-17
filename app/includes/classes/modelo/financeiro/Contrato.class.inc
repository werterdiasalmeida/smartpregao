<?php

class Contrato extends Modelo
{
    /**
     * Número de dias qie atecedem o vencimento para o fechamento da fatura
     */
    const NUM_DIAS_FECHAMENTO_FATURA = 5;

    const VALOR_PORCETAGEM_MULTA_CANCELAMENTO = 0.4;

    const SITUACAO_CONTRATO_ATIVO = 'A';

    const SITUACAO_CONTRATO_PENDENTE_PAGAMENTO = 'P';

    const SITUACAO_CONTRATO_INATIVO = 'I';

    const SITUACAO_CONTRATO_CANCELADO = 'C';

    const DIAS_INADINPLENCIA_CANCELAMENTO_AUTOMATICO = 90;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "financeiro.tb_contrato";

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
        'tipo_contrato_id' => null,
        'periodo_contrato_id' => null,
        'quantidade_parcelas' => null,
        'numero' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'situacao' => null,
        'excluido' => null,
        'dia_pagamento' => null,
        'inicio_vigencia' => null,
        'fim_vigencia' => null,
        'valor_contratado' => null
    );

    public function getLista($arWhere = array(), $paginacao = false)
    {
        $arWhere[] = "con.excluido IS FALSE";

        $sql = "SELECT
					con.numero,
					pjr.cnpj,
					pjr.razao_social,
					CASE
					  WHEN con.situacao = '" . self::SITUACAO_CONTRATO_ATIVO . "' THEN 'Ativo'
					  WHEN con.situacao = '" . self::SITUACAO_CONTRATO_INATIVO . "' THEN 'Inativo'
					  WHEN con.situacao = '" . self::SITUACAO_CONTRATO_PENDENTE_PAGAMENTO . "' THEN 'Com Pendência de Pagamento'
					END AS str_situacao,
					con.criado_em,
					usu.usunome AS usuario_criacao,
					con.situacao,
					con.id
				FROM
					{$this->stNomeTabela} con
				INNER JOIN
					estabelecimentosaude.tb_estabelecimento_contrato esc ON esc.contrato_id = con.id
				INNER JOIN
					estabelecimentosaude.tb_estabelecimento est ON est.id = esc.estabelecimento_id
				INNER JOIN
					corporativo.tb_pessoajuridica pjr ON pjr.id = est.pessoajuridica_id
				INNER JOIN
					seguranca.usuario usu ON usu.id = con.criado_por
				WHERE
					" . implode(' AND ', $arWhere);

        if (!$_POST["ordem"]) {
            $sql .= " ORDER BY con.criado_em DESC";
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 50);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public static function getComboDataDePagamento()
    {
        $arDiasPagamento = array(
            array('codigo' => 5, 'descricao' => "5"),
            array('codigo' => 10, 'descricao' => "10"),
            array('codigo' => 15, 'descricao' => "15"),
            array('codigo' => 20, 'descricao' => "20"),
        );

        return $arDiasPagamento;
    }

    public function salvarContrato($arrDados)
    {
        $this->salvarDadosDeContatoPessoaJuridica($arrDados);
        $arrDados['inicio_vigencia'] = formata_data_sql($arrDados['inicio_vigencia']);
        $arrDados['fim_vigencia'] = formata_data_sql($arrDados['fim_vigencia']);
        $arrDados['valor_contratado'] = desformata_valor($arrDados['valor_contratado']);
        if (empty($arrDados['id'])) {
            $arrDados['situacao'] = self::SITUACAO_CONTRATO_ATIVO;
            $arrDados['numero'] = $this->gerarNumeroDeContrato();
        }
        $arrDados['excluido'] = 'false';

        $arrDados['contrato_id'] = $this->manter($arrDados);

        $mContratoFatura = new ContratoFatura();
        $mContratoFatura->gerarFaturasDoContrato($arrDados);

        $arrDadosEstabelecimentoContrato = array(
            'estabelecimento_id' => $arrDados['estabelecimento_id'],
            'contrato_id' => $arrDados['contrato_id'],
            'excluido' => 'false'
        );

        $mEstabelecimentoContrato = new EstabelecimentoContrato();
        $mEstabelecimentoContrato->manter($arrDadosEstabelecimentoContrato);

        return $arrDados['contrato_id'];
    }

    private function gerarNumeroDeContrato()
    {
        $numContrato = mt_rand(0, 999999);
        $numContrato = str_pad($numContrato, 6, "0", STR_PAD_LEFT);
        $numContrato = $numContrato . '/' . date('Y');

        return $numContrato;
    }

    public function getTodosDadosPorId($id)
    {
        $sql = "SELECT
                    con.id,
					con.tipo_contrato_id,
					con.periodo_contrato_id,
					con.quantidade_parcelas,
					con.numero,
					con.criado_em,
					con.criado_por,
					con.atualizado_em,
					con.atualizado_por,
					CASE
					  WHEN con.situacao = '" . self::SITUACAO_CONTRATO_ATIVO . "' THEN 'Ativo'
					  WHEN con.situacao = '" . self::SITUACAO_CONTRATO_INATIVO . "' THEN 'Inativo'
					  WHEN con.situacao = '" . self::SITUACAO_CONTRATO_PENDENTE_PAGAMENTO . "' THEN 'Com Pendência de Pagamento'
					END AS str_situacao,
					con.situacao,
					con.excluido,
					con.dia_pagamento,
					con.inicio_vigencia,
					con.fim_vigencia,
					con.valor_contratado,
					pjr.id as pessoajuridica_id,
					pjr.cnpj,
					pjr.razao_social,
					pjr.nome_fantasia,
					esc.estabelecimento_id
				FROM
					{$this->stNomeTabela} con
				INNER JOIN
					estabelecimentosaude.tb_estabelecimento_contrato esc ON esc.contrato_id = con.id
				INNER JOIN
					estabelecimentosaude.tb_estabelecimento est ON est.id = esc.estabelecimento_id
				INNER JOIN
					corporativo.tb_pessoajuridica pjr ON pjr.id = est.pessoajuridica_id
				INNER JOIN
					seguranca.usuario usu ON usu.id = con.criado_por
                WHERE
                  con.id = {$id}";

        $contrato = $this->pegaLinha($sql);
        $contrato = $contrato ? $contrato : [];

        if ($contrato) {
            $mPessoaJuridica = new PessoaJuridica();
            $contrato = $mPessoaJuridica->getDadosEnderecoEContatoPrincipal($contrato);
        }

        return $contrato;
    }

    public function exibirCabecalhoContrato($id)
    {
        $dadosContrato = $this->getTodosDadosPorId($id);
        ?>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align: center; font-weight: bold;">
                    Dados do Contrato
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">
                    Número do Contrato:
                </td>
                <td>
                    <?= $dadosContrato['numero']; ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">
                    CNPJ:
                </td>
                <td>
                    <?= $dadosContrato['cnpj']; ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">
                    Razão Social:
                </td>
                <td>
                    <?= $dadosContrato['razao_social']; ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">
                    Vigencia do Contrato:
                </td>
                <td>
                    <?= formata_data($dadosContrato['inicio_vigencia']) . ' à ' . formata_data($dadosContrato['fim_vigencia']); ?>
            </tr>
            <tr>
                <td class="SubTituloDireita">
                    Situação:
                </td>
                <td>
                    <?= Contrato::getArraySituacaoContrato()[$dadosContrato['situacao']]['descricao']; ?>
                </td>
            </tr>
        </table>
        <?php
    }

    public function getDadosContratoVigentePorEstabelecimento($estabelecimentoId)
    {
        $sql = "SELECT
                    con.id,
					con.tipo_contrato_id,
					con.periodo_contrato_id,
					con.quantidade_parcelas,
					con.numero,
					con.criado_em,
					con.criado_por,
					CASE
					  WHEN con.situacao = '" . self::SITUACAO_CONTRATO_ATIVO . "' THEN 'Ativo'
					  WHEN con.situacao = '" . self::SITUACAO_CONTRATO_INATIVO . "' THEN 'Inativo'
					  WHEN con.situacao = '" . self::SITUACAO_CONTRATO_PENDENTE_PAGAMENTO . "' THEN 'Com Pendência de Pagamento'
					END AS str_situacao,
					con.atualizado_em,
					con.atualizado_por,
					con.situacao,
					con.excluido,
					con.dia_pagamento,
					con.inicio_vigencia,
					con.fim_vigencia,
					con.valor_contratado,
					pjr.id as pessoa_juridica_id,
					pjr.cnpj,
					pjr.razao_social,
					pjr.nome_fantasia,
					tcn.nome AS nome_tipo_contrato,
					pcn.nome AS nome_periodo
				FROM
					{$this->stNomeTabela} con
				INNER JOIN
					estabelecimentosaude.tb_estabelecimento_contrato esc ON esc.contrato_id = con.id
				INNER JOIN
					financeiro.tb_tipo_contrato tcn ON tcn.id = con.tipo_contrato_id
				INNER JOIN
					financeiro.tb_periodo_contrato pcn ON pcn.id = con.periodo_contrato_id
				INNER JOIN
					estabelecimentosaude.tb_estabelecimento est ON est.id = esc.estabelecimento_id
				INNER JOIN
					corporativo.tb_pessoajuridica pjr ON pjr.id = est.pessoajuridica_id
				INNER JOIN
					seguranca.usuario usu ON usu.id = con.criado_por
                WHERE
                  esc.estabelecimento_id = {$estabelecimentoId} AND now()::date BETWEEN con.inicio_vigencia AND con.fim_vigencia";

        $retorno = $this->pegaLinha($sql);
        return $retorno;
    }

    public function verificarSituacaoDePagamentoDoContrato($contratoId)
    {
        $dadosContrato = $this->carregarPorId($contratoId)->getDados();

        if ($dadosContrato['situacao'] != Contrato::SITUACAO_CONTRATO_CANCELADO) {
            $sql = "SELECT 
                    con.id,
                    COUNT(fat.id) FILTER(WHERE fat.situacao = 'V') AS qtd_faturas_vencidas,
                    now()::date BETWEEN con.inicio_vigencia AND con.fim_vigencia AS contrato_em_vigencia
                FROM
                    financeiro.tb_contrato con
                INNER JOIN
                    financeiro.tb_contrato_fatura cnf ON cnf.contrato_id = con.id AND cnf.excluido IS FALSE
                INNER JOIN 
                    financeiro.tb_fatura fat ON fat.id = cnf.fatura_id AND fat.excluido IS FALSE
                WHERE
                    con.id = {$contratoId}
                GROUP BY
                    con.id";

            $dadosSituacaoPagamento = $this->pegaLinha($sql);
            $contratoEmVigencia = $dadosSituacaoPagamento['contrato_em_vigencia'] == 't' ? true : false;

            $situacao = null;
            if ($dadosSituacaoPagamento['qtd_faturas_vencidas'] > 0) {
                $situacao = self::SITUACAO_CONTRATO_PENDENTE_PAGAMENTO;
            } else if (!$contratoEmVigencia) {
                $situacao = self::SITUACAO_CONTRATO_INATIVO;
            } else if ($contratoEmVigencia) {
                $situacao = self::SITUACAO_CONTRATO_ATIVO;
            }

            if (!is_null($situacao)) {
                $arrDadosContrato = [
                    'id' => $contratoId,
                    'situacao' => $situacao
                ];

                $this->setDadosNull();
                $this->manter($arrDadosContrato);
            }
        }
    }

    public function getListaRelatorio($arrWhere = [], $paginacao = true)
    {
        $arrWhere[] = "con.excluido IS FALSE";

        $sql = "SELECT 
                    con.numero,
                    pjr.cnpj,
					pjr.nome_fantasia,
					to_char(con.inicio_vigencia, 'DD/MM/YYYY') || ' à ' || to_char(con.fim_vigencia, 'DD/MM/YYYY') as vigencia_contrato,
                    CASE
					  WHEN con.situacao = '" . self::SITUACAO_CONTRATO_ATIVO . "' THEN 'Ativo'
					  WHEN con.situacao = '" . self::SITUACAO_CONTRATO_INATIVO . "' THEN 'Inativo'
					  WHEN con.situacao = '" . self::SITUACAO_CONTRATO_PENDENTE_PAGAMENTO . "' THEN 'Com Pendência de Pagamento'
					END AS str_situacao,
                    count(fat.id) FILTER (WHERE fat.situacao = '" . Fatura::SITUACAO_FATURA_PAGA . "') AS qtd_parcelas_pagas,
                    count(fat.id) FILTER (WHERE fat.situacao = '" . Fatura::SITUACAO_FATURA_VENCIDA . "') AS qtd_parcelas_vencidas,
                    count(fat.id) FILTER (WHERE fat.situacao = '" . Fatura::SITUACAO_FATURA_GERADA . "' OR fat.situacao = '" . Fatura::SITUACAO_FATURA_PENDENTE . "') AS qtd_parcelas_a_vencer,
                    count(fat.id) AS qtd_total_parcelas,
                    con.id,
                    con.situacao
                FROM
                    financeiro.tb_contrato con
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento_contrato esc ON esc.contrato_id = con.id AND esc.excluido IS FALSE
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento est ON est.id = esc.estabelecimento_id AND est.excluido IS FALSE
				INNER JOIN
					corporativo.tb_pessoajuridica pjr ON pjr.id = est.pessoajuridica_id
                INNER JOIN
                    financeiro.tb_contrato_fatura cnf ON con.id = cnf.contrato_id AND cnf.excluido IS FALSE
                INNER JOIN
                    financeiro.tb_fatura fat ON fat.id = cnf.fatura_id
                WHERE
					" . implode(' AND ', $arrWhere) . "
                GROUP BY
                    con.id, pjr.cnpj, pjr.nome_fantasia";

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public static function getArraySituacaoContrato()
    {
        return array(
            self::SITUACAO_CONTRATO_ATIVO => ['descricao' => 'Ativo', 'cor' => '#3AC386'],
            self::SITUACAO_CONTRATO_INATIVO => ['descricao' => 'Inativo', 'cor' => '#bac3d0'],
            self::SITUACAO_CONTRATO_PENDENTE_PAGAMENTO => ['descricao' => 'Com Pendência de Pagamento', 'cor' => '#F3C200'],
            self::SITUACAO_CONTRATO_CANCELADO => ['descricao' => 'Cancelado', 'cor' => '#ed6b75']
        );
    }

    public static function getSituacoesContratoParaCheckbox($arrWhere = array())
    {
        $arrSituacao = self::getArraySituacaoContrato();

        $arrStiacaoCheckbox = array(
            array(
                'codigo' => '',
                'descricao' => "<div class='descricao label label-sm' style='color: #000;'>Todos</div>",
                'cor' => null
            )
        );

        foreach ($arrSituacao as $flgSituacao => $situacao) {
            $arrStiacaoCheckbox[] = array(
                'codigo' => $flgSituacao,
                'descricao' => "<div class='descricao label label-sm' style='background-color: {$situacao['cor']}'>{$situacao['descricao']}</div>"
            );
        }

        return $arrStiacaoCheckbox;
    }

    public function isPossuiContratoParaPessoaJuridicaNoPeriodo($estabelecimentoId,
                                                                $inicioVigencia, $fimVigencia, $id = null)
    {
        $arrSituacaoSemEfeitoDeVigencia = [
            self::SITUACAO_CONTRATO_INATIVO,
            self::SITUACAO_CONTRATO_CANCELADO
        ];

        $inicioVigenciaSql = formata_data_sql($inicioVigencia);
        $fimVigenciaSql = formata_data_sql($fimVigencia);
        $whereNotId = $id ? " AND con.id <> {$id}" : "";
        $whereSituacao = "con.situacao NOT IN ( '" . implode("', '", $arrSituacaoSemEfeitoDeVigencia) . "' )";

        $sql = "
            SELECT 
                COUNT(con.id) > 0 AS possui_contrato_periodo
            FROM
                financeiro.tb_contrato con
            INNER JOIN
                estabelecimentosaude.tb_estabelecimento_contrato esc ON esc.contrato_id = con.id
            WHERE
                ('{$inicioVigenciaSql}'::date BETWEEN con.inicio_vigencia AND con.fim_vigencia OR
                '{$fimVigenciaSql}'::date BETWEEN con.inicio_vigencia AND con.fim_vigencia)
                AND esc.estabelecimento_id = {$estabelecimentoId}
                AND {$whereSituacao}
                 {$whereNotId}
        ";

        return $this->pegaUm($sql) == 't' ? true : false;
    }

    /**
     * @param $arrDados
     * @throws ErroGeracaoFatura
     */
    public function cancelarContrato($arrDados)
    {
        $mContratoFatura = new ContratoFatura();

        $valorCancelamento = $this->getValorCancelamentoContrato($arrDados);

        $mContratoFatura->cancelarFaturasNaoPagasNoIuguPorContrato($arrDados['id']);
        $mContratoFatura->cancelarFaturasEmAbertoPorContrato($arrDados['id']);

        $mContratoFatura->gerarFaturaDeCancelamento(
            ['contrato_id' => $arrDados['id'], 'valor_cancelamento' => $valorCancelamento]);

        $arrDadosContrato = [
            'id' => $arrDados['id'],
            'situacao' => self::SITUACAO_CONTRATO_CANCELADO
        ];

        $this->manter($arrDadosContrato);
    }

    public function getValorDeCancelamentoDoContratoPorValorFixo($arrDados)
    {
        return $arrDados['valor_restante_contrato'] * Contrato::VALOR_PORCETAGEM_MULTA_CANCELAMENTO;
    }

    public function getValorDeCancelamentoDoContratoPorAtendimento($arrDados)
    {
        $mContratoFatura = new ContratoFatura();
        $valorUltimaFatura = $mContratoFatura->getValorParcelaDeContratoPorAtendimento(
            [
                'contrato_id' => $arrDados['id'],
                'estabelecimento_id' => $arrDados['estabelecimento_id'],
                'valor_contratado' => $arrDados['valor_contratado']
            ]
        );

        return $arrDados['valor_restante_contrato'] + $valorUltimaFatura;
    }

    public function getValorCancelamentoContrato($arrDados)
    {
        $dadosContrato = $this->getTodosDadosPorId($arrDados['id']);
        $valorCancelamento = null;
        $mContratoFatura = new ContratoFatura();
        $valorRestanteAPagar = $mContratoFatura->getValorAPagarDoContrato($dadosContrato['id']);
        $arrDados['estabelecimento_id'] = $dadosContrato['estabelecimento_id'];
        $arrDados['valor_contratado'] = $dadosContrato['valor_contratado'];
        $arrDados['valor_restante_contrato'] = $valorRestanteAPagar;

        switch ($dadosContrato['tipo_contrato_id']) {
            case TipoContrato::TIPO_POR_VALOR_FIXO:
                $valorCancelamento = $this->getValorDeCancelamentoDoContratoPorValorFixo($arrDados);
                break;
            case TipoContrato::TIPO_POR_ATENDIMENTO_REALIZADO:
                $valorCancelamento = $this->getValorDeCancelamentoDoContratoPorAtendimento($arrDados);
                break;
        }

        return $valorCancelamento;
    }

    private function salvarDadosDeContatoPessoaJuridica($arrDados)
    {
        $mContatoPessoaJuridica = new ContatoPessoaJuridica();
        $arrDadosContato = [
            'pessoajuridica_id' => $arrDados['pessoajuridica_id'],
            'contato' => $arrDados['contato_email'],
            'tipo_contato_sigla' => TipoContato::TIPO_EMAIL,
            'flg_principal' => 't',
        ];
        $mContatoPessoaJuridica->salvarContatos($arrDadosContato);

        $mEnderecoPessoaJuridica = new EnderecoPessoaJuridica();
        $arrDadosEndereco = [
            'id' => $arrDados['endereco_pessoajuridica_id'],
            'pessoajuridica_id' => $arrDados['pessoajuridica_id'],
            'logradouro' => $arrDados['logradouro'],
            'bairro' => $arrDados['bairro'],
            'complemento' => $arrDados['complemento'],
            'cep' => $arrDados['cep'],
            'uf_id' => $arrDados['uf_sigla'],
            'uf_sigla' => $arrDados['uf_sigla'],
            'municipio_id' => $arrDados['municipio_id'],
            'muncod' => null,
            'geo_localizacao' => null
        ];
        $mEnderecoPessoaJuridica->salvarEndereco($arrDadosEndereco);
    }

    /**
     * @param $arrDados ['id' => contrato_id]
     */
    public function reativarContrato($arrDados)
    {
        $mContratoFatura = new ContratoFatura();
        $mContratoFatura->reabirirFaturasCanceladasPorContrato($arrDados['id']);

        $arrDadosContrato = [
            'id' => $arrDados['id'],
            'situacao' => self::SITUACAO_CONTRATO_ATIVO
        ];

        $this->manter($arrDadosContrato);
    }
}

