<?php
final class GerenciaQuestionario {
	private $tipoApresentacao;
	private $qrpid;
	private $queid;
	private $perid;
	private $questionario;
	private $dadosPai;
	private $dadosFilho;
	private $dados;
	private $dadosForm;
	private $itemPai;
	private $tipoBusca;
	private $proxPergunta;
	private $antPergunta;
	private $perguntaPai;
	private $resposta;
	private $q;
	private $g;
	private $p;
	private $i;
	private $r;
//	private $db;
	
	function __construct( Array $arrParam ){
		
		include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
		include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionarioResposta.class.inc";
		include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
		include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
		include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
		include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
		include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
		
		$this->qr = new QQuestionarioResposta();
		$this->q  = new QQuestionario();
		$this->g  = new QGrupo(); 
		$this->p  = new QPergunta(); 
		$this->i  = new QItemPergunta();
		$this->r  = new QResposta();
		
		$this->qrpid 		= $arrParam['qrpid'];
		self::buscaQuestionario();
		if ( !$this->qrpid ){die('Faltam Parametros (qrpid)');}		
		
//		$this->queid 		= $arrParam['queid'];
		$this->perid 		= ( $arrParam['perid'] ? $arrParam['perid'] : self::primeiraPerid( array("queid" => $this->queid) ) );
		$this->dadosPai 	= array();
		$this->dadosFilho 	= array();

		// Se for a opção de salvar, não necessita carregar o resto
		if( $arrParam['salvar_questionario'] ) return;
		
		if ( !$arrParam['itpid'] ){
			self::arvorePerguntaPai( $this->perid );
			self::arvorePerguntaFilho( $this->perid );
		}else{
			self::arvoreItemPerguntaFilho( $arrParam['itpid'] );
		}
	}
	
	function arvorePerguntaPai( $perid ){
		$arrFiltro = array(
							"perid" => $perid
						 );
		$pergunta = $this->p->pegaUmObjeto( $arrFiltro );
		self::buscaPerguntaPai( array("perid" => $pergunta->perid) );
		array_push( $this->dadosPai, self::buscaQuestionario() );
		
	}
	
	function arvorePerguntaFilho( $perid ){
		$this->tipoBusca = 'perguntaRespondida';
		$arrFiltro = array(
							"perid" => $perid
						 );
		$pergunta = $this->p->pegaUmObjeto( $arrFiltro );
		
		self::buscaPerguntaFilho( $pergunta );
//		array_push( $this->dadosFilho, self::buscaQuestionario() );
		
	}
	
	function arvoreItemPerguntaFilho( $itpid ){
		$this->tipoBusca = 'perguntaRespondida';
		$arrFiltro = array(
							"itpid" => $itpid
						 );
		$item = $this->i->pegaUmObjeto( $arrFiltro );
		self::buscaItemPerguntaFilho( $item );
		
	}

	private function buscaQuestionario(){
		if (get_class($this->questionario) != 'QQuestionario' ){
			if ( !$this->queid ){
				$this->queid = $this->qr->carregaUm(array("qrpid" => $this->qrpid), 'queid');
			}			
			$this->questionario = $this->q->pegaUmObjeto( array("queid" => $this->queid) );
		}
		return $this->questionario;
	}
	
	/*
	 * Funções
	 * Busca árvore para nível de cima - PAI
	 */	
	private function buscaGrupoPerguntaPai( Array $filtro ){
		if ( !current($filtro) ) return;
		$obj = $this->g->pegaUmObjeto( $filtro );
		
		if ( $this->tipoBusca != "perguntaPai" && $this->tipoBusca != "itemPai" ){
			array_push( $this->dadosPai, $obj );
		}	
//		ver($obj);
		
		self::buscaGrupoPerguntaPai( array("grpid" => $obj->gru_grpid) );
		self::buscaItemPerguntaPai( array("itpid" => $obj->itpid) );
	}
//	
	private function buscaPerguntaPai( Array $filtro, Array $param = null  ){
		if ( !current($filtro) ) return;
//		dbg($filtro);
		$obj = $this->p->pegaUmObjeto( $filtro );
		
		if ( !$param['pulaUm'] && $this->tipoBusca == "perguntaPai"){
			$this->perguntaPai = $obj;
			return;
		}
		
		if ( $this->tipoBusca != "perguntaPai" && $this->tipoBusca != "itemPai" ){
			array_push( $this->dadosPai, $obj );
		}
//		ver($filtro);
		self::carregaResposta( $obj );
		self::buscaGrupoPerguntaPai( array("grpid" => $obj->grpid) );
		self::buscaItemPerguntaPai( array("itpid" => $obj->itpid) );
	}
	
