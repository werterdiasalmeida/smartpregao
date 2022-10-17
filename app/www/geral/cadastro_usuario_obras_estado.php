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
	$estuf = $_REQUEST["estresp"];
	atribuiEstado($usucpf, $pflcod, $estuf);
}

/**
 * Função que lista as uf's
 *
 */
function listaEstados(){
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
		$codigo    = $estadosExistentes[$i]["estuf"];
		$descricao = $estadosExistentes[$i]["estdescricao"];
		
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		
		echo "
			<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"Checkbox\" name=\"estuf\" id=\"".$codigo."\" value=\"".$codigo."\" onclick=\"retorna('".$i."');\">
					<input type=\"hidden\" name=\"estdescricao\" value=\"".$codigo." - ".$descricao."\">
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
 * Função que atribui a responsabilidade de uma uf ao usuário
 *
 * @param string $usucpf
 * @param int $pflcod
 * @param string $estuf
 */
function atribuiEstado($usucpf, $pflcod, $estuf){
	
	$db = new cls_banco();
	
	$data = date("Y-m-d H:i:s");
	
	// 
	if (is_array($estuf)){
		if (in_array("", $estuf)){
			
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
									estuf is not null AND
									orgcod is not null AND
									unicod is null");
			
		}else {
			$count = count($estuf);
			for ($i = 0; $i < $count; $i++){
				$uf = $estuf[$i];
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
						$sql_insere =$db->carregar("
												INSERT INTO
													obras.usuarioresponsabilidade
													(estuf, orgcod, usucpf, rpustatus, rpudata_inc, pflcod, prsano)
												VALUES
													('{$uf}', '{$orgcod}', '{$usucpf}', 'A', '{$data}', '{$pflcod}', '{$_SESSION["exercicio"]}')");
						
					}else {
						
						echo "
							<script>
								alert('Para selecionar um estado, primeiro é necessário ter um órgão cadastrado!');
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

function buscaEstadoAtribuido($usucpf, $pflcod){
	
	$db = new cls_banco();
	
	$sql = "SELECT DISTINCT 
				u.estuf AS codigo, 
				u.estuf||' - '||u.estdescricao AS descricao 
			FROM 
				obras.usuarioresponsabilidade ur 
			INNER JOIN 
				territorios.estado u ON ur.estuf = u.estuf 
			WHERE 
				ur.rpustatus = 'A' AND ur.usucpf = '$usucpf' AND ur.pflcod = $pflcod";
	
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
		$sql = "
			SELECT DISTINCT 
				u.estuf AS codigo, 
				u.estdescricao AS descricao 
			FROM 
				territorios.estado u 
			INNER JOIN 
				cte.unidproposto up 
			ON 
				u.estuf = up.estuf AND up.usucpf = '".$usucpf."' 
			WHERE 
				up.estuf is not null";
		
		$RS = @$db->carregar($sql);
		
		if(is_array($RS)) {
			$nlinhas = count($RS)-1;
			if ($nlinhas>=0) {
				for ($i=0; $i<=$nlinhas;$i++) {
					foreach($RS[$i] as $k=>$v) ${$k}=$v;
					print " <option value=\"$codigo\">$codigo - $descricao</option>";
				}
			}
		} else {
			print '<option value="">Clique no estado selecionar.</option>';
		}
	}
}

?>


<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Estados</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<div align=center id="aguarde"><img src="../imagens/icon-aguarde.gif" border="0" align="absmiddle">
			<font color=blue size="2">Aguarde! Carregando Dados...</font>
		</div>
		<?flush();?>
		
		<!-- Lista de Estados -->
		<div style="overflow:auto; width:496px; height:350px; border:2px solid #ececec; background-color: #ffffff;">
			<form name="formulario">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
					<script language="JavaScript">
						document.getElementById('tabela').style.visibility = "hidden";
						document.getElementById('tabela').style.display  = "none";
					</script>
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione o Estado</strong></td>		
						</tr>
					</thead>
					<?php listaEstados(); ?>
				</table>
			</form>
		</div>
		
		<!-- Estados Selecionadas -->
		<form name="formassocia" action="cadastro_usuario_obras_estado.php?acao=A" method="post">
			<input type="hidden" name="usucpf" value="<?=$usucpf?>">
			<input type="hidden" name="pflcod" value="<?=$pflcod?>">
			<select multiple size="8" name="estresp[]" id="estresp" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
				<?php 
					buscaEstadoAtribuido($usucpf, $pflcod);
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


var campoSelect = document.getElementById("estresp");


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
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique no Estado.', '', false, false);}
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