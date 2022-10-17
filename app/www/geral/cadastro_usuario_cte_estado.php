<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   Módulo:seleciona_unid_perfilresp.php
  
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];

/*
*** INICIO REGISTRO RESPONSABILIDADES ***
*/ 	

if(is_array($_POST['usuunidresp']) && @count($_POST['usuunidresp'])>0) {
	$txtestadosComCoordenador = "";
	$confirmarestados = 0;
	$concluido = 0; // -1 erro, 0 nao concluido, 1 sucesso
	$estadosConfirmadas = $_REQUEST["estadosConfirmadas"];
	//SQL para verificar as responsabilidades
	$sqlSelResp = "SELECT distinct ur.rpuid, ur.usucpf, ur.rpustatus, u.estdescricao, u.estuf, u.estuf FROM cte.usuarioresponsabilidade ur 
		INNER JOIN territorios.estado u ON u.estuf = ur.estuf AND u.estuf = '%s'
		inner join seguranca.perfil pfl on pfl.pflcod=ur.pflcod and pfl.pflsncumulativo='f' and pfl.pflcod='".$pflcod."' 			
		WHERE ur.rpustatus = 'A' AND ur.usucpf <> '" . $usucpf . "'";
	

	$sqlSelestado	= "SELECT u.estuf, u.estuf FROM territorios.estado u WHERE u.estuf = '%s' ";
	
	$sqlInsRpu = "INSERT INTO cte.usuarioresponsabilidade (estuf, usucpf, rpustatus, rpudata_inc, pflcod,prsano) VALUES ('%s', '%s', '%s', '%s', '%s','".$_SESSION['exercicio']."')";

	$sqlUpdRpu = "UPDATE cte.usuarioresponsabilidade SET rpustatus = 'I' WHERE estuf = '%s' AND pflcod = ".$pflcod;

	$sqlUpdRpuUsu = "UPDATE cte.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '".$usucpf."' AND pflcod = ".$pflcod;

	//
	// verificar quais estados possuem outro com o mesm o perfil
	foreach ($_POST['usuunidresp'] as $respcod) {
		$sql='';
		$sql = vsprintf($sqlSelResp, $respcod);
		if ($sql<>"" && ($linhasRpu = @$db->carregar($sql))) {
			foreach ($linhasRpu as $rpu) {
				$confirmarestados = true;
				$txtestadosComCoordenador .= $rpu["estuf"] . ' - ' . $rpu["estdescricao"] . ' CPF: ' . $rpu["usucpf"] . '\n ';
			}
		}
	}
					
	// caso nao existam outros coordenadores de planejamento, registrar os itens selecionados
	if(!$confirmarestados) {
		$sql = $sqlUpdRpuUsu;
		$db->executar($sql);
		foreach ($_POST['usuunidresp'] as $respcod) {
		   $sql = "";
		   if ($respcod!=''){
		   $sql = vsprintf($sqlSelestado, $respcod);
		   $linha = $db->carregar($sql);
			if(is_array($linha) && count($linha)>=1) {
				foreach ($linha as $estado) {
					$estuf = $estado["estuf"]; 		
					$dados = array($estuf, $usucpf, 'A', date("Y-m-d H:i:s"), $pflcod); 						$sql = vsprintf($sqlInsRpu, $dados);
					$db->executar($sql);
				}
				
			}}
		}
		$concluido = 1;
	}
	//
	// verificar se foi confirmado a substituição do coordenador atual pelo
	// usuario que está sendo liberado e/ou alterado
	else
	if($estadosConfirmadas) {
		if (is_array($_REQUEST['usuunidresp'])){
		
		foreach ($_REQUEST['usuunidresp'] as $rpu) {
			$sql = sprintf($sqlUpdRpu, $rpu);
			$db->executar($sql);
			$dados = array($rpu, $usucpf, 'A', date("Y-m-d H:i:s"), $pflcod);
			$sql = vsprintf($sqlInsRpu, $dados);
			$db->executar($sql);
		}
		$concluido = 1;	
	}	
	}
	//
	// exibir a tela de aviso dos itens que já possuem coordenador e confirmar
	// a substituição pelo usuario que está sendo liberado e/ou alterado
	else {
		$msg = 'Existem usuários ativos com o perfil selecionado para estas estados:\n\n';
		$msg .= $txtestadosComCoordenador;
		$msg .= '\nDeseja sobrescrevê-los?\n\n';
		$msg .= 'Ao confirmar, o perfil dos usuários atuais (listados acima) será desativado.';
		?>
		<html>
		<body>
		<form name="formassocia" style="margin:0px;" method="POST">
		<input type="hidden" name="usucpf" value="<?=$usucpf?>">
		<input type="hidden" name="pflcod" value="<?=$pflcod?>">
		<input type="hidden" name="estadosConfirmadas" value="1">
		<?
			foreach ($_POST['usuunidresp'] as $respcod) {
				?><input type="hidden" name="usuunidresp[]" value="<?=$respcod?>"><?
			}
		?>
		</form>
		</body>
		</html>
		
		<script>
			if (confirm("<?=$msg?>")) {		
				document.formassocia.submit();
			} else
			{
				self.close();		
			}
		</script>

		<?
		exit(0);
	}

	if ($concluido>0) {
		$db->commit();
		?>
	<script>
	window.parent.opener.location.reload();self.close();
	</script>
		<?
		exit(0);
	}
}
/*
*** FIM REGISTRO RESPONSABILIDADES ***
*/
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>estados</title>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde"><img src="../imagens/icon-aguarde.gif" border="0" align="absmiddle"> <font color=blue size="2">Aguarde! Carregando Dados...</font></div>
<?flush();?>
<DIV style="OVERFLOW:AUTO; WIDTH:496px; HEIGHT:350px; BORDER:2px SOLID #ECECEC; background-color: White;">
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
<script language="JavaScript">
document.getElementById('tabela').style.visibility = "hidden";
document.getElementById('tabela').style.display  = "none";
</script>
<form name="formulario">
<thead><tr>
<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione a(s) estado(s)</strong></td>
</tr>
<tr>
<?

	  $cabecalho = 'Selecione a(s) estado(s)';
	  $sql = "
			select
				estuf, estuf, estdescricao
			from territorios.estado
			order by estuf, estdescricao
