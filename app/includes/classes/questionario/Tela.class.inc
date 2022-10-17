<?php
class Tela{
	private $ger;
	private $js;
	private $table;
	private $styleParam;
	private $tableQuestionario;
	private $cellQuestionario;
	private $cellRodape;
	private $cellArvore;
	private $nivel;
	private $itemPergunta;
	private $tipoTela;
	private $subPergunta;
	private $arrDivSubPergunta;
	
	/*
	 * Monta a Tela principal do questionário
	 *
	 * Includes necessários: include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
	 * 						 include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
	 * Exemplo de como montá-lo: $tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'habilitado' => 'S') );
	 * 
	 * @param $arrParam com os seguintes parametros que podem ou não ser passados:
	 * qrpid 		- único parametro obrigatório. É o identificador de uma determinada resposta ao questionário. Cada entidade de resposta individual terá um. Ex: "qrpid" => $qrpid (qrpid atribuido anteriormente)
	 * tamDivArvore - Tamanho horizontal da árvore do questionário em porcentagem. Ex: 'tamDivArvore' => 25 (25 %)
	 * tamDivPx 	- Tamanho horizontal da árvore do questionário em pixels. 'tamDivPx' => 400 (400 px)
	 * habilitado	- Habilita/Desabilita o questionário. Precisa de parametro 'S' ou 'N' para habilitado e desabilitado respectivamente. Ex: 'habilitado' => 'S' (habilitado)
	 * relatorio	- Habilita/Desabilita a impressão de relatórios do questionário. Como parametro é passado a pagina onde o relatório será impresso a partir do modulo. Ex: 'relatorio' => 'modulo=relatorioQuestionario&acao=A'
	 * 
	 * TODOS OS PARAMETROS, A NÃO SER O QRPID, SÃO OPCIONAIS. 
	 * 
	 */
	
	function __construct(Array $arrParam){
		
		include_once APPRAIZ . "includes/classes/html/TElement.class.inc";
		include_once APPRAIZ . "includes/classes/html/TTable.class.inc";
		include_once APPRAIZ . "includes/classes/html/TTableRow.class.inc";
		include_once APPRAIZ . "includes/classes/html/TTableCell.class.inc";
		include_once APPRAIZ . "includes/classes/html/TField.class.inc";
		include_once APPRAIZ . "includes/classes/html/TCombo.class.inc";
		include_once APPRAIZ . "includes/classes/html/TTextArea.class.inc";
		include_once APPRAIZ . "includes/classes/html/TText.class.inc";
		include_once APPRAIZ . "includes/classes/html/TCheckGroup.class.inc";
		include_once APPRAIZ . "includes/classes/html/TCheckButton.class.inc";
		include_once APPRAIZ . "includes/classes/html/TRadioGroup.class.inc";
		include_once APPRAIZ . "includes/classes/html/TRadioButton.class.inc";
		include_once APPRAIZ . "includes/classes/html/TData.class.inc";
		include_once APPRAIZ . "includes/classes/html/TLabel.class.inc";
		include_once APPRAIZ . "includes/classes/html/FormularioQuestionario.class.inc";
		
		include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
		include_once APPRAIZ . "includes/classes/questionario/QArvore.class.inc";
		include_once APPRAIZ . "includes/classes/questionario/JavaScript.class.inc";

		if ( $_POST['ajax']){
			ob_clean();
		}
		
		$arrParam['qrpid'] = $_POST['qrpid'] ? $_POST['qrpid'] : $arrParam['qrpid'];
		$arrParam['queid'] = $_POST['queid'] ? $_POST['queid'] : $arrParam['queid']; 
		$arrParam['perid'] = $_POST['perid'] ? $_POST['perid'] : $arrParam['perid'];
		$arrParam['itpid'] = $_POST['itpid'] ? $_POST['itpid'] : $arrParam['itpid'];
		
		$this->styleParam['tamDivArvore'] = $arrParam['tamDivArvore'] ? $arrParam['tamDivArvore'] : 30;
		$this->styleParam['tamDivPx'] 	  = $arrParam['tamDivPx'] ? $arrParam['tamDivPx'] : 400;
		$this->relatorio = $arrParam['relatorio'] ? $arrParam['relatorio'] : 'N';
		
		if ($arrParam['itpid']){
			$this->tipoTela = "questionario";
		}
		
		if ($_POST['salvar_questionario']){
//			ob_clean();
			$arrParamSalv 						 = $arrParam;
			$arrParamSalv['salvar_questionario'] = $_POST['salvar_questionario'];
			$obQ = new GerenciaQuestionario($arrParamSalv);
			$obQ->salvar($_POST);
		}
		$this->ger 	 = new GerenciaQuestionario($arrParam);
		$this->arv 	 = new QArvore( array("questionario" => $this->ger->getQuestionario(), "perid" => $this->ger->getPeridPai(), "qrpid" => $arrParam['qrpid'], "relatorio" => $this->relatorio) );
		$this->js 	 = new JavaScript( $this->ger );
		
		$this->nivel 		= 0;
		$this->itemPergunta = array();
		
		$this->desab = $this->ger->desabilitaQuestionario( $arrParam['habilitado'] );

		if ( $this->tipoTela != "questionario" ){
			self::montaTela();
		}else{
			self::montaTelaQuestionario();
		}
		if ( $_POST['ajax'] ){
			ob_flush();
			ob_end_clean();
			exit;			
		}
		
//		die;
	}
	
