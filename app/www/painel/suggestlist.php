<?
	
	// carrega as funções gerais
	include_once "config.inc";
	include_once APPRAIZ . "includes/funcoes.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";
	
	header("Content-Type: text/html; charset=ISO-8859-1");

/**
 * Contraliza as requisições ajax do módulo.  
 *
 * @author Renê de Lima Barbosa <renebarbosa@mec.gov.br> 
 * @since 25/05/2007
 */

function erro( $codigo, $mensagem, $arquivo, $linha ){
	//echo "$codigo, $mensagem, $arquivo, $linha";
	echo "Ocorreu um erro. Por favor tente mais tarde.";
	exit();
}

function excecao( Exception $excecao ){
	//echo $excecao->getMessage();
	echo "Ocorreu um erro. Por favor tente mais tarde.";
	exit();
}

set_error_handler( 'erro', E_USER_ERROR );
set_exception_handler( 'excecao' );

$strEvent = $_REQUEST[ 'rs' ];
$arrArgs =  $_REQUEST[ 'rsargs' ];

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

function pegaArrayDeUsuariosComNomesProximos( $strWord )
{
	global $db;
	
	$arrSpecialsLower = array(
			"á" , "à" , "â" , "ã" , "ä" , 
			"é" , "è" , "ê" , 		"ë" ,
			"í" , "ì" , "î" , 		"ï" ,
			"ó" , "ò" , "ô" , "õ" , "ö" ,
			"ú" , "ù" , "û"       , "ü" ,
			"ç" 
		);
				
	$arrSpecialsUpper = array(
			"Á" , "À" , "Â" , "Ã" , "Ä" , 
			"É" , "È" , "Ê" , 		"Ë" ,
			"Í" , "Ì" , "Î" , 		"Ï" ,
			"Ó" , "Ò" , "Ô" , "Õ" , "Ö" ,
			"Ú" , "Ù" , "Û"       , "Ü" ,
			"Ç" 
			);
	$arrRegular = array(
			"A" , "A" , "A" , "A" , "A" , 
			"E" , "E" , "E" , 		"E" ,
			"I" , "I" , "I" , 		"I" ,
			"O" , "O" , "O" , "O" , "O" ,
			"U" , "U" , "U"       , "U" ,
			"C" 
			);
			
	$strWord = trim( $strWord );
	$strWord = strtoupper( $strWord );
	$strWord = str_replace( $arrSpecialsLower , $arrSpecialsUpper , $strWord );
	$strWordRegular = str_replace( $arrSpecialsUpper , $arrRegular , $strWord );

	$strSql =	sprintf(	" 
				SELECT
					r.respid,
					r.respnome,
					r.respemail
				FROM
					painel.responsavelsecretaria AS r
				WHERE
					r.respnome LIKE %s
				OR
					r.respnome LIKE %s
				ORDER BY
					r.respnome 
				LIMIT 10		
				",
					$db->escape( '' . $strWord . '%' ),
					$db->escape( '' . $strWordRegular . '%' ),
					$db->escape( '' . strtolower( $strWordRegular ) . '%' )
				);
	$arrUsuarios = $db->carregar( $strSql );
	$arrUsuarios = $arrUsuarios ? $arrUsuarios : array();
	
	$arrUsuariosId = array();
	foreach( $arrUsuarios as $arrUsuario )
	{
		$arrUsuariosId[] = $arrUsuario[ 'respid' ]; 
	}
	
	$strSql =	sprintf(	" 
				SELECT
					r.respid,
					r.respnome,
					r.respemail
				FROM
					painel.responsavelsecretaria AS r
				WHERE
				(
						r.respnome LIKE %s
					OR
						r.respnome LIKE %s
					OR
						r.respemail LIKE %s
				)
				".(($arrUsuariosId)?"AND r.respid NOT IN ( %s )":"")."
				ORDER BY
					r.respnome 
				LIMIT 10		
				",
					$db->escape( '%' . $strWord . '%' ),
					$db->escape( '%' . $strWordRegular . '%' ),
					$db->escape( '%' . strtolower( $strWordRegular ) . '%' ) ,
					$db->escape( implode( ',' , $arrUsuariosId ) )
				);
	$arrUsuariosNomeMeio = $db->carregar( $strSql );
	$arrUsuariosNomeMeio = $arrUsuariosNomeMeio ? $arrUsuariosNomeMeio : array();

	$arrUsuariosFinal = array_merge( $arrUsuarios , $arrUsuariosNomeMeio );
	
	foreach( $arrUsuariosFinal as &$arrUsuario )
	{
		foreach( $arrUsuario as $strKey => $mixValue )
		{
			if( $strKey == 'respnome')
			{
				$mixValue = caseAsName( $mixValue );
				$strCaseWord = caseAsName( $strWord );
				$arrUsuario[ $strKey ] = str_replace( $strCaseWord , '<b>' . $strCaseWord . '</b>' , $mixValue );
			}
			else
			{
				$arrUsuario[ $strKey ] = str_replace( $strWord , '<b>' . $strWord . '</b>' , $mixValue );
			}
		} 
	}
	
	$arrUsuariosFinal = array_slice( $arrUsuariosFinal , 0, 10);
	return $arrUsuariosFinal;
}

