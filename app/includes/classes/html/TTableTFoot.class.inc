<?php
/**
 * classe TTableTFoot
 * Repons�vel pela exibi��o de uma c�lula de uma tabela
 */
class TTableTFoot extends TElement
{
    /**
     * m�todo construtor
     * Instancia uma nova c�lula
     * @param  $value = conte�do da c�lula
     */
    public function __construct($value)
    {
        parent::__construct('tfoot');
        parent::add($value);
    }

}