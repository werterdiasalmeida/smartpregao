<?php
class SidorException extends Exception {}
class SidorCargaException extends SidorException {}
class SidorLoginException extends SidorException {}
class SidorCodReferenciaNaoEncontradoException extends SidorException {}
class SidorProdutoFisicoNaoEncontradoException extends SidorException {}

/**
 * Classe para comunicação com o SIDORNET
 *
 */
class Sidor {
	public $s;

	public function __construct() {
		$this->s 			= new Snoopy;
		$this->s->agent 		= "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)";
		$this->s->_isproxy 		= false;
		$this->s->proxy_host	= "";
		$this->s->proxy_port	= "";
		$this->s->proxy_user	= "";
		$this->s->proxy_pass	= "";
	}

	public function login($user, $pass) {
		$url = "https://sidornet.planejamento.gov.br/captacao/loginAction.do";
		$postdata = array("codUsua"=>$user
			,"senUsua"=>$pass
			,"novSen"=>""
			,"confNovSen"=>""
		);
		$this->s->submit($url, $postdata);
		$this->s->setcookies();
		//throw new SidorLoginException();
		if(strpos($this->s->results, "<--  Efetua o ReDirecionamento caso o Browser não seja o MSIE -->")==false) {
		throw new SidorLoginException();
		}
	}
	
	public function pegarCodReferenciaSidor($unicod, $tipoDetalhamento, $prgcod, $acacod, $loccod) {
		$urlReferencia = "https://sidornet.planejamento.gov.br/captacao/paginas/comuns/arvore.jsp?tipo=ofs.QBO.Locg&niv=5&params=INT:2007:;TEXTO:%s:;INT:%s:;INT:%s:;INT:%s:;TEXTO:%s:;TEXTO:%s:";
		$url = sprintf($urlReferencia, $unicod, $tipoDetalhamento, $tipoDetalhamento, $tipoDetalhamento, $prgcod, $acacod);
		$this->s->fetch($url);
		$codreferencia = array();
		dbg($url);
		dbg($this->s->results,1);
		if(preg_match_all("/codRef='([0-9 ]+)'/m", $this->s->results, $matches)) {
			foreach($matches[1] as $m) {
				$codreferencia[] = trim($m);
			}
		}
		else {
			throw new SidorCodReferenciaNaoEncontradoException();			
		}
		if(is_array($codreferencia) && count($codreferencia>0) && preg_match_all("/>(\d{4})<\//m", $this->s->results, $matches)) {
			foreach($matches[1] as $i=>$m) {
				$codreferencia[trim($m)] = $codreferencia[$i];
			}
		}
		else {
			throw new SidorCodReferenciaNaoEncontradoException();
		}
		if(!isset($codreferencia[$loccod]))
			throw new SidorCodReferenciaNaoEncontradoException();		
		return $codreferencia[$loccod];
	}
	
	public function pegarProdutoUnidadeSidor($codreferencia, $unicod, $tipoDetalhamento, $prgcod, $acacod, $loccod) {
		$urlProduto = "https://sidornet.planejamento.gov.br/captacao/ofs/ofsAction.do?parametro=ofs_prop&referencia=%s&acao=%s&momento=%s&orgao=%s&unidade=%s&prog=%s&locg=%s";
		$url = sprintf($urlProduto, $codreferencia, $acacod, $tipoDetalhamento, "26000", $unicod, $prgcod, $loccod);
		$this->s->fetch($url);
		$ret = array();
		if(preg_match("/name=\"cod_bems\" value=\"([0-9 ]+)\"/m", $this->s->results, $matches)) {
			$ret["codproduto"] = trim($matches[1]);
		}
		if(preg_match("/name=\"des_bems\" value=\"([0-9A-za-z ]+)\"/m", $this->s->results, $matches)) {
			$ret["desproduto"] = trim($matches[1]);
		}
		if(preg_match("/name=\"unidade_med\" style=\"background-color:#FFFFF7\"  value=\"([0-9A-za-z ]+)\"/m", $this->s->results, $matches)) {
			$ret["unidademedida"] = trim($matches[1]);
		}
		return $ret;
	}
	
