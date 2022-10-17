<?php
	
class Arquivo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "public.arquivo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "arqid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'arqid' => null, 
									  	'arqnome' => null, 
									  	'arqdescricao' => null, 
									  	'arqextensao' => null, 
									  	'arqtipo' => null, 
									  	'arqtamanho' => null, 
									  	'arqdata' => null, 
									  	'arqhora' => null, 
									  	'arqstatus' => null, 
									  	'usucpf' => null, 
									  	'sisid' => null, 
									  );

	function antesSalvar(){
		$this->arqdata   = $this->arqdata   ? $this->arqdata   : date('Y-m-d');
		$this->arqhora   = $this->arqhora   ? $this->arqhora   : date('H:i:s');
		$this->arqstatus = $this->arqstatus ? $this->arqstatus : 'A';
		$this->usucpf    = $this->usucpf    ? $this->usucpf    : $_SESSION['usucpf'];
		$this->sisid     = $this->sisid     ? $this->sisid     : $_SESSION['sisid'];
		
		return true;
	}
}