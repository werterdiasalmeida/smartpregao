<?
 /* 
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:cadperfil.inc
   Finalidade: permitir o controle de cadastro dos perfis
   */
$modulo=$_REQUEST['modulo'] ;//
//print "teste";


if (($_REQUEST['act'] == 'inserir') and (! is_array($msgerro)))
{

  $sql= "select pflcod from perfil where pflstatus='A' and pfldsc='".$_REQUEST['pfldsc']."' and sisid='".$_REQUEST['sisid']."'";
  //dbg($_REQUEST ,1);
  $usu = $db->recuperar($sql);
	if (is_array($usu)) {
	   // existe perfil identico, logo, tem que bloquear
	   ?>
	      <script>
	         alert ('O Perfil: <?=$_REQUEST['pfldsc']?> já se encontra cadastrado no sistema.');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
// Alteração Marcelo 06/07/2006
// Início Alteração
 	   
    
   $sql = "insert into perfil (pflstatus,pfldsc,pflfinalidade, pfldatainicio,pfldatafim,sisid) values ('A',".
   "'".$_REQUEST['pfldsc']."',".
   "'".$_REQUEST['pflfinalidade']."',".
    "'".$_REQUEST['pfldatainicio']."',".
   "'".$_REQUEST['pfldatafim']."',".
   "".$_REQUEST['sisid'].")"; 
   
   
    if ((! $_REQUEST['pfldatafim']) or (! $_REQUEST['pfldatainicio']) )
    {
        $sql = "insert into perfil (pflstatus,pfldsc,pflfinalidade,sisid) values ('A',".
   "'".$_REQUEST['pfldsc']."',".
   "'".$_REQUEST['pflfinalidade']."',".
    "".$_REQUEST['sisid'].")";
    }
 

    $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo);
}
//Fim alteração

if (($_REQUEST['act']=='alterar') and (! is_array($msgerro)))
{
   // fazer alteração de perfil na base de dados.
   $sql = '';
   if ($_REQUEST['pfldatafim'] and $_REQUEST['pfldatainicio'] ) {
      $sql = "update perfil set pflfinalidade='".$_REQUEST['pflfinalidade']."', pfldatafim='".
      $_REQUEST['pfldatafim'].
      "',  pfldatainicio='".$_REQUEST['pfldatainicio'].
      "'  where pflcod=".$_REQUEST['pflcod']." and sisid=".$_REQUEST['sisid'];
    }
    if (! $_REQUEST['pfldatafim'] and ! $_REQUEST['pfldatainicio'] ) 
    {
	
    	$sql = "update perfil set pflfinalidade='".$_REQUEST['pflfinalidade']."',pfldatafim=null, pfldatainicio=null where pflcod=".$_REQUEST['pflcod']." and sisid=".$_REQUEST['sisid'];
    }
    

//    dbg($sql,1,'00677864132');
    if ($sql <> '') {
       $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo);
  }
}



if ($_POST['exclui'])
{

    $sql = "update perfil set pflstatus='I' where pflcod=".md5_decrypt($_POST['exclui'],'')." and sisid=".$_SESSION['sisid'];
//	dbg($sql,1,'00677864132');
    $saida = $db->executar($sql);
	$db->commit();
	$db->sucesso($modulo);
}
include APPRAIZ."includes/cabecalho.inc";
?>
<script language="JavaScript" src="../../../../includes/calendario.js"></script>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo('Gerenciar Perfil','<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
?>
<div align="center">
<center>

<?
if($_REQUEST['refcod']) {
        $sql= "select * from perfil where pflcod=".md5_decrypt($_REQUEST['refcod'],'');
        $saida = $db->recuperar($sql,$res);
           
        if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;
        }
 } else {
	$pfldatainicio = $_REQUEST['pfldatainicio'];
	$pfldatafim =$_REQUEST['pfldatafim'];
	$pfldsc = $_REQUEST['pfldsc'];
	$pflresponsabilidade = $_REQUEST['pflresponsabilidade'];

 } ?>

<form method="POST"  name="formulario">
<input type='hidden' name="modulo" value="<?=$modulo?>">
<input type='hidden' name="refcod" value=<?=$_REQUEST['refcod']?>>
<input type='hidden' name="pflcod" value=<?=$pflcod?>>
<input type='hidden' name='exclui' value=0>
<input type='hidden' name='act' value=<?=$_REQUEST['act']?>>
<input type='hidden' name='associa' value=0>
<? if ($_REQUEST['refcod']) {?>
<input type='hidden' name='dtini' value='<?=$pfldatainicio?>'>
<?}?>
    <center>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	    <tr>
		    <td class="SubTituloDireita">Sistema:</td>
			<td>
				<?
				  $sql = "SELECT DISTINCT sisid AS codigo, sisdsc AS descricao FROM seguranca.sistema ORDER BY descricao";

				 $sisid = $_SESSION['sisid'];
		

				$db->monta_combo("sisid",$sql,'N',"Selecione o Sistema",'submete_sistema','');
				?>
				   
				  	</td>
	</tr>
      <tr>
        <td align='right' class="SubTituloDireita">Descrição:</td>
	<td>
	<? if (! $_REQUEST['refcod']) $habil='S' ;else $habil= 'N';?>
	<?=campo_texto('pfldsc','S',$habil,'',45,45,'','');?>
	</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Finalidade:</td>
	<td><?=campo_textarea('pflfinalidade','S','S','',96,5,'');?>
	</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Data início:</td>
        <td>
		<?=campo_data('pfldatainicio', 'S','S','','S');?>
	</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Data fim:</td>
        <td>
	        <?=campo_data('pfldatafim', 'S','S','','S');?>
	</td>
	    
      </tr>
           <?//=seta_combo('sisid');?>
   

