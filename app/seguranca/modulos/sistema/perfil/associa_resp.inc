<?
 /*
   Sistema Simec
   Setor respons?vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   M?dulo:associa_perfil.inc
   Finalidade: permitir a associa??o de um perfil as opera??es de menu
   */

$modulo=$_REQUEST['modulo'] ;//

if ($_REQUEST['act']=='cadastra')
{
  // cadastrar o respons?vel
 
 // se a associa??o ? de programa P, a??o A, sub-a??o S, projeto-especial E
 // verificar se ? tipo acumulador

  $sql = "select pflsncumulativo from perfil where pflcod=".$_REQUEST['pflcod'];

  $saida = $db->recuperar($sql,$res);
  if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}

  if ($pflsncumulativo == 't')
  {
        // verifica se o usucpf j? existe na mesma condi??o
        if ($_REQUEST['tipo']=='P') // programa
        {
        $sql = "select usucpf from monitora.usuarioresponsabilidade where rpustatus='A' and prgid = ".$_SESSION['ident']." and pflcod = ".$_REQUEST['pflcod']." and usucpf = '".$_REQUEST['usucpf']."'";
      }
        if ($_REQUEST['tipo']=='A') // a??o
        {
        $sql = "select usucpf from monitora.usuarioresponsabilidade where rpustatus='A' and acaid = ".$_SESSION['ident']." and pflcod = ".$_REQUEST['pflcod']." and usucpf = '".$_REQUEST['usucpf']."'";
      }
        if ($_REQUEST['tipo']=='E') // projeto especial
        {
        $sql = "select usucpf from monitora.usuarioresponsabilidade where rpustatus='A' and pjeid = ".$_SESSION['ident']." and pflcod = ".$_REQUEST['pflcod']." and usucpf = '".$_REQUEST['usucpf']."'";
      }
        if ($_REQUEST['tipo']=='S') // sub acao
        {
        $sql = "select usucpf from monitora.usuarioresponsabilidade where rpustatus='A' and saoid = ".$_SESSION['ident']." and pflcod = ".$_REQUEST['pflcod']." and usucpf = '".$_REQUEST['usucpf']."'";
      }

       $res = $db->recuperar($sql);
       if(! is_array($res))
       {
             // ainda n?o cadastrou
        if ($_REQUEST['tipo']=='P') // programa
        {
            $sql = "insert into  usuarioresponsabilidade (prgid, pflcod,usucpf,rpustatus,rpudata_inc) values (".$_SESSION['ident'].",".$_REQUEST['pflcod'].", '".$_REQUEST['usucpf']."','A','".date('d/m/Y')."')";
      }
        if ($_REQUEST['tipo']=='A') // a??o
        {
            $sql = "insert into  usuarioresponsabilidade (acaid, pflcod,usucpf,rpustatus,rpudata_inc) values (".$_SESSION['ident'].",".$_REQUEST['pflcod'].", '".$_REQUEST['usucpf']."','A','".date('d/m/Y')."')";
      }
        if ($_REQUEST['tipo']=='E') // projeto especial
        {
            $sql = "insert into  usuarioresponsabilidade (pjeid, pflcod,usucpf,rpustatus,rpudata_inc) values (".$_SESSION['ident'].",".$_REQUEST['pflcod'].", '".$_REQUEST['usucpf']."','A','".date('d/m/Y')."')";
      }
        if ($_REQUEST['tipo']=='S') // sub acao
        {
            $sql = "insert into  usuarioresponsabilidade (saoid, pflcod,usucpf,rpustatus,rpudata_inc) values (".$_SESSION['ident'].",".$_REQUEST['pflcod'].", '".$_REQUEST['usucpf']."','A','".date('d/m/Y')."')";
      }

       $res = $db->executar($sql);
       $db->commit();
       //$db->sucesso($modulo);
       $db->close();
       ?>
       <script>
           window.close();
           opener.location.reload();
       </script>
       <?
       exit();
       }
  } else
  {
   // n?o ? cumulativo
   // verifica se o usucpf j? existe na mesma condi??o
        if ($_REQUEST['tipo']=='P') // programa
        {
              $sql = "select usucpf from monitora.usuarioresponsabilidade where rpustatus='A' and prgid = ".$_SESSION['ident']." and pflcod = ".$_REQUEST['pflcod'];
      }
        if ($_REQUEST['tipo']=='A') // a??o
        {
              $sql = "select usucpf from monitora.usuarioresponsabilidade where rpustatus='A' and acaid = ".$_SESSION['ident']." and pflcod = ".$_REQUEST['pflcod'];

      }
        if ($_REQUEST['tipo']=='E') // projeto especial
        {
              $sql = "select usucpf from monitora.usuarioresponsabilidade where rpustatus='A' and pjeid = ".$_SESSION['ident']." and pflcod = ".$_REQUEST['pflcod'];
      }
        if ($_REQUEST['tipo']=='S') // sub acao
        {
              $sql = "select usucpf from monitora.usuarioresponsabilidade where rpustatus='A' and saoid = ".$_SESSION['ident']." and pflcod = ".$_REQUEST['pflcod'];
      }

       $res = $db->recuperar($sql);
       if(is_array($res))
       {// j? cadastrou antes
           if ($_REQUEST['tipo']=='P') // programa
        {
           // $sql = "update usuarioresponsabilidade set rpustatus = 'I' where prgid= ".$_SESSION['ident']." and pflcod=".$_REQUEST['pflcod'];
        $sql = "delete from monitora.usuarioresponsabilidade where prgid= ".$_SESSION['ident']." and pflcod=".$_REQUEST['pflcod'];
      }
        if ($_REQUEST['tipo']=='A') // a??o
        {
            //$sql = "update usuarioresponsabilidade set rpustatus = 'I' where acaid= ".$_SESSION['ident']." and pflcod=".$_REQUEST['pflcod'];
        $sql = "delete from monitora.usuarioresponsabilidade where acaid= ".$_SESSION['ident']." and pflcod=".$_REQUEST['pflcod'];
      }
        if ($_REQUEST['tipo']=='E') // projeto especial
        {
          //  $sql = "update usuarioresponsabilidade set rpustatus = 'I' where pjeid= ".$_SESSION['ident']." and pflcod=".$_REQUEST['pflcod'];
        $sql = "delete from monitora.usuarioresponsabilidade  where pjeid= ".$_SESSION['ident']." and pflcod=".$_REQUEST['pflcod'];
      }
          $res = $db->executar($sql);
    }
      if ($_REQUEST['tipo']=='P') // programa
        {
           $sql = "insert into  usuarioresponsabilidade (prgid, pflcod,usucpf,rpustatus,rpudata_inc) values (".$_SESSION['ident'].",".$_REQUEST['pflcod'].", '".$_REQUEST['usucpf']."','A','".date('d/m/Y')."')";
      }
        if ($_REQUEST['tipo']=='A') // a??o
        {
           $sql = "insert into  usuarioresponsabilidade (acaid, pflcod,usucpf,rpustatus,rpudata_inc) values (".$_SESSION['ident'].",".$_REQUEST['pflcod'].", '".$_REQUEST['usucpf']."','A','".date('d/m/Y')."')";
           print $sql;      }
        if ($_REQUEST['tipo']=='E') // projeto especial
        {
           $sql = "insert into  usuarioresponsabilidade (pjeid, pflcod,usucpf,rpustatus,rpudata_inc) values (".$_SESSION['ident'].",".$_REQUEST['pflcod'].", '".$_REQUEST['usucpf']."','A','".date('d/m/Y')."')";
      }
        if ($_REQUEST['tipo']=='S') // sub acao
        {
           $sql = "insert into  usuarioresponsabilidade (saoid, pflcod,usucpf,rpustatus,rpudata_inc) values (".$_SESSION['ident'].",".$_REQUEST['pflcod'].", '".$_REQUEST['usucpf']."','A','".date('d/m/Y')."')";
      }
        $res = $db->executar($sql);
        $db->commit();
       //$db->sucesso($modulo);
        $db->close();
       ?>

       <script>
           window.close();
           opener.location.reload();
       </script>
       <?
      
       exit();
       
  }
  $sql = "select usucpf from monitora.usuarioresponsabilidade where rpustatus = 'A' and prgcod ='".
  $_SESSION['prgcod']."' and pflcod = ".$_REQUEST['pflcod'];
}

