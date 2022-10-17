<?php

/**
 * Created by PhpStorm.
 * User: Valtemir Souza
 * Date: 19/08/2015
 * Time: 17:36
 */
abstract class AbstractWs {
    public  static function getFuncoesPermitidasSemLogar(){
        return array();
    }

    protected function gerarRespostaSucesso($arResposta = array()){
        return array(
            "codigo" => 0,
            "mensagem" => "Operação realizada com sucesso",
            "dados" => $arResposta
        );
    }

    protected function gerarRespostaErro($arResposta = array()){
        return array(
            "codigo" => 1,
            "mensagem" => "Não foi possível executar a operação",
            "dados" => $arResposta
        );
    }

    protected function _getServerUrl($returnRequestUri = true)
    {
        return 'http' . (isset($_SERVER['HTTPS']) ? 's' : '') . "://{$_SERVER['HTTP_HOST']}" . ($returnRequestUri ? "{$_SERVER['REQUEST_URI']})" : '');
    }
}