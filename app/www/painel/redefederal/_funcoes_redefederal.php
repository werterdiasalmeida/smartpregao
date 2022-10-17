<?php


/********************************* Funçães do Painel da Rede Federal *********************************/

// anos analisados por tipo de ensino
$anosanalisados[1] = array(2006,2007,2008,2009,2010,2011,2012);
$anosanalisados[2] = array(2006,2007,2008,2009,2010,2011,2012);
// anos analisados por default
$anosanalisados['default'] = array(2008,2009,2010,2011,2012);

//Títulos dos itens
$tituloitens[1] = "REUNI (Pactuação)";// tipo ensino: superior
$tituloitens[2] = "Expansão(Previsão)";// tipo ensino: profissional
$tituloitens['default'] = "Previsão";// tipo ensino: default

// lista de funcoes (cargos) por tipo de ensino "1"-> Superior, "2"->"Profissional"
$_funcoes = array(1 => array('campus' => 24, 'unidade' => 21),
				  2 => array('campus' => 24, 'unidade' => 21)
				  );
				  
// lista do tipo de entidade universidades, ou centros profissionalizantes
$_tipoentidade = array(1 => 12,
					   2 => array(11,14));



/**
 * Exibe lista das unidades de acordo com o estado, caso exista, e com o tipo de ensino
 * 
 * @author Fernando A. Bagno da Silva
 * @since 13/07/2009
 * @param string $estuf
 * @param integer $orgid
 * 
 */
function painel_lista_unidades_painel( $estuf, $orgid ){
	
	global $db;
	
	// pega a descricao do estado
	$dados_uf = $db->pegaUm("SELECT estdescricao FROM territorios.estado WHERE estuf = '{$estuf}'");
	
	switch( $_REQUEST['estuf'] ){
		case 'norte':
			$dados_uf = 'Norte';
		break;
		case 'nordeste':
			$dados_uf = 'Nordeste';
		break;
		case 'sudeste':
			$dados_uf = 'Sudeste';
		break;
		case 'sul':
			$dados_uf = 'Sul';
		break;
		case 'centrooeste':
			$dados_uf = 'Centro-Oeste';
		break;
	}
	
	// imprime título do estado e abre a div da lista de unidades
	print '<fieldset style="background: #ffffff; height: 90%;"><legend>'. ( $dados_uf ? $dados_uf : 'Brasil' ) .'</legend>'
	    . '<div id="tabelalistaunidades" style="overflow: auto; width:97%; height: 200px; border: 1px solid #cccccc; "/>';

	// verifica a função da entidade de acordo com o tipo de ensino 
	switch ( $orgid ){
		case '1':
			$funid = " in ('" . 12 . "')";
		break;
		case '2':
			$funid = " in ('" . 14 . "')";
		break;
	}
	
	// cria filtro e join caso a pesquisa seja por regiões
	switch( $estuf ){
		case 'norte':
			$join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
			$where_estuf = "te.regcod = '1' AND";
		break;
		case 'nordeste':
			$join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
			$where_estuf = "te.regcod = '2' AND";
		break;
		case 'sudeste':
			$join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
			$where_estuf = "te.regcod = '3' AND";
		break;
		case 'sul':
			$join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
			$where_estuf = "te.regcod = '4' AND";
		break;
		case 'centrooeste':
			$join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
			$where_estuf = "te.regcod = '5' AND";
		break;
		default:
			$where_estuf = $estuf == 'todos' ? '' : "ed.estuf = '{$estuf}' AND";
		break;
	}
	
	// cria a query com as unidades de acordo com os filtros
	$sql = "SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || e.entid || '\" name=\"+\" onclick=\"desabilitarConteudo( ' || e.entid || ' ); formatarParametros();abreconteudo(\'painel.php?modulo=principal/redefederal/rede_federal&acao=A&subAcao=gravarCarga&orgid={$orgid}&carga=' || e.entid || '&params=\' + params, ' || e.entid || ');\"/></center>' as img,
				'<a onclick=\"atualiza_div( \'unidade\', \'' || e.entid || '\');\" style=\"cursor:pointer;\">' || CASE WHEN e.entsig IS NULL THEN  e.entnome ELSE  e.entsig || ' - ' || e.entnome END || '</a>' as nome,
				ed.estuf as uf,
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || e.entid || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM
				entidade.entidade e
			INNER JOIN 
				( select min(endid) as endid, entid from entidade.endereco group by entid ) ed2 ON e.entid = ed2.entid
			INNER JOIN 
				entidade.endereco ed ON ed.endid = ed2.endid
				
--			INNER JOIN 
--				entidade.endereco ed ON e.entid = ed.entid
			INNER JOIN
				entidade.funcaoentidade ef ON ef.entid = e.entid
				{$join_regiao}
			WHERE
				{$where_estuf}
				funid {$funid}
			ORDER BY
				uf, nome";
	// exibe a lista de unidades na tela
	$cabecalho = array("Ação", "Nome da Instituição", "UF");
	$db->monta_lista_simples( $sql, $cabecalho, 1000, 30, 'N', '100%');
	
	// fecha a div da lista de unidades
	print '</div>';
	
}

/**
 * Exibe lista dos campi das unidades de acordo com o estado, caso exista, e com o tipo de ensino
 * 
 * @author Fernando A. Bagno da Silva
 * @since 13/07/2009
 * @param integer $entid
 * @param integer $orgid
 * 
 */
function painel_lista_campus_painel( $entid, $orgid ){
	
	global $db;
	
	// verifica a função da entidade de acordo com o tipo de ensino 
	switch ( $orgid ){
		case '1':
			$funid = 18;
		break;
		case '2':
			$funid = 17;
		break;
	}
	
	// cria a query com os campi de acordo com os filtros
	$sql = "SELECT
				'<a onclick=\"atualiza_div( \'campus\', \'' || e2.entid || '\');\" style=\"cursor:pointer;\">' || e2.entnome || '</a>',
				tm.mundescricao,
				'<center>' || CASE WHEN cmpexistencia is not null THEN 
								CASE WHEN cmpexistencia = 'N' THEN 'Não (Novo)' ELSE 'Sim' END
							  ELSE '-' END || '</center>' as preexistente,
				'<center>' || CASE WHEN cmpsituacao is not null THEN 
									CASE WHEN cmpsituacao = 'F' THEN 'Sim' ELSE 'Não' END
							  ELSE '-' END || '</center>' as funcionamento
			FROM
				entidade.entidade e2
			INNER JOIN
				entidade.entidade e ON e2.entid = e.entid
			INNER JOIN
				entidade.endereco ed ON ed.entid = e.entid
			INNER JOIN
				territorios.municipio tm ON tm.muncod = ed.muncod
			INNER JOIN
				entidade.funcaoentidade ef ON ef.entid = e.entid
			INNER JOIN
				entidade.funentassoc ea ON ea.fueid = ef.fueid
			LEFT JOIN
				academico.campus ac ON e2.entid = ac.entid 
			WHERE
				ea.entid = {$entid} AND
				e.entstatus = 'A' AND ef.funid = {$funid}
			ORDER BY
				e.entnome, tm.mundescricao";
	
	
	
	// exibe a lista de campi na tela
	$cabecalho = array("Nome do Campus", "Município", "Pré Existente?", "Em Funcionamento?");
	$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
	
}

/**
 * Exibe a lista da situação atual, por, pais, região, ou estado atravez do tipo de ensino.
 * 
 * @author Felipe Chiavicatti
 * @since 29/07/2009
 * @param string $estuf
 * @param integer $orgid
 * 
 */
function painel_situacao_atual($orgid, $filtro=''){
	
	global $db;

	if ($filtro['estuf'] && $filtro['estuf'] != 'todos'){
		$join  = "INNER JOIN 
					entidade.endereco ee ON ee.entid = e.entid
			      INNER JOIN 
			   		territorios.estado est ON est.estuf = ee.estuf"; 
		$where = "WHERE 
					ee.estuf = '{$filtro['estuf']}'";
	}elseif($filtro['regcod']){
		$join  = "INNER JOIN 
					entidade.endereco ee ON ee.entid = e.entid
			      INNER JOIN 
			   		territorios.estado est ON est.estuf = ee.estuf
		   	      INNER JOIN 
			  		territorios.regiao r ON r.regcod = est.regcod";		
		$where = "WHERE 
					r.regcod = '{$filtro['regcod']}'";
	}
	
	$sql = "SELECT 
				''||itm.itmdsc||'',
				b.valor
			FROM 
				academico.item itm 
			LEFT JOIN (SELECT 
							cpi.itmid,
					   		sum( cpivalor) as valor
					   FROM 
							academico.campusitem cpi 
					   INNER JOIN
							academico.campus c ON c.cmpid = cpi.cmpid	
					   INNER JOIN
					   		entidade.entidade e ON e.entid = c.entid
					   $join
					   $where
					   GROUP BY 
							cpi.itmid
						) b ON b.itmid = itm.itmid
			LEFT JOIN 
				academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
			LEFT JOIN 
				academico.orgaoitem tei ON tei.itmid = itm.itmid 
			WHERE 
				tei.orgid = '$orgid' 
				AND itm.itmglobal = true
			ORDER BY 
				tei.teiordem";
		
	
		
	$cabecalho = array( "Itens", "Atual" );
	$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
	
}

/**
 * Exibe lista das situaçães das obras cadastradas nas unidades de acordo com o estado, caso exista, e com o tipo de ensino
 * 
 * @author Fernando A. Bagno da Silva
 * @since 13/07/2009
 * @param string $estuf
 * @param integer $orgid
 * @param integer $entid
 * 
 */
function painel_situacao_obras( $orgid, $estuf = '', $entid = ''){
	
	global $db;
	
	if ($entid){
		$join_entid  = " INNER JOIN 
							 entidade.entidade e ON e.entid = entidunidade OR e.entid = entidcampus";
		$where_entid = 	" AND e.entid = {$entid} ";	
	}
	// cria o join e o filtro de estado caso exista
	$join_estuf  = ( $estuf && $estuf != 'todos' ) ? "INNER JOIN entidade.endereco ed ON ed.endid = oi.endid" : "";
	$where_estuf = ( $estuf && $estuf != 'todos' ) ? "AND ed.estuf = '{$estuf}'" : "";
	
	// cria o join e o filtro por região, caso exista
	switch( $estuf ){
		case 'norte':
			$join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
			$where_estuf = "AND te.regcod = '1'";
		break;
		case 'nordeste':
			$join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
			$where_estuf = "AND te.regcod = '2'";
		break;
		case 'sudeste':
			$join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
			$where_estuf = "AND te.regcod = '3'";
		break;
		case 'sul':
			$join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
			$where_estuf = "AND te.regcod = '4'";
		break;
		case 'centrooeste':
			$join_regiao = "INNER JOIN territorios.estado te ON te.estuf = ed.estuf";
			$where_estuf = "AND te.regcod = '5'";
		break;
		default:
		break;
	}
	
	// cria a query com as situaçães de obras e a quantidade
	$sql = "SELECT 
				case when oi.stoid is not null 
				then '<a style=\"cursor:pointer;\" onclick=\"ver_obras_situacao(' || oi.stoid || ', \'{$estuf}\', \'{$entid}\');\">' || so.stodesc || '</a>' 
				else '<a style=\"cursor:pointer;\" onclick=\"ver_obras_situacao(\'\', \'{$estuf}\', \'{$entid}\');\"> Não informado </a>' end as descricao,
				count(oi.obrid) as total
			FROM
				obras.obrainfraestrutura oi
			LEFT JOIN
				obras.situacaoobra so ON so.stoid = oi.stoid
			{$join_estuf}
			{$join_regiao}
			{$join_entid}
			WHERE
				oi.obsstatus = 'A' AND
				oi.orgid = {$orgid}
				{$where_estuf}
				{$where_entid}
			GROUP BY 
				oi.stoid, stodesc
			ORDER BY
				descricao";

	// exibe na tela a lista de situaçães de obras
	$cabecalho = array("Situação", "Qtd de Obras");
	$db->monta_lista_simples( $sql, $cabecalho, 1000, 30, 'S', '100%');
	
}

