<?php

include "config.inc";
header( 'Content-Type: text/html; charset=iso-8859-1' );
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

function pegar_filhos( $registro = null )
{
	global $db;
	$sql = sprintf(
		"
			select
				a.atiid,
				a.atiidpai,
				a.atidescricao
			from pde.atividade a
			where
				a.atistatus = 'A' and
				a.atiidpai %s
			order by a.atiordem",
		$registro ? " = " . $registro['atiid'] : " is null "
	);
	$filhos = $db->carregar( $sql );
	return $filhos ? $filhos : array();
}

function exibir_registro( $registro, $nivel = 0, $nivel_max )
{
	global $db;
	global $atividade;
	static $cor = '#f7f7f7';
	
	$cor = ( $cor == '#f7f7f7' ) ? '#ffffff' : '#f7f7f7';
	if ( !$registro )
	{
		return;
	}
	$filhos = pegar_filhos( $registro );
	$somatorio = array();
	$checked = in_array( $registro['atiid'], $atividade ) ? 'checked="checked"' : '';
	
	$sql = "select numero from pde.f_dadosatividade( " . $registro['atiid'] . " )";
	$numero = $db->pegaUm( $sql );
	
	?>
	<tr id="tr<?= $registro['atiidpai'] ?>" bgcolor="<?= $cor ?>" onmouseout="this.bgColor='<?= $cor ?>';" onmouseover="this.bgColor='#ffffcc';" parent="<?= $registro['atiidpai'] ?>">
		<td style="text-align: center; width: 35px;">
			<input type="checkbox" name="atividade[]" value="<?= $registro['atiid'] ?>" <?= $checked ?>/>
		</td>
		<td style="padding-left: <?= ( $nivel * 25 ) + 5 ?>px">
			<?php if( $nivel > 0 ): ?>
				<img src="../imagens/seta_filho.gif" align="left"/>
			<?php endif; ?>
			<div style="float: left; width: 90%; <?= count( $filhos ) > 0 ? 'font-weight: bold' : '' ?>">
				<?php if (count( $filhos ) > 0 ) : ?>
					<img id="img<?= $registro['atiid'] ?>" atividade="<?= $registro['atiid'] ?>" onclick="exibirOcultarAtividadesFilhas( <?= $registro['atiid'] ?>, this, true );" id="imgTarefa<?= $registro['atiidpai'] ?>" src="../imagens/menos.gif"/>&nbsp;
				<?php endif; ?>
				<?= $numero . ' ' . $registro['atidescricao'] ?>
			</div>
		</td>
	</tr>
	<?php
		foreach ( $filhos as $filho )
		{
			exibir_registro( $filho, $nivel + 1, $nivel_max );
		}
	?>
	<?php if( $nivel >= $nivel_max ): ?>
		<script type="text/javascript">
			var img = document.getElementById( "img<?= $registro['atiid'] ?>" );
			if ( img )
			{
				img.onclick();
			}
		</script>
	<?php endif;
}

function pegarTodosFilhos( $atiid )
{
	global $db;
	$atiid = (integer) $atiid;
	$sql = "select atiid from pde.atividade where atiidpai = " . $atiid;
	$linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();
	$res = array();
	foreach ( $linhas as $linha )
	{
		array_push( $res, $linha['atiid'] );
		$res = array_merge( $res, pegarTodosFilhos( $linha['atiid'] ) );
	}
	return $res;
}

$db = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];

$atividade_inicial = isset( $_REQUEST['atividade_inicial'] ) ? (integer) $_REQUEST['atividade_inicial'] : 3;
$nivel_aberto = isset( $_REQUEST['nivel_aberto'] ) ? (integer) $_REQUEST['nivel_aberto'] : 1;

$atividade = $_REQUEST['atividade'] ? $_REQUEST['atividade'] : array();
$atividades_filhas = pegarTodosFilhos( $_REQUEST['atiid_raiz'] );

