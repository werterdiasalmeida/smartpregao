<?php
	
class Tabela extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "spm.tabela";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tblid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tblid' => null, 
									  	'perid' => null, 
									  	'tbldescricao' => null, 
									  	'tbllabelcolunas' => null, 
									  	'tbllabellinhas' => null, 
									  	'tbltipo' => null, 
									  );

	public function pegaDados( $perid ){
		$sql = "SELECT tbldescricao as descricao, tbllabelcolunas as colunas, tbllabellinhas as linhas, tblid, tbltipo FROM {$this->stNomeTabela} WHERE perid = ".$perid;
		$dados = $this->pegaLinha( $sql );
		
		if( $dados ){
			return $dados;
		} else {
			die('Tabela inexistente!');
		}
	}
}