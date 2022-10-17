<?php

class Atividade extends Tarefa {

	public $TABLE_NAME                      = 'pde.atividade';
	public $ROW_NAME_ID                     = 'atiid';
	public $ROW_NAME_CONTAINER_ID           = 'atiidpai';
	public $ROW_NAME_PROJECT_ID             = 'atiid';
	public $ROW_NAME_STR_NAME               = 'atidescricao';
	public $ROW_NAME_STR_DESC               = 'atidetalhamento';
	public $ROW_NAME_DATE_START             = 'atidatainicio';
	public $ROW_NAME_DATE_END               = 'atidatafim';
	public $ROW_NAME_BOOL_ACTIVE_STATUS	    = 'atistatus';
	public $ROW_NAME_INT_POSITION           = 'atiordem';
	public $ROW_NAME_UNIQUE_CODE            = 'atinumeracao';
	public $ROW_NAME_OWNER_ID               = 'usucpf';
	public $ROW_NAME_PREDECESSOR_ID         = 'atiidpredecessora';
	public $ROW_NAME_BOOL_CLOSED_DATE       = 'atisndatafixa';
	
	public function criaTarefa() {
		return new Atividade();
	}
	
//	public function excluir(){
//		# exclui o registro
//		$sql = sprintf(
//			"update %s set %s = 'I' where %s = %s",
//			$this->TABLE_NAME,
//			$this->ROW_NAME_BOOL_ACTIVE_STATUS,
//			$this->ROW_NAME_ID,
//			$this->getId()
//		);
//		if ( !self::$objDatabase->executar( $sql ) ) {
//			return false;
//		}
//		# exclui os filhos
//		foreach( $this->getArraydeTarefasqueContenho() as $tarefa ) {
//			if ( !$tarefa->excluir() ) {
//				return false;
//			}
//		}
//		return true;
//	}
	
	/**
	 * Metodo que reintegra os dados das tarefas de um projeto
	 */
	public static function reparaTarefas()
	{
		$atividades = array();
		$sql =  sprintf(
			"select %s from %s where %s is null and %s = %s",
			$this->ROW_NAME_ID,
			$this->TABLE_NAME,
			$this->ROW_NAME_CONTAINER_ID,
			$this->ROW_NAME_BOOL_ACTIVE_STATUS,
			escape( $this->TYPE_STATUS_ACTIVE )
		);
		foreach ( self::$objDatabase->carregar( $sql ) as $registro ) {
			array_push( $atividades, $origem->getTarefaPeloId( $registro['atiid'] ) );
		}
		$arrTarefas = orderArrayOfObjectsByMethod( $atividades , 'getDataInicioTimestamp' );
		$intCodigoUnicoAtual = 1;
		$intProfundidade = 1;
		$intCodigoUnicoAtual = self::reparaRecursivamente( $arrTarefas , $intCodigoUnicoAtual , $intProfundidade );
	}
	
	/**
	 * Metodo recursivo para a reintegracao dos dados das tarefs
	 * @see reparaTarefas
	 */
	public static function reparaRecursivamente( $arrTarefasReparando , $intCodigoUnicoAtual , $intProfundidade )
	{
		$intPosicaoNoNivel = 1;
		$arrTarefasReparando = orderArrayOfObjectsByMethod( $arrTarefasReparando  , 'getPosicao' );
		foreach( $arrTarefasReparando as $objTarefaReparando )
		{
			$objTarefaReparando->setCodigoUnico( $intCodigoUnicoAtual );
			$objTarefaReparando->setPosicao( $intPosicaoNoNivel );
			$objTarefaReparando->setProfundidade( $intProfundidade );
			++$intCodigoUnicoAtual;
			++$intPosicaoNoNivel;
			
			$arrFilhas = $objTarefaReparando->getArraydeTarefasqueContenho();
			$intCodigoUnicoAtual = self::reparaRecursivamente( $arrFilhas , $intCodigoUnicoAtual , $intProfundidade + 1 );
			$objTarefaReparando->setChanged( true );
		}
		return $intCodigoUnicoAtual;
	}
	
	protected function getArraydeTarefasSeguintes() {
		return array();
	}

	public function getProjeto(){
		return null;
	}
	
	public function insert() {
		# 1. Valida a atividade
		$this->valida();
		# 2. Insere a atividade na persistencia
		$sql = sprintf(
			"insert into %s ( %s, %s, %s, %s, %s, %s, %s, %s, %s, %s ) values ( %s, %s, %s, %s, %s, %s, %s, %s, %s, %s )",
				$this->TABLE_NAME,
				$this->ROW_NAME_PREDECESSOR_ID,
				$this->ROW_NAME_CONTAINER_ID,
				$this->ROW_NAME_STR_NAME,
				$this->ROW_NAME_STR_DESC,
				$this->ROW_NAME_UNIQUE_CODE,
				$this->ROW_NAME_DATE_START,
				$this->ROW_NAME_DATE_END,
				$this->ROW_NAME_BOOL_CLOSED_DATE,
				$this->ROW_NAME_OWNER_ID,
				$this->ROW_NAME_INT_POSITION,
				escape( $this->getPredecessoraId() ),
				escape( $this->getContainerId() ), 
				escape( $this->getNome() ),
				escape( $this->getDescricao() ), 
				escape( $this->getCodigoUnico() ),
				escape( $this->getDataInicio() , 'date' ), 
				escape( $this->getDataFim() , 'date' ),
				escape( $this->getDataFechada() ),
				escape( $this->getDonoId() ),
				escape( $this->getPosicao() )
		);
		$identificador = self::$objDatabase->executar( $sql );
		$this->setId( (integer) $identificador ); # Atualiza o identificador da tarefa
		$this->setInserted( true );
	}
	
	public function __call( $metodo, $parametros ){
	}

}

?>