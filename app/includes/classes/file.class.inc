<?php 
/**
 * Class Files
 * @description : Classe manipuladora de arquivos.
 * 				  Faz:  Upload  de arquivo
 * 						Download de arquivo
 * 						Move arquivos
 *						Copia arquivos
 * 						Cria arquivos
 * @version 	: 1.0
 */
class Files{

	public 	$arquivo;
	public 	$caminho;
	public 	$erro;
	private $chave;
	
	/**
	 * function setFile() 
	 * @description : Carrega o arquivo do Upload na variavel global da classe.
	 * @param 		: array FILE $arquivo (local do arquivo - opcional)
	 * @param 		: string $tipoExtencao (opcional)
	 * @param 		: string $key (opcional) - chave do aray $_FILES Ex: $_FILES[$key]
	 * @author 		: Thiago Tasca Barbosa
	 * @since  		: 10/03/2009
	 * @tutorial 	: Carrega o arquivo do tipo FILE.
	 * @version 	: 1.0
	 */
	protected function setFile( $arquivo = NULL, $tipoExtencao=NULL, $key = NULL ){
		if ( $key == NULL ){
			$this->verificaNomeDoUpload(); 	// Verifica o nome do campo que contêm arquivo.
		}else{
			$this->chave = $key; 	        // Atribui campo especifico.
		}
		if($_FILES[$this->chave]){ 		// Se existe arquivo carrega
			$arquivo = $_FILES[$this->chave];
		}else{ 
			$this->recuperaDadosArquivo($arquivo, $tipoExtencao);
			return true;
		}

        $arquivo["type"] = mime_content_type ( $arquivo['tmp_name'] );

		/*if($arquivo["type"] == 'image/pjpeg') { // Bug do IE para jpeg.
			$arquivo["type"] = 'image/jpeg';
		}*/
		$this->arquivo	= $arquivo; // Carrega o arquivo.
	}
	
	/**
	 * function setCaminho($caminho) 
	 * @description : Carrega o caminho onde será salvo o documento.
	 * @param 		: string $caminho
	 * @author : Thiago Tasca Barbosa
	 * @since  : 10/03/2009
	 * @tutorial : carrega a variavel global.
	 * @version 	: 1.0
	 */
	protected function setCaminho($caminho){
		$this->caminho = $caminho;
	}
	
	/**
	 * function verificaNomeDoUpload() 
	 * @description : Recupera o nome do campo onde tem o Arquivo.
	 * @author : Thiago Tasca Barbosa
	 * @since  : 10/03/2009
	 * @tutorial : Carrega o nome do campo do formulário.
	 * @version 	: 1.0
	 */
	private function verificaNomeDoUpload(){
		if(is_array($_FILES)){	
			$chave = key($_FILES);
		}
		return $this->chave = $chave;
	}
	
	/**
	 * function upload($caminho)
	 * @description : Faz o upload do arquivo da pasta temporaria para a pasta.
	 * @param  : string $caminho (caminho que será salvo o arquivo).
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @version 	: 1.0
	 * @tutorial : Caso seja imagem e o tamanho dela for muito grande, e alterado o tamanho da imagem para não ocorrer erro.
	 */
	protected function upload($caminho){
		switch($arquivo["type"]) {
			case 'image/jpeg':
				ini_set("memory_limit", "128M");
				list($width, $height) = getimagesize($arquivo['tmp_name']);
				$original_x = $width;
				$original_y = $height;
				// se a largura for maior que altura
				if($original_x > $original_y) {
	  	 			$porcentagem = (100 * 640) / $original_x;      
				}else {
	   				$porcentagem = (100 * 480) / $original_y;  
				}
				$tamanho_x = $original_x * ($porcentagem / 100);
				$tamanho_y = $original_y * ($porcentagem / 100);
				$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
				$image   = imagecreatefromjpeg($arquivo['tmp_name']);
				imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
				imagejpeg($image_p, $caminho, 100);
				//Clean-up memory
				ImageDestroy($image_p);
				//Clean-up memory
				ImageDestroy($image);
				break;
			default:
				if ( !move_uploaded_file( $this->arquivo["tmp_name"], $caminho) ) {
					return $this->erro = "Problemas no envio do arquivo.";
				}
			}
	}
	
	/**
	 * function setErro($erro)
	 * @description : Carrega o erro na variavel global.
	 * @param  : string $erro (Carrega o erro).
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @version 	: 1.0
	 */
	protected function setErro($erro){
		$this->erro = $erro; 
	}
	
