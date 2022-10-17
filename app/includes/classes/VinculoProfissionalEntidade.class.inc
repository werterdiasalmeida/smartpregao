<?php
	
class VinculoProfissionalEntidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "entidade.vinculoprofissionalentidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "vpeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'vpeid' => null, 
									  	'entid' => null, 
									  	'tvpid' => null, 
									  	'tvpdscoutros' => null, 
									  );
									  
	public function excluirPorEntid( $entid ){
		$sql = " DELETE FROM $this->stNomeTabela WHERE entid = $entid; ";
		
		return $this->executar( $sql );
	}
}