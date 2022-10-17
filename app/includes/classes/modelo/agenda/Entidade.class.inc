<?php

class Entidade extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.entidade";

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
									  	'entid' 		         => null,
									  	'tpeid' 		         => null,
									  	'entidpai' 		         => null,
									  	'estuf' 		         => null,
    								  	'muncod' 		         => null,
    								  	'arqid' 		         => null,
    								  	'entcpfcnpj' 	         => null,
    								  	'entnome' 		         => null,
    								  	'entstatus' 	         => null,
    								  	'entcep' 		         => null,
    									'entlog' 		         => null,
    									'entcomplemento'         => null,
    								  	'entbairro' 	         => null,
    								  	'entnumero' 	         => null,
    								  	'medlatitude' 	         => null,
    									'modlongitude' 	         => null,
									  	'entzoom' 		         => null,
									  	'entdtnascimento'        => null,
									  	'entrg' 		         => null,
    								  	'entorgaoemissorrg'      => null,
    								  	'entdtemissaorg'         => null,
    								  	'enttituloeleitor'       => null,
    								  	'entfax' 		         => null,
    								  	'enttelfone' 	         => null,
    								  	'enttelfoneprofissional' => null,
    									'entcelular' 	         => null,
    									'entramal'		         => null,
    									'entramalprofissional'	 => null,
    								  	'entemail' 		         => null,
    								  	'entemailprofissional' 	 => null,
    								  	'entobservacao'          => null,
    								  	'entnomemae' 	         => null,
    								  	'entsiape' 	   	         => null,
        								'entnomepai' 	         => null,
        								'entnomesocial'          => null,
										'entordem'    	         => null,
        								'escid' 	   	         => null,
        								'orsid' 	   	         => null,
        								'idgid' 	   	         => null,
        								'corid' 	   	         => null,
										'vifid'	   		         => null,
										'cefid'    		         => null,
										'oroid'    		         => null,
										'entpispasep'	         => null,
    									'entgraduacao'	         => null,
    									'entsexo'		         => null

    );

    public function carregarPorCpf($cpf)
    {
    	$sql   = "SELECT * FROM agenda.entidade WHERE entcpfcnpj='{$cpf}'";
    	$dados = $this->pegaLinha($sql);
    	$this->popularDadosObjeto( $dados );

    	return $this;
    }

	public function antesSalvar()
	{
		if (is_null($this->entid) && $this->entidpai){
			$sql = "SELECT
						COALESCE(MAX(entordem),0) + 1
				   FROM
				   		agenda.entidade
				   WHERE
				   		entidpai = {$this->entidpai}";
			$this->entordem = $this->pegaUm($sql);
		}

		//ESTA VERIFICAÇÃO FOI COLOCADA PARA CONTEMPLAR AS ENTIDADES INSERIDAS PELO VÍNCULO (vinculoSocial.inc)
		//POIS ELAS NÃO POSSUEM CPF E ESTE CAMPO É UNIQUE
		$this->entcpfcnpj = $this->entcpfcnpj ?  $this->entcpfcnpj : '000';

		return true;
	}

    public function depoisSalvar()
    {
    	if (!empty($_POST['arqidRefe']) && $this->arqid != $_POST['arqidRefe']){
	    	include_once APPRAIZ."includes/classes/fileSimec.class.inc";
			$file = new FilesSimec();
			$file->setRemoveUpload($_POST['arqidRefe']);
    	}

    	//ESTA VERIFICAÇÃO FOI COLOCADA PARA CONTEMPLAR AS ENTIDADES INSERIDAS PELO VÍNCULO (vinculoSocial.inc)
		//POIS ELAS NÃO POSSUEM CPF E ESTE CAMPO É UNIQUE
    	if($this->entcpfcnpj == '000'){
			$this->entcpfcnpj =  $this->entid;
    		$this->salvar();
    	}
    	return true;
    }

    function comboPessoaFisica (Array $where=array()){
    	$sql = "
			SELECT
				ent.entid AS codigo,
				ent.entnome || ' (' || TRIM(to_char(entcpfcnpj::float,'000\".\"000\".\"000\"-\"00')) || ')' AS descricao
			FROM
				agenda.entidade ent
			WHERE
				ent.entstatus = 'A'
				AND ent.tpeid = " . ENTIDADE_PESSOA."
				".($where ? " AND ".implode(" AND ", $where) : "")."
			ORDER BY
				ent.entnome";
		 return $this->carregar($sql);
    }

    function listarPessoaFisica(  $where='' ){

	    $where = !empty($where) ? implode(' AND ', $where): ' 1=1';

		 $sql = "
		    	SELECT DISTINCT
		    		ent.entnome,
				--	ent.entrg||' - '||ent.entorgaoemissorrg AS entrg,
					COALESCE(ent.estuf, 'N/A') AS estuf,
					COALESCE(ent.enttelfone, 'N/A') || ' / ' || COALESCE(ent.entramal, 'N/A') AS enttelfone,
					COALESCE(ent.entemail, 'N/A') AS entemail,
					COALESCE(mun.mundescricao, 'N/A') AS mundescricao,
		    		COALESCE(TRIM(to_char(entcpfcnpj::float,'000\".\"000\".\"000\"-\"00')), 'N/A') AS entcpfcnpj,
					ent.entid
				FROM agenda.entidade ent
				JOIN
					agenda.tipoentidade tpent ON tpent.tpeid = ent.tpeid
				LEFT JOIN
					territorios.municipio mun ON ent.muncod = mun.muncod
				LEFT JOIN
					territorios.estado est ON est.estuf = mun.estuf
				LEFT JOIN
					agenda.entidadedeficiencia def ON def.entid = ent.entid
				LEFT JOIN
					agenda.idiomaentidade ide ON ide.entid = ent.entid
				LEFT JOIN
					agenda.pessoa_entidade pet ON pet.entidpessoa = ent.entid
				LEFT JOIN
					agenda.cargoentidade cae ON pet.caeid = cae.caeid
				LEFT JOIN
					agenda.entidadebensmateriais ebm ON ebm.entid = ent.entid
				WHERE
					ent.entstatus = 'A' AND ent.tpeid = '".ENTIDADE_PESSOA."' AND ".$where;

//		 $rs = $this->carregar($sql);

		 return Paginacao::getPaginacao($this, $sql, 50);
//		 return $rs;
     }

    function listarPessoaJuridica(  $where='' ){

	    $where = !empty($where) ? implode(' AND ', $where): ' 1=1';

		 $sql = "SELECT
		    		ent.entnome,
					COALESCE(ent.estuf, 'N/A') AS estuf,
					COALESCE(ent.enttelfone, 'N/A') || ' / ' || COALESCE(ent.entramal, 'N/A') AS enttelfone,
					COALESCE(ent.entemail, 'N/A') AS entemail,
					COALESCE(mun.mundescricao, 'N/A') AS mundescricao,
		    		COALESCE(TRIM(to_char(ent.entcpfcnpj::float,'00\".\"000\".\"000\"/\"0000\"-\"00')), 'N/A') AS entcpfcnpj,
					ent.entid
				 FROM agenda.entidade ent
				 JOIN
					agenda.tipoentidade tpent ON tpent.tpeid = ent.tpeid
				 LEFT JOIN
					territorios.municipio mun ON ent.muncod = mun.muncod
				 LEFT JOIN
					territorios.estado est ON est.estuf = mun.estuf
				 WHERE
					ent.entstatus = 'A'
					AND ent.tpeid != '".ENTIDADE_PESSOA."'
					AND ".$where;
//		 $rs = $this->carregar($sql);
		 return Paginacao::getPaginacao($this, $sql, 50);
//		 return $rs;
     }


    function recuperarHierarquia( $entid ){
		$arrHierarquia = array($entid);
    	$continuar = true;
		while($continuar){
			$entid = $this->recuperarUm('entidpai', array("entid = {$entid}"));
			if ($entid)
				$arrHierarquia[] = $entid;
			else
				$continuar = false;
		}

    	return array_reverse($arrHierarquia);
    }

    function recuperarFilhos( $entid ){
		$arrHierarquia = array($entid);
    	$continuar = true;
		while($continuar){
			$entid = $this->recuperarUm('entidpai', array("entid = {$entid}"));
			if ($entid)
				$arrHierarquia[] = $entid;
			else
				$continuar = false;
		}

    	return array_reverse($arrHierarquia);
    }

	function listarPessoasVinculadas( $entid , $where ='' ){


		$where = !empty($where) ? implode(' AND ', $where): " pet.petstatus = 'A' ";

		 $sql = "
	    	SELECT
	    		TRIM(to_char(ent.entcpfcnpj::float,'000\".\"000\".\"000\"-\"00')) || ' / ' || ent.entnome AS entcpfcnpj,
	    		cae.caedetalhe,
	    		COALESCE(pet.pettelefone,'N/A') || ' / ' || COALESCE(pet.petramal,'N/A') AS enttelfone,
	    		COALESCE(pet.petemail,'N/A'),
				COALESCE(to_char(pet.petdtentrada, 'DD/MM/YYYY'),'N/A') AS petdtentrada,
	 			COALESCE(to_char(pet.petdtsaida, 'DD/MM/YYYY'),'N/A') AS petdtsaida,
				CASE pet.petstatus WHEN 'A' THEN 'Ativo'
            		 			   WHEN 'I' THEN 'Inativo'
       			END AS petstatus,
				ent.entid,
				pet.petid
			FROM agenda.entidade ent
			LEFT JOIN
				territorios.municipio mun ON ent.muncod = mun.muncod
			LEFT JOIN
				territorios.estado est ON est.estuf = mun.estuf
			JOIN
				agenda.tipoentidade tpent ON tpent.tpeid = ent.tpeid
			JOIN
				agenda.pessoa_entidade pet ON pet.entidpessoa = ent.entid
											  AND pet.entid = '{$entid}'
			JOIN
				agenda.cargoentidade cae ON cae.caeid = pet.caeid
			JOIN
				agenda.cargo car ON car.carid = cae.carid
			WHERE
				ent.entstatus = 'A' AND ent.tpeid = '".ENTIDADE_PESSOA."' AND ".$where;
		 $rs = $this->carregar($sql);
		 return $rs;
	}

    function recuperarCabecalho(  $entid ){

		 $sql = "
		    	SELECT
		    		ent.entnome,
					mun.mundescricao||'-'||ent.estuf AS mundescricao,
					ent.entemail
				FROM
					agenda.entidade ent
				LEFT JOIN
					territorios.municipio mun ON ent.muncod = mun.muncod
				WHERE
					ent.entstatus = 'A' AND ent.entid = {$entid}";
		 $rs = $this->pegaLinha($sql);
		 return $rs;
    }

    function recuperarDados($entid = null, Array $where = array())
    {
    	if ($entid)
	    	array_push($where, "entid = {$entid}");

    	$where = (count($where) ? implode(' AND ', $where) : '');
    	$sql = <<<STR
		    	SELECT
		    			   entid, tpeid, entidpai , e.estuf, e.muncod, arqid,vifid,
		    			   CASE LENGTH(entcpfcnpj)
				              WHEN 11 THEN TRIM(to_char(entcpfcnpj::float,'000"."000"."000"-"00'))
							  WHEN 14 THEN TRIM(to_char(entcpfcnpj::float,'00"."000"."000/0000"-"00'))
					       END AS entcpfcnpj,
		    			   entnome, entstatus, entsiape,
		    			   to_char(entcep::float,'00000"-"000') AS entcep, entlog, entcomplemento, entbairro, entnumero,
					       medlatitude, modlongitude, entzoom,
					       to_char(entdtnascimento, 'DD/MM/YYYY') AS entdtnascimento, entrg, entorgaoemissorrg,
					       to_char(entdtemissaorg, 'DD/MM/YYYY') AS entdtemissaorg, enttituloeleitor, entfax, enttelfone, entcelular,
					       entramal, entemail, entobservacao, entnomemae, entnomepai, m.mundescricao AS cidade, entnomesocial,
					       escid, orsid, idgid, corid, cefid, oroid, entpispasep, entgraduacao,entsexo, entramalprofissional, entemailprofissional, enttelfoneprofissional,
					       enttelfoneprofissional, entramalprofissional, entemailprofissional
					    FROM
					  		agenda.entidade e
					  	LEFT JOIN territorios.municipio m on m.muncod = e.muncod
					  	WHERE
					  		{$where};
STR;

		$rs = $this->pegaLinha($sql);
		if($rs){
			if($rs['medlatitude']){
				$medlatitude  = explode('.', $rs['medlatitude']);
				$rs['graulatitude'] = $medlatitude[0];
				$rs['minlatitude']  = $medlatitude[1];
				$rs['seglatitude']  = $medlatitude[2];
				$rs['pololatitude'] = trim($medlatitude[3]);
			}

			if($rs['modlongitude']){
				$modlongitude  = explode('.', $rs['modlongitude']);
				$rs['graulongitude'] = $modlongitude[0];
				$rs['minlongitude']  = $modlongitude[1];
				$rs['seglongitude']  = $modlongitude[2];
			}
		}

    	return $rs;
    }

    function listarPorCargo ( $carid ){
    	 $sql = "
				SELECT DISTINCT
					ent.entnome,
					COALESCE(ent.entramal, 'N/A') AS enttelfone,
					COALESCE(ent.entemail, 'N/A') AS entemail,
					COALESCE(entpessoa.entnome, 'N/A') AS entpessoanome,
					COALESCE(entpessoa.entramalprofissional, 'N/A') AS entpessoatelfone,
					COALESCE(entpessoa.entemailprofissional, 'N/A') AS entpessoaemail,
					ent.entid,
					entpessoa.entid AS entidpessoa,
					CASE
						WHEN ent.entid IS NOT NULL
							THEN 'agenda.php?modulo=principal/popup/exibirEntidade&acao=O&entid=' || ent.entid
							ELSE '#'
					END AS urlorgao,
					CASE
						WHEN entpessoa.entid IS NOT NULL
							THEN 'agenda.php?modulo=principal/popup/exibirEntidade&acao=P&entid=' || entpessoa.entid
							ELSE '#'
					END AS urlpessoa
				FROM
					agenda.entidade ent
				JOIN
					agenda.cargoentidade cae ON cae.entid = ent.entid
				LEFT JOIN
					agenda.pessoa_entidade pet ON pet.caeid = cae.caeid
				LEFT JOIN
					agenda.entidade entpessoa  ON entpessoa.entid = pet.entidpessoa
				WHERE
					ent.entstatus = 'A' AND entpessoa.entstatus = 'A' AND cae.caestatus = 'A' AND pet.petstatus = 'A' AND cae.carid = '{$carid}'
				ORDER BY
				    ent.entnome, entpessoanome";

     $rs = $this->carregar($sql);
	 return $rs;
    }

    function listarCargosVinculados ( $entid , $comArvore =false ){
    	 $sql = "
				SELECT
					cae.caedetalhe,
					entpessoa.entnome,
					entpessoa.entramalprofissional AS enttelfone,
					entpessoa.entemailprofissional AS entemail,
					car.carid,
					entpessoa.entid
				FROM
					agenda.cargo car
				JOIN
					agenda.cargoentidade cae ON cae.carid = car.carid
								    AND cae.caestatus = 'A'
				JOIN
					agenda.entidade ent ON ent.entid = cae.entid
							       AND ent.entstatus = 'A'
				LEFT JOIN
					agenda.pessoa_entidade pet ON pet.caeid = cae.caeid
												  AND pet.petstatus = 'A'
				LEFT JOIN
					agenda.entidade entpessoa ON pet.entidpessoa = entpessoa.entid
												AND entpessoa.entstatus = 'A'

				WHERE
					ent.entid = {$entid}
				ORDER BY
					cae.caedetalhe, entpessoa.entnome";

	 $rs = $this->carregar($sql);
	 return $rs;
    }


    function listarDadosOrgao( Array $where = array() ){

    	$where = !empty($where) ? implode(' AND ', $where): " 1=1 ";

    	 $sql = "
				SELECT
					ent.entnome,
					ent.enttelfone,
					ent.estuf||' - '|| mun.mundescricao,
					ent.entemail,
					ent.entid
				FROM
					agenda.entidade ent
				LEFT JOIN
					territorios.municipio mun on mun.muncod = ent.muncod
				WHERE
					{$where}
				ORDER BY
					ent.entnome";

		 $rs = $this->carregar($sql);
		 return $rs;
    }


    function listarDadosPessoa( Array $where = array() ){

    	$where = !empty($where) ? implode(' AND ', $where): " 1=1 ";

    	 $sql = "
				SELECT
					ent.entnome,
					ent.entnomesocial,
					ent.entramalprofissional || ' ' AS enttelfone,
					ent.entemailprofissional AS entemail,
					(SELECT
                        entnome
                     FROM
                        agenda.pessoa_entidade pet
                     JOIN agenda.entidade org ON org.entid = pet.entid
                                     AND org.entstatus = 'A'
                        WHERE
                            pet.petstatus = 'A'
                            AND pet.entidpessoa = ent.entid
                        LIMIT 1
                    ) AS entnomeorgao,
					ent.entid
				FROM
					agenda.entidade ent
				WHERE
					{$where}
				ORDER BY
					ent.entnome";

		 $rs = $this->carregar($sql);
		 return $rs;
    }

    function recuperarOrgaosPorPessoa($entid){
    	$sql = "
			SELECT
				ent.entid AS codigo,
				ent.entnome AS descricao
			FROM
				agenda.entidade ent
			INNER JOIN
				agenda.cargoentidade cae ON ent.entid = cae.entid AND cae.caestatus = 'A'
			INNER JOIN
				agenda.pessoa_entidade pet ON cae.caeid = pet.caeid AND pet.petstatus = 'A' AND pet.entidpessoa = {$entid}
			WHERE
				ent.entstatus = 'A' AND ent.tpeid != " . ENTIDADE_PESSOA;
		 return $this->carregar($sql);
    }

    function getByWhere(array $arWhere){

        $where = implode(' AND ', $arWhere);

        $sql = "SELECT
                    ent1.entid,
                    ent1.entcpfcnpj,
                    ent1.entnome,
                    ent1.entnome || ' (' || TRIM(to_char(ent1.entcpfcnpj::float,'000\".\"000\".\"000\"-\"00')) || ')' AS entnomecpf,
                    ent2.entid   AS entidcoordenacao,
                    ent2.entnome AS entcoordenacao,
                    ent3.entid   AS entidsecretaria,
                    ent3.entnome AS entsecretaria
                FROM
                    agenda.entidade ent1
                INNER JOIN
                    agenda.pessoa_entidade pet
                        ON pet.entidpessoa = ent1.entid
                           AND pet.petdtsaida IS NULL
                           AND pet.petstatus = 'A'
                INNER JOIN
                    agenda.entidade ent2 ON ent2.entid = pet.entid
                INNER JOIN
                    agenda.entidade ent3 ON ent3.entid = ent2.entidpai
                WHERE
                    ent1.tpeid = " . ENTIDADE_PESSOA . "
                    AND ent1.entstatus = 'A'
                    AND {$where}";

		 return $this->pegaLinha($sql);
    }

}


