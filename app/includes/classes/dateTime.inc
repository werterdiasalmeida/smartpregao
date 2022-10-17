<?php
/**
 * Class Data
 * @description : Classe manipuladora de Data e Hora (ISO)
 * @category Date
 * @version 1.0
 * @author Thiago Tasca Barbosa
 * @since 21/01/2009
 * @tutorial: caso deseja trabalhar apenas com uma data pode-se colocar a data na construtora.
 * 			  caso o formato da data seja americana passar a mascara de como deve ser a data. (obrigatorio neste caso) 
 * @example : $data = new Data('mm/dd/yyyy','2002-01-25 13:29:00');
 * 			  ou 
 * 			  $data = new Data();
 */
class Data{
	protected $value;
	protected $mascara;
	
	
	/**
	 * function construtora 
	 * @description : Instância Valores.
	 * @param integer mascara (opcional)
	 * @param integer dataHora (opcional)
	 * @author Thiago Tasca Barbosa
	 * @since 21/01/2008
	 */
	 function data($dataHora=NULL, $mascara=NULL){
		if(!$dataHora){
			$this->setValue	= "0000/00/00 00:00:00";
		}
		else{
			$this->setValue($dataHora);
		}
		if($mascara){
			$this->setMascara($mascara);
		}
	}
	
	/**
	 * function setValue 
	 * @description : Instância Valores.
	 * @return : string value
	 * @author Thiago Tasca Barbosa
	 * @since 22/01/2009
	 */
	protected function setValue($dataHora){
		return $this->value = $dataHora;
	}
	
	/**
	 * function setMascara 
	 * @description : Instância Valores.
	 * @return : string mascara
	 * @author Thiago Tasca Barbosa
	 * @since 25/02/2009
	 */
	protected function setMascara($mascara){
		return $this->mascara = $mascara;
	}

	/**
	 * function formataData 
	 * @description : Formata data e hora para formato desejado.
	 * @param  : string dataHora 
	 * @param  : string formato (formato desejado)
	 * @param  : string $mascara (formato da data que é passado) (obrigatorio para formato Americano)
	 * @return : string value (Data e hora)
	 * @author : Thiago Tasca Barbosa
	 * @since  : 22/01/2009
	 * @example: Exemplos de como utilizar a class. 
	 * 			$data = new Data();
	 *			$a = $data->formataData('2008-01-29 12:47:51.153357', "YY/MM/DD as HH:MMI:SS");
	 * 			ou
	 * 			$a = $data->formataData('2008-01-29 12:47:51.153357', "Brasília DD de mesTextual de YYYY as HH:MMI:SS");
	 * 			ou caso o formato da data seja Americano passar a $mascara informando a ordem da data, segue exemplo abaixo:
	 * 			$a = $data->formataData(  '02-22-1999 13:29:00', '03-02-2009 11:25:00', 'tempoEntreDadas', 'string','mm/dd/yyyy');	
	 */
	public function formataData($dataHora = NULL , $formato = NULL, $mascara = NULL){
		$formato = $formato ? $formato : 'DD/MM/YYYY';
		
		if(!$dataHora){
			$dataHora	= $this->value;
		}
		if(!$mascara){
			$mascara	= $this->mascara;
		}
		
		$dataHora = $this->inverteData($dataHora, $mascara );

		$dataAno	= substr($dataHora,0,4);
		$dataMes	= substr($dataHora,5,2);
		$dataDia	= substr($dataHora,8,2);
		$dataHor	= substr($dataHora,11,2);
		$dataMnu	= substr($dataHora,14,2);
		$dataSeg	= substr($dataHora,17,2);

		$meses	= array("01" => "Janeiro",
						"02" => "Fevereiro",
						"03" => "Março",
						"04" => "Abril",
						"05" => "Maio",
						"06" => "Junho",
						"07" => "Julho",
						"08" => "Agosto",
						"09" => "Setembro",
						"10" => "Outubro",
						"11" => "Novembro",
						"12" => "Dezembro");
		
		$formato	= str_replace( "mesTextual", $meses[$dataMes], $formato);
		
		$formato	= str_replace( "HH", 	$dataHor,  			  str_replace( "hh", "HH", $formato   ) );
		$formato	= str_replace( "MMI", 	$dataMnu, 			  str_replace( "mmi", "MMI", $formato ) );
		$formato	= str_replace( "SS", 	$dataSeg,  			  str_replace( "ss", "SS", $formato   ) );
		$formato	= str_replace( "YYYY", 	$dataAno,			  str_replace( "yyyy", "YYYY", $formato ) );
		$formato	= str_replace( "YY", 	substr($dataAno,2,2), str_replace( "yy", "YY", $formato ) );
		$formato	= str_replace( "MM",	$dataMes, 			  str_replace( "mm", "MM", $formato ) );
		$formato	= str_replace( "DD",	$dataDia, 			  str_replace( "dd", "DD", $formato ) );

		return ($this->value = $formato);		
	}
	
