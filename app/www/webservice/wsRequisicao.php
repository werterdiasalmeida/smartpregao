<?php
$requestParams = $_REQUEST;

define('APP_VERSOES_PERMITIDAS', serialize(array(
    '0.0.3',
    '0.0.4',
    '0.0.5',
    '0.0.6',
    '0.0.7',
    '0.0.8',
    '0.0.9',
    '0.0.10',
    '0.0.11',
    '0.0.12',
)));
include "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// obtém o tempo inicial da execução
$Tinicio = getmicrotime();

if(!isset($requestParams['t']) || empty($requestParams['t'])){
    $_SESSION['superuser'] = true;
}

define('WS_SISID', 73);

$db     = new cls_banco();
$aumid  = null;
set_error_handler("errorHandler", E_ALL & ~E_NOTICE & ~E_STRICT & ~E_DEPRECATED & ~E_WARNING); //Troca o erro padrão do simec
set_exception_handler( "exceptionHandler" ); // Troca a exception padrão do SIMEC

function __autoload($class_name) {
    global $requestParams;

    include_once APPRAIZ . "includes/funcoes.inc";
    include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

	$arCaminho = array(
						APPRAIZ . "includes/classes/modelo/public/",
						APPRAIZ . "includes/classes/modelo/territorios/",
						APPRAIZ . "includes/classes/modelo/entidade/",
						APPRAIZ . "includes/classes/modelo/seguranca/",
						APPRAIZ . "includes/classes/modelo/adm/",
						APPRAIZ . "includes/classes/modelo/app/",
						APPRAIZ . "includes/classes/modelo/corporativo/",
						APPRAIZ . "adm/classes/",
						APPRAIZ . "includes/classes/modelo/mobile/",
						APPRAIZ . "includes/classes/modelo/mobile/app/",
						APPRAIZ . "includes/classes/controller/",
						APPRAIZ . "includes/classes/view/",
						APPRAIZ . "includes/classes/html/",
						APPRAIZ . "includes/classes/",
						APPRAIZ . "www/webservice/",
					  );

	//incluindo arquivos pelo módulo informado
	if(isset($requestParams["modulo"])) {
	    $strModulo     = strtolower($requestParams["modulo"]);
	    $constantes    = APPRAIZ . "www/" . $strModulo . "/_constantes.php";
	    $funcoes       = APPRAIZ . "www/" . $strModulo . "/_funcoes.php";

	    if ( file_exists( $constantes ) ){
	        include_once( $constantes );
	    }

	    if ( file_exists( $funcoes ) ){
	        include_once( $funcoes );
	    }

	    include_once APPRAIZ . 'includes/workflow.php';

	    $arCaminho[] = APPRAIZ . "includes/classes/modelo/{$strModulo}/";
	    $arCaminho[] = APPRAIZ . "{$strModulo}/classe/controller/";
	    $arCaminho[] = APPRAIZ . "{$strModulo}/classe/modelo/";
	    $arCaminho[] = APPRAIZ . "{$strModulo}/classes/modelo/";

	}

	foreach($arCaminho as $caminho){
		$arquivo = $caminho . $class_name . '.class.inc';

		if ( file_exists( $arquivo ) ){
			require_once( $arquivo );
			break;
		}
	}
}

