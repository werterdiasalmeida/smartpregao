<?php
/**
 * @author Renan de Lima Barbosa <renandelima@gmail.com>
 * @author Renê de Lima Barbosa <renedelima@gmail.com>
 */

$_SESSION['desenvolvedores'] = array('malosso@gmail.com');


class ExceptionHandler
{

    /**
     * @var ErrorHandler
     */
    static $oHandler = null;

    /**
     * @ignore
     */
    private function __construct(){}

    /**
     * @param Error $oException
     * @return void
     */
    public function display( Error $oException )
    {
        $aTrace = $oException->getTrace();
        array_unshift( $aTrace, array(
            'function' => '__construct',
            'line' => $oException->getLine(),
            'file' => $oException->getFile(),
            'class' => get_class( $oException ),
            'object' => null,
            'type' => '::',
            'args' => array( $oException->getMessage(), $oException->getCode() ),
        ) );
        $oTrace = new BackTrace( $aTrace );
        $oTrace->levelTop();
        $sTrace = $oTrace->explain();
        ob_clean();
        $msgretorno = "<fieldset><legend>" . get_class( $oException ) . " - " . $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'] . " (".$_SERVER['SERVER_ADDR'].")</legend>
			<pre><b> " . $oException->getMessage() . "</b></pre>"
            . $sTrace . "</fieldset>";
        return $msgretorno;

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
        $msgLog = str_replace("<q>","<br>",$msgLog);
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
        ?><script>alert('Ocorreu uma falha inesperada e sua operação não foi executada.\nO problema foi enviado automaticamente aos administradores do sistema para análise.\n');history.back();</script><?
        exit();
    }

    /**
     * Manipula exceções disparadas durante a execução.
     *
     * @param Error $oException
     * @return void
     * @see set_exception_handler
     */
    public function handle( Error $oException )
    {
        global $db;

        if( ! isset($_SESSION['usucpf']) )
        {
            session_unset();
            $_SESSION['MSG_AVISO'] = 'Sua sessão expirou. Efetue login novamente.';
            header('Location: login.php');
            exit();
        }

        // cancela a transacao, caso ela exista;
        if (isset($_SESSION['transacao']))
        {
            $db->rollback();
            unset($_SESSION['transacao']);
        }

        $msgLog = $this->display( $oException );

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
         */		// gravando no arquivo de log

        // gravando no arquivo de log
        $this->grava($msgLog);
        // enviar um e-mail de aviso quando acontecer algum erro
        $this->enviaEmail($msgLog);
        //Mostra Mensagem de erro e encerra o programa
        if(!$db->testa_superuser()){
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
        if ( self::$oHandler === null )
        {
            self::$oHandler = new ExceptionHandler();
            $cException = array( self::$oHandler, 'handle' );
            set_exception_handler( $cException );
        }
    }

}
