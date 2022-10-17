<?php
global $db, $atrid;

$atrid = $_REQUEST['atrid'];

if(!empty($atrid)){
	$sql = "SELECT a.atrid, a.atrnome, a.atrdescricao, (CASE WHEN a.atrtipodado = 'C' THEN 'Caracter'
			       WHEN a.atrtipodado = 'N' THEN 'Número'
			       WHEN a.atrtipodado = 'D' THEN 'Data'
			       ELSE 'Boleano' END) as atrtipodado, a.tiaid, ta.tiacampo, 
				a.atrtamanhomax, a.atrcasasdecimais, a.atrmascara, at.atrnome as atridrelacionado,
				ra.ratnome as ratid, a.atrsqlopcoes
			  FROM formulario.atributo a left JOIN formulario.atributo at 
			      ON (a.atridrelacionado = at.atrid) inner join formulario.tipoatributo ta
			      ON (a.tiaid = ta.tiaid) left join formulario.restricaoatributo ra
			      ON (a.ratid = ra.ratid)
			WHERE a.atrid = $atrid";
	
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
	$tiacampo		  = $dados["tiacampo"];
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

if($_REQUEST['VerificaAtributoAjax'] && $_REQUEST['valor']){
	header('content-type: text/html; charset=ISO-8859-1');
	VerificaAtributoAjax($_REQUEST['valor']);
	exit;
}

if($_REQUEST['geraComboOpcoes']){
	header('content-type: text/html; charset=ISO-8859-1');
	geraComboOpcoes($_REQUEST['atridrelacionado']);
	exit;
}

if($_REQUEST['carregaOpcoesAtributo']){
	header('content-type: text/html; charset=ISO-8859-1');
	//carregaOpcoesAtributoAjax($_REQUEST['atrid']);
	exit;
}

if($_REQUEST['listaOpcoesAtributo'] ){
	header('content-type: text/html; charset=ISO-8859-1');
	carregaTabelaOpcoes($_REQUEST['atrid']);
	exit;
}


if($_REQUEST['verificaOpcoesAtributoAjax'] ){
	header('content-type: text/html; charset=ISO-8859-1');
	verificaOpcoesAtributoAjax();
	exit;
}


//include  APPRAIZ."includes/cabecalho.inc";
//print "<br/>";
//$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Atributo', 'Carasterísticas do Atributo' );
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
<form id="frmAtributo" name="frmAtributo" action="" method="post" enctype="multipart/form-data" >

