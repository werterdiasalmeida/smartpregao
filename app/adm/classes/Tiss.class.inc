<?php

class Tiss
{
    const VERSAO_TISS = '3.04.00';

    /**
     * @var DOMDocument
     */
    protected $xml;

    /**
     * @var DOMElement
     */
    protected $guiasTiss;

    public function gerar($loteId)
    {
        $mLote = new Lote();
        $lote = $mLote->getTodosDados($loteId, false);

        $mLoteGuia = new LoteGuia();
        $guias = $mLoteGuia->getGuiasByLote($loteId);

        $this->xml = new DOMDocument("1.0", "UTF-8");
        //remove os espacos em branco
        $this->xml->preserveWhiteSpace = false;
        //Realizar a quebra dos blocos do XML por linha
        $this->xml->formatOutput = true;

        $mensagemTISS = $this->xml->createElement("ans:mensagemTISS");
        $this->xml->appendChild($mensagemTISS);

        //Criação dos elementos do Namespace
        $this->xml->createAttributeNS('http://www.w3.org/2000/09/xmldsig//', 'ds:attr');
        $this->xml->createAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:attr');
        $this->xml->createAttributeNS('http://www.ans.gov.br/padroes/tiss/schemas', 'ans:attr');

        $this->gerarCabecalhoTransacao($mensagemTISS, $lote);
        $this->gerarPrestadorParaOperadora($mensagemTISS, $lote, $guias);
        $this->gerarEpilogo($mensagemTISS, $lote, $guias);

        return $this->xml;
    }

    protected function gerarCabecalhoTransacao(DOMElement $mensagemTISS, $lote)
    {
        $cabecalho = $this->xml->createElement("ans:cabecalho");
        $mensagemTISS->appendChild($cabecalho);

        $this->gerarIdentificacaoTransacao($cabecalho, $lote);
        $this->gerarOrigemTransacao($cabecalho, $lote);
        $this->gerarDestinoTransacao($cabecalho, $lote);

        $padrao = $this->xml->createElement("ans:Padrao", self::VERSAO_TISS);
        $cabecalho->appendChild($padrao);

        return $cabecalho;
    }

    protected function gerarIdentificacaoTransacao(DOMElement $cabecalhoXml, array $lote)
    {
        $identificacaoTransacao = $this->xml->createElement("ans:identificacaoTransacao");
        $cabecalhoXml->appendChild($identificacaoTransacao);

        $tipoTransacao = $this->xml->createElement("ans:tipoTransacao", 'ENVIO_LOTE_GUIAS');
        $identificacaoTransacao->appendChild($tipoTransacao);

        $sequencialTransacao = $this->xml->createElement("ans:sequencialTransacao", $lote['sequencial']); /* TODO */
        $identificacaoTransacao->appendChild($sequencialTransacao);

        $dataRegistroTransacao = $this->xml->createElement("ans:dataRegistroTransacao", date('Y-m-d'));
        $identificacaoTransacao->appendChild($dataRegistroTransacao);

        $horaRegistroTransacao = $this->xml->createElement("ans:horaRegistroTransacao", date('H:i:s'));
        $identificacaoTransacao->appendChild($horaRegistroTransacao);

        return $identificacaoTransacao;
    }

    protected function gerarOrigemTransacao(DOMElement $cabecalhoXml, array $lote)
    {
        $origem = $this->xml->createElement("ans:origem");
        $cabecalhoXml->appendChild($origem);

        $this->gerarPrestador($origem, ['cnpj' => $lote['estabelecimento_cnpj']]);

        return $origem;
    }

    protected function gerarDestinoTransacao(DOMElement $cabecalhoXml, array $lote)
    {
        $origem = $this->xml->createElement("ans:destino");
        $cabecalhoXml->appendChild($origem);

        $this->gerarPrestador($origem, ['cnpj' => $lote['convenio_cnpj']]);

        return $origem;
    }

    protected function gerarPrestador(DOMElement $prestadorXml, array $prestador)
    {
        $identificacaoPrestador = $this->xml->createElement("ans:identificacaoPrestador");
        $prestadorXml->appendChild($identificacaoPrestador);

        $cnpj = $this->xml->createElement("ans:CNPJ", str_replace(['.', '-', '/'], '', $prestador['cnpj']));
        $identificacaoPrestador->appendChild($cnpj);

        return $identificacaoPrestador;
    }

