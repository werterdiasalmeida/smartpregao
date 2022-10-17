<?
// Verificando a segurança
$permissoes = verificaPerfilPainel();
// Fim segurança

/* Executando função (_funcoes.php) */
if($_REQUEST['requisicao']) {
	header('Content-Type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";

echo montarAbasArray(carregardadosmenupainelsolicitacoes(), $_SERVER['REQUEST_URI']);


?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/painel.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<?
if($_REQUEST['solid']) {

	$sql = "SELECT sol.solid, 
				   tip.tipdesc,
				   CASE WHEN sol.solprazo >= NOW() THEN '<font color=blue>' || to_char(sol.solprazo, 'dd/mm/YYYY HH24:MI') || '</font>' ELSE '<font color=red>' || to_char(sol.solprazo, 'dd/mm/YYYY HH24:MI') || '</font>' END as solprazo,
				   sol.soldesc,
				   sts.stsdesc,
				   rsp.rsptxtresposta,
				   rsp.rspid
			FROM painel.solicitacao sol
			LEFT JOIN painel.tiposolicitacao tip ON tip.tipid = sol.tipid 
			LEFT JOIN painel.situacaosol sts ON sts.stsid = sol.stsid 
			LEFT JOIN painel.resposta rsp ON rsp.solid = sol.solid
			WHERE sol.solid='".$_REQUEST['solid']."'";
	
	$solicitacao = $db->pegaLinha($sql);

}

monta_titulo( "Resposta da solicitação", "ommomom momo momomo momomomo momo momomomomo momomomo moom");

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
<td class="SubTituloCentro" colspan="2">Dados da solicitação</td>
</tr>
<tr>
<td class="SubTituloDireita">Código:</td>
<td><? echo $solicitacao['solid']; ?></td>
</tr>
<tr>
<td class="SubTituloDireita">Tipo:</td>
<td><? echo $solicitacao['tipdesc']; ?></td>
</tr>
<tr>
<td class="SubTituloDireita">Prazo:</td>
<td><? echo $solicitacao['solprazo']; ?></td>
</tr>
<tr>
<td class="SubTituloDireita">Descrição:</td>
<td><? echo $solicitacao['soldesc']; ?></td>
</tr>
<tr>
<td class="SubTituloDireita">Situação:</td>
<td><? echo $solicitacao['stsdesc']; ?></td>
</tr>
<tr>
<td class="SubTituloDireita">Histórico:</td>
<td><a style="cursor:pointer;" onclick="window.open('painel.php?modulo=principal/solicitacoes/popuphistoricosolicitacao&acao=A&solid=<? echo $solicitacao['solid']; ?>','Historico','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');"><img src=../imagens/preview.gif align=absmiddle border=0 style="cursor:pointer;"> Clique aqui para visualizar</a></td>
</tr>
</table>
<br />

<form action="painel.php?modulo=principal/solicitacoes/resposta&acao=A" enctype="multipart/form-data" method="post" name="formulario" id="formulario">
<?


if($solicitacao['rspid']) {
	echo "<input type=\"hidden\" name=\"requisicao\" value=\"atualizarsolicitacao\">";
	echo "<input type=\"hidden\" name=\"solid\" value=\"".$_REQUEST['solid']."\">";
} else {
	echo "<input type=\"hidden\" name=\"requisicao\" value=\"inserirresposta\">";
	echo "<input type=\"hidden\" name=\"solid\" value=\"".$_REQUEST['solid']."\">";
}

// carregando os tipos de solicitações
$sql = "SELECT * FROM painel.solicitacao sol 
		LEFT JOIN painel.encaminhamento enc ON enc.solid = sol.solid 
		LEFT JOIN painel.encaminhados ens ON ens.encid = enc.encid 
		LEFT JOIN painel.atendimento ate ON ate.ecaid = ens.ecaid 
		WHERE sol.solid='".$_REQUEST['solid']."'";

$encaminhamentos = $db->carregar($sql);

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
<td class="SubTituloCentro" colspan="2">Dados da resposta</td>
</tr>
<tr>
<td class="SubTituloDireita">Resposta:</td>
<td><? 
$rsptxtresposta = $solicitacao['rsptxtresposta'];
echo campo_textarea( 'rsptxtresposta', 'N', 'S', '', '120', '10', '5000'); 
?></td>
</tr>

<tr>
<td class="SubTituloDireita">Anexos:</td>
<td>
	<table id="anexos">
	<?
	$naopossuiarquivos=true;
	if($solicitacao['rspid']) {
		$sql = "SELECT * FROM painel.anexo WHERE rspid='".$solicitacao['rspid']."'";
		$anexosresposta = $db->carregar($sql);
		if($anexosresposta[0]) {
			$naopossuiarquivos=false;
			foreach($anexosresposta as $key => $anersp) {
	?>
	<td><a href="painel.php?modulo=principal/solicitacoes/encaminhamento&acao=A&requisicao=downloadarquivo&arqid=<? echo $anersp['arqid']; ?>"> <? echo $anersp['anxdsc']; ?></a></td>
	<?
			}
		}
	}
	
	if($naopossuiarquivos) {
	?>
	<tr>
		<td><input type="file" name="arquivo[]"></td>
		<td colspan="2">Nome do arquivo : <input class="normal" type="text" name="arquivonome[]"></td>
	</tr>
	<tr>
		<td colspan="3"><a href="javascript:void(0);" onclick="inserirNovosArquivos();">Clique aqui para adicionar novos arquivos</a></td>
	</tr>

	<?
	}
	?>
	</table>
</td>
</tr>
<?
$sql = "SELECT * FROM painel.anexo ane 
		INNER JOIN painel.atendimento ate ON ate.atdid = ane.atdid 
		INNER JOIN painel.encaminhados en ON en.ecaid = ate.ecaid 
		INNER JOIN painel.encaminhamento enc ON en.encid = enc.encid 
		INNER JOIN painel.responsavelsecretaria res ON res.respid = en.respid
		WHERE enc.solid = '".$_REQUEST['solid']."'";

$anexosatendentes = $db->carregar($sql);

?>
<tr>
<td class="SubTituloDireita">Anexos atendentes:</td>
<td>
	<table>
	<?
	if($anexosatendentes[0]) {
		foreach($anexosatendentes as $key => $anex) {
	?>
	<tr>
		<td><input type="checkbox" name="arqatendentes[]" value="<? echo $anex['anxid']; ?>" checked> <a href="painel.php?modulo=principal/solicitacoes/resposta&acao=A&requisicao=downloadarquivo&arqid=<? echo $anex['arqid']; ?>"> <? echo $anex['anxdsc']; ?> - enviado por <? echo $anex['respnome']; ?></a></td>
	</tr>
	<?
		}
	} else {
	?>
	<tr>
		<td>Nenhum anexo enviado pelos atendentes</td>
	</tr>
	<?
	}
	?>
	</table>
</td>
</tr>


</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0">
	<td colspan="2" align="center"><input type='button' onclick="validarresposta(this);" value='Salvar'></td>
</tr>
</table>
</form>

<script type="text/javascript" src="../includes/wz_tooltip.js"></script>