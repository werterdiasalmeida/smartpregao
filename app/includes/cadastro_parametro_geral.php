<?php
switch ($_REQUEST['ajax']) {
	case 'lista':
		$sisid = ($_REQUEST['sisid'] ? "=".$_REQUEST['sisid'] : 'IS NULL');
		$sql = "SELECT
					'<img src=\"/imagens/alterar.gif\" style=\"cursor:pointer\" title=\"Alterar\" onclick=\"editarParam('||parid||');\">&nbsp;'|| 	 
					'<img src=\"/imagens/excluir.gif\" style=\"cursor:pointer\" title=\"Excluir\" onclick=\"excluirParam('||parid||', \''||COALESCE(sisid::text,'')||'\');\">' AS acao, 	 
					parnome, 
					parvalor 
				FROM 
					seguranca.parametro 
				WHERE 
					sisid {$sisid}
				ORDER BY
					parnome";
		
		$cabecalho = array("Ação", "Nome", "Valor");
		$db->monta_lista_simples($sql,$cabecalho,200,20,'','','');
		die;
		break;
}

if ($_GET['excluir'] == 1){
	$sql = sprintf("DELETE FROM seguranca.parametro WHERE sisid=%s AND parid=%d",
					($_GET['sisid'] ? $_GET['sisid'] : 'NULL'),
					$_GET['parid']);
	$db->executar($sql);
	$db->commit();
	die("<script>
		alert('Operação realizada com sucesso!');
		window.location = '?modulo=sistema/parametro&acao=A&sisid=".$_GET['sisid']."';
		</script>");
}

if ($_POST){
	if (empty($_POST['parid'])){
		$sql = sprintf("INSERT INTO seguranca.parametro(
					    	sisid, parnome, parvalor
					    )VALUES( %s, '%s', '%s');",
						($_POST['sisid'] ? $_POST['sisid'] : 'NULL'),
						strtoupper(htmlentities($_POST['parnome'], ENT_QUOTES)),
						htmlentities($_POST['parvalor'], ENT_QUOTES));
	}else{
		$sql = sprintf("UPDATE seguranca.parametro
					    SET parnome='%s', parvalor='%s'
					    WHERE parid=%d;",
						strtoupper(htmlentities($_POST['parnome'], ENT_QUOTES)),
						htmlentities($_POST['parvalor'], ENT_QUOTES),
						$_POST['parid']);
	}
	$db->executar($sql);
	$db->commit();
	die("<script>
			alert('Operação realizada com sucesso!');
			window.location = '?modulo=sistema/parametro&acao=A&sisid=".$_POST['sisid']."';
			</script>");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo('Parâmetros', '');

$sisid 		= $_REQUEST['sisid'];
$sisHabil 	= 'S';
if ($_SESSION['sisid'] != 4){
	$sisid = $_SESSION['sisid'];
	$sisHabil = 'N';
} 

if ($_GET['parid']){
	$sisHabil = 'N';
	$sql = "SELECT * FROM seguranca.parametro WHERE parid=".$_GET['parid'];
	$dado = $db->pegaLinha($sql);
	if (is_array($dado)) extract($dado);
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form method="post" name="formulario" id="form_param">
	<input type="hidden" name="parid" value="<?php echo $parid ?>">
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita">Sistema</td>
			<td>
			<?php
				$sql = "SELECT 
							sisid AS codigo, 
							sisabrev ||' - '||sisdsc AS descricao 
						FROM 
							seguranca.sistema
						WHERE 
							sisstatus = 'A'
						ORDER BY 2";
				$db->monta_combo( 'sisid', $sql, $sisHabil, 'Selecione', 'listarParametro', '', '', '', 'S' );
			?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Nome:</td>
			<td><?= campo_texto( 'parnome', 'S', 'S', '', 50, 100, '', '' ); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Valor:</td>
			<td><?= campo_texto( 'parvalor', 'S', 'S', '', 50, 100, '', '' ); ?></td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td width="15%">&nbsp;</td>
			<td>
				<input type="button" class="botao" name="btalterar" value="Enviar" onclick="salvar();">
				<input type="button" class="botao" name="btnovo" value="Novo" onclick="novo();">
			</td>
		</tr>
		<tr id="tr_lista_param" style="display:none;">
			<td colspan="2">
				<fieldset>
					<legend>Lista</legend>
					<div id="lista_param"></div>
				</fieldset>
			</td>
		</tr>
	</table>
</form>
<script>
function salvar()
{
	var msg = '';
	if ($("[name=parnome]").val() != $("[name=parnome]").val().match(/[a-zA-Z0-9_]+/)){
//		$("[name=parnome]").value( $("[name=parnome]").value().match(/[a-zA-Z0-9_]+/) );
		msg += "No campo nome é permitido somente caracteres alpha numéricos e/ou underline (_)!\n";
	}
	
	if ($("[name=parvalor]").val() == ''){
		msg += "O campo valor é obrigatório.";
	}

	if (msg != ''){
		alert(msg);
	}else{
		$('#form_param').submit();
	}
}

function novo()
{
	location.href="?modulo=sistema/parametro&acao=A";
}

function editarParam(parid)
{
	location.href="?modulo=sistema/parametro&acao=A&parid="+parid;
}

function excluirParam(parid, sisid)
{
	if (confirm('Deseja realmente excluir?')){
		location.href="?modulo=sistema/parametro&acao=A&excluir=1&sisid="+sisid+"&parid="+parid;
	}
}

function listarParametro(sisid)
{
	sisid = sisid || '';
//	if (sisid){
		$('#tr_lista_param').show();
		$('#lista_param').html("carregando...");
		var href = location.href;
		href += '&ajax=lista&sisid='+sisid;
		$('#lista_param').load(href);
//	}else{
//		$('#tr_lista_param').hide();
//		$('#lista_param').html("");
//	}
}
<?php 
echo "listarParametro({$sisid});";
?>
</script>