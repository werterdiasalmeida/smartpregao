<?php
ini_set("memory_limit", "1024M");
set_time_limit(0);

if($_FILES['arquivo']) {
	
	include APPRAIZ ."includes/funcoes_public_arquivo.php";
	
	$arrValidacao = array('extensao');
	
	$_REQUEST['_sisdiretorio'] = $_REQUEST['schema'];
	
	$resp = atualizarPublicArquivo($arrValidacao);
	
	if($resp['TRUE']) $msg .= 'Foram processados '.count($resp['TRUE']).' arquivos.'.'\n';
	if($resp['FALSE']) {
		$msg .= 'Problemas encontrados:'.'\n';
		foreach($resp['FALSE'] as $k => $v) {
			$msg .= 'ARQID : '.$k.' | '.$v.'\n';
		}
	}
	
	die("<script>
			alert('".$msg."');
			window.location = window.location;
		 </script>");
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = "Arquivos Corrompidos";
monta_titulo( $titulo, '&nbsp' );

$sql = "SELECT
			n.nspname as esquema,
			c.relname as tabela
		FROM
			pg_namespace n,
			pg_class c,
			pg_attribute a,
			pg_type t
		WHERE
		    n.oid = c.relnamespace
			and c.relkind = 'r'
			and n.nspname not like 'pg\\_%'
			and n.nspname != 'information_schema'
			and n.nspname != 'public'
			and a.attnum > 0
			and not a.attisdropped
			and a.attrelid = c.oid
			and a.atttypid = t.oid
			and a.attname='arqid'
			and c.relname != 'arquivo'
			".($_REQUEST['schema'] ? " and n.nspname = '".$_REQUEST['schema']."' " : "")."
			".($_REQUEST['tabela'] ? " and c.relname = '".$_REQUEST['tabela']."' " : "")."
		ORDER BY
			nspname, relname, attname;";

$arrTabelas = $db->carregar($sql);
if(is_array($arrTabelas)){
	foreach($arrTabelas as $tbl){
		$arrTbl[$tbl['esquema']][] = $tbl['tabela'];
	}
}

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">

function limpaUpload(arqid)
{
	document.getElementById('arquivo_' + arqid).value = "";
}

function uploadArquivos(tbl)
{
	document.getElementById('btn_salvar_' + tbl).value="Carregando...";
	document.getElementById('btn_salvar_' + tbl).disabled = "true";
	document.getElementById('form_arquivo_' + tbl).submit();
}

</script>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.msg_erro{color:#990000}
	.link{cursor: pointer}
	.sucess_msg{color: blue;}
	.img_middle{vertical-align:middle}
	.hidden{display:none}
</style>

<form name="formulario" id="formulario"  method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<?php if($_REQUEST['schema'] || $_REQUEST['tabela']): ?>
		<tr class="SubtituloTabela center" >
			<td colspan="5">
				<?php if($_REQUEST['schema']): ?>
					<input type="button" name="listar_todos_schemas" value="Listar Todos os Schemas" onclick="window.location.href='seguranca.php?modulo=relatorio/uploadArquivos&acao=A'" /> 
				<?php endif; ?>
				<?php if($_REQUEST['tabela']): ?>
					<input type="button" name="listar_todas_tbl" value="Listar Todas as Tabelas" onclick="window.location.href='seguranca.php?modulo=relatorio/uploadArquivos&acao=A&schema=<?php echo $_REQUEST['schema'];?>'" /> 
				<?php endif; ?>
			</td>
		</tr>
		<?php endif; ?>
		<?php if(is_array($arrTbl)): ?>
			<?php foreach($arrTbl as $sch => $tbl): ?>
				<tr class="SubtituloTabela center" >
					<td class="bold" colspan="5" >Schema: <a href="seguranca.php?modulo=relatorio/uploadArquivos&acao=A&schema=<?php echo $sch;?>"><?php echo $sch;?></a></td>
				</tr>
				<tr class="SubtituloTabela center" >
					<td width="40%" >Tabela</td>
					<td width="20%" >Número de Arquivos Corrompidos</td>
					<td>Porcentagem de Arquivos Corrompidos</td>
					<td>Número de Arquivos Recuperados</td>
					<td>Porcentagem de Arquivos Recuperados</td>
				</tr>
				<?php $i = 1; ?>
				<?php $total_arq_total = 0; ?>
				<?php $total_arq_perdidos = 0; ?>
				<?php $total_arq_recuperados = 0; ?>
				<?php foreach($tbl as $t): ?>
					<?php $color = $i%2 ? "#F9F9F9" : "" ?>
					<tr bgcolor="<?php echo $color; ?>" >
						<td><a href="seguranca.php?modulo=relatorio/uploadArquivos&acao=A&schema=<?php echo $sch;?>&tabela=<?php echo $t ?>"><?php echo $t ?></a></td>
						<?php $qnt_arq_total = $db->pegaUm("select count(*) from $sch.$t tbl inner join public.arquivo a ON a.arqid = tbl.arqid where tbl.arqid is not null and (a.arqstatus not in('0','I') or a.arqstatus is null)") ?>
						<?php $total_arq_total += $qnt_arq_total; ?>
						<?php $qnt_arq_perdidos = $db->pegaUm("select count(*) from $sch.$t tbl inner join public.arquivo a ON a.arqid = tbl.arqid where tbl.arqid/1000 between 647 and 725 and (a.arqstatus not in('0','I') or a.arqstatus is null)") ?>
						<?php $qnt_arq_per_tbl[$t] = $qnt_arq_perdidos ?>
						<?php $total_arq_perdidos += $qnt_arq_perdidos; ?>
						<td><?php echo number_format($qnt_arq_perdidos,0,3,".")?> de <?php echo number_format($qnt_arq_total,0,3,".")?></td>
						<td><?php echo $qnt_arq_perdidos == 0 ? "0%" : round(($qnt_arq_perdidos/$qnt_arq_total)*100,2)."%" ?></td>
						<?php $qnt_arq_recuperados = $db->pegaUm("select count(*) from $sch.$t tbl inner join public.arquivo a ON a.arqid = tbl.arqid where a.arqid in (select arqid from public.arquivo_recuperado)") ?>
						<?php $total_arq_recuperados += $qnt_arq_recuperados; ?>
						<td><?php echo number_format($qnt_arq_recuperados,0,3,".")?> de <?php echo number_format($qnt_arq_perdidos,0,3,".")?></td>
						<td><?php echo $qnt_arq_recuperados == 0 || $qnt_arq_perdidos == 0 ? ($qnt_arq_recuperados == 0 && $qnt_arq_perdidos == 0 ? "N/A" : "0%") : round(($qnt_arq_recuperados/$qnt_arq_perdidos)*100,2)."%" ?></td>
					</tr>
					<?php $i++; ?>
				<?php endforeach; ?>
				<tr class="SubtituloTabela" >
					<td></td>
					<td class="bold" ><?php echo number_format($total_arq_perdidos,0,3,".")?> de <?php echo number_format($total_arq_total,0,3,".")?></td>
					<td class="bold" ><?php echo $total_arq_perdidos == 0 ? "0%" : round(($total_arq_perdidos/$total_arq_total)*100,2)."%" ?></td>
					<td class="bold" ><?php echo number_format($total_arq_recuperados,0,3,".")?> de <?php echo number_format($total_arq_perdidos,0,3,".")?></td>
					<td class="bold" ><?php echo $total_arq_perdidos == 0 || $total_arq_recuperados == 0 ? ($total_arq_perdidos == 0 && $total_arq_recuperados == 0 ? "N/A" : "0%") : round(($total_arq_recuperados/$total_arq_perdidos)*100,2)."%" ?></td>
				</tr>
				<tr>
					<td colspan="5">&nbsp;</td>
				</tr>
			<?php endforeach; ?>
		<?php endif; ?>
		<?php if($_REQUEST['schema'] && is_array($arrTbl[$_REQUEST['schema']]) ): ?>
			<?php foreach($arrTbl[$_REQUEST['schema']] as $tbl): ?>
				<?php if($qnt_arq_per_tbl[$tbl]): ?>
					<tr class="SubtituloTabela" >
						<td class="bold center" colspan="5" >
							<?php echo $tbl ?>
						</td>
					</tr>
					<?php $cabecalho = array("ID","Nome do Arquivo","Descrição","Tamanho (bytes)","Data","Upload") ?>
					<?php $sql = "	select
										a.arqid,
										a.arqnome||'.'||a.arqextensao,
										a.arqdescricao,
										a.arqtamanho,
										to_char(a.arqdata,'dd/mm/YYYY')||' '||a.arqhora as arqdata,
										'<span style=\"white-space: nowrap\" ><input type=\"file\" name=\"arquivo[' || a.arqid || ']\" id=\"arquivo_' ||  a.arqid || '\" > <img class=\"middle link\" onclick=\"limpaUpload(\'' || a.arqid || '\')\" src=\"../imagens/excluir.gif\" /></span>' as upload
									from
										public.arquivo a
									inner join
										".$_REQUEST['schema'].".$tbl tbl ON tbl.arqid = a.arqid
									where
										a.arqid/1000 between 647 and 725
									and
										a.arqid not in (select arqid from public.arquivo_recuperado)" ?>
				<tr>
					<td style="background-color:#FFFFFF" colspan="5" ><?php $db->monta_lista($sql,$cabecalho,10,5,'N','center','','form_arquivo_'.$tbl,array("5%","30%","25%","10%","10%","20%") ); ?></td>
				</tr>
				<tr>
					<td colspan="5" class="center" ><input type="button" id="btn_salvar_<?php echo $tbl ?>" name="btn_salvar_<?php echo $tbl ?>" value="Salvar" onclick="uploadArquivos('<?php echo $tbl ?>')" /></td>
				</tr>
			<?php endif; ?>
			<?php endforeach; ?>
		<?php endif; ?>
	</table>
	<input type="hidden" id="inp_hdn_acao" name="inp_hdn_acao" value="" />
</form>