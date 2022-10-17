<?php
	
class EntidadeAjudaTecnica extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.entidadeajudatecnica";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "eatid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'eatid' 		=> null, 
									  	'entid' 		=> null, 
									  	'ajtid' 		=> null,
    									'eatobservacao' => null
									  );
									  
									  
  public function excluirPorEntidade($entid){
  		if($entid){
  			$sql = "DELETE FROM {$this->stNomeTabela} WHERE entid = {$entid};";	
  			$this->executar( $sql );
  		}
  }
  
 public function ajudaTecnicaFormatadaPorEntidade($entidade){
  	$sql = "
  		SELECT DISTINCT
			'- ' || ajtdsc AS descricao
		FROM 
			agenda.ajudatecnica ajt
		INNER JOIN
			agenda.entidadeajudatecnica ent ON ent.ajtid = ajt.ajtid
		WHERE 
			entid = {$entidade}
			
		UNION ALL
				
		SELECT 
			'<b>Observação:</b> ' || eatobservacao AS descricao
		FROM 
			agenda.entidadeajudatecnica
		WHERE 
			entid = {$entidade} AND eatobservacao != ''";
  	$rs = $this->carregarColuna($sql);
  	if(is_array($rs))
  		$rs = implode('<br>', $rs);
  	return $rs;
  }
}
