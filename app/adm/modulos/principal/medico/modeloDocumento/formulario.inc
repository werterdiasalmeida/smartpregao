<?php
$abaMedico = true;
$params = getParam();

if(empty($params['id'])) {
    Estabelecimento::setEstabelecimentoDoMedicoNaSession();
    $params['id'] = $_SESSION['estabelecimento_selecionado'];
    $_REQUEST['p'] = setParam($params, false);
}


include_once APPRAIZ . 'adm/modulos/principal/estabelecimento/modeloDocumento/formulario.inc';