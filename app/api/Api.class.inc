<?php
require_once APPRAIZ . 'includes/composer/vendor/autoload.php';
require_once APPRAIZ . 'api/ApiBusinessException.class.inc';
require_once APPRAIZ . 'api/AbstractApi.class.inc';

use flight\Engine;

class Api
{
    /**
     * @var int
     */
    private $sisid = 72;

    /**
     * @var flight\Engine
     */
    private $app;

    /**
     * @var flight\net\Route
     */
    private $currentRoute = null;

    /**
     * @var string
     */
    private $completeRoutePattern = null;

    /**
     * @var array
     */
    private $routes = [];

    /**
     * @var cls_banco
     */
    private $db = null;

    /**
     * @var array
     */
    private $usuario;

    private $clientId;

    private $menu;

    public function init()
    {
        $this->app = new Engine();

        $this->app->set('flight.views.path', APPRAIZ . 'api/view');
        $this->app->set('flight.views.extension', '.inc');

        $this->errorHandler();
        $this->simecStart();
        $this->notFoundHandler();
        $this->beforeRouteStart();
        $this->routesloader();

        $this->app->start();
    }

    private function simecStart()
    {
        global $servidor_bd_apoio, $porta_bd_apoio, $nome_bd_apoio, $usuario_db_apoio, $senha_bd_apoio, $dominio_padrao;

        define('DOMINIO_PADRAO', $dominio_padrao);

        require_once APPRAIZ . "includes/classes_simec.inc";
        require_once APPRAIZ . "includes/funcoes.inc";
        include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
        include_once APPRAIZ . 'includes/workflow.php';

        require_once APPRAIZ . 'includes/composer/vendor/autoload.php';
        spl_autoload_register('namespaceAutoloader');
        spl_autoload_register(function ($className) {
            $caminhos = [
                APPRAIZ . "adm/classes/",
                APPRAIZ . "clubvidacard/classes/",
                APPRAIZ . "includes/classes/modelo/corporativo/",
                APPRAIZ . "includes/classes/modelo/public/",
                APPRAIZ . "includes/classes/modelo/territorios/",
                APPRAIZ . "includes/classes/modelo/entidade/",
                APPRAIZ . "includes/classes/modelo/seguranca/",
                APPRAIZ . "includes/classes/modelo/app/",
                APPRAIZ . "tracker/classes/modelo/",
                APPRAIZ . "includes/classes/modelo/mobile/",
                APPRAIZ . "includes/classes/modelo/mobile/app/",
                APPRAIZ . "includes/classes/",
            ];

            foreach ($caminhos as $caminho) {
                $arquivo = $caminho . $className . '.class.inc';
                if (!file_exists($arquivo)) {
                    continue;
                }

                require_once $arquivo;
                break;
            }
        });
        spl_autoload_register(function ($class_name) {
            $class_name = str_replace('\\', '/', $class_name);

            $arCaminho = array(
                APPRAIZ . "includes/",
            );

            foreach($arCaminho as $caminho){
                $arquivo = $caminho . $class_name . '.php';
                if ( file_exists( $arquivo ) ){
                    require_once( $arquivo );
                    break;
                }
            }
        });

        global $db, $Tinicio;
        $Tinicio = getmicrotime();

        putenv("API=TRUE");

        $db = new cls_banco();
        $this->db = $db;

        register_shutdown_function(function () {
            if (!$this->menu) {
                return;
            }

            gravarEstatistica(
                $this->sisid,
                $this->menu['mnuid'],
                ($this->usuario ? $this->usuario['usucpf'] : null)
            );
        });
    }

    private function beforeRouteStart()
    {
        $this->app->before('start', function () {
            $this->app->router()->route($this->app->request());
            $this->currentRoute = $this->app->router()->current();

            if (!$this->currentRoute) {
                return;
            }

            $this->completeRoutePattern = implode($this->currentRoute->methods, '|')
                . ' ' . $this->currentRoute->pattern;

            if (!isset($this->routes[$this->completeRoutePattern])) {
                return;
            }

            $this->menu = getMenu($this->sisid, $this->completeRoutePattern);
            if ($this->menu) {
                $_SESSION['mnuid'] = $this->menu['mnuid'];
            }

            if (!isset($this->routes[$this->completeRoutePattern]['route']['auth'])
                || $this->routes[$this->completeRoutePattern]['route']['auth'] === true) {

//                $this->checkClient();
                $this->checkAuth();

                $_SESSION['usuario_id'] = $this->usuario['id'];
                $this->routes[$this->completeRoutePattern]['instance']->setUsuario($this->usuario);
//                $this->routes[$this->completeRoutePattern]['instance']->setClientId($this->clientId);
            }

            $this->routes[$this->completeRoutePattern]['instance']->autoloader();
            $this->routes[$this->completeRoutePattern]['instance']->setAllRoutes(array_keys($this->routes));
        });
    }

