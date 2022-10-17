<?php
	abstract class Projeto
	{
		/**
		 * Nome da tabela do banco onde ficam armazenadas os projetos
		 *
		 */
		public $TABLE_NAME = 'monitora.projetoespecial';
		
		/**
		 * Identificador da Tabela
		 */
		public $ROW_NAME_ID 			= 'pjeid';
		
		/**
		 * @see ROW_NAME_ID
		 */
		protected $intId = null;
		
		/**
		 * Coluna de texto que contém o nome do projeto
		 */
		public $ROW_NAME_STR_NAME 		= 'pjedsc';
		
		/**
		 * @var string
		 * @see Projeto::ROW_NAME_STR_NAME
		 */
		protected $strNome = null;
		
		/**
		 * Coluna de data que contém a data de início do projeto
		 */
		public $ROW_NAME_DATE_START 		= 'pjedataini';
		
		/**
		 * @var date
		 * @see Projeto::ROW_NAME_DATE_START
		 */
		protected $datInicio = null;
		
		/**
		 * Coluna de data que contém a data de término do projeto
		 */
		public $ROW_NAME_DATE_END 		= 'pjedatafim';
		
		/**
		 * @var date
		 * @see Projeto::ROW_NAME_DATE_END
		 */
		protected $datFim = null;
		
		/**
		 * Coluna booleana que informa se o projeto possui a data
		 * fechada ou não.
		 * 
		 * Caso a data seja fechada não se pode altera-la por efeito
		 * colateral das tarefas do mesmo, retornando um erro caso
		 * isso seja tentado.
		 *
		 */
		protected  $ROW_NAME_BOOL_CLOSED_DATE	= 'pjesndatafechada';

		/**
		 * @var bool
		 * @see Projeto::ROW_NAME_BOOL_CLOSED_DATE
		 */
		protected $boolDataFechada;
		
		/**
		 * Array das tarefas que deste projeto
		 */
		protected $arrTarefasDoProjeto = null;
		
		/**
		 * Array estatico das projetos já lidas. Evita se instanciar mais
		 * de uma copia do mesmo objeto e recursividade eterna.
		 *
		 * @var Array of Projeto
		 */
		static protected $arrInstances = array();
		
		/**
		 * @var date
		 */
		protected $datNovaDataInicio;
		
		/**
		 * @var date
		 */
		protected $datNovaDataFim;
		
		/**
		 * @var object database connection
		 */		
		static protected $objDatabase;

		/**
		 * Se o objeto já teve suas datas alteradas
		 * 
		 * @var bool
		 */
		protected $boolChanged;
		
		// sets e gets //
				
		public function setId( $intValue )
		{
			if( $intValue !== null)
			{
				$this->intId = (integer) $intValue;
			}
			else
			{
				$this->intId = null;
			}
		}
		
		public function getId()
		{
			return $this->intId;
		}
		
		public function setNome( $strNome )
		{
			if( $strNome !== null )
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
			if( $strDescricao !== null )
			{
				$this->strDescricao =  (string) $strDescricao;
			}
			else
			{
				$this->strDescricao = null;
			}
		}
		
		public function setDataInicio( $datInicio )
		{
			if( $datInicio !== null )
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
			return $this->datNovaDataInicio;
		}
		
		public function setNovaDataInicio( $datInicio )
		{
			if( $datInicio !== null )
			{
				$this->datNovaDataInicio = $datInicio;
			}
			else
			{
				$this->datNovaDataInicio = null;
			}
		}
		
		public function getNovaDataInicioTimestamp()
		{
			return convertToTimestamp( $this->datNovaDataInicio );
		}

		public function setNovaDataInicioTimestamp( $intNovaDataInicio )
		{
			$this->setNovaDataInicio( date( 'd/m/Y' , $intNovaDataInicio ) );
		}
		
		public function setNovaDataFimTimestamp( $intNovaDataFim )
		{
			$this->setNovaDataFim( date( 'd/m/Y' , $intNovaDataFim ) );
		}
		
		public function setDataFim( $datFim )
		{
			if( $datFim !== null )
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
			return $this->datNovaDataFim;
		}
		
		public function setNovaDataFim( $datFim )
		{
			if( $datFim !== null )
			{
				$this->datNovaDataFim = $datFim;
			}
			else
			{
				$this->datNovaDataFim = null;
			}
		}
		
		public function getNovaDataFimTimestamp()
		{
			return convertToTimestamp( $this->datNovaDataFim );
		}
				
		public function setDataFechada( $boolDataFechada )
		{
			if( $boolDataFechada !== null )
			{
				$this->boolDataFechada = (bool) $boolDataFechada;
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
		
		// fim dos sets e gets //
		
		/**
		 * Construtor dos projetos
		 */
		public function __construct()
		{
			global $db;
			self::$objDatabase = $db;
		}
		
		/**
		 * Retorna o tamanho da duração da projeto em timestamp
		 *
		 * @return integer
		 */
		public function getDuracaoTimestamp()
		{
			return $this->getDataFimTimestamp() - $this->getDataInicioTimestamp();
		}
		
		/**
		 * Retorna o tamanho da duração da projeto em timestamp
		 *
		 * @return integer
		 */
		public function getNovaDuracaoTimestamp()
		{
			return $this->getNovaDataFimTimestamp() - $this->getNovaDataInicioTimestamp();
		}

		
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
			}
			else
			{
				throw new Exception( 'Projeto ( ' . $this->getId() . ' ) nao encontrado ') ;
			}
		}
		
		/**
		 * Atualiza um projeto na persistencia
		 * 
		 * 1. Valida o projeto
		 * 2. Atualiza a projeto na persistencia
		 * 3. Marca a projeto como não alterado
		 */
		public function atualizarProjeto()
		{
			
			$this->valida();

			$strSql =' UPDATE ' .
						$this->TABLE_NAME .
					' SET ' . 
						$this->ROW_NAME_STR_NAME				. ' = ' . escape( $this->getNome() ). 
						',' . 
						$this->ROW_NAME_DATE_START			. ' = ' . escape( $this->getDataInicio() , 'date' ) . 
						',' . 
						$this->ROW_NAME_DATE_END				. ' = ' . escape( $this->getDataFim() , 'date' ) .
						',' . 
						$this->ROW_NAME_BOOL_CLOSED_DATE		. ' = ' . escape( $this->getDataFechada() ) .
					' WHERE ' .
						$this->ROW_NAME_ID					. ' = ' . $this->getId();
					'';
					
			// executando a query criada //
			$objSaida = self::$objDatabase->atualizar( $strSql );
			
			$this->setDatasAlteradas( false );
		}
		
		/**
		 * Insere o projeto na persistencia após validar os seus campos
		 * 
		 * 1. Valida o projeto
		 * 2. Insere o projeto na persistencia
		 * 3. Atualiza o identificador do projeto
		 */
		public function inserirProjeto()
		{
			# 1. Valida o projeto										#
			
			try
			{
				$this->valida();
			}
			catch( Exception $ErrorObject )
			{
				throw $ErrorObject;
			}

			# 2. Insere o projeto na persistencia						#
			# 3. Atualiza o identificador do projeto					#
			$this->insert();
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
						' ) ' .
					'';
			
			// executando a query criada //
			
			$objSaida = self::$objDatabase->inserir( $strSql );
			
			# 2. Atualiza o identificador do projeto						#
			$this->setId( (integer) $objSaida );
		}
		
		/**
		 * Retorna o projeto ao receber o id do mesmo.
		 *
		 * @param integer $intId
		 * @return Projeto
		 */
		abstract public static function getProjetoPeloId( $intId );
		/*
		{
			if( isset( Projeto::$arrInstances[ $intId ] ) )
			{
				$objProjeto = Projeto::$arrInstances[ $intId ];
			}
			else
			{
				$objProjeto = new Projeto();
				$objProjeto->setId( $intId );
				$objProjeto->emerge();
				Projeto::$arrInstances[ $intId ] = $objProjeto;
			}
			return $objProjeto;
		}
		*/
		
		/**
		 * Retorna se a tarefa foi alterada ou não
		 */
		public function getDatasAlteradas()
		{
			return $this->boolChanged;
		}
		
		/**
		 * Marca uma tarefa como alterada.
		 * 
		 * As alterações nas tarefas alteradas só serão salvas na persistencia 
		 * quando o método atualiza for chamado.
		 * 
		 * @see Tarefa::atualizaTodasAsTarefasAlteradas
		 * @see Tarefa::valida
		 * @see Projeto::valida
		 * @see Tarefa::verificaPossibilidadeDeAlterar
		 * @param bool $boolChanged
		 */
		public function setDatasAlteradas( $boolChanged )
		{
			$this->boolChanged = (bool) $boolChanged;
		}
		
		public function getMensagemdeErroProjetoNaoComporta()
		{
			return	'Não foi possível alterar os datas projeto pois o período não comporta as alterações necessárias.' .
				'';			
		}
		
		public function getMensagemErroProjetoFechado()
		{
			return	'Não foi possível alterar os datas projeto pois a data do mesmo é fechada.' . 
				$this->getDataInicio() . ':' . $this->getDataFim() . 
				' - ' . 
				$this->getNovaDataInicio() . ':' . $this->getNovaDataFim() .
				'';			
		}

		abstract public function getArrTarefasDoProjeto();
/*
		{
			if( $this->arrTarefasDoProjeto === null )
			{
				 $this->arrTarefasDoProjeto = orderArrayOfObjectsByMethod(  Tarefa::getArrTarefasPeloProjeto( $this )  , 'getDataInicioTimestamp' );
			}
			return $this->arrTarefasDoProjeto;
		}
*/		
		/**
		 * Verifica se o projeto é válido e se está apto para ser salvo.
		 *
		 * . O nome do projeto é obrigatório
		 * . As tarefas que são contidas no projeto devem estar dentro dos 
		 * limites do mesmo
		 */
		public function valida()
		{
			if( $this->getNome() == null )
			{
				throw new Exception( 'O nome do projeto é obrigatório.' );
			}		
			if( $this->getNovaDataFim() == null )
			{
				$this->setNovaDataFim( $this->getDataFim() );
			}
			if( $this->getNovaDataInicio() == null )
			{
				$this->setNovaDataInicio( $this->getDataInicio() );
			}
			if( $this->getNovaDataFim() == null || $this->getNovaDataFim() == null )
			{
				throw new Exception( 'As datas de inicio e término são obrigatórios.' );
			}
			
			$this->verificaPossibilidadeDeAlterarAsQueContenho( $this->getNovaDataInicioTimestamp() , $this->getNovaDataFimTimestamp() );
		}
		
		/**
		 * Move um grupo mantendo estática os períodos entre as tarefas
		 *
		 */
		public function arrastaGrupo()
		{
			if( $this->getNovaDataFim() == null )
			{
				$this->setNovaDataFim( $this->getDataFim() );
			}
			if( $this->getNovaDataInicio() == null )
			{
				$this->setNovaDataInicio( $this->getDataInicio() );
			}
			
			$intDataInicialOriginal		= $this->getDataInicioTimestamp();
			$intDataInicialNova			= $this->getNovaDataInicioTimestamp();
			$intDataFinalOriginal		= $this->getDataFimTimestamp();
			$intDataFinalNova			= $this->getNovaDataFimTimestamp();
			
			if( $intDataInicialNova !== $intDataInicialOriginal )
			{
				$arrTarefasQueContenho		= $this->getArrTarefasDoProjeto();
				$arrTarefasQueContenhoAsc	=  ( orderArrayOfObjectsByMethod( $arrTarefasQueContenho , 'getDataInicioTimestamp' ) );
				foreach( $arrTarefasQueContenhoAsc as $objTarefaQueContenho )
				{
					$intTarefaDataInicial = $objTarefaQueContenho->getDataInicioTimestamp();
					$intDistancia = $intTarefaDataInicial - $intDataInicialOriginal;
					$intDuracao	= $objTarefaQueContenho->getDuracaoTimestamp();
					$intTarefaNovaDataInicial = $intDataInicialNova + $intDistancia;
					$intTarefaNovaDataFinal = $intTarefaNovaDataInicial + $intDuracao;
					$objTarefaQueContenho->setNovaDataInicio(	date('d/m/Y' , $intTarefaNovaDataInicial ) );
					$objTarefaQueContenho->setNovaDataFim( 		date('d/m/Y' , $intTarefaNovaDataFinal ) );
					$objTarefaQueContenho->setAlterada( true );
					$objTarefaQueContenho->verificaPossibilidadeDeArrastasAsQueContenho();
				}
			}
			else if( $intDataFinalOriginal !== $intDataFinalNova )
			{
				$arrTarefasQueContenho		= $this->getArrTarefasDoProjeto();
				$arrTarefasQueContenhoDesc	= array_reverse( $arrTarefasQueContenhoAsc );
				foreach( $arrTarefasQueContenhoDesc as $objTarefaQueContenho )
				{
					$objTarefaQueContenho = new Tarefa();
					$intTarefaDataTermino = $objTarefaQueContenho->getDataFimTimestamp();
					$intDistancia = $intDataFinalOriginal - $intTarefaDataTermino;
					$intDuracao	= $objTarefaQueContenho->getDuracaoTimestamp();
					$intTarefaNovaDataFinal = $intDataFinalNova - $intDistancia;
					$intTarefaNovaDataInicial = $intTarefaNovaDataFinal + $intDuracao;
					$objTarefaQueContenho->setNovaDataInicio(	date('d/m/Y' , $intTarefaNovaDataInicial ) );
					$objTarefaQueContenho->setNovaDataFim( 		date('d/m/Y' , $intTarefaNovaDataFinal ) );
					$objTarefaQueContenho->setAlterada( true );
					$objTarefaQueContenho->verificaPossibilidadeDeArrastasAsQueContenho();
				}
			}
			
			
			$this->valida();
		}
		
		/**
		 * Função responsável pela administração das datas do projeto se 
		 * adequarem corretamente e automaticamente às alterações feitas
		 * nas tarefas.
		 * 
		 * Este método não deve ser diretamente chamado quando o projeto
		 * for alterado, mas sim durante as alterações das tarefas para
		 * conferir se as datas estão compatíveis e manter o processo de
		 * cascata.
		 */
		public function verificaPossibilidadeDeAlterar()
		{
			$intLimiteDataInicio	= $this->getDataInicioTimestamp();
			$intLimiteDataFim		= $this->getDataFimTimestamp();
			$arrTarefasDoProjeto 	= $this->getArrTarefasDoProjeto();
			
			foreach( $arrTarefasDoProjeto as $objTarefaDoProjeto )
			{
				$intTarefaInicioData	= ( $objTarefaDoProjeto->getNovaDataInicioTimestamp()	=== null ) ? $objTarefaDoProjeto->getDataInicioTimestamp()	: $objTarefaDoProjeto->getNovaDataInicioTimestamp();
				$intTarefaFimData		= ( $objTarefaDoProjeto->getNovaDataFimTimestamp()		=== null ) ? $objTarefaDoProjeto->getDataFimTimestamp()		: $objTarefaDoProjeto->getNovaDataFimTimestamp();

				if( $intTarefaInicioData < $intLimiteDataInicio )
				{
					$intLimiteDataInicio = $intTarefaInicioData;
				}
				if( $intTarefaFimData > $intLimiteDataFim )
				{
					$intLimiteDataFim = $intTarefaFimData;
				}
			}
			if( $this->getDataInicio() > $intLimiteDataInicio )
			{
				$intNovaDataInicio	= $intLimiteDataInicio;
				$intNovaDataFim		= $intLimiteDataInicio + $this->getDuracaoTimestamp();
			}
			else if( $this->getDataFim() < $intLimiteDataFim )
			{
				$intNovaDataInicio	= $intLimiteDataFim - $this->getDuracaoTimestamp();
				$intNovaDataFim		= $intLimiteDataFim;
			}
			else
			{
				$intNovaDataInicio	= $this->getDataInicioTimestamp();
				$intNovaDataFim 	= $this->getDataFimTimestamp();
			}
			
			$intDuracao = $intNovaDataFim - $intNovaDataInicio;
			if( $intDuracao > $this->getDuracaoTimestamp() )
			{
				throw new Exception( 'Erro ao alterar o projeto. O período fixo não suporta as alterações.' );
			}
			
			$this->setNovaDataInicio( date('d/m/Y' , $intNovaDataInicio ) );
			$this->setNovaDataFim(  date('d/m/Y' , $intNovaDataFim ) );

			$this->setDatasAlteradas( true );
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
			$arrTarefasQueContenho		= $this->getArrTarefasDoProjeto();
			$arrTarefasQueContenhoAsc	=  ( orderArrayOfObjectsByMethod( $arrTarefasQueContenho , 'getDataInicioTimestamp' ) );
			$arrTarefasQueContenhoDesc	= array_reverse( $arrTarefasQueContenhoAsc );
			
			foreach( $arrTarefasQueContenhoAsc as $objTarefaQueContenho )
			{
				$intDataInicio	= $objTarefaQueContenho->getDataInicioTimestamp();
				$intDataFim		= $objTarefaQueContenho->getDataFimTimestamp();
				$intDuracao		= $intDataFim - $intDataInicio;
				
				// se a tarefa que contenho escapa pelo limite de data de inicio //
				if( $intContainerDataInicio > $intDataInicio )
				{
					// se existe a possibilidade de se alterar a tarefa que contenho //
					if( $objTarefaQueContenho->getDataFechada() == false || $objTarefaQueContenho->foiAlterada() )
					{
						$intNovaDataInicio	= $intContainerDataInicio;
						$intNovaDataFim		= $intContainerDataInicio + $intDuracao;
						// se ja foi anteriormente alterada //
						if( $objTarefaQueContenho->foiAlterada() )
						{
							// se a alteracao eh diferente da que esta agora sendo feita //
							if	( $intContainerDataInicio > $objTarefaQueContenho->getNovaDataInicioTimestamp() )
							{
								// erro de alteracoes simultaneas //
								$strErrorMessage = $objTarefaQueContenho->getMensagemdeErroTarefaNaoComporta();
								throw new Exception( $strErrorMessage );
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
					if( $objTarefaQueContenho->getDataFechada() == false || $objTarefaQueContenho->foiAlterada() )
					{
						$intNovaDataFim		= $intContainerDataFim ;
						$intNovaDataInicio	= $intContainerDataFim - $intDuracao;
						
						// se ja foi anteriormente alterada //
						if( $objTarefaQueContenho->foiAlterada() )
						{
							// se a alteracao eh diferente da que esta agora sendo feita //
							if	(  $intContainerDataFim < $objTarefaQueContenho->getNovaDataFimTimestamp() )
							{
								// erro de alteracoes simultaneas //
								$strErrorMessage = $objTarefaQueContenho->getMensagemdeErroTarefaNaoComporta();
								throw new Exception( $strErrorMessage );
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
		
		public function criaTarefa()
		{
			return Tarefa::criaTarefa();
		}		
	}
?>