<?php
class MunicipioConsorcioIndisponivel extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'O município informado já está vinculado a outro consórcio.';
    }
}