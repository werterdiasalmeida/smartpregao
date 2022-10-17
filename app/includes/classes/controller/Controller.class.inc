<?php
class Controller{
	protected $dado;
	
	function __get($att){
		if ( method_exists($this, 'get' . ucfirst($att)) ){
			return $this->{'get' . ucfirst($att)}();
		}
		return $this->dado[$att];
	}
	
	function __set($att, $val){
		return $this->dado[$att] = $val;
	}
	
	protected function carregaDadosModel( Modelo $obModel, Array $arAtributos = null ){
		foreach( $obModel->getDados() as $att => $val){
			if ( is_null($arAtributos) || empty($arAtributos) ){
				$this->$att = $val;
			}elseif ( in_array($att, $arAtributos) ){
				$this->$att = $val;
			}
		}
	}
	
	protected function carregaDados( $dados, Array $arAtributos = null ){
		$dados = (array) $dados;
		foreach($dados as $att => $val):
			if ( is_null($arAtributos) || empty($arAtributos) ){
				$this->$att = $val;
			}elseif ( in_array($att, $arAtributos) ){
				$this->$att = $val;
			}
		endforeach;		
	}
}
?>