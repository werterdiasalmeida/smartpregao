<?
/*
 * VALIDANDO SE EXISTE O ID DO INDICADOR NA SESS�O
 * � OBRIGAT�RIO A EXIST�NCIA DESTA VARIAVEL NA SESS�O
 */
if(!$_SESSION['indid']) {
	echo "<script>
			alert('N�o existe indicador selecionado.');
			window.location='?modulo=inicio&acao=C';
		  <script>";
	exit;
}

/* Seguran�a : Validando os acessos as fun��es autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('listaSerieHistorica'   =>true,
									 'removerSerieHistorica' =>true);
	break;
}
/* Fim Seguran�a : Validando os acessos as fun��es autorizadas */

if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
		exit;
	}
}

// monta cabe�alho
include APPRAIZ . "includes/cabecalho.inc";
echo "<br />";

$db->cria_aba($abacod_tela,$url,'');
monta_titulo("S�rie Hist�rica", "&nbsp;");
cabecalhoIndicador($_SESSION['indid']);

$dadosindicador = $db->pegaLinha("SELECT * FROM painel.indicador WHERE indid='".$_SESSION['indid']."'");

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="./js/painel.js"></script>
<script>
function excluirSerieHistorica(sehid) {
	var conf = confirm("Deseja realmente excluir esta s�rie historica?");
	if(conf) {
		ajaxatualizar('requisicao=removerSerieHistorica&sehid='+sehid,'');
		ajaxatualizar('requisicao=listaSerieHistorica','listaSerieHistorica');
	}
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita"><input type="button" value="Incluir dados" name="botao" onclick="window.location='?modulo=principal/preenchimentoSerieHistorica&acao=A';"></td>
</tr>
</table>
<div id="listaSerieHistorica"></div>
<script>
ajaxatualizar('requisicao=listaSerieHistorica','listaSerieHistorica');
</script>
