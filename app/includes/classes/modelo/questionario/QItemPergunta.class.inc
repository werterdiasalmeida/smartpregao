<?php
	
class QItemPergunta extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.itempergunta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "itpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     * 
     */    
    protected $arAtributos     = array(
									  	'itpid' => null, 
									  	'perid' => null, 
									  	'itptitulo' => null
									  );
									  
	public function lista(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $item){
			if (get_class($item) == 'QPergunta'){
				$codicao[] = "perid = " . $item->perid;
			}
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );
		
		return $this->carregar($sql, get_class($this));
	}		
	
	public function carregarColuna(Array $where = null, $coluna = 'itpid'){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $k => $item){
			if (get_class($item) == 'QPergunta'){
				$codicao[] = "perid = " . $item->perid;
			}elseif ( $k == 'perid'){
				$codicao[] = "perid IN ('" . (is_array($item) ? implode("', '", $item) : $item) . "')";
			}
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );

		return parent::carregarColuna($sql, $coluna);
	}		
									  
	public function listaObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		foreach ($where as $k => $item){
			if ( get_class($item) == 'QItemPergunta' ){
				$codicao[] = "ip.itpid = " . $item->itpid;
				continue;
			}elseif ( get_class($item) == 'QPergunta' ){
				$codicao[] = "ip.perid = " . $item->perid;
				continue;
			}elseif ( $k === 'itpid' ){
				$codicao[] = "itpid IN ('" . (is_array($item) ? implode("', '", $item) : $item) . "')";
				continue;
			}	
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "ip.*") . "
				FROM
					{$this->stNomeTabela} ip
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) ." ORDER BY ip.itpid";
		
		return $this->carregarObjeto($sql, get_class($this));
	}										  
	
	public function pegaUmObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $k => $item){
			if ( $k === 'itpid' ){
				$codicao[] = "ip.itpid = " . $item;
				continue;				
			}elseif ( $k === 'respondido' && $item === true ){
				$inner = " JOIN questionario.resposta r USING(itpid) ";
				continue;
			}elseif ( get_class($item) == 'QPergunta' ){
				$codicao[] = "ip.perid = " . $item->perid;
				continue;
			}
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela} ip
					{$inner}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );
		return parent::pegaUmObjeto($sql, get_class($this));
	}										  

	public function antesExcluir(){
		$grupo = new QGrupo();
		$pergunta = new QPergunta();
		$retornoG = $grupo->lista( array($this), "count(*) AS total" );
		$retornoP = $pergunta->lista( array($this), "count(*) AS total" );
		$this->commit();
		
		if ( $retornoG[0]['total'] > 0 || $retornoP[0]['total'] > 0 ){
			return false;
		}else{
			return true;
		}
	}	
	
	public function possuiPerguntaGrupoAssociado(Array $param = null){
		if ( !$param ){
			$clausula[] = "ip.itpid = " . $this->itpid;
		}
		$param = (array) $param;
		foreach($param as $k => $value){
			if ($k == "itpid"){
				$clausula[] = "ip.itpid = " . $value;
				continue;
			}elseif ( get_class($value) == "QItemPergunta" ){
				$clausula[] = "ip.itpid = " . $value->itpid;
				continue;
			}
		}
		
		$sql = "SELECT
					count(*)
				FROM
					questionario.itempergunta ip
				LEFT JOIN questionario.pergunta p USING(itpid)
				LEFT JOIN questionario.grupopergunta gp USING(itpid)
				WHERE
				(p.itpid IS NOT NULL OR gp.itpid IS NOT NULL) AND 
				" . implode(" AND ", $clausula);

		return $this->pegaUm($sql);
	}
	
	public function verificaGrupo( $item ){
		if( $item ){
			$sql = "SELECT grpid FROM questionario.grupopergunta WHERE itpid = ". $item;
			return $this->pegaUm($sql);
		}
	}
	
	public function verificaPergunta( $item ){
		if( $item ){
			$sql = "SELECT count(perid) as per FROM questionario.pergunta WHERE itpid = ". $item;
			return $this->pegaUm($sql);
		}
	}

	public function carregaItensPergunta( $perid, $coluna = null ){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela} ip
				WHERE perid = ".$perid;

		return parent::carregar($sql);
	}
}