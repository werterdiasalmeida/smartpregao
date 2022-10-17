<?

 /*
   Sistema Sistema Simec
   Setor responsável: SPO/MEC
   Desenvolvedor: Desenvolvedores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo: recuperar_senha.php
   Finalidade: Permitir recuperar a senha
   */
   	  include "includes/classes_simec.inc";
      include "includes/funcoes.inc";
include "includes/erros.inc";
  if ($_REQUEST['CPF_PESSOA']) {
	  $db = new cls_banco();
 $sql = "select usucpf from usuario where usucpf = '".corrige_cpf($_REQUEST['CPF_PESSOA'])."'";
 $usu = $db->recuperar($sql);
	if (! is_array($usu)) {
	   // não existe cpf idêntico, logo, não pode ser válido
	   ?>
	      <script>
	         alert ('O CPF: <?=$_REQUEST['usucpf']?> não se encontra cadastrado no sistema.\n Por favor, volte à tela principal e faça sua solicitação de cadastro.');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
	   else
	   {
	  
     $sql = "select usudataultacesso, ususenha, usunome, ususexo, usuemail from usuario where usucpf='".corrige_cpf($_REQUEST['CPF_PESSOA'])."'";
     $senha_banco = $db->recuperar($sql);
     $dta =formata_data($senha_banco['usudataultacesso']);
     if (! $senha_banco['usudataultacesso']) $dta = date('d/m/Y');
     $dtb = date('d/m/Y');
     if (checa_datas($dtb,$dta,7776000))
     {
	    //$senha = md5_decrypt($senha_banco['ususenha'],'');
	    $senha = senha();
	    $_SESSION['usucpf'] = corrige_cpf($_REQUEST['CPF_PESSOA']);
        $_SESSION['usucpforigem'] = corrige_cpf($_REQUEST['CPF_PESSOA']);
	    $sql = "update usuario set usuchaveativacao ='f',ususenha='".md5_encrypt($senha,'')."' where usucpf='".corrige_cpf($_REQUEST['CPF_PESSOA'])."'";
         $saida = $db->executar($sql);
         $db -> commit();
	    $assunto = 'Recuperação de senha';
	    $sexo = 'Prezado Sr.  ';
		if ($sexo == 'F') $sexo = 'Prezada Sra. ';
        $mensagem = $sexo. strtoupper($senha_banco['usunome']).'<br>'.'Sua nova senha = '.$senha.'<br>'.'Ao se conectar, altere esta senha para a sua senha preferida.';
        $paraonde = 'gilberto.cerqueira@mec.gov.br';
        email(strtoupper($senha_banco['usunome']), $senha_banco['usuemail'], $assunto, $mensagem);
        ?>
        <script>
           alert ('O Sistema enviou uma mensagem com uma nova senha \n para o seu e-mail cadastrado. ');
           location.href="login.php";
         </script>
        <?
        exit();
     }
     else
     {
        // está a mais de 90 dias sem acesso
        ?> <script>
            alert ('O seu acesso está bloqueado \n (mais de 90 dias sem acesso). \n Entre em contato com a administração.');
           location.href="login.php";
            </script>
        <?
        exit();

     }
     }
  }
?>
<html>
<head>
<title>Recuperar a senha</title>
<script language="JavaScript" src="includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="includes/Estilo.css">
</head>
<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">
<? include "cabecalho.php";?>
<br>
<?
$titulo_modulo='SIMEC - Recuperação de Senha';
$subtitulo_modulo='Digite seu CPF e pressione o bot&atilde;o: "Lembrar Senha". 
		<br> O Sistema enviará um e-mail para você contendo uma nova senha de acesso.';
monta_titulo($titulo_modulo,$subtitulo_modulo);
?>
<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
<form method="post" name="formulario">
<tr><td align="center" colspan="2"><? if ( $_SESSION['MSG_AVISO'] <> '' ) { ?>
	<div align=center><font color=red><? print $_SESSION['MSG_AVISO'];
	$_SESSION['MSG_AVISO'] = '';
	?></font></div>
<? } ?>
<? if ( is_array($erro) ) { ?>
<P><p align=center><font color=red><strong><?=$erro[0]?>!!</strong></font></p><p>
<? } ?></td></tr>
<tr>
        <td align=right class="subtitulodireita"><p>CPF:</td>
        <td><input type="text" name="CPF_PESSOA" value="" size="16" onkeypress="return entra0(event);" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);"  class="normal" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></td>
</tr>
<tr bgcolor="#C0C0C0">
        <td></td>
   <td><input type="button" name="Autenticar" value="Lembrar Senha" onclick="validar_login()" >&nbsp;&nbsp;&nbsp;<input type="Button" value="Voltar" onclick="history.back();"></td>
</tr>
</form>
</table>
<? include "rodape.php";?>
<script>
function validar_login() {
        e = document.formulario.CPF_PESSOA;
        if (document.formulario.CPF_PESSOA.value == '') alert('O CPF precisa ser preenchido corretamente.');
        else {
                 if(DvCpfOk(e)) document.formulario.submit();
			 else alert('O CPF precisa ser preenchido corretamente.');


        }
}


function entra0()
{
	if(window.event.keyCode == 13) return document.formulario.SENHA.focus();
}

function entra()
{
	if(top.window.event.keyCode == 13) {document.formulario.Autenticar.focus()};
}

</script></body></html>
