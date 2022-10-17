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
   	
$modulo=$_REQUEST['modulo'] ;//

if ($_REQUEST['act'] == 'inserir')
{
   //verifica se existe módulo igual cadastrado
   $sql = "select count(*) as total from menu where mnulink<>'' and sisid = ". $_SESSION['sisid'] . " and trim(mnulink)='".trim($_REQUEST['mnulink'])."'";
   $RS = $db->recuperar($sql,$res);
   if ($RS['total']>0)
   {
   ?>
   <script language="JavaScript">alert('Este Menu/Módulo já se encontra cadastrado!');history.back();</script>
   <?
   $db -> close();
   exit();
   }
   // fazer inserção de menu na base de dados.
   if (! $_REQUEST['abacod']) $_REQUEST['abacod']='null';
   $sql = "insert into seguranca.menu (mnucod,mnuidpai,mnudsc,mnutransacao,mnulink,mnutipo,mnustile,mnuhtml, mnusnsubmenu,mnushow,mnustatus,abacod,sisid) values ("
   .$_REQUEST['mnucod'].",";
   if ($_REQUEST['mnuidpai']) $sql = $sql.$_REQUEST['mnuidpai'].","; else $sql = $sql.' null,';
   $sql = $sql .
   "'".$_REQUEST['mnudsc']."',".
   "'".$_REQUEST['mnutransacao']."',".
   "'".$_REQUEST['mnulink']."',".
   "'".$_REQUEST['mnutipo']."',".
   "'".$_REQUEST['mnustile']."',".
   "'".$_REQUEST['mnuhtml']."',".
   "'".$_REQUEST['mnusnsubmenu']."',".
   "'".$_REQUEST['mnushow']."',".
   "'A',".$_REQUEST['abacod'].",".$_SESSION['sisid'].")";
   $saida = $db->executar($sql);
   $db->commit();
   $db->sucesso($modulo);
}
if ($_REQUEST['act']=='alterar')
{
	//verifica se existe outro módulo igual alterado
   $sql = "select count(*) as total from seguranca.menu where mnulink<>'' and sisid = " . $_SESSION['sisid'] . " and mnulink='".$_REQUEST['mnulink']."' and mnuid<>".$_REQUEST['mnuid_int'];
   $RS = $db->recuperar($sql,$res);
   if ($RS['total']>0)
   {
   ?>
   <script language="JavaScript">alert('Este Menu/Módulo já se encontra cadastrado!');history.back();</script>
   <?
   $db -> close();
   exit();
   }
   // fazer alteração do menu na base de dados.
   if ($_REQUEST['mnusnsubmenu'] == 't') $_REQUEST['mnusnsubmenu'] = 'true' ; else $_REQUEST['mnusnsubmenu'] = 'false';
    if ($_REQUEST['mnushow'] == 't') $_REQUEST['mnushow'] = 'true' ; else $_REQUEST['mnushow'] = 'false';
   $sql = "update seguranca.menu set mnucod=".$_REQUEST['mnucod'].", mnudsc='".
   $_REQUEST['mnudsc'].
   "', mnutransacao='".$_REQUEST['mnutransacao'].
   "',  mnulink='".$_REQUEST['mnulink'].
   "',mnutipo='".
   $_REQUEST['mnutipo'].
   "',   mnustile='".
   $_REQUEST['mnustile'].
   "',   mnuhtml='".$_REQUEST['mnuhtml']."',mnuidpai=";
   if ($_REQUEST['mnuidpai']) $sql = $sql.$_REQUEST['mnuidpai'].","; else $sql = $sql.' null,';
	$sql = $sql." abacod = ";
if ($_REQUEST['abacod']) $sql = $sql.$_REQUEST['abacod'].","; else $sql = $sql.' null,';
   $sql = $sql.
   "  mnusnsubmenu=".$_REQUEST['mnusnsubmenu'].
   ",  mnushow=".$_REQUEST['mnushow'].
   "  where mnuid=".$_REQUEST['mnuid_int'];
    $saida = $db->executar($sql);
    $db->commit();
	?><script>
              alert('Operação realizada com sucesso');
              location.href="<?=$_SESSION['sisdiretorio']?>.php?modulo=<?=$modulo.'&acao=A&mnuid_int='.$_REQUEST['mnuid']?>";
              
            </script><?
  		    $db -> close();
			exit();
}

if ($_POST['exclui'] > 0) 
{
	$sql = "delete from seguranca.estatistica where mnuid=".$_POST['exclui'];
	  $saida = $db->executar($sql);
	$sql = "delete from seguranca.auditoria where mnuid=".$_POST['exclui'];
	  $saida = $db->executar($sql);	  
	// não pode deixar escluir se já estiver associado à algum perfil
	//$sql = "delete from seguranca.perfilmenu where mnuid=".$_POST['exclui'];
    //$saida = $db->executar($sql);
	
    $sql = "delete from seguranca.menu where mnuid=".$_POST['exclui'];
    $saida = $db->executar($sql);
    unset($_POST['exclui']);
    $db->commit();
    $db->sucesso($modulo);    
}

