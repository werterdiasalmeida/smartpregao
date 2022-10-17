<?php
/*
[{
	"attr":{"id":"1"},
	"data":"1 \/ Teste2222",
	"state":"open",
	"children":[{
					"attr":{"id":"53"},
					"data":"53 \/ Teste 1.1",
					"state":"open",
					"children":[]
				},{
					"attr":{"id":"35"},
					"data":"35 \/ Teste Mapa",
					"state":"open",
					"children":[{
									"attr":{"id":"41"},
									"data":"41 \/ Teste \u00d3rg\u00e3o Mapa",
									"state":"open",
									"children":[]
								}]
				}]
}]
 */
class  EntidadeArvoreReferencia
{
	private $arvoreJSON;
	private $entid;
	private $entidOrgao;
	private $somenteExpandir;
//	private $carid;
	
	public function __construct($entid, $somenteExpandir=false)
	{
		$this->entid 		   = $entid;
		$this->entidOrgao 	   = $_SESSION['agenda']['orgao']['entid'];
		$this->somenteExpandir = $somenteExpandir;
		$this->arvoreJSON 	   = array();//array();
	}
	
	public function getArvoreJSON()
	{
		$this->arvoreJSON = $this->somenteExpandir == false ? self::montarArvoreJSON() : self::montarArvoreFilhoJSON();
		return json_encode($this->arvoreJSON);
	}
	
	private function montarArvoreFilhoJSON(Array $entidRaiz=array())
	{
		$dadoArvore = array();
		$entidade 	= new Entidade();
		
		$entidRaiz = count($entidRaiz) ? $entidRaiz : array($this->entid);
		$entid 	   = array_shift($entidRaiz);
		// ADD pessoas vinculadas	
//		$dadoArvore = self::montarArvorePessoaJSON($entid, $dadoArvore);

		$where = array("entidpai = {$entid}", "entstatus='A'");
//		if ($this->entidOrgao)
//			array_push($where, "entid != {$this->entidOrgao}");
		
		$dados = $entidade->recuperarTodos( "entid, '(' || COALESCE(to_char(entcpfcnpj::float,'00\".\"000\".\"000\"/\"0000\"-\"00'), 'N/A') || ') ' || entnome AS entnome", $where, "entordem ASC"); 	
		if (count($dados)){	
			for ($i=0; $i<count($dados); $i++){
				$dados[$i] = array_map('tratarRetorno', $dados[$i]);
				$param['state']   = 'closed';
				array_push($dadoArvore, self::agregaArvoreJSON($dados[$i], $param));
			}		
		}
		return $dadoArvore;
	}
	
