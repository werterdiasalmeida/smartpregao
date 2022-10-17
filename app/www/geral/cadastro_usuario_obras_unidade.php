<?
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

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
	$unicod = $_REQUEST["uniresp"];
	atribuiUnidade($usucpf, $pflcod, $unicod);
}

/**
 * Função que lista as unidades
 *
 */
function listaUnidades(){
	$db = new cls_banco();
	
	// SQL para buscar unidades existentes
	$unidadesExistentes = $db->carregar(
								"SELECT
									unicod, unidsc
								FROM 
									unidade
								WHERE
									unistatus='A' AND
									orgcod = '26000'
								ORDER BY 
									unicod, unidsc");
	
	$count = count($unidadesExistentes);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		$codigo    = $unidadesExistentes[$i]["unicod"];
		$descricao = $unidadesExistentes[$i]["unidsc"];
		
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"unicod\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\">
					<input type=\"hidden\" name=\"unidsc\" value=\"".$codigo." - ".$descricao."\">
				</td>
				<td align=\"right\" style=\"color:blue;\" width=\"10%\">
					".$codigo."
				</td>
				<td>
					".$descricao."
				</td>
			</tr>";
	}
			
}

/**
 * Função que atribui a responsabilidade de uma unidade ao usuário
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $unicod
 */
function atribuiUnidade($usucpf, $pflcod, $unicod){
	
	$db = new cls_banco();
	$data = date("Y-m-d H:i:s");
	
	$sql_zera = $db->executar(" 
								UPDATE 
									obras.usuarioresponsabilidade 
								SET 
									rpustatus = 'I' 
								WHERE 
									usucpf = '{$usucpf}' AND 
									pflcod = '{$pflcod}' AND 
									prsano = '{$_SESSION["exercicio"]}' AND 
									unicod is not null ");
	
	if (is_array($unicod)){
		if (in_array("", $unicod)){
		
			// Altera o status para I
			$sql_zera = $db->executar(" 
								UPDATE 
									obras.usuarioresponsabilidade 
								SET 
									rpustatus = 'I' 
								WHERE 
									usucpf = '{$usucpf}' AND 
									pflcod = '{$pflcod}' AND 
									prsano = '{$_SESSION["exercicio"]}' AND 
									unicod is not null ");
			
			// Retorna o status do usuário
			$sql_status_orgao = $db->executar("
										UPDATE 
											obras.usuarioresponsabilidade 
										SET 
											rpustatus = 'A' 
										WHERE 
											usucpf = '{$usucpf}' AND 
											pflcod = '{$pflcod}' AND
											prsano = '{$_SESSION["exercicio"]}' AND
											unicod is null AND
											estuf is null AND
											muncod is null");
		}else {		
			
			$count = count($unicod);
			for ($i = 0; $i < $count; $i++){
				$unidade = $unicod[$i];
				
				$orgao = $db->pegaUm("
								SELECT 
									orgcod 
								FROM 
									obras.usuarioresponsabilidade 
								WHERE
									rpustatus = 'A' AND
									usucpf = '{$usucpf}' AND
									pflcod = '{$pflcod}' AND
									unicod = '{$unidade}' AND
									orgcod is not null");
				if ($orgao){
					continue;
				} else{
					// Pega o órgão do usuário
					$orgcod = $db->pegaUm("
										SELECT 
											orgcod
										FROM
											obras.usuarioresponsabilidade
										WHERE
											usucpf = '{$usucpf}' AND
											pflcod = '{$pflcod}' AND
											prsano = '{$_SESSION["exercicio"]}' AND
											rpustatus = 'A'");
					if($orgcod){
						
						// Insere a nova unidade
						$sql_insert = $db->carregar("
										INSERT INTO 
											obras.usuarioresponsabilidade
											(unicod, orgcod, usucpf, rpustatus, rpudata_inc, pflcod, prsano)
										VALUES
											('{$unidade}', '{$orgcod}', '{$usucpf}', 'A', '{$data}', '{$pflcod}', '{$_SESSION["exercicio"]}')");
						
					}else {
						echo "
							<script>
								alert('Para atribuir uma unidade, primeiro é necessário ter um órgão cadastrado!');
								window.parent.opener.location.reload();
								self.close();
							</script>";
					}
				}
			}
		}
	}
	
	$db->commit();
	
	echo "
		<script>
			alert('Operação realizada com sucesso!');
			window.parent.opener.location.reload();
			self.close();
		</script>";
	
}

function buscaUnidadesCadastradas($usucpf, $pflcod){
	
	$db = new cls_banco();
	
	$sql = "SELECT DISTINCT 
				u.unicod as codigo, u.unicod||' - '||u.unidsc as descricao 
			FROM 
				obras.usuarioresponsabilidade ur 
			INNER JOIN 
				unidade u 
			ON 
				ur.unicod = u.unicod 
			WHERE 
				ur.rpustatus = 'A' AND 
				ur.usucpf = '$usucpf' AND 
				ur.pflcod=$pflcod";
	
	$RS = @$db->carregar($sql);
	
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
	    		print " <option value=\"$codigo\">$descricao</option>";		
			}
		}
	} else{
		$sql = "select distinct u.unicod as codigo, u.unidsc as descricao from unidade u inner join unidproposto up on u.unicod=up.unicod and up.usucpf='".$usucpf."' where up.unicod is not null";
		$RS = @$db->carregar($sql);
		if(is_array($RS)) {
			$nlinhas = count($RS)-1;
			if ($nlinhas>=0) {
				for ($i=0; $i<=$nlinhas;$i++) {
					foreach($RS[$i] as $k=>$v) ${$k}=$v;
					print " <option value=\"$codigo\">$codigo - $descricao</option>";
				}
			}
		} else{
			print '<option value="">Clique na Unidade selecionar.</option>';
		}
	}
}

?><html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Unidades</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		
		<!-- Lista de Unidades -->
		<div style="overflow:auto; width:496px; height:350px; border:2px solid #ececec; background-color: #ffffff;">
			<form name="formulario">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
					<script language="JavaScript">
						document.getElementById('tabela').style.visibility = "hidden";
						document.getElementById('tabela').style.display  = "none";
					</script>
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione a(s) Unidade(s)</strong></td>		
						</tr>
					</thead>
					<?php listaUnidades(); ?>
				</table>
			</form>
		</div>
		
		<!-- Unidades Selecionadas -->
		<form name="formassocia" action="cadastro_usuario_obras_unidade.php?acao=A" method="post">
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="uniresp[]" id="uniresp" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
				<?php 
					buscaUnidadesCadastradas($usucpf, $pflcod);
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

<script language="JavaScript">
//document.getElementById('aguarde').style.visibility = "hidden";
//document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";


var campoSelect = document.getElementById("uniresp");


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
	if (document.formulario.unicod[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.unidsc[objeto].value, document.formulario.unicod[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.unicod[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Unidade.', '', false, false);}
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
	</body>
</html>
