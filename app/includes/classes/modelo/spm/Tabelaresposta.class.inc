<?php
	
class Tabelaresposta extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "spm.tabelaresposta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tbrid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tbrid' => null, 
									  	'tblid' => null, 
									  	'linid' => null, 
									  	'colid' => null, 
									  	'qrpid' => null, 
									  	'tbrdsc' => null, 
									  );

	public function recuperarResposta( $tabela, $qrpid ){
		$resposta = array();
		$sql 	  = "SELECT linid, colid, tbrdsc FROM {$this->stNomeTabela} WHERE tblid = {$tabela} AND qrpid = {$qrpid}";
		$arrResp  = $this->carregar( $sql );
		$arrResp  = !is_bool( $arrResp ) ? $arrResp : array();
		
		foreach( $arrResp as $r ){
			$resposta[$r['linid']][$r['colid']] = $r['tbrdsc'];
		}
		
		return $resposta;
	}
	
	public function excluirPorQuestionarioTabela( $qrpid, $tabela ){
		$sql = "DELETE FROM spm.tabelaresposta WHERE qrpid = {$qrpid} AND tblid =".$tabela;
		$this->executar($sql);
		return true;
	}
	


}