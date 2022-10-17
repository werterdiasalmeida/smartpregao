<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:altsenha.inc
   Finalidade: permitir a alteração de senha
   */

$modulo=$_REQUEST['modulo'] ;//


if ($_REQUEST['act'] == 'alterar') {
   // checagem de consistencias
   $sql = "select ususenha from seguranca.usuario where usucpf='".$_SESSION['usucpf']."'";

   $senha_banco = $db->recuperar($sql);
   $senha = md5_decrypt_senha($senha_banco['ususenha'],'');
	if ($senha <> $_REQUEST['senhaatual']) {
	   // A senha atual não confere
	   ?>
	      <script>
	         alert ('A senha atual não confere.');
	         history.back();
	      </script>
	   <?
	     exit();
	   }   

}
if (($_REQUEST['act']=='alterar') and (! is_array($msgerro)))
{
   // fazer alteração da senha.

   $sql = "update usuario set usuchaveativacao = 't', ususenha='".md5_encrypt_senha($_REQUEST['novasenha'],'').
   "' where usucpf='".$_SESSION['usucpf']."'";
    $saida = $db->executar($sql);
   $db -> commit();     
   $db->sucesso($modulo);

}
include APPRAIZ."includes/cabecalho.inc";
print '<br>';
$titulo_modulo='Alteração de senha';
monta_titulo($titulo_modulo,'');
?>
<title>Alteração de senha de Usuário</title> 
<div align="center">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
    <center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="SubTituloDireita" >Senha atual:</td>
        <td >
	<input type='password' name='senhaatual' size='20'><?=obrigatorio();?></td>
      </tr>
      <tr >	
        <td align='right' class="SubTituloDireita" >Nova senha:</td>
        <td >
	<input type='password' name='novasenha' size='20'><?=obrigatorio();?></td></tr>
      <tr>	
        <td align='right' class="SubTituloDireita">Confirma nova senha:</td>
        <td>
	<input type='password' name='confsenha' size='20'><?=obrigatorio();?></td>
       </tr>
     <tr bgcolor="#C0C0C0">       
       <td></td><td><input type="button" class="botao" name="btalterar" value="Atualizar" onclick="validar_cadastro()"></td>
    </tr>
 </table>
 </center>
 </div>
 <br><br>
  </form>
<script>
	function validar_cadastro() {
        e =document.formulario.novasenha.value;
		if (!validaBranco(document.formulario.senhaatual, 'Senha atual')) return;	
		if (!validaBranco(document.formulario.novasenha, 'Nova senha')) return;
        if ( e.length > 12 ) {alert ('A senha deve ter um máximo de 12 caracteres'); return;}
        if ( e.length < 4 ) {alert ('A senha deve ter um mínimo de 4 caracteres'); return;}
		if (!validaBranco(document.formulario.confsenha, 'Confirma nova senha')) return;
		if (document.formulario.novasenha.value != document.formulario.confsenha.value )
	    	{
	       		alert('A nova senha não confere com a segunda digitação.');
	       		return;
	    	}
	    	document.formulario.act.value = 'alterar';
		document.formulario.submit();
	}
</script>
