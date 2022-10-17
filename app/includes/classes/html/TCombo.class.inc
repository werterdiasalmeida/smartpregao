<?php
class TCombo extends TField{

	function __construct($name){
		parent::__construct($name);
		self::setSize('');
		self::setOpcao('-- '.("Selecione").' --');
		self::setTamanho(5);
	}

	function setOpcao($opcao){
		$this->prop['opcao'] = $opcao;
	}

	function getOpcao(){
		return $this->prop['opcao'];
	}

	function setItens($item){
		$this->prop['itens'][] = $item;
	}

	function getItens(){
		return $this->prop['itens'];
	}

	function setMultiplo($multiplo){
		$multiplo = $multiplo ? true : false;
		$this->prop['multiplo']= $multiplo;
	}

	function getMultiplo(){
		return $this->prop['multiplo'];
	}

	function setTamanho($tamanho){
		$this->prop['tamanho'] = $tamanho;
	}

	function getTamanho(){
		return $this->prop['tamanho'];
	}


	function show(){
		if ($this->multiplo == true){
			global ${$this->name};
			${$this->name} = (array) $this->value;
			$this->db->monta_combo_multiplo($this->name, $this->itens, $this->habil, $this->opcao, $this->acao.';', '', '', $this->tamanho, $this->size);
		}else{
			$this->db->monta_combo($this->name, $this->itens, $this->habil, $this->opcao, $this->acao.';', '', '', $this->size, $this->obrig, $this->id, '', (is_array($this->value) ? current($this->value) : $this->value) );
		}
	}
}
?>