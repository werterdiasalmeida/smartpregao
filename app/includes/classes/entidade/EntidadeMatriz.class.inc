<?php
abstract class EntidadeMatriz
{
	const TIPOPESSOA = 'pessoa';
	const TIPOORGAO = 'orgao';
	private $arqView;
	protected $entidade;
	protected $mantemSessao;
//	protected $view;
	
	public function __construct($entid=null, $type=self::TIPOPESSOA, $mantemSessao=true)
	{
		$entid = (empty($entid) && $mantemSessao) ? $_SESSION['agenda'][$type]['entid'] : $entid;
		if (!empty($entid)){
			$entidade = new Entidade();
			$this->entidade = $entidade->recuperarDados($entid, array("e.entstatus = 'A'"));
		}else{
			$this->entidade = array();
		}
		$this->mantemSessao = $mantemSessao;
//		$this->view = array();
//		dbg($this->entidade);
	}
	
	public function setView($view)
	{
		$this->arqView = $view;
		
		return $this;
	}
	
	public function getView()
	{
		return $this->arqView;
	}
	
	protected function renderView($view)
	{
		if (!empty($view)){
			self::setView($view);
		}else{
			$view = self::getView();
		}
		$caminho = APPRAIZ . "includes/classes/entidade/view/";
		$arquivo = $caminho . self::addExtensao($view);
		
		if (file_exists($arquivo)){
//			extract($this->view);
			require_once $arquivo;
		}else{
			die("O arquivo de view ({$view}) da classe de ENTIDADE não foi encontrado!");
		}
	}
	
	protected function extract($dados)
	{
		if ($dados instanceof Modelo){
			$dados = (array) $dados->getDados();
		}
		
		foreach ($dados as $k => $v){
			$this->$k = $v;
//			$this->view[$k] = $v;
		}
	}
	
	private function addExtensao($file)
	{
		return $file . ".inc";
	}
}