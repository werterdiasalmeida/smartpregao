<?php
global $db;

if($_SESSION['forid']){
	$forid   = $_SESSION['forid'];
	$fornome = $_SESSION['fornome'];
}else{
	$forid   = 0;
	$fornome = "";
}

if($_REQUEST['geraComboAtributoFormulario']){
	header('content-type: text/html; charset=ISO-8859-1');
	geraComboAtributoFormulario($_REQUEST['atrid']);
	//dbg(geraComboAtributoFormulario);
	exit;
}

if($_REQUEST['geraComboGrupoAtributoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	geraComboGrupoAtributoAjax($_REQUEST['forid']);
	exit;
}

if($_REQUEST['insereAtributoFormulario']){
	header('content-type: text/html; charset=ISO-8859-1');
	insereAtributoFormularioAjax($_REQUEST);
	exit;
}

if($_REQUEST['listarAtributoFormulario']){
	header('content-type: text/html; charset=ISO-8859-1');
	if($_REQUEST['forid']){
		listarAtributoFormularioAjax($_REQUEST['forid']);
	}
	exit;
}

if($_REQUEST['carregaDadosVinculoFormulario']){
	header('content-type: text/html; charset=ISO-8859-1');
	carregaDadosVinculoFormularioAjax($_REQUEST['afoid']);
	exit;
}

if($_REQUEST['excluiVinculoFormulario']){
	header('content-type: text/html; charset=ISO-8859-1');
	excluiVinculoFormularioAjax($_REQUEST['afoid']);
	exit;
}

if($_REQUEST['montaCampoValorPadrao']){
	header('content-type: text/html; charset=ISO-8859-1');
	montaCampoValorPadraoAjax($_REQUEST['atrid'], $_REQUEST['valorPadrao']);
	exit;
}

/*if ($_REQUEST['mudaPosicaoAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	//mudaPosicao($_REQUEST['afoid1'],$_REQUEST['afoid2']);
	mudaPosicao($_REQUEST['id1'],$_REQUEST['id2'], $_REQUEST['tabela'], $_REQUEST['atributoOrdem'], $_REQUEST['id']);
	exit;
}*/

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Atributos do Formulário', '' );
?>
<link rel="stylesheet" type="text/css" href="include/formulario.css"/>
<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>

<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="frmAtributoForm" name="frmAtributoForm" action="" method="post" enctype="multipart/form-data">
<div id="erro">
</div>
<table id="tblAtributoForm" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Formulário:</td>
		<td colspan="4"><a href="seguranca.php?modulo=principal/formulario/dadosFormulario&acao=A&forid=<?php echo $forid; ?>"><?php echo $fornome; ?> </a> 
						<input type="hidden" value="<?php echo $forid; ?>" name="forid" id="forid"> </td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Atributo:</td>
		<td><div id="combo"><?php geraComboAtributoFormulario('');?></div></td>		
		<td><img src="/imagens/consultar.gif" border=0 alt="Ir" title="Pesquisa Atributo" onclick="pesquisaAtributo();" style="cursor: pointer">&nbsp;Pesquisar atributo</td>
		<td><img src="/imagens/alterar.gif" border=0 alt="Ir" title="Detalhe do Atributo" onclick="verDetalheAtributo();" style="cursor: pointer">&nbsp;Ver detalhe do atributo</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Identificador:</td>
		<td colspan="4"><?=campo_texto( 'afoid', 'N', 'N', '', 40, 11, '', '','','','','id="afoid"'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome do Atributo:</td>
		<td colspan="4"><?= campo_texto( 'afonome', 'N', 'S', '', 80, 100, '', '','','','','id="afonome"'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Descrição:</td>
		<td colspan="4"><?= campo_textarea('afodescricao','N', 'S', '', 80, 05, 255,'');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Texto de Ajuda:</td>
		<td colspan="4"><?= campo_texto( 'afotextoajuda', 'N', 'S', '', 80, 600, '', '','','','','id="afotextoajuda"'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Grupo:</td>
		<td><div id="comboGrupo"><?php //geraComboGrupoAtributoAjax('');?></div></td>
		<td colspan="2"><img src="/imagens/alterar.gif" border=0 alt="Ir" title="Gerenciar Grupo" onclick="gerenciarGrupo();" style="cursor: pointer">&nbsp;Gerenciar Grupo</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Valor Padrão:</td>
		<td colspan="4"> <div id="valorPadrao"> </div> </td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Obrigatório:</td>
		<td colspan="4"><input type="checkbox" name="afoobrigatorio" id="afoobrigatorio"> </td>
	</tr>
	<tr>
		<th colspan="4"><input type="button" value="Salvar" name="btnSalvar" onclick="return validaForm();">
		<input type="button" value="Cancelar" name="btnCancelar" onclick="limparCampos();">
		<input type="button" value="Novo Atributo do Formulário" name="btnNovo" onclick="limparCampos();"></th>
		<th><span align="right"><input type="button" value="Visualizar Formulário" name="btnVisualizar" onclick="visualizarFormulario();"></span></th>
	</tr>
</table>
</form>
<div id="lista" style="display: ''">
<?php listarAtributoFormularioAjax($forid);?>
</div>

<script type="text/javascript"><!--

geraComboGrupoAtributo($('forid').value);
montaCampoValorPadrao( $('atrid').value, '');
//listarAtributoFormulario();

function listarAtributoFormulario(){
	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/vincularAtributoFormulario&acao=A', {
				        method:     'post',
				        parameters: '&listarAtributoFormulario=true&forid='+$('forid').value,
				        onComplete: function (res){	
							//$('combo').style.display = '';
							$('lista').innerHTML = res.responseText;
				        }
				  });
}

function validaForm(){
	if($('atrid').value == ""){
		alert("O campo Atributo e de preenchimento obrigatório!");
		$('atrid').focus();
		return false;
	}else{
		$('loader-container').show();
		var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/vincularAtributoFormulario&acao=A', {
				        method:     'post',
				        parameters: '&insereAtributoFormulario=true&afoid='+$('afoid').value+
				        										'&forid='+$('forid').value+
				        										'&atrid='+$('atrid').value+
				        										'&gafid='+$('gafid').value+
				        										'&afonome='+$('afonome').value+
				        										'&afodescricao='+$('afodescricao').value+
				        										'&afotextoajuda='+$('afotextoajuda').value+
				        										'&afoobrigatorio='+$('afoobrigatorio').checked+
				        										'&afovalorpadrao='+$('afovalorpadrao').value,
				        onComplete: function (res){
				        	if(Number(res.responseText)){
				        		alert('Operação realizada com sucesso!');
				        		listarAtributoFormulario();
				        	}else{
				        		if(res.responseText == "true"){
				        			alert('Operação realizada com sucesso!');
				        			listarAtributoFormulario();
				        		}else{
				        			alert('Operação não realizada!');
				        		}
				        	}
							//$('erro').innerHTML = res.responseText;
				        }
				  });
		$('loader-container').hide();
		limparCampos();
	}	
}

