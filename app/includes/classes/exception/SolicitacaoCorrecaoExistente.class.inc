<?php
class SolicitacaoCorrecaoExistente extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'Nгo foi possнvel cadastrar o registro pois jб consta no sistema uma solicitaзгo de correзгo pendente.';
    }
}