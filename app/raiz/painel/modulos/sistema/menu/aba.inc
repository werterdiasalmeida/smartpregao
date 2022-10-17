<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:aba.inc
   Finalidade: Permitir o cadastro de abas que serão utilizadas pelos menus/módulos
   */
   //
$modulo=$_REQUEST['modulo'] ;
$act=$_REQUEST['act'];
$acao=$_REQUEST['acao'];

//inclui abas
if (($act == 'incluir' and $acao=='I') or ($act == 'alterar' and $acao=='A'))
{
	if ($acao == 'I') {
    $sql = "insert into aba (abadsc,sisid) values ('".$_POST['abadsc']."',{$_SESSION['sisid']})";
    $db->executar($sql);
	$sql = "select max(abacod) as abacodmax from aba";
	$saida = $db->recuperar($sql);
    if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;
	}
	else
	{
	$abacodmax = $_POST['abacod'];
	$sql = "update aba set abadsc='".$_POST['abadsc']."' where abacod=".$abacodmax;
    $db->executar($sql);
	}
	//inclui aba_menu
	  $sql = "select * from menu where sisid=".$_SESSION['sisid']." order by mnucod";
	  $RS = $db->record_set($sql);
	  $nlinhas = $db->conta_linhas($RS);
	  if ($nlinhas >= 0)
	   {
	     for ($i=0; $i<=$nlinhas;$i++)
	     {
	       $res = $db->carrega_registro($RS,$i);
	       // a linha abaixo transforma em variáveis todos os campos do array
	       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	       if ($_POST[$mnuid]<>'')
	       {
	         // então marcou a opção
	         // procurar se já foi marcada antes. se não foi, então inserir.
	           $sql = 'select * from aba_menu where mnuid='.$mnuid.' and abacod='.$abacodmax;
	            $RS2 = $db->record_set($sql);
	            $nlinhas2 = $db->conta_linhas($RS2);
	            if ($nlinhas2 == -1) {
	              // incluir
	                $sql = 'insert into aba_menu (abacod, mnuid) values ('.$abacodmax.','.$mnuid.')';
	                 $db->executar($sql);
	            }
	           
	       } else
       		{	
         // caso não tenha marcado, procurar se existe no banco. Se existir, então apagar
            $sql = "select * from aba_menu where mnuid=$mnuid and abacod=$abacodmax";
			$RS3 = $db->record_set($sql);
            $nlinhas3 = $db->conta_linhas($RS3);
			if ($nlinhas3 == 0)
            { // excluir
                 $sql = 'delete from aba_menu where abacod='.$_POST['abacod'].'  and mnuid='.$mnuid;
                 $saida = $db->executar($sql);
            }
			}
		   }
	}

	$db ->commit();
    $db->sucesso('sistema/menu/aba');
}

if ($act == 'excluir' and $acao=='A')
{
	$sql = "update menu set abacod=null where abacod=". $_POST['abacod'];
    $db->executar($sql);
	$sql = "delete from aba_menu where abacod=". $_POST['abacod'];
    $db->executar($sql);
	$sql = "delete from aba where abacod=". $_POST['abacod'];
    $db->executar($sql);
	$db ->commit();
    $db->sucesso('sistema/menu/aba');

}
include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?$db->cria_aba($abacod_tela,$url,'');?>
<?
if ($acao=='I' or ($acao=='A' and $_REQUEST['abacod_int']<>'')) 
{
if($_REQUEST['abacod_int']) 
{ 
    $sql= "select * from aba where abacod=".$_REQUEST['abacod_int'];
    $saida = $db->recuperar($sql,$res);
    if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}
}
?>

