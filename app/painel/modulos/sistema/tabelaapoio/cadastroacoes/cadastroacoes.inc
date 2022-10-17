<?php
/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Wesley
   Programador: Marcelo Santos
   Módulo:cadastroacoes.inc
   Finalidade: permitir o cadastro de ações para vicular as secretarias
*/  

// Exclui ação
if ( ($_REQUEST['evento']=='E') && !empty($_REQUEST['acaid'])){
	
	//Verifica a existência de vinculos (Tabelas acaosecretaria, indicador)
	$sql = "SELECT count(*)
			FROM painel.acaosecretaria acaosec
			WHERE acaosec.acaid = $_REQUEST[acaid]";
	$valor = $db->pegaum($sql);
	
	$sql = "SELECT count(*)
			FROM painel.indicador ind
			WHERE ind.indstatus = 'A'
			AND ind.acaid = $_REQUEST[acaid]";
	$valor2 = $db->pegaum($sql);

	if ($valor>0 || $valor2>0){
		echo "<script>alert('A Ação não pode ser excluída, pois está vinculada a uma secretaria e/ou indicador!');
					  location.href='painel.php?modulo=sistema/tabelaapoio/cadastroacoes/cadastroacoes&acao=A';
		  	  </script>";
	}
	else {
	// Exclui ação
	$sql = "UPDATE painel.acao set acastatus='I' WHERE acaid = $_REQUEST[acaid]";
	$db->executar($sql);
	$db->commit();
	
	echo "<script>alert('Ação excluída com sucesso!');
				  location.href='painel.php?modulo=sistema/tabelaapoio/cadastroacoes/cadastroacoes&acao=A';
		  </script>";
	}
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br>";

monta_titulo( "Cadastro de Ações", "");
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">Ação:</td>
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
	  	  <input type="button" class="botao" name="bt" value="Nova Ação" onclick="location.href='?modulo=sistema/tabelaapoio/cadastroacoes/insereacao&acao=A';">
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
	$cabecalho = array("","Título");
	$db->monta_lista($sql,$cabecalho,100,5,'N','95%',$par2);
//}
?>
<script type="text/javascript">
function submeter(){
	document.formulario.submit();	
}

function removeracao(acaid){
	var conf = confirm("Você realmente deseja excluir esta Ação?");	
	if(conf) {
		location.href="painel.php?modulo=sistema/tabelaapoio/cadastroacoes/cadastroacoes&acao=A&evento=E&acaid="+acaid;	
	}
}
function alteraracao(sbaid){
	var janela = location.href='?modulo=sistema/tabelaapoio/cadastroacoes/insereacao&acao=A&evento=G&acaid='+sbaid;
	janela.focus();
}


</script>