<?php
/**
 * classe TTableRow
 * Reponsável pela exibição de uma linha de uma tabela
 */
class TTableRow extends TElement
{
    /**
     * método construtor
     * Instancia uma nova linha
     */
    public function __construct()
    {
        parent::__construct('tr');
    }

    /**
     * método addCell
     * Agrega um novo objeto célula (TTableCell) à linha
     * @param  $value = conteúdo da célula
     */
    public function addCell($value)
    {
        // instancia objeto célula
        $cell = new TTableCell($value);
        parent::add($cell);
        // retorna o objeto instanciado
        return $cell;
    }

    public function addCellHead($value)
    {
        // instancia objeto célula
        $cell = new TTableHead($value);
        parent::add($cell);
        // retorna o objeto instanciado
        return $cell;
    }

	public function addHead($value){
		return $this->addCellHead($value);
	}
}