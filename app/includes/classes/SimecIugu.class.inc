<?php

class SimecIugu
{
    /**
     * Array que guarda os erros
     * @var array
     */
    private $erros  = array();

    private $url = "https://faturas.iugu.com/";

    const SITUACAO_FATURA_PENDENTE = 'pending';

    const SITUACAO_FATURA_PAGA = 'paid';

    const SITUACAO_FATURA_EXPIRADA = 'expired';

    public function __construct()
    {
        require_once(APPRAIZ . "includes/iugu/lib/Iugu.php");
        $this->carregarConfiguracoes();
    }


    public function criarFatura($arDados)
    {
        $retorno = Iugu_Invoice::create($arDados);

        if(isset($retorno->errors ) && is_array($retorno->errors)){
            $this->setErros($retorno->errors);
            return false;
        }else{
            $this->setErros(array());
            return $retorno;
        }
    }

    public function listarFaturas($arDados)
    {
        $retorno = Iugu_Invoice::search($arDados);

        if(isset($retorno->errors ) && is_array($retorno->errors)){
            $this->setErros($retorno->errors);
            return false;
        }else{
            $this->setErros(array());
            return $retorno;
        }
    }

    public function getFaturaById($id)
    {
        $retorno = Iugu_Invoice::fetch($id);

        if(isset($retorno->errors) && is_array($retorno->errors)){
            $this->setErros($retorno->errors);
            return false;
        }else{
            $this->setErros(array());
            return $retorno;
        }
    }

    public function enviarFaturaPorEmail($id)
    {
        $arrDataUrl = array(
            'id' => $id,
            'action' => 'send_email'
        );
        $url = Iugu_Invoice::url($arrDataUrl);

        try {
            Iugu_Invoice::API()->request('POST', $url);
        } catch (Exception $exception) {
            return false;
        }

        return true;
    }

    public function gerarSegundaViaFatura($idIugu, $params = array())
    {
        $mIuguInvoice = new Iugu_Invoice();
        $mIuguInvoice->offsetSet('id', $idIugu);
        $arrData = [
            'current_fines_option' => true,
            'due_date' => $params['data_vencimento']
        ];

        return $mIuguInvoice->duplicate($arrData);
    }

    /**
     * @return array
     */
    public function getErros()
    {
        $erros = [];

        foreach ($this->erros as $campoErro => $erro) {
            $erros[] = " {$campoErro}: " . implode("; ", utf8ArrayDecode($erro)). ".";
        }

        return $erros;
    }

    /**
     * @param array $erros
     */
    private function setErros($erros)
    {
        $this->erros = $erros;
    }

    private function carregarConfiguracoes()
    {
        global $iugu_api_key;

        Iugu::setApiKey($iugu_api_key);
    }

    public function getUrl()
    {
        return $this->url;
    }

    public function cancelarFatura($idIugu)
    {
        $mIuguInvoice = new Iugu_Invoice();
        $mIuguInvoice->offsetSet('id', $idIugu);

        return $mIuguInvoice->cancel();
    }
}