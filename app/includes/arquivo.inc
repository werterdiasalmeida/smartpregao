<?php
define( "PASTA_ARQUIVOS" , APPRAIZ . "arquivos/sistema/" );
define( "QTD_MAX_ARQUIVOS_PAGINA" , 1000 );

/**
 * <sql>
  
	----------------------------------- ARQUIVO ------------------------------------
	-- Sequence: public.arquivo_arqid_seq
	
	-- DROP SEQUENCE public.arquivo_arqid_seq;
	
	CREATE SEQUENCE public.arquivo_arqid_seq
	  INCREMENT 1
	  MINVALUE 1
	  MAXVALUE 9223372036854775807
	  START 1
	  CACHE 1;
	ALTER TABLE public.arquivo_arqid_seq OWNER TO postgres;
	
	-- Table: public.arquivo
	
	-- DROP TABLE public.arquivo
	
	CREATE TABLE public.arquivo
	(
	  arqid			int4 NOT NULL DEFAULT nextval('arquivo_arqid_seq'::regclass),
	  arqnome		varchar(255) NOT NULL,
	  arqdescricao  varchar(255),
	  arqextensao	varchar(255),
	  arqtipo		varchar(255) NOT NULL,
	  arqtamanho	int8,
	  arqdata		date,
	  arqhora		char(8),
	  arqstatus		char(1),

	  usucpf		char(11),
	  sisid			int4,
	
	  CONSTRAINT pk_arquivo_arqid PRIMARY KEY (arqid),
	
	  CONSTRAINT fk_arquivo_usucpf FOREIGN KEY (usucpf)
	      REFERENCES seguranca.usuario (usucpf) MATCH SIMPLE
	      ON UPDATE RESTRICT ON DELETE RESTRICT,
	
	  CONSTRAINT fk_arquivo_sisid FOREIGN KEY (sisid)
	      REFERENCES seguranca.sistema (sisid) MATCH SIMPLE
	      ON UPDATE RESTRICT ON DELETE RESTRICT
	
	) 
	WITH OIDS;
	ALTER TABLE public.arquivo OWNER TO postgres;
	COMMENT ON TABLE arquivo IS 
		'Guarda as informacoes do arquivo inserido no sistema';
	
 * </sql>
 */


function getTiposImagemWeb()
{
	$arrTiposImagemWeb = array();
	$arrTiposImagemWeb[] = 'image/jpeg';
	$arrTiposImagemWeb[] = 'image/gif';
	$arrTiposImagemWeb[] = 'image/png';
	$arrTiposImagemWeb[] = 'image/x-jpeg';
	$arrTiposImagemWeb[] = 'image/x-gif';
	$arrTiposImagemWeb[] = 'image/x-png';
	return $arrTiposImagemWeb;
}

function getTiposImagemWebExtendido()
{
	$arrTiposImagemWebExtendido		= getTiposImagemWeb();
	$arrTiposImagemWebExtendido[]	= 'image/pjpeg';
	$arrTiposImagemWebExtendido[]	= 'image/tiff';
	return $arrTiposImagemWebExtendido;
}

function getTiposArquivoImagem()
{
	$arrTiposArquivoImagem = getTiposImagemWebExtendido();
	$arrTiposArquivoImagem[] = 'image/pict';
	$arrTiposArquivoImagem[] = 'image/pjpeg';
	$arrTiposArquivoImagem[] = 'image/tiff';
	$arrTiposArquivoImagem[] = 'image/vnd.wap.wbmp';
	$arrTiposArquivoImagem[] = 'image/bmp';
	$arrTiposArquivoImagem[] = 'image/jutvision';
	$arrTiposArquivoImagem[] = 'image/x-windows-bmp';
	$arrTiposArquivoImagem[] = 'image/x-tiff';
	$arrTiposArquivoImagem[] = 'image/x-xbitmap';
	$arrTiposArquivoImagem[] = 'image/x-pict';
	$arrTiposArquivoImagem[] = 'image/x-jps';
	return $arrTiposArquivoImagem();
}

