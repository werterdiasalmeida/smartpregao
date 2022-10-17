<?

/*
 * TESTANDO SE A VARIAVEL FOI SELECIONADA
 * INDID É OBRIGATORIO
 */
if(!$_SESSION['indid']) {
	echo "<script>
			alert('Variáveis não foram selecionadas');
			window.location='?modulo=inicio&acao=C';
		  </script>";
	exit;
}


function salvarResponsaveis($dados) {
	global $db;
	$db->executar("DELETE FROM painel.responsavel WHERE indid='".$_SESSION['indid']."'");
	if(count($dados['entid']) > 0) {
		foreach($dados['entid'] as $entid) {
			$sql = "INSERT INTO painel.responsavel(
		            indid, entid, restipo)
		    		VALUES ('".$_SESSION['indid']."', '".$entid."', '".$dados['entidcargo'][$entid]."');";
			$db->executar($sql);
		}
	}
	$db->commit();
	echo "<script>
			alert('Responsáveis foram salvos com sucesso');
			window.location='?modulo=principal/responsaveisindicador&acao=A';
		  </script>";
	exit;
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";

$db->cria_aba($abacod_tela,$url,'');
/*
 * Criando título da TELA
 */
$titulo = "Responsáveis pelo indicador";
monta_titulo( $titulo, 'Lista das pessoas responsáveis pelo indicador' );
/*
 * Criando cabeçalho especifico do Indicador
 */
cabecalhoIndicador($_SESSION['indid']);


?>
<script>
function inserirnovoresponsavel() {
	window.open('?modulo=principal/inserirresponsaveisindicador&acao=A','Observações','scrollbars=yes,height=600,width=700,status=no,toolbar=no,menubar=no,location=no');
}
function editarresponsavel(entid) {
	window.open('?modulo=principal/inserirresponsaveisindicador&acao=A&entid='+entid,'Observações','scrollbars=no,height=500,width=600,status=no,toolbar=no,menubar=no,location=no');
}
function removerresponsavel(obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var nlinha = obj.parentNode.parentNode.rowIndex;
	var cor = new Array();
	cor[0]=tabela.rows[nlinha-1].style.background;
	cor[1]='';
	var crtcor=0;
	for(i=nlinha;i<tabela.rows.length-1;i++) {
		tabela.rows[i].style.background = cor[crtcor];
		if(crtcor) crtcor=0;
		else crtcor=1;
	}
	tabela.deleteRow(nlinha-1);
	if(tabela.rows.length == 1) {
		var linha = tabela.insertRow(0);
		var coluna = linha.insertCell(0);
		coluna.innerHTML = "Não existem responsáveis cadastrados";
		coluna.className = "title";
	}
}
</script>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data" action="" target="iframeUpload">
<input type="hidden" name="requisicao" value="salvarResponsaveis">
<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="listaresponsavel">
<tr>
	<td colspan="4" class="SubTituloEsquerda" >Responsáveis pela Secretaria/Autarquia Executora</td>
</tr>
<? $arrResponsaveis = getResponsavelSecretariaIndicador($_SESSION['indid']);?>
<?php if(is_array($arrResponsaveis)): ?>
<tr>
	<td colspan="4">
		<?php $db->monta_lista_simples($arrResponsaveis,array("Nome","E-mail","Telefone","Ceular"),100,5,'N','100%'); ?>
	</td>
<tr>
<tr>
	<td colspan="4" class="SubTituloEsquerda" >Responsáveis pelo Indicador</td>
</tr>
<?php else: ?>
<tr>
	<td colspan="4" align="center" >Não existem responsáveis atribuídos para a Secretaria/Autarquia Executora.</td>
</tr>
<?php endif; ?>
</table>
<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="listaresponsavel">
<thead>
<tr>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Ação</strong></td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Responsável</strong></td>
	<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"><strong>Cargo</strong></td>
