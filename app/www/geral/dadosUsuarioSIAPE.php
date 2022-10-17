<?
include "config.inc";

if( $_SESSION['usucpf'] ){

header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db = new cls_banco();

$sql = "SELECT
			tss.nu_cpf,
			tss.no_servidor,
			tss.no_mae,
			to_char(tss.dt_nascimento::date,'DD/MM/YYYY') AS dt_nascimento,
			tss.nu_matricula_siape,
			tss.co_grupo_cargo_emprego,
			tss.co_cargo_emprego,
			tse.ds_cargo_emprego,
			tne.ds_nivel_escolaridade,
			tss.sg_funcao,
			tss.co_nivel_funcao,
			tso.co_orgao,
			tso.ds_orgao,
			tso.sg_orgao,
			tso.nu_ddd_crh,
			tso.nu_telefone_crh,
			tso.no_coordenador_crh
		FROM
			siape.tb_siape_cadastro_servidor tss
		LEFT JOIN
			siape.tb_simec_orgao tso ON tso.co_orgao = tss.co_orgao::numeric
		LEFT JOIN
			siape.tb_siape_nivel_escolaridade tne ON tne.co_nivel_escolaridade = tss.co_nivel_escolaridade
		LEFT JOIN	
			siape.tb_simec_cargo_emprego tse ON tse.co_cargo_emprego = tss.co_grupo_cargo_emprego || tss.co_cargo_emprego			
			WHERE
			nu_cpf = '".$_REQUEST["cpf"]."'";
$dados = $db->carregar($sql);

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<table class="listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;" width="100%" border="0">
	<tr>
		<td align="center" bgcolor="#c0c0c0" colspan="2">
			<b>Dados Pessoais</b>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top" align="right"><b>CPF:</b></td>
		<td><?=$dados[0]["nu_cpf"]?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top" align="right"><b>Nome Servidor:</b></td>
		<td><?=$dados[0]["no_servidor"]?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top" align="right"><b>Nome da Mãe:</b></td>
		<td>
			<?=$dados[0]["no_mae"]?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top" align="right" style="border-bottom:1px solid black"><b>Data de Nascimento:</b></td>
		<td style="border-bottom:1px solid black">
			<?=$dados[0]["dt_nascimento"]?>
		</td>
	</tr>
	
	<?
		for($i=0; $i<count($dados); $i++) {
			$style = ($i == (count($dados)-1)) ? '' : 'style="border-bottom:1px solid black"';
			
			echo '<tr>
			<td align="center" bgcolor="#c0c0c0" colspan="2">
			<b>Dados Funcionais</b>	</td>
				</tr>
				<tr>
			
					<td class="SubTituloDireita" valign="top" align="right"><b>Matrícula SIAPE:</b></td>
					<td>
						'.$dados[$i]["nu_matricula_siape"].'
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="top" align="right"><b>Órgão:</b></td>
					<td>
						'.$dados[$i]["co_orgao"].'&nbsp;-&nbsp;'.$dados[$i]["ds_orgao"].'&nbsp;('.trim($dados[$i]["sg_orgao"]).') 
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="top" align="right"><b>Cargo:</b></td>
					<td>
						'.$dados[$i]["co_grupo_cargo_emprego"].'&nbsp;'.$dados[$i]["co_cargo_emprego"].'&nbsp;-&nbsp;'.$dados[$i]["ds_cargo_emprego"].' 
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="top" align="right"><b>Função:</b></td>
					<td>
						'.$dados[$i]["sg_funcao"].' 
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="top" align="right"><b>Nível:</b></td>
					<td>
						'.$dados[$i]["co_nivel_funcao"].' 
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="top" align="right"><b>Coordenador CRH:</b></td>
					<td>
						'.$dados[$i]["no_coordenador_crh"].' 
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" valign="top" align="right" '.$style.'><b>Telefone:</b></td>
					<td '.$style.'>
						('.$dados[$i]["nu_ddd_crh"].')&nbsp;'.$dados[0]["nu_telefone_crh"].'
					</td>
				</tr>';
		}
	?>
	<tr bgcolor="#c0c0c0">
		<td colspan="2" align="center">
			<input type="button" value="Fechar Janela" onclick="self.close();" />
		</td>
	</tr>
</table>
<?php } ?>
