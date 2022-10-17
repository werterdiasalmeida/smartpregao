<?

/**
 <sql>
	------------------------------------ MANUAL ------------------------------------
	-- Sequence: manual_manid_seq
	
	-- DROP SEQUENCE public.manual_manid_seq;
	
	CREATE SEQUENCE public.manual_manid_seq
	  INCREMENT 1
	  MINVALUE 1
	  MAXVALUE 9223372036854775807
	  START 1
	  CACHE 1;
	ALTER TABLE public.manual_manid_seq OWNER TO postgres;
	
	
	-- Table: manual
	
	-- DROP TABLE public.public.manual
	
	CREATE TABLE public.manual
	(
	  manid		int4 			NOT NULL DEFAULT nextval('manual_manid_seq'::regclass),
	  mannome	varchar(255)	NOT NULL,
	  mandata	date			NOT NULL,
	  manhora	char(8)			NOT NULL,

	  arqid		int4					,
	  usucpf	char(11)		NOT NULL,
	  sisid		int4			NOT NULL,
	
	  CONSTRAINT pk_manual PRIMARY KEY (manid),
	
	  CONSTRAINT fk_manual_usucpf FOREIGN KEY (usucpf)
			REFERENCES seguranca.usuario (usucpf) MATCH SIMPLE
			ON UPDATE RESTRICT ON DELETE RESTRICT,
	
	  CONSTRAINT fk_manual_sisid FOREIGN KEY (sisid)
			REFERENCES seguranca.sistema (sisid) MATCH SIMPLE
			ON UPDATE RESTRICT ON DELETE RESTRICT,
	
	  CONSTRAINT fk_manual_arqid FOREIGN KEY (arqid)
			REFERENCES public.arquivo (arqid) MATCH SIMPLE
			ON UPDATE RESTRICT ON DELETE RESTRICT
	
	) 
	WITH OIDS;
	ALTER TABLE public.manual OWNER TO postgres;
	COMMENT ON TABLE manual IS 
		'Guarda as informacoes do manual para cada subsistema e tambem quem e quando as alterou';
	
	ALTER TABLE public.manual ADD UNIQUE (sisid);
 </sql>
**/

################################### CABECALHO PADRAO DO SISTEMA ##############################################

