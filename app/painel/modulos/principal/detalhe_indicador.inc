<?php
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

$permissoes = verificaPerfilPainel();

/* Segurança : Validando os acessos as funções autorizadas */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('carregarDetalhesTipoIndicador'=>true,
									 'inserirDetalhesTipoIndicador'=>true,
									 'atualizarDetalhesTipoIndicador'=>true,
									 'carregarDetalhesTipoDadosIndicador'=>true,
									 'inserirDetalhesTipoDadosIndicador'=>true,
									 'ordenarDetalhesTipoIndicador'=>true,
									 'atualizarDetalhesTipoDadosIndicador'=>true,
									 'removerDetalhesTipoIndicador'=>true,
									 'excluirDetalheTipoDadosIndicador'=>true,
									 'removerDetalhesTipoDadosIndicador'=>true);
	break;
}
/* Fim Segurança : Validando os acessos as funções autorizadas */
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}

if($permissoes['verindicadores'] == 'vertodos') {
	$acesso = true;
} else {
	$acesso = validaAcessoIndicadores($permissoes['verindicadores'], $_SESSION['indid']);
}


// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = "Detalhes do Indicador";
monta_titulo( $titulo, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );

cabecalhoIndicador($_SESSION['indid']); 
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="./js/painel.js"></script>
<table class="tabela" align="center">
<tr id="inserirTipoIndicador">
<td class="SubTituloDireita">
<?
if($acesso) {
	echo "<a onclick=\"inserirDetalheTipoIndicador();\" style=\"cursor:pointer;\"><img src=\"../imagens/gif_inclui.gif\" align=\"absmiddle\"> <b>Inserir tipo detalhe</b></a>";
} else {
	echo "&nbsp;";
}
?>
</td>
</tr>
</table>
<div id="detalhetipoindicador"></div>
<div id="auxiliar" style="display:none"></div>
<script>
ajaxatualizar('requisicao=carregarDetalhesTipoIndicador&numeroregistro=true','auxiliar');
if(document.getElementById('auxiliar').innerHTML == '2') {
	document.getElementById('inserirTipoIndicador').style.display = 'none';
}
ajaxatualizar('requisicao=carregarDetalhesTipoIndicador','detalhetipoindicador');
</script>
<script>
function inserirDetalheTipoIndicador() {
	var tdidsc = prompt("Descrição:");
	if(tdidsc) {
		ajaxatualizar('requisicao=inserirDetalhesTipoIndicador&tdidsc='+tdidsc,'');
		ajaxatualizar('requisicao=carregarDetalhesTipoIndicador','detalhetipoindicador');
		ajaxatualizar('requisicao=carregarDetalhesTipoIndicador&numeroregistro=true','auxiliar');
		if(document.getElementById('auxiliar').innerHTML == '2') {
			document.getElementById('inserirTipoIndicador').style.display = 'none';
		}
	}
}

function inserirDetalheTipoDadosIndicador(tdiid, referencia) {
	var tiddsc = prompt("Descrição:");
	if(tiddsc) {
		ajaxatualizar('requisicao=inserirDetalhesTipoDadosIndicador&tdiid='+tdiid+'&tiddsc='+tiddsc,'');
		if(document.getElementById('imagem'+tdiid).title == "menos") {
			carregarDetalheTipoDadosIndicador(tdiid, document.getElementById('imagem'+tdiid));
		}
		carregarDetalheTipoDadosIndicador(tdiid, document.getElementById('imagem'+tdiid));
	}
}

function ordenar(tdiid, acao) {
	ajaxatualizar('requisicao=ordenarDetalhesTipoIndicador&tdiid='+tdiid+'&executar='+acao,'');
	ajaxatualizar('requisicao=carregarDetalhesTipoIndicador','detalhetipoindicador');
}

function editarDetalheTipoIndicador(tdiid, referencia) {
	var linha = referencia.parentNode.parentNode.parentNode;
	var tdidsc = prompt("Descrição:", linha.cells[3].innerHTML);
	if(tdidsc) {
		ajaxatualizar('requisicao=atualizarDetalhesTipoIndicador&tdiid='+tdiid+'&tdidsc='+tdidsc,'');
		ajaxatualizar('requisicao=carregarDetalhesTipoIndicador','detalhetipoindicador');
	}
}

function excluirDetalheTipoIndicador(tdiid) {
	var conf = confirm("Deseja realmente excluir este tipo detalhe?");
	if(conf) {
		ajaxatualizar('requisicao=removerDetalhesTipoIndicador&tdiid='+tdiid,'detalhetipoindicador');
		ajaxatualizar('requisicao=carregarDetalhesTipoIndicador','detalhetipoindicador');
		ajaxatualizar('requisicao=carregarDetalhesTipoIndicador&numeroregistro=true','auxiliar');
		if(document.getElementById('auxiliar').innerHTML == '2') {
			document.getElementById('inserirTipoIndicador').style.display = 'none';
		}
	}
}
function excluirDetalheTipoDadosIndicador(tidid, tdiid, referencia) {
	var conf = confirm("Deseja realmente excluir este dado do tipo detalhe?");
	if(conf) {
		ajaxatualizar('requisicao=removerDetalhesTipoDadosIndicador&tidid='+tidid,'');
		ajaxatualizar('requisicao=carregarDetalhesTipoDadosIndicador&tdiid='+tdiid,referencia.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.id);
	}
	
}


function editarDetalheTipoDadosIndicador(tidid, tdiid, referencia) {
	var linha = referencia.parentNode.parentNode.parentNode;
	var tiddsc = prompt("Descrição:", linha.cells[1].innerHTML);
	if(tiddsc) {
		ajaxatualizar('requisicao=atualizarDetalhesTipoDadosIndicador&tidid='+tidid+'&tiddsc='+tiddsc,'');
		ajaxatualizar('requisicao=carregarDetalhesTipoDadosIndicador&tdiid='+tdiid,referencia.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode.id);
	}
	
}

function carregarDetalheTipoDadosIndicador(tdiid, referencia) {
	var linha = referencia.parentNode.parentNode.parentNode;
	var tabela = referencia.parentNode.parentNode.parentNode.parentNode;
	switch(referencia.title) {
		case 'mais':
			referencia.title = "menos";
			referencia.src = "../imagens/menos.gif";
			var novalinha = tabela.insertRow(linha.rowIndex);
			var col0 = novalinha.insertCell(0);
			var col1 = novalinha.insertCell(1);
			col0.innerHTML = "&nbsp;";
			col1.colSpan=4;
			col1.id="coluna"+novalinha.rowIndex;
			ajaxatualizar('requisicao=carregarDetalhesTipoDadosIndicador&tdiid='+tdiid,"coluna"+novalinha.rowIndex);
			break;
		case 'menos':
			referencia.title = "mais";
			referencia.src = "../imagens/mais.gif";
			tabela.deleteRow(linha.rowIndex);
			break;
	}
}
</script>