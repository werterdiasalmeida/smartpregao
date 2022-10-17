<?php
class ConteudoFlutuante{
	
	private $jquery = '';
	private $jqueryInterface = '';
	private $javaScript;
	private $css;
	private $prototype;
	private $localParte1 = '<script type="text/javascript" src="';
	private $localParte2 = '"></script>';
	private $localCssParte1 = '<link rel="stylesheet" type="text/css" href="';
	private $localCssParte2 = '"/>';

	
	/**
	 * __construct
	 *
	 * @param string $javaScript 		(não obrigatorio)
	 * @param string $jquery 			(não obrigatorio)
	 * @param string $jqueryInterface 	(não obrigatorio)
	 * @param string $prototype 		(não obrigatorio)
	 * @param string $css 				(não obrigatorio)
	 * 
	 * @desc  Inicia a montagem de div flutuantes.
	 * @author Thiago Tasca Barbosa.
	 * @version 1.0
	 * @since  : 14/07/2009
	 * 
	 */
	public function __construct($javaScript = NULL, $jquery = NULL, $jqueryInterface = NULL, $prototype = NULL, $css = NULL){
		$this->setPrototype($prototype);
		$this->setJquery($jquery);
		$this->setJqueryInterface($jqueryInterface);
		$this->setJavaScriptConteudoFlutuante($javaScript);
		$this->setCss($css);
	}
	
	/**
	 * setCss
	 *
	 * @param string $css (não obrigatorio)
	 * @return css
	 * 
	 * @desc Adiciona o CSS passado por parametro, caso não tenha ele pega o CSS padrão.
	 * @author Thiago Tasca Barbosa.
	 * @version 1.0
	 * @since  : 14/07/2009
	 */
	protected function setCss($css = NULL){
		if($css){
			return $this->css = $this->localCssParte1.$css.$this->localCssParte2;
		}else{
			return $this->css = $this->localCssParte1.'/includes/conteudoFlutuante.css'.$this->localCssParte2;
		}	
	}
	
	/**
	 * setJquery
	 *
	 * @param string $jquery (não obrigatorio)
	 * @return jguery
	 * 
	 * @desc Adiciona o JavaScript Jquery passado por parametro, caso não tenha ele pega o padrão.
	 * @author Thiago Tasca Barbosa.
	 * @version 1.0
	 * @since  : 14/07/2009
	 */
	protected function setJquery($jquery = NULL){
		if($jquery){
			return $this->jquery = $this->localParte1.$jguery.$this->localParte2;
		}else{
			return $this->jquery = $this->localParte1.'/includes/JQuery/jquery.js'.$this->localParte2;
		}
	}
	
	/**
	 * setJqueryInterface
	 *
	 * @param string $jqueryInterface (não obrigatorio)
	 * @return jqueryInterface
	 * 
	 * @desc Adiciona o JavaScript Interface do Jquery passado por parametro, caso não tenha ele pega o padrão.
	 * @author Thiago Tasca Barbosa.
	 * @version 1.0
	 * @since  : 14/07/2009
	 */
	protected function setJqueryInterface($jqueryInterface = NULL){
		if($jqueryInterface){
			return $this->jqueryInterface = $this->localParte1.$jqueryInterface.$this->localParte2;
		}else{
			return $this->jqueryInterface = $this->localParte1.'/includes/JQuery/interface.js'.$this->localParte2;
		}
	}
	
	/**
	 * setPrototype
	 *
	 * @param string $jqueryInterface (não obrigatorio)
	 * @return prototype
	 * 
	 * @desc Adiciona o JavaScript Prototype passado por parametro, caso não tenha ele pega o padrão.
	 * @author Thiago Tasca Barbosa.
	 * @version 1.0
	 * @since  : 14/07/2009
	 */
	protected function setPrototype($prototype = NULL){
		if($prototype){
			return $this->prototype = $this->localParte1.$prototype.$this->localParte2;
		}else{
			return $this->prototype = $this->localParte1.'/includes/prototype.js'.$this->localParte2;
		}
	}
	
