<?php
/*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Wesley
   Programador: Marcelo Santos
   M�dulo:cadastrosecretarias.inc
   Finalidade: permitir o cadastro de Secretarias para vicular as a��es
*/  

// Exclui a��o
if ( ($_REQUEST['evento']=='E') && !empty($_REQUEST['secid'])){
	
	//Verifica a exist�ncia de vinculos (Tabelas acaosecretaria, indicador)
	$sql = "SELECT count(*)
			FROM painel.acaosecretaria acaosec
			WHERE acaosec.secid = $_REQUEST[secid]";
	$valor = $db->pegaum($sql);
	
	$sql = "SELECT count(*)
			FROM painel.indicador ind
			WHERE ind.secid = $_REQUEST[secid]";
	$valor2 = $db->pegaum($sql);

	if ($valor>0 || $valor2>0){
		echo "<script>alert('A Secretaria n�o pode ser exclu�da, pois est� vinculada a uma a��o e/ou indicador!');
					  location.href='painel.php?modulo=sistema/tabelaapoio/secretarias/cadastrosec&acao=A';
		  	  </script>";
	}
	else {
	// Exclui a��o
	$sql = "UPDATE painel.secretaria set secstatus='I' WHERE secid = $_REQUEST[secid]";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>alert('Secretaria exclu�da com sucesso!');
				  location.href='painel.php?modulo=sistema/tabelaapoio/secretarias/cadastrosec&acao=A';
		  </script>";
	}
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br>";

monta_titulo( "Cadastro de Secretarias", "");
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">Secretaria:</td>
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
	  	  <input type="button" class="botao" name="bt" value="Nova Secretaria" onclick="location.href='?modulo=sistema/tabelaapoio/secretarias/inseresec&acao=A';">
	      </tr>
</table>
</form>
<?
//if($_POST){ 
 if(!empty($_REQUEST['sbacao'])){
	$where = " WHERE sec.secstatus = 'A' and upper(sec.secdsc) like upper('%$_REQUEST[sbacao]%') ";
 }	
 else {
	$where = "where sec.secstatus = 'A'";
 }
	$sql = "SELECT '<center>
						<a style=\"cursor:pointer;\" onclick=\"alteraracao(\''||sec.secid||'\');\">
							<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
						</a>
						<a  style=\"cursor:pointer;\" onclick=\"removeracao(\''||sec.secid||'\');\">
							<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
						</a>
					</center>' as acao, 
				sec.secdsc
				FROM painel.secretaria sec 
				$where
				ORDER BY sec.secdsc";
	$cabecalho = array("","T�tulo");
	$db->monta_lista($sql,$cabecalho,100,5,'N','95%',$par2);
//}
?>
<script type="text/javascript">
function submeter(){
	document.formulario.submit();	
}

function removeracao(secid){
	var conf = confirm("Voc� realmente deseja excluir esta Secretaria?");	
	if(conf) {
		location.href="painel.php?modulo=sistema/tabelaapoio/secretarias/cadastrosec&acao=A&evento=E&secid="+secid;	
	}
}
function alteraracao(secid){
	var janela = location.href='?modulo=sistema/tabelaapoio/secretarias/inseresec&acao=A&evento=G&secid='+secid;
	janela.focus();
}


</script>