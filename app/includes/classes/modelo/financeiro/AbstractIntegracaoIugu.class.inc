<?php

/**
 * Qualquer implementa��o adicional que precisa ser feita com a integra��o do Iugu deve ser extendida
 * dessa classe, lembrando que deve ser adiconando nas vari�veis da fatura do iugu o m�dulo e a classe de integra��o
 */
abstract class AbstractIntegracaoIugu {

    /**
     * M�todo utlizado para implementar uma mudan�a na situa��o da fatura
     * @param $arrDadosFatura mixed Dados da Fatura alterada
     * @return null
     */
    public function processarMudancaDeSituacaoDaFatura($arrDadosFatura){
        return null;
    }
}