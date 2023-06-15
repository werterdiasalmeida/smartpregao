<?php

// carrega as funções gerais
$pathConfig = "../../global/config.inc";
include_once(realpath($pathConfig) ? $pathConfig : "config.inc");
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "seguranca/www/_funcoes.php";
include_once APPRAIZ . "seguranca/www/_geracontratofatura.php";

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
$mCargaEstabelecimento = new CargaEstabelecimento();
$mCargaEditaisCaptacao = new CargaEditaisCaptacao();
$data_referencia =  date('Y-m-d');

/*
* Carga das empresas que estão participando dos editais
*/
$arrLog = '=== Carga das empresas que estão participando dos editais === \n';
$arrWhere[] = " situacao  =  'Cadastrado'";
$rs = $mCargaEditaisCaptacao->getListEditaisCaptacao($arrWhere);
$resultadoCarga = '';
foreach ($rs as  $result) {
    $params['prgcod'] = $result['codigo'];
    $resultadoCarga .= $mCargaEstabelecimento->baixarGravarEmpresasPregao($params);
}
if ($resultadoCarga) {
    $arrLog .= $resultadoCarga;
} else {
    $arrLog .= 'Sem dados para Baixar. \n';
}


/*
* Atualização dos dados cadastrais das empresas, na API-BUS
*/
$arrLog .= '=== Atualização dos dados cadastrais das empresas, na API-BUS === \n';
$arrWhere[] = " situacao_carga  =  'dados-basicos'";
$rs = $mCargaEstabelecimento->getListCNPJ($arrWhere);
$resultadoCarga = '';
foreach ($rs as $result) {
    $resultadoCarga .= $mCargaEstabelecimento->salvarDadosAPICNPJ($result['cnpj']);
}
if ($resultadoCarga) {
    $arrLog .= $resultadoCarga;
} else {
    $arrLog .= 'Sem dados para Baixar.';
}

die($arrLog);
