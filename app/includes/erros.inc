<?
/*
  Sistema SIMEC
  Setor responsável: SPO/MEC
  Módulo: classes_simec.inc
  Finalidade: Trata os Erros do Sistema
  Data de criação: 02/08/2005
  Analista: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
  Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
  */
error_reporting(0);

$old_error_handler = set_error_handler("erros");

function erros($errno, $errmsg, $filename, $linenum, $vars) {
    /**
     * Determina quais codigos de erro serao reportados
     */
    switch($errno) {
        case E_NOTICE:
        case E_STRICT:
            return;
            break;
        default:
            $erro = new Erro($errno, $errmsg, $filename, $linenum, $vars);
            break;
    }
}

class Erro
{
    var $nErro;
    var $msgErro;
    var $nomeArquivo;
    var $numLinha;
    var $vars;

    function Erro($nErro,$msgErro,$nomeArquivo,$numLinha, $vars){
        $this->nErro       = $nErro;
        $this->msgErro     = $msgErro;
        $this->nomeArquivo = $nomeArquivo;
        $this->numLinha    = $numLinha;
        $this->vars        = $vars;
        $this->logErro();
    }

    function getNErro(){
        return $this->nErro;
    }

    function getMsgErro(){
        return $this->msgErro;
    }


    function getNomeArquivo(){
        return $this->nomeArquivo;
    }

    function getNumLinha(){
        return $this->numLinha;
    }

    function getVars(){
        return $this->vars;
    }

    function getDescricaoErro($nErro){
        // Define uma matriz associativa com as strings dos erros
        $desErro = array(
            1    => "Error",
            2    => "Warning",
            4    => "Parsing Error",
            8    => "Notice",
            16   => "Core Error",
            32   => "Core Warning",
            64   => "Compile Error",
            128  => "Compile Warning",
            256  => "User Error",
            512  => "User Warning",
            1024 => "User Notice"
        );
        return $desErro[$nErro];
    }

    function logErro()
    {
        if ($this->getNErro() != 8)
        {
            $mensagem = $_SESSION['ambiente'];
            $mensagem .= str_replace("\n","",$this->getMsgErro());
            $msgLog = "Data: ".date("d-m-Y H:i:s").".<q>Código do erro: ".$this->getNErro().".<q>".
                "Tipo de Erro: ".$this->getDescricaoErro($this->getNErro()).".<q>".
                "Arquivo: ".$this->getNomeArquivo().".<q>".
                "Linha: ".$this->getNumLinha().".<q><q>".
                "Ambiente: ".$_SESSION['ambiente'].".<q><q>".
                "Mensagem de ERRO: $mensagem.<q><q>";

            // cancela a transacao, caso ela exista;
            if (isset($_SESSION['transacao']))
            {
                pg_query ('rollback;');
                unset($_SESSION['transacao']);
            }
            // gravando no arquivo de log
            $this->grava($msgLog);
            // enviar um e-mail de aviso quando acontecer algum erro
            $this->enviaEmail($msgLog);
            //Mostra Mensagem de erro e encerra o programa
            $this->msgErro($msgLog);
        }
    }

    function grava($msgLog)
    {
        $msgLog = str_replace("<q>","\n",$msgLog);
        $msgLog = str_replace("'","''",$msgLog);
        $sql = "insert into auditoria (auddata, audsql,usucpf,audmsg,audip,mnucod, audtipo) values ('".date("d-m-Y H:i:s")."','".str_replace("'","''",$_SESSION['sql'])."','".$_SESSION['usucpf']."','".$msgLog."','".$_SESSION['ip']."','".$_SESSION['mnucod']."','X')";
        pg_query($sql);
    }

    function enviaEmail($msgLog)
    {
        $msgLog = str_replace("<q>","<br>",$msgLog);
        $paraquem = 'Administrador do Sistema';
        $paraonde = 'cristiano.cabral@mec.gov.br';
        $assunto = 'Erro no Simec '.date("d-m-Y H:i:s");
        email($paraquem,$paraonde,$assunto,$msgLog."SQL: ".$_SESSION['sql']);
    }

    function msgErro($msgLog)
    {
        $msgLog = str_replace("<q>",'\n',$msgLog);
        $msgLog = str_replace("'","\'",$msgLog);
        ?><script>alert('O sistema gerou um erro inesperado e sua operação não foi executada.\n Este erro foi enviado aos administradores do sistema para análise, se necessário entre em contato com o setor responsável.\n\n<?=$msgLog?>');history.back();</script><?
        // pg_close();
        exit();
    }
}
