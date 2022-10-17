<?php
/*
 Sistema Simec
 Setor responsável: SPO/MEC
 Desenvolvedor: Wesley Romualdo da Silva
 Analista: Renan Mendes Gaya Lopes dos Santos
 Programador: Wesley Romualdo da Silva
 Módulo: emenda
 Finalidade: importar arquivos txt do FNDE
 Data de criação: 24/09/2009
 */
/*include_once( APPRAIZ. "emenda/classes/validaCPFCNPJ.class.inc" );*/
//include_once( APPRAIZ. "www/wiki/install-utils.inc" );

class importacaoHabilitaFNDE {
	private $log = "";
	private $nomeArquivo;
	private $linhaArquivo;
	private $nomeArquivoLog = "";
	private $erroLinha = false;
	private $arLinhaErro = "";
	private $hloid;
	private $totalregistroprocessado = 0;
	private $msgErroNumeric = "Não é um valor numérico";
	private $msgErroDate = "Não é data valida";
	private $msgErroCNPJ = "Não é um CNPJ valido";
	private $msgErroArquivo = "Arquivo Imcompleto";
	private $linha;
	private $milesegundo = 0;
	private $hdocnpj;
	private $hencnpj;
	private $hdicnpj = array();
	static $oHandler = null;
	
	function __construct($diretorio, $arTipo ){
		$this->hdocnpj = array();
		$this->hencnpj = array();

		//extrair os arquivos .zip
		//enviar_email(array("nome" => "SIMEC EMENDAS","email" => "wesley.silva@mec.gov.br"), "alexandre.dourado@mec.gov.br", "Start do script", "TESTE", "alexandre.dourado@mec.gov.br");
		$arquivoZip = $this->retornaArquivosDiretorio($diretorio, 'zip');
		/*ob_start();
        echo "<pre>";
		print_r($arquivoZip);
        $saida = ob_get_contents();
        ob_clean();
        enviar_email(array("nome" => "SIMEC EMENDAS","email" => "wesley.silva@mec.gov.br"), "alexandre.dourado@mec.gov.br", "1º Passo do script", $saida, "alexandre.dourado@mec.gov.br");*/
		
		if($arquivoZip){
			foreach($arquivoZip as $listar){
		   		if(!$this->extrairArquivosZIP($listar)){
		   			$this->nomeArquivoLog = 'log_'.date('YmdHisu').$this->milesegundo.'_Habilita.txt';
		   			$this->addLog('Extraindo arquivos da habilitação', 'Erro de extração de arquivo', $listar );
		   			$this->gravaLogTxt(DIRETORIO_LOG_HABILITA);
		   		}
			}
			//manipular os arquivos .txt
			$i = 0;
			$arquivo = $this->retornaArquivosDiretorio(DIRETORIO_PROCESSANDO_HABILITA, 'txt');		
			if($arquivo && $arquivoZip){
				foreach($arquivo as $listar){
					$this->deletaArquivoHabilita(DIRETORIO_ORIGEM_HABILITA, $arquivoZip[$i], DIRETORIO_PROCESSANDO_HABILITA, $listar);
			   		$this->importar($listar, DIRETORIO_PROCESSANDO_HABILITA, $arTipo);
			   		$i++;
				}
	
			}
		} else {
			$this->nomeArquivoLog = 'log_'.date('YmdHisu').$this->milesegundo.'_Habilita.txt';
			$this->addLog('Extraindo arquivos da habilitação', 'Erro de extração de arquivo - Arquivo(s) não encontrado(s)', '' );
		   	$this->gravaLogTxt(DIRETORIO_LOG_HABILITA);
		}
	}
	
	function __destruct(){
		$this->milesegundo = 0;
	}
	
	function init(){
		global $db;
		$sql = "INSERT INTO 
				  emenda.habilitalog(
				  hlonomearquivo,
				  hlodatainicial,
				  hlostatus
				) 
				VALUES (
				  '".$this->nomeArquivo."',
				  now(),
				  'Em Processamento'
				)RETURNING hloid";
		$this->hloid = $db->pegaUm($sql);
		$db->commit();
	}
	
