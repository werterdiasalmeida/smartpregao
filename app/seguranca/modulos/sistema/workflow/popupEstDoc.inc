<?php
if (!$_GET[tpdid])
die("<script>window.close();</script>");

$_POST[esdid] ? $esdid = $_POST[esdid] : $esdid = '';

if ($_POST[tpdid] && $_POST[esdordem] && $_POST[esddsc]):
	if ($esdid):
		$sql = sprintf("UPDATE
							workflow.estadodocumento
						SET
							esdordem = %d,
							esddsc = '%s'
						WHERE
							esdid = %d",$_POST[esdordem],trim($_POST[esddsc]),$esdid);

		$db->executar($sql);
		$db->commit();
		die("<script>
				alert('Operação executada com sucesso!');
				window.opener.location.replace(window.opener.location);
				//window.opener.document.formulario.NEWesdid.value = ".$esdid.";
				//window.opener.document.formulario.submit();
				window.close();
			  </script>");		
	else:
		$sql = sprintf("INSERT INTO workflow.estadodocumento(
							tpdid,
							esdordem,
							esddsc			 
						)VALUES(
							%d,
							%d,
							'%s'
						) returning esdid ",$_GET[tpdid],$_POST[esdordem],trim($_POST[esddsc]));
		$esdid = $db->pegaUm($sql);
		$db->commit();		
		die ("<script>
				alert('Operação executada com sucesso!');
				window.opener.location.replace(window.opener.location);
				//window.opener.document.formulario.NEWesdid.value = ".$esdid.";
				//window.opener.document.formulario.submit();
				window.close();
			  </script>");
	endif;
endif;

unset($sql);
/*
if ($_GET[esdidDel]):
	$sql = sprintf("SELECT
						COUNT(aedid)
					FROM
						workflow.acaoestadodoc
					WHERE
						aedstatus = 'A' AND
						esdidorigem = %d", $_GET[esdidDel]);
	$totEstDoc = $db->pegaUm($sql);
	if ($totEstDoc):
		echo '<script type="text/javascript">
				  alert(\'Operação não realizada!\nExiste Ação cadastrada neste Estado Documento.\');
				  window.opener.location.replace(window.opener.location);				  
			  </script>';	
	else:
		$sql_confere = sprintf("SELECT
									COUNT(esdidorigem)
								FROM
									workflow.acaoestadodoc
								WHERE
									esdidorigem = %d OR
									esdiddestino = %d",$_GET[esdidDel],$_GET[esdidDel]);

		$determina = $db->pegaUm($sql_confere);

		$determina ? $sql = "UPDATE workflow.estadodocumento SET esdstatus = 'I' WHERE esdid = ".$_GET[esdidDel] : $sql = "DELETE FROM workflow.estadodocumento WHERE esdid = ".$_GET[esdidDel];
	
		$db->executar($sql);
		$db->commit();
		
		die("<script>
				alert('Operação executada com sucesso!');
				window.opener.location.replace(window.opener.location);
				//window.opener.document.formulario.NEWesdid.value = 'del';
				//window.opener.document.formulario.submit();
				window.close();
			  </script>");
	endif;
	unset($totEstDoc);		
endif;
*/
unset($sql);

$sql = "SELECT
			tpddsc
		FROM 
			workflow.tipodocumento
		WHERE
			tpdid = ".$_GET[tpdid];	

$tpddsc = (string) $db->pegaUm( $sql );

unset($sql);

if (!$_GET[esdid]):
	$sql = "SELECT
				MAX(esdordem) as esdordem
			FROM 
				workflow.estadodocumento
			WHERE
				esdstatus = 'A' AND
				tpdid = ".$_GET[tpdid];	
	
	$esdordem = (int) 1 + $db->pegaUm( $sql );
	
else:
	$sql = "SELECT
				esdordem,
				esddsc
			FROM
				workflow.estadodocumento
			WHERE
				esdstatus = 'A' AND
				esdid = ".$_GET[esdid];
	
	$dados = $db->carregar($sql);
	#echo $dados[0][];
	foreach ($dados[0] as $k => $val)
		${$k} = $val;	
endif;
unset($sql);
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php
monta_titulo( 'Estado Documento', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<form method="post" name="formulario" action="">
<table class="tabela" bgcolor="#f5f5f5"
	cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" align="right">Tipo Documento:</td>
		<td><?= campo_texto( 'tpddsc', 'S', 'N', '', 55, 75, '', '' ); ?> <input
			name="tpdid" type="hidden" value="<?php echo $_GET[tpdid];?>"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">Ordem:</td>
		<td><?= campo_texto( 'esdordem', 'S', 'S', '', 55, 75, '', '' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">Descrição:</td>
		<td><?= campo_textarea( 'esddsc', 'S', 'S', '', 60, 4, 500 ); ?></td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td></td>
		<td>
			<?php
			if ($_GET[esdid]):
				echo "<input type='hidden' value='".$_GET[esdid]."' name='esdid'>";
				$inputName = 'Alterar';
				$inputNew = "<input type='button' class='botao' value='Novo' id='cad'name='cad' onclick=\"javascript: location.href='?modulo=sistema/workflow/popupEstDoc&acao=I&tpdid=". $_GET[tpdid] ."' \">"; 
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
			title=\"Alterar Estado Documento\"
			onclick=\"javascript:location.href=\'?modulo=sistema/workflow/popupEstDoc&acao=I&tpdid=' ||". $_GET[tpdid] ."|| '&esdid= ' || esdid || ' \'\">
			&nbsp
			<img
			border=\"0\"
			src=\"../imagens/excluir.gif\"
			title=\"Excluir Estado Documento\"
			onclick=\"javascript: confirma(\'Deseja apagar o estado documento ' || esddsc || '?\',\'?modulo=sistema/workflow/popupEstDoc&acao=I&tpdid=' ||". $_GET[tpdid] ."|| '&esdidDel= ' || esdid || ' \') \">'
			as acao,
			esdordem,
			esddsc
		FROM
			workflow.estadodocumento
		WHERE
			tpdid = ".$_GET[tpdid]."
		ORDER BY
			esdordem ASC;	
			";

$cabecalho = array( 'Ações', 'Ordem','Descrição');
$db->monta_lista( $sql, $cabecalho, 200, 20, '', '', '' );
*/
?>
<script>
function validar () {
	d = document.formulario;
	var txt = '';

	if (d.esdordem.value == '')
		txt = 'Campo Ordem deve ser preenchido!\n';

	if (d.esddsc.value.length < 3)
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
