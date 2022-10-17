<?php 

class Visao {

	/**
	 * Array de variáveis
	 * @access private
	 */
	private $vars = array();
	
	public function __set($index, $value)
	{
	    $this->vars[$index] = $value;
	}
	
	public function visualizar($visao)
	{
		$arURL = explode('=', $_SERVER['REQUEST_URI']);
		$arPRM = explode('&', $arURL[1]);
		$arDIR = explode('/', $arPRM[0]);
		
		for($x=0;$x<count($arDIR)-1;$x++){
			$diretorio .= $arDIR[$x]."/";
		}

		if(!empty($visao)){
		    $arquivo = APPRAIZ . $_SESSION['sisdiretorio'] . '/modulos/'.$diretorio.'visao/' . $visao . '.php';

		    if( file_exists($arquivo) == false ){
		        throw new Exception('Visão não encontrada em: ' . $arquivo);
		        return false;
		    }
			extract($this->vars);
		    require_once $arquivo;
		}		
	}
}