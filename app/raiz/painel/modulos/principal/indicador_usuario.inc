<?php

if ($_REQUEST['ajaxAddMeusIndicadores']) {
	header('content-type: text/html; charset=ISO-8859-1');
	AddMeusIndicadores($_REQUEST['acao'],$_REQUEST['indid']);
	exit;
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';

$titulo = "Meus Indicadores";
monta_titulo( $titulo, '&nbsp;' );
?>
<style type="text/css">	
	/*Drag adn Drop Google*/
	
		.groupWrapper
	{
		width: 49%;
		float: left;
		margin-right: 5px;
		min-height: 400px;
	}
		
	.groupItem
	{
		margin-left: 15px;
		margin-bottom: 20px;
		border: #e0e0e0 solid 2px;
		font-size: 12px;
		font-family: Verdana,Sans-serif;
	}
	.groupItem .itemHeader
	{
		text-align:center;
		font-size: 12px;
		font-family: Verdana,Sans-serif;
		line-height: 28px;
		background-color: #e0e0e0;
		color: #000;
		cursor: move;
		font-weight: bold;
		font-size: 16px;
		height: 28px;
		position: relative;
		width: 100%;
	}
	
	.groupItem .itemHeader a
	{
		text-align:center;
		position: absolute;
		right: 10px;
		top: 0px;
		font-weight: normal;
		font-size: 11px;
		text-decoration: none;
	}
	.sortHelper
	{
		font-size: 12px;
		font-family: Verdana,Sans-serif;
		border: 3px dashed #666;
		width: auto !important;
	}
	.groupWrapper p
	{
		font-size: 12px;
		font-family: Verdana,Sans-serif;
		height: 1px;
		overflow: hidden;
		margin: 0;
		padding: 0;
		width: 100%;
	}
	.itemContent{
		text-align:left;
		font-size: 12px;
		font-family: Verdana,Sans-serif;
		height:100%;
		width: 100%;
		background-color: #FFFFFF;
	}
	
	.div_editar{
		width:160px;
		height:80px;
		border: solid 1px #000000;
		background-color: #FFFFFF;
		position:absolute;
		z-index:999999;
		margin-left:50px;
	}
</style>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr bgcolor="#DCDCDC" >
	<td style="text-align: left;font-weight: bold;" >[ Mapa ] | [ Busca ] | [ Adicionar Box ] | Etc</td>
</tr>
<tr>
	<td  style="text-align: center;" >
	<!-- Início da div que possui o box flutuante  -->
	<div id="conteudo_flutuante" class="conteudo_flutuante" style="width:100%" >
	
		<!-- Início da div 'coluna 1' que possui o box  -->
		<div id="sort1" class="groupWrapper">
			<?php exibeNovosIndicadores(); ?>
		<!-- Fim da div 'coluna 1' que possui o box  -->
		</div>
		
		<!-- Início da div 'coluna 2' que possui o box  -->
		<div id="sort2" class="groupWrapper">
		<!-- Fim da div 'coluna 2' que possui o box  -->
		</div>
	
	<!-- Fim da div que possui o box flutuante  -->
	</div>
	</td>
</tr>
</table>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
<script type="text/javascript" src="/includes/JQuery/interface.js"></script>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(
	function () {
		jQuery('div.groupWrapper').Sortable(
			{
				accept: 'groupItem',
				helperclass: 'sortHelper',
				activeclass : 	'sortableactive',
				hoverclass : 	'sortablehover',
				handle: 'div.itemHeader',
				tolerance: 'pointer',
				onChange : function(ser)
				{
				},
				onStart : function()
				{
					jQuery.iAutoscroller.start(this, document.getElementsByTagName('body'));
				},
				onStop : function()
				{
					jQuery.iAutoscroller.stop();
				}
			}
		);
	}
);

function salvaColunas(s){
			 
			serial = jQuery.SortSerialize(s);
			// Faz uma requisição ajax, passando o parametro 'ordid', via POST
			var req = new Ajax.Request('painel.php?modulo=principal/eixostemas&acao=A', {
					        method:     'post',
					        parameters: '&salvaParametros=1&' + serial.hash,
					        onComplete: function (res)
					        {	
								alert('Operação Realizada com Sucesso!');
								div.innerHTML = res.responseText;
					        }
					  });
		}

function removerMeuIndicador(indid){

	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: "ajaxAddMeusIndicadores=true&acao=remove&indid=" + indid,
			asynchronous: false,
			onComplete: function(resp){
				document.getElementById('tr_' + indid).style.display = 'none';
				if(resp.responseText != "ok"){
					alert("Não foi possível remover o indicador!");
				}
			},
			onLoading: function(){
			}
	});
	
}

function editarMeuIndicador(indid){
	document.getElementById('div_editar_' + indid).style.display = '';
	document.getElementById('acao_editar_' + indid).setAttribute("onclick","escondeEditarIndicador(" + indid + ")");	
}
function escondeEditarIndicador(indid){
	document.getElementById('div_editar_' + indid).style.display = 'none';
	document.getElementById('acao_editar_' + indid).setAttribute("onclick","editarMeuIndicador(" + indid + ")");
}

</script>