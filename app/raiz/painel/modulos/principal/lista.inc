<?

//Ajax
if ($_REQUEST['filtraAcaoAjax'] && $_REQUEST['secid']) {
	header('content-type: text/html; charset=ISO-8859-1');
	filtraAcao($_REQUEST['secid']);
	exit;
}
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
// atribuindo as permissões
$permissoes = verificaPerfilPainel();
// construindo o menu
$enderecosweb = array("/painel/painel.php?modulo=inicio&acao=C"          => true,
					  "/painel/painel.php?modulo=principal/lista&acao=A" => true);

$menu[0] = array("descricao" => "Lista de Indicadores", "link"=> ($enderecosweb[$_SERVER['REQUEST_URI']])?$_SERVER['REQUEST_URI']:key($enderecosweb));
$menu[1] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=novoIndicador");
// se for super usuário não precisa mostrar quais os indicadores, pois tem acesso a todos
if(!$db->testa_superuser()) {
	$menu[2] = array("descricao" => "Meus indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&evento=M");
}

echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
// titulos da tela
$titulo = "Indicadores";
monta_titulo( $titulo, '&nbsp;' );
// analisando o evento que será executado
switch($_REQUEST['evento']){		
	// excluindo indicador selecionado
	case 'E':
		$indid = $_REQUEST['hidden_indid'];
		$sql = "SELECT sehid FROM painel.seriehistorica WHERE indid='".$indid."' AND (sehstatus='A' OR sehstatus='H')";
		$sehid = $db->pegaUm($sql);
		if($sehid) {
			echo "<script>alert('Existe(m) dado(s) cadastrado(s) neste indicador.');window.location='?modulo=inicio&acao=A';</script>";
		} else {
			$sql_D = "UPDATE painel.indicador SET indstatus = 'I' where indid = ".$indid;	
			$db->executar($sql_D);
			$db->commit();
			echo "<script>alert('Registro removido com sucesso.');window.location='?modulo=inicio&acao=A';</script>";
		}
		exit;
		break;
	// filtrando apenas aqueles indicadores destinados ao perfil	
	case 'M':
		if($permissoes['condicaomeusindicadores']) {
			$filtroprocesso[] = $permissoes['condicaomeusindicadores'];
		}
		break;
	// filtrar pelos filtros selecionados na lista
	case 'P':
		
		//Cria uma sessão de array com o nome do módulo para evitar conflito de sessões com o mesmo nome em outros módulos;
		unset($_SESSION['painel']);
		
		//filtros
		if($_REQUEST['indid'] || $_SESSION['painel']['indid']) {
			$filtroprocesso[] = "i.indid=".$_REQUEST['indid']."";
		}
		if($_REQUEST['indnome'] || $_SESSION['painel']['indnome']) {				
			$filtroprocesso[] = "upper(i.indnome) like upper('%".$_REQUEST['indnome']."%')";
			$_SESSION['painel']['indnome'] = $_REQUEST['indnome'];
		}
		if($_REQUEST['cliid'] || $_SESSION['painel']['cliid']) {
			$filtroprocesso[] = "i.cliid=".$_REQUEST['cliid']."";
			$_SESSION['painel']['cliid'] = $_REQUEST['cliid'];
		}
		if($_REQUEST['tpiid'] || $_SESSION['painel']['tpiid']) {
			$filtroprocesso[] = "i.tpiid=".$_REQUEST['tpiid']."";
			$_SESSION['painel']['tpiid'] = $_REQUEST['tpiid'];
		}
		if($_REQUEST['secid'] || $_SESSION['painel']['secid']) {
			$filtroprocesso[] = "i.secid=".$_REQUEST['secid']."";
			$_SESSION['painel']['secid'] = $_REQUEST['secid'];
		}
		if($_REQUEST['acaid'] || $_SESSION['painel']['acaid']) {
			$filtroprocesso[] = "i.acaid=".$_REQUEST['acaid']."";
			$_SESSION['painel']['acaid'] = $_REQUEST['acaid'];
		}
		if($_REQUEST['mapid'] || $_SESSION['painel']['mapid']) {
			$filtroprocesso[] = "obj.mapid=".$_REQUEST['mapid']."";
			$_SESSION['painel']['mapid'] = $_REQUEST['mapid'];
		}
		if($_REQUEST['pesid'] || $_SESSION['painel']['pesid']) {
			$filtroprocesso[] = "obj.pesid=".$_REQUEST['pesid']."";
			$_SESSION['painel']['pesid'] = $_REQUEST['pesid'];
		}
		if($_REQUEST['temid'] || $_SESSION['painel']['temid']) {
			$filtroprocesso[] = "obj.temid=".$_REQUEST['temid']."";
			$_SESSION['painel']['temid'] = $_REQUEST['temid'];
		}
		if($_REQUEST['obeid'] || $_SESSION['painel']['obeid']) {
			$filtroprocesso[] = "obj.obeid=".$_REQUEST['obeid']."";
			$_SESSION['painel']['obeid'] = $_REQUEST['obeid'];
		}
		if($_REQUEST['regid'] || $_SESSION['painel']['regid']) {
			$filtroprocesso[] = "reg.regid=".$_REQUEST['regid']."";
			$_SESSION['painel']['regid'] = $_REQUEST['regid'];
		}
		if($_REQUEST['exoid'] || $_SESSION['painel']['exoid']) {
			$filtroprocesso[] = "ex.exoid=".$_REQUEST['exoid']."";
			$_SESSION['painel']['exoid'] = $_REQUEST['exoid'];
		}
		break;
		
	case 'L':
		
		unset($_SESSION['painel']);
		break;
}

