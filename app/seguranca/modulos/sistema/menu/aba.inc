
<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:menu.inc
   Finalidade: permitir o cadastro de itens de menu
   */
   //
$modulo=$_REQUEST['modulo'] ;
$act=$_REQUEST['act'];
$acao=$_REQUEST['acao'];

//inclui abas
if (($act == 'incluir' and $acao=='I') or ($act == 'alterar' and $acao=='A'))
{
	if ($acao == 'I') {
    $sql = "insert into aba (abadsc,sisid) values ('".$_POST['abadsc']."',".$_REQUEST['sisid'].")";
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
	  $sql = 'select * from menu order by mnuid';
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
            $sql = 'select * from aba_menu where mnuid='.$mnuid.' and abacod='.$abacodmax;
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
monta_titulo($titulo_modulo,'<img src="/imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<form method="POST"  name="formulario1">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<input type='hidden' name="act">
<input type='hidden' name="abacod" value="<?=$abacod?>">

<tr>
        <td align='right' class="SubTituloDireita">Sistema:</td>
        <td>
	        
		 <?
		
		
		 
		 //Caso a página de inclusão tenha sido aberta pelo menu Administrar abas
	     if($_REQUEST['abaselecionada']=='')
	     {
		$sql="SELECT sisid as codigo, sisdsc as descricao FROM sistema order by sisdsc";
		
		 $sisid = $_REQUEST['sisid'];
		
		$db->monta_combo("sisid",$sql,'S',"Selecione o Sistema",'','');
	     }
	     //Caso tenha sido aberta pela tela de administração de menus faça
	     else
	     {
	      //Armazena o sisid passado por querystring na váriável de sessão
	      
	      
	      //Caso não tenha sido passado o sisid ainda atribui-se zero a variável de sessão
	      if($_REQUEST['sisid']== '')
	      {
	      	
	      	$sisid = 0;
	      	
	      }
	      	
	     	 $sql= "select * from sistema where sisid=".$sisid;
	   $saida = $db->recuperar($sql,$res);
       if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}
	     	
	     	print $sisdsc;
	    ?>
	     	<input type ="hidden" name = "sisid" value ="<?=$sisid?>">
	    <? 	
	     }
		
		?>
        </td>
      </tr>
      <tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Descrição:</td>
        <td><?=campo_texto('abadsc','S','S','',50,50,'','');?></td>

        
	  <tr>
        <td align='right'  class="SubTituloDireita">Módulos/Menus:</td>
        <td>
   		<table cellpadding="0" cellspacing="0" border="0" align="left">
   		
   		<?


	//$cabecalho = array('Ações','Código','Aba','Qtde. de Módulos');
		//$sql = "select case when aba_menu.mnuid isnull then '<input type=\"checkbox\" name=\"'||menu.mnuid||'\" value=\"'||menu.mnuid||'\">' else '<input type=\"checkbox\" name=\"'||menu.mnuid||'\" value=\"'||menu.mnuid||'\" checked>' end as check, menu.mnuid||' - '||menu.mnudsc from menu left join aba_menu on menu.mnuid=aba_menu.mnuid and aba_menu.abacod =".$abacod;
		if ($acao=='I') $sql = "select menu.mnuid, menu.mnudsc, menu.mnulink, menu.mnutransacao, menu.mnutipo, '' as check from menu order by menu.mnuid";
		else $sql = "select menu.mnuid, menu.mnudsc, menu.mnulink, menu.mnutransacao, menu.mnutipo, case when aba_menu.mnuid isnull then '' else 'checked' end as check from menu left join aba_menu on menu.mnuid=aba_menu.mnuid and aba_menu.abacod =".$abacod.' order by menu.mnuid';
		
		//print $sql;
		
		$RS = $db->carregar($sql);
		  $nlinhas = count($RS)-1;
		  if ($nlinhas >= 0)
		  {
		    for ($i=0; $i<=$nlinhas;$i++)
		    {	
		    	
				if(is_array($RS)) foreach($RS[$i] as $k=>$v) ${$k}=$v;
				if ($mnutipo==2) $space = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'; elseif ($mnutipo==3) $space = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'; else $space='';
				if ($mnulink<>'') $block = ''; else $block = 'disabled';
				print $space.'<input type="checkbox" name="'.$mnuid.'" value="'.$mnuid.'"'.$check.' '.$block.'>'.$mnuid.' - '.$mnutransacao.'<br>';
			}
		  }
		?></tr></table>
	</td></tr>
<tr bgcolor="#CCCCCC">
   <td></td><td>
   <?if ($acao == 'A'){?>
   <input type="button" name="btalterar" value="Alterar" onclick="altera()" class="botao">
   &nbsp;&nbsp;&nbsp;&nbsp;<input type="button" name="btexcluir" value="Excluir" onclick="return exclui()" class="botao">
   <?}else{?>
   <input type="button" name="btinserir" value="Incluir" onclick="return inclui()" class="botao">
   <?}?>
   &nbsp;&nbsp;&nbsp;&nbsp;<input type="button" name="btcancelar" value="Voltar" onclick="history.back();" class="botao"></td>
 </tr>

</table>
</form>
<form name = "formulario">
<script language="JavaScript">

  function inclui() {
  	
  	if(document.formulario1.sisid.value=='')
  	{
  		
  			alert('Campo Obrigatório: Sistema')
  			document.formulario1.sisid.focus();
  			return false;
  		
  	}
  	
	if (!validaBranco(document.formulario1.abadsc, 'Descrição')) return;
    document.formulario1.act.value = 'incluir';
	document.formulario1.submit();
  }
  
  function altera() {
  	
 	
	if (!validaBranco(document.formulario1.abadsc, 'Descrição')) return;
    document.formulario1.act.value = 'alterar';
	document.formulario1.submit();
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
<?monta_titulo($titulo_modulo,'<img src="/imagens/alterar.gif" border="0" align="absmiddle"> = Alterar / <img src="/imagens/excluir.gif" border="0" align="absmiddle"> = Excluir');?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<form method="POST"  name="formulario">

<input type='hidden' name="act">
<input type='hidden' name="abacod">

     <tr>
        <td align='right' class="SubTituloDireita">Sistema:</td>
        <td>
<?
$sisid = $_REQUEST['sisid'];

if($_REQUEST['sisid']=='')
{
	
	$sisid =0;
}
	$sql = "select distinct sisid as CODIGO,sisdsc as DESCRICAO from sistema order by DESCRICAO ";
	$db->monta_combo('sisid',$sql,'S',"Selecione o Sistema",'submete_sistema','');




 $sql = "select distinct sisid as CODIGO,sisdsc as DESCRICAO from sistema where sisid=".$sisid." order by DESCRICAO ";
	//$saida = $db->recuperar($sql,$res);
      // if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}
?>
     </td>
   </tr>
 </table>
<?
//teste utilizando a função Monta Lista
$cabecalho = array('Ações','Código','Aba','Qtde. de Módulos');
//$sql = "select acacod, acadsc from acao";
$sql= "select '<img border=\"0\" src=\"/imagens/alterar.gif\" title=\"Alterar Aba\" onclick=\"altera_aba('||abacod||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"/imagens/excluir.gif\" title=\"Excluir Aba\" onclick=\"excluir_aba('||abacod||')\">', abacod, abadsc, (select count(*) from aba_menu where aba_menu.abacod=aba.abacod) as abaqtd from aba where sisid =".$sisid." order by abadsc";
$db->monta_lista($sql,$cabecalho,100,20,'','','');
?>
</form>

<script>
  function altera_aba(cod) {
     location.href = 'seguranca.php?modulo=sistema/menu/aba&acao=A&abacod_int='+cod + "&abaselecionada=sim&sisid=<?=$sisid?>";
  }
  

  function excluir_aba(cod) { 
    if( window.confirm( "Confirma a exclusão do ítem "+ cod) )
    {
	document.formulario.abacod.value = cod;
	document.formulario.act.value = 'excluir';
	document.formulario.submit();
    } else {}
  }
  
  function submete_sistema(sisid)
  {
   
  	
  	location.href ='seguranca.php?modulo=sistema/menu/aba&acao=A&sisid=' + sisid
  	  
  }
  
</script>
<?
}
?>