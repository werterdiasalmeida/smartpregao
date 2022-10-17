<?
/*
 * VALIDANDO SE EXISTE O ID DO INDICADOR NA SESSÃO
 * É OBRIGATÓRIO A EXISTÊNCIA DESTA VARIAVEL NA SESSÃO
 */
if(!$_SESSION['indid']) {
	echo "<script>
			alert('Não existe indicador selecionado.');
			window.location='?modulo=inicio&acao=C';
		  <script>";
	exit;
}

/* Segurança : Validando os acessos as funções autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('listaSerieHistorica'    =>true,
									 'removerSerieHistorica'  => true,
									 'bloquearSerieHistorica' => true,
									 'exportarsehcsv'         => true,
									 'atualizardatacoleta'    => true);
	break;
}
/* Fim Segurança : Validando os acessos as funções autorizadas */

if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
		exit;
	}
}

// monta cabeçalho
include APPRAIZ . "includes/cabecalho.inc";
echo "<br />";

$db->cria_aba($abacod_tela,$url,'');
monta_titulo("Série Histórica", "&nbsp;");
cabecalhoIndicador($_SESSION['indid']);

$dadosindicador = $db->pegaLinha("SELECT * FROM painel.indicador WHERE indid='".$_SESSION['indid']."'");

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="./js/painel.js"></script>
<script>
function excluirSerieHistorica(sehid) {
	var conf = confirm("Deseja realmente excluir esta série historica?");
	if(conf) {
		ajaxatualizar('requisicao=removerSerieHistorica&sehid='+sehid,'');
		ajaxatualizar('requisicao=listaSerieHistorica','listaSerieHistorica');
	}
}
function alterarBloqueio(sehid, situacao) {
	if(situacao == 'nao') {
		var conf = confirm("Deseja realmente desbloquear a serie historica?");
	} else {
		var conf = confirm("Deseja realmente bloquear a serie historica?");
	}
	if(conf) {
		ajaxatualizar('requisicao=bloquearSerieHistorica&sehid='+sehid+'&situacao='+situacao,'listaSerieHistorica');
		ajaxatualizar('requisicao=listaSerieHistorica','listaSerieHistorica');
	}
}

function exportarsehcsv(sehid) {
	window.location='painel.php?modulo=principal/listaSerieHistorica&acao=A&requisicao=exportarsehcsv&sehid='+sehid;
}

function atualizardatacoleta() {
	var conf = confirm("Deseja realmente declarar que não existem novas atualizações para este indicador?");
	if(conf) {
		ajaxatualizar('requisicao=atualizardatacoleta','');
		ajaxatualizar('requisicao=listaSerieHistorica','listaSerieHistorica');
		alert("Serie historica atualizada com sucesso");
	}
}
</script>
<?
/*
 * Carregando permissões do perfil
 */
$permissoes = verificaPerfilPainel();

if($permissoes['verindicadores'] == 'vertodos') {
	$acesso = true;
} else {
	$acesso = validaAcessoIndicadores($permissoes['verindicadores'], $_SESSION['indid']);
}

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita">
	&nbsp;
	<? if($acesso) : ?>
	<input type="button" value="Sem novas atualizações" name="botao" onclick="atualizardatacoleta();">
	<input type="button" value="Incluir dados" name="botao" onclick="window.location='?modulo=principal/preenchimentoSerieHistorica&acao=A';">
	<? endif; ?>
	
	</td>
</tr>
</table>
<div id="listaSerieHistorica"></div>
<script>
document.observe("dom:loaded", function() {
  // initially hide all containers for tab content
  ajaxatualizar('requisicao=listaSerieHistorica','listaSerieHistorica');
});
</script>
