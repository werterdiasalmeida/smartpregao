<?php
unset($_SESSION['painel']['metid']);
include (APPRAIZ.'www/painel/_funcoes_metas.php');

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['requisicaoAjax']();
	exit();
}

if($_POST['requisicao']){
	$_POST['requisicao']();
}

// monta cabe�alho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = "Metas do Indicador";
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
</style>

<form name="formulario_metas" id="formulario_metas"  method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita" >Selecione a Secretaria:</td>
			<td id="td_combo_secid" >
				<?php 
				$secid = $_POST['secid'];
				$sql = "select 
							secid as codigo,
							secdsc as descricao
						from
							painel.secretaria
						where
							secstatus = 'A'";
				$db->monta_combo('secid',$sql,'S','Selecione a Secretaria','filtrarAcao','');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Selecione a A��o:</td>
			<td id="td_combo_acaid" >
				<?php 
				$acaid = $_POST['acaid'];
				if($secid){
					$habilitado = "S";
					$sql = "select 
								acaid as codigo,
								acadsc as descricao
							from
								painel.acao
							where
								acastatus = 'A'
							and
								acaid in ( select acaid from painel.acaosecretaria where secid = $secid)";
				}else{
					$habilitado = "N";
					$sql = array();
				}
				$db->monta_combo('acaid',$sql,$habilitado,'Selecione a A��o','filtrarIndicador','');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Selecione o Indicador:</td>
			<td id="td_combo_indid" >
				<?php 
				$indid = $_POST['indid'];
				if($secid && $acaid){
					$habilitado = "S";
					$sql = "select 
								indid as codigo,
								indid || ' - ' || indnome as descricao
							from
								painel.indicador
							where
								indstatus = 'A'
							and
								secid = $secid
							and
								acaid = $acaid";
				}else{
					$habilitado = "N";
					$sql = array();
				}
				$db->monta_combo('indid',$sql,$habilitado,'Selecione o Indicador','','');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Meta:</td>
			<td><?$metdesc = $_POST['metdesc'] ; echo campo_texto('metdesc','N',"S",'',60,80,'','');?></td>
		</tr>
		<tr>
			<td class="SubtituloTabela" ></td>
			<td class="SubtituloTabela" >
				<input type="hidden" name="requisicao" value="pesquisarMetas" />
				<input type="hidden" name="metid" value="" />
				<input type="button" id="btn_pesquisar" value="Pesquisar" onclick="pesquisarMetas()" />
				<input type="button" id="btn_listar_todos" value="Listar Todos" onclick="listarTodasMetas()" />
			</td>
		</tr>
	</table>
</form>
<?php
$arrWhere[] = "metstatus = 'A'";
$arrWhere[] = "indstatus = 'A'";

if($_POST['secid']){
	$arrWhere[] = "ind.secid = {$_POST['secid']}";
}
if($_POST['acaid']){
	$arrWhere[] = "ind.acaid = {$_POST['acaid']}";
}
if($_POST['indid']){
	$arrWhere[] = "ind.indid = {$_POST['indid']}";
}
if($_POST['metdesc']){
	$arrWhere[] = "removeacento(upper(met.metdesc)) like removeacento(('%".strtoupper($_POST['metdesc'])."%'))";
}

$sql = "select
			'<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarMeta(\'' || met.metid || '\')\" /> <img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirMeta(\'' || met.metid || '\')\" />' as acao,
			met.metdesc,
			ind.indid || ' - ' || ind.indnome as indnome,
			(select secdsc from painel.secretaria sec where sec.secid = ind.secid) as secdsc,
			(select acadsc from painel.acao aca where aca.acaid = ind.acaid) as acadsc,
			per.perdsc,
			met.metid,
			ind.indid
		from
			painel.metaindicador met
		inner join
			painel.indicador ind ON ind.indid = met.indid
		inner join
			painel.periodicidade per ON per.perid = met.perid
		".($arrWhere ? " where ".implode(" and ",$arrWhere) : "")."
		order by
			metdesc,
			indnome";
$arrDados = $db->carregar($sql);
if($arrDados){
	foreach($arrDados as $dado){
		
		unset($arrEstid);
		unset($arrEstdesc);
		unset($arrAcadsc);
		
		$sqlEstrategia = "	select 
								estid,
								estdesc
							from
								painel.estrategiameta
							where
								metid = {$dado['metid']}
							and
								eststatus = 'A'";
		$arrEstrategias = $db->carregar($sqlEstrategia);
		if($arrEstrategias){
			foreach($arrEstrategias as $estrategia){
				$arrEstid[]   = $estrategia['estid'];
				$arrEstdesc[] = $estrategia['estdesc'];
			}
		}
		if($arrEstid){
			$sqlAcao = "	select 
								acadsc
							from
								painel.acao aca
							inner join
								painel.acaoestrategia ace ON ace.acaid = aca.acaid
							where
								estid in (".implode(",",$arrEstid).")
							and
								acastatus = 'A'";
			$arrAcao = $db->carregar($sqlAcao);
			if($arrAcao){
				foreach($arrAcao as $acao){
					$arrAcadsc[] = $acao['acadsc'];
				}
			}
		}
		
		$dados_array[] = array(	"acao" 			=> 	$dado['acao'],   
							 	"metdesc" 		=> 	$dado['metdesc'],   
							 	"indnome" 		=> 	$dado['indnome'],  
								"secdsc" 		=> 	$dado['secdsc'],
								"acadsc" 		=> 	$dado['acadsc'],
							 	"perdsc" 		=> 	$dado['perdsc'],  
							 	"estrategia" 	=> 	(is_array($arrEstdesc) ? implode(",",$arrEstdesc) : "N/A"),  
							 	"acoes" 		=> 	(is_array($arrAcadsc) ? implode(",",$arrAcadsc) : "N/A")
			   		   );
	}
}
$cabecalho = array("A��es", "Meta", "Indicador", "Secretaria","A��o","Periodicidade da Meta","Estrat�gias","A��es Estrat�gicas");
$db->monta_lista_array($dados_array,$cabecalho,100,5,'N','center',$par2);
?>