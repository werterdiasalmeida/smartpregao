<?php 
$titulo = "Período";
monta_titulo( $titulo,"&nbsp;");

if($_REQUEST['dpeid']){
	$sql = "select
				*
			from
				painel.detalheperiodicidade
			where
				dpeid = {$_REQUEST['dpeid']}";
	$dados = $db->carregar($sql);
	extract($dados);
}

if($_REQUEST['dpedsc'] && $_REQUEST['dpedatainicio'] && $_REQUEST['dpedatafim']){
	
	$data = explode('/',$_REQUEST['dpedatainicio']);
	$dpedatainicio = "'$data[2]-$data[1]-$data[0]'";
	
	if($_REQUEST['dpeid']){
		$sql = "update 
					painel.detalheperiodicidade
				set
					dpedsc = '{$_REQUEST['dpedsc']}',
					dpedatainicio = '$dpedatainicio',
					dpedatafim = '$dpedatafim',
					dpemesref = $dpemesref,
					dpeanoref = $dpeanoref
				where
					dpeid = {$_REQUEST['dpeid']}";
	}
	
	//continuar ...
}

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST" name="formulario">

<input type="hidden" id="perid" name="perid" value="<?=$_GET['perid'] ?>" >

<input type="hidden" id="perid" name="perid" value="<?=$dpeid?>" >

<table style="border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
    <td align='right' class="SubTituloDireita">Descrição:</td>
    <td><?=campo_texto('dpedsc','S','S','',60,100,'','','','','','id="dpedsc"','',$dpedsc);?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Data Início:</td>
    <td><?=campo_data('dpedatainicio','S','S','','','','');?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Data Fim:</td>
    <td><?=campo_data('dpedatafim','S','S','','','','');?></td>
</tr>
<tr>
    <td colspan="2" style="text-align: center;" class="SubTituloDireita"><input type="button" onclick="salvar();" value="Salvar" /> <input type="button" value="Cancelar" onclick="window.close();" /></td>
</tr>
</table>
</form>
<script>
	var d = document;
	
	function salvar(){
		var perid = d.getElementById('perid');
		var dpedsc = d.getElementById('dpedsc');
		var dataInicio = d.getElementById('dpedatainicio');
		var dataFim = d.getElementById('dpedatafim');
		
		if(!perid.value){
			alert('Operação não Realizada!');
			window.close();
			return false;
		}
		if(!dpedsc.value){
			alert('Informe a Descrição!');
			dpedsc.focus();
			return false;
		}
		if(!dataInicio.value){
			alert('Informe a Data Início!');
			dataInicio.focus();
			return false;
		}
		if(!dataFim.value){
			alert('Informe a Data Fim!');
			dataFim.focus();
			return false;
		}
		else{
			formulario.submit();
		}
	}

</script>