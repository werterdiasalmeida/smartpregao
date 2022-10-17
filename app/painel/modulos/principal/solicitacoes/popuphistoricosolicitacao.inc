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

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?

if($_REQUEST['solid']) {

	$sql = "SELECT sol.solid, 
				   tip.tipdesc,
				   CASE WHEN sol.solprazo >= NOW() THEN '<font color=blue>' || to_char(sol.solprazo, 'dd/mm/YYYY HH24:MI') || '</font>' ELSE '<font color=red>' || to_char(sol.solprazo, 'dd/mm/YYYY HH24:MI') || '</font>' END as solprazo,
				   to_char(sol.soldatainclusao, 'dd/mm/YYYY HH24:MI') as soldatainclusao,
				   sol.soldesc,
				   sts.stsdesc,
				   usu.usunome
			FROM painel.solicitacao sol
			LEFT JOIN painel.tiposolicitacao tip ON tip.tipid = sol.tipid 
			LEFT JOIN painel.situacaosol sts ON sts.stsid = sol.stsid
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = sol.usucpfsol
			WHERE solid='".$_REQUEST['solid']."'";
	
	$solicitacao = $db->pegaLinha($sql);
	
	$sql = "SELECT * FROM painel.resposta WHERE solid='".$_REQUEST['solid']."'";
	
	$resposta = $db->pegaLinha($sql);
	
	$sql = "SELECT enc.solid, 
				   enc.encid, 
				   enc.encdestinatario, enc.encdesc, usu.usunome, 
				   to_char(enc.encdataenc, 'dd/mm/YYYY HH24:MI') as encdataenc,
				   to_char(enc.encprazo, 'dd/mm/YYYY HH24:MI') as encprazo 
			FROM painel.encaminhamento enc 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = enc.usucpfenc 
			WHERE solid='".$_REQUEST['solid']."'";
	
	$encaminhamentos = $db->carregar($sql);

}

monta_titulo("Histórico da solicitação", "&nbsp;");

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
<td class="SubTituloCentro" colspan="4">Dados da solicitação</td>
</tr>
<tr>
<td class="SubTituloDireita">Tipo:</td>
<td><? echo $solicitacao['tipdesc']; ?></td>
<td class="SubTituloDireita">Prazo:</td>
<td><? echo $solicitacao['solprazo']; ?></td>
</tr>
<tr>
<td class="SubTituloDireita" nowrap>Usuário inclusão:</td>
<td><? echo $solicitacao['usunome']; ?></td>
<td class="SubTituloDireita">Data inclusão:</td>
<td><? echo $solicitacao['soldatainclusao']; ?></td>
</tr>
<tr>
<td class="SubTituloDireita">Situação:</td>
<td colspan="3"><b><? echo $solicitacao['stsdesc']; ?></b></td>
</tr>
<tr>
<td class="SubTituloDireita">Descrição:</td>
<td colspan="3"><? echo $solicitacao['soldesc']; ?></td>
</tr>
<tr>
<td class="SubTituloCentro" colspan="4">Dados da resposta</td>
</tr>
<?
if($resposta) {
?>
<tr>
<td class="SubTituloDireita">Resposta:</td>
<td colspan="3"><? echo $resposta['rsptxtresposta']; ?></td>
</tr>
<?
$sql = "SELECT * FROM painel.anexo WHERE rspid='".$resposta['rspid']."'";
$anexosrsp = $db->carregar($sql);

if($anexosrsp[0]) {
	foreach($anexosrsp as $key => $anxrsp) {
		$anexosr[] = "<a href=\"painel.php?modulo=principal/solicitacoes/popuphistoricosolicitacao&acao=A&requisicao=downloadarquivo&arqid=".$anxrsp['arqid']."\">".$anxrsp['anxdsc']."</a>"; 
	}
}

?>
<tr>
<td class="SubTituloDireita">Resposta:</td>
<td colspan="3"><? echo (($anexosr)?implode(",", $anexosr):"<font color=red>Não existem anexos</font>"); ?></td>
</tr>

<?
} else {
?>
<tr>
<td colspan="4"><b>Nenhuma resposta enviada</b></td>
</tr>
<?
}
?>
<tr>
<td class="SubTituloCentro" colspan="4">Dados dos encaminhamentos</td>
</tr>
<?