	private function buscaItemPerguntaPai( Array $filtro ){
		if ( !current($filtro) ) return;
		
		$obj = $this->i->pegaUmObjeto( $filtro );
//		if ( $this->tipoBusca != "ItemPai" ){
		if ( $this->tipoBusca == "itemPai" ){
			$this->itemPai = $obj;
			return;
		}elseif ( $this->tipoBusca != "perguntaPai" && $this->tipoBusca != "itemPai" ){
			array_push( $this->dadosPai, $obj );
		}
		
		self::buscaPerguntaPai( array("perid" => $obj->perid) );
	}
	/*
	 * FIM  - Funções
	 * Busca árvore para nível de cima - PAI
	 */	
	
	/*
	 * Funções
	 * Busca árvore para nível de baixo - FILHO
	 */
	private function buscaGrupoPerguntaFilho( $obParam ){
		if ( !is_object($obParam) ) return;
		
		$arrObGrupo = $this->g->listaObjeto( array($obParam) );
		// Caso Retorne vazio termina
		if ( !$arrObGrupo ) return;
		
		foreach( $arrObGrupo  as $objGrupo ):
		
			array_push( $this->dadosFilho, $objGrupo );
			// Busca Pergunta
			self::buscaPerguntaFilho( $objGrupo );
			// Busca Grupo
			self::buscaGrupoPerguntaFilho( $objGrupo );
		endforeach;
	}
//	
	private function buscaPerguntaFilho( $obParam ){
		if ( !is_object($obParam) ) return;
		
		$arrObPergunta = $this->p->listaObjeto( array($obParam) );
		// Caso Retorne vazio termina
		if ( !$arrObPergunta ) return;
		
		foreach( $arrObPergunta  as $objPergunta ):
			array_push( $this->dadosFilho, $objPergunta );
			
			// Busca item pergunta
			self::buscaItemPerguntaFilho( $objPergunta );
		
		endforeach;
	}
	
	private function buscaItemPerguntaFilho( $obParam ){
		if ( !is_object($obParam) ) return;
		// Parametro de filtro
		$param[] = $obParam;
		
		if ( get_class($obParam) == 'QPergunta' ){
			$arrResp = self::trataResposta( self::carregaResposta($obParam) );
			if ( is_array( $arrResp ) && $this->tipoBusca == 'perguntaRespondida' ){
				// Adiciona parametro de filtro
				// Somente os itens que foram escolhidos como resposta à pergunta
				$param['itpid'] = $arrResp;
			}elseif( $this->tipoBusca == 'perguntaRespondida' ){
				return;
			}
		}
		
		$arrObItem = $this->i->listaObjeto( $param );
		// Caso Retorne vazio termina
		if ( !$arrObItem ) return;

		foreach( $arrObItem  as $objItem ):
			array_push( $this->dadosFilho, $objItem );
			
			// Busca Pergunta
			self::buscaPerguntaFilho( $objItem );
			// Busca Grupo
			self::buscaGrupoPerguntaFilho( $objItem );
		endforeach;
	}
	/*
	 * FIM - Funções
	 * Busca árvore para nível de baixo - FILHO
	 */
	public function getQrpid(){
		return $this->qrpid;
	}
	
	public function getQueTitulo(){
		return $this->questionario->quetitulo;
	}
	
	public function getQueDtInicio(){
		return $this->questionario->quedtinicio;
	}

	public function getQueDtFim(){
		return $this->questionario->quedtfim;
	}
	
	public function getPerid(){
		return $this->perid;
	}
	
	public function getPeridPai(){
		return $this->perguntaPai ? $this->perguntaPai->perid : $this->perid;
	}
	
	public function getDados(){
		if ( !$this->dados ){
			
			$arrObDadosPai 	 = array_reverse( $this->dadosPai );
			$arrObDadosFilho = $this->dadosFilho; 
			array_shift( $arrObDadosFilho );
			
			$this->dados	 = array_merge($arrObDadosPai, $arrObDadosFilho);
		}
		
		return $this->dados;
	}
	
	public function getQuestionario(){
		return $this->questionario;
	}
	
	public function getQueid(){
		return $this->queid;
	}
	
	public function carregaObItensPergunta( QPergunta $obPergunta ){
		$arrObj = $this->i->listaObjeto( array($obPergunta), array("itpid", "itptitulo") );
		return $arrObj;
	}

	public function carregaResposta( QPergunta $obPergunta ){
		if ( !$this->resposta[$obPergunta->perid] ){
			$this->resposta[$obPergunta->perid] = $this->r->listaObjeto( array($obPergunta, "qrpid" => $this->qrpid) );
		}
		return $this->resposta[$obPergunta->perid];
	}
	
