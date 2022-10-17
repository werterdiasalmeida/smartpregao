<?php

include_once("_funcoes_indicador_usuario.php");

/**
 * Função que controi o cabeçalho das páginas
 *
 * @author Juliano Meinen de Souza
 * @param inteiro indid (ID do Indicador)
 * @return null
 */
function cabecalhoIndicador($indid){
	global $db;
	
	if(!$indid):
		echo "<script>alert('Indicador inexistente.');window.location.href='painel.php?modulo=inicio&acao=A';</script>";	
	endif;
	if($indid):
		$sql = "SELECT 
						i.indnome,
						i.indobjetivo,
						s.sehid,
						um.umedesc,
						sec.secdsc,
						e.exodsc,
						per.perdsc,
						reg.regdescricao,
						peratual.perdsc as atualizacao
					FROM  
						painel.indicador	i
					LEFT JOIN
						painel.eixo e ON e.exoid = i.exoid
					LEFT JOIN
						painel.secretaria sec ON sec.secid = i.secid
					LEFT JOIN
						painel.unidademeta um ON um.umeid = i.umeid
					LEFT JOIN
						painel.periodicidade per ON per.perid = i.perid 
					LEFT JOIN 
						painel.seriehistorica s ON s.indid = i.indid
					LEFT JOIN 
						painel.regionalizacao reg ON reg.regid = i.regid
					LEFT JOIN
						painel.periodicidade peratual ON peratual.perid = i.peridatual 
					WHERE 
						i.indid = $indid 
					LIMIT 1";
			
			$dados = $db->pegaLinha($sql);
	?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="30%" valign="top" class="SubTituloDireita">
			Identificador do Indicador:
		</td>
		<td width="70%">
			<?=$indid;?>
		</td>
	</tr>
	<tr>
		<td width="30%" valign="top" class="SubTituloDireita">
			Eixo:
		</td>
		<td width="70%">
			<?=$dados['exodsc'];?>
		</td>
	</tr>
	<tr>
		<td width="30%" valign="top" class="SubTituloDireita">
			Secretaria/Autarquia Executora:
		</td>
		<td width="70%">
			<?=$dados['secdsc'];?>
		</td>
	</tr>
	<tr>
		<td width="30%" valign="top" class="SubTituloDireita">
			Nome Indicador:
		</td>
		<td width="70%">
			<?=$dados['indnome'];?>
		</td>
	</tr>	
	
	<tr >
		<td valign="top" class="SubTituloDireita">
			Objetivo:
		</td>
		<td>
			<?=$dados['indobjetivo'];?>
		</td>
	</tr>	
	<tr >
		<td valign="top" class="SubTituloDireita">
			Unidade Meta:
		</td>
		<td>
			<?=$dados['umedesc'];?>
		</td>
	</tr>
	<tr >
		<td valign="top" class="SubTituloDireita">
			Regionalização:
		</td>
		<td>
			<?=$dados['regdescricao'];?>
		</td>
	</tr>
	<tr >
		<td valign="top" class="SubTituloDireita">
			Periodicidade do Indicador:
		</td>
		<td id="td_cab_periodo" >
			<?=$dados['perdsc'];?>
		</td>
	</tr>
	<tr >
		<td valign="top" class="SubTituloDireita">
			Periodicidade de atualização:
		</td>
		<td id="td_cab_periodo" >
			<?=$dados['atualizacao'];?>
		</td>
	</tr>
	</table>
	<?
	endif;
}


function montaCampoFormulario($campo,$valor = null,$indid = null){
	global $db;
	
	//Estado,Município,Quantidade,Valor,Ações
	
	switch($campo){
		
		case is_array($campo):
			
			$sql = "select
						tiddsc AS descricao, 
						tidid AS codigo
					from
						painel.detalhetipodadosindicador
					where
						tdiid = {$campo['tdiid']}
					and
						tidstatus = 'A'
					order by
						descricao";
			$db->monta_combo('detalhe_'.$campo['tdiid'],$sql,'S','Selecione...','','','','','S','detalhe_'.$campo['tdiid'],'',$valor);
		break;

		case 'Estado':
			$sql= "SELECT 
						estdescricao AS descricao, 
						estuf AS codigo
					FROM 		
						territorios.estado
					order by
			 			descricao";
			$db->monta_combo('Estado',$sql,'S','Selecione...','mudaMunicipios','','','','S','estado','',$valor);
			break;
		
		case 'Município':
			
			!$valor? $valor = "false" : $valor = $valor;		
					
			
			$sql = array(array("codigo" => "","descricao" => ""));
						
			echo "<div id=\"exibe_municipios\">";
			$db->monta_combo('municipio',$sql,'N','Selecione...','mudaIES(this.value);mudaEscola','','','','S','municipio','',$valor);
			echo "</div>";
			echo "<script>mudaMunicipios(document.getElementById('estado').value,$valor);</script>";
			break;
		
		case 'Escola':
			
			!$valor? $valor = "false" : $valor = $valor;
			
			if($valor != "false"){
				$permissao = "S";
				$sql = "SELECT 
							esc.escdsc AS descricao, 
							esc.esccodinep AS codigo 
						FROM 
							painel.escola esc 
						INNER JOIN 
							territorios.municipio ter ON ter.muncod = esc.escmuncod 
						WHERE 
							ter.estuf = ( 
									select 
										estuf 
									from 
										territorios.municipio
									where 
										muncod = (
						
											select 
												escmuncod 
											from 
												painel.escola
											where 
												esccodinep = '$valor' 
											)
								) 
						order by 
							descricao";
			}else{
			$permissao = "N";
			$sql= "SELECT 
						escdsc AS descricao, 
						esccodinep AS codigo
					FROM 		
						painel.escola
					order by
			 			descricao
			 		limit
			 			5";
			}
			
			echo "<div id=\"exibe_escolas\">";
			$db->monta_combo('Escola',$sql,$permissao,'Selecione...','','','','','S','escola','',$valor);
			echo "</div>";
			echo "<script>mudaEscola(document.getElementById('municipio').value,'$valor');</script>";
			break;
		
		case 'IES':
			
			!$valor? $valor = "false" : $valor = $valor;
			
			if($valor != "false"){
				$permissao = "S";
				$sql = "SELECT 
							ies.iesdsc AS descricao, 
							ies.iesid AS codigo
						FROM 
							painel.ies ies
						INNER JOIN 
							territorios.municipio ter ON ter.muncod = ies.iesmuncod 
						WHERE 
							ter.muncod =  (
						
											select distinct
												iesmuncod 
											from 
												painel.ies
											where 
												iesid = '$valor' 
										)
						AND
							ies.iesano = (
									select
										max(iesano) 
									from 
										painel.ies 
									) 
						order by 
							descricao";
			}else{
				$permissao = "N";
				$sql= "SELECT 
							iesdsc AS descricao, 
							iesid AS codigo
						FROM 		
							painel.ies
						order by
				 			descricao
				 		limit
				 			5";
			}
			echo "<div id=\"exibe_ies\">";
			$db->monta_combo('IES',$sql,$permissao,'Selecione...','','','','','S','ies','',$valor);
			echo "</div>";
			echo "<script>mudaIES(document.getElementById('municipio').value,'$valor');</script>";
			break;
		
		case 'Quantidade':
			if($indid){
			//Tipo de Quantitativo do Indicador
				$sql = "select
							unmid
						from
							painel.indicador
						where
							indid = $indid";
				$unmid = $db->pegaUm($sql);
				
				switch($unmid){
					case 1:
						$mask = "###.###.###.###,##";
						$valor = number_format(str_replace(',','',$valor),2,',','.');
						break;
					case 2:
						$mask = "###.###.###.###,##";
						$valor = number_format(str_replace(',','',$valor),2,',','.');
						break;
					case 3:
						$mask = "###.###.###.###.###";
						$valor = str_replace(',00','',$valor);
						break;
					case 4:
						$mask = "###.###.###.###.###";
						$valor = str_replace(',00','',$valor);
						break;
					case 5:
						$mask = "###.###.###.###,##";
						$valor = number_format(str_replace(',','',$valor),2,',','.');
						break;
				}
				
			}
			
			echo campo_texto('quantidade','S','S','',20,20,$mask,'quantidade','','','','id="quantidade"','',$valor);
			break;
			
		case 'Valor':
			echo campo_texto('valor','S','S','',20,20,'###.###.###.###,##','valor','','','','id="valor"','',$valor,'');
			break;
		
		case 'Ações':
					
			echo "<input type=\"hidden\" name=\"dshid\" value=\"$valor\" id=\"dshid\" />";
			echo '<img src="/imagens/gif_inclui.gif" style="cursor:pointer" align="absmiddle"  onclick="addDetalheSH(document.getElementById(\'dshid\').value);" title="Adicionar Série Histórica" />';
			break;
	}
	
}

function carregaMunicipios($regcod,$muncod){
	global $db;
	$sql= "SELECT 
					mundescricao AS descricao, 
					muncod AS codigo
				FROM 		
					territorios.municipio
				WHERE
					estuf = '$regcod'
				order by
		 			mundescricao ";
		
		echo "<div id=\"exibe_municipios\">";
		$db->monta_combo('municipio',$sql,'S','Selecione...','mudaIES(this.value);mudaEscola','','','','S','municipio','',$muncod);
		echo "</div>";
}

function carregaEscolas($muncod,$esccodinep){
	global $db;
	$sql= "SELECT 
					escdsc AS descricao, 
					esccodinep AS codigo
				FROM 		
					painel.escola
				WHERE
					escmuncod = '$muncod'
				order by
		 			descricao ";
		
		echo "<div id=\"exibe_escolas\">";		
		$db->monta_combo('escola',$sql,'S','Selecione...','','','','','S','escola','',$esccodinep);
		echo "</div>";
}

function carregaIES($muncod,$iesid){
	global $db;
	$sql= "SELECT 
					iesdsc AS descricao, 
					iesid AS codigo
				FROM 		
					painel.ies
				WHERE
					iesmuncod = '$muncod'
				AND
					iesano = (
								select 
									max(iesano) as ano 
								from 
									painel.ies
							)
				order by
		 			descricao ";
		
		echo "<div id=\"exibe_ies\">";
		$db->monta_combo('ies',$sql,'S','Selecione...','','','','','S','ies','',$ies);
		echo "</div>";
}


function carregaSerieHistorica($indid,$sehid = null){
	global $db;
	
	$sql = "select
				indqtdevalor,
				regid
			from
				painel.indicador
			where 
				indid = $indid";
	
	$ind = $db->pegaLinha($sql);

	
	//painel.detalhetipoindicador
//	$sql = "select
//				dtdi.tiddsc
//			from
//				painel.detalhetiposeriehistorica as dts
//			inner join
//				painel.detalhetipodadosindicador as dtdi ON dtdi.tidid = dts.tidid
//				dshid = $sehid
//			and
//				tdistatus = 'A'";
//	$detalhe = $db->carregar($sql);	
	
	$sql= "SELECT 
						count(ddiid)
					FROM 		
						painel.detalhedadoindicador
					WHERE
						ddistatus = 'A'
						and indid = $indid";
						
	$detalhe = $db->pegaUm($sql);

	if(count($detalhe) != 0){
//		$n = 1;
//		foreach($detalhe as $det){
//			$sqlDetalhe .= " {$det['tdiid']} as detalhe_$n,";
//			$n++;
//		}
		$imgEditar = "<center><img src=\"../imagens/alterar_01.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"alert(\'Operação não Permitida.\')\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /> <img title=\"Detalhe não Atribuído\" src=\"../imagens/atencaoVermelho.png\" /></center>";
	}else{
		$SQLdetalhe = "";
		$imgEditar = "<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>";
	}
	
	
	$sql= "SELECT 
						unmid
					FROM 		
						painel.indicador
					WHERE
						indid = $indid";
						
	$unmid = $db->pegaUm($sql);
	
	$unmid == 1 ? $qtd = "Porcentagem" : $qtd = "Quantidade";
	
	echo "<input type=\"hidden\" value=\"$sehid\" id=\"SHsehid\" />";
	
	if($ind['indqtdevalor'] == 't')
			$valor = "coalesce(dsh.dshvalor,0) as dshvalor,";
	
$sql = "select
				unmdesc
			from
				painel.indicador i
			left join
				painel.unidademedicao u on i.unmid = u.unmid
			where
				indid = $indid";
	$unmdesc = $db->pegaUm($sql);
	($unmdesc == "")? $unmdesc = "Razão" : $unmdesc = $unmdesc;

//Verifica o tipo de medição e aplica as regras
	switch($unmdesc){
		case "Número inteiro":
			//$SQLsehqtde = "coalesce(sehqtde,0)::integer as sehqtde ";
			$SQLsehqtde = "replace(
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999'), 1, position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))-1)
				       , ',', '.')
				       || ' ' ||
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999'), position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))+1) as dshqtde";
			
			break;
		case "Percentual":
			//$SQLsehqtde = "coalesce(sehqtde,0) || '%' as sehqtde ";
			$SQLsehqtde = "replace(
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'), 1, position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))-1)
				       , ',', '.')
				       || ',' ||
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'), position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))+1) || '%'as dshqtde";
			break;
		case "Razão":
			//$SQLsehqtde = "coalesce(sehqtde,0) as sehqtde ";
			$SQLsehqtde = "replace(
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'), 1, position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))-1)
				       , ',', '.')
				       || ',' ||
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'), position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))+1) as dshqtde";
			break;
		case "Número índice":
			$SQLsehqtde = "coalesce(dsh.dshqtde,0) as sehqtde ";
			break;
		default:
			//$SQLsehqtde = "coalesce(sehqtde,0) as sehqtde ";
			$SQLsehqtde = "replace(
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'), 1, position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))-1)
				       , ',', '.')
				       || ',' ||
				       substring(to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'), position('.' in to_char(coalesce(dsh.dshqtde,0), '999,999,999.99'))+1) as dshqtde";
			break;
	}
	
			
	
	!$sehid? $sehid = 0 : $sehid = $sehid;
	
	switch($ind['regid']){
			case 1: //Brasil
				$cabecalho = array($qtd);
				$sql = "(select
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						order by
							dsh.dshid desc)";
				if($detalhe != 0){
						$sql .="UNION
						(select
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.detalhedadoindicador ddi ON ddi.ddiid = dsh.ddiid
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						order by
							dsh.dshid desc)";
				}
				break;
			case 2: //Escola
				$cabecalho = array('Estado','Município','Escola',$qtd);
				$sql = "(select
							est.estdescricao,
							mun.mundescricao,
							esc.escdsc,
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.escola esc ON esc.esccodinep = dsh.dshcod
						inner join
							territorios.municipio mun ON mun.muncod = esc.escmuncod
						inner join
							territorios.estado est ON mun.estuf = est.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						order by
							dsh.dshid desc)";
				if($detalhe != 0){
						$sql .="UNION
						(select
							est.estdescricao,
							mun.mundescricao,
							esc.escdsc,
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.escola esc ON esc.esccodinep = dsh.dshcod
						nner join
							territorios.municipio mun ON mun.muncod = esc.escmuncod
						inner join
							territorios.estado est ON mun.estuf = est.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						order by
							dsh.dshid desc)";
				}
				break;
			case 3: //Global
			$cabecalho = array($qtd);
				$sql = "(select
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						order by
							dsh.dshid desc)";
				if($detalhe != 0){
						$sql .="UNION
						(select
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.detalhedadoindicador ddi ON ddi.ddiid = dsh.ddiid
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						order by
							dsh.dshid desc)";
				}
				break;
			case 4: //Municipal
				$cabecalho = array('Estado','Município',$qtd);
				$sql = "(select
							uf.estdescricao,
							mun.mundescricao,
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						inner join
							territorios.municipio mun ON dsh.dshcodmunicipio = mun.muncod
						inner join
							territorios.estado uf ON uf.estuf = mun.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						order by
							dsh.dshid desc)";
				if($detalhe != 0){
						$sql .="UNION
						(select
							uf.estdescricao,
							mun.mundescricao,
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.detalhedadoindicador ddi ON ddi.ddiid = dsh.ddiid
						inner join
							territorios.municipio mun ON dsh.dshcodmunicipio = mun.muncod
						inner join
							territorios.estado uf ON uf.estuf = mun.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						order by
							dsh.dshid desc)";
				}
				break;
			case 5: //IES
				$cabecalho = array('Estado','Município','IES',$qtd);
				$sql = "(select distinct
							est.estdescricao,
							mun.mundescricao,
							ies.iesdsc,
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.ies ies ON ies.iesid = dsh.dshcod::integer
						inner join
							territorios.municipio mun ON mun.muncod = ies.iesmuncod
						inner join
							territorios.estado est ON mun.estuf = est.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						and
							ies.iesano = (	select 
												max(iesano) as ano 
											from 
												painel.ies )
						)";
				if($detalhe != 0){
						$sql .="UNION
						(select distinct
							est.estdescricao,
							mun.mundescricao,
							ies.iesdsc,
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.ies ies ON ies.iesid = dsh.dshcod::integer
						inner join
							territorios.municipio mun ON mun.muncod = ies.iesmuncod
						inner join
							territorios.estado est ON mun.estuf = est.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						and
							ies.iesano = (	select 
												max(iesano) as ano 
											from 
												painel.ies )";
				}
				break;
			case 6: //Estado
				$cabecalho = array('Estado',$qtd);
				$sql = "(select distinct
							est.estdescricao,
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						inner join
							territorios.estado est ON dsh.dshuf = est.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						and
							dsh.dshcodmunicipio is null)
							";
				if($detalhe != 0){
						$sql .="UNION
						(select distinct
							est.estdescricao,
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							territorios.estado est ON dsh.dshuf = est.estuf
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						and
							dsh.dshcodmunicipio is null)";
				}
				break;
			default:
				
			$cabecalho = array($qtd);
				$sql = "(select
							$SQLsehqtde,
							$SQLdetalhe
							$valor
							'$imgEditar'
						from
							painel.detalheseriehistorica dsh
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is null
						order by
							dsh.dshid desc)";
				if($detalhe != 0){
						$sql .="UNION
						(select
							$SQLsehqtde,
							ddi.ddidsc as detalhe,
							$valor
							'<center><img src=\"../imagens/alterar.gif\" title=\"Editar Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"editarDetalheSH(' || dsh.dshid || ');\" /> <img src=\"../imagens/excluir.gif\" title=\"Excluir Detalhe da Série Histórica\" style=\"cursor:pointer\" onclick=\"excluirDetalheSH(' || dsh.dshid || ')\" /></center>'
						from
							painel.detalheseriehistorica dsh
						inner join
							painel.detalhedadoindicador ddi ON ddi.ddiid = dsh.ddiid
						where
							dsh.sehid = $sehid
						and
							dsh.ddiid is not null
						order by
							dsh.dshid desc)";
				}
				break;
		}
		
		if($detalhe != 0)
			array_push($cabecalho, "Detalhe");
		
		if($ind['indqtdevalor'] == 't')
			array_push($cabecalho, "Valor");
			array_push($cabecalho, "Ações");

	$serieHistoria = $db->carregar($sql);
	$db->monta_lista_array($serieHistoria, $cabecalho, 50, 20, '', 'center', '');
}


function verificaRegistro($sehid,$dshcod,$dshcodmunicipio,$dshuf){
	global $db;
	if($dshcod != "null"){
		$sql = "select 
					count(dshid)
				from
					painel.detalheseriehistorica
				where
					sehid = $sehid
				and
					dshcod = '$dshcod'";
	}
	elseif($dshcodmunicipio != "null" && $dshuf != "null"){
		$sql = "select 
					count(dshid)
				from
					painel.detalheseriehistorica
				where
					sehid = $sehid
				and
					dshcodmunicipio = '$dshcodmunicipio'
				and
					dshuf = $dshuf";
	}
	elseif($dshcodmunicipio == "null" && $dshuf != "null"){
		$sql = "select 
					count(dshid)
				from
					painel.detalheseriehistorica
				where
					sehid = $sehid
				and
					dshuf = $dshuf";
	}
	

	$num = $db->pegaUm($sql);
	
	if($num != 0){
		return true;
	}
	else{
		return false;
	}
		
}


function excluirDetalheSH($indid,$sehid,$dshid){
	global $db;
		
	$sql = "delete 
			from
				painel.detalheseriehistorica
			where
				dshid = $dshid";
	$db->executar($sql);
	
	$sql = "select 
					coalesce(sum(dshqtde),0) as dshqtde,
					coalesce(sum(dshvalor),0) as dshvalor
				from
					painel.detalheseriehistorica
				where
					sehid = $sehid";
	$seh = $db->pegaLinha($sql);
		
	$sql = "update
					painel.seriehistorica
				set
					sehvalor = {$seh['dshvalor']},
					sehqtde = {$seh['dshqtde']}
				where
					sehid = $sehid";
	
	$db->executar($sql);	
	$db->commit();
	
	carregaSerieHistorica($indid,$sehid);
}

