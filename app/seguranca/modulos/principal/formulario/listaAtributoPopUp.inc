<?php

if($_REQUEST['carregaAtributosAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	carregaAtributosAjaxPopUp($_REQUEST['atrid'], $_REQUEST['atrnome']);
	exit;
}

monta_titulo( 'Filtro de Pesquisa', '' );
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel="stylesheet" type="text/css" href="include/formulario.css"/>
<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>

<script type="text/javascript" src="/includes/prototype.js"></script>

<form id="frmlistaAtributo" name="frmlistaAtributo" action="" method="post" enctype="multipart/form-data">
<div id="erro">
</div>
<table id="tblGerenciarGrupo" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Identificador:</td>
		<td><?=campo_texto( 'atrid', 'N', 'S', '', 40, 250, '', '','','','','id="atrid"');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td><?=campo_texto( 'atrnome', 'N', 'S', '', 40, 250, '', '','','','','id="atrnome"');?></td>
	</tr>
	<th colspan="2"><input type="button" value="Pesquisar" name="btnPesquisar" onclick="Pesquisa();"></th>
</table>
<div id="lista" style="display: ''">
<? carregaAtributosAjaxPopUp('','');	?>	
</div>
</form>
<script type="text/javascript">
function Pesquisa(){
	$('loader-container').show();
	
	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/listaAtributoPopUp&acao=A', {
				        method:     'post',
				        asynchronous: false,
				        parameters: '&carregaAtributosAjax=true&atrid='+$('atrid').value+
				        										'&atrnome='+$('atrnome').value,
				        onComplete: function (res){	

							$('lista').style.display = '';
							$('lista').innerHTML = res.responseText;
							$('loader-container').hide(); 
				        }
				  });
}
function carregaAtributo(atrid){
	
	window.opener.document.getElementById('atrid').value = atrid;
	window.opener.montaCampoValorPadrao( atrid, '');
	self.close();
}

function verificaOpcao(atrid, atrnome){
	window.open('seguranca.php?modulo=principal/formulario/listarOpcoes&acao=A&atrnome='+atrnome+'&atrid='+atrid,'page1212','toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,width=600,height=480');
}
</script>