<?php
	// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
	if ( !isset( $_SESSION[ 'usucpf' ] ) )
	{
		print '<html><script language="javascript"> alert( "Dados da sessão perdidos. Possivelmente sua sessão expirou." ); window.close(); </script></hmtl>';
		exit();
	}
	$usucpf = $_SESSION[ 'usucpf' ];
	
	
	print 'teste';exit();
	//dbg($paginacao,1);
	
	//Instancia um objeto Mensagem e manda carregar as mensagens de acordo com o usucpf
	include APPRAIZ . "includes/Mensagem.php";
	$objMensagem = new Mensagem();
	
	switch( $_REQUEST[ 'act' ] )
	{
		case "excluir":
			if( $_REQUEST[ 'msgid' ] )
			{
				$objMensagem->excluir( $_REQUEST[ 'msgid' ] );
				$db->commit();
				header( "Location: popup_mensagem.php" );
			}
			foreach( $_REQUEST as $chave => $dadoForm )
			{
				if( strpos( $chave, "chkMsg" ) === 0 )
				{
					$objMensagem->excluir( $dadoForm );
				}
			}
			$db->commit();
			header( "Location: popup_mensagem.php" );
			break;
	}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Caixa de mensagens</title>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<style type="text/css">
	a:link, a:visited{color:#0f55a9 !important;}
</style>
<script type="text/javascript">
	function selecionar_todas()
	{
		elmForm = document.formulario;
		elmMarcaTodas = document.getElementById( 'marcaTodas' );
		for( var i = 0 ; i < 30 ; i++ )
		{
			if( elmForm[ 'chkMsg' + i ] )
			{
				elmForm[ 'chkMsg' + i ].checked = elmMarcaTodas.checked ? "checked" : "";
			}
		}
	}
	
	function pagina( numero )
	{
		numPag = Math.floor( numero / 30 );
		document.location.href = 'popup_mensagem.php?offset=' + numero;
	}
	
	function actionForm( elm )
	{
		if( elm.value != "" )
		{
			for( var i = 0 ; i < 30 ; i++ )
			{
				if( document.formulario[ 'chkMsg' + i ].checked )
				{
					document.formulario.act.value = elm.value;
					document.formulario.submit();
					return;
				}
			}
			
			alert( 'Selecione ao menos uma mensagem.' );
			
		}
	}
	
</script>
</head>

<body>
<?
	//Lista as mensagens
	if( !$_REQUEST[ 'msgid' ] ):
		$offset = $_REQUEST[ 'offset' ] ? floor( $_REQUEST[ 'offset' ] / 30 ) : 0;
		$paginacao = $objMensagem->listar_recebidas( $usucpf, $offset );

	
?>
<table cellpadding="0" cellspacing="0" border="0">
	<tr>
		<td style="padding-bottom:5px;">Com marcadas:&nbsp;</td>

		<td style="padding-bottom:5px;">
			<select name="sclAcao" onchange="actionForm( this );">
				<option value="">Ação</option>
				<option value="excluir">Excluir</option>
			</select>
		</td>
	</tr>
</table>
<table cellpadding="0" cellspacing="0" border="0" width="100%" class="listagem">
	<thead>
		<tr>
			<th width="25" align="center"><input type="checkbox" name="marcaTodas" id="marcaTodas" onchange="selecionar_todas();" /></th>
			<th align="left">Assunto</th>
			<th align="left">Remetente</th>
			<th align="left">Data</th>
		</tr>
	</thead>
	<tbody>
		<form action="" name="formulario">
			<input type="hidden" name="act" value="" />
			<?
				if( $paginacao->dados ):
					$contador = 0;
					foreach( $paginacao->dados as $mensagem ):
						$fundo = $contador % 2 == 0 ? '#f4f4f4' : '#e0e0e0';
			?>
						<tr bgcolor="<?=$fundo?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$fundo?>';">
							<td width="25" align="center" style="padding-left:1px;"><input type="checkbox" name="chkMsg<?=$contador?>" id="chkMsg<?=$contador?>" value="<?=$mensagem[ 'msgid' ]?>" /></td>
							<td>
								<a href="popup_mensagem.php?msgid=<?=$mensagem[ 'msgid' ]?>">
									<? if( $mensagem[ 'msulida' ] == 'f' ) : ?>
										<img src="../imagens/email.gif" style="border:none;position:relative;top:2px;margin-right:5px;" />
										<strong><?=$mensagem[ 'msgassunto' ]?></strong>
									<? else : ?>
										<img src="../imagens/email_lido.gif" style="border:none;position:relative;top:2px;margin-right:5px;" />
										<?=$mensagem[ 'msgassunto' ]?>
									<? endif; ?>
								</a>
							</td>
							<td><?=$mensagem[ 'remetente' ]?></td>
							<td><?=date( 'd/m/Y à\s h:i:s', strtotime( $mensagem[ 'msgdata' ] ) );?></td>				
						</tr>
			<?
						$contador++;
					endforeach;
				else:				
			?>
				<tr bgcolor="#f4f4f4">
					<td></td>
					<td colspan="3" style="color:#ff0000;">Não há mensagens</td>
				</tr>
			<?
				endif;
			?>
		</form>
		<tr>
			<td colspan="2" style="padding:5px 5px 5px 8px;"><strong>Total de mensagens: <?=$paginacao->total;?></strong></td>
			<td colspan="2" align="right">
				<? 
					$perpage = 30;
					$pages = 10;
					$total_reg = $paginacao->total;
					if ( $_REQUEST['offset'] == '' ) $numero = 1; else $numero = intval( $_REQUEST[ 'offset' ] );
					include APPRAIZ . "includes/paginacao.inc"; 
				?>
			</td>
		</tr>	
	</tbody>
</table>
<? 
	//Mostra a mensagem de acordo com o msgid passado pela query string
	else:
		$mensagem = $objMensagem->carregar_mensagem( $_REQUEST[ 'msgid' ] );
		if( $mensagem ) :
			if( $mensagem[ 'msulida' ] == 'f' )
			{
				$objMensagem->marcar_lida( $_REQUEST[ 'msgid' ], 't' );
			}
?>
			<table cellpadding="0" cellspacing="0" border="0">
				<tr>
					<td style="padding:5px; width:100px; text-align:center;height:15px;background:#e0e0e0;"><a href="popup_mensagem.php">Caixa de entrada</a></td>
					<td width="10"></td>
					<td style="padding:5px; width:100px; text-align:center;height:15px;background:#e0e0e0;"><a href="popup_mensagem.php?act=excluir&msgid=<?=$mensagem[ 'msgid' ];?>">Excluir</a></td>
				</tr>
			</table>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" style="width:100%;">
				<tr>
					<td class="SubTituloDireita" style="width:144px;text-align:right; padding:5px 5px 5px 0;">Mensagem enviada por:</td>
					<td><?=$mensagem[ 'remetente' ]?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="text-align:right; padding:5px 5px 5px 0;">Em:</td>
					<td><?=date( 'd/m/Y à\s h:i:s', strtotime( $mensagem[ 'msgdata' ] ) );?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="text-align:right; padding:5px 5px 5px 0;">Assunto:</td>
					<td><?=$mensagem[ 'msgassunto' ];?></td>
				</tr>
				<tr>
					<td colspan="2" style="padding:20px;border:1px solid black; background:#ffffff;"><?=$mensagem[ 'msgconteudo' ];?></td>
				</tr>
			</table>
	<?
		else :
		//Mostrar que a mensagem não pode ser aberta
		endif;
	?>
<?  endif; ?>
</body>
</html>