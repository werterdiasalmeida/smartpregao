<?php
function __autoload($nameClass) {
//	$arPath[] = APPRAIZ . "includes/classes/modelo/agenda/";
//	$arPath[] = APPRAIZ . "includes/classes/modelo/entidade/";
//	$arPath[] = APPRAIZ . "includes/classes/modelo/obras/";
//	$arPath[] = APPRAIZ . "includes/classes/modelo/public/";
//	$arPath[] = APPRAIZ . "includes/classes/modelo/questionario/";
//	$arPath[] = APPRAIZ . "includes/classes/modelo/seguranca/";
//	$arPath[] = APPRAIZ . "includes/classes/modelo/spm/";
//	$arPath[] = APPRAIZ . "includes/classes/modelo/territorios/";
	$arPath[] = APPRAIZ . "includes/classes/view/";
	$arPath[] = APPRAIZ . "includes/classes/html/";
	$arPath[] = APPRAIZ . "includes/classes/entidade/";
	$arPath[] = APPRAIZ . "includes/classes/";
	
	$loop = true; 
	do{	
		$arquivo = current($arPath) . $nameClass . '.class.inc';
		if (is_file($arquivo)){
    		require_once $arquivo;
    		$loop = false;
    		break;
		}
	}while ($loop && next($arPath));
}

//Carrega parametros iniciais
include_once "controleInicio.inc";
// carrega as funções específicas do módulo
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_funcoesseriehistorica.php';
include_once '_funcoesagendamentoindicador.php';
include_once '_funcoessolicitacoes.php';
include_once '_componentes.php';

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>