	/**
	 * setJavaScriptConteudoFlutuante
	 *
	 * @param string $javaScript (não obrigatorio)
	 * @return javaScript
	 * 
	 * @desc Adiciona o JavaScript conteudoFlutuante passado por parametro, caso não tenha ele pega o padrão.
	 * @author Thiago Tasca Barbosa.
	 * @version 1.0
	 * @since  : 14/07/2009
	 */
	protected function setJavaScriptConteudoFlutuante($javaScript = NULL){
		if($javaScript){
			return $this->javaScript = $this->localParte1.$javaScript.$this->localParte2;
		}else{
			return $this->javaScript = $this->localParte1.'/includes/conteudoFlutuante.js'.$this->localParte2;
		}
	}
	
	
	/**
	 * criaConteudoFlutuante
	 *
	 * @param string  $quantidade
	 * @param array   $arrDivLocalColuna
	 * @param array   $arrTituloDiv
	 * @param array   $arrConteudoDiv
	 * @param array   $arrScroll (true tem scroll, false não tem)
	 * @param array   $arrEstadoBox (max = maximizado, min = minimizado)
	 * @param boleano $usuarioExiste (Não precisa passar - controle interno da função)
	 * @param boleano $scrollExiste  (Não precisa passar - controle interno da função)
	 * 
	 * @desc Cria o Conteudo Flutuante - Cria uma Div com varias caixas internas que podem ser movidas de um lado para outro.
	 * @example	
	 * 			$conteudo1 = "<div>Conteudo1</div>";
	 * 			$conteudo2 = "<div>Conteudo2</div>";
	 * 			$conteudo3 = "<div>Conteudo3</div>";
	 * 			$conteudo4 = "<div>Conteudo4</div>";
	 * 
	 * 			$areaFlutuante 		= new ConteudoFlutuante(); 	// Instancia a Classe.
	 * 			$qtdColunas 		= 3; 						// Quantidade de colunas desejada.
	 *			$arrPosições	 	= array(0=>array(0=>0, 1=>1),
	 *										1=>array(0=>2),
	 *										2=>array(0=>3));    // Array com as colunas e linhas; 1 array contem as colunas, array interno a chave e a linha na coluna e o valor e a div. 
	 * 															// Exemplo: 0=>array(0=>0, 1=>1) sendo assim:  coluna=>array(linha1=>Box2, linha2=>box1);
	 * 
	 *			$arrTituloDiv 		= array(0=>'Dados da Unidade', 
	 * 										1=>'Programa Pró Letramento', 
	 * 										2=>'Programa Escola Ativa', 
	 * 										3=>'Relatórios de subações que constam no termo de Cooperação',4=>'teste'); // Array com os título do Box.
	 * 
	 *			$arrConteudoDiv 	= array(0=>$conteudo1, 1=>$conteudo2, 2=>$conteudo3, 3=>$conteudo4); // Array com o Conteudo do Box.
	 *			$arrScroll 			= array(0=>false, 1=>false, 2=>true, 3=>false); // Array dizendo se o Box tem ou não scroll - true tem, false não
	 *			$arrEstadoBox 		= array(0=>'min', 1=>'max', 2=>'max', 3=>'max'); // Array dizendo se o Box está maximizado ou minimizado. 
	 * 
	 * 			$areaFlutuante->criaConteudoFlutuante($qtdColunas, $arrPosições, $arrTituloDiv, $arrConteudoDiv, $arrScroll, $arrEstadoBox); 
	 * 
	 * 			// Para salvar os dados so incluir o botão com o função salva();
	 * 			// Para adicionar colunas apenas adicionar o botão addColuna
	 * 			<input onclick="salva();" type="button" value="Salvar" />
	 *			<input onclick="addColuna();" type="button" value="addColuna" />
	 * 
	 * 
	 * @author Thiago Tasca Barbosa.
	 * @version 1.0
	 * @since  : 14/07/2009
	 */
	public function criaConteudoFlutuante($quantidade, $arrDivLocalColuna, $arrTituloDiv, $arrConteudoDiv, $arrScroll, $arrEstadoBox, $usuarioExiste = NULL, $scrollExiste = NULL){
		if($this->verificaSeExisteDados() && !$usuarioExiste){
			$this->recuperaParametros($arrConteudoDiv, $arrTituloDiv, $quantidade);
		}else{
			$qtdColunas = count($arrDivLocalColuna);
			$qtdColunas = ($qtdColunas != 0) ? $qtdColunas : 1;
			$tamanhoBox = $this->montaTamanhoBox($qtdColunas);
			$conteudo 	= '<div id="conteudo_flutuante">';
			$conteudo 	= '<input type="hidden" id="qtdcolunas" name="qtdcolunas" value="'.$quantidade.'"  />';
			$contScroll = 0;
			$contEstado = 0;
			//foreach($arrDivLocalColuna as $coluna=>$linhas){
			for($contQuantidade = 0; $contQuantidade < $quantidade; $contQuantidade++){
				$conteudo  .= '<div id="sort'.$contQuantidade.'" class="groupWrapper" '.$tamanhoBox.' >';
				if(is_array($arrDivLocalColuna[$contQuantidade])){
				foreach($arrDivLocalColuna[$contQuantidade] as $posicao=>$div){
					if($scrollExiste){
						(!$arrScroll[$contScroll])? $scroll = "scrollfalse" : $scroll = $arrScroll[$contScroll];
					}else{
						(!$arrScroll[$div])? $scroll = "scrollfalse" : $scroll = $arrScroll[$div];
					}
					
					if($scrollExiste){
						(!$arrEstadoBox[$contEstado])? $estadoBox = "max" : $estadoBox = $arrEstadoBox[$contEstado];
					}else{
						(!$arrEstadoBox[$div])? $estadoBox = "max" : $estadoBox = $arrEstadoBox[$div];
					}
					
					($estadoBox == 'max')? $style_est = "" : $style_est = "none";
					($estadoBox == 'max')? $src_img_est = "../imagens/menos.gif" : $src_img_est = "../imagens/mais.gif";
					$contScroll++;
					$contEstado++;
					$conteudo .= '<div id="colid_'.$div.'" class="groupItem">';
					$conteudo .= '	<input type="hidden" id="estado_'.$div.'" name="estado_'.$div.'" value="'.$estadoBox.'"  />
									<input type="hidden" id="scroll_'.$div.'" name="scroll_'.$div.'" value="'.$scroll.'"  />
									<div style="-moz-user-select: none;" class="itemHeader">
										<div class="TituloCombo">'.$arrTituloDiv[$div].'</div>
										<div id="edit_'.$div.'"  class="edit" ><img onclick="exibeEditar(\''.$div.'\')"  src="../imagens/ico_config.gif" border=1 /></div>
											<a id="'.$div.'" class="closeEl" href="#">	<img border="0" src="'.$src_img_est.'"/></a>
										</div>
										<div style="display:none"  id="editar_'.$div.'" class="editar" >
											<div style="padding: 3px 3px 3px 3px;" >
												<input type="checkbox" id="autoscroll_'.$div.'" name="autoscroll" value="'.$div.'" onclick="executarEditar(\''.$div.'\',\''.$scroll.'\')" /> Rolagem
											</div>
										</div>
										<div class="itemContent" style="display:'.$style_est.'" id="itemContent_'.$div.'" >';
						if($scroll == "scrolltrue"){
							$conteudo .= "<script>
											document.getElementById( 'autoscroll_".$div."').checked = true;
											executarEditar('".$div."',true);
											document.getElementById( 'itemContent_".$div."').style.display = '".$style_est."';
										</script>";
						}
						$conteudo .= $arrConteudoDiv[$div];
						$conteudo .= '	</div>';
						$conteudo .= '</div>';
				}
			}else{
				$conteudo .= "<script>
								document.getElementById('sort".$contQuantidade."').style.display = 'none'; 
								</script>";
			}
				$conteudo .= '</div>';
			}
			$conteudo .= '</div>';
			echo $this->css.$this->prototype.$this->jquery.$this->jqueryInterface.$this->javaScript.$conteudo;
		}
	}
	
