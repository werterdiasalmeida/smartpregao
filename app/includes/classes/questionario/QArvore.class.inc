<?php
class QArvore{
	private $g;
	private $q;
	private $p;
	private $s;
	private $it;
	public $arvore;
	private $tipoArvore;
	private $img;
	private $questionario;
    private $perid;
    private $qrpid;
                
	public function __construct(Array $param = null){
		$this->s  = new Sistema();
		$this->q  = new QQuestionario();
		$this->g  = new QGrupo();
		$this->p  = new QPergunta();
		$this->it = new QItempergunta();
 
		$this->perid 	 = $param['perid'];
		$this->qrpid 	 = $param['qrpid'];
		$this->relatorio = $param['relatorio'];
		
		if ($param['questionario']){
			$this->questionario = $param['questionario'];
			self::detalheTipoArvore('resposta');
		}else{
			self::detalheTipoArvore('cadastro');
		}                              
	}
                
	function detalheTipoArvore($tipoArvore = 'cadastro'){
		$this->tipoArvore = $tipoArvore;
                               
		if ( $tipoArvore == 'cadastro' ){
			// Sistema
			$this->img['s']['open']	   				= '../includes/dtree/img/globe.gif'; 
			$this->img['s']['close']   				= '../includes/dtree/img/globe.gif';
			$this->img['s']['incluirQ']             = '<img src=\'/imagens/gif_inclui.gif\' title=\'Incluir novo "Questionário"\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroQuestionario&acao=A&sisid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=600,height=400\'); w.focus();">'; 
			$this->img['s']['alterar']              = '';
			$this->img['s']['excluir']              = '';
			// Questionario
			$this->img['q']['open']                 = '../includes/dtree/img/base.gif'; 
			$this->img['q']['close']  				= '../includes/dtree/img/base.gif';
			$this->img['q']['incluirP']             = '<img src=\'/imagens/gif_inclui.gif\' title=\'Incluir nova "Pergunta"\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&queid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=530,height=550\'); w.focus();">';
			$this->img['q']['incluirG']             = '<img src=\'/imagens/gif_inclui.gif\' title=\'Incluir novo "Grupo de Pergunta"\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroGrupo&acao=A&queid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=480,height=260\'); w.focus();">';
			$this->img['q']['alterar']              = '<img src=\'/imagens/alterar.gif\' title=\'Alterar Questionário\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroQuestionario&acao=A&queid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=600,height=400\'); w.focus();">';
			$this->img['q']['excluir']              = '<img src=\'/imagens/excluir.gif\' title=\'Excluir Questionário\' onClick="apagar(\'%s&instrucao=excluir&queid=%s\', \'Deseja apagar o Questionário ( %s )?\') "/>';
			$this->img['q']['gera']                 = '<img src=\'/imagens/report.gif\' title=\'Gerar Script de Questionário\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/geraScript&acao=A&queid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=800,height=600\'); w.focus();">';
			// Grupo
			$this->img['g']['open']                 = '../includes/dtree/img/folderopen.gif';
			$this->img['g']['close']  				= '../includes/dtree/img/folder.gif';
			$this->img['g']['incluirP']             = '<img src=\'/imagens/gif_inclui.gif\' title=\'Incluir nova "Pergunta"\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&grpid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=530,height=550\'); w.focus();">';
			$this->img['g']['incluirG']             = '<img src=\'/imagens/gif_inclui.gif\' title=\'Incluir novo "Grupo de Pergunta"\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroGrupo&acao=A&gru_grpid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=480,height=260\'); w.focus();">';
			$this->img['g']['alterar']              = '<img src=\'/imagens/alterar.gif\' title=\'Alterar Grupo\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroGrupo&acao=A&grpid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=480,height=260\'); w.focus();">';
			$this->img['g']['excluir']              = '<img src=\'/imagens/excluir.gif\' title=\'Excluir Grupo\' onClick="apagar(\'%s&instrucao=excluir&grpid=%s\', \'Deseja apagar o Grupo ( %s )?\') "/>';
			// Pergunta
			$this->img['p']['open']                 = '../includes/dtree/img/page.gif';
			$this->img['p']['close']  				= '../includes/dtree/img/page.gif';
			$this->img['p']['incluir']              = '';
			$this->img['p']['alterar']              = '<img src=\'/imagens/alterar.gif\' title=\'Alterar Pergunta\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&perid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=530,height=550\'); w.focus();">';
			$this->img['p']['excluir']              = '<img src=\'/imagens/excluir.gif\' title=\'Excluir Pergunta\' onClick="apagar(\'%s&instrucao=excluir&perid=%s\', \'Deseja apagar a Pergunta ( %s )?\') "/>';
			// Item
			$this->img['i']['open']                 = '../imagens/ico_config.gif';
			$this->img['i']['close']    			= '../imagens/ico_config.gif';
			$this->img['i']['incluirP']             = '<img src=\'/imagens/gif_inclui.gif\' title=\'Incluir nova "Pergunta"\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&itpid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=530,height=550\'); w.focus();">';
			$this->img['i']['incluirG']             = '<img src=\'/imagens/gif_inclui.gif\' title=\'Incluir novo "Grupo de Pergunta"\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroGrupo&acao=A&itpid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=480,height=260\'); w.focus();">';
			$this->img['i']['alterar'] 				= '';
			$this->img['i']['excluir'] 				= '';
		}else{
			// Sistema
			$this->img['s']['open']                 = '../includes/dtree/img/globe.gif'; 
			$this->img['s']['close']   				= '../includes/dtree/img/globe.gif';
			$this->img['s']['incluirQ']             = ''; 
			$this->img['s']['alterar']              = '';
			$this->img['s']['excluir']              = '';
			// Questionario
			$this->img['q']['open']                 = '../includes/dtree/img/base.gif'; 
			$this->img['q']['close']  				= '../includes/dtree/img/base.gif';
			$this->img['q']['incluirP']             = '';
			$this->img['q']['incluirG']             = '';
			$this->img['q']['alterar']              = '';
			$this->img['q']['excluir']              = '';
			$this->img['q']['imprime']              = '../imagens/print.png';
			// Grupo
			$this->img['g']['open']                 = '../includes/dtree/img/folderopen.gif';
			$this->img['g']['close']  				= '../includes/dtree/img/folder.gif';
			$this->img['g']['incluirP']             = '';
			$this->img['g']['incluirG']             = '';
			$this->img['g']['alterar']              = '';
			$this->img['g']['excluir']              = '';
			// Pergunta
			$this->img['p']['open']                 = '../includes/dtree/img/page.gif';
			$this->img['p']['close']  				= '../includes/dtree/img/page.gif';
			$this->img['p']['respondido']     		= '<img src=\'/imagens/check_p.gif\' title=\'Pergunta respondida\'>';
			$this->img['p']['incluir']              = '';
			$this->img['p']['alterar']              = '';
			$this->img['p']['excluir']              = '';
			// Item
			$this->img['i']['open']                 = '../imagens/ico_config.gif';
			$this->img['i']['close']    			= '../imagens/ico_config.gif';
			$this->img['i']['incluirP']             = '';
			$this->img['i']['incluirG']             = '';
			$this->img['i']['alterar'] 				= '';
			$this->img['i']['excluir'] 				= '';
		}              
	}
	
