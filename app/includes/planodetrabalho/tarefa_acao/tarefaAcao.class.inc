<?php
	class TarefaAcao extends TarefaPT
	{
		protected $objAcao = null;
		
		protected static $boolUma = null;
		
		public $TABLE_NAME_RELATIONSHIP_ACTION_WORK = 'monitora.plantrabacao';
		/**
		 * Posicao da ordenacao das tarefas filhas
		 * de um mesmo nу
		 */
		public $ROW_NAME_INT_POSITION 			= 'ptoordemprovacao';
		
		/**
		 * Coluna numйrica inteira que contйm um cуdigo ъnico, no escopo
		 * do projeto, de cada tarefa.
		 *
		 */
		public $ROW_NAME_UNIQUE_CODE 			= 'ptoordemacao';
				
		public function setOrigemEspecial( $boolOrigemEspecial )
		{
			$this->boolOrigemEspecial = ( $boolOrigemEspecial == true );
			$this->setSomenteLeitura(
				( $this->getProjetoId() != null ) 
				||
				(
					( $this->getProfundidade() == 1 ) && ( $this->boolOrigemEspecial )
				) 
			);
		}
		
		protected function verificaSeHaConfitoEntreAsTarefasDoProjeto( $intTarefaDataInicio , $intTarefaDataFim )
		{
		}
		
		/**
		 * Retorna as tarefas de um projeto
		 *
		 * @param Project $objProject
		 * @return Tarefa
		 */
		public static function getArrTarefasPelaAcao( Acao $objAcao )
		{
			$objTarefa = $objAcao->criaTarefa();
			$objTarefa->setAcaoId( $objAcao->getId() );
			$arrResult = array();
			$strSql =	' SELECT ' .
							$objTarefa->ROW_NAME_ID . 
						' FROM ' .
							$objTarefa->TABLE_NAME .
						' WHERE ' .
							$objTarefa->ROW_NAME_ID . ' IN ' . 
							' ( ' . 
								' SELECT ' .
									$objTarefa->ROW_NAME_ID . 
								' FROM ' .
									$objTarefa->TABLE_NAME_RELATIONSHIP_ACTION_WORK .
								' WHERE ' . 
									$objTarefa->ROW_NAME_ACTION_ID . 
								' = ' .	
									escape( $objAcao->getId() ) .
							' ) ' .							
						' AND ' .
							$objTarefa->ROW_NAME_CONTAINER_ID . ' IS ' . escape( null ) .
						' AND ' .
							$objTarefa->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $objTarefa->TYPE_STATUS_ACTIVE ) .
						'';
			if( self::$objDatabase == null )
			{
				global $db;
				self::$objDatabase = $db;
			}
			
			$objSaida = self::$objDatabase->carregar( $strSql );
			
			if( $objSaida !== FALSE )
			{
				foreach( $objSaida as $objLinha )			
				{
					$objTarefa = $objTarefa->getTarefaPeloId( $objLinha[ $objTarefa->ROW_NAME_ID ] );
					$arrResult[] = $objTarefa;
				}
			}
			return $arrResult;
		}

		/**
		 * Retorna a Acao desta tarefa
		 */
		public function getAcao()
		{
			if( ( $this->objAcao === NULL ) && ( $this->intAcaoId != NULL ) )
			{
				$this->objAcao = Acao::getAcaoPeloId( $this->intAcaoId ); 
			}
			return $this->objAcao;
		}

		public function getElementoNome()
		{
			return $this->getAcao()->getElementoNome();
		}
		
		public function getElementoNomePlural()
		{
			return $this->getAcao()->getElementoNomePlural();
		}

		protected function getPerfilUMA()
		{
			if( self::$boolUma == null )
			{
				// o controle atual de perfis permite que o super usuario tenha todos os perfis o que		//
				// provoca problemas no processo de filtros para um controle espeficico por isto o filtro	//
				// para usuarios que sao UMAs й feito para todos mas antes deve se checar se o usuario й	//
				// um super usuario ou nao																	//
				
				 self::$boolUma = ( !self::$objDatabase->testa_superuser() & self::$objDatabase->testa_uma() );
			}
			return  self::$boolUma;
		}
		
		/**
		 * 
		 * Nesta extensao de tarefa, as tarefas que sao somente leitura sao:
		 * 1. Aquelas que naturalmente seriam somente leitura pelo processo de controle nativo extendido
		 * 2. Todas as tarefas caso o perfil do usuario seja UMA	
		 * @return boolean
		 */
		public function getSomenteLeitura()
		{
			return parent::getSomenteLeitura() || $this->getPerfilUMA() ;
		}

		public function getInstanceById( $intId )
		{
			if( isset( self::$arrInstances[ get_class( $this ) ][ $intId ] ) )
			{
				return self::$arrInstances[ get_class( $this ) ][ $intId ];
			}
			else
			{
				$strClassName = get_class( $this );
				$objElement = new $strClassName();
				$objElement->setId( $intId );
				$objElement->setAcaoId( $this->getAcaoId() );
				$objElement->emerge();
				return $objElement;
			}
		}
			
		/**
		 * Recebe os dados da tarefa da persistencia e os preenche na
		 * entidade.
		 *
		 */
		public function emerge()
		{
			$strSql =	' SELECT ' .
							'*' . 
						' FROM ' .
							$this->TABLE_NAME .
						' WHERE ' .
							$this->ROW_NAME_ID . ' = ' . escape( $this->getId() ) .
						' AND ' .
							$this->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $this->TYPE_STATUS_ACTIVE ) .
						' LIMIT 1 ' .
						'';
			
			$objSaida = self::$objDatabase->carregar( $strSql );
			
			if( is_array( $objSaida ) && sizeof( $objSaida ) == 1 )
			{
				$objSaida = $objSaida[ 0 ];

				$this->setId(						unescape( @$objSaida[ $this->ROW_NAME_ID ]						, 'integer' ) );
				$this->setPredecessoraId(			unescape( @$objSaida[ $this->ROW_NAME_PREDECESSOR_ID ]			, 'integer' ) );
				$this->setContainerId(				unescape( @$objSaida[ $this->ROW_NAME_CONTAINER_ID ]			, 'integer' ) );
				$this->setProjetoId(				unescape( @$objSaida[ $this->ROW_NAME_PROJECT_ID ]				, 'integer' ) );
				$this->setNome(						unescape( @$objSaida[ $this->ROW_NAME_STR_NAME ] 				, 'string' ) );
				$this->setDescricao(				unescape( @$objSaida[ $this->ROW_NAME_STR_DESC ] 				, 'string' ) );
				$this->setCodigoUnico(				unescape( @$objSaida[ $this->ROW_NAME_UNIQUE_CODE ]				, 'integer' ) );
				$this->setDataInicio(				unescape( @$objSaida[ $this->ROW_NAME_DATE_START ]				, 'date') );
				$this->setDataFim(					unescape( @$objSaida[ $this->ROW_NAME_DATE_END ]				, 'date') );
				$this->setDataFechada(				unescape( @$objSaida[ $this->ROW_NAME_BOOL_CLOSED_DATE ]		, 'bool') );
				$this->setPosicao(					unescape( @$objSaida[ $this->ROW_NAME_INT_POSITION ]			, 'integer') );
				$this->setDonoId(					unescape( @$objSaida[ $this->ROW_NAME_OWNER_ID ]				, 'integer') );
				$this->setQtdDiasAntesParaAviso(	unescape( @$objSaida[ $this->ROW_NAME_INT_ALERT_MISSING_DAYS ]	, 'integer') );
				$this->setPrevisaoMeta(				unescape( @$objSaida[ $this->ROW_NAME_DBL_AIM ]					, 'double') );
				$this->setProdutoId(				unescape( @$objSaida[ $this->ROW_NAME_PRODUCT_ID ]				, 'integer') );
//				$this->setAcaoId(					unescape( @$objSaida[ $this->ROW_NAME_ACTION_ID ]				, 'integer') );
				$this->setAcaoId( 					unescape( $_SESSION['acaid']    								, 'integer') );
				$this->setUnidadeDeMedidaId(		unescape( @$objSaida[ $this->ROW_NAME_MEAUSE_UNIT_ID ]			, 'integer') );
				$this->setProfundidade(				unescape( @$objSaida[ $this->ROW_NAME_INT_DEEPER ]				, 'integer') );
				$this->setOrigemEspecial(			unescape( @$objSaida[ $this->ROW_NAME_BOOL_SPECIAL_ORIGIN ]		, 'bool') );
				$this->setSubAcao(					unescape( @$objSaida[ $this->ROW_NAME_BOOL_IS_SUB_ACTION ]		, 'bool') );
				
				$this->emergeQtdFilhas();
			}
			else
			{
				throw new Exception( 'Tarefa ( ' . $this->getId() . ' ) nгo existe' ) ;
			}
		}
		
		protected function emergeQtdFilhas()
		{
			$strSql =	' SELECT ' .
							'COUNT( * ) AS qtd_filhas ' . 
						' FROM ' .
							$this->TABLE_NAME .
						' WHERE ' .
							$this->ROW_NAME_CONTAINER_ID . ' = ' . escape( $this->getId() ) .
						' AND ' .
							$this->ROW_NAME_ID . ' IN ' . 
							' ( ' . 
								' SELECT ' .
									$this->ROW_NAME_ID . 
								' FROM ' .
									$this->TABLE_NAME_RELATIONSHIP_ACTION_WORK .
								' WHERE ' . 
									$this->ROW_NAME_ACTION_ID . 
								' = ' .	
									escape( $this->getAcaoId() ) .
							' ) ' .
						' AND ' .
							$this->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $this->TYPE_STATUS_ACTIVE ) .
						'';
			
			$objSaida = self::$objDatabase->carregar( $strSql );
			if( is_array( $objSaida ) && sizeof( $objSaida ) == 1 )
			{
				$objSaida = $objSaida[ 0 ];
			}
			$this->intQtdTarefasFilhas = (integer) @$objSaida[ 'qtd_filhas' ];
		}
		
		// fim dos setters and getters //
		public function appendChild()
		{
			$objNovaTarefa = $this->criaTarefa();
			$objNovaTarefa->setDataInicio( $this->getDataInicio() );
			$objNovaTarefa->setNovaDataInicio( $this->getDataInicio() );
			$objNovaTarefa->setDataFim( $this->getDataFim() );
			$objNovaTarefa->setNovaDataFim( $this->getDataFim() );
			$objNovaTarefa->setNome( 'Nova ' . $this->getElementoNome() );
			$objNovaTarefa->setDescricao( 'Nova Descriзгo' );
			$objNovaTarefa->setProjetoId( $this->getProjetoId() );
			$objNovaTarefa->setContainerId( $this->getId() );
			$objNovaTarefa->setProfundidade( $this->getProfundidade() + 1 );
			$objNovaTarefa->setOrigemEspecial( $this->getOrigemEspecial() );
			$objNovaTarefa->setAcaoId( $this->getAcaoId() );
			$objNovaTarefa->inserirTarefa();			
		}
		
		public function append( $intAcaoId )
		{
//			$this->setAcaoId( $intAcaoId );
			$this->setAcaoId( $_SESSION['acaid'] );
			$this->setNome( 'Nova ' . $this->getElementoNome() );
			$this->setDescricao( 'Nova Descriзгo' );
			$this->setDataInicio( $this->getAcao()->getDataInicio() );
			$this->setDataFim( $this->getAcao()->getDataFim() );
			$this->setProfundidade( 1 );
			$this->inserirTarefa();
		}
		
		public function removerTarefa()
		{
			$this->setAcaoId( $_SESSION['acaid'] );
			parent::removerTarefa();
		}
		/**
		 * Cria uma nova instancia de tarefaAcao
		 */
		public function criaTarefa()
		{
			return new TarefaAcao();
		}		

		/**
		 * Metodo que reintegra os dados das tarefas de um projeto
		 */
		public static function reparaTarefas( Acao $objAcao )
		{
			$objTarefa = $objAcao->criaTarefa();;
			$arrTarefas = $objTarefa->getArrTarefasPelaAcao( $objAcao );
			$intCodigoUnicoAtual = 1;
			$intProfundidade = 1;
			$intCodigoUnicoAtual = self::reparaRecursivamente( $arrTarefas , $intCodigoUnicoAtual , $intProfundidade );
		}
				
				
		public function getPredecessora()
		{
			if( ( $this->objPredecessora === NULL ) && ( $this->intPredecessoraId != NULL ) )
			{
				$this->objPredecessora = $this->getTarefaPeloId( $this->intPredecessoraId );
				$this->intPredecessoraId = $this->objPredecessora->getId();
				$this->intPredecessoraCodigoUnico = $this->objPredecessora->getCodigoUnico();
			}
			if( ( $this->objPredecessora === NULL ) && ( $this->intPredecessoraCodigoUnico != NULL ) )
			{
				$this->objPredecessora = $this->getTarefaPeloCodigoUnico( $this->intPredecessoraCodigoUnico , $this->getAcao() );
				$this->intPredecessoraCodigoUnico = $this->objPredecessora->getCodigoUnico();
				$this->intPredecessoraId = $this->objPredecessora->getId();
			}
			return $this->objPredecessora;
		}
		
		public function getTarefaPeloCodigoUnico( $intTarefaCodigo , Acao $objAcao )
		{
			$arrResult = array();
			
			/*
			$strSql =	' SELECT ' .
							$this->ROW_NAME_ID . 
						' FROM ' .
							$this->TABLE_NAME .
						' WHERE ' .
							$this->ROW_NAME_UNIQUE_CODE . ' = ' . escape( $intTarefaCodigo ) .
						' AND ' .
							$this->ROW_NAME_ACTION_ID . ' = ' . escape( $objAcao->getId() ) .
						' AND ' .
							$this->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $this->TYPE_STATUS_ACTIVE ) .
						'';
			*/
			
//			select * from monitora.planotrabalho  
//where $ROW_NAME_ID in (select $ROW_NAME_ID from monitora.plantrabacao where acaid=326)
			

			$strSql = ' SELECT ' . 
							$this->ROW_NAME_ID .
						' FROM ' .
							$this->TABLE_NAME .
						' WHERE ' .
							$this->ROW_NAME_UNIQUE_CODE . ' = ' . escape( $intTarefaCodigo ) .
						' AND ' .
							$this->ROW_NAME_ID . ' IN ' . 
							' ( ' . 
								' SELECT ' .
									$this->ROW_NAME_ID . 
								' FROM ' .
									$this->TABLE_NAME_RELATIONSHIP_ACTION_WORK .
								' WHERE ' . 
									$this->ROW_NAME_ACTION_ID . 
								' = ' .	
									escape( $objAcao->getId() ) .
							' ) ' .
						' AND ' .
							$this->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $this->TYPE_STATUS_ACTIVE ) .
						'';
							
			
			$objSaida = self::$objDatabase->carregar( $strSql );
			
			if( $objSaida !== FALSE )
			{
				foreach( $objSaida as $objLinha )
				{
					$objTarefaPredecessora = $this->getTarefaPeloId( $objLinha[ $this->ROW_NAME_ID ] );
					$arrResult[] = $objTarefaPredecessora;
				}
				if( sizeof( $arrResult ) > 1 )
				{
					throw new Exception( 'Mais de uma Tarefa por codigo unico ( ' . $intTarefaCodigo . ' ) no projeto ( ' . $objAcao->getId() . ' ) ') ;	
				}
				return $arrResult[ 0 ];
			}
			else
			{
				return NULL;
			}
		}

		/**
		 * Retorna o array de sucessoras diretas desta tarefa
		 * 
		 * @return Array of Tarefa
		 */
		protected function getArraydeTarefasSeguintes()
		{
			if ( !$this->ROW_NAME_PREDECESSOR_ID ) {
				# adaptaзгo necessбria para tarefas que nгo tem controle de precedкncia
				return array();
			}
			if( $this->arrTarefasSeguintes === null )
			{
				$arrResult = array();
				
				$strSql =	' SELECT ' .
								$this->ROW_NAME_ID . 
							' FROM ' .
								$this->TABLE_NAME .
							' WHERE ' .
								$this->ROW_NAME_UNIQUE_CODE . ' > ' . escape( $this->getCodigoUnico() ) .
							' AND ' .
								$this->ROW_NAME_ID . ' IN ' . 
								' ( ' . 
									' SELECT ' .
										$this->ROW_NAME_ID . 
									' FROM ' .
										$this->TABLE_NAME_RELATIONSHIP_ACTION_WORK .
									' WHERE ' . 
										$this->ROW_NAME_ACTION_ID . 
									' = ' .	
										escape( $this->getAcao()->getId() ) .
								' ) ' .
							' AND ' .
								$this->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $this->TYPE_STATUS_ACTIVE ) .
							'';
								
				$objSaida = self::$objDatabase->carregar( $strSql );
				if( $objSaida !== FALSE )
				{
					foreach( $objSaida as $objLinha )
					{
						$objTarefaSeguinte = $this->getTarefaPeloId( $objLinha[ $this->ROW_NAME_ID ] );
						$arrResult[] = $objTarefaSeguinte;
					}
					$this->arrTarefasSeguintes = $arrResult;
				}
				else
				{
					$this->arrTarefasSeguintes  = array();
				}
			}
			return $this->arrTarefasSeguintes;
		}

			
		protected function getArraydeTarefasIrmas()
		{
			$objContainer = $this->getContainer();
			if( $objContainer !== NULL )
			{
				if( !$objContainer->getRemoved() )
				{
					$arrIrmas = $objContainer->getArraydeTarefasqueContenho();
				}
			}
			else
			{
				$arrIrmas = $this->getArrTarefasPelaAcao( $this->getAcao() );
			}
			$arrIrmas = orderArrayOfObjectsByMethod( $arrIrmas , 'getPosicao' );
			return $arrIrmas;
		}
		
		/**
		 * Retorna o array das tarefas que seja contidas diretamente por esta tarefa.
		 * 
		 * @return Array of Tarefa
		 */
		public function getArraydeTarefasqueContenho()
		{
			if( $this->arrTarefasQueContenho === null )
			{
				$arrResult = array();
				
				$strSql =	' SELECT ' .
								$this->ROW_NAME_ID . 
							' FROM ' .
								$this->TABLE_NAME .
							' WHERE ' .
								$this->ROW_NAME_CONTAINER_ID . ' = ' . escape( $this->getId() ) .
							' AND ' .
								$this->ROW_NAME_ID . ' IN ' . 
								' ( ' . 
									' SELECT ' .
										$this->ROW_NAME_ID . 
									' FROM ' .
										$this->TABLE_NAME_RELATIONSHIP_ACTION_WORK .
									' WHERE ' . 
										$this->ROW_NAME_ACTION_ID . 
									' = ' .	
										escape( $this->getAcao()->getId() ) .
								' ) ' .
							' AND ' .
								$this->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $this->TYPE_STATUS_ACTIVE ) .
							'';
				$objSaida = self::$objDatabase->carregar( $strSql );
				if( $objSaida !== FALSE )
				{
					foreach( $objSaida as $objLinha )
					{
						$objTarefaQueContenho = $this->getTarefaPeloId( $objLinha[ $this->ROW_NAME_ID ] );
						$arrResult[] = $objTarefaQueContenho;
					}
				}
				$this->arrTarefasQueContenho = $arrResult;
			}
			return $this->arrTarefasQueContenho;
		}
		
			
		/**
		 * Metodo que de fato insere o elemento na persistencia
		 */
		public function insert()
		{
			# 1. Valida a tarefa										#

			try
			{
				//
				$this->valida();
			}
			catch( Exception $ErrorObject )
			{
				throw $ErrorObject;
			}
			
			# 2. Insere a tarefa na persistencia						#
			
			// gerando a query //
			$strSql =	' INSERT INTO ' . $this->TABLE_NAME .
						' ( ' .
							$this->ROW_NAME_PREDECESSOR_ID .
							',' . 
							$this->ROW_NAME_CONTAINER_ID . 
							',' . 
							$this->ROW_NAME_PROJECT_ID .
							',' .
							$this->ROW_NAME_STR_NAME . 
							',' . 
							$this->ROW_NAME_STR_DESC . 
							',' . 
							$this->ROW_NAME_UNIQUE_CODE .
							',' . 
							$this->ROW_NAME_DATE_START . 
							',' . 
							$this->ROW_NAME_DATE_END .
							',' . 
							$this->ROW_NAME_BOOL_CLOSED_DATE .
							',' .
							$this->ROW_NAME_OWNER_ID . 
							',' .
							$this->ROW_NAME_INT_ALERT_MISSING_DAYS . 
							',' .
							 $this->ROW_NAME_DBL_AIM .
							 ',' .
							 $this->ROW_NAME_PRODUCT_ID .
							 ',' .
							 $this->ROW_NAME_ACTION_ID .
							 ',' .
							 $this->ROW_NAME_MEAUSE_UNIT_ID .
							 ',' .
							 $this->ROW_NAME_INT_POSITION .
							 ',' .
							 $this->ROW_NAME_BOOL_SPECIAL_ORIGIN .
							 ',' .
							 $this->ROW_NAME_INT_DEEPER .
							 ',' .
							 $this->ROW_NAME_BOOL_IS_SUB_ACTION .
							 ' ) ' .
					' VALUES ' .
						'( ' .
							escape( $this->getPredecessoraId() ) .
							',' . 
							escape( $this->getContainerId() ) . 
							',' . 
							escape( $this->getProjetoId() ) . 
							',' . 
							escape( $this->getNome() ). 
							',' . 
							escape( $this->getDescricao() ) . 
							',' . 
							escape( $this->getCodigoUnico() ) .
							',' . 
							escape( $this->getDataInicio() , 'date' ) . 
							',' . 
							escape( $this->getDataFim() , 'date' ) .
							',' . 
							escape( $this->getDataFechada() ) .
							',' . 
							escape( $this->getDonoId() ) .
							',' . 
							escape( $this->getQtdDiasAntesParaAviso() ) .
							',' .
							escape( $this->getPrevisaoMeta() ) .
							',' .
							escape( $this->getProdutoId() ) .
							',' .
							escape( $this->getAcaoId() ) .
							',' .
							escape( $this->getUnidadeDeMedidaId() ) .
							',' .
							escape( $this->getPosicao() ) .
							',' .
							escape( $this->getOrigemEspecial() ) .
							',' .
							escape( $this->getProfundidade() ) .
							',' .
							escape( $this->getSubAcao() ) .
						' ) ' .
//					' RETURNING ' . $this->ROW_NAME_ID .
					'';
					
			// executando a query criada //

			$objSaida = self::$objDatabase->executar( $strSql );	

			// caso o returning esteja habilitado //
//			$intNovoId = (integer) $objSaida;

			$intOid = (integer) pg_last_oid( $objSaida );

			$strSql =	' SELECT ' . 
							'ptoid' . 
						' FROM ' . 
							'monitora.planotrabalho' . 
						' WHERE '.
							'oid' . ' = ' . $intOid .
					'';

			$intNovoId = self::$objDatabase->pegaUm( $strSql );

			# 3. Atualiza o identificador da tarefa						#
			$this->setId( $intNovoId );

			// gerando a query //
			$strSql =	' INSERT INTO ' . $this->TABLE_NAME_RELATIONSHIP_ACTION_WORK .
						' ( ' .
							$this->ROW_NAME_ID .
							',' . 
							$this->ROW_NAME_ACTION_ID . 
						' ) ' .
						' VALUES ' .
						'( ' .
							escape( $this->getId() ) .
							',' . 
							escape( $this->getAcaoId() ) .
						' ) ' .
						'';
							
			$objSaida = self::$objDatabase->executar( $strSql );
			$this->setInserted( true );
		}		
	}
?>