<?php
include_once APPRAIZ . "includes/classes/file.class.inc";
/**
 * Class FilesSimec
 * @description : Classe manipuladora de Arquivos. Salva dos dados referente aos arquivos nas tabelas do bando de dados
 * @category file (manipuladora de arquivos exclusiva esquema SIMEC)
 * @version 1.0
 * @author Thiago Tasca Barbosa
 * @since 03/03/2009
 * @package: file.class.inc ( Classe Files() ).
 * @tutorial: O SIMEC trabalha da seguinte forma para gravaчуo de arquivos pelo sistema:
 * 				- 1К Salva as informaчѕes dos arquivos na tabela public.arquivo
 * 				- 2К Salva a chave primaria (arqid) da tabela  public.arquivo na tabela do esquema desejado para referencia do arquivo.
 * 				- 3К Salva o arquivo no caminho informado, utilizando a classe file.class.inc
 * @example : include_once APPRAIZ . "includes/classes/file.class.inc";
 *			  include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
 * 			  $file = new FilesSimec("Tabela do Esquema", "Array de campos da tabela" ,"Esquema");
 */
class FilesSimec extends Files {

	private $db;
	private $nomeEsquema;
	private $idSistema;
	private $tabela;
	private $idArquivo;
	private $campos;

	/**
	 * function __construct
	 * @description : Mщtodo construtor da classe.
	 * @param  : string $tabela
	 * @param  : array  $campos
	 * @param  : string $nomeSistema
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial :
	 * 	Existe 3 parametros a serem passados:
	 * 	Sendo o 1К: Tabela do esquema.
	 * 			2К: Campos da tabela do esquema.
	 * 			3К: Nome do Sistema (nуo obrigatorio).
	 *
	 * 	A funчуo construtora carrega os dados minimos para que se possa gravar os dados no banco de dados.
	 *  E carregado: a conexуo com o banco,
	 * 				 o esquema, caso nуo se tenha passado o esquema a funчуo entende que o esquema serс o que a pessoa estiver no momento,
	 * 				 a tabela do esquema.
	 * 				 e os campos da tabela do esquema.
	 * @example: include_once APPRAIZ . "includes/classes/file.class.inc";
	 *			 include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	 * 			 $file = new FilesSimec("convenioxml", $campos ,"cte");
	 */
	public function __construct($tabela = NULL, $campos = NULL, $esquema = NULL){

		$this->conexaoBanco(); 				// Carrega conexуo com o Banco.

		if( $tabela && $campos  ){
			try {
				if(!$tabela){
					throw new Exception("Obrigatorio informar a Tabela do Esquema.");
				} else
				if(!$campos){
					throw new Exception("Obrigatorio informar os campos da Tabela do Esquema.");
				} else {
					if(!$this->setEsquema($esquema)){ 			// Carrega o Esquema.
						throw new Exception("Nуo foi possivel instanciar o Mѓdulo - Esquema.");
					}
					$this->setTabela($tabela);			// Carrega a Tabela do Esquema.
					$this->setCampos($campos);			// Carrega os Campos da Tabela instanciados.
				}
			}catch (Exception $e) {
				$this->setErro("Exceчуo na iniciaчуo da classe FilesSimec: ".  $e->getMessage());
				$this->getErroSimec();
			}
		}else{
			try{
				$this->setTabela($tabela);			// Carrega a Tabela do Esquema.
				if(!$this->setEsquema($esquema)){ 			// Carrega o Esquema.
					throw new Exception("Nуo foi possivel instanciar o Mѓdulo - Esquema.");
				}
			}catch (Exception $e) {
				$this->setErro("Exceчуo na iniciaчуo da classe FilesSimec: ".  $e->getMessage());
				$this->getErroSimec();
			}
		}
	}

	/**
	 * function conexaoBanco()
	 * @description : Abre conexуo com o Banco de dados pela classe cls_banco().
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 */
	private function conexaoBanco(){
		global $db;

		if( !is_object($db) ){
			$db = new cls_banco();
		}
		$this->db = $db;
	}

	/**
	 * function setEsquema($nomeSistema = NULL)
	 * @description : Instancia o Esquema do Mѓdulo. Classe privada
	 * @param  : string $nomeSistema
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial : Caso nуo seja passado o Nome do Esquema a funчуo recupera pelo ID do sistema que estс na session.
	 */
	private function setEsquema($nomeSistema = NULL){
			if($nomeSistema){
				$this->nomeEsquema 	= $nomeSistema;
			}else{
				$this->nomeEsquema 	= $_SESSION["sisdiretorio"];
			}
			$this->idSistema = $_SESSION["sisid"] ? $_SESSION["sisid"] : $this->db->pegaUm("SELECT sisid FROM seguranca.sistema WHERE sisdiretorio = '{$this->nomeEsquema}'") ;
			if($this->nomeEsquema && $this->idSistema){
				return true;
			}else{
				return false;
			}
	}

