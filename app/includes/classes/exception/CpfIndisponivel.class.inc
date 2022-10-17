<?php
class CpfIndisponivel extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'O CPF informado não está disponível.';
    }
}