    protected function gerarPrestadorParaOperadora(DOMElement $mensagemTISS, $lote, $guias)
    {
        $prestadorOperadora = $this->xml->createElement("ans:prestadorParaOperadora");
        $mensagemTISS->appendChild($prestadorOperadora);

        $loteGuias = $this->xml->createElement("ans:loteGuias");
        $prestadorOperadora->appendChild($loteGuias);

        $numeroLote = $this->xml->createElement("ans:numeroLote", $lote['numero']);
        $loteGuias->appendChild($numeroLote);

        $guiasTiss = $this->xml->createElement("ans:guiasTISS");
        $loteGuias->appendChild($guiasTiss);

        $mTipoGuia = new TipoGuia();
        $tipoGuia = $mTipoGuia->getTipoById($lote['tipo_guia_id']);

        foreach ($guias as $guia) {
            $this->gerarGuia($guiasTiss, utf8ArrayEncode($guia['guia']), $tipoGuia);
        }

        return $loteGuias;
    }

    protected function gerarEpilogo(DOMElement $mensagemTISS, $lote, $guias)
    {
        $epilogo = $this->xml->createElement("ans:epilogo");
        $mensagemTISS->appendChild($epilogo);

        $numeroHash = md5(serialize(array_merge($lote, $guias)));
        $hash = $this->xml->createElement("ans:hash", $numeroHash);
        $epilogo->appendChild($hash);
    }

    protected function gerarGuia(DOMElement $guiasTiss, array $guia, $tipoGuia)
    {
        switch ($tipoGuia) {
            case TipoGuia::TIPO_GUIA_EXAME :
                $guia = $this->gerarExame($guiasTiss, $guia);
                break;
            default :
                $guia = $this->gerarConsulta($guiasTiss, $guia);
                break;
        }

        return $guia;
    }

    protected function gerarConsulta(DOMElement $guiasTiss, $guia)
    {
        $guiaConsulta = $this->xml->createElement("ans:guiaConsulta");
        $guiasTiss->appendChild($guiaConsulta);

        $this->montarCabecalhoGuia($guiaConsulta, $guia);

        if ($guia['numero_guia_operadora']) {
            $numeroGuiaOperadora = $this->xml->createElement("ans:numeroGuiaOperadora", $guia['numero_guia_operadora']);
            $guiaConsulta->appendChild($numeroGuiaOperadora);
        }

        $this->montarBeneficiario($guiaConsulta, $guia);
        $this->montarContratadoExecutanteConsulta($guiaConsulta, $guia);
        $this->montarContratadoProfissional($guiaConsulta, $guia);

        $indicacaoAcidente = $this->xml->createElement("ans:indicacaoAcidente", $guia['indicacao_acidente']);
        $guiaConsulta->appendChild($indicacaoAcidente);

        $this->montarDadosAtendimento($guiaConsulta, $guia);

        if ($guia['observacao']) {
            $observacao = $this->xml->createElement("ans:observacao", $guia['observacao']);
            $guiaConsulta->appendChild($observacao);
        }

        return $guiaConsulta;
    }

    protected function montarCabecalhoGuia(DOMElement $guiaXml, array $guia)
    {
        $cabecalhoConsulta = $this->xml->createElement("ans:cabecalhoConsulta");
        $guiaXml->appendChild($cabecalhoConsulta);

        $this->montarCabecalho($cabecalhoConsulta, $guia);

        return $cabecalhoConsulta;
    }

    protected function montarCabecalho(DOMElement $cabecalho, array $guia)
    {
        $registroANS = $this->xml->createElement("ans:registroANS", $guia['registro_ans']);
        $cabecalho->appendChild($registroANS);

        $numeroGuiaPrestador = $this->xml->createElement("ans:numeroGuiaPrestador", $guia['numero_guia_prestador']);
        $cabecalho->appendChild($numeroGuiaPrestador);

        return $cabecalho;
    }

