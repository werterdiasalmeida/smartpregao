<?php
if(!$_SESSION['painel']['metid']){
	header("Location: painel.php?modulo=principal/listarMetasIndicador&acao=A");
}
include (APPRAIZ.'www/painel/_funcoes_metas.php');

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	if($_POST['requisicaoAjax'] == "getComboPeriodoPorPerid"){
		$perid = $db->pegaUm("select perid from painel.detalheperiodicidade where dpeid = {$_POST['dpeid']}");
		getComboPeriodoPorPerid($perid,false,$_POST['dmiid']);
	}else{
		$_POST['requisicaoAjax']();
	}
	exit();
}

if($_POST['requisicao']){
	$resultado = $_POST['requisicao']();
}

$sql = "select 
			ind.indid,
			ind.indnome,
			ind.indqtdevalor,
			met.perid,
			ume.umedesc
		from 
			painel.metaindicador met
		inner join
			painel.indicador ind ON ind.indid = met.indid
		inner join
			painel.unidademeta ume ON ind.umeid = ume.umeid 
		where 
			metid = {$_SESSION['painel']['metid']}";
$arrDados = $db->pegaLinha($sql);

// monta cabe�alho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$menu[0] = array("descricao" => "Metas do Indicador", 		 "link"=> "/painel/painel.php?modulo=principal/listarMetasIndicador&acao=A");
$menu[1] = array("descricao" => "Editar Metas do Indicador", "link"=> "/painel/painel.php?modulo=principal/cadastrarMetasIndicador&acao=A");
$menu[2] = array("descricao" => "Estrat�gias de Meta", 		 "link"=> "/painel/painel.php?modulo=principal/estrategiaMetasIndicador&acao=A");
$menu[3] = array("descricao" => "Valores de Meta", 		 	 "link"=> "/painel/painel.php?modulo=principal/valorMetasIndicador&acao=A");
echo montarAbasArray($menu, "/painel/painel.php?modulo=principal/valorMetasIndicador&acao=A");

$titulo = "Valores de Meta";
monta_titulo( $titulo, $arrDados['indid']." - ".$arrDados['indnome'] );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../painel/js/metas_indicador.js"></script>
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
<?php $formatoinput = pegarFormatoInput($arrDados['indid']);
	  $formatoinput['campovalor']['mascara'] = !$formatoinput['campovalor']['mascara'] ? "###.###.###.###.###,##" : $formatoinput['campovalor']['mascara']; ?>
<script>
function editarValorMeta(dmiid,dpeid,dmiqtde,dmivalor)
{
	$("[name=dmiid]").val(dmiid);
	executarAjax("requisicaoAjax=getComboPeriodoPorPerid&dpeid=" + dpeid + "&dmiid=" + dmiid,$("#combo_dpeid"));
	$("[name=dmiqtde]").val(mascaraglobal('[<?php echo $formatoinput['mascara'] ?>]',dmiqtde));
	if($("[name=dmivalor]") && dmivalor){
		$("[name=dmivalor]").val(mascaraglobal('<?php echo $formatoinput['campovalor']['mascara'] ?>',dmivalor));
	}else{
		$("[name=dmivalor]").val("");
	}
}
</script>
<form name="formulario_valor_metas" id="formulario_valor_metas"  method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita" >Per�odo:</td>
			<td id="combo_dpeid"><?php getComboPeriodoPorPerid($arrDados['perid']) ?></td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita" ><?php echo $arrDados['umedesc']; ?>:</td>
			<td><?=campo_texto('dmiqtde','S',"S",'',20,20,$formatoinput['mascara'],'');?></td>
		</tr>
		<?php if($arrDados['indqtdevalor'] == "t"): ?>
			<tr>
				<td width="25%" class="SubTituloDireita" >Valor:</td>
				<td><?=campo_texto('dmivalor','N',"S",'',20,20,$formatoinput['campovalor']['mascara'],'');?></td>
			</tr>
		<?php endif; ?>
		<tr>
			<td width="25%" class="SubtituloTabela" ></td>
			<td class="SubtituloTabela" >
				<input type="hidden" id="dmiid" name="dmiid" value="" />
				<input type="hidden" name="requisicao" value="salvarValorMeta" />
				<input type="button" id="btn_salvar" value="Salvar" onclick="salvarValorMeta()" />
				<input type="button" id="btn_cancelar" value="Cancelar" onclick="window.location.href='painel.php?modulo=principal/listarMetasIndicador&acao=A'" />
			</td>
		</tr>
	</table>
</form>
<div id="lista_valor_meta">
<?php listarValorMetas($_SESSION['painel']['metid']); ?>
</div>
<?php if($resultado['msg']): ?>
<script>
	alert('<?php echo $resultado['msg'] ?>');
</script>
<?php endif; ?>