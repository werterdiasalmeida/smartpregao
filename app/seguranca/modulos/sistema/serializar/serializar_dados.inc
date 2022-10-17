<?php
include APPRAIZ."includes/classes/Serialize.class.inc";
$s = new Serialize();

if($_GET['requisicaoAjax']){
	if($_GET['echoAjax'])
		echo $s->$_GET['requisicaoAjax']($_GET['parametrosAjax']);
	else
		$s->$_GET['requisicaoAjax']($_GET['parametrosAjax']);
	die;
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = "Serializar Dados";
monta_titulo( $titulo, '&nbsp' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript">
$(document).ready(function() {
	
	//Início - Combo Schema
	$('#btn_hdn_schema_1').click(function() {
	 	$.ajax({
			url: 'seguranca.php?modulo=sistema/serializar/serializar_dados&acao=A',
			data: "echoAjax=true&requisicaoAjax=viewCampoBuscaSchema&parametrosAjax=search_schema_1",
			success: function(data) {
				$('#span_schema1').html(data);
			}
		});
	});
	//Fim - Combo Schema
});

function buscaPorNome(obj){
	alert($("#"+obj.id).attr("tipo"));
}

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2">Selecione o(s) campo(s) para serealizar.</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" >Schema:</td>
	<td><span id="span_schema1"><?$db->monta_combo('schema_1',$s->getSchemas(),'S','','','','','','','schema_1') ?> <? echo $s->viewButton(array('id' => 'btn_hdn_schema_1' , 'name' => 'btn_hdn_schema_1' , 'value' => 'Buscar')) ?><span></td>
</tr>
</table>