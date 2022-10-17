<?php
global $db;

if(empty($_REQUEST['forid'])){
	$forid = 0;
}else{
	$forid = $_REQUEST['forid'];
	
	$sql = "SELECT fornome
  			   FROM formulario.formulario
  			WHERE forid = $forid";
	
	$fornome = $db->pegaUm($sql);
	
	if(!empty( $_REQUEST['gafid'])){
		$gafid = $_REQUEST['gafid'];
		
		$sql = "SELECT gafid, forid, gafnome, gafdescricao, gafordem
	  			   FROM formulario.grupoatributoformulario
	  			WHERE forid = $forid
	  			AND gafid = $gafid";
		
		$dados = $db->pegaLinha($sql);
		
		$gafnome 	  = $dados["gafnome"];
		$gafdescricao = $dados["gafdescricao"];
	}
}

if ($_REQUEST['insereGrupoAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	insereGrupoAjax($_REQUEST['gafid'], $_REQUEST['forid'], $_REQUEST['gafnome'], $_REQUEST['gafdescricao']);
	exit;
}

if ($_REQUEST['listarGrupoFormularioAjax'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	listarGrupoFormularioAjax($_REQUEST['forid']);
	exit;
}

if ($_REQUEST['alterarGrupo'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	alterarGrupoFormularioAjax($_REQUEST['gafid'], $_REQUEST['forid']);
	exit;
}

if ($_REQUEST['excluiGrupo'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	excluiGrupoFormularioAjax($_REQUEST['gafid'], $_REQUEST['forid']);
	exit;
}

monta_titulo( 'Gerenciar Grupo de Atributos', '' );
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

<form id="frmGerenciarGrupo" name="frmGerenciarGrupo" action="" method="post" enctype="multipart/form-data">
<div id="erro">
</div>
<table id="tblGerenciarGrupo" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:19.5%;">Formulários</td>
		<td><?=$fornome ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome:</td>
		<td><?php echo campo_texto( 'gafnome', 'S', 'S', '', 75, 240, '', '','','','','id="gafnome"'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Descrição:</td>
		<td colspan="4"><?php echo campo_textarea('gafdescricao','N', 'S', '', 78, 05, 4000,'');?></td>
	</tr>
	<tr>
		<th colspan="4"><input type="button" value="Salvar" name="btnSalvar" onclick="return validaForm();">
		<input type="button" value="Cancelar" name="btnCancelar" onclick="Voltar();"></th>
	</tr>
</table>
<input type="hidden" name="gafid" id="gafid" value="<?=$gafid ?>">
<input type="hidden" name="forid" id="forid" value="<?=$forid ?>">
</form>
<div id="lista" style="display: ''">
<?
listarGrupoFormularioAjax($forid);?>
</div>

<script type="text/javascript">

function validaForm(){
	if($('gafnome').value == ""){
		alert("O campo Nome do grupo e de preenchimento obrigatório!");
		$('gafnome').focus();
		return false;
	}else{
		insereGrupo();
	}
}

function insereGrupo(){
	$('loader-container').show();
	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/gerenciarGrupo&acao=A',{
					method:		'post',
					parameters: '&insereGrupoAjax=true&gafid='+$('gafid').value+
													  '&forid='+$('forid').value+
													  '&gafnome='+$('gafnome').value+
													  '&gafdescricao='+$('gafdescricao').value,
					asynchronous: false,
					onComplete: function(res){
						
						if(Number(res.responseText) ){
							alert('Operação realizada com sucesso!');
							listarGrupoFormulario();
						}else{
							if(res.responseText == "false"){
								alert('Operação não realizada!');
							}else if(res.responseText == "true"){
								alert('Operação realizada com sucesso!');
								listarGrupoFormulario();
							}else{
								alert(res.responseText);
							}
						}
						$('loader-container').hide();
					}			
				});
	limpaCampos();
}

function limpaCampos(){
	$('gafnome').value = "";
	$('gafdescricao').value = "";
}

function listarGrupoFormulario(){
	$('loader-container').show();
	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/gerenciarGrupo&acao=A',{
					method:		'post',
					parameters: '&listarGrupoFormularioAjax=true&forid='+$('forid').value,
					asynchronous: false,
					onComplete: function(res){
						 $('lista').innerHTML = res.responseText;						 
					}						
				});
	$('loader-container').hide();
}

function alterarGrupo(gafid, forid){
	$('loader-container').show();
	var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/gerenciarGrupo&acao=A', {
			        method:     'post',
			        parameters: '&alterarGrupo=true&gafid='+gafid+'&forid='+forid,
			        onComplete: function (res){
			        	//alert(res.responseText);
			        	arr = res.responseText.split("|");
			        	$('gafid').value = gafid;
			        	$('forid').value = forid;
			        	$('gafnome').value = arr[2];
			        	$('gafdescricao').value = arr[3];					
			        }
			  });
	$('loader-container').hide();
}

