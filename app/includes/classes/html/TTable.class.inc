<?php
/**
 * classe TTable
 * responsável pela exibição de tabelas
 */
class TTable extends TElement
{

	private $_thead;
	private $_tfoot;

    /**
     * método construtor
     * Instancia uma nova tabela
     */
    public function __construct($arrOptions = array())
    {
        parent::__construct('table');

        $arrOptionsDefault = array(
            'cellspacing' => "1",
            'cellpadding' => "3",
            'bgcolor' => "#f5f5f5",
            'align' => "center",
            'width' => "95%",
            'class' => "tabela",
        );

        $arrOptions = array_merge($arrOptionsDefault, $arrOptions);

        $this->cellspacing = $arrOptions['cellspacing'];
        $this->cellpadding = $arrOptions['cellpadding'];
        $this->bgcolor = $arrOptions['bgcolor'];
        $this->align = $arrOptions['align'];
        $this->width = $arrOptions['width'];
        $this->class = $arrOptions['class'];

    }

    /**
     * método addRow
     * Agrega um novo objeto linha (TTableRow) na tabela
     */
    public function addRow()
    {
        // instancia objeto linha
        $row = new TTableRow;
        // armazena no aray de linhas
        parent::add($row);
        return $row;
    }

    public function addHeadRow($addThead = true)
    {
    	if(!$this->_thead && $addThead){
    		$this->_thead = new TTableTHead("");
    		parent::add($this->_thead);
    	}

        // instancia objeto linha
        $row = new TTableRow;

        // armazena no aray de linhas
        if($this->_thead){
	        $this->_thead->add($row);
        }else{
	        parent::add($row);
        }
        return $row;
    }

    public function addFootRow()
    {
    	if(!$this->_tfoot){
    		$this->_tfoot = new TTableTFoot("");
    		parent::add($this->_tfoot);
    	}

        // instancia objeto linha
        $row = new TTableRow;

        // armazena no array de linhas
        if($this->_tfoot){
	        $this->_tfoot->add($row);
        }else{
	        parent::add($row);
        }
        return $row;
    }
}