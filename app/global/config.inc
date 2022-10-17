<?php
error_reporting (E_ERROR | E_PARSE);

// Ajusta os par?metros para evitar injections
if (!get_magic_quotes_gpc()) {
    $process = array(&$_GET, &$_POST, &$_COOKIE, &$_REQUEST);
    while (list($key, $val) = each($process)) {
        foreach ($val as $k => $v) {
            unset($process[$key][$k]);
            if (is_array($v)) {
                $process[$key][addslashes($k)] = $v;
                $process[] = &$process[$key][addslashes($k)];
            } else {
                $process[$key][addslashes($k)] = addslashes($v);
            }
        }
    }
    unset($process);
}

function fechar_conexoes()
{
    /*while(pg_ping())
    {
        pg_close();
    }*/
}

register_shutdown_function( 'fechar_conexoes' );

global $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd, $email_sistema;

// Configura??o
// Configura??o
$configFile = "config.ini";
$env = getenv("APP_ENV");
if ($env && file_exists(__DIR__ . DIRECTORY_SEPARATOR . "config-{$env}.ini")) {
    $configFile = "config-{$env}.ini";
}

$ini_array      = parse_ini_file($configFile, true);

// Geral
define(APPRAIZ, $ini_array['geral']['APPRAIZ']);

// DB
$servidor_bd        = $ini_array['db']['servidor_bd'];
$porta_bd           = $ini_array['db']['porta_bd'];
$nome_bd            = $ini_array['db']['nome_bd'];
$usuario_db         = $ini_array['db']['usuario_db'];
$senha_bd           = $ini_array['db']['senha_bd'];
$email_sistema      = $ini_array['email']['email_from'];

// DB APOIO
$servidor_bd_apoio  = $ini_array['db']['servidor_bd_apoio'];
$porta_bd_apoio     = $ini_array['db']['porta_bd_apoio'];
$nome_bd_apoio      = $ini_array['db']['nome_bd_apoio'];
$usuario_db_apoio   = $ini_array['db']['usuario_db_apoio'];
$senha_bd_apoio     = $ini_array['db']['senha_bd_apoio'];

// APP
$ionic_profile     = $ini_array['app']['ionic_profile'];
$ionic_authtoken   = $ini_array['app']['ionic_authtoken'];
$ionic_push_url    = $ini_array['app']['ionic_push_url'];
$android_authtoken = $ini_array['app']['android_authtoken'];
$android_push_url  = $ini_array['app']['android_push_url'];
$ios_passphrase    = $ini_array['app']['ios_passphrase'];
$ios_certificado   = $ini_array['app']['ios_certificado'];
$ios_push_url      = $ini_array['app']['ios_push_url'];

//IUGU
$iugu_api_key       = $ini_array['iugu']['iugu_api_key'];

//SMS
$sms_url            = $ini_array['sms']['url'];
$sms_url_callback   = $ini_array['sms']['url_callback'];
$sms_usuario        = $ini_array['sms']['usuario'];
$sms_senha          = $ini_array['sms']['senha'];

// EMAIL
$email_from         = $ini_array['email']['email_from'];
$email_host         = $ini_array['email']['email_host'];
$email_mailer       = $ini_array['email']['email_mailer'];
$email_auth         = $ini_array['email']['email_auth'];
$email_login        = $ini_array['email']['email_login'];
$email_pass         = $ini_array['email']['email_pass'];
$email_port         = $ini_array['email']['email_port'];
$email_secure       = $ini_array['email']['email_secure'];
$email_debug        = $ini_array['email']['email_debug'];
$email_api          = $ini_array['email']['email_api'];
$email_api_url      = $ini_array['email']['email_api_url'];
$email_api_token    = $ini_array['email']['email_api_token'];

// URL CERTIFICADO DIGITAL
$url_servidor_assinatura = $ini_array['servidor_assinatura']['url'];

// Usu?rios que n?o s?o bloqueados por n?mero de tentativas no login
$cpfs_nao_bloqueia_tentativas = $ini_array['usuarios']["cpf_nao_bloqueia_tentativas"];

// url teleconsulta
$teleconsulta_url = $ini_array['teleconsulta']['url'];
$teleconsulta_token = $ini_array['teleconsulta']['token'];

$whatsapp_template = $ini_array['whatsapp']['template'];

$telegram_template = $ini_array['telegram']['template'];

define( 'AUTHSSD', false );

/**
 * Tempo m?ximo que o usu?rio pode ficar conectado ao sistema sem que sua
 * sess?o expire. Essa funcionalidade ? controlada no arquivo estouvivo.php,
 * no diret?rio www.
 */
define( 'MAXONLINETIME', 3600);


session_start();

//Controle de erros do sistema
 if ( $ini_array['erros']['ativado'] && ($argv[1] != "--executado_agendamento_simec") )
	include_once( 'config.dev.php' );

//Emula outro usu?rio
if ( $_POST['usucpf_simu'] && ( $_SESSION['superuser'] || $_SESSION['usuuma'] ) )
{
    $_SESSION['usucpf'] = $_POST['usucpf_simu'];
}

date_default_timezone_set('America/Sao_Paulo');

$_SESSION['ambiente'] = $ini_array['sistema']['ambiente'];
$email_sistema = '';

/**
 * Solu??o paleativa para o problema de navega??o entre sistemas. Esta
 * rotina tenta adivinhar qual m?dulo o usu?rio teve a inten??o de acessar.
 * A decis?o ? tomada a partir da url solicitada pelo usu?rio no qual ele
 * indica o diret?rio e a a??o pretendida.
 */
preg_match( '/\/([a-zA-Z]*)\//', $_SERVER['REQUEST_URI'], $sisdiretorio );
$sisdiretorio = $sisdiretorio[1];

preg_match( '/\/([a-zA-Z]*)\.php/', $_SERVER['REQUEST_URI'], $sisarquivo );
$sisarquivo = $sisarquivo[1];

define( 'SISRAIZ', APPRAIZ . $_SESSION['sisdiretorio'] . '/' );


global $parametros_sistema_tela;

$parametros_sistema_tela = array(
    'nome_completo'=> $ini_array['sistema']['nome_completo'],
    'sigla'=> $ini_array['sistema']['sigla'],
    'unidade'=> $ini_array['sistema']['unidade'],
    'unidade_pai'=>$ini_array['sistema']['unidade_pai'],
    'email'=>$ini_array['email']['email_from'],
);

//$parametros_sistema_tela['sigla-nome_completo'] = $parametros_sistema_tela['sigla'].' - '.$parametros_sistema_tela['nome_completo'];
$parametros_sistema_tela['sigla-nome_completo'] = $parametros_sistema_tela['nome_completo'];
$parametros_sistema_tela['nome_e_orgao'] = $parametros_sistema_tela['sigla-nome_completo'].' / '.$parametros_sistema_tela['unidade'].' / '.$parametros_sistema_tela['unidade_pai'];
$parametros_sistema_tela['orgao_e_pai'] = $parametros_sistema_tela['unidade'].' / '.$parametros_sistema_tela['unidade_pai'];