<?php

class CargoEntidade extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.cargoentidade";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "caeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'caeid' 		=> null,
									  	'carid' 		=> null,
									  	'entid' 		=> null,
									  	'caeidpai' 		=> null,
    								    'caeordem' 		=> null,
    								    'caestatus' 	=> null,
    									'caedetalhe' 	=> null
    );

    function comboCargosVinculados( $entid, $caeidpai='' ){

    	$where = empty($caeidpai) ? ' AND caeidpai IS NULL ' : ' AND caeidpai = '.$caeidpai ;

    	$sql = "
	    	SELECT
	    		cae.caeid AS codigo,
	    		cae.caedetalhe AS descricao
			FROM
				agenda.cargoentidade cae
			JOIN
				agenda.entidade ent ON ent.entid = cae.entid AND ent.entstatus = 'A'
			WHERE
				cae.caestatus = 'A'
				AND ent.entid = {$entid}
    			{$where}";
//    	$sql = "
//	    	SELECT
//	    		cae.caeid AS codigo,
//	    		cae.caedetalhe AS descricao
//			FROM
//				agenda.cargo car
//			JOIN
//				agenda.cargoentidade cae ON car.carid = cae.carid
//											AND cae.caestatus = 'A'
//			JOIN
//				agenda.entidade ent ON ent.entid = cae.entid
//			WHERE
//				ent.entstatus = 'A'
//				AND ent.entid = {$entid}
//    			{$where}";

		 $rs = $this->carregar($sql);
		 return $rs;
    }

    function comboCargosVinculadosCount( $entid, $caeidpai='' ){
    	$where = empty($caeidpai) ? ' AND caeidpai IS NULL ' : ' AND caeidpai = '.$caeidpai ;

    	$sql = "
	    	SELECT
	    		count(*) as count
			FROM
				agenda.cargo car
			JOIN
				agenda.cargoentidade cae ON car.carid = cae.carid
											AND cae.caestatus = 'A'
			JOIN
				agenda.entidade ent ON ent.entid = cae.entid
			WHERE
				ent.entstatus = 'A'
				AND ent.entid = {$entid}
    			{$where}";

		 $rs = $this->pegaUm($sql);
		 return $rs;
    }

    function recuperarHierarquia( $caeid, $nome=false ){
    	if($nome){
    		$temp = $this->recuperarUm('caedetalhe', array("caeid = {$caeid}"));
			$arrHierarquia = array("<b>".$temp."</b>");
    	}else{
			$arrHierarquia = array($caeid);
    	}

		$continuar = true;
		while($continuar){
			$caeid = $this->recuperarUm('caeidpai', array("caeid = {$caeid}"));
			if ($caeid){
			    if($nome){
    				$temp = $this->recuperarUm('caedetalhe', array("caeid = {$caeid}"));
					$arrHierarquia[] = $temp;
    			}else{
    				$arrHierarquia[] = $caeid;
    			}
			}else{
				$continuar = false;
			}
		}
    	if($nome){
    		return $arrHierarquia;
    	}else{
    		return array_reverse($arrHierarquia);
    	}
    }


    function listarCargosVinculados( $entid, Array $param=array() ){

    	$coluna = $param['coluna']
    				?
    					$param['coluna']
    				:
    					"cae.caedetalhe AS cardsc,
			    		 cae.caeordem||'&nbsp;',
		    			 CASE car.carsnmultocupantes WHEN 't' THEN 'Sim'
		                	 WHEN 'f' THEN 'Não'
		       			 END AS carsnmultocupantes,
		       			 cae.caeid";

    	$filtro = is_array($param['filtro']) ? " AND " . implode(" AND ", $param['filtro']) : "";

    	$sql = "
	    	SELECT
	    		$coluna
			FROM
				agenda.cargo car
			JOIN
				agenda.cargoentidade cae ON car.carid = cae.carid
											AND cae.caestatus = 'A'
			JOIN
				agenda.entidade ent ON cae.entid = ent.entid
			WHERE
				car.carstatus = 'A'
				AND ent.entstatus = 'A'
				{$filtro}
				AND ent.entid = {$entid}
			ORDER BY
				cae.caeordem, cae.caedetalhe";

		 $rs = $this->carregar($sql);
		 return $rs;
    }

    function listarCargosComPessoa($entid, Array $param = array())
    {
        $coluna = $param['coluna']
    		?
    		$param['coluna']
    		:
    		"e.entid, e.entnome, e.idgid, cae.caedetalhe";

    	$where = $param['where'] ? ' AND ' . implode(' AND ', $param['where']) : "";

    	$sql = "SELECT
		    		$coluna
				FROM
					agenda.cargo car
				INNER JOIN
					agenda.cargoentidade cae ON car.carid = cae.carid
												AND cae.caestatus = 'A'
												AND cae.entid = {$entid}
				INNER JOIN
					agenda.pessoa_entidade pe ON pe.caeid = cae.caeid
												 AND pe.petstatus = 'A'
												 AND pe.entid = {$entid}
				INNER JOIN
					agenda.entidade e ON e.entid = pe.entidpessoa
										 AND e.entstatus = 'A'
				WHERE
					car.carstatus = 'A' {$where}";
		$rs = $this->carregar($sql);
		return $rs;
    }

}