function getTiposArquivoVideo()
{
	$arrTiposArquivoVideo = array();
	$arrTiposArquivoVideo[] = 'application/x-troff-msvideo';
	$arrTiposArquivoVideo[] = 'video/avi';
	$arrTiposArquivoVideo[] = 'video/mpeg';
	$arrTiposArquivoVideo[] = 'video/msvideo';
	$arrTiposArquivoVideo[] = 'video/x-avi';
	$arrTiposArquivoVideo[] = 'video/x-mpeg';
	$arrTiposArquivoVideo[] = 'video/x-msvideo';
	$arrTiposArquivoVideo[] = 'video/x-sgi-movie';
return $arrTiposArquivoVideo;
}

function getTiposArquivoCompresso()
{
	$arrTiposArquivoCompresso = array();
	$arrTiposArquivoCompresso[] = 'multipart/zip';
	$arrTiposArquivoCompresso[] = 'multipart/gzip';
	$arrTiposArquivoCompresso[] = 'application/gtar';
	$arrTiposArquivoCompresso[] = 'application/zip';
	$arrTiposArquivoCompresso[] = 'application/gzip';
	$arrTiposArquivoCompresso[] = 'application/compress';
	$arrTiposArquivoCompresso[] = 'application/zip-compressed';
	$arrTiposArquivoCompresso[] = 'application/x-gtar';
	$arrTiposArquivoCompresso[] = 'application/x-gzip';
	$arrTiposArquivoCompresso[] = 'application/x-compress';
	$arrTiposArquivoCompresso[] = 'application/x-zip-compressed';
	$arrTiposArquivoCompresso[] = 'multipart/x-zip';
	$arrTiposArquivoCompresso[] = 'multipart/x-gzip';

	return $arrTiposArquivoCompresso;
}

function getTiposArquivoPlanilha()
{
	$arrTiposArquivoPlanilha = array();
	$arrTiposArquivoPlanilha[] = 'application/excel';
	$arrTiposArquivoPlanilha[] = 'application/x-excel';
	$arrTiposArquivoPlanilha[] = 'application/x-msexcel';
	$arrTiposArquivoPlanilha[] = 'application/vnd.ms-excel';
	$arrTiposArquivoPlanilha[] = 'application/vnd.oasis.opendocument.spreadsheet';
	$arrTiposArquivoPlanilha[] = 'application/vnd.sun.xml.calc';

	return $arrTiposArquivoPlanilha;
}

function getTiposArquivoDocumento()
{
	$arrTiposArquivoDocumento = array();
	$arrTiposArquivoDocumento[] = 'text/richtext';
	$arrTiposArquivoDocumento[] = 'text/plain';
	$arrTiposArquivoDocumento[] = 'application/msword';
	$arrTiposArquivoDocumento[] = 'application/wordperfect';
	$arrTiposArquivoDocumento[] = 'application/mswrite';
	$arrTiposArquivoDocumento[] = 'application/rtf';
	$arrTiposArquivoDocumento[] = 'application/x-wri';
	$arrTiposArquivoDocumento[] = 'application/x-world';
	$arrTiposArquivoDocumento[] = 'application/x-rtf';
	$arrTiposArquivoDocumento[] = 'application/pdf';
	$arrTiposArquivoDocumento[] = 'application/x-real';
	$arrTiposArquivoDocumento[] = 'application/vnd.oasis.opendocument.text';
	$arrTiposArquivoDocumento[] = 'application/vnd.oasis.opendocument.text-web';
	$arrTiposArquivoDocumento[] = 'application/vnd.sun.xml.writer';

	return $arrTiposArquivoDocumento;
}

function getTiposArquivoEscritorio()
{
	$arrTiposArquivoEscritorio = array_merge(
	getTiposArquivoDocumento()	,
	getTiposArquivoPlanilha()	,
	getTiposArquivoCompresso()
	);

	return $arrTiposArquivoEscritorio;
}

