<?php

/**
 * Qualquer implementaзгo adicional que precisa ser feita com a integraзгo do Iugu deve ser extendida
 * dessa classe, lembrando que deve ser adiconando nas variбveis da fatura do iugu o mуdulo e a classe de integraзгo
 */
abstract class AbstractIntegracaoIugu {

    /**
     * Mйtodo utlizado para implementar uma mudanзa na situaзгo da fatura
     * @param $arrDadosFatura mixed Dados da Fatura alterada
     * @return null
     */
    public function processarMudancaDeSituacaoDaFatura($arrDadosFatura){
        return null;
    }
}