";
	  $RS = @$db->carregar($sql);
	  $nlinhas = count($RS)-1;
	  for ($i=0; $i<=$nlinhas;$i++)
		 {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
	   ?>
	   		
		   		<tr bgcolor="<?=$cor?>">
				<td align="right"><input type="Checkbox" name="estuf" id="<?=$estuf?>" value="<?=$estuf?>" onclick="retorna(<?=$i?>);"><input type="Hidden" name="estdescricao" value="<?=$estuf.' - '.$estdescricao?>"></td>
				<td align="right" style="color:blue;"><?=$estuf?></td>
				<td><?=$estdescricao?></td>
				</tr>
	   
	   <?}
?>
</form>
</table>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<select multiple size="8" name="usuunidresp[]" id="usuunidresp" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
<?
$sql = "select distinct u.estuf as codigo, u.estuf||' - '||u.estdescricao as descricao from cte.usuarioresponsabilidade ur inner join territorios.estado u on ur.estuf=u.estuf where ur.rpustatus='A' and ur.usucpf = '$usucpf' and ur.pflcod=$pflcod";
$RS = @$db->carregar($sql);
if(is_array($RS)) {
	$nlinhas = count($RS)-1;
	if ($nlinhas>=0) {
		for ($i=0; $i<=$nlinhas;$i++) {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
    		print " <option value=\"$codigo\">$descricao</option>";		
		}
	}
}
else{
	$sql = "select distinct u.estuf as codigo, u.estdescricao as descricao from territorios.estado u inner join cte.unidproposto up on u.estuf=up.estuf and up.usucpf='".$usucpf."' where up.estuf is not null";
	$RS = @$db->carregar($sql);
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
				print " <option value=\"$codigo\">$codigo - $descricao</option>";
			}
		}
} else {?>
<option value="">Clique na estado selecionar.</option>
<?	}
}?>
</select>
</form>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
<tr bgcolor="#c0c0c0">
<td align="right" style="padding:3px;" colspan="3">
<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
</td></tr>
</table>
<script language="JavaScript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";


var campoSelect = document.getElementById("usuunidresp");


if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++)
		{document.getElementById(campoSelect.options[i].value).checked = true;}
}



function abreconteudo(objeto)
{
if (document.getElementById('img'+objeto).name=='+')
	{
	document.getElementById('img'+objeto).name='-';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
	document.getElementById(objeto).style.visibility = "visible";
	document.getElementById(objeto).style.display  = "";
	}
	else
	{
	document.getElementById('img'+objeto).name='+';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
	document.getElementById(objeto).style.visibility = "hidden";
	document.getElementById(objeto).style.display  = "none";
	}
}



function retorna(objeto)
{

	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
	if (document.formulario.estuf[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.estdescricao[objeto].value, document.formulario.estuf[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.estuf[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na estado.', '', false, false);}
			sortSelect(campoSelect);
	}
}

function moveto(obj) {
	if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();}
}



</script>