<?php 
extract($_REQUEST);

if ($_POST){
	salva();
	die("<script>
			alert('Operação realizada com sucesso!');
			window.opener.location.replace(window.opener.location);
			window.close();
		 </script>");
}

if (!$sisid){
	die("<script>
			alert('Faltam parametros!\nTente novamente.');
			window.close();
		 </script>");
}
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php
monta_titulo( 'Replicação de WorkFlow', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<form action="" method="post" name="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" align="right">Módulo:</td>
		<td>
		<?
		
		$sql = "SELECT 
				sisdsc
			   FROM 
			   	seguranca.sistema
			   WHERE 
			   	sisid = $sisid";
		echo $db->pegaUm($sql);		
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">WorkFlow:</td>
		<td>
		<?

		$sql_estadodocumento = "SELECT
									tpdid AS codigo,
									tpddsc AS descricao
								 FROM
								  	workflow.tipodocumento
								 WHERE
								 	tpdstatus != 'I' AND
								 	sisid = $sisid
								 ORDER BY tpddsc ASC 
								";

		$db->monta_combo("tpdidorigem", $sql_estadodocumento, "S", "Selecione", '', '', '', '200', 'S', "tpdidorigem");
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Replicar <b>workflow</b> para</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right">Módulo (Destino):</td>
		<td>
		<?
		$sql_estadodocumento = "SELECT
									sisid AS codigo,
									sisdsc AS descricao
								 FROM
								  	seguranca.sistema
								 WHERE
								 	sisstatus != 'I'
								 ORDER BY sisdsc ASC 
								";

		$db->monta_combo("sisiddestino", $sql_estadodocumento, "S", "Selecione", '', '', '', '200', 'S', "sisiddestino");
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">WorkFlow (Destino):</td>
		<td>
			<?= campo_texto( 'tpdiddsc', 'S', 'S', '', 35, 100, '', ''); ?>		
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<td>
			<input type='button' class='botao' name='consultar' value='Replicar WorkFlow' onclick="validar()" />
			<input type='button' class='botao' value='Fechar' id='btFechar' name='btFechar' onclick='window.close();' />
			<input type='hidden' name='sisid' value='<?php echo $sisid; ?>' />
		</td>			
	</tr>			
</table>
</form>
<script>
function validar(){
	d = document;

	if (d.getElementById('tpdidorigem').value == ''){
		alert('É obrigatório marcar o WorkFlow que será replicado!');
		d.getElementById('tpdidorigem').focus();
		return false;
	} else if (d.getElementById('sisiddestino').value == ''){
		alert('É obrigatório marcar o Módulo!');
		d.getElementById('sisiddestino').focus();
		return false;
	} else if (d.getElementsByName('tpdiddsc')[0].value == ''){
		alert('É obrigatório o campo WorkFlow (Destino)!');
		d.getElementsByName('tpdiddsc')[0].focus();
		d.getElementsByName('tpdiddsc')[0].select();
		return false;		
	}
	d.formulario.submit();
}
</script>
<?php 
function salva(){
	global $db;
	extract($_REQUEST);

	/*
	Replica tipo do documento
	*/	
	$sql = "INSERT INTO workflow.tipodocumento (
				tpddsc,
				sisid,
				tpdstatus
			) VALUES (
				'$tpdiddsc', 
				'$sisiddestino',
				'A'
			) RETURNING tpdid";

	$tpdid = $db->pegaUm($sql);

	/*
	Replica estado documento
	*/
	$sql = "INSERT INTO workflow.estadodocumento (
				tpdid,
				esdstatus,
				esdordem,
				esddsc
			) SELECT 
				'$tpdid',
				esdstatus,
				esdordem,
				esddsc 
			  FROM 
			  	workflow.estadodocumento 
			  WHERE 
			  	tpdid = $tpdidorigem;";
	
	$db->executar($sql);

	/*
	Replica Ação
	*/
	$sql = "INSERT INTO workflow.acaoestadodoc 
			(esdidorigem,esdiddestino,aeddscrealizar,aedstatus,aeddscrealizada,esdsncomentario,aedcondicao,aedobs,aedposacao) 
			 SELECT
			  novo.esdid as origem,
			  novo2.esdid as destino,
			  aed.aeddscrealizar,
			  aed.aedstatus,
			  aed.aeddscrealizada,
			  aed.esdsncomentario,
			  aed.aedcondicao,
			  aed.aedobs,
			  aed.aedposacao 
			 FROM
			  workflow.acaoestadodoc aed
			  inner join workflow.estadodocumento ed on ed.esdid = aed.esdidorigem 
			  inner join (SELECT
							esdid,
							esddsc
						  FROM
							workflow.estadodocumento ed1 
						  WHERE
							tpdid = $tpdid) novo ON novo.esddsc = ed.esddsc
			  inner join workflow.acaoestadodoc aed2 ON aed2.aedid = aed.aedid
			  inner join workflow.estadodocumento ed2 on ed2.esdid = aed2.esdiddestino 
			  inner join (SELECT
							esdid,
							esddsc
						  FROM
							workflow.estadodocumento ed2 
						  WHERE
							tpdid = $tpdid) novo2 ON novo2.esddsc = ed2.esddsc
			 WHERE
			  ed.tpdid = $tpdidorigem";
	
	$db->executar($sql);
	$db->commit();
}
?>