	public function insereDadosProposta($codReferencia, $unicod, $tipoDetalhamento, $prgcod, $acacod, $loccod, $qtdLinhasProposta, $dadosProposta, $valorTotal, $qtdFisico, $codProduto, $desProduto, $unidadeMedida, $justificativa) {		
		$urlInsereDados = "https://sidornet.planejamento.gov.br/captacao/ofs/ofsPropostaAction.do";
		$urlReferer = "https://sidornet.planejamento.gov.br/captacao/ofs/ofsAction.do?parametro=ofs_prop&referencia=%s&acao=%s&momento=%s&orgao=%s&unidade=%s&prog=%s&locg=%s";
		$this->s->referer = sprintf($urlReferer, $codReferencia, $acacod, $tipoDetalhamento, '26000', $unicod, $prgcod, $loccod);
		if (!$qtdFisico) $qtdFisico=1;
		$postdata = array(
			"acao"=>$acacod
			,"referencia"=>$codReferencia
			,"momento"=>$tipoDetalhamento
			,"operacao"=>"salvar"
			,"unidade"=>$unicod
			,"cod_bems"=>(string)trim($codProduto)
			,"des_bems"=>(string)trim($desProduto)
			,"unidade_med"=>(string)trim($unidadeMedida)
			,"quantidade"=>$qtdFisico
			,"despMedia"=>number_format($valorTotal / $qtdFisico, "2", ",", ".")
			,"despTotal"=>$valorTotal
			,"totalLinhas"=>$qtdLinhasProposta
		);
		
		$postdata = array_merge($postdata, $dadosProposta);
		$postdata["justificativa"]=$justificativa;

		$this->s->submit($urlInsereDados, $postdata);

		if(strpos($this->s->results, "<DIV class=erro id=div_erro ></DIV><script>Mostra('Operação realizada com sucesso!');</script></DIV>")===false) {
			return new SidorCargaException();
		}
		return true;
	}
}

class SidorPPA extends Sidor {
	public function pegarCodReferenciaSidor($unicod, $prgcod, $acacod, $loccod) {
		$urlReferencia = "https://sidornet.planejamento.gov.br/captacao/paginas/comuns/arvore.jsp?tipo=ppa.qbo.ofs.locg&niv=5&params=INT:2007:;TEXTO:%s:;TEXTO:%s:;TEXTO:%s:;INT:2007:;TEXTO:%s:;TEXTO:%s:;";
		// unidade, progama, acao, unidade, programa
		$url = sprintf($urlReferencia, $unicod, $prgcod, $acacod, $unicod, $prgcod);
		$this->s->fetch($url);
		$codreferencia = array();
		if(preg_match_all("/codRef='([0-9 ]+)'/m", $this->s->results, $matches)) {
			foreach($matches[1] as $m) {
				$codreferencia[] = trim($m);
			}
		}
		else {
			throw new SidorCodReferenciaNaoEncontradoException();			
		}
		if(is_array($codreferencia) && count($codreferencia>0) && preg_match_all("/>(\d{4})<\//m", $this->s->results, $matches)) {
			foreach($matches[1] as $i=>$m) {
				$codreferencia[trim($m)] = $codreferencia[$i];
			}
		}
		else {
			throw new SidorCodReferenciaNaoEncontradoException();
		}
		if(!isset($codreferencia[$loccod]))
			throw new SidorCodReferenciaNaoEncontradoException();		
		return $codreferencia[$loccod];
	}
	
	public function pegarInputs($url, $momento, $codref, $unicod, $acacod) {
		$ret = array();
		$url = sprintf($url, $momento, $codref, $unicod, $acacod);

		$this->s->fetch($url);
		if(!preg_match_all("/<input.*?>/", $this->s->results, $matches)) {
			return array();
		}
		foreach($matches[0] as $input) {
			if(preg_match("/(name=\"(.*?)\"|id=\"(.*?)\")/", $input, $name) && preg_match("/value=\"(.*?)\"/", $input, $value)) {
				$ret[$name[2]] = $value[1];
			}
		}
		return $ret;
	}
	
	public function pegarMsgRetorno() {
		if(preg_match("/<script>Mostra\('(.*?)'\);<\/script>/m", $this->s->results, $matches)) {
			return $matches[1];
		}
		elseif(preg_match("/<script>mostraSemLogo\('(.*?)'\);<\/script>/m", $this->s->results, $matches)) {
			return $matches[1];
		}
		else return false;
	}
	
	public function sucessoOperacao($msg="") {
		$msg = $msg?$msg:$this->pegarMsgRetorno();
		if(strpos($msg, "sucesso")!==false)
			return true;
		else
			return false;
	}
	