function editarDetalheSH($indid,$sehid,$dshid){
	global $db;
	$sql = "select
				indqtdevalor,
				regid
			from
				painel.indicador
			where 
				indid = $indid";
	
	$ind = $db->pegaLinha($sql);
	
	
	//painel.detalhetipoindicador
	$sql = "select
				tdiid,
				tdidsc
			from
				painel.detalhetipoindicador
			where
				indid = $indid
			and
				tdistatus = 'A'";
	$detalhe = $db->carregar($sql);
	
//	$sql= "SELECT 
//						count(ddiid)
//					FROM 		
//						painel.detalhedadoindicador
//					WHERE
//						ddistatus = 'A'
//						and indid = $indid";
//						
//	$detalhe = $db->pegaUm($sql);
	
//	if(count($detalhe) != 0){
//		$sqlDetalhe = "ddi.ddiid as detalhe,";
//		$innerDetalhe = "inner join
//							painel.detalhedadoindicador ddi ON dsh.ddiid = ddi.ddiid";
//	}

	if(count($detalhe) != 0){
		$n = 1;
		foreach($detalhe as $det){
			$sqlDetalhe .= " {$det['tdiid']} as detalhe_$n,";
			$n++;
		}
//		$innerDetalhe = "inner join
//							painel.detalhedadoindicador ddi ON dsh.ddiid = ddi.ddiid";
	}
	
		
	if($ind['regid'] == 4){
		$sql = "select
					dsh.dshid,
					uf.estuf,
					dsh.dshcodmunicipio,
					$sqlDetalhe
					coalesce(dsh.dshvalor,0) as dshvalor,
					coalesce(dsh.dshqtde,0) as dshqtde
				from
					painel.detalheseriehistorica dsh
				$innerDetalhe
				inner join
					territorios.municipio mun ON dsh.dshcodmunicipio = mun.muncod
				inner join
					territorios.estado uf ON uf.estuf = mun.estuf
				where
					dshid = $dshid";
		}
	elseif($ind['regid'] == 2){//Escola
			$sql = "select
					dsh.dshid,
					es.estuf,
					esc.escmuncod,
					esc.esccodinep,
					$sqlDetalhe
					coalesce(dsh.dshvalor,0) as dshvalor,
					coalesce(dsh.dshqtde,0) as dshqtde
				from
					painel.detalheseriehistorica dsh
				$innerDetalhe
				inner join
					painel.escola esc ON dsh.dshcod = esc.esccodinep
				inner join
					territorios.municipio mun ON esc.escmuncod = mun.muncod
				inner join
					territorios.estado es ON es.estuf = mun.estuf
				where
					dshid = $dshid";
		
	}
	elseif($ind['regid'] == 5){//IES
			$sql = "select
					dsh.dshid,
					es.estuf,
					ies.iesmuncod,
					ies.iesid,
					$sqlDetalhe
					coalesce(dsh.dshvalor,0) as dshvalor,
					coalesce(dsh.dshqtde,0) as dshqtde
				from
					painel.detalheseriehistorica dsh
				$innerDetalhe
				inner join
					painel.ies ies ON dsh.dshcod = ies.iesid::varchar(20)
				inner join
					territorios.municipio mun ON ies.iesmuncod = mun.muncod
				inner join
					territorios.estado es ON es.estuf = mun.estuf
				where
					dshid = $dshid";
	}
	elseif($ind['regid'] == 6){//Estado
			$sql = "select
					dsh.dshid,
					dsh.dshuf,
					$sqlDetalhe
					coalesce(dsh.dshvalor,0) as dshvalor,
					coalesce(dsh.dshqtde,0) as dshqtde
				from
					painel.detalheseriehistorica dsh
				$innerDetalhe
				where
					dshid = $dshid";
	}
	else{
		$sql = "select
					dsh.dshid,
					dsh.dshcodmunicipio,
					$sqlDetalhe
					coalesce(dsh.dshvalor,0) as dshvalor,
					coalesce(dsh.dshqtde,0) as dshqtde
				from
					painel.detalheseriehistorica dsh
				$innerDetalhe
				where
					dshid = $dshid";
	}
	
	$dsh = $db->pegaLinha($sql);
			
	switch($ind['regid']){
			case 1: //Brasil
				$arrTitulo = array('Quantidade');
				$arrValor = array($dsh['dshqtde']);
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				
				break;
			case 2: //Escola
				$arrTitulo = array('Estado','Município','Escola','Quantidade');
				$arrValor = array($dsh['estuf'],$dsh['escmuncod'],$dsh['esccodinep'],number_format(str_replace(',','',$dsh['dshqtde']),2,',','.'));
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				break;
			case 3: //Global
				$arrTitulo = array('Quantidade');
				$arrValor = array($dsh['dshqtde']);
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				break;
			case 4: //Municipal
				$arrTitulo = array('Estado','Município','Quantidade');
				$arrValor = array($dsh['estuf'],$dsh['dshcodmunicipio'],number_format(str_replace(',','',$dsh['dshqtde']),2,',','.'));
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				break;
			case 5: //IES
				$arrTitulo = array('Estado','Município','IES','Quantidade');
				$arrValor = array($dsh['estuf'],$dsh['iesmuncod'],$dsh['iesid'],number_format(str_replace(',','',$dsh['dshqtde']),2,',','.'));
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				break;
			case 6: //Estado
				$arrTitulo = array('Estado','Quantidade');
				$arrValor = array($dsh['dshuf'],number_format(str_replace(',','',$dsh['dshqtde']),2,',','.'));
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				break;
			default:
				$arrTitulo = array('Quantidade');
				$arrValor = array($dsh['dshqtde']);
				if(count($detalhe) != 0){
					foreach($detalhe as $det){
							array_push($arrTitulo, array('titulo' => $det['tdidsc'] ,'tdiid' => $det['tdiid']));
							array_push($arrValor, $det['tdiid']);
					}
				}
				break;
		}
		
		if($ind['indqtdevalor'] == 't'){
			array_push($arrTitulo, "Valor");
			array_push($arrValor, number_format(str_replace(',','',$dsh['dshvalor']),2,',','.'));
			
		}
			array_push($arrTitulo, "Ações");
			array_push($arrValor, $dsh['dshid']);

	
		$k = 0;
		foreach($arrTitulo as $titulo):?>
				<td style="font-weight: bold;text-align:center" bgcolor="#fcfcfc" onmouseout="this.bgColor='#fcfcfc';" onmouseover="this.bgColor='#c0c0c0';" valign="top" class="title">
					<? montaCampoFormulario($titulo,$arrValor[$k],$indid) ?>
				</td>
				
			<? 
			$k++;
		endforeach;
	
}

function excluirSerieHistorica($indid,$sehid){
	global $db;
	
	$sql = "update 
				painel.seriehistorica
			set 
				sehstatus = 'I'
			where
				sehid = $sehid";
				
	$db->executar($sql);
	$db->commit(); 
	
	carregaSerieHistoria($indid);
}
/**
 * Função que cria todas as regras de negocio envolvendo perfis
 * 
 * @author Alexandre Dourado
 * @return array $permissoes
 * @global class $db classe que instância o banco de dados 
 * @version v1.0 12/06/2009
 */
function verificaPerfilPainel() {
	global $db;
	/*
	 * Permissão padrão é sem acesso para todos os perfis
	 */
	// construindo o menu
	$enderecosweb = array("/painel/painel.php?modulo=principal/lista&acao=A"     => true,
						  "/painel/painel.php?modulo=principal/lista&acao=A&1=1" => true);
	
	
	if($db->testa_superuser()) {
		$permissoes['verindicadores']         = 'vertodos';
		$permissoes['condicaolista']          = "'<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 onclick=\"alterar(\'E\','||i.indid||');\" title=\"Excluir\">'";
		$permissoes['bloquearseriehistorica'] = true;
		$permissoes['sou_solicitante']        = true;
		$permissoes['removerseriehistorica'] = true;
		$permissoes['sou_atendente']          = (($db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PAINEL_PERFIL_ATENDENTE."'"))?true:false);
		
		$permissoes['menu'][0] = array("descricao" => "Lista de Indicadores", "link"=> ($enderecosweb[$_SERVER['REQUEST_URI']])?$_SERVER['REQUEST_URI']:key($enderecosweb));
		$permissoes['menu'][1] = array("descricao" => "Meus indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&evento=M");
		$permissoes['menu'][2] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=novoIndicador");
		$permissoes['menu'][3] = array("descricao" => "Tabela de Indicadores", "link"=> "/painel/painel.php?modulo=principal/tabela&acao=A");
		
	} else {
		// selecionando o perfil de maior nivel
		$sql = "SELECT p.pflcod FROM seguranca.perfil p 
				LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod 
				WHERE pu.usucpf = '". $_SESSION['usucpf'] ."' and p.pflstatus = 'A' and p.sisid =  '". $_SESSION['sisid'] ."' 
				ORDER BY pflnivel ASC LIMIT 1";
		
		$perfilcod = $db->pegaUm($sql);
		
		switch($perfilcod) {
			case PAINEL_PERFIL_ATUALIZA_SH:
				$permissoes['condicaolista']		 = "'<img style=\"cursor: pointer;\" src=\"/imagens/excluir_01.gif \" border=\"0\" title=\"Excluir\">'";
				$permissoes['verindicadores'] 		 = array();
				$permissoes['removerseriehistorica'] = true;
				$permissoes['sou_solicitante']        = (($db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PAINEL_PERFIL_SOLICITANTE."'"))?true:false);
			case PAINEL_PERFIL_ADM_ACAO:
				$permissoes['sou_solicitante']        = (($db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PAINEL_PERFIL_SOLICITANTE."'"))?true:false);
				$permissoes['condicaolista']           = "CASE WHEN (i.acaid in (SELECT ur.acaid FROM painel.usuarioresponsabilidade ur WHERE ur.usucpf='".$_SESSION['usucpf']."' AND ur.acaid = i.acaid AND ur.rpustatus='A')) THEN '<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 onclick=\"alterar(\'E\','||i.indid||');\" title=\"Excluir\">' ELSE '<img style=\"cursor: pointer;\" src=\"/imagens/excluir_01.gif \" border=\"0\" title=\"Excluir\">' END";
				$permissoes['condicaomeusindicadores'] = "i.acaid in (SELECT ur.acaid FROM painel.usuarioresponsabilidade ur WHERE ur.usucpf='".$_SESSION['usucpf']."' AND ur.acaid = i.acaid AND ur.rpustatus='A')";
				
				$permissoes['menu'][0] = array("descricao" => "Lista de Indicadores", "link"=> ($enderecosweb[$_SERVER['REQUEST_URI']])?$_SERVER['REQUEST_URI']:key($enderecosweb));
				$permissoes['menu'][1] = array("descricao" => "Meus indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&evento=M");
				$permissoes['menu'][2] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=novoIndicador");
				$permissoes['menu'][3] = array("descricao" => "Tabela de Indicadores", "link"=> "/painel/painel.php?modulo=principal/tabela&acao=A");
				
				$sql = "SELECT ind.indid FROM painel.indicador ind
						INNER JOIN painel.usuarioresponsabilidade ur ON ur.acaid = ind.acaid 
						WHERE ur.usucpf='".$_SESSION['usucpf']."' AND ur.rpustatus='A' GROUP BY ind.indid";
				
				$dadosacesso = $db->carregar($sql);
				if($dadosacesso[0]) {
					unset($permissoes['verindicadores']);
					foreach($dadosacesso as $ac) {
						$permissoes['verindicadores'][] = $ac['indid'];
					}
				}
				break;
			case PAINEL_PERFIL_ADM_EIXO:
				$permissoes['condicaolista']           = "CASE WHEN (i.exoid in (SELECT ur.exoid FROM painel.usuarioresponsabilidade ur WHERE ur.usucpf='".$_SESSION['usucpf']."' AND ur.exoid = i.exoid AND ur.rpustatus='A')) THEN '<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 onclick=\"alterar(\'E\','||i.indid||');\" title=\"Excluir\">' ELSE '<img style=\"cursor: pointer;\" src=\"/imagens/excluir_01.gif \" border=\"0\" title=\"Excluir\">' END";
				$permissoes['condicaomeusindicadores'] = "i.exoid in (SELECT ur.acaid FROM painel.usuarioresponsabilidade ur WHERE ur.usucpf='".$_SESSION['usucpf']."' AND ur.exoid = i.exoid AND ur.rpustatus='A')";
				$permissoes['removerseriehistorica'] = true;
				$permissoes['sou_solicitante']        = (($db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PAINEL_PERFIL_SOLICITANTE."'"))?true:false);
				
				$permissoes['menu'][0] = array("descricao" => "Lista de Indicadores", "link"=> ($enderecosweb[$_SERVER['REQUEST_URI']])?$_SERVER['REQUEST_URI']:key($enderecosweb));
				$permissoes['menu'][1] = array("descricao" => "Meus indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&evento=M");
				$permissoes['menu'][2] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=novoIndicador");
				$permissoes['menu'][3] = array("descricao" => "Tabela de Indicadores", "link"=> "/painel/painel.php?modulo=principal/tabela&acao=A");
				
				
				$sql = "SELECT ind.indid FROM painel.indicador ind
						INNER JOIN painel.usuarioresponsabilidade ur ON ur.exoid = ind.exoid 
						WHERE ur.usucpf='".$_SESSION['usucpf']."' GROUP BY ind.indid";
				
				$dadosacesso = $db->carregar($sql);
				if($dadosacesso[0]) {
					unset($permissoes['verindicadores']);
					foreach($dadosacesso as $ac) {
						$permissoes['verindicadores'][] = $ac['indid'];
					}
				}
				break;
			case EQUIPE_APOIO_GESTOR_PDE:
				$permissoes['condicaolista']		  = "'<img style=\"cursor: pointer;\" src=\"/imagens/excluir_01.gif \" border=\"0\" title=\"Excluir\">'";
				$permissoes['verindicadores'] 		  = array();
				$permissoes['bloquearseriehistorica'] = true;
				$permissoes['removerseriehistorica'] = true;
				$permissoes['sou_solicitante']        = (($db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PAINEL_PERFIL_SOLICITANTE."'"))?true:false);
				
				$permissoes['menu'][0] = array("descricao" => "Lista de Indicadores", "link"=> ($enderecosweb[$_SERVER['REQUEST_URI']])?$_SERVER['REQUEST_URI']:key($enderecosweb));
				$permissoes['menu'][1] = array("descricao" => "Meus indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&evento=M");
				$permissoes['menu'][2] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=novoIndicador");
				$permissoes['menu'][3] = array("descricao" => "Tabela de Indicadores", "link"=> "/painel/painel.php?modulo=principal/tabela&acao=A");
				break;
			default:
				$permissoes['condicaolista']		 = "'<img style=\"cursor: pointer;\" src=\"/imagens/excluir_01.gif \" border=\"0\" title=\"Excluir\">'";
				$permissoes['verindicadores'] 		 = array();
				$permissoes['sou_solicitante']        = (($db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".PAINEL_PERFIL_SOLICITANTE."'"))?true:false);
				$permissoes['menu'][0] = array("descricao" => "Lista de Indicadores", "link"=> ($enderecosweb[$_SERVER['REQUEST_URI']])?$_SERVER['REQUEST_URI']:key($enderecosweb));				
		}
	}
	return $permissoes;
}

function validaAcessoIndicadores($permissoes, $indid) {
	if($permissoes == 'semacesso')$permissoes = array();
	if(is_array($permissoes)) $permissoes = array_flip($permissoes);
	if(!isset($permissoes[$indid])) {
		return false;
	} else {
		return true;
	}
}

function pegaPerfil(){
	global $db;
	
	$sql = "select 
				pu.pflcod
			from 
				seguranca.perfilusuario pu 
			inner join 
				seguranca.perfil p on p.pflcod = pu.pflcod
			and 
				pu.usucpf = '{$_SESSION['usucpf']}' 
			and 
				p.sisid = {$_SESSION['sisid']}
			and
				pflstatus = 'A'";
				
	$arrPflcod = $db->carregar($sql);
	
	!$arrPflcod? $arrPflcod = array() : $arrPflcod = $arrPflcod;
	
	foreach($arrPflcod as $pflcod){
		$arrPerfil[] = $pflcod['pflcod'];
	}
	
	return $arrPerfil;
}

function filtraAcao($secid){
	global $db;
	
/*	$sql= "	select 
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
							secid = $secid
						)
			order by
				descricao";
*/
//	$sql= "	select 
//				acadsc AS descricao, 
//				acaid AS codigo
//			FROM 		
//				painel.acao
//			where
//				acaid in(
//						select 
//							distinct acaid
//						from
//							painel.acaosecretaria
//						where
//							secid = $secid
//						)
//				and acastatus='A'		
//			order by
//				descricao";

	$sql= "	select 
				acadsc AS descricao, 
				acaid AS codigo
			FROM 		
				painel.acao
			where
				acastatus='A'";
				
	$sql .= !empty($secid)? " AND acaid in( select distinct acaid from painel.acaosecretaria where secid = {$secid} ) order by descricao;" : " order by descricao;";
				
	echo $db->monta_combo('acaid',$sql,'S','Selecione...','','','','','N',"acaid","");

}
function filtraAtributos($dirid, $obrigatorio=false){
	global $db;
	
	$sql = "SELECT 
		atrdsc AS descricao, 
		atrid AS codigo
	FROM 		
		painel.atributos";
	
	$sql .= !empty($dirid)? " WHERE dirid = '{$dirid}' order by 1;": " order by 1;";

    echo $db->monta_combo('atrid',$sql,'S','Selecione...',"",'','','450',$obrigatorio? 'S' : 'N','','',$atrid);
}

function filtraDiretrizes($exiid, $obrigatorio=false){
	global $db;
	
		$sql= "SELECT 
			dridsc AS descricao, 
			driid AS codigo
		FROM 		
			painel.diretrizindicador";
	
	$sql .= !empty($exiid)? " WHERE exiid = '{$exiid}' order by 2;": " order by 2;";

	echo $db->monta_combo('driid',$sql,'S','Selecione...',"filtraObjetivos",'','','450',$obrigatorio? 'S' : 'N','','',$driid);
}

function filtraObjetivos($driid, $obrigatorio=false, $exiid=null){
	global $db;
	
		$sql= "SELECT 
			obi.obidsc AS descricao,
			obi.obiid AS codigo
		FROM 		
			painel.objetivoindicador obi";
	
	if($exiid){
		$sql .= " JOIN 
					painel.diretrizindicador dri ON dri.driid = obi.driid
													AND dri.exiid = {$exiid} ";
	}

	$sql .= !empty($driid)? " WHERE obi.driid = '{$driid}' order by 2;": " order by 2;";

	echo $db->monta_combo('obiid',$sql,'S','Selecione...','','','','450',$obrigatorio? 'S' : 'N','','',$obiid);
}