	/**
	 * function setTabela($tabela = NULL)
	 * @description : Instancia a tabela do Esquema setado. Classe privada
	 * @param  : string $tabela
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial : Carrega para variavel global da classe o nome da tabela.
	 */
	private function setTabela($tabela){
		$this->tabela = $tabela;
	}

	/**
	 * function setCampos($campos)
	 * @description : Carrega os campos da tabela. Classe privada
	 * @param  : array $campos
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial : Carrega campos da tabela para variavel global da classe.
	 */
	private function setCampos($campos){
		$this->campos = $campos;
	}

	/**
	 * function setUpload
	 * @description : Salva o arquivo e os dados no banco de dados.
	 * @param  : string $descricaoArquivo (Campo de descriчуo do arquivo OPcional).
	 * @param  : string $key (Compo opcional com a chave do aray $_FILES. Ex: $_FILES[$key] )
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial :
	 * 	Existe 1 parametros a ser passados:
	 * 	Sendo o 1К: A pasta a onde serс salvo o arquivo.
	 * 	Caso deseja passar a descriчуo do arquivo informa a variavel $descricaoArquivo.
	 *
	 * 	Lembrando que por default
	 *  E carregado: a conexуo com o banco,
	 * 				 o esquema, caso nуo se tenha passado o esquema a funчуo entende que o esquema serс o que a pessoa estiver no momento,
	 * 				 a tabela do esquema.
	 * 				 e os campos da tabela do esquema.
	 * @example: include_once APPRAIZ . "includes/classes/file.class.inc";
	 *			 include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	 * 			 $file = new FilesSimec("convenioxml", $campos ,"cte");
	 */
	public function setUpload( $descricaoArquivo = NULL, $key = NULL ){
		if( $key == NULL ){
			Files::setFile();// Carrega o arquivo do tipo FILE.
		}else{
			Files::setFile( NULL, NULL, $key );// Carrega o arquivo do tipo FILE com chave especifica.
		}
		$this->inserePublicArquivoDB($descricaoArquivo); // Insere na tabela padrуo de arquivos. (public.arquivo)
		$this->insereTabelaEsquemaDB(); // Insere na tabela do esquema.
		$caminhoEarquivo = APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($this->idArquivo/1000).'/'.$this->idArquivo;
		$this->criaPasta(); // caso a pasta nуo exista entуo cria a pasta.
		Files::setCaminho($caminhoEarquivo);
		Files::upload($caminhoEarquivo);
		if($this->getErroSimec()){ // Se ocorreu tudo com sucesso.
			$this->db->commit();
			return true;
		}else{
			return false;
		}
	}

    public function setUploadArquivoEspecifico( $descricaoArquivo, $key){
        Files::setFile( NULL, NULL, $key );// Carrega o arquivo do tipo FILE com chave especifica.
        $this->inserePublicArquivoDB($descricaoArquivo); // Insere na tabela padrуo de arquivos. (public.arquivo)
        $caminhoEarquivo = APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($this->idArquivo/1000).'/'.$this->idArquivo;
        $this->criaPasta(); // caso a pasta nуo exista entуo cria a pasta.
		Files::setCaminho($caminhoEarquivo);
		Files::upload($caminhoEarquivo);
		if($this->getErroSimec()){ // Se ocorreu tudo com sucesso.
			return $this->idArquivo;
		}else{
			return false;
		}
	}

