<?php
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

if($_REQUEST['evento']){
	
	switch($_REQUEST['evento']){
		
		case "E":
			$sql = "update painel.cargoresponsavel set crgstatus = 'I' where crgid = {$_REQUEST['crgid']}";
			$db->executar($sql);
			$db->commit($sql);
		break;
		
	}
	
}

monta_titulo( "Cargo de Responsável por Secretaria", "");
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">Cargo:</td>
    <td><? 
    	echo campo_texto('cargodsc','N','S','',100,255,'',''); 
    	?>
    </td>
</tr>
<tr bgcolor="#cccccc">
	      <td></td>
	  	  <td>
	  	  <input type="button" class="botao" name="bta" value="Pesquisar" onclick="submeter();">
	  	  <input type="button" class="botao" name="bt" value="Novo Cargo" onclick="location.href='?modulo=sistema/tabelaapoio/cargoresponsavel/inserecargo&acao=A';">
	      </tr>
</table>
</form>
<?
 if($_REQUEST['cargodsc']){
	$where = " WHERE crgstatus = 'A' and upper(crgdsc) like upper('%{$_REQUEST['crgodsc']}%') ";
 }else{
 	$where = "where crgstatus = 'A'";
 }
	$sql = "SELECT '<center>
						<a style=\"cursor:pointer;\" onclick=\"alterarCargo(\''||crgid||'\');\">
							<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
						</a>
						<a  style=\"cursor:pointer;\" onclick=\"removerCargo(\''||crgid||'\');\">
							<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
						</a>
					</center>' as acao, 
				crgdsc
				FROM painel.cargoresponsavel 
				$where
				ORDER BY crgdsc";
	$cabecalho = array("","Título");
	$db->monta_lista($sql,$cabecalho,100,5,'N','95%',$par2);
?>
<script type="text/javascript">
function submeter(){
	document.formulario.submit();	
}

function removerCargo(cargoid){
	var conf = confirm("Você realmente deseja excluir este Cargo?");	
	if(conf) {
		location.href="?modulo=sistema/tabelaapoio/cargoresponsavel/cadastrocargo&acao=A&evento=E&crgid="+cargoid;	
	}
}
function alterarCargo(cargoid){
	location.href='?modulo=sistema/tabelaapoio/cargoresponsavel/inserecargo&acao=A&crgid='+cargoid;
}


</script>