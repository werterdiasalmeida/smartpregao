<?php
class SolicitacaoCorrecaoExistente extends AbstractException
{
    protected function getDefaultErrorMessage()
    {
        return 'N�o foi poss�vel cadastrar o registro pois j� consta no sistema uma solicita��o de corre��o pendente.';
    }
}