	private function imgNivel(){
		$img 		= new TElement( 'img' );
		$img->style = 'padding-left:' . $this->nivel * 10 . 'px;';
		$img->src 	= '../imagens/seta_filho.gif';
		$img->align = 'absmiddle';
		return $img;
	}
	

	private function imgAjuda( $ajuda ){
//		$ajuda = htmlspecialchars_decode(str_replace(array("\n", "\r", "\t"), "", nl2br($ajuda)), ENT_QUOTES);
		
		$img 			  = new TElement( 'img' );
		$img->src 		  = '/imagens/ajuda.gif';
		$img->title 	  = $ajuda;
//		$img->onmouseover = "return escape('" . addslashes( $ajuda ) . "')";
		return $img;
	}
	
	function montaTelaQuestionario(){
		$this->cellQuestionario = new TElement("div");
		self::questionario();
		
		// Gera linha do javascript
		$row 		= $this->tableQuestionario->addRow();		
		$row->style = "display : none;";
		// Monta o JavaScript que insere a sub-pergunta na class Pergunta => (JavaScript).	
		$this->js->notInclude();
		$this->js->notFonte();
		$this->js->notParamJs();
//		$this->js->atualizaPergunta();
		$row->addCell( $this->js );
		
		$this->cellQuestionario->show();
	}
	
	function montaTela(){
		$this->table   = new TTable;
		$tela 		   = $this->table;
		
		if ( !$_POST['ajax'] ){
			$tela 		   = new FormularioQuestionario();
			$div = new TElement('div');
			$div->id = 'telacentral';
			$div->add( $this->table );
			$tela->add( $div );
//			$tela->add( $form );
		}
		$this->table->border = 0;
		$this->table->height = 500;
		$row  		   		 = $this->table->addRow();
		// Gera célula da árvore
		$cell 			  = $row->addCell('');
		$cell->id 		  = 'telaarvore'; 
//		$cell->style 	  = 'width:30%;'; 
		$cell->rowspan 	  = 2; 
		$this->cellArvore = $cell; 
		// Gera célula do questionario
		$cell 		  			= $row->addCell(''); 
		$cell->id 	  			= 'telaquestionario';
		$cell->align  			= "center";
		$cell->valign 			= "top";
		$this->cellQuestionario = $cell; 
		// Gera célula do rodapé
		$row  		   	  = $this->table->addRow();
		$cell 		  	  = $row->addCell(''); 
		$cell->id 	  	  = 'telarodape';
		$cell->align  	  = "center";
		$cell->valign 	  = "bottom";
		$cell->style 	  = "padding-bottom:13px;";
		$this->cellRodape = $cell; 
		
		self::arvore();
		self::questionario();
		
		if ( $_POST['ajax'] ){
			$this->js->notInclude();
			$this->js->notFonte();
			$this->js->notParamJs();
			$this->js->atualizaPergunta();
			// Gera linha do javascript
			$row = $this->table->addRow();	
			$row->style = "display : none;";	
			$row->addCell( $this->js );
		}else{
			$tela->add( $this->js );
		}
		
		$tela->show();
	}


