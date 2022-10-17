<?php
// carrega as funções gerais
$pathConfig = "../../global/config.inc";
include_once(realpath($pathConfig) ? $pathConfig : "config.inc");
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "seguranca/www/_funcoes.php";

function __autoload($class_name)
{
    $arCaminho = array(
        APPRAIZ . "adm/classes/",
        APPRAIZ . "includes/classes/modelo/app/",
        APPRAIZ . "includes/classes/modelo/public/",
        APPRAIZ . "includes/classes/modelo/territorios/",
        APPRAIZ . "includes/classes/modelo/entidade/",
        APPRAIZ . "includes/classes/modelo/financeiro/",
        APPRAIZ . "includes/classes/modelo/corporativo/",
        APPRAIZ . "includes/classes/modelo/seguranca/",
        APPRAIZ . "includes/classes/modelo/mensageria/",
        APPRAIZ . "includes/classes/controller/",
        APPRAIZ . "includes/classes/",
        APPRAIZ . "includes/classes/exception/",
        APPRAIZ . "includes/classes/view/",
        APPRAIZ . "includes/classes/html/",
    );

    foreach ($arCaminho as $caminho) {
        $arquivo = $caminho . $class_name . '.class.inc';
        if (file_exists($arquivo)) {
            require_once($arquivo);
            break;
        }
    }
}

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

// Estabelecimento
$prestador = 2;
// Convenio
$operadora = 5;

$mEstabelecimento = new Estabelecimento();
$dadosPrestador = $mEstabelecimento->getTodosDados($prestador);

$mConvenioPlano = new ConvenioEstabelecimento();
$dadosOperadora = $mConvenioPlano->getTodosDados($operadora);

$sqlConsultas = "SELECT
                    est.registro_ans AS registro_ans,
                    999999 AS numero_guia_prestador, /* TODO */
                    cep.numero AS numero_carteira,
                    'N' AS atendimento_rn, 
                    paciente.nome_completo AS nome_beneficiario,
                    pje.cnpj AS cnpj_contratado,
                    pje.nome_fantasia AS nome_contratado,
                    est.cnes AS cnes,
                    p.nome_completo AS nome_profissional,
                    tpr.sigla_tipo_registro AS conselho_profissional,
                    pro.registro_profissional AS numero_conselho_profissional,
                    pro.uf_registro AS uf_conselho_profissional,
                    tpr.cbos AS cbos,
                    9 AS indicacao_acidente,
                    epa.inicio::date AS data_atendimento,
                    CASE WHEN epa.tipo_consulta_id = 5 THEN 1 ELSE 2 END AS tipo_consulta,
                    999999 AS codigo_tabela, /* TODO */
                    999999 AS codigo_procedimento, /* TODO */
                    vlr.valor AS valor_procedimento
                FROM
                    estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                LEFT JOIN
                    estabelecimentosaude.tb_valor_procedimento vlr ON epa.valor_procedimento_id = vlr.id
                LEFT JOIN
                    estabelecimentosaude.tb_tabela_procedimento_convenio tpc ON vlr.tabela_procedimento_convenio_id = tpc.id
                LEFT JOIN
                    estabelecimentosaude.tb_convenio_estabelecimento coe ON tpc.convenio_id = coe.id
                LEFT JOIN
                    estabelecimentosaude.tb_tipo_consulta tco ON epa.tipo_consulta_id = tco.id
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id
                INNER JOIN
                    profissionalsaude.tb_especialidade_profissional ep ON eep.especialidade_profissional_id = ep.id
                INNER JOIN
                    profissionalsaude.tb_profissional pro ON ep.profissional_id = pro.id
                INNER JOIN
                    corporativo.tb_pessoafisica p ON pro.pessoafisica_id = p.id
                LEFT JOIN
                    corporativo.tb_pessoafisica paciente ON epa.pessoa_paciente_id = paciente.id
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento est ON eep.estabelecimento_id = est.id
                INNER JOIN
                    corporativo.tb_pessoajuridica pje ON est.pessoajuridica_id = pje.id
                INNER JOIN
                    profissionalsaude.tb_tipo_profissional tpr ON tpr.id = pro.tipo_profissional_id
                LEFT JOIN
                    estabelecimentosaude.tb_convenio_estabelecimento_paciente cep ON coe.id = cep.convenio_estabelecimento_id 
                                                                                 AND cep.pessoa_paciente_id = epa.pessoa_paciente_id
                                                                                 AND cep.excluido IS FALSE
                WHERE
                    epa.excluido IS FALSE
                    AND
                    est.id = {$prestador}
                    AND
                    coe.id = {$operadora}
                LIMIT 
                  100";
