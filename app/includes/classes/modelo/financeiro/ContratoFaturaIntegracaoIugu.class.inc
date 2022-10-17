<?php

class ContratoFaturaIntegracaoIugu extends AbstractIntegracaoIugu {

    /**
     * Método utlizado para implementar uma mudança na situação da fatura
     * @param $arrDadosFatura array Dados da Fatura alterada
     * @return null
     */
    public function processarMudancaDeSituacaoDaFatura($arrDadosFatura){
        $mContratoFatura = new ContratoFatura();
        $mContrato = new Contrato();
        $arrDadosContratoFatura = $mContratoFatura->getTodosDadosPorWhere(["cnf.fatura_id = {$arrDadosFatura['id']}"]);
        $mContrato->verificarSituacaoDePagamentoDoContrato($arrDadosContratoFatura['contrato_id']);
    }
}