function getExtensaoPorTipo( $strTipo , $strExtensaoOriginal = null )
{
	switch( $strTipo )
	{
		case 'image/jpeg'					:
		case 'image/x-jpeg'					:
		case 'image/pjpeg'					:
			{
				return "jpg";
				break;
			}
		case 'image/gif'					:
		case 'image/x-gif'					:
			{
				return "gif";
				break;
			}
		case 'image/png'					:
		case 'image/x-png'					:
			{
				return "png";
				break;
			}
		case 'image/tiff'					:
		case 'image/x-tiff'					:
			{
				return "tif";
				break;
			}
		case 'image/pict'					:
		case 'image/x-pict'					:
			{
				return "pic";
				break;
			}
		case 'image/bmp'					:
		case 'image/x-windows-bmp'			:
		case 'image/x-xbitmap'				:
			{
				return "bmp";
				break;
			}
		case 'image/vnd.wap.wbmp'			:
			{
				return "wbmp";
				break;
			}
		case 'image/jutvision'				:
			{
				return "jut";
				break;
			}
		case 'image/x-jps'					:
			{
				return "jps";
				break;
			}
		case 'application/x-troff-msvideo'	:
		case 'video/avi'					:
		case 'video/mpeg'					:
		case 'video/msvideo'				:
		case 'video/x-msvideo'				:
		case 'video/x-avi'					:
			{
				return "avi";
				break;
			}
		case 'video/x-mpeg'					:
			{
				return "mpg";
				break;
			}
		case 'video/x-sgi-movie'			:
			{
				return "movie";
				break;
			}
		case 'multipart/zip'				:
		case 'application/zip'				:
		case 'application/compress'			:
		case 'application/zip-compressed'	:
		case 'application/x-compress'		:
		case 'application/x-zip-compressed'	:
		case 'multipart/x-zip'				:
			{
				return "zip";
				break;
			}
		case 'multipart/gzip'				:
		case 'application/gzip'				:
		case 'application/x-gzip'			:
		case 'multipart/x-gzip'				:
			{
				return "gzip";
				break;
			}
		case 'application/x-gtar'			:
		case 'application/gtar'				:
			{
				return "gtar";
				break;
			}
		case 'application/excel'			:
		case 'application/x-excel'			:
		case 'application/x-msexcel'		:
		case 'application/vnd.ms-excel'		:
			{
				return "xsl";
				break;
			}
		case 'text/richtext'				:
		case 'application/rtf'				:
			{
				return "rtf";
				break;
			}
		case 'text/plain'					:
			{
				return "txt";
				break;
			}
		case 'application/msword'			:
			{
				return "doc";
				break;
			}
		case 'application/wordperfect'		:
			{
				return "wp";
				break;
			}
		case 'application/mswrite'			:
		case 'application/x-wri'			:
			{
				return "wri";
				break;
			}
			case'application/x-world'			:
			{
				return "wrl";
				break;
			}
		case 'application/x-rtf'			:
			{
				return "rtf";
				break;
			}
		case 'application/pdf'				:
			{
				return "pdf";
				break;
			}
		case 'application/vnd.oasis.opendocument.text'			:
			{
				return "odt";
				break;
			}
		case 'application/vnd.oasis.opendocument.text-web'		:
			{
				return "oth";
				break;
			}
		case 'application/vnd.sun.xml.writer'					:
			{
				return "sxw";
				break;
			}
		case 'application/vnd.oasis.opendocument.spreadsheet'	:
			{
				return "ods";
				break;
			}
		case 'application/vnd.sun.xml.calc'						:
			{
				return "sxc";
				break;
			}
		default													:
			{
				return $strExtensaoOriginal;
				break;
			}
	}
}

class FileException extends Exception{};

