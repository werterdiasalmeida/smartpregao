 <?
/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Marcelo Freire Costa (marcelofreire.freire@gmail.com)
   Programador: Marcelo Freire Costa (e-mail: marcelofreire.freire@gmail.com)
   Módulo:associa_perfil.inc
   Finalidade: permitir a associação de um perfil as operações de menu
   */
   
//Armazena o caminho de redirecionamento
$modulo=$_REQUEST['modulo'] ;


//Caso seja seja solicitada a inclusão de menus ao perfil faça
if ($_REQUEST['act']=='cadastra' and $_POST['pflcod']<>'') {
//Deleta-se os menus do perfil para cadastrar novamente
  $sql = 'delete from perfilmenu where pflcod='.$_POST['pflcod'];
  $db->executar($sql);
  $mnuid = $_POST['mnuid'];
  $nlinhas = count($mnuid)-1;
	for ($j=0; $j<=$nlinhas;$j++)
		{
			//Cadastra-se o menu do perfil
		  $sql = "insert into perfilmenu (pflcod, mnuid, pmnstatus) VALUES (".$_POST['pflcod'].",".$mnuid[$j].",'A')";
		  $db->executar($sql);
		 }
  $db -> commit();
  $db->sucesso($modulo);
}
//Caso seja passado o código do perfil atribui-se a variável $pflcod
if ($_REQUEST['pflcod']) $pflcod = $_REQUEST['pflcod'];

//Inclui o cabeçalho da página
include APPRAIZ."includes/cabecalho.inc";

?>
<br>
<?
//cria as abas
$db->cria_aba($abacod_tela,$url,$parametros);
?>
<?
//título da página
monta_titulo($titulo_modulo,'Selecione os Módulos para este Perfil.');
?>

<form method="POST" name="formulario">
<input type=hidden name="act" value=0>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?
if($_SESSION['sisid'] == 4) {
?>
<tr><td class="SubTituloDireita">Sistema:</td>
<td>
	<?
	  //Atribui o sisid passado a uma variável
	  $sisid = $_REQUEST['sisid'];
		  
	      //Caso não seja passado o sisid atribui-se valor zero para não dar erro nas consultas abaixo
		  if($sisid == '')
		  {
		  	$sisid =0;
		  }
	  
	//Pesquisa-se os sistemas cadastrados para montar a combo	  
	  $sql = "select distinct sisid as CODIGO,sisdsc as DESCRICAO from sistema order by DESCRICAO ";
	  //Monta a combo
	  $db->monta_combo("sisid",$sql,'S',"Selecione o Sistema",'submete_sistema','');
	
	  ?>
</td></tr>
<tr><td class="SubTituloDireita">Associação do Perfil:</td>
<td>
	<?
	 //Pesquisa os perfis cadastrados para o sistema para montar a combo
	  $sql = "select pflcod as CODIGO,pfldsc as DESCRICAO from perfil where pflstatus='A' and sisid=".$sisid." order by pfldsc ";
	  //monta a combo de perfis
	  $db->monta_combo("pflcod",$sql,'S',"Selecione o Perfil",'submete_perfil','');
	?>
</td></tr>
<?
} else {
?>
<tr><td class="SubTituloDireita">Associação do Perfil:</td>
<td>
	<?
	  $sisid = $_SESSION['sisid'];
	 //Pesquisa os perfis cadastrados para o sistema para montar a combo
	  $sql = "select pflcod as CODIGO,pfldsc as DESCRICAO from perfil where pflstatus='A' and sisid=".$_SESSION['sisid']." order by pfldsc ";
	  //monta a combo de perfis
	  $db->monta_combo("pflcod",$sql,'S',"Selecione o Perfil",'submete_perfil','');
	?>
</td></tr>
<?	
}
?>
</table>
<?
 if ($pflcod && $sisid) {
//teste utilizando a função Monta Lista
$cabecalho = array('Sim/Não','Código','Menu / Módulo','Visível','Link','Transação');
//$sql = "select acacod, acadsc from acao";
//$sql= "select  case when p.mnuid is null then '<input type=\"Checkbox\" name=\"mnuid[]\" value=\"'||menu.mnuid||'\">' else '<input type=\"Checkbox\" name=\"mnuid[]\" value=\"'||menu.mnuid||'\" checked>' end as acao, menu.mnucod, case when mnutipo=2 then '&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc when  mnutipo=3 then '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc when  mnutipo=4 then '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc else mnudsc end as mnudsc, case when mnushow=false then '<font color=#808080>Não</font>' else '<font color=#008000>Sim</font>' end as mnushow, mnutransacao from menu left join perfilmenu p on menu.mnuid = p.mnuid and p.pflcod=".$pflcod." where  mnustatus = 'A' and menu.sisid=".$_SESSION['sisid']."  order by mnucod";
	




$sql= "select  case when p.mnuid is null then '<input type=\"Checkbox\" name=\"mnuid[]\" value=\"'||menu.mnuid||'\">' else '<input type=\"Checkbox\" name=\"mnuid[]\" value=\"'||menu.mnuid||'\" checked>' end as acao, menu.mnucod, case when mnutipo=2 then '&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc when  mnutipo=3 then '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc when  mnutipo=4 then '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc else mnudsc end as mnudsc, case when mnushow=false then '<font color=#808080>Não</font>' else '<font color=#008000>Sim</font>' end as mnushow, mnulink, mnutransacao from menu left join perfilmenu p on menu.mnuid = p.mnuid and p.pflcod=".$pflcod." where  mnustatus = 'A' and menu.sisid=".$sisid."  order by mnucod";

//Monta a lista de menus do perfil
$db->monta_lista_simples($sql,$cabecalho,300,20,'','','');
?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr  bgcolor="#CCCCCC"><td class="SubTituloDireita"></td><td><input type='button' value='Cadastrar' onclick="validar_cadastro()"></td></tr>
</table>
<?}?>
</form>
<script language="JavaScript">

  function validar_cadastro() {
    //Atribui a ação a ser realizada
    document.formulario.act.value = 'cadastra';
    //Submete a página
	document.formulario.submit();
  }
  
  function submete_perfil(cod){
  	//Caso o código do perfil e do sistema sejam informados
  	    if (document.formulario.pflcod.value != '' || document.formulario.sisid.value != '')
    {
    	//Submete a página
        document.formulario.submit();
    }
  }
  
    function submete_sistema(cod){
    	
    	 //Limpa o código do perfil
    	document.formulario.pflcod.value = '';
    	//Submete a página
        document.formulario.submit();
    }

</script>