	/**
	 * function timeStampDeUmaData 
	 * @description : Retorna o TimeStamp UNIX de uma hora solicita.
	 * @param  : string dataHora
	 * @param  : string mascara
	 * @return : string value
	 * @author Thiago Tasca Barbosa
	 * @since 22/01/2009
	 * @example : 
	 * 			include APPRAIZ . "includes/classes/dateTime.inc";
	 *			$data = new Data();
	 * 			$retorno = $data->timeStampDeUmaData(  '2002-01-25 13:29:00'); 
	 *   ou 	
	 * 			$data = new Data('2002-01-25 13:29:00');
	 * 		    $retorno = $data->timeStampDeUmaData(); 
	 *   ou caso o formato Americano (obrigatorio mascara).
	 *  		$retorno = $data->timeStampDeUmaData( '01-25-2002 13:29:00','mm/dd/yyyy');
	 */
	public function timeStampDeUmaData($dataHora = NULL, $mascara = NULL){
		if(!$dataHora){
			$dataHora	= $this->value;
		}
		if(!$mascara){
			$mascara	= $this->mascara;
		}
		$dataHora = $this->inverteData($dataHora, $mascara);
		
		$dataAno	= substr($dataHora,0,4);
		$dataMes	= substr($dataHora,5,2);
		$dataDia	= substr($dataHora,8,2);
		$dataHor	= substr($dataHora,11,2);
		$dataMnu	= substr($dataHora,14,2);
		$dataSeg	= substr($dataHora,17,2);

		$formato	= mktime($dataHor, $dataMnu, $dataSeg, $dataMes, $dataDia, $dataAno);
		return ($this->value	= $formato);		
	}
	
