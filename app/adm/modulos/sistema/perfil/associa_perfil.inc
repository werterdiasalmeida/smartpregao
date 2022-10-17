 <?
/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:associa_perfil.inc
   Finalidade: permitir a associação de um perfil as operações de menu
   */
   

$modulo=$_REQUEST['modulo'] ;

if ($_REQUEST['act']=='cadastra' and $_POST['pflcod']<>'') {
  $sql = 'delete from perfilmenu where pflcod='.$_POST['pflcod'];

  $db->executar($sql);
  $mnuid = $_POST['mnuid'];
  //print_r($_POST['mnuid']);
  $nlinhas = count($mnuid)-1;

	for ($j=0; $j<=$nlinhas;$j++)
		{
		  $sql = "insert into perfilmenu (pflcod, mnuid, pmnstatus) VALUES (".$_POST['pflcod'].",".$_POST['mnuid'][$j].",'A')";
		 // print $_POST['mnuid'][$j].'<br>';
		  $db->executar($sql);
		 }
  $db -> commit();
  $db->sucesso($modulo);
}

if ($_REQUEST['pflcod']) $pflcod = $_REQUEST['pflcod'];

include APPRAIZ."includes/cabecalho.inc";

?>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
//título da página
monta_titulo($titulo_modulo,'Selecione os Módulos para este Perfil.');
?>

<form method="POST" name="formulario">
<input type=hidden name="act" value=0>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr><td class="SubTituloDireita">Associação do Perfil:</td>
<td>
	<?
	  $sql = "select pflcod as CODIGO,pfldsc as DESCRICAO from perfil where pflstatus='A' and sisid=".$_SESSION['sisid']." order by pfldsc ";
	  $db->monta_combo("pflcod",$sql,'S',"Selecione o Perfil",'submete_perfil','');
	?>
</td></tr>
</table>
<?
 if ($pflcod) {
//teste utilizando a função Monta Lista
$cabecalho = array('Sim/Não','Código','Menu / Módulo','Visível','Transação');
//$sql = "select acacod, acadsc from acao";
$sql= "select  case when p.mnuid is null then '<input type=\"Checkbox\" name=\"mnuid[]\" value=\"'||menu.mnuid||' \">' else '<input type=\"Checkbox\" name=\"mnuid[]\" value=\"'||menu.mnuid||'\" checked>' end as acao, menu.mnucod, case when mnutipo=2 then '&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc when  mnutipo=3 then '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc when  mnutipo=4 then '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc else mnudsc end as mnudsc, case when mnushow=false then '<font color=#808080>Não</font>' else '<font color=#008000>Sim</font>' end as mnushow, mnutransacao from menu left join perfilmenu p on menu.mnuid = p.mnuid and p.pflcod=".$pflcod." where  mnustatus = 'A' and menu.sisid=".$_SESSION['sisid']."  order by mnucod";
//print $sql;
$db->monta_lista_simples($sql,$cabecalho,200,20,'','','');
?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr  bgcolor="#CCCCCC"><td class="SubTituloDireita"></td><td><input type='button' value='Cadastrar' onclick="validar_cadastro()"></td></tr>
</table>
<?}?>
</form>
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