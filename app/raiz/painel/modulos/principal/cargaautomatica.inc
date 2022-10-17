<?
if($_REQUEST['requisicao']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicao']($_REQUEST);
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";

?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/painel.js"></script>
<?

switch($_REQUEST['evento']) {
case 'detalhe':
if($_REQUEST['indid']) $indicador = $db->pegaLinha("SELECT * FROM painel.indicador WHERE indid='".$_REQUEST['indid']."' AND colid='".COLETA_AUTOMATICA."' AND indstatus='A'");

if(!$indicador) {
	echo "<script>
			alert('Indicador requisitado não é de coleta automática e/ou não esta ativo');
			window.location='?modulo=principal/cargaautomatica&acao=A';
		  </script>";
	exit;
}
$titulo = "Gerenciar cargas automáticas";
monta_titulo( $titulo, '&nbsp;');

$cargaautomatica = $db->pegaLinha("SELECT * FROM painel.cargaautomatica WHERE indid='".$indicador['indid']."'");
?>
<form method="post" name="formulario" id="formulario">
<?
if($cargaautomatica['cauid']) {
	echo "<input type='hidden' name='requisicao' value='atualizarcargaautomatica'>";
	echo "<input type='hidden' name='cauid' value='".$cargaautomatica['cauid']."'>";
	$causql = trim($cargaautomatica['causql']);
} else {
	echo "<input type='hidden' name='requisicao' value='inserircargaautomatica'>";
}
?>
<input type="hidden" name="indid" value="<? echo $indicador['indid']; ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
<td class="SubTituloDireita">Nome do indicador:</td>
<td><? echo $indicador['indnome']; ?></td>
</tr>
<tr>
<td class="SubTituloDireita"><font size=1 color=#FF0000>*</font> Consulta SQL:</td>
<td><? echo campo_textarea( 'causql', 'S', 'S', '', 60, 5, 12000 ); ?></td>
</tr>
<tr>
<td colspan="2" align="center"><input type="submit" value="Gravar"> <input type="button" value="Voltar" onclick="window.location='?modulo=principal/cargaautomatica&acao=A';"></td>
</tr>
<tr>
<td colspan="2"><font size=1 color=#FF0000>* Ceritifique-se que a consulta retorne apenas um registro e uma coluna (somente o valor necessário para a carga)</font></td>
</tr>
</table>
</form>
<?
break;
// por padrão mostrar a lista de indicadores com carga automatica
default:
/*
 * Criando título da TELA
 */
$titulo = "Lista de cargas automáticas";
monta_titulo( $titulo, '&nbsp;');

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr><td id="listacargaautomatica"></td></tr>
</table>
<script>
function testarconsulta() {
	ajaxatualizar('requisicao=testarconsulta','resultadoconsulta');
}

function deletarcargaautomatica(cauid) {
	var conf = confirm("Deseja realmente excluir o detalhamento da carga automatica deste indicador?");
	if(conf) {
		ajaxatualizar('requisicao=removercargaautomatica&cauid='+cauid,'');
		ajaxatualizar('requisicao=carregarlistacargaautomatica','listacargaautomatica');
	}
}
ajaxatualizar('requisicao=carregarlistacargaautomatica','listacargaautomatica');
</script>
<?
break;

}

?>