<?php
class ActiveFrozenField
{
	protected $intIdObject = null;
	
	protected $strContainerClassName = null;

	protected $strMethod = null;
	
	protected $strAttributeName = null;
	
	protected $mixOriginalValue = null;
	
	protected $mixNewValue = null;
	
	protected $objElement = null;
	
	protected $arrAttributesOriginalValues = array();
	
	protected $arrAttributesNewValues = array();
	
	protected $arrAttributesValues = array();
	
	protected $arrChangedEntities = array();
	
	protected $arrRemovedEntities = array();
	
	protected $arrWarnings = array();
	
	public function setContainerClassName( $strConainerClassName )
	{
		$this->strContainerClassName = $strConainerClassName;
	}
	
	public function setAttributeName( $strAttributeName )
	{
		$this->strAttributeName = $strAttributeName;
	}
	
	public function setOriginalValue( $mixOriginalValue )
	{
		$this->mixOriginalValue = $mixOriginalValue;
	}
	
	public function setNewValue( $mixNewValue )
	{
		$this->mixNewValue = $mixNewValue;
	}
	
	public function setAttributesOfContainer( $arrAttributesNewValues)
	{
		return $this->arrAttributesValues = $arrAttributesNewValues;
	}
	
	public function setMethod( $strMethod )
	{
		switch( $strMethod )
		{
			case 'update':
			case 'remove':
			case 'insert':
				case 'analiseRemotionImpact':
				{
				$this->strMethod = $strMethod;
				break;			
			}
			default:
			{
				throw new Exception( 'Metodo invalido' , 0 );
			}
		}
	}
	
	public function getMethod()
	{
		return $this->strMethod;
	}
	
	public function getAttributesNewValues()
	{
		return $this->arrAttributesNewValues;	
	}
	
	public function setChangedEntities( $arrChangedEntities )
	{
		$this->arrChangedEntities = $arrChangedEntities;
	}
	
	public function setRemovedEntities( $arrRemovedEntities )
	{
		$this->arrRemovedEntities = $arrRemovedEntities;
	}
	
	public function setId( $intIdObjectContainer )
	{
		$this->intIdObject = $intIdObjectContainer;
	}
	
	protected function validate()
	{
		if	(	
				( $this->intIdObject === null )
			||	
				( $this->strContainerClassName === null )
			||	
				( $this->strMethod === null )
			||	
				( $this->strAttributeName === null )
			||	
				( $this->mixNewValue === null )
			)
		{
			throw new Exception( 'Parametros invalidos' , 1 );	
		}
				
		if( ! class_exists( $this->strContainerClassName ) )
		{
			throw new Exception( 'A classe ' . $this->strContainerClassName . ' nao pode ser encontrada ' , 2 );
		}
		
		$this->objElement = new $this->strContainerClassName();
		if( $this->intIdObject )
		{
			$this->objElement = $this->objElement->getInstanceById( $this->intIdObject );
		}
		
		if( !is_a( $this->objElement , 'AbstractEntity' ) )
		{
			throw new Exception( 'A classe ' . $this->strContainerClassName . ' nao eh uma entidade do sistema ' , 3 );
		}
	}
	
	protected function update()
	{
		$this->objElement->__toActiveFrozenField( $this );
		
		$this->arrAttributesOriginalValues = $this->arrAttributesValues;
		
		if( ! array_key_exists( $this->strAttributeName ,  $this->arrAttributesOriginalValues) ) 
		{
			throw new Exception( 'O Campo ' . $this->strAttributeName . ' nao foi encontrado ou nao pode acessado na classe ' . get_class( $this->objElement ) , 5 );
		}
		
		$strType = gettype( $this->arrAttributesOriginalValues[ $this->strAttributeName ] );
		
		$objReceivedOriginalValue = unxmlentities( $this->mixOriginalValue );
		settype( $objReceivedOriginalValue  , $strType );
		
		$objGetterOriginalValue = unxmlentities( $this->arrAttributesOriginalValues[ $this->strAttributeName ] );
		settype( $objGetterOriginalValue  , $strType );
				
		$mixNewValue = unxmlentities( $this->mixNewValue );
		settype( $mixNewValue , $strType );
			
		if( $mixNewValue != $objGetterOriginalValue )
		{
			$this->arrAttributesNewValues = array( $this->strAttributeName => $mixNewValue );
		}
		$this->objElement->__fromActiveFrozenField( $this );
	}
	
