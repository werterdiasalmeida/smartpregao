<?php
class MunicipioConsorcioIndisponivel extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'O munic�pio informado j� est� vinculado a outro cons�rcio.';
    }
}