	private function arvore(){
		// conteúdo célula I 
		$fieldSet = new TElement("fieldset");
		$fieldSet->style = "overflow:auto; background:#FFF; height:510px%; width:".$this->styleParam['tamDivPx']."px;";
		$legend = new TElement("legend");
		$legend->add( $this->arv->quebraTitulo( $this->ger->getQueTitulo() ) );
		$fieldSet->add( $legend );
		
		// Div externa
		$dI 		  = new TElement('div');
		$dI->id    = "bloco";
		$dI->class = "dtree";
		$dI->style = "overflow:auto; text-align:left; height:500px; width:".$this->styleParam['tamDivPx']."px;";
//		$dI->style = "overflow:hidden; text-align:left; width:100px;";
		// Paragrafo Interno a div externa
		$p = new TElement('p');
		// Link I de dentro do paragrafo
		$aI = new TElement('a');
		$aI->href = 'javascript: arvore.openAll();';
		$aI->add('Abrir Todos');
		// Link II de dentro do paragrafo
		$aII = new TElement('a');
		$aII->href = 'javascript: arvore.closeAll();';
		$aII->add('Fechar Todos');
		// Div da árvore
		$dII 		  = new TElement('div');
		$dII->id    = "_arvore";
//		$dII->width = "100";
//		$dII->style = "overflow:auto; width:100px;";

		$p->add( $aI );
		$p->add(" &nbsp;|&nbsp; ");
		$p->add( $aII );
		
		$dI->add( $p );
		$dI->add( $dII );
		
		$fieldSet->add( $dI );
		$fieldSet->add( $this->arv->montaArvore() );
//		// Div javascript
//		$dJS = new TElement('div');
//		$dJS->style = "display:none;";
//		$dJS->add($this->arv->montaArvore());
		
		// célula I
		$cell = $this->cellArvore;
		$cell->add( $fieldSet );
//		$cell->add( $dJS );
		$cell->valign = 'top';
//		$cell->width  = '30%';
		$cell->align  = 'center';
		$cell->style  = 'overflow:auto; width:'.$this->styleParam['tamDivArvore'].'%;';
	}

	private function questionario(){
		$this->tableQuestionario = new TTable;
		$this->tableQuestionario->valign = 'top';
		$this->cellQuestionario->add( $this->tableQuestionario );
		
		$arrObDados = (array) $this->ger->getDados();
		foreach($arrObDados as $obDados){
			if ( method_exists($this, 'tela' . get_class( $obDados ) )){
				$this->{'tela' . get_class( $obDados ) }( $obDados );
				$this->nivel++;
			}
		}
		
		if ( $this->tipoTela != "questionario" ){
			self::questionarioRodape();
		}/*else{	
			$this->tableQuestionario->style = "width : 100%";
		}*/	
	}
		
	private function questionarioRodape(){
		
		$table = new TTable;
		$this->cellRodape->add($table);	
	
		// linha I
		$row 			= $table->addRow();
		$row->bgcolor   = '#cccccc';
		// célula I
		$cell = $row->addCell('');
		
		$pergAnterior = $this->ger->anteriorPergunta();
		$pergProxima  = $this->ger->proximaPergunta();
		
		// Div/Botão I
		$div = new TElement('div');
		$div->style = "float:left; width:30%; text-align: left;";
		//Botão
		$botao = new TElement('input');
		if ( $pergAnterior ){
			$botao->onclick = "javascript: quest.atualizaTela(" . $pergAnterior->perid . ");";
		}else{
			$botao->disabled = "disabled";
		}
		$botao->value = 'Anterior';
		$botao->type  = 'button';
		$div->add($botao);
		$cell->add($div);		
		
		// Div/Botão II
		$div = new TElement('div');
		$div->style = "float: left; width:40%; text-align: center;";
		//Botão
		$botao = new TElement('input');
		if ( $pergAnterior ){
			if( $this->desab == false ){
				$botao->disabled = "disabled";
			}else{
				$botao->onclick = "javascript: quest.salvar(" . $pergAnterior->perid . ");";
			}
		}else{
			$botao->disabled = "disabled";
		}
		$botao->value = 'Salvar Anterior';
		$botao->type  = 'button';
		$div->add($botao);
		
//		//Botão
//		$botao = new TElement('input');
//		$botao->onclick  = '';
//		$botao->name 	 = 'salvar_questionario';
//		$botao->value 	 = 'true';
//		$botao->type  	 = 'text';
////		$botao->onclick  = 'quest.salvar();';
//		$div->add($botao);
//
//		//Botão
//		$botao = new TElement('input');
//		$botao->onclick  = '';
//		$botao->name 	 = 'ajax';
//		$botao->value 	 = 'true';
//		$botao->type  	 = 'text';
////		$botao->onclick  = 'quest.salvar();';
//		$div->add($botao);
//					
//		//Botão
//		$botao = new TElement('input');
//		$botao->onclick  = '';
//		$botao->name 	 = 'perid';
//		$botao->value 	 = $this->ger->getPerid();
//		$botao->type  	 = 'text';
////		$botao->onclick  = 'quest.salvar();';
//		$div->add($botao);
		
		//Botão
		$botao = new TElement('input');
		$botao->onclick  = '';
		$botao->value 	 = 'Salvar';
//		$botao->type  	 = 'submit';
		$botao->type  	 = 'button';
		if( $this->desab == false ){
			$botao->disabled = "disabled";
		}else{
			$botao->onclick  = 'quest.salvar(' . $this->ger->getPeridPai() . ');';
		}
		$div->add($botao);
		
		//Botão
		$botao = new TElement('input');
		if ( $pergProxima ){
			if( $this->desab == false ){
				$botao->disabled = "disabled";
			}else{
				$botao->onclick = "javascript: quest.salvar(" . $pergProxima->perid . ");";
			}
		}else{
			$botao->disabled = "disabled";
		}		
		$botao->value = 'Salvar Próximo';
		$botao->type  = 'button';
		$div->add($botao);
		$cell->add($div);
				
		// Div/Botão III
		$div = new TElement('div');
		$div->style = "float:right; width:30%; text-align: right;";
		//Botão
		$botao = new TElement('input');
		if ( $pergProxima ){
			$botao->onclick = "javascript: quest.atualizaTela(" . $pergProxima->perid . ");";
		}else{
			$botao->disabled = "disabled";
		}
		$botao->value = 'Próximo';
		$botao->type  = 'button';
		$div->add($botao);
		$cell->add($div);		
		
	}
	
