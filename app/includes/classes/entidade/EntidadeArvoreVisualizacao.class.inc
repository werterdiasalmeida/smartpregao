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
class  EntidadeArvoreVisualizacao
{
	private $arvoreJSON;
	private $entid;
	private $somenteExpandir;
//	private $carid;

	public function __construct($entid, $somenteExpandir=false)
	{
		$this->entid 		   = $entid;
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
		$dadoArvore = self::montarArvorePessoaJSON($entid, $dadoArvore);
		$dados = $entidade->recuperarTodos( "entid, entnome", array("entidpai = {$entid}", "entstatus='A'"), "entordem ASC");
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
			$entidRaiz 	= $entidade->recuperarHierarquia($this->entid);
			$entid 		= array_shift($entidRaiz);
			$dados 		= $entidade->recuperarLinha("entid, entnome", array("entid = {$entid}", "entstatus='A'"), false);
			$dados 		= array_map('tratarRetorno', $dados);
			$param['state'] = 'open';
			array_push($dadoArvore, self::agregaArvoreJSON($dados, $param));
			// ADD pessoas vinculadas
			$dadoArvore[0]->children = self::montarArvorePessoaJSON($entid, $dadoArvore[0]->children);
			$loop = false;
		}else{
			$entidRaiz = count($entidRaiz) ? $entidRaiz : array($this->entid);
			$entid = array_shift($entidRaiz);
			$loop = true;
		}
		$dados = $entidade->recuperarTodos( "entid, entnome", array("entidpai = {$entid}", "entstatus='A'"), "entordem ASC");
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
					// ADD pessoas vinculadas
					$dadoArvore[$count]->children = self::montarArvorePessoaJSON($dados[$i]['entid'], $dadoArvore[$count]->children);
				}
			}else{
				array_push($dadoArvore[0]->children, self::agregaArvoreJSON($dados[$i], $param));
				if ($dados[$i]['entid'] == current($entidRaiz) && count($entidRaiz)){
					$count = count($dadoArvore[0]->children) -1;
					array_push($dadoArvore[0]->children[$count]->children, self::montarArvoreJSON($entidRaiz));
					// ADD pessoas vinculadas
					$dadoArvore[0]->children[$count]->children = self::montarArvorePessoaJSON($dados[$i]['entid'], $dadoArvore[0]->children[$count]->children);
				}
			}
		}

		return $dadoArvore;
	}

	private function agregaArvoreJSON(Array $dados, Array $param = array()){
		$txt = $dados['entnome'];
		$txt = ($param['negrito'] ? self::addNegrito($txt) : $txt);

		$d = new stdClass;
		$d->attr  = array("id" => $dados['entid'],
						  "entid" => $dados['entid']);
		$d->data  = array('title' => $txt,
						  'attr'  	 => array("href" => ($dados['entid'] ? "javascript:janela('?modulo=principal/popup/exibirEntidade&acao=O&entid={$dados['entid']}',900,800,'nome');" : "")));

		//verifica se a entidade possui filhos(outras entidades ou pessoas vinculadas), se possuir aparece o ícone de [+] ou [-]
		$entidade = new Entidade();
		$filhosEntidade = $entidade->recuperarUm('count(*) as count', array("entidpai = {$dados['entid']}"));
		$pessoasEntidade = $entidade->listarPessoasVinculadas($dados['entid']) ? 1 : 0;

		if($filhosEntidade > 0 || $pessoasEntidade > 0)
			$d->state = ($param['state'] ? $param['state'] : 'closed');
		//fim verifica filhos entidade

		$d->children = array();

		return $d;
	}

	/*
	 * PESSOAS início
	 */
	private function montarArvorePessoaJSON($entid, Array $dadoArvore=array(), $caeid=null)
    {
        $cargoEnt = new CargoEntidade();
        if($caeid)
            $dadosCargo = $cargoEnt->recuperarColuna("caeid", array("caestatus = 'A'", "caeidpai = {$caeid}"), 'caeordem, caedetalhe');
        else
            $dadosCargo = $cargoEnt->recuperarColuna("caeid", array("caestatus = 'A'", "entid = {$entid}", "caeidpai IS NULL"), 'caeordem, caedetalhe');
        //inverte o array para que os cargos fiquem na mesma ordem da ArvoreCompleta
        $dadosCargo = array_reverse($dadosCargo);
        foreach ($dadosCargo as $cargo){
        	$dados = $cargoEnt->listarCargosComPessoa($entid, array('where'  => array("cae.caeid = {$cargo}"),
        														    "coluna" => "e.entid, UPPER(e.entnome) AS entnome, e.idgid, cae.caedetalhe"));

        	//verifica se há dados e se a entidade não é vazia
        	if ($dados){
                for ($i=0; $i<count($dados); $i++){
                    $dados[$i] = array_map('tratarRetorno', $dados[$i]);
                    array_unshift($dadoArvore, self::agregaArvorePessoaJSON($dados[$i]));
                }
                $count = 0;//count($dadoArvore) - 1;
                array_push($dadoArvore[$count]->children, $this->montarArvorePessoaJSON($entid, $dadoArvore[$count]->children, $cargo));
            }
    	}

        return $dadoArvore;
    }

	private function agregaArvorePessoaJSON(Array $dados, Array $param = array())
	{
		$txt = $dados['entnome'] . " (". $dados['caedetalhe'] .")";
		$txt = ($param['negrito'] ? self::addNegrito($txt) : $txt);

		$d = new stdClass;
		$d->data  = array('title' 	 => $txt,
						  'attr'  	 => array("href" => ($dados['entid'] ? "javascript:janela('?modulo=principal/popup/exibirEntidade&acao=P&entid={$dados['entid']}',900,800,'nome');" : "")),
						  'icon'  	 => (in_array($dados['idgid'], array('', 2)) == 2 ? '/imagens/usuario.gif' : '/imagens/usuaria.gif'));
		$d->children = array();



		return $d;
	}
	/*
	 * PESSOAS fim
	 */

	private function addNegrito($txt)
	{
		return '<b>' . $txt . '</b>';
	}
}