    private function routesloader()
    {
        $basePathApi = realpath(APPRAIZ . 'api/classes');
        $apiDirHandle = opendir($basePathApi);

        while ($nomeArquivo = readdir($apiDirHandle)) {
            if (!preg_match('/^(\w)+(Api\.class\.inc)+/', $nomeArquivo)) {
                continue;
            }

            include_once "{$basePathApi}/{$nomeArquivo}";
            preg_match('/^(\w)+/', $nomeArquivo, $classe);
            $classe = current($classe);

            $apiObj = new $classe($this->app);

            foreach ($apiObj->routes() as $route) {
                $this->routes[$route['route']] = [
                    'instance' => $apiObj,
                    'route' => $route
                ];

                $this->app->route($route['route'], [$apiObj, $route['callback']]);
            }
        }

        closedir($apiDirHandle);
    }

    private function checkAuth()
    {
        $headers = $this->headers();
        if (!isset($headers['authorization'])) {
            $this->responseJsonError('Não autorizado', 401, true);
        }

        $authorization = explode(' ', $headers['authorization']);
        $token = end($authorization);

        $mUsuario = new Usuario();

        $this->usuario = $mUsuario->byTokenApi($token);

        if (empty($this->usuario)) {
            $this->responseJsonError('Não autorizado', 401, true);
        }
    }

//    private function checkClient()
//    {
//        $versoes = include 'versions.inc';
//
//        $headers = $this->headers();
//
//        $version = isset($headers['accept-version']) ? $headers['accept-version'] : null;
//
//        if (!in_array($version, $versoes)) {
//            $this->responseJsonError('Versão não suportada', 410, true);
//        }
//
//        $clientes = include 'clients.inc';
//        $clientId = isset($headers['client-id']) ? $headers['client-id'] : '';
//
//        if (!in_array($clientId, $clientes)) {
//            $this->responseJsonError('Cliente inválido', 400, true);
//        }
//
//        $this->clientId = $clientId;
//    }

    private function notFoundHandler()
    {
        $this->app->map('notFound', function () {
            $this->responseJsonError('Recurso não encontrado', 404);
        });
    }

    private function errorHandler()
    {
        $exceptionHandler = function (Throwable $ex) {
            if($this->db) {
                $this->db->rollback();
            }

            if (!($ex instanceof ApiBusinessException)) {
                $this->emailErro($ex);
            }

            $this->responseJsonError($ex->getMessage(), 500);
        };

        $errorHandler = function ($errno, $errstr, $errfile, $errline) {
            throw new \Exception($errstr, $errno, null);
        };

        set_error_handler($errorHandler, E_ALL & E_WARNING & ~E_NOTICE & ~E_STRICT & ~E_DEPRECATED); //Troca o erro padrão do simec
        set_exception_handler($exceptionHandler); // Troca a exception padrão do SIMEC

        $this->app->map('error', $exceptionHandler);
    }

    private function responseJsonError($message, $code, $halt = false, $response = [])
    {
        $this->app->json(array_merge($response, [
            'message' => utf8_encode($message),
            'code' => $code
        ]), $code);

        if ($halt) {
            $this->app->halt($code);
        }
    }

    private function headers()
    {
        $headers = getallheaders();
        return array_change_key_case($headers, CASE_LOWER);
    }

    function emailErro(Throwable $oException)
    {
        global $erros_ativado;

        if ($erros_ativado) {
            include_once APPRAIZ . 'includes/backtrace/BackTrace.php';
            include_once APPRAIZ . 'includes/backtrace/BackTraceExplain.php';

            $aTrace = $oException->getTrace();
            array_unshift($aTrace, [
                'function' => '__construct',
                'line' => $oException->getLine(),
                'file' => $oException->getFile(),
                'class' => get_class($oException),
                'object' => null,
                'type' => '::',
                'args' => [$oException->getMessage(), $oException->getCode()],
            ]);
            $oTrace = new BackTrace($aTrace);
            $oTrace->levelTop();
            $sTrace = $oTrace->explain();
            ob_clean();
            $msgretorno = "<fieldset><legend>" .
                get_class($oException) . " - API (" . $_SERVER['SERVER_ADDR'] . ")</legend>
                <pre><b> " . $oException->getMessage() .
                "</b></pre>" .
                $sTrace .
                "</fieldset>";

            $this->enviarEmailErroSistema($msgretorno);
        }
    }

    private function enviarEmailErroSistema($msgLog)
    {
        $msgLog = str_replace(array("<span style=\"color: #007700\"></span>",
            "<span style=\"color: #0000BB\"></span>",
            "<span style=\"color: #DD0000\"></span>",
            "<span style=\"color: #FF8000\"></span>"),
            array("",
                "",
                "",
                ""),
            $msgLog);

        $msgLog = str_replace("<q>", "<br>", $msgLog);
        $remetente = array('nome' => 'Administrador do Sistema');
        $assunto = 'Erro no Simec ' . date("d-m-Y H:i:s") . " - " . $_SESSION['ambiente'];
        $arEmails = getEmailsErroSistema();

        enviar_email($remetente, null, $assunto, $msgLog, '', $arEmails);
    }
}
