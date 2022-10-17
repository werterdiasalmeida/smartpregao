<?php

class Fatura extends Modelo
{
    const SITUACAO_FATURA_AGUARDANDO_FECHAMENTO = 'A';

    const SITUACAO_FATURA_PENDENTE = 'N';

    const SITUACAO_FATURA_GERADA = 'G';

    const SITUACAO_FATURA_PAGA = 'P';

    const SITUACAO_FATURA_VENCIDA = 'V';

    const SITUACAO_FATURA_CANCELADA = 'C';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "financeiro.tb_fatura";

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
        'data_vencimento' => null,
        'data_pagamento' => null,
        'comprovante_pagamento' => null,
        'valor' => null,
        'secure_id_iugu' => null,
        'id_iugu' => null,
        'situacao' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
        'json_fatura_iugu' => null
    );

    public function enviarFaturaPorEmail($arDados)
    {
        $this->recuperarLinha('*', array("id = {$arDados['id']}"));
        $fatura = $this->getDados();

        $iugo = new SimecIugu();
        return $iugo->enviarFaturaPorEmail($fatura['id_iugu']);
    }

    public function confirmarPagamentoPorComprovante($arrParams)
    {
        $arrDados = array(
            'id' => $arrParams['id'],
            'data_pagamento' => $arrParams['data_pagamento'],
            'comprovante_pagamento' => $arrParams['comprovante_pagamento'],
            'situacao' => self::SITUACAO_FATURA_PAGA,
            'atualizado_em' => 'now()',
            'atualizado_por' => $_SESSION['usuario_id']
        );

        $this->popularDadosObjeto($arrDados)->salvar($arrDados);
    }

    public function salvarEGerarFaturaIugu($arrData)
    {
        if (!isset($arrData['data_venciemnto'])) {
            $novaDataVencimento = new DateTime();
            $novaDataVencimento->add(new DateInterval("P3D"));
            $arrData['data_vencimento'] = $novaDataVencimento->format('Y-m-d');
        }

        $dados = array(
            'situacao' => self::SITUACAO_FATURA_PENDENTE,
            'data_vencimento' => $arrData['data_vencimento'],
            'valor' => $arrData['valor'],
            'atualizado_em' => 'now()',
            'excluido' => 'false',
            'atualizado_por' => $_SESSION['usuario_id'],
        );

        $faturaId = $this->manter($dados);

        $arParam = Array(
            'email' => $arrData['email'],
            'due_date' => $arrData['data_vencimento'],
            'items' => array(
                array(
                    'description' => utf8_encode($arrData['descricao']),
                    'quantity' => 1,
                    'price_cents' => str_replace('.', '', $arrData['valor'])
                )
            ),
            'customer_id' => '',
            'ignore_due_email' => false,
            'custom_variables' => array(
                array(
                    'name' => 'fatura_id',
                    'value' => $faturaId
                )
            ),
            'payer' => array(
                'cpf_cnpj' => $arrData['cnpj'],
                'name' => utf8_encode($arrData['razao_social']),
                'phone_prefix' => (isset($arrData['telefone']) && !empty($arrData['telefone']) ? substr($arrData['telefone'], 1, 2) : ''),
                'phone' => (isset($arrData['telefone']) && !empty($arrData['telefone']) ? substr($arrData['telefone'], 5) : ''),
                'email' => $arrData['email'],
                'address' => array(
                    'zip_code' => $arrData['cep'],
                    'number' => $arrData['numero']
                )
            )
        );

        $this->gerarFaturaIugu($faturaId, $arParam);

        return $faturaId;
    }


    /**
     * @param $faturaId
     * @param $arrDadosFaturaIugu
     * @return bool|Iugu_SearchResult
     * @throws ErroGeracaoFatura
     */
    public function gerarFaturaIugu($faturaId, $arrDadosFaturaIugu)
    {
        $iugo = new SimecIugu();
        $retorno = $iugo->criarFatura($arrDadosFaturaIugu);

        if ($retorno !== false) {
            $dados = array(
                'id' => $faturaId,
                'situacao' => self::SITUACAO_FATURA_GERADA,
                'id_iugu' => $retorno->id,
                'secure_id_iugu' => $retorno->secure_id,
                'atualizado_em' => 'now()',
                'atualizado_por' => $_SESSION['usuario_id'],
                'json_fatura_iugu' => json_encode($arrDadosFaturaIugu)
            );

            $this->popularDadosObjeto($dados)->salvar();
        } else {
            $arrErros = $iugo->getErros();
            $msg = "Falha ao gerar a Fatura. </br> Alguns erros do iugu foram retornados: </br>";
            $msg .= implode('</br>', $arrErros);
            throw new ErroGeracaoFatura($msg);
        }
        return $retorno;
    }

    public function alterarSituacaoFaturaComSituacaoIugu($arrParams, $sitaucaoIugo)
    {
        $arrData['id'] = $arrParams['fatura_id'];

        $faturaAntesDaAlteracao = $this->carregarPorId($arrData['id'])->getDados();

        if ($faturaAntesDaAlteracao['situacao'] != self::SITUACAO_FATURA_CANCELADA) {
            switch ($sitaucaoIugo) {
                case SimecIugu::SITUACAO_FATURA_PAGA:
                    $arrData['situacao'] = self::SITUACAO_FATURA_PAGA;
                    $arrData['data_pagamento'] = 'now()';
                    break;
                case SimecIugu::SITUACAO_FATURA_EXPIRADA:
                    $arrData['situacao'] = self::SITUACAO_FATURA_VENCIDA;
                    break;
            }

            $arrData['atualizado_em'] = 'now()';
            $arrData['atualizado_por'] = null;

            $this->setDadosNull();
            $arrData['id'] = $this->popularDadosObjeto($arrData)->salvar();
        }

        return $arrData['id'];
    }

    public function alterarSituacaoDeFaturasParaVencida()
    {
        $situacaoVencida = self::SITUACAO_FATURA_VENCIDA;
        $situacaoPaga = self::SITUACAO_FATURA_PAGA;

        $sql = "UPDATE 
                      financeiro.tb_fatura SET situacao= '{$situacaoVencida}'
	            WHERE 
	                  excluido IS FALSE 
	                  AND situacao NOT IN ('{$situacaoPaga}', '{$situacaoVencida}')
	                  AND data_vencimento < now()::date;";

        $this->pegaUm($sql);
    }

    public function gerarSegundaViaDeFaturaIugu($params)
    {
        $dadosFatura = $this->carregarPorId($params['id'])->getDados();

        $mSimecIugu = new SimecIugu();

        if (!isset($params['data_venciemnto'])) {
            $novaDataVencimento = new DateTime(date('Y-m-d'));
            $novaDataVencimento->add(new DateInterval("P3D"));
            $params['data_vencimento'] = $novaDataVencimento->format('Y-m-d');
        }

        $dadosFaturaIuguAnterior = $mSimecIugu->getFaturaById($dadosFatura['id_iugu']);

        $dadosFaturaIugu = false;
        switch ($dadosFaturaIuguAnterior->status) {
            case SimecIugu::SITUACAO_FATURA_PENDENTE:
                $dadosFaturaIugu = $mSimecIugu->gerarSegundaViaFatura($dadosFatura['id_iugu'], $params);
                break;
            case SimecIugu::SITUACAO_FATURA_EXPIRADA:
                $arrDadosFaturaIugu = utf8ArrayDecode(json_decode($dadosFatura['json_fatura_iugu'], true));
                $arrDadosFaturaIugu['due_date'] = $params['data_venciemnto'];
                $dadosFaturaIugu = $mSimecIugu->criarFatura($arrDadosFaturaIugu);
                break;
        }

        if ($dadosFaturaIugu) {
            $dados = array(
                'id' => $dadosFatura['id'],
                'situacao' => self::SITUACAO_FATURA_GERADA,
                'data_vencimento' => $params['data_vencimento'],
                'id_iugu' => $dadosFaturaIugu->id,
                'secure_id_iugu' => $dadosFaturaIugu->secure_id,
                'atualizado_em' => 'now()',
                'atualizado_por' => $_SESSION['usuario_id'],
            );
            $this->setDadosNull();
            $this->popularDadosObjeto($dados)->salvar();
            return $dadosFatura['id'];
        }

        return false;
    }

    public function cancelarFatura($faturaId)
    {
        $arrDadosFatura = $this->carregarPorId($faturaId)->getDados();

        $iugo = new SimecIugu();
        $sucessoCancelamento = $iugo->cancelarFatura($arrDadosFatura['id_iugu']);

        $arrDadosFatura['situacao'] = Fatura::SITUACAO_FATURA_CANCELADA;
        $this->manter($arrDadosFatura);

        return $sucessoCancelamento;
    }

    public static function getArraySituacaoFatura()
    {
        return array(
            self::SITUACAO_FATURA_AGUARDANDO_FECHAMENTO => ['descricao' => 'Aguardando Fechamento de Fatura'],
            self::SITUACAO_FATURA_PENDENTE => ['descricao' => 'Pendente'],
            self::SITUACAO_FATURA_GERADA => ['descricao' => 'Fatura Gerada'],
            self::SITUACAO_FATURA_PAGA => ['descricao' => 'Fatura Paga'],
            self::SITUACAO_FATURA_VENCIDA => ['descricao' => 'Fatura Vencida'],
            self::SITUACAO_FATURA_CANCELADA => ['descricao' => 'Fatura Cancelada']
        );
    }
}