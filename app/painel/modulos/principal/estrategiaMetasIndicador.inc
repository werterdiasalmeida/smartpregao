<?php
if(!$_SESSION['painel']['metid']){
	header("Location: painel.php?modulo=principal/listarMetasIndicador&acao=A");
}
include (APPRAIZ.'www/painel/_funcoes_metas.php');

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['requisicaoAjax']();
	exit();
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$menu[0] = array("descricao" => "Metas do Indicador", 		 "link"=> "/painel/painel.php?modulo=principal/listarMetasIndicador&acao=A");
$menu[1] = array("descricao" => "Editar Metas do Indicador", "link"=> "/painel/painel.php?modulo=principal/cadastrarMetasIndicador&acao=A");
$menu[2] = array("descricao" => "Estratégias de Meta", 		 "link"=> "/painel/painel.php?modulo=principal/estrategiaMetasIndicador&acao=A");
$menu[3] = array("descricao" => "Valores de Meta", 		 	 "link"=> "/painel/painel.php?modulo=principal/valorMetasIndicador&acao=A");
echo montarAbasArray($menu, "/painel/painel.php?modulo=principal/estrategiaMetasIndicador&acao=A");

$titulo = "Estratégias de Meta";
monta_titulo( $titulo, '&nbsp' );

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
	#div_estrategia{width:400px;height:130px;position:absolute;margin-left:150px;margin-top:-20px;z-index:0;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<a name="topo"></a>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="25%" class="SubTituloDireita" ></td>
		<td>
			<input type="button" name="btn_add_estrategia" onclick="addEstrategia()" value="Adicionar Estratégia" />
			<div id="div_estrategia">
				<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:-5px;margin-left:395px;cursor:pointer" onclick="fecharDiv('div_estrategia')" />
				<form>
					<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
						<tr>
							<td width="25%" class="SubTituloDireita" id="td_descricao" >Estratégia</td>
							<td id="td_input" ><?=campo_textarea( 'campo_descricao', 'S', 'S', '', 60, 2, 1000 );?></td>
						</tr>
						<tr>
							<td width="25%" class="SubTituloDireita" ></td>
							<td>
								<input type="hidden" id="metid" name="metid" value="<?php echo $_SESSION['painel']['metid'] ?>" />
								<input type="hidden" id="identificador" name="identificador" value="" />
								<input type="hidden" id="estid" name="estid" value="" />
								<input type="hidden" id="requisicao" name="requisicao" value="salvarEstrategia" />
								<input type="button" id="btn_salvar" value="Salvar" onclick="salvarEstrategiaAcao()" />
								<input type="button" id="btn_cancelar" value="Cancelar" onclick="fecharDiv('div_estrategia')" />
							</td>
						</tr>
					</table>
				</form>
			</div>
		</td>
	</tr>
</table>
<div id="lista_estrategia">
<?php listarEstrategias(); ?>