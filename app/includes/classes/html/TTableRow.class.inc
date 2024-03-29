<?php
/**
 * classe TTableRow
 * Repons�vel pela exibi��o de uma linha de uma tabela
 */
class TTableRow extends TElement
{
    /**
     * m�todo construtor
     * Instancia uma nova linha
     */
    public function __construct()
    {
        parent::__construct('tr');
    }

    /**
     * m�todo addCell
     * Agrega um novo objeto c�lula (TTableCell) � linha
     * @param  $value = conte�do da c�lula
     */
    public function addCell($value)
    {
        // instancia objeto c�lula
        $cell = new TTableCell($value);
        parent::add($cell);
        // retorna o objeto instanciado
        return $cell;
    }

    public function addCellHead($value)
    {
        // instancia objeto c�lula
        $cell = new TTableHead($value);
        parent::add($cell);
        // retorna o objeto instanciado
        return $cell;
    }

	public function addHead($value){
		return $this->addCellHead($value);
	}
}