	function limitaString( $titulo ){
		if(strlen($titulo) > 60){
			$titulo = substr($titulo,0,60) . '...';
		}
		return $titulo;
	}
	
	function quebraTitulo( $titulo ){
		$titulo = wordwrap($titulo, 60, "<br />");
		return $titulo;
	}
                
	function montaArvore( $sisid = null ){
    	if ($this->tipoArvore == 'cadastro'){
        	$this->arvore .= "arvore.add(0, -1, 'Questionários');\n";                             
            $arrObSistema  = $this->s->listaObjeto(array('sisid' => $sisid), array('sisid', 'sisdsc'));
                     
            foreach($arrObSistema as $obSistema){
				$this->arvore .= "arvore.add('sis_" . $obSistema->sisid . "', 0, '".$obSistema->sisdsc."', 'javascript:void(0)', '', '', '".$this->img['s']['open']."', '".$this->img['s']['close']."');\n";
				// Busca Questionarios associados
				$imgInclui = $this->img['s']['incluirQ'] ? sprintf($this->img['s']['incluirQ'], $obSistema->sisid) : '';
				$imgInclui = addslashes($imgInclui);
				$this->arvore .= "arvore.add('sis_imgInclui_" . $obSistema->sisid . "', 'sis_" . $obSistema->sisid . "', '" . $imgInclui . " Adicionar Questionário', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
				self::montaArvoreQ($obSistema);                                         
			}
		}else{
			self::montaArvoreQ( $this->questionario );                                      
		}
		return	'<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
				<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
				<script>
				arvore = new dTree( "arvore" );
				
				arvore.config.folderLinks = true;
				arvore.config.useIcons = true;
				arvore.config.useCookies = true; 
				
				'.$this->arvore.'
				
				elemento = document.getElementById( "_arvore" );
				elemento.innerHTML = arvore;
				
				 /*
				 * adaptação de largura e controle de overflow de conteúdo para o internet explorer
				 */
				var dav = navigator.appVersion;
				IE = document.all ? true : false;
				IE6 = dav.indexOf( "MSIE 6.0" ) >= 0;
				IE7 = dav.indexOf( "MSIE 7.0" ) >= 0;
				if ( IE ) {
					width = document.body.offsetWidth;
					height = document.body.offsetHeight;
					document.getElementById( "bloco" ).style.width = ( width * 0.80 ) + "px";
				}	
				arvore.openTo("per_'.$this->perid.'", true, false);
				</script>
				<script>
				function apagar (url,mens) {
					if (confirm(mens)) {
						location.href = url;
						return true;
					}
					return false;
				}
				function imprimeQ(queid){
					var url = String(document.location).split(\'?\');
					window.open(url[0]+"?'.$this->relatorio.'&imprime=true&queid="+queid+"&qrpid='.$this->qrpid.'","page","toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=600");
				}
				</script>';
                }
                
	function montaArvoreQ( $obPai ){
		// Filtra Questionarios
		$arrObQuestionario = (array) $this->q->listaObjeto( array($obPai), array('queid', 'quetitulo') );		
		foreach($arrObQuestionario as $obQuestionario){
			$queid = $obQuestionario->queid;
			if ($this->tipoArvore == 'cadastro'){
				$idPai = "sis_" . $obPai->sisid;
				//Perguntas
				$imgInclui = $this->img['q']['incluirP'] ? sprintf($this->img['q']['incluirP'], $obQuestionario->queid) : '';
				$imgInclui = addslashes($imgInclui);
				$addP = "arvore.add('que_imgInclui_per_" . $obQuestionario->queid . "', 'que_" . $obQuestionario->queid . "', '" . $imgInclui . " Adicionar Pergunta', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
				//Grupos
				$imgInclui = $this->img['q']['incluirG'] ? sprintf($this->img['q']['incluirG'], $obQuestionario->queid) : '';
				$imgInclui = addslashes($imgInclui);
				$addG = "arvore.add('que_imgInclui_gru_" . $obQuestionario->queid . "', 'que_" . (($obQuestionario->queid)) . "', '" . $imgInclui . " Adicionar Grupo de Pergunta', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
			}else{
				$idPai = "-1";
			}
			$imgGeraScript = $this->img['q']['gera'] ? sprintf($this->img['q']['gera'], $obQuestionario->queid) : '';
			$imgGeraScript = addslashes($imgGeraScript);
			$imgAltera = $this->img['q']['alterar'] ? sprintf($this->img['q']['alterar'], $obQuestionario->queid) : '';
			$imgAltera = addslashes($imgAltera);
			$imgExclui = $this->img['q']['excluir'] ? sprintf($this->img['q']['excluir'], $_SERVER['REQUEST_URI'], $obQuestionario->queid, $obQuestionario->quetitulo) : '';
			$imgExclui = addslashes($imgExclui);
			$this->arvore .= "arvore.add('que_" . $obQuestionario->queid . "', '$idPai', '$imgGeraScript $imgAltera $imgExclui " . self::limitaString($obQuestionario->quetitulo) . "', 'javascript:void(0)', '', '', '".$this->img['q']['open']."', '".$this->img['q']['close']."');\n";
											
			// Busca Perguntas associadas
			$this->arvore .= $addP;
			self::montaArvoreP($obQuestionario);
			
			// Busca Grupo Pergunta associadas
			
			$this->arvore .= $addG;
			self::montaArvoreG($obQuestionario);
		}
		if( $this->relatorio != 'N' && $this->tipoArvore != 'cadastro' ){
			$this->arvore .= "arvore.add('imprimir', '$idPai', 'Imprimir Questionário', 'javascript:imprimeQ(".$queid.")', '', '', '".$this->img['q']['imprime']."', '".$this->img['q']['imprime']."');\n";
		}
	}
	
	function montaArvoreG( $obPai ){
		if (get_class($obPai) == 'QQuestionario'){
			$idPai = 'que_' . $obPai->queid;
		}elseif (get_class($obPai) == 'QGrupo'){
			$idPai = 'grp_' . $obPai->grpid;
		}elseif (get_class($obPai) == 'QItemPergunta'){
			$idPai = 'itp_' . $obPai->itpid;
		}else{
			die('Erro: montaArvoreG falta OBJETO do parametro $obPai.');
		}	
		// Filtra Grupos vinculadas
		$arrObGrupoPergunta = (array) $this->g->listaObjeto(array( $obPai ), array("grpid, grptitulo, grpordem"));
		foreach ($arrObGrupoPergunta as $obGrupoPergunta){
			if ($this->tipoArvore == 'cadastro'){
				//Perguntas
				$imgInclui = $this->img['g']['incluirP'] ? sprintf($this->img['g']['incluirP'], $obGrupoPergunta->grpid) : '';
				$imgInclui = addslashes($imgInclui);
				$addP = "arvore.add('grp_imgInclui_per_" . $obGrupoPergunta->grpid . "', 'grp_" . $obGrupoPergunta->grpid . "', '" . $imgInclui . " Adicionar Pergunta', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
				//Grupos
				$imgInclui = $this->img['g']['incluirG'] ? sprintf($this->img['g']['incluirG'], $obGrupoPergunta->grpid) : '';
				$imgInclui = addslashes($imgInclui);
				$addG = "arvore.add('grp_imgInclui_gru_" . $obGrupoPergunta->grpid . "', 'grp_" . $obGrupoPergunta->grpid . "', '" . $imgInclui . " Adicionar Grupo de Pergunta', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
			}
			$imgAltera = $this->img['g']['alterar'] ? sprintf($this->img['g']['alterar'], $obGrupoPergunta->grpid) : '';
			$imgAltera = addslashes($imgAltera);
			$imgExclui = $this->img['g']['excluir'] ? sprintf($this->img['g']['excluir'], $_SERVER['REQUEST_URI'], $obGrupoPergunta->grpid, $obGrupoPergunta->grptitulo) : '';
			$imgExclui = addslashes($imgExclui);
			$this->arvore .= "arvore.add('grp_" . $obGrupoPergunta->grpid . "', '{$idPai}', '$imgAltera $imgExclui " . self::limitaString($obGrupoPergunta->grptitulo) . "', 'javascript:void(0)', '', '', '". $this->img['g']['close'] ."', '". $this->img['g']['open'] ."');\n";
					
			// Busca Perguntas associadas
			$this->arvore .= $addP;
			self::montaArvoreP($obGrupoPergunta);
			
			// Busca Grupo Pergunta associadas
			if(!(get_class($obPai) == 'ItemPergunta')){//Se for filho de um item então não inclui mais grupos!
				$this->arvore .= $addG;
				self::montaArvoreG($obGrupoPergunta);
			}
			
		}	
		
	}
	
	function montaArvoreP( $obPai ){
		
		if (get_class($obPai) == 'QQuestionario'){
			$idPai = 'que_' . $obPai->queid;
		}elseif (get_class($obPai) == 'QGrupo'){
			$idPai = 'grp_' . $obPai->grpid;
		}elseif (get_class($obPai) == 'QItemPergunta'){
			$idPai = 'itp_' . $obPai->itpid;
		}else{
			die('Erro: MontaArvoreP falta OBJETO do parametro $obPai.');
		}
		// Filtra perguntas vinculadas
		$arrObPergunta = (array) $this->p->listaObjeto(array( $obPai ), array("perid, pertitulo, perordem"));
		foreach ($arrObPergunta as $obPergunta){
			$imgAltera = $this->img['p']['alterar'] ? sprintf($this->img['p']['alterar'], $obPergunta->perid) : '';
			$imgAltera = addslashes($imgAltera);
			$imgExclui = $this->img['p']['excluir'] ? sprintf($this->img['p']['excluir'], $_SERVER['REQUEST_URI'], $obPergunta->perid, $obPergunta->pertitulo) : '';
			$imgExclui = addslashes($imgExclui);
			if($this->tipoArvore == 'resposta'){
				$arrPerguntaResp = $this->p->perguntaRespondida($obPergunta->perid, $this->qrpid);
				if($arrPerguntaResp){
					$imgResp = $this->img['p']['respondido'];
					$imgResp = addslashes($imgResp);
				} else {
					$imgResp = '';
				}
				if($this->perid == $obPergunta->perid){
					$negrito = "<b>";
					$negritoF = "</b>";
				}
				$this->arvore .= "arvore.add('per_" . $obPergunta->perid . "', '{$idPai}', '$imgResp $imgAltera $imgExclui $negrito" . self::limitaString($obPergunta->pertitulo) . "$negritoF', 'javascript: quest.atualizaTela(" . $obPergunta->perid . ");', 'Clique para abrir a pergunta', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
				$negrito = "";
				$negritoF = "";
				$arrItemResp = $this->p->perguntaRespondidaComItem( $obPergunta->perid, $this->qrpid );
				if($arrItemResp){
					self::montaArvoreI($obPergunta, $arrItemResp);
				}
			} else { 
				$this->arvore .= "arvore.add('per_" . $obPergunta->perid . "', '{$idPai}', '$imgAltera $imgExclui " . self::limitaString($obPergunta->pertitulo) . "', 'javascript:void(0)', '', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
				self::montaArvoreI($obPergunta);
			}
		}
	}
	
	function montaArvoreI( $obPai, $arrItemResp = null ){
		
		if (get_class($obPai) == 'QPergunta'){
			$idPai = 'per_' . $obPai->perid;
		}else{
			die('Erro: MontaArvoreI falta OBJETO do parametro $obPai.');
		}
		$where[0] = $obPai;
		if($arrItemResp[0]){
			$where['itpid'] = $arrItemResp;
		}
		// Filtra perguntas vinculadas
		$arrObItPergunta = (array) $this->it->listaObjeto($where, array("itpid, itptitulo"));
		foreach ($arrObItPergunta as $obItPergunta){
			if ($this->tipoArvore == 'cadastro'){
				//Perguntas
				$imgInclui = $this->img['i']['incluirP'] ? sprintf($this->img['i']['incluirP'], $obItPergunta->itpid) : '';
				$imgInclui = addslashes($imgInclui);
				$addP = "arvore.add('itp_imgInclui_per_" . $obItPergunta->itpid . "', 'itp_" . $obItPergunta->itpid . "', '" . $imgInclui . " Adicionar Pergunta', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
				//Grupos
				$imgInclui = $this->img['i']['incluirG'] ? sprintf($this->img['i']['incluirG'], $obItPergunta->itpid) : '';
				$imgInclui = addslashes($imgInclui);
				$addG = "arvore.add('itp_imgInclui_gru_" . $obItPergunta->itpid . "', 'itp_" . $obItPergunta->itpid . "', '" . $imgInclui . " Adicionar Grupo de Pergunta', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
			}
			$this->arvore .= "arvore.add('itp_" . $obItPergunta->itpid . "', '{$idPai}', '$imgAltera $imgExclui " . self::limitaString($obItPergunta->itptitulo) . "', 'javascript:void(0)', '', '', '". $this->img['i']['close'] ."', '". $this->img['i']['open'] ."');\n";
			
			// Busca Perguntas associadas
			$verifica = $this->it->verificaGrupo( $obItPergunta->itpid );
			if(!($verifica)){
				$this->arvore .= $addP;
				self::montaArvoreP($obItPergunta);
			}

			// Busca Grupo Pergunta associadas
			unset( $verifica );
			$verifica = $this->it->verificaPergunta( $obItPergunta->itpid );
			if(!($verifica)){
				$this->arvore .= $addG;
				self::montaArvoreG($obItPergunta);
			}			
		}
	}
}