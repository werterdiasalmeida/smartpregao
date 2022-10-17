<?php
	
class EntidadeDeficiencia extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.entidadedeficiencia";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "edeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'edeid' 		=> null, 
									  	'entid' 		=> null, 
									  	'defid' 		=> null,
									  	'tdeid' 		=> null,
    									'edeobservacao' => null
									  );
									  
									  
  public function excluirPorEntidade($entid){
  		if($entid){
  			$sql = "DELETE FROM {$this->stNomeTabela} WHERE entid = {$entid};";	
  			$this->executar( $sql );
  		}
  }
  
  public function deficienciasFormatadaPorEntidade($entidade){

  	$out = array();	
	$sql = "SELECT DISTINCT
				tde.tdeid AS codigo,
				tde.tdedsc AS descricao
			FROM 
				agenda.tipodeficiencia tde
			INNER JOIN
				agenda.entidadedeficiencia ede ON ede.tdeid = tde.tdeid AND ede.entid = {$entidade}";
	$rsTipoDef = $this->carregar($sql);
	if($rsTipoDef){
		foreach ($rsTipoDef as $valueTipoDef){
			$out[] = "<b>{$valueTipoDef['descricao']}</b>";
			$sql = "SELECT DISTINCT
						'- ' || def.defdsc AS descricao
					FROM 
						agenda.deficiencia def
					LEFT JOIN
						agenda.entidadedeficiencia ent ON ent.defid = def.defid AND ent.entid = {$entidade}
					WHERE 
						ent.tdeid = {$valueTipoDef['codigo']}
					
					UNION ALL
					
					SELECT 
						'<b>Observação:</b> ' || edeobservacao AS descricao
					FROM 
						agenda.entidadedeficiencia
					WHERE 
						entid = {$entidade} AND edeobservacao != '' AND tdeid = {$valueTipoDef['codigo']}"; 
			$rsDef = $this->carregar($sql);
			
			foreach ($rsDef as $valueDef)
				$out[] = "&nbsp;&nbsp;&nbsp;&nbsp; {$valueDef['descricao']}";
		}
	}
	if (is_array($out))
		$out = implode('<br>', $out);	
  	return $out;
  }
  
  

}