<? if   ($_REQUEST["refcod"]) { ?>
<script>
document.formulario.sisid.disabled =true;
</script>
<tr bgcolor="#CCCCCC">
   <td></td>
   <td><input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao">
   <input type="button" name="btvoltar" value="Cancelar" onclick="history.back();" class="botao"></td>
 </tr>
<? } else { ?>
<script>

//document.formulario.sisid.value =true;

</script>
<tr bgcolor="#CCCCCC">
   <td></td>
   <td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao"></td>

 </tr>
<? } ?>
    </table>
    <br><br>

<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" class="title"><strong>Ações</strong></td>
      <td valign="top" class="title"><strong>Perfil</strong></td>  
      <td valign="top" class="title"><strong>Sistema</strong></td>   
      <td valign="top" class="title"><strong>Início</strong></td>
      <td valign="top" class="title"><strong>Fim</strong></td>
    </tr>
</thead>
<tbody>
<?
$sql= "select pflcod as codigo, pfldsc as descricao, pfldatainicio, pfldatafim,sisdsc from perfil left join sistema on perfil.sisid = sistema.sisid where pflstatus = 'A' and sistema.sisid = ". $_SESSION['sisid'];

if($_REQUEST['sisid'] != '')
{
	$sql= $sql." and sistema.sisid=".$_REQUEST['sisid'];
}


 $sql= $sql." order by pfldsc";

 
$rs = $db->carregar( $sql );
if (  $rs && count($rs) > 0 )
	{
	 $i=0;
	 foreach ( $rs as $linha )
		{
		 if(is_array($linha)) foreach($linha as $k=>$v) ${$k}=$v;
		 if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
  		 print '<tr class="'.$marcado.'"><td><img border="0" src="../imagens/alterar.gif" title="Alterar o perfil." onclick="altera_perfil('."'".md5_encrypt($codigo,'')."'".')">&nbsp;&nbsp;&nbsp;<img border="0" src="../imagens/excluir.gif" title="Excluir o perfil." onclick="excluir_perfil('."'".md5_encrypt($codigo,'')."','".$descricao."'".')"></td><td>'.$descricao.'</td><td>'.$sisdsc.'</td><td>'.formata_data($pfldatainicio).'</td><td>'.formata_data($pfldatafim).'</td></tr>';
  		 $i++;
		}
	}
else
	{
  	 print '<tr class="'.$marcado.'"><td></td><td>Não há registros de perfil</td></tr>';
	}
?>
</tbody>
    </table>
    </center>
  </div>
</form>

<script>
  function altera_perfil(cod) {
  	
  	if(document.formulario.sisid.value=='')
  	{
  		alert('Campo obrigatório: Sistema')
  		return false;
  	}
  	
    document.formulario.refcod.value = cod;
     document.formulario.submit();
  }
  function excluir_perfil(cod,dsc) {
  
  	
  	
    if( window.confirm( "Confirma a exclusão do Perfil "+dsc+" ?") )
    {
	document.formulario.exclui.value = cod;
	document.formulario.submit();
    } else document.formulario.exclui.value = 0;
    
  }
  function associar_perfil(cod,dsc) {
	document.formulario.associa.value =cod;
	document.formulario.submit();

  }
  
function validar_cadastro(cod) 
{

	 document.formulario.sisid.disabled =false;
	
	 if(document.formulario.sisid.value == '')
	{
		alert('Campo Obrigatório: Sistema.');
		document.formulario.sisid.focus();		
		return false;
	}
	
	if (!validaBranco(document.formulario.pfldsc, 'Descrição')) return false;
	if (document.formulario.pfldatainicio.value != "" )
	{		
		if (!validaData(document.formulario.pfldatainicio))
		{
		   alert("Data Inicio Inválida.");
		   document.formulario.pfldatainicio.focus();
		   return false;
		}
		//data existe e é válida, nesta caso tem de existir uma data fim
		if (!validaBranco(document.formulario.pfldatafim, 'Data fim')) return;
		// a data fim exite. verifico se é válida
		if (!validaData(document.formulario.pfldatafim))
		{
		   alert("Data Fim Inválida.");
		   document.formulario.pfldatafim.focus();
		   return false;
		}
		// a data fim é válida. Tenho que verificar se ela é menor ouigual a de início.
		if (!validaDataMaior(document.formulario.pfldatainicio , document.formulario.pfldatafim))
		{
				alert("Data Fim  não pode ser Anterior que Data Inicio.");
				document.formulario.pfldatafim.focus();
				return false;
		} 
		
	}		
	if (cod == 'I') document.formulario.act.value = 'inserir'; else 		document.formulario.act.value = 'alterar';
	
	
	
	
   	document.formulario.submit();

}    

function submete_sistema()
{
	
	document.formulario.submit()
	
}

</script>