	private function montarArvoreJSON(Array $entidRaiz=array())
	{
		$dadoArvore = array();
		$entidade = new Entidade();
		if (!count($entidRaiz)){
			// Busca Hierarquia
			if ($this->entid){
				$entidRaiz 		= $entidade->recuperarHierarquia($this->entid);	
				$entid 			= array_shift($entidRaiz);
			}
			// Busca todos órgãos de primeiro nível
			$where = array("entidpai IS NULL", 
						   "entstatus = 'A'", 
						   "tpeid != " . ENTIDADE_PESSOA);
			
			$entidadeRaizTodos = $entidade->recuperarColuna("entid", $where);
			$entidadeRaizTodos = array_diff($entidadeRaizTodos, array($entid));
			
			foreach ($entidadeRaizTodos as $entidItem){			
				$dados 			= $entidade->recuperarLinha("entid, '(' || COALESCE(TRIM(TO_CHAR(entcpfcnpj::float,'00\".\"000\".\"000\"/\"0000\"-\"00')), 'N/A') || ') ' || entnome AS entnome", array("entid = {$entidItem}", "entstatus='A'"), false);
				$dados 			= array_map('tratarRetorno', $dados);
				$param['state'] = 'closed';
				array_push($dadoArvore, self::agregaArvoreJSON($dados, $param));
			}
			
			if ($this->entid){
				$dados 			= $entidade->recuperarLinha("entid, '(' || COALESCE(TRIM(TO_CHAR(entcpfcnpj::float,'00\".\"000\".\"000\"/\"0000\"-\"00')), 'N/A') || ') ' || entnome AS entnome", array("entid = {$entid}", "entstatus='A'"), false);
				$dados 			= array_map('tratarRetorno', $dados);
				$param['state'] = 'open';
				$param['negrito'] = ($dados['entid'] == $this->entid ? true : false);
				array_unshift($dadoArvore, self::agregaArvoreJSON($dados, $param));
//				$entidRaiz 		= array_diff($entidRaiz, array($this->entidOrgao));
			}else{
				return $dadoArvore;
			}	

			
			$loop = false;
		}else{
			$entidRaiz = count($entidRaiz) ? $entidRaiz : array($this->entid);
			$entid = array_shift($entidRaiz);
			$loop = true;
		}
		
		$where = array("entidpai = {$entid}", "entstatus='A'");
		
		$dados = $entidade->recuperarTodos( "entid, '(' || COALESCE(TRIM(TO_CHAR(entcpfcnpj::float,'00\".\"000\".\"000\"/\"0000\"-\"00')), 'N/A') || ') ' || entnome AS entnome", $where, "entordem ASC"); 	
		for ($i=0; $i<count($dados); $i++){
			$dados[$i] = array_map('tratarRetorno', $dados[$i]);
			
			if ($dados[$i]['entid'] == current($entidRaiz)){
				$param['state']   = 'open';
			}else{
				$param['state']   = 'closed';
			}
			$param['negrito'] = ($dados[$i]['entid'] == $this->entid ? true : false);
			if ($loop == true){
				array_push($dadoArvore, self::agregaArvoreJSON($dados[$i], $param));
				if ($dados[$i]['entid'] == current($entidRaiz) && count($entidRaiz)){
					$count = count($dadoArvore) -1;
					array_push($dadoArvore[$count]->children, self::montarArvoreJSON($entidRaiz));
				}
			}else{
				array_push($dadoArvore[0]->children, self::agregaArvoreJSON($dados[$i], $param));
				if ($dados[$i]['entid'] == current($entidRaiz) && count($entidRaiz)){
					$count = count($dadoArvore[0]->children) -1;
					array_push($dadoArvore[0]->children[$count]->children, self::montarArvoreJSON($entidRaiz));
				}
			}
		}		
		return $dadoArvore;
	}
	
	private function agregaArvoreJSON(Array $dados, Array $param = array()){
		$chek = $dados['entid'] !== $this->entidOrgao  
					? '<input id="chbx_'.$dados['entid'].'" type="checkbox" ' . (($dados['entid'] == $this->entid) ? 'checked="checked"' : '') . ' onclick="markRefe({entid:'.$dados['entid'].', entnome:\''.$dados['entnome'].'\'}, this);">'
					: '';
		$txt = $chek . '<label for="chbx_'.$dados['entid'].'">'.$dados['entnome'].'</label>';
		$txt = ($param['negrito'] ? self::addNegrito($txt) : $txt);
		
		$d = new stdClass;
		$d->attr  = array("id" => $dados['entid'], "entid" => $dados['entid']);
		$d->data  = $txt;
		
		//verifica se a entidade possui filhos, se possuir aparece o ícone de [+] ou [-]
		$entidade = new Entidade();
		$filhosEntidade = $entidade->recuperarUm('count(*) as count', array("entidpai = {$dados['entid']}"));		
		//fim verifica filhos entidade
		
		if ($filhosEntidade > 0 && $dados['entid'] !== $this->entidOrgao)
			$d->state = ($param['state'] ? $param['state'] : 'closed');
		
		$d->children = array();

		return $d;
	}
	
	private function addNegrito($txt)
	{
		return '<b>' . $txt . '</b>';
	}
}