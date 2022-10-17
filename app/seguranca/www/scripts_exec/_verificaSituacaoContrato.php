<?php

// carrega as funções gerais
$pathConfig = "../../global/config.inc";
include_once(realpath($pathConfig) ? $pathConfig : "config.inc");
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "seguranca/www/_funcoes.php";
include_once APPRAIZ . "seguranca/www/_verificaSituacaoContrato.php";

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

// CPF do administrador de sistemas
if (!$_SESSION['usucpf']) {
    $_SESSION['superuser'] = true;
}

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$mContrato = new Contrato();
$situacaoCancelada = Contrato::SITUACAO_CONTRATO_CANCELADO;
$situacaoInativo = Contrato::SITUACAO_CONTRATO_INATIVO;
$arrWhere = [
    "con.situacao <> '{$situacaoInativo}'",
    "con.situacao <> '{$situacaoCancelada}'",
];
$arrListContrato = $mContrato->getLista($arrWhere);

$html = "Atualizando a situação dos contratos.<br>";
foreach ($arrListContrato as $arrContrato) {
    $html .= "Verificando a situação de pagamento do contrato {$arrContrato['numero']}</br>";
    $mContrato->verificarSituacaoDePagamentoDoContrato($arrContrato['id']);
    $mContrato->commit();
}
$html .= "Fim da Execução.";

die($html);
