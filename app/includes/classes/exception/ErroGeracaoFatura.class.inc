<?php
class ErroGeracaoFatura extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'Erro ao Gerar a Fatura.';
    }
}