</tr>
</thead>
<?
$responsaveis = $db->carregar("SELECT resp.entid, ent.entnome, resp.restipo FROM painel.responsavel resp 
							   LEFT JOIN entidade.entidade ent ON ent.entid = resp.entid WHERE indid='".$_SESSION['indid']."'");
$arrPflcod = pegaPerfil();
if(!$arrPflcod) $arrPflcod = array();

if($responsaveis[0]) {
	foreach($responsaveis as $key => $resp) {
		$cor = ($key%2);
		
		if(!in_array(PAINEL_PERFIL_ATUALIZA_SH,$arrPflcod) && !in_array(PAINEL_PERFIL_CONSULTA,$arrPflcod)){
			$imgs = "<img src='/imagens/alterar.gif' border=0 title='Editar' onclick=editarresponsavel('".$resp['entid']."'); style='cursor:pointer;'> 
				 <img src='/imagens/excluir.gif' border=0 onclick=removerresponsavel(this); title='Editar' style='cursor:pointer;'>";
		}else{
			$imgs = "<img src='/imagens/alterar_01.gif' border=0 title='Editar' onclick=\"alert('Operação Não Permitida!');\" style='cursor:pointer;'> 
				 <img src='/imagens/excluir_01.gif' border=0 onclick=\"alert('Operação Não Permitida!');\" title='Editar' style='cursor:pointer;'>";
		}
		
		if(in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod) && !$db->testa_superuser()){
			$sql= "SELECT 
						indid
					FROM 
						painel.indicador
					WHERE
						acaid in (
									select 
										distinct acaid
									from
										painel.usuarioresponsabilidade
									where
										usucpf = '{$_SESSION['usucpf']}'
									and
										rpustatus = 'A'
									)";
			$arrIndicadores = $db->carregar($sql);
		}
		if(in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) && !$db->testa_superuser()){
			$sql= "SELECT 
						indid
					FROM 
						painel.indicador
					WHERE
						exoid in (
									select 
										distinct exoid
									from
										painel.usuarioresponsabilidade
									where
										usucpf = '{$_SESSION['usucpf']}'
									and
										rpustatus = 'A'
									)";
			$arrIndicadores = $db->carregar($sql);
		}
		
		if($arrIndicadores){
			foreach($arrIndicadores as $Ind){
				$arrInd[] = $Ind['indid'];
			}
		}else{
			$arrIndicadores = array();
			$arrInd = array();
		}
		
		if((in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) || in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod)) && !in_array($indid,$arrInd)){
			$imgs = "<img src='/imagens/alterar_01.gif' border=0 title='Editar' onclick=\"alert('Operação Não Permitida!');\" style='cursor:pointer;'> 
				 <img src='/imagens/excluir_01.gif' border=0 onclick=\"alert('Operação Não Permitida!');\" title='Editar' style='cursor:pointer;'>";
		}
		
		
		echo "<tr style='background:".(($cor)?"#F7F7F7":"")."' id=\"linha_".$resp['entid']."\">
				<td align='center'><input type='hidden' name='entid[]' value='".$resp['entid']."'> 
				$imgs
				</td>
				<td>".$resp['entnome']."</td>
				<td align='center'><select name='entidcargo[".$resp['entid']."]'><option value='C' ".(($resp['restipo']=='C')?"selected":"").">Coordenador</option><option value='D' ".(($resp['restipo']=='D')?"selected":"").">Diretor</option><option value='E' ".(($resp['restipo']=='E')?"selected":"").">Equipe de Apoio</option></select></td>
			  </tr>";
	}
} else {
	echo "<tr><td valign=\"top\"align=\"center\" >Não existem responsáveis cadastrados</td></tr>";
}
?>

<? 
if(!in_array(PAINEL_PERFIL_ATUALIZA_SH,$arrPflcod) && !in_array(PAINEL_PERFIL_CONSULTA,$arrPflcod) && !in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod) && !in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod)){ ?>
	<tr>
		<td align="center" colspan="3" class="SubTituloDireita"><a style="cursor:pointer;" onclick="inserirnovoresponsavel();"><strong><img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"> Inserir novo responsável</strong></a></td>
	</tr>
	</table>
	<table class="tabela" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#C0C0C0">
		<td colspan="2">
			<div style="float: left;">
			<input type='submit' value='Gravar' onclick="document.getElementById('alteracaodados').value=0;">
			<input type='button' onclick="this.disabled=true;window.location='?modulo=principal/listartabela&acao=A&entid=<? echo $_SESSION['rehuf_var']['entid']; ?>'" value='Voltar'>
			</div>
		</td>
	</tr>
<? } 
if(in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod) && !$db->testa_superuser()){
	$sql= "SELECT 
				indid
			FROM 
				painel.indicador
			WHERE
				acaid in (
							select 
								distinct acaid
							from
								painel.usuarioresponsabilidade
							where
								usucpf = '{$_SESSION['usucpf']}'
							and
								rpustatus = 'A'
							)";
	$arrIndicadores = $db->carregar($sql);
}
if(in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) && !$db->testa_superuser()){
	$sql= "SELECT 
				indid
			FROM 
				painel.indicador
			WHERE
				exoid in (
							select 
								distinct exoid
							from
								painel.usuarioresponsabilidade
							where
								usucpf = '{$_SESSION['usucpf']}'
							and
								rpustatus = 'A'
							)";
	$arrIndicadores = $db->carregar($sql);
}

if($arrIndicadores){
	foreach($arrIndicadores as $Ind){
		$arrInd[] = $Ind['indid'];
	}
}else{
	$arrIndicadores = array();
}

if((in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) || in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod)) && !in_array($indid,$arrInd)){
?>
<table class="tabela" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#C0C0C0">
		<td colspan="2">
			<div style="float: left;">
			<input type='button' value='Gravar' onclick="alert('Operação Não Permitida!')">
			<input type='button' onclick="this.disabled=true;window.location='?modulo=principal/listartabela&acao=A&entid=<? echo $_SESSION['rehuf_var']['entid']; ?>'" value='Voltar'>
			</div>
		</td>
	</tr>
<?php } ?>
</table>
</form>