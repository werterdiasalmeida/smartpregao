<?php
$mContrato = new Contrato();
$mContratoFatura = new ContratoFatura();
$mFatura = new Fatura();
$params = getParam();
$contratoId = isset($params['contratoId']) ? $params['contratoId'] : null;

if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'gerarFatura' :
            try {
                $dadosFatura = $mContratoFatura->fecharFaturaDeContratoPorValorFixo($params);
                $db->commit();
                $db->sucesso('financeiro/fatura/lista', setParam(array('contratoId' => $contratoId)), "Fatura Gerada com sucesso");
            } catch (ErroGeracaoFatura $exception) {
                $msgErro = 'Não foi possível gerar a Fatura';
                $db->insucesso($msgErro, setParam(array('contratoId' => $contratoId)), 'financeiro/fatura/lista');
            }
        case 'enviarFaturaPorEmail' :
            $sucessoEnvio = $mFatura->enviarFaturaPorEmail(array('id' => $params['faturaId']));
            if($sucessoEnvio) {
                $db->sucesso('financeiro/fatura/lista', setParam(array('contratoId' => $contratoId)), "Email enviado com sucesso");
            } else {
                $db->insucesso('Não foi possível enviar o Email', setParam(array('contratoId' => $contratoId)), 'financeiro/fatura/lista');
            }
        case 'gerarSegundaViaFatura' :
            $sucessoEnvio = $mContratoFatura->gerarSegundaViaDeFatura($params['id']);
            if($sucessoEnvio) {
                $db->commit();
                $db->sucesso('financeiro/fatura/lista', setParam(array('contratoId' => $contratoId)), "Segunda via gerada com sucesso");
            } else {
                $db->insucesso('Não foi possível gerar a fatura', setParam(array('contratoId' => $contratoId)), 'financeiro/fatura/lista');
            }
        case 'download' :
            $file = new FilesSimec(null, null, 'seguranca');
            $params = getParam();
            $file->getDownloadArquivo($params['comprovante_pagamento']);
            die;
            break;
    }
}


$arWhere = array(
    "contrato_id = {$contratoId}"
);

include APPRAIZ . "includes/cabecalho.inc";
$codFuncFormularioFatura = getMnuidPorMnulink("seguranca.php?modulo=financeiro/fatura/formulario&acao=C");
$db->cria_aba($abacod_tela, $url, setParam(array('contratoId' => $contratoId)), array($codFuncFormularioFatura));
monta_titulo($titulo_modulo, '');

$mContrato->exibirCabecalhoContrato($contratoId);

$arConfig = array(
    "style" => "width:100%;",
    "totalLinha" => false,
    "totalRegistro" => true
);

$arCabecalho = array(
    "Ação",
    "Descrição",
    "Valor",
    "Data de Vencimento",
    "Situação"
);

$arParamCol = array();

$situacaoFaturaPendente = Fatura::SITUACAO_FATURA_PENDENTE;
$situacaoFaturaGerada = Fatura::SITUACAO_FATURA_GERADA;
$situacaoFaturaVencida = Fatura::SITUACAO_FATURA_VENCIDA;
$situacaoFaturaPaga = Fatura::SITUACAO_FATURA_PAGA;
$mSimecIugu = new SimecIugu();
$url = $mSimecIugu->getUrl();

$linkEditar = <<<LEDITAR
<a href="?modulo=financeiro/fatura/formulario&acao=C&p=\'' . setParam(array('id' => '{id}', 'contratoId' => '{$contratoId}'), false) . '\'" data-toggle="tooltip" title="Editar pagamento">
    <img src="/imagens/editar_nome.gif">
</a>
LEDITAR;

$linkConsulta = <<<LVIEWFATURA
<a target="_blank" href="{$url}{secure_id_iugu}" data-toggle="tooltip" title="Vizualisar Fatura">
    <img src="/imagens/consultar.gif">
</a>
LVIEWFATURA;


$acao = <<<ACAO
            <div style="min-width: 5px;">
                <php>
                    switch ('{situacao}') {
                        case '{$situacaoFaturaPaga}':
                            if('{comprovante_pagamento}' == '') {
                                return '$strVisualizarFatura $linkConsulta';
                            } else {
                                return '$linkEditar <a href="javascript:downloadComprovante(\'' . setParam(array('comprovante_pagamento' => '{comprovante_pagamento}', 'contratoId' => '{$contratoId}'), false) . '\');" title="Download comprovante de pagamento">
                                <img src="/imagens/download.png">
                            </a>'; 
                            }
                            return '$linkEditar'; 
                        case '{$situacaoFaturaPendente}':
                            return '<a href="javascript:gerarFatura(\'' . setParam(array('id' => '{id}', 'contratoId' => '{$contratoId}'), false) . '\');" title="Gerar boleto">
                                <img src="/imagens/money.gif">
                            </a> $linkEditar'; 
                        case '{$situacaoFaturaGerada}':
                            return '<a href="javascript:enviarFaturaPorEmail(\'' . setParam(array('faturaId' => '{fatura_id}', 'contratoId' => '{$contratoId}'), false) . '\');" data-toggle="tooltip" title="Enviar por Email">
                                <img src="/imagens/email.gif">
                            </a> $linkEditar $linkConsulta';
                        case '{$situacaoFaturaVencida}':
                            return '<a href="javascript:gerarSegundaViaFatura(\'' . setParam(array('id' => '{id}', 'faturaId' => '{fatura_id}', 'contratoId' => '{$contratoId}'), false) . '\');" title="Gerar segunda via de fatura">
                                <img src="/imagens/money.gif">
                            </a>'; 
                    }
                </php>
            </div>
ACAO;

$rs = $mContratoFatura->getLista($arWhere);

$arParamCol[2]["style"] = "text-align: right;";
$arParamCol[3]["style"] = "text-align: center;";
$arParamCol[2]["html"] = "<php>return  formata_data('{data_vencimento}');</php>";
$arParamCol[3]["html"] = "<php>return  Fatura::getArraySituacaoFatura()['{situacao}']['descricao'];</php>";

$oLista = new Lista($arConfig);
$oLista->setCabecalho($arCabecalho);
$oLista->setCorpo($rs, $arParamCol);
$oLista->setAcao($acao);
$oLista->show();
?>

<form id="form-fatura" method="post">
    <input type="hidden" id="act" name="act"/>
    <input type="hidden" id="p" name="p"/>
</form>

<link rel="stylesheet" href="../includes/bootstrap/3.0.0/css/bootstrap.min-simec.css">

<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1.min.js"></script>
<script type="text/javascript" src="../includes/bootstrap/3.0.0/js/bootstrap.min.js"></script>
<script type="text/javascript">
    function gerarFatura(pFatura) {
        $("#form-fatura #act").val('gerarFatura');
        $("#form-fatura #p").val(pFatura);
        $('#form-fatura').submit();
    }

    function enviarFaturaPorEmail(pFatura) {
        $("#form-fatura #act").val('enviarFaturaPorEmail');
        $("#form-fatura #p").val(pFatura);
        $('#form-fatura').submit();
    }

    function gerarSegundaViaFatura(pFatura) {
        $("#form-fatura #act").val('gerarSegundaViaFatura');
        $("#form-fatura #p").val(pFatura);
        $('#form-fatura').submit();
    }

    function downloadComprovante(id) {
        window.location = "?modulo=financeiro/fatura/lista&acao=C&act=download&p=" + id;
    }
</script>