/**
 * 
 * @author Fernando A. Bagno da Silva
 * @since 13/07/2009
 * @param string $estuf
 * @param integer $orgid
 * 
 */
function painel_obras_estado( $orgid, $estuf = '' ){
	
	global $db;
	
	if( $estuf && $estuf != 'todos' ){
	
		$titulo = 'Município';
		
		$sql = "SELECT
					CASE WHEN tm.mundescricao <> '' 
						 THEN '' || tm.mundescricao || '' 
						 ELSE 'Não Informado' END as municipio,
					count(oi.obrid) as total				
				FROM
					obras.obrainfraestrutura oi
				INNER JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				INNER JOIN
					territorios.municipio tm ON tm.muncod = ed.muncod
				WHERE
					oi.obsstatus = 'A' AND
					oi.orgid = {$orgid} AND
					ed.estuf = '{$estuf}'
				GROUP BY 
					tm.mundescricao, ed.estuf
				ORDER BY
					municipio";
	} else{
		
		$titulo = 'Estado';
		
		$sql = "SELECT
					CASE WHEN ed.estuf <> '' 
						 THEN '<a onclick=\"atualiza_div( \'obras\', \'' || ed.estuf || '\');\" style=\"cursor:pointer;\">' || ed.estuf || '</a>' 
						 ELSE 'Não Informado' END as estado,
					count(oi.obrid) as total				
				FROM
					obras.obrainfraestrutura oi
				INNER JOIN
					entidade.endereco ed ON ed.endid = oi.endid
				WHERE
					oi.obsstatus = 'A' AND
					oi.orgid = {$orgid}
				GROUP BY 
					estuf
				ORDER BY
					estuf";
		
	}
	
	$cabecalho = array( $titulo, "Qtd de Obras" );
	$db->monta_lista_simples( $sql, $cabecalho, 1000, 30, 'S', '100%');
	
}

/**
 * Exibe lista das obras cadastradas nas unidade informada e com o tipo de ensino
 * 
 * @author Fernando A. Bagno da Silva
 * @since 14/07/2009
 * @param integer $entid
 * @param integer $orgid
 * 
 */