	public function enviarDadosFisicos($momento, $unicod, $acacod, $dadosEnvio, $referencia='0') {

		$urlReferer = "https://sidornet.planejamento.gov.br/captacao/ppa/ppaOfsFisAction.do?acao=listar&mom=%s&ref=%s&unid=%s&codAcao=%s";
		$urlEnvio = "https://sidornet.planejamento.gov.br/captacao/ppa/ppaOfsFisAction.do?acao=salvar&mom=%s&unid=%s&codAcao=%s";
	
		$this->s->referer = sprintf($urlReferer, $momento, $referencia, $unicod, $acacod);
		$url = sprintf($urlEnvio, $momento, $unicod, $acacod);
		$this->s->submit($url, $dadosEnvio);
		//dbg($this->s->results);
		//dbg($dadosEnvio);
		$msg = $this->pegarMsgRetorno();
		//print sprintf($urlReferer, $momento, $referencia, $unicod, $acacod).'<br>';
		//print sprintf($urlEnvio, $momento, $unicod, $acacod).'<br>';
		//dbg($msg,1);
		if(!$this->sucessoOperacao($msg)) {
			//dbg($this->s->results);
			throw new SidorCargaException("ERRO ao inserir dados proposta: " . $msg);
		}
		return $msg;
	}
	
	public function enviarDadosFinanceiros($momento, $unidade, $dados) {
		//dbg($dados);
		$urlReferer = "https://sidornet.planejamento.gov.br/captacao/ppa/ppaOfsFinPopupAction.do?acao=detalhar&td=%s&fonte=%s&mom=10&ref=%s&esfe=&codacao=%s&unid=%s&descFonte=Fiscal/Seguridade (Exceto Financiamento Externo)";
		//dbg(sprintf($urlReferer, $dados["codTd"], $dados["strCodFonte"], $dados["codReferencia"], $dados["codAcao"], $unidade),1);
		$this->s->referer = sprintf($urlReferer, $dados["codTd"], $dados["strCodFonte"], $dados["codReferencia"], $dados["codAcao"], $unidade);
		$urlEnvio = "https://sidornet.planejamento.gov.br/captacao/ppa/ppaOfsFinPopupAction.do?acao=salvar&mom=%s&unid=%s";
		$url = sprintf($urlEnvio, $momento, $unidade);
		$tentativas = 0;
		do {
			$this->s->submit($url, $dados);
			$codresp = preg_match("/\d{3}/", $this->s->response_code, $results) ? $results[0] : 0;
			$tentativas++;
			echo "<pre>Tentativa -> $tentativas :: $codresp</pre>";
		} while ($codresp!=200 && $tentativas<5);
		$msg = $this->pegarMsgRetorno();
		if(!$this->sucessoOperacao($msg)) {
			dbg($this->s->results);
			throw new SidorCargaException("ERRO ao inserir dados proposta: " . $msg);
		}
		print $url.'<br>';
		
		return $msg;
	}
	
	public function pegarFontesPPA() {
		$url = "https://sidornet.planejamento.gov.br/captacao/biblioteca/scripts/comuns/popup/XML/fonte_ppa.xml";
		$this->s->fetch($url);
		$xml = simplexml_load_string($this->s->results);
		$saida = array();
		foreach($xml->node as $fonte) {
			$saida[(string)$fonte->codigo] = (string)$fonte->descricao;
		}
		return $saida;
	}
	
	public function pegarCamposDetalhamento($td, $fonte, $momento, $ref, $acacod, $unicod, $descfonte) {
		$ret = array();
		$urlInputs = "https://sidornet.planejamento.gov.br/captacao/ppa/ppaOfsFinPopupAction.do?acao=detalhar&td=%s&fonte=%s&mom=%s&ref=%s&esfe=&codacao=%s&unid=%s&descFonte=%s";
		$url = sprintf($urlInputs, $td, $fonte, $momento, $ref, $acacod, $unicod, $descfonte);
		$this->s->fetch($url);
		if(!preg_match_all("/<input.*?>/", $this->s->results, $matches)) {
			return array();
		}
		foreach($matches[0] as $input) {
			if(preg_match("/name=\"(.*?)\"/", $input, $name) && preg_match("/value=\"(.*?)\"/", $input, $value) && $name[1]) {
				$ret[$name[1]] = $value[1];
			}
		}
		return $ret;
	}
}
?>