<?php
ob_start();

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';
//Carrega parametros iniciais
include_once "controleInicio.inc";

function __autoload($class_name) {
    $arCaminho = array(
        APPRAIZ . "adm/classes/",
        APPRAIZ . "clubvidacard/classes/",
        APPRAIZ . "includes/classes/modelo/public/",
        APPRAIZ . "includes/classes/modelo/app/",
        APPRAIZ . "includes/classes/modelo/territorios/",
        APPRAIZ . "includes/classes/modelo/entidade/",
        APPRAIZ . "includes/classes/modelo/corporativo/",
        APPRAIZ . "includes/classes/modelo/seguranca/",
        APPRAIZ . "includes/classes/modelo/mensageria/",
        APPRAIZ . "includes/classes/modelo/financeiro/",
        APPRAIZ . "includes/classes/controller/",
        APPRAIZ . "includes/classes/",
        APPRAIZ . "includes/classes/exception/",
        APPRAIZ . "includes/classes/view/",
        APPRAIZ . "includes/classes/html/",
    );

    foreach($arCaminho as $caminho){
        $arquivo = $caminho . $class_name . '.class.inc';
        if ( file_exists( $arquivo ) ){
            require_once( $arquivo );
            break;
        }
    }
}

include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

/*
 * O contador de tempo online na tela deve ser atualizado toda vez que o
 * usuário carregar uma tela do sistema. Ele é utilizado pelo "estou vivo"
 * de acordo com a constante MAXONLINETIME, definido no config.inc.
 */
include APPRAIZ . "includes/registraracesso.php";

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
