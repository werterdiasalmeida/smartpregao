<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:associa_perfil.inc
   Finalidade: permitir a associação de um perfil as operações de menu
   */
   

$modulo=$_REQUEST['modulo'] ;//

if ($_REQUEST['act']=='cadastra' and $_POST['pflcod']<>'') {
  $sql = 'select * from menu order by mnucod';
  $RS = $db->record_set($sql);
  $nlinhas = $db->conta_linhas($RS);
  if ($nlinhas >= 0)
   {
     for ($i=0; $i<=$nlinhas;$i++)
     {
       $res = $db->carrega_registro($RS,$i);
       // a linha abaixo transforma em variáveis todos os campos do array
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       if ($_POST[$mnucod]<>'')
       {
         // então marcou a opção
         // procurar se já foi marcada antes. se não foi, então inserir.
           $sql = 'select * from perfilmenu where mnucod='.$mnucod.' and pflcod='.$_POST['pflcod'];
            $RS2 = $db->record_set($sql);
            $nlinhas2 = $db->conta_linhas($RS2);
            if ($nlinhas2 == -1) {
              // incluir
                $sql = 'insert into perfilmenu (pflcod, mnucod) values ('.$_POST['pflcod'].','.$mnucod.')';
                 $saida = $db->executar($sql);
            }
           
       } else
       {
         // caso não tenha marcado, procurar se existe no banco. Se existir, então apagar
            $sql = 'select * from perfilmenu where mnucod='.$mnucod.' and pflcod='.$_POST['pflcod'];
            $RS3 = $db->record_set($sql);
            $nlinhas3 = $db->conta_linhas($RS3);
            if ($nlinhas3 == 0)
            { // excluir
                 $sql = 'delete from perfilmenu where pflcod='.$_POST['pflcod'].'  and mnucod='.$mnucod;
                 $saida = $db->executar($sql);
            }

       }
     }
     $db -> commit();
     $db->sucesso($modulo);
   }
}

if ($_REQUEST['pflcod']) {
    $sql= "select pflcod, pfldsc from perfil where pflcod=".$_REQUEST['pflcod'];
      $saida = $db->recuperar($sql,$res);
      if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}

  }

?>
<title>Listagem de Responsáveis</title>
<div align="center">
<table  width='95%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;" align='center'>
  <tr align="center">
    <td width="100%"><strong>Listagem de Responsáveis</strong></td>
  </tr>
</table>
	<?
	  $sql = "select pflcod as CODIGO,pfldsc as DESCRICAO from perfil order by pflcod ";
	  $db->monta_combo("usucpf",$sql,'S',"Selecione o Gerente",'submete_perfil','');
	?>
<form method="POST" name="formulario">
<input type=hidden name="act" value=0>

<table width='95%' align="center">
<tr>
<td> Escolha o tipo de responsabilidade</td>
</tr>
<tr>
<td>
  <input type="radio"  checked name="R1" value="V1"> Gerente<br>
  <input type="radio" name="R1" value="V2"> Gerente-Executivo<br>
  <input type="radio" name="R1" value="V3"> Gerente de Ações-Transversais<br>
  <input type="radio" name="R1" value="V4"> Coordenador de Ação<br>
  <input type="radio" name="R1" value="V5"> Responsável por sub-ação<br>
  <input type="radio" name="R1" value="V6"> Responsável por Projeto Especial<br>
  <input type="radio" name="R1" value="V7"> Digitador<br>
</td>
</tr>
<?
   if ($pflcod) {
  $indice = 0;
  $sql = 'select * from menu where mnutipo = 1 order by mnucod';
  $RS = $db->record_set($sql);
  $nlinhas = $db->conta_linhas($RS);
  if ($nlinhas >= 0)
  {
    ?>
    <tr><td><input type='button' value='Cadastrar' onclick="validar_cadastro()"></td></tr>
    <?
    for ($i=0; $i<=$nlinhas;$i++)
    {
    // monta a matriz de opções
       $res = $db->carrega_registro($RS,$i);
       // a linha abaixo transforma em variáveis todos os campos do array
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       // verifica se já foi marcado
       $sql = 'select * from perfilmenu where mnucod='.$mnucod.' and pflcod='.$_REQUEST['pflcod'];
       $RSC = $db->record_set($sql);
       $nlinhasC = $db->conta_linhas($RSC);
       if ($nlinhasC == 0) $cb = '  checked ' ; else $cb = '';
       print '<tr><td><input type="checkbox" name="'.$mnucod.'" value="'.$mnucod.'"'.$cb.' >'.$mnucod.'--'.$mnutransacao.'</td></tr>';
       $sql = 'select * from menu where mnutipo = 2 and mnucodpai = '.$mnucod.' order by mnucod';
       $RS2 = $db->record_set($sql);
       $nlinhas2 = $db->conta_linhas($RS2);
       for ($ii=0; $ii<=$nlinhas2;$ii++)
       {
          $res = $db->carrega_registro($RS2,$ii);
          if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
          // verifica se já foi marcado
          $sql = 'select * from perfilmenu where mnucod='.$mnucod.' and pflcod='.$_REQUEST['pflcod'];
          $RSC = $db->record_set($sql);
          $nlinhasC = $db->conta_linhas($RSC);
          if ($nlinhasC == 0) $cb = '  checked ' ; else $cb = '';
          print '<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox" name="'.$mnucod.'" value="'.$mnucod.'"'.$cb.' >'.$mnucod.'--'.$mnutransacao.'</td></tr>';
          $sql = 'select * from menu where mnutipo = 3 and mnucodpai = '.$mnucod.' order by mnucod';
          $RS3 = $db->record_set($sql);
          $nlinhas3 = $db->conta_linhas($RS3);
          for ($j=0; $j<=$nlinhas3;$j++)
          {
              $res = $db->carrega_registro($RS3,$j);
              if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
              // verifica se já foi marcado
              $sql = 'select * from perfilmenu where mnucod='.$mnucod.' and pflcod='.$_REQUEST['pflcod'];
              $RSC = $db->record_set($sql);
              $nlinhasC = $db->conta_linhas($RSC);
              if ($nlinhasC == 0) $cb = '  checked ' ; else $cb = '';
              print '<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox" name="'.$mnucod.'" value="'.$mnucod.'"'.$cb.' >'.$mnucod.'--'.$mnutransacao.'</td></tr>';
          }
        }
    }
  }
  else
  {
    print '<tr><td></td><td>Não há registros de Menu</td></tr>';
  }
?>
<tr><td><input type='button' value='Cadastrar' onclick="validar_cadastro()"></td></tr>
<?}?>
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

</script>
