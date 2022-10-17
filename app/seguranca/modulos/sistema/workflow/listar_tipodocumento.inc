<?php

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

$db->cria_aba($abacod_tela,$url,$parametros);
?>
<br>
<?
$titulo_modulo='Cadastro de Estados';
monta_titulo($titulo_modulo,'');

// variáveis que alimentam o formulário
$sisid = (integer) $_REQUEST['sisid'];
$tpdid = (integer) $_REQUEST['tpdid'];
?>

<br>
<form method="POST" name="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td align='right' class="SubTituloDireita">Sistema:</td>
		<td><?php		
		//montado a combo do sistema
		$sql_sistema = "
							select sisid as codigo, sisdsc as descricao
							from seguranca.sistema
							where sisstatus = 'A'
						";
		
		$db->monta_combo("sisid", $sql_sistema, "S", "Selecione",	"sisSelecionado", '', '', '', '','cb_sistema');
		
		?></td>
	</tr>
</table>

<br>

<?php
//monta a tabela com os tipos de documentos cadastrados

if ($sisid) $wh = " sistema.sisid=".$sisid." ";
if ($wh) $wh = " where ".$wh;
$sql = "
				SELECT
				'<img
				border=\"0\"
				src=\"../imagens/alterar.gif\"
				title=\"Alterar Documento\"
				onclick=\"alterarDoc( ' || tipodocumento.tpdid || ')\">'
				as acao,
				
				tipodocumento.tpddsc,
				sistema.sisdsc
				FROM 
				workflow.tipodocumento
				inner join seguranca.sistema using (sisid)
				". $wh;

$cabecalho = array( 'Ações', 'Tipo Documento','Sistema');
$db->monta_lista( $sql, $cabecalho, 200, 20, '', '', '' );
?>

</form>

<script><!--

	function alterarDoc(tpdid){
		location.href='seguranca.php?modulo=sistema/workflow/cadastrar_estado&acao=A&tpdid='+tpdid;
	}
	
	function sisSelecionado()	
	{			
		document.formulario.submit();		
	}
--></script>