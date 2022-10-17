<?php
	
class Cargo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.cargo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "carid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'carid' 			 => null, 
									  	'cardsc' 			 => null, 
									  	'carstatus' 		 => null,
    									'carsnmultocupantes' => null
									  );

  
	public function listarPorEntidade( $entid, $entidpessoa = null )
	{
		 $sql = "		 
			SELECT 
				cae.caeid AS codigo,
				cae.caedetalhe AS descricao    		
			FROM 
				agenda.cargo car
			JOIN
				agenda.cargoentidade cae ON cae.carid = car.carid
							    AND cae.caestatus = 'A'  						
			JOIN
				agenda.entidade ent ON ent.entid = cae.entid 
						       AND ent.entstatus = 'A'								
			WHERE 
				ent.entid = {$entid} 
				AND car.carstatus = 'A' 
				AND ( 	
					car.carsnmultocupantes = true		
					OR NOT EXISTS(
							SELECT
								ce.carid
							FROM
								agenda.cargoentidade ce
							JOIN 
								agenda.pessoa_entidade pe ON pe.caeid = ce.caeid
															 " . ($entidpessoa ? " AND pe.entidpessoa != {$entidpessoa} " : " ") . "
											     			 AND pe.petstatus = 'A'
											     			 AND pe.entid = {$entid}
							WHERE
								ce.caestatus = 'A' AND ce.carid = car.carid
						   )
				     );";
	 
			 $rs = $this->carregar($sql);;			
			 return $rs;
	}
	
	public function verificaCargoUnicoPreenchido($entid, $caeid)
	{
		$sql = "SELECT
					count(*)
				FROM
					agenda.cargo c
				JOIN agenda.cargoentidade ce ON ce.carid = c.carid
												AND ce.caestatus = 'A'
												AND ce.caeid = {$caeid}
				JOIN agenda.pessoa_entidade pe ON pe.caeid = ce.caeid
												  AND pe.petstatus = 'A'
								  				  AND pe.entid = {$entid}
				WHERE
					c.carstatus = 'A'
					AND c.carsnmultocupantes = false;";
		
		$existe = $this->pegaUm($sql);
		
		return $existe > 0;
	}
	
	
	public function listarCargosQtd($like){
		$sql = "SELECT 
					car.cardsc, 
					'<center>' ||
					( SELECT 
						count(cae.caeid) 
					FROM
						agenda.pessoa_entidade pet
					JOIN
						agenda.cargoentidade cae ON cae.caeid = pet.caeid
					JOIN	
						agenda.cargo car2 ON cae.carid = car2.carid
					WHERE 
						car2.carid = car.carid AND pet.petstatus = 'A') || '</center>' AS qtd,
					car.carid 
				FROM 
					agenda.cargo car
				WHERE  
					cardsc 
				ILIKE '%{$like}%'  AND  carstatus = 'A' ";
		
		$resultado = $this->carregar( $sql );
		
		return $resultado ? $resultado : array();
	}
	
	public function verificaCargoEntidadePorCargo($carid){
		$sql = "SELECT 
						count(*) AS qtd
				FROM
						agenda.cargo car
				JOIN
						agenda.cargoentidade cae ON cae.carid = car.carid
				WHERE  
						cae.caestatus = 'A' AND car.carid = '{$carid}'";
		return $this->carregarColuna($sql);
	}
	
	public function verificaPessoaPorCargo($carid){
		$sql = "SELECT 
						count(*) AS qtd
				FROM
						agenda.cargo car
				JOIN
						agenda.cargoentidade cae ON cae.carid = car.carid
				JOIN
						agenda.pessoa_entidade pet ON cae.caeid = pet.caeid
				WHERE  
						pet.petstatus = 'A' AND car.carid = '{$carid}'";
		return $this->carregarColuna( $sql );
	}
	
	public function verificaPessoaPorCargoMultiplo($carid){
		$sql = "SELECT 
						count(*) AS qtd,
						cae.entid
				FROM
						agenda.cargo car
				JOIN
						agenda.cargoentidade cae ON cae.carid = car.carid
				JOIN
						agenda.pessoa_entidade pet ON cae.caeid = pet.caeid
				WHERE  
						pet.petstatus = 'A' AND car.carid = '{$carid}'
				GROUP BY 
						cae.entid
				having count(*) > 1";
		$restultado = $this->carregar( $sql );
		return !is_array($restultado);
	}
	
	public function pegarCargoPorPessoa($entid){
		$sql = "SELECT
					car.carid
				FROM 
					agenda.cargo car
				INNER JOIN
					agenda.cargoentidade cae ON cae.carid = car.carid
				INNER JOIN
					agenda.pessoa_entidade pet ON pet.caeid = cae.caeid AND pet.entidpessoa = '{$entid}' AND pet.petstatus = 'A'";
		return $this->carregarColuna($sql,'carid');
	}
	
}