function excluiGrupo(gafid, forid){
	if(confirm("Tem certeza que deseja excluir este registro?") ){	
		$('loader-container').show();
		var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/gerenciarGrupo&acao=A', {
				        method:     'post',
				        parameters: '&excluiGrupo=true&gafid='+gafid+'&forid='+forid,
				        onComplete: function (res){
				        	if(res.responseText == "false"){
				        		alert('O grupo não pode ser excluido, pois existe vinculo com algum atributo');
				        	}else{
				        		alert('Operação realizada com sucesso');
				        		listarGrupoFormulario();
				        	}
				        }
				  });
		$('loader-container').hide();
	}
}


//function mudaPosicao(idTabela,tipo,rowIndex){
//	var table = 'formulario.grupoatributoformulario';
//	var atributoOrdem = 'gafordem';
//	var id = 'gafid';
//	
//	var tabela = document.getElementById(idTabela);
//	maxRows = tabela.rows.length - 1;
//	
//	if(tipo == "baixo" && rowIndex != maxRows){
//		var tr1 =  tabela.rows[rowIndex].innerHTML;
//		var tr2 =  tabela.rows[rowIndex + 1].innerHTML;
//		
//		//Pegando id 1
//		var ini1 = tr1.indexOf("ordem_");
//		var fim1 = tr1.indexOf("_ordem");
//		var id1 = tr1.substring(ini1,fim1);
//		id1 = id1.replace(/ordem_/, "");
//		
//		//Pegando id 2
//		var ini2 = tr2.indexOf("ordem_");
//		var fim2 = tr2.indexOf("_ordem");
//		var id2 = tr2.substring(ini2,fim2);
//		id2 = id2.replace(/ordem_/, "");
//		
//		var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/gerenciarGrupo&acao=A', {
//				        method:     'post', 
//				        parameters: '&mudaPosicaoGrupoAjax=true&id1='+id1+'&id2='+id2+'&tabela='+table+'&atributoOrdem='+atributoOrdem+'&id='+id,
//				        onComplete: function (res){
//							tabela.rows[rowIndex + 1].innerHTML = tr1;
//							tabela.rows[rowIndex].innerHTML = tr2;
//				        }
//				  });
//	}
//	if(tipo == "cima" && rowIndex != 1){
//		var tr1 =  tabela.rows[rowIndex].innerHTML;
//		var tr2 =  tabela.rows[rowIndex - 1].innerHTML;
//		
//		//Pegando id 1
//		var ini1 = tr1.indexOf("ordem_");
//		var fim1 = tr1.indexOf("_ordem");
//		var id1 = tr1.substring(ini1,fim1);
//		id1 = id1.replace(/ordem_/, "");
//		
//		//Pegando id 2
//		var ini2 = tr2.indexOf("ordem_");
//		var fim2 = tr2.indexOf("_ordem");
//		var id2 = tr2.substring(ini2,fim2);
//		id2 = id2.replace(/ordem_/, "");
//
//		var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/gerenciarGrupo&acao=A', {
//				        method:     'post',
//				        parameters: '&mudaPosicaoGrupoAjax=true&id1='+id1+'&id2='+id2+'&tabela='+table+'&atributoOrdem='+atributoOrdem+'&id='+id,
//				        onComplete: function (res){			        	
//							tabela.rows[rowIndex - 1].innerHTML = tr1;
//							tabela.rows[rowIndex].innerHTML = tr2;
//				        }
//				  });
//	}
//	listarGrupoFormulario();
//}

</script>
</body>