	private function telaQPergunta( QPergunta $obj){
		// Inclui pergunta ao JavaScript
		$pergPai = $this->ger->perguntaPai( array("perid" => $obj->perid) );
		$this->js->addPergunta($obj, ($pergPai instanceof QPergunta ? $pergPai->perid : "" ), $this->ger->itemPai($obj) );
		
		
		if ( $this->arrDivSubPergunta[$this->ger->itemPai($obj)] instanceof TElement ){
			$table = new TTable;
			$this->arrDivSubPergunta[$this->ger->itemPai($obj)]->style = "margin-top:5px;";
			$this->arrDivSubPergunta[$this->ger->itemPai($obj)]->add( $table );
		}else{
			$table = $this->tableQuestionario;
		}
		
		// linha I
		$row 			= $table->addRow();
		$cell 			= $row->addCell( $obj->pertitulo . ' ' );
		$cell->class	= "SubTituloEsquerda";
		$imagem 		= $obj->perajuda ? self::imgAjuda( $obj->perajuda ) : '';
		$cell->add( $imagem ); 
		// linha II
		$row 			= $table->addRow();
		
 		switch ($obj->pertipo){
 			case 'RA':
 				$row->addCell( $this->campoRadio( $obj ) );
 				break;
 			case 'CK':
 				$row->addCell( $this->campoCheck( $obj ) );
 				break;
 			case 'TX':
 				$row->addCell( $this->campoText( $obj ) );
 				break;
 			case 'TA':
 				$row->addCell( $this->campoTextArea( $obj ) );
 				break;
 			case 'CB':
 				$cell = $row->addCell( $this->campoCombo( $obj ) );
 				$this->inserirDivSubPerg($cell, $obj);
 				break;
 			case 'CM':
 				$cell = $row->addCell( $this->campoComboMultiplo( $obj ) );
 				$this->inserirDivSubPerg($cell, $obj);
 				break;
 			case 'DT':
 				$row->addCell( $this->campoData( $obj ) );
 				break;
 			case 'EXT':
 				$cell = $row->addCell( $this->campoExternoTabela( $obj ) );
 				$cell->add( $this->campoHidden( $obj ) );
 				break;
 			case 'EXF':
 				$cell = $row->addCell( $this->campoExternoFormulario( $obj ) );
 				$cell->add( $this->campoHidden( $obj ) );
 				break;
 		}
//		if ($obj->perid != 416 && $obj->perid != 417){
//			ver($obj, d);
//			ver($table,d);
//		}	
// 		if($this->itemPergunta[$obj->perid]){
//			
//			$resposta = $this->ger->trataResposta($this->ger->carregaResposta( $obj ));
//			if ( count($resposta) < 1 ){
//				$style = "display:none;";
// 			}else{
//				$style = "";	
//			}
//			$itemPai = $this->ger->itemPai($obj);
//			if ( $itemPai && $this->subPergunta ){
//				$row = $this->divSubPergunta[$itemPai]->addRow();
//			}else{
// 				$row = $this->tableQuestionario->addRow();
//			}
//			
// 			$row->id 	= "tr_subpergunta_" . $obj->perid;
// 			$row->style = $style;
// 			 
// 			$cell 		= $row->addCell('');
// 			foreach($this->itemPergunta[$obj->perid] as $itpid => $itemPergunta){
// 				// linhas dos sub-itens
// 				$div 		= new TElement("div");
// 				$div->id 	= $itemPergunta;
// 				$div->add('');
// 				
// 				if ( count($resposta) > 0 && in_array($itpid, $resposta) ){
// 					$subPergTable 		    	  = new TTable;
//	 				$this->divSubPergunta[$itpid] = $subPergTable;
//					$div->add( $this->divSubPergunta[$itpid] );
// 				}
//	 			$cell->add( $div );
// 			}
// 			
// 			if ( is_null($this->subPergunta) ){
//				$this->subPergunta = true;
//				$this->nivel = 0;
//			}
// 		}
	}
	
