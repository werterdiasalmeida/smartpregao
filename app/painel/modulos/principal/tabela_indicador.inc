<?php 

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';


if($_SESSION['indid']):
	$db->cria_aba($abacod_tela,$url,'');
	!$_SESSION['indid']? $_SESSION['indid'] = $_REQUEST['indid']: $_SESSION['indid'] = $_SESSION['indid'];
	
	/*
	 * VALIDANDO SE USUÁRIO POSSUI PERMISSÃO PARA ACESSAR
	 * O INDICADOR
	 */
	$permissoes = verificaPerfilPainel();
	if($permissoes['verindicadores'] == 'vertodos') {
		$acesso = true;
	} else {
		$acesso = validaAcessoIndicadores($permissoes['verindicadores'], $_SESSION['indid']);
	}
	/*
	 * FIM
	 * VALIDANDO SE USUÁRIO POSSUI PERMISSÃO PARA ACESSAR
	 * O INDICADOR
	 */
endif;

$titulo = "Tabela de Detalhamento";
monta_titulo( $titulo, "Tabela de detalhamento do Indicador" );
cabecalhoIndicador($_SESSION['indid']); 

$sql = "select
			prtobj
		from
			public.parametros_tela
		where 
			mnuid = {$_SESSION['mnuid']}
		and
			usucpf is null
		and
			prtdsc = '{$_SESSION['indid']}'";
$arrPOST = $db->pegaUm($sql);

if($_POST['rbt_relatorio'] == "1"){
	$sql = "update 
				painel.indicador 
			set 
				indrelatorio = true
			where 
				indid = {$_SESSION['indid']}";
	$db->executar($sql);
	$db->commit();
}

if($_POST['rbt_relatorio'] == "2"){
	$sql = "update 
				painel.indicador 
			set 
				indrelatorio = false
			where 
				indid = {$_SESSION['indid']}";
	$db->executar($sql);
	$db->commit();
}

$_POST = !$_POST['salvar'] ? unserialize($arrPOST) : $_POST; 

$sql = "select count(*) from painel.seriehistorica where indid = {$_SESSION['indid']}";
$sh = $db->pegaUm($sql);

if(!$sh || $sh == "0"){
	?>
	<script>
		alert('Série Histórica inexistente!');
		history.back(-1);
	</script>
	<?php
	die;
}