function painel_lista_obras( $entid, $orgid ){
	
	global $db;
	
	// verifica se é uma unidade ou um campus
	$unidade = $db->pegaUm("SELECT obrid FROM obras.obrainfraestrutura where entidunidade = {$entid}");
	
	if ( !empty( $unidade ) ){

		$sql = "SELECT 
					oi.stoid,
					oi.obrid, 
					oi.obrdesc as nome,
					oi.obrdtinicio, 
					oi.obrdttermino, 
					tm.mundescricao||'/'||ed.estuf as local,
					case when oi.stoid is not null then so.stodesc else 'Não Informado' end as situacao,
					(SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual, 
					oi.obrcomposicao 
				FROM
					obras.obrainfraestrutura oi
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = oi.stoid   
				LEFT JOIN 
					entidade.endereco ed ON ed.endid = oi.endid 
				LEFT JOIN 
					territorios.municipio tm ON tm.muncod = ed.muncod
				WHERE 
					oi.entidunidade = {$entid} AND oi.orgid = {$orgid} AND oi.obsstatus = 'A'
				ORDER BY
					oi.obrdesc, so.stodesc";
		
		$obras = $db->carregar($sql);
	
		// monta quadro do painel em modelo de unidade
		painel_cabecalho_quadro_painel( $entid );
		
	}else{
		
		$sql = "SELECT 
					oi.stoid,
					oi.obrid, 
					oi.obrdesc as nome,
					oi.obrdtinicio, 
					oi.obrdttermino, 
					tm.mundescricao||'/'||ed.estuf as local,
					trim(ed.medlatitude) as latitude,
					trim(ed.medlongitude) as longitude,
					case when oi.stoid is not null then so.stodesc else 'Não Informado' end as situacao,
					(SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual, 
					oi.obrcomposicao 
				FROM
					obras.obrainfraestrutura oi
				LEFT JOIN
					obras.situacaoobra so ON so.stoid = oi.stoid  
				LEFT JOIN 
					entidade.endereco ed ON ed.endid = oi.endid 
				LEFT JOIN 
					territorios.municipio tm ON tm.muncod = ed.muncod
				WHERE 
					oi.entidcampus = {$entid}  AND oi.orgid = {$orgid} AND oi.obsstatus = 'A'
				ORDER BY
					oi.obrdesc, so.stodesc";
		
		$obras = $db->carregar($sql);

		// monta quadro do painel em modelo de campus
		painel_cabecalho_quadro_painel( $entid, 'campus', $orgid );
		
	}
?>	
	<table width="98%" cellSpacing="1" cellPadding="3" align="center" style="border:1px solid #ccc; background-color:#fff;">
		<tr>
	 		<td>
				<div id="quadrosituacao1" style="width:100%; border:1px solid #cccccc;"/>	
					<table cellspacing="1" cellpadding="3" width="100%">
						<tr>
							<td style="text-align: center; background-color: #dedede; font-weight: bold;"> Resumo de Obras </td>
						</tr>
						<tr>
							<td style="padding: 0px; margin: 0px;">
								<? painel_situacao_obras( $_SESSION['redefederal']['orgid'], '', $entid ) ?>
							</td>
						</tr>
					</table>
				</div>
			</td>
		</tr>
	</table>
	<div style="width: 97%; margin-top: 5px; margin-bottom: 1px; padding:3px; text-align: center; background-color: #dedede; font-weight: bold;"> Lista de Obras </div>
	
<?
	if( $obras[0] ) {
		$zoom = "<input type='hidden' id='endzoom'  value='15'/>"; 		
		foreach( $obras as $obr ) {
	
			switch ( $obr["stoid"] ){
				
				case "1":
					$obr['situacao'] = '<label style="color:#00AA00">' . $obr['situacao'] . '</label>';
				break;
				case "2":
					$obr['situacao'] = '<label style="color:#DD0000">' . $obr['situacao'] . '</label>';
				break;
				case "3":
					$obr['situacao'] = '<label style="color:blue">' . $obr['situacao'] . '</label>';
				break;
				case "6":
					$obr['situacao'] = '<label style="color:#DD0000">' . $obr['situacao'] . '</label>';
				break;
				
			}
			
			// latitude
			$dadoslatitude = explode(".", $obr["latitude"]);
			$graulatitude  = $dadoslatitude[0];
			$minlatitude   = $dadoslatitude[1];
			$seglatitude   = $dadoslatitude[2];
			$pololatitude  = $dadoslatitude[3];
			
			$latitude = !empty($graulatitude) ? $graulatitude . 'º ' . $minlatitude . '\' ' . $seglatitude .'" ' . $pololatitude : 'Não Informado';
						
			$campograulatitude = "<input type='hidden' id='{$obr["obrid"]}graulatitude' value='{$graulatitude}'/>";
			$campominlatitude  = "<input type='hidden' id='{$obr["obrid"]}minlatitude'  value='{$minlatitude}'/>";
			$camposeglatitude  = "<input type='hidden' id='{$obr["obrid"]}seglatitude'  value='{$seglatitude}'/>";
			$campopololatitude = "<input type='hidden' id='{$obr["obrid"]}pololatitude' value='{$pololatitude}'/>";
			
			$camposhiddenlat = $campograulatitude . $campominlatitude . $campopololatitude . $camposeglatitude;
			
			// longitude
			$dadoslongitude = explode(".", $obr["longitude"]);
			$graulongitude  = $dadoslongitude[0];
			$minlongitude   = $dadoslongitude[1];
			$seglongitude   = $dadoslongitude[2];
			
			$longitude = !empty($graulongitude) ? $graulongitude . 'º ' . $minlongitude . '\' ' . $seglongitude .'" W' : 'Não Informado';
			
			$campograulongitude = "<input type='hidden' id='{$obr["obrid"]}graulongitude' value='{$graulongitude}'/>";
			$campominlongitude  = "<input type='hidden' id='{$obr["obrid"]}minlongitude'  value='{$minlongitude}'/>";
			$camposeglongitude  = "<input type='hidden' id='{$obr["obrid"]}seglongitude'  value='{$seglongitude}'/>";
			
			$camposhiddenlog = $campograulongitude . $campominlongitude . $camposeglongitude;
			
			// visualizar mapa 
			$mapa = !empty($graulatitude) && !empty($graulongitude) ? '<tr><td class="SubTituloDireita"></td><td><a style="cursor:pointer;" onclick="abreMapa(' . $obr["obrid"] . ');">Visualizar / Buscar No Mapa</a></td></tr>': ''; 
			
			$obrid = "<input type='hidden' id='obrid'  value='{$obr["obrid"]}'/>";
			
			print '<form action="" method="post" id="formulario">'
				. '<table width="98%" cellSpacing="1" cellPadding="3" align="center" style="border:1px solid #ccc; background-color:#fff;">'
				. '		<tr>'
			 	. '			<td class="SubTituloDireita" style="width:20%;">Nome da obra:</td><td colspan="2" style="width:45%;"><b>' . $obr['nome'] . $obrid . '</b></td>'
				. '			<td class="SubTituloDireita">Município/UF:</td><td>' . $obr['local'] . '</td>'
				. '		</tr>'
				. '		<tr>'
				. '			<td class="SubTituloDireita">Início programado:</td><td colspan="2">' . formata_data($obr['obrdtinicio']) . '</td>'
				. '			<td class="SubTituloDireita">Término programado:</td><td>' . formata_data($obr['obrdttermino']) . '</td>'
				. '		</tr>'
				. '		<tr>'
				. '			<td class="SubTituloDireita">Situação da Obra:</td><td colspan="2">' . $obr['situacao'] . '</td>'
				. '			<td class="SubTituloDireita">% Executado:</td><td colspan="2">' . $obr['percentual'] . '</td>'		
				. '		</tr>'
				. '		<tr>'
				. '			<td class="SubTituloDireita">Latitude:</td><td colspan="2">' . $latitude . $camposhiddenlat . '</td>'
				. '			<td class="SubTituloDireita">Longitude:</td><td colspan="2">' . $longitude . $camposhiddenlog . $zoom . '</td>'		
				. '		</tr>'
				. $mapa
				. '		<tr>'
				. '			<td class="SubTituloDireita">Descrição:</td><td colspan="4" align="justify">'. ( ($obr['obrcomposicao']) ? nl2br($obr['obrcomposicao']) : "Nenhuma observação inserida" ) . '</td>'
				. '		</tr>'
				. '		<tr>'
				. '			<td class="SubTituloCentro" colspan="5">Fotos</td>'
				. '		</tr>';

				$sql = "SELECT 
							arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao, 
							to_char(oar.aqodtinclusao,'dd/mm/yyyy') as aqodtinclusao 
						FROM 
							public.arquivo arq
						INNER JOIN 
							obras.arquivosobra oar ON arq.arqid = oar.arqid
						INNER JOIN 
							obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
						WHERE 
							obr.obrid='". $obr['obrid'] ."' AND
		  					aqostatus = 'A' AND
		  				   (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
						ORDER BY 
							arq.arqid DESC LIMIT 4";

				$fotos = $db->carregar($sql);
				
				print '<tr>';
				
				if( $fotos[0] ){
					for( $k = 0; $k < count($fotos); $k++ ){

						$_SESSION['imgparametos'][$fotos[$k]["arqid"]] = array( "filtro" => "cnt.obrid=".$uni['obrid']." AND aqostatus = 'A'", 
																				"tabela" => "obras.arquivosobra");
						
						print "<td valign=\"top\" align=\"center\">"
							. "<img id='".$fotos[$k]["arqid"]."' onclick='window.open(\"../slideshow/slideshow/ajustarimgparam3.php?pagina=0&_sisarquivo=obras&obrid={$obr['obrid']}\",\"imagem\",\"width=850,height=600,resizable=yes\");' src='../slideshow/slideshow/verimagem.php?_sisarquivo=obras&newwidth=120&newheight=90&arqid=".$fotos[$k]["arqid"]."' hspace='10' vspace='3' style='width:80px; height:80px;' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\"/><br />"
							. $fotos[$k]["aqodtinclusao"]."<br />"
							. $fotos[$k]["arqdescricao"]
							. "</td>";
						
					}
				} else {
					print "<td colspan='5'>Não existe(m) foto(s) cadastrada(s).</td>";
				}
				
			print '		</tr>'
				. '</table>'
				. '</form>'
				. '<br/>';
				
		}
	}else{
		
		print '<tr><td align="center"><b>Não existe(m) Obra(s) cadastrada(s).</b></td></tr>';
		
	}
	
	// fecha quadro do painel
	print '	</table>'
	    . '</div>';
	
}

/**
 * Controla as atualizaçães do quadro do painel de acordo com a ação passada
 * 
 * @author Fernando A. Bagno da Silva
 * @since 14/07/2009
 * @param string $acao
 * @param string $dado
 * @param integer $orgid
 * 
 */
function painel_atualiza_quadro( $acao, $dado, $orgid, $estuf = '', $entid = '' ){
	
	switch( $acao ){
		case 'listaobras':
			// exibe lista de obras
			painel_lista_obras( $dado, $orgid );
			die;
		break;
		case 'situacaoobras':
			// exibe lista de obras
			painel_dados_obras( $dado, $orgid, $estuf, $entid );
			die;
		break;
		case 'dadosobras':
			// exibe lista de obras
			painel_dados_obras( $dado, $orgid );
			die;
		break;
		case 'dadosunidade':
			// exibe dados da unidade
			painel_dados_unidade( $dado, $orgid );
			die;
		break;
		case 'dadoscampus':
			// exibe dados do campus
			painel_dados_unidade( $dado, $orgid );
			die;
		case 'dadosacademico':
			// exibe dados academico da unidade
			painel_dados_academico( $dado, $orgid );
			die;
		break;
		case 'dadosconcursos':
			painel_dados_concursos( $dado, $orgid );
			die;
		break;
		case 'dadosfinanceiro':
			painel_dados_financeiro( $dado, $orgid );
			die;
		break;
		case 'mapa':
			// exibe novamente o mapa do brasil
			painel_abre_mapa( $orgid );
			die;
		break;
		case 'listaCampus':
			// exibe campus da unidade
			painel_lista_campus( $dado, $orgid );
			//academico_lista_campus_painel( $dado, $orgid );
			die;
		break;
		default:
		break;
		
	}
	
}

function painel_montaabas($itensMenu, $url = false, $acao = 'link'){
    
	global $db;
	
	$url = $url ? $url : $_SERVER['REQUEST_URI'];
	$rs  = (is_array($itensMenu)) ? $itensMenu : $db->carregar($itensMenu);
    

    $menu = '<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint">'
          . '	<tr>'
          . '		<td>'
          . '			<table cellpadding="0" cellspacing="0" align="left">'
          . '				<tr>';

    $nlinhas = count($rs) - 1;

    for ($j = 0; $j <= $nlinhas; $j++) {
        
    	extract($rs[$j]);
		
    	$gifaba = ($j == 0) ? 'aba_nosel_ini.gif' : 'aba_nosel.gif';
    	$giffundo_aba = 'aba_fundo_nosel.gif';
		$onclick = $acao == 'link' ? "window.location='".$link."'" : $link;
		
//		$giffundo_aba_mak = "aba_fundo_sel.gif";
//		$giffundo_aba = $giffundo_aba_mak;
		
        
        $menu .= '<td height="20" valign="top"><img src="../imagens/'.$gifaba.'" width="11" height="20" alt="" border="0"></td>'
              .  '<td height="20" align="center" valign="middle" background="../imagens/'.$giffundo_aba.'" style="padding-left: 10px; padding-right: 10px;cursor:pointer;" onclick="' . $onclick . '">'
			  .  '<a>' . $descricao . '</b></a></td>';
        
    }

    $gifaba = 'aba_nosel_fim.gif';

    $menu .= '<td height="20" valign="top"><img src="../imagens/'.$gifaba.'" width="11" height="20" alt="" border="0"></td></tr></table></td></tr></table>';

    return $menu;
}

/**
 * Cria as abas que compõe o quadro do painel
 * 
 * @author Fernando A. Bagno da Silva
 * @since 14/07/2009
 * @param integer $entid
 * @param string $dados
 * @return mixed
 * 
 */
function painel_abas_painel( $entid, $dados = 'unidade' ){
	
	global $db;
	
	// cria o array com os dados do menu 
	$menu_painel = array( 0 => array("id" => 1, "descricao" => "Dados",      "link" => "atualiza_div( '{$dados}', {$entid} );"),
						  1 => array("id" => 2, "descricao" => "Concursos",  "link" => "atualiza_div( 'concursos', {$entid} );"),
						  2 => array("id" => 3, "descricao" => "Academico",  "link" => "atualiza_div( 'academico', {$entid} );"),
						  3 => array("id" => 4, "descricao" => "Obras", 	 "link" => "atualiza_div( 'obras', {$entid} );"),
						  4 => array("id" => 5, "descricao" => "Financeiro", "link" => "atualiza_div( 'financeiro', {$entid} );")
						  );
	if ($dados == 'unidade'){					  
		array_push($menu_painel, array("id" => 6, "descricao" => "Campus", "link" => "atualiza_div( 'listaCampus', {$entid} );"));
	}					  
						  
	// retorna as abas
	return painel_montaabas($menu_painel, "atualiza_div( '{$dados}', {$entid} );", 'js' );
						  
}

/***
 * Cria o cabeçalho do quadro que compõe o painel com as informaçães da unidade
 * 
 * @author Fernando A. Bagno da Silva
 * @since 15/07/2009
 * @param integer $entid
 * @param string $tipo
 * @param string $orgid
 * 
 */
function painel_cabecalho_quadro_painel( $entid, $tipo = 'unidade', $orgid = '', $obrid = '' ){
	global $db;
	
	// case seja no modelo campus
	if ( $tipo == 'campus' ){
		
		// verifica a função da entidade de acordo com o tipo de ensino 
		switch ( $orgid ){
			case '1':
				$funid = 18;
			break;
			case '2':
				$funid = 17;
			break;
		}
		
		// pega id da unidade pai
		$idunidade = $db->pegaUm("SELECT ea.entid
								  FROM entidade.funcaoentidade ef
								  INNER JOIN entidade.entidade e ON e.entid = ef.entid
								  INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid 
								  WHERE ef.entid = {$entid} AND funid = {$funid}");
		
		// pega a descricao da unidade pai
		if ( $idunidade ){
			$unidade = $db->pegaUm("SELECT '<a style=\"cursor:pointer\" title=\"Clique para abrir os dados da Instituição\" onclick=\"atualiza_div( \'unidade\', \'' || entid || '\');\">' || UPPER(entnome) || '</a>' AS entnome FROM entidade.entidade WHERE entid={$idunidade}");
			$unidade = $unidade  ? $unidade . '<br/>' : '';
		}
		
	}
	
	if ( $tipo == 'obra' && $obrid != '' ){
		
		$obra = $db->pegaUm("SELECT obrdesc FROM obras.obrainfraestrutura WHERE obrid = {$obrid}");
		
	}
	
	// pega o nome das undiades e a sua caracterização
	$nome   = $db->pegaUm("SELECT UPPER(entnome) FROM entidade.entidade WHERE entid={$entid}");
	
	// monta a tabela com os dados da unidade e as abas
	print '<table width="460px;" style="text-align:center;">'
		. '	   <tr><td colspan="2" align="right"> <a onclick="displayMessage(\'?modulo=principal/redefederal/mapapainel&acao=A\');" style="cursor:pointer;">Abrir Mapa</a> </td></tr>'
	    . '    <tr><td>';
	
	print painel_abas_painel( $entid, $tipo ) . '';
	
	print '    </td><tr>' 
	    . '    <tr><td colspan="2" class="titulounidade">' . $unidade . $nome . '</td></tr>'
	    . '</table>'
	    . '<div style="overflow:auto; height:280px; width:455px; border:1px solid #cccccc;">'
	    . '<table width="100%">';
	
}

/**
 * Lista os campus da unidade selecionada
 * 
 * @author Felipe Tarchiani Cerávolo Chiavicatti
 * @since 31/08/2009
 * @param integer $entid
 * @param integer $orgid
 *
 */
function painel_lista_campus($entid, $orgid){

	painel_cabecalho_quadro_painel( $entid );
	print '		<tr><td>';			
	painel_lista_campus_painel( $entid, $orgid );	
	print '		</td>'
		. '		</tr>'
	    . '	</table>'
	    . '</div>';
}


/**
 * Exibe os dados da unidade informada
 * 
 * @author Fernando A. Bagno da Silva
 * @since 15/07/2009
 * @param integer $entid
 *
 */
function painel_dados_unidade( $entid, $orgid ){
	
	global $db;

	switch ( $orgid ){
		case '1':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . 18);
			break;
		case '2':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . 17);
			break;
	}
	// monta quadro do painel
	!empty( $campus ) ? painel_cabecalho_quadro_painel( $entid, 'campus', $orgid ) : painel_cabecalho_quadro_painel( $entid );
	
	// pega a caracterização da unidade
	$edtdsc = $db->pegaUm("SELECT edtdsc FROM academico.entidadedetalhe WHERE entid={$entid}");
	$edtdsc = $edtdsc ? $edtdsc : 'Não Informado';
	
	// cria a tabela com os dados da unidade
	print '    <tr><td class="SubTituloEsquerda" colspan="2">Caracterização da Instituição</td></tr>'
	    . '    <tr><td colspan="2" style="text-align:justify;">' . $edtdsc . '</td></tr>'
	    . '    <tr><td class="SubTituloEsquerda" colspan="2">Dados da Instituição</td>';

	// monta os dados do endereço da unidade
	painel_monta_endereco( $entid );
		
	print '		</tr>'
	    . '		<tr><td class="SubTituloEsquerda" colspan="2">Dados do dirigente</td>';

	// monta dados dos dirigentes da unidade
	painel_monta_dirigente( $entid, 'unidade', 1 );
			
	print '		</tr>'
	    . '	</table>'
	    . '</div>';
		
}

function painel_monta_endereco($id) {
	
	global $db;
	?>
	<tr><td class="SubTituloDireita">Endereço :</td><td>
	<? 
	$linha = $db->pegaLinha("SELECT * FROM entidade.endereco WHERE entid={$id}");
	
	$dadosendvazio = true;
	unset($endes);
	if(trim($linha['endlog'])) {
		$endes[] = $linha['endlog'];
		$dadosendvazio = false;
	}
	if(trim($linha['endnum'])) {
		$endes[] = " número ".$linha['endnum'];
		$dadosendvazio = false;
	}
	if(trim($linha['endbai'])) {
		$endes[] = $linha['endbai'];
		$dadosendvazio = false;
	}
	if(trim($linha['estuf'])) {
		$endes[] = $linha['estuf'];
		$dadosendvazio = false;
	}
	if(trim($linha['endcep'])) {
		$endes[] = "CEP ".$linha['endcep'];
		$dadosendvazio = false;
	}
	if($dadosendvazio) {
		echo "Não informado";
	} else {
		echo implode(", ",$endes);
	}
	?></td></tr>
	<tr><td class="SubTituloDireita" nowrap>Municípios limítrofes :</td><td><?
	if($linha['muncod']) {
		unset($dadosvizinhos,$vizs);
		$sql = "SELECT mun.mundescricao FROM territorios.municipiosvizinhos muv 
				LEFT JOIN territorios.municipio mun ON muv.muncodvizinho = mun.muncod 
				WHERE muv.muncod='".$linha['muncod']."'";
		
		$dadosvizinhos = $db->carregar($sql);
		if($dadosvizinhos[0]) {
			foreach($dadosvizinhos as $viz) {
				$vizs[]= $viz['mundescricao'];
			}
			echo implode(", ", $vizs);
		} else {
			echo "Não informado";
		}
	} else {
		echo "Não informado";
	}
	?></td></tr><?
}

function painel_monta_dirigente($id, $ent, $tipoensino) {
	
	global $db,$_funcoes;
	
	$dirigente = $db->pegaLinha("SELECT ent.*, fundsc FROM entidade.entidade ent 
								 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
								 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
								 LEFT JOIN entidade.funcao ef ON ef.funid = fen.funid 
								 WHERE fea.entid = '".$id."' AND fen.funid='". $_funcoes[$tipoensino][$ent] ."'");
								 
	
	if($dirigente) {
	?>
	<tr><td class="SubTituloDireita">Nome :</td><td><? echo (($dirigente['entnome'])?$dirigente['entnome']:"Não informado"); ?></td></tr>
	<tr><td class="SubTituloDireita" nowrap>Função / Ocupação :</td><td><? echo (($dirigente['fundsc'])?$dirigente['fundsc']:"Não informado");?></td></tr>
	<tr><td class="SubTituloDireita">Telefones :</td><td><? 
	$dadostelvazio = true;
	if($dirigente['entnumresidencial']) {
		$tels[] = "Residencial: (".trim($dirigente['entnumdddresidencial']).") ".trim($dirigente['entnumresidencial']);
		$dadostelvazio = false;
	}
	unset($ramal);
	if($dirigente['entnumcomercial']) {
		if(trim($dirigente['entnumramalcomercial']))
			$ramal = " ramal ".trim($dirigente['entnumramalcomercial']);
		$tels[] = "Comercial: (".trim($dirigente['entnumdddcomercial']).") ".trim($dirigente['entnumcomercial']).$ramal;
		$dadostelvazio = false;
	}
	unset($ramal);
	if($dirigente['entnumfax']) {
		if(trim($dirigente['entnumramalfax']))
			$ramal = " ramal ".trim($dirigente['entnumramalfax']);
		$tels[] = "Fax: (".trim($dirigente['entnumdddfax']).") ".trim($dirigente['entnumfax']).$ramal;
		$dadostelvazio = false;
	}
	if($dadostelvazio) {
		echo "Não informado";
	} else {
		echo implode(", ",$tels);
	}
	?></td></tr>
	<tr><td class="SubTituloDireita">E-mail :</td><td><? echo (($dirigente['entemail'])?$dirigente['entemail']:"Não informado"); ?></td></tr>
	<?
	} else {
	?><tr><td colspan="2">NNãoo Informado</td></tr><?
	}
}

function painel_dados_academico( $entid, $orgid ){
	
	global $db, $anosanalisados, $tituloitens;

	switch ( $orgid ){
		case '1':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . 18);
			break;
		case '2':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . 17);
			break;
	}
	
	if(	!empty($campus) ) {
		
		painel_cabecalho_quadro_painel( $entid, 'campus', $orgid );
		
		$sql = "SELECT DISTINCT
					 e.entid as campusid,
		    		 e.entnome as campus 
			    FROM 
			    	entidade.entidade e 
				WHERE 
					e.entid = {$entid}";
			
		$campus = $db->carregar($sql);
	
		echo '<tr><td class="SubTituloEsquerda" colspan="2">INFORMAÇÕES ACADÊMICAS E FINANCEIRAS</td></tr>'
		   . '<tr><td colspan="2"><strong>Processo seletivo de estudantes</strong></td></tr>'
		   . '<tr><td colspan="2">a';
		
		if ( is_array($campus) ){
			foreach($campus as $camp) {
				
				$sql = "SELECT 
							'de '||(to_char(prsinscricaoini,'DD/MM/YYYY') ||' até '|| to_char(prsinscricaofim,'DD/MM/YYYY')) as perinsc, 
							'de '||(to_char(prsprovaini,'DD/MM/YYYY') ||' até '|| to_char(prsprovafim,'DD/MM/YYYY')) as perprova, 
							to_char(prsinicioaula,'DD/MM/YYYY') 
						FROM 
							academico.campus cam
					    INNER JOIN 
					    	academico.processoseletivo prs ON prs.cmpid = cam.cmpid  
					    WHERE 
					    	cam.entid='".$camp['campusid']."'";
				
				echo '<tr><td style="margin: 0px;">';
				
				$cabecalho = array( "Inscrição", "Provas", "Início das Aulas" );
				$db->monta_lista_simples( $sql, $cabecalho, 1000, 30, 'N', '100%');
				
				echo '<br/></td></tr>';
				
			}
		}
	
		$listacampus = $db->carregar("SELECT 
										ent.entid, ent.entnome, cmp.cmpid 
									  FROM 
									  	entidade.entidade ent
									  LEFT JOIN 
									  	academico.campus cmp ON cmp.entid = ent.entid 
									  LEFT JOIN 
									  	entidade.funcaoentidade fen ON fen.entid = ent.entid 
									  LEFT JOIN 
									  	entidade.funentassoc fea ON fea.fueid = fen.fueid  
									  WHERE 
									  	ent.entid='".$entid."' AND 
									  	cmp.cmpid IS NOT NULL");

		if( $listacampus[0] ) {
			foreach( $listacampus as $campus ) {
			
				print '<table width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">'
					. '		<tr><td class="SubTituloEsquerda">' .( ($tituloitens[$orgid]) ? $tituloitens[$orgid] : $tituloitens['default'] ) . '</td></tr>'
					. '		<tr><td style="margin:0px;">';
					
				// Se tiver anos analisados por tipo de ensino (declarado no constantes.php), caso Não, utilizar o padrão
				$anos = $anosanalisados[$orgid] ? $anosanalisados[$orgid] : $anosanalisados['default'];  
				
				unset($cabecalho,$paramselects);
				$cabecalho[] = "Itens";
				
				foreach($anos as $ano) {
					
					$paramselects[] = "(SELECT 
											coalesce(cast(cpivalor as varchar),'') 
										FROM 
											academico.campusitem cpi 
										WHERE 
											itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND 
											cpi.cmpid = '". $campus['cmpid'] ."' AND cpi.cpitabnum = 1) AS ano_".$ano;
					$cabecalho[] 	= $ano;
							
				}
				
				$paramselects = implode( ",", $paramselects );
				
				// 	criando o SELECT
				$sql = "SELECT 
							'<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' );\" >'||itm.itmdsc||'</span></strong>',
						". $paramselects ."
						FROM 
							academico.item itm 
						LEFT JOIN 
							academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
						LEFT JOIN 
							academico.orgaoitem tei ON tei.itmid = itm.itmid 
						WHERE 
							tei.orgid = '". $orgid ."' AND itm.itmglobal = false
						ORDER BY 
							tei.teiordem";
				
				$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
				
				print '</td></tr>'
					. '<tr><td class="SubTituloEsquerda">Situação Atual</td></tr>'
					. '<tr><td>';
			
				unset($cabecalho);
				
				$cabecalho = array( "Itens", "Atual" );
				$paramselct = "(SELECT 
									coalesce(cast(cpivalor as varchar),'') 
								FROM 
									academico.campusitem cpi 
								WHERE 
									itm.itmid = cpi.itmid AND 
									cpi.cmpid = '". $campus['cmpid'] ."' AND cpi.cpitabnum = 1) AS ano";
	
				$sql = "SELECT 
							'<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' )\" >'||itm.itmdsc||'</span></strong>',
							". $paramselct ."
						FROM 
							academico.item itm 
						LEFT JOIN 
							academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
						LEFT JOIN 
							academico.orgaoitem tei ON tei.itmid = itm.itmid 
						WHERE 
							tei.orgid = '". $orgid ."' AND itm.itmglobal = true
						ORDER 
							BY tei.teiordem";
					
				$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
				
				print '		</td></tr>'
					. '</table>';
			
			}
			
		} else {
			print "<tr><td align=\"center\"><b>Não exitem campus associados.</b></td></tr>";
		}
			
	}else{
	
		painel_cabecalho_quadro_painel( $entid );
		
		echo '		<tr><td class="SubTituloEsquerda">' . ( ($tituloitens[$orgid]) ? $tituloitens[$orgid] : $tituloitens['default']) . '</td></tr>'
		  .	 '		<tr><td>';
		
		// Se tiver anos analisados por tipo de ensino (declarado no constantes.php), caso Não, utilizar o padrão
		if($anosanalisados[$orgid]) {
			$anos = $anosanalisados[$orgid];
		} else {
			$anos = $anosanalisados['default'];
		}
		unset($cabecalho);
		$cabecalho[] = "Itens";
		foreach($anos as $ano) {
			$paramselects[] = "CASE WHEN cast((SELECT SUM(cpivalor) FROM academico.campusitem cpi 
																				 LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
																  			     LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid 
																  			     LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
																  			     LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
																  			     WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '".$ano."' AND fea.entid = '".$entid."' AND cpitabnum = 1) as varchar) is null 
																  THEN '' 
																  ELSE 	   cast((SELECT SUM(cpivalor) FROM academico.campusitem cpi LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
																  			     LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid 
																  			     LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
																  			     LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
																  			     WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '".$ano."' AND fea.entid = '".$entid."' AND cpitabnum = 1) as varchar)
																  END  AS ano_".$ano;
			$cabecalho[] = $ano;		
		}
		$paramselects = implode(",",$paramselects);
		
		// criando o SELECT
		$sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' );\" >'||itm.itmdsc||'</span></strong>',
				". $paramselects ."
				FROM academico.item itm 
				LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
				LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
				WHERE tei.orgid = '". $orgid ."' AND itm.itmglobal = false
				ORDER BY tei.teiordem";
		
		$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
		
		echo '</td></tr><tr><td class="SubTituloEsquerda">Situação Atual</td></tr>';
		echo '<tr><td>';
		unset($cabecalho);
		$cabecalho = array("Itens", "Atual");
		$paramselct = "CASE WHEN cast((SELECT SUM(cpivalor) FROM academico.campusitem cpi LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
																  	     LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid 
																  	     LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
																  	     LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
																  		 WHERE itm.itmid = cpi.itmid AND fea.entid = '".$entid."' AND cpi.cpitabnum = 1) as varchar) is null 
														  THEN '' 
														  ELSE 	   cast((SELECT SUM(cpivalor) FROM academico.campusitem cpi LEFT JOIN academico.campus cmp ON cmp.cmpid = cpi.cmpid
																  			     LEFT JOIN entidade.entidade ent ON ent.entid = cmp.entid 
																  	     		 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
																  	             LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
																  			     WHERE itm.itmid = cpi.itmid AND fea.entid = '".$entid."' AND cpi.cpitabnum = 1) as varchar)
														  END  AS ano";
		
		$sql = "SELECT '<strong><span onmouseover=\"this.parentNode.parentNode.title=\'\';return escape(\'' ||itm.itmobs|| '\' )\" >'||itm.itmdsc||'</span></strong>',
				". $paramselct ."
				FROM academico.item itm 
				LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
				LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
				WHERE tei.orgid = '". $orgid ."' AND itm.itmglobal = true
				ORDER BY tei.teiordem";
		$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
		
		$sql = "SELECT DISTINCT
				 cam.cmpid,
				 cam.cmpobs,
				 cam.cmpdataimplantacao,
				 cam.cmpexistencia,
				 cam.cmpsituacao,
				 cam.cmpinstalacao,
				 cam.cmpsituacaoobra,
				 e.entid as campusid,
	    		 e.entnome as campus, 
		    	 en.estuf AS uf,
		    	 to_char(cam.cmpdataatualizacao, 'dd/mm/yyyy HH24:MI:SS') as cmpdataatualizacao,
				 m.mundsc AS municipio
				FROM entidade.entidade e 
				INNER JOIN entidade.funcaoentidade fen ON fen.entid = e.entid 
				INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
				INNER JOIN entidade.entidade au ON fea.entid = au.entid 
				INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = au.entid
				INNER JOIN academico.orgaouo teu ON teu.funid = fen2.funid 
				INNER JOIN academico.campus cam ON e.entid = cam.entid					
				INNER JOIN entidade.endereco en ON( e.entid = en.entid AND en.tpeid = 1)
				INNER JOIN public.municipio m on m.muncod = en.muncod  
				WHERE fea.entid = {$entid}";
		
		$campus = $db->carregar($sql);
	
		print '<tr><td></td></tr>'
			. '<tr><td class="SubTituloEsquerda" colspan="2">INFORMAÇÕES ACADÊMICAS E FINANCEIRAS</td></tr>'
		    . '<tr><td colspan="2"><strong>Processo seletivo de estudantes</strong></td></tr>'
		    . '<tr><td colspan="2">';
		
		if ( is_array($campus) ){
			foreach($campus as $camp) {
				
				echo '<tr><td colspan="2" class="titulounidade">' . strtoupper($camp['campus']) .'</td></tr>';
				
				$sql = "SELECT 'de '||(to_char(prsinscricaoini,'DD/MM/YYYY') ||' até '|| to_char(prsinscricaofim,'DD/MM/YYYY')) as perinsc, 'de '||(to_char(prsprovaini,'DD/MM/YYYY') ||' até '|| to_char(prsprovafim,'DD/MM/YYYY')) as perprova, to_char(prsinicioaula,'DD/MM/YYYY') FROM academico.campus cam
					    INNER JOIN academico.processoseletivo prs ON prs.cmpid = cam.cmpid  
					    WHERE cam.entid='".$camp['campusid']."'";
				
				echo '<tr><td style="margin: 0px;">';
				
				$cabecalho = array( "Inscrição", "Provas", "Início das Aulas" );
				$db->monta_lista_simples( $sql, $cabecalho, 1000, 30, 'N', '100%');
				
				echo '<br/></td></tr>';
				
			}
		}
		
	}
	
	echo '		</tr>'
	   . '	</table>'
	   . '</div>';
	
}

