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
<!-- AUTO COMPLEMENTO -->
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/keys/keyEvents.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/tags/suggest.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<style>
	.suggestMarked
	{
		background-color: #AAAAFF;
		width:500px; 
		border-style: solid; 
		border-width: 1px; 
		border-color: #DDDDFF;
		border-top-width: 0px;
		position: relative;
		z-index: 100;
	}
	.suggestUnmarked
	{
		background-color: #EEEEFF;
		width:500px; 
		border-style: solid; 
		border-width: 1px; 
		border-color: #DDDDFF;
		border-top-width: 0px;
		z-index: 100;
		position: relative;
	}
	
	fieldset{
		width: 475px;
		padding: 10px;
	}
	
	fieldset input{
		width: 150px;
	}
	fieldset label{
		display: block;
		width: 40px;
		float: left;
	}
	
	fieldset br{
		clear: both;
	}
	
</style>


<form action="painel.php?modulo=principal/solicitacoes/encaminhamento&acao=A" enctype="multipart/form-data" method="post" name="formulario" id="formulario">
<?
if($_REQUEST['solid']) {

	$solicitacao = $db->pegaLinha("SELECT solid, 
										  tipdesc, 
										  CASE WHEN s.solprazo >= NOW() THEN '<font color=blue>' || to_char(s.solprazo, 'dd/mm/YYYY HH24:MI') || '</font>' ELSE '<font color=red>' || to_char(s.solprazo, 'dd/mm/YYYY HH24:MI') || '</font>' END as solprazo,
										  s.soldesc,
										  sts.stsdesc 
								   FROM painel.solicitacao s 
								   LEFT JOIN painel.tiposolicitacao t ON s.tipid = t.tipid 
								   LEFT JOIN painel.situacaosol sts ON sts.stsid = s.stsid 
								   WHERE solid='".$_REQUEST['solid']."'");
	
	echo "<input type=\"hidden\" name=\"solid\" value=\"".$solicitacao['solid']."\">";
}

if($_REQUEST['encid']) {

	$solicitacao = $db->pegaLinha("SELECT s.solid, 
										  t.tipdesc, 
										  CASE WHEN s.solprazo >= NOW() THEN '<font color=blue>' || to_char(s.solprazo, 'dd/mm/YYYY HH24:MI') || '</font>' ELSE '<font color=red>' || to_char(s.solprazo, 'dd/mm/YYYY HH24:MI') || '</font>' END as solprazo,
										  s.soldesc,
										  sts.stsdesc,
										  enc.encdestinatario,
										  enc.encdesc,
										  enc.encprazo,
										  ane.arqid
								   FROM painel.solicitacao s 
								   LEFT JOIN painel.encaminhamento enc ON enc.solid = s.solid 
								   LEFT JOIN painel.anexo ane ON ane.encid = enc.encid   
								   LEFT JOIN painel.tiposolicitacao t ON s.tipid = t.tipid 
								   LEFT JOIN painel.situacaosol sts ON sts.stsid = s.stsid 
								   WHERE enc.encid='".$_REQUEST['encid']."'");
	
	echo "<input type=\"hidden\" name=\"requisicao\" value=\"atualizarencaminhamento\">";
	echo "<input type=\"hidden\" name=\"encid\" value=\"".$_REQUEST['encid']."\">";
	
} else {
	echo "<input type=\"hidden\" name=\"requisicao\" value=\"inserirencaminhamento\">";
}

monta_titulo( "Encaminhamento da solicitação", "&nbsp;");

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
<td class="SubTituloDireita">Código:</td>
<td><? echo $solicitacao['solid'];  ?></td>
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
<td class="SubTituloCentro" colspan="2">Dados do encaminhamento</td>
</tr>
<tr>
<td class="SubTituloDireita">Destinatários:</td>
<td>
					<textarea type="text" name="pessoas" id="pessoas" style="width: 600px; height: 100px" ><? echo $solicitacao['encdestinatario']; ?></textarea>
					<br/>
					<div id="divSuggestPessoas" style="position: absolute; z-index: 1000;"></div>
					<script>
						objSuggestPessoas = new window.Suggest( 
							document.getElementById( "pessoas" ) , 
							document.getElementById( "divSuggestPessoas" ),
							"suggestlist.php"
						);
					</script>
</td>
</tr>
<tr>
<td class="SubTituloDireita">Anexo:</td>
<td>
<?
if($solicitacao['arqid']) {
?>
<a href="painel.php?modulo=principal/solicitacoes/encaminhamento&acao=A&requisicao=downloadarquivo&arqid=<? echo $solicitacao['arqid']; ?>">Clique aqui para fazer o download</a>
<?	
} else {
?>
<input type="file" name="arquivo">
<?
}
?>
</td>
</tr>
<tr>
<td class="SubTituloDireita">Mensagem:</td>
<td>
<?
$encdesc = $solicitacao['encdesc'];
echo campo_textarea( 'encdesc', 'N', 'S', '', '120', '10', '5000'); 
?>
</td>
</tr>
<tr>
<td class="SubTituloDireita" nowrap>Prazo para atendimento:</td>
<td>
<?

$encprazo = explode(" ", $solicitacao['encprazo']);
$encprazodata = $encprazo[0];
$encprazohora = $encprazo[1];

echo campo_data2( 'encprazodata', 'N', 'S', '', 'S' );

echo "&nbsp;&nbsp;";

$dados = array(0 => array( "codigo" => "08:00:00", "descricao" => "08:00"),
		       1 => array( "codigo" => "09:00:00", "descricao" => "09:00"),
			   2 => array( "codigo" => "10:00:00", "descricao" => "10:00"),
			   3 => array( "codigo" => "11:00:00", "descricao" => "11:00"),
			   4 => array( "codigo" => "12:00:00", "descricao" => "12:00"),
			   5 => array( "codigo" => "13:00:00", "descricao" => "13:00"),
			   6 => array( "codigo" => "14:00:00", "descricao" => "14:00"),
			   7 => array( "codigo" => "15:00:00", "descricao" => "15:00"),
			   8 => array( "codigo" => "16:00:00", "descricao" => "16:00"),
			   9 => array( "codigo" => "17:00:00", "descricao" => "17:00"),
			   10 => array("codigo" => "18:00:00", "descricao" => "18:00")
			 );
			 
$db->monta_combo('encprazohora', $dados, 'S', 'HH:MM', '', '', '', '', 'N', '', false);
?>
</td>
</tr>
</table>



<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0">
<td colspan="2" align="center">	
<input type='button' onclick="validarencaminhamento(this);" value='Salvar'> <input type='button' onclick="window.location='sig.php?modulo=principal/encaminhamento&acao=A&solid=<? echo $solicitacao['solid']; ?>'" value='Novo'>
</td>
</tr>
</table>
</form>
<?

$sql = "SELECT '<img src=../imagens/alterar.gif border=0 align=absmiddle style=cursor:pointer; onclick=window.location=\'painel.php?modulo=principal/solicitacoes/encaminhamento&acao=A&encid='|| enc.encid ||'\';> <img src=../imagens/preview.gif align=absmiddle border=0 style=cursor:pointer; onclick=window.open(\'painel.php?modulo=principal/solicitacoes/popuphistoricosolicitacao&acao=A&solid='||sol.solid||'\',\'Historico\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');>' as acoes, 
			   '<center>'|| sol.solid || '.' || enc.encid || '</center>', 
			   tip.tipdesc, 
			   enc.encdesc, 
			   to_char(enc.encdataenc, 'dd/mm/YYYY HH24:MI') as encdataenc, 
			   to_char(enc.encprazo, 'dd/mm/YYYY HH24:MI') as encprazo  
		FROM painel.encaminhamento enc 
		LEFT JOIN painel.solicitacao sol ON sol.solid = enc.solid
		LEFT JOIN painel.tiposolicitacao tip ON tip.tipid = sol.tipid 
		LEFT JOIN painel.situacaosol sts ON sts.stsid = sol.stsid 
		LEFT JOIN painel.resposta rsp ON rsp.solid = sol.solid 
		WHERE enc.solid='".$solicitacao['solid']."'";

$cabecalho = array("", "Código", "Tipo", "Descrição", "Data de envio", "Prazo do encaminhamento");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);

?>