<?php
class ValorProcedimentoInvalido extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'Valor do Procedimento Inválido.';
    }
}