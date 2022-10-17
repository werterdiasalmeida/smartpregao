<?php

$forid = $_REQUEST['forid'];

if($_SESSION['forid']){
	$forid   = $_SESSION['forid'];
	$fornome = $_SESSION['fornome'];
}else{
	//$forid   = 0;
	//$fornome = "";
	unset($_SESSION['forid']);
	unset($_SESSION['fornome']);
}

if(!empty($forid)){
	$sql = "SELECT forid, fornome, fordetalhamento 
			  FROM formulario.formulario
			WHERE forid = $forid
			and forstatus = true";
	
	$dados 			  	 = $db->pegaLinha($sql);
	
	$forid 			  	 = $dados['forid'];
	$fornome 		 	 = $dados["fornome"];
	$fordetalhamento  	 = $dados["fordetalhamento"];
	$_SESSION['forid']   = $forid;
	$_SESSION['fornome'] = $fornome;
}else{
	$forid 			  = 0;
	$fornome 		  = "";
	$fordetalhamento  = "";
	unset($_SESSION['forid']);
	unset($_SESSION['fornome']);
}

if($_REQUEST['insereFormularioAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	insereFormularioAjax($_REQUEST['forid'], $_REQUEST['fornome'], $_REQUEST['fordetalhamento']);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Dados do Formulário', '' );
?>
<link rel="stylesheet" type="text/css" href="include/formulario.css"/>
<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>

<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="frmFormulario" name="frmFormulario" action="" method="post" enctype="multipart/form-data">
<div id="erro">
</div>
<table id="tblAtributo" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Identificador:</td>
		<td><?=campo_texto( 'forid', 'N', 'N', '', 40, 11, '', '','','','','id="forid"'); ?></td>
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
		<th colspan="2"><input type="button" value="Salvar" name="btnSalvar" onclick="return validaForm();">
		<input type="button" value="Cancelar" name="btnCancelar" onclick="Voltar();">
		<input type="button" value="Novo Formulário" name="btnNovo" onclick="limparCampos();"></th>
		<th><span align="right"><input type="button" value="Visualizar Formulário" name="btnVisualizar" onclick="visualizarFormulario();"></span></th>
	</tr>
</table>
</form>
<script type="text/javascript">
function validaForm(){
	if($('fornome').value == ""){
		alert("O campo Nome do Formulário e de preenchimento obrigatório!");
		$('fornome').focus();
		return false;
	}else{
		insereFormulario();
	}
}

function insereFormulario(){
	$('loader-container').show();
	
	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/dadosFormulario&acao=A', {
						method:        'post',
						parameters:    'insereFormularioAjax=true&forid='+$('forid').value+
																'&fornome='+$('fornome').value+
																'&fordetalhamento='+$('fordetalhamento').value,
						asynchronous : false,
						onComplete: function(res){
							//$('erro').innerHTML = res.responseText;
							if(Number(res.responseText)){
								alert("Operação realizada com sucesso");
								$('forid').value = res.responseText;
								//Voltar();
							}else{
								if(res.responseText == "false"){
									alert("Operação não realizada!");
								}else{
									alert(res.responseText);
									Voltar();
								}
							}
							$('loader-container').hide();
						}
					});
}
function limparCampos(){
	var tipo, nome, valor = "";
	var msg = false;
	for(i=0; i<frmFormulario.length; i++ ){
		tipo  = frmFormulario.elements[i].type;
		nome  = frmFormulario.elements[i].name;
		valor = frmFormulario.elements[i].value;
		
		if( (tipo == "text" || tipo == "textarea") && (nome != "no_fordetalhamento") ){
			if( (valor != "") && (valor != '0') ){
				msg = true;
			}
		}
	}
	if(msg){
		if(confirm('Tem certeza que deseja cancelar a Inserção/Edição dos dados?')){
			$('forid').value = "0";
			$('fornome').value = "";
			$('fordetalhamento').value = "";
		}
	}
}

function Voltar(){
	window.location.href = 'seguranca.php?modulo=principal/formulario/listarFormulario&acao=A';
}

function visualizarFormulario(){
	alert('Em desenvolvimento, aguarde...');
}
</script>
</body>