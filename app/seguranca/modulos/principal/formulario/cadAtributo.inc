<?php
//Chamada de programa
global $db, $atrid;

$atrid = $_REQUEST['atrid'];

if(!empty($atrid)){
	$sql = "SELECT atrid, atrnome, atrdescricao, atrtipodado, tiaid, 
				atrtamanhomax, atrcasasdecimais, atrmascara, atridrelacionado,
				ratid, atrsqlopcoes
			  FROM formulario.atributo
			WHERE atrid = $atrid";
	
	$dados 			  = $db->pegaLinha($sql);
	$atrnome 		  = $dados["atrnome"];
	$atrdescricao 	  = $dados["atrdescricao"];
	$atrtipodados 	  = $dados["atrtipodado"];
	$tiaid 			  = $dados["tiaid"];
	$atrtamanhomax 	  = $dados["atrtamanhomax"];
	$atrcasasdecimais = $dados["atrcasasdecimais"];
	$atrmascara 	  = $dados["atrmascara"];
	$atridrelacionado = $dados["atridrelacionado"];
	$ratid 			  = $dados["ratid"];
	$atrsqlopcoes 	  = $dados["atrsqlopcoes"];
}else{
	$atrid 			  = 0;
	$atrnome 		  = "";
	$atrdescricao 	  = "";
	$atrtipodados 	  = "";
	$tiaid 			  = "";
	$atrtamanhomax 	  = "";
	$atrcasasdecimais = "";
	$atrmascara 	  = "";
	$atridrelacionado = "";
	$ratid 			  = "";
	$atrsqlopcoes 	  = "";
}

if ($_REQUEST['mudaPosicaoAjax'] && $_REQUEST['opaid1'] && $_REQUEST['opaid2']) {
	header('content-type: text/html; charset=ISO-8859-1');
	mudaPosicaoAjax($_REQUEST['opaid1'],$_REQUEST['opaid2']);
	exit;
}

if($_REQUEST['VerificaAtributoAjax'] && $_REQUEST['valor']){
	header('content-type: text/html; charset=ISO-8859-1');
	VerificaAtributoAjax($_REQUEST['valor']);
	exit;
}

if($_REQUEST['geraComboOpcoes']){
	header('content-type: text/html; charset=ISO-8859-1');
	geraComboOpcoes($_REQUEST['atridrelacionado']);
	//unset($_SESSION['insereOpcao']);
	exit;
}

