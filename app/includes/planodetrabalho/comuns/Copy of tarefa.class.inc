<?php
	abstract class Tarefa extends AbstractEntity
	{
		/**
		 * Nome da tabela do banco onde ficam armazenadas as tarefas
		 *
		 */
		public $TABLE_NAME = 'monitora.planotrabalho';
		
		/**
		 * Identificador da Tabela
		 */
		public $ROW_NAME_ID 				= 'ptoid';
		
		
		/**
		 * @see ROW_NAME_ID
		 */
//		protected $intId = null;
		
		/**
		 * @var integer
		 * Campo para a chave do id do dono da tarefa
		 */		
		public $ROW_NAME_OWNER_ID 	= 'usucpf';
		
		
		/**
		 * @var integer
		 * @see self::ROW_NAME_OWNER_ID
		 */
		protected $intDonoId = null;
		
		/**
		 * Chave estrageira do id da tarefa predecessora direta
		 * 
		 * A predecessora direta é a tarefa que esta depende estar concluida
		 * para ser iniciada. Cada tarefa só tem uma predecessora direta mas
		 * várias tarefas podem ser predecessidas diretamente pela mesma 
		 * tarefa.
		 */
		public $ROW_NAME_PREDECESSOR_ID 	= 'ptoid_antecessor';

		/**
		 * @var Tarefa
		 * @see self::ROW_NAME_PREDECESSOR_ID
		 * @see Tarefa::$intPredecessoraId
		 */
		protected $objPredecessora = null;
		
		/**
		 * @var integer
		 * @see Tarefa::$ROW_NAME_PREDECESSOR_ID
		 * @see Tarefa::$objPredecessora
		 * @activeFrozenField
		 */
		protected $intPredecessoraId = null;
		
		/**
		 * Variavel de controle para acessar a predecessora
		 * no mesmo projeto a partir do codigo unico da mesma
		 * @activeFrozenField
		 */
		protected $intPredecessoraCodigoUnico = null;
		
		/**
		 * Chave estrangeira do id da tarefa que contém esta tarefa
		 * 
		 * As tarefas podem conter tarefas. As tarefas que não são contidas
		 * por nenhum outra tarefa possuem este campo nulo enquanto as que
		 * pertencem a uma tarefa possuem o id da tarefa que a contem neste
		 * campo.
		 *
		 */
		public $ROW_NAME_CONTAINER_ID 	= 'ptoid_pai';
		/**
		 * @var Tarefa
		 * @see self::ROW_NAME_CONTAINER_ID
		 * @see Tarefa::$intContainerId
		 */
		protected $objContainer = null;
		
		/**
		 * @var integer
		 * @see Tarefa::$ROW_NAME_CONTAINER_ID
		 * @see Tarefa::$objContainer
		 * @activeFrozenField
		 */
		protected $intContainerId = null;
		
		/**
		 * Chave estrangeira do id do projeto de que estas tarefas pertencem.
		 * 
		 * As tarefas pertecem a um projeto então todas as tarefas devem
		 * ter o id do projeto a quem pertencem. As tarefas não devem exceder
		 * os limites das datas do projeto. Caso alguma tente fazer isto é 
		 * checado se o projeto possui flexibilidade de data, se isto for 
		 * verdade o mesmo é alterado para a tarefa permanecer dentro dos 
		 * seus limites, se não é retornado um erro.
		 *
		 */
		public $ROW_NAME_PROJECT_ID 		= 'pjeid';
		
		/**
		 * @var integer
		 * @see self::ROW_NAME_PROJECT_ID
		 * @see Tarefa::$objProjeto
		 */
		protected $intProjetoId = null;
		
		/**
		 * @var Projeto
		 * @see self::ROW_NAME_PROJECT_ID
		 * @see Tarefa::$intProjetoId
		 */
		protected $objProjeto = null;
		
		/**
		 * Coluna de texto que contém o nome da tarefa
		 */
		public $ROW_NAME_STR_NAME 		= 'ptodsc';
		
		/**
		 * @var string
		 * @see self::ROW_NAME_STR_NAME
		 * @activeFrozenField
		 */
		protected $strNome = null;
		
		/**
		 * Coluna de texto que contém a descrição da tarefa
		 *
		 */
		public $ROW_NAME_STR_DESC 		= 'ptodescricao';
		
		/**
		 * @var string
		 * @see self::ROW_NAME_STR_DESC
		 * @activeFrozenField
		 */
		protected $strDescricao = null;
				
		/**
		 * Coluna numérica inteira que contém um código único, no escopo
		 * do projeto, de cada tarefa.
		 *
		 */
		public $ROW_NAME_UNIQUE_CODE 		= 'ptoordem';
		
		/**
		 * @var integer
		 * @see self::ROW_NAME_UNIQUE_CODE
		 * @activeFrozenField
		 */
		protected $intCodigoUnico = null;
		
		/**
		 * Coluna de data que contém a data de início da tarefa
		 */
		public $ROW_NAME_DATE_START 		= 'ptodata_ini';
		
		/**
		 * @var date
		 * @see self::ROW_NAME_DATE_START
		 * @activeFrozenField
		 */
		protected $datInicio = null;
		
		/**
		 * Coluna de data que contém a data de término da tarefa
		 */
		public $ROW_NAME_DATE_END 		= 'ptodata_fim';
		
		/**
		 * @var date
		 * @see self::ROW_NAME_DATE_END
		 * @activeFrozenField
		 */
		protected $datFim = null;
		
		/**
		 * Coluna booleana que informa se a tarefa possui a data
		 * fechada ou não.
		 * 
		 * Caso a data seja fechada não se pode altera-la por efeito
		 * colateral das tarefas relacionadas, retornando um erro caso
		 * isso seja tentado.
		 *
		 */
		public $ROW_NAME_BOOL_CLOSED_DATE	= 'ptosndatafechada';

		/**
		 * @var bool
		 * @see self::ROW_NAME_BOOL_CLOSED_DATE
		 * @activeFrozenField
		 */
		protected $boolDataFechada = false;
		
		/**
		 * @var bool
		 * @see self::ROW_NAME_BOOL_CLOSED_PERIDO
		 * @activeFrozenField
		 */
		protected $boolPeridoFechado = false;
		
		/**
		 * Campo de controle para quando faltarem esta
		 * quantidade de dias para o fim de uma tarefa
		 * enviar uma mensagem ao dono da mesma
		 */
		protected $intQtdDiasAntecedenciaParaAviso = null;
		
		/**
		 * Chave estrangeira para a coluna da unidade de medida do
		 * progresso da tarefa 
		 */
		public $ROW_NAME_MEAUSE_UNIT_ID	= 'unmcod';

		/**
		 * @var integer
		 * @see self::ROW_NAME_MEAUSE_UNIT_ID
		 * @activeFrozenField
		 */
		protected $intUnidadeDeMedidaId = null;
		
		/**
		 * @var integer
		 * @see self::ROW_NAME_DBL_AIM
		 * @activeFrozenField
		 */
		protected $dblPrevisaoMeta = null;
		
		/**
		 * Coluna booleana que informa se a tarefa esta em uso
		 * ou nao. 
		 *
		 */
		public $ROW_NAME_BOOL_ACTIVE_STATUS	= 'ptostatus';

		protected $TYPE_STATUS_ACTIVE 	= 'A';
		protected $TYPE_STATUS_INACTIVE 	= 'I';
		
		/**
		 * @var bool
		 * @see self::ROW_NAME_BOOL_ACTIVE_STATUS
		 * @activeFrozenField
		 */
		protected $boolStatusAtivo = true;
		
		/**
		 * Posicao da ordenacao das tarefas filhas
		 * de um mesmo nó
		 */
		public $ROW_NAME_INT_POSITION = 'ptoordemprov';
		
		/**
		 * @var Tarefa
		 * @see self::ROW_NAME_CONTAINER_ID
		 * @see Tarefa::$intContainerId
		 */
		protected $intPosicao = null;
		
		/**
		 * Variavel de controle para evitar recursao infinita ou alteracoes
		 * que tornem os dados invalidos
		 */
		protected $boolAlterada = false;
		
		/**
		 * Variavel de controle da quantidade de tarefas filhas que a tarefa
		 * atual tem
		 */
		protected $intQtdTarefasFilhas = 0;
		
		/**
		 * Valor em timestamp de um segundo
		 *
		 */		
		protected $TIMESTAMP_SECOND	= 1;
		/**
		 * Valor em timestamp de um minuto
		 *
		 */		
		protected $TIMESTAMP_MINUTE	= 60;
		
		/**
		 * Valor em timestamp de uma hora
		 *
		 */		
		protected $TIMESTAMP_HOUR	= 3600;
		
		/**
		 * Valor em timestamp de um dia
		 *
		 */		
		protected $TIMESTAMP_DAY		= 86400;
				
		/**
		 * Array estatico das tarefas já lidas. Evita se instanciar mais
		 * de uma copia do mesmo objeto e recursividade eterna.
		 *
		 * @var Array of Tarefa
		 */
		static protected $arrTarefasInstances = array();
		
		/**
		 * Array das tarefas sucessoras a esta tarefa
		 */
		protected $arrTarefasSucessoras = null;
		
		/**
		 * Array das tarefas predecessoras a esta tarefa
		 */
		protected $arrTarefasPredecessoras = null;
		
		/**
		 * Array das tarefas sucessoras diretas e indiretas a este tarefa
		 */
		protected $arrTarefasSucessorasDiretaseIndiretas = null;
		
		/**
		 * Array das tarefas predecessoras diretas e indiretas a este tarefa
		 */
		protected $arrTarefasPredecessorasDiretaseIndiretas = null;
		
		/**
		 * Array das tarefas que contem esta tarefa
		 */
		protected $arrTarefasQueMeContem = null;
		
		/**
		 * Array das tarefas que esta tarefa contem
		 */
		protected $arrTarefasQueContenho = null;
		
		/**
		 * Array das tarefas predecessoras diretas e indiretas a este tarefa
		 */
		protected $arrTarefasQueContenhoDiretaseIndiretas = null;
		
		/**
		 * Array das tarefas predecessoras diretas e indiretas a esta tarefa
		 */
		protected $arrTarefasQueContenhoeRelacionadas = null;
		
		/**
		 * Array de tarefas que possuem o codigo unico maior do que o desta tarefas
		 */
		protected $arrTarefasSeguintes = null;
		
		/**
		 * @var date
		 * @activeFrozenField
		 */
		protected $datNovaDataInicio;
		
		/**
		 * @var date
		 * @activeFrozenField
		 */
		protected $datNovaDataFim;
		
		/**
		 * Codigo Estruturado 
		 */
		protected $strCodigoEstruturado;
		
		/**
		 * @var object database connection
		 */		
		static protected $objDatabase;
		
		protected $boolNovaDataInicioMantemPeriodo = true;
		
		protected $boolNovaDataFimMantemPeriodo = false;
		
		// sets e gets //
				
		public function setNome( $strNome )
		{
			if( $strNome != null )
			{
				$this->strNome = $strNome;
			}
			else
			{
				$this->strNome = null;
			}
		}
		
		public function getNome()
		{
			return $this->strNome;
		}
		
		public function setDescricao( $strDescricao )
		{
			if( $strDescricao != null )
			{
				$this->strDescricao =  (string) $strDescricao;
			}
			else
			{
				$this->strDescricao = null;
			}
		}
		
		public function getDescricao()
		{
			return $this->strDescricao;
		}
		
		public function setCodigoUnico( $intCodigoUnico )
		{
			if( $intCodigoUnico != null )
			{
				$this->intCodigoUnico = (integer) $intCodigoUnico;
			}
			else
			{
				$this->intCodigoUnico = null;
			}
		}
		
		public function getCodigoUnico()
		{
			return $this->intCodigoUnico;
		}
		
		public function setDataInicio( $datInicio )
		{
			if( $datInicio != null )
			{
				$this->datInicio = $datInicio;
			}
			else
			{
				$this->datInicio = null;
			}
		}
		
		public function getDataInicio()
		{
			return $this->datInicio;
		}
		
		public function getDataInicioTimestamp()
		{
			return convertToTimestamp( $this->datInicio );
		}
		
		public function getNovaDataInicio()
		{
			if( $this->datNovaDataInicio !== null )
			{
				return $this->datNovaDataInicio;
			}
			else
			{
				return $this->datInicio;
			}
		}
		
		public function setNovaDataInicio( $datInicio )
		{
			if( $datInicio != null )
			{
				$intDuracao = $this->getDuracaoTimestamp();
				$this->datNovaDataInicio = $datInicio;
				if( $this->boolNovaDataInicioMantemPeriodo )
				{
					if( $this->getNovaDataFim() == $this->getDataFim() )
					{
						$this->datNovaDataFim = date( 'd/m/Y' , $this->getNovaDataInicioTimestamp() + $intDuracao );
					}
					else
					{
					}
				}
			}
			else
			{
				$this->datNovaDataInicio = null;
			}
			$this->setDatasAlteradas( true );
		}
		
		public function getNovaDataInicioTimestamp()
		{
			return convertToTimestamp( $this->getNovaDataInicio() );
		}
				
		public function setDataFim( $datFim )
		{
			if( $datFim != null )
			{
				$this->datFim = $datFim;
			}
			else
			{
				$this->datFim = null;
			}
		}
		
		public function getDataFim()
		{
			return $this->datFim;
		}
		
		public function getDataFimTimestamp()
		{
			return convertToTimestamp( $this->datFim );
		}
		
		public function getNovaDataFim()
		{
			if( $this->datNovaDataFim !== null )
			{
				return $this->datNovaDataFim;
			}
			else
			{
				return $this->datFim;
			}
		}
		
		public function setNovaDataFim( $datFim )
		{
			if( $datFim != null )
			{
				$this->datNovaDataFim = $datFim;
				if( $this->boolNovaDataFimMantemPeriodo )
				{
					if( $this->getNovaDataInicio() == $this->getDataInicio() )
					{
						$intDuracao = $this->getDuracaoTimestamp();
						$this->datNovaDataInicio = date( 'd/m/Y' , $this->getNovaDataFimTimestamp() - $intDuracao );
					}
				}
			}
			else
			{
				$this->datNovaDataFim = null;
			}
			$this->setDatasAlteradas( true );
		}
		
		public function getNovaDataFimTimestamp()
		{
			return convertToTimestamp( $this->getNovaDataFim() );
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
				$this->objPredecessora = $this->getTarefaPeloCodigoUnico( $this->intPredecessoraCodigoUnico , $this->getProjeto() );
				$this->intPredecessoraCodigoUnico = $this->objPredecessora->getCodigoUnico();
				$this->intPredecessoraId = $this->objPredecessora->getId();
			}
			return $this->objPredecessora;
		}
		
		public function setPredecessora( Tarefa $objPredecessora )
		{
			$this->objPredecessora = $objPredecessora;
			$this->intPredecessoraId = $objPredecessora->getId();
			$this->intPredecessoraCodigoUnico = $objPredecessora->getCodigoUnico();
		}
		
		public function setPredecessoraCodigoUnico( $intPredecessoraCodigoUnico )
		{
			$this->limpaPredecessora();
			if( $intPredecessoraCodigoUnico != null )
			{
				$this->intPredecessoraCodigoUnico = $intPredecessoraCodigoUnico;
			}
			else
			{
				$this->intPredecessoraCodigoUnico = null;
			}
		}
		
		public function setPredecessoraId( $intPredecessoraId )
		{
			$this->limpaPredecessora();
			if( $intPredecessoraId != null )
			{
				$this->intPredecessoraId = (integer) $intPredecessoraId;
			}
			else
			{
				$this->intPredecessoraId = null;
			}
			$this->objPredecessora = null;
		}
		
		public function getPredecessoraId()
		{
			return $this->intPredecessoraId;
		}
		
		public function setContainer( Tarefa $objContainer )
		{
			$this->objContainer = $objContainer;
			$this->intContainerId = $objContainer->getId();
		}
		
		public function getContainer()
		{
			if( ( $this->objContainer === NULL ) && ( $this->intContainerId != NULL ) )
			{
				$this->objContainer = $this->getTarefaPeloId( $this->intContainerId );
			}
			return $this->objContainer;
		}
		
		public function setContainerId( $intContainerId )
		{
			if( $intContainerId != null )
			{
				$this->intContainerId = (integer) $intContainerId;
			}
			else
			{
				$this->intContainerId = null;
			}
			$this->objContainer = null;
		}
		
		public function getContainerId()
		{
			return $this->intContainerId;
		}
		
		public function setProjetoId( $intProjetoId )
		{
			if( $intProjetoId != null )
			{
				$this->intProjetoId = (integer) $intProjetoId;
			}
			else
			{
				$this->intProjetoId = null;
			}
			$this->objProjeto = null;
		}
		
		public function getProjetoId()
		{
			return $this->intProjetoId;
		}
		
		public function setProjeto( Projeto $objProjeto )
		{
			$this->objProjeto = $objProjeto;	
		}
		
		abstract public function getProjeto();
