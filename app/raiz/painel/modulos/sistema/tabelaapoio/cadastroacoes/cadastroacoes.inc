<?php
/*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Wesley
   Programador: Marcelo Santos
   M�dulo:cadastroacoes.inc
   Finalidade: permitir o cadastro de a��es para vicular as secretarias
*/  

// Exclui a��o
if ( ($_REQUEST['evento']=='E') && !empty($_REQUEST['acaid'])){
	
	//Verifica a exist�ncia de vinculos (Tabelas acaosecretaria, indicador)
	$sql = "SELECT count(*)
			FROM painel.acaosecretaria acaosec
			WHERE acaosec.acaid = $_REQUEST[acaid]";
	$valor = $db->pegaum($sql);
	
	$sql = "SELECT count(*)
			FROM painel.indicador ind
			WHERE ind.acaid = $_REQUEST[acaid]";
	$valor2 = $db->pegaum($sql);

	if ($valor>0 || $valor2>0){
		echo "<script>alert('A A��o n�o pode ser exclu�da, pois est� vinculada a uma secretaria e/ou indicador!');
					  location.href='painel.php?modulo=sistema/tabelaapoio/cadastroacoes/cadastroacoes&acao=A';
		  	  </script>";
	}
	else {
	// Exclui a��o
	$sql = "UPDATE painel.acao set acastatus='I' WHERE acaid = $_REQUEST[acaid]";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>alert('A��o exclu�da com sucesso!');
				  location.href='painel.php?modulo=sistema/tabelaapoio/cadastroacoes/cadastroacoes&acao=A';
		  </script>";
	}
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br>";

monta_titulo( "Cadastro de A��es", "");
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">A��o:</td>
    <td><? 
    	echo campo_texto('sbacao','N','S','',100,255,'',''); 
//    	echo "<input type='text' name='sbatitulo'>";
    	?>
    </td>
</tr>
<tr bgcolor="#cccccc">
	      <td></td>
	  	  <td>
	  	  <input type="button" class="botao" name="bta" value="Pesquisar" onclick="submeter();">
	  	  <input type="button" class="botao" name="bt" value="Nova A��o" onclick="location.href='?modulo=sistema/tabelaapoio/cadastroacoes/insereacao&acao=A';">
	      </tr>
</table>
</form>
<?
//if($_POST){ 
 if(!empty($_REQUEST['sbacao'])){
	$where = " WHERE ac.acastatus = 'A' and upper(ac.acadsc) like upper('%$_REQUEST[sbacao]%') ";
 }	
 else {
	$where = "where ac.acastatus = 'A'";
 }
	$sql = "SELECT '<center>
						<a style=\"cursor:pointer;\" onclick=\"alteraracao(\''||ac.acaid||'\');\">
							<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
						</a>
						<a  style=\"cursor:pointer;\" onclick=\"removeracao(\''||ac.acaid||'\');\">
							<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
						</a>
					</center>' as acao, 
				ac.acadsc
				FROM painel.acao ac 
				$where
				ORDER BY ac.acadsc";
	$cabecalho = array("","T�tulo");
	$db->monta_lista($sql,$cabecalho,100,5,'N','95%',$par2);
//}
?>
<script type="text/javascript">
function submeter(){
	document.formulario.submit();	
}

function removeracao(acaid){
	var conf = confirm("Voc� realmente deseja excluir esta A��o?");	
	if(conf) {
		location.href="painel.php?modulo=sistema/tabelaapoio/cadastroacoes/cadastroacoes&acao=A&evento=E&acaid="+acaid;	
	}
}
function alteraracao(sbaid){
	var janela = location.href='?modulo=sistema/tabelaapoio/cadastroacoes/insereacao&acao=A&evento=G&acaid='+sbaid;
	janela.focus();
}


</script>