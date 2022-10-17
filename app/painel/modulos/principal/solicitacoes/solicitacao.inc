<?
// Verificando a segurança
$permissoes = verificaPerfilPainel();

if($permissoes['sou_atendente'] && !$permissoes['sou_solicitante']) {
	header("location: painel.php?modulo=principal/atendimento&acao=A");
	exit;
}

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

<form action="painel.php?modulo=principal/solicitacoes/solicitacao&acao=A" method="post" name="formulario" id="formulario">
<?
if($_REQUEST['solid']) {
	echo "<input type=\"hidden\" name=\"requisicao\" value=\"atualizarsolicitacao\">";
	echo "<input type=\"hidden\" name=\"solid\" value=\"".$_REQUEST['solid']."\">";
	$solicitacao = $db->pegaLinha("SELECT * FROM painel.solicitacao WHERE solid='".$_REQUEST['solid']."'");
} else {
	echo "<input type=\"hidden\" name=\"requisicao\" value=\"inserirsolicitacao\">"; 	
}

monta_titulo( "Solicitações", "&nbsp;");

// carregando os tipos de solicitações
$sql = "SELECT * FROM painel.tiposolicitacao WHERE tipstatus='A'";
$tiposolicitacoes = $db->carregar($sql);

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
<td class="SubTituloDireita">Tipo de solicitação:</td>
<td>
<?
if($tiposolicitacoes[0]) {
	foreach($tiposolicitacoes as $key => $tpsolicitacao) {
		echo "<input type='radio' name='tipid' value='".$tpsolicitacao['tipid']."' ".((!$key && !$solicitacao['tipid'])?"checked":"")." ".(($solicitacao['tipid'] == $tpsolicitacao['tipid'])?"checked":"")."> ".$tpsolicitacao['tipdesc'];
	}
} else {
	echo "Não cadastrado";
}
?>
</td>
</tr>
<tr>
<td class="SubTituloDireita">Prazo de resposta:</td>
<td>
<?

$solprazo = explode(" ", $solicitacao['solprazo']);
$solprazodata = $solprazo[0];
$solprazohora = $solprazo[1];

echo campo_data2( 'solprazodata', 'N', 'S', '', 'S' );
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
			 
$db->monta_combo('solprazohora', $dados, 'S', 'HH:MM', '', '', '', '', 'N', 'solprazohora', false);
?>
</td>
</tr>
<tr>
<td class="SubTituloDireita">Descrição:</td>
<td><? 
$soldesc = $solicitacao['soldesc'];
echo campo_textarea( 'soldesc', 'N', 'S', '', '120', '10', '5000'); 

?></td>
</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0">
	<td colspan="2" align="center"><input type='button' onclick="validarsolicitacao(this);" value='Salvar'> <input type='button' onclick="window.location='painel.php?modulo=principal/solicitacoes/solicitacao&acao=A';" value='Novo'></td>
</tr>
</table>
</form>
<br />
<?
	$sql = "SELECT  CASE WHEN rsp.rspid IS NULL THEN '<img src=../imagens/alterar.gif border=0 align=absmiddle style=cursor:pointer; onclick=window.location=\'painel.php?modulo=principal/solicitacoes/solicitacao&acao=A&solid='|| sol.solid ||'\';><img src=../imagens/usuario.gif align=absmiddle border=0 style=cursor:pointer; onclick=window.location=\'painel.php?modulo=principal/solicitacoes/encaminhamento&acao=A&solid='|| sol.solid ||'\';>' ELSE '<img src=../imagens/fav.gif align=absmiddle border=0 style=cursor:pointer; onclick=arquivarsolicitacao(\''|| sol.solid ||'\')>' END ||'<img src=../imagens/recuo_e.gif align=absmiddle border=0 style=cursor:pointer; onclick=window.location=\'painel.php?modulo=principal/solicitacoes/resposta&acao=A&solid='|| sol.solid ||'\';><img src=../imagens/preview.gif align=absmiddle border=0 style=cursor:pointer; onclick=window.open(\'painel.php?modulo=principal/solicitacoes/popuphistoricosolicitacao&acao=A&solid='||sol.solid||'\',\'Historico\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');>' as acoes, sol.solid, tip.tipdesc, CASE WHEN sol.solprazo >= NOW() THEN '<font color=blue>' || to_char(sol.solprazo, 'dd/mm/YYYY HH24:MI') || '</font>' ELSE '<font color=red>' || to_char(sol.solprazo, 'dd/mm/YYYY HH24:MI') || '</font>' END as solprazo, sol.soldesc, sts.stsdesc, rsp.rsptxtresposta FROM painel.solicitacao sol 
			LEFT JOIN painel.tiposolicitacao tip ON tip.tipid = sol.tipid 
			LEFT JOIN painel.situacaosol sts ON sts.stsid = sol.stsid 
			LEFT JOIN painel.resposta rsp ON rsp.solid = sol.solid 
			WHERE sol.stsid != '".SITSOL_ARQUIVADO."'
			ORDER BY soldatainclusao";
	
	$cabecalho = array("", "Código", "Tipo", "Prazo", "Solicitação", "Situação", "Resposta");
	$tamanho   = array("5%","3%","10%","10%","30%","10%","30%");
	$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2,'', $tamanho);
?>
<script type="text/javascript" src="../includes/wz_tooltip.js"></script>