//filtros
if($_SESSION['painel']['indnome']) {				
	$filtroprocesso[] = "upper(i.indnome) like upper('%".$_SESSION['painel']['indnome']."%')";
}
if($_SESSION['painel']['cliid']) {
	$filtroprocesso[] = "i.cliid=".$_SESSION['painel']['cliid']."";
}
if($_SESSION['painel']['tpiid']) {
	$filtroprocesso[] = "i.tpiid=".$_SESSION['painel']['tpiid']."";
}
if($_SESSION['painel']['secid']) {
	$filtroprocesso[] = "i.secid=".$_SESSION['painel']['secid']."";
}
if($_SESSION['painel']['acaid']) {
	$filtroprocesso[] = "i.acaid=".$_SESSION['painel']['acaid']."";
}
if($_SESSION['painel']['mapid']) {
	$filtroprocesso[] = "obj.mapid=".$_SESSION['painel']['mapid']."";
}
if($_SESSION['painel']['pesid']) {
	$filtroprocesso[] = "obj.pesid=".$_SESSION['painel']['pesid']."";
}
if($_SESSION['painel']['temid']) {
	$filtroprocesso[] = "obj.temid=".$_SESSION['painel']['temid']."";
}
if($_SESSION['painel']['obeid']) {
	$filtroprocesso[] = "obj.obeid=".$_SESSION['painel']['obeid']."";
}
if($_SESSION['painel']['regid']) {
	$filtroprocesso[] = "reg.regid=".$_SESSION['painel']['regid']."";
}
if($_SESSION['painel']['exoid']) {
	$filtroprocesso[] = "ex.exoid=".$_SESSION['painel']['exoid']."";
}

$filtroprocesso[] = "i.indstatus = 'A'" ;
array_unique($filtroprocesso);

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
<input type="hidden" name="evento" id="evento" value="I">
<input type="hidden" name="hidden_indid" id="hidden_indid" value="<?=$indid?>">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita">Identificador do Indicador:</td>
	<td><?=campo_texto('indid','N','S','',15,15,'############','','','','',"id='indid'",'',$_REQUEST['indid']); ?></td>
</tr>
<tr>
	<? 
	$sql= "SELECT exodsc AS descricao, exoid AS codigo FROM painel.eixo ORDER BY descricao";
	?>
	<td align='right' class="SubTituloDireita">Eixo:</td>
	<td><?=$db->monta_combo('exoid',$sql,'S','Selecione...','','','','','N',"exoid","",$_SESSION['painel']['exoid']);?></td>
</tr> 
<tr>
	<td  align='right' class="SubTituloDireita">Nome Indicador:</td>
	<td><?=campo_texto('indnome','N','S','',60,100,'','',"","","","id='indnome'","",$_SESSION['painel']['indnome']);?></td>
</tr>
<tr style="display: none">
	<?php 
		$sql= "SELECT 
			clidsc AS descricao, 
			cliid AS codigo
		FROM 		
			painel.classeindicador
		order by
			descricao 
		";
	?>
	<td align='right' class="SubTituloDireita">Classe do Indicador:</td>
	<td><?=$db->monta_combo('cliid',$sql,'S','Selecione...','','','','','S');?></td>
