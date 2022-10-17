<?php

include_once "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/SimecIugu.class.inc";

$fatura = null;
$arVariaveis = array();

$_SESSION['superuser'] = true;

if ($_REQUEST['data']['id']) {
    $mSimecIugu = new SimecIugu();
    $fatura = $mSimecIugu->getFaturaById($_REQUEST['data']['id']);

    foreach ($fatura->custom_variables AS $variavel) {
        $arVariaveis[$variavel->name] = $variavel->value;
    }
}

function __autoload($class_name)
{
    global $arVariaveis;

    $arCaminho = array(
        APPRAIZ . "includes/classes/modelo/public/",
        APPRAIZ . "includes/classes/modelo/app/",
        APPRAIZ . "includes/classes/modelo/territorios/",
        APPRAIZ . "includes/classes/modelo/entidade/",
        APPRAIZ . "includes/classes/modelo/corporativo/",
        APPRAIZ . "includes/classes/modelo/seguranca/",
        APPRAIZ . "includes/classes/modelo/financeiro/",
        APPRAIZ . "includes/classes/",
    );


    //incluindo arquivos pelo módulo informado
    if (isset($arVariaveis["modulo"]) && isset($arVariaveis['classe_integracao'])) {
        $strModulo = strtolower($arVariaveis["modulo"]);
        $constantes = APPRAIZ . "www/" . $strModulo . "/_constantes.php";
        $funcoes = APPRAIZ . "www/" . $strModulo . "/_funcoes.php";

        if (file_exists($constantes)) {
            include_once($constantes);
        }

        if (file_exists($funcoes)) {
            include_once($funcoes);
        }

        include_once APPRAIZ . 'includes/workflow.php';

        $arCaminho[] = APPRAIZ . "includes/classes/modelo/{$strModulo}/";
        $arCaminho[] = APPRAIZ . "{$strModulo}/classe/controller/";
        $arCaminho[] = APPRAIZ . "{$strModulo}/classe/modelo/";
        $arCaminho[] = APPRAIZ . "{$strModulo}/classes/modelo/";

    }

    foreach ($arCaminho as $caminho) {
        $arquivo = $caminho . $class_name . '.class.inc';
        if (file_exists($arquivo)) {
            require_once($arquivo);
            break;
        }
    }
}

if ($fatura && trim($_REQUEST['data']['status']) == $fatura->status) {
    $mFatura = new Fatura();
    $faturaId = $mFatura->alterarSituacaoFaturaComSituacaoIugu($arVariaveis, $fatura->status);
    $arrDadosFatura = $mFatura->carregarPorId($faturaId)->getDados();

    if (isset($arVariaveis["modulo"]) && isset($arVariaveis['classe_integracao'])) {
        $classeIntegracao = snakeCaseParaCamelCase($arVariaveis['classe_integracao']);
        if(class_exists($classeIntegracao)) {
            /* @var $mClasseIntegracao AbstractIntegracaoIugu */
            $mClasseIntegracao = new $classeIntegracao;
            $mClasseIntegracao->processarMudancaDeSituacaoDaFatura($arrDadosFatura);
        }
    }

    $mFatura->commit();
}