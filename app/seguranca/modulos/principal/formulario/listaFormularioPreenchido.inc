<?php
global $db;

$mnuid =0;

if($_SESSION['forid']){
	$forid   = $_SESSION['forid'];
	$fornome = $_SESSION['fornome'];
}else{
	//$forid   = 0;
	//$fornome = "";
	unset($_SESSION['forid']);
	unset($_SESSION['fornome']);
}

if($_REQUEST['montaComboTela']){
	header('content-type: text/html; charset=ISO-8859-1');
	montaComboTelaAjax($_REQUEST['sisid']);
	exit;
}

if($_REQUEST['carregaFormularioPreenchido']){
	header('content-type: text/html; charset=ISO-8859-1');
	carregaFormularioPreenchidoAjax($_REQUEST['forid'], $_REQUEST['sisid'], $_REQUEST['mnuid']);
	exit;
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Pesquisar Formulários Preenchidos', '' );
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
		<td class="SubTituloDireita">Sistema:</td>
		<td><?
			$sisid = $_REQUEST['sisid'];
		  	
		    //Caso não seja passado o sisid atribui-se valor zero para não dar erro nas consultas abaixo
			if($sisid == ''){
				$sisid =0;
			}
			$sql = "SELECT distinct sisid as codigo, sisdsc as descricao FROM seguranca.sistema order by sisdsc";
			//Monta a combo			  
			$db->monta_combo("sisid",$sql, 'S','-- Selecione o Sistema --','montaComboTela', '', '','','N','sisid');
	
	  		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tela:</td>
		<td><div id="comboTela"><? montaComboTelaAjax(''); ?></div> </td>
	</tr>
	<tr>
		<th colspan="2"><input type="button" value="Pesquisar" name="btnPesquisar" onclick="carregaFormularioPreenchido();"></th>
	</tr>
</table>
<div id="listaFormulario"></div>
</form>
<script type="text/javascript">
carregaFormularioPreenchido();
function montaComboTela(codigo){
	//$('loader-container').show();

	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/listaFormularioPreenchido&acao=A', {
				        method:     'post',
				        parameters: '&montaComboTela=true&sisid='+codigo,
				        onComplete: function (res){	
							$('comboTela').innerHTML = res.responseText; 
							//$('loader-container').hide(); 
				        }
				  });
}

function carregaFormularioPreenchido(){
	$('loader-container').show();
	var myAjax = new Ajax.Request('seguranca.php?modulo=principal/formulario/listaFormularioPreenchido&acao=A', {
				        method:     'post',
				        parameters: '&carregaFormularioPreenchido=true&forid='+$('forid').value+
				        											'&sisid='+$('sisid').value+
				        											'&mnuid='+$('mnuid').value,
				        onComplete: function (res){	

							$('listaFormulario').innerHTML = res.responseText; 
							$('loader-container').hide(); 
				        }
				  });
}

</script>
</body>