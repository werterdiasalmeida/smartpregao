<?php
ini_set("memory_limit", "1024M");
set_time_limit(0);

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = "Upload de Arquivos";
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
			<td colspan="3">
				<?php if($_REQUEST['schema']): ?>
					<input type="button" name="listar_todos_schemas" value="Listar Todos os Schemas" onclick="window.location.href='seguranca.php?modulo=sistema/geral/uploadArquivos&acao=A'" /> 
				<?php endif; ?>
				<?php if($_REQUEST['tabela']): ?>
					<input type="button" name="listar_todas_tbl" value="Listar Todas as Tabelas" onclick="window.location.href='seguranca.php?modulo=sistema/geral/uploadArquivos&acao=A&schema=<?php echo $_REQUEST['schema'];?>'" /> 
				<?php endif; ?>
			</td>
		</tr>
		<?php endif; ?>
		<?php if(is_array($arrTbl)): ?>
			<?php foreach($arrTbl as $sch => $tbl): ?>
				<tr class="SubtituloTabela center" >
					<td class="bold" colspan="3" >Schema: <a href="seguranca.php?modulo=sistema/geral/uploadArquivos&acao=A&schema=<?php echo $sch;?>"><?php echo $sch;?></a></td>
				</tr>
				<tr class="SubtituloTabela center" >
					<td width="40%" >Tabela</td>
					<td width="20%" >Nº de Arquivos Perdidos</td>
					<td>%</td>
				</tr>
				<?php $i = 1; ?>
				<?php $total_arq_total = 0; ?>
				<?php $total_arq_perdidos = 0; ?>
				<?php foreach($tbl as $t): ?>
					<?php $color = $i%2 ? "#F9F9F9" : "" ?>
					<tr bgcolor="<?php echo $color; ?>" >
						<td><a href="seguranca.php?modulo=sistema/geral/uploadArquivos&acao=A&schema=<?php echo $sch;?>&tabela=<?php echo $t ?>"><?php echo $t ?></a></td>
						<?php $qnt_arq_total = $db->pegaUm("select count(*) from $sch.$t tbl inner join public.arquivo a ON a.arqid = tbl.arqid where tbl.arqid is not null and (a.arqstatus not in('0','I') or a.arqstatus is null)") ?>
						<?php $total_arq_total += $qnt_arq_total; ?>
						<?php $qnt_arq_perdidos = $db->pegaUm("select count(*) from $sch.$t tbl inner join public.arquivo a ON a.arqid = tbl.arqid where tbl.arqid/1000 between 647 and 725 and (a.arqstatus not in('0','I') or a.arqstatus is null)") ?>
						<?php $total_arq_perdidos += $qnt_arq_perdidos; ?>
						<td><?php echo number_format($qnt_arq_perdidos,0,3,".")?> de <?php echo number_format($qnt_arq_total,0,3,".")?></td>
						<td><?php echo $qnt_arq_perdidos == 0 ? "0%" : round(($qnt_arq_perdidos/$qnt_arq_total)*100,2)."%" ?></td>
					</tr>
					<?php $i++; ?>
				<?php endforeach; ?>
				<tr class="SubtituloTabela" >
					<td></td>
					<td class="bold" ><?php echo number_format($total_arq_perdidos,0,3,".")?> de <?php echo number_format($total_arq_total,0,3,".")?></td>
					<td class="bold" ><?php echo $total_arq_perdidos == 0 ? "0%" : round(($total_arq_perdidos/$total_arq_total)*100,2)."%" ?></td>
				</tr>
				<tr>
					<td colspan="3">&nbsp;</td>
				</tr>
			<?php endforeach; ?>
		<?php endif; ?>
		<?php if($_REQUEST['schema'] && is_array($arrTbl[$_REQUEST['schema']]) && 1==2): ?>
			<?php foreach($arrTbl[$_REQUEST['schema']] as $tbl): ?>
				<?php $sql = "	select
									a.arqid,
									a.arqnome,
									a.arqdescricao,
									a.arqextensao,
									a.arqtipo,
									a.arqtamanho,
									a.usucpf
								from
									public.arquivo a
								inner join
									".$_REQUEST['schema'].".$tbl tbl ON tbl.arqid = a.arqid
								where
									a.arqid/1000 between 647 and 725" ?>
				<?php dbg($sql); ?>		
			<?php endforeach; ?>
		<?php endif; ?>
	</table>
	<input type="hidden" id="inp_hdn_acao" name="inp_hdn_acao" value="" />
</form>