	private function gravaLogProcessamento($arHeader, $arTrailler){
		global $db;
		
		if($this->log){
			if ($this->totalregistroprocessado == 0){
				$status = 'Não Processado';
				$nomeArqLog = $this->nomeArquivoLog;
			} else {
				$status = 'Processado com Erro';
				$nomeArqLog = $this->nomeArquivoLog;
			}
		}else{
			if($this->totalregistroprocessado != $arTrailler[1]){
				$status = 'Processado com Erro';
				$nomeArqLog = $this->nomeArquivoLog;
			} else if ($this->totalregistroprocessado == 0){
				$status = 'Não Processado';
				$nomeArqLog = $this->nomeArquivoLog;
			} else {
				$status = 'Processado com Sucesso';
				$nomeArqLog = "";
			}
		}
				
		$sql = "UPDATE 
				  emenda.habilitalog  
				SET 
				  hlosequecial = ".$arHeader[0].",
				  hlodatafinal = now(),
				  hlototallinhas = ".$arTrailler[0].",
				  hlototalregistro = ".$arTrailler[1].",
				  hlototalregistroprocessado = ".$this->totalregistroprocessado.",
				  hlonomearquivolog = '".$nomeArqLog."',
				  hlostatus = '$status'
				 
				WHERE 
				  hloid = ".$this->hloid;

		$db->executar($sql);
		return $db->commit();
	}
	function finaliza(){		
		$this->nomeArquivo = null;
		$this->linhaArquivo = 0;
		$this->totalregistroprocessado = null;
		$this->log = array();
		$this->nomeArquivoLog = "";
		$this->hloid = null;
		$this->linha = null;
		$this->hdocnpj = array();
		$this->hdicnpj = array();
		$this->hencnpj = array();
	}
	
	function importar($arquivo, $diretorio, $arTipo){
		$data = explode('_', $arquivo);
		$arq = explode('.', $data[1]);
		
		if( !in_array( $arq[0], $arTipo ) ) return 'false';
		
		try{			
			$this->milesegundo++;
			$this->linhaArquivo = 0;
			$this->nomeArquivo = $arquivo;
			$dataArquivo = substr($data[0], 0, 8);
			$horaArquivo = substr($data[0], 8, strlen($data[0]));
			
			$arquivo = $diretorio.'/'.$arquivo;
			
			if($this->verificaArquivoProcessamento() == 0){
				$this->init();
				$this->nomeArquivoLog = $data[0].'_'.$arq[0].'_'.$this->hloid.'.log';
				
				switch ($arq[0]){
					case 'CadastroEntidade':
						$this->importaEntidade($arquivo);
						$this->copiaArquivoHabilita();
						$this->deletaArquivoHabilita($diretorio, $this->nomeArquivo, DIRETORIO_DESTINO_HABILITA, $this->nomeArquivo);
						break;
					case 'DocumentosHabilitacao':
						$this->importaDocumento($arquivo);
						$this->copiaArquivoHabilita();
						$this->deletaArquivoHabilita($diretorio, $this->nomeArquivo, DIRETORIO_DESTINO_HABILITA, $this->nomeArquivo);
						break;
					case 'Diligencia':
						$this->importaDiligencia($arquivo);
						$this->copiaArquivoHabilita();
						$this->deletaArquivoHabilita($diretorio, $this->nomeArquivo, DIRETORIO_DESTINO_HABILITA, $this->nomeArquivo);
						break;		
				}
			} else {
				$this->nomeArquivoLog = $arq[0].'.log';
				chmod(DIRETORIO_PROCESSANDO_HABILITA.'/'.$this->nomeArquivo, 0755);
				unlink(DIRETORIO_PROCESSANDO_HABILITA.'/'.$this->nomeArquivo);
				$this->addLog('Processando arquivo '.$this->nomeArquivo , 'Arquivo '.$this->nomeArquivo.' ja foi processado!', '');
			}
		} catch (Exception $e){
			//ver('Exception');
			//$this->addLog('Erro de Processamento do Arquivo', $e->getMessage(), '');
		}
		
		$this->gravaLogTxt(DIRETORIO_LOG_HABILITA);
		$this->finaliza();
	}
	private function verificaArquivoProcessamento(){
		global $db;
		
		$sql = "SELECT 
				  count(hloid) as total
				FROM 
				  emenda.habilitalog
				WHERE
				  hlonomearquivo = '$this->nomeArquivo'";
		//return $db->pegaUm($sql);
		return false;
	}
	
