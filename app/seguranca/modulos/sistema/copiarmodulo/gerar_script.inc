<?php
include APPRAIZ . 'seguranca/www/_funcoes_copiar_modulo.php';

if($_GET['requisicao']){
	$_GET['requisicao']($_REQUEST);
	die;
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = "Gerar Scrip de Módulo";
monta_titulo( $titulo, '&nbsp' );
?>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.msg_erro{color:#990000}
</style>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
$(document).ready(function() {
	$('#marcar_todos_tbl').click(function() {
		if(this.checked == true){
			$("input[type=checkbox]").each(function() { 
				if(this.id != "marcar_todos_tbl")
					this.checked = true; 
			});
		}
		if(this.checked == false){
			$("input[type=checkbox]").each(function() { 
				if(this.id != "marcar_todos_tbl")
					this.checked = false;
			});
		}
	});
	
	$('#btn_gerar_script').click(function() {
		$(this).val("Carregando...");
		$(this).click(function(){alert("Aguarde... Carregando.")});
		$(this).attr("disabled","disabled");
		$("#form_sistema").attr("action","seguranca.php?modulo=sistema/copiarmodulo/gerar_script&acao=A&requisicao=donwloadScript");
		$("#form_sistema").submit();
	});
});

function exibeDadosSistema(sisid){
	if(sisid){
		$("#form_sistema").attr("action","");
		$("#form_sistema").submit();
	}
}
</script>

<form name="form_sistema" id="form_sistema"  method="post" action="" >
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td width="25%" class="SubTituloDireita" >Módulo:</td>
	<?php $sql = "select
					sisid as codigo,
					sisdsc as descricao
				 from
				 	seguranca.sistema
				 where
				 	sisstatus = 'A'
				 order by
				 	sisdsc" 
	?>
	<td><?php $combo_sisid = $_POST['combo_sisid'] ; $db->monta_combo('combo_sisid',$sql,'S','Selecione o Módulo','exibeDadosSistema','')?></td>
</tr>
<?php if($_POST['combo_sisid']): ?>
	<?php $arrDados = getDadosSistema($_POST['combo_sisid']);?>
	<tr>
		<td class="SubTituloEsquerda SubtituloTabela" colspan="2">Informações Gerais do Sistema:</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Identificador:</td>
		<td><?php echo $arrDados['sisid'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Nome:</td>
		<td><?php echo $arrDados['sisdsc'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Descrição:</td>
		<td><?php echo $arrDados['sisfinalidade'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Schema:</td>
		<td><?php echo $arrDados['sisdiretorio'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda SubtituloTabela" colspan="2">Tabelas do Sistesma:</td>
	</tr>
	<?php $arrTabelasSistema = getTabelasSistema($arrDados['sisid']);?>
	<?php if ( is_array($arrTabelasSistema) && count($arrTabelasSistema) ): ?>
		<tr>
			<td class="SubTituloEsquerda normal" colspan="2" >Selecione a(s) <a class="under" title="Tabelas de Apoio" href="#tabelaapoio">tabela(s) de apoio*</a>: </td>
		</tr>
		<tr>
			<td colspan="2" >
				<?php $arrCabecalho = array( "Marcar/Desmarcar <input type='checkbox' id='marcar_todos_tbl' />", "Nome da Tabela") ?>
				<?php echo $db->monta_lista_simples($arrTabelasSistema,$arrCabecalho,'100','50','N','95%','N',true); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda center" colspan="2">
				<input type="button" id="btn_gerar_script" value="Gerar Script" /> 
			</td>
		</tr>
		<tr>
			<td class="SubtituloTabela normal" colspan="2">
				<font class="bold">*<a class="none" name="tabelaapoio">Tabelas de Apoio:</a></font> São tabelas utilizadas pelo próprio módulo. Todos os registros das tabelas de apoio serão copiados. 
			</td>
		</tr>
	<?php else: ?>
		<tr>
			<td class="SubtituloTabela bold center" colspan="2">
				<span class="msg_erro" >Não existe(m) tabela(s) associada(s).</span>  
			</td>
		</tr>
	<?php endif; ?>
<?php endif; ?>
</table>
</form>