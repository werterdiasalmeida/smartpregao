<?php
class AgendaIndisponivel extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'A agenda informado n�o est� dispon�vel.';
    }
}