	/*
	 * Funções Referente ao Cadastro de Entidade
	*/
	private function importaEntidade($arquivo){

		if( $fh = fopen($arquivo, 'r') ){
			
			$linha = array();
			$arHeader = array();
			$arDetalhe = array();
			$arTrailler = array();
			
			while($linha = fgets($fh)){
				$l = trim(substr($linha, 0, 1));
				
				$this->linhaArquivo++;
				$this->linha = trim($linha);

				if($l == HEADERFNDE){
					$arHeader = $this->montaHeaderEntidade($linha);
				} else if ( $l == DETALHEFNDE ){
					$arDetalhe[] = $this->montaDetalheEntidade($linha);
				} else if ( $l == TRAILLERFNDE){
					$arTrailler = $this->montaTraillerEntidade($linha);
				}
					
			}
			
			if( sizeof($arHeader) == 0 ){
				$this->vadidaRegistroArquivo('Header não encontrado no arquivo', 'arquivo', 'Processando HEADER do arquivo', false);
			}
			if( sizeof($arTrailler) == 0 ){
				$this->vadidaRegistroArquivo('Trailler não encontrado no arquivo', 'arquivo', 'Processando TRAILLER do arquivo', false);
			}
			if( sizeof($arDetalhe) == 0 ){
				$this->vadidaRegistroArquivo('Detalhe não encontrado no arquivo', 'arquivo', 'Processando DETALHE do arquivo', false);
			} else {
				$this->insereEntidade($arDetalhe);
			}
			$this->gravaLogProcessamento($arHeader, $arTrailler);
			fclose($fh);
		}		
	}
	private function montaHeaderEntidade($linha){
	
					trim(substr($linha, 0, 1))/ //Tipo_registro
					$this->vadidaRegistroArquivo( trim(substr($linha, 1, 6)), 'numeric', 'Numero Sequencial da linha', true );
		$lines[] = sprintf( "%s" , implode( "," , array(
					$this->vadidaRegistroArquivo( trim(substr($linha, 7, 5)), 'numeric', 'Numero de seqüência do arquivo', false )
					))
				);
					$this->vadidaRegistroArquivo( trim(substr($linha, 12, 30)), 'string', 'Nome do arquivo ', false );
					$this->vadidaRegistroArquivo( trim(substr($linha, 42, 8)), 'date', 'Data de geração do arquivo', true );
					$this->vadidaRegistroArquivo( trim(substr($linha, 50, 6)), 'numeric', 'Hora, minuto e segundo de geração do arquivo', true );

		unset($this->linha);
		if($this->erroLinha){
			$this->erroLinha = false;
			return '';
		} else {
			return $lines;
		}
	}
	
