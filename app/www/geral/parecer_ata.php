<?php

	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();

	$cod=$_REQUEST['arquivo'];
	// com o arquivo preciso verificar algumas coisas:
	/*
	1 - se o arquivo é realmente uma ata;
	2 - se a ata está aberta para parecer
	3 - se o usuário é um participante da reunião
	4 - se o usuário já opinou anteriormente
	*/
	
	// verificando se o arquivo é realmente uma ata
	$sql = "select tpdcod from documento where docid=$cod";
	$tipoarquivo = $db->pegaum($sql);
	if ($tipoarquivo !=3)
	{
		// o arquivo NÃO é uma ata
		print '<html><script language="javascript"> alert( "O arquivo solicitado não é uma Ata." ); window.close(); </script></hmtl>';
		exit();
	}
	// verificando se a ata está aberta para parecer
	$atafechada=0; // assume que toda ata está aberta para parecer
	$sql = "select aacsituacao from ata_acompanhamento where aacstatus='A' and docid=$cod";
	$statusata = $db->pegaum($sql);
	if ($statusata !='A')	
	{
        $atafechada=1;	
	}
	// verificar se o usuário fez parte da reunião
	$sql = "select arp.usucpf from agenda_reuniao_participante arp inner join agenda_reuniao ar on ar.agrid=arp.agrid inner join documento_processo dp on dp.proid=ar.agrid and dp.docid=$cod where arp.usucpf='".$_SESSION['usucpf']."'";
	$participante = $db->pegaum($sql);
	if (! $participante)	
	{
        $atafechada=1;	
	}
	$sql = "select * from ata_acompanhamento where aacstatus='A' and docid=$cod";
	$dados_ata = $db->carregar($sql);
	
	$lista = $_REQUEST['lista'];
	
	if ($_REQUEST['act']=='incluir')
	{
		// verificar se o usuário já possui parecer
		$sql = "select aa.aacid from ata_acompanhamento aa where docid=".$_REQUEST['doc']. " and usucpf = '".$_SESSION['usucpf']."' and aa.aacsituacao='A' limit 1";
		$possuiparecer = $db->pegaum($sql);
		
		if (! $possuiparecer)
		$sql = "insert into ata_acompanhamento (docid,usucpf,aacparecer,aacparecertexto) values ($cod,'".$_SESSION['usucpf']."','".$_REQUEST['lista']."','".$_REQUEST['aacparecertexto']."')";
		else 
		$sql = "update ata_acompanhamento set aacparecer='".$_REQUEST['lista']."', aacparecertexto='".$_REQUEST['aacparecertexto']."', aacdataparecer = now() where aacid=$possuiparecer";
        $db->executar($sql);
		$db->commit();
        ?>
        <script>
            alert ('Operação realizada com sucesso!');
            close();       
        </script>
        <?
	}
	if ($_REQUEST['act']=='fecharata')
	{
		$sql = "update ata_acompanhamento set aacsituacao='F', aacdataparecer = now() where docid=$cod";
        $db->executar($sql);
		$db->commit();
        ?>
        <script>
            alert ('Operação realizada com sucesso!');
            close();
        
        </script>
        <?
	}
	
	if ($_REQUEST['act']=='reabreata')
	{
		$sql = "update ata_acompanhamento set aacsituacao='A', aacdataparecer = now() where docid=$cod";
        $db->executar($sql);
		$db->commit();
        ?>
        <script>
            alert ('Operação realizada com sucesso!');
            close();
        
        </script>
        <?
	}	
	$coordenador = $db->testa_responsavel_projespec($_SESSION['pjeid']);

?>
<html>
<head>
<title>Parecer sobre ata de reunião</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>

</head>

<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="doc" value="<?=$cod?>">
<input type=hidden name="act" value="0">
<br>
 <center>
 <table>
<tr valign='top'><td ><input type="radio" value="C" name="lista" onclick="mostra_div('C')"
<?if ($lista=='C') print '   checked'?>><b>Concordo com a Ata</b></td>
<td><input type="radio" value="D" name="lista" onclick="mostra_div('D')" <?if ($lista=='D') print '   checked'?>><b>Discordo da Ata</b>
</tr></table>
  