include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?
$parametros = array('','','');
$db->cria_aba($abacod_tela,$url,$parametros);
?>
<?if ($_REQUEST['acao']=='I' or ($_REQUEST['acao']=='A' and $_REQUEST['mnuid_int']<>'')){?>
<?
//título da página
if ($_REQUEST['acao']=='I') $titulo_modulo = 'Incluir Menu'; else $titulo_modulo = 'Alterar Menu';
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<?
if($_REQUEST['mnuid_int'] and $_REQUEST["act"]=='') { 
       $sql= "select * from seguranca.menu where mnuid=".$_REQUEST['mnuid_int'];    
	   $saida = $db->recuperar($sql,$res);
       if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}

?>
<? } else { 

    //recupera todas as variaveis que veio pelo post
	foreach($_REQUEST as $k=>$v) ${$k}=$v;
 } ?>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<form method="POST"  name="formulario">
	<input type='hidden' name="modulo" value="<?=$modulo?>">
	<input type='hidden' name="mnuid_int" value=<?=$_REQUEST['mnuid_int']?>>
	<input type='hidden' name='exclui' value=0>
	<input type='hidden' name='act' value=0>
	<input type='hidden' name='acao' value=<?=$_REQUEST['acao']?>>
      <tr>
        <td align='right' class="SubTituloDireita">Código:</td>
        <td>
	        <? if (! $_REQUEST['mnuid']) $habil='S' ;else $habil= 'N';?>
		<?=campo_texto('mnucod','S',$habil,'',6,5,'#####','');?>
        </td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Descrição:</td>
        <td><?=campo_texto('mnudsc','S','S','',50,50,'','');?></td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Transação:</td>
        <td><?=campo_texto('mnutransacao','S','S','',50,50,'','');?></td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Tipo:</td>
        <td>
         <select name="mnutipo" onchange="document.formulario.submit();" class="CampoEstilo">
			<option value=""></option>
			<option value="1" <?if ($mnutipo=="1") print "selected";?>>1</option>
			<option value="2" <?if ($mnutipo=="2") print "selected";?>>2</option>
			<option value="3" <?if ($mnutipo=="3") print "selected";?>>3</option>
			<option value="4" <?if ($mnutipo=="4") print "selected";?>>4</option>
		</select>
	<?=obrigatorio();?></td>
      </tr>
      <?if ($mnutipo <> "" and $mnutipo <> "1") {?>
      <tr>
        <td align='right'  class="SubTituloDireita">Menu Pai:</td>
        <td>
         <?
		$sql="SELECT mnuid as codigo, mnudsc as descricao FROM seguranca.menu where mnutipo = " . ($mnutipo-1) . " and mnusnsubmenu='t' and sisid=".$_SESSION['sisid']."  order by mnudsc";
		$db->monta_combo("mnuidpai",$sql,'S',"Selecione o Menu",'','');
	 ?>
    	 <?=obrigatorio();?></td>
        </tr>
      <?}?>
      <tr>
        <td align='right'  class="SubTituloDireita">Possui Sub-menu?</td>
        <td>
        <input type="radio" name="mnusnsubmenu" value="t" onchange="submenu('S');" <?=($mnusnsubmenu=='t'?"CHECKED":"")?>> Sim
            <input type="radio" name="mnusnsubmenu" value="f" onchange="submenu('N');" <?=($mnusnsubmenu=='f'?"CHECKED":"")?>> Não
        </td>
      </tr> 
      <tr>
        <td id="sub0" align='right'  class="SubTituloDireita">Link:</td>
        <td id="sub1"><?=campo_texto('mnulink','N','S','',50,100,'','');?></td>
      </tr>
      <tr>
        <td id="sub2" align='right'  class="SubTituloDireita">Aba:</td>
        <td id="sub3"><?$sql="SELECT abacod as codigo, abadsc as descricao FROM seguranca.aba where sisid=".$_SESSION['sisid'];
		$db->monta_combo("abacod",$sql,'S',"Selecione a Aba",'','');?></td>
      </tr>
	  <tr>
	  
        <td id="sub4" align='right'  class="SubTituloDireita">Faz parte da árvore?</td>
        <td id="sub5"><input type="radio" name="mnushow" value="t" <?=($mnushow=='t'?"CHECKED":"")?>> Sim
            <input type="radio" name="mnushow" value="f" <?=($mnushow=='f'?"CHECKED":"")?>> Não
	</td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Estilo:</td>
        <td><?=campo_texto('mnustile','N','S','',50,150,'','');?></td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Html:</td>
        <td><?=campo_textarea('mnuhtml','N','S','',60,5,500);?></td>
      </tr>

