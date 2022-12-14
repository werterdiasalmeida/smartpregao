<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:cadastro_usuario_elaboracao_acao.php
   
   */

// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];

/*
*** INICIO REGISTRO RESPONSABILIDADES ***
*/
if(is_array($_POST['usuacaresp']) && @count($_POST['usuacaresp'])>0) {
	$txtAcoesComCoordenador = "";
	$confirmarAcoes = false;
	$concluido = 0; // -1 erro, 0 nao concluido, 1 sucesso
	$acoesConfirmadas = (bool)$_REQUEST["acoesConfirmadas"];

	$sqlSelPerfil = "SELECT pflsncumulativo FROM seguranca.perfil WHERE pflcod = " . $pflcod;
	$rsPerfil = $db->carregar($sqlSelPerfil);
	$pflsncumulativo = $rsPerfil[0]["pflsncumulativo"] == 't' ? true : false;
	
	$sqlSelResp = "SELECT ur.rpuid, ur.usucpf, ur.rpustatus, a.acadsc, a.acaid, u.usunome FROM elabrev.usuarioresponsabilidade ur 
		INNER JOIN elabrev.ppaacao_proposta a ON a.acaid = ur.acaid
			AND a.prgcod = '%s' AND a.acacod = '%s' and a.acastatus='A' 
		INNER JOIN seguranca.usuario u on ur.usucpf=u.usucpf 
		WHERE ur.rpustatus = 'A' AND ur.usucpf <> '".$usucpf."' AND ur.pflcod = ".$pflcod;

	$sqlSelAcao	= "SELECT a.acaid FROM elabrev.ppaacao_proposta a 
		WHERE a.prgcod = '%s' AND a.acacod = '%s' and a.acastatus='A' 
			 AND a.prsano='".$_SESSION['exercicio']."'";

	$sqlInsRpu = "INSERT INTO elabrev.usuarioresponsabilidade (acaid, usucpf, rpustatus, rpudata_inc, pflcod,prsano) VALUES ('%s', '%s', '%s', '%s', '%s','".$_SESSION['exercicio']."')";

	$sqlUpdRpu = "UPDATE elabrev.usuarioresponsabilidade SET rpustatus = 'I' WHERE acaid = '%s' AND pflcod = ".$pflcod;

	$sqlUpdRpuUsu = "UPDATE elabrev.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '".$usucpf."' AND pflcod = ".$pflcod;
	//
	// verificar quais itens possuem outro coordenador ativo
	if(!$pflsncumulativo && $_POST['usuacaresp'][0]!="") {
		foreach ($_POST['usuacaresp'] as $respcod) {
			$sql = "";
			$arrCodigoAcao = explode(".", $respcod);
			$sql = vsprintf($sqlSelResp, $arrCodigoAcao);
			if ($sql<>"" && ($linhasRpu = $db->carregar($sql))) {
				foreach ($linhasRpu as $rpu) {
					$confirmarAcoes = true;
					$txtAcoesComCoordenador .= $respcod . " - " . $rpu["acadsc"] . " - Nome: ".$rpu['usunome']." - CPF: " . $rpu["usucpf"] . '\n';
				}
			}
		}
	}

	//
	// caso nao existam outros coordenadores, registrar os itens selecionados
	if(!$confirmarAcoes || $acoesConfirmadas) {
		$db->executar($sqlUpdRpuUsu);
		if($_POST['usuacaresp'][0]!="") {
			foreach ($_POST['usuacaresp'] as $respcod) {
				$sql = "";
				$arrCodigoAcao = explode(".", $respcod);
				$sql = vsprintf($sqlSelAcao, $arrCodigoAcao);
				
				$linha = $db->carregar($sql);
	//dbg($sql);dbg($linha);
				if(is_array($linha) && count($linha)>=1) {
					foreach ($linha as $acao) {
						$acaid = $acao["acaid"];
						// no caso de um perfil cumulativo, năo desativa os usuarios atuais
						if(!$pflsncumulativo) {
							$sql = sprintf($sqlUpdRpu, $acaid);
							$db->executar($sql);
						}
						
						$dados = array($acaid, $usucpf, 'A', date("Y-m-d H:i:s"), $pflcod);		
						$sql = vsprintf($sqlInsRpu, $dados);
						$db->executar($sql);
					}
				}
			}
		}
		$concluido = 1;
		//$db->rollback();
		//dbg(1,1);
	}
	//
	// exibir a tela de aviso dos itens que já possuem coordenador e confirmar
	// a substituiçăo pelo usuario que está sendo liberado e/ou alterado
	else {
		$msg = 'Existem usuários ativos com o perfil selecionado para estas açőes:\n\n';
		$msg .= $txtAcoesComCoordenador;
		$msg .= '\nDeseja sobrescrevę-los?\n\n';
		$msg .= 'Ao confirmar, o perfil dos usuários atuais (listados acima) será desativado.';
		?>
		<body>
		<form name="formassocia" style="margin:0px;" method="POST">
		<input type="hidden" name="usucpf" value="<?=$usucpf?>">
		<input type="hidden" name="pflcod" value="<?=$pflcod?>">
		<input type="hidden" name="acoesConfirmadas" value="1">
		<?
			foreach ($_POST['usuacaresp'] as $respcod) {
				?><input type="hidden" name="usuacaresp[]" value="<?=$respcod?>"><?	
			}
		?>
		</form>
		<script>
			if (confirm("<?=$msg?>")) {
				document.formassocia.submit();
			}
			else
			{
				self.close();			
			}
		</script>
		</body>
		<?
		exit(0);
	}
	
	if ($concluido>0) {
		$db->commit();
		?>
		<script language="javascript">
			alert("Operaçăo realizada com sucesso!");
			opener.location.reload();
			self.close();
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
<title>Atribuir Açőes</title>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff" onload="self.focus()">
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
<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Clique no Programa para selecionar as Açőes</strong></td>
</tr>
<tr>
<?
	  $sql = "select a.prgcod, p.prgdsc, a.acacod, a.acadsc, p.prgid, count(*) as totalaca from elabrev.ppaacao_proposta a inner join elabrev.ppaprograma_proposta p on a.prgid = p.prgid where a.acastatus='A' and p.prsano ='". $_SESSION['exercicio'] ."' group by a.prgcod, p.prgdsc, a.acacod, a.acadsc, p.prgid order by a.prgcod, a.acacod, a.acadsc";
	  $RS = $db->carregar($sql);
	  $nlinhas = count($RS)-1;
	  for ($i=0; $i<=$nlinhas;$i++)
		 {
		 	extract( $RS[$i] );
			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
			if ($v_prgcod<>$prgcod) {
				if ($corp == '#e0e0e0') $corp = '#f4f4f4' ; else $corp='#e0e0e0';
				if ($v_prgcod) {?>
			 </table>
	  			 </td></tr>
			   <script language="JavaScript">
				   document.getElementById('<?=$v_prgcod?>').style.visibility = "hidden";
				   document.getElementById('<?=$v_prgcod?>').style.display  = "none";
			   </script>
				<?}?>
	   		<tr bgcolor="<?=$corp?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$corp?>';">
				<td align="left" onclick="abreconteudo('<?=$prgcod?>');document.getElementById('ok').focus();"><img src="../imagens/mais.gif" border="0" width="9" height="9" align="absmiddle" vspace="3" id="img<?=$prgcod?>" name="+">&nbsp;&nbsp;<font color="#0000ff"><?=$prgcod?></font> - <?=$prgdsc?></td>
			</tr>
			<tr id="<?=$prgcod?>"><td>
			   <table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" >
	   <?$v_prgcod=$prgcod;}?>
<tr bgcolor="<?=$cor?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor?>';"><td align="left" nowrap style="color:#006666;"> <input type="Checkbox" name="prgid" id="<?=$prgcod.'.'.$acacod?>" value="<?=$prgcod.'.'.$acacod?>" onclick="retorna(<?=$i?>);"><input type="Hidden" name="prgdsc" value="<?=$prgcod.'.'.$acacod?> - <?=$acadsc?>"><?=$acacod?></td><td style="color:#666666;"><font color="#333333"><?=$acadsc?></font> (<?=$unidsc?>)</td><td align="right" style="color:#666666;">(<?=$totalaca?>)</td></tr><?}?>
<script language="JavaScript">
				   document.getElementById('<?=$v_prgcod?>').style.visibility = "hidden";
				   document.getElementById('<?=$v_prgcod?>').style.display  = "none";
</script>
 </table>
</td></tr>
</form>
</table>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<select multiple size="8" name="usuacaresp[]" id="usuacaresp" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
<?
$sql = "select distinct a.prgcod||'.'||a.acacod as codigo, a.acadsc as descricao from elabrev.usuarioresponsabilidade u inner join elabrev.ppaacao_proposta a on u.acaid=a.acaid where rpustatus='A' and u.usucpf = '$usucpf' and u.pflcod=$pflcod and u.prsano='".$_SESSION['exercicio']."'";

$RS = $db->carregar($sql);
if(is_array($RS)) {
	$nlinhas = count($RS)-1;
	if ($nlinhas>=0) {
		for ($i=0; $i<=$nlinhas;$i++) {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
    		print " <option value=\"$codigo\">$codigo - $descricao</option>";		
		}
	}
}
else
{
	$sql = "select distinct a.prgcod||'.'||a.acacod as codigo, a.acadsc as descricao from elabrev.ppaacao_proposta a inner join elabrev.progacaoproposto p on a.acacod=p.acacod and a.prgid=p.prgid and p.usucpf='".$usucpf."' where p.acacod is not null";
	$RS = $db->carregar($sql);
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
				print " <option value=\"$codigo\">$codigo - $descricao</option>";
			}
		}
} else {?>
<option value="">Clique no Programa para selecionar as Açőes.</option>
<?	}
}

//dbg($sql,1);
?>
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


var campoSelect = document.getElementById("usuacaresp");
if (campoSelect.options[0].value != ''){
	v_prg=0;
	for(var i=0; i<campoSelect.options.length; i++)
		{ 	
			document.getElementById(campoSelect.options[i].value).checked = true;
			
			if (v_prg!=campoSelect.options[i].value.slice(0,campoSelect.options[i].value.indexOf('.')))
				{ 
					v_prg = campoSelect.options[i].value.slice(0,campoSelect.options[i].value.indexOf('.'));
					abreconteudo(campoSelect.options[i].value.slice(0,campoSelect.options[i].value.indexOf('.')));
					document.getElementById('ok').focus();
				}
		}
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
	if (document.formulario.prgid[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.prgdsc[objeto].value, document.formulario.prgid[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.prgid[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique no Programa para selecionar as Açőes.', '', false, false);}
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