$consultas = $db->carregarArray($sqlConsultas);

$desformataCNPJ = function ($cnpj) {
    return str_replace(['.', '-', '/'], '', $cnpj);
};

$dadosCabecalho = [
    'tipo_transacao' => 'ENVIO_LOTE_GUIAS',
    'sequencial_transacao' => '999999', /* TODO */
    'data_registro_transacao' => date('Y-m-d'),
    'hora_registro_transacao' => date('H:i:s'),
    'origem_cnpj' => $dadosPrestador['cnpj'],
    'destino_cnpj' => $dadosOperadora['cnpj'],
    'padrao' => '03.04.00'
];

$dadosPrestadorOperadora = [
    'numero_lote' => '999999', /* TODO */
    'guias' => $consultas
];

$hash = md5(serialize(array_merge($dadosCabecalho, $dadosPrestadorOperadora)));

//versao XML e codificação
$xml = new DOMDocument("1.0", "UTF-8");

//remove os espacos em branco
$xml->preserveWhiteSpace = false;

//Realizar a quebra dos blocos do XML por linha
$xml->formatOutput = true;

// Nó / Bloco Principal
// ans:mensagemTISS
$mensagemTISS = $xml->createElement("ans:mensagemTISS");
$xml->appendChild($mensagemTISS);

//Criação dos elementos do Namespace ans:mensagemTISS
$xml->createAttributeNS('http://www.w3.org/2000/09/xmldsig//', 'ds:attr');
$xml->createAttributeNS('http://www.w3.org/2001/XMLSchema-instance', 'xsi:attr');
$xml->createAttributeNS('http://www.ans.gov.br/padroes/tiss/schemas http://www.ans.gov.br/padroes/tiss/schemas/tissV3_04_00.xsd', 'schemaLocation:attr');
$xml->createAttributeNS('http://www.ans.gov.br/padroes/tiss/schemas', 'ans:attr');

/* primeiro bloco */
// ans:mensagemTISS / ans:cabecalho
$cabecalho = $xml->createElement("ans:cabecalho");
$mensagemTISS->appendChild($cabecalho);

// ans:mensagemTISS / ans:cabecalho / ans:identificacaoTransacao
$identificacaoTransacao = $xml->createElement("ans:identificacaoTransacao");
$cabecalho->appendChild($identificacaoTransacao);

// ans:tipoTransacao
$tipoTransacao = $xml->createElement("ans:tipoTransacao", $dadosCabecalho['tipo_transacao']);
$identificacaoTransacao->appendChild($tipoTransacao);

//sequencialTransacao
$sequencialTransacao = $xml->createElement("ans:sequencialTransacao", $dadosCabecalho['sequencial_transacao']);
$identificacaoTransacao->appendChild($sequencialTransacao);

//dataRegistroTransacao
$dataRegistroTransacao = $xml->createElement("ans:dataRegistroTransacao", $dadosCabecalho['data_registro_transacao']);
$identificacaoTransacao->appendChild($dataRegistroTransacao);

//horaRegistroTransacao
$horaRegistroTransacao = $xml->createElement("ans:horaRegistroTransacao", $dadosCabecalho['hora_registro_transacao']);
$identificacaoTransacao->appendChild($horaRegistroTransacao);

// ans:mensagemTISS / ans:cabecalho / ans:origem
$origem = $xml->createElement("ans:origem");
$cabecalho->appendChild($origem);

// ans:mensagemTISS / ans:cabecalho / ans:origem / ans:identificacaoPrestador
$origemIdentificacaoPrestador = $xml->createElement("ans:identificacaoPrestador");
$origem->appendChild($origemIdentificacaoPrestador);

