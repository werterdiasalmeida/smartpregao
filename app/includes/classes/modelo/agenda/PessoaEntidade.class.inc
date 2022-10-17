<?php
	
class PessoaEntidade extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.pessoa_entidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "petid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'petid' 		  => null, 
									  	'entid' 		  => null, 
									  	'entidpessoa' 	  => null,
 									  	'caeid' 		  => null,
     								  	'petdtentrada' 	  => null,
     								  	'petdtsaida' 	  => null,
     								  	'petcompendereco' => null,
     								  	'petemail' 		  => null,
     								  	'petramal' 		  => null,
     								  	'pettelefone' 	  => null,
    									'petstatus'		  => null
									  );
									  

	public function antesSalvar()
	{ 
		$this->petstatus 	= $this->petstatus ? $this->petstatus : 'A'; 
		//$this->petdtentrada = $this->petdtentrada ? $this->petdtentrada : date("Y-m-d"); 
		
		return true;
	}
	
    function listarCargosOcupados(  $petid , $where ='' ){
		 
    	$where = !empty($where) ? implode(' AND ', $where): " 1=1 ";
    			
		 $sql = "
		    	SELECT 
		 			ent.entnome, 
		 			cae.caedetalhe,
		    		to_char(pet.petdtentrada, 'DD/MM/YYYY') AS petdtentrada,
		 			to_char(pet.petdtsaida, 'DD/MM/YYYY') AS petdtsaida,
 					CASE pet.petstatus 
 						WHEN 'A' THEN 'Ativo'
 						WHEN 'I' THEN 'Inativo'
      				END AS petstatus,
      				ent.entid,
      				car.carid,
      				pet.petid 			
				FROM 
					agenda.pessoa_entidade pet 
				JOIN
					agenda.cargoentidade cae ON pet.caeid = cae.caeid
												AND cae.caestatus = 'A'  
				JOIN
					agenda.cargo car ON car.carid = cae.carid
										AND car.carstatus = 'A'
				JOIN
					agenda.entidade ent ON ent.entid = pet.entid 
										   AND ent.entstatus = 'A' 									
				WHERE 
					pet.entidpessoa = {$petid} AND {$where}
				ORDER BY
					petstatus ASC ,  petdtentrada DESC";
		 $rs = $this->carregar($sql);
		 return $rs;			
     }
     
   	function listarPessoasPorCargo(  $caeid , $detalheCargo = false){
		 
		 $sql = "
		    	SELECT 
		 			ent.entnome,
		    		to_char(pet.petdtentrada, 'DD/MM/YYYY') AS petdtentrada,
		 			to_char(pet.petdtsaida, 'DD/MM/YYYY') AS petdtsaida,
		 			CASE pet.petstatus 
		 				WHEN 'A' THEN 'Ativo'
		 				WHEN 'I' THEN 'Inativo'
		      		END AS petstatus";
		 if($detalheCargo)
		 	$sql .= ", cae.caedetalhe ";
		      				 			
		$sql .=	" FROM 
					agenda.cargoentidade cae 
				JOIN
					agenda.pessoa_entidade pet ON cae.caeid = pet.caeid 
				JOIN
					agenda.entidade ent ON pet.entidpessoa = ent.entid  
										   AND ent.entstatus = 'A' 						
				WHERE 
					pet.caeid = {$caeid}";
		if ($detalheCargo)
			$sql .= " AND pet.petstatus = 'A' ";
		
		$sql .=	" ORDER BY
					petstatus ASC ,  petdtentrada ASC";

		$rs = $this->carregar($sql);
		 return $rs;			
	}
     
    public function listarPessoaPorCargoNoOrgao($entidOrgao, $caeid, Array $param=array())
    {
    	$coluna = $param['coluna']
    				?
    				$param['coluna']
    				:
    				"e.entid, e.entnome, e.idgid";
    				
    	$sql = "SELECT
					{$coluna}
				FROM
					agenda.entidade e
				JOIN agenda.pessoa_entidade pe ON pe.entidpessoa = e.entid
								  				  AND pe.petstatus = 'A'
								  				  AND pe.entid = {$entidOrgao}
				JOIN agenda.cargoentidade ce ON ce.caeid = pe.caeid
												AND ce.caestatus = 'A'
												AND ce.caeid = {$caeid}
				WHERE
					e.entstatus = 'A'";
    	$rs = $this->carregar($sql);
		return $rs;
    }     
    
    public function listarPessoaPorEntidade($entidOrgao, Array $param=array())
    {
    	$coluna = $param['coluna']
    				?
    				$param['coluna']
    				:
    				"e.entid, e.entnome, e.idgid, ce.caedetalhe";
    				
    	$sql = "SELECT
					{$coluna}
				FROM
					agenda.entidade e
				JOIN agenda.pessoa_entidade pe ON pe.entidpessoa = e.entid
								  				  AND pe.petstatus = 'A'
								  				  AND pe.entid = {$entidOrgao}
				JOIN agenda.cargoentidade ce ON ce.caeid = pe.caeid
												AND ce.caestatus = 'A'
				WHERE
					e.entstatus = 'A'";
    	$rs = $this->carregar($sql);
		return $rs;
    }     
}
