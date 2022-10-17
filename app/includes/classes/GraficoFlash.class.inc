<?php

class GraficoFlash extends cls_banco {
	
	private $id = "id";
	private $tipo = "linha";
	private $titulo = "Grafico";
	private $style_titulo = "font-weight:bold;font-size:16px";
	private $arrDados;
	private $elements;
	private $legenda = true;
	private $altura = "300px";
	private $largura = "400px";
	private $inc_js = false;
	private $style = false;
	private $classCss = false;
	
	public function __construct($id = null, $js = true){
		
		parent::__construct();
		
		$id ? $this->setId($id) : false;
		
		$this->setStyle("height:".$this->altura.";width:".$this->largura);
		
		$inc_ofc = '<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>';
		$inc_json = '<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/json/json2.js"></script>';
		
		echo $js ? $inc_ofc.$inc_json : false;
		
	}
	
	public function setId($id){
		
		$this->id = $this->remove(" ",$id);
		
	}
	
	public function setTipo($tipo){
		
		$this->tipo = $tipo;
		
	}
	
	public function setTitutlo($titulo){
		
		$this->titulo = $titulo;
		
	}
	
	public function setArrDados($arrDados = array("valor" => array(1,2,5,7) , "legenda" => array("a","b","c","d") ) ){
		
		$this->arrDados = is_array($arrDados) ?  $this->setElements( $this->criaElementos( $this->arrDados = $arrDados ) ) : false;
		
	}
	
	public function setElements($elements){
		
		$this->elements = $elements; 
		
	}
	
	public function setLegenda($legenda){
		
		$this->legenda = $legenda ? true : false;
		
	}
	
	public function setAltura($altura){
		
		$this->altura = $altura;
		
	}
	
	public function setLargura($largura){
		
		$this->largura = $largura;
		
	}
	
	public function setIncJS($include_js){
		
		$this->inc_js = $include_js;
		$this->escreveJS();
		
	}
	
	public function setStyle($style){
		
		$this->style = $style;
		
	}
	
	public function setClassCss($class){
		
		$this->classCss = $class;
		
	}
	
	public function criaGraficoFlash($arrDados = null){
		
		$arrDados ? $this->setArrDados($arrDados) : false;
		
		$elementsDefault = 'var data_'.$this->id.' = { "elements": [ { "type": "line", "values": [ 9, 8, 7, 6, 5, 4, 3, 2, 1 ] } ], "title": { "text": "Grafico Linha - Exemplo" }, "x_axis": { "steps": 2 } }';
		
		$this->elements ? $this->elements : $this->setElements($elementsDefault);
		
		$js = 'swfobject.embedSWF(';
		$js.= '"/includes/open_flash_chart/open-flash-chart.swf", "'.$this->id.'",';
		$js.= '"100%", "100%", "9.0.0" ,';
		$js.= '{"loading":"Carregando gráfico..."} );';
		$js.= 'function open_flash_chart_data(){return JSONOFC.stringify( data_'.$this->id.' );}';
		
		$js.= $this->elements;
		
		$this->escreveJS($js);
		$this->escreveHTML("<div ".($this->style ? "style=\"".$this->style."\"" : "")." ".($this->classCss ? "class=\"".$this->classCss."\"" : "")."><div id=\"$this->id\" ></div></div>");
		
		
	}
	
	public function escreveJS($js){
		
		echo $js ? '<script language="javascript" type="text/javascript" >'.$js.'</script>' : false;
		
	}
	
	public function includeJS($inc_js = null){
		
		if($inc_js)
			 setIncJS($inc_js);
		
		if(strstr($this->inc_js,".js")){
			echo '<script language="javascript" src="'.$this->inc_js.'" type="text/javascript" />';	
		}else{
			return false;
		}
		
	}
	
	public function escreveHTML($html){
		
		echo $html;
		
	}
	
	private function remove( $remove, $texto ){
		
		return str_replace( $remove, "", $texto );
		
	}
	