	protected function remove()
	{
		$this->objElement->__fromActiveFrozenField( $this );
	}
	
	protected function insert()
	{
		if( $this->objElement->getId() )
		{
			// adicionando uma filha ao objelement //
			$this->objElement->appendChild();
		}
		else
		{
			// adicionando o objelement ao seu pai de contexto //
			$this->objElement->append( $this->mixOriginalValue );
		}
		$this->objElement->__fromActiveFrozenField( $this );
	}
	
	protected function analiseRemotionImpact()
	{
		$this->arrWarnings = $this->objElement->analiseRemotionImpact();
	}
	
	public function apply()
	{
		$arrReturn = array();
		try
		{
			$this->validate();
			
			switch( $this->strMethod )
			{
				case 'update':
				case 'analiseRemotionImpact':
				case 'remove':
				{
					if( $this->objElement->getId() == null )
					{
						throw new Exception( 'Elemento nao encontrado na persistencia.' , 4 );
					}
					break;
				}
			}
			
			switch( $this->strMethod )
			{
				case 'update':
				{
					$this->update();
					break;
				}
				case 'analiseRemotionImpact':
				{
					//dbg( __LINE__ , 1 );
					$this->analiseRemotionImpact();
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
					///dbg( __LINE__ , 1 );
					$this->insert();
					break;		
				}
				default:
				{
					//dbg( __LINE__ , 1 );
					throw new Exception( 'Tipo de metodo invalido ' , 5 );
				}
			}
			//dbg( __LINE__ , 1 );
			
			//dbg( sizeof( $this->arrChangedEntities ) , 1 );
			
			foreach( $this->arrChangedEntities as $arrClassInstance )
			{
				foreach( $arrClassInstance as $objChangedElement )
				{
					$objChangedElement->__toActiveFrozenField( $this );
					$this->arrAttributesValues[ 'id' ] = $objChangedElement->getId();
					$this->arrAttributesValues[ 'className' ] = get_class( $objChangedElement );
					$arrReturn[] = $this->arrAttributesValues;
				}
			}
			
			foreach( $this->arrRemovedEntities as $arrClassInstance )
			{
				foreach( $arrClassInstance as $objChangedElement )
				{
					$arrRemovedEntity = array();
					$arrRemovedEntity[ 'id' ] = $objChangedElement->getId();
					$arrRemovedEntity[ 'className' ] =  get_class( $objChangedElement );
					$arrRemovedEntity[ 'removed' ] = true; 
					$arrReturn[] = $arrRemovedEntity;
				}
			}
			
			foreach( $this->arrWarnings as $strWarning )
			{
				$arrReturn[] = xmlentities( $strWarning );
			}
			
			AbstractEntity::updateAllChangedEntities();
			AbstractEntity::deleteAllRemovedEntities();
		}
		catch( Exception $objError )
		{
			$arrError = array();
			$arrError[ 'className' ]		= get_class( $objError );
			$arrError[ 'message' ]			= xmlentities( $objError->getMessage() );
			$arrError[ 'code' ] 			= $objError->getCode();
			$arrError[ 'id' ]				= $this->intIdObject;
			$arrError[ 'classChanged' ]		= $this->strContainerClassName;
			$arrError[ 'originalValue' ]	= xmlentities( $this->mixOriginalValue !== null ? $this->mixOriginalValue : '' );
			$arrError[ 'newValue' ]			= xmlentities( $this->mixNewValue !== null ? $this->mixNewValue : '' );
			$arrError[ 'attributeName' ]	= xmlentities( $this->strAttributeName );
			$arrReturn = $arrError;
		}
		return $arrReturn;		
	}
}
?>