function exibeTabelaIndicador ( $indid, $arrTipoDetalhe = array("naoexibir"), $periodo = null, $arrQtdVlr = array("quantidade","valor"), $arrFiltros = array() , $arrTotalizadores = array(1,1) , $exibirRegionalizador = false, $semDetalhe = false, $exibeNome = true, $exibeObservacao = false, $exibeResponsavel = false, $exibeFonte = false){
	global $db;
		
	/*
	 $indid -> inteiro que recebe o identificador do indicador (campo obrigatório)
	 
	 $arrTipoDetalhe -> é composto de um array que passa os identificadores (tdiid) dos detalhamentos do Indicador ou apenas informa que não é necessária a exibição dos detalhamentos (campo não obrigatório)
		Exempo: array(100) ou array(200) ou array(100,200) ou array(200,100) ou array("naoexibir")
		
	 $periodo -> inteiro indentificador (perid) da periodicidade (painel.periodicidade) (campo não obrigatório)
	 
	 $arrQtdVlr -> array que informa quais as informações exibidas na tabela, podendo ser composta por 'quantidade' e 'valor'
	 	Exemplo: array("quantidade") ou array("valor") ou array("quantidade","valor")
	 
	 $arrFiltros -> array que recebe todos os filtros que serão aplicados na query que retorna os dados exibidos na tabela
	 
	 $arrTotalizadores -> array que informa quais totalizadores serão exibidos na tabela, sendo array(1,0) p/ apenas Total (direita), array(0,1) p/ apenas Total Geral (abaixo), array(1,1) p/ ambos (direita e abaixo) e array(0,0) p/ não exibir totalizadores
	 
	 $exibirRegionalizador -> boleana sendo true p/ exibir o total de regionalizadores (escolas,municípios,estados,etc) e false p/ ñ exibir
	 
	 $exibeNome -> boleana que indica se vai ou não ser exibido o nome do indicador (schema painel, tabela indicador, campo indnome)

	$exibeObservacao -> boleana que informa a Observação do Indicador;

	$exibeResponsavel -> boleana que informa os responsáveis 

	$exibeFonte -> boleana que informa a fonte
	 
	*/
			
	/* ********* *  INICIO QUERY PARA PEGAR OS DADOS DO INDICADOR * ********* */
	$sql = "select 
				ind.indid,
				ind.indnome,
				ind.perid,
				ind.indcumulativo,
				ind.indcumulativovalor,
				per.perdsc,
				per.perunidade,
				ind.unmid,
				reg.regunidade,
				ind.regid,
				ume.umedesc,
				ind.indqtdevalor,
				ind.indobjetivo,
				ind.indobservacao,
				ind.secid,
				ind.indfontetermo
			from
				painel.indicador ind
			inner join
				painel.periodicidade per ON per.perid = ind.perid
			inner join
				painel.unidademeta ume ON ind.umeid = ume.umeid
			inner join
				painel.regionalizacao reg ON reg.regid = ind.regid
			where
				ind.indid = $indid
			and
				ind.indstatus = 'A'";
	$arrDadosIndicador = $db->pegaLinha($sql);
	
	if(!$arrDadosIndicador)
		return false;
	
	extract($arrDadosIndicador);
	$regid = (int)$regid;
	
	/* ********* *  FIM QUERY PARA PEGAR OS DADOS DO INDICADOR * ********* */
	
	
	/* ********* *  INICÍO REGRAS OBRIGATÓRIAS * ********* */
	
	//Se o indicador for monetário, a quantidade é a representação financeira
	if($unmid == UNIDADEMEDICAO_MOEDA)
		$arrQtdVlr = array("quantidade");
	//Se o indicador for inteiro, deve-se pegar apenas valores antes da ','
	if($unmid == UNIDADEMEDICAO_NUM_INTEIRO)
		$tipoUnmid = "::integer ";
		
	//Mensagem de informação de registros não encontrados
	$msgErro = "Não Atendido.";
	
	//Se o indicador for razão é necessário informar o regionalizador
	$sql = "select 
				regdescricao, 
				rgaidentificador,
				rgafiltro 
			from 
				painel.regagreg reg1
			inner join
				painel.regionalizacao reg2 ON reg1.regid = reg2.regid 
			where 
				reg1.regid = $regid 
			and 
				regsqlcombo is not null";
	$campoRegiona = $db->pegaLinha($sql);
	/* Fim - Filtro por regvalue*/
	
	$retorno = true;
	
	if($campoRegiona && $unmid == UNIDADEMEDICAO_RAZAO && !$arrFiltros[$campoRegiona['rgaidentificador']] ){
		$msgErro = "Indicador de razão! Por favor, selecione o(a) {$campoRegiona['regdescricao']} e tente novamente!";
		$retorno = false;
	}
		
	/* ********* *  FIM REGRAS OBRIGATÓRIAS * ********* */
	
	
	/* ********* *  INICIO RETORNO FALSO * ********* */
	if($retorno == false){
		if($msgErro){
			echo "<center>$msgErro</center>";
		}
		return false;
	}
	/* ********* *  FIM RETORNO FALSO * ********* */
	
	
	/* ********* *  INICÍO DA APLICAÇÃO DOS FILTROS * ********* */
	
	//Filtro por tidid

	if($arrFiltros['tidid_1']){
		foreach($arrFiltros['tidid_1'] as $key => $tdi1){
			$sql = "select 
						tdinumero 
					from 
						painel.detalhetipoindicador 
					where 
						tdiid = $key";
			$tdinumero = $db->pegaUm($sql);
			if(!empty($tdi1[0])){
				$and[] = "d.tidid$tdinumero in ( ".implode(",",$tdi1)." ) ";
				$AndTiid[$key] = " and tidid in ( ".implode(",",$tdi1)." )";
			}
		}
	}
	if($arrFiltros['tidid_2']){
		foreach($arrFiltros['tidid_2'] as $key => $tdi2){
			$sql = "select 
						tdinumero 
					from 
						painel.detalhetipoindicador 
					where 
						tdiid = $key";
			$tdinumero = $db->pegaUm($sql);
			if(!empty($tdi2[0])){
				$and[] = "d.tidid$tdinumero in ( ".implode(",",$tdi2)." ) ";
				$AndTiid[$key] = " and tidid in ( ".implode(",",$tdi2)." )";
			}
		}
		
	}
	//Filtro por regcod
	if($arrFiltros['miccod'] && $arrFiltros['miccod'] != "" && $arrFiltros['miccod'] != "todos"){
		$and[] = "d.dshcodmunicipio in (	select muncod from territorios.municipio where miccod = '{$arrFiltros['miccod']}' )";
		$groupBy[] = "d.dshcodmunicipio";
	}		
	//Filtro por regcod
	if($arrFiltros['regcod'] && $arrFiltros['regcod'] != "" && $arrFiltros['regcod'] != "todos"){
		$and[] = "d.dshuf in ( select estuf from territorios.estado where regcod = '{$arrFiltros['regcod']}' ) ";
		$msgErro = "A região não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
		$groupBy[] = "d.dshuf";
	}
	//Filtro por estuf
	if($arrFiltros['estuf'] && $arrFiltros['estuf'] != "" && $arrFiltros['estuf'] != "todos"){
		$and[] = "d.dshuf = '{$arrFiltros['estuf']}'";
		$msgErro = "O estado não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
		$groupBy[] = "d.dshuf";
	}
	//Início - Filtro por grupo de municípios
	if($arrFiltros['gtmid'] && $arrFiltros['gtmid'] != "" && $arrFiltros['gtmid'] != "todos"){
		$and[] = "d.dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid in (select tpmid from territorios.tipomunicipio where gtmid = {$arrFiltros['gtmid']}) )";
		$groupBy[] = "d.dshcodmunicipio";
	}
	//Filtro por tpmid
	if($arrFiltros['tpmid'] && $arrFiltros['tpmid'] != "" && $arrFiltros['tpmid'] != "todos"){
		$and[] = "d.dshcodmunicipio in (	select muncod from territorios.muntipomunicipio where tpmid = '{$arrFiltros['tpmid']}' )";
		$groupBy[] = "d.dshcodmunicipio";
	}
	//Filtro por muncod
	if($arrFiltros['muncod'] && $arrFiltros['muncod'] != "" && $arrFiltros['muncod'] != "todos"){
		$and[] = "d.dshcodmunicipio = '{$arrFiltros['muncod']}' ";
		$msgErro = "O município não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
		$groupBy[] = "d.dshcodmunicipio";
	}
	//Filtro por mescod
	if($arrFiltros['mescod'] && $arrFiltros['mescod'] != ""){
		$and[] = "d.dshcodmunicipio in (select distinct muncod from territorios.municipio where mescod  = '{$arrFiltros['mescod']}') ";
		$msgErro = "A mesorregião não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
		$groupBy[] = "d.dshcodmunicipio";
	}
	//Filtro por polid
	if($arrFiltros['polid'] && $arrFiltros['polid'] != ""){
		$and[] = "d.polid = '{$arrFiltros['polid']}' ";
	}
	//Filtro por unicod
	if($arrFiltros['unicod'] && $arrFiltros['unicod'] != ""){
		$and[] = "d.unicod = '{$arrFiltros['unicod']}' ";
	}
	//Filtro por entid
	if($arrFiltros['entid'] && $arrFiltros['entid'] != ""){
		$and[] = "d.entid = '{$arrFiltros['entid']}' ";
	}
	//Filtro por iepid
	if($arrFiltros['iepid'] && $arrFiltros['iepid'] != ""){
		$and[] = "d.iepid = '{$arrFiltros['iepid']}' ";
	}
	//Filtro por dshcod
	if($arrFiltros['dshcod'] && $arrFiltros['dshcod'] != ""){
		$and[] = "d.dshcod = '{$arrFiltros['dshcod']}' ";
		$groupBy[] = "d.dshcod";
	}
	//Filtro por entid
	if($arrFiltros['entid'] && $arrFiltros['entid'] != ""){
		$and[] = "d.entid = '{$arrFiltros['entid']}' ";
		$groupBy[] = "d.entid";
	}
	
	//Filtro Por dpeid
	if(count($arrFiltros['dpeid'])){
		if($arrFiltros['dpeid'][0] && $arrFiltros['dpeid'][1]){
			$andDpe = " and 
						d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = {$arrFiltros['dpeid'][0]})
					and
						d.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = {$arrFiltros['dpeid'][1]})";
		}
	}
	
	/* Início - Filtro por regvalue*/
	if($arrFiltros[$campoRegiona['rgaidentificador']] && $arrFiltros[$campoRegiona['rgaidentificador']] != "" && $arrFiltros[$campoRegiona['rgaidentificador']] != "todos"){
		$and[] = "d.{$campoRegiona['rgaidentificador']} = '{$arrFiltros[$campoRegiona['rgaidentificador']]}' ";
		$groupBy[] = "d.{$campoRegiona['rgaidentificador']}";
	}
	/* Fim - Filtro por regvalue*/
	
		
	if($exibirRegionalizador){
		switch($regid){
			case REGIONALIZACAO_ESCOLA:
				$campoReg = "d.dshcod";
				$campoFiltro = "dshcod";
				$campoSelecionar = "a Escola";
				//$groupBy[] = "d.dshcod";
			break;
			case REGIONALIZACAO_IES:
				$campoReg = "d.dshcod";
				$campoFiltro = "dshcod";
				$campoSelecionar = "o Instituto de Ensino Superiro";
				//$groupBy[] = "d.dshcod";
			break;
			case REGIONALIZACAO_MUN:
				$campoFiltro = "muncod";
				$campoSelecionar = "o Município";
				if($arrFiltros['muncod'] && $arrFiltros['muncod'] != "" && $arrFiltros['muncod'] != "todos"){
					$campoReg = "";
					$exibirRegionalizador = false;
				}else
					$campoReg = "d.dshcodmunicipio";
					//$groupBy[] = "d.dshcodmunicipio";
			break;
			case REGIONALIZACAO_UF:
				$campoFiltro = "estuf";
				$campoSelecionar = "o Estado";
				if($arrFiltros['estuf'] && $arrFiltros['estuf'] != "" && $arrFiltros['estuf'] != "todos"){
					$campoReg = "";
					$exibirRegionalizador = false;
				}else
					$campoReg = "d.dshuf";
					//$groupBy[] = "d.dshuf";
			break;
			case REGIONALIZACAO_POSGRADUACAO:
				$campoFiltro = "dshcod";
				$campoReg = "d.dshcod";
				$campoSelecionar = "Instituto de Pós-Graduação";
				//$groupBy[] = "d.dshcod";
			break;
			case REGIONALIZACAO_CAMPUS_SUPERIOR:
				$campoFiltro = "entid";
				$campoReg = "d.entid";
				$campoSelecionar = "o Campus";
				//$groupBy[] = "d.entid";
			break;
			case REGIONALIZACAO_CAMPUS_PROFISSIONAL:
				$campoFiltro = "entid";
				$campoReg = "d.entid";
				$campoSelecionar = "o Campus";
				//$groupBy[] = "d.entid";
			break;
			case REGIONALIZACAO_UNIVERSIDADE:
				$campoFiltro = "unicod";
				$campoReg = "d.unicod";
				$campoSelecionar = "a Universidade";
				//$groupBy[] = "d.unicod";
			break;
			case REGIONALIZACAO_INSTITUTO:
				$campoFiltro = "dshcod";
				$campoReg = "d.dshcod";
				$campoSelecionar = "o Instituto";
				//$groupBy[] = "d.dshcod";
			break;
			case REGIONALIZACAO_HOSPITAL:
				$campoFiltro = "entid";
				$campoReg = "d.entid";
				$campoSelecionar = "o Hospital";
				//$groupBy = "d.entid";
			break;
			case REGIONALIZACAO_POLO:
				$campoFiltro = "polid";
				$campoReg = "d.polid";
				$campoSelecionar = "o Pólo";
				//$groupBy[] = "d.polid";
			break;
			default:
				$campoFiltro = "";
				$campoReg = "";
		}
	}
	
	if(is_array($and)){
		foreach($and as $andInt){
			$andInterno[] = str_replace("dsh.","dsh2.",$andInt);
		}
	}
	if(is_array($groupBy)){
		foreach($groupBy as $group){
			$groupByInterno[] = str_replace("dsh.","dsh2.",$group);
		}
	}
	
	/* ********* *  FIM DA APLICAÇÃO DOS FILTROS * ********* */
	
	/* ********* *  INÍCIO - REGRA PARA PROCENTAGEM E ÍNDICE * ********* */
	if(($unmid == UNIDADEMEDICAO_PERCENTUAL || $unmid == UNIDADEMEDICAO_NUM_INDICE) && $regid != REGIONALIZACAO_BRASIL){
		
		if($regid == REGIONALIZACAO_UF && $arrFiltros["estuf"] != "todos" && $arrFiltros["estuf"] != "" && $arrFiltros["estuf"] != null){
			
		}elseif($regid == REGIONALIZACAO_MUN && $arrFiltros["muncod"] != "todos" && $arrFiltros["muncod"] != "" && $arrFiltros["muncod"] != null){
			
		}else{?>
			<table class="tabela" width="100%" cellSpacing="1" border=0 cellPadding="3" align="center">
				<tr><td style="text-align: center;color:#990000" >Favor selecionar <?=$regid == REGIONALIZACAO_UF ? "o Estado" : "o Município"?>.</td></tr>
			</table>
		<?php
			return false;
		}
	}
	/* ********* *  FIM - REGRA PARA PROCENTAGEM E ÍNDICE * ********* */
	
	
	
	/* ********* *  INÍCIO SQL COMPLETO - 19/07/10 * ********* */
	$arrTipoDetalhe = !$arrTipoDetalhe ? array("naoexibir") : $arrTipoDetalhe;
	
	$periodo = !$periodo ? $perid : $periodo;
	
	$periodo =  strtoupper($periodo) == "ANUAL" || strtoupper($periodo) == "ANO" ? PERIODO_ANUAL : $periodo;
	$periodo = !$periodo ? PERIODO_ANUAL : $periodo;

	/* Inicio - Se não é necessária a exibição dos detalhamentos */
	if( in_array("naoexibir",$arrTipoDetalhe)){
	
		$sqlNovo = "select 
					dpeid,
					dpedsc,
					".($campoReg ? "(	select 
											count( distinct $campoReg) as reg 
										from 
											painel.v_detalheindicadorsh d
										where
											d.indid = foo.indid
										and
											d.dpedatainicio >= foo.dpedatainicio
										and
											d.dpedatainicio <= foo.dpedatafim
										".str_replace(array("dsh.","d1."),"d.",$andDpe)."
										".(count($and) ? " and ".implode(" and ",$and) : "" )." 
										group by
											d.indid
									) as reg," : "" )."
					sum(qtde)$tipoUnmid as dshqtde,
					sum(valor) as dshvalor 
				from (
						select 
							dp.dpeid,
							d.indid,
							dp.dpedsc,
							dp.dpedatainicio,
							dp.dpedatafim,
							case when d.indcumulativo = 'N' then
				        			case when (
						                        select 
						                        	count(d1.dpeid)
						                        from 
						                        	painel.detalheperiodicidade d1
												inner join 
													painel.seriehistorica sh on sh.dpeid=d1.dpeid
												where 
													d1.dpedatainicio>=dp.dpedatainicio 
												and 
													d1.dpedatafim<=dp.dpedatafim 
												and 
													sh.indid=d.indid
												".str_replace(array("dsh.","d."),"d1.",$andDpe)."
												
												limit 
													1
				                				) > 0 then sum(d.qtde) 
				                	else 0 end
								else sum(d.qtde)
							end as qtde,
							case when d.indcumulativovalor = 'N' then
				        			case when (
				                        		select 
				                        			count(d1.dpeid)
				                        		from 
				                        			painel.detalheperiodicidade d1
				                                inner join 
				                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
				                                where 
				                                	d1.dpedatainicio>=dp.dpedatainicio 
				                                and 
				                                	d1.dpedatafim<=dp.dpedatafim 
				                                and 
				                                	sh.indid=d.indid
				                                ".str_replace(array("dsh.","d."),"d1.",$andDpe)."
				                                
				                                limit 
				                                	1
				                				) > 0 then sum(d.valor) 
				                			else 0 end
									else sum(d.valor)
							end as valor
						from 
							painel.v_detalheindicadorsh d
						inner join 
							painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
						-- periodo que vc quer exibir
						where 
							dp.perid = $periodo
						-- indicador que vc quer exibir
						and 
							d.indid = $indid
						".(count($and) ? " and ".implode(" and ",$and) : "" )."
						--range de data compreendida no periodo
						".str_replace("dsh.","d.",$andDpe)."
						group by 
							d.indid,
							d.dpeid,
							dp.dpedsc,
							dp.dpeid,
							dp.dpedatainicio,
							dp.dpedatafim,
							d.indcumulativo,
							d.indcumulativovalor
							".(count($groupBy) ? ",".implode(",",$groupBy) : "" )."
					) foo
				group by 
					dpedatainicio,
					dpedatafim,
					dpeid,
					dpedsc,
					indid
				order by 
					dpedatainicio";

		$arrDados = $db->carregar($sqlNovo);
		
		if(is_array($arrDados)){
			
			foreach($arrDados as $arrDado){
				
				$arrValor[ $arrDado['dpeid'] ] = array( "periodo" 		 => $arrDado["dpedsc"],
														"qtde"	  		 => $arrDado["dshqtde"] ,
														"valor"	  		 => $arrDado["dshvalor"] ,
														"regionalizador" => $arrDado["reg"] );
				
				$arrPeriodos[ $arrDado['dpeid'] ] = $arrDado["dpedsc"];
				
			}
			
		}
	/* Fim - Se não é necessária a exibição dos detalhamentos */
	
	/* Inicio - Se é necessária a exibição de apenas 1 detalhamento */
	}elseif( !in_array("naoexibir",$arrTipoDetalhe) && count($arrTipoDetalhe) == 1 ){
	
		//Pega o numero do detalhe
		$sqlTdiNumero = "select tdinumero from painel.detalhetipoindicador where tdiid = {$arrTipoDetalhe[0]}";
		$tdinumero = $db->pegaUm($sqlTdiNumero);
					
		$sqlDetalhe = "select 
							tidid,
							tiddsc
						from 
							painel.detalhetipodadosindicador 
						where 
							tdiid = {$arrTipoDetalhe[0]}
						and
							tidstatus = 'A'
						".$AndTiid[$arrTipoDetalhe[0]]."
						order by tiddsc";
		$arrDetalhes1 = $db->carregar($sqlDetalhe);
	
		$sqlNovo = "select 
					dpeid,
					tidid$tdinumero,
					dpedsc,
					".($campoReg ? "(	select 
											count( distinct $campoReg) as reg 
										from 
											painel.v_detalheindicadorsh d
										where
											d.indid = foo.indid
										and
											d.dpedatainicio >= foo.dpedatainicio
										and
											d.dpedatainicio <= foo.dpedatafim
										and
											d.tidid$tdinumero = foo.tidid$tdinumero
										".str_replace(array("dsh.","d1."),"d.",$andDpe)."
										".(count($and) ? " and ".implode(" and ",$and) : "" )." 
										group by
											d.indid
									) as reg," : "" )."
					sum(qtde)$tipoUnmid as dshqtde,
					sum(valor) as dshvalor 
				from (
						select 
							d.indid,
							dp.dpeid,
							dp.dpedsc,
							tidid$tdinumero,
							dp.dpedatainicio,
							dp.dpedatafim,
							case when d.indcumulativo = 'N' then
				        			case when (
						                        select 
						                        	count(d1.dpeid) 
						                        from 
						                        	painel.detalheperiodicidade d1
												inner join 
													painel.seriehistorica sh on sh.dpeid=d1.dpeid
												where 
													d1.dpedatainicio>=dp.dpedatainicio 
												and 
													d1.dpedatafim<=dp.dpedatafim
												and 
													sh.indid=d.indid
												".str_replace(array("dsh.","d."),"d1.",$andDpe)."
												
												limit 
													1
				                				) > 0 then sum(d.qtde) 
				                	else 0 end
								else sum(d.qtde)
							end as qtde,
							case when d.indcumulativovalor = 'N' then
				        			case when (
				                        		select 
				                        			count(d1.dpeid) 
				                        		from 
				                        			painel.detalheperiodicidade d1
				                                inner join 
				                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
				                                where 
				                                	d1.dpedatainicio>=dp.dpedatainicio 
				                                and 
				                                	d1.dpedatafim<=dp.dpedatafim 
				                                and 
				                                	sh.indid=d.indid
				                                ".str_replace(array("dsh.","d."),"d1.",$andDpe)."
				                                 
				                                limit 
				                                	1
				                				) > 0 then sum(d.valor) 
				                			else 0 end
									else sum(d.valor)
							end as valor
						from 
							painel.v_detalheindicadorsh d
						inner join 
							painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
						-- periodo que vc quer exibir
						where 
							dp.perid = $periodo
						-- indicador que vc quer exibir
						and 
							d.indid = $indid
						".(count($and) ? " and ".implode(" and ",$and) : "" )."
						--range de data compreendida no periodo
						".str_replace("dsh.","d.",$andDpe)."
						group by 
							d.indid,
							d.dpeid,
							dp.dpedsc,
							dp.dpeid,
							dp.dpedatainicio,
							dp.dpedatafim,
							d.indcumulativo,
							d.indcumulativovalor,
							tidid$tdinumero
							".(count($groupBy) ? ",".implode(",",$groupBy) : "" )."
					) foo
				group by 
					indid,
					dpedatainicio,
					dpedatafim,
					dpeid,
					dpedsc,
					tidid$tdinumero
				order by 
					dpedatainicio";
	
		$arrDados = $db->carregar($sqlNovo);
		
		if(is_array($arrDados)){
			
			foreach($arrDados as $arrDado){
				
				$arrValor[ $arrDado['dpeid'] ] [ $arrDado["tidid$tdinumero"] ] = array( 	"periodo" 		 => $arrDado["dpedsc"],
																						"qtde"	  		 => $arrDado["dshqtde"] ,
																						"valor"	  		 => $arrDado["dshvalor"] ,
																						"regionalizador" => $arrDado["reg"] );
				
				$arrPeriodos[ $arrDado['dpeid'] ] = $arrDado["dpedsc"];
				
			}
			
		}
		
	/* Fim - Se é necessária a exibição de apenas 1 detalhamento */
	
	/* Inicio - Se é necessária a exibição de 2 detalhamentos */
	}elseif( !in_array("naoexibir",$arrTipoDetalhe) && count($arrTipoDetalhe) == 2 ){
	
		//Pega o numero do detalhe 1
		$sqlTdiNumero1 = "select tdinumero from painel.detalhetipoindicador where tdiid = {$arrTipoDetalhe[0]}";
		$tdinumero1 = $db->pegaUm($sqlTdiNumero1);
		
		//Pega o numero do detalhe 2
		$sqlTdiNumero2 = "select tdinumero from painel.detalhetipoindicador where tdiid = {$arrTipoDetalhe[1]}";
		$tdinumero2 = $db->pegaUm($sqlTdiNumero2);
					
		$sqlDetalhe1 = "select 
							tidid,
							tiddsc 
						from 
							painel.detalhetipodadosindicador 
						where 
							tdiid = {$arrTipoDetalhe[0]}
						and
							tidstatus = 'A'
						".$AndTiid[$arrTipoDetalhe[0]]."
						order by tiddsc";
		$arrDetalhes1 = $db->carregar($sqlDetalhe1);
		
		$sqlDetalhe2 = "select 
							tidid,
							tiddsc 
						from 
							painel.detalhetipodadosindicador 
						where 
							tdiid = {$arrTipoDetalhe[1]}
						and
							tidstatus = 'A'
						".$AndTiid[$arrTipoDetalhe[1]]."
						order by tiddsc";
		$arrDetalhes2 = $db->carregar($sqlDetalhe2);
		
	
		$sqlNovo = "select 
					dpeid,
					tidid$tdinumero1,
					tidid$tdinumero2,
					dpedsc,
					".($campoReg ? "(	select 
											count( distinct $campoReg) as reg 
										from 
											painel.v_detalheindicadorsh d
										where
											d.indid = foo.indid
										and
											d.dpedatainicio >= foo.dpedatainicio
										and
											d.dpedatainicio <= foo.dpedatafim
										and
											d.tidid$tdinumero1 = foo.tidid$tdinumero1
										and
											d.tidid$tdinumero2 = foo.tidid$tdinumero2
										".str_replace(array("dsh.","d1."),"d.",$andDpe)."
										".(count($and) ? " and ".implode(" and ",$and) : "" )." 
										group by
											d.indid
									) as reg," : "" )."
					sum(qtde)$tipoUnmid as dshqtde,
					sum(valor) as dshvalor 
				from (
						select 
							dp.dpeid,
							d.indid,
							dp.dpedsc,
							tidid$tdinumero1,
							tidid$tdinumero2,
							dp.dpedatainicio,
							dp.dpedatafim,
							case when d.indcumulativo = 'N' then
				        			case when (
						                        select 
						                        	count(d1.dpeid) 
						                        from 
						                        	painel.detalheperiodicidade d1
												inner join 
													painel.seriehistorica sh on sh.dpeid=d1.dpeid
												where 
													d1.dpedatainicio>=dp.dpedatainicio 
												and 
													d1.dpedatafim<=dp.dpedatafim 
												and 
													sh.indid=d.indid
												".str_replace(array("dsh.","d."),"d1.",$andDpe)."
												
												limit 
													1
				                				) > 0 then sum(d.qtde) 
				                	else 0 end
								else sum(d.qtde)
							end as qtde,
							case when d.indcumulativovalor = 'N' then
				        			case when (
				                        		select 
				                        			count(d1.dpeid)
				                        		from 
				                        			painel.detalheperiodicidade d1
				                                inner join 
				                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
				                                where 
				                                	d1.dpedatainicio>=dp.dpedatainicio 
				                                and 
				                                	d1.dpedatafim<=dp.dpedatafim 
				                                and 
				                                	sh.indid=d.indid
				                                ".str_replace(array("dsh.","d."),"d1.",$andDpe)."
				                                
				                                limit 
				                                	1
				                				) > 0 then sum(d.valor) 
				                			else 0 end
									else sum(d.valor)
							end as valor
						from 
							painel.v_detalheindicadorsh d
						inner join 
							painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
						-- periodo que vc quer exibir
						where 
							dp.perid = $periodo
						-- indicador que vc quer exibir
						and 
							d.indid = $indid
						".(count($and) ? " and ".implode(" and ",$and) : "" )."
						--range de data compreendida no periodo
						".str_replace("dsh.","d.",$andDpe)."
						group by 
							d.indid,
							d.dpeid,
							dp.dpedsc,
							dp.dpeid,
							dp.dpedatainicio,
							dp.dpedatafim,
							d.indcumulativo,
							d.indcumulativovalor,
							tidid$tdinumero1,
							tidid$tdinumero2
							".(count($groupBy) ? ",".implode(",",$groupBy) : "" )."
					) foo
				group by 
					dpedatainicio,
					dpedatafim,
					indid,
					dpeid,
					dpedsc,
					tidid$tdinumero1,
					tidid$tdinumero2
				order by 
					dpedatainicio";
	
		$arrDados = $db->carregar($sqlNovo);
		
		if(is_array($arrDados)){
			
			foreach($arrDados as $arrDado){
				
				$arrValor[ $arrDado['dpeid'] ] [ $arrDado["tidid$tdinumero1"] ] [ $arrDado["tidid$tdinumero2"] ] = array( 	"periodo" 		 => $arrDado["dpedsc"],
																															"qtde"	  		 => $arrDado["dshqtde"] ,
																															"valor"	  		 => $arrDado["dshvalor"] ,
																															"regionalizador" => $arrDado["reg"] );
				
				$arrPeriodos[ $arrDado['dpeid'] ] = $arrDado["dpedsc"];
				
			}
			
		}
		
	};
	/* Fim - Se é necessária a exibição de 2 detalhamentos*/
	
	/* ********* *  FIM SQL COMPLETO - 19/07/10 * ********* */	
	//Verifica possibilidade de exibição de Qtd. / Valor
	if(count($arrQtdVlr) == 2 && $indqtdevalor == "t" ){
		$QtdValor = 2;
		$ExibeQtd = true;
		$ExibeValor = true;
	}elseif(in_array("quantidade", $arrQtdVlr)){
		$QtdValor = 1;
		$ExibeQtd = true;
		$ExibeValor = false;
	}elseif(in_array("valor", $arrQtdVlr) && $indqtdevalor == "t"){
		$QtdValor = 1;
		$ExibeQtd = false;
		$ExibeValor = true;
	}

	if(trim($regunidade) == trim($umedesc)){
		$exibirRegionalizador = false;
		$campoReg = false;
	}

	if($exibirRegionalizador && $campoReg != ""){
		$QtdValor += 1;	
	}

	$count1 = !$arrDetalhes1 ? 1 : count($arrDetalhes1);
	$count2 = !$arrDetalhes2 ? 1 : count($arrDetalhes2); 
	$arrDetalhes1 = !$arrDetalhes1 ? array() : $arrDetalhes1;
	$arrDetalhes2 = !$arrDetalhes2 ? array() : $arrDetalhes2;
	
	/* INÍCIO - Retira exibição de detalhes sem valores */	
	foreach($arrDetalhes1 as $arrD1){
		$arrTidid1[] = $arrD1['tidid'];
	}
	if($arrTidid1){
		$sql = "select 
								distinct tdinumero 
							from 
								painel.detalhetipoindicador dti 
							inner join
								painel.detalhetipodadosindicador dtdi ON dtdi.tdiid = dti.tdiid 
							where 
								dtdi.tidid in (".implode(",",$arrTidid1).")";
					$tdinumero = $db->pegaUm($sql);
		
		$sql = "select 
				tidid, 
				tiddsc 
			from 
				painel.detalhetipodadosindicador 
			where 
				tidid in (	select 
								distinct tidid$tdinumero 
							from 
								painel.v_detalheindicadorsh d
							where
								d.tidid$tdinumero in (".implode(",",$arrTidid1).") 
							and
								d.indid = $indid
							and
								d.sehstatus <> 'I'
							".(count($and) ? " and ".implode(" and ",$and) : "" )."
						 )
				order by tiddsc";
		$arrDetalhes1 = $db->carregar($sql);
	}
	
	foreach($arrDetalhes2 as $arrD2){
		$arrTidid2[] = $arrD2['tidid'];
	}
	if($arrTidid2){
		$sql = "select 
								distinct tdinumero 
							from 
								painel.detalhetipoindicador dti 
							inner join
								painel.detalhetipodadosindicador dtdi ON dtdi.tdiid = dti.tdiid 
							where 
								dtdi.tidid in (".implode(",",$arrTidid2).")";
					$tdinumero2 = $db->pegaUm($sql);
		
		$sql = "select 
				tidid, 
				tiddsc 
			from 
				painel.detalhetipodadosindicador 
			where 
				tidid in (	select 
								distinct tidid$tdinumero2 
							from 
								painel.v_detalheindicadorsh d
							where
								d.tidid$tdinumero2 in (".implode(",",$arrTidid2).")
							and
								d.indid = $indid
							and
								d.sehstatus <> 'I'
							".(count($and) ? " and ".implode(" and ",$and) : "" )."
						 )
				order by tiddsc";
		$arrDetalhes2 = $db->carregar($sql);
	}
	
	/* Início - Aplicação de filtro para regionalizadores */
	if($arrFiltros['dpeid'][0])
		$arrDpeReg[] = "d.dpedatainicio >= (select dpedatainicio from painel.detalheperiodicidade where dpeid = {$arrFiltros['dpeid'][0]} )"; 
	if($arrFiltros['dpeid'][1])
		$arrDpeReg[] = "d.dpedatafim <= (select dpedatafim from painel.detalheperiodicidade where dpeid = {$arrFiltros['dpeid'][1]} )";
	/* Fim - Aplicação de filtro para regionalizadores */
	
	$count1 = !$arrDetalhes1 ? 1 : count($arrDetalhes1);
	$count2 = !$arrDetalhes2 ? 1 : count($arrDetalhes2); 
	$arrDetalhes1 = !$arrDetalhes1 ? array() : $arrDetalhes1;
	$arrDetalhes2 = !$arrDetalhes2 ? array() : $arrDetalhes2;
	/* FIM - Retira exibição de detalhes sem valores */
	
	//$coslpanGeral = (Periodo + (detalhe1 * detalhe2 * $QtdValor) + regionalizador + total)
	$coslpanGeral = (1 + ( $count1 * $count2 * $QtdValor) + $QtdValor);
				
	if($exibeResponsavel){

		$sqlResp = "select 
					respnome,
					respemail,
					(case when respdddcelular is not null
						then '(' || respdddcelular || ') ' || respcelular
						else 'N/A'
					end) as  celular,
					(case when respdddtelefone is not null
						then '(' || respdddtelefone || ') ' || resptelefone
						else 'N/A'
					end) as  telefone
				from
					painel.responsavelsecretaria
				where
					secid = $secid
				and
					respstatus = 'A'";
				$resp = $db->carregar($sqlResp);
				if($resp){
						$responsaveis = "<table cellspacing=\"0\" border=\"0\" cellpadding=\"0\" >";
					foreach($resp as $rs){
						$responsaveis .= "<tr><td style=\"color:#888888;border:0px\"><b>Responsável:</b> <span style=\"padding-right:30px\" >{$rs['respnome']}</span> </td><td style=\"color:#888888;border:0px\"><b>E-mail:</b></span> {$rs['respemail']}</td></tr>";
						$responsaveis .= "<tr><td style=\"color:#888888;border:0px;\" ><b>Telefone:</b> {$rs['telefone']} </td><td style=\"color:#888888;border:0px\" ><b>Celular:</b></span> {$rs['celular']}</td></tr>";
					}
					$responsaveis .= "</table>";
				}
	}
	
	$exibeFonte;
	?>