    protected function montarBeneficiario(DOMElement $guiaXml, $guia)
    {
        $dadosBeneficiario = $this->xml->createElement("ans:dadosBeneficiario");
        $guiaXml->appendChild($dadosBeneficiario);

        $numeroCarteira = $this->xml->createElement("ans:numeroCarteira", $guia['numero_carteira']);
        $dadosBeneficiario->appendChild($numeroCarteira);

        $atendimentoRN = $this->xml->createElement("ans:atendimentoRN", $guia['atendimento_rn']);
        $dadosBeneficiario->appendChild($atendimentoRN);

        $nomeBeneficiario = $this->xml->createElement("ans:nomeBeneficiario", $guia['nome_beneficiario']);
        $dadosBeneficiario->appendChild($nomeBeneficiario);

        if ($guia['cartao_nacional_saude']) {
            $numeroCNS = $this->xml->createElement("ans:numeroCNS", $guia['cartao_nacional_saude']);
            $dadosBeneficiario->appendChild($numeroCNS);
        }

        return $dadosBeneficiario;
    }

    protected function montarContratadoExecutanteConsulta(DOMElement $guiaXml, array $guia)
    {
        $contratadoExecutante = $this->montarContratadoExecutante($guiaXml, $guia);

        $cnes = $this->xml->createElement("ans:CNES", $guia['cnes']);
        $contratadoExecutante->appendChild($cnes);

        return $contratadoExecutante ;
    }

    protected function montarContratadoExecutante(DOMElement $guiaXml, array $guia)
    {
        $contratadoExecutante = $this->xml->createElement("ans:contratadoExecutante");
        $guiaXml->appendChild($contratadoExecutante);

        $this->montarContratado($contratadoExecutante, $guia);

        return $contratadoExecutante;
    }

    protected function montarContratadoProfissional(DOMElement $guiaXml, array $guia)
    {
        $profissionalExecutante = $this->xml->createElement("ans:profissionalExecutante");
        $guiaXml->appendChild($profissionalExecutante);

        $this->montarProfissional($profissionalExecutante, $guia);

        return $profissionalExecutante;
    }

    protected function montarDadosAtendimento(DOMElement $guiaXml, array $guia)
    {
        $dadosAtendimento = $this->xml->createElement("ans:dadosAtendimento");
        $guiaXml->appendChild($dadosAtendimento);

        $dataAtendimento = $this->xml->createElement("ans:dataAtendimento", $guia['data_atendimento']);
        $dadosAtendimento->appendChild($dataAtendimento);

        $tipoConsulta = $this->xml->createElement("ans:tipoConsulta", $guia['tipo_consulta']);
        $dadosAtendimento->appendChild($tipoConsulta);

        $this->montarProcedimento($dadosAtendimento, $guia);

        return $dadosAtendimento;
    }

    private function montarProcedimento(DOMElement $dadosAtendimento, array $guia)
    {
        $procedimento = $this->xml->createElement("ans:procedimento");
        $dadosAtendimento->appendChild($procedimento);

        $codigoTabela = $this->xml->createElement("ans:codigoTabela", $guia['codigo_tabela']);
        $procedimento->appendChild($codigoTabela);

        $codigoProcedimento = $this->xml->createElement("ans:codigoProcedimento", $guia['codigo_procedimento']);
        $procedimento->appendChild($codigoProcedimento);

        $valorProcedimento = $this->xml->createElement("ans:valorProcedimento", $guia['valor_procedimento']);
        $procedimento->appendChild($valorProcedimento);

        return $procedimento;
    }

    private function gerarExame(DOMElement $guiasTiss, array $guia)
    {
        $guiaSPSADT = $this->xml->createElement("ans:guiaSP-SADT");
        $guiasTiss->appendChild($guiaSPSADT);

        $this->montarCabecalhoExame($guiaSPSADT, $guia);
        $this->montarAutorizacao($guiaSPSADT, $guia);
        $this->montarBeneficiario($guiaSPSADT, $guia);
        $this->montarSolicitante($guiaSPSADT, $guia);
        $this->montarSolicitacao($guiaSPSADT, $guia);
        $this->montarExecutante($guiaSPSADT, $guia);
        $this->montarDadosAtendimentoExame($guiaSPSADT, $guia);
        $this->montarProcedimentosExecutados($guiaSPSADT, $guia);

        if ($guia['observacao']) {
            $observacao = $this->xml->createElement("ans:observacao", $guia['observacao']);
            $guiaSPSADT->appendChild($observacao);
        }

        $this->montarValorTotal($guiaSPSADT, $guia);

        return $guiaSPSADT;
    }

