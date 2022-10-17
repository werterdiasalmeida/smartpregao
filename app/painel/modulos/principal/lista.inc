<?

//Ajax
if ($_REQUEST['filtraAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	if( isset($_REQUEST['secid'])){
		filtraAcao($_REQUEST['secid']);
	}elseif( isset($_REQUEST['dirid'])){
		filtraAtributos($_REQUEST['dirid']);
	}elseif( isset($_REQUEST['exiid']) && !isset($_REQUEST['driid'])){
		echo "<primeiro>";
			filtraDiretrizes($_REQUEST['exiid']);
		echo "</primeiro>";
		echo "<segundo>";
			filtraObjetivos(null,false,$_REQUEST['exiid']);
		echo "</segundo>";
	}elseif( isset($_REQUEST['driid'])){
		filtraObjetivos($_REQUEST['driid'],false,$_REQUEST['exiid']);
	}
	exit;
}  

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
// atribuindo as permissões
$permissoes = verificaPerfilPainel();
echo montarAbasArray($permissoes['menu'], $_SERVER['REQUEST_URI']);

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
			echo "<script>alert('Registro removido com sucesso.');window.location='?modulo=principal/lista&acao=A&1=1';</script>";
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
		if($_REQUEST['intid'] || $_SESSION['painel']['intid']) {
			$filtroprocesso[] = "i.intid=".$_REQUEST['intid']."";
			$_SESSION['painel']['intid'] = $_REQUEST['intid'];
		}
		if($_REQUEST['oppid'] || $_SESSION['painel']['oppid']) {
			$filtroprocesso[] = "o.oppid=".$_REQUEST['oppid']."";
			$_SESSION['painel']['oppid'] = $_REQUEST['oppid'];
		}
		if($_REQUEST['secid'] || $_SESSION['painel']['secid']) {
			$filtroprocesso[] = "i.secid=".$_REQUEST['secid']."";
			$_SESSION['painel']['secid'] = $_REQUEST['secid'];
		}
		if($_REQUEST['dirid'] || $_SESSION['painel']['dirid']) {
			$filtroprocesso[] = "di.dirid=".$_REQUEST['dirid']."";
			$_SESSION['painel']['dirid'] = $_REQUEST['dirid'];
		}
		if($_REQUEST['atrid'] || $_SESSION['painel']['atrid']) {
			$filtroprocesso[] = "at.atrid=".$_REQUEST['atrid']."";
			$_SESSION['painel']['atrid'] = $_REQUEST['atrid'];
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
		if($_REQUEST['perid'] || $_SESSION['painel']['perid']) {
			$filtroprocesso[] = "i.perid=".$_REQUEST['perid']."";
			$_SESSION['painel']['perid'] = $_REQUEST['perid'];
		}
		if($_REQUEST['obiid'] || $_SESSION['painel']['obiid']) {
			$filtroprocesso[] = "ob.obiid=".$_REQUEST['obiid']."";
			$_SESSION['painel']['obiid'] = $_REQUEST['obiid'];
		}
		
		if($_REQUEST['driid'] || $_SESSION['painel']['driid']) {
			$filtroprocesso[] = "dr.driid=".$_REQUEST['driid']."";
			$_SESSION['painel']['driid'] = $_REQUEST['driid'];
		}
		
		if($_REQUEST['exiid'] || $_SESSION['painel']['exiid']) {
			$filtroprocesso[] = "ei.exiid=".$_REQUEST['exiid']."";
			$_SESSION['painel']['exiid'] = $_REQUEST['exiid'];
		}
		if($_REQUEST['indpublicado'] || $_SESSION['painel']['indpublicado']) {
			if($_REQUEST['indpublicado'] != 'ambos' && $_SESSION['painel']['indpublicado'] != 'ambos') $filtroprocesso[] = "i.indpublicado=".$_REQUEST['indpublicado']."";
			$_SESSION['painel']['indpublicado'] = $_REQUEST['indpublicado'];
		}
		
		if($_REQUEST['indpublico'] || $_SESSION['painel']['indpublico']) {
			if($_REQUEST['indpublico'] != 'ambos' && $_SESSION['painel']['indpublico'] != 'ambos') $filtroprocesso[] = "i.indpublico=".$_REQUEST['indpublico']."";
			$_SESSION['painel']['indpublico'] = $_REQUEST['indpublico'];
		}
		
		if($_REQUEST['colid'] || $_SESSION['painel']['colid']) {
			if($_REQUEST['colid'] != 'ambos' && $_SESSION['painel']['colid'] != 'ambos') $filtroprocesso[] = "i.colid=".$_REQUEST['colid']."";
			$_SESSION['painel']['colid'] = $_REQUEST['colid'];
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
if($_SESSION['painel']['intid']) {
	$filtroprocesso[] = "i.intid=".$_SESSION['painel']['intid']."";
}
if($_SESSION['painel']['oppid']) {
	$filtroprocesso[] = "o.oppid=".$_SESSION['painel']['oppid']."";
}
if($_SESSION['painel']['secid']) {
	$filtroprocesso[] = "i.secid=".$_SESSION['painel']['secid']."";
}
if($_SESSION['painel']['dirid']) {
	$filtroprocesso[] = "di.dirid=".$_SESSION['painel']['dirid']."";
}
if($_SESSION['painel']['atrid']) {
	$filtroprocesso[] = "at.atrid=".$_SESSION['painel']['atrid']."";
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
if($_SESSION['painel']['perid']) {
	$filtroprocesso[] = "i.perid=".$_SESSION['painel']['perid']."";
}

if($_SESSION['painel']['obiid']) {
	$filtroprocesso[] = "ob.obiid=".$_SESSION['painel']['obiid']."";
}
if($_SESSION['painel']['driid']) {
	$filtroprocesso[] = "dr.driid=".$_SESSION['painel']['driid']."";
}
if($_SESSION['painel']['exiid']) {
	$filtroprocesso[] = "ei.exiid=".$_SESSION['painel']['exiid']."";
}
if(!$_SESSION['painel']['indpublicado']) $_SESSION['painel']['indpublicado'] = 'true';
if($_SESSION['painel']['indpublicado'] && $_SESSION['painel']['indpublicado'] != 'ambos') {
	$filtroprocesso[] = "i.indpublicado=".$_SESSION['painel']['indpublicado']."";
}
if(!$_SESSION['painel']['colid']) $_SESSION['painel']['colid'] = 'ambos';
if($_SESSION['painel']['colid'] && $_SESSION['painel']['colid'] != 'ambos') {
	$filtroprocesso[] = "i.colid=".$_SESSION['painel']['colid']."";
}

$filtroprocesso[] = "i.indstatus = 'A'" ;
array_unique($filtroprocesso);
?>

<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
<input type="hidden" name="evento" id="evento" value="I">
<input type="hidden" name="hidden_indid" id="hidden_indid" value="<?= $indid ?>">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td align='right' class="SubTituloDireita" width="25%">Identificador do Indicador:</td>
	<td><?= campo_texto('indid','N','S','',15,15,'############','','','','',"id='indid'",'',$_REQUEST['indid']); ?></td>
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
	<td><?= $db->monta_combo('cliid',$sql,'S','Selecione...','','','','','S'); ?></td>
</tr>  
<tr>
	<?php 
		$sql= "SELECT 
			intdesc AS descricao, 
			intid AS codigo
		FROM 		
			painel.indicadortipo
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Tipo Indicador:</td>
    <td><?=$db->monta_combo('intid',$sql,'S','Selecione...','','','','','N',"intid","",$_SESSION['painel']['intid']);?></td>
</tr>
<tr>
	<?php 
		$sql= "SELECT
			oppid AS codigo, 
			oppdsc AS descricao 
		FROM 		
			painel.objetivo_ppa
		WHERE
			oppstatus = 'A'
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Objetivo PPA:</td>
    <td><?=$db->monta_combo('oppid',$sql,'S','Selecione...',"",'','','','N','oppid','',$_SESSION['painel']['oppid']);?></td>
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
    <td align='right' class="SubTituloDireita">Secretaria/Autarquia Executora:</td>
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
						acastatus = 'A'
					and
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
					where
						acastatus = 'A'
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
		WHERE regstatus = 'A' order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Regionalização:</td>
    <td><?=$db->monta_combo('regid',$sql,'S','Selecione...','','','','','N',"regid","",$_SESSION['painel']['regid']);?></td>
</tr> 
<tr>
	<?php 
		$sql= "SELECT 
			perdsc AS descricao, 
			perid AS codigo
		FROM 
			painel.periodicidade
		WHERE perstatus = 'A'
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Periodicidade:</td>
    <td><?=$db->monta_combo('perid',$sql,'S','Selecione...','','','','','N',"perid","",$_SESSION['painel']['perid']);?></td>
</tr>

<tr>
	<?php 
		$sql= "SELECT 
			dirdsc AS descricao, 
			dirid AS codigo
		FROM 		
			painel.direitos
		order by
			 1	 
		";
	?>
    <td align='right' class="SubTituloDireita">Direitos:</td>
    <td>
    	<?
    		$dirid = $dirid ? $dirid : $_SESSION['painel']['dirid'];
    		echo $db->monta_combo('dirid',$sql,'S','Selecione...',"filtraAtributos",'','','450','N','','');
//    		if (isset($_SESSION['painel']['dirid'])){
//    			echo "<script> filtraAtributos(".$_SESSION['painel']['dirid'].") </script>";
//    		}
    	?>
    </td>
</tr> 
<tr>
	<?php 
		$sql = "SELECT 
			atrdsc AS descricao, 
			atrid AS codigo
		FROM 		
			painel.atributos";
		
		$sql .= !empty($dirid)? " WHERE dirid = '{$dirid}' order by 1;": " order by 1;";

	?>
    <td align='right' class="SubTituloDireita">Atributos:</td>
    <td id='td_atributos'><?=$db->monta_combo('atrid',$sql,'S','Selecione...',"",'','','450','N','','',$_SESSION['painel']['atrid']);?></td>
</tr>

<tr>
	<?php 
		$sql= "SELECT 
			exidsc AS descricao, 
			exiid AS codigo
		FROM 		
			painel.eixoindicador
		order by
			 2	 
		";
	?>
    <td align='right' class="SubTituloDireita">PNDH-3 Eixo:</td> 
    <td>
    	<?
    		$exiid = $exiid ? $exiid : $_SESSION['painel']['exiid'];
    		echo $db->monta_combo('exiid',$sql,'S','Selecione...',"filtraDiretrizes",'','','450','N','','');
    	?>
    </td>
</tr>

<tr>
	<?php 
		$sql= "SELECT 
			dridsc AS descricao, 
			driid AS codigo
		FROM 		
			painel.diretrizindicador
		";
		
		$sql .= !empty($exiid)? " WHERE exiid = '{$exiid}' order by 2;": " order by 2;";
	?>
    <td align='right' class="SubTituloDireita">PNDH-3 Diretriz:</td>
    <td  id='td_diretrizes'>
    	<?
    		$driid = $driid ? $driid : $_SESSION['painel']['driid'];
    		echo $db->monta_combo('driid',$sql,'S','Selecione...',"filtraObjetivos",'','','450','N','','',$_SESSION['painel']['driid']);
    	?>
    </td>
</tr>

<tr>
	<?php 
		$sql= "SELECT 
			o.obidsc AS descricao,
			o.obiid AS codigo
		FROM 		
			painel.objetivoindicador o
		";
		$sql .= empty($driid) && !empty($exiid) ? " JOIN painel.diretrizindicador d ON d.driid = o.driid AND d.exiid = {$exiid} ": "";
		$sql .= !empty($driid)? " WHERE driid = '{$driid}' order by 2;": " order by 2;";
	?>
    <td align='right' class="SubTituloDireita">PNDH-3 Objetivo:</td>
    <td id='td_objetivos'> 
    	<?
    		echo $db->monta_combo('obiid',$sql,'S','Selecione...','','','','450','N','','',$_SESSION['painel']['obiid']);
    	?>
    </td>

    </td>
</tr>

<?php
$arrPerfil = pegaPerfil();
if(PAINEL_PERFIL_CONSULTA == $arrPerfil[0]){
	$_SESSION['painel']['indpublicado']='true';
} else {
?>
	<tr>
	    <td align='right' class="SubTituloDireita">Indicador publicado:</td>
	    <td><input type="radio" name="indpublicado" value="ambos" <? echo (($_SESSION['painel']['indpublicado']=='ambos')?'checked':''); ?> > Todos <input type="radio" name="indpublicado" value="true" <? echo (($_SESSION['painel']['indpublicado']=='true')?'checked':''); ?> > Sim <input type="radio" name="indpublicado" value="false" <? echo (($_SESSION['painel']['indpublicado']=='false')?'checked':''); ?> > Não</td>
	</tr>
	<tr>
	    <td align='right' class="SubTituloDireita">Módulo Público:</td>
	    <td>
	    	<input type="radio" name="indpublico" value="ambos" <? echo (($_SESSION['painel']['indpublico']!='true') && ($_SESSION['painel']['indpublico']!='false')?'checked':''); ?> > Todos 
	    	<input type="radio" name="indpublico" value="true"  <? echo (($_SESSION['painel']['indpublico']=='true') ?'checked':''); ?>  > Sim 
	    	<input type="radio" name="indpublico" value="false" <? echo (($_SESSION['painel']['indpublico']=='false')?'checked':''); ?> > Não
	    </td>
	</tr>
<?php
}
?>
<tr>
    <td align='right' class="SubTituloDireita">Coleta:</td>
    <td><input type="radio" name="colid" value="ambos" <? echo (($_SESSION['painel']['colid']=='ambos')?'checked':''); ?> > Todos <input type="radio" name="colid" value="2" <? echo (($_SESSION['painel']['colid']=='2')?'checked':''); ?> > Automática <input type="radio" name="colid" value="1" <? echo (($_SESSION['painel']['colid']=='1')?'checked':''); ?> > Manual</td>
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
$sql = "SELECT  DISTINCT '<center><a href=\"painel.php?modulo=principal/cadastro&acao=A&indid='||i.indid||'&evento=A\"><img src=\"/imagens/alterar.gif \" border=0 title=\"Visualizar\"></a> '||".$permissoes['condicaolista']."||'</center>',
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
			    CASE WHEN indcumulativo = 'S' THEN 'Sim'  
			    	 WHEN indcumulativo = 'N' THEN 'Não' 
			    	 WHEN indcumulativo = 'A' THEN 'Por ano' END as indcumulativo,
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
	    LEFT JOIN painel.indicadoratributos  ia ON ia.indid = i.indid
	    LEFT JOIN painel.atributos  at ON at.atrid = ia.atrid
	    LEFT JOIN painel.direitos di ON di.dirid = at.dirid
	    LEFT JOIN painel.indicadorobjetivoindicador  io ON io.indid = i.indid
	    LEFT JOIN painel.objetivoindicador  ob ON ob.obiid = io.obiid
	    LEFT JOIN painel.diretrizindicador dr ON dr.driid = ob.driid
	    LEFT JOIN painel.eixoindicador ei ON ei.exiid = dr.exiid
	    LEFT JOIN painel.objetivo_ppa o ON o.oppid = i.oppid
	    
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
//	if(!secid){
//		return false;
//	}
	var destino = document.getElementById("td_acao");
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: "filtraAjax=true&" + "secid=" + secid,
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

function filtraAtributos(dirid) {
//	if(!dirid){
//		return false;
//	}
	var destino = document.getElementById("td_atributos");
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: "filtraAjax=true&" + "dirid=" + dirid,
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

function filtraDiretrizes(exiid) {
//	if(!exiid){
//		return false;
//	}
	var destino1 = document.getElementById("td_diretrizes");
	var destino2 = document.getElementById("td_objetivos");
	
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: "filtraAjax=true&" + "exiid=" + exiid,
			asynchronous: false,
			onComplete: function(resp) {
				if(destino1 && destino2) {
					
					
					destino1.innerHTML = pegaRetornoAjax('<primeiro>','</primeiro>', resp.responseText);
					destino2.innerHTML = pegaRetornoAjax('<segundo>','</segundo>', resp.responseText);
				} 
			},
			onLoading: function(){
				destino.innerHTML = 'Carregando...';
			}
		});
}

function filtraObjetivos(driid) {
//	if(!driid){
//		return false;
//	}
	var destino = document.getElementById("td_objetivos");
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: "filtraAjax=true&" + "driid=" + driid + "&exiid="+document.getElementsByName('exiid')[0].value,
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
	document.getElementById('intid').value = '';
	document.getElementById('oppid').value = '';
	document.getElementById('regid').value = '';
	document.getElementById('perid').value = '';
	document.formulario.submit();
}

function graficohistorica(indid)
{
	var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/painel/painel.php?modulo=principal/graficohistorica&acao=A&indid='+indid,'blank','height=800,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
	janela.focus();
}

</script>