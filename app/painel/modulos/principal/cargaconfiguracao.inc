<?php

$indid = $_SESSION['indid'];

switch ($_POST['evento']) {
	case 'salvar':
		$cgiid = $_POST['cgiid'];
		if (empty($cgiid) || $_POST['replicar'] == 'S'){
			$sql = "INSERT 
					  INTO painel.cargaindicador
					  	(cgidsc, usucpf, regid)
					  VALUES
					  	('{$_REQUEST['cgidsc']}', '{$_SESSION['usucpf']}', '{$_SESSION['regid']}')
					  RETURNING	cgiid";
			
			$cgiid = $db->pegaUm($sql);
		}else{
			$sql = "UPDATE painel.cargaindicador 
					SET
						cgidsc = '{$_POST['cgidsc']}'
					WHERE
						cgiid = {$cgiid}";
			$db->executar($sql);
			$sql = "DELETE FROM painel.dadocargaindicador WHERE cgiid={$cgiid}";
			$db->executar($sql);
		}	
		
		foreach($_REQUEST['dcidsc'] as $dcidsc) {
			$sql = "INSERT INTO painel.dadocargaindicador(dcidsc, cgiid) VALUES ('". $dcidsc ."', ". $cgiid .")";			
			$db->executar($sql);
		}
	
		$db->commit();
		die("<script>
				alert('Opera��o realizada com sucesso!');
				window.location.href = \"?modulo=principal/cargaconfiguracao&acao=A\";
			 </script>");
	break;
	case 'excluir':
		$cgiid = $_POST['cgiid'];
		// Verificar se a config � do usu�rio na sess�o antes de deletar
		$sql = "SELECT 
					cgiid
				FROM
					painel.cargaindicador
				WHERE
					cgiid = {$cgiid} AND
					usucpf = '{$_SESSION['usucpf']}'";
		$cgiid = $db->pegaUm($sql);
		if ($cgiid){
			$sql = "DELETE FROM painel.dadocargaindicador WHERE cgiid={$cgiid}";
			$db->executar($sql);
			$sql = "DELETE FROM painel.cargaindicador WHERE cgiid = {$cgiid}";
			$db->executar($sql);
			$db->commit();
			die("<script>
					alert('Opera��o realizada com sucesso!');
					window.location.href = \"?modulo=principal/cargaconfiguracao&acao=A\";
			 	</script>");
		}
		die("<script>
				alert('Voc� n�o pode deletar essa configura��o!');
				window.location.href = \"?modulo=principal/cargaconfiguracao&acao=A\";
		 	</script>");
	break;
	case 'publicar':
		$cgiid = $_POST['cgiid'];
		$sql = "UPDATE painel.cargaindicador 
				SET
					cgipublico = true
				WHERE
					cgiid = {$cgiid}";
		$db->executar($sql);		
		$db->commit();
		die("<script>
				alert('Opera��o realizada com sucesso!');
				window.location.href = \"?modulo=principal/cargaconfiguracao&acao=A\";
		 	</script>");
	break;
	case 'despublicar':
		$cgiid = $_POST['cgiid'];
		$sql = "UPDATE painel.cargaindicador 
				SET
					cgipublico = false
				WHERE
					cgiid = {$cgiid}";
		$db->executar($sql);		
		$db->commit();
		die("<script>
				alert('Opera��o realizada com sucesso!');
				window.location.href = \"?modulo=principal/cargaconfiguracao&acao=A\";
		 	</script>");
	break;
	case 'carregar':
		$cgiid = $_POST['cgiid'];
		$sql = "SELECT
					cgidsc,
					usucpf	
				FROM
					painel.cargaindicador
				WHERE
					cgiid = {$cgiid}";
		$arDado = (array) $db->pegaLinha($sql);
		extract($arDado);
	break;
}
if ( $_REQUEST['evento'] ){
}

// monta cabe�alho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// monta abas
$db->cria_aba($abacod_tela,$url,'');

// monta titulo
$titulo = "Configura��o de Regionaliza��o";
monta_titulo( $titulo,'<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigat�rio.' );

$arrRegionalizacao = array(REGIONALIZACAO_MUN, REGIONALIZACAO_UF);
if (in_array($_SESSION['regid'], $arrRegionalizacao)):
?>
<form method="POST"  name="formulario">
	<input type="hidden" name="evento" id="evento" value="">	
	<input type="hidden" name="replicar" id="replicar" value="<?php echo (empty($usucpf) || $usucpf == $_SESSION['usucpf']) ? 'N' : 'S' ?>">	
	<input type="hidden" name="cgiid" id="cgiid" value="<?php echo $cgiid;?>">	
	<table style="border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
    		<td align='right' class="SubTituloDireita" width="30%">Descri��o:</td>
    		<td><?=campo_texto('cgidsc','S','S','',75,200,'','','','','',"id='cgidsc'");?></td>
		</tr> 
<!-- 		
		<tr>
		    <td align='right' class="SubTituloDireita">Periodicidade:</td>
		    <td>
		    	<?
		    		$sql = "SELECT
						      	dpeid as codigo,
						       	dpedsc as descricao
							FROM
						       	painel.detalheperiodicidade dsh        
							JOIN 
								painel.indicador ind ON ind.perid = dsh.perid
							WHERE
						       	dsh.dpestatus = 'A' AND
						       	ind.indid = {$indid}
							ORDER BY
						       	dpeanoref ASC, dpemesref ASC";
		    		
		    		//echo $db->monta_combo('dpeid',$sql,'S','Selecione...',"",'','',200,'S','','',$dpeid);
		    	?>
		    </td>
		</tr>
 -->		
 <?php 
switch ($_SESSION['regid']){
	case REGIONALIZACAO_MUN:
		$txtDado = "Munic�pios";	
 ?>
		<tr>
		    <td align='right' class="SubTituloDireita">Munic�pios:</td>
		    <td>
		    	<?
		    	if ($cgiid){
					$sql_atr = "SELECT 
									muncod AS codigo,
									estuf||' / '||mundescricao AS descricao,
									mundescricao,
									estuf
								FROM 
									painel.dadocargaindicador ci 		
								JOIN territorios.municipio m ON m.muncod = ci.dcidsc 
								WHERE
									ci.cgiid = {$cgiid}
								ORDER BY 2";
					
					$dcidsc = $db->carregar($sql_atr);
		    	}
				$sql = "SELECT 
							muncod AS codigo,
							estuf||' / '||mundescricao AS descricao,
							mundescricao,
							estuf
						FROM 		
							territorios.municipio
						ORDER BY 2";
						
								       
				$htmlReplace = "<b>UF:</b> {estuf}
								<br><b>Munic�pio:</b> {mundescricao}";
				$where = array(
								array("codigo" 	  => "estuf",
									  "descricao" => "UF")
							  );
				combo_popup('dcidsc',
							$sql,
		                    'Selecione o(s) Munic�pio(s)',
		                    '400x500',
		                    null,
		                    null,
		                    1,
		                    'S',
		                    false,
		                    false, 
		                    10, 
		                    400 , 
		                    null, 
		                    null, 
		                    false,
		                    $where);          
				?>
		    </td>
		</tr>
<?php 
	break;
	case REGIONALIZACAO_UF:
		$txtDado = "Estados";	
?>	
		<tr>
		    <td align='right' class="SubTituloDireita">Estados:</td>
		    <td>
		    	<?
		    	if ($cgiid){
					$sql_atr = "SELECT 
									estuf AS codigo,
									estuf||' / '||estdescricao AS descricao
								FROM 
									painel.dadocargaindicador ci 		
								JOIN territorios.estado e ON e.estuf = ci.dcidsc 
								WHERE
									ci.cgiid = {$cgiid}
								ORDER BY 2";
					
					$dcidsc = $db->carregar($sql_atr);
		    	}
				$sql = "SELECT 
							estuf AS codigo,
							estuf||' / '||estdescricao AS descricao
						FROM 		
							territorios.estado
						ORDER BY 2";
						
								       
//				$htmlReplace = "<b>UF:</b> {estuf}
//								<br><b>Munic�pio:</b> {mundescricao}";
//				$where = array(
//								array("codigo" 	  => "estuf",
//									  "descricao" => "UF")
//							  );
				combo_popup('dcidsc',
							$sql,
		                    'Selecione o(s) Estados(s)',
		                    '400x500',
		                    null,
		                    null,
		                    1,
		                    'S',
		                    false,
		                    false, 
		                    10, 
		                    400 , 
		                    null, 
		                    null, 
		                    false,
		                    $where);          
				?>
		    </td>
		</tr>
<?php 
	}
?>			
		<tr bgcolor="#cccccc">
			<td>&nbsp;</td>
			<td>
				<?php  
				if (empty($usucpf) || $usucpf == $_SESSION['usucpf']):
				?>
					<input type="button" class="botao" name="btngravar" value="Gravar" onclick="submeter();">
				<?php  
				else:
				?>
					<input type="button" class="botao" name="btngravar" value="Replicar para mim" onclick="submeter();">
				<?php  
				endif;
				?>
				<input type="button" class="botao" name="btnnovo" value="Novo Registro" onclick="novo();">	  	    	  	     
			</td>
		</tr>
	</table>
</form>

<?php
$sql = "SELECT
			ci.cgidsc,
			dc.total,
			ci.cgiid
		FROM
			painel.cargaindicador ci
		JOIN (SELECT COUNT(*) as total, cgiid FROM painel.dadocargaindicador GROUP BY cgiid) dc ON dc.cgiid = ci.cgiid
		WHERE
			cgipublico = false AND
			regid = {$_SESSION['regid']} AND
			usucpf = '{$_SESSION['usucpf']}'";
$rs = $db->carregar($sql);

$arCabecalho = array("A��o","Descri��o","Qtd. Regionaliza��o");
$acao = "<center>
			 <img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"publicar({cgiid}, 'publicar')\">&nbsp;&nbsp;
			 <img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar configura��o \" onclick=\"carregar({cgiid})\">&nbsp;&nbsp;
			 <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir configura��o \" onclick=\"excluir({cgiid});\">
		 </center>";
// ARRAY de parametros de configura��o da tabela
$arConfig = array("style" => "width:95%;",
				  "totalLinha" => false,
				  "totalRegistro" => true);

$oLista = new Lista($arConfig);
$oLista->setCabecalho( $arCabecalho );
$oLista->setCorpo( $rs, $arParamCol );
$oLista->setAcao( $acao );
?>
<center>
	<fieldset style="width:94%;">
		<legend><img id="minhasconfigimg" src="/imagens/menos.gif" onclick="expandirDiv('#minhasconfig')">&nbsp;Minhas configura��es</legend>
		<div id="minhasconfig">
		<?php $oLista->show();?>
		</div>
	</fieldset>
</center>
<?php
$sql = "SELECT
			ci.cgidsc,
			dc.total,
			u.usunome,
			ci.cgiid,
			CASE WHEN u.usucpf='{$_SESSION['usucpf']}'
				THEN
					'<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"publicar('||ci.cgiid||', \'despublicar\');\">&nbsp;&nbsp;'
				ELSE
					''
			END AS btndespublicar
		FROM
			painel.cargaindicador ci
		JOIN (SELECT COUNT(*) as total, cgiid FROM painel.dadocargaindicador GROUP BY cgiid) dc ON dc.cgiid = ci.cgiid
		JOIN seguranca.usuario u ON u.usucpf = ci.usucpf
		WHERE
			cgipublico = true AND
			regid = {$_SESSION['regid']}";
$rs = $db->carregar($sql);

$arCabecalho = array("A��o","Descri��o","Qtd. Regionaliza��o", "Publicado por");
$acao = "<center>
			 {btndespublicar}
			 <img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar configura��o \" onclick=\"carregar({cgiid})\">&nbsp;&nbsp;
			 <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir configura��o \" onclick=\"excluir({cgiid});\">
		 </center>";
// ARRAY de parametros de configura��o da tabela
$arConfig = array("style" => "width:95%;",
				  "totalLinha" => false,
				  "totalRegistro" => true);

$oLista = new Lista($arConfig);
$oLista->setCabecalho( $arCabecalho );
$oLista->setCorpo( $rs, $arParamCol );
$oLista->setAcao( $acao );
?>
<center>
	<fieldset style="width:94%;">
		<legend><img id="publicoconfigimg" src="/imagens/mais.gif" onclick="expandirDiv('#publicoconfig')">&nbsp;Configura��es p�blicas</legend>
		<div id="publicoconfig" style="display:none;">
		<?php $oLista->show();?>
		</div>
	</fieldset>
</center>
<?php 
else:
?>
<center style="color:red;">Op��o n�o dispon�vel para o tipo de regionaliza��o.</center>
<?php 
endif;
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
//jQuery.noConflict();
function expandirDiv(id){
	$(id).toggle('slow', function (){
			var src = $(id+'img').attr('src');
			$(id+'img').attr('src', (src.indexOf('mais.gif') > -1 ? '/imagens/menos.gif' : '/imagens/mais.gif'));
		});
}

function submeter(){
	selectAllOptions( document.getElementById( 'dcidsc' ) );
	if(validar()){
		$('#evento').val('salvar');
		document.formulario.submit();	
	}
}

function validar(){	
	var msg = '';

	if(document.formulario.cgidsc.value == ''){
		msg+="O preenchimento do campo \"Descri��o\" � obrigat�rio.\n";
	}
//	if(document.formulario.dpeid.value == ''){
//		msg+="O preenchimento do campo \"Periodicidade\" � obrigat�rio.\n";
//	}
	if(jQuery('#dcidsc').val() == '' ){
		msg+="O preenchimento do campo \"<?php echo $txtDado; ?>\" � obrigat�rio.\n";
	}

	if(msg != "") {
		alert(msg);
		return false;
	} else {
		return true;			
	}	
}

function novo(){
	window.location.href = "?modulo=principal/cargaconfiguracao&acao=A";		
}

function carregar(cgiid){
	$('#cgiid').val(cgiid);
	$('#evento').val('carregar');
	document.formulario.submit();	
}

function excluir(cgiid){
	$('#cgiid').val(cgiid);
	$('#evento').val('excluir');
	document.formulario.submit();	
}

function publicar(cgiid, evento){
	$('#cgiid').val(cgiid);
	$('#evento').val(evento);
	document.formulario.submit();	
}
</script>