	/**
	 * function getErro()
	 * @description : Retorna o erro na tela..
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @version 	: 1.0
	 */
	public function getErro(){
		if($this->erro){
			echo '<script type="text/javascript"> alert(" '.$this->erro.'");</script>';
		}
	}
	
	/**
	 * function Download($caminho)
	 * @description : Retorna o arquivo (Download do arquivo);
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @version 	: 1.0
	 */
	public function Download($caminho){
		if ( !is_file( $caminho ) ) {
	            $this->erro = "Arquivo não encontrado na pasta.";
	            return false;
	    }else{
	    	return true;
	    }    
	}
	
	/**
	 * function mover($origem, $destino)
	 * @description : Move o arquivo de uma pasta para outra;
	 * @param  : string $caminho (caminho onde está o arquivo).
	 * @param  : string $destino (caminho que será salvo o arquivo).
	 * @author : Thiago Tasca Barbosa
	 * @since  : 08/05/2009
	 * @version 	: 1.0
	 */
	protected function mover($origem, $destino = NULL){
		if(!$destino){
			$destino = $this->caminho;
		}
		if(file_exists($origem)){
			$passo1			= @copy($origem,$destino);
			$passo2			= @unlink($origem);
			$this->arquivo	= $destino;
			return true;
		}
		else{
			$this->erro = "Arquivo não encontrado na pasta de origem.";
			return false;
		}
	}	
	
	/**
	 * function recuperaDadosArquivo($arquivo, $tipoExtencao)
	 * @description : Se o $this->arquivo não for do tipo $_FILE a função simula o arquivo passado com do tipo $_FILE 
	 * @param  : string $arquivo (caminho onde está o arquivo).
	 * @param  : string $tipoExtencao
	 * @author : Thiago Tasca Barbosa
	 * @since  : 08/05/2009
	 * @version 	: 1.0
	 */
	public function recuperaDadosArquivo($arquivo, $tipoExtencao){
		if (file_exists($arquivo)) {
			$nomeExtencao	= end(explode("/", $arquivo));
			$tamanho 		= filesize($arquivo);
			$tipo 			= $tipoExtencao;
			$extencao 		= pathinfo($arquivo);
			$extencao		= $extencao["extension"];
		}else{
			$this->erro = "Arquivo não encontrado";
			return false;
		}
		$this->arquivo["name"] 		= $nomeExtencao;
		$this->arquivo["type"] 		= $tipo;
		$this->arquivo["size"] 		= $tamanho;
		$this->arquivo["extencao"] 	= $extencao;
		return true;
	}
	
	/**
	 * function criaArquivo($caminho, $conteudo)
	 * @description : Cria e escreve em um arquivo.
	 * @param  : string $caminho (arquivo criado).
	 * @param  : string $conteudo (conteudo que será colocado no arquivo).
	 * @author : Thiago Tasca Barbosa
	 * @since  : 08/05/2009
	 * @version 	: 1.0
	 */
	public function criaArquivo($caminho, $conteudo){
		if(!$criaArquivo = fopen ($caminho, "w+")){
			$this->erro = "Não foi possivel criar o arquivo.";
			return false;
		}
		if($criado = fwrite($criaArquivo, $conteudo)){
			fclose($criaArquivo);
			return true;
		}else{
			$this->erro = "Problemas ao escrever no arquivo.";
			return false;
		}
	}
	
	/**
	 * function deletaArquivo($arquivo)
	 * @description : Deleta o arquivo.
	 * @param  : string $arquivo (caminho + arquivo).
	 * @author : Thiago Tasca Barbosa
	 * @since  : 08/05/2009
	 * @version 	: 1.0
	 */
	public function deletaArquivo($arquivo){
		if(unlink($arquivo)){
			return true;
		}else{
			$this->erro = "Erro ao apagar o arquivo.";
			return false;
		}
		 
	}
	
	/**
	 * function criaPasta()
	 * @description : Cria uma pasta.
	 * @author : Thiago Tasca Barbosa
	 * @since  : 08/05/2009
	 * @version 	: 1.0
	 */
	private function criaPasta(){
		$caminho = $this->caminho;
		$pedaco 		= explode("/",$caminho);
		$caminhoAtual 	= "";
		foreach($pedaco as $dados){
			if($dados){
				$caminhoAtual .= $dados."/";
			}
			if($dados && !is_dir($caminhoAtual)) {
				mkdir($caminhoAtual, 0777);
			}
		}
	}


}

?>