<?php
/**
 * classe TTableHead
 * Repons�vel pela exibi��o de uma c�lula de uma tabela
 */
class TTableHead extends TElement
{
    /**
     * m�todo construtor
     * Instancia uma nova c�lula
     * @param  $value = conte�do da c�lula
     */
    public function __construct($value)
    {
        parent::__construct('th');
        parent::add($value);
    }

    function setAlign($align){
    	if (strlen($align) == 1){
	        $this->class = ( strtoupper($align) == 'L' ? "SubTituloEsquerda" : "SubTituloDireita");
    	}else{
	        $this->align = $align;
    	}
    }
}