<?php
	
class TipoAnexoEntidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.tipoanexoentidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "taeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'taeid' 		=> null, 
									  	'taedsc' 		=> null, 
									  	'taestatus' 	=> null
									  );
									  
	public function recuperarTodos( $stCampos, $arClausulas = null, $stOrdem = null ){
		$arClausulas = is_array($arClausulas) ? $arClausulas : array();
		array_push($arClausulas, "taestatus='A'");
		
		return parent::recuperarTodos( $stCampos, $arClausulas, $stOrdem);
	}
}