$modulo=$_REQUEST['modulo'];
include APPRAIZ."includes/cabecalho.inc";
include APPRAIZ."includes/arquivo.inc";
?>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo('Editar Manual','<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

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

function pegaDadosManual( $intSisId )
{
	global $db;
	
	$strSql = "	 select
					 *
				 from
					 public.manual as manual
				 WHERE
					 manual.sisid = {$intSisId}
				";
	$arrDadosManual = $db->pegaLinha( $strSql );
	
	if( $arrDadosManual === FALSE )
	{
		$arrDadosManual = array();
		$arrDadosManual[ 'manid' ]			= null;	
		$arrDadosManual[ 'mannome' ]		= '';	
		$arrDadosManual[ 'arqid' ]			= '';	
		$arrDadosManual[ 'mandata' ]		= '';	
		$arrDadosManual[ 'manhora' ]		= '';	
		$arrDadosManual[ 'usucpf' ]			= '';	
		$arrDadosManual[ 'sisid' ]			= '';	
	}
	
	return $arrDadosManual;
}

function indicaObrigatorio()
{
	?>
		<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'>
	<?
}

function preparaTelaEditarManual( $arrDadosManual , $arrMensagens = array() )
{
	$arrArquivoBanco = pegaArquivoPeloId( $arrDadosManual[ 'arqid' ]  );
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
			.tabelaFaleManual th
			{
				vertical-align: top;
				padding-top: 5px;
				font-size: 11px;
				font-weight: normal;
				text-align: right;
			}
		</style>
		<form enctype="multipart/form-data" action=<?= montaFormularioRecursivoAction() ?> method="post" >
		<center>
			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<? if( sizeof( $arrMensagens ) > 0 ): ?>
					<tr>
						<td  class="SubTituloDireita" colspan="2" style="text-align: center;color: #dd0000;">
							<?foreach( $arrMensagens as $strMensagem ): ?>
								<?= $strMensagem ?><br/>
							<?endforeach ?>
						</td>
					</tr>
				<? endif ?>
				<? if( $arrDadosManual[ 'arqid' ] && $arrArquivoBanco[ "arqnome" ] ): ?>
					<tr>
						<th style="" class="SubTituloDireita">
							<label for="arquivo">
								Arquivo Atual:
							</label>
						</th>
						<td style="text-align: left;">
							<a
								href="../mostra_arquivo.php?id=<?= $arrDadosManual[ 'arqid' ] ?>"
							><?= $arrArquivoBanco[ "arqnome" ] ?>.<?= $arrArquivoBanco[ "arqextensao" ] ?></a>
							<? geraLinkParaArquivo( $arrDadosManual[ 'arqid' ] )  ?>
							<? geraLinkParaRemoverArquivo( $arrDadosManual[ 'arqid' ] ) ?>
						</td>
					</tr>
				<? endif ?>
				<tr>
					<td class="SubTituloDireita">
						<label for="nome" >
							Arquivo Novo:
						</label>
					</td>
					<td>
						<input type="file" name="arquivo" />
					</td>	
				</tr>
				<tr>
					<td class="SubTituloDireita">
						Descrição:
					</td>
					<td>
						<input style="width: 190px" type="text" name="nome" value="<?= $arrDadosManual[ 'mannome' ] ?>" />
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">&nbsp;</td>
					<td class="SubTituloDireita" style="text-align: left;" colspan="2">
						<input type="submit" value="Enviar" name="evento" />
					</td>
				</tr>
			</table>
		</center>
		</form>
	<?
}

function atualizaDadosManual( $arrDadosManualOriginal, $arrArquivo, $strNovoNome, 
	$boolRemoveArquivoAtual , $intSisId )
{
	global $db;
	$strUsuarioCpf = $_SESSION['usucpforigem'];
	
	if( is_array( $arrArquivo ) )
	{
		if( $arrDadosManualOriginal[ "arqid" ] )
		{
			// removendo o arquivo anterior //
			$arrDadosArquivoAntigo = pegaArquivoPeloId( $arrDadosManualOriginal[ "arqid" ] );
			removeArquivo( $arrDadosManualOriginal[ "arqid" ], $arrDadosArquivoAntigo['arqnome'] );
		}
		// ira incluir o arquivo //
		$intArquivoId = salvarArquivo( $strUsuarioCpf , $intSisId , $arrArquivo,  getTiposArquivoDocumento() , "Manual" );
		
		if( $strNovoNome == '' )
		{
			$strNovoNome = $arrArquivo[ "name" ];
		}
	}
	else
	{
		// nao ira incluir um novo arquivo //
		if( $boolRemoveArquivoAtual )
		{
			// ira remover o arquivo existente - se existir //
			if( $arrDadosManualOriginal[ "arqid" ] )
			{
				removeArquivo( $arrDadosManualOriginal[ "arqid" ] );
			}
		}
		$strNovoNome = null;
		$intArquivoId = null;
	}
	
	if( $arrDadosManualOriginal[ 'manid' ] != null )
	{
		
		// ja exisitia manual para este sis id //
		$strSql =	sprintf(
			" update
				 public.manual
			 set
				 mannome	= %s ,
				 mandata	= %s ,
				 manhora	= %s ,
				 arqid		= %d ,
				 usucpf		= %s
			 where
				 sisid		= %s 
			and
				 manid		= %s	
			",
				$db->escape( $strNovoNome ),
				$db->escape( date( 'Y-m-d' ) ),
				$db->escape( date( 'h:i' ) ),
				$intArquivoId ? $intArquivoId : 'null',
				$db->escape( $strUsuarioCpf ),
				$db->escape( $intSisId ),
				$db->escape( $arrDadosManualOriginal[ 'manid' ] )
		);
	}
	else
	{
		// nao exisitia manual para este sis id //
		$strSql =	sprintf(
			" insert into							
					public.manual
					 (
						 mannome,
						 mandata,
						 manhora,
						 arqid,
						 usucpf,
						 sisid
					 )
				 values
					 (
						 %s,
						 %s,
						 %s,
						 %s,
						 %s,
						 %s
					 )
			",
				$db->escape( $strNovoNome ),
				$db->escape( date( 'Y-m-d' ) ),
				$db->escape( date( 'h:i:s' ) ),
				$db->escape( $intArquivoId ),
				$db->escape( $strUsuarioCpf ),
				$db->escape( $intSisId )
		);
	}
	
	$db->executar( $strSql );
	$db->commit();
}

switch( @$_REQUEST[ "evento" ] )
{
	case 'Enviar':
	{
		$arrMensagens = array();
		try
		{
			$strNome		= @$_REQUEST[ "nome" ];
			$arrArquivo		= @$_FILES[ "arquivo" ];
			$arrManual = pegaDadosManual( $intSisId );
			atualizaDadosManual( $arrManual, $arrArquivo , $strNome , false , $intSisId );
			$arrMensagens[] = "Manual cadastrado com sucesso";
		}
		catch( FileException $objError )
		{
			$arrMensagens[] = $objError->getMessage();
		}
		$arrManual = pegaDadosManual( $intSisId );
		preparaTelaEditarManual( $arrManual , $arrMensagens );
		break;
	}
	default:
	{
		$arrManual = pegaDadosManual( $intSisId );
		preparaTelaEditarManual( $arrManual );
		break;
	}
}
?>