?>
<script language="javascript" type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="/includes/agrupador.js"></script>
<form method="POST" name="formulario" id="formulario">
<input type="hidden" name="salvar" id="salvar" value="1" />
<input type="hidden" name="tipoEvento" id="tipoEvento" value="S" />
<table style="border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td colspan="2" class="SubTituloEsquerda"><b>Opções de Visualização:</b></td>
	</tr>
	<?php 
	$sql = "select 
				per.perdsc,
				per.perid,
				ind.unmid,
				ind.indcumulativo,
				unm.umedesc,
				reg.regunidade,
				ind.indqtdevalor,
				ind.regid,
				ind.indrelatorio
			from
				painel.periodicidade per
			inner join
				painel.indicador ind ON ind.perid = per.perid
			inner join
				painel.unidademeta unm ON unm.umeid = ind.umeid
			inner join
				painel.regionalizacao reg ON reg.regid = ind.regid
			where
				ind.indid = {$_SESSION['indid']}";
	$ind = $db->pegaLinha($sql);
	
	$sql = "select  
		dpe.dpeid,
		dpe.dpedsc
	from 
		painel.seriehistorica seh
	inner join
		painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
	where
		indid = {$_SESSION['indid']}
	and
		sehstatus <> 'I'
	order by
		dpe.dpedatainicio asc";
	
	$arrPeriodo = $db->carregar($sql);
	
	$sql = "select 
				tdi.tdiid,
				tdi.tdidsc
			from
				painel.detalhetipoindicador tdi
			where
				tdi.indid = {$_SESSION['indid']}
			and
				tdi.tdistatus = 'A'
			order by
				tdinumero";
	$arrDetalhes1 = $db->carregar($sql);
	
	$arrDetalhes1 = !$arrDetalhes1 ? array() : $arrDetalhes1;
	foreach($arrDetalhes1 as $arrD1){
		$arrDet1[] = array($arrD1['tdiid'],$arrD1['tdidsc']);
	}
	
	if(count($arrDet1) === 1) {?>
	<tr>
		<td width=25% class="SubTituloDireita">Detalhamento do Indicador:</td>
		<td>
			<select id="sel_det" name="sel_det" onchange="exibeDetalhamento(this.value)">
				<option <?php echo ($_POST['sel_det'] == $arrDet1[0][0] || !$_POST['sel_det'] ? "selected='selected'" : "")?> value="<?=$arrDet1[0][0]?>" ><?=$arrDet1[0][1]?></option>
				<option <?php echo ($_POST['sel_det'] == "naoexibir" ? "selected='selected'" : "")?> value="naoexibir" >Não Exibir</option>
			</select>
		</td>
	</tr>
	<?
	}elseif(count($arrDet1) > 1){?>
	<tr>
		<td width=25% class="SubTituloDireita">Detalhamento do Indicador:</td>
		<td>
			<select id="sel_det" name="sel_det" onchange="exibeDetalhamento(this.value)">
				<option <?php echo ($_POST['sel_det'] == $arrDet1[0][0] || !$_POST['sel_det'] ? "selected='selected'" : "")?> value="<?=$arrDet1[0][0]?>" ><?=$arrDet1[0][1]?></option>
				<option <?php echo ($_POST['sel_det'] == $arrDet1[1][0] ? "selected='selected'" : "")?> value="<?=$arrDet1[1][0]?>" ><?=$arrDet1[1][1]?></option>
				<option <?php echo ($_POST['sel_det'] == $arrDet1[0][0].",".$arrDet1[1][0] ? "selected='selected'" : "")?> value="<?=$arrDet1[0][0]?>,<?=$arrDet1[1][0]?>" ><?=$arrDet1[0][1]?> por <?=$arrDet1[1][1]?></option>
				<option <?php echo ($_POST['sel_det'] == $arrDet1[1][0].",".$arrDet1[0][0] ? "selected='selected'" : "")?> value="<?=$arrDet1[1][0]?>,<?=$arrDet1[0][0]?>" ><?=$arrDet1[1][1]?> por <?=$arrDet1[0][1]?></option>
				<option <?php echo ($_POST['sel_det'] == "naoexibir" ? "selected='selected'" : "")?> value="naoexibir" >Não Exibir</option>
			</select>
		</td>
	</tr>
	<?php } ?>
	
	<?php if($arrDet1[0]){
	
		// Detalhamento 1
		$stSql = " SELECT 
						tidid AS codigo,
						tiddsc AS descricao
					FROM 
						painel.detalhetipodadosindicador
					WHERE
						tidstatus = 'A'
					AND
						tdiid = {$arrDet1[0][0]}
					ORDER BY
						tiddsc ";
		
		if($_POST['detalhe_'.$arrDet1[0][0]] && !empty($_POST['detalhe_'.$arrDet1[0][0]][0]) && $_POST['detalhe1'] != array("naoexibir") && $_POST['detalhe_'.$arrDet1[0][0].'_campo_flag']){
			$stSqlCarregados1 = "SELECT 
									tidid AS codigo,
									tiddsc AS descricao
								FROM 
									painel.detalhetipodadosindicador
								WHERE
									tidstatus = 'A'
								AND
									tidid in ( ".implode(",",$_POST['detalhe_'.$arrDet1[0][0]])." )
								ORDER BY
									tiddsc";
		}
		mostrarComboPopup( $arrDet1[0][1] , "detalhe_".$arrDet1[0][0] ,  $stSql, $stSqlCarregados1, 'Selecione o(s) Detalhamento(s)' );
		
	}?>
	<?php if($arrDet1[1]){
	
		// Detalhamento 1
		$stSql = " SELECT 
						tidid AS codigo,
						tiddsc AS descricao
					FROM 
						painel.detalhetipodadosindicador
					WHERE
						tidstatus = 'A'
					AND
						tdiid = {$arrDet1[1][0]}
					ORDER BY
						tiddsc ";
		if($_POST['detalhe_'.$arrDet1[1][0]] && !empty($_POST['detalhe_'.$arrDet1[1][0]][0]) && $_POST['detalhe1'] != array("naoexibir") && $_POST['detalhe_'.$arrDet1[1][0].'_campo_flag']){
			$stSqlCarregados2 = "SELECT 
									tidid AS codigo,
									tiddsc AS descricao
								FROM 
									painel.detalhetipodadosindicador
								WHERE
									tidstatus = 'A'
								AND
									tidid in ( ".implode(",",$_POST['detalhe_'.$arrDet1[1][0]])." )
								ORDER BY
									tiddsc";
		}
		
		mostrarComboPopup( $arrDet1[1][1] , "detalhe_".$arrDet1[1][0] ,  $stSql, $stSqlCarregados2, 'Selecione o(s) Detalhamento(s)' );
		
	}?>
	
	<tr>
		<td width=25% class="SubTituloDireita">Período:</td>
		<td>
			<select id="sel_per_ini" name="sel_per_ini">
				<?php $arrPeriodo = !$arrPeriodo ? array() : $arrPeriodo ?>
				<?php foreach($arrPeriodo as $k => $per){?>
					<?php if($_POST['sel_per_fim']){?>
						<option <?php echo $per['dpeid'] == $_POST['sel_per_ini'] ? "selected='selected'" : ""?> value="<?=$per['dpeid']?>"><?=$per['dpedsc']?></option>
					<?php }else{ ?>
						<option <?php echo $k == 0 ? "selected='selected'" : ""?> value="<?=$per['dpeid']?>"><?=$per['dpedsc']?></option>
					<?php } ?>
				<?php } ?>
			</select>
			 até 
			<select id="sel_per_fim" name="sel_per_fim">
				<?php foreach($arrPeriodo as $k => $per){?>
					<?php if($_POST['sel_per_fim']){?>
						<option <?php echo $per['dpeid'] == $_POST['sel_per_fim'] ? "selected='selected'" : "" ?> value="<?=$per['dpeid']?>"><?=$per['dpedsc']?></option>
					<?php }else{ ?>
						<option <?php echo $k == ( count($arrPeriodo) - 1) ? "selected='selected'" : "" ?> value="<?=$per['dpeid']?>"><?=$per['dpedsc']?></option>
					<?php } ?>
				<?php } ?>
			</select>
		</td>
	</tr>
	<?php if($ind['unmid'] != UNIDADEMEDICAO_PERCENTUAL && $ind['unmid'] != UNIDADEMEDICAO_RAZAO){ ?>
	
	<tr>
		<td width=25% class="SubTituloDireita">Periodicidade:</td>
		<td>
		
		<?php 
		$arrPeriodicidade = getPeriodicidadeIndicador($_SESSION['indid']);
		$arrPeriodicidade = !$arrPeriodicidade ? array() : $arrPeriodicidade;
		?>
	
		<select id="sel_periodo" name="sel_periodo">
			<?php $perid = $_POST['sel_periodo'] == "" || !$_POST['sel_periodo'] ? $ind['perid'] : $_POST['sel_periodo']; ?>
			<?php foreach($arrPeriodicidade as $arrPer): ?>
				<?php $arrPerid[] = $arrPer['perid']; ?>
				<option <?php echo $perid == $arrPer['perid'] ? "selected\"selected\"" : "" ?>  value="<?php echo $arrPer['perid'] ?>" ><?php echo $arrPer['perdsc'] ?></option>
			<?php endforeach; ?>
			<? $arrPerid = !$arrPerid ? array() : $arrPerid ?>
			<?php if(!in_array(PERIODO_ANUAL,$arrPerid)): ?>
				<option <?php echo $perid == PERIODO_ANUAL ? "selected\"selected\"" : "" ?>  value="<?php echo PERIODO_ANUAL ?>" >Anual</option>
			<?php endif; ?>

				
			</select>
		</td>
	</tr>
	<?php }?>
	<tr>
		<td width=25% class="SubTituloDireita">Exibir totalizador(es):</td>
		<td>
			<?php if($ind['indcumulativo'] == "S" || $ind['indcumulativo'] == "A"){?>
				<input type="radio" <?php echo ($_POST['rbt_total'] == "1" ? "checked='checked'" : "")?> name="rbt_total" value="1" />Somente Total
				<input type="radio" <?php echo ($_POST['rbt_total'] == "2" ? "checked='checked'" : "")?> name="rbt_total" value="2" />Somente Total Geral
				<input type="radio" <?php echo ($_POST['rbt_total'] == "3" || !$_POST['rbt_total'] ? "checked='checked'" : "")?> name="rbt_total" value="3" />Ambos
				<input type="radio" <?php echo ($_POST['rbt_total'] == "4" ? "checked='checked'" : "")?> name="rbt_total" value="4" />Nenhum
			<?php }else{ ?>
				<input type="radio" <?php echo ($_POST['rbt_total'] == "1" || !$_POST['rbt_total'] ? "checked='checked'" : "")?> name="rbt_total" value="1" />Sim
				<input type="radio" <?php echo ($_POST['rbt_total'] == "4" ? "checked='checked'" : "")?> name="rbt_total" value="4" />Não
			<?php } ?>
		</td>
	</tr>
		
	<?php if( ($ind['indqtdevalor'] == "S" || $ind['indcumulativo'] == "A") && $ind['unmid'] != UNIDADEMEDICAO_MOEDA){?>
	<tr>
		<td width=25% class="SubTituloDireita">Exibir valor(es):</td>
		<td>
			<input type="radio" <?php echo ($_POST['rbt_valor'] == "1" ? "checked='checked'" : "")?> name="rbt_valor" value="1" />Somente <?=$ind['umedesc']?>
			<input type="radio" <?php echo ($_POST['rbt_valor'] == "2" ? "checked='checked'" : "")?> name="rbt_valor" value="2" />Somente Monetário
			<input type="radio" <?php echo ($_POST['rbt_valor'] == "3" || !$_POST['rbt_valor'] ? "checked='checked'" : "")?> name="rbt_valor" value="3" />Ambos
		</td>
	</tr>
	<?php } ?>
	
	<?php if($ind['regid'] == 7 || $ind['regid'] == 2 || $ind['regid'] == 4 || $ind['regid'] == 5 || $ind['regid'] == 6 || $ind['regid'] == 8 || $ind['regid'] == 9 || $ind['regid'] == 10 || $ind['regid'] == 11 || $ind['regid'] == 12){ //Estado?>
	
	<tr>
		<td width=25% class="SubTituloDireita">Exibir total de <?=$ind['regunidade']?>:</td>
		<td>
			<input type="radio" <?php echo ($_POST['rbt_reg'] == "1" || !$_POST['rbt_reg'] ? "checked='checked'" : "")?> name="rbt_reg" value="1" />Sim
			<input type="radio" <?php echo ($_POST['rbt_reg'] == "2" ? "checked='checked'" : "")?> name="rbt_reg" value="2" />Não
		</td>
	</tr>
	<tr>
		<td width=25% class="SubTituloDireita">Região:</td>
		<td>
			<select name="regcod"  id="regcod" onchange="filtraEstadoDB(this.value)" >
				<option selected="selected" value="">Selecione...</option>
				<? 
				$sql = "select 
							regcod, regdescricao
						from
							territorios.regiao
						order by
							regdescricao";
				$regiao = $db->carregar($sql);
				foreach($regiao as $rg){?>
					<option <?php echo ($_POST['regcod'] == $rg['regcod'] ? "selected='selected'" : "")?> value="<? echo $rg['regcod'] ?>"><? echo $rg['regdescricao'] ?></option>
				<? } ?>
			</select>
		</td>
	</tr>
	<tr>
		<td width=25% class="SubTituloDireita">Estado:</td>
		<td>
			<span id="exibeEstado" >
				<select name="estuf"  id="estuf" onchange="filtraMunicipio(this.value)" >
					<option selected="selected" value="todos">Selecione...</option>
					<? 
					if(!$_POST['regcod'] || $_POST['regcod'] == "todos" || $_POST['regcod'] == ""){					
						$sql = "select 
									estuf, estdescricao
								from
									territorios.estado
								order by
									estdescricao";
					}else{
						$sql = "select 
									estuf, 
									estdescricao
								from 
									territorios.estado 
								where 
									regcod = '{$_POST['regcod']}' 
								order 
									by estdescricao";	
					}
					$estados = $db->carregar($sql);
					
					foreach($estados as $uf){?>
						<option <?php echo ($_POST['estuf'] == $uf['estuf'] ? "selected='selected'" : "")?> value="<? echo $uf['estuf'] ?>"><? echo $uf['estdescricao'] ?></option>
					<? } ?>
				</select>
			</span>
		</td>
	</tr>
	<?php } ?>
	
	<? if($ind['regid'] == 7 || $ind['regid'] == 2 || $ind['regid'] == 4 || $ind['regid'] == 5 || $ind['regid'] == 8 || $ind['regid'] == 9 || $ind['regid'] == 10 || $ind['regid'] == 11 || $ind['regid'] == 12){ //Município?>
	
	<tr>
		<td width=25% class="SubTituloDireita">Grupo de Municípios:</td>
		<td>
			<span id="exibe_grupo_municipio">
				<select id="gtmid" name="gtmid" onchange="filtraGrupoMunicipios(this.value)" >
					<option selected="selected" value="">Selecione...</option>
				<? 
					$sql = "select 
								gtmid, gtmdsc
							from
								territorios.grupotipomunicipio
							where
								gtmstatus = 'A'
							order by
								gtmdsc";
					$grupoMun = $db->carregar($sql);
					foreach($grupoMun as $gm){?>
						<option <?php echo ($_POST['gtmid'] == $gm['gtmid'] ? "selected='selected'" : "")?> value="<? echo $gm['gtmid'] ?>"><? echo $gm['gtmdsc'] ?></option>
					<? } ?>
				</select>
			</span>
		</td>
	</tr>
	<tr>
		<td width=25% class="SubTituloDireita">Tipos de Municípios:</td>
		<td>
			<span id="exibe_tipo_municipio">
				<?php if($_POST['gtmid'] && $_POST['gtmid'] != "todos" && $_POST['gtmid'] != ""){?>
					<select id="tpmid" name="tpmid" onchange="filtraMunicipioPorTipoGrupo(this.value)" >
						<option  selected="selected" value="">Selecione...</option>
						<?php 
							$sql = "select tpmid as codigo, tpmdsc as descricao from territorios.tipomunicipio where gtmid = '{$_POST['gtmid']}' and tpmstatus = 'A' order by descricao";
							$tipoMun = $db->carregar($sql);
							foreach($tipoMun as $tM){?>
								<option <?php echo ($_POST['tpmid'] == $tM['codigo'] ? "selected='selected'" : "")?> value="<? echo $tM['codigo'] ?>"><? echo $tM['descricao'] ?></option>
							<?php } ?>
					</select>
				<?php }else{?>
					<select disabled="disabled" id="tpmid" style="width:200px;">
						<option  selected="selected" value="todos">Selecione...</option>
					</select>
				<?php }?>
			</span>
		</td>
	</tr>
	
	<tr>
		<td width=25% class="SubTituloDireita">Município:</td>
		<td>
			<span id="exibe_municipio">
				<?php if( ($_POST['estuf'] && $_POST['estuf'] != "todos") || ($_POST['tpmid'] && $_POST['tpmid'] != "" ) ){?>
					<select name="muncod" id="muncod" onchange="filtraRegionalizador(this.value)">	
						<option selected="selected" value="todos">Todos...</option>
						<?php 
						if( ($_POST['tpmid'] && $_POST['tpmid'] != "" ) && (!$_POST['estuf'] || $_POST['estuf'] == "todos")){
							$sql = "select 
										muncod,mundescricao 
									from
										territorios.municipio
									where
										muncod in 	(
														select muncod from territorios.muntipomunicipio where tpmid = {$_POST['tpmid']} 
													)
									order by
										mundescricao";
							$municipios = $db->carregar($sql);
							$municipios = !$municipios ? array() : $municipios;
						}
						elseif($_POST['tpmid'] && ($_POST['estuf'] || $_POST['estuf'] != "todos")){
							$sql = "select 
										muncod,mundescricao 
									from
										territorios.municipio
									where
										muncod in 	(
														select muncod from territorios.muntipomunicipio where tpmid = {$_POST['tpmid']} and estuf = '{$_POST['estuf']}'
													)
									order by
										mundescricao";
							$municipios = $db->carregar($sql);
							$municipios = !$municipios ? array() : $municipios;
						}
						elseif( (!$_POST['tpmid'] || $_POST['tpmid'] == "") && ($_POST['estuf'] && $_POST['estuf'] != "todos")){
							$sql = "select 
										muncod,mundescricao 
									from
										territorios.municipio
									where
										muncod in 	(
														select muncod from territorios.muntipomunicipio where estuf = '{$_POST['estuf']}'
													)
									order by
										mundescricao";
							$municipios = $db->carregar($sql);
							$municipios = !$municipios ? array() : $municipios;
						}
						$municipios = !$municipios ? array() : $municipios;
						foreach($municipios as $mun){?>
							<option <?php echo ($_POST['muncod'] == $mun['muncod'] ? "selected='selected'" : "")?> value="<? echo $mun['muncod'] ?>"><? echo $mun['mundescricao'] ?></option>
						<?php } ?>
					</select>
				<?php }else{?>
					<select disabled="disabled" name="muncod"  id="muncod">
						<option  selected="selected" value="todos">Selecione...</option>
					</select>
				<?php }?>
			</span>
		</td>
	</tr>
	
	<?php
		$sql = "	select
						regdescricao,
						regsqlcombo
					from
						painel.regionalizacao
					where
						regid = {$ind['regid']}
					and
						regsqlcombo is not null";
		$regDados = $db->pegaLinha($sql);
	if($regDados):
	?>
	<tr>
		<td width=25% class="SubTituloDireita"><?php echo $regDados['regdescricao'] ?>:</td>
		<td id="td_regvalue" >
		<?php if($_REQUEST['muncod'] && $regDados): ?>
			<?php require_once(APPRAIZ."www/painel/_funcoes_painel_controle.php"); ?>
			<?php filtraRegionalizador($_SESSION['indid'],$_REQUEST['muncod'],$_REQUEST['regvalue']) ?>
		<?php else: ?>
			<select disabled="disabled" name="regvalue" id="regvalue" style="width:200px;">
				<option  selected="selected" value="todos">Selecione...</option>
			</select>
		<?php endif; ?>
		</td>
	</tr>
	<?php endif; ?>
	
	<?php } ?>
	<?php if($db->testa_superuser()){?>
		<tr>
			<td width=25% class="SubTituloDireita">Exibir no Relatório de Indicadores:</td>
			<td>
				<?php $rbt_relatorio = $ind['indrelatorio'] == "t" ? "1" : "2";?>
				<input type="radio" <?php echo ($rbt_relatorio == "1" ? "checked='checked'" : "")?> name="rbt_relatorio" value="1" />Sim
				<input type="radio" <?php echo ($rbt_relatorio == "2" ? "checked='checked'" : "")?> name="rbt_relatorio" value="2" />Não
			</td>
		</tr>
	<?php } ?>
	<tr>
		<td bgcolor="#DCDCDC" width=25%></td>
		<td align="left" bgcolor="#DCDCDC" >
			<input type="button" onclick="enviaFormulario('S');" name="btn_enviar" id="btn_enviar" value="Enviar" />
			<?php if($db->testa_superuser()){?>
				<input type="button" onclick="enviaFormulario('SV');" name="btn_enviar" id="btn_enviar" value="Salvar Visualização" />
			<?php } ?>
			</td>
	</tr>
