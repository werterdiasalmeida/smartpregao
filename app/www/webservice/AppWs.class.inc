<?php

/**
 * Classe responsável por guardar os métodos que serão acessados pelo webservice
 *
 */
class AppWs extends AbstractWs
{
    const PLAIN_VERIFICACAO = 'adm_app_1';
    const APP_ID_CLUBVIDA_CLIENTE = 1;

    private $md5Verificacao;

    public function __construct()
    {
        $_SESSION['sisid'] = 4;
        $this->md5Verificacao = md5(self::PLAIN_VERIFICACAO);
    }

    public static function getFuncoesPermitidasSemLogar(){
        return array(
            'cadastrarDispositivo',
            'notificarRecebimento',
            'notificarLeitura',
        );
    }

    private function verificarTokenAcesso($tokenVerificacao){
        if($tokenVerificacao != $this->md5Verificacao) {
            return array("codigo" => '1', "mensagem" => 'Token de verificação inválido', "dados" => array());
        }

        return true;
    }

    public function cadastrarDispositivo($dados)
    {
        $appid = isset($dados['app']) ? $dados['app'] : self::APP_ID_CLUBVIDA_CLIENTE;
        $verificacaoTokenAcesso = $this->verificarTokenAcesso($dados['verificacao']);
        if(is_array($verificacaoTokenAcesso)) {
            return $verificacaoTokenAcesso;
        }

        $mDispositivo = new Dispositivo();
        $mDispositivo->recuperarLinha('*', array("distoken = '{$dados['token']}'", "disstatus = 'A'", "appid = {$appid}"));

        if($mDispositivo->disid){
            return array("codigo" => '1', "mensagem" => 'O dispositivo já está cadastrado', "dados" => array());
        }

        $arDadosDispositivo = array(
            'distoken' => $dados['token'],
            'displataforma' => $dados['plataforma'],
            'disversaoplataforma' => $dados['versao_plataforma'],
            'disidentificador' => $dados['identificador'],
            'disipcadastro' => $this->recuperarIpRemoto(),
            'disstatus' => 'A',
            'disdtcadastro' => 'now()',
            'appid' => $appid
        );

        $mDispositivo->popularDadosObjeto($arDadosDispositivo)->salvar();
        if(empty($_SESSION['usucpforigem'])){
            $_SESSION['usucpforigem'] = $_SESSION['usucpf'];
        }
        $mDispositivo->commit();
        return $this->gerarRespostaSucesso(array());
    }

    public function notificarRecebimento($dados)
    {
        $arDadosNotificacaoRecebidaLida = array(
            'dindtrecebida'	=> 'now()',
            'diniprecebida'	=> $this->recuperarIpRemoto(),
        );

        return $this->gravarRecebimentoLeitura($dados, $arDadosNotificacaoRecebidaLida);
    }

    public function notificarLeitura($dados)
    {
        $arDadosNotificacaoRecebidaLida = array(
            'dindtlida'	=> 'now()',
            'diniplida'	=> $this->recuperarIpRemoto(),
        );

        return $this->gravarRecebimentoLeitura($dados, $arDadosNotificacaoRecebidaLida);
    }

    private function gravarRecebimentoLeitura($dados, array $arDadosNotificacaoRecebidaLida)
    {
        $verificacaoTokenAcesso = $this->verificarTokenAcesso($dados['verificacao']);
        if(is_array($verificacaoTokenAcesso)) {
            return $verificacaoTokenAcesso;
        }

        $mDispositivo = new Dispositivo();
        $mDispositivo->recuperarLinha('*', array("distoken = '{$dados['token']}'", "disstatus = 'A'"));

        $mNotificacaoEnviada = new NotificacaoAppEnviada($dados['notificacao']);
        if(!$mNotificacaoEnviada->naeid){
            return array("codigo" => '1', "mensagem" => 'Notificação não encontrada', "dados" => array());
        }

        $mDispositivoNotificacao = new DispositivoNotificacaoApp();
        $mDispositivoNotificacao->recuperarLinha('*', array("naeid = {$mNotificacaoEnviada->naeid}", "disid = {$mDispositivo->disid}"));

        $arDadosNotificacaoRecebidaLida['disid'] = $mDispositivo->disid;
        $arDadosNotificacaoRecebidaLida['naeid'] = $mNotificacaoEnviada->naeid;
        $mDispositivoNotificacao->popularDadosObjeto($arDadosNotificacaoRecebidaLida)->salvar();
        $mDispositivoNotificacao->commit();

        return array("codigo" => '0', "mensagem" => 'Operação realizada com sucesso', "dados" => array());
    }

    private function recuperarIpRemoto()
    {
        $ipaddress = 'UNKNOWN';

        if (getenv('HTTP_CLIENT_IP')) {
            $ipaddress = getenv('HTTP_CLIENT_IP');
        }else if(getenv('HTTP_X_FORWARDED_FOR')) {
            $ipaddress = getenv('HTTP_X_FORWARDED_FOR');
        }else if(getenv('HTTP_X_FORWARDED')) {
            $ipaddress = getenv('HTTP_X_FORWARDED');
        }else if(getenv('HTTP_FORWARDED_FOR')) {
            $ipaddress = getenv('HTTP_FORWARDED_FOR');
        }else if(getenv('HTTP_FORWARDED')) {
            $ipaddress = getenv('HTTP_FORWARDED');
        }else if(getenv('REMOTE_ADDR')) {
            $ipaddress = getenv('REMOTE_ADDR');
        }

        return $ipaddress;
    }
}