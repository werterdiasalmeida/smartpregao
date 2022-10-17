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
$pflcod = (int)$_REQUEST['pflcod'];

/*
 *** INICIO REGISTRO RESPONSABILIDADES ***
 */

if(isset($_REQUEST['enviar'])) {



	$sql = "update cte.usuarioresponsabilidade set rpustatus = 'I' where usucpf = '$usucpf' and pflcod = $pflcod";
	$db->executar($sql);


	if(isset($_POST['usuunidresp'])){
		foreach($_POST['usuunidresp'] as $muncod) {
				
			$sql = "";

			$sql =
		   "update 
		   		cte.usuarioresponsabilidade 
		   set  
		   		rpustatus 	= 'A',
		   		rpudata_inc = now()
		   	where
		   		usucpf  	= '$usucpf' and
		   		pflcod 	    =  $pflcod and
		   		muncod  	= '$muncod'
		   ";
				
			$sqlQtd = "
				select count(*) as qtd from cte.usuarioresponsabilidade 
				where
				   		usucpf  	= '$usucpf' and
				   		pflcod 	    =  $pflcod and
				   		muncod  	= '$muncod'
				";

			$qtd = $db->pegaUm($sqlQtd);
				
			if($qtd == 0)
			{
				$sql = "";

				$sql = "insert into
				   		cte.usuarioresponsabilidade 
					   (
					        rpustatus 	,
					   		rpudata_inc ,					   		
					   		muncod  	,
					   		usucpf  	,
					   		pflcod 	
					   	)
					   		values
					   	(
					   		'A'			,
					   		now()		,					   		
					   		'$muncod'	,
					   		'$usucpf'	,
					   		$pflcod
						)					 			   			
				   ";					   						 
			}
			$db->executar($sql);
		}
	}
	$db->commit();

	?>
<script>
	
	window.parent.opener.location.reload();self.close();
	
</script>
	<?
	exit(0);
}

/*
 *** FIM REGISTRO RESPONSABILIDADES ***
 */
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Estados e Municípios</title>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css'
	href='../../includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0"
	MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde"><img src="../imagens/icon-aguarde.gif"
	border="0" align="absmiddle"> <font color=blue size="2">Aguarde!
Carregando Dados...</font></div>
<?flush();?>
<DIV
	style="OVERFLOW: AUTO; WIDTH: 496px; HEIGHT: 350px; BORDER: 2px SOLID #ECECEC; background-color: White;">
<table width="100%" align="center" border="0" cellspacing="0"
	cellpadding="2" class="listagem" id="tabela">
	<script language="JavaScript">
document.getElementById('tabela').style.visibility = "hidden";
document.getElementById('tabela').style.display  = "none";
</script>
	<form name="formulario" method="post" action="">
	
	
	<thead>
		<tr>
			<td valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				colspan="3"><strong>Selecione a(s) estado(s)</strong></td>

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
		$j = 0 ;
		for ($i=0; $i<=$nlinhas;$i++)
		{
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
			?>
		
		
		<tr bgcolor="<?=$cor?>">
			<td width="20" align="right"><img src="../imagens/mais.gif"
				id="<?=$estuf."_img" ?>" onclick="mostraEsconde('<?=$estuf?>')">&nbsp;</td>
			<td align="left" style="color: blue;"><?=$estuf . ' - ' . $estdescricao?></td>
		</tr>
		<tr>
			<td style="height: 0"></td>
			<td style="height: 0">
			<div id="<?=$estuf?>" style="display: none;">
			<table width="100%" cellpadding="0" cellspacing="0" border="0">


			<?
			$sql = "select mundescricao, muncod from territorios.municipio where estuf = '$estuf' order by mundescricao";
			$municipios = $db->carregar($sql);
			foreach ($municipios as $municipio) {
				if ($cor2 == '#e0e0e0') $cor2 = '#f4f4f4' ; else $cor2='#e0e0e0';
				?>
				<tr bgcolor="<?=$cor2?>">
					<td align="left" style="border: 0"><input type="checkbox"
						name="muncod" id="<?=$municipio['muncod']?>"
						value="<?=$municipio['muncod']?>"
						onclick="retorna( this, '<?= $municipio['muncod'] ?>', '<?= $estuf.' - '. addslashes( $municipio['mundescricao'] ) ?>' );" />
						<?=$municipio['mundescricao']?></td>
				</tr>
				<?
}
?>
			</table>
			</div>
			</td>
		</tr>
		<?}
		?>
		</form>

</table>
</div>
<form name="formassocia" style="margin: 0px;" method="POST"><input
	type="hidden" name="usucpf" value="<?=$usucpf?>"> <input type="hidden"
	name="pflcod" value="<?=$pflcod?>"> <input type="hidden" name="enviar"
	value=""> <select multiple size="8" onclick="mostraMunicipio(this);"
	name="usuunidresp[]" id="usuunidresp" style="width: 500px;"
	class="CampoEstilo">
	<?
	$sql = "
			select 
				distinct m.muncod as codigo, m.estuf||' - '||m.mundescricao as descricao 
			from 
				cte.usuarioresponsabilidade ur inner join territorios.municipio m on ur.muncod = m.muncod
	 		where ur.rpustatus='A' and ur.usucpf = '$usucpf' and ur.pflcod=$pflcod";
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

	?>
</select></form>
<table width="100%" align="center" border="0" cellspacing="0"
	cellpadding="2" class="listagem">
	<tr bgcolor="#c0c0c0">
		<td align="right" style="padding: 3px;" colspan="3"><input
			type="Button" name="ok" value="OK"
			onclick="selectAllOptions(campoSelect);enviarFormulario();" id="ok"></td>
	</tr>
</table>
<script language="JavaScript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";


function mostraEsconde(estado){
	var estadoAtual = document.getElementById(estado).style.display;
	var objImagem = document.getElementById(estado+'_img');
	if(estadoAtual == 'none'){
		document.getElementById(estado).style.display = 'block';
		
		objImagem.src = '../imagens/menos.gif';
		
	}else{
		document.getElementById(estado).style.display = 'none';
		objImagem.src = '../imagens/mais.gif';
	}
	
}


var campoSelect = document.getElementById("usuunidresp");


if (campoSelect.options[0] && campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++)
		{document.getElementById(campoSelect.options[i].value).checked = true;}
}


function enviarFormulario(){
	document.formassocia.enviar.value=1;
	document.formassocia.submit();

}


function mostraMunicipio(objSelect){
	for( var i = 0; i < objSelect.options.length; i++ )
	{
		if ( objSelect.options[i].value == objSelect.value )
		{
			var estado = objSelect.options[i].innerHTML.substring(0,2);
			break;
		}
	}
	var estadoAtual = document.getElementById(estado).style.display;
	if(estadoAtual != 'block'){
		 mostraEsconde(estado);
	}
	document.getElementById(objSelect.value).focus();
		
}


function retorna( check, muncod, mundescricao )
{
	if ( check.checked )
	{
		// põe
		campoSelect.options[campoSelect.options.length] = new Option( mundescricao, muncod, false, false );
	}
	else
	{
		// tira
		for( var i = 0; i < campoSelect.options.length; i++ )
		{
			if ( campoSelect.options[i].value == muncod )
			{
				campoSelect.options[i] = null;
			}
		}
	}
	sortSelect( campoSelect );
}




</script>