<?php /* INÍCIO - EXIBIÇÃO DO INICIO DA TABELA COM O NOME DO INDICADOR */?>
	<table class="tabela" width="100%" bgcolor="FFFFFF" cellSpacing="1" border=0 cellPadding="3" align="center">
<thead>
<?php if($exibeNome){?>
		<tr bgcolor="#e9e9e9" >
			<th style="text-align:center;font-weight:bold" colspan="<?php echo $coslpanGeral ?>"><?=$indnome?></th>
		</tr>
<?php } 
/* FIM - EXIBIÇÃO DO INICIO DA TABELA COM O NOME DO INDICADOR */

/* INÍCIO - EXIBIÇÃO DA OBSERVAÇÃO DO INDICADOR */?>
<?php if($exibeObservacao && ($indobjetivo || $indobservacao) ){?>
		<tr bgcolor="#e9e9e9" >
			<th style="color:#888888;text-align:justify;font-size:11px;font-weight: normal" colspan="<?php echo $coslpanGeral ?>"><?=$indobjetivo?> <? echo $indobservacao ? "<br/><b>Obs:</b> $indobservacao" : ""?></th>
		</tr>
<?php } ?>
<?php /* FIM - EXIBIÇÃO DA OBSERVAÇÃO DO INDICADOR */ 

/* INÍCIO - EXIBIÇÃO DA OBSERVAÇÃO DO INDICADOR */?>
<?php if($exibeResponsavel && $responsaveis){?>
		<tr bgcolor="#e9e9e9" >
			<th style="color:#888888;text-align:justify;font-size:11px;font-weight: normal" colspan="<?php echo $coslpanGeral ?>"><?=$responsaveis?></th>
		</tr>
<?php } ?>
<?php /* FIM - EXIBIÇÃO DA OBSERVAÇÃO DO INDICADOR */ ?>

<?php	
$arrPeriodos = !$arrPeriodos ? array() : $arrPeriodos;

if($arrPeriodos == null){?>
	<tr bgcolor="#e9e9e9" >
		<td style="color:#880000;text-align:center;font-size:11px;font-weight: bold" colspan="<?php echo $coslpanGeral ?>">Não Atendido.</td>
	</tr>
<?php return false;	
}?>

<?php /* INICIO - EXIBIÇÃO DO DETALHES 1 */ ?>
<tr bgcolor="#e9e9e9" >
			<?php $perunidade = $db->pegaUm( "select perunidade from painel.periodicidade where perid = ".($periodo ? $periodo : $perid)."" ) ?>
			<th rowspan="3" style="font-weight:bold;text-align:center;color:#0000AA;"><?=$perunidade?></th>
			
			<?php foreach ( $arrDetalhes1 as $detalhe1){/* Inicio foreach detalhe1 */ ?>
				<th  colspan=<?=($count2 * $QtdValor)?> style="font-weight:bold;text-align:center"><?=$detalhe1['tiddsc']?></th>
			<?php }/* Fim foreach detalhe1 */ ?>
			
			<?php /* INICIO - EXIBIÇÃO DE TOTALIZADORES */ ?>
			<?php if($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0)){ ?>
				<th rowspan="2" <?php echo ( $QtdValor != 1 ? "colspan='$QtdValor'" : "") ?> style="font-weight:bold;text-align:center">Total</th>
			<?php } ?>
			<?php /* FIM - EXIBIÇÃO DE TOTALIZADORES */ ?>
			
</tr>
<?php /* FIM - EXIBIÇÃO DO DETALHES 1 */?>

<?php /* INICIO - EXIBIÇÃO DO DETALHES 2 */ ?>
<tr bgcolor="#e9e9e9" >
	<?php foreach ( $arrDetalhes1 as $detalhe1){/* Inicio foreach detalhe1 */ ?>
			<?php foreach ( $arrDetalhes2 as $detalhe2){/* Inicio foreach detalhe2 */ ?>
				<th  colspan=<?=$QtdValor?> style="font-weight:bold;text-align:center"><?=$detalhe2['tiddsc']?></th>
			<?php }/* Fim foreach detalhe2 */ ?>
	<?php }/* Fim foreach detalhe1 */ ?>
</tr>
<?php /* FIM - EXIBIÇÃO DO DETALHES 2 */ ?>

<?php /* INCIO - EXIBIÇÃO DOS CAMPOS */?>
<tr bgcolor="#e9e9e9" >
<?php for($i=0;$i < ($count1 * $count2); $i++){?>
	<?php if($exibirRegionalizador && $campoReg!= ""){?>
		<th style="text-align:center" ><?=$regunidade?>*</th>
	<?php }?>
	<?php if($ExibeQtd){?>
		<th style="text-align:center" ><?=$umedesc?></th>
	<?php }?>
	<?php if($ExibeValor){?>
		<th style="text-align:center" >Valor (R$)</th>
	<?php }?>
<?php } ?>

	<?php /* INICIO - EXIBIÇÃO DE TOTALIZADORES */ ?>
	<?php if($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0)){ ?>
		<?php if($arrTipoDetalhe != array("naoexibir")){?>
			<?php if($exibirRegionalizador && $campoReg!= ""){?>
				<th style="text-align:center" ><?=$regunidade?>*</th>
			<?php }?>
			<?php if($ExibeQtd){?>
				<th style="text-align:center" ><?=$umedesc?></th>
			<?php }?>
			<?php if($ExibeValor){?>
				<th style="text-align:center" >Valor (R$)</th>
			<?php }?>
		<?php }?>
	<?php }?>
	<?php /* FIM - EXIBIÇÃO DE TOTALIZADORES */ ?>

</tr>
</thead>
<?php /* FIM - EXIBIÇÃO DOS CAMPOS */ ?>

<?php /* INCIO - EXIBIÇÃO DOS PERIODOS */?>
<?php $mascara = pegarMascaraIndicador($indid); $num = 0?>
<?php foreach($arrPeriodos as $dpedid => $dpedsc){
	$cor = ($num%2) ? "#f7f7f7" : "";
				$num++;
				?>
	<tr bgcolor="<?=$cor?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
		<td style="font-weight:bold;text-align:center;color:#0000AA" ><?=$dpedsc?></td>
		
		<?php if(count($arrTipoDetalhe) == 1 && $arrTipoDetalhe != array("naoexibir")){ /* Inicio 1 IF */ ?>
				
				<?php /* INICIO - EXIBIÇÃO DO DETALHES 1 */ ?>
						
						<?php $totalQtde = 0; $totalValor = 0;?>
				
					<?php foreach ( $arrDetalhes1 as $detalhe1){/* Inicio foreach detalhe1 */ ?>
						
								<?php if($exibirRegionalizador && $campoReg!= ""){?>
									<td style="text-align:right" >
										<?php echo ((!$arrValor[$dpedid][$detalhe1['tidid']]['regionalizador']) ? "-" : number_format($arrValor[$dpedid][$detalhe1['tidid']]['regionalizador'],0,3,".")); ?>
									</td>
								<?php }?>
								<?php if($ExibeQtd){?>
									<td style="text-align:right" ><?php echo !$arrValor[$dpedid][$detalhe1['tidid']]['qtde'] ? "-" :  mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']]['qtde']) , $mascara['mascara'] )  ?></td>
									<?php $totalQtde += $arrValor[$dpedid][$detalhe1['tidid']]['qtde']?>
									<?php $totalGeral[$detalhe1['tidid']]['qtde'] += $arrValor[$dpedid][$detalhe1['tidid']]['qtde']?>
								<?php }?>
								<?php if($ExibeValor){?>
									<td style="text-align:right" ><?php echo !$arrValor[$dpedid][$detalhe1['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']]['valor']) , $mascara['campovalor']['mascara'] ) ?></td>
									<?php $totalValor += $arrValor[$dpedid][$detalhe1['tidid']]['valor']?>
									<?php $totalGeral[$detalhe1['tidid']]['valor'] += $arrValor[$dpedid][$detalhe1['tidid']]['valor']?>
								<?php }?>
								
					<?php }/* Fim foreach detalhe1 */ ?>
				<?php /* FIM - EXIBIÇÃO DO DETALHES 1 */ ?>
				
				<?php /* INICIO - EXIBIÇÃO DE TOTALIZADORES */ ?>
				<?php if($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0)){ ?>
					<?php if($exibirRegionalizador && $campoReg!= ""){?>
					<?php					
						$sql = "select 
									count( distinct $campoReg) as reg
								from 
									painel.v_detalheindicadorsh d
								where
									d.indid = $indid
								and
									d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpedid )
								and
									d.dpedatafim <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = $dpedid )
								$andDpe
								".(count($and) ? " and ".implode(" and ",$and) : "" )."
								group by
									d.indid";
						$totalReg = $db->pegaUm($sql); ?>
						<td style="text-align:right;font-weight:bold" ><?=number_format($totalReg,0,3,".")?></td>
					<?php }?>
					<?php if($ExibeQtd){?>
						<td style="text-align:right;font-weight:bold" ><?=mascaraglobal( str_replace(".00", "", $totalQtde) , $mascara['mascara'] )?></td>
						<?php $totalGeralQtde += $totalQtde?>
					<?php }?>
					<?php if($ExibeValor){?>
						<td style="text-align:right;font-weight:bold" ><?=mascaraglobal( str_replace(".00", "", $totalValor) , $mascara['campovalor']['mascara'] )?></td>
						<?php $totalGeralValor += $totalValor?>
					<?php }?>
				<?php } ?>
				<?php /* FIM - EXIBIÇÃO DE TOTALIZADORES */ ?>
				
		<?php } /* Fim 1 IF */?>
		
		<?php if(count($arrTipoDetalhe) == 2){ /* Inicio 2 IF */ ?>
				
				<?php /* INICIO - EXIBIÇÃO DO DETALHES 2 */ ?>
					
					<?php $totalQtde = 0; $totalValor = 0;?>
				
					<?php foreach ( $arrDetalhes1 as $detalhe1){/* Inicio foreach detalhe1 */ ?>
							<?php foreach ( $arrDetalhes2 as $detalhe2){/* Inicio foreach detalhe2 */ ?>
								<?php if($exibirRegionalizador && $campoReg!= ""){?>
									<td style="text-align:right" ><?php echo !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['regionalizador'] ? "-" : $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['regionalizador'] ?></td>
								<?php }?>
								<?php if($ExibeQtd){?>
									<td style="text-align:right" ><?php echo !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] ? "-" :  mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']) , $mascara['mascara'] )  ?></td>
									<?php $totalQtde += $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']?>
									<?php $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] += $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']?>
								<?php }?>
								<?php if($ExibeValor){?>
									<td style="text-align:right" ><?php echo !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor']) , $mascara['campovalor']['mascara'] ) ?></td>
									<?php $totalValor += $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor']?>
									<?php $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['valor'] += $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor']?>
								<?php }?>
							<?php }/* Fim foreach detalhe2 */ ?>
					<?php }/* Fim foreach detalhe1 */ ?>
				<?php /* FIM - EXIBIÇÃO DO DETALHES 2 */ ?>
				
				<?php /* INICIO - EXIBIÇÃO DE TOTALIZADORES */ ?>
				<?php if($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0)){ ?>
					<?php if($exibirRegionalizador && $campoReg!= ""){?>
					<?php					
						$sql = "select 
									count( distinct $campoReg) as reg
								from 
									painel.v_detalheindicadorsh d
								where
									d.indid = $indid
								and
									d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpedid )
								and
									d.dpedatafim <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = $dpedid )
								$andDpe
								".(count($and) ? " and ".implode(" and ",$and) : "" )."
								group by
									d.indid";
						$totalReg = $db->pegaUm($sql); ?>
						<td style="text-align:right;font-weight:bold" ><?=number_format($totalReg,0,3,".")?></td>
					<?php }?>
					<?php if($ExibeQtd){?>
						<td style="text-align:right;font-weight:bold" ><?=mascaraglobal( str_replace(".00", "", $totalQtde) , $mascara['mascara'] )?></td>
						<?php $totalGeralQtde += $totalQtde?>
					<?php }?>
					<?php if($ExibeValor){?>
						<td style="text-align:right;font-weight:bold" ><?=mascaraglobal( str_replace(".00", "", $totalValor) , $mascara['campovalor']['mascara'] )?></td>
						<?php $totalGeralValor += $totalValor?>
					<?php }?>
				<?php } ?>
				<?php /* FIM - EXIBIÇÃO DE TOTALIZADORES */ ?>
				
		<?php } /* Fim 2 IF */?>
		
		<?php if($arrTipoDetalhe == array("naoexibir")){ /* Inicio 3 IF */ ?>
				
				<?php /* INICIO - EXIBIÇÃO DE TOTALIZADORES */ ?>
				<?php if($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0)){ ?>
					<?php if($exibirRegionalizador && $campoReg!= ""){?>
						<?php					
							$sql = "select 
										count( distinct $campoReg) as reg
									from 
										painel.v_detalheindicadorsh d
									where
										d.indid = $indid
									and
										d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = $dpedid )
									and
										d.dpedatafim <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = $dpedid )
									$andDpe
									".(count($and) ? " and ".implode(" and ",$and) : "" )."
									group by
										d.indid";
							$totalReg = $db->pegaUm($sql);?>
						<td style="text-align:right;font-weight:bold" ><?=number_format($totalReg,0,3,".")?></td>
					<?php }?>
					<?php if($ExibeQtd){?>
						<td style="text-align:right;font-weight:bold" ><?=mascaraglobal( str_replace(".00", "", $arrValor[$dpedid]['qtde']) , $mascara['mascara'] )?></td>
						<?php $totalGeralQtde += $arrValor[$dpedid]['qtde']?>
					<?php }?>
					<?php if($ExibeValor){?>
						<td style="text-align:right;font-weight:bold" ><?=mascaraglobal( str_replace(".00", "", $arrValor[$dpedid]['valor']) , $mascara['campovalor']['mascara'] )?></td>
						<?php $totalGeralValor += $arrValor[$dpedid]['valor']?>
					<?php }?>
				<?php } ?>
				<?php /* FIM - EXIBIÇÃO DE TOTALIZADORES */ ?>
				
		<?php } /* Fim 3 IF */?>
		
