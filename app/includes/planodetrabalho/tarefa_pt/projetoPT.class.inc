<?php
	class ProjetoPT extends Projeto
	{
		/**
		 * Nome da tabela do banco onde ficam armazenadas os projetos
		 *
		 */
		public $TABLE_NAME = 'monitora.projetoespecial';
		
		/**
		 * Identificador do projeto
		 */
		public $ROW_NAME_ID 				= 'pjeid';
		
		/**
		 * Codigo do Projet
o		 */
		public $ROW_NAME_STR_CODE			= 'pjecod';
		
		/**
		 * Coluna de texto que contém o nome do projeto
		 */
		public $ROW_NAME_STR_NAME 		= 'pjedsc';
		
		/**
		 * Coluna de data que contém a data de início do projeto
		 */
		public $ROW_NAME_DATE_START 		= 'pjedataini';
		
		/**
		 * Coluna de data que contém a data de término do projeto
		 */
		public $ROW_NAME_DATE_END 		= 'pjedatafim';
		
		/**
		 * Coluna booleana que informa se o projeto possui a data
		 * fechada ou não.
		 * 
		 * Caso a data seja fechada não se pode altera-la por efeito
		 * colateral das tarefas do mesmo, retornando um erro caso
		 * isso seja tentado.
		 *
		 */
		public $ROW_NAME_BOOL_CLOSED_DATE	= 'pjesndatafechada';

		protected $strCodigo = null;
		
		// setters and getters //
		
		public function setCodigo( $strCodigo )
		{
			$this->strCodigo = $strCodigo;
		}
		
		public function getCodigo()
		{
			return $this->strCodigo;
		}
		
		// fim do setters and getters //
		/**
		 * Recebe os dados da tarefa da persistencia e os preenche na
		 * entidade.
		 *
		 */
		protected function emerge()
		{
			$strSql =	' SELECT ' .
							'*' . 
						' FROM ' .
							$this->TABLE_NAME .
						' WHERE ' .
							$this->ROW_NAME_ID . ' = ' . escape( $this->getId() ) .
						' LIMIT 1 ' .
						'';
			$objSaida = self::$objDatabase->carregar( $strSql );
			if( sizeof( $objSaida ) == 1 )
			{
				$objSaida = $objSaida[ 0 ];
							
				$this->setId(					unescape( @$objSaida[ $this->ROW_NAME_ID ]					, 'integer' ) );
				$this->setNome(					unescape( @$objSaida[ $this->ROW_NAME_STR_NAME ] 			, 'string' ) );
				$this->setDataInicio(			unescape( @$objSaida[ $this->ROW_NAME_DATE_START ]			, 'date') );
				$this->setDataFim(				unescape( @$objSaida[ $this->ROW_NAME_DATE_END ]			, 'date') );
				$this->setDataFechada(			unescape( @$objSaida[ $this->ROW_NAME_BOOL_CLOSED_DATE ]	, 'bool') );
				$this->setCodigo(				unescape( @$objSaida[ $this->ROW_NAME_STR_CODE ]			, 'string') );
			}
			else
			{
				throw new Exception( 'Projeto ( ' . $this->getId() . ' ) nao encontrado ') ;
			}
		}
		
		public function getArrTarefasDoProjeto()
		{
			$objTarefa = new TarefaPT();
			if( $this->arrTarefasDoProjeto === null )
			{
				 $this->arrTarefasDoProjeto = orderArrayOfObjectsByMethod(  $objTarefa->getArrTarefasPeloProjeto( $this )  , 'getDataInicioTimestamp' );
			}
			return $this->arrTarefasDoProjeto;
		}
		
		public function getProjetoAberto()
		{
			$strSql = ' SELECT ' .
						'tpscod' .
					' FROM ' .
						'monitora.projetoespecial' .
					' WHERE ' .
						'pjeid' . ' = ' . $this->getId() .
					' LIMIT 1 ' .
					'';
					
			$objSaida = self::$objDatabase->carregar( $strSql );
			if( sizeof( $objSaida ) == 1 )
			{
				$objSaida = $objSaida[ 0 ];
				$strTipo = $objSaida[ 'tpscod' ];
			}
			else
			{
				$strTipo = null;
			}
				
			switch( $strTipo )
			{
				case '10':
				case '11':
				{
					return true;
					break;
				}
				default:
				{
					return false;
					break;
				}
			}
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
		
		protected function insert()
		{
			# 1. Insere o projeto na persistencia						#
			
			// gerando a query //
			
			$strSql =	' INSERT INTO ' . $this->TABLE_NAME .
						' ( ' .
							$this->ROW_NAME_STR_NAME . 
							',' . 
							$this->ROW_NAME_DATE_START . 
							',' . 
							$this->ROW_NAME_DATE_END .
							',' . 
							$this->ROW_NAME_BOOL_CLOSED_DATE .
							',' . 
							$this->ROW_NAME_STR_CODE .
							' ) ' .
					' VALUES ' .
						'( ' .
							escape( $this->getNome() ). 
							',' . 
							escape( $this->getDataInicio() , 'date' ) . 
							',' . 
							escape( $this->getDataFim() , 'date' ) .
							',' . 
							escape( $this->getDataFechada() ) .
							',' . 
							escape( $this->getCodigo() ) .
						' ) ' .
					'';
			
			// executando a query criada //
			
			$objSaida = self::$objDatabase->inserir( $strSql );
			
			# 2. Atualiza o identificador do projeto						#
			$this->setId( (integer) $objSaida );
		}
				
		public function criaTarefa()
		{
			return TarefaPT::criaTarefa();
		}		
	}
?>