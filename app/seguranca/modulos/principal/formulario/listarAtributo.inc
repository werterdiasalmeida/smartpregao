<?php
//Chamada de programa
global $db;

if($_REQUEST['carregaAtributosAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$registro_array = array($_REQUEST['atrid'], $_REQUEST['atrnome'], $_REQUEST['atrtipodados'],
						 $_REQUEST['tiaid'], $_REQUEST['atridrelacionado'], $_REQUEST['ratid']);
	carregaAtributosAjax($registro_array);
	exit;
}

if($_REQUEST['excluirAtributoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	excluirAtributoAjax($_REQUEST['atrid']);
	exit;
}
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Listar Atributo', 'Filtro de Pesquisa' );
?>
<link rel="stylesheet" type="text/css" href="include/formulario.css"/>
<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="frmListaAtributo" name="frmListaAtributo" action="" method="post" enctype="multipart/form-data">
<div id="erro">
</div>
<table id="tblListaAtributo" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Identificador:</td>
		<td><?=campo_texto( 'atrid', 'N', 'S', '', 50, 20, '[#]', '','','','','id="atrid"','','','this.value=mascaraglobal(\'[#]\',this.value);'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td><?=campo_texto( 'atrnome', 'N', 'S', '', 50, 100, '', '','','','','id="atrnome"'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Tipo de dado:</td>
		<?
		$sql_array = array(array("codigo" => "C", "descricao" => "Caracter"), 
						   array("codigo" => "N", "descricao" => "Número"),
						   array("codigo" => "D", "descricao" => "Data"),
						   array("codigo" => "B", "descricao" => "Boleano")); 
		?>
		<td><?= $db->monta_combo("atrtipodados",$sql_array, 'S','-- Selecione um tipo de dados --','', '', '',250,'N','atrtipodados');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Tipos de atributo:</td>
		<?
		$sql = "SELECT tiaid as codigo, tiacampo as descricao
					FROM formulario.tipoatributo"; 
		?>
		<td><?= $db->monta_combo("tiaid",$sql, 'S','-- Selecione um tipo de atributo --','', '', '',250,'N','tiaid');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Vinculado a outro atributo:</td>
		<?
		/*$sql = "SELECT a.atrid as Codigo, a.atrnome as Descricao 
				   FROM formulario.atributo a
				     inner join formulario.tipoatributo ta
					on (a.tiaid = ta.tiaid)
				  WHERE ta.tiaopcoes = true
				    AND ta.tiaselecionavariasopcoes = false
				    AND a.atrstatus = true
				ORDER BY a.atrdescricao";*/
		 $sql = "SELECT at.atrid as codigo, at.atrnome as descricao
					FROM formulario.atributo a inner JOIN formulario.atributo at ON (a.atridrelacionado = at.atrid) 
				 LEFT JOIN formulario.opcoesatributo oa ON (a.atrid = oa.atrid)
				WHERE a.atrstatus = true 
				ORDER BY a.atrnome"; 
		?>
		<td><?= $db->monta_combo("atridrelacionado", $sql, 'S','-- Selecione um vinculo --','', '', '',250,'N','atridrelacionado');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Restrição:</td>
		<?
		$sql = "SELECT ratid as codigo, ratnome as descricao
  					FROM formulario.restricaoatributo"; 
		?>
		<td><?= $db->monta_combo("ratid",$sql, 'S','-- Selecione uma restrição --','', '', '',250,'','ratid');?></td>
	</tr>
	<tr>
		<th colspan="2"><input type="button" value="Pesquisar" name="btnPesquisar" onclick="pesquisarAtributo();">
						<input type="button" value="Novo Atributo" name="btnNovo" onclick="cadastrarAtributo();"></th>
	</tr>
</table>
</form>

<div id="lista" style="display: ''">
<?php //monta_titulo( '', 'Listagem de Atributos' );

	$array = Array();
	carregaAtributosAjax($array);
	?>
	
</div>
<script type="text/javascript">
//pesquisarAtributo();


function pesquisarAtributo(){
	var nome = "";
	$('loader-container').show();
	
	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/listarAtributo&acao=A', {
				        method:     'post',
				        parameters: '&carregaAtributosAjax=true&atrid='+$('atrid').value+
				        										'&atrnome='+$('atrnome').value+
				        										'&atrtipodados='+$('atrtipodados').value+
				        										'&tiaid='+$('tiaid').value+
				        										'&atridrelacionado='+$('atridrelacionado').value+
				        										'&ratid='+$('ratid').value,
				        onComplete: function (res){	

							$('lista').style.display = '';
							$('lista').innerHTML = res.responseText;
							$('loader-container').hide(); 
				        }
				  });
}

function cadastrarAtributo(){
	window.location.href = 'seguranca.php?modulo=principal/formulario/cadAtributo&acao=A';
}

function verificaOpcao(atrid, atrnome){
	window.open('seguranca.php?modulo=principal/formulario/listarOpcoes&acao=A&atrnome='+atrnome+'&atrid='+atrid,'page','toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,width=600,height=480');
}

function excluiAtributo(atrid){
	if(confirm("Tem certeza que deseja excluir este atributo?")){
		$('loader-container').show();
	
		var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/listarAtributo&acao=A', {
					        method:     'post',
					        parameters: '&excluirAtributoAjax=true&atrid='+atrid,
					        onComplete: function (res){	
								alert(res.responseText);
								pesquisarAtributo();
								$('loader-container').hide(); 
					        }
					  });
	}
}
</script>