/*			
		{
			if( ( $this->objProjeto === NULL ) && ( $this->intProjetoId != NULL ) )
			{
				$this->objProjeto = Projeto::getProjetoPeloId( $this->intProjetoId );
			}
			return $this->objProjeto;
		}
*/

		public function setDataFechada( $boolDataFechada )
		{
			if( $boolDataFechada !== null )
			{
				$this->boolDataFechada = ( ( $boolDataFechada === "true" ) || ( $boolDataFechada === true ) || ( $boolDataFechada === 't' ) || ( $boolDataFechada === 1 ) );
			}
			else
			{
				$this->boolDataFechada = null;
			}
		}
		
		public function getDataFechada()
		{
			return $this->boolDataFechada;
		}
		
		public function setPeriodoFechado( $boolDataFechada )
		{
			if( $boolDataFechada !== null )
			{
				$this->boolPeridoFechado = ( ( $boolPeridoFechado === "true" ) || ( $boolPeridoFechado === true ) || ( $boolPeridoFechado === 't' ) || ( $boolPeridoFechado === 1 ) );
			}
			else
			{
				$this->boolPeridoFechado = null;
			}
		}
		
		public function getPeriodoFechado()
		{
			return $this->boolPeridoFechado;
		}
		
		public function setPosicao( $intPosicao , $boolMudaPosicaoCascata = false )
		{
			if( $boolMudaPosicaoCascata )
			{
				$this->mudaPosicaoCascata( $intPosicao );
			}
			else
			{
				$this->intPosicao = $intPosicao;
			}
		}
		
		public function getPosicao()
		{
			return $this->intPosicao;
		}
		
		public function getQuantidadeDeTarefasFilhas()
		{
			return $this->intQtdTarefasFilhas;
		}
				
		public function getCodigoEstruturado()
		{
			if( $this->strCodigoEstruturado == null )
			{
				$objContainer = $this->getContainer();
				if( $objContainer === null )
				{
					$strResult = $this->getPosicao();	
				}
				else
				{
					$strResult = $objContainer->getCodigoEstruturado() . '.' . $this->getPosicao() ;
				}
				$this->strCodigoEstruturado = $strResult;
			}
			return $this->strCodigoEstruturado;
		}
		
		public function setDonoId( $intDonoId )
		{
			$this->intDonoId = $intDonoId;
		}
		
		public function getDonoId()
		{
			return $this->intDonoId;
		}
		
		
		
		// fim dos sets e gets //
		
		/**
		 * Construtor das tarefas.
		 */
		public function init()
		{
			global $db;
			self::$objDatabase = $db;
		}

		protected function limpaPredecessora()
		{
			$this->objPredecessora = null;
			$this->intPredecessoraId = null;
			$this->intPredecessoraCodigoUnico = null;
		}

		protected function mudaPosicaoCascata( $intPosicao )
		{
			$objContainer =  $this->getContainer();
			$arrTarefasIrmas  = array();
			if( $objContainer != NULL )
			{
				$arrTarefasIrmas = $this->getContainer()->getArraydeTarefasqueContenho();
			}
			foreach( $arrTarefasIrmas as $objTarefaIrma )
			{
				if( $objTarefaIrma->getPosicao() == $intPosicao )
				{
					$objTarefaIrma = $this->intPosicao;
					$this->intPosicao = $intPosicao;
					return;
				}
			}
			$this->intPosicao = (integer) $intPosicao;
		}
		
		/**
		 * Retorna o tamanho da duração da tarefa em timestamp
		 *
		 * @return integer
		 */
		public function getDuracaoTimestamp()
		{
			return $this->getDataFimTimestamp() - $this->getDataInicioTimestamp();
		}
		
		/**
		 * Retorna o novo tamanho da duração da tarefa em timestamp
		 *
		 * @return integer
		 */
		public function getNovaDuracaoTimestamp()
		{
			return $this->getNovaDataFimTimestamp() - $this->getNovaDataInicioTimestamp();
		}
		
		/**
		 * Recebe um array com os valores das chaves pré-determinados e
		 * preenche os campos do array.
		 *
		 * @param Array of strings
		 */
		public function setValoresPorArray( $arrInput )
		{
			$this->setCodigoUnico(			@$arrInput[ 'codigoUnico' ] );
			$this->setDataFechada(			@$arrInput[ 'dataFechada' ] );
			$this->setDataFim(				@$arrInput[ 'dataFim' ] );
			$this->setDataInicio(			@$arrInput[ 'dataInicio' ] );
			$this->setDescricao(			@$arrInput[ 'descricao' ] );
			$this->setId(					@$arrInput[ 'id' ] );
			$this->setNome(					@$arrInput[ 'nome' ] );
			$this->setPredecessoraId(		@$arrInput[ 'predecessora' ] );
			$this->setProjetoId(			@$arrInput[ 'pjeid' ] );
		}
		
		
		/**
		 * Seleciona todas as tarefas do projeto que tenham o mesmo id desta e que
		 * não tenham o mesmo id.
		 *
		 * @return array of Tarefas
		 */
		public function getArrCodigoUnicoPorProjeto()
		{
			$arrResult = array();
			
			$strSql =	' SELECT ' .
							$this->ROW_NAME_ID . 
						' FROM ' .
							$this->TABLE_NAME .
						' WHERE ' .
							$this->ROW_NAME_PROJECT_ID . ' = ' . escape( $this->getProjetoId() ) .
						' AND ' .
							$this->ROW_NAME_UNIQUE_CODE . ' = ' . escape( $this->getCodigoUnico() ) .
						' AND ' .
							$this->ROW_NAME_ID	. ' != ' . escape( $this->getId() ) .
						' AND ' .
							$this->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $this->TYPE_STATUS_ACTIVE ) .
						'';
			$objSaida = self::$objDatabase->carregar( $strSql );
			
			if( $objSaida !== FALSE )
			{
				foreach( $objSaida as $objLinha )
				{
					$objTarefaQueMeContem = $this->getTarefaPeloId( $objLinha[ $this->ROW_NAME_ID ] );
					$arrResult[] = $objTarefaQueMeContem;
				}
			}
			
			
			return $arrResult;
		}
		
		
		/**
		 * Atualiza na persistencia todas as tarefas que foram alteradas nesta execução.
		 *
		 */
		public function atualizaTodasAsTarefasAlteradas()
		{
			foreach( self::$arrTarefasInstances as $objTarefa )
			{
				if( $objTarefa->getDatasAlteradas() )
				{
					$objTarefa->atualizarTafefa();
				}
			}
		}
		
		/**
		 * Atualiza uma tarefa na persistencia
		 * 
		 * 1. Valida a tarefa
		 * 2. Atualiza a tarefa na persistencia
		 * 3. Marca a tarefa como não alterada
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
						$this->ROW_NAME_INT_POSITION			. ' = ' . escape( $this->getPosicao() ) .
						' WHERE ' . "\n" .
						$this->ROW_NAME_ID				. ' = ' . $this->getId();
					'';
			// executando a query criada //
			$objSaida = self::$objDatabase->executar( $strSql );
			if ( !$objSaida ) {
				return false;
			}
			$this->setDatasAlteradas( false );
			return true;
		}
		
		public function appendChild()
		{
			$objNovaTarefa = $this->criaTarefa();
			$objNovaTarefa->setDataInicio( $this->getDataInicio() );
			$objNovaTarefa->setNovaDataInicio( $this->getDataInicio() );
			$objNovaTarefa->setDataFim( $this->getDataInicio() );
			$objNovaTarefa->setNovaDataFim( $this->getDataFim() );
			$objNovaTarefa->setNome( 'Nova Tarefa' );
			$objNovaTarefa->setDescricao( 'Nova Descrição' );
			$objNovaTarefa->setProjetoId( $this->getProjetoId() );
			$objNovaTarefa->setContainerId( $this->getId() );
			$objNovaTarefa->inserirTarefa();			
		}
		
		public function append( $intProjetoId )
		{
			$this->setProjetoId( $intProjetoId );
			$this->setDataInicio( $this->getProjeto()->getDataInicio() );
			$this->setDataFim( $this->getProjeto()->getDataFim() );
			$this->inserirTarefa();
		}
		
		/**
		 * Insere a tarefa na persistencifa após validar os seus campos
		 * 
		 * 1. Valida a tarefa
		 * 2. Insere a tarefa na persistencia
		 * 3. Atualiza o identificador da tarefa
		 */
		public function inserirTarefa()
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
				$arrIrmas = $this->getArrTarefasPeloProjeto( $this->getProjeto() );
			}
			$arrIrmas = orderArrayOfObjectsByMethod( $arrIrmas , 'getPosicao' );
			
			$this->setPosicao(  1 + sizeof( $arrIrmas ) );
			
			if( sizeof( $arrIrmas ) > 0 )
			{
				$objTarefaAnterior = ( $arrIrmas[ sizeof( $arrIrmas ) - 1 ] );
			}
			elseif( $this->getContainerId() !== null )
			{
				$objTarefaAnterior = $this->getContainer();
			}
			else
			{
				$objTarefaAnterior = null;
			}
			
			if( $objTarefaAnterior === null )
			{
				$this->setCodigoUnico( 1 );
			}
			else
			{
				$arrTarefasSeguintes = $objTarefaAnterior->getArraydeTarefasSeguintes();
				
				foreach( $arrTarefasSeguintes as $objTarefaSeguinte )
				{
					$objTarefaSeguinte->setCodigoUnico( $objTarefaSeguinte->getCodigoUnico() + 1 );
					$objTarefaSeguinte->setChanged( true );
				}
				
				$this->setCodigoUnico( $objTarefaAnterior->getCodigoUnico() + 1 );
			}
			$this->insert();
		}
		
		public function delete()
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
			$objContainer = $this->getContainer();
			$arrIrmas = array();
			if( $objContainer !== NULL )
			{
				if( !$objContainer->getRemoved() )
				{
					$arrIrmas = $objContainer->getArraydeTarefasqueContenho();
				}
			}
			else
			{
				if ( $this->getProjeto() ) {
					# adaptação necessária para tarefas que não dependem de projeto
					$arrIrmas = $this->getArrTarefasPeloProjeto( $this->getProjeto() );
				}
			}
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
		 * Retorna o array de tarefas predecessoras diretas desta tarefa
		 *
		 * @return Array of Tarefa
		 */
		protected function getArraydePredecessoras()
		{
			if ( !$this->ROW_NAME_PREDECESSOR_ID ) {
				# adaptação necessária para tarefas que não tem controle de precedência
				return array();
			}
			if( $this->arrTarefasPredecessoras === null )
			{
				if( $this->getPredecessora() === null )
				{
					$arrResult = array();
				}
				else if ( isset( self::$arrTarefasInstances[ $this->getPredecessora()->getId() ] ) )
				{
					$arrResult = array( self::$arrTarefasInstances[ $this->getPredecessora()->getId() ] );
				}
				else
				{
					$arrResult = array();
					
					$strSql =	' SELECT ' .
									$this->ROW_NAME_ID . 
								' FROM ' .
									$this->TABLE_NAME .
								' WHERE ' .
									$this->ROW_NAME_ID . ' = ' . escape( $this->getPredecessora()->getId() ) .
								' AND ' .
									$this->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $this->TYPE_STATUS_ACTIVE ) .
								'';
									
					$objSaida = self::$objDatabase->carregar( $strSql );
					
					if( $objSaida !== FALSE )
					{
						foreach( $objSaida as $objLinha )
						{
							$objTarefaPredecessor = $this->getTarefaPeloId( $objLinha[ $this->ROW_NAME_ID ] );
							$arrResult[] = $objTarefaPredecessor;
						}
					}
				}
				$this->arrTarefasPredecessoras = $arrResult;
			}
			return $this->arrTarefasPredecessoras ;
		}
		
		public function getTarefaPeloCodigoUnico( $intTarefaCodigo , Projeto $objProjeto )
		{
			$arrResult = array();
			
			$strSql =	' SELECT ' .
							$this->ROW_NAME_ID . 
						' FROM ' .
							$this->TABLE_NAME .
						' WHERE ' .
							$this->ROW_NAME_UNIQUE_CODE . ' = ' . escape( $intTarefaCodigo ) .
						' AND ' .
							$this->ROW_NAME_PROJECT_ID . ' = ' . escape( $objProjeto->getId() ) .
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
					throw new Exception( 'Mais de uma Tarefa por codigo unico ( ' . $intTarefaCodigo . ' ) no projeto ( ' . $objProjeto->getId() . ' ) ') ;	
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
		protected function getArraydeSucessoras()
		{
			if ( !$this->ROW_NAME_PREDECESSOR_ID ) {
				# adaptação necessária para tarefas que não tem controle de precedência
				return array();
			}
			if( $this->arrTarefasSucessoras === null )
			{
				$arrResult = array();
				$strSql =	' SELECT ' .
								$this->ROW_NAME_ID . 
							' FROM ' .
								$this->TABLE_NAME .
							' WHERE ' .
								$this->ROW_NAME_PREDECESSOR_ID . ' = ' . escape( $this->getId() ) .
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
					$this->arrTarefasSucessoras = $arrResult;
				}
				else
				{
					$this->arrTarefasSucessoras  = array();
				}
			}
			return $this->arrTarefasSucessoras;
		}
		
			/**
		 * Retorna o array de sucessoras diretas desta tarefa
		 * 
		 * @return Array of Tarefa
		 */
		protected function getArraydeTarefasSeguintes()
		{
			if ( !$this->ROW_NAME_PREDECESSOR_ID ) {
				# adaptação necessária para tarefas que não tem controle de precedência
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
								$this->ROW_NAME_PROJECT_ID . ' = ' . escape( $this->getProjeto()->getId() ) .
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
		
		/**
		 * Retorna o array das tarefas que contenham diretamente esta tarefa.
		 * 
		 * @return Array of Tarefa
		 */
		protected function getArraydeTarefasqueMeContem()
		{
			if( $this->arrTarefasQueMeContem === null )
			{
				if( $this->getContainerId() == null )
				{
					$arrResult = array();
				}
				if( isset( self::$arrTarefasInstances[ $this->getContainerId() ] ) )
				{
					// Na configuração atual do banco está previsto que sempre haverá apenas	//
					// uma tarefa que contenha diretamente esta, logo pode-se otimizar caso		//
					// esta tarefa container já tenha sido carregada.							//
					$arrResult = array( self::$arrTarefasInstances[ $this->getContainerId() ] );
				}
				else
				{
					$arrResult = array();
					
					$strSql =	' SELECT ' .
									$this->ROW_NAME_ID . 
								' FROM ' .
									$this->TABLE_NAME .
								' WHERE ' .
									$this->ROW_NAME_ID . ' = ' . escape( $this->getContainerId() ) .
								' AND ' .
									$this->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $this->TYPE_STATUS_ACTIVE ) .
								'';
					$objSaida = self::$objDatabase->carregar( $strSql );
					
					if( $objSaida !== FALSE )
					{
						foreach( $objSaida as $objLinha )
						{
							$objTarefaQueMeContem = $this->getTarefaPeloId( $objLinha[ $this->ROW_NAME_ID ] );
							$arrResult[] = $objTarefaQueMeContem;
						}
					}
				}
				$this->arrTarefasQueMeContem = $arrResult;
			}
			return $this->arrTarefasQueMeContem ;
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
		 * Retorna a tarefa ao receber o id da mesma.
		 *
		 * @param integer $intId
		 * @return Tarefa
		 */
		public function getTarefaPeloId( $intId )
		{
			return $this->getInstanceById( $intId );
		}
		
/*
		{
			return new Tarefa();
		}
*/
		/**
		 * Retorna as tarefas de um projeto
		 *
		 * @param Project $objProject
		 * @return Tarefa
		 */
		public function getArrTarefasPeloProjeto( Projeto $objProjeto )
		{
			$arrResult = array();
			$strSql =	' SELECT ' .
							$this->ROW_NAME_ID . 
						' FROM ' .
							$this->TABLE_NAME .
						' WHERE ' .
							$this->ROW_NAME_PROJECT_ID . ' = ' . escape( $objProjeto->getId() ) .
						' AND ' .
							$this->ROW_NAME_CONTAINER_ID . ' IS ' . escape( null ) .
						' AND ' .
							$this->ROW_NAME_BOOL_ACTIVE_STATUS . ' = ' . escape( $this->TYPE_STATUS_ACTIVE ) .
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
					$objTarefa = $objProjeto->criaTarefa();
					$objTarefaDoProjeto = $objTarefa->getTarefaPeloId( $objLinha[ $this->ROW_NAME_ID ] );
					$arrResult[] = $objTarefaDoProjeto;
				}
			}
			return $arrResult;
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
			else
			{
				throw new Exception( 'Tarefa ( ' . $this->getId() . ' ) não existe' ) ;
			}
		}
		
		protected function getMensagemdeAvisoRiscoAoAlterar()
		{
			return 'Atenção, outras tarefas fazem referencia a esta. ' . 
				'Ao remove-la irá gerar alterações nas mesmas.' .
				'';
		}
		
		protected function getMensagemdeAvisoPossuiFilhas( $intQtdFilhas )
		{
			return 'Esta Tarefa possui ' . $intQtdFilhas . ' filha(s). ' . 
				'Esta(s) tarefa(s) serão removida(s) juntamente. ' .
				'';
		}
		
		protected function getMensagemdeAvisoPossuiDescendentesIndiretos( $intQtdTarefasContidas )
		{
			return 'Caso confirmada, a operação estará totalizando, ' . 
				'devido ao processo de remoção em cascata, a remoção ' .
				'da(s) ' . $intQtdTarefasContidas . ' Tarefa(s) contida(s) nesta.' . 
				'';
		}
		
		protected function getMensagemdeAvisoPossuiSucessoras( $intQtdSucessoras )
		{
			return 'Esta Tarefa possui ' . $intQtdSucessoras . ' sucessora(s).' . 
				'Esta(s) tarefa(s) perderá a predecessora. '.
				'';
		}
		
		
		
		/**
		 * Mensagem de erro quando uma sucessora de data fixa precisa ser atualizada
		 * 
		 * @return string
		 */
		protected function getMensagemdeErroSucessorasDataFixa()
		{
			return	'Não foi possível alterar as tarefas sucessoras.' .
				'A tarefa ' . $this->getCodigoUnico() . ' - ' . $this->getNome() . ' não pode ter a sua data atualizada pois a data ' .
				' da mesma é fechada.' .
				'';			
		}
		
		/**
		 * Mensagem de erro quando uma tarefa tem que ser alterada para duas datas diferentes.
		 * 
		 * Normalmente ocorre quando as tarefas contidas não comportam mais no período da tarefa
		 * que as contém.
		 * 
		 * @return string
		 */
		protected function getMensagemdeErroTarefaNaoComporta()
		{
			return	'A tarefa ' . $this->getCodigoUnico() . ' - ' . $this->getNome() . ' não pode ter a sua data atualizada pois o '.
				' seu período não comporta as alterações requisitadas.' .
				'';			
		}
		
		/**
		 * Mensagem de erro quando uma predecessora de data fixa precisa ser atualizada.
		 *
		 * @return string
		 */
		protected function getMensagemdeErroPredecessorasDataFixa()
		{
			return	'Não foi possível alterar as tarefas predecessoras.' .
				'A tarefa ' . $this->getCodigoUnico() . ' - ' . $this->getNome() . ' não pode ter a sua data atualizada ' .
				'pois a data da mesma é fechada.' .
				'';			
		}
		
		/**
		 * Mensagem de erro quando uma tarefa tenta ser predecessora e sucessora de outra, direta
		 * ou indiretamente.
		 * 
		 * @return string
		 */
		protected function getMensagemErroPredecessoraESucessora()
		{
			return 	''.
				' A tarefa não pode ter como predecessora a tarefa: ' . $this->getCodigoUnico() . ' - ' . $this->getNome() . 
				' que é uma de suas sucessoras, diretas ou indiretas.' .
				'';
		}
		
		/**
		 * Mensagem de erro quando uma tarefa contida de data fixa precisa ter sua data atualizada.
		 *
		 * @return string
		 */
		protected function getMensagemErroTarefaContida()
		{
			return	'Não foi possível alterar alguma(s) tarefas que contém esta tarefa.' .
				'A tarefa ' . $this->getCodigoUnico() . ' - ' . $this->getNome() . ' não pode ter a sua data atualizada '.
				'pois a data da mesma é fechada.' .
				'';			
		}
		
		/**
		 * Mensagem de erro quando uma tarefa container desta de data fixa precisa ter sua data atualizada.
		 *
		 * @return string
		 */
		protected function getMensagemErroTarefaContainer()
		{
			return	'Não foi possível alterar as tarefas contidas nesta. ' . "\n" .
				'A tarefa ' .$this->getCodigoUnico() . ' - ' . $this->getNome() . ' não pode ter a sua data atualizada pois ' .
				'a data da mesma é fechada.' .
				'';			
		}
		
		/**
		 * Mensagem de erro quando uma tarefa tenta ser predecessora dela mesma.
		 *
		 * @return string
		 */
		protected function getMensagemErroPropriaPredecessora()
		{
			return	'A tarefa ' . $this->getCodigoUnico() . ' - ' . $this->getNome(). ' não pode ser predecessora de si mesma. '.
				' Não foi possível alterar as tarefas contidas nesta.' .
				'';			
		}
		
		/**
		 * Mensagem de erro quando uma tarefa tenta ser sucessora dela mesma.
		 *
		 * @return string
		 */
		protected function getMensagemErroPropriaSucessora()
		{
			return	'A tarefa ' . $this->getCodigoUnico() . ' - ' . $this->getNome() . ' não pode ser sucessora de si mesma. '.
				'';			
		}
		
		/**
		 * Mensagem de erro quando uma tarefa tenta ser predecessora e contida por outra tarefa.
		 *
		 * @return string
		 */
		protected function getMensagemErroPredecessoraEContida()
		{
			return	'A tarefa ' . $this->getCodigoUnico() . ' - ' . $this->getNome() . ' não pode ser predecessora e contida ' . 
				'pela mesma tarefa. '.
				'';			
		}
		
		/**
		 * Mensagem de erro quando uma tarefa tenta ser sucessora e contida por outra tarefa.
		 *
		 * @return string
		 */
		protected function getMensagemErroSucessoraEContida()
		{
			return	'A tarefa ' . $this->getCodigoUnico() . ' - ' . $this->getNome() . ' não pode ser sucessora e contida ' . 
				'pela mesma tarefa. '.
				'';
		}
		
		/**
		 * Mensagem de erro quando uma tarefa tenta ser predecessora e container de outra tarefa.
		 *
		 * @return string
		 */
		protected function getMensagemErroPredecessoraEContainer()
		{
			return	'A tarefa ' . $this->getCodigoUnico() . ' - ' . $this->getNome() . ' não pode ser predecessora e conter ' . 
				'uma mesma tarefa. '.
				'';			
		}
		
		/**
		 * Mensagem de erro quando uma tarefa tenta ser sucessora e container de outra tarefa.
		 *
		 * @return string
		 */
		protected function getMensagemErroSucessoraEContainer()
		{
			return	'A tarefa ' . $$this->getCodigoUnico() . ' - ' . $this->getNome() . ' não pode ser sucessora e conter ' . 
				'uma mesma tarefa. '.
				'';
		}
		
		/**
		 * Mensagem de erro quando uma tarefa tenta conter uma tarefa que a contém.
		 *
		 * @return string
		 */
		protected function getMensagemErroContainerEContida()
		{
			return	'A tarefa ' . $this->getCodigoUnico() . ' - ' . $this->getNome(). ' não pode conter uma tarefa que ' . 
				'já a contém. ' . 
				'';
		}
		
		/**
		 * Mensagem de erro quando uma tarefa tenta conter a si mesma.
		 *
		 * @return string
		 */
		protected function getMensagemErroPropriaContainer()
		{
			return	'A tarefa ' . $this->getCodigoUnico() . ' - ' . $this->getNome() . ' não pode conter a si mesma. ' . 
				'';
		}
		
		/**
		 * Mensagem de erro quando uma tarefa tenta ter um código único já cadastrado.
		 *
		 * @return string
		 */
		protected function getMensagemErroCodigoUnico()
		{
			return ' Este código único já está em uso pela tarefa ' . $this->getCodigoUnico() . ' - ' . $this->getNome() . '.' .
				'';
		}
		
		/**
		 * Mensagem de erro quando uma tarefa container se relacionada com uma de suas
		 * tarefas contidas diretas ou indiretas ou com as suas respectivas sucessoras 
		 * e precursoras diretas ou indiretas.
		 *
		 * @return string
		 */
		protected function getMensagemErroContainerRelacionada()
		{
			return ' A tarefa ' . $this->getContainer()->getNome() . ' já possui um relacionamento '.
				'indireto com alguma das tarefas contidas em ' . $this->getNome() . ' não podendo, ' .
				' deste modo, se tornar a tarefa que a contenha. ' .
				'';
		}
		
		/**
		 * Após alterar as novas datas de uma tarefa verifica se existe algum erro
		 * no processo em cascata da alteração.
		 * 
		 * 1. As alterações das datas de inicio e fim devem ser
		 * efetivadas com sucesso e em cascata na tarefa que contém
		 * esta tarefa.
		 * 
		 * 2. As alterações das datas de inicio e fim devem ser
		 * efetivadas com sucesso e em cascata nas tarefas contidas
		 * por esta tarefas.
		 * 
		 * 3. As alterações da data de término devem ser efetivadas
		 * com sucesso e em cascata nas tarefas sucessoras.
		 * 
		 * 4. As alterações da data de inicio devem ser efetivadas
		 * com sucesso e em cascata nas tarefas predecessoras.
		 * 
		 * @see valida
		 * @see setDatasAlteradas
		 */
		public function verificaPossibilidadeDeAlterar( $intNovaDataInicio, $intNovaDataFim )
		{
			
//			$intNovaDatadeTerminoDosPredecessorasDeste 	= $intNovaDataInicio - $this->TIMESTAMP_DAY;
//			$intNovaDatadeInicioDosSucessorasDeste 		= $intNovaDataFim 	 + $this->TIMESTAMP_DAY;
			$intNovaDatadeTerminoDosPredecessorasDeste 	= $intNovaDataInicio;
			$intNovaDatadeInicioDosSucessorasDeste 		= $intNovaDataFim;
			
			if( !$this->getDatasAlteradas() )
			{
				if	(
						( $this->getDataFimTimestamp() !== $intNovaDataFim )
					||
						( $this->getDataInicioTimestamp() !== $intNovaDataInicio )
					)
				{
					$this->setNovaDataInicio( date( 'd/m/Y' , $intNovaDataInicio ) );
					$this->setNovaDataFim( date( 'd/m/Y' , $intNovaDataFim ) );
					$this->setDatasAlteradas( true );
					
					# 1. As alterações das datas de inicio e fim devem ser		#
					# efetivadas com sucesso e em cascata na tarefa que contém	#
					# esta tarefa.												#
				 	$this->verificaPossibilidadeDeAlterarAsQueMeContem( $intNovaDataInicio , $intNovaDataFim );
					
				 	# 2. As alterações das datas de inicio e fim devem ser 		#
					# efetivadas com sucesso e em cascata nas tarefas contidas	#
					# por esta tarefas.											#
					$this->verificaPossibilidadeDeAlterarAsQueContenho( $intNovaDataInicio , $intNovaDataFim );
				 	
					# 3. As alterações da data de término devem ser efetivadas	#
				 	# com sucesso e em cascata nas tarefas sucessoras.			#
					$this->verificaPossibilidadeDeAlterarSucessoras(	$intNovaDatadeInicioDosSucessorasDeste );
					
					# 4. As alterações da data de inicio devem ser efetivadas	#
					# com sucesso e em cascata nas tarefas predecessoras.		#
					$this->verificaPossibilidadeDeAlterarPredecessoras(	$intNovaDatadeTerminoDosPredecessorasDeste );
					
				}
			}
			else if	(
						( $this->getNovaDataInicioTimestamp() !== $intNovaDataInicio )
					||
						( $this->getNovaDataFimTimestamp() !== $intNovaDataFim )
					)
			{
				$strErrorMessage = $this->getMensagemdeErroTarefaNaoComporta();
				throw new Exception( $strErrorMessage . ' 1 ' );
			}
		}
		
		/**
		 * Verifica a possibilidade de alterar as tarefas sucessoras.
		 * 
		 * Verifica a possibilidade de alterar as tarefas sucessoras atualizando 
		 * suas datas e mantendo a consistencia dos dados. Recebe a nova data de
		 * inico desta.
		 * 
		 * @param integer $intNovaDatadeInicioDosSucessoras
		 */
		protected function verificaPossibilidadeDeAlterarSucessoras( $intNovaDatadeInicioDosSucessoras )
		{
			$arrSucessoras = $this->getArraydeSucessoras();
			foreach( $arrSucessoras as $objTarefaSucessora )
			{
				if( $this->getContainer() !== $objTarefaSucessora->getContainer() )
				{
					$objContainer = $objTarefaSucessora->getContainer();
					if( $objContainer !== NULL )
					{
						$objContainer->verificaPossibilidadeDeAlterar( $intNovaDatadeInicioDosSucessoras , $intNovaDatadeInicioDosSucessoras + $objContainer->getDuracaoTimestamp() );
					}
					else
					{
						$objTarefaSucessora->verificaPossibilidadeDeAlterar( $intNovaDatadeInicioDosSucessoras , $intNovaDatadeInicioDosSucessoras + $objTarefaSucessora->getDuracaoTimestamp() );
					}
				}
				else
				{
				
					$intDataInicio	= $objTarefaSucessora->getDataInicioTimestamp();
					$intDataFim		= $objTarefaSucessora->getDataFimTimestamp();
					$intDuracao		= $intDataFim - $intDataInicio;
					if( $intNovaDatadeInicioDosSucessoras > $intDataInicio )
					{
						if( $objTarefaSucessora->getDataFechada() == false || $objTarefaSucessora->getDatasAlteradas() )
						{
							// as datas desta tarefa podem ser alteradas	//
							// gerando as novas datas da tarefa				//
							
							$intNovaDataInicio	= $intNovaDatadeInicioDosSucessoras;
							$intNovaDataFim		= $intNovaDatadeInicioDosSucessoras + $intDuracao;
							
							// se a data ja houver sido alterada anteriormente //
							if( $objTarefaSucessora->getDatasAlteradas() )
							{
								// se a tarefa com os dados novos ainda necessita de alteracao //
								if	( $intNovaDatadeInicioDosSucessoras > $objTarefaSucessora->getNovaDataInicioTimestamp() )
								{
									$strErrorMessage = $objTarefaSucessora->getMensagemdeErroTarefaNaoComporta();
									throw new Exception( $strErrorMessage . ' 2 ' );
								}
							}
							else
							{
								try
								{
									// gerando o efeito em cascata de checagem //
									$objTarefaSucessora->verificaPossibilidadeDeAlterar( $intNovaDataInicio , $intNovaDataFim );
								}
								catch( Exception $ErrorObject )
								{
									// emerge o erro de algum das tarefas filhas que nao aceitou a alteracao //
									
									throw $ErrorObject;
								}
							}
						}
						else
						{
							// esta tarefa nao aceita a alteracao pois sua data é fechada //
							
							$strErrorMessage = $objTarefaSucessora->getMensagemdeErroSucessorasDataFixa();
							throw new Exception( $strErrorMessage );
						}
					}
				}
			}
		}
		
		/**
		 * Verifica a possibilidade de alterar as tarefas predecessoras.
		 * 
		 * Verifica a possibilidade de alterar as tarefas predecessoras atualizando 
		 * suas datas e mantendo a consistencia dos dados. Recebe a nova data de termino
		 * desta.
		 * 
		 * @param integer $intNovaDatadeTerminoDosPredecessoras 
		 */
		protected function verificaPossibilidadeDeAlterarPredecessoras( $intNovaDatadeTerminoDosPredecessoras )
		{
			$arrPredecessoras = $this->getArraydePredecessoras();
			foreach( $arrPredecessoras as $objTarefaPredecessora )
			{
				$intDataInicio	= $objTarefaPredecessora->getDataInicioTimestamp();
				$intDataFim		= $objTarefaPredecessora->getDataFimTimestamp();
				$intDuracao		= $intDataFim - $intDataInicio;
				if( $intNovaDatadeTerminoDosPredecessoras < $intDataFim )
				{
					if( $objTarefaPredecessora->getDataFechada() == false || $objTarefaPredecessora->getDatasAlteradas() )
					{
						$intNovaDataFim		= $intNovaDatadeTerminoDosPredecessoras;
						$intNovaDataInicio	= $intNovaDatadeTerminoDosPredecessoras - $intDuracao;
						
						if( $objTarefaPredecessora->getDatasAlteradas() )
						{
							if	( $intNovaDatadeTerminoDosPredecessoras < $objTarefaPredecessora->getNovaDataFimTimestamp() )
							{
								$strErrorMessage = $objTarefaPredecessora->getMensagemdeErroTarefaNaoComporta();
								throw new Exception( $strErrorMessage . ' 3 ' );
							}
						}
						else
						{
							try
							{
								// gerando o efeito em cascata de checagem //
								$objTarefaPredecessora->verificaPossibilidadeDeAlterar( $intNovaDataInicio , $intNovaDataFim );
							}
							catch( Exception $ErrorObject )
							{
								throw $ErrorObject;
							}
						}
					}
					else
					{
						$strErrorMessage = $objTarefaPredecessora->getMensagemdeErroPredecessorasDataFixa();
						throw new Exception( $strErrorMessage );
					}
				}
			}
		}
		
		/**
		 * Verifica a possibilidade de alterar as tarefas que esta contém
		 * no modo de arrastar grupo.
		 * 
		 * Verifica a possibilidade de alterar as tarefas que esta contém atualizando 
		 * suas datas e mantendo a consistencia dos dados como também a distancia
		 * entre as tarefas. Recebe as novas datas de inico e termino desta.
		 * 
		 * @param integer $intContainerDataInicio 
		 * @param integer $intContainerDataFim
		 */
		public function verificaPossibilidadeDeArrastasAsQueContenho()
		{
			if( $this->getNovaDataFim() == $this->getDataFim() && $this->getNovaDataInicio() == $this->getDataInicio() )
			{
				return;
			}
			
			$intDataInicialOriginal		= $this->getDataInicioTimestamp();
			$intDataFinalOriginal		= $this->getDataFimTimestamp();
			if( $this->getNovaDataInicio() !== $this->getDataInicio() )
			{
				$intDataInicialNova	= $this->getNovaDataInicioTimestamp();
			}
			else
			{
				$intDataInicialNova = $intDataInicialOriginal;
			}
			
			if( $this->getNovaDataFim() !== $this->getDataFim() )
			{
				$intDataFinalNova	= $this->getNovaDataFimTimestamp();
			}
			else
			{
				$intDataFinalNova = $intDataFinalOriginal;
			}
			
			$arrTarefasQueContenho		= $this->getArraydeTarefasqueContenho();
			$arrTarefasQueContenhoAsc	=  ( orderArrayOfObjectsByMethod( $arrTarefasQueContenho , 'getDataInicioTimestamp' ) );
			$arrTarefasQueContenhoDesc	= array_reverse( $arrTarefasQueContenhoAsc );
			
			if( $intDataInicialNova !== $intDataInicialOriginal )
			{
				foreach( $arrTarefasQueContenhoAsc as $objTarefaQueContenho )
				{
					$intTarefaDataInicial = $objTarefaQueContenho->getDataInicioTimestamp();
					$intDistancia = $intTarefaDataInicial - $intDataInicialOriginal;
					$intDuracao	= $objTarefaQueContenho->getDuracaoTimestamp();
					$intTarefaNovaDataInicial = $intDataInicialNova + $intDistancia;
					$intTarefaNovaDataFinal = $intTarefaNovaDataInicial + $intDuracao;
					$objTarefaQueContenho->setNovaDataInicio( 	date('d/m/Y' , $intTarefaNovaDataInicial ) );
					$objTarefaQueContenho->setNovaDataFim( 		date('d/m/Y' , $intTarefaNovaDataFinal ) );
					$objTarefaQueContenho->setDatasAlteradas( true );
					$objTarefaQueContenho->verificaPossibilidadeDeArrastasAsQueContenho();
				}
			}
			else if( $intDataFinalOriginal !== $intDataFinalNova )
			{
				foreach( $arrTarefasQueContenhoDesc as $objTarefaQueContenho )
				{
					$strClass = get_class( $this );
					$objTarefaQueContenho = self::criaTarefa();
					$intTarefaDataTermino = $objTarefaQueContenho->getDataFimTimestamp();
					$intDistancia = $intDataFinalOriginal - $intTarefaDataTermino;
					$intDuracao	= $objTarefaQueContenho->getDuracaoTimestamp();
					$intTarefaNovaDataFinal = $intDataFinalNova - $intDistancia;
					$intTarefaNovaDataInicial = $intTarefaNovaDataFinal + $intDuracao;
					$objTarefaQueContenho->setNovaDataInicio( date('d/m/Y' , $intTarefaNovaDataInicial ) );
					$objTarefaQueContenho->setNovaDataFim( date('d/m/Y' , $intTarefaNovaDataFinal ) );
					$objTarefaQueContenho->setDatasAlteradas( true );
					$objTarefaQueContenho->verificaPossibilidadeDeArrastasAsQueContenho();
				}
			}
		}
		
		/**
		 * Verifica a possibilidade de alterar as tarefas que esta contém.
		 * 
		 * Verifica a possibilidade de alterar as tarefas que esta contém atualizando 
		 * suas datas e mantendo a consistencia dos dados. Recebe as novas datas
		 * de inico e termino desta.
		 * 
		 * @param integer $intContainerDataInicio 
		 * @param integer $intContainerDataFim
		 */
		protected function verificaPossibilidadeDeAlterarAsQueContenho( $intContainerDataInicio , $intContainerDataFim )
		{
			$arrTarefasQueContenho = $this->getArraydeTarefasqueContenho();
			$arrTarefasQueContenhoAsc =  ( orderArrayOfObjectsByMethod( $arrTarefasQueContenho , 'getDataInicioTimestamp' ) );
			$arrTarefasQueContenhoDesc = array_reverse( $arrTarefasQueContenhoAsc );
			
			foreach( $arrTarefasQueContenhoAsc as $objTarefaQueContenho )
			{
//				$intDataInicio	= $objTarefaQueContenho->getDataInicioTimestamp();
//				$intDataFim		= $objTarefaQueContenho->getDataFimTimestamp();
//				$intDuracao		= $intDataFim - $intDataInicio;
				$intDataInicio	= $objTarefaQueContenho->getNovaDataInicioTimestamp();
				$intDataFim		= $objTarefaQueContenho->getNovaDataFimTimestamp();
				$intDuracao		= $intDataFim - $intDataInicio;
				
				// se a tarefa que contenho escapa pelo limite de data de inicio //
				if( $intContainerDataInicio > $intDataInicio )
				{
					// se existe a possibilidade de se alterar a tarefa que contenho //
					if( $objTarefaQueContenho->getDataFechada() == false || $objTarefaQueContenho->getDatasAlteradas() )
					{
						$intNovaDataInicio	= $intContainerDataInicio;
						$intNovaDataFim		= $intContainerDataInicio + $intDuracao;
						// se ja foi anteriormente alterada //
						if( $objTarefaQueContenho->getDatasAlteradas() )
						{
							// se a alteracao eh diferente da que esta agora sendo feita //
							if	( $intContainerDataInicio > $objTarefaQueContenho->getNovaDataInicioTimestamp() )
							{
								if( $this->boolPeridoFechado )
								{
									// erro de alteracoes simultaneas //
									$strErrorMessage = $objTarefaQueContenho->getMensagemdeErroTarefaNaoComporta();
									throw new Exception( $strErrorMessage . ' 4 ' . $objTarefaQueContenho->getNovaDataInicio() . ' : ' . date( 'd/m/Y' , $intContainerDataInicio ) );
								}
								else
								{
									$this->setNovaDataInicio(	date( 'd/m/Y' , min( $intContainerDataInicio	, $this->getNovaDataInicioTimestamp()	, $this->getDataInicioTimestamp() ) ) );
									$this->setNovaDataFim(		date( 'd/m/Y' , max( $intContainerDataFim		, $this->getNovaDataFimTimestamp()		, $this->getDataFimTimestamp() ) ) );
									// expandindo o periodo da tarefa container //
									//throw new Exception( 'Debugando ' . $this->getCodigoEstruturado()  . ' : ' .$objTarefaQueContenho->getCodigoEstruturado() ); 
								}
							}
						}
						// se nao foi alterada anteriormente //
						else
						{
							try
							{
								// gerando o efeito em cascata de checagem //
								$objTarefaQueContenho->verificaPossibilidadeDeAlterar( $intNovaDataInicio , $intNovaDataFim );
							}
							catch( Exception $ErrorObject )
							{
								// emergindo o erro de alguma das tarefas filhas //
								throw $ErrorObject;
							}
						}
					}
					else
					{
						// a tarefa nao pode ser alterada por se de data fechada //
						$strErrorMessage = $objTarefaQueContenho->getMensagemErroTarefaContainer();
						throw new Exception( $strErrorMessage );
					}
				}
			}
			foreach( $arrTarefasQueContenhoDesc as $objTarefaQueContenho )
			{
				$intDataInicio	= $objTarefaQueContenho->getDataInicioTimestamp();
				$intDataFim		= $objTarefaQueContenho->getDataFimTimestamp();
				$intDuracao		= $intDataFim - $intDataInicio;
				
				// se a tarefa que contenho escapa pelo limite de data de termino //
				if( $intContainerDataFim < $intDataFim )
				{
					// se existe a possibilidade de se alterar a tarefa que contenho //
					if( $objTarefaQueContenho->getDataFechada() == false || $objTarefaQueContenho->getDatasAlteradas() )
					{
						$intNovaDataFim		= $intContainerDataFim ;
						$intNovaDataInicio	= $intContainerDataFim - $intDuracao;
						
						// se ja foi anteriormente alterada //
						if( $objTarefaQueContenho->getDatasAlteradas() )
						{
							// se a alteracao eh diferente da que esta agora sendo feita //
							if	(  $intContainerDataFim < $objTarefaQueContenho->getNovaDataFimTimestamp() )
							{
								if( $this->boolPeridoFechado )
								{
									// erro de alteracoes simultaneas //
									$strErrorMessage = $objTarefaQueContenho->getMensagemdeErroTarefaNaoComporta();
									throw new Exception( $strErrorMessage . ' 5 ' . $objTarefaQueContenho->getNovaDataInicio() . ' : ' . date( 'd/m/Y' , $intContainerDataInicio ) );
								}
								else
								{
									$this->setNovaDataInicio(	date( 'd/m/Y' , min( $intContainerDataInicio	, $this->getNovaDataInicioTimestamp()	, $this->getDataInicioTimestamp() ) ) );
									$this->setNovaDataFim(		date( 'd/m/Y' , max( $intContainerDataFim		, $this->getNovaDataFimTimestamp()		, $this->getDataFimTimestamp() ) ) );
								}
								// erro de alteracoes simultaneas //
								$strErrorMessage = $objTarefaQueContenho->getMensagemdeErroTarefaNaoComporta();
							}
						}
						else
						{
							try
							{
								// gerando o efeito em cascata de checagem //
								$objTarefaQueContenho->verificaPossibilidadeDeAlterar( $intNovaDataInicio , $intNovaDataFim );
							}
							catch( Exception $ErrorObject )
							{
								// emergindo o erro de alguma das tarefas filhas //
								throw $ErrorObject;
							}
						}
					}
					else
					{
						// a tarefa nao pode ser alterada por se de data fechada //
						$strErrorMessage = $objTarefaQueContenho->getMensagemErroTarefaContainer();
						throw new Exception( $strErrorMessage );
					}
				}
			}
		}
		
		/**
		 * Verifica a possibilidade de alterar as tarefas que contém esta tarefa.
		 * 
		 * Verifica a possibilidade de alterar as tarefas que contém esta tarefa atualizando 
		 * suas datas e mantendo a consistencia dos dados. Recebe a nova data de inicio e termino
		 * desta tarefa.
		 * 
		 * @param integer $intContidaDataInicio 
		 * @param integer $intContidaDataFim
		 */
		protected function verificaPossibilidadeDeAlterarAsQueMeContem( $intContidaDataInicio , $intContidaDataFim )
		{
			$arrTarefasQueMeContem = $this->getArraydeTarefasqueMeContem();
			
			if( sizeof( $arrTarefasQueMeContem ) > 0 )
			{
				// esta tarefa é contida por outras //
				foreach( $arrTarefasQueMeContem as $objTarefaQueMeContem )
				{
					$intDataInicio	= $objTarefaQueMeContem->getDataInicioTimestamp();
					$intDataFim		= $objTarefaQueMeContem->getDataFimTimestamp();
					$intDuracao		= $intDataFim - $intDataInicio;
					
					// se a tarefa que me contem escapa pelo limite de data de inicio //
					if( $intContidaDataInicio < $intDataInicio )
					{
						// se existe a possibilidade de se alterar a tarefa que contenho //
						if( $objTarefaQueMeContem->getDataFechada() == false || $objTarefaQueMeContem->getDatasAlteradas() )
						{
							$intNovaDataInicio	= $intContidaDataInicio;
							$intNovaDataFim		= $intContidaDataInicio + $intDuracao;
							
							// se ja foi anteriormente alterada //
							if( $objTarefaQueMeContem->getDatasAlteradas() )
							{
								// se mesmo com os dados novos ainda gera erro //
								if	(  $intContidaDataInicio < $objTarefaQueMeContem->getNovaDataInicioTimestamp() )
								{
									// erro de alteracoes simultaneas //
									$strErrorMessage = $objTarefaQueMeContem->getMensagemdeErroTarefaNaoComporta();
									throw new Exception( $strErrorMessage . ' 6 ' );
								}
							}
							// se nao foi alterada anteriormente //
							else
							{
								try
								{
									// gerando o efeito em cascata de checagem //
									$objTarefaQueMeContem->verificaPossibilidadeDeAlterar( $intNovaDataInicio , $intNovaDataFim );
								}
								catch( Exception $ErrorObject )
								{
									// emergindo o erro de alguma das tarefas filhas //
									throw $ErrorObject;
								}
							}
						}
						else
						{
							// a tarefa nao pode ser alterada por se de data fechada //
							$strErrorMessage = $objTarefaQueMeContem->getMensagemErroTarefaContida();
							throw new Exception( $strErrorMessage );
						}
					}
					
					// se a tarefa que contenho escapa pelo limite de data de termino //
					else if( $intContidaDataFim > $intDataFim )
					{
						// se existe a possibilidade de se alterar a tarefa que contenho //
						if( $objTarefaQueMeContem->getDataFechada() == false || $objTarefaQueMeContem->getDatasAlteradas() )
						{
							$intNovaDataFim		= $intContidaDataFim ;
							$intNovaDataInicio	= $intContidaDataFim - $intDuracao;
							
							// se ja foi anteriormente alterada //
							if( $objTarefaQueMeContem->getDatasAlteradas() )
							{
								// se mesmo com os dados novos ainda gera erro //
								if	(  $intContidaDataFim > $this->getNovaDataFimTimestamp() )
								{
									// erro de alteracoes simultaneas //
									$strErrorMessage = $objTarefaQueMeContem->getMensagemdeErroTarefaNaoComporta();
									throw new Exception( $strErrorMessage . ' 7 ' );
								}
							}
							else
							{
								try
								{
									// gerando o efeito em cascata de checagem //
									$objTarefaQueMeContem->verificaPossibilidadeDeAlterar( $intNovaDataInicio , $intNovaDataFim );
								}
								catch( Exception $ErrorObject )
								{
									// emergindo o erro de alguma das tarefas filhas //
									throw $ErrorObject;
								}
							}
						}
						else
						{
							// a tarefa nao pode ser alterada por se de data fechada //
							$strErrorMessage = $objTarefaQueMeContem->getMensagemErroTarefaContida();
							throw new Exception( $strErrorMessage );
						}
					}
				}
			}
			else
			{
				// essa tarefa nao é contida por outras sendo assim uma tarefa raiz //
				$this->verificaPossibilidadeDeAlterarPeloProjeto( $intContidaDataInicio , $intContidaDataFim );				
			}
		}

		protected function verificaPossibilidadeDeAlterarPeloProjeto( $intContidaDataInicio , $intContidaDataFim )
		{
			// para otimizar o processo serão checadas apenas as tarefas que não estejam contidas 	//
			// em outras pois as tarefas que contém as outras já extendem suas datas para 			//
			// abrange-las, tornando as checagens das tarefas contidas uma redundancia.				//

			if( $this->getContainerId() !== null )
			{
				return;
			}
			
			$objProjeto		= $this->getProjeto();
			
			$intDataInicio	= $objProjeto->getDataInicioTimestamp();
			$intDataFim		= $objProjeto->getDataFimTimestamp();
			$intDuracao		= $objProjeto->getDuracaoTimestamp();
			
			// se a tarefa que me contem escapa pelo limite de data de inicio //
			if	(
					( $intContidaDataInicio < $intDataInicio )
					||
					( $intContidaDataInicio > $intDataFim )
				)
			{
				// se existe a possibilidade de se alterar a tarefa que contenho //
				if( $objProjeto->getDataFechada() == false || $objProjeto->getDatasAlteradas() )
				{
					$intNovaDataInicio	= $intContidaDataInicio;
					$intNovaDataFim		= $intContidaDataInicio + $intDuracao;
					
					// se ja foi anteriormente alterada //
					if( $objProjeto->getDatasAlteradas() )
					{
						// se mesmo com os dados novos ainda gera erro //
						if	(
								(  $intContidaDataInicio < $objProjeto->getNovaDataInicioTimestamp() )
								||
								( $intContidaDataInicio > $objProjeto->getNovaDataFimTimestamp() )
							)
						{
							// erro de alteracoes simultaneas //
							$strErrorMessage = $objProjeto->getMensagemdeErroProjetoNaoComporta();
							throw new Exception( $strErrorMessage );
						}
					}
					// se nao foi alterada anteriormente //
					else
					{
						try
						{
							$this->verificaPossibilidadeDeAlterar( $intNovaDataInicio , $intNovaDataFim );
							// gerando o efeito em cascata de checagem //
							// $objProjeto->verificaPossibilidadeDeAlterar( $intNovaDataInicio , $intNovaDataFim );
						}
						catch( Exception $ErrorObject )
						{
							// emergindo o erro de alguma das tarefas filhas //
							throw $ErrorObject;
						}
					}
				}
				else
				{
					$objProjeto->setNovaDataInicioTimestamp( $intContidaDataInicio );
					$objProjeto->setNovaDataFimTimestamp( $intContidaDataFim );
					// a tarefa nao pode ser alterada por se de data fechada //
					$strErrorMessage = $objProjeto->getMensagemErroProjetoFechado();
					throw new Exception( $strErrorMessage );
				}
			}
			
			// se a tarefa que contenho escapa pelo limite de data de termino	//
			// ou																//
			// se a tarefa que contenho es
			else if
			(
				( $intContidaDataFim > $intDataFim )
				||
				( $intContidaDataFim < $intDataInicio )
			)
			{
				// se existe a possibilidade de se alterar a tarefa que contenho //
				if( $objProjeto->getDataFechada() == false || $objProjeto->getDatasAlteradas() )
				{
					$intNovaDataFim		= $intContidaDataFim ;
					$intNovaDataInicio	= $intContidaDataFim - $intDuracao;
					
					// se ja foi anteriormente alterada //
					if( $objProjeto->getDatasAlteradas() )
					{
						// se mesmo com os dados novos ainda gera erro //
						if	(  $intContidaDataFim > $this->getNovaDataFimTimestamp() )
						{
							// erro de alteracoes simultaneas //
							$strErrorMessage = $objProjeto->getMensagemdeErroProjetoNaoComporta();
							throw new Exception( $strErrorMessage );
						}
					}
					else
					{
						try
						{
							// gerando o efeito em cascata de checagem //
							$objProjeto->verificaPossibilidadeDeAlterar( $intNovaDataInicio , $intNovaDataFim );
						}
						catch( Exception $ErrorObject )
						{
							// emergindo o erro de alguma das tarefas filhas //
							throw $ErrorObject;
						}
					}
				}
				else
				{
					$objProjeto->setNovaDataInicioTimestamp( $intContidaDataInicio );
					$objProjeto->setNovaDataFimTimestamp( $intContidaDataFim );
					// a tarefa nao pode ser alterada por se de data fechada //
					$strErrorMessage = $objProjeto->getMensagemErroProjetoFechado();
					throw new Exception( $strErrorMessage );
				}
			}
		}
		
		/**
		 * Faz as alterações das sucessoras em cascata
		 * enquanto for necessário
		 *
		 * @return bool
		 */
		protected function alteraSucessoras()
		{
			$boolAlterada = $this->getDatasAlteradas();
			if( $boolAlterada )	
			{
				$arrSucessoras = $this->getArraydeSucessoras();
				foreach( $arrSucessoras as $objTarefaSucessora )
				{
					$boolChildAlterada = $objTarefaSucessora->alteraSucessoras();
					if( $boolChildAlterada )
					{
						$objTarefaSucessora->atualizarTarefa();
					}
				}
			}
			return $boolAlterada;
		}
		
		/**
		 * Faz as alterações das predecessoras em cascata
		 * enquanto for necessário
		 *
		 * @return bool
		 */
		protected function alteraPredecessoras()
		{
			$boolAlterada = $this->getDatasAlteradas();
										
			if( $boolAlterada )	
			{
				$arrPredecessoras = $this->getArraydePredecessoras();
				foreach( $arrPredecessoras as $objTarefaPredecessora )
				{
					$boolChildAlterada = $objTarefaPredecessora->alteraPredecessoras();
					if( $boolChildAlterada )
					{
						$objTarefaSucessora->atualizarTarefa();
					}
				}
			}
			return $boolAlterada;
		}
		
		/**
		 * Retorna o array de todas as tarefas sucessoras, sejam diretas
		 * ou indiretas
		 * 
		 * @return Array of Tarefa
		 */
		protected function getArrSucessorasDiretaseIndiretas()
		{
			if( $this->arrTarefasSucessorasDiretaseIndiretas === null )
			{
				$arrResult = array();
				$arrSucessoras = $this->getArraydeSucessoras();
				foreach( $arrSucessoras as $objTarefaSucessora )
				{
					$arrResult = array_merge( $arrResult , $objTarefaSucessora->getArrSucessorasDiretaseIndiretas() );
					$arrResult[] = $objTarefaSucessora;
				}
				$this->arrTarefasSucessorasDiretaseIndiretas = $arrResult;
			}
			return $this->arrTarefasSucessorasDiretaseIndiretas;
		}
		
		/**
		 * Retorna o array de todas as tarefas predecessoras, sejam diretas
		 * ou indiretas
		 * 
		 * @return Array of Tarefa
		 */
		protected function getArrPredecessorasDiretaseIndiretas( $arrAlreadyRead = array() )
		{
			if( $this->arrTarefasPredecessorasDiretaseIndiretas === null )
			{
				$this->arrTarefasPredecessorasDiretaseIndiretas = array();
				$arrResult = array();
				$arrPredecessoras = $this->getArraydePredecessoras();
				foreach( $arrPredecessoras as $objTarefaPredecessora )
				{
					$arrResult = array_merge( $arrResult , $objTarefaPredecessora->getArrPredecessorasDiretaseIndiretas() );
					$arrResult[] = $objTarefaPredecessora;
				}
				$this->arrTarefasPredecessorasDiretaseIndiretas = $arrResult;
			}
			return $this->arrTarefasPredecessorasDiretaseIndiretas;
		}
		
		/**
		 * Retorna o array de todas as tarefas que esta tarefa contem, seja direta
		 * ou indiretamente
		 * 
		 * @return Array of Tarefa
		 */
		protected function getArrQueContenhoDiretaseIndiretas()
		{
			if( $this->arrTarefasQueContenhoDiretaseIndiretas === null )
			{
				$arrResult = array();
				$arrContidas = $this->getArraydeTarefasqueContenho();
				foreach( $arrContidas as $objTarefaContida )
				{
					$arrResult = array_merge( $arrResult , $objTarefaContida->getArrQueContenhoDiretaseIndiretas() );
					$arrResult[] = $objTarefaContida;
				}
				$this->arrTarefasQueContenhoDiretaseIndiretas = $arrResult;
			}
			return $this->arrTarefasQueContenhoDiretaseIndiretas  ;
		}
		
		/**
		 * Retorna o array de todas as tarefas que esta tarefa contem , 
		 * seja direta ou indiretamente e suas sucessoras e predecessoras .
		 * 
		 * @return Array of Tarefa
		 */
		protected function getArrTarefasContidaseRelacionadas()
		{
			if( $this->arrTarefasQueContenhoeRelacionadas  === null )
			{
				$arrResult = array();
				$arrContidas = $this->getArraydeTarefasqueContenho();
				foreach( $arrContidas as $objTarefaContida )
				{
					$arrResult = array_merge( $arrResult , $objTarefaContida->getArrTarefasContidaseRelacionadas() );
					$arrResult = array_merge( $arrResult , $objTarefaContida->getArrPredecessorasDiretaseIndiretas() );
					$arrResult = array_merge( $arrResult , $objTarefaContida->getArrSucessorasDiretaseIndiretas() );
					$arrResult[] = $objTarefaContida;
				}
				$this->arrTarefasQueContenhoeRelacionadas = $arrResult;
			}
			return array_unique_object( $this->arrTarefasQueContenhoeRelacionadas );
		}
		
		/**
		 * Verifica se a Tarefa está valida e apta para ser salva.
		 * 
		 * 1. O código único deve ser único no projeto.
		 * 
		 * 2. Nenhuma predecessora pode ser uma das sucessoras 
		 * ou vice versa.
		 * 	2.1. Nenhuma predecessora pode ser uma das sucessoras.
		 * 	2.2. A tarefa não pode ser sucessora de si mesma.
		 * 	2.3. Nenhuma sucessora pode ser uma das predecessoras.
		 * 	2.4. A tarefa não pode ser predecessora de si mesma.
		 * 
		 * 3. Uma tarefa não pode ficar fora dos limites do projeto,
		 * tentando altera-los caso isso ocorra. Caso uma tarefa 
		 * tente alterar as datas do projeto, este não pode ser de 
		 * data fixa.
		 * 
		 * 4. Nenhuma tarefa contida ou container pode ser 
		 * 		sucessora ou predecessora desta.
		 * 		4.1 Nenhuma tarefa pode ser contida e predecessora
		 * 		4.2 Nenhuma tarefa pode ser contida e sucessora
		 * 		4.3 Nenhuma tarefa pode ser container e predecessora
		 * 		4.4 Nenhuma tarefa pode ser container e sucessora
		 * 
		 * 5. Nenhuma tarefa contida pode conter esta tarefa.
		 * 		5.1 Nenhuma tarefa pode ser contida e container
		 * 		5.2 Nenhuma tarefa pode conter a si mesma
		 * 
		 * 6. Nenhuma tarefa container pode se relacionar como 
		 * sucessao ou precursao com as tarefas direta ou 
		 * indiretamente contidas
		 * 
		 * 7. As alterações das datas de inicio e fim devem ser
		 * efetivadas com sucesso e em cascata na tarefa que contém
		 * esta tarefa.
		 * 
		 * 8. As alterações das datas de inicio e fim devem ser 
		 * efetivadas com sucesso e em cascata nas tarefas contidas
		 * por esta tarefas.
		 * 
		 * 9. As alterações da data de término devem ser efetivadas
		 * com sucesso e em cascata nas tarefas sucessoras.
		 * 
		 * 10. As alterações da data de inicio devem ser efetivadas
		 * com sucesso e em cascata nas tarefas predecessoras.
		 */
		public function valida()
		{
			$arrSucessorasDiretaseIndiretas		= $this->getArrSucessorasDiretaseIndiretas();
			$arrPredecessorasDiretasIndiretas	= $this->getArrPredecessorasDiretaseIndiretas();
			$arrContidasDiretaseIndiretas		= $this->getArrQueContenhoDiretaseIndiretas();
			$arrContidaseRelacionadas			= $this->getArrTarefasContidaseRelacionadas();
			$this->setNovaDataInicio( $this->getNovaDataInicio() );
			$this->setNovaDataFim( $this->getNovaDataFim() );

			if( $this->getNovaDataFimTimestamp() < $this->getNovaDataInicioTimestamp() )
			{
				throw new Exception( 'A data de termino deve ser maior que a data de inicio' );
			}
/*
			if( ( $this->getNovaDataFim() == $this->getDataFim() ) && ( $this->getNovaDataInicio() !== $this->getDataInicio() ) )
			{
				$this->setNovaDataFim( date( 'd/m/Y', 
						( $this->getNovaDataInicioTimestamp() + $this->getDuracaoTimestamp() )
					 )
				);
			}
			if( ( $this->getNovaDataInicio() == $this->getDataInicio() ) && ( $this->getNovaDataFim() !== $this->getDataFim() ) )
			{
				$this->setNovaDataInicio( date( 'd/m/Y', 
						( $this->getNovaDataFimTimestamp() - $this->getDuracaoTimestamp() )
					 )
				);
			}
*/			
			try
			{
				# 1. O código único deve ser único no projeto 			#
/*				
				$arrTarefasComCodigo = $this->getArrCodigoUnicoPorProjeto();
				
				if( sizeof( $arrTarefasComCodigo ) > 0 )
				{
					$strErrorMessage = $arrTarefasComCodigo[ 0 ]->getMensagemErroCodigoUnico();
					throw new Exception( $strErrorMessage );
				}
*/				
				# 2.1. Nenhuma predecessora pode ser uma das sucessoras.	#
				
				$arrIntersect = array_intersect_object( $arrPredecessorasDiretasIndiretas , $arrSucessorasDiretaseIndiretas );
				if( sizeof( $arrIntersect ) > 0 )
				{
					$strErrorMessage = $arrIntersect[ 0 ]->getMensagemErroPredecessoraESucessora();	
					throw new Exception( $strErrorMessage );
				}
				
				# 2.2. A tarefa não pode ser sucessora de si mesma.		#
				if( in_array_object( $this , $arrSucessorasDiretaseIndiretas ) )
				{
					$strErrorMessage = $this->getMensagemErroPropriaSucessora();	
					throw new Exception( $strErrorMessage );
				}
				
				# 2.3. Nenhuma sucessora pode ser uma das predecessoras.	#
				
				// a Predecessora nao pode ser uma das sucessoras		//
				// a checagem de Predecessoras nao se faz necessaria	//
				// pois o o unico ponteiro existente nesta classe eh	//
				// o da predecessora que ja foi checado na lista das 	//
				// sucessoras											//
				
				# 2.4. A tarefa não pode ser predecessora de si mesma.	#
						
				if( in_array_object( $this , $arrPredecessorasDiretasIndiretas ) )
				{
					$strErrorMessage = $this->getMensagemErroPropriaPredecessora();
					throw new Exception( $strErrorMessage );
				}
				
				# 4. Nenhuma tarefa contida pode ser sucessora ou		#
				# predecessora desta.									#
				
				# 4.1 Nenhuma tarefa pode ser contida e predecessora	#
				
				$arrIntersect = array_intersect_object( $arrPredecessorasDiretasIndiretas , $arrContidasDiretaseIndiretas );
				if( sizeof( $arrIntersect ) > 0 )
				{
					$strErrorMessage = $arrIntersect[ 0 ]->getMensagemErroPredecessoraEContida();	
					throw new Exception( $strErrorMessage );
				}
				
				# 4.2  Nenhuma tarefa pode ser contida e sucessora		#
				
				$arrIntersect = array_intersect_object( $arrSucessorasDiretaseIndiretas , $arrContidasDiretaseIndiretas );
				if( sizeof( $arrIntersect ) > 0 )
								{
					$strErrorMessage =  $arrIntersect[ 0 ]->getMensagemErroSucessoraEContida();	
					throw new Exception( $strErrorMessage );
				}
	
				# 4.3 Nenhuma tarefa pode ser container e predecessora	#
				if( in_array_object( $this->getContainer() , $arrPredecessorasDiretasIndiretas ) )
				{
					$strErrorMessage = $this->getContainer()->getMensagemErroPredecessoraEContainer();	
					throw new Exception( $strErrorMessage );
				}
				
				# 4.4  Nenhuma tarefa pode ser container e sucessora		#
				
				if( in_array_object( $this->getContainer() , $arrSucessorasDiretaseIndiretas ) )
				{
					$strErrorMessage = $this->getContainer()->getMensagemErroSucessoraEContainer();	
					throw new Exception( $strErrorMessage );
				}
				
				# 5. Nenhuma tarefa contida pode conter esta tarefa.	#
				
				# 5.1  Nenhuma tarefa pode ser contida e container		#
				
				if( in_array_object( $this->getContainer() , $arrContidasDiretaseIndiretas ) )
				{
					$strErrorMessage = $this->getContainer()->getMensagemErroContainerEContida();
					throw new Exception( $strErrorMessage );
				}
			
				# 5.2  Nenhuma tarefa pode conter a si mesma			#
				
				if	(
						in_array_object( $this , $arrContidasDiretaseIndiretas ) 
					|| 
						$this === $this->getContainer()
					)
				{
					$strErrorMessage = $this->getMensagemErroPropriaContainer();
					throw new Exception( $strErrorMessage );
				}
				
				# 6. Nenhuma tarefa container pode se relacionar como 		#
				# sucessao ou precursao com as tarefas direta ou			# 
				# indiretamente contidas									#
								
				if( in_array_object( $this->getContainer() , $arrContidaseRelacionadas ) )
				{
					$strErrorMessage = $this->getMensagemErroContainerRelacionada();
					throw new Exception( $strErrorMessage );
				}
				
				# 7. As alterações das datas de inicio e fim devem ser		#
				# efetivadas com sucesso e em cascata na tarefa que contém	#
				# esta tarefa.												#
				# 8. As alterações das datas de inicio e fim devem ser 		#
				# efetivadas com sucesso e em cascata nas tarefas contidas	#
				# por esta tarefas.											#
			 	# 9. As alterações da data de término devem ser efetivadas	#
			 	# com sucesso e em cascata nas tarefas sucessoras.			#
				# 10. As alterações da data de inicio devem ser efetivadas	#
				# com sucesso e em cascata nas tarefas predecessoras.		#
				$this->setDatasAlteradas( false );
				$this->verificaPossibilidadeDeAlterar( $this->getNovaDataInicioTimestamp() , $this->getNovaDataFimTimestamp() );
				if( $this->getProjeto() )
				{
					$this->getProjeto()->verificaPossibilidadeDeAlterar();
				}
			}
			catch( Exception $ErrorObject )
			{
				throw $ErrorObject;
			}
		}
		
		public function __setAttribute( $strAttribute, $mixValue )
		{
			switch( $strAttribute )
			{
				case 'datInicio':
				{
					$this->setNovaDataInicio( $mixValue );
					break;	
				}
				case 'datFim':
				{
					$this->setNovaDataFim( $mixValue );
					break;			
				}
				case 'intPredecessoraCodigoUnico':
				{
					$this->setPredecessoraCodigoUnico( $mixValue );
					break;		
				}
				default:
				{
					$this->$strAttribute = $mixValue;		
					break;
				}
			}
		}
		
		public function validate()
		{
			$this->valida();
		}
		
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
							 $this->ROW_NAME_MEAUSE_UNIT_ID .
							 ',' .
							 $this->ROW_NAME_INT_POSITION .
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
							escape( $this->getUnidadeDeMedidaId() ) .
							',' .
							escape( $this->getPosicao() ) .
						' ) ' .
					'';
			
			// executando a query criada //

			$objSaida = self::$objDatabase->executar( $strSql );
			# 3. Atualiza o identificador da tarefa						#
			$this->setId( (integer) $objSaida );
			
			$this->setInserted( true );
		}
		
		public function update()
		{
			$this->atualizarTarefa();
		}
		
		public function remove()
		{
			$this->removerTarefa();
		}
		
		public function getDatasAlteradas()
		{
			return $this->boolAlterada;
		}
		
		public function setDatasAlteradas( $boolChanged )
		{
			$this->boolAlterada = $boolChanged;
			if( $boolChanged )
			{
				$this->setChanged( true );
			}
		}
		
		public function atualizaTodasAsTarefas()
		{
			self::updateAllChangedEntities();
		}
		
		public function getArrChanged()
		{
			return self::getChangedEntities();
		}
		
		public function analiseRemotionImpact()
		{
			return $this->verificaPossibilidadeDeRemover();
		}

		public static function reparaTarefas( Projeto $objProjeto)
		{
			$objTarefa = $objProjeto->criaTarefa();;
			$arrTarefas = $objTarefa->getArrTarefasPeloProjeto( $objProjeto );
			$intCodigoUnicoAtual = 1;
			$intCodigoUnicoAtual = self::reparaRecursivamente( $arrTarefas , $intCodigoUnicoAtual );
		}
		
		public static function reparaRecursivamente( $arrTarefasReparando , $intCodigoUnicoAtual )
		{
			$intPosicaoNoNivel = 1;
			$arrTarefasReparando = orderArrayOfObjectsByMethod( $arrTarefasReparando  , 'getPosicao' );
			foreach( $arrTarefasReparando as $objTarefaReparando )
			{
				$objTarefaReparando->setCodigoUnico( $intCodigoUnicoAtual );
				$objTarefaReparando->setPosicao( $intPosicaoNoNivel );
				++$intCodigoUnicoAtual;
				++$intPosicaoNoNivel;
				$arrFilhas = $objTarefaReparando->getArraydeTarefasqueContenho();
				$intCodigoUnicoAtual = self::reparaRecursivamente( $arrFilhas , $intCodigoUnicoAtual );
				$objTarefaReparando->setChanged( true );
			}
			return $intCodigoUnicoAtual;
		}
		
		/**
		 * Gera o array dos attributos acessiveis pelo ActiveFrozenFields
		 *
		 * @return array
		 */
		public function __toActiveFrozenField( ActiveFrozenField $objActiveFrozenField )
		{
			$arrObjectsVar = get_object_vars( $this );
			$arrObjectsVar[ 'datInicio' ] = $this->getNovaDataInicio();
			$arrObjectsVar[ 'datFim' ] = $this->getNovaDataFim();
			$arrObjectsVar[ 'intPredecessoraCodigoUnico' ] = $this->getPredecessora() ? $this->getPredecessora()->getCodigoUnico() :  $this->getPredecessora();
			$arrObjectsVar = ActiveFrozenFieldEntity::__toJson( $arrObjectsVar , get_class( $this ) );
			$arrObjectsVar[ 'strCodigoEstruturado' ] = $this->getCodigoEstruturado();
			$objActiveFrozenField->setAttributesOfContainer( $arrObjectsVar );
		}
		
		
		public function criaTarefa()
		{
			throw new Exception( 'O cria tarefa deve ser recriado para cada extensao' );
		}		
	}
?>