	public function trataResposta( $arrObResposta ){
		if ( !is_array($arrObResposta) ) return;
		
		foreach($arrObResposta as $obResposta){
			if ( $obResposta->itpid ){
				$resp[] = $obResposta->itpid;
			}else{
				$resp = $obResposta->resdsc;
			}
		}
		return $resp;
	}

//	public function itemPossuiPergunta ( ItemPergunta $obItemPerg ){
//		$this->p->
//	}
/*
 * Funções que buscam a PRÓXIMA PERGUNTA
 * 
 */
	public function proximaPergunta(){
		$perid  = self::getPeridPai();
		$obPerg = $this->p->pegaUmObjeto(array("perid" => $perid));
		// Busca próxima pergunta filha do questionário
		if ( $obPerg->queid ){
			$obPergProx = self::proximaPerguntaQuestionario( $obPerg );
			if ($obPergProx){ 
				$this->proxPergunta = $obPergProx;
				return $this->proxPergunta;
			}
			$where['queid'] = $obPerg->queid;
			$where['grpordem']  = 1;
			$obGrupo 			= $this->g->pegaUmObjeto($where);
			$obPergProx = self::proximaPerguntaGrupo( $obGrupo, array("pergGrupoAtual" => true) );
			if ($obPergProx){ 
				$this->proxPergunta = $obPergProx;
				return $this->proxPergunta;
			}
		// Busca próxima pergunta filha do grupo	
		}elseif ( $obPerg->grpid ){
			$obPergProx = self::proximaPerguntaGrupo( $obPerg );
			if ($obPergProx){ 
				$this->proxPergunta = $obPergProx;
				return $this->proxPergunta;
			}
		}
		//
		if ( $obPerg->grpid ){
			$obGrupo 			= self::buscaGrupoRaiz( $obPerg );
			$this->proxPergunta = self::proximaPerguntaGrupo( $obGrupo, array("pula" => true) );
		}
		return $this->proxPergunta;
	}

	private function proximaPerguntaGrupo( $obj, $param = null ){
		
		if ( $obj instanceof QPergunta ){
			$where['grpid']    = $obj->grpid;
			$where['perordem'] = ($obj->perordem + 1);
			
			$obPerg = $this->p->pegaUmObjeto($where);
			if ( !$obPerg ){
				unset($where);
				$where["grpid"] = $obj->grpid;
				$obGrupo 		= $this->g->pegaUmObjeto($where);
				$obPerg 		= self::proximaPerguntaGrupo($obGrupo);
			}
		}elseif ( $obj instanceof QGrupo ){
			
			
				if ( !$param['pula'] ){
					if ( !$param['pergGrupoAtual'] ){
						$where['gru_grpid'] = $obj->grpid;
					}else{
						$where['grpid'] = $obj->grpid;
					}
					$where['grpordem']  = $param['grpordem'] ? ($param['grpordem'] + 1) : 1;
					$obGrupo 			= $this->g->pegaUmObjeto($where);
				}
				
				if ( $obGrupo ){
					unset($where);
					$where['grpid']	   = $obGrupo->grpid; 
					$where['perordem'] = 1;
					$obPerg = $this->p->pegaUmObjeto($where);
				}else{
					unset($where);
					if ( $obj->queid ){
						$where['queid']    = $obj->queid;
						$where['grpordem'] = ($obj->grpordem + 1);
					}else{
						$where['gru_grpid'] = $obj->gru_grpid;
						$where['grpordem']  = $param['novoNivel'] ? 1 : ($obj->grpordem + 1);
//						$where['grpordem']  = ($obj->grpordem + 1);
//						$where['grpordem']  = 1;
					}
					$obGrupo = $this->g->pegaUmObjeto($where);
					if ( $obGrupo ){
						unset($where);
						$where['grpid']	   = $obGrupo->grpid; 
						$where['perordem'] = 1;
						$obPerg = $this->p->pegaUmObjeto($where);
						if ( !$obPerg ){
							unset($where);
							$where['gru_grpid'] = $obGrupo->grpid;
							$where['grpordem']  = 1;
							$obGrupo 			= $this->g->pegaUmObjeto($where);
						}
					}					
					
				}
				if ($obGrupo && !$obPerg){
					$obPerg = self::proximaPerguntaGrupo($obGrupo, array("novoNivel" => true));
				}
				if (!$obPerg){
					$obGrupoVez = $obj;
					$loop = true;
					while ($loop){
						unset($where);
						if (!$obGrupoVez->gru_grpid){
							break;
						}
						
						$where['gru_grpid'] = $obGrupoVez->gru_grpid;
						$where['grpordem']  = ($obGrupoVez->grpordem + 1);
						$obGrupo = $this->g->pegaUmObjeto($where);
						if ( !$obGrupo ){
							unset($where);
							$where['grpid'] = $obGrupoVez->gru_grpid;
							$obGrupoVez     = $this->g->pegaUmObjeto($where);
						}else{
							unset($where);
							$where["grpordem"] 		 = ($obGrupo->grpordem - 1); 
							$where["pergGrupoAtual"] = true;
							$obPerg = self::proximaPerguntaGrupo($obGrupo, $where);
							$loop   = false;						
						}
					}
				}
		}
		return $obPerg;
		
	}	
	