</table>
</form>
<script>
	function enviaFormulario(tipo){
		var dpeid_inicio = document.getElementById('sel_per_ini').value;
		var dpeid_fim = document.getElementById('sel_per_fim').value;

		if(document.getElementById('detalhe_<?php echo $arrDet1[0][0]?>')){
			selectAllOptions( document.getElementById('detalhe_<?php echo $arrDet1[0][0]?>') );
		}
		if(document.getElementById('detalhe_<?php echo $arrDet1[1][0]?>')){
			selectAllOptions( document.getElementById('detalhe_<?php echo $arrDet1[1][0]?>') );
		}
		
		var myAjax = new Ajax.Request(
				'painel.php?modulo=principal/painel_controle&acao=A',
				{
					method: 'post',
					parameters: "testaDatasDpeid=true&dpeid1=" + dpeid_inicio + "&dpeid2=" + dpeid_fim,
					asynchronous: false,
					onComplete: function(resp){
						erro = resp.responseText;
						if(erro.search("false") >= 0){
							if(document.getElementById('tipoEvento'))
								document.getElementById('tipoEvento').value = tipo;
							document.getElementById('formulario').submit();
						}else{
							alert('A data incial deve ser menor que a data final!');
						}
					},
					onLoading: function(){
					}
		});
	}

	function filtraEstadoDB(regcod,estuf){
		var div = document.getElementById('exibeEstado');
			var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: 'filtraEstado=true&regcod=' + regcod + '&filtro=true',
					        onComplete: function (res)
					        {	
								div.innerHTML = res.responseText;
								if(estuf)
									document.getElementById('estuf').value = estuf;
					        }
					  });
	}

	function filtraGrupoMunicipios(gtmid,tpmid){
		var div = document.getElementById('exibe_tipo_municipio');
			var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: 'filtraGrupoMunicipios=true&gtmid=' + gtmid + '&filtro=true',
					        onComplete: function (res)
					        {	
								div.innerHTML = res.responseText;
								extraiScript(res.responseText);
								if(tpmid)
									document.getElementById('tpmid').value = tpmid;
					        }
					  });
	}

	function filtraMunicipioPorTipoGrupo(tpmid,muncod){
		var estuf = document.getElementById('estuf').value;
		var div = document.getElementById('exibe_municipio');
			var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
					        method:     'post',
					        parameters: 'filtraMunicipioPorTipoGrupo=true&tpmid=' + tpmid + '&estuf=' + estuf,
					        onComplete: function (res)
					        {	
								div.innerHTML = res.responseText;
								extraiScript(res.responseText);
								if(muncod)
									document.getElementById('muncod').value = muncod;
								
					        }
					  });
	}

	function filtraMunicipio(estuf,muncod){
		var div = document.getElementById('exibe_municipio');
			if(estuf != "todos"){
				var myAjax = new Ajax.Request(
					'painel.php?modulo=principal/painel_controle&acao=A',
					{
						method: 'post',
						parameters: "filtraMunicipio=true&estuf=" + estuf,
						asynchronous: false,
						onComplete: function(resp){
							div.innerHTML = resp.responseText;
							if(muncod)
								document.getElementById('muncod').value = muncod;
						},
						onLoading: function(){
							div.innerHTML = '<select disabled="disabled" id="muncod" style="width:200px;"><option  selected="selected" value="todos">Carregando...</option></select>';
						}
					});
			}else{
				div.innerHTML = '<select disabled="disabled" id="muncod" style="width:200px;"><option selected="selected" value="todos">Selecione...</option></select>';
			}		
	}
	
	function filtraRegionalizador(muncod){
		if(document.getElementById('td_regvalue') && muncod && muncod != "todos"){
			var req = new Ajax.Request('painel.php?modulo=principal/painel_controle&acao=A', {
			        method:     'post',
			        parameters: 'filtraRegionalizador=true&indid=<? echo $_SESSION['indid'] ?>&muncod=' + muncod,
			        onComplete: function (res)
			        {	
						document.getElementById('td_regvalue').innerHTML = res.responseText;
			        }
			  });
		}
	}

	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
	if(document.getElementById('detalhe_<?php echo $arrDet1[0][0]?>')){
		selectAllOptions( document.getElementById('detalhe_<?php echo $arrDet1[0][0]?>') );
	}
	if(document.getElementById('detalhe_<?php echo $arrDet1[1][0]?>')){
		selectAllOptions( document.getElementById('detalhe_<?php echo $arrDet1[1][0]?>') );
	}

	function exibeDetalhamento(det){
		
		if(det == "naoexibir"){
			if(document.getElementById('tr_detalhe_<?php echo $arrDet1[0][0]?>')){
				document.getElementById("tr_detalhe_<?php echo $arrDet1[0][0]?>").style.display = "none";
				document.getElementById('detalhe_<?php echo $arrDet1[0][0]?>_campo_on' ).style.display = "none";
				document.getElementById('detalhe_<?php echo $arrDet1[0][0]?>_campo_off' ).style.display = "block";
				document.getElementById('detalhe_<?php echo $arrDet1[0][0]?>_campo_flag' ).value = '0';
			}
			if(document.getElementById('tr_detalhe_<?php echo $arrDet1[1][0]?>')){
				document.getElementById("tr_detalhe_<?php echo $arrDet1[1][0]?>").style.display = "none";
				document.getElementById('detalhe_<?php echo $arrDet1[1][0]?>_campo_on' ).style.display = "none";
				document.getElementById('detalhe_<?php echo $arrDet1[1][0]?>_campo_off' ).style.display = "block";
				document.getElementById('detalhe_<?php echo $arrDet1[1][0]?>_campo_flag' ).value = '0';
			}
		}if(det == "<?php echo $arrDet1[0][0]?>"){
			if(document.getElementById('tr_detalhe_<?php echo $arrDet1[0][0]?>'))
				document.getElementById("tr_detalhe_<?php echo $arrDet1[0][0]?>").style.display = "";
			if(document.getElementById('tr_detalhe_<?php echo $arrDet1[1][0]?>')){
				document.getElementById("tr_detalhe_<?php echo $arrDet1[1][0]?>").style.display = "none";
				document.getElementById('detalhe_<?php echo $arrDet1[1][0]?>_campo_on' ).style.display = "none";
				document.getElementById('detalhe_<?php echo $arrDet1[1][0]?>_campo_off' ).style.display = "block";
				document.getElementById('detalhe_<?php echo $arrDet1[1][0]?>_campo_flag' ).value = '0';
			}
		}if(det == "<?php echo $arrDet1[1][0]?>"){
			if(document.getElementById('tr_detalhe_<?php echo $arrDet1[1][0]?>'))
				document.getElementById("tr_detalhe_<?php echo $arrDet1[1][0]?>").style.display = "";
			if(document.getElementById('tr_detalhe_<?php echo $arrDet1[0][0]?>')){
				document.getElementById("tr_detalhe_<?php echo $arrDet1[0][0]?>").style.display = "none";
				document.getElementById('detalhe_<?php echo $arrDet1[0][0]?>_campo_on' ).style.display = "none";
				document.getElementById('detalhe_<?php echo $arrDet1[0][0]?>_campo_off' ).style.display = "block";
				document.getElementById('detalhe_<?php echo $arrDet1[0][0]?>_campo_flag' ).value = '0';
			}
		}if(det == "<?php echo $arrDet1[0][0]?>,<?php echo $arrDet1[1][0]?>" || det == "<?php echo $arrDet1[1][0]?>,<?php echo $arrDet1[0][0]?>"){
			if(document.getElementById('tr_detalhe_<?php echo $arrDet1[1][0]?>'))
				document.getElementById("tr_detalhe_<?php echo $arrDet1[1][0]?>").style.display = "";
			if(document.getElementById('tr_detalhe_<?php echo $arrDet1[0][0]?>'))
				document.getElementById("tr_detalhe_<?php echo $arrDet1[0][0]?>").style.display = "";
		}
	}
	if(document.getElementById("sel_det"))
		exibeDetalhamento(document.getElementById("sel_det").value)