function painel_abre_mapa( $orgid ){
	
	echo '<img src="/imagens/mapa_brasil.png" width="444" height="357" border="0" usemap="#mapaBrasil" />
			<map name="mapaBrasil" id="mapaBrasil">
				<area shape="rect" coords="388,15,427,48"   style="cursor:pointer;" onclick="painel_listaUnidades(\'todos\', '.$orgid.');" title="Brasil"/>							
				<area shape="rect" coords="48,124,74,151"   style="cursor:pointer;" onclick="painel_listaUnidades(\'AC\', '.$orgid.');" title="Acre"/>
				<area shape="rect" coords="364,147,432,161" style="cursor:pointer;" onclick="painel_listaUnidades(\'AL\', '.$orgid.');" title="Alagoas"/>
				<area shape="rect" coords="202,27,233,56"   style="cursor:pointer;" onclick="painel_listaUnidades(\'AP\', '.$orgid.');" title="Amapá"/>
				<area shape="rect" coords="89,76,133,107"   style="cursor:pointer;" onclick="painel_listaUnidades(\'AM\', '.$orgid.');" title="Amazonas"/>
				<area shape="rect" coords="294,155,320,183" style="cursor:pointer;" onclick="painel_listaUnidades(\'BA\', '.$orgid.');" title="Bahia"/>
				<area shape="rect" coords="311,86,341,114"  style="cursor:pointer;" onclick="painel_listaUnidades(\'CE\', '.$orgid.');" title="Ceará"/>
				<area shape="rect" coords="244,171,281,197" style="cursor:pointer;" onclick="painel_listaUnidades(\'DF\', '.$orgid.');" title="Distrito Federal"/>
				<area shape="rect" coords="331,215,369,242" style="cursor:pointer;" onclick="painel_listaUnidades(\'ES\', '.$orgid.');" title="Espírito Santo"/>
				<area shape="rect" coords="217,187,243,218" style="cursor:pointer;" onclick="painel_listaUnidades(\'GO\', '.$orgid.');" title="Goiís"/>
				<area shape="rect" coords="154,155,210,186" style="cursor:pointer;" onclick="painel_listaUnidades(\'MT\', '.$orgid.');" title="Mato Grosso"/>
				<area shape="rect" coords="156,219,202,246" style="cursor:pointer;" onclick="painel_listaUnidades(\'MS\', '.$orgid.');" title="Mato Grosso do Sul"/>
				<area shape="rect" coords="248,80,301,111"  style="cursor:pointer;" onclick="painel_listaUnidades(\'MA\', '.$orgid.');" title="Maranhão"/>
				<area shape="rect" coords="264,206,295,235" style="cursor:pointer;" onclick="painel_listaUnidades(\'MG\', '.$orgid.');" title="Minas Gerais"/>
				<area shape="rect" coords="188,84,217,112"  style="cursor:pointer;" onclick="painel_listaUnidades(\'PA\', '.$orgid.');" title="Pará"/>
				<area shape="rect" coords="368,112,433,130" style="cursor:pointer;" onclick="painel_listaUnidades(\'PB\', '.$orgid.');" title="Paraíba"/>
				<area shape="rect" coords="201,262,231,289" style="cursor:pointer;" onclick="painel_listaUnidades(\'PR\', '.$orgid.');" title="Paraná"/>
				<area shape="rect" coords="369,131,454,147" style="cursor:pointer;" onclick="painel_listaUnidades(\'PE\', '.$orgid.');" title="Pernambuco"/>
				<area shape="rect" coords="285,116,313,146" style="cursor:pointer;" onclick="painel_listaUnidades(\'PI\', '.$orgid.');" title="Piauí"/>
				<area shape="rect" coords="349,83,383,108"  style="cursor:pointer;" onclick="painel_listaUnidades(\'RN\', '.$orgid.');" title="Rio Grande do Norte"/>
				<area shape="rect" coords="189,310,224,337" style="cursor:pointer;" onclick="painel_listaUnidades(\'RS\', '.$orgid.');" title="Rio Grande do Sul"/>
				<area shape="rect" coords="302,250,334,281" style="cursor:pointer;" onclick="painel_listaUnidades(\'RJ\', '.$orgid.');" title="Rio de Janeiro"/>
				<area shape="rect" coords="98,139,141,169"  style="cursor:pointer;" onclick="painel_listaUnidades(\'RO\', '.$orgid.');" title="Rondônia"/>
				<area shape="rect" coords="112,24,147,56"   style="cursor:pointer;" onclick="painel_listaUnidades(\'RR\', '.$orgid.');" title="Roraima"/>
				<area shape="rect" coords="228,293,272,313" style="cursor:pointer;" onclick="painel_listaUnidades(\'SC\', '.$orgid.');" title="Santa Catarina"/>
				<area shape="rect" coords="233,243,268,270" style="cursor:pointer;" onclick="painel_listaUnidades(\'SP\', '.$orgid.');" title="São Paulo"/>
				<area shape="rect" coords="337,161,401,178" style="cursor:pointer;" onclick="painel_listaUnidades(\'SE\', '.$orgid.');" title="Sergipe"/>
				<area shape="rect" coords="227,130,270,163" style="cursor:pointer;" onclick="painel_listaUnidades(\'TO\', '.$orgid.');" title="Tocantins"/>
				<area shape="rect" coords="17,264,85,282"   style="cursor:pointer;" onclick="painel_listaUnidades(\'norte\', '.$orgid.');" title="Norte" />
				<area shape="rect" coords="16,281,94,296"   style="cursor:pointer;" onclick="painel_listaUnidades(\'nordeste\', '.$orgid.');" title="Nordeste" />
				<area shape="rect" coords="15,296,112,312"  style="cursor:pointer;" onclick="painel_listaUnidades(\'centrooeste\', '.$orgid.');" title="Centro-Oeste" />
				<area shape="rect" coords="14,312,100,329"  style="cursor:pointer;" onclick="painel_listaUnidades(\'sudeste\', '.$orgid.');" title="Sudeste" />
				<area shape="rect" coords="13,329,68,344"   style="cursor:pointer;" onclick="painel_listaUnidades(\'sul\', '.$orgid.');" title="Sul" />
			</map>';
	
}

