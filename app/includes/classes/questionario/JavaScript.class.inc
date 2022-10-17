<?php
final class JavaScript {
	private $obQuest;
	private $arrPerg;
	private $codigo;
	private $paramJs;
	private $paramCtrl;
	
	function __construct( GerenciaQuestionario $obQuest ){
		$this->obQuest = $obQuest;
		
		$this->paramCtrl['include']		  = true;
		$this->paramCtrl['fonte']		  = true;
		$this->paramCtrl['paramJs']		  = true;
		$this->paramCtrl['limpaPergunta'] = false;
	}
	
	private function codigoInclude(){
		return '<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
				<script type="text/javascript" src="../includes/Questionario.class.js"></script>
				<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
				<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
				<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
				';
	}
	
	private function codigoFonte(){
		return 'var quest = new Questionario({"peridAtual" : "' . $this->obQuest->getPeridPai() . '", "qrpid" : "' . $this->obQuest->getQrpid() . '", "url" : "' . $_SERVER['REQUEST_URI'] . '"});' . "\n";		
	}
	
	private function codigoPergunta( Array $arrPerg ){
		$obPerg   = $arrPerg[0];
		$idPai    = $arrPerg[1];
		$itemPai  = $arrPerg[2];
		
		return 'quest.obPerg.add({"id" : ' . $obPerg->perid . ', "obrig" : ' . ($obPerg->perobrigatorio == 't' ? 'true' : 'false') . ', "tipo" : "' . $obPerg->pertipo . '", "idPai" : "' . $idPai . '", "itemPai" : "' . $itemPai . '", "txt" : "' . addslashes($obPerg->pertitulo) . '", "perposacao" : "' . $obPerg->perposacao . '"});' . "\n";		
	}

	private function setParamJs($param=null, $arrLabel = null){
		if (!$param){
			return;
		}
		
		if (is_array($param)):
			foreach($param as $k => $val){
				if (is_array($val)){
					$label = $arrLabel ? $arrLabel . "[" . $k ."]" : $k;
					$this->setParamJs($val, $label);	
				}else{
					$val = htmlspecialchars($val, ENT_QUOTES);
					$this->paramJs .= empty($arrLabel) ? "{'name':'$k', 'value':'$val'}" : "{'name':'$arrLabel" . "[" . $k . "]', 'value':'$val'}";	
				}					
			}
		endif;
	}	
	
	public function notInclude(){
		$this->paramCtrl['include'] = false;
	}
	
	public function notFonte(){
		$this->paramCtrl['fonte'] = false;
	}
	
	public function notParamJs(){
		$this->paramCtrl['paramJs'] = false;
	}
	
	public function atualizaPergunta(){
		$this->paramCtrl['limpaPergunta'] = true;
	}
	
	public function addPergunta(QPergunta $obPerg, $peridPai, $itemPai){
		$this->arrPerg[] = array($obPerg, $peridPai, $itemPai);
	}
	
	public function show(){
		if ( $this->paramCtrl['include'] ){
			$this->codigo  = self::codigoInclude();
		}
				
		$this->codigo .= "\n<script type=\"text/javascript\">\n\n";
		
		if ( $this->paramCtrl['fonte'] ){
			$this->codigo .= self::codigoFonte();
		}

		if ( $this->paramCtrl['limpaPergunta'] ){
			$this->codigo .= "quest.obPerg.clean(); \n";
		}
		
		foreach ($this->arrPerg as $arrPerg){
			$this->codigo .= self::codigoPergunta( $arrPerg );		
		}
		
		if ( $this->paramCtrl['paramJs'] ){
			self::setParamJs($_REQUEST);
			$this->codigo .= "quest.carregaParamUrl( [" . ( str_replace('}{', '},{', $this->paramJs) ) . "] );\n";
		}
		
		$this->codigo .= " \n</script>\n ";
		
		echo $this->codigo;
	}
}
?>