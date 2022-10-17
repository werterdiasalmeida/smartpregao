<?php
	
class Coluna extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "spm.coluna";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "colid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'colid' => null, 
									  	'itmid' => null, 
									  	'grcid' => null, 
									  );
	public function pegaDados( $tabela ){
		$sql = "SELECT
				c.colid as codigo,
				i.itmdsc as descricao
			FROM
				{$this->stNomeTabela} c
			INNER JOIN spm.item i ON c.itmid = i.itmid
			INNER JOIN spm.grupocoluna gc USING (grcid)
			INNER JOIN spm.tabela t USING (tblid)
			WHERE
				tblid = ".$tabela;
		return $this->carregar( $sql );
	}
	
	public function conta( $arrGrcid ){
		
		$arrGrcid = (array) $arrGrcid;
		
		$sql = "SELECT
					count(colid)
				FROM
					{$this->stNomeTabela}
				WHERE
					grcid IN (".implode(",", $arrGrcid).")";

					
		return $this->pegaUm( $sql );
	}
}