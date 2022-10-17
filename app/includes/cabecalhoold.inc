<?
/*
   Sistema   Simec
   Setor responsável: SPO - MEC
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Mdulo: cabecalho.inc
   Finalidade:Cabeçalho do Sistema
   */


$cabecalho_sistema=true;
	if ($_REQUEST['exercicio']) {
		if ($_SESSION['exercicio'] <> $_REQUEST['exercicio'])
		{
			$_SESSION['exercicio'] = $_REQUEST['exercicio'];  
		    $exercicio=$_SESSION['exercicio'];
		    $sql= "select prsexercicioaberto from ".$_SESSION['sisdiretorio'].".programacaoexercicio where prsano='".$_REQUEST['exercicio']."'" ;
		    $res = $db->pegaLinha($sql,0);
		    $_SESSION['exercicioaberto']= $res['prsexercicioaberto'];
		    		$db->checagem_geral();
		}

	}
	

//
// sistemas que o usuario pode acessar
$sql = "";
		
?>
<html>
<head>
<!-- 
/**************************************
   Sistema INFRASIG - Cliente: MEC
   SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação
   Desenvolvido por: Cristiano Cabral (cristiano.cabral@gmail.com), Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Finalidade: Permitir o Monitoramento Físico e Financeiro e a Avaliação das Ações e Programas do PPA integrado com o SIGPLAN
*****************************************/
-->


<META Http-Equiv="Cache-Control" Content="no-cache">
<META Http-Equiv="Pragma" Content="no-cache">
<META Http-Equiv="Expires" Content="-1">
<center>
<DIV id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300;font-size:12px;"><br><img src="../imagens/wait.gif" border="0" align="middle"> Aguarde! Carregando Dados...<br><br></DIV>
</center>
<?flush();?>
<tible><?= $GLOBALS['parametros_sistema_tela']['sigla-nome_completo']; ?></title>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<?include (APPRAIZ."includes/gera_menu.inc");?>
</head>
<body LEFTMARGIN="0" TOPMARGIN="0" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<table width="100%" border="0" cellpadding="0" cellspacing="0" height="100%"><tr><td align="left" valign="top" colspan="2"  background="/imagens/fundo.gif" height="100%">
<table width="100%" border="0" cellpadding="2" cellspacing="0" class="notprint" style="border-bottom:2px solid #8B8B8B;">
  <tr bgcolor="#003A5D"> 
  <form name="setperfil" action=<?=$_SESSION['sisdiretorio']?>.php?modulo=inicio&acao=C" method="post">
  <td nowrap> <a href="<?=$_SESSION['sisdiretorio']?>.php?modulo=inicio&acao=C" style="color:#ffffff;margin-left:10px;margin-bottom:5px;"> Página Inicial </a></td>
	<?

	if ($_SESSION['superuser'] or $db->testa_uma() or $_SESSION['usuuma'])
    {
 if ($_SESSION['superuser'])
    $sql = "select distinct u.usucpf as codigo, u.usunome as descricao from seguranca.usuario u inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf inner join perfil p on p.pflcod = pf.pflcod and p.sisid =".$_SESSION['sisid']." where usustatus='A' order by 2";
else
	$sql = "select distinct u.usucpf as codigo, u.usunome as descricao from seguranca.usuario u
where usustatus='A' and u.usucpf not in (select pu.usucpf from seguranca.perfilusuario pu, seguranca.perfil p where pu.pflcod=p.pflcod and p.pflsuperuser = 't' and p.sisid=".$_SESSION['sisid'].") order by 2";
//dbg($sql,1);
	$usucpf_simu = $_SESSION['usucpf'];
    $_SESSION['usuuma']=1;
	?>
	<td nowrap>
	<font color="#FFFFFF">Usuário: <?$db->monta_combo("usucpf_simu",$sql,'S',"Selecione o Usuário",'setpfl','');?></font>
	</td>
	<?}
    else
    {?>
	<td nowrap><font color="#FFFFFF">Usuário: <strong><?=$_SESSION['usunome'];?></strong> Órgão: <?=$_SESSION['usuorgao'];?></font></td>
	<?}
//ok 
	if($_SESSION["sisexercicio"]=='t') {
?>	
	<? if ($_SESSION['exercicio']) $exercicio=$_SESSION['exercicio']; 
	//$sql = "select prsano as codigo, prsano as descricao from ".$_SESSION['sisdiretorio'].".programacaosimec order by 1";
	$sql = "select prsano as codigo, prsano as descricao,prsexercicioaberto from ".$_SESSION['sisdiretorio'].".programacaoexercicio order by 1";	
	if ($_SESSION['sisdiretorio']=='monitora') $exerc='Exerccio';
	else $exerc = 'Exercício base de comparação';
?>
	<td nowrap align="right"><font color="#FFFFFF"><?=$exerc?>: <?$db->monta_combo('exercicio',$sql,'S','','setpfl','');?></font></td>
<? } ?>
	<td align="center"><img src="/imagens/barra_top.gif" border="0" align="middle" style="margin:0px;" usemap="#Map">
		<map name="Map">
		  <area title="Imprimir" shape="rect" coords="0,-1,22,29" href="javascript:self.print();">
		  <area title="Ajuda do Sistema" shape="rect" coords="21,-1,45,27" href="javascript:DoPopup('<?=$_SESSION['sisdiretorio']?>.php?modulo=ajuda/ajuda&acao=C&mnuid=<?=$mnuid?>','helpsimec',440,550);">
		  <area title="Adicionar a Favoritos" shape="rect" coords="44,-2,68,33" href="javascript:DoPopup('<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/favorito/cadfavorito&acao=A','favorito',440,400);">
		  <area title="Sair do Sistema" shape="rect" coords="67,-3,100,55" href="/logout.php">
		</map>
	</td>
	</form>
  </tr>