if($_REQUEST['validaMascaraAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	validaMascaraAjax($_REQUEST['opavalor'], $_REQUEST['atrmascara']);
	exit;
}

if($_REQUEST['limpaSessionDados']){
	header('content-type: text/html; charset=ISO-8859-1');
	limpaSessionDadosAjax();
	exit;
}

if($_REQUEST['carregaOpcoesAtributo']){
	header('content-type: text/html; charset=ISO-8859-1');
	carregaOpcoesAtributoAjax($_REQUEST['atrid']);
	exit;
}

if($_REQUEST['insereAtributoAjax'] ){	
	header('content-type: text/html; charset=ISO-8859-1');
	/*$registro_array = array($_REQUEST['atrid'], $_REQUEST['atrnome'], $_REQUEST['atrdescricao'], 
					    $_REQUEST['atrtipodados'], $_REQUEST['tiaid'], $_REQUEST['atrtamanhomax'], 
					    $_REQUEST['atrcasasdecimais'], $_REQUEST['atrmascara'], $_REQUEST['atridrelacionado'], 
					    $_REQUEST['ratid'], $_REQUEST['atrsqlopcoes'], $_REQUEST['situacao']);*/
					    
	insereAtributoAjax( $_REQUEST );
	exit;
}

if($_REQUEST['insereOpcoesArray'] ){
	header('content-type: text/html; charset=ISO-8859-1');	
	insereOpcoesArray($_REQUEST['opaid'], $_REQUEST['opaidA'], $_REQUEST['atrid'], $_REQUEST['opavalor'],
							$_REQUEST['opadescricao'], $_REQUEST['opavalorrelacionado'], $_REQUEST['opaordem']);
	exit;
}

if($_REQUEST['mudaPosicaoArray'] ){
	header('content-type: text/html; charset=ISO-8859-1');
	mudaPosicaoArrayAjax($_REQUEST['atributo'], $_REQUEST['move'], $_REQUEST['posicao']);
	exit;
}

if($_REQUEST['listaArrayOpcoes'] ){
	header('content-type: text/html; charset=ISO-8859-1');
	listaArrayOpcoesAjax();
	exit;
}

if($_REQUEST['recuperaValorSession'] ){
	header('content-type: text/html; charset=ISO-8859-1');	
	recuperaValorSessionAjax($_REQUEST['id']);
	exit;
}

if($_REQUEST['verificaOpcoesAtributoAjax'] ){
	header('content-type: text/html; charset=ISO-8859-1');
	verificaOpcoesAtributoAjax();
	exit;
}

if($_REQUEST['excluiOpcaoAtributo']){
	header('content-type: text/html; charset=ISO-8859-1');
	//excluiOpcaoAtributoAjax($_REQUEST['opaid'], $_REQUEST['atrid']);
	excluiOpcaoAtributoAjax($_REQUEST['id'], $_REQUEST['opaid']);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Atributo', "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios" );
?>
<link rel="stylesheet" type="text/css" href="include/formulario.css"/>
<body>

<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>

<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="frmAtributo" name="frmAtributo" action="" method="post" enctype="multipart/form-data" >

<table id="tblAtributo" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Identificador:</td>
		<td><?=campo_texto( 'atrid', 'N', 'N', '', 40, 20, '', '','','','','id="atrid"'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome:</td>
		<td><?= campo_texto( 'atrnome', 'S', 'S', '', 80, 100, '', '','','','','id="atrnome"'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Descrição:</td>
		<td><?= campo_textarea('atrdescricao','N', 'S', '', 80, 05, 1000,'');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Tipos de dados:</td>
		<?
		$sql_array = array(array("codigo" => "C", "descricao" => "Caracter"), 
						   array("codigo" => "N", "descricao" => "Número"),
						   array("codigo" => "D", "descricao" => "Data"),
						   array("codigo" => "B", "descricao" => "Boleano")); 
		?>
		<td><?= $db->monta_combo("atrtipodados",$sql_array, 'S','-- Selecione um tipo de dados --','', '', '',250,'S','atrtipodados');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Tipos de atributo:</td>
		<?
		$sql = "SELECT tiaid as codigo, tiacampo as descricao
					FROM formulario.tipoatributo"; 
		?>
		<td><?= $db->monta_combo("tiaid",$sql, 'S','-- Selecione um tipo de atributo --','VerificaAtributo', '', '',250,'S','tiaid');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Tamanho máximo:</td>
		<td><?= campo_texto('atrtamanhomax', 'N', 'S', '', 40, 11, '[#]', '','','','','id="atrtamanhomax"');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Quantidade de casas decimais:</td>
		<td><?= campo_texto('atrcasasdecimais', 'N', 'S', '', 40, 11, '[#]', '','','','','id="atrcasasdecimais"');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Máscara:</td>
		<td><?= campo_texto('atrmascara', 'N', 'S', '', 80, 50, '', '','','','','id="atrmascara"', '', '', '');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Vincular a outro atributo:</td>
		<?
		$sql = "SELECT a.atrid as Codigo, a.atrnome as Descricao 
				   FROM formulario.atributo a
				     inner join formulario.tipoatributo ta
					on (a.tiaid = ta.tiaid)
				  WHERE ta.tiaopcoes = true
				    AND ta.tiaselecionavariasopcoes = false
				    AND a.atrstatus = true
				ORDER BY a.atrdescricao"; 
		?>
		<td><?= $db->monta_combo("atridrelacionado", $sql, 'S','-- Selecione um vinculo --','geraComboOpcoes', '', '',250,'N','atridrelacionado');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Restrição:</td>
		<?
		$sql = "SELECT ratid as codigo, ratnome as descricao
  					FROM formulario.restricaoatributo"; 
		?>
		<td><?= $db->monta_combo("ratid",$sql, 'S','-- Selecione uma restrição --','', '', '',250,'','ratid');?></td>
	</tr>
</table>
<div id="mostraOpcoes" style="display: none">
	<? monta_titulo( '', 'Definir Opções do Atributo' );?>
	<table id="tblAtributo" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" style="width:39.5%;"><label for="radDefinirSql"><input type="radio" name="radDefinirSql" id="radDefinirSql" onclick="verificaSubFormulario();"> Definir SQL:</label> </td>
			<td>SQL<br/>(A query deve possui duas colunas, a primeira referente ao texto - deve se chamar 'descricao' - e a segunda, ao valor da opção - deve se chamar 'codigo'.)<br/>
				(Caso o atributo esteja vinculado a outro, poderá ser utilizada, no texto do SQL, a variável {ValorVinculado} para representar o valor do atributo vinculado)
				<?= campo_textarea('atrsqlopcoes','N', 'S', '', 130, 05, 100,'');?>
				<input type="button" value="Testar SQL" name="btnTestaSQL" id="btnTestaSQL" onclick="testaSQL();">
				</td>
		</tr>
	</table>
	<? monta_titulo( '', 'ou' );?>
	<table id="tblAtributo" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" style="width:39.5%;" rowspan="6"><label for="radDefinirOpcao"><input type="radio" name="radDefinirSql" id="radDefinirOpcao" onclick="verificaSubFormulario();"> Definir Opções:</label> </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="width:15.5%;">Texto: </td>
			<td><?=campo_texto('opadescricao', 'S', 'S', '', 80, 255, '', '','','','','id="opadescricao"');?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Valor: </td>
			<td><?=campo_texto('opavalor', 'S', 'S', '', 80, 255, '', '','','','','id="opavalor"', '','','');?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Valor do atributo relacionado: </td>
			<td><div id="relaciona">
				</div>
			</td>
		</tr>
		<tr>
			<td align="center" colspan="2"><input type="button" value="Incluir Opção" name="btnIncluir" id="btnIncluir" onclick="validaOpcoesAtributo();">
			<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="limpaDados();"></td>
		</tr>
		<tr>
			<td colspan="2">
				<div id="erro"> </div>
				<div id="tabela">
					
				</div>
			</td>
		</tr>
	</table>
</div>
<table id="tblAtributo" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th colspan="2"><input type="button" value="Salvar" name="btnSalvar" onclick="return validaForm();">
		<input type="button" value="Cancelar" name="btnCancelar" onclick="Voltar();">
		<input type="button" value="Novo Atributo" name="btnNovo" onclick="limparCampos();"></th>
	</tr>
</table>
<input type="hidden" name="hidOpcao" id="hidOpcao" value="true">
<input type="hidden" name="opaid" id="opaid" value="">
<input type="hidden" name="opaidA" id="opaidA" value="">
<input type="hidden" name="opaordem" id="opaordem" value="">
</form><script type="text/javascript">
var	msg = "";
VerificaAtributo( $('tiaid').value );

geraComboOpcoes();

function geraComboOpcoes(){
	var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/cadAtributo&acao=A', {
			        method:     'post',
			        parameters: '&geraComboOpcoes=true&atridrelacionado='+$('atridrelacionado').value,
			        asynchronous: false,
			        onComplete: function (res){	
						$('relaciona').innerHTML = res.responseText;
			        }
			  });
}

/*
* Lista os dados da tabela de opcoes do atributo
*/
function listaArrayOpcoes(){

	var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/cadAtributo&acao=A', {
			        method:     'post',
			        parameters: '&listaArrayOpcoes=true',
			        asynchronous: false,
			        onComplete: function (res){	
						$('tabela').innerHTML = res.responseText;							
			        }
			  });
}
/*
* Visualiza a informação selecionada na tabela de opções do atributo
*/

