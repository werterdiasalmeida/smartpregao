<?php
class CnpjIndisponivel extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'O CNPJ informado não está disponível.';
    }
}