</tr>
<?php } ?>

<?php /* FIM - EXIBIÇÃO DOS PERIODOS */?>

<?php $arrTotalizadores = count($arrPeriodos) == 1 ? array(0,0) : $arrTotalizadores; ?>

<?php /* INICIO - EXIBIÇÃO DOS TOTALIZADORES GERAIS */ ?>
<?php if(($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0) || $arrTotalizadores == array(0,1)) &&  ( ($indcumulativo == "S" || $indcumulativo == "A") || ($indcumulativovalor == "S" || $indcumulativovalor == "A") ) ){ ?>
	<tr bgcolor="#e9e9e9" >
	<td style="text-align:right;font-weight:bold" >Total Geral</td>
	<?php if($arrTipoDetalhe == array("naoexibir")){ /* Inicio 3 IF */ ?>
	<?php if($exibirRegionalizador && $campoReg!= ""){?>
					
					<?php $sql = "select 
										count( distinct $campoReg) as reg
									from 
										painel.v_detalheindicadorsh d
									where
										d.indid = $indid
									$andDpe
									".(count($and) ? " and ".implode(" and ",$and) : "" )."
									".(count($arrDpeReg) ? " and ".implode(" and ",$arrDpeReg) : "" )."
									group by
										d.indid";
					$totalGeralReg = $db->pegaUm($sql);
					?>
						<td style="text-align:right;font-weight:bold" ><?=number_format($totalGeralReg,0,3,".")?></td>
					<?php }?>
		<?php if($ExibeQtd){?>
			<td style="text-align:right;font-weight:bold" >
				<?php if($indcumulativo == "S"){ ?>
					<?=mascaraglobal( str_replace(".00", "", $totalGeralQtde ), $mascara['mascara'] )?>
				<?php }elseif($indcumulativo == "A" && ($periodo ? $periodo : $perid) == PERIODO_ANUAL){ ?>
					<?=mascaraglobal( str_replace(".00", "", $arrValor[$dpedid]['qtde'] ), $mascara['mascara'] )?>
				<?php }else{ ?>
					-
				<?php }?>
			</td>
		<?php }?>
		<?php if($ExibeValor){?>
			<td style="text-align:right;font-weight:bold" >
				<?php if($indcumulativovalor == "S"){ ?>
					<?=mascaraglobal( str_replace(".00", "", $totalGeralValor) , $mascara['campovalor']['mascara'] )?>
				<?php }elseif($indcumulativovalor == "A" && ($periodo ? $periodo : $perid) == PERIODO_ANUAL){ ?>
					<?=mascaraglobal( str_replace(".00", "", $arrValor[$dpedid]['valor']) , $mascara['campovalor']['mascara'])?>
				<?php }else{ ?>
					-
				<?php }?>
			</td>
		<?php }?>
		
	<?php }else{?>
		
		<?php if(count($arrTipoDetalhe) == 1 && $arrTipoDetalhe != array("naoexibir")){ /* Inicio 1 IF */ ?>
					
					<?php /* INICIO - EXIBIÇÃO DO DETALHES 1 */ ?>
							
						<?php foreach ( $arrDetalhes1 as $detalhe1){/* Inicio foreach detalhe1 */ ?>
							
									<?php if($exibirRegionalizador && $campoReg!= ""){
										
											$sql = "select 
														tdinumero 
													from 
														painel.detalhetipoindicador d1
													inner join
														painel.detalhetipodadosindicador d2 ON d1.tdiid = d2.tdiid 
													where 
														tidid = {$detalhe1['tidid']}";
											$tdinumero = $db->pegaUm($sql);
										
											$sql = "select 
														count( distinct $campoReg) as reg
													from 
														painel.v_detalheindicadorsh d
													where
														 tidid$tdinumero = {$detalhe1['tidid']}
													and
														indid = $indid
													$andDpe
													".(count($and) ? " and ".implode(" and ",$and) : "" )."
													group by
														d.indid";
											$regionalizador = $db->pegaUm($sql);
											?>
											<td style="text-align:right;font-weight:bold" ><?=number_format($regionalizador,0,3,".")?></td>
										
									<?php }?>
									<?php if($ExibeQtd){?>
										<td style="text-align:right;font-weight:bold" >
										<?php if($indcumulativo == "S"){ ?>
											<?php echo !$totalGeral[$detalhe1['tidid']]['qtde'] ? "-" :  mascaraglobal( str_replace(".00", "", $totalGeral[$detalhe1['tidid']]['qtde']) , $mascara['mascara'] )  ?>
											<?php $TotalGeralQtde+=  !$totalGeral[$detalhe1['tidid']]['qtde'] ? 0 : $totalGeral[$detalhe1['tidid']]['qtde']?>
										<?php }elseif($indcumulativo == "A" && ($periodo ? $periodo : $perid) == PERIODO_ANUAL){ ?>
											<?=!$arrValor[$dpedid][$detalhe1['tidid']]['qtde'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']]['qtde']) , $mascara['mascara'] )?>
											<?php $TotalGeralQtde+=  !$arrValor[$dpedid][$detalhe1['tidid']]['qtde'] ? 0 : $arrValor[$dpedid][$detalhe1['tidid']]['qtde']?>
										<?php }else{ ?>
											-
											<?php $TotalGeralQtde+= 0?>
										<?php }?>
										</td>
									<?php }?>
									<?php if($ExibeValor){?>
										<td style="text-align:right;font-weight:bold" >
										<?php if($indcumulativovalor == "S"){ ?>
											<?php echo !$totalGeral[$detalhe1['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $totalGeral[$detalhe1['tidid']]['valor']) , $mascara['campovalor']['mascara'] ) ?>
											<?php $TotalGeralValor+= !$totalGeral[$detalhe1['tidid']]['valor'] ? 0 : $totalGeral[$detalhe1['tidid']]['valor']?>
										<?php }elseif($indcumulativovalor == "A" && ($periodo ? $periodo : $perid) == PERIODO_ANUAL){ ?>
											<?=!$arrValor[$dpedid][$detalhe1['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']]['valor']) , $mascara['campovalor']['mascara'] )?>
											<?php $TotalGeralValor+= !$arrValor[$dpedid][$detalhe1['tidid']]['valor'] ? 0 : $arrValor[$dpedid][$detalhe1['tidid']]['valor']?>
										<?php }else{ ?>
											-
											<?php $TotalGeralValor+= 0?>
										<?php }?>
										</td>
										<?php $totalValor += $arrValor[$dpedid][$detalhe1['tidid']]['valor']?>
									<?php }?>
									
						<?php }/* Fim foreach detalhe1 */ ?>
					<?php /* FIM - EXIBIÇÃO DO DETALHES 1 */ ?>
					
									
			<?php } /* Fim 1 IF */?>
			
			
			<?php if(count($arrTipoDetalhe) == 2){ /* Inicio 2 IF */ ?>
					
					<?php /* INICIO - EXIBIÇÃO DO DETALHES 2 */ ?>
						
						<?php $totalQtde = 0; $totalValor = 0;?>
					
						<?php foreach ( $arrDetalhes1 as $detalhe1){/* Inicio foreach detalhe1 */ ?>
								<?php foreach ( $arrDetalhes2 as $detalhe2){/* Inicio foreach detalhe2 */ ?>
									<?php if($exibirRegionalizador && $campoReg!= ""){
										
											$sql = "select 
														tdinumero 
													from 
														painel.detalhetipoindicador d1
													inner join
														painel.detalhetipodadosindicador d2 ON d1.tdiid = d2.tdiid 
													where 
														tidid = {$detalhe1['tidid']}";
											$tdinumero1 = $db->pegaUm($sql);
											
											$sql = "select 
														tdinumero 
													from 
														painel.detalhetipoindicador d1
													inner join
														painel.detalhetipodadosindicador d2 ON d1.tdiid = d2.tdiid 
													where 
														tidid = {$detalhe2['tidid']}";
											$tdinumero2 = $db->pegaUm($sql);
										
											$sql = "select 
														count( distinct $campoReg) as reg
													from 
														painel.v_detalheindicadorsh d
													where
														 tidid$tdinumero1 = {$detalhe1['tidid']}
													and
														 tidid$tdinumero2 = {$detalhe2['tidid']}
													and
														d.indid = $indid
													$andDpe
													".(count($and) ? " and ".implode(" and ",$and) : "" )."
													group by
														d.indid";
											$regionalizador = $db->pegaUm($sql);
											
											?>
											<td style="text-align:right;font-weight:bold" ><?=number_format($regionalizador,0,3,".")?></td>
									<?php }?>
									<?php if($ExibeQtd){?>
										<td style="text-align:right;font-weight:bold" >
										<?php if($indcumulativo == "S"){ ?>
											<?php echo !$totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] ? "-" :  mascaraglobal( str_replace(".00", "", $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['qtde']) , $mascara['mascara'] )  ?>
											<?php $TotalGeralQtde+= !$totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] ? 0 : $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['qtde']?>
										<?php }elseif($indcumulativo == "A" && ($periodo ? $periodo : $perid) == PERIODO_ANUAL){ ?>
											<?=!$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']) , $mascara['mascara'] )?>
											<?php $TotalGeralQtde+= !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde'] ? 0 : $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']?>
										<?php }else{ ?>
											-
											<?php $TotalGeralQtde+= 0?>
										<?php }?>
										</td>
										<?php $totalQtde += $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['qtde']?>
									<?php }?>
									<?php if($ExibeValor){?>
										<td style="text-align:right;font-weight:bold" >
										<?php if($indcumulativovalor == "S"){ ?>
											<?php echo !$totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['valor']) , $mascara['campovalor']['mascara'] ) ?>
											<?php $TotalGeralValor+= !$totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['valor'] ? 0 : $totalGeral[$detalhe1['tidid']][$detalhe2['tidid']]['valor']?>
										<?php }elseif($indcumulativovalor == "A" && ($periodo ? $periodo : $perid) == PERIODO_ANUAL){ ?>
											<?=!$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor'] ? "-" : mascaraglobal( str_replace(".00", "", $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor']) , $mascara['campovalor']['mascara'] )?>
											<?php $TotalGeralValor+= !$arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor'] ? 0 : $arrValor[$dpedid][$detalhe1['tidid']][$detalhe2['tidid']]['valor']?>
										<?php }else{ ?>
											-
											<?php $TotalGeralValor+= 0?>
										<?php }?>
										</td>
									<?php }?>
								<?php }/* Fim foreach detalhe2 */ ?>
						<?php }/* Fim foreach detalhe1 */ ?>
					<?php /* FIM - EXIBIÇÃO DO DETALHES 2 */ ?>
					
			<?php } /* Fim 2 IF */?>
		<?php if($arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0)){ ?>
			<?php if($exibirRegionalizador && $campoReg!= ""){?>
						
						<?php $sql = "select 
											count( distinct $campoReg) as reg
										from 
											painel.v_detalheindicadorsh d
										where
											indid = $indid
										$andDpe
										".(count($and) ? " and ".implode(" and ",$and) : "" )."
										".(count($arrDpeReg) ? " and ".implode(" and ",$arrDpeReg) : "" )."
										group by
											d.indid";
						
						$totalGeralReg = $db->pegaUm($sql);
						
						?>
							<td style="text-align:right;font-weight:bold" ><?=number_format($totalGeralReg,0,3,".")?></td>
						<?php }?>
			<?php if($ExibeQtd){?>
				<td style="text-align:right;font-weight:bold" ><?=!$TotalGeralQtde || $TotalGeralQtde == 0 ? "-" : mascaraglobal( str_replace(".00", "", $TotalGeralQtde ), $mascara['mascara'] )?></td>
			<?php }?>
			<?php if($ExibeValor){?>
				<td style="text-align:right;font-weight:bold" ><?=!$TotalGeralValor || $TotalGeralValor == 0 ? "-" : mascaraglobal( str_replace(".00", "", $TotalGeralValor) , $mascara['campovalor']['mascara'] )?></td>
			<?php }?>
		<?php } ?>
	<?php } ?>
	</tr>
<?php } ?>
<?php /* FIM - EXIBIÇÃO DOS TOTALIZADORES GERAIS */ ?>

<?php /* INICIO - EXIBIÇÃO DA EXPLICAÇÃO DOS TOTALIZADORES REGIONAIS */ ?>
<?php if($exibirRegionalizador && $campoReg!= "" && ( $arrTotalizadores == array(0,1) || $arrTotalizadores == array(1,1) || $arrTotalizadores == array(1,0) ) ){?>
<tr bgcolor="#e9e9e9" >
	<th style="text-align:left;font-weight:normal" colspan="<?php echo $coslpanGeral ?>">* No cálculo dos totais foram considerada(o)s apenas <?=$regunidade?> distinta(o)s.</th>
</tr>
<?php } ?>
<?php /* FIM - EXIBIÇÃO DA EXPLICAÇÃO DOS TOTALIZADORES REGIONAIS */ ?>

<?php /* INÍCIO - EXIBIÇÃO DA OBSERVAÇÃO DO INDICADOR */?>
<?php if($indfontetermo && $exibeFonte){?>
		<tr bgcolor="#e9e9e9" >
			<td style="text-align:right;font-size:10px;color:#888888" colspan="<?php echo $coslpanGeral ?>">FONTE: <?=$indfontetermo?></td>
		</tr>
<?php } ?>
<?php /* FIM - EXIBIÇÃO DA OBSERVAÇÃO DO INDICADOR */ ?>

</table>
	<?php 
	
}

function pegarMascaraIndicador($indid = null) {
	global $db;
	/*
	 * Verificando o tipo de unidade de medição do indicador
	 * regra 1: se for moeda (unmid=5), o formato dos campos devem ser ###.###.###,##
	 * regra 2: se for Inteiro (unmid=3), verificar se indqtdevalor == true, caso sim, mostrar os dois campos
	 */
	$indid = !$indid ? $_SESSION['indid'] : $indid;
	if($indid) {
		$ind = $db->pegaLinha("SELECT unmid, indqtdevalor FROM painel.indicador WHERE indid='".$indid."'");
		switch($ind['unmid']) {
			case '5':
				$formatoinput = array('mascara'             => '###.###.###.###,##',
									  'size'                => '18',
									  'maxlength'           => '17',
									  'label'               => 'Valor',
									  'unmid'				=> $ind['unmid']);
				break;
			case '1':
				$formatoinput = array('mascara'             => '###.###.###.###,##',
									  'size'                => '18',
									  'maxlength'           => '17',
									  'label'               => 'Valor',
									  'unmid'				=> $ind['unmid']);
				break;
			case '2':
				$formatoinput = array('mascara'             => '###.###.###.###,##',
									  'size'                => '18',
									  'maxlength'           => '17',
									  'label'               => 'Valor',
									  'unmid'				=> $ind['unmid']);
				break;
			case '4':
				$formatoinput = array('mascara'             => '###,##',
									  'size'                => '7',
									  'maxlength'           => '6',
									  'label'               => 'Indíce',
									  'unmid'				=> $ind['unmid']);
				break;
			case '3':
				$formatoinput = array('mascara'             => '###.###.###.###',
									  'size'                => '15',
									  'maxlength'           => '14',
									  'label'               => 'Quantidade',
									  'unmid'				=> $ind['unmid']);
				
				if($ind['indqtdevalor'] == "t") {
					// mostar os dois campos (quantidade e valor)
					$formatoinput['campovalor'] = array('mascara'             => '###.###.###.###,##',
									  					'size'                => '18',
									  					'maxlength'           => '17',
									  					'label'               => 'Valor',
									  					'unmid'				  => $ind['unmid']);
				}
				break;
			default:
				$formatoinput = array('mascara'             => '###.###.###.###',
									  'size'                => '15',
									  'maxlength'           => '14',
									  'label'               => 'Quantidade',
									  'unmid'				=> $ind['unmid']);
		}
		return $formatoinput;
	} else {
		echo "<p align='center'>Problemas na identificação do indicador. <b><a href=\"?modulo=inicio&acao=C\">Clique aqui</a></b> e refaça os procedimentos.</p>";
		//exit;
	}

}