function salvarArquivo( $strCpfUsuario , $intSisId , $arrArquivo ,	$arrTiposAceitos = array()	,
$strNomePadronizado = null	, $strDescricao	= null  	,
$intTamanhoMaximo	= null	, $intTamanhoMinimo	= null  )
{
	global $db;

	if( $arrArquivo['size'] == 0 )
	{
		return null;
	}


	if( $strCpfUsuario == null )
	{
		throw new FileException( "Cpf inválido ao se tentar salvar arquivo" );
	}

	if( $strNomePadronizado == null )
	{
		$arrNomeArquivo = explode( "."  , $arrArquivo[ "name" ] );
		$strNomeDownload = 	$arrNomeArquivo[0];
		$strExtensao = $arrNomeArquivo[1];
	}
	else
	{
		$arrNomeArquivo = explode( "."  , $strNomePadronizado );
		$strNomeDownload = 	$arrNomeArquivo[0];
		$strExtensao = $arrNomeArquivo[1];
	}

	if( $arrTiposAceitos != null )
	{
		if( !in_array( $arrArquivo[ "type" ] , $arrTiposAceitos ) )
		{
			throw new FileException( "Tipo de arquivo inválido" );
		}
	}

	$strTempFile = ( $arrArquivo[ "tmp_name" ] );
	$strTypeFile = $arrArquivo[ "type" ];
	$strExtensao = getExtensaoPorTipo( $strTypeFile , $strExtensao );

	$strFileContent = file_get_contents( $strTempFile );
	$intTamanhoArquivo = filesize( $strTempFile );
	if( $intTamanhoMaximo != null )
	{
		if( $intTamanhoArquivo > $intTamanhoMaximo )
		{
			throw new FileException( "Tamanho de Arquivo excede o máximo" );
		}
	}
	if( $intTamanhoMinimo != null )
	{
		if( $intTamanhoArquivo < $intTamanhoMinimo )
		{
			throw new FileException( "Tamanho de Arquivo muito pequeno" );
		}
	}


	$strSql = sprintf( "
		 insert into
		 arquivo
		 (
			 arqnome,
			 arqdescricao,
			 arqextensao,
			 arqtipo,
			 arqtamanho,
			 arqdata,
			 arqhora,
			 arqstatus,
			 usucpf,
			 sisid
		 )
		 values
		(
			%s,
			%s,
			%s,
			%s,
			%d,
			%s,
			%s,
			%s,
			%s,
			%s
		)
		returning arqid
	",
	$db->escape( $strNomeDownload ),
	$db->escape( $strDescricao ),
	$db->escape( $strExtensao ),
	$db->escape( $strTypeFile ),
	$db->escape( $intTamanhoArquivo ),
	$db->escape( date( 'Y-m-d' ) ),
	$db->escape( date( 'h:i:s' ) ),
	$db->escape( TRUE ),
	$db->escape( $strCpfUsuario ),
	$db->escape( $intSisId )
	);
	$objResultado = $db->executar( $strSql );
	$arrLinha = pg_fetch_assoc($objResultado);
	$intArquivoId = $arrLinha[ 'arqid' ];
	$intPasta = floor( $intArquivoId / QTD_MAX_ARQUIVOS_PAGINA );

	$strPasta = PASTA_ARQUIVOS . $intPasta;

	if( !is_dir( $strPasta) )
	{
		mkdir( $strPasta );
	}
	file_put_contents( $strPasta . "/" . $intArquivoId ."_". $strNomeDownload, $strFileContent );

	return $intArquivoId;
}

function pegaArquivoPeloId( $intArquivoId )
{
	global $db;

	$strSql = sprintf( "
		  select
			 *
		  from
		    arquivo
		  where
		    arqid = %d and
			arqstatus = '%s'
		",
	$intArquivoId,
	1
	);

	$arrArquivoBanco = $db->pegaLinha( $strSql );

	return $arrArquivoBanco;
}

function existeArquivoFisico( $intArquivoId )
{
	$intPasta = floor( $intArquivoId / QTD_MAX_ARQUIVOS_PAGINA );
	$strCaminho = PASTA_ARQUIVOS . $intPasta . "/" . $intArquivoId;
	$arrayLinhaArqv = pegaArquivoPeloId( $intArquivoId );
	$strCaminho .= "_" . $arrayLinhaArqv['arqnome'];
	return file_exists( $strCaminho ) && is_readable( $strCaminho );
}

