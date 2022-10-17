<?php
	
class EntidadeBensMateriais extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.entidadebensmateriais";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ebmid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ebmid' 		=> null, 
									  	'entid' 		=> null, 
									  	'bmaid' 		=> null,
    									'ebmobservacao' => null
									  );
									  
									  
  public function excluirPorEntidade($entid){
  		if($entid){
  			$sql = "DELETE FROM {$this->stNomeTabela} WHERE entid = {$entid};";	
  			$this->executar( $sql );
  		}
  }
  
  
   public function bensMateriaisFormatadosPorEntidade($entidade){
  	$sql = "
  		SELECT DISTINCT
			'- ' || bmadsc AS descricao
		FROM 
			agenda.bensmateriais bma
		JOIN
			agenda.entidadebensmateriais ebm ON ebm.bmaid = bma.bmaid
		WHERE 
			ebm.entid = {$entidade} AND bma.bmastatus = 'A'
			
		UNION ALL
				
		SELECT 
			'<b>Observação:</b> ' || ebmobservacao AS descricao
		FROM 
			agenda.entidadebensmateriais
		WHERE 
			entid = {$entidade} AND ebmobservacao != ''";
			
  	$rs = $this->carregarColuna($sql);
  	if(is_array($rs))
  		$rs = implode('<br>', $rs);
  	return $rs;
  }		
  
// public function ajudaTecnicaFormatadaPorEntidade($entidade){
//  	$sql = "
//  		SELECT DISTINCT
//			'- ' || ajtdsc AS descricao
//		FROM 
//			agenda.ajudatecnica ajt
//		INNER JOIN
//			agenda.entidadeajudatecnica ent ON ent.ajtid = ajt.ajtid
//		WHERE 
//			entid = {$entidade}
//			
//		UNION ALL
//				
//		SELECT 
//			'<b>Observação:</b> ' || eatobservacao AS descricao
//		FROM 
//			agenda.entidadeajudatecnica
//		WHERE 
//			entid = {$entidade} AND eatobservacao != ''";
//  	$rs = $this->carregarColuna($sql);
//  	if(is_array($rs))
//  		$rs = implode('<br>', $rs);
//  	return $rs;
//  }
}
