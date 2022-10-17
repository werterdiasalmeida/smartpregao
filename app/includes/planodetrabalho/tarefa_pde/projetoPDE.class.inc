<?php
	class ProjetoPDE extends Projeto
	{
		/**
		 * Nome da tabela do banco onde ficam armazenadas os projetos
		 *
		 */
		public $TABLE_NAME = 'pde.atividade';
		
		/**
		 * Identificador da Tabela
		 */
		public $ROW_NAME_ID 				= 'atvid';
		
		/**
		 * Coluna de texto que contém o nome do projeto
		 */
		public $ROW_NAME_STR_NAME 		= 'atvdescricao';
		
		/**
		 * Coluna de data que contém a data de início do projeto
		 */
		public $ROW_NAME_DATE_START 		= 'atvdatainicio';
		
		/**
		 * Coluna de data que contém a data de término do projeto
		 */
		public $ROW_NAME_DATE_END 		= 'atvdatafim';
		
		/**
		 * Coluna booleana que informa se o projeto possui a data
		 * fechada ou não.
		 * 
		 * Caso a data seja fechada não se pode altera-la por efeito
		 * colateral das tarefas do mesmo, retornando um erro caso
		 * isso seja tentado.
		 *
		 */
		protected $ROW_NAME_BOOL_CLOSED_DATE	= '';

		public function getArrTarefasDoProjeto()
		{
			if( $this->arrTarefasDoProjeto === null )
			{
				$objTarefa = new TarefaPDE();
				$this->arrTarefasDoProjeto = orderArrayOfObjectsByMethod(  $objTarefa->getArrTarefasPeloProjeto( $this )  , 'getDataInicioTimestamp' );
			}
			return $this->arrTarefasDoProjeto;
		}
		
		public static function getProjetoPeloId( $intId )
		{
			if( isset( Projeto::$arrInstances[ $intId ] ) )
			{
				$objProjeto = Projeto::$arrInstances[ $intId ];
			}
			else
			{
				$objProjeto = new self();
				$objProjeto->setId( $intId );
				$objProjeto->emerge();
				Projeto::$arrInstances[ $intId ] = $objProjeto;
			}
			return $objProjeto;
		}
		
		public function criaTarefa()
		{
			return TarefaPDE::criaTarefa();
		}
	}
?>