	private function montaDetalheEntidade($linha){
					//trim(substr($linha, 0, 1)), //Tipo_registro
					$this->vadidaRegistroArquivo( trim(substr($linha, 1, 6)), 'numeric', 'Numero Sequencial da linha', true ); //Numero Sequencial da linha
		$lines = sprintf( "(%s)" , implode( "," , array(
					$this->vadidaRegistroArquivo( trim(substr($linha, 7, 14)), 'cnpj', 'Número do CNPJ da entidade', true ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 21, 60)), 'string', 'Nome da entidade', true ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 81, 50)), 'string', 'Nome do Municipio', true ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 131, 2)), 'string', 'Sigla da UF', true ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 133, 17)), 'string', 'Número do processo', true ), 
					$this->vadidaRegistroArquivo( trim(substr($linha, 150, 11)), 'numeric', 'Numero do Documenta(Tramita)', true ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 161, 1)), 'numeric', 'DV do numero do', true ), 
					$this->vadidaRegistroArquivo( trim(substr($linha, 162, 60)), 'string', 'Descrição da situação', true ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 222, 8)), 'date', 'Data do cadastramento do processo', true ),
					'now()'
					))
				);
				
		unset($this->linha);
		if($this->erroLinha){
			$this->erroLinha = false;
			return '';
		} else {
			return $lines;
		}
	}
	
	private function montaTraillerEntidade($linha){
					trim(substr($linha, 0, 1)); //Tipo_registro
					$this->vadidaRegistroArquivo( trim(substr($linha, 1, 6)), 'numeric', 'Numero Sequencial da linha', true );
		$lines[] = sprintf( "%s" , implode( "," , array(
					$this->vadidaRegistroArquivo( trim(substr($linha, 7, 7)), 'numeric', 'Quantidade total de linhas', true )
					))
				);
		$lines[] = sprintf( "%s" , implode( "," , array(
					$this->vadidaRegistroArquivo( trim(substr($linha, 14, 7)), 'numeric', 'Quantidade total de Registros', true )
					))
				);
		
		unset($this->linha);
		if($this->erroLinha){
			$this->erroLinha = false;
			return '';
		} else {
			return $lines;
		}
	}
	
	/*
	 * Funções Referente ao Documentos Habilitacao
	*/
	private function importaDocumento($arquivo){

		if( $fh = fopen($arquivo, 'r') ){
			
			$linha = array();
			$arHeader = array();
			$arDetalhe = array();
			$arTrailler = array();
			
			while($linha = fgets($fh)){
				$l = trim(substr($linha, 0, 1));
				$this->linhaArquivo++;
				$this->linha = $linha;
					
				if($l == HEADERFNDE){
					$arHeader = $this->montaHeaderDocumento($linha);
				} else if ( $l == DETALHEFNDE ){
					$arDetalhe[] = $this->montaDetalheDocumento($linha);
				} else if ( $l == TRAILLERFNDE){
					$arTrailler = $this->montaTraillerDocumento($linha);
				}
			}
		}
		if(sizeof($arHeader) == 0 ){
			$this->vadidaRegistroArquivo('Header não encontrado no arquivo', 'arquivo', 'Processando HEADER do arquivo', false);
		}
		if(sizeof($arDetalhe) == 0 ){
			$this->vadidaRegistroArquivo('Detalhe não encontrado no arquivo', 'arquivo', 'Processando DETALHE do arquivo', false);
		}
		if(sizeof($arTrailler) == 0 ){
			$this->vadidaRegistroArquivo('Trailler não encontrado no arquivo', 'arquivo', 'Processando TRAILLER do arquivo', false);
		} else {
			$this->insereDocumento($arDetalhe);
		}

		$this->gravaLogProcessamento($arHeader, $arTrailler);
		fclose($fh);
	}
	
	private function montaHeaderDocumento($linha){
					trim(substr($linha, 0, 1)); //Tipo_registro
					$this->vadidaRegistroArquivo( trim(substr($linha, 1, 6)), 'numeric', 'Numero Sequencial da linha', true );
					$this->vadidaRegistroArquivo( trim(substr($linha, 12, 30)), 'string', 'Nome do arquivo ', false );
					$this->vadidaRegistroArquivo( trim(substr($linha, 42, 8)), 'date', 'Data de geração do arquivo', false );
					$this->vadidaRegistroArquivo( trim(substr($linha, 50, 6)), 'numeric', 'Hora, minuto e segundo de geração do arquivo', false );
		$lines[] = sprintf( '%s' , implode( ',' , array(
					$this->vadidaRegistroArquivo( trim(substr($linha, 7, 5)), 'numeric', 'Numero de seqüência do arquivo', false )
					))
				);

		unset($this->linha);
		if($this->erroLinha){
			$this->erroLinha = false;
			return '';
		} else {
			return $lines;
		}
	}
	
	private function montaDetalheDocumento($linha){
					//trim(substr($linha, 0, 1)), //Tipo_registro
					$this->vadidaRegistroArquivo( trim(substr($linha, 1, 6)), 'numeric', 'Numero Sequencial da linha', true );
		$lines = sprintf( "(%s)" , implode( "," , array(
					$this->vadidaRegistroArquivo( trim(substr($linha, 7, 14)), 'cnpj', 'Número do CNPJ da entidade', true ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 21, 600)), 'string', 'Descrição do documento', true ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 621, 20)), 'string', 'Descrição reduzida do documento', false ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 641, 1)), 'string', 'Status de validade', false ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 642, 8)), 'date', 'Data da validade', false ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 650, 50)), 'string', 'Descrição da situação do documento de habilitação', true ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 700, 250)), 'string', 'Descrição do erro do documento de habilitação', false ),
					'now()' 
					))
				);
		
		unset($this->linha);
		if($this->erroLinha){
			$this->erroLinha = false;
			return '';
		} else {
			return $lines;
		}
	}
	
	private function montaTraillerDocumento($linha){
					trim(substr($linha, 0, 1)); //Tipo_registro
					$this->vadidaRegistroArquivo( trim(substr($linha, 1, 6)), 'numeric', 'Numero Sequencial da linha', false );
		$lines[] = sprintf( '%s' , implode( ',' , array(
					$this->vadidaRegistroArquivo( trim(substr($linha, 7, 7)), 'numeric', 'Quantidade total de linhas', false )  
					)));
		$lines[] = sprintf( '%s' , implode( ',' , array(
					$this->vadidaRegistroArquivo( trim(substr($linha, 14, 7)), 'numeric', 'Quantidade total de Registros', false )  
					)));
		
		unset($this->linha);		
		if($this->erroLinha){
			$this->erroLinha = false;
			return '';
		} else {
			return $lines;
		}
	}
	
	/*
	 * Funções Referente ao Diligencia
	*/
	private function importaDiligencia($arquivo){
		
		if( $fh = fopen($arquivo, 'r') ){
			
			$linha = array();
			$arHeader = array();
			$arDetalhe = array();
			$arTrailler = array();
			
			while($linha = fgets($fh)){
				$l = trim(substr($linha, 0, 1));
				$this->linhaArquivo++;
				
				$this->linha = $linha;
				
				if($l == HEADERFNDE){
					$arHeader = $this->montaHeaderDiligencia($linha);
				} else if ( $l == DETALHEFNDE ){
					$arDetalhe[] = $this->montaDetalheDiligencia($linha);
				}  else if ( $l == TRAILLERFNDE){
					$arTrailler = $this->montaTraillerDiligencia($linha);
				}
			}
		}
		
		if(sizeof($arHeader) == 0 ){
			$this->vadidaRegistroArquivo('Header não encontrado no arquivo', 'arquivo', 'Processando HEADER do arquivo', false);
		}
		if(sizeof($arTrailler) == 0 ){
			$this->vadidaRegistroArquivo('Trailler não encontrado no arquivo', 'arquivo', 'Processando TRAILLER do arquivo', false);
		}
		if(sizeof($arDetalhe) == 0 ){
			$this->vadidaRegistroArquivo('Detalhe não encontrado no arquivo', 'arquivo', 'Processando DETALHE do arquivo', false);
		}else{
			$this->insereDiligencia($arDetalhe);
		}
		
		$this->gravaLogProcessamento($arHeader, $arTrailler);
		fclose($fh);
		
	}
	
	private function montaHeaderDiligencia($linha){
					trim(substr($linha, 0, 1)); //Tipo_registro
					$this->vadidaRegistroArquivo( trim(substr($linha, 1, 6)), 'numeric', 'Numero Sequencial da linha', true );
					$this->vadidaRegistroArquivo( trim(substr($linha, 12, 30)), 'string', 'Nome do arquivo ', false );
					$this->vadidaRegistroArquivo( trim(substr($linha, 42, 8)), 'date', 'Data de geração do arquivo', false );
					$this->vadidaRegistroArquivo( trim(substr($linha, 50, 6)), 'numeric', 'Hora, minuto e segundo de geração do arquivo', false );
		$lines[] = sprintf( '%s' , implode( ',' , array(
					$this->vadidaRegistroArquivo( trim(substr($linha, 7, 5)), 'numeric', 'Numero de seqüência do arquivo', false )
					))
				);
				
		unset($this->linha);
		if($this->erroLinha){
			$this->erroLinha = false;
			return '';
		} else {
			return $lines;
		}
	}
	
	private function montaDetalheDiligencia($linha){
					//trim(substr($linha, 0, 1)), //Tipo_registro
					$this->vadidaRegistroArquivo( trim(substr($linha, 1, 6)), 'numeric', 'Numero Sequencial da linha', true );					
		$lines = sprintf( '(%s)' , implode( ',' , array(
					$this->vadidaRegistroArquivo( trim(substr($linha, 7, 14)), 'cnpj', 'Número do CNPJ da entidade', true ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 21, 5)), 'numeric', 'Numero da diligencia', false ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 26, 8)), 'date', 'Data da emissão', false ),					
					$this->vadidaRegistroArquivo( trim(substr($linha, 34, 3999)), 'string', 'Observação da diligencia', false ),
					$this->vadidaRegistroArquivo( trim(substr($linha, 4033, 44)), 'string', 'Nome do usuário', false ), 
					$this->vadidaRegistroArquivo( trim(substr($linha, 4077, 600)), 'string', 'Descrição do documento', false ), 
					$this->vadidaRegistroArquivo( trim(substr($linha, 4676, 20)), 'string', 'Descrição reduzida do documento', false ),
					'now()',
					$this->vadidaRegistroArquivo( trim(substr($linha, 4696, 250)), 'string', 'Descrição do erro do documento de habilitacao', false )
					))
				);
				
		unset($this->linha);
		if($this->erroLinha){
			$this->erroLinha = false;
			return '';
		} else {
			return $lines;
		}
	}
	
	private function montaTraillerDiligencia($linha){
		trim(substr($linha, 0, 1)); //Tipo_registro
		$this->vadidaRegistroArquivo( trim(substr($linha, 1, 6)), 'numeric', 'Numero Sequencial da linha', true );
					
		$lines[] = sprintf( '%s' , implode( "," , array(
					$this->vadidaRegistroArquivo( trim(substr($linha, 7, 7)), 'numeric', 'Quantidade total de linhas', true )  
					))
				);
		$lines[] = sprintf( '%s' , implode( "," , array(
					$this->vadidaRegistroArquivo( trim(substr($linha, 14, 7)), 'numeric', 'Quantidade total de Registros', true )  
					))
				);
		
		unset($this->linha);
		if($this->erroLinha){
			$this->erroLinha = false;
			return '';
		} else {
			return $lines;
		}
	}
	
	private function vadidaRegistroArquivo($registro, $tipo, $dsc, $obrigatorio){

		if($registro == "")
		{
			if($obrigatorio)
			{
				$this->addLog($dsc, "Campo obrigatório não informado", $registro, $this->linha, $this->linhaArquivo);
				$this->erroLinha = true;
			}
			else
			{
				return "NULL";
			}
		}
		else
		{
			switch($tipo){
				case 'numeric' :
					if(!is_numeric($registro)){
						$this->addLog($dsc, $this->msgErroNumeric, $registro, $this->linha, $this->linhaArquivo);
						$this->erroLinha = true;
					}
					return (integer) $registro;
						
					break;
				case 'date' :
					$dia = substr($registro, 0, 2);
					$mes = substr($registro, 2, 2);
					$ano = substr($registro, 4, 4);
					
					$data = "'".$ano.'-'.$mes.'-'.$dia."'";
					
					if(!checkdate($mes, $dia, $ano)){
						$registro = $ano.'-'.$mes.'-'.$dia;
						$this->addLog($dsc, $this->msgErroDate, $registro, $this->linha, $this->linhaArquivo);
						$this->erroLinha = true;
					}
					return $data;
	
					break;
				case 'string':
					//return "'".iconv("iso-8859-1", "utf-8", pg_escape_string($registro) )."'";
					return "'".$registro."'";	
					break;
				case 'cnpj' :
					/*$valida = new validar;
					$cnpj = formatar_cnpj($registro);
					if( $valida->cnpj($cnpj) == 'false' ){
						$this->addLog($dsc, $this->msgErroCNPJ, $registro, $this->linha, $this->linhaArquivo);
						$this->erroLinha = true;
					}*/
					
					if(!is_numeric($registro)){
						$this->addLog($dsc, $this->msgErroNumeric, $registro, $this->linha, $this->linhaArquivo);
						$this->erroLinha = true;
					}					
					return "'".$registro."'";				
					break;
				case 'arquivo':
					$this->addLog($dsc, $this->msgErroArquivo, $registro, $this->linha, $this->linhaArquivo);
					break;
			}
		}
	}
	
	private function extrairArquivosZIP($arquivo){

		$zip = new ZipArchive();
		$arquivo = DIRETORIO_ORIGEM_HABILITA.'/'.$arquivo;
		
		if($zip->open($arquivo) === true ){
			$zip->extractTo(DIRETORIO_PROCESSANDO_HABILITA);
			$zip->close();
			
			return true;
		} else {
			return false;
		}
	}
	
	/*
	 * Funções retorna os arquivos do diretorio informado
	*/
	private function retornaArquivosDiretorio($dir, $tipo){
		$itens = array();

		if( file_exists($dir) ){
			$diretorio = opendir($dir);
			// monta os vetores com os itens encontrados na pasta
			while($arquivo = readdir($diretorio)){
				$itens[] = $arquivo;
			}
			// ordena o vetor de itens
			sort($itens);
			
			// percorre o vetor para fazer a separacao entre arquivos e pastas 
			foreach ($itens as $lista) {
				if($lista != '.' && $lista != '..'){
					$t = explode('.', $lista);
					
					// checa se o tipo de arquivo encontrado é uma pasta
					if(is_file($dir.'/'.$lista)){
						//arquivo
						if($t[1] == $tipo){
							$arquivo[] = $lista;
						}
					}else{
						//pasta
					}
				}
			}
			return $arquivo;
		}else {
			return false;
		}
	}

	/**
	 * Insere dados da Entidade
	 *
	 * @param array $arDados
	 * @return void
	 */
	private function insereEntidade( $arDados){
		global $db;
		
		if($arDados){
			foreach ($arDados as $value) {
				if($value){
					$hencnpj = substr($value, 2, 14);
					
					if( !in_array($hencnpj, $this->hencnpj) ){
							$sql = "DELETE FROM 
									  emenda.habilitaentidade 
									WHERE 
									  hencnpj = '$hencnpj'";
									  
							$db->executar($sql);
							$this->hencnpj[] = $hencnpj;
					}
					
					$this->totalregistroprocessado++;
					$sql = "INSERT INTO 
							  emenda.habilitaentidade(
							  hencnpj,
							  henrazaosocial,
							  mundescricao,
							  estuf,
							  henprocesso,
							  hendocumenta,
							  hendivdocumenta,
							  hensituacao,
							  hendatacadastramento,
							  hendataimportacao
							) 
							VALUES 
							  $value
							";
								  
					$db->executar($sql);
				}
			}
			return $db->commit();
		}
	}
	/**
	 * Verifica se a entidade está cadastrada
	 *
	 * @param string $hencnpj
	 * @return void
	 */
	private function buscaEntidadeHabilitacao($hencnpj){
		global $db;
		
		$sql = "SELECT 
				  count(henid) as total
				FROM 
				  emenda.habilitaentidade
				WHERE
				  hencnpj = '$hencnpj'";

		return $db->pegaUm($sql);
	}
	/**
	 * Insere dados do Documento
	 *
	 * @param array $arDados
	 * @return void
	 */
	private function insereDocumento( $arDados){
		global $db;
		
		if($arDados){
			foreach ($arDados as $value) {
				if($value){
					$hdocnpj = substr($value, 2, 14);
					
					if( !in_array($hdocnpj, $this->hdocnpj) ){
							$sql = "DELETE FROM 
									  emenda.habilitadocumento 
									WHERE 
									  hdocnpj = '$hdocnpj'";
									  
							$db->executar($sql);
							$this->hdocnpj[] = $hdocnpj;
					}					
					
					$this->totalregistroprocessado++;
								
					$sql = "INSERT INTO 
							  emenda.habilitadocumento(
							    hdocnpj,
								hdodsc,
								hdodscreduzido,
								hdopossuivalidade,
								hdodatavalidade,
								hdosituacao,
								hdodescricaoerro,
								hdodataimportacao
							) 
							VALUES 
							  $value
							";
					$db->executar( $sql );
				}
			}
			return $db->commit();
		}
	}
	private function buscaDocumentoHabilitacao($hdocnpj){
		global $db;
		
		$sql = "SELECT 
				  count(hdoid) as total
				FROM 
				  emenda.habilitadocumento
				WHERE
				  hdocnpj = '$hdocnpj'";

		return $db->pegaUm($sql);
	}
	/**
	 * Insere dados da Diligencia
	 *
	 * @param array $arDados
	 * @return void
	 */
	private function insereDiligencia( $arDados){
		global $db;	
		if($arDados){
			foreach ($arDados as $value) {
				if($value){
					$hdicnpj = substr($value, 2, 14);
					if( !in_array($hdicnpj, $this->hdicnpj) ){
							$sql = "DELETE FROM 
									  emenda.habilitadiligencia 
									WHERE 
									  hdicnpj = '$hdicnpj'";
									  
							$db->executar($sql);
							$this->hdicnpj[] = $hdicnpj;
					}
					$sql = "INSERT INTO 
							  emenda.habilitadiligencia(
							  hdicnpj,
							  hdicod,
							  hdidataemissao,
							  hdidescricao,
							  hdiusunome,
							  hdodsc,
							  hdodscreduzido,
							  hdidataimportacao,
							  hdidescricaoerro
							) 
							VALUES  
							  $value
							";
					$db->executar($sql);
					$this->totalregistroprocessado++;			
					
				}
			}
			return $db->commit();			
		}
	}
	private function buscaDiligenciaHabilitacao($hdicnpj){
		global $db;
		
		$sql = "SELECT 
				  count(hdiid) as total
				FROM 
				  emenda.habilitadiligencia
				WHERE
				  hdicnpj = '$hdicnpj'";

		return $db->pegaUm($sql);
	}
	
	/**
	 * Função grava log de erro em arquivo txt
	 *
	 * @param array $diretorio
	 * @return void
	 */
	private function gravaLogTxt($diretorio){

		if( !empty($this->log) ){
			
			$arquivo = $diretorio.'/'.$this->nomeArquivoLog;
			
			$arq = fopen($arquivo, 'a+');
			foreach ($this->log as $arLog) {
				fwrite($arq, 'Linha: ' . $arLog['linha']."\n");	
				fwrite($arq, 'Descrição: ' . $arLog['descricao']."\n");	
				fwrite($arq, 'Erro: ' . $arLog['erro']."\n");		
				fwrite($arq, 'Registro com Erro: ' . ($arLog['registroErro'] ? $arLog['registroErro'] : 'null') ."\n");	
				fwrite($arq, 'Linha com Erro: ' . ($arLog['linhaComErro'] ? $arLog['linhaComErro'] : '0')."\n"."\n");	
			}
			fclose($arq);
		}
	}
	/**
	 * Função gera log de erro
	 * 
	 * @return void
	 */
	 private function addLog($desc, $erro, $registro, $linha="", $numeroLinha="" ){
		$this->log[] = array("descricao" => $desc,
							 "erro" => $erro,
							 "linha" => $numeroLinha,
							 "registroErro" => $registro,
							 "linhaComErro" => $linha
						);	
		//$this->erroLinha = false;
	}
	
	public function pegaErroException($errno, $errstr, $errfile, $errline){
	    //throw new Exception($errstr, $errno);
	    //ver( $errstr, $errno);
	}
	
	public function copiaArquivoHabilita(){		
		$arquivoOrigem = DIRETORIO_PROCESSANDO_HABILITA;
		//$arquivoOrigem = str_replace('\\', '/', $arquivoOrigem);
		
		$arquivoDestino = DIRETORIO_DESTINO_HABILITA;
		//$arquivoDestino = str_replace('\\', '/', $arquivoDestino);
				
		$this->copyfileto($arquivoOrigem, $this->nomeArquivo, $arquivoDestino, $this->nomeArquivo);
	}
	
	function deletaArquivoHabilita($diretorio='', $arquivoO='', $destino='', $arquivoD=''){		
		
		$arquivoOrigem = $diretorio."/".$arquivoO;
		$arquivoOrigem = str_replace('\\', '/', $arquivoOrigem);
		
		$arquivoDestino = $destino."/".$arquivoD;
		$arquivoDestino = str_replace('\\', '/', $arquivoDestino);

		//chmod($arquivoOrigem, 0755);
		if(file_exists($arquivoDestino) ){
			unlink($arquivoOrigem);
		}
	}	
	function copyfileto( $sdir, $sname, $ddir, $dname, $perms = 0664 ) {
		global $wgInstallOwner, $wgInstallGroup;
	
		$d = "{$ddir}/{$dname}";
		if ( copy( "{$sdir}/{$sname}", $d ) ) {
			if ( isset( $wgInstallOwner ) ) { chown( $d, $wgInstallOwner ); }
			if ( isset( $wgInstallGroup ) ) { chgrp( $d, $wgInstallGroup ); }
			chmod( $d, $perms );
			# print "Copied \"{$sname}\" to \"{$d}\".\n";
		} else {
			print "Failed to copy file \"{$sname}\" to \"{$ddir}/{$dname}\".\n";
			exit();
		}
	}
	
	public function enviaEmail(){
		/*foreach ($this->log as $arLog) {
			fwrite($arq, 'Linha: ' . $arLog['linha']."\n");	
			fwrite($arq, 'Descrição: ' . $arLog['descricao']."\n");	
			fwrite($arq, 'Erro: ' . $arLog['erro']."\n");		
			fwrite($arq, 'Registro com Erro: ' . $arLog['registroErro']."\n");	
			fwrite($arq, 'Linha com Erro: ' . $arLog['linhaComErro']."\n"."\n");
		}*/
		
		
	}
	
}


?>