	/**
	 * montaTamanhoBox
	 *
	 * @param string $qtdColunas
	 * @return $tamanhoBox (Tamanho de cada coluna)
	 * 
	 * @desc Retorna o Tamanho que cada coluna deve ter.
	 * @author Thiago Tasca Barbosa.
	 * @version 1.0
	 * @since  : 14/07/2009
	 * 
	 */
	private function montaTamanhoBox($qtdColunas){
		$calculo = 100 * ($qtdColunas * 2);
		$total = $qtdColunas * 100;
		$resultado = $calculo / $total;
		$borda = 100 - $resultado;
		$tamanho =   $borda / (integer) $qtdColunas ;
		$tamanhoBox = 'style="width:'.$tamanho.'%;"';
		return $tamanhoBox;
	}

	
	/**
	 * salvar
	 *
	 * @return true ou false (true sucesso / falso erro ao salvar os dados)
	 * 
	 * @desc Monta um array com as colunas e seu boxs. E envia para função salvaParametros para salvar os parametros no banco de dados. 
	 * @author Thiago Tasca Barbosa.
	 * @version 1.0
	 * @since  : 14/07/2009
	 * 
	 * @example 
	 * 			// Para salvar os dados so incluir o botão com o função salva();
	 * 			// Para adicionar colunas apenas adicionar o botão addColuna
	 * 			<input onclick="salva();" type="button" value="Salvar" />
	 *			<input onclick="addColuna();" type="button" value="addColuna" />
	 */
	public function salvar(){
		header('content-type: text/html; charset=ISO-8859-1');
		
		$local ='';
		for($a = 0; $a <= count($_REQUEST); $a++){
			if($_REQUEST['sort'.$a]){
				$local['sort'.$a] = $_REQUEST['sort'.$a];
			}
		}

		foreach($local as $coluna=>$dados){
			$box = '';
			$box .= implode(';',$dados);
			$col .=$coluna."=".$box."/";
			
		}

		$this->salvaParametros($col,$_REQUEST['estados'],$_REQUEST['scroll'],$_SESSION['usucpf']);
		return true;
	}
	
	
	/**
	 * salvaParametros
	 * @desc 	Salva dados no Banco de dados. 
	 * 			Salva a posição dos boxs + se ele está com scroll e maximizado ou minimizado. 
	 *
	 * @param strig $local
	 * @param array $estados
	 * @param array $scroll
	 * @param string $usucpf
	 * @return true / false
	 * @since  : 14/07/2009
	 */
	private function salvaParametros($local,$estados,$scroll,$usucpf){
		global $db;
	
		$sql = "select usucpf from public.parametros_tela
				where usucpf = '$usucpf' and mnuid = '{$_SESSION['mnuid']}'";
		$parm = $db->pegaUm($sql);
		
		($estados) ? $estados = $estados : $estados = "";
		($scroll) ? $scroll = "/".$scroll : $scroll = "";
		
		$prtbj = $local.$estados.$scroll;
		
		if($parm == ""){ //insert
			$sql = "insert into public.parametros_tela
				(prtdsc,prtobj,prtpublico,prtdata,usucpf,mnuid) values
				('Eixo - Parametros de Colunas','$prtbj','FALSE',now(),'$usucpf','{$_SESSION['mnuid']}')";
		}
		if($parm != ""){ //update
			$sql = "update public.parametros_tela set
				prtobj = '$prtbj',
				prtdata = now()
				where usucpf = '$usucpf'
				and mnuid = '{$_SESSION['mnuid']}'";
		}

		$db->executar($sql);	
		$db->commit();
		return true;
	}
	