if ( $_REQUEST['tipo'] == 'gravar' && count( $atividade ) )
{
	$atividades_in = implode( ',', $atividades_filhas );
	$sql_remocao = <<<EOT
		update pde.usuarioresponsabilidade
		set rpustatus = 'I'
		where
			usucpf = '$usucpf' and
			pflcod = '$pflcod' and
			atiid in ( $atividades_in )
EOT;
	$db->executar( $sql_remocao );
	// grava as atividades
	foreach ( $atividade as $atiid )
	{
		$sql_existe = "select count(*) as existe from pde.usuarioresponsabilidade where usucpf = '$usucpf' and pflcod = '$pflcod' and atiid = $atiid";
		$existe = (integer) $db->pegaUm( $sql_existe );
		if ( $existe )
		{
			$sql_atualizacao = <<<EOT
				update pde.usuarioresponsabilidade
				set rpustatus = 'A'
				where
					usucpf = '$usucpf' and
					pflcod = '$pflcod' and
					atiid = $atiid
EOT;
			$db->executar( $sql_atualizacao );
			dbg( $sql_atualizacao );
		}
		else
		{
			$sql_insercao = <<<EOT
				insert into pde.usuarioresponsabilidade
				( atiid, usucpf, rpustatus, pflcod )
				values
				( $atiid, '$usucpf', 'A',  '$pflcod')
EOT;
			$db->executar( $sql_insercao );
			dbg( $sql_insercao );
		}
	}
	$db->commit();
	?>
		<script>
			window.parent.opener.location.reload();
			self.close();
		</script>
	<?php
	exit();
}
else
{
	// carrega do banco
	$sql = <<<EOT
		select
			ur.atiid
		from pde.usuarioresponsabilidade ur
		where 
			ur.rpustatus = 'A' and
			ur.usucpf = '$usucpf' and
			ur.pflcod = '$pflcod'
EOT;
	$ids = $db->carregar( $sql );
	$ids = $ids ? $ids : array();
	foreach ( $ids as $id )
	{
		array_push( $atividade, $id['atiid'] );
	}
}

?>
<html>
	<head>
		<META http-equiv="Pragma" content="no-cache">
		<title>Atividades</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
		<script type="text/javascript">
		
			function filtrar()
			{
				document.formulario.tipo.value = 'filtrar';
				document.formulario.submit();
			}
			
			function gravar()
			{
				document.formulario.tipo.value = 'gravar';
				document.formulario.submit();
			}
			
			var IE = document.all ? true : false;
			
			function exibirOcultarAtividadesFilhas( atividade, imagem, origem )
			{
				var atividades = document.getElementById( 'atividades' ).getElementsByTagName( 'tr' );
				for( var i = 0; i < atividades.length ; ++i )
				{
					if( atividades[i].getAttribute( 'parent' ) == atividade )
					{
						if ( atividades[i].style.display == "none" )
						{
//							armazenaNoCookieVisibilidadeAtividade( atividades[i].id , true );
							if( !IE )
							{
								atividades[i].style.display = "table-row";
							}
							else
							{
								atividades[i].style.display = "block";
							}
							if ( origem == true )
							{
								imagem.src = imagem.src.replace( 'mais' , 'menos' );
							}
						}
						else
						{
//							armazenaNoCookieVisibilidadeAtividade( atividades[i].id , false );
							atividades[i].style.display = "none";
							if ( origem == true )
							{
								imagem.src = imagem.src.replace( 'menos' , 'mais' );
							}
						}
						var imagens = atividades[i].getElementsByTagName( 'img' );
						for( var j = 0; j < imagens.length ; ++j )
						{
							if( imagens[j].getAttribute( 'atividade' ) != null && imagens[j].src.indexOf( 'menos' ) > 0 )
							{
								exibirOcultarAtividadesFilhas( imagens[j].getAttribute( 'atividade' ), imagens[j], false );
							}
						}
					}
				}
			}
			
		</script>
	</head>
	<body bgcolor="#ffffff" topmargin="0" leftmargin="0" style="padding: 0;">
		<?php
			$sql = "select atidescricao from pde.atividade where atiid = " . $atividade_inicial; 
			$titulo = $db->pegaum( "select atidescricao from pde.atividade where atiid = " . $atividade_inicial );
		?>
		<form name="formulario" action="" method="post">
			<input type="hidden" name="tipo" value=""/>
			<input type="hidden" name="usucpf" value="<?= $usucpf ?>"/>
			<input type="hidden" name="pflcod" value="<?= $pflcod ?>"/>
			<input type="hidden" name="atiid_raiz" value="<?= $atividade_inicial ?>"/>
			<table align="left" border="0" cellpadding="5" cellspacing="1" width="100%" bgcolor="#efefef">
				<tr>
					<td bgcolor="#dcdcdc">
						<label for="orc_agrupar">Nível Inicial</label>
					</td>
					<td style="padding: 0 20px 0 10px;">
						<?php
							$sql = <<<EOT
								select
									atiordempai || atiordem,
									atiid as codigo,
									substr( numero || ' - ' || atidescricao, 0, 40 ) ||
										case when char_length( numero || ' - ' || atidescricao ) > 39
										then '...'
										else '' end as descricao
								from (
										select
											trim( to_char( a1.atiordem, '99' ) ) as numero,
											a1.atiid,
											a1.atidescricao,
											a1.atiid as atiidpai,
											0 as atiordem,
											a1.atiordem as atiordempai
										from pde.atividade a1
										where
											a1.atiidpai = 3 and
											a1.atistatus = 'A'
									union all
										select
											'&nbsp;&nbsp;&nbsp;' || trim( to_char( a1.atiordem, '99' ) ) || '.' || trim( to_char( a2.atiordem, '99' ) ) as numero,
											a2.atiid,
											a2.atidescricao,
											a2.atiidpai,
											a2.atiordem,
											a1.atiordem as atiordempai
										from pde.atividade a1 
											inner join pde.atividade a2 on a2.atiidpai=a1.atiid
										where
											a1.atiidpai = 3 and
											a1.atistatus = 'A' and
											a2.atistatus = 'A'
									) as foo
								order by atiordempai, atiordem