	private function criaElementos(){
		
		include_once ("../../includes/open_flash_chart/open-flash-chart.php");
		include_once ("../../includes/open_flash_chart/ofc_sugar.php");
		
		switch($this->tipo){
			
			case "linha":

				//Título do Gráfico
				$title = new title( $this->removeAcentosGrafico($this->titulo)  );
				$this->style_titulo ? $title->set_style( "{".$this->style_titulo."}" ) : false;
				
				if(is_array($this->arrDados['valor'])){
					
					foreach( $this->arrDados['valor'] as $chave => $arrValor ){
						
						if(is_array($arrValor)){
							
							foreach( $arrValor as $chave2 => $valor ){
								
								//Linha do Gráfico
								$line_default_dot[ $chave ] = new dot();
								
								//Cor do ponto da Linha do Gráfico
								$line_default_dot[ $chave ]->colour( $this->arrDados['cor'][ $chave ] ? $this->arrDados['cor'][ $chave ] : '#f00000');
								
								//Instância da linha
								$line[ $chave ] = new line();
								
								//Atribuindo as caracteristicas da linha
								$line[ $chave ]->set_default_dot_style( $line_default_dot[ $chave ] );
								
								//Atribuindo os valores da linha
								$line[ $chave ]->set_values( $this->arrDados['valor'][ $chave ] ? $this->arrDados['valor'][ $chave ] : array(0) );
								
								//Atribuindo a largura da linha
								$line[ $chave ]->set_width( 2 );
								
								//Atribuindo a cor a linha
								$line[ $chave ]->set_colour( $this->arrDados['cor'][ $chave ] );
								
								//Atribuindo a legenda
								$this->arrDados['legenda'][ $chave ] ? $line[ $chave ]->set_key( $this->arrDados['legenda'][ $chave ], 12 ) : false;
								
								//Valor máximo dos dados
								$arrMax[ $chave ] = round(max($this->arrDados['valor'][ $chave ]));
								
							}
							
							$max = max($arrMax);
							
						}else{
							
							//Linha do Gráfico
							$line_1_default_dot = new dot();
							
							//Cor da Linha do Gráfico
							$line_1_default_dot->colour( $this->arrDados['cor'] ? $this->arrDados['cor'] : '#f00000');
							
							//Instância da linha
							$line[0] = new line();
							
							//Atribuindo as caracteristicas da linha
							$line[0]->set_default_dot_style($line_1_default_dot);
							
							//Atribuindo os valores da linha
							$line[0]->set_values( $this->arrDados['valor'] ? $this->arrDados['valor'] : array(0) );
							
							//Atribuindo a largura da linha
							$line[0]->set_width( 2 );
							
							//Atribuindo a cor a linha
							$line[0]->set_colour( $this->arrDados['cor'] );
							
							//Atribuindo a legenda
							$this->arrDados['legenda'] ? $line[0]->set_key( $this->arrDados['legenda'], 12 ) : false;
							
							//Valor máximo dos dados
							$max = round(max($this->arrDados['valor']));
							
						}
						
					}
					
				}
				
				
				//Intância do Eixo Y
				$y = new y_axis();
				
				//Calcula valores para o Eixo Y
				$arrMax = explode(".",$max);
				$MaxValue = substr($arrMax[0],0,1);
				$MaxValue = $MaxValue + 1;
				for($i = 1; $i < strlen($arrMax[0]);$i++){
					$MaxValue .= "0";
				}
				$max = round($MaxValue);
				
				//Definindo o range
				$range = count($this->arrDados['valor']) == 1 || count($this->arrDados['valor']) == 0 ? 1 : ($MaxValue / 4);
				
				//Atribuindo valor mínimo, máximo e range Eixo Y
				$y->set_range( 0, $MaxValue , $range);
				
				//Difinição do Eixo X
				$x_labels = new x_axis_labels();
				
				//adiciona a cor dos elementos do eixo x
				$x_labels->set_colour( '#990000' );
				
				//adiciona os elementos do eixo x
				$x_labels->set_labels( $this->arrDados['eixo_x'] ? $this->removeAcentosGrafico($this->arrDados['eixo_x']) : array("Legenda") );
				
				//rotaciona o eixo x
				$x_labels->rotate(30);
				
				//Instância do Eixo x
				$eixo_x = new x_axis();
				
				//Adiciona a definição do eixo x
				$eixo_x->set_labels($x_labels) ;
				
				//Instância do gráfico
				$chart = new open_flash_chart();
				
				//adiciona o eixo x ao grafico
				$chart->set_x_axis( $eixo_x );
				
				//adiciona o eixo y ao grafico
				$chart->set_y_axis( $y );
				
				//adiciona o elemento título no gráfico 
				$chart->set_title( $title );
				
				//adiciona o elemento linha 1 no gráfico
				$line = !$line ? array() : $line;
				foreach($line as $chave => $objLine){ 
					$chart->add_element( $line[ $chave ] );	
				}
				
				//informa a cor de background do grafico
				$chart->set_bg_colour( '#ffffff' );
				
				//retorna os elementos do gráfico
				return 'var data_'.$this->id.' = '.$chart->toPrettyString();
				
				break;
			
			default:
				
				return 'var data_'.$this->id.' = { "elements": [ { "type": "line", "values": [ 9, 8, 7, 6, 5, 4, 3, 2, 1 ] } ], "title": { "text": "Grafico Linha - Exemplo" }, "x_axis": { "steps": 2 } }';
			
			break;
			
		}
		
		
		//return $this->arrDados;
		
	}
	