	private function campoTextArea( QPergunta $obj ){
		$campo = new TTextArea("perg_{$obj->perid}");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setObrig( $obj->perobrigatorio == "t" );
		$campo->setHabil( $this->desab );
		
		return $campo;
		
	}
	
	private function campoText( QPergunta $obj ){
		$campo = new TText("perg[{$obj->perid}]");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setMascara( $obj->permascara );
		$campo->setObrig( $obj->perobrigatorio == "t" );
//		dbg($this->desab);
		$campo->setHabil( $this->desab );
		
		return $campo;
		
	}
	
	private function campoExternoTabela( QPergunta $obj ){
		
		$campo = new Montatabela( );		
		$campo->montaNovaTabela( $obj->perid, $this->ger->getQrpid() );
				
		return $campo;
		
	}
	
	private function campoExternoFormulario( QPergunta $obj ){
		
		$campo = new CampoExternoControle( );		
		$campo->montaNovoCampo( $obj->perid, $this->ger->getQrpid() );
				
		return $campo;
		
	}
	
	private function campoHidden( QPergunta $obj ){
		
		$hidden = new TElement('input');
		$hidden->type  = 'hidden';
		$hidden->value  = true;
		$hidden->name  = 'perg['.$obj->perid.']';
		$hidden->id    = 'perg['.$obj->perid.']';
				
		return $hidden;
		
	}
	
	private function campoData( QPergunta $obj ){
		$campo = new TData("perg[{$obj->perid}]");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setLabel( $obj->perajuda );
		$campo->setObrig( $obj->perobrigatorio == "t" );
		$campo->setHabil( $this->desab );
		
		return $campo;
	}
	
	private function campoCombo( QPergunta $obj ){
		
		$campo = new TCombo("perg[{$obj->perid}]");
		
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setAcao("javascript: quest.buscaSubPergunta({\"perid\" : {$obj->perid}, \"itpid\" : this.value}, this)");
		$campo->setHabil( $this->desab );
		
		$arrObItens = $this->ger->carregaObItensPergunta($obj);
		foreach($arrObItens as $obItem ){
//			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
////				$this->itemPergunta[$obj->perid][$obItem->itpid] = "linha_{$obj->perid}_{$obItem->itpid}";
//				$this->criaDivSubPerg($obItem->itpid , "linha_{$obj->perid}_{$obItem->itpid}");
//			}
			$campo->setItens( array("codigo" => $obItem->itpid, "descricao" => $obItem->itptitulo) );
		}
		return $campo;		
	}
	