function visualizaOpcao(id, opaid){
	$('loader-container').show();
	var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/cadAtributo&acao=A', {
			        method:     'post',
			        parameters: '&recuperaValorSession=true&id='+id,
			        onComplete: function (res){
			        	arr = res.responseText.split("|");
			        	$('opaid').value = id;
			        	$('opaidA').value = opaid,
			        	$('atrid').value = arr[1];
			        	$('opadescricao').value = arr[2];
			        	$('opavalor').value = arr[3];
			        	$('opavalorrelacionado').value = arr[4];
			        	$('opaordem').value = arr[5];						
			        }
			  });
	$('loader-container').hide();
}

/*
* Deleta a informação selecionada na tabela de opções do atributo
*/

function excluiOpcaoAtributo(id, opaid){
	if(confirm("Tem certeza que deseja excluir este registro?") ){	
		$('loader-container').show();
		var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/cadAtributo&acao=A', {
				        method:     'post',
				        parameters: '&excluiOpcaoAtributo=true&id='+id+'&opaid='+opaid,
				        onComplete: function (res){
				        	
				        	//$('erro').innerHTML = res.responseText;
				        	if(res.responseText.length > 100){
				        		$('erro').innerHTML = res.responseText;
				        	}else{
				        		if(res.responseText != ""){
				        			alert(res.responseText);
				        		}
				        		listaArrayOpcoes();
				        	}
				        }
				  });
		$('loader-container').hide();
	}
}
/*
* muda a posição dos registros dentro do array
*/