	/**
	 * recuperaParametros
	 * @desc Se existe dados gravados no banco de dados a função recupera os dados da tela.
	 *
	 * @param array $arrConteudoDiv
	 * @param array $arrTituloDiv
	 * @param string $quantidade
	 * 
	 * 
	 * @author Thiago Tasca Barbosa
	 * @version 1.0
	 * @since  : 14/07/2009
	 * 
	 */
	public function recuperaParametros($arrConteudoDiv, $arrTituloDiv, $quantidade){
		global $db;
		$sql = "select * from public.parametros_tela where usucpf = '{$_SESSION['usucpf']}' and mnuid = '{$_SESSION['mnuid']}'";
		$parm = $db->pegaLinha($sql);
		$param = $parm['prtobj'];
		
		if($param){
			$sort 		= explode("/",$param);
			$cont = 0;
			$contBox = 0;
			$total = count($sort);
			$total = $total -1;
			$scroll = $total -1;
			$tes = $sort[$total];
			$tes1 = $sort[$scroll];
			$arrEstadoBox = explode(',',$tes1);
			$arrScroll = explode(',',$tes);
			
			
			foreach($sort as $dados){
				$arrConteudo = explode("=",$dados);
				if($arrConteudo[1]){
					$arrBox 	= explode(";",$arrConteudo[1]);
					for($a = 0; $a < count($arrBox); $a++){
						$numBox = str_replace('colid_','',$arrBox[$a]);
						$colunas[$contBox] = $numBox;
						$contBox++;
						
					}
				}
				$nome = substr($arrConteudo[0], 0, -1);
				if($nome == 'sort'){
					$arrDivLocalColuna[$cont] = $colunas;
					$cont++;
					$contBox = 0;
				}
				unset($colunas);
			}
			$usuarioExiste = 1;
			$scroll = 1;
			$this->criaConteudoFlutuante($quantidade, $arrDivLocalColuna, $arrTituloDiv, $arrConteudoDiv, $arrScroll, $arrEstadoBox,$usuarioExiste, $scroll );
		}
	}
	
	/**
	 * verificaSeExisteDados
	 * @desc 	Verifica se existe dados da tela salvo no banco de dados. 
	 * 			Salva a posição dos boxs + se ele está com scroll e maximizado ou minimizado. 
	 * @return true or false;
	 *
	 * @author Thiago Tasca Barbosa
	 * @version 1.0
	 * @since  : 14/07/2009
	 * 
	 */
	private function verificaSeExisteDados(){
		global $db;
		$sql = "select usucpf from public.parametros_tela
				where usucpf = '".$_SESSION['usucpf']."' and mnuid = '{$_SESSION['mnuid']}'";
		$parm = $db->pegaUm($sql);
		
		if($parm){
			return true;
		}else{
			return false;
		}
	}
	

}
?>