function painel_painel_agrupador(){
	
	$agrupador = array( "ano", "classe" );
	
	if (!$agrupador){
		$agrupador = array(
							'tipoensino',
/*							'ano',
							'portaria',
							'unidade',
							'campus',
							'programa',
							'classe'*/
						 );
	}elseif ( !is_array($agrupador) ){
		$agrupador = explode(",", $agrupador);
	}	
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"projetado",	
									   		"autorizado", 
 									   		"provimento",
 									   		"publicado",
 									   		"homologado",
											"lepvlrprovefetivados",
											"provimentosnaoefetivados",
											"provimentopendencia",
										    "homocolor"
										  )	  
				);
	
	foreach ($agrupador as $val): 
		switch ($val) {
		    case 'tipoensino':
				array_push($agp['agrupador'], array(
													"campo" => "tipoensino",
											  		"label" => "Tipo Ensino")										
									   				);				
		    	continue;
		        break;
		    case 'portaria':
				array_push($agp['agrupador'], array(
													"campo" => "portaria",
											  		"label" => "Portaria")										
									   				);					
		    	continue;
		        break;		    	
		    case 'campus':
				array_push($agp['agrupador'], array(
													"campo" => "campus",
											 		"label" => "Campus")										
									   				);					
		    	continue;			
		        break;	
		    case 'classe':
				array_push($agp['agrupador'], array(
												"campo" => "classe",
												"label" => "Classe")										
										   		);	
				continue;
				break;	    	
		    case 'programa':
				array_push($agp['agrupador'], array(
												"campo" => "programa",
												"label" => "Programa")										
										   		);	
				continue;
				break;	    	
		    case 'unidade':
				array_push($agp['agrupador'], array(
												"campo" => "unidade",
												"label" => "Unidade")										
										   		);	
				continue;
				break;					
		    case 'ano':
				array_push($agp['agrupador'], array(
												"campo" => "ano",
												"label" => "Ano")										
										   		);	
				continue;
				break;					
		}
	endforeach;
	return $agp;
	
}