function mudaPosicaoArray(atributo, movimento, posicao){
	$('loader-container').show();
	var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/cadAtributo&acao=A', {
			        method:     'post',
			        parameters: '&mudaPosicaoArray=true&atributo='+atributo+'&move='+movimento+'&posicao='+posicao,
			        onComplete: function (res){
			        	$('erro').innerHTML = res.responseText;
			        	listaArrayOpcoes();
			        }
			  });
	$('loader-container').hide();
}


//verifica se tipo de atributo escolhido exigir Opções
function VerificaAtributo(valor){

	if(valor != ""){
		var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/cadAtributo&acao=A',{
						method:		'post',
						parameters: '&VerificaAtributoAjax=true&valor=' + valor,
						asynchronous: false,
						onComplete: function(res){
							//alert(res.responseText);
							if(res.responseText == "false"){
								$('mostraOpcoes').style.display = 'none';
							}else{
								$('mostraOpcoes').style.display = '';
								if($('atrsqlopcoes').value == ""){
									desabilitaSQL();
									$('radDefinirOpcao').checked = true;
									habilitaOpcoes();
									verificaSubFormulario();
									carregaOpcoesAtributo();//Carrega os registro da tabela opcoes do atributo para um array
									listaArrayOpcoes();
								}else{
									habilitaSQL();
									desabilitaOpcoes();
									$('radDefinirSql').checked = true;
								}
							}
						}						
					});
	}else{
		$('mostraOpcoes').style.display = 'none';
	}
}

/*
* Carrega os registro da tabela opcoes do atributo para um array
*/

function carregaOpcoesAtributo(){

	var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/cadAtributo&acao=A',{
					method:		'post',
					parameters: '&carregaOpcoesAtributo=true&atrid=' + $('atrid').value,
					onComplete: function(res){
						listaArrayOpcoes();
						//$('erro').innerHTML = res.responseText;
					}						
				});
}

/*
* Valida as informações inseridas do atributo
*/

function validaForm(){
	if($('atrnome').value == ""){
		alert("O campo Nome do Atributo é de preenchimento obrigatório!");
		$('atrnome').focus();
		return false;
	}else if($('atrtipodados').value == ""){
		alert("O campo Tipo de Dados é de preenchimento obrigatório!");
		$('atrtipodados').focus();
		return false;
	}else if($('tiaid').value == ""){
		alert("O campo Tipo de Atributo é de preenchimento obrigatório!");
		$('tiaid').focus();
		return false;
	}	
	/*else if(VerificaAtributo( $('tiaid').value ) ){
		
	}*/
	else{
		var retorno;
		
		var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/cadAtributo&acao=A',{
					method:		'post',
					parameters: '&verificaOpcoesAtributoAjax=true',
					asynchronous: false,
					onComplete: function(res){
						
						if(Number(res.responseText) ){
							retorno = "true";
						}else{
							retorno = "false";
						}
					}						
				});
		if(retorno == "false"){
			alert("É necessário definir algumas opções");
			//$('radDefinirSql').checked = true;
			//verificaSubFormulario();
			return false;
		}else{
			insereAtributo();
		}
	}
}
function validaOpcoesAtributo(){
	if($('opadescricao').value == ""){
		alert("O campo Texto é de preenchimento obrigatório");
		$('opadescricao').focus();
		return false;
	}else if($('opavalor').value == ""){
		alert("O campo Valor é de preenchimento obrigatório");
		$('opavalor').focus();
		return false;
	}else{
		if($('obrigatorio').value == "sim" ){
			alert("O campo Valor do atributo relacionado é de preenchimento obrigatório");
			$('opavalorrelacionado').focus();
			return false;
		}else{
			insereOpcoes();
		}
	}
	
}

