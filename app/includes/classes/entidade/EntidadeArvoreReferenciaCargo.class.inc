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
class  EntidadeArvoreReferenciaCargo
{
	private $arvoreJSON;
	private $entid;
	private $caeidpaiMarcado;
	private $somenteExpandir;
	private $caeidpai;
	private $caeidAtual;
//	private $carid;
	
	public function __construct($entid, $somenteExpandir=false, $caeidpai, $caeidpaiMarcado,$caeidAtual)
	{
		$this->entid 		   = $entid;
		$this->caeidpaiMarcado = $caeidpaiMarcado;
		$this->caeidpai 	   = $caeidpai;
		$this->somenteExpandir = $somenteExpandir;
		$this->arvoreJSON 	   = array();//array();
		$this->caeidAtual 	   = $caeidAtual;
	}
	
	public function getArvoreJSON()
	{
		$this->arvoreJSON = $this->somenteExpandir == false ? self::montarArvoreJSON() : self::montarArvoreFilhoJSON();
		return json_encode($this->arvoreJSON);
	}
	
	private function montarArvoreFilhoJSON(Array $entidRaiz=array())
	{
		$dadoArvore = array();
		
		$cargo = new CargoEntidade();
		$dadosCargo = $cargo->comboCargosVinculados( $this->entid , $this->caeidpai);

		if (is_array($dadosCargo)){	
			for ($i=0; $i<count($dadosCargo); $i++){
				$dadosCargo[$i] = array_map('tratarRetorno', $dadosCargo[$i]);
				if($cargo->comboCargosVinculadosCount($this->entid, $dadosCargo[$i]['codigo']) > 0)
					$param['state'] = 'closed';
				
				$param['negrito'] = ($dadosCargo[$i]['codigo'] == $this->caeidpaiMarcado ? true : false);
				array_push($dadoArvore, self::agregaArvoreJSON($dadosCargo[$i], $param));
			}		
		}
		return $dadoArvore;
	}
	
	private function montarArvoreJSON($caeidpaiMatriz = '')
	{
		$dadoArvore = array();
		$cargo      = new CargoEntidade();

		if($this->caeidpaiMarcado != -1)
			$hierarquiaDeCargos = $cargo->recuperarHierarquia($this->caeidpaiMarcado);
		
		if($caeidpaiMatriz == '')
			$dadosCargo = $cargo->comboCargosVinculados($this->entid);
		else
			$dadosCargo = $cargo->comboCargosVinculados($this->entid, $caeidpaiMatriz);
		
		if($dadosCargo){
			foreach ($dadosCargo as $dadoCargo){
				$dadoCargo = array_map('tratarRetorno', $dadoCargo);
				if($cargo->comboCargosVinculadosCount($this->entid, $dadoCargo['codigo']) > 0 )
					$param['state'] = 'closed';
				
				$param['negrito'] = ($dadoCargo['codigo'] == $this->caeidpaiMarcado ? true : false);
				array_push($dadoArvore, self::agregaArvoreJSON($dadoCargo, $param));
				
				if(!$param['negrito'] && is_array($hierarquiaDeCargos) && in_array($dadoCargo['codigo'], $hierarquiaDeCargos)){
					$count = count($dadoArvore)-1;
					array_push($dadoArvore[$count]->children, self::montarArvoreJSON($dadoCargo['codigo']));
					$dadoArvore[$count]->state = 'open';
				}
			}
		}else{
			array_push($dadoArvore,self::agregaArvoreJSON(array('codigo'=>-1,'descricao'=>'Esta entidade não possui cargos cadastrados.')));
		}

		return $dadoArvore;
	}
	
	private function agregaArvoreJSON(Array $dados, Array $param = array()){
		$chek = '<input id="chbx_'.$dados['codigo'].'" type="checkbox" ' . (($dados['codigo'] == $this->caeidpaiMarcado) ? 'checked="checked"' : '') . ' onclick="markRefe({caeidpai:'.$dados['codigo'].', caedetalhe:\''.$dados['descricao'].'\'}, this);">';
		if($dados['codigo'] == $this->caeidAtual || $dados['codigo'] == -1)
			$txt = '<label>'.$dados['descricao'].'</label>';		
		else
			$txt = $chek . '<label for="chbx_'.$dados['codigo'].'">'.$dados['descricao'].'</label>';
		
		$txt = ($param['negrito'] ? self::addNegrito($txt) : $txt);
		
		$d = new stdClass;
		$d->attr  = array("id" => $dados['codigo'], "caeidpai" => $dados['codigo']);
		$d->data  = $txt;
		$cargo = new CargoEntidade();
		if ($dados['caeidpai'] !== $this->caeidpaiMarcado && $cargo->comboCargosVinculadosCount($this->entid, $dados['codigo']) > 0 && ($dados['codigo'] != $this->caeidAtual))
			$d->state = ($param['state'] ? $param['state'] : 'closed');
		
		$d->children = array();
		return $d;
	}
	
	private function addNegrito($txt)
	{
		return '<b>' . $txt . '</b>';
	}
}