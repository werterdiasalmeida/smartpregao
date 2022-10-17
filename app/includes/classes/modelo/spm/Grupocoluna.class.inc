<?php
	
class Grupocoluna extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "spm.grupocoluna";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "grcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'grcid' => null, 
									  	'tblid' => null, 
									  	'gru_grcid' => null, 
									  	'grcdsc' => null, 
									  );

	public function pegaDados( $tabela ){
		$sql = "SELECT
					grcid as codigo,
					grcdsc as descricao,
					gru_grcid as grupai
				FROM
					{$this->stNomeTabela}
				WHERE
					tblid = {$tabela} ORDER BY grcid";
		return $this->carregar( $sql );
	}
	
	public function pegaArrUltimoNivel( $paramGrcid ){
		
		$paramGrcid = (array) $paramGrcid;
		
		$sql = "SELECT
					grcid
				FROM
					{$this->stNomeTabela}
				WHERE
					gru_grcid IN (" . implode(",", $paramGrcid) . ")";
					
		$arrGrcid = $this->carregarColuna( $sql );		
		
		if( $arrGrcid ){
			$arrGrcid = $this->pegaArrUltimoNivel( $arrGrcid );
			return $arrGrcid; 
		}
		
		return $paramGrcid;
	}
	
	public function verificaNiveis( $tabela, $grcid = null, $i = null ){
		
		if( $grcid == '' ){
			$sql = "SELECT
						grcid
					FROM
						{$this->stNomeTabela}
					WHERE
						gru_grcid IS NULL AND tblid = ".$tabela;
						
			$grcid = $this->pegaUm( $sql );		
			$i = $grcid ? 2 : 1;
		}
		
		$sql = "SELECT
					grcid
				FROM
					{$this->stNomeTabela}
				WHERE
					gru_grcid = ".$grcid;
					
		$filho = $this->pegaUm( $sql );	
		
		if( $filho ){
			$i = $i + 1;
			$i = $this->verificaNiveis( $tabela, $filho, $i );
		}
		return $i;
	}
}