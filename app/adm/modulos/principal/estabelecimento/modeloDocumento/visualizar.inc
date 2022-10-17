<?php
$p = getParam();
$mEstabelecimento = new Estabelecimento();
$estabelecimento = $mEstabelecimento->getTodosDados($p['id']);

$mTipo = new TipoProntuario($_REQUEST['tipo_prontuario_id']);

$mEndereco = new EnderecoPessoaJuridica();
$enderecos = $mEndereco->listarPorPessoaJuridica($estabelecimento['pessoajuridica_id']);

$mContato = new ContatoPessoaJuridica();
$contatos = $mContato->listarPorPessoaJuridica($estabelecimento['pessoajuridica_id']);

$paciente = ['nome_completo' => $_SESSION['usunome'], 'cpf_paciente' => $_SESSION['usucpf']];
$endereco = array_shift($enderecos);
$contato = array_shift($contatos);
$tipo = $mTipo->getDados();
$profissional = ['nome_completo' => $_SESSION['usunome']];

$variaveis = [
    'NOME_PACIENTE' => $paciente['nome_completo'],
    'CPF_PACIENTE' => formatar_cpf($paciente['cpf_paciente']),
    'NOME_MEDICO' => $profissional['nome_completo'],
    'PROCEDIMENTO' => $tipo['descricao']
];

$mModeloDocumento = new ModeloDocumento();
$variaveis = array_merge($mModeloDocumento->getValoresDeVariavelPadrao(), $variaveis);
$descricao = $mModeloDocumento->substituirVariaveis($_REQUEST['descricao'], $variaveis);

if($endereco){
    $profissional['registro_profissional'] = rand(10000000, 99999999);
    $profissional['uf_registro'] = $endereco['uf_sigla'];
}

include_once APPRAIZ . "adm/classes/ProntuarioImpressao.class.inc";

$mProntuarioImpressao = new ProntuarioImpressaoWkHtml($descricao, $estabelecimento, $paciente, $tipo);
$mProntuarioImpressao->setProfissional($profissional);
$mProntuarioImpressao->setEndereco($endereco);
$mProntuarioImpressao->setContato($contato);

$mProntuarioImpressao->generate();