<? if   ($_REQUEST["mnuid_int"]) { ?>
<tr bgcolor="#CCCCCC">
   <td></td>
   <td><input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao">&nbsp;&nbsp;&nbsp;&nbsp;<input type="button" name="btcancelar" value="Voltar" onclick="history.back();" class="botao"></td>
 </tr>
</table>
<? } else { ?>
<tr bgcolor="#CCCCCC">
   <td></td><td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao">&nbsp;&nbsp;&nbsp;&nbsp;<input type="button" name="btcancelar" value="Voltar" onclick="history.back();" class="botao"></td>
 </tr>
 </form>
 </table>
<? } 
} elseif ($_REQUEST['acao']=='A' and !$_REQUEST['mnuid_int']){?>
<?
//título da página
monta_titulo($titulo_modulo,'Escolha uma ação desejada referente ao Menu / Módulo : 	<img src="../imagens/alterar.gif" border="0" align="absmiddle"> = Alterar / 	<img src="../imagens/excluir.gif" border="0" align="absmiddle"> = Excluir / 	<img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"> = Incluir');
?>
<form method="POST"  name="formulario">
<input type='hidden' name='mnuidpai'>
<input type='hidden' name='mnutipo'>
<input type='hidden' name='exclui'>
</form>
<?
//teste utilizando a função Monta Lista
$cabecalho = array('Ações','Código','Menu / Módulo','Visível','Transação');
//$sql = "select acacod, acadsc from acao";
$sql= "select  case when mnusnsubmenu=true then '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Menu\" onclick=\"altera_menu('||mnuid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Menu\" onclick=\"excluir_menu('||mnuid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/gif_inclui.gif\" title=\"Incluir Menu em » '||mnudsc||'\" onclick=\"incluir_menu('||mnuid||','||mnutipo||')\">' else '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Menu\" onclick=\"altera_menu('||mnuid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Menu\" onclick=\"excluir_menu('||mnuid||')\">' end as acao, mnucod, case when mnutipo=2 then '&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc when  mnutipo=3 then '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc when  mnutipo=4 then '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc else mnudsc end as mnudsc, case when mnushow=false then '<font color=#808080>Não</font>' else '<font color=#008000>Sim</font>' end as mnushow, mnutransacao from seguranca.menu where mnustatus = 'A' and sisid=".$_SESSION['sisid']."  order by mnucod";
$db->monta_lista_simples($sql,$cabecalho,200,20,'','','');
}?>

<script>
<?if ($_REQUEST["mnuid_int"]){?>
if (document.formulario.mnusnsubmenu[0].checked)
{
 		  document.formulario.mnulink.disabled = true;
		  document.formulario.abacod.disabled = true;
		  //document.formulario.mnushow[0].checked = false;
		  //document.formulario.mnushow[1].checked = false;
		  for (i=0;i<6;i++)
					{
					document.getElementById("sub"+i).style.visibility = "hidden";
					document.getElementById("sub"+i).style.display = "none";
					}
}
<?}?>


  function submenu(op)
  {
	  if (op == 'S')
	  {
		  document.formulario.mnulink.disabled = true;
		  document.formulario.abacod.disabled = true;
		  document.formulario.mnushow[0].checked = true;
		  document.formulario.mnushow[1].checked = false;
		  for (i=0;i<6;i++)
					{
					document.getElementById("sub"+i).style.visibility = "hidden";
					document.getElementById("sub"+i).style.display = "none";
					}
	  }
	  else
	  {
	  	  document.formulario.mnulink.disabled = false;
		  document.formulario.abacod.disabled = false;
		  for (i=0;i<6;i++)
					{
					document.getElementById("sub"+i).style.visibility = "visible";
					document.getElementById("sub"+i).style.display = "";
					}
	  }
  }
  function altera_menu(cod) {
     location.href = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/menu/menu&acao=A&mnuid_int='+cod;
  }
  
  function incluir_menu(codpai, tipo) {
     document.formulario.mnuidpai.value = codpai;
	 document.formulario.mnutipo.value = tipo+1;
	 location.href = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/menu/menu&acao=I&mnuidpai='+document.formulario.mnuidpai.value+'&mnutipo='+document.formulario.mnutipo.value;
  }
  
  function excluir_menu(cod) { 
    if( window.confirm( "Confirma a exclusão do ítem "+ cod + " no Menu?") )
    {
	document.formulario.exclui.value = cod;
	document.formulario.submit();
    } else document.formulario.exclui.value = 0;
  }
  
    function validar_cadastro(cod) {    	
		if (!validaBranco(document.formulario.mnucod, 'Código')) return;	
		if (!validaBranco(document.formulario.mnudsc, 'Descrição')) return;	
		if (!validaBranco(document.formulario.mnutransacao, 'Transação')) return;
		if (!validaBranco(document.formulario.mnutipo, 'Tipo')) return;			
		if (document.formulario.mnutipo.value != "1" )
		{		
		if (!validaBranco(document.formulario.mnuidpai, 'Menu Pai')) return;
		   
		}		
		if (!validaRadio(document.formulario.mnusnsubmenu,'Possui sub-menu')) return;
		if (document.formulario.mnusnsubmenu[1].checked) {
			if (!validaBranco(document.formulario.mnulink, 'Link')) return;
			if (!validaRadio(document.formulario.mnushow,'Faz Parte da Árvore')) return;
		}else	{
			document.formulario.mnulink.value = '';
			document.formulario.abacod.value = '';
			}
		
		
	   	if (cod == 'I') document.formulario.act.value = 'inserir'; else document.formulario.act.value = 'alterar';
   	   	document.formulario.submit();

     }   		    
	      

</script>