	/**
	 * function dataAtual 
	 * @description : Retorna data e hora local.
	 * @param  : string dataHora
	 * @return : string value
	 * @author Thiago Tasca Barbosa
	 * @since 22/01/2009
	 * @tutorial : Se não passar o $formato ele pega o formato padrão sendo dd/mm/yyyy h:m:s
	 * @example: 
	 * 			include APPRAIZ . "includes/classes/dateTime.inc";
	 *			$data = new Data();
	 * 			$retorno = $teste->dataAtual('d-m-Y');
	 * 	ou pode recupera a data padrão da função apenas chamando a mesma:
	 * 			$retorno = $data->dataAtual();
	 */
	public function dataAtual($formato = NULL){
		if(!$formato){
			$dataHoraAtual	= date( 'd/m/Y H:m:s' );
		}else{
			$dataHoraAtual	= date( $formato );
		}
		return ($this->value	= $dataHoraAtual);		
	}
	
	
	/**
	 * function diferencaEntreDatas 
	 * @description : Retorna a diferença entre duas datas.
	 * @param  : string $dataInicial 
	 * @param  : string $dataFinal 
	 * @param  : string mascara (obrigatorio para formato Americano)
	 * @param  : string $tipo (Se for 'maiorDataBolean' retorna true ou false / 'tempoEntreDadas' retorna o tempo entre elas )
	 * @param  : string $formato (Formato desejado para retorno)
	 * @return : string value (diferença das datas)
	 * @author : Thiago Tasca Barbosa
	 * @since  : 26/01/2009
	 * @see    : timeStampDeUmaData (dependency)
	 * @tutorial :
	 * 	Existe 5 parametros a serem passados:
	 * 	Sendo o 1º: data de inicio
	 * 			2º: data de final
	 * 			3º: O tipo de operação desejada
	 * 			4º: O formato que deseja de retorno.
	 * 			5º: Formato que a data está vindo.
	 * 
	 * 	A variavel $tipo pode ser 'maiorDataBolean' 
	 *  onde a função irá retornar se a 1º data e maior que a 2º data com True ou False;
	 * 
	 *  O $formato serve para dizer se vc quer que ele retorne uma string com a diferença entre as datas ou um array com cada pedaço das datas
	 *  sendo que o retorno terá um array com [dd, mm, aaaa, hora, minuto, segundo]
	 * 
	 *  Caso o formato da data seja Americano informar a mascara, para que os calculos sejam feitos corretamentes.
	 * 
	 * @example: 
	 * 			include APPRAIZ . "includes/classes/dateTime.inc";
	 *			$data = new Data();
	 *  se deseja uma string retornando como exemplo: (7 anos, 2 dias, 21 horas e  56 minutos)
	 * 			$retorno = $data->diferencaEntreDatas(  '2002-01-25 13:29:00', '2009-01-26 11:25:00', 'tempoEntreDadas', 'string');
	 * 	ou se deseja apenas um array como retorno:		
	 * 			$retorno = $data->diferencaEntreDatas(  '2002-01-25 13:29:00', '2009-01-26 11:25:00', 'tempoEntreDadas', 'array');
	 *  se quero apenas saber a comparação entre duas datas basta:
	 * 			$retorno = $data->diferencaEntreDatas(  '2002-01-25 13:29:00', '2009-01-26 11:25:00', 'maiorDataBolean');
	 *  se o formato de data for americano utilizar a função dessa forma:
	 * 			$retorno = $data->diferencaEntreDatas(  '02-22-1999 13:29:00', '03-02-2009 11:25:00', 'tempoEntreDadas', 'string','mm/dd/yyyy');	
	 *  
	 */
	public function diferencaEntreDatas($dataInicial, $dataFinal, $tipo, $formato = NULL, $mascara){
		$dataInicial 	= $this->timeStampDeUmaData($dataInicial, $mascara);
		$dataFinal 		= $this->timeStampDeUmaData($dataFinal, $mascara);
		$resto 			= $dataFinal-$dataInicial;  //diferença entre as datas em segundos;
		$anos 			= $resto/31536000; 			//quantidade de anos
		$intAnos 		= floor($anos);
		$arrayRetorno 	= array();
		
		switch($tipo) {
			case 'maiorDataBolean':
				if($dataInicial > $dataFinal){
					return true;
				}else{
					return false;
				}
				break; 
			case 'tempoEntreDadas':
				if($intAnos >= 1){ //se houver anos
		            $strRetorno .= ($intAnos == 1) ? $intAnos.' ano, ' : $intAnos.' anos, ';  //mostra quantos anos passaram
		            $arrayRetorno[anos] = $intAnos;
		            $resto 		 = $resto-($intAnos*31536000); //retira do total, o tempo em segundos dos anos passados
		        }  
		        $meses 			= $resto/2628000; //quantidade de meses (anos/12 e não dias*30)
		        $intMeses 		= floor($meses);
		        
		        if($intMeses >= 1){  //se houver meses
		            $strRetorno .= ($intMeses == 1) ? $intMeses.' mês, ' : $intMeses.' meses, '; //mostra quantos meses passaram
		            $arrayRetorno[mes] = $intMeses;
		            $resto 		 = $resto-($intMeses*2628000); //retira do total, o tempo em segundos dos meses passados
		        }
		        
		        $semanas 		= $resto/604800; //quantidade de semanas
		        $intSemanas 	= floor($semanas);
		        
		        if($intSemanas >= 1){ //se houver semanas
		            $strRetorno .= ($intSemanas == 1) ? $intSemanas.' semana, ' : $intSemanas.' semanas, '; //mostra quantas semanas passaram
		            $arrayRetorno[semanas] = $intSemanas;
		            $resto 		 = $resto-($intSemanas*604800); //retira do total, o tempo em segundos das semanas passados
		        }
		                    
		        $dias 			= $resto/86400; //quantidade de dias
		        $intDias 		= floor($dias);
		        if($intDias >= 1){//se houver dias
		            $strRetorno .= ($intDias == 1) ? $intDias.' dia, ' : $intDias.' dias, '; //mostra quantos dias passaram
		            $arrayRetorno[dias] = $intDias;
		            $resto = $resto-($intDias*86400); //retira do total, o tempo em segundos dos dias passados
		        }
		                    
		        $horas 			= $resto/3600; //quantidade de horas
		        $intHoras 		= floor($horas);
		        if($intHoras >= 1){	//se houver horas
		            $strRetorno .= ($intHoras == 1) ? $intHoras.' hora, ' : $intHoras.' horas, '; //mostra quantas horas passaram
		            $arrayRetorno[horas] = $intHoras;
		            $resto 		 = $resto-($intHoras*3600); //retira do total, o tempo em segundos das horas passados
		        }
		                    
		        $minutos 		= $resto/60; //quantidade de minutos
		        $intMinutos 	= floor($minutos);
		        if($intMinutos >= 1){ //se houver minutos
		            $strRetorno .= ($intMinutos == 1) ? $intMinutos.' minuto, ' : $intMinutos.' minutos, '; //mostra quantos minutos passaram
		            $arrayRetorno[minutos] = $intMinutos;
		            $resto 		 = $resto-($intMinutos*60); //retira do total, o tempo em segundos dos minutos passados
		        }
		        
		        if($resto > 0) //mostra quantos minutos passaram
		            $strRetorno .= ceil($resto).' segundos, ';
		            $arrayRetorno[segundos] = ceil($resto);
		        	$strRetorno = rtrim($strRetorno, ', '); //retira a ultima virgula
		        	               
		        
		        $arrExplode 		= explode(',', $strRetorno);
		        $strRetornoFinal 	= '';
		        $nPedacos 			= count($arrExplode);
		        
		        for($i=0; $i<$nPedacos; $i++){
		        	$escreve = $strRetornoFinal ? ' e ' : '';
		            if($i == ($nPedacos-1))
		                $strRetornoFinal .= $escreve.$arrExplode[$i];//coloca "e" no lugar da ultima virgula
		            elseif($i == ($nPedacos-2))
		                $strRetornoFinal .= $arrExplode[$i];
		            else
		                $strRetornoFinal .= $arrExplode[$i].',';
		        }
		        if($formato == 'array'){
					return ($this->value	= $arrayRetorno);
				}else if($formato == 'string'){
					return ($this->value	= $strRetornoFinal);
				}
		
		        
				}
	}
	