if ($_REQUEST['pflcod']) 
{
    $sql= "select pflcod, pfldsc from seguranca.perfil where pflcod=".$_REQUEST['pflcod'].' and sisid='.$_SESSION['sisid'];
      $saida = $db->recuperar($sql,$res);
      if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}
}
$sql= "select pfldsc from seguranca.perfil where pflcod = ".$_REQUEST['cod'].' and sisid='.$_SESSION['sisid'];
$RS2 = $db->record_set($sql);
$nlinhas2 = $db->conta_linhas($RS2);
if ($nlinhas2 >=0)
{
    $res =  $db->carrega_registro($RS2,0);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
}


?>
<title>Listagem de Respons?veis</title>
<div align="center">
<table  width='100%' border="0" cellspacing="0" cellpadding="3" align="center">
  <tr bgcolor="#f0f0f0">
    <td width="100%" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#D4D0C8', gradientType='1')"><label class="TituloTela">Listar Respons?veis <?=$pfldsc?></label></td>
  </tr>
</table>
<form method="POST" name="formulario">
<input type=hidden name="act" value=0>
<input type=hidden name="pflcod" value=<?=$_REQUEST['cod']?>>
<input type=hidden name="tipo" value=<?=$_REQUEST['ogm']?>>
<div align="center">
  <center>
  <table border="0" width="85%">
    <tr>
      <td width="100%">Por interm?dio desta tela voc? poder? INCLUIR ou SUBSTITUIR respons?veis. O sistema far? isto automaticamente. <br>Quando a tarefa n?o for cumulativa (ou seja, apenas um por opera??o - ex. Gerente de Programa), o sistema ir? substituir o atual, caso contrario, ir? acrescentar.<br>Ao retornar para a tela principal, caso a nova realidade n?o esteja vis?vel, pressione a tecla F5.</td>
    </tr>
  </table>
  </center>
</div>

<table width='95%' align="center">
<tr>
<td>
<?
	  $sql = "select u.usucpf as CODIGO,u.usunome ||' (' || o.orgdsc ||')' as DESCRICAO from usuario u , orgao o , perfilusuario pu where u.orgcod = o.orgcod and pu.usucpf = u.usucpf and pu.pflcod = ".$_REQUEST['cod']. " order by u.usunome ";


	  $db->monta_combo("usucpf",$sql,'S',"Selecione o Respons?vel",'','');
?>
</td>
<tr><td><input type='button' value='Cadastrar' onclick="validar_cadastro()">
<input type='button' value='Fechar' onclick="fechar_janela()">
</td></tr>
</table>
</form>
</div>
<script language="JavaScript">
  function validar_cadastro() {
    document.formulario.act.value = 'cadastra';
	document.formulario.submit();
  }
  function submete_perfil(cod){
    if (document.formulario.pflcod.value != '')
    {
        document.formulario.submit();
    }
  }
  function fechar_janela()
  {
    window.close();
  }
</script>