	function setCrop($param)
	{
		$pathFile = APPRAIZ . 'www/' . $param['folder'] . $param['name'];
		$pathFile = str_replace('//','/',$pathFile);

		$param['type'] = mime_content_type($pathFile);
		$param['size'] = 0;
		$this->arquivo = $param;

		$this->inserePublicArquivoDB('');
		$this->criaPasta(); // caso a pasta nуo exista entуo cria a pasta.

		$pathFileNew = APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($this->idArquivo/1000).'/'.$this->idArquivo;

		//crop our image..
		$imgSize = getimagesize($pathFile);
		switch ($imgSize['mime']) {
		    case "image/gif":
				$src_img = imagecreatefromgif($pathFile);
		        break;
		    case "image/jpeg":
				$src_img = imagecreatefromjpeg($pathFile);
		        break;
		    case "image/png":
				$src_img = imagecreatefrompng($pathFile);
		        break;
		    default:
		    	return false;
		}

		$dst_img=ImageCreateTrueColor($param['w'], $param['h']);
		imagecopyresampled($dst_img,$src_img,0,0,$param['x'], $param['y'], $param['w'], $param['h'], $param['w'], $param['h']);
		imagejpeg($dst_img,$pathFileNew,100);
		imagedestroy($dst_img);
		imagedestroy($src_img);

		if (is_file($pathFile)){
			// Limpa Imagem
			unlink($pathFile);
		}
		if($this->getErroSimec()){ // Se ocorreu tudo com sucesso.
			$this->db->commit();
			return $this->idArquivo;
		}else{
			return false;
		}
	}

	/**
	 * function setMover
	 * @description : Move um arquivo de uma pasta para outra
	 * 				  OBS:  No esquema.
	 * 						Nуo move de uma pasta de um sistema para outro.
	 * @param  : string $origem (Arquivo de origem).
	 * @param  : string $tipoExtencao (tipo de arquivo).
	 * @author : Thiago Tasca Barbosa
	 * @since  : 08/05/2009
	 * @tutorial :
	 *
	 * @example: 	include_once APPRAIZ."includes/classes/fileSimec.class.inc";
	 * 				$campos		= array("cnvid"	=>726);
	 * 				$file 		= new FilesSimec("convenioxml", $campos);
	 * 				$arquivo 	= $file->criaArquivo(APPRAIZ."arquivos/".$descricao.".xml",$xml);
	 * 				$file->setMover(APPRAIZ."arquivos/".$descricao.".xml", "text/xml");
	 */
	public function setMover($origem, $tipoExtencao){
		Files::setFile($origem, $tipoExtencao); // Carrega o arquivo do tipo FILE.
		$this->inserePublicArquivoDB($descricaoArquivo); // Insere na tabela padrуo de arquivos. (public.arquivo)
		if ($this->campos && $this->tabela && $this->nomeEsquema){
			$this->insereTabelaEsquemaDB(); // Insere na tabela do esquema.
		}
		$destino = APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($this->idArquivo/1000).'/'.$this->idArquivo;
		Files::setCaminho($destino);
		$this->criaPasta(); // caso a pasta nуo exista entуo cria a pasta.
		Files::mover($origem);
		if($this->getErroSimec()){ // Se ocorreu tudo com sucesso.
			$this->db->commit();
			return $this->idArquivo;
		}else{
			$this->erro = "Nуo foi possivel deletar o arquivo do BD.";
			$this->getErroSimec();
			return false;
		}
	}


