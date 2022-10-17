<?php
	
class FormacaoEntidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "entidade.formacaoentidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "foeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'foeid' => null, 
									  	'entid' => null, 
									  	'tfoid' => null, 
									  	'tfodscoutros' => null, 
									  );
									  
	public function excluirPorEntid( $entid ){
		$sql = " DELETE FROM $this->stNomeTabela WHERE entid = $entid; ";
		
		return $this->executar( $sql );
	}
	
}