<?php
class CpfIndisponivel extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'O CPF informado n�o est� dispon�vel.';
    }
}