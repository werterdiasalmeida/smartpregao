<?php
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

/*
 * Carregando permiss�es do perfil
 */
$permissoes = verificaPerfilPainel();

if($permissoes['verindicadores'] == 'vertodos') {
	$acesso = true;
} else {
	$acesso = validaAcessoIndicadores($permissoes['verindicadores'], $_SESSION['indid']);
}


$arrPerfil = pegaPerfil();
/* 
 * Validando os acessos as fun��es autorizadas 
 */
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('carregarDetalhesTipoIndicador'      =>true,
									 'inserirDetalhesTipoIndicador'       =>true,
									 'atualizarDetalhesTipoIndicador'     =>true,
									 'carregarDetalhesTipoDadosIndicador' =>true,
									 'inserirDetalhesTipoDadosIndicador'  =>true,
									 'ordenarDetalhesTipoIndicador'       =>true,
									 'atualizarDetalhesTipoDadosIndicador'=>true,
									 'removerDetalhesTipoIndicador'       =>true,
									 'excluirDetalheTipoDadosIndicador'   =>true,
									 'removerDetalhesTipoDadosIndicador'  =>true,
									 'verificarExisteDados'               =>true);
	break;
}
/*
 * Executando a fun��o requerida
 */
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}

// monta cabe�alho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = "Detalhes do Indicador";
monta_titulo( $titulo, 'Inserir detalhes do indicador' );

cabecalhoIndicador($_SESSION['indid']); 
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="./js/painel.js"></script>
<table class="tabela" align="center">
<tr id="inserirTipoIndicador">
<td class="SubTituloDireita">
<? if(!$acesso){ ?>
<a style="cursor:pointer;"></a>
<? }else{ ?>
<a onclick="inserirDetalheTipoIndicador();" style="cursor:pointer;"><img src="../imagens/gif_inclui.gif" align="absmiddle"> <b>Inserir tipo detalhe</b></a>
<? } ?>
</td>
</tr>
</table>
<div id="detalhetipoindicador"></div>
<div id="auxiliar" style="display:none"></div>
<script>
// carregando o n�mero de registros cadastrados (m�ximo 2)
ajaxatualizar('requisicao=carregarDetalhesTipoIndicador&numeroregistro=true','auxiliar');
// verificando se o n�mero de registro � igual a 2(m�ximo), caso sim, desabilita a inser��o de outros
if(document.getElementById('auxiliar').innerHTML == '2') {
	document.getElementById('inserirTipoIndicador').style.display = 'none';
} else {
	document.getElementById('inserirTipoIndicador').style.display = '';
}
// carregando os detalhes cadastrados
ajaxatualizar('requisicao=carregarDetalhesTipoIndicador','detalhetipoindicador');
</script>