	private function proximaPerguntaQuestionario( QPergunta $obPerg ){
		$param[0] 		   = $this->questionario;
		$param['perordem'] = $obPerg->perordem + 1; 
		return $this->p->pegaUmObjeto($param);
	}	
	
//	function proximaPergunta(){
//		if ( !$this->proxPergunta ){
//			$obPerg 	 = $this->p->pegaUmObjeto(array("perid" => $this->perid));
//			$perguntaPai = self::perguntaPai( array("perid" => $obPerg->perid) );
//			
//			if ( $perguntaPai ){
//				$obPerg = $perguntaPai;
//			}
//			
//			if ( $obPerg->queid ){
//				// Busca próxima pergunta na raiz do questinário
//				$obPergProx = self::proximaPerguntaQuestionario( $obPerg );
//				// se encontrar próxima pergunta, retorna.
//				if ( $obPergProx ){
//					$this->proxPergunta = $obPergProx; 
//					return $this->proxPergunta;
//				}
//				// Busca 1ª pergunta dentro do 1º Grupo
//				$obPergProx = self::proximaPerguntaGrupo(array("grpordem" => 1), $this->questionario );
//				if ( $obPergProx ){
//					$this->proxPergunta = $obPergProx;
//					return $this->proxPergunta; 
//				}else{ 
//					return false;
//				}	
//			}
//			
//			if ( $obPerg->grpid ){
//				$obPergProx = self::proximaPerguntaGrupo( array("grpid" => $obPerg->grpid, "perordem" => ($obPerg->perordem + 1) ));
//				if ( $obPergProx ){
//					$this->proxPergunta = $obPergProx;
//					return $this->proxPergunta;
//				}else{
//					$obGrupo = $this->g->pegaUmObjeto(array("grpid" => $obPerg->grpid));
//					// busca próximo grupo, pela ordem
//					if ($obGrupo->queid){
//						$obGrupo = $this->g->pegaUmObjeto(array("queid" => $obGrupo->queid, "grpordem" =>($obGrupo->grpordem + 1) ));
//					}else{
//						$obGrupo = $this->g->pegaUmObjeto(array("gru_grpid" => $obGrupo->gru_grpid, "grpordem" =>($obGrupo->grpordem + 1) ));
//					}
//					if ($obGrupo){
//						$obPergProx 		= self::proximaPerguntaGrupo( array("grpid" => $obGrupo->grpid, "perordem" => 1 ));
//						$this->proxPergunta = $obPergProx;
//						return $this->proxPergunta;
//					}
//				}
//			}
//			
//			$dadosPai = array_reverse($this->dadosPai);
//			$obj 	  = $dadosPai[1];
//			if (get_class($obj) == "Grupo"){
//				$this->proxPergunta = self::proximaPerguntaGrupo(array("grpordem" => ($obj->grpordem + 1) ) , $this->questionario);
//				return $this->proxPergunta;
//			}  
//			
//		}else{
//			return $this->proxPergunta;
//		}
//	}
//		
//	private function proximaPerguntaGrupo( Array $param, $obj = null ){
//		foreach( $param as $k => $item ){
//			if ($k == 'grpordem'){
//				$where['grpordem'] = $item;
//			}elseif ($k === 'grpid'){
//				$where['grpid'] = $item;
//			}elseif ($k === 'gru_grpid'){
//				$where['gru_grpid'] = $item;
//			}elseif ($k === 'perordem'){
//				$where['perordem'] = $item;
//			}			
//		}
//		
//		if (get_class($obj) == 'Questionario'){
//			$where[0] = $obj;
//			$obGrupo = $this->g->pegaUmObjeto( $where );
//			if ( !$obGrupo ) return false;
//			unset($where);
//			$where[0] 		   = $obGrupo;
//			$where['perordem'] = 1;	 	
//			$this->proxPergunta = $this->p->pegaUmObjeto($where);
//			return $this->proxPergunta;
//		}
//		
//		$obPerg = $this->p->pegaUmObjeto($where);
////		dbg($where);
//		if ( $obPerg ){
//			$this->proxPergunta = $obPerg;
//			return $this->proxPergunta;
//		}
//
//		unset($where);
//		$where['gru_grpid'] = $param['grpid'];
//		$arrObGrupo = $this->g->listaObjeto($where);
//		
//		if ($arrObGrupo){
//			foreach ($arrObGrupo as $obGrupo){
//				unset($where);
//				$where['grpid']    = $obGrupo->grpid; 
//				$where['perordem'] = 1; 
//				
//				if ( self::proximaPerguntaGrupo($where) ) return $this->proxPergunta; 
//			}
//		}
//		return false;
//	}
//	
//	private function proximaPerguntaQuestionario( Pergunta $obPerg ){
//		$param[0] 		   = $this->questionario;
//		$param['perordem'] = $obPerg->perordem + 1; 
//		return $this->p->pegaUmObjeto($param);
//	}
/*
 * FIM - Funções que buscam a PRÓXIMA PERGUNTA
 * 
 */
	
/*
 * Funções que buscam a PERGUNTA ANTERIOR
 */	
	
