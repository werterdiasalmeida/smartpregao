<?

$sql_alerta = "SELECT a.aledtinic, a.aledtfim, a.aledsc 
						FROM seguranca.alerta a 						
						WHERE a.sisid = '".$_SESSION['sisid']."' ";

$dados = $db->recuperar($sql_alerta);

$menssagem = $dados['aledsc'];

?>

<table class="tabela" bgcolor="#ffffcc" cellSpacing="1" cellPadding="3" width="100%" height="100%"
	align="center">
	
	<tr>
		<td align='center' class="SubTituloDireita">
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" width="80%">
		<div  style="float: left; width: 95%; height: 300px; overflow: auto; text-align: justify;">
			<b>	<?=$menssagem ?> </b>
		</div>
		</table>
		</td>	
	</tr>
	
</table>