function validaOpcoesAtributo1(){
	if($('opadescricao').value == ""){
		alert("O campo Texto é de preenchimento obrigatório");
		$('opadescricao').focus();
	}else if($('opavalor').value == ""){
		alert("O campo Valor é de preenchimento obrigatório");
		$('opavalor').focus();
	}else{
		insereOpcoes();
	}
}

//função responsavel pela inserção dos dados na tabela atributo
function insereAtributo(){
	var situacao = "";
	var nome = "";
	
	$('loader-container').show();
	
	var req = new Ajax.Request("seguranca.php?modulo=principal/formulario/cadAtributo&acao=A",{
					method:		"post",
					parameters: "&insereAtributoAjax=true&atrid=" + $("atrid").value+
													    "&atrnome="+$("atrnome").value+
													    "&atrdescricao="+$("atrdescricao").value+
													    "&atrtipodados="+$("atrtipodados").value+
													    "&tiaid="+$("tiaid").value+
													    "&atrtamanhomax="+$("atrtamanhomax").value+
													    "&atrcasasdecimais="+$("atrcasasdecimais").value+
													    "&atrmascara="+$("atrmascara").value+
													    "&atridrelacionado="+$("atridrelacionado").value+
													    "&ratid="+$("ratid").value+
													    "&atrsqlopcoes="+$("atrsqlopcoes").value,
					asynchronous: false,
					onComplete: function(res){
						/*
						* este if trata o retorno dos insert.
						* se retorno for numero significa que o registro foi cadastrado com sucesso.
						*/
						//$('erro').innerHTML = res.responseText;
						if(Number(res.responseText)){
							if(msg == ""){
								alert("Operação realizada com sucesso");
								$('atrid').value = res.responseText;
							}else{
								msg = "";
							}
							if($('hidOpcao').value == "true"){
								window.location.href = "seguranca.php?modulo=principal/formulario/listarAtributo&acao=A";
								limpaSessionDados();
							}
						}else{
							/* 
							* retorno igual a true, isto e, cadastro realizado com sucesso
							* codição usada para tratar o retorno das alterações 
							*/
							if(res.responseText == "true"){ // registro alterado com sucesso
								alert("Operação realizada com sucesso");
								window.location.href = "seguranca.php?modulo=principal/formulario/listarAtributo&acao=A";
								limpaSessionDados();
							}else if(res.responseText == "false"){ // registro não alterado com sucesso
								alert("Operação não realizada");
							}else{
								if(res.responseText.length < 120){
									alert(res.responseText); // registro já cadastrado
								}
								//$('erro').innerHTML = res.responseText;
							}
						}						
					}						
				});
	$('loader-container').hide();
}

//insere dados na tabela opcoesatributos
function insereOpcoes(){
	var nome = "";
	var id = "";
	var valor = "";
	var descricao;
	var val;
	var valorrelacioando;
	var ordem;
	
	if(validaTipoDado()){
		
		if ($('atrid').value == 0){
			$('hidOpcao').value = "false";
		}else{
			$('hidOpcao').value = "true";
		}

		$('loader-container').show();
		
		var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/cadAtributo&acao=A',{
						method:		'post',
						parameters: '&insereOpcoesArray=true&opaid='+$('opaid').value+
															'&opaidA='+$('opaidA').value+
															'&atrid='+$('atrid').value+
															'&opavalor='+$('opavalor').value+
															'&opadescricao='+$('opadescricao').value+
															'&opavalorrelacionado='+$('opavalorrelacionado').value+
															'&opaordem='+$('opaordem').value,
						asynchronous: false,
						onComplete: function(res){
							listaArrayOpcoes();
							//$('erro').innerHTML = res.responseText;
							alert(res.responseText);
							limpaDados();
							$('opadescricao').focus();
						}						
					});
		
		$('opaid').value = "";
		$('opaordem').value = "";
		
		$('loader-container').hide();
	}
}