	function anteriorPergunta(){
		if ( !$this->antPergunta ){
			$perguntaPai = self::perguntaPai(array("perid" => $this->perid));
			
			if ( $perguntaPai ){
				$obPerg = $perguntaPai;
			}else{
				$obPerg = $this->p->pegaUmObjeto(array("perid" => $this->perid));	
			}
			
			if ( $obPerg->queid ){
				$this->antPergunta = self::anteriorPerguntaQuestionario( $obPerg );
				return $this->antPergunta;
			}
			
			if ( $obPerg->grpid ){
				$param['perordem'] = ($obPerg->perordem - 1);
				$param['grpid']	   = $obPerg->grpid;	
				$this->antPergunta = self::anteriorPerguntaGrupo($param);
				return $this->antPergunta;
			}
		}else{
			return $this->antPergunta;
		}
		
	}
	
	private function anteriorPerguntaGrupo(Array $param, $obj = null){
		foreach( $param as $k => $item ){
			if ($k == 'grpordem'){
				$where['grpordem'] = $item;
			}elseif ($k === 'grpid'){
				$where['grpid'] = $item;
			}elseif ($k === 'gru_grpid'){
				$where['gru_grpid'] = $item;
			}elseif ($k === 'perordem'){
				$where['perordem'] = $item;
			}elseif ($k === 'queid'){
				$where['queid'] = $item;
			}elseif ($k === 'ordem'){
				$where['ordem'] = $item;
			}			
		}

		
		$obPerg = $this->p->pegaUmObjeto( $where );
		if ( $obPerg || ($param['queid'] && !$obPerg) ) return $obPerg;
		
		$obGrupo = $this->g->pegaUmObjeto(array("grpid" => $where['grpid']));
		
		unset($where);
//		dbg($obGrupo);
		if ( $obGrupo ){
			if ( $obGrupo->grpordem > 1  ){
				if ( $obGrupo->gru_grpid ){
					$where['gru_grpid'] = $obGrupo->gru_grpid; 
					$where['grpordem']  = ($obGrupo->grpordem - 1); 
				}else{
					$where['queid'] 	= $obGrupo->queid; 
					$where['grpordem']  = ($obGrupo->grpordem - 1); 
				}
				// Busca Grupo anterior na árvore
				$obGrupo = $this->g->pegaUmObjeto( $where );
				// Busca Ultimo nível de grupo, dentro do grupo.
				$obGrupo = self::ultimoNivelGrupo( $obGrupo );
				unset($where);
				$where['grpid'] = $obGrupo->grpid;
				$where['ordem'] = " perordem DESC ";
				return self::anteriorPerguntaGrupo( $where );
			}else{
				if ( $obGrupo->gru_grpid ){
					$where['grpid'] = $obGrupo->gru_grpid; 
					$where['ordem'] = " perordem DESC "; 
				}else{
					$where['queid'] 	= $obGrupo->queid; 
					$where['ordem'] = " perordem DESC "; 
				}
//				dbg($where, 1);
				return self::anteriorPerguntaGrupo( $where );
			}	
		}
	}
	
	private function ultimoNivelGrupo( QGrupo $obGrupo ){
		$where['gru_grpid'] = $obGrupo->grpid;
		$where['ordem'] 	= " grpordem DESC ";
		$obGrupoNovo = $this->g->pegaUmObjeto( $where );
		if ( $obGrupoNovo ){	
			$obGrupo = self::ultimoNivelGrupo( $obGrupoNovo );
		}	
		return $obGrupo;			
	}

