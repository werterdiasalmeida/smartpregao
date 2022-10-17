<?php

unset($_SESSION['forid']);
unset($_SESSION['fornome']);

if($_REQUEST['listarFormularioAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	listarFormularioAjax($_REQUEST['forid'], $_REQUEST['fornome'], $_REQUEST['fordetalhamento']);
	exit();
}

if($_REQUEST['excluiFormularioAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	excluiFormularioAjax($_REQUEST['forid']);
	exit();
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Listagem de Formulários', '' );
?>
<link rel="stylesheet" type="text/css" href="include/formulario.css"/>
<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>

<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="frmAtributo" name="frmAtributo" action="" method="post" enctype="multipart/form-data">
<div id="erro">
</div>
<table id="tblAtributo" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Identificador:</td>
		<td><?=campo_texto( 'forid', 'N', 'S', '', 40, 11, '', '','','','','id="forid"'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome:</td>
		<td><?= campo_texto( 'fornome', 'S', 'S', '', 80, 250, '', '','','','','id="fornome"'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Descrição:</td>
		<td><?= campo_textarea('fordetalhamento','N', 'S', '', 80, 05, 2000,'');?></td>
	</tr>
	<tr>
		<th colspan="2"><input type="button" value="Pesquisar" name="btnPesquisar" onclick="pesquisarFormulario();">
		<input type="button" value="Novo Formulário" name="btnNovo" onclick="cadastrarFormulario();"></th>
		<th><span align="right"><input type="button" value="Visualizar Formulário" name="btnVisualizar" onclick="visualizarFormulario();"></span></th>
	</tr>
</table>
</form>
<div id="lista" style="display: ''">
<? listarFormularioAjax('','','');?>
</div>
<script type="text/javascript">

function pesquisarFormulario(){
	$('loader-container').show();
	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/listarFormulario&acao=A', {
						method 	   	 : 'post',
						parameters 	 : 'listarFormularioAjax=true&forid='+$('forid').value+
																'&fornome='+$('fornome').value+
																'&fordetalhamento='+$('fordetalhamento').value,
						asynchronous : false,
						onComplete   : function(res){
							
							$('lista').style.display = '';
							$('lista').innerHTML = res.responseText;
							$('loader-container').hide();
						}
					});
}

function cadastrarFormulario(){
	window.location.href = 'seguranca.php?modulo=principal/formulario/dadosFormulario&acao=A';
}

function excluiFormulario(forid){
	if(confirm("Tem certeza que deseja excluir este formulário?")){
		$('loader-container').show();

		var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/listarFormulario&acao=A', {
						method 	   	 : 'post',
						parameters 	 : 'excluiFormularioAjax=true&forid='+forid,
						asynchronous : false,
						onComplete   : function(res){
							//$('erro').innerHTML = res.responseText;
							alert(res.responseText);
							pesquisarFormulario();
						}
					});
		$('loader-container').hide();
	}
}

function alterarFormulario(forid){
	window.location.href = 'seguranca.php?modulo=principal/formulario/dadosFormulario&acao=A&forid='+forid;
}

function visualizarFormulario(){
	alert('Em desenvolvimento, aguarde...');
}
</script>
</body>