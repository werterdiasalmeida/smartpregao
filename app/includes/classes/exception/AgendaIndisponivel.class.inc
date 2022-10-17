<?php
class AgendaIndisponivel extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'A agenda informado não está disponível.';
    }
}