<?php
	
class Sistema extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.sistema";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sisid' => null, 
									  	'sisdsc' => null, 
									  	'sisurl' => null, 
									  	'sisabrev' => null, 
									  	'sisdiretorio' => null, 
									  	'sisfinalidade' => null, 
									  	'sisrelacionado' => null, 
									  	'sispublico' => null, 
									  	'sisstatus' => null, 
									  	'sisexercicio' => null, 
									  	'sismostra' => null, 
									  	'sisemail' => null, 
									  	'paginainicial' => null, 
									  	'sisarquivo' => null, 
									  	'sistel' => null, 
									  	'sisfax' => null, 
									  );
									  
	public function listaObjeto( Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $k => $item){
			if ( $k === 'sisid' ){
				$codicao[] = "sisid = " . $item;
				continue;	
			}	
		}
		$codicao[] = "sisstatus = 'A'";
		
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				WHERE					
				" . ( count($codicao) ? implode(" AND ", $codicao) : "" );
		
		return $this->carregarObjeto($sql, get_class($this));
	}									  
}