if (isset($_REQUEST["funcao"]) && $_REQUEST['funcao'] == 'download' && isset($_REQUEST['modulo']) && isset($_REQUEST['id'])) {
    verificarSegurancaToken($_REQUEST['t'], $_REQUEST, true);

    $file = new FilesSimec(null, null, $_REQUEST['modulo']);
    $file->getDownloadArquivo($_REQUEST['id']);
    die;
}else if (isset($_REQUEST["funcao"]) && $_REQUEST['funcao'] == 'upload' && isset($_REQUEST['modulo'])){
    verificarSegurancaToken($_REQUEST['t'], $_REQUEST, true);
    // Remove possíveis parâmetros que estejam após o nome do arquivo
    $_FILES['arquivo']['name'] = str_replace(strstr($_FILES['arquivo']['name'], '?'), "", $_FILES['arquivo']['name']);
    $file = new FilesSimec(null, null, $_REQUEST['modulo']);
    $idArquivo = $file->setUploadArquivoEspecifico('', 'arquivo');
    $db->commit();

    die(json_encode(utf8ArrayEncode(array(
        "codigo" => 0,
        "mensagem" => "Operação realizada com sucesso",
        "dados" => array('id' => $idArquivo)
    ))));
}else if ($_SERVER ["REQUEST_METHOD"] == "POST" && isset($requestParams["funcao"])) {
    header('Content-type: application/json');

    $retorno = false;

    // funções genéricas
    if($requestParams ["funcao"] && empty($requestParams["modulo"])){
        switch ($requestParams ["funcao"]) {
            case "logar" :
                logAuditoria($requestParams);
                $retorno = verificarSeguranca($requestParams['usuario'],$requestParams['senha'], $requestParams);

                if($retorno['codigo'] === 0){
                    gravarUsuarioDispositivo($requestParams);
                }
                break;
            case "logOff":
                logAuditoria($requestParams);
                logOff($requestParams);
                break;
            case "gravaToken":
                $retorno = gravaToken($requestParams);
                break;
        }
    }

    //funções especificas
    $modulo = ucfirst(strtolower($requestParams["modulo"]));

    $classe = $modulo . 'Ws';
    if(!empty($modulo) && class_exists($classe)){
        $obj = new $classe;

        //verifica se as credenciais enviadas são válidas
        if(!in_array($requestParams['funcao'], $obj->getFuncoesPermitidasSemLogar()) ){
            verificarSegurancaToken($requestParams['t'], $requestParams, true);
        }

        $menu = getMenu($requestParams["modulo"], $requestParams['funcao']);
        if(method_exists ( $obj , $requestParams ["funcao"] )){
            logAuditoria($requestParams);

            $dados = trataDados($requestParams['dados']);
            $dados = $dados ? json_decode($dados, true) : array();
            $dados = utf8ArrayDecode($dados);
            $dados['aumid'] = $aumid;

            $retorno = $obj->{$requestParams["funcao"]}($dados);
        }
    }

    echo ($retorno ? gerarJson($retorno) : funcaoInvalida());

    if($menu){
        gravarEstatistica(WS_SISID, $menu['mnuid']);
    }
    die;
}

function funcaoInvalida(){
    $arDados = array('codigo' => 1, 'mensagem' => "Função inválida");
    return gerarJson($arDados);
}

function appDesatualizado(){
    $arDados = array('codigo' => 1, 'mensagem' => "O Aplicativo está desatualizado, por favor faça a atualização.");
    return gerarJson($arDados);
}

function verificarSeguranca($usuario, $senha, $requestParams, $valida = false){
    verificarVersao($requestParams);

    $mUsuario = new Usuario();

    $usuario = pg_escape_string($usuario);
    $senha   = pg_escape_string($senha);

    $retorno = $mUsuario->loginWebService($usuario, $senha);

    if($retorno['dados']['foto']){
        $serverUrl = 'http' . (isset($_SERVER['HTTPS']) ? 's' : '') . "://{$_SERVER['HTTP_HOST']}";
        $avatar = array(
            '' => "{$serverUrl}/imagens/man.png",
            'M' => "{$serverUrl}/imagens/man.png",
            'F' => "{$serverUrl}/imagens/woman.png"
        );

        $retorno['dados']['foto'] = $retorno['dados']['foto']
            ? "{$serverUrl}/slideshow/slideshow/verimagem.php?arqid={$retorno['dados']['foto']}&_sisarquivo=adm&newwidth=150"
            : $avatar[$retorno['dados']['sexo']];
    }

    if($valida && $retorno['codigo'] === 1){
        die(gerarJson($retorno));
    }

    return $retorno;
}

function verificarSegurancaToken($token, $requestParams, $valida){
    $arToken = getParam($token);

    if(is_array($arToken) && count($arToken) > 0
    && isset($arToken['usuario']) && !empty($arToken['usuario'])
    && isset($arToken['senha']) && !empty($arToken['senha'])){
        return verificarSeguranca($arToken['usuario'], $arToken['senha'], $requestParams, $valida);
    }else{
        $arRetorno = array("codigo" => 1, "mensagem" => 'Token inválido.');
        if($valida){
            die(gerarJson($arRetorno));
        }

        return $arRetorno;
    }
}

function codificarUtf8($arDados){
    return utf8ArrayEncode($arDados);
}

function gerarJson($param, $gravarLog = true){
    $param = codificarUtf8($param);
    $retorno = json_encode($param);

    if($gravarLog){
        gravarRetornoLogAuditoria($retorno);
    }

    return $retorno;
}

function errorHandler($errno, $errstr, $errfile, $errline){
    global $db;
    $db->rollback();
    $db = new cls_banco();

    $arErro = array('codigo' => 1, 'mensagem' => "Não foi possível executar a operação", "numero" => $errno, "erro" => $errstr, "arquivo" => $errfile, "linha" => $errline);
    die(gerarJson($arErro));
}

function exceptionHandler($oException){
    global $db;
    $db->rollback();
    $db = new cls_banco();

    $arErro = array('codigo' => 1, 'mensagem' => "Não foi possível executar a operação", "numero" => $oException->getCode(), "erro" => $oException->getMessage(), "arquivo" => $oException->getFile(), "linha" => $oException->getLine());
    die(gerarJson($arErro));
}

