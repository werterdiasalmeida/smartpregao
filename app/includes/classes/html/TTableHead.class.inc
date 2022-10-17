<?php
/**
 * classe TTableHead
 * Reponsável pela exibição de uma célula de uma tabela
 */
class TTableHead extends TElement
{
    /**
     * método construtor
     * Instancia uma nova célula
     * @param  $value = conteúdo da célula
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