function pesquisaAtributo(){
	window.open('seguranca.php?modulo=principal/formulario/listaAtributoPopUp&acao=A','page','toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,width=1000,height=400');
}

function gerenciarGrupo(){
	window.open('seguranca.php?modulo=principal/formulario/gerenciarGrupo&acao=A&forid='+$('forid').value,'page','toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,width=600,height=480');
}

function geraComboAtributoFormulario(atrid){

	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/vincularAtributoFormulario&acao=A&atrid='+atrid, {
				        method:     'post',
				        parameters: '&geraComboAtributoFormulario=true',
				        onComplete: function (res){
							$('combo').style.display = '';
							$('combo').innerHTML = res.responseText;
				        }
				  });
}

function verDetalheAtributo(){
	if($('atrid').value == ""){
		alert('É necessário informar um atributo!');
		$('atrid').focus();
		return false;
	}else{
		window.open('seguranca.php?modulo=principal/formulario/verDetalheAtributo&acao=A&atrid='+$('atrid').value,'page','toolbar=no,location=no,status=no,menubar=no,scrollbars=yes,resizable=no');
	}
}

function limparCampos(){
	
	for(i=0; i<frmAtributoForm.length; i++){
		
		if(frmAtributoForm.elements[i].type == "text" || frmAtributoForm.elements[i].type == "select-one" || frmAtributoForm.elements[i].type == "textarea" || frmAtributoForm.elements[i].type == "checkbox"){
			if(frmAtributoForm.elements[i].name != "no_afodescricao"){
				if(frmAtributoForm.elements[i].name == "afoid"){
					frmAtributoForm.elements[i].value = 0;
				}else{
					if(frmAtributoForm.elements[i].type == "checkbox"){
						frmAtributoForm.elements[i].checked = false;
					}else{
						frmAtributoForm.elements[i].value = "";
					}
				}
			}
		}
	}
}

