<?php
	
class Identificacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "spm.identificacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "idtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'idtid' => null, 
									  	'perid' => null, 
									  	'qrpid' => null, 
									  	'idtmun' => null, 
									  	'idtuf' => null, 
									  	'idtdtpreench' => null, 
									  	'idtresp' => null, 
									  	'idtcargo' => null, 
									  	'idtformacao' => null, 
									  	'idtemailorg' => null, 
									  	'idttelorg' => null, 
									  	'idtnomeorg' => null, 
									  	'idtend' => null, 
									  	'idtresppreench' => null, 
									  	'idttel' => null, 
									  	'idtemail' => null, 
									  	'idtger' => null, 
									  );
									  

	public function recuperarResposta( $perid, $qrpid ){
		$sql 	  = "SELECT * FROM {$this->stNomeTabela} WHERE perid = {$perid} AND qrpid = {$qrpid}";
		return $this->pegaLinha( $sql );
	}
	
	public function excluirPorQuestionarioFormulario( $qrpid, $perid ){
		$sql = "DELETE FROM spm.identificacao WHERE qrpid = {$qrpid} AND perid =".$perid;
		$this->executar($sql);
		return true;
	}
}