</script>
<?php 

if($_POST['salvar'] && $arrPOST && $_POST['tipoEvento'] == "SV"){
	$post = serialize($_POST);
	$sql = "update 
				public.parametros_tela 
			set 
				prtdata = now(),
				prtobj = '$post' 
			where 
				mnuid = {$_SESSION['mnuid']}
			and
				usucpf is null
			and
				prtdsc = '{$_SESSION['indid']}'";
	$db->executar($sql);
	$db->commit();
}elseif($_POST['salvar'] && !$arrPOST && $_POST['tipoEvento'] == "SV"){
	$post = serialize($_POST);
	$sql = "insert into 
				public.parametros_tela
				( prtdsc , prtobj , prtpublico , prtdata , usucpf , mnuid)  
			values
				( '{$_SESSION['indid']}', '$post' , FALSE , now() , null , {$_SESSION['mnuid']} )";
	$db->executar($sql);
	$db->commit();
}

if(count($arrDet1) > 0){
	$arrTdiid = !$_POST['sel_det'] ? array($arrDet1[0][0]) : explode(",",$_POST['sel_det']);
}else{
	$arrTdiid = array("naoexibir");
}

switch($_POST['rbt_valor']){
	case 2:
		$arrQtdVlr = array("valor");
	break;
	case 1:
		$arrQtdVlr = array("quantidade");
	break;
	default:
		$arrQtdVlr = array("quantidade","valor");
	break;
}

