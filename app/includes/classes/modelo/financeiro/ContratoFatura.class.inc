<?php

class ContratoFatura extends Modelo
{
    const VALOR_MININO_FATURA_POR_ATENDIMENTO = 80.0;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "financeiro.tb_contrato_fatura";

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
        'contrato_id' => null,
        'numero_parcela' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
        'fatura_id' => null,
        'descricao' => null,
        'data_fechamento' => null
    );

    public function gerarFaturasDoContrato($arrDadosContrato)
    {
        $dataAtual = new DateTime(date('Y-m-d'));
        $dataPagamento = new DateTime($arrDadosContrato['inicio_vigencia']);
        $dataPagamento->setDate($dataPagamento->format('Y'), $dataPagamento->format('m'), $arrDadosContrato['dia_pagamento']);
        $valorParcela = null;
        $situacao = Fatura::SITUACAO_FATURA_AGUARDANDO_FECHAMENTO;

        if ($arrDadosContrato['tipo_contrato_id'] == TipoContrato::TIPO_POR_VALOR_FIXO) {
            $valorParcela = $arrDadosContrato['valor_contratado'] / $arrDadosContrato['quantidade_parcelas'];
            $situacao = Fatura::SITUACAO_FATURA_PENDENTE;
        }

        if ($dataPagamento <= $dataAtual || $dataPagamento->diff($dataAtual)->days <= 7) {
            $dataPagamento->add(new DateInterval('P1M'));
        }

        for ($parcela = 1; $parcela <= $arrDadosContrato['quantidade_parcelas']; $parcela++) {
            $mFatura = new Fatura();
            $arrFatura = array(
                'valor' => $valorParcela,
                'excluido' => 'false',
                'situacao' => $situacao,
                'data_vencimento' => $dataPagamento->format('Y-m-d')
            );
            $faturaId = $mFatura->manter($arrFatura);

            $arrDados = array(
                'contrato_id' => $arrDadosContrato['contrato_id'],
                'numero_parcela' => $parcela,
                'descricao' => "Parcela nº {$parcela}",
                'fatura_id' => $faturaId,
                'excluido' => 'false'
            );

            $this->setDadosNull();
            $this->manter($arrDados);
            $dataPagamento->add(new DateInterval('P1M'));
        }
    }

    private static function getArrayEstadosDeFaturaEmAberto()
    {
        $arrEstadoFaturaEmAberto = [
            Fatura::SITUACAO_FATURA_PENDENTE,
            Fatura::SITUACAO_FATURA_VENCIDA,
            Fatura::SITUACAO_FATURA_GERADA,
            Fatura::SITUACAO_FATURA_AGUARDANDO_FECHAMENTO
        ];
        return $arrEstadoFaturaEmAberto;
    }

    public function getLista($arWhere, $paginacao = false)
    {
        $arWhere[] = "cnf.excluido IS FALSE";

        $sql = "SELECT
					cnf.descricao,
					fat.valor,
					fat.data_vencimento,
					fat.data_pagamento,
					fat.situacao,
					fat.comprovante_pagamento,
					fat.secure_id_iugu,
					cnf.id,
					cnf.fatura_id
				FROM
					{$this->stNomeTabela} cnf
                INNER JOIN
                    financeiro.tb_fatura fat ON fat.id = cnf.fatura_id
				WHERE
					" . implode(' AND ', $arWhere);

        if (!$_POST["ordem"]) {
            $sql .= " ORDER BY cnf.numero_parcela ASC";
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql);
            $dado['dados'] = $this->setParametrosLista($dado['dados']);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            $retorno = $this->setParametrosLista($retorno);
            return $retorno;
        }
    }

    /**
     * @param $arDados
     * @return bool|Iugu_SearchResult|null
     * @throws ErroGeracaoFatura
     */
    public function fecharFaturaDeContratoPorValorFixo($arDados)
    {
        return $this->gerarFaturaIugu($arDados);
    }

    public function getTodosDadosPorId($id)
    {
        $arrWhere = [
            "cnf.id = {$id}"
        ];

        return $this->getTodosDadosPorWhere($arrWhere);
    }

    public function gerarFaturaDePagamentoDeFaturasPendentes()
    {
        $arrFaturas = $this->getListFaturasPendentesNaDataDeFechamentoDaFatura();
        $arrLog[] = "Buscando faturas a serem geradas no dia " . date('d/m/Y') . ".";
        foreach ($arrFaturas as $fatura) {
            try {
                $arrLog[] = "Gerando a fatura da parcela de Nº {$fatura['numero_parcela']} do contrato {$fatura['numero_contrato']}.";
                $this->fecharFatura($fatura);
                $this->commit();
                $arrLog[] = "Fatura gerada com sucesso";
            } catch (Exception $exception) {
                $arrLog[] = "Não foi possivel gerar a fatura da parcela de Nº {$fatura['numero_parcela']} do contrato {$fatura['numero_contrato']}.";
                $this->rollback();
            }
        }

        $arrLog[] = "Fim do processamento.";

        return $arrLog;
    }

    /**
     * @param $arrParams
     * @throws ErroGeracaoFatura
     */
    public function fecharFatura($arrParams)
    {
        switch ($arrParams['tipo_contrato_id']) {
            case TipoContrato::TIPO_POR_VALOR_FIXO:
                $this->fecharFaturaDeContratoPorValorFixo($arrParams);
                break;
            case TipoContrato::TIPO_POR_ATENDIMENTO_REALIZADO:
                $this->fecharFaturaDeContratoPorAtendimento($arrParams);
                break;
        }
    }

    private function getListFaturasPendentesNaDataDeFechamentoDaFatura()
    {
        $dataAtual = new DateTime();
        $dataVencimentoFatura = $dataAtual;
        $dataVencimentoFatura->add(new DateInterval('P' . Contrato::NUM_DIAS_FECHAMENTO_FATURA . 'D'));

        $arWhere[] = "cnf.excluido IS FALSE";
        $arWhere[] = "fat.data_vencimento = '{$dataVencimentoFatura->format('Y-m-d')}'";

        $sql = "
            SELECT 
                cnf.id,
                cnf.fatura_id,
                con.tipo_contrato_id,
                etc.estabelecimento_id,
                con.numero AS numero_contrato,
                con.valor_contratado,
                cnf.numero_parcela
            FROM 
                financeiro.tb_contrato_fatura cnf 
            INNER JOIN
                financeiro.tb_contrato con ON con.id = cnf.contrato_id AND con.excluido IS FALSE
            INNER JOIN
                estabelecimento.tb_estabelecimento_contrato etc ON etc.contrato_id = con.id AND etc.excluido IS FALSE 
            INNER JOIN
                financeiro.tb_fatura fat ON fat.id = cnf.fatura_id AND fat.excluido IS FALSE 
            WHERE
                " . implode(' AND ', $arWhere);

        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : array();
    }

    /**
     * @param $arrParams
     * @return bool|Iugu_SearchResult|null
     * @throws ErroGeracaoFatura
     */
    public function fecharFaturaDeContratoPorAtendimento($arrParams)
    {
        $dataFechamento = new DateTime();
        $dataFechamento = $dataFechamento->format('Y-m-d');

        $valorParcela = $this->getValorParcelaDeContratoPorAtendimento($arrParams, $dataFechamento);

        $arrDadosContratoFatura = [
            'id' => $arrParams['id'],
            'data_fechamento' => $dataFechamento
        ];
        $this->manter($arrDadosContratoFatura);

        $arrDadosFatura = array(
            'id' => $arrParams['fatura_id'],
            'valor' => $valorParcela,
        );
        $mFatura = new Fatura();
        $mFatura->manter($arrDadosFatura);

        return $this->gerarFaturaIugu($arrParams);
    }

    /**
     * @param $arrDados
     * @return bool|Iugu_SearchResult|null
     * @throws ErroGeracaoFatura
     */
    public function gerarFaturaIugu($arrDados)
    {
        $retorno = null;
        $mContrato = new Contrato();

        $fatura = $this->getTodosDadosPorId($arrDados['id']);
        $contrato = $mContrato->getTodosDadosPorId($fatura['contrato_id']);

        $mContatoPessoaJuridica = new ContatoPessoaJuridica();
        $mEnderecoPessoaJuridica = new EnderecoPessoaJuridica();
        $contatoPessoaJuridica = $mContatoPessoaJuridica->listarEspecificosPorPessoaJuridica($contrato['pessoajuridica_id']);
        $listEnderecoPessoaJuridica = $mEnderecoPessoaJuridica->listarPorPessoaJuridica($contrato['pessoajuridica_id']);
        $enderecoPessoaJuridica = $listEnderecoPessoaJuridica[0];

        if (!isset($arrDados['descricao_fatura'])) {
            $arrDados['descricao_fatura'] = "Contrato Nº: " . $contrato['numero'] .
                ', Estabelecimento: ' . $contrato['razao_social'] .
                ', Parcela Nº: ' . $fatura['numero_parcela'];
        }

        $telefone = isset($contatoPessoaJuridica['contato_telefone']) ? $contatoPessoaJuridica['contato_telefone'] : null;


        $arParam = Array(
            'email' => $contatoPessoaJuridica['contato_email'],
            'due_date' => $fatura['data_vencimento'],
            'items' => array(
                array(
                    'description' => utf8_encode($arrDados['descricao_fatura']),
                    'quantity' => 1,
                    'price_cents' => str_replace('.', '', $fatura['valor'])
                )
            ),
            'customer_id' => '',
            'ignore_due_email' => false,
            'custom_variables' => array(
                array(
                    'name' => 'modulo',
                    'value' => 'seguranca'
                ),
                array(
                    'name' => 'classe_integracao',
                    'value' => 'contrato_fatura_integracao_iugu'
                ),
                array(
                    'name' => 'contrato_fatura_id',
                    'value' => $fatura['id']
                ),
                array(
                    'name' => 'fatura_id',
                    'value' => $fatura['fatura_id']
                )
            ),
            'payer' => array(
                'cpf_cnpj' => $contrato['cnpj'],
                'name' => utf8_encode($contrato['razao_social']),
                'phone_prefix' => (!empty($telefone) ? substr($telefone, 1, 2) : ''),
                'phone' => (!empty($telefone) ? substr($telefone, 5) : ''),
                'email' => $contatoPessoaJuridica['contato_email'],
                'address' => array(
                    'zip_code' => $enderecoPessoaJuridica['cep'],
                    'number' => $enderecoPessoaJuridica['complemento']
                )
            )
        );

        $mFatura = new Fatura();
        $retorno = $mFatura->gerarFaturaIugu($fatura['fatura_id'], $arParam);
        return $retorno;
    }

    public function getListaVisaoEstabelecimento($arWhere, $paginacao = false)
    {
        $arWhere[] = "cnf.excluido IS FALSE";

        $sql = "SELECT
					cnf.descricao,
					fat.valor,
					fat.data_vencimento,
					fat.data_pagamento,
					CASE 
					  WHEN fat.situacao = '" . Fatura::SITUACAO_FATURA_PENDENTE . "' THEN 'À Vencer'
					  WHEN fat.situacao = '" . Fatura::SITUACAO_FATURA_GERADA . "' THEN 'À Vencer'
					  WHEN fat.situacao = '" . Fatura::SITUACAO_FATURA_PAGA . "' THEN 'Fatura Paga'
					  WHEN fat.situacao = '" . Fatura::SITUACAO_FATURA_VENCIDA . "' THEN 'Fatura Vencida'
					  WHEN fat.situacao = '" . Fatura::SITUACAO_FATURA_CANCELADA . "' THEN 'Fatura Cancelada'
					  ELSE 'Aguardando Fechamento de Fatura'
					END as str_situacao,
					fat.situacao,
					fat.comprovante_pagamento,
					fat.secure_id_iugu,
					cnf.id,
					cnf.fatura_id
				FROM
					{$this->stNomeTabela} cnf
                INNER JOIN
                    financeiro.tb_fatura fat ON fat.id = cnf.fatura_id
				WHERE
					" . implode(' AND ', $arWhere);

        if (!$_POST["ordem"]) {
            $sql .= " ORDER BY cnf.numero_parcela ASC";
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 12);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public function getTodosDadosPorWhere($arrWhere)
    {
        $sql = "
            SELECT 
                cnf.id, 
                cnf.contrato_id, 
                cnf.numero_parcela, 
                fat.data_vencimento, 
                fat.data_pagamento, 
                fat.comprovante_pagamento, 
                cnf.criado_em, 
                cnf.criado_por, 
                cnf.atualizado_em, 
                cnf.atualizado_por, 
                cnf.excluido, 
                cnf.fatura_id, 
                fat.valor, 
                fat.secure_id_iugu, 
                fat.id_iugu, 
                CASE 
                    WHEN fat.situacao = '" . Fatura::SITUACAO_FATURA_AGUARDANDO_FECHAMENTO . "' THEN 'Aguardando Fechamento de Fatura'
                    WHEN fat.situacao = '" . Fatura::SITUACAO_FATURA_PENDENTE . "' THEN 'Pendente'
                    WHEN fat.situacao = '" . Fatura::SITUACAO_FATURA_GERADA . "' THEN 'Fatura Gerada'
                    WHEN fat.situacao = '" . Fatura::SITUACAO_FATURA_PAGA . "' THEN 'Fatura Paga'
                    WHEN fat.situacao = '" . Fatura::SITUACAO_FATURA_VENCIDA . "' THEN 'Fatura Vencida'
                END as str_situacao,
                fat.situacao,
                con.valor_contratado,
                con.tipo_contrato_id,
                esc.estabelecimento_id
	        FROM 
                financeiro.tb_contrato_fatura cnf
            INNER JOIN
                financeiro.tb_fatura fat ON fat.id = cnf.fatura_id AND fat.excluido IS FALSE
            INNER JOIN
                financeiro.tb_contrato con ON con.id = cnf.contrato_id AND con.excluido IS FALSE
            INNER JOIN
                estabelecimento.tb_estabelecimento_contrato esc ON esc.contrato_id = con.id AND esc.excluido IS FALSE
            WHERE 
                " . implode(' AND ', $arrWhere);

        $retorno = $this->pegaLinha($sql);
        return $retorno ? $retorno : array();
    }

    public function gerarSegundaViaDeFatura($id)
    {
        $dados = $this->getTodosDadosPorId($id);

        $mFatura = new Fatura();
        if($dados['id_iugu']) {
            $retorno = $mFatura->gerarSegundaViaDeFaturaIugu(array('id' => $dados['fatura_id']));
        } else {
            $retorno = $this->gerarFaturaIugu($dados);
        }

        if ($retorno) {
            $mContrato = new Contrato();
            $mContrato->verificarSituacaoDePagamentoDoContrato($dados['contrato_id']);
        }

        return $retorno;
    }

    public function confirmarPagamentoPorComprovante($arrDados)
    {
        $mFatura = new Fatura();
        $mContrato = new Contrato();

        $mFatura->confirmarPagamentoPorComprovante($arrDados);
        $mContrato->verificarSituacaoDePagamentoDoContrato($arrDados['contrato_id']);
    }

    public function setParametrosLista($lista)
    {
        $lista = $lista ? $lista : [];

        foreach ($lista as &$item) {
            $item['p'] = setParam(['fatura_id' => $item['fatura_id'], 'id' => $item['id']], false);
        }

        return $lista;
    }

    public function enviarEmailAvisoCancelamento($id)
    {
        global $email_from;
        $remetente = array();
        $remetente['email'] = $email_from;
        $remetente['nome'] = $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'];

        $mEstabelecimento = new Estabelecimento();
        $mContatoPessoaJuridica = new ContatoPessoaJuridica();
        $mContrato = new Contrato();

        $dados = $this->getTodosDadosPorId($id);
        $dadosContrato = $mContrato->getTodosDadosPorId($dados['contrato_id']);
        $dadosEstabelecimento = $mEstabelecimento->getTodosDados($dados['estabelecimento_id']);

        $valorFormatado = formata_valor($dados['valor']);
        $valorPorExtenso = valorMonetarioExtenso($dados['valor']);
        $numDiasCancelamento = Contrato::DIAS_INADINPLENCIA_CANCELAMENTO_AUTOMATICO;
        $dataVencimento = formata_data($dados['data_vencimento']);
        $contatoPessoaJuridica = $mContatoPessoaJuridica->listarEspecificosPorPessoaJuridica($dadosEstabelecimento['pessoajuridica_id']);

        $descricaoEmail = <<<EMAIL
Verificamos que há uma pendência de pagamento referente a parcela de número <b>{$dados['numero_parcela']}</b> 
do contrato <b>{$dadosContrato['numero']}</b>, estabelecimento <b>{$dadosEstabelecimento['nome_fantasia']}</b> 
no valor de <b>$valorFormatado</b>($valorPorExtenso), com venciemnto em <b>$dataVencimento</b>.</br>
Lembrando que o não pagamento da fatura em até <b>{$numDiasCancelamento}</b> dias após o vencimento gera o cancelamento automático do contrato. </br>
Caso já tenha efetuado o pagamento da fatura, favor desconsiderar a mensagem.        
EMAIL;
        $retornoEmail = enviar_email($remetente, $contatoPessoaJuridica['contato_email'], 'ClubVida - Aviso de Pendência de Pagamento', $descricaoEmail);
        return $retornoEmail;
    }

    public function cancelarFaturasNaoPagasNoIuguPorContrato($contratoId)
    {
        $mFatura = new Fatura();

        $arrEstadoFaturaNaoPaga = [
            Fatura::SITUACAO_FATURA_VENCIDA,
            Fatura::SITUACAO_FATURA_GERADA
        ];

        $strInEstado = "(' " . implode("', '", $arrEstadoFaturaNaoPaga) . " ')";

        $sql = "
            SELECT  
                cnf.id, 
                cnf.fatura_id
	        FROM 
                financeiro.tb_contrato_fatura cnf
            INNER JOIN
                financeiro.tb_fatura fat ON fat.id = cnf.fatura_id AND fat.excluido IS FALSE
            WHERE
                cnf.contrato_id = {$contratoId}
                AND fat.situacao IN {$strInEstado}
        ";

        $arrFatura = $this->carregarArray($sql);

        foreach ($arrFatura as $fatura) {
            if ($fatura['fatura_id']) {
                $mFatura->cancelarFatura($fatura['fatura_id']);
            }
        }
    }

    public function cancelarFaturasEmAbertoPorContrato($contratoId)
    {
        $situacao = Fatura::SITUACAO_FATURA_CANCELADA;

        $arrEstadoFaturaEmAberto = self::getArrayEstadosDeFaturaEmAberto();
        $strInEstado = "('" . implode("', '", $arrEstadoFaturaEmAberto) . "')";

        $sql = "UPDATE 
                  financeiro.tb_fatura 
                SET 
                  situacao = '{$situacao}' 
                WHERE
                  id IN (
                    SELECT
                        fat.id
                    FROM
                        financeiro.tb_fatura fat
                    INNER JOIN
                        financeiro.tb_contrato_fatura cnf ON cnf.fatura_id = fat.id AND cnf.excluido IS false 
                    WHERE
                        fat.excluido IS FALSE 
                        AND fat.situacao IN {$strInEstado} 
                        AND cnf.contrato_id = {$contratoId}
                  )";

        $this->executar($sql);
    }

    public function getValorAPagarDoContrato($contratoId)
    {
        $arrEstadoFaturaEmAberto = self::getArrayEstadosDeFaturaEmAberto();
        $strInEstado = "('" . implode("', '", $arrEstadoFaturaEmAberto) . "')";

        $sql = "SELECT
                    SUM(fat.valor) AS valor_restante
                FROM
                    financeiro.tb_fatura fat
                INNER JOIN
                    financeiro.tb_contrato_fatura cnf ON cnf.fatura_id = fat.id AND cnf.excluido IS false 
                WHERE
                    fat.excluido IS FALSE 
                    AND fat.situacao IN {$strInEstado}
                    AND cnf.contrato_id = {$contratoId}";

        $valorAPagar = $this->pegaUm($sql);
        return $valorAPagar ? $valorAPagar : 0;
    }

    /**
     * Retorna a data do último fechamento de fatura, caso não exista é retornado a data de início
     * de vigência do contrato
     * @param $contratoId
     * @return bool|string
     */
    public function getDataUltimoFechamentoDeFaturaPorContrato($contratoId)
    {
        $sql = "SELECT
                    COALESCE(MAX(cnf.data_fechamento), con.inicio_vigencia) AS data_fechamento
                FROM
                    financeiro.tb_fatura fat
                INNER JOIN
                    financeiro.tb_contrato_fatura cnf ON cnf.fatura_id = fat.id AND cnf.excluido IS false
                INNER JOIN
                    financeiro.tb_contrato con ON con.id = cnf.contrato_id AND con.excluido is false 
                WHERE
                    fat.excluido IS FALSE 
                    AND cnf.contrato_id = {$contratoId}
                GROUP BY
                    con.id";

        return $this->pegaUm($sql);
    }

    /**
     * @param $arrParams
     * @param $dataFechamento
     * @return float|int
     */
    public function getValorParcelaDeContratoPorAtendimento($arrParams, $dataFechamento = null)
    {
        if (is_null($dataFechamento)) {
            $dataFechamento = new DateTime();
            $dataFechamento = $dataFechamento->format('Y-m-d');
        }
        $dataFechamentoFaturaAnterior = $this->getDataUltimoFechamentoDeFaturaPorContrato($arrParams['contrato_id']);

        $arrWhereQtdAtendimento = array(
            'epa.excluido IS FALSE',
            "est.id = {$arrParams['estabelecimento_id']}",
            "epa.inicio::date >= '{$dataFechamentoFaturaAnterior}'",
            "epa.inicio::date < '{$dataFechamento}'",
        );

        $sql = "SELECT
                  COUNT(epa.id) AS qtd
                FROM
                    estabelecimento.tb_estabelecimento_profissional_agenda epa
                LEFT JOIN
                    estabelecimento.tb_sala_estabelecimento se ON se.id = epa.sala_estabelecimento_id AND se.excluido IS FALSE
                INNER JOIN
                    estabelecimento.tb_tipo_consulta tc ON tc.id = epa.tipo_consulta_id AND tc.excluido IS FALSE
                INNER JOIN
                    estabelecimento.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id AND eep.excluido IS FALSE
                INNER JOIN
                    profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id AND ep.excluido IS FALSE
                INNER JOIN
                    profissionalsaude.tb_especialidade esp ON esp.id = ep.especialidade_id AND esp.excluido IS FALSE
                INNER JOIN
                    profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id AND pr.excluido IS FALSE
                INNER JOIN
                    profissionalsaude.tb_tipo_profissional tp ON tp.id = pr.tipo_profissional_id AND tp.excluido IS FALSE
                INNER JOIN
                    estabelecimento.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                INNER JOIN
                    estabelecimento.tb_valor_procedimento vp ON vp.id = epa.valor_procedimento_id AND vp.excluido IS FALSE
                INNER JOIN
                    estabelecimento.tb_procedimento p ON p.id = vp.procedimento_id AND p.excluido IS FALSE
                INNER JOIN
                  workflow.documento doc ON epa.documento_id = doc.docid AND doc.esdid = " . EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA . "
                WHERE
                  " . implode(' AND ', $arrWhereQtdAtendimento);

        $retorno = $this->pegaUm($sql);
        $qtdAtendimentos = $retorno ? $retorno : 0;

        $valorParcela = $qtdAtendimentos * $arrParams['valor_contratado'];
        $valorParcela = $valorParcela < self::VALOR_MININO_FATURA_POR_ATENDIMENTO ? self::VALOR_MININO_FATURA_POR_ATENDIMENTO : $valorParcela;
        return $valorParcela;
    }

    /**
     * @param $arrParams
     * @return bool|Iugu_SearchResult|null
     * @throws ErroGeracaoFatura
     * @throws Exception
     */
    public function gerarFaturaDeCancelamento($arrParams)
    {
        $mContrato = new Contrato();
        $contrato = $mContrato->getTodosDadosPorId($arrParams['contrato_id']);

        $mFatura = new Fatura();

        $dataVencimento = new DateTime();
        $dataVencimento->add(new DateInterval("P3D"));

        $arrDadosFatura = [
            'valor' => round($arrParams['valor_cancelamento'], 2),
            'data_vencimento' => $dataVencimento->format('Y-m-d'),
            'excluido' => 'false'
        ];
        $faturaId = $mFatura->manter($arrDadosFatura);

        $arrDadosContratoFatura = [
            'contrato_id' => $contrato['id'],
            'fatura_id' => $faturaId,
            'descricao' => "Multa de cancelamento referente ao contrato de número {$contrato['numero']}",
            'excluido' => 'false',
            'numero_parcela' => '0'
        ];
        $contratoFaturaId = $this->manter($arrDadosContratoFatura);

        $arrDadosFaturaIugu = [
            'id' => $contratoFaturaId,
            'descricao_fatura' => $arrDadosContratoFatura['descricao']
        ];

        return $this->gerarFaturaIugu($arrDadosFaturaIugu);
    }

    /**
     * @param $id
     * @return string
     * @throws ErroGeracaoFatura
     */
    public function getUrlFaturaIuguPorId($id)
    {
        $fatura = $this->getFaturaIuguPorContratoFaturaId($id);

        $mSimecIugu = new SimecIugu();
        $url = $mSimecIugu->getUrl();
        return "{$url}/{$fatura['secure_id_iugu']}";
    }

    /**
     * @param $id
     * @return bool
     * @throws ErroGeracaoFatura
     */
    public function enviarFaturaIuguParaEmailPorId($id)
    {
        $fatura = $this->getFaturaIuguPorContratoFaturaId($id);

        $mFatura = new Fatura();
        return $mFatura->enviarFaturaPorEmail(array('id' => $fatura['fatura_id']));
    }

    /**
     * @param $id
     * @return array|bool
     * @throws ErroGeracaoFatura
     */
    public function getFaturaIuguPorContratoFaturaId($id)
    {
        $fatura = $this->getTodosDadosPorId($id);
        if (empty($fatura['id_iugu']) && $fatura['tipo_contrato_id'] == TipoContrato::TIPO_POR_VALOR_FIXO) {
            $this->fecharFaturaDeContratoPorValorFixo($fatura);
        }
        return $fatura;
    }

    public static function getWhereSituacaoFaturaParaRelatorio($situacao)
    {
        $whereSituacao = "fat.situacao = '{$situacao}'";

        if ($situacao == Fatura::SITUACAO_FATURA_GERADA) {
            $arrSituacaoAVencer = [
                Fatura::SITUACAO_FATURA_GERADA,
                Fatura::SITUACAO_FATURA_PENDENTE
            ];
            $whereSituacao = "fat.situacao IN ('" . implode("', '", $arrSituacaoAVencer) . "')";
        }

        return $whereSituacao;
    }

    public function reabirirFaturasCanceladasPorContrato($contratoId)
    {
        $situacaoCancelada = Fatura::SITUACAO_FATURA_CANCELADA;
        $situacaoPendente = Fatura::SITUACAO_FATURA_PENDENTE;

        $sql = "UPDATE 
                  financeiro.tb_fatura 
                SET 
                  situacao = '{$situacaoPendente}' 
                WHERE
                  id IN (
                    SELECT
                        fat.id
                    FROM
                        financeiro.tb_fatura fat
                    INNER JOIN
                        financeiro.tb_contrato_fatura cnf ON cnf.fatura_id = fat.id AND cnf.excluido IS false 
                    WHERE
                        fat.excluido IS FALSE 
                        AND fat.situacao = '{$situacaoCancelada}' 
                        AND cnf.contrato_id = {$contratoId}
                  )";

        $this->executar($sql);
    }
}