<table id="tblAtributo" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Identificador:</td>
		<td><?=campo_texto( 'atrid', 'N', 'N', '', 40, 20, '', '','','','','id="atrid"'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome:</td>
		<td><?= campo_texto( 'atrnome', 'N', 'N', '', 80, 100, '', '','','','','id="atrnome"'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Descrição:</td>
		<td><?= campo_textarea('atrdescricao','N', 'N', '', 80, 05, 1000,'');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Tipos de dados:</td>
		<td><?= campo_texto( 'atrtipodados', 'N', 'N', '', 50, 100, '', '','','','','id="atrtipodados"'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Tipos de atributo:</td>
		<td><?= campo_texto( 'tiacampo', 'N', 'N', '', 50, 100, '', '','','','','id="tiacampo"'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Tamanho máximo:</td>
		<td><?= campo_texto('atrtamanhomax', 'N', 'N', '', 40, 11, '[#]', '','','','','id="atrtamanhomax"');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Quantidade de casas decimais:</td>
		<td><?= campo_texto('atrcasasdecimais', 'N', 'N', '', 40, 11, '[#]', '','','','','id="atrcasasdecimais"');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Máscara:</td>
		<td><?= campo_texto('atrmascara', 'N', 'N', '', 80, 50, '', '','','','','id="atrmascara"', '', '', '');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Vincular a outro atributo:</td>
		<td><?= campo_texto('atridrelacionado', 'N', 'N', '', 50, 50, '', '','','','','id="atridrelacionado"', '', '', '');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Restrição:</td>
		<td><?= campo_texto('ratid', 'N', 'N', '', 50, 50, '', '','','','','id="ratid"', '', '', '');?></td>
	</tr>
</table>
<div id="mostraOpcoesSQL" style="display: none">
	<? monta_titulo( '', 'Definir Opções do Atributo' );?>
	<table id="tblAtributo" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" style="width:39.5%;"><label for="radDefinirSql"><input type="radio" name="radDefinirSql" id="radDefinirSql"> Definir SQL:</label> </td>
			<td>SQL<br/>(A query deve possui duas colunas, a primeira referente ao texto - deve se chamar 'descricao' - e a segunda, ao valor da opção - deve se chamar 'codigo'.)<br/>
				(Caso o atributo esteja vinculado a outro, poderá ser utilizada, no texto do SQL, a variável {ValorVinculado} para representar o valor do atributo vinculado)
				<?= campo_textarea('atrsqlopcoes','N', 'N', '', 130, 05, 100,'');?>
				<input type="button" value="Testar SQL" name="btnTestaSQL" id="btnTestaSQL" onclick="testaSQL();">
				</td>
		</tr>
	</table>
</div>
<div id="mostraOpcoes" style="display: none">
	<?php 
		if($atrsqlopcoes){
			monta_titulo( '', 'ou' );
		}else{
	 		monta_titulo( '', 'Definir Opções do Atributo' );
		} 
	?>
	<table id="tblAtributo" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" style="width:39.5%;" rowspan="6"><label for="radDefinirOpcao"><input type="radio" name="radDefinirSql" id="radDefinirOpcao"> Definir Opções:</label> </td>
		</tr>
		<tr>
			<td colspan="2">
				<div id="tabela"></div>
			</td>
		</tr>
	</table>
</div>
<table id="tblAtributo" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th colspan="2"><input type="button" value="Fechar" name="btnFechar" onclick="javascript: window.close();"></th>
	</tr>
</table>
<input type="hidden" name="hidOpcao" id="hidOpcao" value="true">
<input type="hidden" name="opaid" id="opaid" value="">
<input type="hidden" name="opaidA" id="opaidA" value="">
<input type="hidden" name="opaordem" id="opaordem" value="">

<input type="hidden" name="tiaid" id="tiaid" value="<?php echo $tiaid; ?>">
<input type="hidden" name="atrsqlopcoes" id="atrsqlopcoes" value="<?php echo $atrsqlopcoes; ?>">

</form>
<script type="text/javascript">
var	msg = "";
VerificaAtributo( $('tiaid').value );

/*
* Lista os dados da tabela de opcoes do atributo
*/
function listaArrayOpcoes(){

	var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/verDetalheAtributo&acao=A', {
			        method:     'post',
			        parameters: '&listaOpcoesAtributo=true&atrid=' + $('atrid').value,
			        onComplete: function (res){	
						$('tabela').innerHTML = res.responseText;							
			        }
			  });
}

function VerificaAtributo(valor){

	if(valor != ""){
		var req = new Ajax.Request('seguranca.php?modulo=principal/formulario/cadAtributo&acao=A',{
						method:		'post',
						parameters: '&VerificaAtributoAjax=true&valor=' + valor,
						asynchronous: false,
						onComplete: function(res){
							
							if(res.responseText == "false"){
								$('mostraOpcoes').style.display = 'none';
							}else{
								$('mostraOpcoes').style.display = '';
								if($('atrsqlopcoes').value == ""){
									$('mostraOpcoesSQL').style.display = 'none';
									$('mostraOpcoes').style.display = '';
									$('radDefinirOpcao').checked = true;
									listaArrayOpcoes();
								}else{
									$('mostraOpcoesSQL').style.display = '';
									$('mostraOpcoes').style.display = 'none';
									$('radDefinirSql').checked = true;
								}
							}
						}						
					});
	}else{
		$('mostraOpcoes').style.display = 'none';
	}
}


function testaSQL(){
	var sql = $('atrsqlopcoes').value.toLowerCase();

	if(sql != ""){
		strSql = new String(sql);
		str = "where";
		if(strSql.search(str) == -1){
			window.open('seguranca.php?modulo=principal/formulario/testaSQL&acao=A&sql='+sql+'&vinculo='+$('atridrelacionado').value,'page123','toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,width=600,height=480');
		}else{
			if($('atridrelacionado').value == ""){
				alert("Para execução do sql e necessário vincular um atributo!");
				$('atridrelacionado').focus();
				return false;
			}else{
				sql = sql.replace("where", " where");
				window.open('seguranca.php?modulo=principal/formulario/testaSQL&acao=A&sql='+sql+'&vinculo='+$('atridrelacionado').value,'page1234','toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,width=600,height=480');
				return true;
			}
		}
	}else{
		alert('Instrunção SQL não encontrada!');
	}
	
}


</script>
</body>