<div style='display:none' id='discordar'> 

    <table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
     <tr>
	 <td align="Center" bgcolor="#dedede">Escreva sua justificativa</td>
      <td ><?=campo_textarea('aacparecertexto','N','S','','95%',8,'');?></td>
     </tr>
	   </table>
	 </div>	 
<div style='display:none' id='concordar'> 
	 </div>	 
	     <table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
	 <tr>
	 <td colspan="2" align="right" class="subtitulodireita">
	 <?
     if (! $atafechada )
     {?>
	 <input type='button' class="botao" value='Gravar parecer' onclick="grava_parecer(<?=$cod?>)">&nbsp;&nbsp;&nbsp;
	 <?}	 
	 if ($coordenador and  ! $atafechada) {?>
	 <input type='button' class="botao" value='Fechar a Ata!' onclick="fecha_ata(<?=$cod?>)">&nbsp;&nbsp;&nbsp;	 
	 <?}
	 if ($coordenador and  $atafechada) {?>
	 <input type='button' class="botao" value='Reabrir a Ata!' onclick="reabre_ata(<?=$cod?>)">&nbsp;&nbsp;&nbsp;	 
	 <?}?>
	 <input type='button' class="botao" value='Fechar esta janela' onclick="fechar_janela()">
	 </td>
	 </tr>
	   </table> 
	   <?
	   // lista os demais pareceres
	   $sql = "select u.usunome, case when aa.aacparecer = 'C' then 'Concorda' when aa.aacparecer='D' then 'Discorda' else 'Sem parecer' end as parecer,aa.aacparecertexto, to_char(aacdataparecer,'dd/mm/yyyy') as dataparecer from ata_acompanhamento aa inner join seguranca.usuario u on u.usucpf= aa.usucpf where aa.docid = $cod order by parecer";
	   $cabecalho = array( 'Participante', 'Parecer', 'Justificativa','Data' );
	   $db->monta_lista( $sql, $cabecalho, 50, 20, '', '' ,'' );
?>
	   
	   
<?
if ($_REQUEST['lista']=='C')
{
  ?>
<script>
   document.getElementById("concordar").style.visibility = "visible";
   document.getElementById("concordar").style.display = "";
</script>
<?}?>
<? 
if ($_REQUEST['lista']=='D')
{
  ?>
<script>
   document.getElementById("discordar").style.visibility = "visible";
   document.getElementById("discordar").style.display = "";
</script>
<?}?>
</form> 
<script>

  function fechar_janela()
  {
    window.close();
  }
  function grava_parecer(cod)
  {
  	if (! document.formulario.lista[0].checked && ! document.formulario.lista[1].checked) 
  	{
  		alert ('É necessário concordar ou discordar da Ata!');
  		return;
  	}
  	
  	if (document.formulario.lista[1].checked) 
  	{
   	     if (! validaBranco(document.formulario.aacparecertexto, 'Justificativa')) return;
  	}
  	document.formulario.doc.value=cod	;  	
  	document.formulario.act.value='incluir'	;
	document.formulario.submit();

  }
  
  function fecha_ata(cod)
  {
  	document.formulario.doc.value=cod	;  	
  	document.formulario.act.value='fecharata'	;
	document.formulario.submit();
  }  
  
  function reabre_ata(cod)
  {
  	document.formulario.doc.value=cod	;  	
  	document.formulario.act.value='reabreata'	;
	document.formulario.submit();
  }   
 
  
   function mostra_div(cod)
  {
    if (cod == 'C')
    {
		document.getElementById("concordar").style.visibility = "visible";
		document.getElementById("concordar").style.display = "";
		document.getElementById("discordar").style.visibility = "hidden";
	    document.getElementById("discordar").style.display = "none";
     }
    if (cod == 'D')
    {      	
		document.getElementById("discordar").style.visibility = "visible";
		document.getElementById("discordar").style.display = "";
		document.getElementById("concordar").style.visibility = "hidden";
	    document.getElementById("concordar").style.display = "none";
     }
  }

</script>
</body>
</html>