	private function removeAcentosGrafico($string){
		
		if(is_array($string)){
			
			foreach($string as $str){
				
				$ACENTOS   = array("À","Á","Â","Ã","à","á","â","ã");
				$SEMACENTOS= array("A","A","A","A","a","a","a","a");
				$str=str_replace($ACENTOS,$SEMACENTOS, $str);
		      
				$ACENTOS   = array("È","É","Ê","Ë","è","é","ê","ë");
				$SEMACENTOS= array("E","E","E","E","e","e","e","e");
				$str=str_replace($ACENTOS,$SEMACENTOS, $str);
				
				$ACENTOS   = array("Ì","Í","Î","Ï","ì","í","î","ï");
				$SEMACENTOS= array("I","I","I","I","i","i","i","i");
				$str=str_replace($ACENTOS,$SEMACENTOS, $str);
		      
				$ACENTOS   = array("Ò","Ó","Ô","Ö","Õ","ò","ó","ô","ö","õ");
				$SEMACENTOS= array("O","O","O","O","O","o","o","o","o","o");
				$str=str_replace($ACENTOS,$SEMACENTOS, $str);
		     
				$ACENTOS   = array("Ù","Ú","Û","Ü","ú","ù","ü","û");
				$SEMACENTOS= array("U","U","U","U","u","u","u","u");
				$str=str_replace($ACENTOS,$SEMACENTOS, $str);
				
				$ACENTOS   = array("Ç","ç","ª","º","°");
				$SEMACENTOS= array("C","c","a.","o.","o.");
				$str=str_replace($ACENTOS,$SEMACENTOS, $str);
				
				$arrStr[] = $str;
				
			}
			
			return $arrStr;
			
		}else{

			$ACENTOS   = array("À","Á","Â","Ã","à","á","â","ã");
			$SEMACENTOS= array("A","A","A","A","a","a","a","a");
			$string=str_replace($ACENTOS,$SEMACENTOS, $string);
	      
			$ACENTOS   = array("È","É","Ê","Ë","è","é","ê","ë");
			$SEMACENTOS= array("E","E","E","E","e","e","e","e");
			$string=str_replace($ACENTOS,$SEMACENTOS, $string);
			
			$ACENTOS   = array("Ì","Í","Î","Ï","ì","í","î","ï");
			$SEMACENTOS= array("I","I","I","I","i","i","i","i");
			$string=str_replace($ACENTOS,$SEMACENTOS, $string);
	      
			$ACENTOS   = array("Ò","Ó","Ô","Ö","Õ","ò","ó","ô","ö","õ");
			$SEMACENTOS= array("O","O","O","O","O","o","o","o","o","o");
			$string=str_replace($ACENTOS,$SEMACENTOS, $string);
	     
			$ACENTOS   = array("Ù","Ú","Û","Ü","ú","ù","ü","û");
			$SEMACENTOS= array("U","U","U","U","u","u","u","u");
			$string=str_replace($ACENTOS,$SEMACENTOS, $string);
			
			$ACENTOS   = array("Ç","ç","ª","º","°");
			$SEMACENTOS= array("C","c","a.","o.","o.");
			$string=str_replace($ACENTOS,$SEMACENTOS, $string);
			
			return $string;
			
		}
		
	}
	
	
}