function caseAsName( $strText )
{
	$arrSpecialsLower = array(
			"á" , "à" , "â" , "ã" , "ä" , 
			"é" , "è" , "ê" , 		"ë" ,
			"í" , "ì" , "î" , 		"ï" ,
			"ó" , "ò" , "ô" , "õ" , "ö" ,
			"ú" , "ù" , "û"       , "ü" ,
			"ç" 
		);
				
	$arrSpecialsUpper = array(
			"Á" , "À" , "Â" , "Ã" , "Ä" , 
			"É" , "È" , "Ê" , 		"Ë" ,
			"Í" , "Ì" , "Î" , 		"Ï" ,
			"Ó" , "Ò" , "Ô" , "Õ" , "Ö" ,
			"Ú" , "Ù" , "Û"       , "Ü" ,
			"Ç" 
			);
				
	$arrWords = explode( " " , $strText );
	foreach( $arrWords as &$strWord )
	{
		$strWord = str_replace( $arrSpecialsUpper , $arrSpecialsLower , $strWord );
		$strWord = ucfirst( strtolower( $strWord ) );
	}
	$strText = implode( " " , $arrWords );
	return $strText;	
}

function PrepareSuggestList( $strWord , $intIdSuggest )
{
	$arrUsuarios = pegaArrayDeUsuariosComNomesProximos( $strWord );
	?>
		<div style="width:500px">
			<? foreach ( $arrUsuarios as $intKey => $arrUsuario ): ?>
				<div style=""
				onmouseover="window.Suggest.arrInstances[<?= $intIdSuggest ?>].mouseOverSuggest(<?= $intKey ?>)"
				onclick="window.Suggest.arrInstances[<?= $intIdSuggest ?>].clickSuggest(<?= $intKey ?>)"
				>
					<?=  ( $arrUsuario[ 'respnome' ] ) ?> &lt;<?=$arrUsuario[ 'respemail' ] ?>&gt;
				</div>
			<? endforeach ?>
		</div>
	<?
}

try
{
	switch( $strEvent )
	{
		case 'SuggestsUsuario':
		{
			if( sizeof( $arrArgs ) < 0 )
			{
			//	throw new Exception( 'Parametros Invalidos' );
			}
			$strWord		= $arrArgs[ 0 ];
			$intIdSuggest	= $arrArgs[ 1 ]; 
			PrepareSuggestList( $strWord , $intIdSuggest);
			break; 
		}
		default:
		{
			throw new Exception( 'Parametros Invalidos' );
			break;		
		}
	}
}
catch( Exception $objError )
{
	excecao( $objError );
}
?>