function trataDados($dados){
    $dados = str_replace('\"','"', $dados);
    $dados = str_replace("\\'","'", $dados);
    return $dados;
}


function logAuditoria($arDadosPost){
	/*
    global $aumid;
    $arDadosPost['dados'] = trataDados($arDadosPost['dados']);

    $classe = $requestParams["modulo"] . 'Ws';
    if(!empty($requestParams["modulo"]) && class_exists($classe)) {
        new $classe; //Para setar o atributo do sistema na Session
    }

    $mAuditoriaMobile = new AuditoriaMobile();
    $aumid = $mAuditoriaMobile->gerarLog($arDadosPost);
    return $aumid;
	*/
}

function gravaToken($arDadosPost){
    $arDadosPost['dados'] = trataDados($arDadosPost['dados']);
    $dados = $arDadosPost['dados'] ? json_decode($arDadosPost['dados'], true) : array();

    //verificando se o token de verificação que chegou é igual ao que foi montado (Ano-mês-dia)
    $tk = MD5(date('Y-m-d'));
    if(!empty($dados['tkv']) && $dados['tkv'] == $tk) {
        $mTokenDispositivo = new TokenDispositivo();

        $arDadosGravar = array(
            'todid'         => $mTokenDispositivo->getIdByToken($dados["token"]),
            'todtoken'      => $dados["token"],
            'todversaoapp'  => $dados["versaoapp"],
            'toddscso'      => $dados["so"],
            'todversaoso'   => $dados["versaoso"],
            'todstatus'     => "A",
            'toddtcadastro' => "now()"
        );

        $todid = $mTokenDispositivo->gravar($arDadosGravar);

        if ($todid) {
            $arRetorno = array('codigo' => 0, 'mensagem' => "operação realizada com sucesso");
        } else {
            $arRetorno = array('codigo' => 1, 'mensagem' => "operação não realizada");
        }
    }else{
        $arRetorno = array('codigo' => 1, 'mensagem' => "Token de verificação inexistente ou inválido");
    }

    return $arRetorno;
}

function gravarRetornoLogAuditoria($retorno){
    global $aumid;

    if($aumid){
        $mAuditoriaMobile = new AuditoriaMobile();
        $mAuditoriaMobile->gravarRetornoLog($aumid, $retorno);
    }
}

function isAppAtualizado($arDadosPost){
    $appAtualizado = true;
    $arDadosPost['dados'] = trataDados($arDadosPost['dados']);
    $dados = $arDadosPost['dados'] ? json_decode($arDadosPost['dados'], true) : array();

    $classe = $dados['nomeapp'] . 'App';

    if(class_exists($classe)) {
        $objApp = new $classe; //Para setar o atributo do sistema na Session
        $appAtualizado = $objApp->isAppAtualizado($dados);
    }

    if(!$appAtualizado){
        die(appDesatualizado());
    }
}

function gravarUsuarioDispositivo($arDadosPost){
    $usuario = isset($_SESSION['usucpf']) ? $_SESSION['usucpf'] : $arDadosPost['usuario'];
    if(isset($arDadosPost['identificador'])){
        $mDispisitivoUsuario = new DispositivoUsuario();
        $mDispisitivoUsuario->vincular($usuario, $arDadosPost['identificador']);
        $mDispisitivoUsuario->commit();
    }
	/*
    $arDados = array();

    $dados = trataDados($arDadosPost['dados']);
    $dados = $dados ? json_decode($dados, true) : array();

    $arDados['usucpf'] = $arDadosPost['usuario'];
    $arDados['token']  = $dados['token'];
    $arDados['so']     = $dados['so'];

    $mUsuarioDispositvo = new UsuarioDispositivo();
    $mUsuarioDispositvo->gravar($arDados);
	*/
}

function logOff($arDados){
    $dados = trataDados($arDados['dados']);
    $dados = $dados ? json_decode($dados, true) : array();

    $arDados['usucpf'] = $arDados['usuario'];
    $arDados['token']  = $dados['token'];

    $mUsuario = new Usuario();
    $retorno  = $mUsuario->logOffWebService($arDados);

    $mUsuario->commit();
    die(gerarJson($retorno));
}

function verificarVersao($arParams){
    if(!isset($arParams['v']) || !in_array($arParams['v'], unserialize(APP_VERSOES_PERMITIDAS))){
        die(gerarJson(array(
            'codigo' => 2,
            'mensagem' => "O Aplicativo está desatualizado, por favor faça a atualização."
        )));
    }
}

function getMenu($modulo, $funcao) {
    global $db;
    $sisid = WS_SISID;
    $link = "{$modulo}::{$funcao}";

    $sql = "SELECT * FROM seguranca.menu WHERE sisid = {$sisid} AND mnulink = '{$link}' AND mnustatus = 'A'";
    return $db->pegaLinha($sql);
}