	/**
	 * function inverteData 
	 * @description : retorna a data invertida (OBS: para a classe trabalhar com data o formato e YYYY-MM-DD 
	 * 				  caso o formato esteja invertido ele retorna o formato correto para a classe utilizar.)
	 * @param  : string $dataHora 
	 * @param  : string $mascara (utilizado para padrão americano) 
	 * @return : string $dataHora (data correta).
	 * @author : Thiago Tasca Barbosa
	 * @since  : 29/01/2009
	 * @internal : função privada (apenas as metodos da classe podem acessar está função)
	 */
	private function inverteData($dataHora = NULL, $mascara= NULL){
		if(!$dataHora){
			$dataHora	= $this->value;
		}
		if(!$mascara){
			$mascara	= $this->mascara;
		}

		if($formatoData = strpos($dataHora, "/")){ 
			$aryData = explode("/", $dataHora);
		}else{
			$aryData = explode("-", $dataHora);
		}
		
		$dataAno	= strlen ($aryData[0]);// quantidades de caracteres da 1º parte da data.
		$mascara = explode("/",$mascara);
		
		if($mascara[0] == "MM" or $mascara[0] == "mm"){
				$aryMinutos = explode(" ", $aryData[2]);
				$dia 		= $aryData[1];
				$mes 		= $aryData[0];
				$ano 		= $aryMinutos[0];
				$horas	 	= $aryMinutos[1];	
		}else{
			if( $dataAno != 4){ //Se não for ano o primeiro item da data inverte a mesma.
				$aryMinutos = explode(" ", $aryData[2]);
				$dia 		= $aryData[0];
				$mes 		= $aryData[1];
				$ano 		= $aryMinutos[0];
				$horas	 	= $aryMinutos[1];
			}else{
				return $dataHora;
			}
		}
		
		if($formatoData){
			$dataCorreta = $ano.'/'.$mes.'/'.$dia.' '.$horas;
		}else{
			$dataCorreta = $ano.'-'.$mes.'-'.$dia.' '.$horas;
		}
		
		if($dataCorreta){
			return ($this->value	= $dataCorreta);
		}
		
	}
	
