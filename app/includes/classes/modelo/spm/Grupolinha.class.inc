<?php
	
class Grupolinha extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "spm.grupolinha";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "grlid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'grlid' => null, 
									  	'tblid' => null, 
									  	'gru_grlid' => null, 
									  	'grldsc' => null, 
									  );
									  
	public function conta( $tabela, $where = null ){
		if( $where ){
			$cond = " AND grldsc <> '$nbsp;'";
		}
		$sql = "SELECT
					count(grlid)
				FROM
					{$this->stNomeTabela}
				WHERE
					tblid = ".$tabela."
					".$cond;
		return $this->pegaUm( $sql );
	}
}