<?
//título da página
if ($acao=='I') $titulo_modulo = 'Incluir Aba'; else $titulo_modulo = 'Alterar Aba';
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<form name="formulario" method="post">
<input type='hidden' name="act">
<input type='hidden' name="abacod" value="<?=$abacod?>">
      <tr>
        <td align='right'  class="SubTituloDireita">Descrição:</td>
        <td><?=campo_texto('abadsc','S','S','',50,50,'','');?></td>
      </tr>
	  <tr>
        <td align='right'  class="SubTituloDireita">Módulos/Menus:</td>
        <td>
   		<table cellpadding="0" cellspacing="0" border="0" align="left">
		<?
		//$cabecalho = array('Ações','Código','Aba','Qtde. de Módulos');
		//$sql = "select case when aba_menu.mnucod isnull then '<input type=\"checkbox\" name=\"'||menu.mnucod||'\" value=\"'||menu.mnucod||'\">' else '<input type=\"checkbox\" name=\"'||menu.mnucod||'\" value=\"'||menu.mnucod||'\" checked>' end as check, menu.mnucod||' - '||menu.mnudsc from menu left join aba_menu on menu.mnucod=aba_menu.mnucod and aba_menu.abacod =".$abacod;
		if ($acao=='I') $sql = "select menu.mnuid,menu.mnucod, menu.mnudsc, menu.mnulink, menu.mnutransacao, menu.mnutipo, '' as check from menu where sisid=".$_SESSION['sisid']." order by menu.mnucod";
		else $sql = "select menu.mnuid, menu.mnucod, menu.mnudsc, menu.mnulink, menu.mnutransacao, menu.mnutipo, case when aba_menu.mnuid isnull then '' else 'checked' end as check from menu left join aba_menu on menu.mnuid=aba_menu.mnuid and aba_menu.abacod =$abacod where menu.sisid=".$_SESSION['sisid']." order by menu.mnucod";
		$RS = $db->carregar($sql);
		  $nlinhas = count($RS)-1;
		  if ($nlinhas >= 0)
		  {
		    for ($i=0; $i<=$nlinhas;$i++)
		    {	
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
				if ($mnutipo==2) $space = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'; elseif ($mnutipo==3) $space = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'; else $space='';
				if ($mnulink<>'') $block = ''; else $block = 'disabled';
				print $space.'<input type="checkbox" name="'.$mnuid.'" value="'.$mnuid.'"'.$check.' '.$block.'>'.$mnucod.' - '.$mnutransacao.'<br>';
			}
		  }
		?></tr></table>
	</td></tr>
<tr bgcolor="#CCCCCC">
   <td></td><td>
   <?if ($acao == 'A'){?>
   <input type="button" name="btalterar" value="Alterar" onclick="return altera()" class="botao">
   &nbsp;&nbsp;&nbsp;&nbsp;<input type="button" name="btexcluir" value="Excluir" onclick="return exclui()" class="botao">
   <?}else{?>
   <input type="button" name="btinserir" value="Incluir" onclick="return inclui()" class="botao">
   <?}?>
   &nbsp;&nbsp;&nbsp;&nbsp;<input type="button" name="btcancelar" value="Voltar" onclick="history.back();" class="botao"></td>
 </tr>
</form>
</table>

<script language="JavaScript">

  function inclui() {
	if (!validaBranco(document.formulario.abadsc, 'Descrição')) return;
    document.formulario.act.value = 'incluir';
	document.formulario.submit();
  }
  
  function altera() {
	if (!validaBranco(document.formulario.abadsc, 'Descrição')) return;
    document.formulario.act.value = 'alterar';
	document.formulario.submit();
  }
  
  
  function exclui() { 
    if( window.confirm( "Confirma a exclusão deste Ítem?") )
    {
	document.formulario.act.value = 'excluir';
	document.formulario.submit();
    } else {};
  }
</script>

<?}
elseif ($acao=='A')
{?>
<?monta_titulo($titulo_modulo,'<img src="../imagens/alterar.gif" border="0" align="absmiddle"> = Alterar / <img src="../imagens/excluir.gif" border="0" align="absmiddle"> = Excluir');?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<form method="POST"  name="formulario">
<input type='hidden' name="act">
<input type='hidden' name="abacod">
<?
//teste utilizando a função Monta Lista
$cabecalho = array('Ações','Código','Aba','Qtde. de Módulos');
//$sql = "select acacod, acadsc from acao";
$sql= "select '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Aba\" onclick=\"altera_aba('||abacod||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Aba\" onclick=\"excluir_aba('||abacod||')\">', abacod, abadsc, (select count(*) from aba_menu where aba_menu.abacod=aba.abacod and aba.sisid=".$_SESSION['sisid']." ) as abaqtd from aba where aba.sisid=".$_SESSION['sisid']."  order by abadsc";
$db->monta_lista($sql,$cabecalho,100,20,'','','');
?>
</form>
</table>
<script>
  function altera_aba(cod) {
     location.href = 'painel.php?modulo=sistema/menu/aba&acao=A&abacod_int='+cod;
  }
  

  function excluir_aba(cod) { 
    if( window.confirm( "Confirma a exclusão do ítem "+ cod) )
    {
	document.formulario.abacod.value = cod;
	document.formulario.act.value = 'excluir';
	document.formulario.submit();
    } else {};
  }
</script>
<?
}
?>