function geraComboGrupoAtributo(forid){
					 
	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/vincularAtributoFormulario&acao=A', {
				        method:     'post',
				        parameters: '&geraComboGrupoAtributoAjax=true&forid='+forid,
				        onComplete: function (res){
							$('comboGrupo').style.display = '';
							$('comboGrupo').innerHTML = res.responseText;
				        }
				  });
}

function excluiFormulario(afoid){
	
	if(confirm("Tem certeza que deseja excluir este registro?") ){			
		var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/vincularAtributoFormulario&acao=A', {
				        method:     'post',
				        parameters: '&excluiVinculoFormulario=true&afoid='+afoid,
				        onComplete: function (res){			        
				        	alert(res.responseText);
				        	listarAtributoFormulario();
				        }
				  });
	}
}

function alterarFormulario(afoid){

	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/vincularAtributoFormulario&acao=A', {
			        method:     'post',
			        parameters: '&carregaDadosVinculoFormulario=true&afoid='+afoid,
			        onComplete: function (res){
			        	
			        	var json = res.responseText.evalJSON();
			        	
			        	arr = res.responseText.split("|");
			        	$('forid').value = json.forid;
			        	$('afoid').value = json.afoid;
        				$('atrid').value = json.atrid;
        				$('gafid').value = json.gafid;
        				$('afonome').value = json.afonome;
        				$('afodescricao').value = json.afodescricao;
        				$('afotextoajuda').value = json.afotextoajuda;
        				
        				if(json.afoobrigatorio == 't'){
        					$('afoobrigatorio').checked = true;
        				}else{
        					$('afoobrigatorio').checked = false;
        				}
        				
        				if(json.afovalorpadrao != ""){
        					montaCampoValorPadrao(json.atrid, json.afovalorpadrao);
        				}
        				
			        }
			  });

}

function montaCampoValorPadrao(atrid, valorPadrao){

	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/vincularAtributoFormulario&acao=A', {
			        method:     'post',
			        parameters: '&montaCampoValorPadrao=true&atrid='+atrid+'&valorPadrao='+valorPadrao,
			        onComplete: function (res){
			        	//alert(res.responseText);
			        	$('valorPadrao').innerHTML = res.responseText;
			        }
			  });
}

function validaTipoDado(){

	var valor = $('afovalorpadrao').value;
	valor = valor.replace(".","");
	valor = valor.replace(",","");
	
	if( $('afovalorpadrao').value != "" ){
		if($('tipoDados').value == "N"){
			if(!Number(valor) ){
				alert("O campo valor deve ser preenchido com valor tipo numerico!");
				$('afovalorpadrao').focus();
				return false;
			}
		}else if($('tipoDados').value == "B"){
			if( (valor != 'true') && (valor != 'false') ){
				alert("O campo valor deve ser preenchido com valor tipo boleano!");
				$('afovalorpadrao').focus();
				return false;
			}
		}else if($('tipoDados').value == "D"){
			if( !Date.parse(valor) ){
				alert("O campo valor deve ser preenchido com valor tipo date!");
				$('afovalorpadrao').focus();
				return false;
			}
		}else{
			return true;
		}
	}else{
		return true;
	}
}


--></script>
</body>