	private function anteriorPerguntaQuestionario( QPergunta $obPerg ){
		$param[0] 		   = $this->questionario;
		$param['perordem'] = ($obPerg->perordem - 1); 
		return $this->p->pegaUmObjeto($param);		
	}
	
/*
 * FIM - Funções que buscam a PERGUNTA ANTERIOR
 */	
	function buscaGrupoRaiz($obj){
		if ( get_class($obj) == 'QPergunta' ){
			$obGrupo = $this->g->pegaUmObjeto( array("grpid" => $obj->grpid) );
		}else{
			$obGrupo = $this->g->pegaUmObjeto( array("grpid" => $obj->gru_grpid) );
		}
		//
		if ( !$obGrupo->queid ){
			$obGrupo = self::buscaGrupoRaiz( $obGrupo );
		}
		
		return $obGrupo; 
	}
	
	function perguntaPai( $param, Array $paramLocal = null ){
		$pergPai = $this->perguntaPai;
		if ( is_null($this->perguntaPai) || $paramLocal['paiPergAleatoria'] ){
			$this->tipoBusca = "perguntaPai";
			$this->buscaPerguntaPai($param, array("pulaUm" => true));
		}
		
		if ($paramLocal['paiPergAleatoria']){
			$pergPaiAleatorio  = $this->perguntaPai;
			$this->perguntaPai = $pergPai;
			return $pergPaiAleatorio;    
		}else{
			return $this->perguntaPai;
		}
	}
	
	function itemPai( $obj /*, Array $param = null*/){
		if ( !is_object($obj) ){return;}
		
		$this->tipoBusca = "itemPai";
		if ( get_class($obj) == 'QPergunta' ){
//			ver($obj);
			$this->buscaPerguntaPai(array($obj));
		}else{
			$this->buscaGrupoPerguntaPai(array("grpid" => $obj->grpid));
		}
		return $this->itemPai->itpid;
	}
	
	function salvar($dados = null){
		$this->dadosForm = (array) ($dados ? $dados : $_POST);
		$dados = $this->dadosForm;
		
		if ( !is_array($dados['perg']) ) return true;
		
		$arrPergPossivel = array();
		foreach ($dados['perg'] as $perid => $item):
			if ( $itemAnt ){
				// Busca as perguntas associadas aos itens selecionados no form 
				$arrPerg 		 = (array) $this->p->carregarColuna(array("itpid" => $itemAnt, 'perid'));
				$arrPergPossivel = array_merge($arrPergPossivel, $arrPerg);
				// Caso não encontre Perguntas associadas
				$arrObGrupo = (array) $this->g->listaObjeto(array("itpid" => $itemAnt));
				foreach($arrObGrupo as $obGrupo){
					if ( get_class($obGrupo) == 'QGrupo' ){
						$obGrupo = $this->ultimoNivelGrupo($obGrupo);
						// Busca as perguntas associadas ao grupo de último nível 
						$arrPergPossivelGrupo = (array) $this->p->carregarColuna(array($obGrupo));
						$arrPergPossivel      = array_merge($arrPergPossivel, $arrPergPossivelGrupo);
					}
				}	
				
				if ( !in_array($perid , $arrPergPossivel) ){
					continue;
				}
			}
			$p = new QPergunta($perid);
			$arrayItem = array("RA", "CK", "CM", "CB");
			
			if( in_array($p->pertipo, $arrayItem) ){
				if (!$item) continue;
				
				$this->salvarItem($p, $item);
				$itemAnt = (array) $item; //( empty($itemAnt) ? $item : $itemAnt );
			}else{
				$this->salvarText($p, $item);
			}
		endforeach;
	}
	
