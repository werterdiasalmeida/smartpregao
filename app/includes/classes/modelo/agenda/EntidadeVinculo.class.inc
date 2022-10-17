<?php
	
class EntidadeVinculo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.entidadevinculo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "envid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'envid' 			=> null, 
									  	'tevid' 			=> null, 
									  	'entidpessoa'	    => null,
									  	'entidcontato'	    => null,
									  	'entidorgao'	    => null,
									  	'envdata'	    	=> null,
									  	'envobservacao'	    => null,
									  	'envnivel'	    	=> null,
									  	'envstatus'	    	=> null,
									  	'envdatacadastro'	=> null,
									  	'usucpf'	    	=> null
									  );

	public function listar($entid, Array $where=array())
	{
		$sql = "SELECT
					'(' || TRIM(to_char(e.entcpfcnpj::float,'000\".\"000\".\"000\"-\"00')) || ') ' || e.entnome AS entcontato,
					'(' || COALESCE(TRIM(to_char(e1.entcpfcnpj::float,'00\".\"000\".\"000\"/\"0000\"-\"00')), 'N/A') || ') ' || COALESCE(e1.entnome, 'N/A') AS entorgao,
					tev.tevdsc,
					COALESCE(CAST(ev.envnivel AS character),'N/A') AS envnivel,
					u.usunome,
					to_char(ev.envdata, 'DD-MM-YYYY') AS envdata,
					ev.entidcontato,
					ev.entidorgao,
					ev.envobservacao,
					'(' || TRIM(to_char(e2.entcpfcnpj::float,'000\".\"000\".\"000\"-\"00')) || ') ' || e2.entnome AS entpessoa,
					ev.envid
				FROM
					agenda.entidadevinculo ev
				JOIN agenda.entidade e ON e.entid = ev.entidcontato AND e.entstatus = 'A'
				LEFT JOIN agenda.entidade e1 ON e1.entid = ev.entidorgao
				JOIN agenda.entidade e2 ON e2.entid = ev.entidpessoa AND e2.entstatus = 'A'
				JOIN agenda.tipoentidadevinculo tev ON tev.tevid = ev.tevid
				LEFT JOIN seguranca.usuario u ON u.usucpf = ev.usucpf
				WHERE
					ev.envstatus = 'A' AND
					ev.entidpessoa = {$entid}
					".($where ? " AND ".implode(" AND ", $where) : "")."";
		$dados = $this->carregar($sql);
		return $dados ? $dados : array();
	}
	
	
	public function carregaDados($envid)
	{
		$sql = "SELECT
				".(implode(", ", array_keys($this->arAtributos))).",
				'(' || COALESCE(TRIM(to_char(e.entcpfcnpj::float,'00\".\"000\".\"000\"/\"0000\"-\"00')), 'N/A') || ') ' || e.entnome AS entorgao
			    FROM
			   		agenda.entidadevinculo ev
			   	JOIN agenda.entidade e ON e.entid = ev.entidorgao	
			    WHERE
			    	ev.envstatus = 'A' AND
			    	ev.envid = {$envid}";	
		
		return $this->pegaLinha($sql);
	}
	
	public function inativar($envid)
	{
		$sql = "UPDATE agenda.entidadevinculo
				SET envstatus = 'I'
				WHERE
					envid = {$envid};";
		$this->executar($sql);
	}
	
	public function pegaEntidadeComMaiorVinculo(){
		$sql = "SELECT 
					ent.entid,
					COUNT(*) AS total
				FROM 
					agenda.entidadevinculo entv
				INNER JOIN 
					agenda.entidade ent ON ent.entid = entv.entidcontato
							       OR ent.entid = entv.entidpessoa
				GROUP BY
					ent.entid
				ORDER BY 
					total desc
				LIMIT 1";
		$rs = $this->pegaUm($sql);
		return $rs ? $rs : null;
	}
}
