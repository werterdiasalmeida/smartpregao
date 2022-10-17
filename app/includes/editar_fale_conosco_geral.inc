<?

################################### CABECALHO PADRAO DO SISTEMA ##############################################

$modulo=$_REQUEST['modulo'];
include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo('Editar Fale Conosco','<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

##############################################################################################################

$intAno 	= $_SESSION['exercicio'];
$intSisId	= $_SESSION['sisid'];

function montaFormularioRecursivoAction()
{
	return	
			$_SESSION['sisarquivo'] . ".php"	.	"?" .
			"modulo="	. $_REQUEST['modulo'] 	.	"&" .
			"acao="		.  $_REQUEST['acao'];
			
}

function pegaDadosFaleConosco( $intSisId )
{
	global $db;
	
	$strSql = "	SELECT
					*
				FROM
					public.fale_conosco as fale_conosco
				WHERE
					fale_conosco.sisid = {$intSisId}
				";
	$arrDadosFaleConosco = $db->pegaLinha( $strSql );
	
	if( $arrDadosFaleConosco === FALSE )
	{
		$arrDadosFaleConosco = array();
		$arrDadosFaleConosco[ 'flcid' ]			= null;	
		$arrDadosFaleConosco[ 'flcemail' ]		= '';	
		$arrDadosFaleConosco[ 'flcconteudo' ]	= '';	
		$arrDadosFaleConosco[ 'flcdata' ]		= '';	
		$arrDadosFaleConosco[ 'flchora' ]		= '';	
		$arrDadosFaleConosco[ 'usucpf' ]		= '';	
		$arrDadosFaleConosco[ 'sisid' ]			= '';	
	}
	
	return $arrDadosFaleConosco;
}

function indicaObrigatorio()
{
	?>
		<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'>
	<?
}

function preparaTelaEditarFaleConosco( $arrDadosFaleConosco )
{
	?>
		<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
		<script language="JavaScript">
		//Editor de textos
		tinyMCE.init({
			mode : "textareas",
			theme : "advanced",
			plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
			theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,forecolor,backcolor,fontsizeselect,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist, code",
			theme_advanced_buttons2 : "",
			theme_advanced_buttons3 : "",
			theme_advanced_toolbar_location : "top",
			theme_advanced_toolbar_align : "left",
			extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
			language : "pt_br",
			entity_encoding : "raw"
			});
		</script>
		<style>
			.tabelaFaleConosco th
			{
				vertical-align: top;
				padding-top: 5px;
				font-size: 11px;
				font-weight: normal;
				text-align: right;
			}
		</style>
		<form action=<?= montaFormularioRecursivoAction() ?> method="post" >
			<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela tabelaFaleConosco" border="0">
				<tr>
					<th style="width: 100px" class="SubTituloDireita">
						<label for="mensagem">
							Mensagem:
						</label>
					</th>
					<td style="width:600px">
						<textarea name="mensagem" style="width: 600px; height: 200px" ><?= $arrDadosFaleConosco[ 'flcconteudo' ] ?></textarea>
					</td>
					<td>
						<? indicaObrigatorio() ?>
					</td>
					<td>
					</td>
				</tr>
				<tr>
					<th class="SubTituloDireita">
						<label for="email" >
							E-mail:
						</label>
					</th>
					<td>
						<input type="text" name="email" style="width: 600px" value="<?= $arrDadosFaleConosco[ 'flcemail' ] ?>" />
					</td>
					<td>
						<? indicaObrigatorio() ?>
					</td>
				</tr>
				<tr>
					<th class="SubTituloDireita">
					</th>
					<td colspan="2">
						<input type="submit" value="Enviar" name="evento" />
					</td>
				</tr>
			</table>
		</form>
	<?
}

function atualizaDadosFaleConosco( $arrFaleConoscoOriginal, $strMensagem , $strEmail , $intSisId )
{
	global $db;
	//$db = new cls_banco();
	
	if( $arrFaleConoscoOriginal[ 'flcid' ] !== null )
	{
		// nao exisitia fale conosco original para este sis id //
		//dbg( $strEmail  );
		$strSql =	sprintf(
			" update
				 public.fale_conosco
			 set
				 flcemail = '%s' ,
				 flcconteudo = '%s' ,
				 flcdata = '%s' ,
				 flchora = '%s' ,
				 usucpf = '%s'
			 where
				 sisid = '%s' 
			and
				 flcid = '%s'	
			",
			( $strEmail ),
			( $strMensagem ),
			( date( 'Y-m-d' ) ),
			( date( 'h:i' ) ),
			( $_SESSION['usucpforigem'] ),
			( $intSisId ),
			( $arrFaleConoscoOriginal[ 'flcid' ] )
		);
	}
	else
	{
		$strSql =	sprintf(
			" insert into							
					public.fale_conosco
				 	 (
						 flcemail,
						 flcconteudo,
						 flcdata,
						 flchora,
						 usucpf,
						 sisid
					 )
				 values
					 (
						 '%s',
						 '%s',
						 '%s',
						 '%s',
						 '%s',
						 '%s'
					 )
			",
			( $strEmail ),
			( $strMensagem ),
			( date( 'Y-m-d' ) ),
			( date( 'h:i' ) ),
			( $_SESSION['usucpforigem'] ),
			( $intSisId )
		);
	}
	
	$db->executar( $strSql );
	$db->commit();
}

if( $_SESSION["superuser"] )
{
	switch( @$_REQUEST[ "evento" ] )
	{
		case 'Enviar':
		{
			$strMensagem	= @$_REQUEST[ "mensagem" ];
			$strEmail		= @$_REQUEST[ "email" ];
			
			$arrFaleConosco = pegaDadosFaleConosco( $intSisId );
			atualizaDadosFaleConosco( $arrFaleConosco, $strMensagem , $strEmail , $intSisId );
			$arrFaleConosco = pegaDadosFaleConosco( $intSisId );
			preparaTelaEditarFaleConosco( $arrFaleConosco );
			break;
		}
		default:
		{
			$arrFaleConosco = pegaDadosFaleConosco( $intSisId );
			preparaTelaEditarFaleConosco( $arrFaleConosco );
			break;
		}
	}
}
else
{
	echo "<script>
			alert('Seu perfil não tem permissão de acesso a esta página.');
			history.back(-1);
		  </script>";
	die;
}

?>