	function salvarItem( QPergunta $p, $itens ){
		$itens 	  = (array) $itens;
		$arrResp  = (array) $this->carregaResposta($p);
		$arrItpid = (array) $this->trataResposta( $arrResp );

		
//		// Deleta Respostas de itens desmarcadas
//		foreach( $arrResp as $resp ){
//			if ( !in_array($resp->itpid, $itens) ){
//				// Busca pergunta filha
//				$arrObSubPerg = (array) $this->p->listaObjeto(array("itpid" => $resp->itpid));
//				// Caso não tenha pergunta filha
//				if ( !$arrObSubPerg ){
//					// Busca Grupo filho
//					$obGrupo = $this->g->pegaUmObjeto(array("itpid" => $resp->itpid));
//					// Caso encontre
//					if ( get_class($obGrupo) == 'Grupo' ){
//						// Vai para o último nível de grupo
//						$obGrupo = $this->ultimoNivelGrupo($obGrupo);
//						// Busca as perguntas associadas ao grupo de último nível
//						$arrObSubPerg = (array) $this->p->listaObjeto(array($obGrupo));
//					}
//				}
//				foreach( $arrObSubPerg as $obSubPerg ){
//					$arrResp = $this->r->listaObjeto(array($obSubPerg));
//					if ($arrResp){
//						foreach($arrResp as $subResp){
//							$subResp->excluir();
//							$subResp->commit();
//						}
//					}
//				}
//				$resp->excluir();
//				$resp->commit();
//			}
//		}

		// Deleta SubPerguntas Desmarcadas
		$this->deletaRespSubPerguntaTotalmenteDesmarcada($p);
		
		// Deleta Resposta de Itens desmarcados
		$this->deletaRespItensDesmarcados($p, $itens);
		
		// Insere Novos itens
		foreach( $itens as $item ){
			if ( !in_array($item, $arrItpid) ){
				$obResp = new QResposta();
				$arCampos = array("perid", "qrpid", "itpid", "usucpf");
				$arDados  = array("perid" => $p->perid, "qrpid" => $this->qrpid, "itpid" => $item, "usucpf" => $_SESSION['usucpf']);
				$obResp->popularObjeto( $arCampos, $arDados );
				$obResp->salvar();
				$obResp->commit();
//				$ultimoItem = $item;
			}
		}
		return true;
	}
	
	private function salvarText( QPergunta $p, $txt ){
		$obResp = $this->r->pegaUmObjeto(array($p, "qrpid" => $this->qrpid));
		if ( get_class($obResp) != 'QResposta' ){
			$arCampos = array("perid", "qrpid", "resdsc", "usucpf");
			$arDados  = array("perid" => $p->perid, "qrpid" => $this->qrpid, "resdsc" => iconv("UTF-8", "ISO-8859-1", $txt), "usucpf" => $_SESSION['usucpf']);
			$obResp = new QResposta();
		}else{
			$arCampos = array("perid", "resdsc", "usucpf");
			$arDados  = array("perid" => $p->perid, "resdsc" => iconv("UTF-8", "ISO-8859-1", $txt), "usucpf" => $_SESSION['usucpf']);
		}
		
		$obResp->popularObjeto( $arCampos, $arDados );
		$obResp->salvar();
		$obResp->commit();
	}
	
	private function deletaRespSubPerguntaTotalmenteDesmarcada( QPergunta $p ){
		// Dados do formulario (POST)
		$dados 		   = $this->dadosForm;
		$dados['perg'] = $this->limpaArrDadosNull($dados['perg']);
		
		// Busca Itens da pergunta
		$arrItens = $this->i->carregarColuna(array($p));
		// Busca as perguntas associadas aos itens da pergunta
		$arrPergPossivel = (array) $this->p->carregarColuna(array("itpid" => $arrItens, 'perid'));
		// Busca Grupo(S) associados aos itens
		$arrObGrupo = (array) $this->g->listaObjeto(array("itpid" => $arrItens));
		foreach($arrObGrupo as $obGrupo){
			if ( get_class($obGrupo) == 'QGrupo' ){
				$obGrupo = $this->ultimoNivelGrupo($obGrupo);
				// Busca as perguntas associadas ao grupo de último nível 
				$arrPergPossivelGrupo = (array) $this->p->carregarColuna(array($obGrupo));
				$arrPergPossivel = array_merge($arrPergPossivel, $arrPergPossivelGrupo);
			}
		}
		// Pega Array de Perguntas que não estão no POST			
		$arrPergLimpa = array_diff($arrPergPossivel, array_keys($dados['perg']));
		
		foreach($arrPergLimpa as $pergLimpa){
			// Busca QResposta(S) da Pergunta
			//AGORA PASSO O QRPID PARA APAGAR SOMENTE DAQUELA RESPOSTA!! VICTOR BENZI - 26/08/2010
			$arrObRespLimpa = (array) $this->r->listaObjeto(array("perid" => $pergLimpa, "qrpid" => $dados['qrpid'])); 
			foreach($arrObRespLimpa as $obRespLimpa){
				$respDel = new QResposta($obRespLimpa->resid);
				// Exclui Resposta
				$respDel->excluir();
				$respDel->commit();
			}
			
		}
		// Se existir perguntas há serem deletadas
		if ( $arrPergLimpa ){
			// Busca Itens das perguntas
			$arrItpid = $this->i->carregarColuna( array("perid" => $arrPergLimpa) , 'itpid');
			// Se existir itens, envia os itens para deletar suas perguntas de sub-Nivel
			if ( $arrItpid ){
				$this->deletaRespSubPerguntaNivel( $arrItpid );
			}
		}
	}
	