// ans:mensagemTISS / ans:cabecalho / ans:origem / ans:identificacaoPrestador / ans:CNPJ
$origemCNPJ = $xml->createElement("ans:CNPJ", $desformataCNPJ($dadosCabecalho['origem_cnpj']));
$origemIdentificacaoPrestador->appendChild($origemCNPJ);


// ans:mensagemTISS / ans:cabecalho / ans:destino
$destino = $xml->createElement("ans:destino");
$cabecalho->appendChild($destino);

// ans:mensagemTISS / ans:cabecalho / ans:destino / ans:identificacaoPrestador
$destinoIdentificacaoPrestador = $xml->createElement("ans:identificacaoPrestador");
$destino->appendChild($destinoIdentificacaoPrestador);

// ans:mensagemTISS / ans:cabecalho / ans:destino / ans:identificacaoPrestador / ans:CNPJ
$destinoCNPJ = $xml->createElement("ans:CNPJ", $desformataCNPJ($dadosCabecalho['destino_cnpj']));
$destinoIdentificacaoPrestador->appendChild($destinoCNPJ);

// ans:mensagemTISS / ans:cabecalho / ans:Padrao
$padrao = $xml->createElement("ans:Padrao", $dadosCabecalho['padrao']);
$cabecalho->appendChild($padrao);

/* segundo bloco */
// ans:mensagemTISS / ans:prestadorParaOperadora
$prestadorParaOperadora = $xml->createElement("ans:prestadorParaOperadora");
$mensagemTISS->appendChild($prestadorParaOperadora);

// ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias
$loteGuias = $xml->createElement("ans:loteGuias");
$prestadorParaOperadora->appendChild($loteGuias);

// ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:numeroLote
$numeroLote = $xml->createElement("ans:numeroLote", $dadosPrestadorOperadora['numero_lote']);
$loteGuias->appendChild($numeroLote);

// ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS
$guiasTISS = $xml->createElement("ans:guiasTISS");
$loteGuias->appendChild($guiasTISS);

