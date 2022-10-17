<?php
	
class QGrupo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.grupopergunta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "grpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'grpid' => null, 
    									'queid' => null, //Questionario
									  	'perid' => null, //Pergunta
    									'itpid'		=> null, //ItemPergunta
									  	'gru_grpid' => null, //GrupoPergunta com ela mesma
									  	'grptitulo' => null,
									  	'grpordem' => null,
    									'grpstatus'	=> null,
    									'grpajuda'	=> null
									  );

	public function lista(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $item){
			if (get_class($item) == 'QQuestionario'){
				$codicao[] = "queid = " . $item->queid;
			}elseif (get_class($item) == 'QGrupo'){
				$codicao[] = "gru_grpid = " . $item->grpid;
			}elseif (get_class($item) == 'QItemPergunta'){
				$codicao[] = "itpid = " . $item->itpid;
			} 
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );
			
		return $this->carregar($sql);
	}									  
									  
	public function listaObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		foreach ($where as $k => $item){
			if ($k === 'itpid'){
				if ( is_array($item) ){
					$codicao[] = "itpid IN (" . implode(", ", $item) . ")";
				}else{
					$codicao[] = "itpid = " . ($item ? $item : 'NULL');
				}
				continue;	
			}elseif ($k === 'gru_grpid'){
				$codicao[] = "gru_grpid = " . $item;
				continue;
			}elseif ($k === 'grpid'){
				$codicao[] = "grpid = " . $item;
				continue;
			}elseif (get_class($item) == 'QQuestionario'){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif (get_class($item) == 'QGrupo'){
				$codicao[] = "gru_grpid = " . $item->grpid;
				continue;
			}elseif (get_class($item) == 'QItemPergunta'){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ). " ORDER BY grpordem";
				
				return $this->carregarObjeto($sql, get_class($this));
	}
					
	public function pegaUmObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $k => $item){
			if ($k === 'grpid'){
				$codicao[] = "grpid = " . $item;
				continue;
			}elseif ($k === 'gru_grpid'){
				$codicao[] = "gru_grpid = " . $item;
				continue;
			}elseif ($k === 'grpordem'){
				$codicao[] = "grpordem = " . $item;
				continue;					
			}elseif ($k === 'queid'){
				$codicao[] = "queid = " . $item;
				continue;					
			}elseif ($k === 'itpid'){
				if ( is_array($item) ){
					$codicao[] = "itpid IN (" . implode(", ", $item) . ")";
				}else{
					$codicao[] = "itpid = " . ($item ? $item : 'NULL');
				}
				continue;					
			}elseif (get_class($item) == 'QQuestionario'){
				$codicao[] = "queid = " . $item->queid;
				continue;					
			}elseif (get_class($item) == 'QGrupo'){
				$codicao[] = "gru_grpid = " . $item->grpid;
				continue;					
			}elseif (get_class($item) == 'QItemPergunta'){
				$codicao[] = "itpid = " . $item->itpid;
				continue;					
			}elseif (get_class($item) == 'QPergunta'){
				$codicao[] = "perid = " . $item->perid;
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
					
		return parent::pegaUmObjeto($sql, get_class($this));

	}		
					
	public function carregaUm(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $k => $item){
			if ($k === 'grpid'){
				$codicao[] = "grpid = " . $item;
				continue;
			}elseif ($k === 'gru_grpid'){
				$codicao[] = "gru_grpid = " . $item;
				continue;
			}elseif ($k === 'grpordem'){
				$codicao[] = "grpordem = " . $item;
				continue;					
			}elseif ($k === 'queid'){
				$codicao[] = "queid = " . $item;
				continue;					
			}elseif ($k === 'itpid'){
				if ( is_array($item) ){
					$codicao[] = "itpid IN (" . implode(", ", $item) . ")";
				}else{
					$codicao[] = "itpid = " . ($item ? $item : 'NULL');
				}
				continue;					
			}elseif (get_class($item) == 'QQuestionario'){
				$codicao[] = "queid = " . $item->queid;
				continue;					
			}elseif (get_class($item) == 'QGrupo'){
				$codicao[] = "gru_grpid = " . $item->grpid;
				continue;					
			}elseif (get_class($item) == 'QItemPergunta'){
				$codicao[] = "itpid = " . $item->itpid;
				continue;					
			}elseif (get_class($item) == 'QPergunta'){
				$codicao[] = "perid = " . $item->perid;
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
					
		return parent::pegaUm($sql);

	}						

//	function montaGrupo(Array $arr, $paiid){
//		foreach($arr as $obGru){
//			
//			// IMG nível 3
//			$imgAltera = addslashes('<img src=\'/imagens/alterar.gif\' title=\'Alterar Tipo Documento\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroGrupo&acao=A&grpid=' . $obGru->grpid . '\',\'Janela1\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=480,height=240\'); w.focus();">'); 
//			$imgExclui = addslashes('<img src=\'/imagens/excluir.gif\' title=\'Excluir Grupo\' onClick="apagar(\'' . $_SERVER['REQUEST_URI'] . '&instrucao=excluir&grpid=' . $obGru->grpid . '\', \'Deseja apagar o Grupo ( ' . $obGru->grptitulo . ' )?\') "/>');
//			$arvore .= "arvore.add('gru_" . $obGru->grpid . "', 'que_" . $paiid . "', '$imgAltera $imgExclui " . $obGru->grptitulo . "', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
//			
//			// IMG inclui nível 4
//			$imgInclui = addslashes('<img src=\'/imagens/gif_inclui.gif\' title=\'Incluir nova "Pergunta"\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&grpid=' . $obGru->grpid . '\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=530,height=400\'); w.focus();">');
//			$arvore .= "arvore.add('gru_imgInclui_" . $obGru->grpid . "', 'gru_" . $obGru->grpid . "', '" . $imgInclui . " Adicionar Pergunta', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
//			
//			//$arrObPergunta = (array) $p->list  aObjeto( array( 0 => $obGru, "itpid" => null ) );
//			//$arvore .= $this->a->mo     ntaGrupo($arrObAba, $obQuestionario->queid); 
//		}
//		return $arvore;
//	}

	public function ordena(Array $where){
		if($where['gru_grpid']){
			$where = "gru_grpid = " . $where['gru_grpid'];
		} elseif ($where['queid']){
			$where = "queid = " . $where['queid'];
		} else {
			$where = "itpid = " . $where['itpid'];
		}
		$sql = "SELECT grpid FROM questionario.grupopergunta WHERE $where ORDER BY grpordem";
		$arrGrpid = (array) $this->carregarColuna($sql);
		if($arrGrpid){
			$i = 0;
			foreach($arrGrpid as $grpidA){
				$i++;
				$ordemA = $i;
				$sql = "UPDATE questionario.grupopergunta SET grpordem = $ordemA  WHERE grpid = $grpidA";
				$this->executar($sql);
			}
			$this->commit();
		}
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
}