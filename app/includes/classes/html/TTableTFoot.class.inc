<?php
/**
 * classe TTableTFoot
 * Reponsсvel pela exibiчуo de uma cщlula de uma tabela
 */
class TTableTFoot extends TElement
{
    /**
     * mщtodo construtor
     * Instancia uma nova cщlula
     * @param  $value = conteњdo da cщlula
     */
    public function __construct($value)
    {
        parent::__construct('tfoot');
        parent::add($value);
    }

}