function desabilitaOpcoes(){
	$('opavalor').disabled  = 1;
	$('opadescricao').disabled  = 1;
	//$('opavalorrelacionado').disabled  = 1;
	$('btnIncluir').disabled  = 1;
	$('btnCancelar').disabled  = 1;
}
function desabilitaSQL(){
	$('atrsqlopcoes').disabled  = 1;
	$('btnTestaSQL').disabled  = 1;
}
function habilitaOpcoes(){
	$('opavalor').disabled  = 0;
	$('opadescricao').disabled  = 0;
	//$('opavalorrelacionado').disabled  = 0;
	$('btnIncluir').disabled  = 0;
	$('btnCancelar').disabled  = 0;
}
function habilitaSQL(){
	$('atrsqlopcoes').disabled  = 0;
	$('btnTestaSQL').disabled  = 0;
}

//verifica se alguns dos subformularios estão preenchidos
function verificaSubFormulario(){
	var marcado = -1;
	var nome = "";
	$('loader-container').show();
	
	for(i=0; i<frmAtributo.length; i++){
		if(frmAtributo.elements[i].type == "radio"){
			if(frmAtributo.elements[i].checked){
				marcado = i;
				nome = frmAtributo.elements[i].id;
			}
		}
	}

	if(nome == "radDefinirOpcao"){
		if($('atrsqlopcoes').value != ""){
			if(confirm('Existem dados preenchidos, tem certeza que deseja continuar?')){
				habilitaOpcoes();
				desabilitaSQL();
				//carregaTabela($('atrid').value);
				$('tabela').style.display = '';
				$('atrsqlopcoes').value = "";
				$('opadescricao').focus();
			}else{
				$('radDefinirSql').checked = true;
				$('tabela').style.display = 'none';
			}
		}else{
			habilitaOpcoes();
			desabilitaSQL();
			$('opadescricao').focus();
			$('tabela').style.display = '';
		}
	}
	else{
		verificaOpcoesAtributo();
	}
	
	$('loader-container').hide();
}

//verifica se existe dados carregados da tabela opcoesatributo
function verificaOpcoesAtributo(){
	var retorno;
	
	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/cadAtributo&acao=A',{
					method:		'post',
					parameters: '&verificaOpcoesAtributoAjax=true',
					asynchronous: false,
					onComplete: function(res){

						if(Number(res.responseText) ){
							
							if(confirm('Existem dados preenchidos, tem certeza que deseja continuar?')){
								habilitaSQL();
								desabilitaOpcoes();
								excluiOpcaoAtributo('','');
								$('tabela').style.display = 'none';
								retorno = true;
								limpaDados();
								$('atrsqlopcoes').focus();
							}else{
								$('radDefinirOpcao').checked = true;
								$('tabela').style.display = '';
								retorno = false;
								limpaDados();
							}
						}else{
							$('tabela').style.display = 'none';
							retorno = true;
							limpaDados();
							habilitaSQL();
							desabilitaOpcoes();
						}
					}						
				});
				return retorno;
}

function testaSQL(){
	var sql = $('atrsqlopcoes').value.toLowerCase();

	if(sql != ""){
		strSql = new String(sql);
		str = "where";
		if(strSql.search(str) == -1){
			window.open('seguranca.php?modulo=principal/formulario/testaSQL&acao=A&sql='+sql+'&vinculo='+$('atridrelacionado').value,'page','toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,width=600,height=480');
		}else{
			if($('atridrelacionado').value == ""){
				alert("Para execução do sql e necessário vincular um atributo!");
				$('atridrelacionado').focus();
				return false;
			}else{
				sql = sql.replace("where", " where");
				window.open('seguranca.php?modulo=principal/formulario/testaSQL&acao=A&sql='+sql+'&vinculo='+$('atridrelacionado').value,'page','toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,width=600,height=480');
				return true;
			}
		}
	}else{
		alert('Instrunção SQL não encontrada!');
	}
	
}

