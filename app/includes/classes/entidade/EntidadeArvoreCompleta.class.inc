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
class  EntidadeArvoreCompleta
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
//		dbg($this->arvoreJSON);
//		return json_encode(array($this->arvoreJSON));
		return json_encode($this->arvoreJSON);
	}

	public function getArvoreCargoJSON($caeidpai)
	{
		$this->arvoreJSON = self::montarArvoreCargoJSON($caeidpai);

		return json_encode($this->arvoreJSON);
	}

	public function getArvorePessoaJSON($entid, $caeid)
	{
		$this->entid = $entid;
		$this->arvoreJSON = self::montarArvorePessoaJSON($entid, $caeid);

		return json_encode($this->arvoreJSON);
	}

	private function montarArvoreFilhoJSON(Array $entidRaiz=array())
	{
		$dadoArvore = array();
		$entidade = new Entidade();

		$entidRaiz = count($entidRaiz) ? $entidRaiz : array($this->entid);
		$entid = array_shift($entidRaiz);

		$dados = $entidade->recuperarTodos( "entid, entnome", array("entidpai = {$entid}", "entstatus='A'"), "entordem ASC");
		if (count($dados)){
			$param['totalOrdem'] = count($dados);
			// ADD icone cargo
			array_unshift($dadoArvore, self::agregaIconeCargo($entid));
			for ($i=0; $i<count($dados); $i++){
				$param['numItem'] = ($i + 1);
				$dados[$i] = array_map('tratarRetorno', $dados[$i]);
				$param['state']   = 'closed';
				array_push($dadoArvore, self::agregaArvoreJSON($dados[$i], $param));
			}
		}else{
			// ADD icone cargo
			array_unshift($dadoArvore, self::agregaIconeCargo($entid));
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
			$param['totalOrdem'] = false;
			array_push($dadoArvore, self::agregaArvoreJSON($dados, $param));
			array_unshift($dadoArvore[0]->children, self::agregaIconeCargo($dados['entid']));
			$loop = false;
			unset($param);
		}else{
			$entidRaiz = count($entidRaiz) ? $entidRaiz : array($this->entid);
			$entid = array_shift($entidRaiz);
			$loop = true;
		}
		$dados = $entidade->recuperarTodos( "entid, entnome", array("entidpai = {$entid}", "entstatus='A'"), "entordem ASC");
		$param['totalOrdem'] = count($dados);
		for ($i=0; $i<count($dados); $i++){
			$param['numItem'] = ($i + 1);
			$dados[$i] 		  = array_map('tratarRetorno', $dados[$i]);

			if ($dados[$i]['entid'] == current($entidRaiz)){
				$param['state'] = 'open';
			}else{
				$param['state'] = 'closed';
			}
			$param['negrito'] = ($dados[$i]['entid'] == $this->entid ? true : false);
			if ($loop == true){
				array_push($dadoArvore, self::agregaArvoreJSON($dados[$i], $param));
				if ($dados[$i]['entid'] == current($entidRaiz) && count($entidRaiz)){
					$count = count($dadoArvore) -1;
					array_push($dadoArvore[$count]->children, self::montarArvoreJSON($entidRaiz));
					// ADD icone cargo
					array_unshift($dadoArvore[$count]->children, self::agregaIconeCargo($dados[$i]['entid']));
				}
			}else{
				array_push($dadoArvore[0]->children, self::agregaArvoreJSON($dados[$i], $param));
				if ($dados[$i]['entid'] == current($entidRaiz) && count($entidRaiz)){
					$count = count($dadoArvore[0]->children) -1;
					array_push($dadoArvore[0]->children[$count]->children, self::montarArvoreJSON($entidRaiz));
					// ADD icone cargo
					array_unshift($dadoArvore[0]->children[$count]->children, self::agregaIconeCargo($dados[$i]['entid']));
				}
			}
		}
		return $dadoArvore;
	}

	private function agregaArvoreJSON(Array $dados, Array $param = array()){
		$txt = $dados['entnome'];
		$txt = ($param['negrito'] ? self::addNegrito($txt) : $txt);

		if($dados['entid'])
    		$txt = "<a href=\"javascript:janela('?modulo=principal/popup/exibirEntidade&acao=O&entid={$dados['entid']}',900,800,'nome');\">$txt</a> ";

		if ($param['totalOrdem'] !== false){

			$txt  = self::setaDirecional($dados['entid'], $param['numItem'], $param['totalOrdem']) . $txt;
		}
		$d = new stdClass;
		$d->attr  = array("id" => $dados['entid'], "entid" => $dados['entid']);
		$d->data  = array('title' => $txt,
//						  'attr'  => array("href" => ($dados['entid'] ? "javascript:janela('?modulo=principal/popup/cadOrgao&acao=P&entid={$dados['entid']}',900,800,'nome');" : "")));
//						  'attr'  => array("href" => ($dados['entid'] ? "javascript:janela('?modulo=principal/popup/exibirEntidade&acao=O&entid={$dados['entid']}',900,800,'nome');" : ""))
                          );
//		$d->data  = $txt;
		$d->state = ($param['state'] ? $param['state'] : 'closed');
		$d->children = array();

		return $d;
	}

	private function setaDirecional($id, $numItem, $total)
	{
		if ($numItem > 1){
			$srcSubir 	= "/imagens/seta_cima.gif";
			$clickSubir = "sobe('{$id}', {$numItem}, {$total});";
		}else{
			$srcSubir = "/imagens/seta_cimad.gif";
			$clickSubir = "javascript:void(0);";
		}

		if ($numItem != $total){
			$srcDescer 	 = "/imagens/seta_baixo.gif";
			$clickDescer = "desce('{$id}', {$numItem}, {$total});";
		}else{
			$srcDescer 	 = "/imagens/seta_baixod.gif";
			$clickDescer = "javascript:void(0);";
		}
		$img  = "&nbsp;<img id='{$id}_sobe' src='{$srcSubir}' onclick=\"{$clickSubir}\" border='0' title='Subir no posicionamento'>";
		$img .= "&nbsp;{$param['numItem']}&nbsp;";
		$img .= "<img id='{$id}_desce' src='{$srcDescer}' onclick=\"{$clickDescer}\" border='0' title='Descer no posicionamento'>&nbsp;";

		return $img;
	}

	/*
	 * FUNÇÕES DE CARGO
	 */

	private function agregaIconeCargo($entid)
	{
		$icon = new stdClass;
		$icon->attr  = array("id" => "ico_".$entid,
							 "entid" => $entid,
							 "func" => "buscarCargo");
		$icon->data  = array('title' => 'Cargos ',
							 'icon' => '/imagens/consultar.gif');
		$icon->state = "closed";

		return $icon;
	}

	private function montarArvoreCargoJSON($caeidpai)
	{
		$dadoArvore = array();

		$cargoEnt = new CargoEntidade();
		$param['coluna'] = "cae.caedetalhe, cae.caeid, ent.entid";
		$param['filtro'] = array(($caeidpai ? " cae.caeidpai={$caeidpai} " : " cae.caeidpai IS NULL "));
		$dados = $cargoEnt->listarCargosVinculados($this->entid, $param);

		if ($dados){
			for ($i=0; $i<count($dados); $i++){
				$dados[$i] = array_map('tratarRetorno', $dados[$i]);
				array_push($dadoArvore, self::agregaArvoreCargoJSON($dados[$i]));
			}
		}

		if ($caeidpai){
//			array_unshift($dadoArvore, self::agregaIconeNovaPessoa($this->entid, $caeidpai));
			array_push($dadoArvore, self::agregaIconePessoa($this->entid, $caeidpai));
		}
		array_unshift($dadoArvore, self::agregaIconeNovoCargo($this->entid, $caeidpai));
//			array_unshift($dadoArvore, self::agregaIconePessoa($this->entid, $caeidpai));

		return $dadoArvore;
	}

	private function agregaArvoreCargoJSON(Array $dados, Array $param = array())
	{
		$txt = $dados['caedetalhe'];
		$txt = ($param['negrito'] ? self::addNegrito($txt) : $txt);

		$d = new stdClass;
		$d->attr  = array("id" => "car_" . $dados['entid']."_".$dados['caeid'],
						  "entid" => $dados['entid'],
						  "caeid" => $dados['caeid'],
						  "func" => "buscarCargo");
		$d->data  = array('title' => $txt,
						  'icon' => '/imagens/codigo.gif');
		$d->state = ($param['state'] ? $param['state'] : 'closed');
		$d->children = array();

		return $d;
	}

	private function agregaIconeNovoCargo($entid, $caeidpai)
	{
		$url = "?modulo=principal/popup/cadCargoEntidade&acao=I&noheader=1&entid={$entid}&caeidrefe={$caeidpai}";
		$icon = new stdClass;
		$icon->data  = array('title' => 'Novo Cargo',
							 'icon' => '/imagens/gif_inclui.gif',
							 'attr' => array("href" => "javascript:javascript:janela('{$url}', 600, 450, 'novo_cargo');"));
		return $icon;
	}

	/*
	 * FUNÇÕES DE PESSOA
	 */

	private function montarArvorePessoaJSON($entid, $caeid)
	{
		$dadoArvore = array();

		$pessoaEnt = new PessoaEntidade();
		$dados = $pessoaEnt->listarPessoaPorCargoNoOrgao($entid, $caeid);

		array_unshift($dadoArvore, self::agregaIconeNovaPessoa($entid, $caeid));
		if ($dados){
			for ($i=0; $i<count($dados); $i++){
				$dados[$i] = array_map('tratarRetorno', $dados[$i]);
				array_push($dadoArvore, self::agregaArvorePessoaJSON($dados[$i]));
			}
		}

		return $dadoArvore;
	}

	private function agregaArvorePessoaJSON(Array $dados, Array $param = array())
	{
		$entid = $dados['entid'];
		$txt = $dados['entnome'];
		$txt = ($param['negrito'] ? self::addNegrito($txt) : $txt);

		$d = new stdClass;
		$d->data  = array('title' => $txt,
		  				  'attr' => array("href" => "javascript:janela('?modulo=principal/popup/cadEntidade&acao=P&entid=$entid',700,800,'nome');"),
		  				  //'attr' => array("href" => "javascript:alert('oi')"),
						  'icon' => (in_array($dados['idgid'], array('', 2)) == 2 ? '/imagens/usuario.gif' : '/imagens/usuaria.gif'));

		return $d;
	}

	private function agregaIconePessoa($entid, $caeid)
	{
		$icon = new stdClass;
		$icon->attr  = array("id" 	 => "pes_".$entid."_".$caeid,
							 "entid" => $entid,
							 "caeid" => $caeid,
							 "func"	 => "buscarPessoa");
		$icon->data  = array('title' => ' Pessoas Vinculadas ',
							 'icon' => '/imagens/pessoas.png');
		$icon->state = "closed";

		return $icon;
	}

	private function agregaIconeNovaPessoa($entid, $caeid)
	{
		$cargo = new Cargo();
		$preenchido = $cargo->verificaCargoUnicoPreenchido($entid, $caeid);

		$icon = new stdClass;
		if (!$preenchido){
			$url = "?modulo=principal/vincularPessoas&acao=A&noheader=1&entid={$entid}&caeid={$caeid}";
			$icon->data  = array('title' => 'Vincular Pessoa',
								 'icon' => '/imagens/gif_inclui.gif',
								 'attr' => array("href" => "javascript:janela('{$url}', 600, 400, 'nova_pessoa');"));
		}else{
			$msg = tratarRetorno('Não é possível vincular mais de uma pessoa para este cargo!');
			$icon->data  = array('title' => 'Vincular Pessoa',
								 'icon' => '/imagens/gif_inclui_d.gif',
								 'attr' => array("href" => "javascript:alert('" . $msg . "');"));
		}

		return $icon;
	}

	/*
	 * GERAL
	 */
	public static function reordenarOrgao($entid, $entordem)
	{
		// Busca entidade para nova ordem
		$entidade 			= new Entidade( $entid );
//		$entordemAntiga 	= $entidade->entordem;
		$entidpai 			= $entidade->entidpai;

		if ( $entidpai ){
    		$arEntidade = $entidade->recuperarTodos("entid, entordem", array("entidpai = {$entidpai}", "entstatus='A'", "entid != {$entid}"), 'entordem');

            $ordem = 1;
    		foreach ($arEntidade as $dadoEntidade){
    		    if ( $ordem == $entordem ){
                    $entidade = new Entidade( $entid );
                    $entidade->entordem = $ordem;
                    $entidade->salvar();
                    $entidade->commit();

                    $ordem++;
    		    }

                $entidade = new Entidade( $dadoEntidade['entid'] );
                $entidade->entordem = $ordem;
                $entidade->salvar();
                $entidade->commit();

                $ordem++;
    		}
		}
	}

	private function addNegrito($txt)
	{
		return '<b>' . $txt . '</b>';
	}
}