<?

include_once "./_funcoesagendamentoscripts.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando cabeçalho */
monta_titulo("Agendamento scripts", "Agendamento para execução dos scripts");

?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/seguranca.js"></script>
<script language="javascript" type="text/javascript" src="../includes/open_flash_chart/swfobject.js"></script>
<?

$sql = "SELECT '<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"window.location=''seguranca.php?modulo=principal/agendamentoscriptsgerenciar&acao=A&agsid='||agsid||'''\"> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"window.location=''seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A&requisicao=deletarAgendamentoScripts&agsid='||agsid||''';\"></center>' as alterar, agsfile, CASE WHEN agsperiodicidade='diario' THEN 'Diário'
																							 WHEN agsperiodicidade='semanal' THEN 'Semanal'
																							 WHEN agsperiodicidade='mensal' THEN 'Mensal' END as per
		FROM seguranca.agendamentoscripts WHERE agsstatus='A'";

$arr = $db->carregar($sql);

if($arr[0]) {
	foreach($arr as $ar) {
		$ar['link'] = ((is_file('./scripts_exec/'.$ar['agsfile']))?"<font color=blue>Sim</font>":"<font color=red>Não</font>");
		$regs[] = $ar;

	}

} else {
	$regs[] = array();
}

$cabecalho = array("&nbsp;", "Arquivo", "Periodicidade", "Existe?");

$db->monta_lista($regs,$cabecalho,100,5,'N','center',$par2);

?>

<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloDireita'><b>Os arquivos cadastrados devem estar na pasta : /var/www/simec/seguranca/www/scripts_exec/</b></td>
</tr>
<tr>
	<td class='SubTituloDireita'>
		<input type="button" name="log" value="Log de Execuções" onclick="window.location='seguranca.php?modulo=principal/logscripts&acao=A';">
		<input type="button" name="inseriragendamento" value="Inserir" onclick="window.location='seguranca.php?modulo=principal/agendamentoscriptsgerenciar&acao=A';">
	</td>
</tr>
</table>