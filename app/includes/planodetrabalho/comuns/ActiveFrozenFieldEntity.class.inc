<?php
class ActiveFrozenFieldEntity
{
	public static function isActiveFrozenField( $strAttribute , $strClassName )
	{
		$oReflection = new ReflectionProperty( $strClassName , $strAttribute );
		return ereg( '[[:space:]]@activeFrozenField[[:space:]]', $oReflection->getDocComment() ) > 0;
	}
	
	protected static function loopJsonFieldGenerator( &$arrAttributes , $strClassName )
	{
		foreach( $arrAttributes as $strAttributeName => $mixValue )
		{
			if ( self::isActiveFrozenField( $strAttributeName , $strClassName ) )
			{
				if( $arrAttributes[ $strAttributeName ] === null )
				{
					$arrAttributes[ $strAttributeName ] = '';
				}
				switch( gettype( $arrAttributes[ $strAttributeName ] ) )
				{
					case 'integer':
					case 'double':
					case 'boolean':
					{
						break;	
					}
					case 'string':
					{
						$arrAttributes[ $strAttributeName ] = xmlentities( $arrAttributes[ $strAttributeName ] ) ;
						break;							
					}
					case 'object':
					{
						if( is_a( $arrAttributes[ $strAttributeName ] , 'InterfaceEntityValue' ) )
						{
							$arrAttributes[ $strAttributeName ] = xmlentities( $arrAttributes[ $strAttributeName ]->getValueToTemplate() ) ; 
						}
						else
						{
							unset( $arrAttributes[ $strAttributeName ] );
						}
						break;	
					}
					case 'array':
					{
						self::loopJsonFieldGenerator( $arrAttributes[ $strAttributeName ], $strClassName );
						break;	
					}
				}
			}
			else
			{
				unset( $arrAttributes[ $strAttributeName ] );
			}
		}
	}	
	
	public static function __toJson( $arrAttributes , $strClassName )
	{
		self::loopJsonFieldGenerator( $arrAttributes , $strClassName );
		return $arrAttributes;
	}	
}
?>