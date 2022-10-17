<?php

class SimecSMS
{
    /**
     * @var string
     */
    private $url = "";

    /**
     * @var string
     */
    private $error = "";

    /**
     * SimecSMS constructor.
     */
    public function __construct()
    {
        global $sms_url;
        $this->url = $sms_url;
    }

    /**
     * @return string
     */
    public function getUrl()
    {
        global $sms_url, $sms_usuario, $sms_senha;
        $this->url = $sms_url . "?lgn=" . $sms_usuario . "&pwd=" . $sms_senha;
        return $this->url;
    }

    /**
     * @return string
     */
    public function getError()
    {
        return $this->error;
    }

    /**
     * @param $url
     * @return mixed
     */
    private function enviaRequisicao($url){
        $curl = curl_init();

        curl_setopt_array($curl, array(
            CURLOPT_URL => $url,
            CURLOPT_RETURNTRANSFER => true
        ));

        $response = curl_exec($curl);
        $this->error = curl_error($curl);

        curl_close($curl);

        return $response;
    }

    /**
     * Consulta o saldo disponível de mensagens (a api só permite consultas de 1 em 1 minuto)
     * @return array
     */
    public function consultarSaldo()
    {
        $url = $this->getUrl();
        $url .= "&action=getbalance";

        $retorno = $this->enviaRequisicao($url);
        return json_decode($retorno,true);
    }

    /**
     * @param $codigo
     * @return array
     */
    public function consultarStatusMensagem($codigo)
    {
        $url = $this->getUrl();
        $url .= "&action=getstatus";
        $url .= "&id={$codigo}";

        $retorno = json_decode($this->enviaRequisicao($url),true);
        $retorno['data'] = utf8_decode($retorno['data']);

        return $retorno;
    }


    /**
     * Envia uma mensagem de SMS para os números informados.
     *
     * Pode ser enviado mais de um número separado por vírgula e sem espaços
     * em branco (não é permitido colocar o código do pais, ex: 553199999999)
     * Ex: 3199999999,3288888888
     *
     * @param $mensagem
     * @param $numeros
     * @param $callback
     * @return array
     * @throws Exception
     */
    public function enviarSMS($mensagem, $numeros, $categoriaMensagem, $callback = false)
    {
        //Trata os erros da mensagem
        $this->trataErrosMensagem($mensagem,$numeros);

        //Monta a url da requisição definindo os parâmetros
        $url = $this->getUrl();
        $url .= "&action=sendsms";
        $url .= "&msg=" . urlencode(utf8_encode($mensagem));
        $url .= "&numbers={$numeros}";
        if($callback === true){
            global $sms_url_callback;
            $url .= "&url_callback=" .  $sms_url_callback;
        }

        //Dispara a requisição e já trata o retorno pra de JSON para array
        $retorno = json_decode($this->enviaRequisicao($url),true);

        //Trata os erros da requisição
        $this->trataErrosRetorno($retorno);

        $dadosMensagem = [];
        if($categoriaMensagem) {
            //Loga a mensagem na tabela do schema de mensageria
            $dadosMensagem = $this->gravarMensagem($retorno,$mensagem, $numeros, $categoriaMensagem);
        }

        return $dadosMensagem;
    }

    /**
     * Grava o registro da mensagem no banco de dados
     * @param $retorno
     * @param $mensagem
     * @param $numeros
     * @return string
     */
    private function gravarMensagem($retorno,$mensagem, $numeros, $categoriaMensagem)
    {
        $arDados = [
            'tipo_mensagem_id'      => TipoMensagem::TIPO_MENSAGEM_SMS,
            'categoria_mensagem_id' => $categoriaMensagem,
            'mensagem'              => $mensagem,
            'destinatario'          => $numeros,
            'codigo'                => $retorno['data'],
            'excluido'              => 'false'
        ];

        $mMensagem = new Mensagem();
        $mMensagem->manter($arDados);
        return $mMensagem->getDados();
    }

    /**
     * @param $retorno
     * @return bool
     * @throws Exception
     */
    private function trataErrosRetorno($retorno)
    {
        if($this->getError() != ''){
            throw new Exception($this->error);
            $this->error = "";
        }

        if($retorno['status'] == 0){
            throw new Exception($retorno['msg']);
        }

        return true;
    }

    /**
     * @param string $mensagem
     * @param string $numeros
     * @return bool
     * @throws Exception
     */
    private function trataErrosMensagem($mensagem, $numeros)
    {
        if(strlen($mensagem) >= 160){
            throw new Exception('A mensagem não pode conter mais que 160 caracteres');
        }

        if(empty($numeros)){
            throw new Exception('Informe ao menos um número para enviar uma mensagem');
        }

        return true;
    }
}