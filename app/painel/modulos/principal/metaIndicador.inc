<?php 
$indid = $_SESSION['indid'];

include (APPRAIZ.'www/painel/_funcoes_metas.php');

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['requisicaoAjax']();
	exit();
}

if(!$_SESSION['indid']){
	header("Location: painel.php?modulo=principal/lista&acao=A");
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');

$titulo = "Metas do Indicador";
monta_titulo( $titulo, "&nbsp;" );
cabecalhoIndicador($_SESSION['indid']);

$arrInd = $db->pegaLinha("select ind.*, u.umedesc from painel.indicador ind inner join painel.unidademeta u ON u.umeid = ind.umeid where indid = $indid");

$sql = "select prtobj from public.parametros_tela where mnuid = {$_SESSION['mnuid']} and prtdsc = '$indid'";
$prtobj = $db->pegaUm($sql);

if($prtobj){
	$prtobj = unserialize($prtobj);
	extract($prtobj);
}

?>
<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../painel/js/metas_indicador.js"></script>

<script type="text/javascript" >
function exibirGraficoMeta()
{
	var indid	     = $("[name=indid]").val();
	var perid	     = $("[name=perid]").val();
	var dpeidinicio  = $("[name=dpeidinicio]").val();
	var dpeidfim     = $("[name=dpeidfim]").val();
	var projecao     = $("[name=projecao]").val();
	<?php if($arrInd['indqtdevalor'] == "t"): ?>
		var rd_qtdevalor = $("[name=rd_qtdevalor]:checked").val();
	<?php else: ?>
		var rd_qtdevalor = $("[name=rd_qtdevalor]").val();
	<?php endif; ?>
	var erro = false;
	
	if(!perid){
	 	alert('Selecione a periodicidade!');
	 	erro = 1;
	 	$("[name=perid]").focus();
	 	return false;
	}
	if(!dpeidinicio){
	 	alert('Selecione a data inicial!');
	 	erro = 1;
	 	$("[name=dpeidinicio]").focus();
	 	return false;
	}
	if(!projecao){
	 	alert('Selecione a data inicial da projeção!');
	 	erro = 1;
	 	$("[name=projecao]").focus();
	 	return false;
	}
	
	if(!dpeidfim){
	 	alert('Selecione a data final da projeção!');
	 	erro = 1;
	 	$("[name=dpeidfim]").focus();
	 	return false;
	}
	
	if(projecao != dpeidinicio){
		if(testaDataPorDpeid(projecao,dpeidinicio).responseText.search("false") >= 0){
			erro = 1;
			alert('A data inicial de projeção deve ser menor ou igual a data inicial!');
			return false;
		}
	}
	
	if(dpeidfim != projecao){
		if(testaDataPorDpeid(dpeidfim,projecao).responseText.search("false") >= 0){
			erro = 1;
			alert('A data inicial de projeção deve ser menor ou igual a data final de projeção!');
			return false;
		}
	}
	
	if(rd_qtdevalor == "valor"){
		var finac_qtde = "";
		var finac_valor = 1;
	}if(rd_qtdevalor == "ambos"){
		var finac_qtde = 1;
		var finac_valor = 1;
	}if(rd_qtdevalor == "qtde" || rd_qtdevalor == "" || !rd_qtdevalor){
		var finac_qtde = 1;
		var finac_valor = "";
	}
	
	if(erro == false){
		var geraGrafico = "geraGrafico.php?tipo=meta;indid=" + indid + ";dpeid=" + dpeidinicio + ";dpeid2=" + dpeidfim + ";finac_qtde=" + finac_qtde + ";finac_valor=" + finac_valor + ";projecao=" + projecao + ";periodicidade=" + perid;
		var altura = "250";
		var largura = "100%";
		swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_meta", largura, altura, "9.0.0", "expressInstall.swf", {"data-file":geraGrafico,"loading":"Carregando gráfico..."} );
	}
	
}

function testaDataPorDpeid(dpeid1,dpeid2){

	 return $.ajax({
		   type: "POST",
		   url: 'painel.php?modulo=principal/painel_controle&acao=A',
		   async: false,
		   data: "testaDatasDpeid=true&dpeid1=" + dpeid1 + "&dpeid2=" + dpeid2,
		   success: function(msg){
		   }
		 });
}

function salvarExibicaoGraficoMeta()
{
	var parms = $("#form_meta").serialize();
	$.ajax({
		type: "POST",
		url: window.location,
		async: false,
		data: "requisicaoAjax=salvarExibicaoMeta&" + parms,
		success: function(msg){
			alert('Operação realizada com sucesso!');
		}
	});
}
<?php if($prtobj): ?>
	$(function() {
		exibirGraficoMeta();
	});
<?php endif; ?>

</script>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.link{cursor: pointer;}
	.hidden{display:none}
</style>
<?php $arrDados = getMetaIndicador()?>
<form name="form_meta" id="form_meta" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?php if($arrDados): ?>
	<tr>
		<td width="30%" class="SubTituloDireita" >Periodicidade:</td>
		<td>
			<?php
				$sql = "select
							per.perid as codigo,
							per.perdsc as descricao
						from
							painel.periodicidade per
						where
							per.pernivel >= ( 
											select 
												min(per.pernivel)
											from
												painel.periodicidade per
											inner join
												painel.detalheperiodicidade dpe ON per.perid = dpe.perid
											inner join
												painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
											where
												seh.indid = $indid
											and
												per.perstatus = 'A'
											and
												seh.sehstatus <> 'I'
										  )
						and
							per.perstatus = 'A'
						order by
							per.pernivel";
				$db->monta_combo('perid',$sql,"S",'Selecione a Periodicidade','','','','','','','');
			 ?>
		</td>
	</tr>
	<tr>
		<td width="30%" class="SubTituloDireita" >Período Inicial:</td>
		<td>
			<?php
				$sql = "select  
							dpe.dpeid as codigo,
							dpe.dpedsc as descricao
						from 
							painel.seriehistorica seh
						inner join
							painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
						where
							indid = $indid
						and
							sehstatus <> 'I'
						order by
							dpe.dpedatainicio asc";
				$db->monta_combo('dpeidinicio',$sql,"S",'Selecione o Período','','','','','','','');
			 ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Projeção:</td>
		<td>
			<?php
				 
				$sql = "select  
							dpe.dpeid as codigo,
							dpe.dpedsc as descricao
						from 
							painel.detalheperiodicidade dpe
						where
							dpe.perid in ( select perid from painel.indicador where indid = $indid)
						and
							dpedatainicio >= ( 	select  
													dpe.dpedatainicio
												from 
													painel.seriehistorica seh
												inner join
													painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
												where
													indid = $indid
												and
													sehstatus <> 'I'
												order by
													dpe.dpedatainicio asc
												limit 1 
											  )
						and
							dpedatafim <=    ( 	select  
													dpe.dpedatafim
												from 
													painel.detalhemetaindicador dmi
												inner join
													painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid
												inner join
													painel.metaindicador met ON met.metid = dmi.metid
												where
													met.indid = $indid
												order by
													dpe.dpedatainicio desc
												limit 1 
											  )
						order by
							dpe.dpedatainicio asc";
				$db->monta_combo('projecao',$sql,"S",'Selecione o Período','','','','','','','');?> até
				<span id="span_dpeidfim"> 
				<?php 
				$sql = "select  
							dpe.dpeid as codigo,
							dpe.dpedsc as descricao
						from 
							painel.detalheperiodicidade dpe
						where
							/*dpe.perid in ( select perid from painel.indicador where indid = $indid)
						and*/
							dpeid in ( 	select  
													dmi.dpeid
												from 
													painel.detalhemetaindicador dmi
												inner join
													painel.metaindicador met ON met.metid = dmi.metid
												where
													met.indid = $indid
												and
													met.metstatus = 'A'
												and
													dmi.dmistatus = 'A'
											  )
						order by
							dpe.dpedatainicio asc";
				
				$db->monta_combo('dpeidfim',$sql,"S",'Selecione o Período','','','','','','','');?>
				</span>
		</td>
	</tr>
	<?php if($arrInd['indqtdevalor'] == "t"): ?>
	<tr>
		<td class="SubTituloDireita" >Exibir:</td>
		<td>
			<input type="radio" name="rd_qtdevalor" value="qtde" /><?php echo $arrInd['umedesc'] ?> 
			<input type="radio" name="rd_qtdevalor" value="valor" />Valor 
			<input type="radio" name="rd_qtdevalor" checked="checked" value="ambos" />Ambos
		</td>
	</tr>
	<?php else: ?>
		<input type="hidden" name="rd_qtdevalor" checked="checked" value="qtde" />
	<?php endif; ?>
	<tr>
		<td class="SubtituloTabela" ></td>
		<td class="SubtituloTabela" >
			<input type="hidden" name="indid" value="<?php echo $indid ?>"/>
			<input type="button" name="btn_exibir" value="Exibir" onclick="exibirGraficoMeta()" />
			<input type="button" name="btn_salvar_view" value="Definir Visualização Padrão" onclick="salvarExibicaoGraficoMeta()" />
		</td>
	</tr>
	<tr>
		<td width="30%" class="SubTituloDireita" ></td>
		<td>
			<div style="width:100%;height:100%;background-color:#FF0000" id="grafico_meta" >
				<div id="grafico_meta" ></div>
			</div>
		</td>
	</tr>
<?php else: ?>
	<tr>
		<td class="SubtituloTabela bold center">Não existe(m) meta(s) para este indicador.</td>
	</tr>
<?php endif; ?>
</table>
</form>