</tr>  
<tr>
	<?php 
		$sql= "SELECT 
			tpidsc AS descricao, 
			tpiid AS codigo
		FROM 		
			painel.tipoindicador
		order by
			descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Tipo Indicador:</td>
    <td><?=$db->monta_combo('tpiid',$sql,'S','Selecione...','','','','','N',"tpiid","",$_SESSION['painel']['tpiid']);?></td>
</tr> 
<tr>
	<?php 
		$sql= "SELECT 
			secdsc AS descricao, 
			secid AS codigo
		FROM 		
			painel.secretaria 
		order by
			descricao
		";
	?>
    <td align='right' class="SubTituloDireita">Autarquia/Secretaria Executora:</td>
    <td><?=$db->monta_combo('secid',$sql,'S','Selecione...',"filtraAcao","",'','','N',"secid","",$_SESSION['painel']['secid']);?></td>
</tr> 
<tr style="display: none">
	<?php 
		$sql= "	SELECT 
					secdsc AS descricao, 
					secid AS codigo
				FROM 		
					painel.secretaria";
	?>
    <td align='right' class="SubTituloDireita">Programa:</td>
    <td><?=$db->monta_combo('prgid',$sql,'S','Selecione...','','','','','S');?></td>
</tr> 
<tr>
	<?php 
		if($_SESSION['painel']['secid']){
			$sql= "	select 
						acadsc AS descricao, 
						acaid AS codigo
					FROM 		
						painel.acao
					where
						acaid in(
								select 
									distinct acaid
								from
									painel.indicador
								where
									secid = {$_SESSION['painel']['secid']}
							)
					order by
						descricao";
		}else{
			$sql= "	SELECT 
						acadsc AS descricao, 
						acaid AS codigo
					FROM 		
						painel.acao
					order by
						 descricao";
		}
	?>
    <td align='right' class="SubTituloDireita">Ação:</td>
    <td id="td_acao" ><?=$db->monta_combo('acaid',$sql,'S','Selecione...','','','','','N',"acaid","",$_SESSION['painel']['acaid']);?></td>
</tr> 
<tr style="display: none">
	<?php 
		$sql= "SELECT 
			secdsc AS descricao, 
			secid AS codigo
		FROM 		
			painel.secretaria
		order by
			 descricao
		";
	?>
    <td align='right' class="SubTituloDireita">Projeto:</td>
    <td><?=$db->monta_combo('prjid',$sql,'S','Selecione...','','','','','S');?></td>
</tr> 
<tr style="display: none">
	<?php 
		$sql= "SELECT 
			mapdsc AS descricao, 
			mapid AS codigo
		FROM 
			painel.mapa
		order by
			 descricao 
		";
	?>
    <td align='right' class="SubTituloDireita">Mapa Estratégico:</td>
    <td><?=$db->monta_combo('mapid',$sql,'S','Selecione...','','','','','S');?></td>
</tr> 
<tr style="display: none">
	<?php 
		$sql= "SELECT 
			pesdsc AS descricao, 
			pesid AS codigo
		FROM 		
			painel.perspectiva
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Perspectiva Estratégica:</td>
    <td><?=$db->monta_combo('pesid',$sql,'S','Selecione...','','','','','S');?></td>
</tr> 
<tr style="display: none">
	<?php 
		$sql= "SELECT 
		temdsc AS descricao, 
		temid AS codigo
		FROM 		
		painel.tema		 
		";
	?>
    <td align='right' class="SubTituloDireita">Tema Estratégico:</td>
    <td><?=$db->monta_combo('temid',$sql,'S','Selecione...','','','','','S');?></td>
</tr>

<tr style="display: none">
	<?php 
		$sql= "SELECT 
		obenome AS descricao, 
		obeid AS codigo
		FROM 		
		painel.objetivoestrategico		 
		";
	?>
    <td align='right' class="SubTituloDireita">Objetivo Estratégico:</td>
    <td><?=$db->monta_combo('obeid',$sql,'S','Selecione...','','','','','S');?></td>
</tr>
<tr>
	<?php 
		$sql= "SELECT 
			regdescricao AS descricao, 
			regid AS codigo
		FROM 
			painel.regionalizacao
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Regionalização:</td>
    <td><?=$db->monta_combo('regid',$sql,'S','Selecione...','','','','','N',"regid","",$_SESSION['painel']['regid']);?></td>