function painel_painel_sql( $orgid, $entid ){
	
	global $db;
	
	switch ( $orgid ){
		case '1':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . 18);
			break;
		case '2':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . 17);
			break;
	}
	
	$select = array();
	$from	= array();
	
	$where[] = !empty( $orgid ) ? " orgid = {$orgid} " 	   : "";
	
	if ( !empty( $entid ) ){
		$where[] = !empty( $campus ) ? " idcampus = {$entid} " : " idunidade = {$entid}";
	}
	
	$agrupador = is_array($agrupador) ? implode(',', $agrupador) : $agrupador; 
	
	//if (strpos($agrupador, "unidade") !== false){
		$select[] = "COALESCE(unidade , 'Não informado') as unidade";
		$select[] = "idunidade as idunidade";		
	//}

	//if (strpos($agrupador, "campus") !== false){
		$select[] = "COALESCE(campus, 'Não informado') as campus";
		$select[] = "idcampus as idcampus";	
	//}	

	//if (strpos($agrupador, "classe") !== false){
		$select[] = "COALESCE(classe, 'Não informado') as classe";
		$select[] = "idclasse as idclasse";	
	//}	

	//if (strpos($agrupador, "tipoensino") !== false || !$agrupador){
		$select[] = "tipoensino as tipoensino";	
	//}	
	
	//if (strpos($agrupador, "programa") !== false){
		$select[] = "programa as programa";
		$select[] = "idprograma as idprograma";	
	//}	

	//if (strpos($agrupador, "ano") !== false){
		$select[] = "ano";
	//}	
	
	//if (strpos($agrupador, "portaria") !== false){
		$select[] = "'Nº controle: ' || COALESCE(portaria::VARCHAR,'Não informado') || ' / ' || 'Nº portaria: ' || COALESCE(numeroportaria::VARCHAR, 'Não informado') as portaria";
	//}	
			
	$sql = "select
				COALESCE(sum( projetado ),0) as projetado, 
				COALESCE(sum( autorizado ),0) as autorizado,  
				COALESCE(sum( provimentoautorizado ),0) as provimento, 
				COALESCE(sum( publicado ),0) as publicado, 
				COALESCE(sum( homologado ),0) as homologado,
				COALESCE(sum(lepvlrprovefetivados),0) as lepvlrprovefetivados,
				COALESCE((sum( provimentoautorizado ) - sum(lepvlrprovefetivados)),0) as provimentosnaoefetivados,
				--(sum( homologado ) - (sum( provimentoautorizado ) - sum(lepvlrprovefetivados)) ) AS provimentopendencia,
				CASE 
					WHEN COALESCE((COALESCE(sum( homologado ), 0) - COALESCE(sum(provimentoautorizado), 0)),0) >= 0 THEN COALESCE((COALESCE(sum( homologado ), 0) - COALESCE(sum(provimentoautorizado), 0)),0)
					ELSE 0
				END AS provimentopendencia,
				" . (implode(" , ", $select)) . "
			from
			(
			
				
				SELECT 
				p.prtid as portaria,
				p.prtnumero as numeroportaria,
				p.prtano as ano, 
				0 as projetado, 
				0 as autorizado, 
				0 as provimentoautorizado,
				sum(lepvlrpublicacao) as publicado,
				sum( lepvlrhomologado ) as homologado,
				sum(lepvlrprovefetivados) as lepvlrprovefetivados,
				COALESCE(ent.entnome , 'Não informado') as unidade ,
				ent.entid as idunidade, 
				COALESCE(cp.entnome, 'Não informado') as campus ,
				cp.entid as idcampus, 
				COALESCE(cl.clsdsc, 'Não informado') as classe ,
				cl.clsid as idclasse, 
				ao.orgdesc as tipoensino , 
				ao.orgid,
				pp.prgdsc as programa,
				p.prgid as idprograma 
				FROM 
					academico.portarias p 
				inner join academico.editalportaria ep ON ep.prtid = p.prtid 
				inner join academico.lancamentoeditalportaria lep ON lep.edpid = ep.edpid 
																	 AND lep.lepstatus = 'A'		
				INNER JOIN entidade.entidade cp ON cp.entid = ep.entidcampus 
				INNER JOIN entidade.entidade ent ON ent.entid = ep.entidentidade
				inner join academico.cargos c ON c.crgid = lep.crgid
				inner join academico.classes cl ON cl.clsid = c.clsid
				INNER JOIN academico.orgao ao ON ao.orgid = p.orgid 
				INNER JOIN academico.programa pp ON p.prgid = pp.prgid
				where 
					p.tprid = " . 1 . " 
					AND p.prtstatus = 'A' 
					AND ep.edpstatus = 'A' 
					AND ep.tpeid in ( " . 2 . ", " . 3 . ", " . 5 . " )					
				group by 
					p.prtid ,
					p.prtnumero,
					p.prtano, 
					COALESCE(ent.entnome , 'Não informado'),
					ent.entid, 
					COALESCE(cp.entnome, 'Não informado'),
					cp.entid, 
					COALESCE(cl.clsdsc, 'Não informado') ,
					cl.clsid, 
					ao.orgdesc , 
					ao.orgid,
					pp.prgdsc,
					p.prgid 
			
			
			
			union all
			
			
				SELECT 
					p.prtid as portaria,
					p.prtnumero as numeroportaria, 
					p.prtano as ano, 
					ap.acpvalor as projetado, 
					lp.lnpvalor as autorizado, 
					0 as provimentoautorizado,
					0 as publicado ,
					0 as homologado	,
					0 as lepvlrprovefetivados,
					COALESCE(ent.entnome , 'Não informado') as unidade ,
					ent.entid as idunidade, 
					COALESCE(c.entnome, 'Não informado') as campus ,
					c.entid as idcampus, 
					COALESCE(cl.clsdsc, 'Não informado') as classe ,
					cl.clsid as idclasse,  
					ao.orgdesc as tipoensino ,
					ao.orgid, 
					pp.prgdsc as programa,
					p.prgid as idprograma 
				FROM 
					academico.portarias p 
				INNER JOIN 
					( SELECT 
						prtid, entidcampus, entidentidade, clsid, sum(lnpvalor) as lnpvalor 
					  FROM 
						academico.lancamentosportaria a 
					  WHERE
					  	a.lnpstatus = 'A'							
					  GROUP BY 
						prtid, entidcampus, entidentidade, clsid ) lp ON lp.prtid = p.prtid 
																		 --and lp.lnpano = p.prtano
				INNER JOIN ( SELECT 
								prtid, entidcampus, entidentidade, clsid, sum(acpvalor) as acpvalor 
							  FROM 
								academico.acumuladoprojetado a 
							  GROUP BY 
								prtid, entidcampus, entidentidade, clsid ) ap ON ap.prtid = p.prtid
				

				INNER JOIN entidade.entidade ent ON ent.entid = lp.entidentidade and ent.entid = ap.entidentidade 
				INNER JOIN entidade.entidade c ON c.entid = lp.entidcampus and c.entid = ap.entidcampus
				INNER JOIN entidade.funcaoentidade fen ON fen.entid = c.entid 
				INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid and fea.entid = ent.entid		
											  
				INNER JOIN academico.classes cl ON cl.clsid = lp.clsid and cl.clsid = ap.clsid AND cl.clsstatus = 'A' 
				INNER JOIN academico.orgao ao ON ao.orgid = p.orgid 
				INNER JOIN academico.programa pp ON p.prgid = pp.prgid
				where 
					p.tprid = " . 1 . "
					AND p.prtstatus = 'A'
			
			
			union all
			
				SELECT 
					p2.prtid as portaria, 
					p2.prtnumero as numeroportaria,
					p2.prtano as ano, 
					0 as projetado, 
					0 as autorizado, 
					lp.lnpvalor as provimentoautorizado,
					0 as publicado ,
					0 as homologado	,
					0 as lepvlrprovefetivados,
					COALESCE(ent.entnome , 'Não informado') as unidade ,
					ent.entid as idunidade, 
					COALESCE(c.entnome, 'Não informado') as campus ,
					c.entid as idcampus, 
					COALESCE(cl.clsdsc, 'Não informado') as classe ,
					cl.clsid as idclasse,  
					ao.orgdesc as tipoensino , 
					ao.orgid,
					pp.prgdsc as programa,
					p.prgid as idprograma 
				FROM 
					academico.portarias p 
				inner join academico.portarias p2 ON p.prtidautprov = p2.prtid 
													 AND p2.prtstatus = 'A' 
				INNER JOIN 
					( SELECT 
						prtid, entidcampus, entidentidade, clsid, sum(lnpvalor) as lnpvalor 
					  FROM 
						academico.lancamentosportaria a 
					  WHERE
					  	a.lnpstatus = 'A'
					  GROUP BY 
						prtid, entidcampus, entidentidade, clsid ) lp ON lp.prtid = p.prtid --and lp.lnpano = p.prtano
				inner JOIN entidade.entidade ent ON ent.entid = lp.entidentidade 
				inner JOIN entidade.entidade c ON c.entid = lp.entidcampus 
				INNER JOIN academico.classes cl ON cl.clsid = lp.clsid AND cl.clsstatus = 'A' 
				INNER JOIN academico.orgao ao ON ao.orgid = p.orgid 
				INNER JOIN academico.programa pp ON p.prgid = pp.prgid
				WHERE
					p.tprid = " . 2 . "
					AND p.prtstatus = 'A'
			) as foo 
			" . (is_array($where) ? " WHERE ".implode(' AND ', $where) : '') . "
			group by 
				portaria,
				numeroportaria,
				unidade,
				idunidade,
				campus,
				idcampus,
				classe,
				idclasse,
				tipoensino,
				orgid,
				programa,
				idprograma,
				ano
			order by 
				ano";

	return $sql;
	
}

function painel_painel_coluna(){
	
	//$arrPerfil = array( PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR );
	$permissao = true;
	
	$coluna = array(
					array(
						  "campo" => "autorizado",
				   		  "label" => "Concursos <br/>Autorizados <br/>(B)",
						  "type"  => "numeric"
					),
					array(
						  "campo" => "publicado",
				   		  "label" => "Concursos <br/>Publicados <br/>(C)",
						  "type"  => "numeric"
					)
				);
					
	if ( $permissao ){				
		array_push($coluna, array(
								  "campo" 	 => "homologado",
						   		  "label" 	 => "Concursos <br/>Homologados <br/>(D)",
								  "type"  	 => "numeric",
								  "html"  	 => "<span style='color:{color}'>{homologado}</span>",
								  "php" 	 => array(
									  					"expressao" => "{homologado} > {autorizado}",
														"var" 		=> "color",
														"true"	    => "red",
														"false"     => "#0066CC",
									  				 )	
							)
				  );			
	}else{
		array_push($coluna, array(
								  "campo" 	 => "homologado",
						   		  "label" 	 => "Concursos <br/>Homologados <br/>(D)",
								  "type"  	 => "numeric"
								 )
				  );			

	}
	
	array_push($coluna,array(
							  "campo" => "provimento",
					   		  "label" => "Provimentos <br/>Autorizados <br/>(E)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "lepvlrprovefetivados",
					   		  "label" => "Provimentos <br/>Efetivados <br/>(F)",
							  "type"  => "numeric"
						)
				);		
	

	return $coluna;
				
}

function painel_dados_concursos( $entid, $orgid ){
	
	global $db;
	
	switch ( $orgid ){
		case '1':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . 18);
			break;
		case '2':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . 17);
			break;
	}
	
	// monta quadro do painel
	!empty( $campus ) ? painel_cabecalho_quadro_painel( $entid, 'campus', $orgid ) : painel_cabecalho_quadro_painel( $entid );
	
	$sql   = painel_painel_sql( $orgid, $entid );
	$dados = $db->carregar($sql);
	$agrup = painel_painel_agrupador();
	$col   = painel_painel_coluna();

	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $dados); 
	$r->setColuna($col);
	$r->setBrasao($true ? true : false);
	$r->setTotNivel(true);

	echo $r->getRelatorio();
	
	
}

function painel_dados_financeiro( $entid, $orgid ){
	
	global $db, $servidor_bd_siafi, $porta_bd_siafi, $nome_bd_siafi, $usuario_db_siafi, $senha_bd_siafi,	
		   $servidor_bd, $servidor_bd, $nome_bd, $usuario_db, $senha_bd;

	switch ( $orgid ){
		case '1':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . 18);
			break;
		case '2':
			$campus = $db->pegaUm("SELECT fueid FROM entidade.funcaoentidade WHERE entid = {$entid} AND funid = " . 17);
			break;
	}
	
	// monta quadro do painel
	!empty( $campus ) ? painel_cabecalho_quadro_painel( $entid, 'campus', $orgid ) : painel_cabecalho_quadro_painel( $entid );

	
	// Parâmetros para a nova conexão com o banco do SIAFI
	$servidor_bd = $servidor_bd_siafi;
	$porta_bd    = $porta_bd_siafi;
	$nome_bd     = $nome_bd_siafi;
	$usuario_db  = $usuario_db_siafi;
	$senha_bd    = $senha_bd_siafi;
	
	// Cria o novo objeto de conexão
	$db2 = new cls_banco();
	
	$sql = array( 0 => array( "acao" 	  => "<center><img style=\"cursor:pointer;\" border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio('execucao', {$entid}, '2009');\"></center>",
							  "descricao" => "Execução Por Unidade (Sem % de Execução) 2009" ),
				  1 => array( "acao" 	  => "<center><img style=\"cursor:pointer;\" border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio('execucao', {$entid}, '2010');\"></center>",
							  "descricao" => "Execução Por Unidade (Sem % de Execução) 2010" )
				);
	
	$cabecalho = array("Ação", "Relatórios");
	$db2->monta_lista_simples( $sql, $cabecalho, 1000, 30, 'N', '100%');
	
}

