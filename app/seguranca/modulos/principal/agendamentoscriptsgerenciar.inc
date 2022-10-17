<?

include_once "./_funcoesagendamentoscripts.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando cabeçalho */
monta_titulo( "Agendamento scripts", "Agendamento para execução dos scripts");


if($_REQUEST['agsid']) {
	$ags = $db->pegaLinha("SELECT * FROM seguranca.agendamentoscripts WHERE agsid='".$_REQUEST['agsid']."'");
	$agsfile = $ags['agsfile'];
	$agsperiodicidade = $ags['agsperiodicidade'];
}

?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/seguranca.js"></script>
<script>
function selecionarPeriodicidade(valor) {
	if(valor) {
		document.getElementById('tr_agsperdetalhes').style.display = "";
		document.getElementById('td_agsperdetalhes').innerHTML = "Carregando...";
		ajaxatualizar('requisicao=pegarDetalhesPeriodicidade&agsperiodicidade='+valor,'td_agsperdetalhes');
	} else {
		document.getElementById('tr_agsperdetalhes').style.display = "none";
		document.getElementById('td_agsperdetalhes').innerHTML = "";
	}	
}

function submeterAgendamento() {
	document.getElementById('formulario').submit();
}
</script>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">

<input type="hidden" name="requisicao" value="<? echo (($ags['agsid'])?"atualizarAgendamentoScripts":"inserirAgendamentoScripts"); ?>">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloDireita'>Arquivo executado :</td>
	<td><? echo campo_texto('agsfile', 'S', 'S', 'Arquivo executado', 67, 150, "", "", '', '', 0, 'id="agsfile"' ); ?></td>
</tr>
<tr>
	<td class='SubTituloDireita'>Periodicidade :</td>
	<td>
	<?
	$pers = array(0 => array("codigo" => "diario", "descricao" => "Diário"),
				  1 => array("codigo" => "semanal", "descricao" => "Semanal"),
				  2 => array("codigo" => "mensal", "descricao" => "Mensal"));
				  
	$db->monta_combo('agsperiodicidade', $pers, 'S', 'Selecione', 'selecionarPeriodicidade', '', '', '', 'S', 'agsperiodicidade');
	?>
	</td>
</tr>
<tr id="tr_agsperdetalhes" style="display:none;">
	<td class="SubTituloDireita">Detalhamento da periodicidade :</td>
	<td id="td_agsperdetalhes"></td>
</tr>
<tr>
	<td class='SubTituloDireita' colspan="2"><input type="button" name="inseriragendamento" value="Salvar" onclick="submeterAgendamento();"> <input type="button" name="voltar" value="Voltar" onclick="window.location='seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A';"></td>
</tr>
</table>
</form>

<? if($ags['agsid']) : ?>
<script>
document.observe("dom:loaded", function() {
	document.getElementById('tr_agsperdetalhes').style.display = "";
	document.getElementById('td_agsperdetalhes').innerHTML = "Carregando...";
	ajaxatualizar('requisicao=pegarDetalhesPeriodicidade&agsperiodicidade=<? echo $ags['agsperiodicidade']; ?>&agsid=<? echo $ags['agsid']; ?>','td_agsperdetalhes');
});
</script>
<? endif; ?>