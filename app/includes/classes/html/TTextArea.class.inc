<?php
class TTextArea extends TField{

	function __construct($name){
		parent::__construct($name);
		parent::setSize(70);
		self::setRows(6);
		self::setMaxlength(2000);
	}

	function setRows($rows){
		$this->prop['rows'] = $rows;
	}

	function getRows(){
		return $this->prop['rows'];
	}

	function setMaxlength($max){
		$max = (int) $max;
		$this->prop['maxlength'] = $max;
	}

	function getMaxlength(){
		return $this->prop['maxlength'];
	}


	function show(){
		echo campo_textarea($this->name,
						   $this->obrig,
						   $this->habil,
						   '',
						   $this->size,
						   $this->rows,
						   $this->maxlength,
						   '',
						   '',
						   '',
						   '',
		                   '',
						   $this->value);
	}
}
?>