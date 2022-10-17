<?

 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Marcelo Freire
   Programador: Marcelo Freire
   Módulo:testa_muda_sistema.php
   Finalidade: permitir a mudança de sistema sem ter que logar de novo
 */

session_start(); 
if (!$_SESSION['usucpf']) {
	header("Location: ../login.php");
	exit();
}
 include "config.inc";
 include APPRAIZ."includes/classes_simec.inc";
 //$db = new cls_banco();
include APPRAIZ."includes/funcoes.inc"; 
  $db = new cls_banco();
 //Variavel global com Ano de Referência
 $exercicio = getdate();
 $_SESSION['exercicio_atual']=$exercicio['year'];;
 
$_REQUEST['CPF_PESSOA'] = formatar_cpf($_SESSION['usucpf']);
if ($_SESSION['usucpf_simu'])
{
	$_REQUEST['CPF_PESSOA'] = formatar_cpf($_SESSION['usucpf_simu']);
    $sql = "select ususenha from seguranca.usuario where usucpf='".$_SESSION['usucpf_simu']."' ";
}
else $sql = "select ususenha from seguranca.usuario where usucpf='".$_SESSION['usucpf']."' ";
    $_POST['SENHA']=md5_decrypt_senha($db->pegaUm($sql),'');
    //$_POST['SENHA'] = $_SESSION['senha'];
$_REQUEST['codigo'] = $_REQUEST['id'];


 include APPRAIZ."includes/autenticar.inc";

?>