	/**
	 * function inserePublicArquivoDB()
	 * @description : Salva os dados do arquivo na tabela public.arquivo.
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial : Estс funчуo salva todos os dados do arquivo na tabela public.arquivo. (Padrуo SIMEC)
	 * 	Existe 1 parametros a ser passados mas nуo e obrigatorio:
	 * 	Sendo: $descricaoArquivo = Caso tenha um campo no formulсrio informando a descriчуo do arquivo
	 * 							   ele deverс ser salvo na public.arquivo.
	 */
	private function inserePublicArquivoDB($descricaoArquivo = NULL){
		$arquivo = $this->arquivo;
		//Insere o registro do arquivo na tabela public.arquivo
		if($arquivo){
			$sql = "INSERT INTO public.arquivo (arqnome,
												arqextensao,
												arqdescricao,
												arqtipo,
												arqtamanho,
												arqdata,
												arqhora,
												usucpf,
												sisid,
												arqstatus)
					VALUES( '".current(explode(".", $arquivo["name"]))."',
							'".end(explode(".", $arquivo["name"]))."',
							'".$descricaoArquivo."',
							'".$arquivo["type"]."',
							'".$arquivo["size"]."',
							'".date('Y-m-d')."',
							'".date('H:i:s')."',
							'".$_SESSION["usucpf"]."',
							$this->idSistema,
							'A')
					RETURNING arqid;";
			$arqid = $this->db->pegaUm($sql);
            $this->idArquivo = $arqid;
		}
	}

	/**
	 * function insereTabelaEsquemaDB()
	 * @description : Insere os dados na tabela do Esquema utilizando os campos da tabela passado.
	 * @package	: function setCampos();
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial :  Insere na tabela do esquema passado os dados desejado.
	 */
	private function insereTabelaEsquemaDB(){
		$campos 		= $this->campos; // Recupera os campos da tabela.
		$camposInsert 	= implode( ",",array_keys($campos)); // nome do campo.
		$valorInsert 	= "'" . implode( "', '",$campos) . "'"; // valor do campo.
		//$campos = $this->recuperaCamposTabela();
		//Insere o registro na tabela do esquema
		$sql = "INSERT INTO ".$this->nomeEsquema.".".$this->tabela." (".$camposInsert.",arqid)
				VALUES(".$valorInsert.",".$this->idArquivo.");";
		$this->db->executar($sql);
	}

	/**
	 * function criaPasta()
	 * @description : Cria pasta.
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @tutorial : Cria a pasta caso nуo exista.
	 * 	Sendo que o Simec trabalha com arquivos e pastas da seguinte forma:
	 * 		1К Os arquivos serуo salvo na pasta do Esquema.
	 * 		2К O arquivo e salvo dentro de uma pasta criada da seguinte forma:
	 * 			- o ID do arquivo que estс na tabela public arquivo / por 10000. (Feito istѓ para melhor indexaчуo dos arquivos).
	 */
	private function criaPasta(){
		if(!is_dir(APPRAIZ."arquivos/".$this->nomeEsquema)) {
			mkdir(APPRAIZ."arquivos/".$this->nomeEsquema, 0777);
		}
		if(!is_dir(APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($this->idArquivo/1000))) {
			mkdir(APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($this->idArquivo/1000), 0777);
		}
	}

	/**
	 * function getErroSimec()
	 * @description : Retorna erros e desfaz operaчѕes feitas no banco.
	 * @package : file.class.inc
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @example: Caso do retorno seja TRUE, quer dizer que nуo tem erros. Operaчуo com Sucesso.
	 */
	private function getErroSimec(){
		if($this->erro){
			if($this->db){
				$this->db->rollback();
			}
			$this->getErro();
			return false;
		}else{
			return true;
		}
	}



	public function getCaminhodArquivo($arqid){
		$caminho = APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($arqid/1000).'/'.$arqid;
		return realpath($caminho);
	}


	/**
	 * function getDownloadArquivo($arqid)
	 * @description : Faz o Download do arquivo..
	 * @param  : string $arqid (ID do arquivo).
	 * @author : Thiago Tasca Barbosa
	 * @since  : 09/03/2009
	 * @example: $arquivo = $file->getDownloadArquivo('13398');
	 *			 echo($arquivo);
	 */
	public function getDownloadArquivo($arqid){
		$arquivo = $this->obterArquivo($arqid);
		if (!$arquivo) {
		    return false;
        }
		$caminho = $this->getCaminhodArquivo($arqid);
		if(!Files::Download($caminho)){
			$this->getErroSimec();
			return false;
		}
	    $filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
	    header( 'Content-type: '. $arquivo['arqtipo'] );
        header( 'Content-Disposition: attachment; filename='.$filename);
        readfile( $caminho );
        exit();
	}

	private function obterCaminho($arquivo) {
	    return APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($arqid/1000).'/'.$arqid;

    }

	private function obterArquivo($arqid) {
        $sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arqid;
        $arquivo = $this->db->carregar($sql);
        if($arquivo){
            $arquivo = current($arquivo);
        } else {
            $this->erro = "Arquivo nуo encontrado.";
            $this->getErroSimec();
            return false;
        }
        return $arquivo;
    }


	public function obterArquivoBase64($arqid) {
        $arquivo = $this->obterArquivo($arqid);
        if (!$arquivo) {
            return false;
        }
        $caminho = $this->getCaminhodArquivo($arqid);
        $data = file_get_contents($caminho);
        return base64_encode($data);
    }

 	/* function excluiArquivoFisico($arqid)
	 * @description : Faz o Exclusуo fэsica do arquivo..
	 * @param  : string $arqid (ID do arquivo).
	 * @author : Gustavo Fernandes
	 * @since  : 14/09/2009
	 * @example: $file->excluiArquivoFisico('13398');
	 */
	public function excluiArquivoFisico($arqid){
		$caminho = APPRAIZ."arquivos/".$this->nomeEsquema.'/'.floor($arqid/1000).'/'.$arqid;
		if(Files::Download($caminho)){
			unlink($caminho);
		}
	}


