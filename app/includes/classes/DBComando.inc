<?php
final class DBComando extends cls_banco{
	
	private   $MV;
//	public $db;
	
	
	function __construct(){
		
		parent::__construct();
		
		$this->MV = new DBMontagemValidacao();
//		$this->db = new cls_banco();
	}

	public function __destruct(){
		
		if( isset($_SESSION['transacao'] ) ){
			pg_query( $this->link, 'rollback; ');
			unset( $_SESSION['transacao'] );
		}
		
	}	
	
	public function insert($tabela, $atributo, $atributoWhere = null){
		$campos = array();
		$values = array();
		
		if ($atributoWhere == null){
			foreach($atributo as $k => $val){
				if ($val['chave'] == 'PK'){
					$pk = $k;
					continue;
				}
//				if ($val['value'] != null){
					$atributo->{$k} = $this->MV->startMontVal($atributo->{$k});
					if (is_bool($atributo->{$k}))
						return false;
					$campos[] = $k;
					$values[] = ((empty($atributo->{$k}['value'])) ? 'NULL' : $atributo->{$k}['value']);
//				}
			}
			
			
			$sql = "INSERT INTO {$tabela} (" . implode(", ", $campos) . ")VALUES(
						" . implode(", ", $values) . "
					) RETURNING {$pk};";
			
			return $this->pegaUm($sql);			
		}else{
			$where = array();
			
			foreach($atributo as $k => $val){
				if ($val['chave'] == 'PK'){
					continue;
				}
//				if (!is_null($val['value'])){
					$atributo->{$k} = $this->MV->startMontVal($atributo->{$k});
					if (is_bool($atributo->{$k}))
						return false;
					$values[] = "$k = " . ((empty($atributo->{$k}['value'])) ? 'NULL' : $atributo->{$k}['value']);
//				}
			}
			foreach($atributoWhere as $k => $val){
//				if (!is_null($val['value'])){
					$atributoWhere->{$k} = $this->MV->startMontVal($atributoWhere->{$k});
					if (is_bool($atributoWhere->{$k}))
						return false;
					$where[] = "$k = " . ((empty($atributoWhere->{$k}['value'])) ? 'NULL' : $atributoWhere->{$k}['value']);
//				}
			}
			
			$sql = "UPDATE {$tabela} SET 
						" . implode(", ", $values) . " 
					WHERE 
						" . implode(" AND ", $where) . ";";
			$this->executar( $sql );
			return true;
		}
	}

	public function delete($tabela, $arrWhere){
//		$campos = array();
		$where  = array();
		
		foreach($arrWhere as $k => $v){
			$where[] = "$k = '$v'";
		}
		
		$sql = "DELETE FROM 
					{$tabela} 
				WHERE 
					" . implode(" AND ", $where) . ";";
				
		$this->executar( $sql );			
		return true;
		
	}
	
}
?>