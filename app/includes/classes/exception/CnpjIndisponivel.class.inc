<?php
class CnpjIndisponivel extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'O CNPJ informado n�o est� dispon�vel.';
    }
}