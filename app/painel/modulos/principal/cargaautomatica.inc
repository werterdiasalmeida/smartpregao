<?
if($_REQUEST['requisicao']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicao']($_REQUEST);
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";

?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/painel.js"></script>
<script language="JavaScript">
function alterardetalhesperiodo(valor) {
	document.getElementById('detalhes_2').style.display='none';
	document.getElementById('detalhes_3').style.display='none';
	if(valor != '' && valor != '1') {
		document.getElementById('detalhes_'+valor).style.display='';
	}
}
</script>
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
	if(!is_null($cargaautomatica['caudiasem'])) {
		$caudiasem = explode(";", $cargaautomatica['caudiasem']);
	} else {
		$caudiasem = array();			
	}
	
	$caudiames = $cargaautomatica['caudiames'];
	
} else {
	echo "<input type='hidden' name='requisicao' value='inserircargaautomatica'>";
	$caudiasem = array();
	$cargaautomatica['cautipoproc'] = '1';
}
cabecalhoIndicador($indicador['indid']);
?>
<input type="hidden" name="indid" value="<? echo $indicador['indid']; ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
<td class="SubTituloDireita"><font size=1 color=#FF0000>*</font> Consulta SQL:</td>
<td><? echo campo_textarea( 'causql', 'S', 'S', '', 60, 5, 12000 ); ?></td>
</tr>
<tr>
<td class="SubTituloCentro" colspan=2>Agendamento por períodos</td>
</tr>
<tr>
<td class="SubTituloDireita">Processar este agendamento:</td>
<td><input type="radio" name="cautipoproc" id="cautipoproc_1" value="1" onclick="if(this.checked){alterardetalhesperiodo(this.value);}" <? echo (($cargaautomatica['cautipoproc'] == '1')?'checked':''); ?> > Diariamente <input type="radio" name="cautipoproc" value="2" id="cautipoproc_2" onclick="if(this.checked){alterardetalhesperiodo(this.value);}" <? echo (($cargaautomatica['cautipoproc'] == '2')?'checked':''); ?> > Semanalmente <input type="radio" name="cautipoproc" id="cautipoproc_3" value="3"  onclick="if(this.checked){alterardetalhesperiodo(this.value);}" <? echo (($cargaautomatica['cautipoproc'] == '3')?'checked':''); ?> > Mensalmente</td>
</tr>
<tr style="display:none" id="detalhes_2">
<td class="SubTituloDireita">Dias da semana:</td>
<td><input type="checkbox" name="caudiasem[]" value="1" <? echo ((in_array("1", $caudiasem))?"checked":""); ?> > Segunda-feira <input type="checkbox" name="caudiasem[]" value="2" <? echo ((in_array("2", $caudiasem))?"checked":""); ?> > Terça-feira <input type="checkbox" name="caudiasem[]" value="3" <? echo ((in_array("3", $caudiasem))?"checked":""); ?> > Quarta-feira <input type="checkbox" name="caudiasem[]" value="4" <? echo ((in_array("4", $caudiasem))?"checked":""); ?> > Quinta-feira <input type="checkbox" name="caudiasem[]" value="5" <? echo ((in_array("5", $caudiasem))?"checked":""); ?> > Sexta-feira <input type="checkbox" name="caudiasem[]" value="6" <? echo ((in_array("6", $caudiasem))?"checked":""); ?> > Sabado <input type="checkbox" name="caudiasem[]" value="0" <? echo ((in_array("0", $caudiasem))?"checked":""); ?> > Domingo</td>
</tr>
<tr style="display:none" id="detalhes_3">
<td class="SubTituloDireita">Dia do mês:</td>
<td><? echo campo_texto('caudiames', "N", "S", "Dia do mês", 3, 2, "", "", "", "", 0, "id='caudiames'"); ?></td>
</tr>
<tr bgcolor="#cccccc">
<td colspan="2" align="center"><input type="submit" value="Gravar"> <input type="button" value="Voltar" onclick="window.location='?modulo=principal/cargaautomatica&acao=A';"></td>
</tr>
<tr>
<td colspan="2"><font size=1 color=#FF0000>* Ceritifique-se que a consulta retorne apenas um registro e uma coluna (somente o valor necessário para a carga)</font></td>
</tr>
</table>
</form>
<? 
if($cargaautomatica['cautipoproc']) { 
?>
<script>alterardetalhesperiodo('<? echo $cargaautomatica['cautipoproc']; ?>');</script>
<?
}
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

function rodarcaragaautomatica(indid) {

	divCarregando();
	
	var myAjax = new Ajax.Request(
		'http://<? echo $_SERVER['HTTP_HOST']; ?>/painel/processaragendamento.php',
		{
			method: 'post',
			parameters: 'indidat='+indid,
			asynchronous: false,
			onComplete: function(resp) {},
			onLoading: function(){}
		});
		
	divCarregado();
}

ajaxatualizar('requisicao=carregarlistacargaautomatica','listacargaautomatica');
</script>
<?
break;

}

?>