function mostraArquivo( $intArquivoId , $boolComoAnexo = true )
{
	global $db;
	
	$intArquivoId = (integer) $intArquivoId;
	
	$intPasta = floor( $intArquivoId / QTD_MAX_ARQUIVOS_PAGINA );
	$strCaminho = PASTA_ARQUIVOS . $intPasta . "/" . $intArquivoId;
	$arrayLinhaArqv = pegaArquivoPeloId( $intArquivoId );
	$strCaminho .= "_" . $arrayLinhaArqv['arqnome'];	
	
	if( !is_file( $strCaminho ) )
	{
//		throw new Exception( "Arquivo {$intArquivoId} não encontrado" );
		alert("Arquivo {$intArquivoId} não encontrado!");
		return false;
	}
	$arrArquivoInformacoes = stat(  $strCaminho );
	$strFileContent = file_get_contents( $strCaminho );
	$arrArquivoBanco = pegaArquivoPeloId( $intArquivoId );

	header( "Content-MD5: " . md5( $strFileContent ) );
	header( "Content-Length: " . $arrArquivoInformacoes[ 7 ] );
	header( "Content-Type: " . $arrArquivoBanco[ "arqtipo" ] );
	if( $boolComoAnexo )
	{
		header( "Content-Disposition: attachment; filename=" . $arrArquivoBanco[ "arqnome" ] . "." . $arrArquivoBanco[ "arqextensao" ] );
	}
	else
	{
		header( "Content-Disposition: inline; filename=" . $arrArquivoBanco[ "arqnome" ] . "." . $arrArquivoBanco[ "arqextensao" ] );
	}
	print $strFileContent;
}

function removeArquivo( $intArquivoId , $strNomeArquivo)
{
	global $db;
	
	
	$arrayLinhaArqv = pegaArquivoPeloId($intArquivoId);

	if($arrayLinhaArqv['arqnome'] == $strNomeArquivo){


		$strSql = sprintf( "
		  update
			 public.arquivo
		  set
			 arqstatus = %d
		  where
			 arqid = %d
		",
		 0 ,
		$db->escape((int)$intArquivoId )
		);
		
	
		$db->executar( $strSql );
		$db->commit();

		return true;
	}
	else 
	{
		return false;
	}
}

function mudaArquivo( $intArquivoId , $strNome , $strDescricao )
{
	global $db;

	$strSql = sprintf( "
		  update
			 public.arquivo
		  set
			 arqnome = %s,
			 arqdescricao = %s
		  where
			 arqstatus = %d
		  and
			 arqid = %d
		",
	$db->escape( $strNome ),
	$db->escape( $strDescricao ),
	$db->escape( TRUE ),
	$db->escape( $intArquivoId )
	);
	$db->executar( $strSql );
}

function geraLinkParaArquivo( $intArquivoId ,$mostraNome = false )
{
	$arrArquivoBanco = pegaArquivoPeloId( $intArquivoId );
	if( (bool)$arrArquivoBanco['arqstatus'] == true)
	{
		?>
			<a href="http://<?=$_SERVER['SERVER_NAME']?>/mostra_arquivo.php?id=<?= $intArquivoId ?>_<?=$arrArquivoBanco[ "arqnome" ]?>" >
				<img style="border:0px" src="http://<?=$_SERVER['SERVER_NAME']?>/imagens/clipe.gif" title="<?= $arrArquivoBanco[ "arqnome" ] ?>.<?= $arrArquivoBanco[ "arqextensao" ] ?>" alt="<?= $arrArquivoBanco[ "arqdescricao" ] ?>" />
				<? if( $mostraNome ){?>
				&nbsp;<?= $arrArquivoBanco[ "arqnome" ] ?>.<?= $arrArquivoBanco[ "arqextensao" ] ?>
				<? } ?>
			</a>
		<?
	}
}

function geraLinkParaRemoverArquivo( $intArquivoId , $parametro = "" )
{
	$arrArquivoBanco = pegaArquivoPeloId( $intArquivoId );
	
	if( (bool)$arrArquivoBanco['arqstatus'] == true)
	{
		
		?>
			<a href="http://<?=$_SERVER['SERVER_NAME']?>/remove_arquivo.php?id=<?= $intArquivoId ?>_<?=$arrArquivoBanco[ "arqnome" ]?>&parametro=<?=$parametro?>" onclick="if(!confirm('Deseja remover o arquivo?'))return false;">
				<img style="border:0px" src="http://<?=$_SERVER['SERVER_NAME']?>/imagens/excluir.gif" title="<?= $arrArquivoBanco[ "arqnome" ] ?>" alt="<?= $arrArquivoBanco[ "arqdescricao" ] ?>" />
			</a>
		<?
	}
}

?>