if($encaminhamentos[0]) {
	foreach($encaminhamentos as $enc) {
		$sql = "SELECT * FROM painel.anexo WHERE encid='".$enc['encid']."'";
		$anexosenc = $db->carregar($sql);
		
		unset($anexose);
		
		if($anexosenc[0]) {
			foreach($anexosenc as $key => $anxenc) {
				$anexose[] = "<a href=\"painel.php?modulo=principal/solicitacoes/encaminhamento&acao=A&requisicao=downloadarquivo&arqid=".$anxenc['arqid']."\">".$anxenc['anxdsc']."</a>"; 
			}
		}
?>
	<tr>
	<td class="SubTituloEsquerda" colspan="4"><i>Encaminhamento <? echo $enc['solid'].'.'.$enc['encid']; ?></i></td>
	</tr>
	<tr>
	<td class="SubTituloDireita">Destinatários:</td>
	<td colspan="3"><? echo htmlentities($enc['encdestinatario']); ?></td>
	</tr>
	<tr>
	<td class="SubTituloDireita">Descrição:</td>
	<td colspan="3"><? echo $enc['encdesc']; ?></td>
	</tr>
	<tr>
	<td class="SubTituloDireita" nowrap>Usuário encaminhamento:</td>
	<td><? echo $enc['usunome']; ?></td>
	<td class="SubTituloDireita">Data encaminhamento:</td>
	<td><? echo $enc['encdataenc']; ?></td>
	</tr>
	<tr>
	<td class="SubTituloDireita" nowrap>Anexos:</td>
	<td><? echo (($anexose)?implode(",", $anexose):"<font color=red>Não existem anexos</font>"); ?></td>
	<td class="SubTituloDireita">Prazo encaminhamento:</td>
	<td><? echo $enc['encprazo']; ?></td>
	</tr>

	<?
	$sql = "SELECT a.atdtxtresposta, res.respnome, to_char(a.atddataatend, 'dd/mm/YYYY HH24:MI') as atddataatend, a.atdid FROM painel.encaminhados e 
			LEFT JOIN painel.responsavelsecretaria res ON res.respid = e.respid
			LEFT JOIN painel.atendimento a ON a.ecaid = e.ecaid 
			WHERE e.encid = '".$enc['encid']."'";
	
	$encaminhados = $db->carregar($sql);
	
	?>
	<tr>
	<td class="SubTituloEsquerda" colspan="4"><i> ## Atendimentos do Encaminhamento <? echo $enc['solid'].'.'.$enc['encid']; ?></i></td>
	</tr>
	<?
	if($encaminhados[0]) {
		foreach($encaminhados as $en) {
			
			unset($anexosat);
			
			if($en['atdid']) {
				$sql = "SELECT * FROM painel.anexo WHERE atdid='".$en['atdid']."'";
				$anexosatd = $db->carregar($sql);
				
				unset($anexose);
				
				if($anexosatd[0]) {
					foreach($anexosatd as $key => $anxatd) {
						$anexosat[] = "<a href=\"painel.php?modulo=principal/solicitacoes/encaminhamento&acao=A&requisicao=downloadarquivo&arqid=".$anxatd['arqid']."\">".$anxatd['anxdsc']."</a>"; 
					}
				}
			}
	?>
	<tr>
	<td class="SubTituloDireita">Usuário atendimento:</td>
	<td><b><? echo $en['respnome']; ?></b></td>
	<td class="SubTituloDireita">Data atendimento:</td>
	<td><? echo (($en['atddataatend'])?$en['atddataatend']:"<font color=red>Não atendido</font>"); ?></td>
	</tr>
	<tr>
	<td class="SubTituloDireita">Atendimento:</td>
	<td colspan="3"><? echo (($en['atdtxtresposta'])?$en['atdtxtresposta']:"<font color=red>Não atendido</font>"); ?></td>
	</tr>
	<tr>
	<td class="SubTituloDireita" nowrap>Anexos:</td>
	<td colspan="3"><? echo (($anexosat)?implode(",", $anexosat):"<font color=red>Não existem anexos</font>"); ?></td>
	</tr>

	<?
		}
	}
	?>
<?
	}
}
?>
</table>