	private function campoComboMultiplo( QPergunta $obj ){
		
		$campo = new TCombo("perg[{$obj->perid}]");
		$campo->setMultiplo(true);
		$campo->setOpcao(null);
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setAcao("javascript: quest.buscaSubPergunta({\"perid\" : {$obj->perid}, \"itpid\" : this.value}, this)");
		$campo->setHabil( $this->desab );
		
		$arrObItens = $this->ger->carregaObItensPergunta($obj);
		foreach($arrObItens as $obItem ){
			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
				$this->itemPergunta[$obj->perid][$obItem->itpid] = "linha_{$obj->perid}_{$obItem->itpid}";
			}
			$campo->setItens( array("codigo" => $obItem->itpid, "descricao" => $obItem->itptitulo) );
		}
		return $campo;		
	}
	
	private function campoRadio( QPergunta $obj ){
		
		$campo = new TRadioGroup("perg[{$obj->perid}]");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setHabil( $this->desab );
		
		$arrObItens = $this->ger->carregaObItensPergunta($obj);
		foreach($arrObItens as $obItem ){
			unset($onclick, $divSubPerg);
			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
//				$this->itemPergunta[$obj->perid][$obItem->itpid] = "linha_{$obj->perid}_{$obItem->itpid}";
				$divSubPerg = $this->criaDivSubPerg($obItem->itpid , "linha_{$obj->perid}_{$obItem->itpid}");
				$onclick = "javascript: quest.buscaSubPergunta({\"perid\" : {$obj->perid}, \"itpid\" : {$obItem->itpid}}, this)";
			}else{
				$onclick = "javascript: quest.closeSubPerguntas({$obj->perid});";
			}
			$campo->setItens( array("codigo" => $obItem->itpid, "descricao" => $obItem->itptitulo, "onClick" => $onclick, "divSubPergunta" => $divSubPerg ));
		}
		return $campo;		
	}
	
	private function campoCheck( QPergunta $obj ){
		
		$campo 		= new TCheckGroup("perg[{$obj->perid}]");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setHabil( $this->desab );
		
		$arrObItens = $this->ger->carregaObItensPergunta($obj);
		foreach($arrObItens as $obItem ){
			unset($onclick);
			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
//				$this->itemPergunta[$obj->perid][$obItem->itpid] = "linha_{$obj->perid}_{$obItem->itpid}";
				$divSubPerg = $this->criaDivSubPerg($obItem->itpid , "linha_{$obj->perid}_{$obItem->itpid}");
				$onclick = "javascript: quest.buscaSubPergunta({\"perid\" : {$obj->perid}, \"itpid\" : {$obItem->itpid}}, this)";
			}
			$campo->setItens( array("codigo" => $obItem->itpid, "descricao" => $obItem->itptitulo, "onClick" => $onclick, "divSubPergunta" => $divSubPerg ));
		}
		return $campo;		
	}
	
	private function telaQQuestionario($obj){
		if ( is_null($this->subPergunta) ){
			$table = $this->tableQuestionario;	
		}/*else{
			$table = $this->divSubPergunta[$this->ger->itemPai($obj)];
		}*/
		$row 			= $table->addRow();
		$cell 			= $row->addCell( $obj->quetitulo. ' ' );
		$imagem 		= $obj->quedescricao ? self::imgAjuda( $obj->quedescricao ) : '';
		$cell->add( $imagem );
		$cell->class	= "SubTituloEsquerda";
		$cell->colspan 	= 2;
	}
	
	private function telaQGrupo($obj){
		if ( is_null($this->subPergunta) ){
			$table = $this->tableQuestionario;	
		}else{
			$table = $this->divSubPergunta[$this->ger->itemPai($obj)];
		}
		
		$row 			= $table->addRow();
		$cell 			= $row->addCell( self::imgNivel() );
		$cell->class	= "SubTituloEsquerda";
		$cell->colspan 	= 2;
		$cell->add( $obj->grptitulo. ' ' );
		$imagem 		= $obj->grpajuda ? self::imgAjuda( $obj->grpajuda ) : '';
		$cell->add( $imagem );
	}

	private function criaDivSubPerg($itpid, $idDiv){
		$divSubPerg 	   = new TElement("div");
        $divSubPerg->id    = $idDiv;
        $divSubPerg->style = "display:none; margin-top:5px;";
        $divSubPerg->add('');			

		$this->arrDivSubPergunta[$itpid] = $divSubPerg;
		
		return $divSubPerg; 
	}
	
	private function inserirDivSubPerg(TElement $cell, QPergunta $obPerg){
		$arrObItens = $this->ger->carregaObItensPergunta($obPerg);
		foreach($arrObItens as $obItem ){
			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
				$divSubPerg = $this->criaDivSubPerg($obItem->itpid , "linha_{$obPerg->perid}_{$obItem->itpid}");
				$cell->add( $divSubPerg );
			}
		}	
	}

}
?>		