</table>
<script language="JavaScript">function setpfl(){document.setperfil.submit();}</script>
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen"  style="border-bottom: 1px solid;">
  <tr bgcolor="#ffffff"> 
    <td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td><td height="20" nowrap>SIMEC - Sistema Integrado de Monitoramento do MEC<br>
	Ministério da Educação / SE - Secretaria Executiva<br>
	SPO - Subsecretaria de Planejamento e Orçamento<br>	
	</td>
	<td height="20" align="right">Impresso por: <strong><?=$_SESSION['usunome'];?></strong><br>rgo: <?=$_SESSION['usuorgao'];?><br>
	Hora da Impressão: <?=date("d/m/Y - H:i:s")?>
	</td>
	<tr bgcolor="#ffffff"> 
    <td colspan="2">&nbsp;</td></tr>	
</table>
<table background="/imagens/fundo_top.gif" width="100%" border="0" cellpadding="0" cellspacing="0" class="notprint">
  <tr> 
    <td style="color:#000000;font-family:Arial;font-size:12pt;">&nbsp;<b><?=$_SESSION['sisabrev']?></b><br><span style="color:#555555;font-family:Arial;font-size:8pt;">&nbsp;<?=$_SESSION['sisdsc']?></span></td>
    <td valign="bottom" align="right"><?
    $sql = "select distinct s.sisid, s.sisabrev, s.sisdsc, s.sisfinalidade, s.sispublico, s.sisrelacionado from seguranca.sistema s INNER JOIN seguranca.perfil p ON s.sisid = p.sisid INNER JOIN seguranca.perfilusuario pf ON p.pflcod = pf.pflcod AND pf.usucpf = '" . $_SESSION["usucpforigem"] . "' where s.sisstatus='A' order by s.sisid";
	$RS = $db->record_set($sql);
	$nlinhas = $db->conta_linhas($RS);
	if($nlinhas > 0) {
	?><table cellpadding="0" cellspacing="0"><tr><?				  
	  for ($i=0; $i<=$nlinhas;$i++){
		$res = $db->carrega_registro($RS,$i);
	  		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
			if ($sisid == $_SESSION['sisid']){?><td style="padding-top: 2px; padding-left:5px; padding-right:5px; background-color: #3B5D82; color:#ffffff; border-top: 1px solid #555555; border-right: 1px solid #555555; border-left: 1px solid #555555; font-weight: bolder;"><?} else {?><td style="padding-top: 2px; padding-left:5px; padding-right:5px; background-color: #ffffff; border-top: 1px solid #555555; border-right: 1px solid #555555; border-left: 1px solid #555555;" onmouseover="this.style.color='#ffff00';this.getElementsByTagName('bgcolor').value='#000000';" onmouseout="this.style.color='#000000';"><?}?>
			<?=$sisabrev?> </td> <td width="2"></td>
			<?}?>
	</tr></table><?}?></td>
	</tr>
    <tr bgcolor="#3B5D82"> 
    <td height="20" colspan="2"></td>
	</tr>
	<tr bgcolor="#C0C0C0"> 
    <td height="2" colspan="2"></td>
	</tr>	
</table>
<table width="100%" border="0" cellpadding="0" cellspacing="0">
<?
//Montar Migalha de po
if ($modulo_atual<>"") {
$sql = "select distinct mnu1.mnudsc  || '  ' || mnu2.mnudsc || ' »» ' || mnu3.mnudsc as migalha from seguranca.menu mnu1, seguranca.menu mnu2,  seguranca.menu mnu3 where mnu3.mnuidpai=mnu2.mnuid and mnu2.mnuidpai=mnu1.mnuid and mnu3.mnuid=".$modulo_atual;
$saida = $db->recuperar($sql);
if ($saida['migalha']) $migalha = $saida['migalha'];}?>
<tr><td align="right" height="17" valign="middle" bgcolor="#e0e0e0">
<font color="#696969" style="margin-right:10px;"><?if ($migalha) print "<b>Você está aqui:</b> ".$migalha; else print "&nbsp;"?></font>
</td></tr>
<?//Fim Migalha?>
  <tr> 
    <td>
    <script>
function selecionasistema()
	{
	document.formulario.submit()
  }    

</script>