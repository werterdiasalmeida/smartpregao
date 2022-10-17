<?php
	
class Entidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "entidade.entidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "entid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'entid' => null, 
									  	'njuid' => null, 
									  	'entnumcpfcnpj' => null, 
									  	'entnome' => null, 
									  	'entemail' => null, 
									  	'entnuninsest' => null, 
									  	'entobs' => null, 
									  	'entstatus' => null, 
									  	'entnumrg' => null, 
									  	'entorgaoexpedidor' => null, 
									  	'entsexo' => null, 
									  	'entdatanasc' => null, 
									  	'entdatainiass' => null, 
									  	'entdatafimass' => null, 
									  	'entnumdddresidencial' => null, 
									  	'entnumresidencial' => null, 
									  	'entnumdddcomercial' => null, 
									  	'entnumramalcomercial' => null, 
									  	'entnumcomercial' => null, 
									  	'entnumdddfax' => null, 
									  	'entnumramalfax' => null, 
									  	'entnumfax' => null, 
									  	'tpctgid' => null, 
									  	'tpcid' => null, 
									  	'tplid' => null, 
									  	'tpsid' => null, 
									  	'entcodentsup' => null, 
									  	'entcodent' => null, 
									  	'entescolanova' => null, 
									  	'entdatainclusao' => null, 
									  	'entsig' => null, 
									  	'entunicod' => null, 
									  	'entungcod' => null, 
									  	'entproep' => null, 
									  	'entnumdddcelular' => null, 
									  	'entnumcelular' => null, 
									  	'entorgcod' => null, 
									  	'entsede' => null, 
									  	'entrazaosocial' => null, 
									  	'entescolaespecializada' => null, 
									  );
									  
	function buscaNomeEntidade($entid){
		$sql = "SELECT
					entnome
				FROM
					{$this->stNomeTabela}
				WHERE
					entid = {$entid}";
					
		return $this->pegaUm( $sql );			
	}									  
}