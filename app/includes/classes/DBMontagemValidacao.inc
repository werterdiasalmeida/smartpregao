<?php
final class DBMontagemValidacao {
	
	public function montType($atributo){
		if ( is_null($atributo['value']) || empty($atributo['value']) ){
			$atributo['value'] = ( ($atributo['type'] == 'integer' || $atributo['type'] == 'numeric' || $atributo['type'] == 'data') || ( $atributo['chave'] ) ) ? 'NULL' : "''";
			return $atributo;
		}
		
		if ($atributo['type'] == 'string' || $atributo['type'] == 'char'){
			$atributo['value'] = "'" . trim(pg_escape_string($atributo['value'])) . "'";
		}elseif ($atributo['type'] == 'integer'){
			$atributo['value'] = ($atributo['value'] ? intval($atributo['value']) : '');
		}elseif ($atributo['type'] == 'data'){
			$atributo['value'] = ($atributo['value'] ? "'" . trim($atributo['value']) . "'" : '');
		}
		
		return $atributo;
	}
	
	public function montMascara($atributo){
		if (is_null($atributo['value']) || empty($atributo['value'])){
			return $atributo;
		}
		
		if ($atributo['mascara'] == 'cpf' && strpos($atributo['value'], '.') > -1){
			$atributo['value'] = corrige_cpf($atributo['value']);
		}elseif ($atributo['mascara'] == 'data'){
			$atributo['value'] = formata_data_sql($atributo['value']);
		}
		
		return $atributo;
	}
	
	public function validacaoGeral($atributo){
		$return = true;
		if (!$this->verificaIsNull($atributo)){
			return false;
		}
		
		if (is_null($atributo['value']) || empty($atributo['value'])){
			return $atributo;
		}
				
		if ($atributo['mascara'] == 'cpf'){
			$return = validaCPF($atributo['value']);
		}
		
		return $return;
	}

	public function verificaIsNull($atributo){
		if ( (is_null($atributo['value']) || (empty($atributo['value']) && $atributo['type'] == 'integer')) && $atributo['nulo'] == false ){
			return false;
		}else{
			return true;
		}
	}
		
	public function startMontVal($obj){
		// Bloco de validaчуo
		if (!$this->validacaoGeral($obj)){
			return false;
		}
		// Bloco de tratamento de mascaras
		$obj = $this->montMascara($obj);
		
		// Bloco de preparaчуo por tipo
		$obj = $this->montType($obj);
		
		return $obj;
	}
	
}

?>