switch($_POST['rbt_total']){
	case 1:
		$arrTotalizadores = array(1,0);
	break;
	case 2:
		$arrTotalizadores = array(0,1);
	break;
	case 4:
		$arrTotalizadores = array(0,0);
	break;
	default:
		$arrTotalizadores = array(1,1);
	break;
}

$exibirRegionalizador = $_POST['rbt_reg'] == 1 || !$_POST['rbt_reg'] ? true : false;

if($_POST['sel_per_ini']){
	$arrFiltros = array( "dpeid" => array($_POST['sel_per_ini'],$_POST['sel_per_fim']) );
}else{
	$arrFiltros = array( "dpeid" => array() );
}
$arrFiltros["estuf"] = $_POST['estuf'];
$arrFiltros["regcod"] = $_POST['regcod'];
$arrFiltros["gtmid"] = $_POST['gtmid'];
$arrFiltros["tpmid"] = $_POST['tpmid'];
$arrFiltros["muncod"] = $_POST['muncod'];

if($_POST['detalhe_'.$arrDet1[0][0]] && $_POST['detalhe_'.$arrDet1[0][0].'_campo_flag']){
	$arrFiltros["tidid_1"] = array( $arrDet1[0][0] => $_POST['detalhe_'.$arrDet1[0][0]]);
}
if($_POST['detalhe_'.$arrDet1[1][0]] && $_POST['detalhe_'.$arrDet1[1][0].'_campo_flag']){
	$arrFiltros["tidid_2"] = array( $arrDet1[1][0] => $_POST['detalhe_'.$arrDet1[1][0]]);
}
if($_POST['regvalue'] && $_POST['regvalue'] != "" && $_POST['regvalue'] != "todos"){
	$sql = "select rgaidentificador,rgafiltro from painel.regagreg where regid = {$ind['regid']}";
	$campoReg = $db->pegaLinha($sql);
	$arrFiltros[$campoReg['rgaidentificador']] = $_POST['regvalue'];
}
exibeTabelaIndicador($_SESSION['indid'],$arrTdiid,$_POST['sel_periodo'],$arrQtdVlr,$arrFiltros,$arrTotalizadores,$exibirRegionalizador,$semDetalhe); ?>