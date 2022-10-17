<?php
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
monta_titulo( "Cargo de Responsável", '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

if(isset($_REQUEST['evento'])){
	
	switch($_REQUEST['evento']){
		
		case "I":
			$sql = "insert into painel.cargoresponsavel (crgdsc, crgstatus) values ('{$_POST['crgdsc']}', 'A') returning crgid ";
			$crgid = $db->pegaUm($sql);
			$db->commit($sql);
		break;
		
		case "U":
			$sql = "update painel.cargoresponsavel set crgdsc = '{$_POST['crgdsc']}' where crgid = {$_POST['crgid']}";
			$db->executar($sql);
			$db->commit($sql);
		
		
	}
	echo "<script>alert('Operação realizada com sucesso!');</script>";
	
}

$crgid = !$_REQUEST['crgid'] ? $crgid : $_REQUEST['crgid']; 

if($crgid){
	$sql = "select crgdsc from painel.cargoresponsavel where crgid = $crgid";
	$crgdsc = $db->pegaUm($sql);
}

?>
<form method="POST"  name="formulario">
<input type="hidden" name="evento" id="evento" value="<? if($crgid)echo 'U'; else echo 'I'; ?>">
<input type="hidden" name="crgid"  id="crgid" value="<?=$crgid;?>">

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<tr>
	<td align='right' class="SubTituloDireita">Cargo:</td>
    <td><? echo campo_texto('crgdsc','N','S','',75,255,'','','','','',' id ="crgdsc" '); ?>
		<img src="../imagens/obrig.gif" border="0">
	</td>
</tr>
<tr bgcolor="#cccccc">
	      <td></td>
	  	  <td>
	  	  <input type="button" class="botao" name="btg" value="Gravar" onclick="submeter();">
	  	  <input type="button" class="botao" name="btassociar3" value="Voltar" onclick="location.href='painel.php?modulo=sistema/tabelaapoio/cargoresponsavel/cadastrocargo&acao=A'"></td>
	  	  
	      </tr>
</table>
</form>
<script type="text/javascript">
function submeter() {
		if(!document.getElementById('crgdsc').value){
			alert('O campo "Cargo" é de preenchimento obrigatório!');
			return false;
		}
		else
			document.formulario.submit();
}
</script>