function painel_dados_obras( $stoid, $orgid, $estuf = '', $entid = '' ){
	
	global $db;
	
	$situacao = !empty($stoid) ? $db->pegaUm("SELECT stodesc FROM obras.situacaoobra WHERE stoid = {$stoid}") : 'Não Informado';
	
	// monta a tabela com os dados da unidade e as abas
	print '<table width="505px" style="text-align:center;">'
	    . '    <tr><td>'
	    . '    </td><td align="right"> <a onclick="displayMessage(\'?modulo=principal/mapapainel&acao=A\');" style="cursor:pointer;">Abrir Mapa</a> </td></tr>'
	    . '    <tr><td colspan="2" class="titulounidade"> Lista de Obras</td></tr>'
	    . '</table>'
	    . '<div style="overflow:auto; height:280px; width:500px; border:1px solid #cccccc;">'
	    . '<table width="100%">';

	$where_estuf = !empty($estuf) ? "AND ed.estuf = '{$estuf}'" : "";
	$where_stoid = !empty($stoid) ? "oi.stoid = " . $stoid : 'oi.stoid is null'; 
	$where_entid = !empty($entid) ? "AND oi.entidunidade = " . $entid : "";
	
	$sql = "SELECT 
				oi.stoid,
				oi.obrid,
				case when oi.stoid is not null then so.stodesc else 'Não Informado' end as situacao,
				obrdesc as nome,
				to_char(oi.obrdtinicio, 'dd/mm/yyyy') as inicio, 
				to_char(oi.obrdttermino, 'dd/mm/yyyy') as termino, 
				tm.mundescricao||'/'||ed.estuf as local,
				e.entnome as unidade,
				trim(ed.medlatitude) as latitude,
				trim(ed.medlongitude) as longitude,
				(SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual,
				obrcomposicao
			FROM 
				obras.obrainfraestrutura oi
			LEFT JOIN 
				obras.situacaoobra so ON oi.stoid = so.stoid
			INNER JOIN
				entidade.entidade e ON e.entid = oi.entidunidade
			LEFT JOIN
				entidade.endereco ed ON ed.endid = oi.endid
			LEFT JOIN
				territorios.municipio tm ON ed.muncod = tm.muncod 
			WHERE 
				{$where_stoid}
				AND oi.orgid = {$orgid} 
				{$where_estuf} AND
				oi.obsstatus = 'A'
				{$where_entid}
			ORDER BY
				unidade, nome, situacao";
	
	$obras = $db->carregar($sql);
	
	if ( $obras[0] ){
		$zoom = "<input type='hidden' id='endzoom'  value='15'/>"; 	
		foreach ( $obras as $obr ){

			switch ( $obr["stoid"] ){
				case "1":
					$obr['situacao'] = '<label style="color:#00AA00">' . $obr['situacao'] . '</label>';
				break;
				case "2":
					$obr['situacao'] = '<label style="color:#DD0000">' . $obr['situacao'] . '</label>';
				break;
				case "3":
					$obr['situacao'] = '<label style="color:blue">' . $obr['situacao'] . '</label>';
				break;
				case "6":
					$obr['situacao'] = '<label style="color:#DD0000">' . $obr['situacao'] . '</label>';
				break;
			}
			
			$dadoslatitude = explode(".", $obr["medlatitude"]);
			$graulatitude = $dadoslatitude[0];
			$minlatitude  = $dadoslatitude[1];
			$seglatitude  = $dadoslatitude[2];
			$pololatitude = $dadoslatitude[3];
			
			$latitude = !empty($graulatitude) ? $graulatitude . 'º ' . $minlatitude . '\' ' . $seglatitude .'" ' . $pololatitude : 'Não Informado';
			
			$campograulatitude = "<input type='hidden' id='{$obr["obrid"]}graulatitude' value='{$graulatitude}'/>";
			$campominlatitude  = "<input type='hidden' id='{$obr["obrid"]}minlatitude'  value='{$minlatitude}'/>";
			$camposeglatitude  = "<input type='hidden' id='{$obr["obrid"]}seglatitude'  value='{$seglatitude}'/>";
			$campopololatitude = "<input type='hidden' id='{$obr["obrid"]}pololatitude' value='{$pololatitude}'/>";
			
			$camposhiddenlat = $campograulatitude . $campominlatitude . $campopololatitude . $camposeglatitude;
			
			$dadoslongitude = explode(".", $obr["medlongitude"]);
			$graulongitude = $dadoslongitude[0];
			$minlongitude  = $dadoslongitude[1];
			$seglongitude  = $dadoslongitude[2];
			
			$longitude = !empty($graulongitude) ? $graulongitude . 'º ' . $minlongitude . '\' ' . $seglongitude .'" W' : 'Não Informado';
			
			$campograulongitude = "<input type='hidden' id='{$obr["obrid"]}graulongitude' value='{$graulongitude}'/>";
			$campominlongitude  = "<input type='hidden' id='{$obr["obrid"]}minlongitude'  value='{$minlongitude}'/>";
			$camposeglongitude  = "<input type='hidden' id='{$obr["obrid"]}seglongitude'  value='{$seglongitude}'/>";
			$camposhiddenlog = $campograulongitude . $campominlongitude . $camposeglongitude;
			
			// visualizar mapa 
			$mapa = !empty($graulatitude) && !empty($graulongitude) ? '<tr><td class="SubTituloDireita"></td><td><a onclick="abreMapa(' . $obr["obrid"] . ');" style="cursor:pointer;">Visualizar / Buscar No Mapa</a></td></tr>': '';
			
			$obrid = "<input type='hidden' id='obrid'  value='{$obr["obrid"]}'/>";
			
			print '<form action="" method="post" id="formulario">'
				. '<table width="98%" cellSpacing="1" cellPadding="3" align="center" style="border:1px solid #ccc; background-color:#fff;">'
				. '		<tr>'
				. '			<td class="SubTituloDireita">Unidade Implantadora:</td>'
				. '			<td colspan="4"><b>' . ( ($obr['unidade']) ? nl2br($obr['unidade']) : "Não Informada" ) . $obrid . '</b></td>'
				. '		</tr>'
				. '		<tr>'
			 	. '			<td class="SubTituloDireita" style="width:20%;">Nome da obra:</td><td colspan="2" style="width:45%;">' . $obr['nome'] . '</td>'
				. '			<td class="SubTituloDireita">Município/UF:</td><td>' . $obr['local'] . '</td>'
				. '		</tr>'
				. '		<tr>'
				. '			<td class="SubTituloDireita">Início programado:</td><td colspan="2">' . $obr['inicio'] . '</td>'
				. '			<td class="SubTituloDireita">Término programado:</td><td>' . $obr['termino'] . '</td>'
				. '		</tr>'
				. '		<tr>'
				. '			<td class="SubTituloDireita">Situação da Obra:</td><td colspan="2">' . $obr['situacao'] . '</td>'
				. '			<td class="SubTituloDireita">% Executado:</td><td colspan="2">' . $obr['percentual'] . '</td>'		
				. '		</tr>'
				. '		<tr>'
				. '			<td class="SubTituloDireita">Latitude:</td><td colspan="2">' . $latitude . $camposhiddenlat .  '</td>'
				. '			<td class="SubTituloDireita">Longitude:</td><td colspan="2">' . $longitude . $camposhiddenlog . $zoom . '</td>'		
				. '		</tr>'
				. $mapa
				. '		<tr>'
				. '			<td class="SubTituloDireita">Descrição:</td><td colspan="4" align="justify">'. ( ($obr['obrcomposicao']) ? nl2br($obr['obrcomposicao']) : "Nenhuma observação inserida" ) . '</td>'
				. '		</tr>'
				. '		<tr>'
				. '			<td class="SubTituloCentro" colspan="5">Fotos</td>'
				. '		</tr>';

				$sql = "SELECT 
								arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao, 
								to_char(oar.aqodtinclusao,'dd/mm/yyyy') as aqodtinclusao 
							FROM 
								public.arquivo arq
							INNER JOIN 
								obras.arquivosobra oar ON arq.arqid = oar.arqid
							INNER JOIN 
								obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
							WHERE 
								obr.obrid='". $obr['obrid'] ."' AND
			  					aqostatus = 'A' AND
			  				   (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
							ORDER BY 
								arq.arqid DESC LIMIT 4";
	
				$fotos = $db->carregar($sql);
				
				print '<tr>';
				
				if( $fotos[0] ){
					for( $k = 0; $k < count($fotos); $k++ ){

						$_SESSION['imgparametos'][$fotos[$k]["arqid"]] = array( "filtro" => "cnt.obrid=".$uni['obrid']." AND aqostatus = 'A'", 
																				"tabela" => "obras.arquivosobra");
						
						print "<td valign=\"top\" align=\"center\">"
							. "<img id='".$fotos[$k]["arqid"]."' onclick='window.open(\"../slideshow/slideshow/ajustarimgparam3.php?pagina=0&_sisarquivo=obras&obrid={$obr['obrid']}\",\"imagem\",\"width=850,height=600,resizable=yes\");' src='../slideshow/slideshow/verimagem.php?_sisarquivo=obras&newwidth=120&newheight=90&arqid=".$fotos[$k]["arqid"]."' hspace='10' vspace='3' style='width:80px; height:80px;' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\"/><br />"
							. $fotos[$k]["aqodtinclusao"]."<br />"
							. $fotos[$k]["arqdescricao"]
							. "</td>";
						
					}
				} else {
					print "<td colspan='5'>Não existe(m) foto(s) cadastrada(s).</td>";
				}
					
				print '		</tr>'
					. '</table>'
					. '</form>'
					. '<br/>';
				
		}
	}else{
		
		print '<tr><td align="center"><b>Não existe(m) Obra(s) cadastrada(s).</b></td></tr>';
		
	}
		
	print '		</tr>'
	    . '	</table>'
	    . '</div>';
	    
}

function painel_painel_dados_sig( $orgid, $estuf = '' ){
	
	global $db;
	
	$array_dados = array();
	$arr 		 = array();
	
	// cria o filtro com o funid
	switch ( $orgid ){
		case '1':
			$funid = " in ('" . 12 . "')";
			$funidFiltro = " AND fen.funid = 18 ";
		break;
		case '2':
			$funid = " in ('" . 11 . "', '" . 14 . "')";
			$funidFiltro = " AND fen.funid = 17 ";
		break;
	}
	
	// cria o join e o filtro de estado caso exista
	//$join_estuf   = ( $estuf && $estuf != 'todos' ) ? "LEFT JOIN entidade.endereco ed ON ed.entid = ent.entid" : "";
	$filtro_estuf = ( $estuf && $estuf != 'todos' ) ? "AND edc.estuf = '{$estuf}'" : "";
	
	// cria o join e o filtro por região, caso exista
	switch( $estuf ){
		case 'norte':
			$join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = edc.estuf";
			$filtro_estuf = "AND te.regcod = '1'";
		break;
		case 'nordeste':
			$join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = edc.estuf";
			$filtro_estuf = "AND te.regcod = '2'";
		break;
		case 'sudeste':
			$join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = edc.estuf";
			$filtro_estuf = "AND te.regcod = '3'";
		break;
		case 'sul':
			$join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = edc.estuf";
			$filtro_estuf = "AND te.regcod = '4'";
		break;
		case 'centrooeste':
			$join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = edc.estuf";
			$filtro_estuf = "AND te.regcod = '5'";
		break;
		default:
		break;
	}
	
	$sql = "(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpinstalacao || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpinstalacao || '\' ); formatarParametros();abreconteudo(\'painel.php?modulo=principal/redefederal/rede_federal&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpinstalacao || '&params=\' + params, \'' || ac.cmpinstalacao || '\');\"/></center>' as img,
				'Unidades em funcionamento em instalaçães definitivas' as nome,
				coalesce(count(ac.cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpinstalacao || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM 
				academico.campus ac
			LEFT JOIN 
				entidade.entidade ent ON ent.entid = ac.entid 
		  	LEFT JOIN 
		  		entidade.endereco edc ON edc.entid = ent.entid 
		  	LEFT JOIN 
		  		territorios.municipio mun ON mun.muncod = edc.muncod 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro 
		  	LEFT JOIN 
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid    
		  	LEFT JOIN 
		  		entidade.entidade uor ON uor.entid = fea.entid 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid 
		  	LEFT JOIN 
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE 
				cmpinstalacao = 'D' AND 
				ac.cmpexistencia = 'N' AND 
				ac.cmpsituacao = 'F' AND 
				tuo.funid {$funid} 
		  		{$filtro_estuf}
			GROUP BY 
				ac.cmpinstalacao)
					
				UNION ALL
					
			(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpinstalacao || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpinstalacao || '\' ); formatarParametros();abreconteudo(\'painel.php?modulo=principal/redefederal/rede_federal&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpinstalacao || '&params=\' + params, \'' || ac.cmpinstalacao || '\');\"/></center>' as img,
				'Unidades em funcionamento em instalaïções provisórias' as nome,
				coalesce(count(cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpinstalacao || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM 
				academico.campus ac
			LEFT JOIN 
				entidade.entidade ent ON ent.entid = ac.entid 
		  	LEFT JOIN 
		  		entidade.endereco edc ON edc.entid = ent.entid 
		  	LEFT JOIN 
		  		territorios.municipio mun ON mun.muncod = edc.muncod 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro 
		  	LEFT JOIN 
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid    
		  	LEFT JOIN 
		  		entidade.entidade uor ON uor.entid = fea.entid 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid 
		  	LEFT JOIN 
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE 
				cmpinstalacao = 'P' AND 
				ac.cmpexistencia = 'N' AND 
				ac.cmpsituacao = 'F' AND 
				tuo.funid {$funid} 
		  		{$filtro_estuf}
			GROUP BY 
				ac.cmpinstalacao)
				
				UNION ALL

			(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpsituacaoobra || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpsituacaoobra || '\' ); formatarParametros();abreconteudo(\'painel.php?modulo=principal/redefederal/rede_federal&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpsituacaoobra || '&params=\' + params, \'' || ac.cmpsituacaoobra || '\');\"/></center>' as img,
				'Unidades com obras concluídas' as nome,
				coalesce(count(cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpsituacaoobra || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM 
				academico.campus ac
			LEFT JOIN 
				entidade.entidade ent ON ent.entid = ac.entid 
		  	LEFT JOIN 
		  		entidade.endereco edc ON edc.entid = ent.entid 
		  	LEFT JOIN 
		  		territorios.municipio mun ON mun.muncod = edc.muncod 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro
		  	LEFT JOIN 
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid    
		  	LEFT JOIN 
		  		entidade.entidade uor ON uor.entid = fea.entid 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid 
		  	LEFT JOIN 
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE 
				ac.cmpexistencia = 'N' AND 
				ac.cmpsituacaoobra = 'C' AND 
				tuo.funid {$funid} 
		  		{$filtro_estuf}
			GROUP BY 
				ac.cmpsituacaoobra)
			
				UNION ALL
				
			(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpsituacaoobra || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpsituacaoobra || '\' ); formatarParametros();abreconteudo(\'painel.php?modulo=principal/redefederal/rede_federal&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpsituacaoobra || '&params=\' + params, \'' || ac.cmpsituacaoobra || '\');\"/></center>' as img,
				'Unidades com obras em andamento' as nome,
				coalesce(count(cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpsituacaoobra || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM 
				academico.campus ac
			LEFT JOIN 
				entidade.entidade ent ON ent.entid = ac.entid  
		  	LEFT JOIN 
		  		entidade.endereco edc ON edc.entid = ent.entid 
		  	LEFT JOIN 
		  		territorios.municipio mun ON mun.muncod = edc.muncod 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro
		  	LEFT JOIN 
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid    
		  	LEFT JOIN 
		  		entidade.entidade uor ON uor.entid = fea.entid 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid 
		  	LEFT JOIN 
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE 
				ac.cmpexistencia = 'N' AND 
				ac.cmpsituacaoobra = 'A' AND 
				tuo.funid {$funid} 
		  		{$filtro_estuf}
			GROUP BY 
				ac.cmpsituacaoobra)
				
				UNION ALL
				
			(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpsituacaoobra || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpsituacaoobra || '\' ); formatarParametros();abreconteudo(\'painel.php?modulo=principal/redefederal/rede_federal&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpsituacaoobra || '&params=\' + params, \'' || ac.cmpsituacaoobra || '\');\"/></center>' as img,
				'Unidades com obras em licitação' as nome,
				coalesce(count(cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpsituacaoobra || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM 
				academico.campus ac
			LEFT JOIN 
				entidade.entidade ent ON ent.entid = ac.entid 
		  	LEFT JOIN 
		  		entidade.endereco edc ON edc.entid = ent.entid 
		  	LEFT JOIN 
		  		territorios.municipio mun ON mun.muncod = edc.muncod 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro
		  	LEFT JOIN 
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid    
		  	LEFT JOIN 
		  		entidade.entidade uor ON uor.entid = fea.entid 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid 
		  	LEFT JOIN 
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE 
				ac.cmpexistencia = 'N' AND 
				ac.cmpsituacaoobra = 'L' AND 
				tuo.funid {$funid} 
		  		{$filtro_estuf}
			GROUP BY 
				ac.cmpsituacaoobra)
				
				UNION ALL
				
			(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpsituacaoobra || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpsituacaoobra || '\' ); formatarParametros();abreconteudo(\'painel.php?modulo=principal/redefederal/rede_federal&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpsituacaoobra || '&params=\' + params, \'' || ac.cmpsituacaoobra || '\');\"/></center>' as img,
				'Unidades com obras em elaboração de projetos' as nome,
				coalesce(count(cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpsituacaoobra || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM 
				academico.campus ac
			LEFT JOIN 
				entidade.entidade ent ON ent.entid = ac.entid 
		  	LEFT JOIN 
		  		entidade.endereco edc ON edc.entid = ent.entid 
		  	LEFT JOIN 
		  		territorios.municipio mun ON mun.muncod = edc.muncod 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro
		  	LEFT JOIN 
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid    
		  	LEFT JOIN 
		  		entidade.entidade uor ON uor.entid = fea.entid 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid 
		  	LEFT JOIN 
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE 
				ac.cmpexistencia = 'N' AND 
				ac.cmpsituacaoobra = 'E' AND 
				tuo.funid {$funid} 
		  		{$filtro_estuf}
			GROUP BY 
				ac.cmpsituacaoobra)

				UNION ALL
				
			(SELECT
				'<center><img src=\"../imagens/mais.gif\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ac.cmpsituacaoobra || '\" name=\"+\" onclick=\"desabilitarConteudo( \'' || ac.cmpsituacaoobra || '\' ); formatarParametros();abreconteudo(\'painel.php?modulo=principal/redefederal/rede_federal&acao=A&subAcao=gravarCarga&orgid={$orgid}&estuf={$estuf}&cargaexpansao=' || ac.cmpsituacaoobra || '&params=\' + params, \'' || ac.cmpsituacaoobra || '\');\"/></center>' as img,
				'Unidades com obras em dominialidade de imóvel' as nome,
				coalesce(count(cmpid), 0),
				'<tr><td style=\"padding:0px;margin:0;width:0px;\"></td><td id=\"td' || ac.cmpsituacaoobra || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
			FROM 
				academico.campus ac
			LEFT JOIN 
				entidade.entidade ent ON ent.entid = ac.entid 
		  	LEFT JOIN 
		  		entidade.endereco edc ON edc.entid = ent.entid 
		  	LEFT JOIN 
		  		territorios.municipio mun ON mun.muncod = edc.muncod 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen ON fen.entid = ent.entid $funidFiltro
		  	LEFT JOIN 
		  		entidade.funentassoc fea ON fea.fueid = fen.fueid    
		  	LEFT JOIN 
		  		entidade.entidade uor ON uor.entid = fea.entid 
		  	LEFT JOIN 
		  		entidade.funcaoentidade fen2 ON fen2.entid = uor.entid 
		  	LEFT JOIN 
		  		academico.orgaouo tuo ON tuo.funid = fen2.funid
		  	{$join_regiao}
			WHERE 
				ac.cmpexistencia = 'N' AND 
				ac.cmpsituacaoobra = 'D' AND 
				tuo.funid {$funid} 
		  		{$filtro_estuf}
			GROUP BY 
				ac.cmpsituacaoobra)
				";
	$cabecalho = array( "Ação", "Descrição", "Qtd." );
	$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
	
}


function painel_painel_campus_dados_sig( $orgid, $dado, $estuf = '' ){
	
	global $db;
	
	// cria o filtro com o funid
	switch ( $orgid ){
		case '1':
			$funid = 18;
		break;
		case '2':
			$funid = 17;
		break;
	}
	
	// cria o campo do filtro 
	switch ( $dado ){
		case 'D':
			$campo = "ac.cmpexistencia = 'N' AND ac.cmpsituacao = 'F' AND ac.cmpinstalacao";
		break;
		case 'P':
			$campo = "ac.cmpexistencia = 'N' AND ac.cmpsituacao = 'F' AND ac.cmpinstalacao";
		break;
		case 'C':
			$campo = "ac.cmpexistencia = 'N' AND ac.cmpsituacaoobra";
		break;
		case 'A':
			$campo = "ac.cmpexistencia = 'N' AND ac.cmpsituacaoobra";
		break;
		case 'E':
			$campo = "ac.cmpexistencia = 'N' AND ac.cmpsituacaoobra";
		break;
		case 'L':
			$campo = "ac.cmpexistencia = 'N' AND ac.cmpsituacaoobra";
		break;
	}
	
	$join_estuf   = ( $estuf && $estuf != 'todos' ) ? "LEFT JOIN entidade.endereco ed ON ed.entid = ee.entid" : "";
	$filtro_estuf = ( $estuf && $estuf != 'todos' ) ? "AND ed.estuf = '{$estuf}'" : "";
	
	// cria o join e o filtro por região, caso exista
	switch( $estuf ){
		case 'norte':
			$join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = ed.estuf";
			$filtro_estuf = "AND te.regcod = '1'";
		break;
		case 'nordeste':
			$join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = ed.estuf";
			$filtro_estuf = "AND te.regcod = '2'";
		break;
		case 'sudeste':
			$join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = ed.estuf";
			$filtro_estuf = "AND te.regcod = '3'";
		break;
		case 'sul':
			$join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = ed.estuf";
			$filtro_estuf = "AND te.regcod = '4'";
		break;
		case 'centrooeste':
			$join_regiao = "LEFT JOIN territorios.estado te ON te.estuf = ed.estuf";
			$filtro_estuf = "AND te.regcod = '5'";
		break;
		default:
		break;
	}
	
	$sql = "SELECT
				'<a onclick=\"atualiza_div( \'unidade\', \'' || ee2.entid || '\');\" style=\"cursor:pointer;\">' || ee2.entnome || '</a>'as unidade, 
				'&nbsp;&nbsp;&nbsp;&nbsp;<a onclick=\"atualiza_div( \'unidade\', \'' || ee.entid || '\');\" style=\"cursor:pointer;\">' || ee.entnome || '</a>' as campus
			FROM
				entidade.entidade ee
			INNER JOIN
				academico.campus ac ON ac.entid = ee.entid
			INNER JOIN
				entidade.funcaoentidade fe ON ac.entid = fe.entid
			INNER JOIN
				entidade.funentassoc fa ON fa.fueid = fe.fueid
			INNER JOIN
				entidade.entidade ee2 ON ee2.entid = fa.entid
			{$join_estuf} {$join_regiao}
			WHERE
				{$campo} = '{$dado}' AND funid = {$funid}
				{$filtro_estuf}
			ORDER BY 
				unidade, campus";
	
	$db->monta_lista_grupo($sql, $cabecalho, 1000, 5, 'N', 'N', '', '', 'unidade') ;
	
}

?>