function limparCampos(){
	var tipo, nome, valor = "";
	var modo = false;
	for(i=0; i<frmAtributo.length; i++ ){
		tipo  = frmAtributo.elements[i].type;
		nome  = frmAtributo.elements[i].name;
		valor = frmAtributo.elements[i].value;
		
		if( (tipo == "text" || tipo == "textarea" || tipo == "select-one") && (nome != "no_atrdescricao") && (nome != "no_atrsqlopcoes") ){
			if( (valor != "") && (valor != '0') ){
				//alert(nome);
				modo = true;
			}
		}
	}
	if(modo){
		if(confirm('Tem certeza que deseja cancelar a Inserção/Edição dos dados?')){
			$('atrid').value = "0";
			$('atrnome').value = "";
			$('atrdescricao').value = "";
			$('atrtipodados').value = "";
			$('tiaid').value = "";
			$('atrtamanhomax').value = "";
			$('atrcasasdecimais').value = "";
			$('atrmascara').value = "";
			$('atridrelacionado').value = "";
			$('ratid').value = "";
			$('atrsqlopcoes').value = "";
			$('mostraOpcoes').style.display = 'none';
		}
	}
}
function limpaDados(){
	$('opadescricao').value = "";
	$('opavalor').value = "";
	$('opavalorrelacionado').value = "";
	//$('opadescricao').focus();
}
function Voltar(){
	var tipo, nome, valor = "";
	var modo = false;
	for(i=0; i<frmAtributo.length; i++ ){
		tipo  = frmAtributo.elements[i].type;
		nome  = frmAtributo.elements[i].name;
		valor = frmAtributo.elements[i].value;
		
		if( (tipo == "text" || tipo == "textarea" || tipo == "select-one") && (nome != "no_atrdescricao") && (nome != "no_atrsqlopcoes") ){
			if( (valor != "") && (valor != '0') ){
				modo = true;
			}
		}
	}
	if(modo){
		if(confirm('Tem certeza que deseja cancelar a Inserção/Edição dos dados?')){
			window.location.href = 'seguranca.php?modulo=principal/formulario/listarAtributo&acao=A';
			limpaSessionDados();
		}
	}else{
		window.location.href = 'seguranca.php?modulo=principal/formulario/listarAtributo&acao=A';
		limpaSessionDados();
	}
}

function limpaSessionDados(){
	
	var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/cadAtributo&acao=A', {
			        method:     'post',
			        parameters: '&limpaSessionDados=true',
			        onComplete: function (res){
				
			        }
			  });
}

function validaTipoDado(){

	var valor = $('opavalor').value;
	valor = valor.replace(".","");
	valor = valor.replace(",","");
	if($('atrtipodados').value == ""){
		alert('É necessário informar o tipo de dados');
		$('atrtipodados').focus();
		return false;
	}else{
		if($('atrtipodados').value == "N"){
			if(!Number(valor) ){
				alert("O campo valor deve ser preenchido com valor tipo numerico!");
				$('opavalor').focus();
				return false;
			}
		}else if($('atrtipodados').value == "B"){
			if( (valor != 'true') && (valor != 'false') ){
				alert("O campo valor deve ser preenchido com valor tipo boleano!");
				$('opavalor').focus();
				return false;
			}
		}else if($('atrtipodados').value == "D"){
			if( !Date.parse(valor) ){
				alert("O campo valor deve ser preenchido com valor tipo date!");
				$('opavalor').focus();
				return false;
			}
		}

		if($('atrmascara').value != ""){
				
			if(!validaMascara()){//verifica se a expressão regularae valida
				alert('O valor informado não está no padrão da máscara digitada!');
				$('opavalor').focus();
				return false;
			}
		}
			return true;
	}
}

function validaMascara(){

	var retorno = false;
	var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/cadAtributo&acao=A', {
			        method:     'post',
			        parameters: '&validaMascaraAjax=true&opavalor='+$('opavalor').value+
			        									'&atrmascara='+$('atrmascara').value,
			        asynchronous: false,
			        onComplete: function (res){
						//alert(res.responseText); 
			        	if(res.responseText == "1"){
			        		retorno = true;
			        	}else{
			        		retorno = false;
			        	}
			        }
			  });
	return retorno;
}


</script>