    protected function montarCabecalhoExame(DOMElement $guiaXml, array $guia)
    {
        $cabecalhoGuia = $this->xml->createElement("ans:cabecalhoGuia");
        $guiaXml->appendChild($cabecalhoGuia);

        $this->montarCabecalho($cabecalhoGuia, $guia);

        if ($guia['numero_guia_principal']) {
            $guiaPrincipal = $this->xml->createElement("ans:guiaPrincipal", $guia['numero_guia_principal']);
            $cabecalhoGuia->appendChild($guiaPrincipal);
        }

        return $cabecalhoGuia;
    }

    protected function montarAutorizacao(DOMElement $guiaXml, array $guia)
    {
        if(empty($guia['numero_guia_operadora']) && empty($guia['data_autorizacao'])
            && empty($guia['senha']) && empty($guia['data_validade_senha'])) {
            return null;
        }

        $dadosAutorizacao = $this->xml->createElement("ans:dadosAutorizacao");
        $guiaXml->appendChild($dadosAutorizacao);

        if($guia['numero_guia_operadora']) {
            $numeroGuiaOperadora = $this->xml->createElement("ans:numeroGuiaOperadora", $guia['numero_guia_operadora']);
            $dadosAutorizacao->appendChild($numeroGuiaOperadora);
        }

        if($guia['data_autorizacao']) {
            $dataAutorizacao = $this->xml->createElement("ans:dataAutorizacao", $guia['data_autorizacao']);
            $dadosAutorizacao->appendChild($dataAutorizacao);
        }

        if($guia['senha']) {
            $senha = $this->xml->createElement("ans:senha", $guia['senha']);
            $dadosAutorizacao->appendChild($senha);
        }

        if($guia['data_validade_senha']) {
            $dataValidadeSenha = $this->xml->createElement("ans:dataValidadeSenha", $guia['data_validade_senha']);
            $dadosAutorizacao->appendChild($dataValidadeSenha);
        }

        return $dadosAutorizacao;
    }

    protected function montarSolicitante(DOMElement $guiaXml, array $guia)
    {
        $dadosSolicitante = $this->xml->createElement("ans:dadosSolicitante");
        $guiaXml->appendChild($dadosSolicitante);

        $this->montarContratadoSolicitante($dadosSolicitante, $guia);
        $this->montarProfissionalSolicitante($dadosSolicitante, $guia);

        return $dadosSolicitante;
    }

    protected function montarContratadoSolicitante(DOMElement $guiaXml, array $guia)
    {
        $contratadoSolicitante = $this->xml->createElement("ans:contratadoSolicitante");
        $guiaXml->appendChild($contratadoSolicitante);

        $this->montarContratado($contratadoSolicitante, $guia);

        return $contratadoSolicitante;
    }

    protected function montarProfissionalSolicitante(DOMElement $guiaXml, array $guia)
    {
        $profissionalSolicitante = $this->xml->createElement("ans:profissionalSolicitante");
        $guiaXml->appendChild($profissionalSolicitante);

        $this->montarProfissional($profissionalSolicitante, $guia);

        return $profissionalSolicitante;
    }

    protected function montarProfissional(DOMElement $guiaXml, array $guia)
    {
        $nomeProfissional = $this->xml->createElement("ans:nomeProfissional", $guia['nome_profissional']);
        $guiaXml->appendChild($nomeProfissional);

        $conselhoProfissional = $this->xml->createElement("ans:conselhoProfissional", $guia['conselho_profissional']);
        $guiaXml->appendChild($conselhoProfissional);

        $numeroConselhoProfissional = $this->xml->createElement("ans:numeroConselhoProfissional", $guia['numero_conselho_profissional']);
        $guiaXml->appendChild($numeroConselhoProfissional);

        $uf = $this->xml->createElement("ans:UF", $guia['uf_conselho_profissional']);
        $guiaXml->appendChild($uf);

        $cbos = $this->xml->createElement("ans:CBOS", $guia['cbos']);
        $guiaXml->appendChild($cbos);
    }

    private function montarContratado(DOMElement $guiaXml, array $guia)
    {
        $codigoPrestadorNaOperadora = $this->xml->createElement("ans:codigoPrestadorNaOperadora", $guia['codigo_operadora']);
        $guiaXml->appendChild($codigoPrestadorNaOperadora);

        $nomeContratado = $this->xml->createElement("ans:nomeContratado", $guia['nome_contratado']);
        $guiaXml->appendChild($nomeContratado);
    }

