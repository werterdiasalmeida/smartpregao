<?php
/**
 * @author Renan de Lima Barbosa <renandelima@gmail.com>
 * @author Renê de Lima Barbosa <renedelima@gmail.com>
 */

$_SESSION['desenvolvedores'] = array('malosso@gmail.com');


class ErrorHandler
{

    /**
     * @var array
     */
    static $aErrorType = array(
        E_ERROR => 'Error',
        E_WARNING => 'Warning',
        E_PARSE => 'Parsing Error',
        E_NOTICE => 'Notice',
        E_CORE_ERROR => 'Core Error',
        E_CORE_WARNING => 'Core Warning',
        E_COMPILE_ERROR => 'Compile Error',
        E_COMPILE_WARNING => 'Compile Warning',
        E_USER_ERROR => 'User Error',
        E_USER_WARNING => 'User Warning',
        E_USER_NOTICE => 'User Notice',
        E_STRICT => 'Runtime Notice'
    );

    /**
     * @var ErrorHandler
     */
    static $oHandler = null;

    /**
     * @ignore
     */
    private function __construct()
    {
    }

    /**
     * @param integer $iCode
     * @param string $sMessage
     * @param string $sFile
     * @param integer $iLine
     * @param array $aContext
     * @return void
     */
    public function display($iCode, $sMessage, $sFile, $iLine, array $aContext = array())
    {
        $oTrace = new BackTrace();
        $oTrace->levelDown(2);
        $sTrace = $oTrace->explain();
        ob_clean();

        ob_start();
        print_r($_REQUEST);
        $v_requ = ob_get_contents();
        ob_clean();

        ob_start();
        print_r($_SESSION);
        $v_session = ob_get_contents();
        ob_clean();

        ob_start();
        print_r($_SERVER);
        $v_server = ob_get_contents();
        ob_clean();


        $msg_erro = "
	   <fieldset>
			   <legend>" . self::$aErrorType[$iCode] . " - " . $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'] . "</legend>
			   <pre><b>" . $sMessage . "</b></pre>" . $sTrace . "
	   </fieldset>
	   <fieldset>
			   <legend><b>Variaveis de _REQUEST</b></legend>
			   <pre>" . $v_requ . "</pre>
	   </fieldset>
	   <fieldset>
			   <legend><b>Variaveis de _SESSION</b></legend>
			   <pre>" . $v_session . "</pre>
	   </fieldset>
	   <fieldset>
			   <legend><b>Variaveis de _SERVER</b></legend>
			   <pre>" . $v_server . "</pre>
	   </fieldset>        ";


        return $msg_erro;
    }


    /**
     * Grava Log de erro
     *
     * @param Text $msgLog
     * @return void
     */
    function grava($msgLog)
    {
        $conerr = pg_connect("host=".$GLOBALS["servidor_bd"]." port=".$GLOBALS["porta_bd"]." dbname=".$GLOBALS['nome_bd']."  user=".$GLOBALS["usuario_db"] ." password=".$GLOBALS["senha_bd"] ."");

        $auddata = date("Y-m-d H:i:s");
        $audsql = str_replace("'","\'",$_SESSION['sql']);
        $usucpf = $_SESSION['usucpf'];
        $audmsg = str_replace(["<q>", "'"],["\n", "\'"],$msgLog);
        $audip = $_SESSION['ip'];
        $mnuid = $_SESSION['mnuid'];

        $sql = "insert into seguranca.auditoria 
                  (auddata, audsql,usucpf,audmsg,audip,mnuid, audtipo) 
                values 
                  ('{$auddata}','{$audsql}','{$usucpf}','{$audmsg}','{$audip}','{$mnuid}','X')";

        pg_query($conerr, $sql);
        // pg_close($conerr);
    }

    /**
     * Envia email de erro para administrador
     *
     * @param Text $msgLog
     * @return void
     */
    function enviaEmail($msgLog)
    {
        $msgLog = str_replace("<q>", "<br>", $msgLog);
        $remetente = array('nome' => 'Administrador do Sistema');
        $assunto = "Erro no {$GLOBALS['parametros_sistema_tela']['sigla-nome_completo']} " . date("d-m-Y H:i:s") . " - " . $_SERVER['SERVER_NAME'];
        $arEmails = getEmailsErroSistema();

        if($arEmails){
            enviar_email($remetente, $arEmails, $assunto, $msgLog);
        }
    }


    /**
     * Mostra mensagem para o usuário
     *
     * @param Text $msgLog
     * @return void
     */
    function msgErro($msgLog)
    {
        ?>
        <script>alert('Ocorreu uma falha inesperada e sua operação não foi executada.\nO problema foi enviado automaticamente aos administradores do sistema para análise,\nse necessário entre em contato com o setor responsável.\n');
            history.back();</script><?
        exit();
    }


    /**
     * Manipula erros disparados durante a execução.
     *
     * Captura todos os erros disparados durante a execução.
     *
     * @param integer $iCode
     * @param string $sMessage
     * @param string $sFile
     * @param integer $iLine
     * @param array $aContext
     * @return void
     * @see set_error_handler
     */
    public function handle($iCode, $sMessage, $sFile, $iLine, array $aContext = array())
    {
        global $db;

        if (!isset($_SESSION['usucpf'])) {
            session_unset();
            $_SESSION['MSG_AVISO'] = 'Sua sessão expirou. Efetue login novamente.';
            header('Location: login.php');
            exit();
        }

        // cancela a transacao, caso ela exista;
        if (isset($_SESSION['transacao'])) {
            $db->rollback();
            unset($_SESSION['transacao']);
        }


        $msgLog = $this->display($iCode, $sMessage, $sFile, $iLine, $aContext);
        /*
         * Trecho que limpa o código HTML com o número excessivo de <span>
         */
        $msgLog = str_replace(array("<span style=\"color: #007700\"></span>",
            "<span style=\"color: #0000BB\"></span>",
            "<span style=\"color: #DD0000\"></span>",
            "<span style=\"color: #FF8000\"></span>"),
            array("",
                "",
                "",
                ""),
            $msgLog);
        /*
         * FIM -  Trecho que limpa o código HTML com o número excessivo de <span>
         */        // gravando no arquivo de log
        $this->grava($msgLog);
        // enviar um e-mail de aviso quando acontecer algum erro
        $this->enviaEmail($msgLog);

        //Mostra Mensagem de erro e encerra o programa
        if (!$db->testa_superuser()) {
            $this->msgErro($msgLog);
        }
        print $msgLog;
        exit();
    }

    /**
     * @return void
     */
    public static function start()
    {
        if (self::$oHandler === null) {
            self::$oHandler = new ErrorHandler();
            $cError = array(self::$oHandler, 'handle');
            set_error_handler($cError, E_ALL & ~E_NOTICE & ~E_STRICT & ~E_DEPRECATED);
        }
    }

}
