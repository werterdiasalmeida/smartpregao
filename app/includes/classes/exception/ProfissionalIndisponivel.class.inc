<?php
class ProfissionalIndisponivel extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'O profissional j� possui agenda marcada nesse hor�rio.';
    }
}