    protected function montarSolicitacao(DOMElement $guiaXml, array $guia)
    {
        $dadosSolicitacao = $this->xml->createElement("ans:dadosSolicitacao");
        $guiaXml->appendChild($dadosSolicitacao);

        $dataSolicitacao = $this->xml->createElement("ans:dataSolicitacao", $guia['data_solicitacao']);
        $dadosSolicitacao->appendChild($dataSolicitacao);

        $caraterAtendimento = $this->xml->createElement("ans:caraterAtendimento", $guia['carater_atendimento']);
        $dadosSolicitacao->appendChild($caraterAtendimento);

        if($guia['indicacao_clinica']) {
            $indicacaoClinica = $this->xml->createElement("ans:indicacaoClinica", $guia['indicacao_clinica']);
            $dadosSolicitacao->appendChild($indicacaoClinica);
        }

        return $dadosSolicitacao;
    }

    protected function montarExecutante(DOMElement $guiaXml, array $guia)
    {
        $dadosExecutante = $this->xml->createElement("ans:dadosExecutante");
        $guiaXml->appendChild($dadosExecutante);

        $contratadoExecutante = [
            'cnes' => $guia['cnes'],
            'codigo_operadora' => $guia['codigo_operadora_executante'],
            'nome_contratado' => $guia['nome_contratado_executante'],
        ];
        $this->montarContratadoExecutante($dadosExecutante, $contratadoExecutante);

        $cnes = $this->xml->createElement("ans:CNES", $contratadoExecutante['cnes']);
        $dadosExecutante->appendChild($cnes);

        return $dadosExecutante;
    }

    protected function montarDadosAtendimentoExame(DOMElement $guiaXml, array $guia)
    {
        $dadosAtendimento = $this->xml->createElement("ans:dadosAtendimento");
        $guiaXml->appendChild($dadosAtendimento);

        $tipoAtendimento = $this->xml->createElement("ans:tipoAtendimento", $guia['tipo_atendimento']);
        $dadosAtendimento->appendChild($tipoAtendimento);

        $indicacaoAcidente = $this->xml->createElement("ans:indicacaoAcidente", $guia['indicacao_acidente']);
        $dadosAtendimento->appendChild($indicacaoAcidente);

        if ($guia['tipo_consulta']) {
            $tipoConsulta = $this->xml->createElement("ans:tipoConsulta", $guia['tipo_consulta']);
            $dadosAtendimento->appendChild($tipoConsulta);
        }

        if ($guia['motivo_encerramento']) {
            $motivoEncerramento = $this->xml->createElement("ans:motivoEncerramento", $guia['motivo_encerramento']);
            $dadosAtendimento->appendChild($motivoEncerramento);
        }

        return $dadosAtendimento;
    }

    protected function montarProcedimentosExecutados(DOMElement $guiaXml, array $guia)
    {
        $procedimentosExecutados = $this->xml->createElement("ans:procedimentosExecutados");
        $guiaXml->appendChild($procedimentosExecutados);

        $guia['executado'] = is_array($guia['executado']) ? $guia['executado'] : [];
        foreach ($guia['executado'] as $sequencial => $procedimentoExecutado) {
            $procedimentoExecutado['sequencial_item'] = 1 + $sequencial;
            $this->montarProcedimentoExecutado($procedimentosExecutados, $procedimentoExecutado);
        }

        return $procedimentosExecutados;
    }

    protected function montarValorTotal(DOMElement $guiaXml, array $guia)
    {
        $valorTotal = $this->xml->createElement("ans:valorTotal");
        $guiaXml->appendChild($valorTotal);

        if ($guia['total_procedimentos']) {
            $valorProcedimentos = $this->xml->createElement("ans:valorProcedimentos", $guia['total_procedimentos']);
            $valorTotal->appendChild($valorProcedimentos);
        }

        if ($guia['total_taxas_alugueis']) {
            $valorTaxasAlugueis = $this->xml->createElement("ans:valorTaxasAlugueis", $guia['total_taxas_alugueis']);
            $valorTotal->appendChild($valorTaxasAlugueis);
        }

        if ($guia['total_materiais']) {
            $valorMateriais = $this->xml->createElement("ans:valorMateriais", $guia['total_materiais']);
            $valorTotal->appendChild($valorMateriais);
        }

        if ($guia['total_medicamentos']) {
            $valorMedicamentos = $this->xml->createElement("ans:valorMedicamentos", $guia['total_medicamentos']);
            $valorTotal->appendChild($valorMedicamentos);
        }

        if ($guia['total_opme']) {
            $valorOPME = $this->xml->createElement("ans:valorOPME", $guia['total_opme']);
            $valorTotal->appendChild($valorOPME);
        }

        if ($guia['total_gases_medicinais']) {
            $valorGasesMedicinais = $this->xml->createElement("ans:valorGasesMedicinais", $guia['total_gases_medicinais']);
            $valorTotal->appendChild($valorGasesMedicinais);
        }

        $valorProcedimentos = $this->xml->createElement("ans:valorTotalGeral", $guia['total_geral']);
        $valorTotal->appendChild($valorProcedimentos);

        return $valorTotal;
    }

