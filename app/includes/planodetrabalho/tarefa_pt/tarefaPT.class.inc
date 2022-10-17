<?php
	class TarefaPT extends Tarefa
	{
		// nomes das tabelas e colunas da persisntecia utilizadas na classe //
		
		/**
		 * Nome da tabela do banco onde ficam armazenadas as tarefas
		 *
		 */
		public $TABLE_NAME = 'monitora.planotrabalho';
		
		/**
		 * Identificador da Tabela
		 */
		public $ROW_NAME_ID 					= 'ptoid';
		
		/**
		 * @var integer
		 * Campo para a chave do id do dono da tarefa
		 */		
		public $ROW_NAME_OWNER_ID 				= 'usucpf';
		
		/**
		 * Chave estrageira do id da tarefa predecessora direta
		 * 
		 * A predecessora direta й a tarefa que esta depende estar concluida
		 * para ser iniciada. Cada tarefa sу tem uma predecessora direta mas
		 * vбrias tarefas podem ser predecessidas diretamente pela mesma 
		 * tarefa.
		 */
		public $ROW_NAME_PREDECESSOR_ID 		= 'ptoid_antecessor';
		
		/**
		 * Chave estrangeira do id da tarefa que contйm esta tarefa
		 * 
		 * As tarefas podem conter tarefas. As tarefas que nгo sгo contidas
		 * por nenhum outra tarefa possuem este campo nulo enquanto as que
		 * pertencem a uma tarefa possuem o id da tarefa que a contem neste
		 * campo.
		 *
		 */
		public $ROW_NAME_CONTAINER_ID 			= 'ptoid_pai';
		
		/**
		 * Campo booleano de controle se o elemento й subacao
		 */
		public $ROW_NAME_BOOL_IS_SUB_ACTION			= 'ptosnsubacao';
		
		//ptosnsubacao
		/**
		 * Chave estrangeira do id do projeto de que estas tarefas pertencem.
		 * 
		 * As tarefas pertecem a um projeto entгo todas as tarefas devem
		 * ter o id do projeto a quem pertencem. As tarefas nгo devem exceder
		 * os limites das datas do projeto. Caso alguma tente fazer isto й 
		 * checado se o projeto possui flexibilidade de data, se isto for 
		 * verdade o mesmo й alterado para a tarefa permanecer dentro dos 
		 * seus limites, se nгo й retornado um erro.
		 *
		 */
		public $ROW_NAME_PROJECT_ID 			= 'pjeid';
		
		/**
		 * Coluna de texto que contйm o nome da tarefa
		 */
		public $ROW_NAME_STR_NAME 				= 'ptodsc';
		
		/**
		 * Coluna de texto que contйm a descriзгo da tarefa
		 *
		 */
		public $ROW_NAME_STR_DESC 				= 'ptodescricao';
		
		/**
		 * Coluna numйrica inteira que contйm um cуdigo ъnico, no escopo
		 * do projeto, de cada tarefa.
		 *
		 */
		public $ROW_NAME_UNIQUE_CODE 			= 'ptoordem';
		
		/**
		 * Coluna de data que contйm a data de inнcio da tarefa
		 */
		public $ROW_NAME_DATE_START 			= 'ptodata_ini';
		
		/**
		 * Coluna de data que contйm a data de tйrmino da tarefa
		 */
		public $ROW_NAME_DATE_END 				= 'ptodata_fim';
		
		/**
		 * Coluna booleana que informa se a tarefa possui a data
		 * fechada ou nгo.
		 * 
		 * Caso a data seja fechada nгo se pode altera-la por efeito
		 * colateral das tarefas relacionadas, retornando um erro caso
		 * isso seja tentado.
		 *
		 */
		public $ROW_NAME_BOOL_CLOSED_DATE		= 'ptosndatafechada';

		/**
		 * Coluna inteira para o controle do aviso de mensagens
		 * quando a data de termino se aproxima
		 */
		public $ROW_NAME_INT_ALERT_MISSING_DAYS = 'ptoavisoantecedencia';

		/**
		 * Chave estrangeira para a coluna do Produto 
		 */
		public $ROW_NAME_PRODUCT_ID				= 'procod';

		/**
		 * Chave estrangeira para a coluna da Acao 
		 */
		public $ROW_NAME_ACTION_ID				= 'acaid';

		/**
		 * Chave estrangeira para a coluna da unidade de medida do
		 * progresso da tarefa 
		 */
		public $ROW_NAME_MEAUSE_UNIT_ID			= 'unmcod';

		/**
		 * Campo de valor do controle da meta da Tarefa
		 * @see $dblPrevisaoMeta
		 */
		public $ROW_NAME_DBL_AIM				= 'ptoprevistoexercicio';

		/**
		 * Coluna booleana que informa se a tarefa esta em uso
		 * ou nao. 
		 *
		 */
		public $ROW_NAME_BOOL_ACTIVE_STATUS		= 'ptostatus';

		/**
		 * Coluna booleana que informa se a tarefa esta em uso
		 * ou nao. 
		 *
		 */
		public $ROW_NAME_BOOL_SPECIAL_ORIGIN 	= 'ptoorigemppa';

		/**
		 * Posicao da ordenacao das tarefas filhas
		 * de um mesmo nу
		 */
		public $ROW_NAME_INT_POSITION 			= 'ptoordemprov';
		
		/**
		 * Controle de Profundidade das tarefas do
		 * projeto
		 */
		public $ROW_NAME_INT_DEEPER 			= 'ptonivel';
		
		// fim dos nomes das tabelas e colunas da persisntecia utilizadas na classe //
				
		/**
		 * @var bool
		 */
		protected $boolSomenteLeitura = false;
		
		/**
		 * @var bool
		 */
		protected $boolEhSubAcao = false;
		
		/**
		 * @var bool
		 */
		protected $boolOrigemEspecial = false;
		
		/**
		 * Valor padrao da profundidade de uma tarefa
		 */
		protected $intProfundidade 			= 1;
		
		/**
		 * Valor default de uma meta prevista
		 */
		protected $dblPrevisaoMeta 			= 100;
		
		/**
		 * Valor padrao da unidade de medida
		 */
		protected $intUnidadeDeMedidaId 	= 2;

		/**
		 * Valor padrao para aviso de antecedencia
		 */
		protected $intQtdDiasAntecedenciaParaAviso = 7;
		
		/**
		 * @var integer
		 * @see self::ROW_NAME_PRODUCT_ID
		 * @activeFrozenField
		 */
		protected $intProdutoId = null;
				
		/**
		 * @var integer
		 * @see self::ROW_NAME_ACTION_ID
		 * @activeFrozenField
		 */
		protected $intAcaoId = null;
		
		/**
		 * @see $intDonoId
		 */
		protected $objDono = null;
		
		// setters and getters //
		
		public function setSubAcao( $boolSubAcao )
		{
			$this->boolEhSubAcao = $boolSubAcao;
		}
		
		public function getSubAcao()
		{
			return $this->boolEhSubAcao;
		}
		
		public function setPrevisaoMeta( $dblPrevisaoMeta )
		{
			$this->dblPrevisaoMeta = $dblPrevisaoMeta;
		}
		
		public function getPrevisaoMeta()
		{
			return $this->dblPrevisaoMeta;
		}
		
		public function setProfundidade( $intProfunidade )
		{
			$this->intProfundidade = $intProfunidade;
			$this->setSomenteLeitura( ( $this->getProfundidade() == 1 ) && ( $this->boolOrigemEspecial ) );
		}
		
		public function getProfundidade()
		{
			return $this->intProfundidade;
		}
		
		public function setQtdDiasAntesParaAviso( $intQtdDias )
		{
			$this->intQtdDiasAntecedenciaParaAviso = (integer) $intQtdDias;
		}
		
		public function getQtdDiasAntesParaAviso()
		{
			return $this->intQtdDiasAntecedenciaParaAviso;
		}
		
		public function setSomenteLeitura( $boolSomenteLeitura )
		{
			$this->boolSomenteLeitura = ( $boolSomenteLeitura == true );
		}
		
		public function getSomenteLeitura()
		{
			return $this->boolSomenteLeitura;
		}
		
		public function setUnidadeDeMedidaId( $intUnidadeMedidaId )
		{
			$this->intUnidadeDeMedidaId = $intUnidadeMedidaId;
		}
		
		public function getUnidadeDeMedidaId()
		{
			return $this->intUnidadeDeMedidaId;
		}
		
		public function setOrigemEspecial( $boolOrigemEspecial )
		{
			$this->boolOrigemEspecial = ( $boolOrigemEspecial == true );
			$this->setSomenteLeitura( ( $this->getProfundidade() == 1 ) && ( $this->boolOrigemEspecial ) );
		}
		
		public function getOrigemEspecial()
		{
			return $this->boolOrigemEspecial;
		}
		
		public function setProdutoId( $intProdutoId )
		{
			if( $intProdutoId )
			{
				$this->intProdutoId = (integer)$intProdutoId;
			}
			else
			{
				$this->intProdutoId = null;
			}
		}
		
		public function getProdutoId()
		{
			return $this->intProdutoId;
		}
		
		public function setAcaoId( $intAcaoId )
		{
			$this->intAcaoId = $intAcaoId;
		}
		
		public function getAcaoId()
		{
			return $this->intAcaoId;
		}
		
		public function getDono()
		{
			if ( $this->objDono == null && $this->intDonoId != null )
			{
				$objDono = new Usuario();
				$objDono->setId( $this->intDonoId );
				$objDono->emerge();
				$this->objDono = $objDono;  
			}
			return $this->objDono;
		}
		
		// fim dos setters and getters //
		public function appendChild()
		{
			$objNovaTarefa = $this->criaTarefa();
			$objNovaTarefa->setDataInicio( $this->getDataInicio() );
			$objNovaTarefa->setNovaDataInicio( $this->getDataInicio() );
			$objNovaTarefa->setDataFim( $this->getDataFim() );
			$objNovaTarefa->setNovaDataFim( $this->getDataFim() );
			$objNovaTarefa->setNome( 'Nova Tarefa' );
			$objNovaTarefa->setDescricao( 'Nova Descriзгo' );
			$objNovaTarefa->setProjetoId( $this->getProjetoId() );
			$objNovaTarefa->setContainerId( $this->getId() );
			$objNovaTarefa->setProfundidade( $this->getProfundidade() + 1 );
			$objNovaTarefa->setOrigemEspecial( $this->getOrigemEspecial() );
			$objNovaTarefa->setAcaoId( $this->getAcaoId() );
			$objNovaTarefa->inserirTarefa();			
		}
		
		public function append( $intProjetoId )
		{
			$this->setNome( 'Nova Tarefa' );
			$this->setDescricao( 'Nova Descriзгo' );
			$this->setProjetoId( $intProjetoId );
			$this->setDataInicio( $this->getProjeto()->getDataInicio() );
			$this->setDataFim( $this->getProjeto()->getDataFim() );
			$this->setProfundidade( 1 );
			$this->inserirTarefa();
		}
				
		/**
		 * Atualiza uma tarefa na persistencia
		 * 
		 * 1. Valida a tarefa
		 * 2. Atualiza a tarefa na persistencia
		 * 3. Marca a tarefa como nгo alterada
		 */
		public function atualizarTarefa()
		{
			
			$this->setDataFim( $this->getNovaDataFim() );
			$this->setDataInicio( $this->getNovaDataInicio() );

			$strSql =' UPDATE ' . "\n" .
						$this->TABLE_NAME .
					' SET ' .  "\n" .
						$this->ROW_NAME_PREDECESSOR_ID			. ' = ' . escape( $this->getPredecessora() != null ?  $this->getPredecessora()->getId() : null ) .
						',' . "\n" .
						$this->ROW_NAME_CONTAINER_ID			. ' = ' . escape( $this->getContainerId() ) . 
						',' .  "\n" .
						$this->ROW_NAME_STR_DESC				. ' = ' . escape( $this->getDescricao() ) . 
						',' .  "\n" .
						$this->ROW_NAME_STR_NAME				. ' = ' . escape( $this->getNome() ). 
						',' .  "\n" .
						$this->ROW_NAME_UNIQUE_CODE				. ' = ' . escape( $this->getCodigoUnico() ) .
						',' .  "\n" .
						$this->ROW_NAME_DATE_START				. ' = ' . escape( $this->getDataInicio(), 'date' ) . 
						',' .  "\n" .
						$this->ROW_NAME_DATE_END				. ' = ' . escape( $this->getDataFim() , 'date' ) .
						',' .  "\n" .
						$this->ROW_NAME_BOOL_CLOSED_DATE		. ' = ' . escape( $this->getDataFechada() ) .
						',' . "\n" .
						$this->ROW_NAME_OWNER_ID				. ' = ' . escape( $this->getDonoId() ) .
						',' . "\n" .
						$this->ROW_NAME_INT_ALERT_MISSING_DAYS	. ' = ' . escape( $this->getQtdDiasAntesParaAviso() ) .
						',' . "\n" .
						$this->ROW_NAME_DBL_AIM					. ' = ' . escape( $this->getPrevisaoMeta() ) .
						',' . "\n" .
						$this->ROW_NAME_PRODUCT_ID				. ' = ' . escape( $this->getProdutoId() ) .
						',' . "\n" .
						$this->ROW_NAME_ACTION_ID				. ' = ' . escape( $this->getAcaoId() ) .
						',' . "\n" .
						$this->ROW_NAME_INT_POSITION			. ' = ' . escape( $this->getPosicao() ) .
						',' . "\n" .
						$this->ROW_NAME_BOOL_SPECIAL_ORIGIN		. ' = ' . escape( $this->getOrigemEspecial() ) .
						',' . "\n" .
						$this->ROW_NAME_MEAUSE_UNIT_ID			. ' = ' . escape( $this->getUnidadeDeMedidaId() ) .
						',' . "\n" .
						$this->ROW_NAME_INT_DEEPER				. ' = ' . escape( $this->getProfundidade() ) .
						',' . "\n" .
						$this->ROW_NAME_BOOL_IS_SUB_ACTION		. ' = ' . escape( $this->getSubAcao() ) .
						' WHERE ' . "\n" .
						$this->ROW_NAME_ID				. ' = ' . $this->getId();
					'';

			// executando a query criada //
			$objSaida = self::$objDatabase->executar( $strSql );

			$this->setDatasAlteradas( false );
		}
		
		/**
		 * Executa de fato a remocao do elemento na persistencia
		 */
		public function delete()
		{
			if( ! $this->getSomenteLeitura() )
			{
	/*
				$strSql =	' DELETE FROM ' . 
								$this->TABLE_NAME .
							' WHERE ' .
								$this->ROW_NAME_ID . ' = ' . escape( $this->getId() ) .
							'';
	*/
				$strSql =	' UPDATE ' .
								$this->TABLE_NAME . 
							' SET ' .
								$this->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $this->TYPE_STATUS_INACTIVE ) .
							' WHERE ' .
								$this->ROW_NAME_ID . ' = ' . escape( $this->getId() ) .
							'';
				// executando a query criada //
				$objSaida = self::$objDatabase->executar( $strSql );
				
				# 3. Atualiza o identificador da tarefa						#
				$this->setId( NULL );
			}
			else
			{
				throw new Exception( 'Esta tarefa nгo pode ser excluida pois esta no modo somente leitura.' );	
			}
		}
		
		/**
		 * Executa o processo necessario para manter os dados validos apos a remocao de
		 * uma tarefa na persistencia.
		 * 
		 * 1. As Sucessoras Perdem a Referencia
		 * 2. As filhas sao removidas em cascata 
		 * 3. As Tarefas irmas, caso tambem nao sejam removidas 
		 * devem ser reorganizadas em suas posicao.
		 * 4. As tarefas a de codigo unico maiores do que a tarefa excluida de maior 
		 * codigo unico devem ter seus codigos unicos atualizados para os valores 
		 * sequencias segundo a ordem atual ignorando os valores que foram atualmente 
		 * removidos
		 */
		public function removerTarefa()
		{
			$this->setRemoved( true );
			$arrSucessoras = $this->getArraydeSucessoras();
			# 1. As Sucessoras Perdem a Referencia 
			foreach( $arrSucessoras as $objTarefaSucessora )
			{
				$objTarefaSucessora->setPredecessoraId( null );
				$objTarefaSucessora->setChanged( true );
			}
			# 2. As filhas sao removidas em cascata 
			$arrFilhas = $this->getArraydeTarefasqueContenho();
			foreach( $arrFilhas as $objTarefaFilha )
			{
				$objTarefaFilha->removerTarefa();
			}
			# 3. As Tarefas irmas, caso tambem nao sejam removidas
			# devem ser reorganizadas em suas posicao
			$arrIrmas = $this->getArraydeTarefasIrmas();
			$intPosicaoEmRelacaoAoPai = 1;
			$arrIrmas = orderArrayOfObjectsByMethod( $arrIrmas , 'getPosicao' );
			foreach( $arrIrmas as $objTarefaIrma )
			{
				if( ! $objTarefaIrma->getRemoved() )
				{
					$objTarefaIrma->setPosicao( $intPosicaoEmRelacaoAoPai );
					++$intPosicaoEmRelacaoAoPai;
					$objTarefaIrma->setChanged(  true );
				}
			}
			
			# 4. As tarefas a de codigo unico maiores do que a tarefa excluida de maior
			# codigo unico devem ter seus codigos unicos atualizados para os valores
			# sequencias segundo a ordem atual ignorando os valores que foram atualmente
			# removidos
			if( $objContainer == null || $this->objContainer->getRemoved() == false )
			{
				$arrTarefasSeguintes = $this->getArraydeTarefasSeguintes();
				$arrTarefasSeguintes = orderArrayOfObjectsByMethod( $arrTarefasSeguintes , 'getCodigoUnico' );	
				$intCodigoUnico = $this->getCodigoUnico();
				foreach( $arrTarefasSeguintes as $objTarefaSeguinte )
				{
					if( $objTarefaSeguinte->getRemoved() == false )
					{
						$objTarefaSeguinte->setCodigoUnico( $intCodigoUnico );
						$objTarefaSeguinte->setChanged( true );
						++$intCodigoUnico;
					}
				}
			}
		}
		
		public function verificaPossibilidadeDeRemover()
		{
			$arrMessangens = array();
			$intQtdSucessoras = sizeof( $this->getArraydeSucessoras() );
			$intQtdContidasDiretas = sizeof( $this->getArraydeTarefasqueContenho() );
			$intQtdContidasIndiretas = sizeof( $this->getArrQueContenhoDiretaseIndiretas() );
			if( ( $intQtdSucessoras + $intQtdContidasDiretas + $intQtdContidasIndiretas ) > 0 )
			{
				$arrMessangens[] = $this->getMensagemdeAvisoRiscoAoAlterar();
				
				if( $intQtdSucessoras > 0 )
				{
					$arrMessangens[] = $this->getMensagemdeAvisoPossuiSucessoras( $intQtdSucessoras );
				}
				if( $intQtdContidasDiretas > 0 )
				{
					$arrMessangens[] = $this->getMensagemdeAvisoPossuiFilhas( $intQtdContidasDiretas );
				}
				if( ( $intQtdContidasIndiretas > 0 ) && ( $intQtdContidasDiretas != $intQtdContidasIndiretas ) )
				{
					$arrMessangens[] = $this->getMensagemdeAvisoPossuiDescendentesIndiretos( $intQtdContidasIndiretas );
				}
			}
			return $arrMessangens;
		}
		
		/**
		 * Metodo que reintegra os dados das tarefas de um projeto
		 */
		public static function reparaTarefas( Projeto $objProjeto)
		{
			$objTarefa = $objProjeto->criaTarefa();;
			$arrTarefas = $objTarefa->getArrTarefasPeloProjeto( $objProjeto );
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
				$objTarefaReparando->setUnidadeDeMedidaId( 2 );
				$objTarefaReparando->setPrevisaoMeta( 100 );
				++$intCodigoUnicoAtual;
				++$intPosicaoNoNivel;
				$arrFilhas = $objTarefaReparando->getArraydeTarefasqueContenho();
				$intCodigoUnicoAtual = self::reparaRecursivamente( $arrFilhas , $intCodigoUnicoAtual , $intProfundidade + 1 );
				$objTarefaReparando->setChanged( true );
			}
			return $intCodigoUnicoAtual;
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
				$this->setAcaoId(					unescape( @$objSaida[ $this->ROW_NAME_ACTION_ID ]				, 'integer') );
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
							$this->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $this->TYPE_STATUS_ACTIVE ) .
						'';
			$objSaida = self::$objDatabase->carregar( $strSql );
			if( is_array( $objSaida ) && sizeof( $objSaida ) == 1 )
			{
				$objSaida = $objSaida[ 0 ];
			}
			$this->intQtdTarefasFilhas = (integer) @$objSaida[ 'qtd_filhas' ];
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
					'';
					
			// executando a query criada //

			$objSaida = self::$objDatabase->executar( $strSql );
			
			# 3. Atualiza o identificador da tarefa						#
			$this->setId( (integer) $objSaida );
			
			$this->setInserted( true );
		}
		
		/**
		 * Retorna o ProjetoPt desta tarefa
		 */
		public function getProjeto()
		{
			if( ( $this->objProjeto === NULL ) && ( $this->intProjetoId != NULL ) )
			{
				$this->objProjeto = ProjetoPT::getProjetoPeloId( $this->intProjetoId );
			}
			return $this->objProjeto;
		}
		
		/**
		 * Cria uma nova instancia de tarefaPT
		 */
		public function criaTarefa()
		{
			return new TarefaPT();
		}
	
	}
?>