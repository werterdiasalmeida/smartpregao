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

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$acao   = $_REQUEST["acao"];

if ($acao == "A"){
	$muncod = $_REQUEST["munresp"];
	atribuiMunicipio($usucpf, $pflcod, $muncod);
}

/**
 * Função que lista as uf's
 *
 */
function listaMunicipios(){
	$db = new cls_banco();
	
	// SQL para buscar estados existentes
	$estadosExistentes = $db->carregar(
								"SELECT
									estuf, estdescricao
								 FROM 
								 	territorios.estado
								ORDER BY 
									estuf, estdescricao");
	
	$count = count($estadosExistentes);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		$estuf     = $estadosExistentes[$i]["estuf"];
		$descricao = $estadosExistentes[$i]["estdescricao"];
		
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		echo "
			<tr bgcolor=\"".$cor."\">
				<td width=\"20\" align=\"right\"><img src=\"../imagens/mais.gif\"
					id=".$estuf."_img\" onclick=\"mostraEsconde('".$estuf."')\">&nbsp;</td>
				<td align=\"left\" style=\"color: blue;\">".$estuf . " - " . $descricao."</td>
			</tr>
			<tr>
				<td style=\"height: 0\"></td>
				<td style=\"height: 0\">
					<div id=\"".$estuf."\" style=\"display: none;\">
						<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" border=\"0\">";
		
		$municipios = $db->carregar("
							SELECT 
								mundescricao, muncod 
							FROM 
								territorios.municipio 
							WHERE 
								estuf = '{$estuf}' 
							ORDER BY 
								mundescricao");
		
		foreach ($municipios as $municipio) {
			if ($cor2 == '#e0e0e0'){ 
				$cor2 = '#f4f4f4';
			} else{ 
				$cor2='#e0e0e0';
			}
			
			echo '
				<tr bgcolor="'.$cor2.'">
					<td align="left" style="border: 0">
						<input type="checkbox" name="muncod" id="'.$municipio['muncod'].'" value="'.$municipio['muncod'].'" 
						onclick="retorna( this, \''.$municipio['muncod'].'\', \''.$estuf.' - '.addslashes($municipio['mundescricao']).'\');"/>
						'.$municipio['mundescricao'].'
					</td>
				</tr>';
						
		}
		echo '</table></div></td></tr>';
	}
}

/**
 * Função que atribui a responsabilidade de uma uf ao usuário
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $estuf
 */
function atribuiMunicipio($usucpf, $pflcod, $muncod){
	
	$db = new cls_banco();
	
	$data = date("Y-m-d H:i:s");
	
	if (is_array($muncod)){
		
		$count = count($muncod);
		
		if ($count == 1){
			
			$sql_atualiza = $db->executar("
									UPDATE 
										obras.usuarioresponsabilidade 
									SET 
										rpustatus = 'I' 
									WHERE 
										usucpf = '{$usucpf}' AND 
										pflcod = '{$pflcod}' AND 
										prsano = '{$_SESSION["exercicio"]}' AND 
										orgcod is not null AND
										muncod != '{$muncod[0]}' AND
										unicod is null");
			
		}else {
		
			for ($i = 0; $i < $count; $i++){
				$municipio = $muncod[$i];
				$orgao = $db->pegaUm("
							SELECT DISTINCT
								orgcod 
							FROM
								obras.usuarioresponsabilidade 
							WHERE
								orgcod is not null AND
								pflcod = '{$pflcod}' AND
								usucpf = '{$usucpf}' AND
								estuf = '{$uf}' AND
								muncod = '{$municipio}' AND
								rpustatus = 'A'");
			
				if ($orgao){
					continue;
				}else{
					$orgcod = $db->pegaUm("
										SELECT DISTINCT
											orgcod
										FROM
											obras.usuarioresponsabilidade
										WHERE
											orgcod is not null AND
											pflcod = '{$pflcod}' AND
											usucpf = '{$usucpf}' AND
											rpustatus = 'A'");
					if ($orgcod){
						$sql_insere = $db->executar("
												INSERT INTO
													obras.usuarioresponsabilidade
													(muncod, orgcod, usucpf, rpustatus, rpudata_inc, pflcod, prsano)
												VALUES
													('{$municipio}', '{$orgcod}', '{$usucpf}', 'A', '{$data}', '{$pflcod}', '{$_SESSION["exercicio"]}')");
					}else {
						echo "
							<script>
								alert('Para selecionar um município, primeiro é necessário ter um órgão cadastrado!');
								window.parent.opener.location.reload();
								self.close();
							</script>";
					}
				}
			}
		}	
	}else {
			
		// Altera o status para I
		$sql_limpa = $db->executar("
							UPDATE 
								obras.usuarioresponsabilidade 
							SET 
								rpustatus = 'I' 
							WHERE 
								usucpf = '{$usucpf}' AND 
								pflcod = '{$pflcod}' AND 
								prsano = '{$_SESSION["exercicio"]}' AND 
								orgcod is not null AND
								muncod is not null AND
								unicod is null");
		
	}
	
	$db->commit();
	
	echo "
		<script>
			alert('Operação realizada com sucesso!');
			window.parent.opener.location.reload();
			self.close();
		</script>";
	
}

/**
 * Enter description here...
 *
 */
function buscaMunicipiosAtribuido($usucpf, $pflcod){
	
	$db = new cls_banco();
	
	$sql = "SELECT DISTINCT 
				m.muncod as codigo, 
				m.estuf||' - '||m.mundescricao as descricao 
			FROM 
				obras.usuarioresponsabilidade ur 
			INNER JOIN 
				territorios.municipio m ON ur.muncod = m.muncod
	 		WHERE
	 			ur.rpustatus='A' AND ur.usucpf = '$usucpf' AND ur.pflcod=$pflcod";
	
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
}

?>


<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Estados e Municípios</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<div align=center id="aguarde"><img src="../imagens/icon-aguarde.gif" border="0" align="absmiddle">
			<font color=blue size="2">Aguarde! Carregando Dados...</font>
		</div>
		<?flush();?>
		
		<!-- Lista de Estados e Municípios -->
		<div style="overflow:auto; width:496px; height:350px; border:2px solid #ececec; background-color: #ffffff;">
			<form name="formulario">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione o Estado</strong></td>		
						</tr>
					</thead>
					<?php listaMunicipios(); ?>
				</table>
			</form>
		</div>
		
		<!-- Estados Selecionadas -->
		<form name="formassocia" action="cadastro_usuario_obras_municipio.php?acao=A" method="post">
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="munresp[]" id="munresp" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
				<?php 
					buscaMunicipiosAtribuido($usucpf, $pflcod);
				?>
			</select>
		</form>
		
		<!-- Submit do Formulário -->
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding:3px;" colspan="3">
					<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
				</td>
			</tr>
		</table>
	</body>
</html>

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


var campoSelect = document.getElementById("munresp");


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

function moveto(obj) {
	if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();}
}  m                                                                                             


</script>