</tr> 
<tr bgcolor="#cccccc">
<td></td>
	<td>
	<input type="button" class="botao" name="btassociar" value="Pesquisar" onclick="submeter('P', '<?=$indid?>');">
	<input type="button" class="botao" name="btassociar" value="Mostrar Todos" onclick="limpar();">
	</td>
</tr>
</table>
</form>
<?php
$sql = "SELECT  '<center><a href=\"painel.php?modulo=principal/cadastro&acao=A&indid='||i.indid||'&evento=A\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Visualizar\"></a> '||".$permissoes['condicaolista']."||'</center>',
				(SELECT 
					(CASE WHEN count(*)>0 THEN 
						'<center><a href=\"painel.php?modulo=principal/pc_indicador&acao=A&indid='||i.indid||'\"><img src=\"/imagens/seriehistorica_ativa.gif\" border=\"0\" title=\"Painel do Indicador\"></a></center>' 
					ELSE 
						'<center><img src=\"/imagens/seriehistorica_inativa.gif\" title=\"Não possui Série Histórica\"></center>' 
					END)
				from painel.seriehistorica 
				where indid = i.indid and sehstatus in ('A','H')) as serie,		
			    i.indid, 
			    i.indnome,
			    CASE WHEN indcumulativo = TRUE THEN 'Sim' ELSE 'Não' END as indcumulativo,
			    CASE WHEN indpublicado = TRUE THEN 'Sim' ELSE 'Não' END as indpublicado,
			    reg.regdescricao,
			    CASE WHEN dt1.tdidsc IS NULL THEN '' ELSE 'Por '||dt1.tdidsc END || CASE WHEN dt2.tdidsc IS NULL THEN ' ' ELSE ' / Por '||dt2.tdidsc END as tdidsc, 
			    s.secdsc, 
			    a.acadsc,
			    unm.unmdesc, 
			    i.indobjetivo 
	    FROM painel.indicador i 
   		LEFT JOIN painel.detalhetipoindicador dt1 ON dt1.indid = i.indid AND dt1.tdinumero = 1
   		LEFT JOIN painel.detalhetipoindicador dt2 ON dt2.indid = i.indid AND dt2.tdinumero = 2
	    LEFT JOIN painel.mapa m ON m.mapid = m.mapid 
	    LEFT JOIN painel.secretaria s ON i.secid = s.secid 
	    LEFT JOIN painel.acao a ON a.acaid = i.acaid 
	    LEFT JOIN painel.eixo ex ON ex.exoid = i.exoid 
	    LEFT JOIN painel.unidademedicao unm ON unm.unmid = i.unmid 
	    LEFT JOIN painel.regionalizacao reg ON reg.regid = i.regid 
	    ".(($filtroprocesso)?"WHERE ".implode(" AND ", $filtroprocesso):"")." 
	    ORDER BY i.indid";

$cabecalho = array("&nbsp;Ações&nbsp;&nbsp;&nbsp;&nbsp;", "Painel do Indicador", "Identificador do Indicador", "Nome", "Cumulativo", "Publicado", "Regionalização", "Detalhe", "Autarquia/Secretaria Executora" , "Ação", "Unidade de Medição", "Objetivo");

$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);

?>
<div id="erro"></div>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

function submeter(evento, indid){		
		document.getElementById('evento').value = evento;		
		document.formulario.submit();
}

function filtraAcao(secid) {
	if(!secid){
		return false;
	}
	var destino = document.getElementById("td_acao");
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: "filtraAcaoAjax=true&" + "secid=" + secid,
			asynchronous: false,
			onComplete: function(resp) {
				if(destino) {
					destino.innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				destino.innerHTML = 'Carregando...';
			}
		});
}

function alterar(evento, indid){	
	switch(evento) {
		case 'E':
			var conf = confirm('Deseja realmente excluir este indicador?')
			if(!conf) {
				return false;
			}
			break;
	
	}
	
	document.getElementById('evento').value = evento;
	document.getElementById('hidden_indid').value = indid;

	document.formulario.submit();
}

function limpar(){	
	document.getElementById('evento').value = 'L';
	document.getElementById('indid').value = '';	
	document.getElementById('exoid').value = '';
	document.getElementById('secid').value = '';
	document.getElementById('acaid').value = '';
	document.getElementById('indnome').value = '';
	document.getElementById('tpiid').value = '';
	document.getElementById('regid').value = '';
	document.formulario.submit();
}

function graficohistorica(indid)
{
	var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/painel/painel.php?modulo=principal/graficohistorica&acao=A&indid='+indid,'blank','height=800,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
	janela.focus();
}

</script>