EOT;
						?>
						<?php $db->monta_combo( "atividade_inicial", $sql, 'S', '0 - Raiz PDE', '', '' );?>
					</td>
					<td bgcolor="#dcdcdc" align="center" width="110" rowspan="2">
						<input type="button" name="btFiltrar" value="Filtrar" onclick="filtrar();"/>
					</td>
				</tr>
			    <tr>
					<td bgcolor="#dcdcdc" width="150">Níveis Visíveis</td>
					<td style="padding: 0 20px 0 10px;">
						<select name="nivel_aberto">
							<option value="0" <?php echo $nivel_aberto == 0 ? 'selected="selected"' : ''; ?>>
								&nbsp;&nbsp;0&nbsp;&nbsp;
							</option>
							<option value="1" <?php echo $nivel_aberto == 1 ? 'selected="selected"' : ''; ?>>
								&nbsp;&nbsp;1&nbsp;&nbsp;
							</option>
							<option value="2" <?php echo $nivel_aberto == 2 ? 'selected="selected"' : ''; ?>>
								&nbsp;&nbsp;2&nbsp;&nbsp;
							</option>
							<option value="3" <?php echo $nivel_aberto == 3 ? 'selected="selected"' : ''; ?>>
								&nbsp;&nbsp;3&nbsp;&nbsp;
							</option>
						</select>
					</td>
				</tr>
				<tr>
					<td colspan="3" align="center" style="padding: 5px 0 5px 50px; margin: 0;" bgcolor="#dfdfdf">
						<b>Atividade</b>
					</td>
				</tr>
				<tr>
					<td colspan="3" style="padding: 0; margin: 0;">
						<div style="overflow-y: always; overflow-x: auto; height: 376px; width: 497px; float: left;">
							<table id="atividades" cellspacing="1" width="100%" cellpadding="2" border="0" align="left">
								<?php
									$sql = sprintf( "
										select
											a.atiid,
											a.atiidpai,
											a.atidescricao
										from pde.atividade a
											inner join pde.f_dadostodasatividades() as da on
												da.atiid = a.atiid
										where a.atiid = %d ",
										$atividade_inicial
									);
									$raiz = $db->carregar( $sql );
									// se tiver pai, mostra a partir do pai
									if( $raiz[0]['atiidpai'] )
									{
										exibir_registro( $raiz[0], 0, $nivel_aberto );
									}
									// se não tiver pai, mostra só os filhos (caso atividade raiz)
									else
									{
										$filhos = pegar_filhos( $raiz[0] );
										foreach ( $filhos as $registro )
										{
											exibir_registro( $registro, 0, $nivel_aberto );
										}
									}
								?>
							</table>
						</div>
					</td>
				</tr>
				<tr>
					<td align="center" colspan="3" style="padding: 5px;" bgcolor="#dfdfdf">
						<input type="button" name="btGravar" value="Gravar" onclick="gravar();"/>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>