function exibeRelatorioRedeFederalEducacaoSuperior($arrFiltros = array()){
	global $db;
	
	$msgErro = "Não atendido.";
	
	$filtroSQL[] = "tuo.orgid IN('1')";
	
	$filtroSQL[] = "fen.funid IN (18)";
	
	//Filtro por regcod
	if($arrFiltros['regcod'] && $arrFiltros['regcod'] != "" && $arrFiltros['regcod'] != "todos"){
		$filtroSQL[] = "edc.estuf IN ( select estuf from territorios.estado where regcod = '{$arrFiltros['regcod']}' ) ";
		$msgErro = "A região não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	//Filtro por estuf
	if($arrFiltros['estuf'] && $arrFiltros['estuf'] != "" && $arrFiltros['estuf'] != "todos"){
		$filtroSQL[] = "edc.estuf = '{$arrFiltros['estuf']}'";
		$msgErro = "O estado não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	//Filtro por tpmid
	if($arrFiltros['tpmid'] && $arrFiltros['tpmid'] != "" && $arrFiltros['tpmid'] != "todos"){
		$filtroSQL[] = "edc.muncod in (	select muncod from territorios.muntipomunicipio where tpmid = '{$arrFiltros['tpmid']}' )";
	}
	//Filtro por muncod
	if($arrFiltros['muncod'] && $arrFiltros['muncod'] != "" && $arrFiltros['muncod'] != "todos"){
		$filtroSQL[] = "edc.muncod = '{$arrFiltros['muncod']}' ";
		$municipio = true;
		$msgErro = "O município não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	//Filtro por mescod
	if($arrFiltros['mescod'] && $arrFiltros['mescod'] != ""){
		$filtroSQL[] = "mun.mescod  = '{$arrFiltros['mescod']}' ";
		$msgErro = "A mesorregião não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	
	//Filtro solicitado pelo Vitor a pedido da Manoela (388754)
		$filtroSQL[] = "uor.entid NOT IN (388754) ";
	
	//Filtro solicitado pelo Vitor
		$filtroSQL[] = "uor.entstatus = 'A' AND ent.entstatus = 'A' ";
		
	$sql =  "SELECT  
					mun.mundescricao,
					ent.entnome, 
					uor.entsig,
					uor.entid as ent_pai, 
					uor.entnome as nomeinst,
				 	exi.exidsc, 
				 	cam.cmpsituacao, 
				 	cam.cmpinstalacao, 
				 	cam.cmpsituacaoobra, 
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_SUP."' AND cmi.cpiano = '2009' AND cpitabnum=1) AS ano2009,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_SUP."' AND cmi.cpiano = '2010' AND cpitabnum=1) AS ano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_SUP."' AND cmi.cpiano = '2011' AND cpitabnum=1) AS ano2011,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_VAGAS_SUP."' AND cmi.cpiano = '2012' AND cpitabnum=1) AS ano2012,
				 to_char(cam.cmpdatainauguracao::date,'DD/MM/YYYY') as cmpdatainauguracao,
				 cam.cmpdataimplantacao,
				 (SELECT SUM(cpivalor) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVESTIMENTO_SUP."' AND cpitabnum=1) AS insvtot,
				 (SELECT CASE WHEN count(*) >= 1 THEN 'Sim' ELSE 'Não' END FROM obras.obrainfraestrutura oi WHERE stoid = 1 AND obsstatus = 'A' AND entidcampus = cam.entid) AS obrascampus,
				 cam.datacriacao,
				 ct.cptdsc,
				 uor.entobs,
				 uor.entescolanova
		  FROM academico.campus cam 
		  LEFT JOIN academico.existencia exi ON exi.exiid = cam.exiid  
		  LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid 		  
		  LEFT JOIN entidade.endereco edc ON edc.entid = ent.entid 
		  LEFT JOIN territorios.municipio mun ON mun.muncod = edc.muncod 
		  LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
		  LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid    
		  LEFT JOIN entidade.entidade uor ON uor.entid = fea.entid 
		  LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = uor.entid 
		  LEFT JOIN academico.orgaouo tuo ON tuo.funid = fen2.funid
		  LEFT JOIN academico.campustipo ct ON ct.cptid = cam.cptid
		  ". (($filtroSQL)?"WHERE ".implode(" AND ",$filtroSQL):"") ." 
		  ORDER BY uor.entsig, edc.estuf, ent.entnome";
	$dadoscsv = $db->carregar($sql);
	
	$htmlHEADER = "<table cellspacing=1 cellpadding=3 class=\"tabela\" align=\"center\" bgcolor=\"#ffffff\" >
				<tr style=\"font-weight: bold; text-align: center; color: rgb(0, 0, 170);\" bgcolor=\"#e9e9e9\">
					<td colspan='14' align='center'>
					MINISTÉRIO DA EDUCAÇÃO<br>
					Responsável pela informação: Secretaria de Educação Superior (SESU)<br>
					<br>
					Expansão da Educação Superior
					</td>
				</tr>";
		$htmlCampos =  "
				<tr>
					".($municipio ? "" : "<td rowspan='2' class='SubTituloCentro'>Município</td>")."
					<td rowspan='2' class='SubTituloCentro'>Campus</td>
					<td rowspan='2' class='SubTituloCentro'>Status</td>
					<td rowspan='2' class='SubTituloCentro'>Situação</td>
					<td rowspan='2' class='SubTituloCentro'>Instalações</td>
					<!--<td rowspan='2' class='SubTituloCentro'>Situação sobre obras</td>-->
					<td rowspan='2' class='SubTituloCentro'>Obras no campus</td>
					<td colspan='4' class='SubTituloCentro'>VAGAS PACTUADAS</td>
					<!--<td rowspan='2' class='SubTituloCentro'>Data de criação</td>-->
					<td rowspan='2' class='SubTituloCentro'>Tipo</td>
					<!--<td rowspan='2' class='SubTituloCentro'>Data do início das atividades</td>-->
					<!--<td rowspan='2' class='SubTituloCentro'>Implantação</td>-->
					<!--<td rowspan='2' class='SubTituloCentro'>Investimento total</td>-->
				</tr>
				<tr>
					<td class='SubTituloCentro'>2009</td>
					<td class='SubTituloCentro'>2010</td>
					<td class='SubTituloCentro'>2011</td>
					<td class='SubTituloCentro'>2012</td>
				</tr>";
	
	if($dadoscsv[0]){
		
		//$_cmpexistencia = array("N"=>"Novo","P"=>"Preexistente", "R"=>"Previsto");
		$_cmpinstalacao = array("P"=>"Instalações Provisórias","D"=>"Instalações Definitivas" ,"S"=>"Sem Instalações");
		$_cmpsituacao = array("F"=>"Funcionando","N"=>"Não Funcionando");
		$_cmpsituacaoobra = array("L"=>"Licitação de Obras","A"=>"Obras em Andamento","C"=>"Obras Concluídas");
		$_cmp1etapa = array("N"=>"Não se aplica","A"=>"Em andamento","C"=>"Concluída");
				
		$i=1;
		$html = array();
		
		foreach($dadoscsv as $registro) {
			
			$cor = ($i%2) ? "#f7f7f7" : "";
			$i++;
			
			$html [$registro['ent_pai']]['header'] = "
				<tr style=\"font-weight: bold; text-align: center;\" bgcolor=\"#cccccc\">
					<td colspan='14' style=\"color:#005500;font-size:14px;font-weight:bold;text-align:center\" align='center'>
					{$registro['nomeinst']} - {$registro['entsig']}
					".(($registro['entescolanova'] == "t" && $registro['entobs'] != "")?"<br>({$registro['entobs']})":"")."
					</td>
				</tr>
				<tr>
					".($municipio ? "" : "<td rowspan='2' class='SubTituloCentro'>Município</td>")."
					<td rowspan='2' class='SubTituloCentro'>Campus</td>
					<td rowspan='2' class='SubTituloCentro'>Status</td>
					<td rowspan='2' class='SubTituloCentro'>Situação</td>
					<td rowspan='2' class='SubTituloCentro'>Instalações</td>
					<!--<td rowspan='2' class='SubTituloCentro'>Situação sobre obras</td>-->
					<td rowspan='2' class='SubTituloCentro'>Obras no campus</td>
					<td colspan='4' class='SubTituloCentro'>VAGAS PACTUADAS</td>
					<!--<td rowspan='2' class='SubTituloCentro'>Data de criação</td>-->
					<td rowspan='2' class='SubTituloCentro'>Tipo</td>
					<!--<td rowspan='2' class='SubTituloCentro'>Data do início das atividades</td>-->
					<!--<td rowspan='2' class='SubTituloCentro'>Implantação</td>-->
					<!--<td rowspan='2' class='SubTituloCentro'>Investimento total</td>-->
				</tr>
				<tr>
					<td class='SubTituloCentro'>2009</td>
					<td class='SubTituloCentro'>2010</td>
					<td class='SubTituloCentro'>2011</td>
					<td class='SubTituloCentro'>2012</td>
				</tr>";
			
			/* Gerar linha HTML */
			//if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			$html [$registro['ent_pai']]['tr'][$i] = "<tr onmouseout=\"this.bgColor='".$cor."';\" onmouseover=\"this.bgColor='#ffffcc';\" bgcolor='".$cor."'>".($municipio ? "" : "<td>".$registro['mundescricao']."</td>")."<td>".$registro['entnome']."</td><td>".$registro['exidsc']."</td><td>".$_cmpsituacao[$registro['cmpsituacao']]."</td><td>".$_cmpinstalacao[$registro['cmpinstalacao']]."</td><!--<td>".$_cmpsituacaoobra[$registro['cmpsituacaoobra']]."</td>--><td align='center'>" . $registro['obrascampus'] . "</td><td align='right'>".number_format($registro['ano2009'],0,',','.')."</td><td align='right'>".number_format($registro['ano2010'],0,',','.')."</td><td align='right'>".number_format($registro['ano2011'],0,',','.')."</td><td align='right'>".number_format($registro['ano2012'],0,',','.')."</td><!--<td>".((trim($registro['datacriacao']))?substr($registro['datacriacao'],4,2)."/".substr($registro['datacriacao'],0,4):"")."</td>--><td>".$registro['cptdsc']."</td><!--<td>".((trim($registro['cmpdataimplantacao']))?substr($registro['cmpdataimplantacao'],4,2)."/".substr($registro['cmpdataimplantacao'],0,4):"")."</td>--><!--<td>".$registro['cmpdatainauguracao']."</td>--><!--<td align='right'>".number_format($registro['insvtot'], 2, ',', '.')."</td> --></tr>";
			
			$total_insvtot [$registro['ent_pai']][$i] = $registro['insvtot'];
			//$total_matatual [$registro['ent_pai']][$i] = $registro['ano'.date("Y")];
			$total_ano2012 [$registro['ent_pai']][$i] = $registro['ano2012'];
			$total_ano2011 [$registro['ent_pai']][$i] = $registro['ano2011'];
			$total_ano2010 [$registro['ent_pai']][$i] = $registro['ano2010'];
			$total_ano2009 [$registro['ent_pai']][$i] = $registro['ano2009'];
			
			$arrRegistro[$registro['ent_pai']][] = $registro['cmpexistencia'];
			$arrSituacao[$registro['ent_pai']][] = $registro['cmpsituacao'];
			$arrInstalacao[$registro['ent_pai']][] = $registro['cmpinstalacao'];
			
			$totalGeral_insvtot += $registro['insvtot'];
			//$totalGeral_matatual += $registro['ano'.date("Y")];
			$totalGeral_ano2012 += $registro['ano2012'];
			$totalGeral_ano2011 += $registro['ano2011'];
			$totalGeral_ano2010 += $registro['ano2010'];
			$totalGeral_ano2009 += $registro['ano2009'];
			
		}
		
		$arrRegistro = !$arrRegistro ? array() : $arrRegistro;
		
		foreach($arrRegistro as $k => $reg){
			$i = 0;
			foreach($reg as $re){
				switch($re){
					case "N":
						$arrN[$k][] = $re;
						$arrTotalN[] = $re;
					break; 
					case "P":
						$arrP[$k][] = $re;
						$arrTotalP[] = $re;
					break;
					case "R":
						$arrR[$k][] = $re;
						$arrTotalR[] = $re;
					break;
				}
				
				switch($arrSituacao[$k][$i]){
					case "F":
						$arrSF[$k][] = $arrSituacao[$k][$i];
						$arrTotalSF[] = $arrSituacao[$k][$i];
					break; 
					case "N":
						$arrSN[$k][] = $arrSituacao[$k][$i];
						$arrTotalSN[] = $arrSituacao[$k][$i];
					break;
				}
				
				switch($arrInstalacao[$k][$i]){
					case "P":
						$arrIP[$k][] = $arrInstalacao[$k][$i];
						$arrTotalIP[] = $arrInstalacao[$k][$i];
					break; 
					case "D":
						$arrID[$k][] = $arrInstalacao[$k][$i];
						$arrTotalID[] = $arrInstalacao[$k][$i];
					break;
					case "S":
						$arrIS[$k][] = $arrInstalacao[$k][$i];
						$arrTotalIS[] = $arrInstalacao[$k][$i];
					break;
				}
				
				if($arrRegistro[$k][$i] == "N" && $arrSituacao[$k][$i] == "F"){
					$arrSSF[$k][] = 1;
					$arrTotalSSF[] = 1;
				}
				if($arrRegistro[$k][$i] == "N" && $arrSituacao[$k][$i] == "N"){
					$arrSSN[$k][] = 1;
					$arrTotalSSN[] = 1;
				}
				
				$i++;
				
			}
			
			$arrSSF[$k] = !$arrSSF[$k] ? array(0) : $arrSSF[$k];
			$arrSSN[$k] = !$arrSSN[$k] ? array(0) : $arrSSN[$k];
			$arrTotalSSF = !$arrTotalSSF ? array(0) : $arrTotalSSF;
			$arrTotalSSN = !$arrTotalSSN ? array(0) : $arrTotalSSN;
			
			$html [$k]['css'] = "<style>tr.bold td{font-weight:bold} </style> <tr class=\"bold\" bgcolor='#e9e9e9'><td align=\"right\" colspan=".($municipio ? "5" : "6")." >Total</td><td align='right'>".number_format(array_sum($total_ano2009[$k]),0,',','.')."</td><td align='right'>".number_format(array_sum($total_ano2010[$k]),0,',','.')."</td><td align='right'>".number_format(array_sum($total_ano2011[$k]),0,',','.')."</td><td align='right'>".number_format(array_sum($total_ano2012[$k]),0,',','.')."</td><td colspan=1></td><!--<td>-</td>--><!--<td align='right'>".number_format( array_sum($total_insvtot[$k]), 2, ',', '.')."</td>--></tr>";
			$html [$k]['tr2'] = "<tr class=\"bold\" bgcolor='#e9e9e9'><td valign=\"top\" colspan=".($municipio ? "1" : "2")." ><span style=\"color:#0000AA\">Campus:</span> ".number_format(count($html[$k]['tr']),0,',','.')."</td><td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Status:</span><br/> - Novo: ".number_format(count($arrN[$k]),0,',','.')."<br/>&nbsp;&nbsp;&nbsp;&nbsp;- Funcionando: ".number_format( array_sum($arrSSF[$k]) ,0,',','.')."<br/>&nbsp;&nbsp;&nbsp;&nbsp;- Não Funcionando: ".number_format( array_sum($arrSSN[$k]) ,0,',','.')."<br/> - Preexistente: ".number_format(count($arrP[$k]),0,',','.')."<br/>- Previsto: ".number_format(count($arrR[$k]),0,',','.')."</td><td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Situação:</span><br/> - Funcionando: ".number_format(count($arrSF[$k]),0,',','.')."<br/> - Não Funcionando: ".number_format(count($arrSN[$k]),0,',','.')."</td><td valign=\"top\" colspan=5 ><span style=\"color:#0000AA\">Instalações:</span><br/> - Provisória: ".number_format(count($arrIP[$k]),0,',','.')."<br/> - Definitiva: ".number_format(count($arrID[$k]),0,',','.')."<br/> - Sem Instalação: ".number_format(count($arrIS[$k]),0,',','.')."</td></tr>";
			
			$htmlCABECALHOTOTALGERAL = "<style>tr.bold td{font-weight:bold} </style> <tr class=\"bold\" bgcolor='#cccccc'><td rowspan=2 colspan=".($municipio ? "5" : "6")." ><td colspan=4 align=\"center\" >VAGAS PACTUADAS</td><td rowspan=2></td><!--<td rowspan=2 align='center'>Investimento total</td>--></tr>";
			$htmlCABECALHOTOTALGERAL .= "<style>tr.bold td{font-weight:bold} </style> <tr class=\"bold\" bgcolor='#cccccc'><td>2009</td><td>2010</td><td>2011</td><td>2012</td></tr>";
			
			$htmlTOTALGERAL = "<style>tr.bold td{font-weight:bold} </style> <tr class=\"bold\" bgcolor='#cccccc'><td align=\"right\"colspan=".($municipio ? "5" : "6")." >Total Geral</td><td align='right'>".number_format($totalGeral_ano2009,0,',','.')."</td><td align='right'>".number_format($totalGeral_ano2010,0,',','.')."</td><td align='right'>".number_format($totalGeral_ano2011,0,',','.')."</td><td align='right'>".number_format($totalGeral_ano2012,0,',','.')."</td><td colspan=1></td><!--<td>-</td>--><!--<td align='right'>".number_format($totalGeral_insvtot, 2, ',', '.')."</td>--></tr>";
			$htmlTOTALGERAL .= "<tr class=\"bold\" bgcolor='#cccccc'><td valign=\"top\" colspan=".($municipio ? "1" : "2")." ><span style=\"color:#0000AA\">Campus:</span> ".number_format(count($dadoscsv),0,',','.')."</td><td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Status:</span><br/> - Novo: ".number_format(count($arrTotalN),0,',','.')."<br/>&nbsp;&nbsp;&nbsp;&nbsp;- Funcionando: ".number_format( array_sum($arrTotalSSF) ,0,',','.')."<br/>&nbsp;&nbsp;&nbsp;&nbsp;- Não Funcionando: ".number_format( array_sum($arrTotalSSN) ,0,',','.')."<br/> - Preexistente: ".number_format(count($arrTotalP),0,',','.')."<br/>- Previsto: ".number_format(count($arrTotalR),0,',','.')."</td><td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Situação:</span><br/> - Funcionando: ".number_format(count($arrTotalSF),0,',','.')."<br/> - Não Funcionando: ".number_format(count($arrTotalSN),0,',','.')."</td><td valign=\"top\" colspan=5 ><span style=\"color:#0000AA\">Instalações:</span><br/> - Provisória: ".number_format(count($arrTotalIP),0,',','.')."<br/> - Definitiva: ".number_format(count($arrTotalID),0,',','.')."<br/> - Sem Instalação: ".number_format(count($arrTotalIS),0,',','.')."</td></tr>";
			$htmlTOTALGERAL .= "<tr class=\"bold\" style=\"color:#cccccc\" bgcolor='#cccccc'><td colspan=\"14\" valign=\"top\" >.</td></tr>";
		}
		
	}else{
		$htmlNA = "<tr><td style=\"color:#990000;text-align:center;font-weight:bold\" colspan=\"10\" >$msgErro</td></tr>";
	}
	$htmlFIM = "</table>";
		
		echo $htmlHEADER;
		if(count($html) != 1 ){
			echo $htmlCABECALHOTOTALGERAL;
			echo $htmlTOTALGERAL;	
		}
	if(count($html) != 0 ){
		foreach($html as $k => $h){
			echo $html[$k]['header'];
			if(count($h["tr"]) == 0){
				echo $htmlNA;
			}else{
				foreach($h["tr"] as $i){
					echo $i;
				}
				if(count($h["tr"]) > 1){
					echo $html[$k]['css'];
					echo $html[$k]['tr2'];
				}
			}
		}
	}else{
		echo $htmlCampos;
		echo $htmlNA;
	}
	echo $htmlFIM;
}


function exibeRelatorioRedeFederalEducacaoProfissional($arrFiltros = array()){
	global $db;
	
	$msgErro = "Não atendido.";
	
	$filtroSQL[] = "tuo.orgid IN('2')";
	
	$filtroSQL[] = "fen.funid IN (17)";
	
	//Filtro por regcod
	if($arrFiltros['regcod'] && $arrFiltros['regcod'] != "" && $arrFiltros['regcod'] != "todos"){
		$filtroSQL[] = "edc.estuf IN ( select estuf from territorios.estado where regcod = '{$arrFiltros['regcod']}' ) ";
		$msgErro = "A região não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	//Filtro por estuf
	if($arrFiltros['estuf'] && $arrFiltros['estuf'] != "" && $arrFiltros['estuf'] != "todos"){
		$filtroSQL[] = "edc.estuf = '{$arrFiltros['estuf']}'";
		$msgErro = "O estado não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	//Filtro por tpmid
	if($arrFiltros['tpmid'] && $arrFiltros['tpmid'] != "" && $arrFiltros['tpmid'] != "todos"){
		$filtroSQL[] = "edc.muncod in (	select muncod from territorios.muntipomunicipio where tpmid = '{$arrFiltros['tpmid']}' )";
		$msgErro = "Não atendido.";
	}
	//Filtro por muncod
	if($arrFiltros['muncod'] && $arrFiltros['muncod'] != "" && $arrFiltros['muncod'] != "todos"){
		$filtroSQL[] = "edc.muncod = '{$arrFiltros['muncod']}' ";
		$municipio = true;
		$msgErro = "O município não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	//Filtro por mescod
	if($arrFiltros['mescod']){
		$filtroSQL[] = "edc.estuf IN ( select estuf from territorios.mesoregiao where mescod = '{$arrFiltros['mescod']}' ) ";
		$msgErro = "A mesorregião não solicitou recursos deste programa, não preencheu os critérios de atendimento do MEC, ou está com processo em andamento.";
	}
	
	//Filtros solicitados pela Elisangela
		//Não exibir as instiuições "Colégio Pedro II", "Instituto Nacional de Educação de Surdos" e "Instituto Benjamin Constant"
		$filtroSQL[] = "uor.entid NOT IN (411791, 411790, 388730) ";
		//Não exibir os campus previstos (Regra retirada dia 13/10/2010 às 10:40 a pedido da Manoela)
		//$filtroSQL[] = "cam.cmpexistencia <> 'R' ";
	//Manoela solicitou para retornar os campos avançados 14-04-2010 às 11:40
		//$filtroSQL[] = "ent.entid NOT IN (391951,391827,391949,392760,391948,392763,392761,392762,411770,411747,389177,614647,614658,614648,614699,614655,614688,614700,614682,614659,614687,614685,614697,614683,614649,614695,614656,614698,614653,614694,614660,614650,614651,614654,614681,614696) ";
		//$filtroSQL[] = "ent.entid NOT IN (391827,391949,392760,391948,392763,392761,392762,411770,411747,389177) ";
		
	//Filtro solicitado pelo Vitor
		$filtroSQL[] = "uor.entstatus = 'A' AND ent.entstatus = 'A' ";
	
	$sql =  "SELECT  
					".($municipio ? "" : "mun.mundescricao,")." 
					ent.entnome,
					uor.entid as ent_pai, 
					uor.entsig,
					uor.entnome as nomeinst,
				 	exi.exidsc, 
				 	cam.cmpsituacao, 
				 	cam.cmpinstalacao, 
				 	cam.cmpsituacaoobra, 
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_OFERTATUAL_PROF."' AND cmi.cpiano = '2009' AND cpitabnum=1) AS matatual,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_PREVISTA_PROF."' AND cmi.cpiano = '2010' AND cpitabnum=1) AS ano2010,
				 (SELECT coalesce(cast(cpivalor as varchar),'') FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_MAT_PREVISTA_PROF."' AND cmi.cpiano = '2012' AND cpitabnum=1) AS ano2012,
				 to_char(cam.cmpdatainauguracao::date,'DD/MM/YYYY') as cmpdatainauguracao,
				 cam.cmpdataimplantacao,
				 (SELECT SUM(cpivalor) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_PREVISTO_PROF."' AND cpitabnum=1) AS insprevisto,
				 (SELECT SUM(cpivalor) FROM academico.campusitem cmi WHERE cmi.cmpid = cam.cmpid AND cmi.itmid = '".ITM_INVS_REALIZADO_PROF."' AND cpitabnum=1) AS insrealizado,
				 (SELECT CASE WHEN count(*) >= 1 THEN 'Sim' ELSE 'Não' END FROM obras.obrainfraestrutura oi WHERE stoid = 1 AND obsstatus = 'A' AND entidcampus = cam.entid) AS obrascampus,
				 cam.datacriacao,
				 ct.cptdsc
		  FROM academico.campus cam 
		  LEFT JOIN academico.existencia exi ON exi.exiid = cam.exiid
		  LEFT JOIN entidade.entidade ent ON ent.entid = cam.entid  
		  LEFT JOIN entidade.endereco edc ON edc.entid = ent.entid 
		  LEFT JOIN territorios.municipio mun ON mun.muncod = edc.muncod 
		  LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
		  LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid    
		  LEFT JOIN entidade.entidade uor ON uor.entid = fea.entid 
		  LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = uor.entid 
		  LEFT JOIN academico.orgaouo tuo ON tuo.funid = fen2.funid
		  LEFT JOIN academico.campustipo ct ON ct.cptid = cam.cptid
		  ". (($filtroSQL)?"WHERE ".implode(" AND ",$filtroSQL):"") ." 
		  ORDER BY edc.estuf, uor.entsig, ent.entnome";
	$dadoscsv = $db->carregar($sql);
	
	$htmlHEADER = "<table cellspacing=1 cellpadding=3 class=\"tabela\" align=\"center\" bgcolor=\"#ffffff\">
					<tr style=\"font-weight: bold; text-align: center; color: rgb(0, 0, 170);\" bgcolor=\"#e9e9e9\">
						<td colspan='14' align='center'>
						MINISTÉRIO DA EDUCAÇÃO<br>
						Responsável pela informação: Secretaria de Educação Profissional e Tecnológica (SETEC)<br>
						<br>
						Expansão da Educação Profissional e Tecnológica
						</td>
					</tr>";
		$htmlCampos =  "<tr>
						".($municipio ? "" : "<td rowspan='2' class='SubTituloCentro'>Município</td>")."
						<td rowspan='2' class='SubTituloCentro'>Campus</td>
						<td rowspan='2' class='SubTituloCentro'>Status</td>
						<td rowspan='2' class='SubTituloCentro'>Situação</td>
						<td rowspan='2' class='SubTituloCentro'>Instalações</td>
						<!--<td rowspan='2' class='SubTituloCentro'>Situação sobre obras</td>-->
						<td rowspan='2' class='SubTituloCentro'>Obras no campus</td>
						<td colspan='3' class='SubTituloCentro'>MATRÍCULAS</td>
						<!--<td rowspan='2' class='SubTituloCentro'>Data de criação</td>-->
						<td rowspan='2' class='SubTituloCentro'>Tipo</td>
						<!--<td rowspan='2' class='SubTituloCentro'>Data do início das atividades</td>-->
						<!--<td rowspan='2' class='SubTituloCentro'>Data de inauguração</td>-->
						<!--<td rowspan='2' class='SubTituloCentro'>Investimento atual</td>
						<td rowspan='2' class='SubTituloCentro'>Investimento previsto</td> -->
					</tr>
					<tr>
						<td class='SubTituloCentro'>2009</td>
						<td class='SubTituloCentro'>2010</td>
						<td class='SubTituloCentro'>Funcionamento Pleno</td>
					</tr>";
	
	if($dadoscsv[0]){
		
		//$_cmpexistencia = array("N"=>"Novo","P"=>"Preexistente","R"=>'Previsto');
		$_cmpinstalacao = array("P"=>"Instalações Provisórias","D"=>"Instalações Definitivas" ,"S"=>"Sem Instalações");
		$_cmpsituacao = array("F"=>"Funcionando","N"=>"Não Funcionando");
		$_cmpsituacaoobra = array("L"=>"Licitação de Obras","A"=>"Obras em Andamento","C"=>"Obras Concluídas");
		$_cmp1etapa = array("N"=>"Não se aplica","A"=>"Em andamento","C"=>"Concluída");
		
		$i=1;
		foreach($dadoscsv as $registro) {
			
			$cor = ($i%2) ? "#f7f7f7" : "";
			$i++;
			
			$html [$registro['ent_pai']]['header'] = "
				<tr style=\"font-weight: bold; text-align: center;\" bgcolor=\"#cccccc\">
					<td colspan='14' style=\"color:#005500;font-size:14px;font-weight:bold;text-align:center\" align='center'>
			{$registro['nomeinst']} - {$registro['entsig']}
					</td>
				</tr>
				<tr>
						".($municipio ? "" : "<td rowspan='2' class='SubTituloCentro'>Município</td>")."
						<td rowspan='2' class='SubTituloCentro'>Campus</td>
						<td rowspan='2' class='SubTituloCentro'>Status</td>
						<td rowspan='2' class='SubTituloCentro'>Situação</td>
						<td rowspan='2' class='SubTituloCentro'>Instalações</td>
						<!--<td rowspan='2' class='SubTituloCentro'>Situação sobre obras</td>-->
						<td rowspan='2' class='SubTituloCentro'>Obras no campus</td>
						<td colspan='3' class='SubTituloCentro'>MATRÍCULAS</td>
						<!--<td rowspan='2' class='SubTituloCentro'>Data de criação</td>-->
						<td rowspan='2' class='SubTituloCentro'>Tipo</td>
						<!--<td rowspan='2' class='SubTituloCentro'>Data do início das atividades</td>-->
						<!--<td rowspan='2' class='SubTituloCentro'>Data de inauguração</td>-->
						<!-- <td rowspan='2' class='SubTituloCentro'>Investimento atual</td>
						<td rowspan='2' class='SubTituloCentro'>Investimento previsto</td> -->
					</tr>
					<tr>
						<td class='SubTituloCentro'>2009</td>
						<td class='SubTituloCentro'>2010</td>
						<td class='SubTituloCentro'>Funcionamento Pleno</td>
					</tr>";
			
			
			/* Gerar linha HTML */
			//if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			$html [$registro['ent_pai']]['tr'][$i] = "<tr onmouseout=\"this.bgColor='".$cor."';\" onmouseover=\"this.bgColor='#ffffcc';\" bgcolor='".$cor."'>".($municipio ? "" : "<td>".$registro['mundescricao']."</td>")."<td>".$registro['entnome']."</td><td>".$registro['exidsc']."</td><td>".$_cmpsituacao[$registro['cmpsituacao']]."</td><td>".$_cmpinstalacao[$registro['cmpinstalacao']]."</td><!--<td>".$_cmpsituacaoobra[$registro['cmpsituacaoobra']]."</td>--><td align='center'>" . $registro['obrascampus'] . "</td><td align='right'>".number_format($registro['matatual'],0,',','.')."</td><td align='right'>".number_format($registro['ano2010'],0,',','.')."</td><td align='right'>".number_format($registro['ano2012'],0,',','.')."</td><!--<td>".((trim($registro['datacriacao'])) ? substr($registro['datacriacao'],4,2)."/".substr($registro['datacriacao'],0,4) : "")."</td>--><td>".$registro['cptdsc']."</td><!--<td>".((trim($registro['cmpdataimplantacao']))?substr($registro['cmpdataimplantacao'],4,2)."/".substr($registro['cmpdataimplantacao'],0,4):"")."</td>--><!--<td>".$registro['cmpdatainauguracao']."</td>--><!-- <td align='right'>".number_format($registro['insrealizado'], 2, ',', '.')."</td><td align='right'>".number_format($registro['insprevisto'], 2, ',', '.')."</td> --></tr>";
			
			$total_insprevisto[$registro['ent_pai']][$i] = $registro['insprevisto'];
			$total_insrealizado[$registro['ent_pai']][$i] = $registro['insrealizado'];
			$total_matatual [$registro['ent_pai']][$i] = $registro['matatual'];
			$total_ano2012 [$registro['ent_pai']][$i] = $registro['ano2012'];
			$total_ano2010 [$registro['ent_pai']][$i] = $registro['ano2010'];
			
			$arrRegistro[$registro['ent_pai']][] = $registro['cmpexistencia'];
			$arrSituacao[$registro['ent_pai']][] = $registro['cmpsituacao'];
			$arrInstalacao[$registro['ent_pai']][] = $registro['cmpinstalacao'];
			
			$totalGeral_insprevisto += $registro['insprevisto'];
			$totalGeral_insrealizado += $registro['insrealizado'];
			$totalGeral_matatual += $registro['matatual'];
			$totalGeral_ano2012 += $registro['ano2012'];
			$totalGeral_ano2010 += $registro['ano2010'];
			
			
		}
			$arrRegistro = !$arrRegistro ? array() : $arrRegistro;
			foreach($arrRegistro as $k => $reg){
				$i = 0;
				foreach($reg as $re){
					switch($re){
						case "N":
							$arrN[$k][] = $re;
							$arrTotalN[] = $re;
						break; 
						case "P":
							$arrP[$k][] = $re;
							$arrTotalP[] = $re;
						break;
						case "R":
							$arrR[$k][] = $re;
							$arrTotalR[] = $re;
						break;
					}
					
					switch($arrSituacao[$k][$i]){
						case "F":
							$arrSF[$k][] = $arrSituacao[$k][$i];
							$arrTotalSF[] = $arrSituacao[$k][$i];
						break; 
						case "N":
							$arrSN[$k][] = $arrSituacao[$k][$i];
							$arrTotalSN[] = $arrSituacao[$k][$i];
						break;
					}
					
					switch($arrInstalacao[$k][$i]){
						case "P":
							$arrIP[$k][] = $arrInstalacao[$k][$i];
							$arrTotalIP[] = $arrInstalacao[$k][$i];
						break; 
						case "D":
							$arrID[$k][] = $arrInstalacao[$k][$i];
							$arrTotalID[] = $arrInstalacao[$k][$i];
						break;
						case "S":
							$arrIS[$k][] = $arrInstalacao[$k][$i];
							$arrTotalIS[] = $arrInstalacao[$k][$i];
						break;
					}
					
					if($arrRegistro[$k][$i] == "N" && $arrSituacao[$k][$i] == "F"){
						$arrSSF[$k][] = 1;
						$arrTotalSSF[] = 1;
					}
					if($arrRegistro[$k][$i] == "N" && $arrSituacao[$k][$i] == "N"){
						$arrSSN[$k][] = 1;
						$arrTotalSSN[] = 1;
					}
					
					$i++;
					
				}
				
				$arrSSF[$k] = !$arrSSF[$k] ? array(0) : $arrSSF[$k];
				$arrSSN[$k] = !$arrSSN[$k] ? array(0) : $arrSSN[$k];
				$arrTotalSSF = !$arrTotalSSF ? array(0) : $arrTotalSSF;
				$arrTotalSSN = !$arrTotalSSN ? array(0) : $arrTotalSSN;

				$html [$k]['css'] = "<style>tr.bold td{font-weight:bold} </style> <tr class=\"bold\" bgcolor='#e9e9e9'><td align=\"right\" colspan=".($municipio ? "5" : "6")." >Total</td><td align='right'>".number_format(array_sum($total_matatual[$k]),0,',','.')."</td><td align='right'>".number_format(array_sum($total_ano2010[$k]),0,',','.')."</td><td align='right'>".number_format(array_sum($total_ano2012[$k]),0,',','.')."</td><td colspan=2 ></td><!--<td>-</td>--><!-- <td align='right'>".number_format(array_sum($total_insrealizado[$k]), 2, ',', '.')."</td><td align='right'>".number_format(array_sum($total_insprevisto[$k]), 2, ',', '.')."</td> --></tr>";
				$html [$k]['tr2'] = "<tr class=\"bold\" bgcolor='#e9e9e9'><td valign=\"top\" colspan=".($municipio ? "1" : "2")." ><span style=\"color:#0000AA\">Campus:</span> ".count($html[$k]['tr'])."</td><td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Status:</span><br/> - Novo: ".count($arrN[$k])."<br/>&nbsp;&nbsp;&nbsp;&nbsp;- Funcionando: ".number_format( array_sum($arrSSF[$k]) ,0,',','.')."<br/>&nbsp;&nbsp;&nbsp;&nbsp;- Não Funcionando: ".number_format( array_sum($arrSSN[$k]) ,0,',','.')."<br/> - Preexistente: ".count($arrP[$k])."<br/>- Previsto: ".count($arrR[$k])."</td><td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Situação:</span><br/> - Funcionando: ".count($arrSF[$k])."<br/> - Não Funcionando: ".count($arrSN[$k])."</td><td valign=\"top\" colspan=4 ><span style=\"color:#0000AA\">Instalações:</span><br/> - Provisória: ".count($arrIP[$k])."<br/> - Definitiva: ".count($arrID[$k])."<br/> - Sem Instalação: ".count($arrIS[$k])."</td></tr>";
				
				
				$htmlCABECALHOTOTALGERAL = "<style>tr.bold td{font-weight:bold} </style> <tr class=\"bold\" bgcolor='#cccccc'><td rowspan=2 colspan=".($municipio ? "5" : "6")." ><td colspan=3 align=\"center\" >MATRÍCULAS</td><td rowspan=2 colspan=3 ></td><!-- <td rowspan=2 align='center'>Investimento atual</td><td rowspan=2 align='center'>Investimento previsto</td> --></tr>";
				$htmlCABECALHOTOTALGERAL .= "<style>tr.bold td{font-weight:bold} </style> <tr class=\"bold\" bgcolor='#cccccc'><td>2009</td><td>2010</td><td>Funcionamento Pleno</td></td></tr>";
				
				$htmlTOTALGERAL = "<style>tr.bold td{font-weight:bold} </style> <tr class=\"bold\" bgcolor='#cccccc'><td align=\"right\" colspan=".($municipio ? "5" : "6")." >Total Geral</td><td align='right'>".number_format($totalGeral_matatual,0,',','.')."</td><td align='right'>".number_format($totalGeral_ano2010,0,',','.')."</td><td align='right'>".number_format($totalGeral_ano2012,0,',','.')."</td><td colspan=2 ></td><!--<td>-</td>--><!--<td align='right'>".number_format($totalGeral_insrealizado, 2, ',', '.')."</td><td align='right'>".number_format($totalGeral_insprevisto, 2, ',', '.')."</td> --></tr>";
				$htmlTOTALGERAL .= "<tr class=\"bold\" bgcolor='#cccccc'><td valign=\"top\" colspan=".($municipio ? "1" : "2")." ><span style=\"color:#0000AA\">Campus:</span> ".count($dadoscsv)."</td><td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Status:</span><br/> - Novo: ".count($arrTotalN)."<br/>&nbsp;&nbsp;&nbsp;&nbsp;- Funcionando: ".number_format( array_sum($arrTotalSSF) ,0,',','.')."<br/>&nbsp;&nbsp;&nbsp;&nbsp;- Não Funcionando: ".number_format( array_sum($arrTotalSSN) ,0,',','.')."<br/> - Preexistente: ".count($arrTotalP)."<br/>- Previsto: ".count($arrTotalR)."</td><td valign=\"top\" colspan=2 ><span style=\"color:#0000AA\">Situação:</span><br/> - Funcionando: ".count($arrTotalSF)."<br/> - Não Funcionando: ".count($arrTotalSN)."</td><td valign=\"top\" colspan=4 ><span style=\"color:#0000AA\">Instalações:</span><br/> - Provisória: ".count($arrTotalIP)."<br/> - Definitiva: ".count($arrTotalID)."<br/> - Sem Instalação: ".count($arrTotalIS)."</td></tr>";
				$htmlTOTALGERAL .= "<tr class=\"bold\" style=\"color:#cccccc\" bgcolor='#cccccc' ><td colspan=14>.</td></tr>";
				
		}
		
	}else{
		$htmlNA = "<tr><td style=\"color:#990000;text-align:center;font-weight:bold\" colspan=\"10\" >$msgErro</td></tr>";
	}
	$htmlFIM = "</table>";
		
		echo $htmlHEADER;
		if(count($html) != 1 ){
			echo $htmlCABECALHOTOTALGERAL;
			echo $htmlTOTALGERAL;	
		}
	if(count($html) != 0 ){
		foreach($html as $k => $h){
			echo $html[$k]['header'];
			if(count($h["tr"]) == 0){
				echo $htmlNA;
			}else{
				foreach($h["tr"] as $i){
					echo $i;
				}
				if(count($h["tr"]) > 1){
					echo $html[$k]['css'];
					echo $html[$k]['tr2'];
				}
			}
			
		}
	}else{
		echo $htmlCampos;
		echo $htmlNA;
	}
	echo $htmlFIM;
}

function testaPermissaoTela($usucpf = null, $mnuid = null, $sisid = null){
	global $db;
	
	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	$mnuid = !$mnuid ? $_SESSION['mnuid'] : $mnuid; //6190
	$sisid = !$sisid ? $_SESSION['sisid'] : $sisid;
	
	$sql = "SELECT 
				distinct per.pflcod
			FROM 
				seguranca.perfilmenu men
			INNER JOIN
				seguranca.perfil per ON per.pflcod = men.pflcod
			where
				per.sisid =  $sisid
			and
				mnuid = $mnuid";
	$arrPerfilAcesso = $db->carregar($sql);
	$arrPerfilAcesso = !$arrPerfilAcesso ? array() : $arrPerfilAcesso;
	foreach($arrPerfilAcesso as $pflcod){
		$arrPerfil[] = $pflcod['pflcod'];
	}
	$arrPerfil = !$arrPerfil ? array() : $arrPerfil;
	
	$sql = "SELECT p.pflcod FROM seguranca.perfil p 
			LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod 
			WHERE pu.usucpf = '". $usucpf ."' and p.pflstatus = 'A' and p.sisid =  '". $sisid ."' 
			ORDER BY pflnivel ASC LIMIT 1";
	$PerfilUsuario = $db->pegaUm($sql);
	$PerfilUsuario = !$PerfilUsuario ? "Sem Acesso" : $PerfilUsuario;
	
	if(in_array($PerfilUsuario,$arrPerfil)){
		return true;
	}else{
		return false;
	}	
	
}

function monta_cabecalho_relatorio_painel( $largura  = 100){
	
	global $db;
	
	$cabecalho = '<table width="'.$largura.'%" border="0" cellpadding="0" cellspacing="0" style="border-bottom: 1px solid;">'
				.'	<tr bgcolor="#ffffff">' 	
				.'		<td valign="top" width="50" rowspan="2"></td>'
				.'		<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">'
			.'			'.$GLOBALS['parametros_sistema_tela']['sigla'].'<br/>'
			.'			'.$GLOBALS['parametros_sistema_tela']['unidade'].'<br/>'
//				.'			Acompanhamento da Execução Orçamentária<br/>'					
				.'			MEC / SE - Secretaria Executiva <br />'
				.'		</td>'
				.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'										
				.'			Data do Relatório:' . date( 'd/m/Y - H:i:s' ) . '<br />'					
				.'		</td>'					
				.'	</tr><tr bgcolor="#ffffff">'
				.'		<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">'
				.'			<b><font style="font-size:14px;">' . $_REQUEST["titulo"] . '</font></b>'
				.'		</td>'
				.'	</tr>'					
				.'</table>';					
								
		return $cabecalho;						
						
}

function getPeriodicidadeIndicador($indid = null){
	global $db;

	if(!$indid){
		echo "Favor informar um indicador para retornar as periodicidades!";
		dbg($indid);
		return array();
	}
	
	$sql = "select
				per.perid,
				per.perdsc
			from
				painel.periodicidade per
			where
				per.pernivel >= ( 
								select 
									min(per.pernivel)
								from
									painel.periodicidade per
								inner join
									painel.detalheperiodicidade dpe ON per.perid = dpe.perid
								inner join
									painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
								where
									seh.indid = $indid
								and
									per.perstatus = 'A'
								and
									seh.sehstatus <> 'I'
							  )
			and
				per.perstatus = 'A'";
	
	return $db->carregar($sql);
}


function getPeridIndicador($indid = null,$integer = false){
	global $db;

	if(!$indid){
		echo "Favor informar um indicador para retornar as periodicidades!";
		dbg($indid);
		return array();
	}
	
	$sql = "select
				per.perid,
			from
				painel.periodicidade per
			where
				per.pernivel >= ( 
								select 
									min(per.pernivel)
								from
									painel.periodicidade per
								inner join
									painel.detalheperiodicidade dpe ON per.perid = dpe.perid
								inner join
									painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
								where
									seh.indid = $indid
								and
									per.perstatus = 'A'
								and
									seh.sehstatus <> 'I'
							  )
			and
				per.perstatus = 'A'";
	
	$dados = $db->carregar($sql);
	
	if($dados[0]){
		foreach($dados as $d){
			if($integer)
				$arrPerid[] = (int)$d['perid'];
			else
				$arrPerid[] = $d['perid'];
		}
		return $arrPerid;
	}else{
		return array();
	}
	
}


function getAgrupadoresPorDetalhe($indid = null){
	global $db;
	
	if(!$indid)
		return array();
	
	$sql = "select
				tdiid as codigo,
				tdidsc as descricao
			from
				painel.detalhetipoindicador
			where
				indid = $indid
			and
				tdistatus = 'A'";
	
	$dados = $db->carregar($sql);
	
	//Período - Agrupador Padrão
	$arr[] = array( "codigo" => "dp.dpedsc" , "descricao" => "Período" );
	
	if(is_array($dados)){
		foreach($dados as $d){
			$arr[] = array( "codigo" => $d['codigo'] , "descricao" => $d['descricao'] );
		}
	}
		return $arr;	
}

function getAgrupadorPorRegionalizador($indid = null){
	global $db;
	
	if(!$indid)
		return false;
		
	$sql = "select
				regid
			from
				painel.indicador
			where
				indid = $indid
			and
				indstatus = 'A'";
	
	$regid = $db->pegaUm($sql);
		
	switch($regid){
		
		case REGIONALIZACAO_ESCOLA:
			$arr[] = array( "codigo" => "escdsc" , "descricao" => "Escola" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_IES:
			$arr[] = array( "codigo" => "iesdsc" , "descricao" => "IES" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_MUN:
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_UF:
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_REGIAO:
			$arr[] = array( "codigo" => "regdescricao" , "descricao" => "Região" );
		break;
		
		case REGIONALIZACAO_BRASIL:
			return false;
		break;
		
		case REGIONALIZACAO_POSGRADUACAO:
			$arr[] = array( "codigo" => "iepdsc" , "descricao" => "Pós-Graduação" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_CAMPUS_SUPERIOR:
			$arr[] = array( "codigo" => "instituicaosuperior" , "descricao" => "Instituição" );
			$arr[] = array( "codigo" => "entnome" , "descricao" => "Campus (Superior)" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_CAMPUS_PROFISSIONAL:
			$arr[] = array( "codigo" => "instituicaoprofissional" , "descricao" => "Instituição" );
			$arr[] = array( "codigo" => "entnome" , "descricao" => "Campus (Profissional)" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_UNIVERSIDADE:
			$arr[] = array( "codigo" => "unidsc" , "descricao" => "Universidade" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_INSTITUTO:
			$arr[] = array( "codigo" => "unidsc" , "descricao" => "Instituto" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_HOSPITAL:
			$arr[] = array( "codigo" => "entnome" , "descricao" => "Hospital" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_POLO:
			$arr[] = array( "codigo" => "poldsc" , "descricao" => "Pólo" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		case REGIONALIZACAO_IESCPC:
			$arr[] = array( "codigo" => "iecdsc" , "descricao" => "IES/Município" );
			$arr[] = array( "codigo" => "mundescricao" , "descricao" => "Município" );
			$arr[] = array( "codigo" => "dshuf" , "descricao" => "Estado" );
		break;
		
		default:
			return false;
		
	}
	return $arr;
	
}

function getParametrosRelatorioIndicador( $indid = null , $arrPost = null , $excel = false){
	global $db;
	
	if(!$indid || !$arrPost)
		return false;
		
	$sql = "select
				regid,
				perid,
				indqtdevalor,
				indcumulativo,
				indcumulativovalor,
				unmid
			from
				painel.indicador
			where
				indid = $indid";
	extract($db->pegaLinha($sql));
	
	//Filtro por regvalue
	$sql = "select 
				regdescricao, 
				rgaidentificador,
				rgafiltro,
				regsqlcombo
			from 
				painel.regagreg reg1
			inner join
				painel.regionalizacao reg2 ON reg1.regid = reg2.regid 
			where 
				reg1.regid = $regid 
			and 
				regsqlcombo is not null";
	$campoRegiona = $db->pegaLinha($sql);
	/* Fim - Filtro por regvalue*/
	
	$agroup = false;
	
	if(is_array($arrPost['agrupador'])){
		foreach($arrPost['agrupador'] as $agr){
			if(strstr(strtoupper($campoRegiona['regsqlcombo']), strtoupper("$agr as descricao") )){
				$agroup = true;
			}
		}
	}
	
	if(!$agroup && $campoRegiona && $unmid == UNIDADEMEDICAO_RAZAO && !$arrPost['regvalue']){
		die("<center><br />Indicador de razão! Por favor, selecione o(a) {$campoRegiona['regdescricao']} e tente novamente!</center>");
	}
	
	if($arrPost['periodo_inicio'])
		$arrFiltros[] = "d.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = {$arrPost['periodo_inicio']})";
	if($arrPost['periodo_fim'])
		$arrFiltros[] = "d.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = {$arrPost['periodo_fim']})";
	if($arrPost['regcod'] && $arrPost['regcod'] != "todos" && $arrPost['regcod'] != "")
		$arrFiltros[] = "d.dshuf in ( select estuf from territorios.estado where regcod = '{$arrPost['regcod']}' )";
	if($arrPost['estuf'] && $arrPost['estuf'] != "todos" && $arrPost['estuf'] != "")
		$arrFiltros[] = "d.dshuf = '{$arrPost['estuf']}'";
	if($arrPost['tpmid'] && $arrPost['tpmid'] != "todos" && $arrPost['tpmid'] != "")
		$arrFiltros[] = "d.dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid = {$arrPost['tpmid']} )";
	if($arrPost['gtmid'] && $arrPost['gtmid'] != "todos" && $arrPost['gtmid'] != "")
		$arrFiltros[] = "d.dshcodmunicipio in (select muncod from territorios.muntipomunicipio where tpmid in (select tpmid from territorios.tipomunicipio where gtmid = {$arrPost['gtmid']}) )";
	if($arrPost['muncod'] && $arrPost['muncod'] != "todos" && $arrPost['muncod'] != "")
		$arrFiltros[] = "d.dshcodmunicipio = '{$arrPost['muncod']}'";
	if($arrPost['regvalue'] && $arrPost['regvalue'] != "todos" && $arrPost['regvalue'] != ""){
		$sql = "select rgaidentificador,rgafiltro from painel.regagreg where regid = $regid";
		$campoReg = $db->pegaLinha($sql);
		$arrFiltros[] = str_replace(array("AND","and","{".$campoReg['rgaidentificador']."}"),array("","",$arrPost['regvalue']),$campoReg['rgafiltro']);
	}
	if($arrPost['tidid1'] && $arrPost['tidid1'] != "todos" && $arrPost['tidid1'] != ""){
		$sql = "select
					tdinumero 
				from 
					painel.detalhetipoindicador d1
				inner join
					painel.detalhetipodadosindicador d2 ON d1.tdiid = d2.tdiid
				where 
					d2.tidid = {$arrPost['tidid1']}";
		$tdinumero1 = $db->pegaUm($sql);
		$arrFiltros[] = "tidid$tdinumero1 = '{$arrPost['tidid1']}'";
	}if($arrPost['tidid2'] && $arrPost['tidid2'] != "todos" && $arrPost['tidid2'] != ""){
		$sql = "select 
					tdinumero 
				from 
					painel.detalhetipoindicador d1
				inner join
					painel.detalhetipodadosindicador d2 ON d1.tdiid = d2.tdiid
				where 
					d2.tidid = {$arrPost['tidid2']}";
		$tdinumero2 = $db->pegaUm($sql);
		$arrFiltros[] = "tidid$tdinumero2 = '{$arrPost['tidid2']}'";	
	}
	
	// Início - Monta os Agrupadores
	if(is_array($arrPost['agrupador'])){
		foreach($arrPost['agrupador'] as $agrupador){
			if(is_numeric($agrupador)){
				$sql = "select tdinumero, tdidsc from painel.detalhetipoindicador where tdiid = $agrupador";
				$dado = $db->pegaLinha($sql);
				$arrAgrup[] = array("campo" => "tiddsc".$dado['tdinumero'],
									"label" => $dado['tdidsc']
								    );
				//Campos para o SQL
				$arrCampos[] = "tiddsc".$dado['tdinumero'];
				//Group by
				$arrGroupBy[] = "tiddsc".$dado['tdinumero'];
				//Coluna p/ Excel
				$arrParametros['colunasExcel'][] = $dado['tdidsc'];
			}else{
				if($agrupador == "instituicaoprofissional" || $agrupador == "instituicaosuperior"){
					$arrAgrup[] =  array("campo" => $agrupador,
										 "label" => "Instituição"
										 );
					
					//Campos para o SQL
					$arrCampos[] = $agrupador;
					//Group by
					$arrGroupBy[] = $agrupador;
					//Coluna p/ Excel
					$arrParametros['colunasExcel'][] = "Instituição";
					
				}else{
					$arrAgrup[] =  array("campo" => $agrupador,
										 "label" => getAgrupadorPorCampo($agrupador,$regid)
										 );
					
					//Campos para o SQL
					$arrCampos[] = $agrupador;
					//Group by
					$arrGroupBy[] = $agrupador;
					//Coluna p/ Excel
					$arrParametros['colunasExcel'][] = getAgrupadorPorCampo($agrupador,$regid);
				}
			}
		}
	}
	// Fim - Monta os Agrupadores

	// Início - Monta as Colunas
	$arrCamposInterno = $arrCampos;
	$arrColunas[] = "qtde";

	if($unmid == UNIDADEMEDICAO_MOEDA) {
		$arrLabelColunas[] = array("label" => "R$" , "campo" => "qtde");
		$arrParametros['colunasExcel'][] = "R$";
		$arrCampos[] = "sum(qtde) as qtde";		
	}elseif($unmid == UNIDADEMEDICAO_PERCENTUAL) {
		$arrLabelColunas[] = array("label" => "Porcentagem" , "campo" => "qtde");
		$arrParametros['colunasExcel'][] = "Porcentagem";
		$arrCampos[] = "sum(qtde) || '%' as qtde";		
	} else {
		$arrLabelColunas[] = array("label" => "Quantidade" , "campo" => "qtde", "type" => "numeric");
		$arrParametros['colunasExcel'][] = "Quantidade";
		$arrCampos[] = "sum(qtde) as qtde";
	}

	if($indqtdevalor == "t"){
		$arrColunas[] = "valor";
		$arrCampos[] = "sum(valor) as valor";
		$arrLabelColunas[] = array("label" => "Valor" , "campo" => "valor");
		$arrParametros['colunasExcel'][] = "Valor";
	}
	// Fim - Monta as Colunas
	
	$arrGroupByInterno = $arrGroupBy;

	if(in_array("dp.dpedsc",$arrCampos)){
		foreach($arrCampos as $key => $campo){
			if($campo == "dp.dpedsc"){
				$arrAgrup[$key] = array("campo" => "dpedsc" , "label" => "Período");
				$arrParametros['colunasExcel'][$key] = "Período";
				$arrGroupBy[] = "dpedatainicio";
			} 
		}
	}
	
	$arrParametros['agrupadores'] = array(	
											"agrupador" 		=> $arrAgrup,
								 			"agrupadoColuna" 	=> $arrColunas
										  );
	
	$arrParametros['colunas'] = $arrLabelColunas;
										  
	//Início - Inner
	if(in_array("entnome",$arrCampos)){
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "entnome"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "Campus (Profissional)" || $arrExcel == "Campus (Superior)" || $arrExcel == "Hospital"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código $arrExcel";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "entnome , dp.entid ";
					$arrCamposInterno[$key] = "entnome , d.entid ";
					$arrGroupByInterno[] = "d.entid";
					$arrGroupBy[] = "dp.entid";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "entnome"){
					$arrCamposInterno[$key] = "(entnome || '<span style=\"display:none\" >' || ent.entid || '</span>') as entnome";
					$arrGroupByInterno[] = "ent.entid";
					break;
				}
			}
		}
		$arrInner[] = "entidade.entidade ent ON ent.entid = d.entid";
	}if(in_array("iepdsc",$arrCampos)){
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iepdsc"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "Pós-Graduação"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código $arrExcel";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "iepdsc , dp.iepid ";
					$arrCamposInterno[$key] = "iepdsc , d.iepid ";
					$arrGroupByInterno[] = "d.iepid";
					$arrGroupBy[] = "dp.iepid";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iepdsc"){
					$arrCamposInterno[$key] = "(iepdsc || '<span style=\"display:none\" >' || iep.iepid || '</span>') as iepdsc";
					$arrGroupByInterno[] = "iep.iepid";
					break;
				}
			}
		}
		$arrInner[] = "painel.iepg iep ON iep.iepid = d.iepid";
	}if(in_array("unidsc",$arrCampos)){
	if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "unidsc"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "Instituto" || $arrExcel == "Universidade"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código $arrExcel";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "unidsc , dp.unicod ";
					$arrCamposInterno[$key] = "unidsc , d.unicod ";
					$arrGroupByInterno[] = "d.unicod";
					$arrGroupBy[] = "dp.unicod";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "unidsc"){
					$arrCamposInterno[$key] = "(unidsc || '<span style=\"display:none\" >' || uni.unicod || '</span>') as unidsc";
					$arrGroupByInterno[] = "uni.unicod";
					break;
				}
			}
		}
		$arrInner[] = "public.unidade uni ON uni.unicod = d.unicod";
	}if(in_array("mundescricao",$arrCampos)){
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "mundescricao"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "Município"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código IBGE";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "mundescricao , dp.dshcodmunicipio ";
					$arrCamposInterno[$key] = "mundescricao , d.dshcodmunicipio ";
					$arrGroupByInterno[] = "d.dshcodmunicipio";
					$arrGroupBy[] = "dp.dshcodmunicipio";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "mundescricao"){
					$arrCamposInterno[$key] = "(mundescricao || '<span style=\"display:none\" >' || mun.muncod || '</span>') as mundescricao";
					$arrGroupByInterno[] = "mun.muncod";
					break;
				}
			}
		}
		$arrInner[] = "territorios.municipio mun ON mun.muncod = d.dshcodmunicipio";
	}if(in_array("regdescricao",$arrCampos)){
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "regdescricao"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "Região"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "regdescricao , dp.dshregiao ";
					$arrCamposInterno[$key] = "regdescricao , d.dshregiao ";
					$arrGroupByInterno[] = "d.dshregiao";
					$arrGroupBy[] = "dp.dshregiao";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "regdescricao"){
					$arrCamposInterno[$key] = "(regdescricao || '<span style=\"display:none\" >' || reg.regcod || '</span>') as regdescricao";
					$arrGroupByInterno[] = "reg.regcod";
					break;
				}
			}
		}
		$arrInner[] = "territorios.regiao reg ON reg.regcod = d.dshregiao";
	}
	if(in_array("poldsc",$arrCampos)){
		$arrInner[] = "painel.polo pol ON pol.polid = d.polid";
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "poldsc"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "Pólo"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código do Polo";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "poldsc , dp.polid ";
					$arrCamposInterno[$key] = "poldsc , d.polid ";
					$arrGroupByInterno[] = "d.polid";
					$arrGroupBy[] = "dp.polid";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "poldsc"){
					$arrCamposInterno[$key] = "(poldsc || '<span style=\"display:none\" >' || pol.polid || '</span>') as poldsc";
					$arrGroupByInterno[] = "pol.polid";
					break;
				}
			}
		}
	}
	if(in_array("instituicaosuperior",$arrCampos)){
		foreach($arrCamposInterno as $key => $campoInterno){
			if($campoInterno == "instituicaosuperior"){
				$arrCamposInterno[$key] = "( 
												select 
													ent2.entnome
												from
													entidade.entidade ent
												inner join
													entidade.funcaoentidade fun ON fun.entid = ent.entid and fun.funid = 18
												inner join
													entidade.funentassoc fea ON fea.fueid = fun.fueid
												inner join
													entidade.entidade ent2 ON ent2.entid = fea.entid
												inner join
													entidade.funcaoentidade fun2 ON fun2.entid = ent2.entid and fun2.funid = 12
												where
													ent.entid = d.entid
											) as instituicaosuperior";
				$arrGroupByInterno[] = "instituicaosuperior";
				break;
			}
		}
	}
	if(in_array("instituicaoprofissional",$arrCampos)){
		foreach($arrCamposInterno as $key => $campoInterno){
			if($campoInterno == "instituicaoprofissional"){
				$arrCamposInterno[$key] = "( 
												select 
													ent2.entnome
												from
													entidade.entidade ent
												inner join
													entidade.funcaoentidade fun ON fun.entid = ent.entid and fun.funid = 17
												inner join
													entidade.funentassoc fea ON fea.fueid = fun.fueid
												inner join
													entidade.entidade ent2 ON ent2.entid = fea.entid
												inner join
													entidade.funcaoentidade fun2 ON fun2.entid = ent2.entid and fun2.funid = 11
												where
													ent.entid = d.entid
											) as instituicaoprofissional";
				$arrGroupByInterno[] = "instituicaoprofissional";
				break;
			}
		}
	}
	if(in_array("iesdsc",$arrCampos)){
		$arrInner[] = "painel.ies ies ON ies.iesid::integer = d.dshcod::integer";
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iesdsc"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "IES"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código $arrExcel";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "iesdsc , dp.dshcod ";
					$arrCamposInterno[$key] = "ies.iesdsc , d.dshcod ";
					$arrGroupByInterno[] = "d.dshcod";
					$arrGroupBy[] = "dp.dshcod";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iesdsc"){
					$arrCamposInterno[$key] = "'<span style=\"display:none\" >' || ies.iesid || '</span>' || ies.iesdsc as iesdsc";
					$arrGroupByInterno[] = "ies.iesid";
					break;
				}
			}
		}
	}if(in_array("escdsc",$arrCampos)){
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "escdsc"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "Escola"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código INEP";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "escdsc , esccodinep ";
					$arrCamposInterno[$key] = "esc.escdsc , esc.esccodinep ";
					$arrGroupByInterno[] = "esccodinep";
					$arrGroupBy[] = "esccodinep";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "escdsc"){
					$arrCamposInterno[$key] = "'<span style=\"display:none\" >' || esc.esccodinep || '</span>' || esc.escdsc as escdsc";
					$arrGroupByInterno[] = "esc.esccodinep";
					break;
				}
			}
		}
		$arrInner[] = "painel.escola esc ON esc.esccodinep::text = d.dshcod::text";
	}if(in_array("iecdsc",$arrCampos)){
		if($excel){
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iecdsc"){
					unset($arrColunasExcel);
					foreach($arrParametros['colunasExcel'] as $chave => $arrExcel):
						if($arrExcel == "IES/Município"){
							$arrColunasExcel[] = $arrExcel;
							$arrColunasExcel[] = "Código $arrExcel";
						}else{
							$arrColunasExcel[] = $arrExcel;
						}
					endforeach;
					$arrParametros['colunasExcel'] = $arrColunasExcel;
					$arrCampos[$key] = "iecdsc , dp.iecid ";
					$arrCamposInterno[$key] = "ies.iecdsc , d.iecid ";
					$arrGroupByInterno[] = "d.iecid";
					$arrGroupBy[] = "dp.iecid";
					break;
				}
			}
		}else{
			foreach($arrCamposInterno as $key => $campoInterno){
				if($campoInterno == "iecdsc"){
					$arrCamposInterno[$key] = "'<span style=\"display:none\" >' || ies.iecid || '</span>' || ies.iecdsc as iecdsc";
					$arrGroupByInterno[] = "ies.iecid";
					break;
				}
			}
		}
		$arrInner[] = "painel.iescpc ies ON ies.iecid = d.iecid";
	//Fim - Inner
	}
	//Se o indicador for cumulativo e a coluna de Período não estivee presente, exibi-se apenas o último período de cada ítem
	
	if($indcumulativo == "N" && !in_array("dp.dpedsc",$arrCampos)){
		$whereDataInterna = "";
		if($arrPost['periodo_inicio']){
			$whereDataInterna .= " d1.dpedatainicio >= ( select dpedatainicio from painel.detalheperiodicidade where dpeid = {$arrPost['periodo_inicio']}) and ";
		}
		if($arrPost['periodo_fim']){
			$whereDataInterna .= " d1.dpedatainicio <= ( select dpedatafim from painel.detalheperiodicidade where dpeid = {$arrPost['periodo_fim']}) and ";
		}
	}else{
		$whereDataInterna = " d1.dpedatainicio>=dp.dpedatainicio 
							and 
								d1.dpedatafim<=dp.dpedatafim 
							and ";
	}
										  
	/* Início SQL */
	
	$arrParametros['sql'] = "	select 
									".(count($arrCampos) ? implode(" , ",$arrCampos) : "" )."
								from (
										select 
											dp.dpedatainicio,
											".(count($arrCamposInterno) ? implode(" , ",$arrCamposInterno)."," : "" )."
											case when d.indcumulativo = 'N' then
								        			case when (
										                        select 
										                        	count(d1.dpeid) 
										                        from 
										                        	painel.detalheperiodicidade d1
																inner join 
																	painel.seriehistorica sh on sh.dpeid=d1.dpeid
																where 
																	$whereDataInterna 
																	sh.indid=d.indid
																limit 
																	1
								                				) > 0 then sum(d.qtde) 
								                	else 0 end
												else sum(d.qtde)
											end as qtde,
											case when d.indcumulativovalor = 'N' then
								        			case when (
								                        		select 
								                        			count(d1.dpeid)
								                        		from 
								                        			painel.detalheperiodicidade d1
								                                inner join 
								                                	painel.seriehistorica sh on sh.dpeid=d1.dpeid
								                                where 
								                                	$whereDataInterna 
								                                	sh.indid=d.indid
								                                limit 
								                                	1
								                				) > 0 then sum(d.valor) 
								                			else 0 end
													else sum(d.valor)
											end as valor
										from 
											painel.v_detalheindicadorsh d
										left join 
											painel.detalheperiodicidade dp on d.dpedatainicio>=dp.dpedatainicio and d.dpedatafim<=dp.dpedatafim
										".(count($arrInner) ? " left join ".implode(" left join ",$arrInner) : "" )." 
										-- periodo que vc quer exibir
										where 
											dp.perid = ".(!$arrPost['perid'] ? $perid : $arrPost['perid'])."
										-- indicador que vc quer exibir
										and 
											d.indid = $indid
										".(count($arrFiltros) ? " and ".implode(" and ",$arrFiltros) : "" )."
										--range de data compreendida no periodo
										".str_replace("dsh.","d.",$andDpe)."
										group by 
											d.indcumulativo,
											dp.dpedatainicio,
											dp.dpedatafim,
											d.indid,
											d.dpeid,
											d.indcumulativovalor 
											".(count($arrGroupByInterno) ? ",".implode(",",$arrGroupByInterno) : "")."
									) dp
									".(count($arrGroupBy) ? " group by ".implode(",",$arrGroupBy) : "")."
									".(count($arrGroupBy) ? " order by ".implode(",",$arrGroupBy) : "")."";	
	
	/* Fim SQL */
	//dbg($arrParametros['sql']);
	return $arrParametros;
	
}

