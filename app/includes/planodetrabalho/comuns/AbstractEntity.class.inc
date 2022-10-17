<?php

	/**
	 * Todas as entidades definidas no sistema extendem esta classe.
	 *
	 * @package Class
	 * @subpackage Base
	 */ 
	abstract class AbstractEntity
	{
		// attributes //
		
		/**
		 * Identificado do usuario.
		 */
		private $intId = NULL;

		/**
		 * Chave para controle de updates simultaneos
		 */
		private $intSid = NULL;

		/**
		 * Chave para controle de updates simultaneos
		 */
		private $strStatus = NULL;
		
		/**
		 * Objeto que valida a entidade.
		 */
		protected $objValidate = NULL;

		/**
		 * Variavel de controle se a entidade ja foi alterada ou nao
		 *
		 * @var bool
		 */
		protected $boolChanged = false;
		
		/**
		 * Variavel de controle para objetos em
		 * processo de remocao 
		 */
		protected $boolRemoved = false;
		
		/**
		 * Variavel de controle para objetos em
		 * processo de remocao 
		 */
		protected $boolInserted = false;
		
		/**
		 * Objeto que faz a associacao dos dados da entidade com os dados do banco e possibilita a formacao
		 * de queries automaticamente
		 * @access protected
		 */
		protected $objDatabaseEntity = NULL;
		
		/**
		 * Array estatico das entidades já lidas. Evita se instanciar mais
		 * de uma copia do mesmo objeto e recursividade eterna.
		 *
		 * @var Array of AbstractEntity
		 */
		static protected $arrInstances = array();
				
		/**
		 * Array de Entidades alteradas nesta execucao
		 *
		 * @var array of AbstractEntity
		 */
		protected static $arrChangedEntities = array();
				
		/**
		 * Array de Entidades alteradas nesta execucao
		 *
		 * @var array of AbstractEntity
		 */
		protected static $arrRemovedEntities = array();
				
		// setters and getters //
		
		/**
		 * Retorna o identificador da entidade.
		 *
		 * @return integer
		 */
		public function getId()
		{
			return( $this->intId );
		}

		/**
		 * Retorna a chave para controle de concorrencia
		 *
		 * @return integer
		 * @access public 
		 */
		public function getSid()
		{
			return( $this->intSid );
		}
		
		/**
		 * Retorna a chave para controle de status
		 *
		 * @return string
		 * @access public 
		 */
		public function getStatus()
		{
			return( $this->strStatus );
		}

		/**
		 * Retorna se o campo ja foi alterado nesta execucao
		 */
		public function getChanged()
		{
			return $this->boolChanged;
		}
			
		/**
		 * Retorna verdadeiro caso a entidade
		 * tenha sido marcada como a ser inserida
		 */
		public function getInserted()
		{
			return $this->boolInserted;
		}
			
		/**
		 * Retorna verdadeiro caso a entidade
		 * tenha sido marcada como a ser removida
		 */
		public function getRemoved()
		{
			return $this->boolRemoved;
		}		
		
		/**
		 * Altera o valor do identificador do objeto.
		 *
		 * @param integer $intId
		 * @return void
		 */
		public function setId( $intId )
		{
			if( $this->intId != $intId )
			{
				$this->removeInstance();
				$this->intId = (integer) $intId;
				$this->addInstance();
			}
		}

		/**
		 * Altera o valor do campo de controle de concorrencia
		 *
		 * @param integer $intId
		 * @return void
		 * @access public 
		 */
		public function setSid( $intSid )
		{

			$this->intSid = (integer) $intSid;
		}

		/**
		 * Altera o valor do campo de controle de status
		 *
		 * @param string $strStatus
		 * @return void
		 * @access public 
		 */
		public function setStatus( $strStatus )
		{
			$this->strStatus = $strStatus;
		}
			
		/**
		 * Altera o valor da marcacao de alterada da entidade
		 */
		public function setChanged( $boolChanged )
		{
			$this->boolChanged = ( $boolChanged == true );
		}
		
		public function setInserted( $boolInserted )
		{
			$this->boolInserted  = ( $boolInserted == true  );
		}
		
		public function setRemoved( $boolRemoved )
		{
			$this->boolRemoved  = ( $boolRemoved == true  );
		}
		
		// Magic Methods //
		
		/**
		 * Construtor
		 *
		 */
		public final function __construct()
		{
			$this->reset();
			$this->prepare();
			$this->init();
		}
		
		/**
		 * Alternative construct
		 *
		 */
		public function init()
		{
			
		}
		
		/**
		 * Quando uma entidade eh copiada seu identificador eh alterado para um
		 * valor nulo.
		 *
		 * @return void
		 */
		public function __clone()
		{
			$this->intId = NULL;
		}

		/**
		 * Metodo chamado quando o objeto da classe eh serializado. Teve de ser redefinido para se comportar de maneira
		 * padrao pois o php estava chamando o __call ao nao encontrar este metodo definido.
		 */
		public function __sleep()
		{
			$arrMethods = array_keys( get_object_vars( $this ) );
			return( $arrMethods );
		}
		
		/**
		 * Metodo chamado quando o objeto da classe eh unserializado. Redefinido de maneira padrao pois o php estava chamando
		 * o __call ao nao encontrar este metodo definido.
		 */
		public function __wakeup()
		 {
			 return;
		 }		
				
		// Initialization Methods //
		
		protected function prepare()
		{
		}
		// reset methods //
		
		/**
		 * Altera o valor do identificados da entidade para um valor nulo.
		 *
		 * @return void
		 */
		public function resetId() 
		{
			$this->setId( NULL );
		}
		
		/**
		 * Altera os valores dos atributos basicos da entidade para um valor
		 * nulo. Esse metodo eh sobrescrito por cada entidade, porem deve ser
		 * requisitado pelo mesmo.
		 *
		 * @return void
		 */
		public function reset()
		{
			$this->resetId();
			$this->objValidate 	= NULL;
		}
			
		// Instance Controler //
		
		/**
		 * Remove the entity from the instance array
		 *
		 */
		private function removeInstance()
		{
			if( $this->intId )
			{
				if( isset( self::$arrInstances[ get_class( $this ) ] ) )
				{
					if( isset( self::$arrInstances[ get_class( $this ) ][ $this->intId ] ) )
					{
						unset( self::$arrInstances[ get_class( $this ) ][ $this->intId ] );
					}
				}
				
			}
		}
		
		/**
		 * Add the entity to the instance array
		 *
		 */
		private function addInstance()
		{
			if( $this->intId )
			{
				if( !isset( self::$arrInstances[ get_class( $this ) ] ) )
				{
					self::$arrInstances[ get_class( $this ) ] = array();
				}
				if( !isset( self::$arrInstances[ get_class( $this ) ][ $this->intId ] ) )
				{
					self::$arrInstances[ get_class( $this ) ][ $this->intId ] = $this;
				}
				else 
				{
					throw new Exception( "Already exist another instance of this class ( " . get_class( $this ) . " )with the same id ( " . $this->intId . " )" );
				}
			}
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
				$objElement->emerge();
				return $objElement;
			}
		}
			
		// Xml methods //
			
		/**
		 * Transforma a entidade em um xml.
		 *
		 * @return string
		 *
		 */
		public function toXml()
		{
			Core::import( "xmlserializer" );
			$arrSerializerOptions = array
			(
				'addDecl' => FALSE,
				'indent' => '',
				'linebreak' => '',
				'rootName'  => get_class( $this ),
				'typeHints' => TRUE,
			);
			$objSerializer = new XML_Serializer( $arrSerializerOptions );
			$status = $objSerializer->serialize( $this->toArray() );
			if ( $status )
			{
				return $objSerializer->getSerializedData();
			}
			return NULL;
		}

		public function fromXml( $strXml )
		{
			Core::import( "xmlserializer" );
			$objUnserializer = new XML_Unserializer();
			$status = $objUnserializer->unserialize( $strXml );
		}

		// Validate Methods //
		
		/**
		 * Aciona o metodo que valida o formato da entidade.
		 *
		 * @return EntityValidateResult
		 */
		public abstract function validate();

		// Database Methods //
		
		/**
		 * Recebe os dados do objeto e atualiza a persistencia com os mesmos.
		 * @return bool
		 */
		public abstract function update();
		
		/**
		 * Insere os dados da entidade na persistencia e recebe da mesma o id.
		 * @return bool
		 */
		public abstract function insert();
		
		/**
		 * Emerge os dados da entidade na persistencia
		 */
		public abstract function emerge();
		
		/**
		 * Prepara uma entidade para ser removida do sistema.
		 *
		 * @return bool
		 */
		public abstract function remove();

		/**
		 * Remove uma entidade da persistecia.
		 *
		 * @return bool
		 */
		protected abstract function delete();
		
		/**
		 * Atualiza na Persistencia todas as entidades alteradas
		 *
		 */
		public static function updateAllChangedEntities()
		{
			$arrChangedEntities = self::getChangedEntities();
			foreach( $arrChangedEntities as $strClass => $arrClassInstance )
			{
				foreach( $arrClassInstance as $intObjectId => $objEntity )
				{
					$objEntity->update();					
				}
			}
		}
		
		/**
		 * Remove da Persistencia todas as entidades marcadas para
		 * serem removidas
		 *
		 */
		public static function deleteAllRemovedEntities()
		{
			$arrRemovedEntities = self::getRemovedEntities();
			foreach( $arrRemovedEntities as $strClass => $arrClassInstance )
			{
				foreach( $arrClassInstance as $intObjectId => $objEntity )
				{
					$objEntity->delete();					
				}
			}
		}
		
		// ActiveFrozenField Internal Communication Methods //
		
		/**
		 * Gera o array dos attributos acessiveis pelo ActiveFrozenFields
		 *
		 * @return array
		 */
		public function __toActiveFrozenField( ActiveFrozenField $objActiveFrozenField )
		{
			$arrObjectsVar = get_object_vars( $this );
			$arrObjectsVar = ActiveFrozenFieldEntity::__toJson( $arrObjectsVar , get_class( $this ) );
			$objActiveFrozenField->setAttributesOfContainer( $arrObjectsVar );
		}

		/**
		 * Retorna o array de todas as entidades alteradas nesta execucao
		 *
		 */
		protected static function getChangedEntities()
		{
			self::$arrChangedEntities = array();
			foreach( self::$arrInstances as $strClass => $arrClassInstance )
			{
				foreach( $arrClassInstance as $intObjectId => $objEntity )
				{
					if( ( $objEntity->getChanged() == true ) && ( $objEntity->getRemoved() == false ) ) 
					{
						self::$arrChangedEntities[ $strClass ][ $intObjectId ] = $objEntity;					
					}
				}
			}
			return self::$arrChangedEntities;
		}
		
		public static function getRemovedEntities()
		{
			self::$arrRemovedEntities = array();
			foreach( self::$arrInstances as $strClass => $arrClassInstance )
			{
				foreach( $arrClassInstance as $intObjectId => $objEntity )
				{
					if( $objEntity->getRemoved() == true )
					{
						self::$arrRemovedEntities[ $strClass ][ $intObjectId ] = $objEntity;					
					}
				}
			}
			return self::$arrRemovedEntities;
		}
		/**
		 * Receive the Array and try to change the attribute value
		 *
		 * @param array $arrNewValues
		 */
		public function __fromActiveFrozenField( ActiveFrozenField $objActiveFrozenField )
		{
			switch( $objActiveFrozenField->getMethod() )
			{
				case 'update':
				{
					$arrNewValues = $objActiveFrozenField->getAttributesNewValues();
					$arrToSet = array_intersect_key( $arrNewValues, get_object_vars( $this ) );
					foreach( $arrToSet as $strAttribute => $mixValue )
					{
						if( ActiveFrozenFieldEntity::isActiveFrozenField( $strAttribute , get_class( $this ) ) )
						{
							$this->__setAttribute( $strAttribute , $mixValue );
							$this->setChanged( true );
						}
					}
					if( $this->getChanged() )
					{
						$this->validate();
					}
					break; 
				}
				case 'remove':
				{
					//dbg( __LINE__ , 1 );
					$this->remove();
					break;
				}
				case 'insert':
				{
				//	dbg( __LINE__ , 1 );
					break;
				}
				
			}
			$objActiveFrozenField->setChangedEntities( $this->getChangedEntities() );
			$objActiveFrozenField->setRemovedEntities( $this->getRemovedEntities() );
		}
		
		// Database Internal Communication Methods //
		
		/**
		 * Gera um array com os atributos do objeto para que o banco possa criar os comandos
		 * sql com os mesmos.
		 *
		 * @return unknown
		 */
		public function __toDatabase()
		{
			$arrObjectsVar = get_object_vars( $this );
			return $arrObjectsVar;
		}

		/**
		 * Receive the Array and try to change the attribute value
		 *
		 * @param array $arrNewValues
		 */
		public function __fromDatabase( $arrNewValues )
		{
			$arrToSet = array_intersect_key( $arrNewValues, get_object_vars( $this ) );
			foreach( $arrToSet as $strAttribute => $mixValue )
			{
				$this->__setAttribute( $strAttribute , $mixValue );
			}
		}
		
		/**
		 * Required method to control the attributes of class 
		 * @param string $strAttribute
		 * @param unknown $mixValue
		 */
		abstract protected function __setAttribute( $strAttribute, $mixValue );

		public function analiseRemotionImpact()
		{
			return array();
		}
	}
?>