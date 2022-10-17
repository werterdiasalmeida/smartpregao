<?php
	
class PaginacaoAjax extends Modelo
{
	public $sql;
	
	public $arCabecalho;
	
	public $acao;
	
	public $arCorpo;
	
	public $arParamCol;
	
	public $arConfig;
	
	public $nrPaginaAtual = 1;
	
	public $nrRegPorPagina = 10;
	
	public $nrBlocoPaginacaoMaximo = 10;
	
	public $nrBlocoAtual = 1;
	
	public $nmDiv;
	
	public $nmControleMetodo;
	
	public $nrInicio;
	
	public $arConfigPaginacao;
	
   	public function __construct()
	{
		parent::__construct();
		self::setNmControleMetodo(self::get_caller_controller_method());
	}
	
	public function setNrPaginaAtual($nrPaginaAtual)
	{
		if($nrPaginaAtual){
			$this->nrPaginaAtual = $nrPaginaAtual;
		}
	}
	
	public function setNrRegPorPagina($nrRegPorPagina)
	{
		if($nrRegPorPagina){
			$this->nrRegPorPagina = $nrRegPorPagina;
		}
	}
	
	public function setNrBlocoPaginacaoMaximo($nrBlocoPaginacaoMaximo)
	{
		if($nrBlocoPaginacaoMaximo){
			$this->nrBlocoPaginacaoMaximo = $nrBlocoPaginacaoMaximo;
		}
	}
	
	public function setNrBlocoAtual($nrBlocoAtual)
	{
		if($nrBlocoAtual){
			$this->nrBlocoAtual = $nrBlocoAtual;
		}
	}
	
	public function setDiv($nmDiv)
	{
		$this->nmDiv = $nmDiv;
	}
	
	public function setNmControleMetodo($nmControleMetodo)
	{
		$this->nmControleMetodo = $nmControleMetodo;
	}
	
	public function setCabecalho(Array $arCabecalho = null)
	{
		$this->arCabecalho = $arCabecalho;
	}
	
	public function setAcao( $acao )
	{
		$this->acao = $acao;
	}
	
	public function setSql( $sql )
	{
		$this->sql = $sql;
	}
	
	public function setParamCol( $arParamCol )
	{
		$this->arParamCol = $arParamCol;
	}
	
	public function setConfig( $arConfig )
	{
		$this->arConfig = $arConfig;
	}
	
	private function getFiltros()
	{
		return json_encode($_POST);
	}
	
	public function show()
	{
		$this->arConfigPaginacao['nrPaginaAtual'] 		   = $this->nrPaginaAtual;
		$this->arConfigPaginacao['nrRegPorPagina'] 		   = $this->nrRegPorPagina;
		$this->arConfigPaginacao['nrBlocoPaginacaoMaximo'] = $this->nrBlocoPaginacaoMaximo;
		$this->arConfigPaginacao['nrBlocoAtual'] 		   = $this->nrBlocoAtual;
		$this->arConfigPaginacao['nmControleMetodo'] 	   = $this->nmControleMetodo;
		
		$nrInicio = $this->nrPaginaAtual-1;
		$nrInicio = $nrInicio*$this->nrRegPorPagina;
		$this->nrInicio = $nrInicio;
		
		$arDados = self::paginar();
		
		$arDefinicaoPaginacao['boPaginacao'] = true;
		$arDefinicaoPaginacao['nmDiv']       = $this->nmDiv;
		$arDefinicaoPaginacao['filtros']     = self::getFiltros();
		
		$oLista = new Lista($this->arConfig, $arDefinicaoPaginacao);
		$oLista->setCabecalho( $this->arCabecalho );
		$oLista->setCorpo( $arDados, $this->arParamCol );
		$oLista->setAcao( $this->acao );
		$oLista->show($this->arConfigPaginacao);
	}
	
	private function paginar()
	{
		$sql = trim($this->sql);

		$sqlCount = "select
						count(1)
					 from (" . $sql . ") rs";
		$nrTotalRegistro = $this->pegaUm($sqlCount);
		$sql = $sql . " LIMIT {$this->arConfigPaginacao['nrRegPorPagina']} offset ".($this->nrInicio);
		
		$this->arConfigPaginacao['nrTotalRegistro'] = $nrTotalRegistro;
		return $this->carregar($sql);
	}
	
	private function get_caller_controller_method()
	{
	    $traces = debug_backtrace();
	    if (isset($traces[2]))
	    {
	        return $traces[2]['class'].$traces[2]['type'].$traces[2]['function'];
	    }
	
	    return null;
	}
}