/**
 * function recuperaCamposTabela()
 * @description : Recupera os campos da tabela no esquema, caso nуo passado.
 * @author : Thiago Tasca Barbosa
 * @since  : 09/03/2009
 * @tutorial : Recupera todos os campos da tabela do Esquema.
 */
	/*
	public function recuperaCamposTabela(){
		$sql = "SELECT a.attname AS campo
				FROM  	pg_catalog.pg_class c,
						pg_catalog.pg_namespace nf,
						pg_catalog.pg_type t,
						pg_catalog.pg_attribute a
				LEFT JOIN pg_catalog.pg_description d ON d.objsubid = a.attnum AND a.attrelid = d.objoid
				LEFT JOIN pg_catalog.pg_constraint con ON con.conrelid = a.attrelid AND con.conkey[1] = a.attnum
				LEFT JOIN pg_catalog.pg_class clf ON con.confrelid = clf.oid AND clf.relkind = 'r'
				LEFT JOIN pg_catalog.pg_namespace nfa ON nfa.oid = clf.relnamespace
				LEFT JOIN pg_catalog.pg_attribute af ON af.attrelid = con.confrelid AND af.attnum = con.confkey[1]
				LEFT JOIN pg_catalog.pg_attribute af2 ON af2.attrelid = con.confrelid AND (af2.attname = (substr(af.attname, 0, 4) || 'dsc') OR af2.attname = (substr(af.attname, 0, 4) || 'desc'))
				WHERE
				nf.nspname = '".$this->nomeEsquema."' AND
				c.relname = '".$this->tabela."' AND
				a.attnum > 0 AND
				a.attrelid = c.oid AND
				a.atttypid = t.oid AND
				nf.oid = c.relnamespace";

		$campos = $this->db->carregar($sql);
		$this->campos = $campos;
	}
	*/

    public function salvarPdfBase64($base64, $nomeArquivo) {
        $now = new \DateTime();
        $decoded= base64_decode($base64);
        $pathArquivoTmp = '/tmp/' . $now->getTimestamp();
        file_put_contents($pathArquivoTmp,$decoded);
        $ds = DIRECTORY_SEPARATOR;
        $usucpfLogado = trim($_SESSION['usucpf']);
        $tamanho = filesize($pathArquivoTmp);
        $sql = "INSERT INTO public.arquivo (arqnome,
												arqextensao,
												arqdescricao,
												arqtipo,
												arqtamanho,
												arqdata,
												arqhora,
												usucpf,
												sisid,
												arqstatus)
					VALUES( '".$nomeArquivo."',
							'pdf',
							'".$nomeArquivo."',
							'application/pdf',
							'".$tamanho."',
							'".date('Y-m-d')."',
							'".date('H:i:s')."',
							'".$usucpfLogado."',
							$this->idSistema,
							'A')
					RETURNING arqid;";
        $arqid = $this->db->pegaUm($sql);
        $pastaAdm = APPRAIZ . "arquivos{$ds}adm";
        $pastaArquivo = $pastaAdm . $ds . floor($arqid / 1000);
        $destinoArquivo = $pastaArquivo . $ds . $arqid;
        if (!is_dir($pastaAdm)) {
            mkdir($pastaAdm, 0777);
        }

        if (!is_dir($pastaArquivo)) {
            mkdir($pastaArquivo, 0777);
        }

        copy($pathArquivoTmp, $destinoArquivo);
        unlink($pathArquivoTmp);

        return $arqid;
    }

	public function setRemoveUpload( $arqid ){
		$this->deleteTabelaEsquemaDB($arqid);
		$this->deletePublicArquivoDB($arqid);
		$this->excluiArquivoFisico($arqid);

		$this->db->commit();

		return true;
	}

	private function deletePublicArquivoDB($arqid){
		//deleta arquivo da PublicArquivo
		if($arqid){
			$sql = "DELETE FROM public.arquivo WHERE arqid = ".$arqid;
			$executado = $this->db->executar($sql);
			if($executado){
				return true;
			}else{
				$this->erro = "Nуo foi possivel deletar o arquivo do BD.";
				$this->getErroSimec();
				return false;
			}
		}
	}

	private function deleteTabelaEsquemaDB($arqid){
		//deleta arquivo da PublicArquivo
		if($arqid && $this->tabela){
			$sql = "DELETE FROM ".$this->nomeEsquema.".".$this->tabela." WHERE arqid = ".$arqid;
			$executado = $this->db->executar($sql);
			if($executado){
				return true;
			}else{
				$this->erro = "Nуo foi possivel deletar o arquivo do BD.";
				$this->getErroSimec();
				return false;
			}
		}
	}

}
?>