foreach ($dadosPrestadorOperadora['guias'] as $guia) {
    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta
    $guiaConsulta = $xml->createElement("ans:guiaConsulta");
    $guiasTISS->appendChild($guiaConsulta);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:cabecalhoConsulta
    $cabecalhoConsulta = $xml->createElement("ans:cabecalhoConsulta");
    $guiaConsulta->appendChild($cabecalhoConsulta);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:cabecalhoConsulta / ans:registroANS
    $registroANS = $xml->createElement("ans:registroANS", $guia['registro_ans']);
    $cabecalhoConsulta->appendChild($registroANS);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:cabecalhoConsulta / ans:numeroGuiaPrestador
    $numeroGuiaOperadora = $xml->createElement("ans:numeroGuiaOperadora", $guia['numero_guia_prestador']);
    $cabecalhoConsulta->appendChild($numeroGuiaOperadora);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:cabecalhoConsulta / ans:numeroGuiaPrestador
    $numeroGuiaPrestador = $xml->createElement("ans:numeroGuiaPrestador", $guia['numero_guia_prestador']);
    $cabecalhoConsulta->appendChild($numeroGuiaPrestador);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:dadosBeneficiario
    $dadosBeneficiario = $xml->createElement("ans:dadosBeneficiario");
    $guiaConsulta->appendChild($dadosBeneficiario);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:dadosBeneficiario / ans:numeroCarteira
    $numeroCarteira = $xml->createElement("ans:numeroCarteira", $guia['numero_carteira']);
    $dadosBeneficiario->appendChild($numeroCarteira);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:dadosBeneficiario / ans:atendimentoRN
    $atendimentoRN = $xml->createElement("ans:atendimentoRN", $guia['atendimento_rn']);
    $dadosBeneficiario->appendChild($atendimentoRN);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:dadosBeneficiario / ans:nomeBeneficiario
    $nomeBeneficiario = $xml->createElement("ans:nomeBeneficiario", $guia['nome_beneficiario']);
    $dadosBeneficiario->appendChild($nomeBeneficiario);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:contratadoExecutante
    $contratadoExecutante = $xml->createElement("ans:contratadoExecutante");
    $guiaConsulta->appendChild($contratadoExecutante);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:contratadoExecutante / ans:cnpjContratado
    $cnpjContratado = $xml->createElement("ans:cnpjContratado", $desformataCNPJ($guia['cnpj_contratado']));
    $contratadoExecutante->appendChild($cnpjContratado);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:contratadoExecutante / ans:nomeContratado
    $nomeContratado = $xml->createElement("ans:nomeContratado", $guia['nome_contratado']);
    $contratadoExecutante->appendChild($nomeContratado);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:contratadoExecutante / ans:CNES
    $cnes = $xml->createElement("ans:CNES", $guia['cnes']);
    $contratadoExecutante->appendChild($cnes);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:profissionalExecutante
    $profissionalExecutante = $xml->createElement("ans:profissionalExecutante");
    $guiaConsulta->appendChild($profissionalExecutante);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:profissionalExecutante / ans:nomeProfissional
    $nomeProfissional = $xml->createElement("ans:nomeProfissional", $guia['nome_profissional']);
    $profissionalExecutante->appendChild($nomeProfissional);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:profissionalExecutante / ans:conselhoProfissional
    $conselhoProfissional = $xml->createElement("ans:conselhoProfissional", $guia['conselho_profissional']);
    $profissionalExecutante->appendChild($conselhoProfissional);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:profissionalExecutante / ans:numeroConselhoProfissional
    $numeroConselhoProfissional = $xml->createElement("ans:numeroConselhoProfissional", $guia['numero_conselho_profissional']);
    $profissionalExecutante->appendChild($numeroConselhoProfissional);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:profissionalExecutante / ans:UF
    $uf = $xml->createElement("ans:UF", $guia['uf_conselho_profissional']);
    $profissionalExecutante->appendChild($uf);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:profissionalExecutante / ans:CBOS
    $cbos = $xml->createElement("ans:CBOS", $guia['cbos']);
    $profissionalExecutante->appendChild($cbos);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:indicacaoAcidente
    $indicacaoAcidente = $xml->createElement("ans:indicacaoAcidente", $guia['indicacao_acidente']);
    $guiaConsulta->appendChild($indicacaoAcidente);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:dadosAtendimento
    $dadosAtendimento = $xml->createElement("ans:dadosAtendimento");
    $guiaConsulta->appendChild($dadosAtendimento);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:dadosAtendimento / ans:dataAtendimento
    $dataAtendimento = $xml->createElement("ans:dataAtendimento", $guia['data_atendimento']);
    $dadosAtendimento->appendChild($dataAtendimento);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:dadosAtendimento / ans:tipoConsulta
    $tipoConsulta = $xml->createElement("ans:tipoConsulta", $guia['tipo_consulta']);
    $dadosAtendimento->appendChild($tipoConsulta);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:dadosAtendimento / ans:procedimento
    $procedimento = $xml->createElement("ans:procedimento");
    $dadosAtendimento->appendChild($procedimento);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:dadosAtendimento / ans:procedimento / ans:codigoTabela
    $codigoTabela = $xml->createElement("ans:codigoTabela", $guia['codigo_tabela']);
    $procedimento->appendChild($codigoTabela);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:dadosAtendimento / ans:procedimento / ans:codigoProcedimento
    $codigoProcedimento = $xml->createElement("ans:codigoProcedimento", $guia['codigo_procedimento']);
    $procedimento->appendChild($codigoProcedimento);

    // ans:mensagemTISS / ans:prestadorParaOperadora / ans:loteGuias / ans:guiasTISS / ans:guiaConsulta / ans:dadosAtendimento / ans:procedimento / ans:valorProcedimento
    $valorProcedimento = $xml->createElement("ans:valorProcedimento", $guia['valor_procedimento']);
    $procedimento->appendChild($valorProcedimento);
}


/* terceiro bloco */
// ans:mensagemTISS / ans:epilogo
$epilogo = $xml->createElement("ans:epilogo");
$mensagemTISS->appendChild($epilogo);

// ans:mensagemTISS / ans:epilogo / ans:hash
$hash = $xml->createElement("ans:hash", $hash);
$epilogo->appendChild($hash);

// $xml->save("xml_tiss_{$dadosPrestadorOperadora['numero_lote']}_{$hash}.xml");

header("Content-type: text/xml");
// Imprime / Gera o xml em tela
echo $xml->saveXML();