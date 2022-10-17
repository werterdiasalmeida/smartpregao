<?php
class ProfissionalIndisponivel extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'O profissional já possui agenda marcada nesse horário.';
    }
}