<?php
/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Wesley
   Programador: Marcelo Santos
   Módulo:cadastroacoes.inc
   Finalidade: permitir o cadastro dos índices do IPCA
*/  

// Inserir IPCA
if ($_POST){
	if(empty($_REQUEST['ipcid'])) {
		//Verifica se exite o ano cadastrado
		$sql = "SELECT count(*)
				FROM painel.ipca ip
				WHERE ip.ipcano = '$_REQUEST[txano]'";
		$valor = $db->pegaum($sql);
	
		if ($valor>0){
			echo "<script>alert('O ano informado já existe!');
						  location.href='painel.php?modulo=sistema/tabelaapoio/indices/ipca/cadastroipca&acao=A';
			  	  </script>";
		}
		else {
		// insere na taleba
		$indice = str_replace(',','.',$_REQUEST['txindice']);
		$sql = "INSERT INTO painel.ipca
				(ipcano,ipcindice)
				values
				('$_REQUEST[txano]',$indice)";
		$db->executar($sql);
		$db->commit();
	
		echo "<script>alert('Índice inserido com sucesso!');
					  location.href='painel.php?modulo=sistema/tabelaapoio/indices/ipca/cadastroipca&acao=A';
			  </script>";
		}
	}
	else {
	//Verifica se exite o ano cadastrado
		$sql = "SELECT count(*)
				FROM painel.ipca ip
				WHERE ip.ipcano = '$_REQUEST[txano]' AND ip.ipcid <> $_REQUEST[ipcid]";
		$valor = $db->pegaum($sql);
	
		if ($valor>0){
			echo "<script>alert('O ano informado já existe!');
						  location.href='painel.php?modulo=sistema/tabelaapoio/indices/ipca/cadastroipca&acao=A';
			  	  </script>";
		}
		else {
		// insere na taleba
		$indice = str_replace(',','.',$_REQUEST['txindice']);
		$sql = "UPDATE painel.ipca SET
				ipcano = '$_REQUEST[txano]',
				ipcindice = $indice 
				WHERE
				ipcid = $_REQUEST[ipcid]";
		$db->executar($sql);
		$db->commit();
	
		echo "<script>alert('Índice alterado com sucesso!');
					  location.href='painel.php?modulo=sistema/tabelaapoio/indices/ipca/cadastroipca&acao=A';
			  </script>";
		}
	}
}

//Excluir índice IPCA
if ( ($_REQUEST['evento']=='E') && !empty($_REQUEST['ipcid'])){
	$sql = "UPDATE painel.ipca set ipcstatus='I' WHERE ipcid = $_REQUEST[ipcid]";
	$db->executar($sql);
	$db->commit();

	echo "<script>alert('Índice excluído com sucesso!');
				  location.href='painel.php?modulo=sistema/tabelaapoio/indices/ipca/cadastroipca&acao=A';
		  </script>";
	
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br>";

monta_titulo( "Índices IPCA", '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="">
<input type="hidden" name="ipcid">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita" width="10%">Ano:</td>
    <td><? 
    	echo campo_texto('txano','N','S','',10,4,'',''); 
    	?>
    	<img src="../imagens/obrig.gif" border="0">
    </td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Índice:</td>
    <td><? 
    	echo campo_texto('txindice','N','S','',10,8,'#,######','');
    	?>
    	<img src="../imagens/obrig.gif" border="0">
    </td>
</tr>
<tr bgcolor="#cccccc">
    <td></td>
	<td>
		<input type="button" class="botao" name="bta" value="Salvar" onclick="submeter();">
</tr>
</table>
</form>
<?
	$sql = "SELECT '<center>
						<a style=\"cursor:pointer;\" onclick=\"alteraracao(\''||ipc.ipcid||'\',\''||ipc.ipcano||'\',\''||ipc.ipcindice||'\');\">
							<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
						</a>
						<a  style=\"cursor:pointer;\" onclick=\"removeracao(\''||ipc.ipcid||'\');\">
							<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
						</a>
					</center>' as acao, 
				'<center>'||ipc.ipcano||'</center>', 
				to_char(ipc.ipcindice,'9D999999') 
				FROM painel.ipca ipc 
				where ipc.ipcstatus = 'A'
				ORDER BY ipc.ipcano";
	$cabecalho = array("Ações","Ano","Índice");
	$db->monta_lista($sql,$cabecalho,100,5,'N','Center',$par2);
	
?>
<script type="text/javascript">
function submeter(){
	var ano = "document.formulario.txano.value";
	if (document.formulario.txano.value == '' || document.formulario.txindice.value == ''){
		alert("Todos os campos são de preenchimento obrigatório!");
		return false;
	}
	else {
		if((document.formulario.txano.value < 1970) || (document.formulario.txano.value > 3000)){
			alert("Ano inválido!");
			return false;
		}
		else {
			valida = 0;
		}
		indice = document.formulario.txindice.value.indexOf(",");
		if(indice == 1) 
			valida = 0
		else {
			alert("O índice está informado errado!");
			return false;
		}
		
		if(valida==0)	
			document.formulario.submit();
		else {
			alert("Os dados informados possuem erros!");
			return false;
		}
	}				
}

function removeracao(ipcid){
	var conf = confirm("Você realmente deseja excluir este Índice?");	
	if(conf) {
		location.href="painel.php?modulo=sistema/tabelaapoio/indices/ipca/cadastroipca&acao=A&evento=E&ipcid="+ipcid;	
	}
}
function alteraracao(ipcid,ipcano,ipcindice){
	document.formulario.ipcid.value = ipcid;
	document.formulario.txano.value = ipcano;
	document.formulario.txindice.value = ipcindice.replace(".",",");
	document.formulario.txano.focus();
}


</script>