    private function montarProcedimentoExecutado(DOMElement $procedimentosExecutados, array $guia)
    {
        $procedimentoExecutado = $this->xml->createElement("ans:procedimentoExecutado");
        $procedimentosExecutados->appendChild($procedimentoExecutado);

        $sequencialItem = $this->xml->createElement("ans:sequencialItem", $guia['sequencial_item']);
        $procedimentoExecutado->appendChild($sequencialItem);

        $dataExecucao = $this->xml->createElement("ans:dataExecucao", $guia['data_execucao']);
        $procedimentoExecutado->appendChild($dataExecucao);

        if ($guia['hora_inicial_execucao']) {
            $horaInicial = $this->xml->createElement("ans:horaInicial", $guia['hora_inicial_execucao'] . ':00');
            $procedimentoExecutado->appendChild($horaInicial);
        }

        if ($guia['hora_final_execucao']) {
            $horaFinal = $this->xml->createElement("ans:horaFinal", $guia['hora_final_execucao'] . ':00');
            $procedimentoExecutado->appendChild($horaFinal);
        }

        $this->montarProcedimentoDados($procedimentoExecutado, $guia);

        $quantidadeExecutada = $this->xml->createElement("ans:quantidadeExecutada", $guia['qtd_execucao']);
        $procedimentoExecutado->appendChild($quantidadeExecutada);

        if ($guia['via']) {
            $viaAcesso = $this->xml->createElement("ans:viaAcesso", $guia['via']);
            $procedimentoExecutado->appendChild($viaAcesso);
        }

        if ($guia['tec']) {
            $tecnicaUtilizada = $this->xml->createElement("ans:tecnicaUtilizada", $guia['tec']);
            $procedimentoExecutado->appendChild($tecnicaUtilizada);
        }

        $reducaoAcrescimo = $this->xml->createElement("ans:reducaoAcrescimo", $guia['fator_red_acresc'] ?: 0);
        $procedimentoExecutado->appendChild($reducaoAcrescimo);

        $valorUnitario = $this->xml->createElement("ans:valorUnitario", $guia['valor_unitario']);
        $procedimentoExecutado->appendChild($valorUnitario);

        $valorTotal = $this->xml->createElement("ans:valorTotal", $guia['valor_total']);
        $procedimentoExecutado->appendChild($valorTotal);

        return $procedimentoExecutado;
    }

    public function montarProcedimentoDados(DOMElement $procedimentoExecutado, array $guia)
    {
        $procedimento = $this->xml->createElement("ans:procedimento");
        $procedimentoExecutado->appendChild($procedimento);

        $codigoTabela = $this->xml->createElement("ans:codigoTabela", $guia['codigo_tabela_execucao']);
        $procedimento->appendChild($codigoTabela);

        $codigoProcedimento = $this->xml->createElement("ans:codigoProcedimento", $guia['codigo_procedimento_execucao']);
        $procedimento->appendChild($codigoProcedimento);

        $descricaoProcedimento = $this->xml->createElement("ans:descricaoProcedimento", $guia['descricao_execucao']);
        $procedimento->appendChild($descricaoProcedimento);

        return $procedimento;
    }

    public function isValido(DOMDocument $xml = null)
    {
        $xml = $xml ?: $this->xml;

        $temp = tmpfile();
        fwrite($temp, $xml->saveXML());
        $tempData = stream_get_meta_data($temp);

        $objDom = new DomDocument();
        $objDom->load($tempData['uri']);

        libxml_use_internal_errors(true);
        $versaoArquivo = str_replace('.', '_', self::VERSAO_TISS);
        if (!$objDom->schemaValidate(APPRAIZ . "www/tissXsd/tissV{$versaoArquivo}.xsd")) {
            $erros = libxml_get_errors();
            return $erros;
        }

        return true;
    }
}