	private function deletaRespItensDesmarcados(QPergunta $p, $itens){
		$arrObResp = (array) $this->carregaResposta($p);
		$arrItpid  = (array) $this->trataResposta( $arrResp );
		
		foreach( $arrObResp as $obResp ){
			if ( in_array($obResp->itpid, $itens) ){ continue; }
			
			// Deleta todos os NÍVEIS de subPerguntas associadas a este item (desmarcado)
			$this->deletaRespSubPerguntaNivel( $obResp->itpid );
			
			$obResp->excluir();
			$obResp->commit();
		}
	}
	
	private function deletaRespSubPerguntaNivel( $itens ){
		$itens = (array) $itens;
		foreach($itens as $item){
			// Busca pergunta filha
			$arrObSubPerg = (array) $this->p->listaObjeto(array("itpid" => $item));	
//			foreach($arrObSubPerg as $obSubPerg){
//				// Busca Resposta
//				$arrObResp = (array) $this->carregaResposta( $obSubPerg );
//				// Loop nas respostas
//				foreach ($arrObResp as $obResp){
//					// Deleta Resposta
//					$obResp->excluir();
//					$obResp->commit();
//				}
//			}
			
			// Busca Grupo filho
			$arrObGrupo = (array) $this->g->listaObjeto(array("itpid" => $item));
			foreach ($arrObGrupo as $obGrupo){
				// Busca as perguntas associadas ao grupo
				$arrObSubPerg2 = (array) $this->p->listaObjeto(array($obGrupo));
				
				$arrObSubPerg = array_merge($arrObSubPerg, $arrObSubPerg2);
//				foreach($arrObSubPerg2 as $obSubPerg2){
//					// Busca Resposta
//					$arrObResp = (array) $this->carregaResposta( $obSubPerg2 );
//					// Loop nas respostas
//					foreach ($arrObResp as $obResp){
//						// Deleta Resposta
//						$obResp->excluir();
//						$obResp->commit();
//					}
//				}
			}
			foreach($arrObSubPerg as $obSubPerg){
				// Busca Resposta
				$arrObResp = (array) $this->carregaResposta( $obSubPerg );
				// Loop nas respostas
				foreach ($arrObResp as $obResp){
					// Deleta Resposta
					$obResp->excluir();
					$obResp->commit();
				}
				$arrItpid = $this->i->carregarColuna(array($obSubPerg), 'itpid');
				if ( $arrItpid ){
					$this->deletaRespSubPerguntaNivel( $arrItpid );
				}
			}
		}		
	}
	
	function limpaArrDadosNull( Array $dados){
		$arrDado = array();
		foreach ($dados as $indice => $valor){
			if ($valor){
				$arrDado[$indice] = $valor;	
			}
		}
		return $arrDado;
	}

	private function primeiraPerid( Array $param){
		if ( key($param) == "queid" ){
			$param['perordem'] = 1;
			$perid = $this->p->carregaUm($param);
			if ( !$perid ){
				$param["grpordem"] = 1;
				$grpid = $this->g->carregaUm( $param );
				$param = array("grpid" => $grpid);
				
				return self::primeiraPerid( $param );
			}	
		}else{
			$param['perordem'] = 1;
			$perid = $this->p->carregaUm( $param );
			if ( !$perid ){
				$param = array(
								"gru_grpid" => $param['grpid'],
								"grpordem"	=> 1
							  );
				$grpid = $this->g->carregaUm( $param );
				$param = array("grpid" => $grpid);
				
				return self::primeiraPerid( $param );
			}
		}
		return $perid;
	}		
	

	static function insereQuestionario( Array $arParam ){
		include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionarioResposta.class.inc";
		
		$obQr = new QQuestionarioResposta();
		$arCampos = array("queid", "qrptitulo", "qrpdata");
		$arDados  = array("queid" => $arParam['queid'], "qrptitulo" => $arParam['titulo'], "qrpdata" => 'now()');
		
		$obQr->popularObjeto( $arCampos, $arDados );
		$qrpid = $obQr->salvar();
		$obQr->commit();
		return $qrpid;
	}
	
	public function desabilitaQuestionario( $param ){

		//Caso ja tenha passado o periodo para preenchimento do questionário uma mensagem será enviada alertando o usuário.
		if( !(substr($this->getQueDtFim(),0,10) >= Date("Y-m-d") && substr($this->getQueDtInicio(),0,10) <= Date("Y-m-d")) ){
			$mensagem =  "O questionário só está disponível entre os dias ".date('d/m/Y', strtotime(substr($this->getQueDtInicio(),0,10)))." e ".date('d/m/Y', strtotime(substr($this->getQueDtFim(),0,10)));
			echo "<script>alert('".$mensagem ."');</script>";
			return false;
		} elseif( $param == 'N' ){
			return false;
		} else {
			return true;
		}
	}
		
}
?>