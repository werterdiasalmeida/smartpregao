<?php
	
class QPergunta extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.pergunta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "perid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    
    protected $arAtributos     = array(
									  	'perid' => null, 
									  	'grpid' => null, 
									  	'queid' => null, 
									  	'itpid' => null, 
//									  	'perboleana' => null, 
//									  	'perflagitem' => null, 
//									  	'perflagmultiescolha' => null, 
									  	'pertitulo' => null, 
									  	'perordem' => null, 
									  	'perajuda' => null,
    									'perobrigatorio' => null, 
    									'permascara' => null, 
    									'pertipo' => null, 
    									'permultipla' => null, 
    									'perposacao' => null 
									  );

	public function lista(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $item){
			if (get_class($item) == 'QGrupo'){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ( get_class($item) == 'QItemPergunta' ){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}elseif ( get_class($item) == 'QQuestionario' ){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );
		
		return $this->carregar($sql, get_class($this));
	}									  
									  
	
	public function listaObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( $k === 'itpid' ){
				$codicao[] = "itpid" . ( $item ? " = " . $item : ' IS NULL' );
				continue;	
			}elseif ( get_class($item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( get_class($item) == 'QQuestionario' ){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif ( get_class($item) == 'QGrupo' ){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ( get_class($item) == 'QItemPergunta' ){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}	
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) . " ORDER BY perordem";
		return $this->carregarObjeto($sql, get_class($this));
	}							

//	$this->pegaUmObjeto()
	
	public function pegaUmObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( $k === 'itpid' ){
				$codicao[] = "itpid" . ( $item ? " = " . $item : ' IS NULL' );
				continue;	
			}elseif ($k === 'perid'){
				$codicao[] = "perid = {$item}";
				continue;	
			}elseif ($k === 'grpid'){
				$codicao[] = "grpid = {$item}";
				continue;	
			}elseif ($k === 'queid'){
				$codicao[] = "queid = {$item}";
				continue;	
			}elseif ($k === 'perordem'){
				$codicao[] = "perordem = {$item}";
				continue;	
			}elseif ( get_class($item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( get_class($item) == 'QQuestionario' ){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif ( get_class($item) == 'QGrupo' ){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ( get_class($item) == 'QItemPergunta' ){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}elseif ( $k == 'ordem' ){
				$order = $item;
				continue;
			}	
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) . 
				" " . ( $order ? " ORDER BY " . $order : "" ) . 
				" LIMIT 1";
//		dbg($sql);			
		return parent::pegaUmObjeto($sql, get_class($this));
	}	
								  
	public function carregaUm(Array $where = null, $coluna = 'perid'){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( $k === 'itpid' ){
				$codicao[] = "itpid" . ( $item ? " = " . $item : ' IS NULL' );
				continue;	
			}elseif ($k === 'perid'){
				$codicao[] = "perid = {$item}";
				continue;	
			}elseif ($k === 'grpid'){
				$codicao[] = "grpid = {$item}";
				continue;	
			}elseif ($k === 'queid'){
				$codicao[] = "queid = {$item}";
				continue;	
			}elseif ($k === 'perordem'){
				$codicao[] = "perordem = {$item}";
				continue;	
			}elseif ( get_class($item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( get_class($item) == 'QQuestionario' ){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif ( get_class($item) == 'QGrupo' ){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ( get_class($item) == 'QItemPergunta' ){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}elseif ( $k == 'ordem' ){
				$order = $item;
				continue;
			}	
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) . 
				" " . ( $order ? " ORDER BY " . $order : "" ) . 
				" LIMIT 1";
//		dbg($sql);			
		return parent::pegaUm($sql);
	}								  

	public function carregarColuna(Array $where = null, $coluna = 'perid'){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $k => $item){
			if (get_class($item) == 'QPergunta'){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( get_class($item) == 'QGrupo' ){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ($k === 'itpid'){
				$codicao[] = "itpid IN(" . (is_array($item) ? implode(', ', $item) : $item) . ")";
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
	
	
	public function montaPergunta(Array $arr, $obPai){
		
		if (get_class($obPai) == 'QQuestionario'){
			$idPai = 'que_' . $obPai->queid;
		}elseif (get_class($obPai) == 'QGrupo'){
			$idPai = 'gru_' . $obPai->grpid;
		}elseif (get_class($obPai) == 'QItemPergunta'){
			$idPai = 'item_' . $obPai->itpid;
		}
		
		foreach($arr as $obPergunta){

				// IMG nível 4
				$imgAltera = addslashes('<img src=\'/imagens/alterar.gif\' title=\'Alterar Tipo Documento\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&perid=' . $obPergunta->perid . '\',\'Janela1\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=530,height=400\'); w.focus();">'); 
				$imgExclui = addslashes('<img src=\'/imagens/excluir.gif\' title=\'Excluir Tipo Documento\' onClick="apagar(\'' . $_SERVER['REQUEST_URI'] . '&instrucao=excluir&perid=' . $obPergunta->perid . '\', \'Deseja apagar a Pergunta ( ' . substr($obPergunta->pertitulo, 0, 80) . (strlen($obPergunta->pertitulo) > 80 ? '...' : '') . ' )?\') "/>');
				$arvore .= "arvore.add('per_" . $obPergunta->perid . "', '{$idPai}', '$imgAltera $imgExclui " . substr($obPergunta->pertitulo, 0, 100) . (strlen($obPergunta->pertitulo) > 100 ? '...' : '') . "', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
				
				//$arrObItem = (array) $it->listaObjeto( array( $obPergunta) );
		}
		return $arvore;
	}
	
	public function perguntaRespondida( $perid = null, $qrpid = null ){
		
		$perid = $perid ? $perid : $this->perid;
		
		if($qrpid){
			$where = "AND r.qrpid = {$qrpid}";
		}
		
		$sql = "SELECT
						r.perid
				FROM
						questionario.resposta r
				WHERE
					r.perid = {$perid} {$where}";
		
		return parent::carregarColuna($sql);
		
	}
	
	public function perguntaRespondidaComItem( $perid, $qrpid ){
		
		$sql = "SELECT
						ip.itpid
				FROM
						questionario.itempergunta ip
				INNER JOIN questionario.resposta r using(itpid) 
				LEFT JOIN questionario.pergunta p using(itpid)
				LEFT JOIN questionario.grupopergunta g using(itpid)
				WHERE
					ip.perid = {$perid}
					AND r.qrpid = {$qrpid}
		--			AND	(p.itpid IS NOT NULL OR g.itpid IS NOT NULL)";

		return parent::carregarColuna($sql);
		
	}
	
	public function ordena(Array $where){
		if($where['grpid']){
			$where = "grpid = " . $where['grpid'];
		} elseif ($where['queid']){
			$where = "queid = " . $where['queid'];
		} else {
			$where = "itpid = " . $where['itpid'];
		}
		$sql = "SELECT perid FROM questionario.pergunta WHERE $where ORDER BY perordem";
		$arrPerid = (array) parent::carregarColuna($sql);
		if($arrPerid){
			$i = 0;
			foreach($arrPerid as $peridA){
				$i++;
				$ordemA = $i;
				$sql = "UPDATE questionario.pergunta SET perordem = $ordemA  WHERE perid = $peridA";
				$this->executar($sql);
			}
			$this->commit();
		}
	}
	
	public function antesExcluir(){
		$itp 		   = new QItemPergunta();
		$arrObItemPerg = (array) $itp->listaObjeto( array($this), "itpid" );
		// Faz iteração verificando se os "itens perguntas" podem ser deletados
		$retorno = true;	
		foreach($arrObItemPerg as $obItemPerg){
			if ( !$obItemPerg->antesExcluir() ){
				$retorno = false;
				break;
			}
		}
		
		// Se TODOS os "itens perguntas" podem ser deletados, ou seja,
		// não possuem sub-pergunta, faz a deleção dos mesmos.
		if ($retorno){
			foreach($arrObItemPerg as $obItemPerg){
				$obItemPerg->excluir();
				$this->commit();
			}
		}
		return $retorno;
	}	
	
}