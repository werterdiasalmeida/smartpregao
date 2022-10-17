<?php
	
class QResposta extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.resposta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "resid" );

    /**
     * Atributos
     * @var array
     * @access protected
     * 	
     */    
    protected $arAtributos     = array(
									  	'resid' => null, 
									  	'perid' => null, 
									  	'qrpid' => null, 
									  	'usucpf' => null, 
									  	'itpid' => null, 
									  	'resdsc' => null, 
									  );
									  
	public function pegaUmObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){

			if ( get_class($item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ($k === 'qrpid'){
				$codicao[] = "qrpid = " . $item;
				continue;
			}elseif ($k === 'perid'){
				$codicao[] = "perid = " . $item;
				continue;
			}
				
				
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );

		return parent::pegaUmObjeto($sql, get_class($this));
	}								  

	public function listaObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( get_class($item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ($k === 'perid'){
				$codicao[] = "perid = " . $item;
				continue;
			}elseif ($k === 'qrpid'){
				$codicao[] = "qrpid = " . $item;
				continue;
			}
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );
					
		return $this->carregarObjeto($sql, get_class($this));
		
	}		
	
	public function carregarColuna(Array $where = null, $coluna='resid'){
//		$coluna = (array) $coluna;
//		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( get_class($item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ($k === 'qrpid'){
				$codicao[] = "qrpid = " . $item;
				continue;
			}
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );

		return parent::carregarColuna($sql, $coluna);			
	}
}