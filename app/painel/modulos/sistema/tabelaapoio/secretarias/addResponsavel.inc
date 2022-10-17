<html>
<head>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>

<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script>
	function carregaResponsaveis(secid){
		var myAjax = new Ajax.Request(
				'painel.php?modulo=sistema/tabelaapoio/secretarias/inseresec&acao=A',
				{
					method: 'post',
					parameters: "carregaResponsavel=true&secid=" + secid,
					asynchronous: false,
					onComplete: function(resp){
						window.opener.document.getElementById('td_lista_responsaveis').innerHTML =  resp.responseText;
					},
					onLoading: function(){
					}
			});
	}
</script>
<?php 

if($_POST['evento']){
	
	$_POST['respdddcelular'] = !$_POST['respdddcelular'] ? "null" : $_POST['respdddcelular'];
	$_POST['respcelular'] = !$_POST['respcelular'] ? "null" : $_POST['respcelular'];
	$_POST['respdddtelefone'] = !$_POST['respdddtelefone'] ? "null" : $_POST['respdddtelefone'];
	$_POST['resptelefone'] = !$_POST['resptelefone'] ? "null" : $_POST['resptelefone'];
	$_POST['crgid'] = !$_POST['crgid'] ? "null" : $_POST['crgid'];
	
	switch($_POST['evento']){
		
		case "S":
			$sql = "insert into 
						painel.responsavelsecretaria
				 (respnome, secid, respemail, crgid, respdddcelular, respcelular, respdddtelefone, resptelefone, respstatus)
			values 
				('{$_POST['respnome']}', '{$_POST['secid']}', '{$_POST['respemail']}' , {$_POST['crgid']}, {$_POST['respdddcelular']} , {$_POST['respcelular']} , {$_POST['respdddtelefone']} , {$_POST['resptelefone']} , 'A') RETURNING respid ";
			$respid = $db->pegaUm($sql);
			$db->commit($sql);
		break;
		
		case "A":
			$sql = "update
						painel.responsavelsecretaria 
					set
						respnome = '{$_POST['respnome']}', 
						respemail = '{$_POST['respemail']}' ,
						respdddcelular = {$_POST['respdddcelular']} , 
						respcelular = {$_POST['respcelular']} , 
						respdddtelefone = {$_POST['respdddtelefone']} , 
						resptelefone = {$_POST['resptelefone']},
						crgid = {$_POST['crgid']}
					where
						respid = {$_POST['respid']}";
			$db->executar($sql);
			$db->commit($sql);
		break;
		
	}
	
	echo "<script>carregaResponsaveis({$_POST['secid']});</script>";
	
	
}

?>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php 
monta_titulo("Responsável por Secretaria", '&nbsp');

if($_REQUEST['respid'] || $respid){
	$sql = "select * FROM painel.responsavelsecretaria where respid = ".($respid ? $respid : $_REQUEST['respid'])." and respstatus = 'A'";
	$resp = $db->pegaLinha($sql);
	extract($resp);
}

?>

<form action="" method="post" name="formulario" id="formulario">
<input type="hidden" name="evento" id="evento" value="<?php echo $respid ? "A" : "S" ?>" />
<input type="hidden" name="respid" id="respid" value="<?=$respid?>" />
<input type="hidden" name="secid" id="secid" value="<?php echo $_GET['secid'] ? $_GET['secid'] : $secid ?>" />
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">Nome:</td>
    <td><?=campo_texto('respnome','N','S','',75,250,'','','','','','id="respnome"'); ?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">E-mail:</td>
    <td><?=campo_texto('respemail','N','S','',75,250,'','','','','','id="respemail"'); ?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Cargo:</td>
    <td><?php 
   $sql = "SELECT 
				crgdsc as descricao,
				crgid as codigo
				FROM painel.cargoresponsavel 
				WHERE crgstatus = 'A'
				ORDER BY crgdsc";
	echo $db->monta_combo('crgid',$sql,'S','Selecione...','','','','','S','crgid','',$crgid);
    ?>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Telefone:</td>
    <td><?=campo_texto('respdddtelefone','N','S','',2,2,'','','','','',"onkeyup=\"this.value=mascaraglobal('##',this.value);\"');") ?> - <?=campo_texto('resptelefone','N','S','',10,8,'','','','','',"onkeyup=\"this.value=mascaraglobal('#########',this.value);\"');"); ?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Celular:</td>
    <td><?=campo_texto('respdddcelular','N','S','',2,2,'','','','','',"onkeyup=\"this.value=mascaraglobal('##',this.value);\"');"); ?> - <?=campo_texto('respcelular','N','S','',10,8,'','','','','',"onkeyup=\"this.value=mascaraglobal('#########',this.value);\"');"); ?></td>
</tr>
<tr>
	<td style="text-align:center" colspan="2" class="SubTituloDireita">
		<input type="button" name="salvar" value="Salvar" onclick="salvarResponsavel()" />
		<input type="button" name="fechar" value="Fechar" onclick="window.close()" /></td>
</tr>
</table>

</form>

<script>
	var d = document;
	
	function salvarResponsavel(){
		var erro = false;
		if(!d.getElementById('respnome').value){
			alert('Favor informar o Nome do Responsável.');
			erro = true;
			return false;
		}
		if(erro == false){
			d.getElementById('formulario').submit();
		}	
	}

</script>

</body>
</html>