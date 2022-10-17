<?php
if (!$_GET[sisid])
die("<script>window.close();</script>");

# INSERIR OU EDITAR
if ($_POST[sisid] && $_POST[tpddsc]):
	if (!$_POST[tpdid]):
		$sql = sprintf("INSERT INTO workflow.tipodocumento(
							sisid,
							tpddsc		
						)VALUES(
							%d,
							'%s'
						) returning tpdid", $_POST[sisid], $_POST[tpddsc]);

		$tpdid = $db->pegaUm($sql);
		$db->commit();
		die ("<script>
				alert('Operação executada com sucesso!');
				window.opener.location.replace(window.opener.location);
				//window.opener.document.formulario.NEWtpdid.value = ".$tpdid.";
				//window.opener.document.formulario.submit();
				window.close();
			  </script>");
	else:
		$sql = sprintf("UPDATE
							workflow.tipodocumento
						SET
							sisid = %d,
							tpddsc = '%s'
						WHERE
							tpdid = %d",$_POST[sisid], trim($_POST[tpddsc]), $_POST[tpdid]);

		$db->executar($sql);
		$db->commit();
		die("<script>
				alert('Operação executada com sucesso!');
				window.opener.location.replace(window.opener.location);
				//window.opener.document.formulario.NEWtpdid.value = ".$_POST[tpdid].";
				//window.opener.document.formulario.submit();
				window.close();
			 </script>");		
	endif;
endif;

/*
# DELETAR
if ($_GET[tpdidDel]):
	$sql = sprintf("SELECT
						COUNT(esdid)
					FROM
						workflow.estadodocumento
					WHERE
						esdstatus = 'A' AND
						tpdid = %d", $_GET[tpdidDel]);
	$totTipDocAtivo = $db->pegaUm($sql);
	
	if ($totTipDocAtivo):
		echo '<script type="text/javascript">
				  alert(\'Operação não realizada!\nExiste Estado Documento cadastrado neste Tipo Documento.\');
			  </script>';	
	else:
		$sql_confere = sprintf("SELECT
									COUNT(tpdid) as total
								FROM
									workflow.estadodocumento
								WHERE
									tpdid = %d", $_GET[tpdidDel]);
		$determina = $db->pegaUm($sql_confere);
		
		$determina ? $sql = "UPDATE workflow.tipodocumento SET tpdstatus = 'I' WHERE tpdid=".$_GET[tpdidDel] : $sql = "DELETE FROM workflow.tipodocumento WHERE tpdid = ".$_GET[tpdidDel];
		
		$db->executar($sql);
		$db->commit();
		
			die("<script>
					alert('Operação executada com sucesso!');
					window.opener.document.formulario.NEWtpdid.value = 'del';
					window.opener.document.formulario.submit();
					window.close();
				 </script>");
	endif;
	unset($totTipDoc);			
endif;
*/
$sql = "SELECT
			sisdsc
		FROM 
			seguranca.sistema
		WHERE
			sisid = ".$_GET[sisid];	

$sisdsc = (string) $db->pegaUm( $sql );

unset($sql);

if ($_GET[tpdid]):
	$sql = "SELECT
				tpddsc
			FROM
				workflow.tipodocumento
			WHERE
				tpdstatus = 'A' AND
				tpdid = ".$_GET[tpdid];
	
	$tpddsc = $db->pegaUm($sql);
endif;	
unset($sql);
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php
monta_titulo( 'Tipo Documento', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<form method="post" name="formulario" action="">
<table class="tabela" bgcolor="#f5f5f5"
	cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" align="right">Sistema:</td>
		<td><?= campo_texto( 'sisdsc', 'S', 'N', '', 55, 75, '', '' ); ?> <input
			name="sisid" type="hidden" value="<?php echo $_GET[sisid];?>"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">Descrição:</td>
		<td><?= campo_texto( 'tpddsc', 'S', 'S', '', 55, 500, '', ''); ?></td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td></td>
		<td>
			<?php
			if ($_GET[tpdid]):
				echo "<input type='hidden' value='".$_GET[tpdid]."' name='tpdid'>";
				$inputName = 'Alterar';
				$inputNew = "<input type='button' class='botao' value='Novo' id='cad'name='cad' onclick=\"javascript: location.href='?modulo=sistema/workflow/popupTipDoc&acao=I&sisid=". $_GET[sisid] ."' \">"; 
			else:
				$inputName = 'Incluir';
			endif;
			?>
			<input type='button' class='botao' value='<?php echo $inputName; ?>' id='cad'name='cad' onclick="validar()"> 
			<input type='button' class='botao' value='Fechar' id='cad' name='cad' onclick="javascript:window.opener.location.replace(window.opener.location); window.close();">
			<?php
			echo $inputNew;
			unset($inputNew,$inputName); 
			?>
		</td>
	</tr>
</table>
</form>
<?php
/*
$sql = "SELECT
			'<img
			border=\"0\"
			src=\"../imagens/alterar.gif\"
			title=\"Alterar Tipo Documento\"
			onclick=\"javascript:location.href=\'?modulo=sistema/workflow/popupTipDoc&acao=I&sisid=' ||". $_GET[sisid] ."|| '&tpdid= ' || tpdid || ' \'\">
			&nbsp
			<img
			border=\"0\"
			src=\"../imagens/excluir.gif\"
			title=\"Excluir Tipo Documento\"
			onclick=\"javascript: confirma(\'Deseja apagar o tipo documento ' || tpddsc || '? \',\'?modulo=sistema/workflow/popupTipDoc&acao=I&sisid=' ||". $_GET[sisid] ."|| '&tpdidDel= ' || tpdid || ' \') \">'
			as acao,
			tpddsc
		FROM
			workflow.tipodocumento
		WHERE
			sisid = ".$_GET[sisid]." AND
			tpdstatus = 'A'
		ORDER BY
			tpddsc ASC;	
			";

$cabecalho = array( 'Ações', 'Descrição');
$db->monta_lista( $sql, $cabecalho, 200, 20, '', '', '' );
*/
?>
<script>
function validar () {
	d = document.formulario;
	var txt = '';

	if (d.tpddsc.value.length < 3)
		txt += 'Campo Descrição deve ser preenchido!';

	if (txt){
		alert(txt);
		return false;
	}else{
		d.submit();
		return true;
	}	
}

function confirma(mens,url)	{
			
	if (confirm(mens))
		location.href = url;
	else
		return false;
		
	return true;
}
</script>
</body>