function getAgrupadorPorCampo($campo,$regid){
	switch($campo){	
		case "dp.dpedsc":
			return "Período";
		break;
		case "escdsc":
			return "Escola";
		break;
		case "iesdsc":
			return "IES";
		break;
		case "mundescricao":
			return "Município";
		break;
		case "dshuf":
			return "Estado";
		break;
		case "regdescricao":
			return "Região";
		break;
		case "iepdsc":
			return "Pós-Graduação";
		break;
		case "entnome":
			switch($regid){
				case REGIONALIZACAO_CAMPUS_SUPERIOR:
					return "Campus (Superior)";
				break;
				case REGIONALIZACAO_CAMPUS_PROFISSIONAL:
					return "Campus (Profissional)";
				break;
				case REGIONALIZACAO_HOSPITAL:
					return "Hospital";
				break;
			}
		break;
		case "unidsc":
			switch($regid){
				case REGIONALIZACAO_UNIVERSIDADE:
					return "Universidade";
				break;
				case REGIONALIZACAO_INSTITUTO:
					return "Instituto";
				break;
			}
		break;
		case "poldsc":
			return "Pólo";
		break;
		case "iecdsc":
			return "IES/Município";
		break;
		default:
			return false;
	}
}

function getResponsavelSecretariaIndicador($indid = null)
{
	global $db;
	
	if(!indid)
		return false;

	$sql = "select
					respnome,
					respemail,
					'(' || respdddtelefone || ')' || resptelefone as telefone,
					'(' || respdddcelular || ')' || respcelular as celular
				from 
					painel.responsavelsecretaria 
				where 
					secid = ( select secid from painel.indicador where indid = $indid ) 
				and 
					respstatus = 'A'
				order by
					respnome";
	return $db->carregar($sql);
}
?>