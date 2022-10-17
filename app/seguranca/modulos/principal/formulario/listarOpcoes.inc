
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th style="text-align: right; width: 150px;">Nome do Atributo:</th>
		<td><?=$_REQUEST['atrnome'] ?></td>
	</tr>
</table>
<?php

monta_titulo('', 'Lista de Opções');

$atrid = $_REQUEST['atrid']; 
?>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?
$sql = "SELECT opavalor, opadescricao 
		  FROM formulario.opcoesatributo
		WHERE atrid = $atrid 
		AND opastatus = true";

$cabecalho = array('Código','Descrição');
$db->monta_lista($sql, $cabecalho, 20, 5, 'N', 'center', '');
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th colspan="2"><input type="button" value="Fechar" name="btnFechar" onclick="javascript: window.close();"></th>
	</tr>
</table>