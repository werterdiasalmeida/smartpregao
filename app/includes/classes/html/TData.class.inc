<?php
class TData extends TField{

	function __construct($name){
		parent::__construct($name);

        $this->setMascaraData("dd/mm/yyyy");
//		parent::setSize(50);
	}

//	function setMaxlength($max = 200){
//		$max = (int) $max;
//		$this->prop['maxlength'] = $max;
//	}
//
//	function getMaxlength(){
//		return $this->prop['maxlength'];
//	}
//
//	function setMascara($mascara){
//		$this->prop['mascara'] = $mascara;
//	}
//
//	function getMascara(){
//		return $this->prop['mascara'];
//	}

	function setMascaraData($mascaraData){
		$this->mascaraData = $mascaraData;
	}


	function show(){
		echo campo_data2($this->name, $this->obrig, $this->habil, $this->label, '', '', '', $this->value, '', '', '', $this->mascaraData);
	}
}