	/**
	 * function acrescentaDiasNaData 
	 * @description : Acrescenta mais dias na data.
	 * @param  : string $data (Data passada)
	 * @param  : string $addDias (Quantos dias deseja acrescentar) 
	 * @return : string $dataFinal (data com a soma dos dias).
	 * @author : Thiago Tasca Barbosa
	 * @since  : 14/04/2009
	 */
	public function acrescentaDiasNaData($data,$addDias) {
	     $ano 		= substr ( $data, 0, 4 );
	     $mes 		= substr ( $data, 4, 2 );
	     $dias 		= substr ( $data, 6, 2 );
	     $dataFinal = mktime ( 0, 0, 0, $mes, $dias + $addDias, $ano );
	     return strftime("%Y%m%d", $dataFinal);
	}
	
	/**
	 * function subtraiDiasNaData 
	 * @description : Subitrai dias de uma data.
	 * @param  : string $data (Data passada)
	 * @param  : string $subDias (Quantos dias deseja acrescentar) 
	 * @return : string $dataFinal (data com a soma dos dias).
	 * @author : Thiago Tasca Barbosa
	 * @since  : 26/05/2009
	 */
	public function subtraiDiasNaData($data,$subDias) {
     $ano 		= substr ( $data, 0, 4 );
     $mes 		= substr ( $data, 4, 2 );
     $dias 		= substr ( $data, 6, 2 );
     $dataFinal = mktime ( 0, 0, 0, $mes, $dias - $subDias, $ano );
     return strftime("%Y%m%d", $dataFinal);
	}
	
	public function mesTextual($mes){
		$meses	= array("01" => "Janeiro",
						"02" => "Fevereiro",
						"03" => "Março",
						"04" => "Abril",
						"05" => "Maio",
						"06" => "Junho",
						"07" => "Julho",
						"08" => "Agosto",
						"09" => "Setembro",
						"10" => "Outubro",
						"11" => "Novembro",
						"12" => "Dezembro",
						"1" => "Janeiro",
						"2" => "Fevereiro",
						"3" => "Março",
						"4" => "Abril",
						"5" => "Maio",
						"6" => "Junho",
						"7" => "Julho",
						"8" => "Agosto",
						"9" => "Setembro",);
		return $meses[$mes];
	}
	
	function quantidadeDeDiasEntreDuasDatas($data1, $data2, $mascara = NULL){ 
		if(!$mascara){
			$mascara	= $this->mascara;
		}
		$data1 = $this->inverteData($data1, $mascara );
		$data2 = $this->inverteData($data2, $mascara );
		
  		return round((strtotime($data2) -  strtotime($data1)) / (24 * 60 * 60), 0); 
	} 


	

	
}
?>