<?php
class SalaIndisponivel extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'Sala indisponível.';
    }
}