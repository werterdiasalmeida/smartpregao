<?php
if($_GET['iframeBalao']){
	// carrega as funções gerais
	include_once "config.inc";
	include_once APPRAIZ . "includes/funcoes.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";

	// carrega as funções específicas do módulo
	include_once '_constantes.php';
	include_once '_funcoes.php';
	include_once '_componentes.php';

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco(); 
	
	$sql = "select 
				mun.*,
				estdescricao,
				( select distinct sum(munpopulacao) from territorios.municipio m3 where m3.estuf = mun.estuf  ) as populacao_est,
				mesdsc,
				( select distinct sum(munpopulacao) from territorios.municipio m1 where m1.mescod = mun.mescod  ) as populacao_mes,
				micdsc,
				( select distinct sum(munpopulacao) from territorios.municipio m2 where m2.miccod = mun.miccod  ) as populacao_mic
			from 
				territorios.municipio mun
			inner join
				territorios.estado e on e.estuf = mun.estuf
			inner join
				 territorios.mesoregiao mes ON mes.mescod = mun.mescod
			inner join
				 territorios.microregiao mic ON mic.miccod = mun.miccod
			where 
				mun.muncod = '{$_GET['muncod']}' ";
	$arrDados = $db->pegaLinha($sql);
	if($arrDados)
		extract($arrDados);
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script>
		function exibeListaMunicipio(muncod){
		window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=municipio&muncod='+muncod,'Indicador','scrollbars=yes,height=700,width=700,status=no,toolbar=no,menubar=no,location=no');
	}
	</script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<div style="font-size:12px;margin-3px;height:250px;overflow:auto" >
	<b>Localização:</b> <span style="cursor:pointer" onclick="window.open('http://www.ibge.gov.br/cidadesat/xtras/perfilwindow.php?nomemun=<?php echo $mundescricao ?>&codmun=<?php echo substr($muncod,0,6) ?>&r=2','IBGE','scrollbars=yes,height=400,width=400,status=no,toolbar=no,menubar=no,location=no')" > <?php echo $mundescricao ?> / <?php echo $estuf ?> </span>
	<br /> <b>População:</b> <?php echo number_format($munpopulacao,0,".",".") ?> (<?php echo round(( $munpopulacao/$populacao_est )*100,2) ?>% de <?php echo $estuf ?>)
	<table style="margin-top:4px" class="listagem" width="95%" cellspacing="0" cellpadding="2" border="0" align="center" >
		<thead>
			<tr><td>Tipo</td><td>População</td><td>% Estado</td></tr>
		</thead>
			<tr><td>Microregião: <?php echo $micdsc ?></td><td><?php echo number_format($populacao_mic,0,".",".") ?></td><td><?php echo round(( $populacao_mic/$populacao_est )*100,2) ?>%</td></tr>
			<tr><td>Mesoregião: <?php echo $mesdsc ?></td><td><?php echo number_format($populacao_mes,0,".",".") ?></td><td><?php echo round(( $populacao_mes/$populacao_est )*100,2) ?>%</td></tr>
			<tr><td>Estado: <?php echo $estdescricao ?></td><td><?php echo number_format($populacao_est,0,".",".") ?></td><td><?php echo round(( $populacao_est/$populacao_est )*100,2) ?>%</td></tr>
	</table>
	<br />
	<table class="tabela" width="95%" cellspacing="0" cellpadding="2" border="0" align="center" >
		<tr><td class="SubtituloEsquerda" ><b>Outras Classificações</b></td></tr>
	</table>
	
	<?php $sql = "select gtmdsc,tpmdsc from 
					territorios.muntipomunicipio  m
				inner join
					territorios.tipomunicipio t ON t.tpmid = m.tpmid
				inner join
					territorios.grupotipomunicipio g ON g.gtmid = t.gtmid
				where 
					muncod = '{$_GET['muncod']}'";
	$cabecalho = array("Grupo","Tipo");
	$db->monta_lista_simples($sql,$cabecalho,10,10,"N","95%","N");?>
	<br />
	</div><br />
	<div style="font-size:12px;cursor:pointer" onclick="exibeListaMunicipio('<?php echo $_GET['muncod'] ?>');" style="cursor:pointer;width:100%;text-align:right">Mais detalhes...</div>
	
	<?php exit;
}

if($_REQUEST['geraXML'] = "true" && $_REQUEST['XMLindid'] && $_REQUEST['XMLsehid']){
	header('content-type: text/html; charset=ISO-8859-1');
	geraXML2($_REQUEST['XMLindid'],$_REQUEST['XMLsehid']);
	exit;
}

if($_REQUEST['geraXML'] = "true" && $_REQUEST['muncod']){
	header('content-type: text/html; charset=ISO-8859-1');
	geraXML3($_REQUEST['muncod']);
	exit;
}

if($_REQUEST['BuscaMunicipioAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	buscaMunicipioAjax2($_REQUEST['BuscaMunicipioAjax']);
	exit;
}


function geraXML3($muncod){
	// carrega as funções gerais
	include_once "config.inc";
	include_once APPRAIZ . "includes/funcoes.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";

	// carrega as funções específicas do módulo
	include_once '_constantes.php';
	include_once '_funcoes.php';
	include_once '_componentes.php';

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();
	
	$sql = "select
						mun.muncod,
						mun.mundescricao,
						mun.estuf,
						mun.munpopulacao,
							 --############### LATITUDE ###################### --
						CASE WHEN (SPLIT_PART(munmedlat, '.', 1) <>'' AND SPLIT_PART(munmedlat, '.', 2) <>'' AND split_part(munmedlat, '.', 3) <>'') THEN
			               CASE WHEN split_part(munmedlat, '.', 4) <>'N' THEN
			                   (((split_part(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int)))*(-1)
			                ELSE
			                   ((SPLIT_PART(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int))
			               END
			            ELSE
			            -- Valores do IBGE convertidos em  decimal
			            CASE WHEN (length (munmedlat)=8) THEN
			                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
			                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
			                END
			            ELSE
			                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
			                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
			                END
			            END
			            END as latitude,
			            --############### FIM LATITUDE ###################### --

			            --############### LONGITUDE ###################### --
			            CASE WHEN (SPLIT_PART(munmedlog, '.', 1) <>'' AND SPLIT_PART(munmedlog, '.', 2) <>'' AND split_part(munmedlog, '.', 3) <>'') THEN
			               ((split_part(munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlog, '.', 1)::int))*(-1)
			            ELSE
			                -- Valores do IBGE convertidos em  decimal
			               (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1)
			            END as longitude
			            --############### FIM LONGITUDE ###################### --


            from
            	territorios.municipiosvizinhos viz
            inner join
            	territorios.municipio mun ON mun.muncod = viz.muncodvizinho 
            where
            	viz.muncod = '$muncod'
            group by
            	mun.muncod,
				mun.mundescricao,
				mun.estuf,
				munmedlog,
				munmedlat,
				mun.munpopulacao";
				
			$sql2 = "select
						mun.muncod,
						mun.mundescricao,
						mun.estuf,
						mun.munpopulacao,
							 --############### LATITUDE ###################### --
						CASE WHEN (SPLIT_PART(munmedlat, '.', 1) <>'' AND SPLIT_PART(munmedlat, '.', 2) <>'' AND split_part(munmedlat, '.', 3) <>'') THEN
			               CASE WHEN split_part(munmedlat, '.', 4) <>'N' THEN
			                   (((split_part(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int)))*(-1)
			                ELSE
			                   ((SPLIT_PART(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int))
			               END
			            ELSE
			            -- Valores do IBGE convertidos em  decimal
			            CASE WHEN (length (munmedlat)=8) THEN
			                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
			                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
			                END
			            ELSE
			                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
			                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
			                END
			            END
			            END as latitude,
			            --############### FIM LATITUDE ###################### --

			            --############### LONGITUDE ###################### --
			            CASE WHEN (SPLIT_PART(munmedlog, '.', 1) <>'' AND SPLIT_PART(munmedlog, '.', 2) <>'' AND split_part(munmedlog, '.', 3) <>'') THEN
			               ((split_part(munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlog, '.', 1)::int))*(-1)
			            ELSE
			                -- Valores do IBGE convertidos em  decimal
			               (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1)
			            END as longitude
			            --############### FIM LONGITUDE ###################### --


            from
            	territorios.municipio mun
            where
            	mun.muncod = '$muncod'
            group by
            	mun.muncod,
				mun.mundescricao,
				mun.estuf,
				mun.munpopulacao,
				munmedlog,
				munmedlat";
			
			$municipios = $db->carregar($sql);
			$municipio = $db->carregar($sql2);
			
			if($municipios):
			
				array_push($municipios,$municipio[0]);
			
				$conteudo .= "<markers> "; // inicia o XML
				
				foreach($municipios as $mun):
												
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "muncod='{$mun['muncod']}' "; //adiciona o código do município;
					$conteudo .= "mundsc=\"". $mun['mundescricao'] ."\" "; // adiciona a descrição do município;
					$conteudo .= "estuf='{$mun['estuf']}' "; //adiciona o estado;
					$conteudo .= "lat='{$mun['latitude']}' "; // adiciona a latitude;
					$conteudo .= "lng='{$mun['longitude']}' "; //adiciona a longitude;
					$conteudo .= "tipo='{$mun['longitude']}' "; //adiciona a longitude;
					$conteudo .= "populacao='".number_format($mun['munpopulacao'],0,".",".")."' "; //adiciona a longitude;
					$conteudo .= "percent_estado='{$mun['longitude']}' "; //adiciona a longitude;
					$conteudo .= "grupo='{$mun['longitude']}' "; //adiciona a longitude;
					$conteudo .= "tipo_grupo='{$mun['longitude']}' "; //adiciona a longitude;
					$conteudo .= "muncodcompleto='{$mun['muncodcompleto']}' "; //adiciona a longitude;
					$conteudo .= "/> ";
				
				endforeach;
				
				$conteudo .= "</markers> ";
				print $conteudo;
			endif;
}


function geraXML2($indid,$sehid,$muncod = null){
	// carrega as funções gerais
	include_once "config.inc";
	include_once APPRAIZ . "includes/funcoes.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";

	// carrega as funções específicas do módulo
	include_once '_constantes.php';
	include_once '_funcoes.php';
	include_once '_componentes.php';

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();
	
	$sql = "select
				regid
			from
				painel.indicador
			where
				indid = $indid";
	
	$regid = $db->pegaUm($sql);
	
	switch($regid){
		case 1:
			//sem mapa
			$sqlMun ="	inner join
			            	painel.detalheseriehistorica det ON mun.muncod = det.dshcodmunicipio
			            inner join
			            	painel.seriehistorica seh ON seh.sehid = det.sehid ";
			break;
		case 2:
			$sqlMun = "	inner join
			            	painel.escola esc ON mun.muncod = esc.escmuncod
			            inner join
			            	painel.detalheseriehistorica det ON esc.esccodinep::varchar(20) = det.dshcod
			            inner join
			            	painel.seriehistorica seh ON seh.sehid = det.sehid";
			break;
		case 3:
			//sem mapa
			$sqlMun ="	inner join
			            	painel.detalheseriehistorica det ON mun.muncod = det.dshcodmunicipio
			            inner join
			            	painel.seriehistorica seh ON seh.sehid = det.sehid ";
			break;
		case 4:
			$sqlMun = "	inner join
			            	painel.detalheseriehistorica det ON mun.muncod = det.dshcodmunicipio
			            inner join
			            	painel.seriehistorica seh ON seh.sehid = det.sehid ";
			break;
		case 5:
			$sqlMun = "	inner join
			            	painel.ies ies ON mun.muncod = ies.iesmuncod
			            inner join
			            	painel.detalheseriehistorica det ON ies.iesid::varchar(20) = det.dshcod
			            inner join
			            	painel.seriehistorica seh ON seh.sehid = det.sehid ";
			break;
		case 6:
			//estado, pegar capital
			$sqlMun ="	inner join
							territorios.estado est ON est.muncodcapital = mun.muncod
						inner join
			            	painel.detalheseriehistorica det ON det.dshuf = est.estuf
			            inner join
			            	painel.seriehistorica seh ON seh.sehid = det.sehid ";
			break;
		default:
			//sem mapa
			$sqlMun ="	inner join
			            	painel.detalheseriehistorica det ON mun.muncod = det.dshcodmunicipio
			            inner join
			            	painel.seriehistorica seh ON seh.sehid = det.sehid ";
			break;
	}
	
	
	$sql = "select
						mun.muncod,
						mun.mundescricao,
						mun.estuf,
							 --############### LATITUDE ###################### --
						CASE WHEN (SPLIT_PART(munmedlat, '.', 1) <>'' AND SPLIT_PART(munmedlat, '.', 2) <>'' AND split_part(munmedlat, '.', 3) <>'') THEN
			               CASE WHEN split_part(munmedlat, '.', 4) <>'N' THEN
			                   (((split_part(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int)))*(-1)
			                ELSE
			                   ((SPLIT_PART(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int))
			               END
			            ELSE
			            -- Valores do IBGE convertidos em  decimal
			            CASE WHEN (length (munmedlat)=8) THEN
			                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
			                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
			                END
			            ELSE
			                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
			                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
			                ELSE
			                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
			                END
			            END
			            END as latitude,
			            --############### FIM LATITUDE ###################### --

			            --############### LONGITUDE ###################### --
			            CASE WHEN (SPLIT_PART(munmedlog, '.', 1) <>'' AND SPLIT_PART(munmedlog, '.', 2) <>'' AND split_part(munmedlog, '.', 3) <>'') THEN
			               ((split_part(munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlog, '.', 1)::int))*(-1)
			            ELSE
			                -- Valores do IBGE convertidos em  decimal
			               (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1)
			            END as longitude
			            --############### FIM LONGITUDE ###################### --


            from
            	territorios.municipio mun
            $sqlMun
            where
            	seh.sehid = $sehid
            group by
            	mun.muncod,
				mun.mundescricao,
				mun.estuf,
				munmedlog,
				munmedlat";
			$municipios = array();
			$municipios = $db->carregar($sql);
	
			$sql = "select
						umedesc
					from
						painel.unidademeta unm
					inner join
						painel.indicador ind ON ind.umeid = unm.umeid
					where
						ind.indid = $indid";
			$umedesc = $db->pegaUm($sql);
			!$umedesc? $umedesc = "N/A" : $umedesc = $umedesc;

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
					$SQLsehqtde = "coalesce(dshqtde,0)::integer as dshqtde ";
					break;
				case "Percentual":
					$SQLsehqtde = "coalesce(dshqtde,0) || '%' as dshqtde ";
					break;
				case "Razão":
					$SQLsehqtde = "coalesce(dshqtde,0) as dshqtde ";
					break;
				case "Número índice":
					$SQLsehqtde = "coalesce(dshqtde,0) as dshqtde ";
					break;
				default:
					$SQLsehqtde = "coalesce(dshqtde,0) as dshqtde ";
					break;
			}

			if($municipios):
			
				$conteudo .= "<markers> "; // inicia o XML
				
				foreach($municipios as $mun):
				
				switch($regid){
						case 1:
							//sem mapa
							$sqlMun ="	from
							            	painel.detalheseriehistorica det
							            inner join
							            	painel.seriehistorica seh ON seh.sehid = det.sehid 
							            where
											det.dshcodmunicipio = '{$mun['muncod']}'";
							break;
						case 2:
							$sqlMun = "	from
							            	painel.escola esc
							            inner join
							            	painel.detalheseriehistorica det ON esc.esccodinep::varchar(20) = det.dshcod
							            inner join
							            	painel.seriehistorica seh ON seh.sehid = det.sehid
							            where
											esc.escmuncod = '{$mun['muncod']}'";
							break;
						case 3:
							//sem mapa
							$sqlMun ="	from
							            	painel.detalheseriehistorica det
							            inner join
							            	painel.seriehistorica seh ON seh.sehid = det.sehid 
							            where
											det.dshcodmunicipio = '{$mun['muncod']}'";
							break;
						case 4:
							$sqlMun = "	from
							            	painel.detalheseriehistorica det
							            inner join
							            	painel.seriehistorica seh ON seh.sehid = det.sehid 
							            where
											det.dshcodmunicipio = '{$mun['muncod']}'";
							break;
						case 5:
							$sqlMun = "	from
							            	painel.ies ies
							            inner join
							            	painel.detalheseriehistorica det ON ies.iesid::varchar(20) = det.dshcod
							            inner join
							            	painel.seriehistorica seh ON seh.sehid = det.sehid 
							            where
							            	ies.iesmuncod = '{$mun['muncod']}'";
							break;
						case 6:
							//estado, pegar capital
							$sqlMun ="	from
							            	painel.detalheseriehistorica det 
							            inner join
							            	territorios.estado est ON est.estuf = det.dshuf
							            inner join
							            	painel.seriehistorica seh ON seh.sehid = det.sehid 
							            where
											est.muncodcapital = '{$mun['muncod']}'";
							break;
						default:
							//sem mapa
							$sqlMun ="	from
							            	painel.detalheseriehistorica det
							            inner join
							            	painel.seriehistorica seh ON seh.sehid = det.sehid 
							            where
											det.dshcodmunicipio = '{$mun['muncod']}'";
							break;
					}
				
					$sql = "select
									$SQLsehqtde,
									coalesce(dshvalor,0) as dshvalor
								$sqlMun
								and
									seh.sehid = $sehid";
						
			
					$dados = $db->pegaLinha($sql);
					$qtd = $dados['dshqtde'];
					$valor = $dados['dshvalor'];
				
					$conteudo .= "<marker "; //inicia um ponto no mapa
					$conteudo .= "muncod='{$mun['muncod']}' "; //adiciona o código do município;
					$conteudo .= "mundsc=\"". $mun['mundescricao'] ."\" "; // adiciona a descrição do município;
					$conteudo .= "umedesc=\"". $umedesc ."\" "; // adiciona a unidade meta;
					$conteudo .= "estuf='{$mun['estuf']}' "; //adiciona o estado;
					$conteudo .= "valor='$valor' "; // adiciona o valor;
					$conteudo .= "qtde='$qtd' "; //adiciona a quantidade
					$conteudo .= "lat='{$mun['latitude']}' "; // adiciona a latitude;
					$conteudo .= "lng='{$mun['longitude']}' "; //adiciona a longitude;
					$conteudo .= "tipo='{$mun['longitude']}' "; //adiciona a longitude;
					$conteudo .= "populacao='{$mun['longitude']}' "; //adiciona a longitude;
					$conteudo .= "percent_estado='{$mun['longitude']}' "; //adiciona a longitude;
					$conteudo .= "grupo='{$mun['longitude']}' "; //adiciona a longitude;
					$conteudo .= "tipo_grupo='{$mun['longitude']}' "; //adiciona a longitude;
					
					$conteudo .= "/> ";
				
				endforeach;
				
				$conteudo .= "</markers> ";
				print $conteudo;
			endif;
}


function buscaMunicipioAjax2($dsc){
	// carrega as funções gerais
	include_once "config.inc";
	include_once APPRAIZ . "includes/funcoes.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";

	// carrega as funções específicas do módulo
	include_once '_constantes.php';
	include_once '_funcoes.php';
	include_once '_componentes.php';

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();

	$arrCaracteres = array(",",".","-",":","_","/");

	foreach($arrCaracteres as $crt){
		if(strstr($dsc, $crt)){
			$uf = strstr($dsc, $crt);
			$part = explode($crt,$dsc);
			$mun = trim($part[0]);
			$mun = str_replace($crt,'',$mun);
			$mun = trim($mun);
			$uf = str_replace($crt,'',$uf);
			$uf = trim($uf);
			break;
		}

	}

	if(!$mun){
			$mun = $dsc;
	}

	if($mun){
			$sqlMUN = "where
        				mun.mundescricao ilike '%".utf8_decode($mun)."%'";
	}

	if(!$uf){
			$sqlUF = "";
	}else{
			$sqlUF = "and
        				mun.estuf like '%".strtoupper(utf8_decode($uf))."%'";
	}

	$sql = "select
				mun.muncod,
				mun.mundescricao,
				mun.estuf,
					 --############### LATITUDE ###################### --
				CASE WHEN (SPLIT_PART(munmedlat, '.', 1) <>'' AND SPLIT_PART(munmedlat, '.', 2) <>'' AND split_part(munmedlat, '.', 3) <>'') THEN
	               CASE WHEN split_part(munmedlat, '.', 4) <>'N' THEN
	                   (((split_part(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int)))*(-1)
	                ELSE
	                   ((SPLIT_PART(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int))
	               END
	            ELSE
	            -- Valores do IBGE convertidos em  decimal
	            CASE WHEN (length (munmedlat)=8) THEN
	                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
	                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
	                ELSE
	                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
	                END
	            ELSE
	                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
	                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
	                ELSE
	                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
	                END
	            END
	            END as latitude,
	            --############### FIM LATITUDE ###################### --

	            --############### LONGITUDE ###################### --
	            CASE WHEN (SPLIT_PART(munmedlog, '.', 1) <>'' AND SPLIT_PART(munmedlog, '.', 2) <>'' AND split_part(munmedlog, '.', 3) <>'') THEN
	               ((split_part(munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlog, '.', 1)::int))*(-1)
	            ELSE
	                -- Valores do IBGE convertidos em  decimal
	               (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1)
	            END as longitude
	            --############### FIM LONGITUDE ###################### --


		from
            territorios.municipio mun
        $sqlMUN
        $sqlUF
        group by
			estuf,
			muncod,
			mundescricao,
			latitude,
			longitude
		order by
			estuf";

        $municipios = $db->carregar($sql);

        if(!$municipios){
        	echo "Nenhum resultado encontrado.";
        	return false;
        }

        foreach($municipios as $mun){
        	$estados[] = $mun['estuf'];
        }
        $estados = array_unique($estados);

        $numUF = count($estados);
        $numUF = ($numUF / 3) ;
        $numUF = (int)$numUF;

        if($_GET['redefederal']){
        	echo "<div style=\"max-height:350px;overflow:auto;\" >";
        	echo "<div style=\"width:200px\">";
        }else{
        	echo "<div style=\"max-height:150px;overflow:auto;\" >";
        	echo "<div style=\"float:left;width:32%\">";	
        }
        
        $totalUF = 0;

        foreach($estados as $uf){
        		echo "<fieldset><legend>".$uf."</legend>";
        	foreach($municipios as $mun){
        		if($uf == $mun['estuf']){
        			echo "<span style=\"cursor:pointer\" onclick=\"localizaMapa2('{$mun['muncod']}',{$mun['longitude']},{$mun['latitude']})\" >".$mun['mundescricao']."</span><br>";
        		}
        	}
        	echo "</fieldset>";
        	if($totalUF == $numUF){
        		 	if($_GET['redefederal']){
        		 		echo "</div><div style=\"width:200px\">";
        		 	}else{
        		 		echo "</div><div style=\"float:left;width:32%\">";	
        		 	}
        			$numUF = $numUF * 2;
        	}
        	$totalUF++;
        }
        echo "</fieldset></div>";
	exit;
}

if($_REQUEST['monta_mapa']){
	// carrega as funções gerais
	include_once "config.inc";
	include_once APPRAIZ . "includes/funcoes.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";


	// carrega as funções específicas do módulo
	include_once '_constantes.php';
	include_once '_funcoes.php';
	include_once '_componentes.php';


	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();
}

//Funções utilizadas na montagem dos mapas dos indicadores (Google Maps)
!$_REQUEST['indid']? $_REQUEST['indid'] = 1 : $_REQUEST['indid'] = $_REQUEST['indid'];
!$_REQUEST['sehid']? $_REQUEST['sehid'] = 1 : $_REQUEST['sehid'] = $_REQUEST['sehid'];

if($_REQUEST['indid']){

	$sql = "select
				indnome
			from
				painel.indicador
			where
				indid = {$_REQUEST['indid']}";
	$indicador = $db->pegaLinha($sql);

}

//Função para aplicar a chave do Google Maps
function curPageURL2() {
		 $pageURL = 'http';
		 if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
		 $pageURL .= "://";
		 if ($_SERVER["SERVER_PORT"] != "80") {
		  $pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
		 } else {
		  $pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
		 }
		 return $pageURL;
		}
?>

<?
$url = explode("/",curPageURL2());
?>
<?if ($url[2]=="simec.mec.gov.br" ){ ?>
    <script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBQhVwj8ALbvbyVgNcB-R-H_S2MIRxSRw07TtkPv50s-khCgCjw1KhcuSw" type="text/javascript"></script>
  <? } ?>
  <?if ($url[2]=="simec-d.mec.gov.br"){ ?>
  	<script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBRYtD8tuHxswJ_J7IRZlgTxP-EUtxRD3aBmpKp7QQhM-oKEpi_q_Z6nzQ" type="text/javascript"></script>
<? } ?>
<?if ($url[2]=="simec" ){ ?>
    <script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBTNzTBk8zukZFuO3BxF29LAEN1D1xRrpthpVw0AZ6npV05I8JLIRtHtyQ" type="text/javascript"></script>
  <? } ?>
  <?if ($url[2]=="simec-d"){ ?>
  	<script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAXKOUFW3PNxuPE30S17ocgBTFm3qU4CVFuo3gZaqihEzC-0jfaRSyt8kdLeiWuocejyXXgeTtRztdYQ" type="text/javascript"></script>
<? } ?>
<?if ($url[2]=="simec-local"){ ?>
<script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAiAikFUnGncRW1bW0qcdmBxRzjpIxsx3o6RYEdxEmCzeJMTc4zBRASslYk1IYbFjzsDflWVzHgfQ3gQ" type="text/javascript"></script>
<? } ?>
<?if ($url[2]=="painel.mec.gov.br"){ ?>
<script src="http://maps.google.com/maps?file=api&v=2&amp;key=ABQIAAAAiAikFUnGncRW1bW0qcdmBxTPkFYZwQy2nvpGvFj08HQmPOt9ZBSYNzf3Heg2gr_T0pQ4EmDbxEhaJw" type="text/javascript"></script>
<? } ?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<link href="../includes/Estilo.css" type="text/css" rel="stylesheet"/>
<link href="../includes/listagem.css" type="text/css" rel="stylesheet"/>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

 function inicialytebox2(url) {
	var anchor = this.document.createElement('a'); // cria um elemento de link
    anchor.setAttribute('rev', 'width: 800px; height: 600px; scrolling: auto;');
    anchor.setAttribute('title', '');
    anchor.setAttribute('href', url);
    anchor.setAttribute('rel', 'lyteframe');
    // paramentros para do lytebox //
    myLytebox.maxOpacity = 50;
    myLytebox.outerBorder = true;
    myLytebox.doAnimations = false;
    myLytebox.start(anchor, false, true);
    return false;
}

	var map; //variável do Google Maps
    
	function initialize2() {
		if (GBrowserIsCompatible()) { // verifica se o navegador é compatível
			
			map = new GMap2(document.getElementById("mapa_indicadores"));
			map.setCenter(new GLatLng(37.4419, -122.1419), 13);
		    <?if(!$_REQUEST['monta_mapa']){ ?>
				GEvent.addListener(map, "click", function() {
					inicialytebox('_funcoes_mapa_painel_controle.php?monta_mapa=true');
				});
			<?}else {?>
				// Controles
				map.addControl(new GMapTypeControl());
				map.addControl(new GLargeMapControl3D());
		        map.addControl(new GOverviewMapControl());
		        map.enableScrollWheelZoom();
		        map.addMapType(G_PHYSICAL_MAP);
		    <?}?>
			// Valor inicial do mapa e zoom
			<?if($_REQUEST['monta_mapa']){ ?>
				var zoom = 5;
				var title = 'Clique para ver os detalhes.';
			<?}else{ ?>
				var zoom = 4;
				var title = '';
			<?} ?>
			var lat_i = -14.689881; var lng_i = -52.373047;
			map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom));
		} // fim do if do JS

		// Preenchendo o mapa
		<? if ($_REQUEST['indid'] && $_REQUEST['sehid']){ ?>
			// Criando o ícone do mapa
			var baseIcon = new GIcon();
			baseIcon.iconSize = new GSize(9, 14);
			baseIcon.iconAnchor = new GPoint(9, 14);
			baseIcon.infoWindowAnchor = new GPoint(9, 2);

			// Criando os Marcadores com o resultado
	        baseIcon.image='/imagens/icone_capacete_2.png';

	        var icon = baseIcon;
	        
	        //Cria o XML para preencher o mapa
	        xml_filtro = '_funcoes_mapa_painel_controle.php?geraXML=true&XMLsehid=<?=$_REQUEST['sehid'];?>&XMLindid=<?=$_REQUEST['indid'];?>';
	        
	        
	        // Criando os Marcadores com o resultado
			GDownloadUrl(xml_filtro, function(data) {
				var xml = GXml.parse(data);
				
				var markers = xml.documentElement.getElementsByTagName("marker");
				
				
				if(markers.length > 0) {
					var lat_ant=0;
					var lng_ant=0;
				
					for (var i = 0; i < markers.length; i++) {
					
						var muncod = markers[i].getAttribute("muncod");
						var mundsc = markers[i].getAttribute("mundsc");
						title = "Município de " + mundsc + ". Clique para ver os detalhes"; 
						var umedesc = markers[i].getAttribute("umedesc");
						var estuf = markers[i].getAttribute("estuf");
						var valor = markers[i].getAttribute("valor");
						var qtde = markers[i].getAttribute("qtde");
						var lat = markers[i].getAttribute("lat");
						var lng = markers[i].getAttribute("lng");
						
						var html = '<div style=\"font-family:verdana;font-size:11px;\" ><b>Localização:</b> ' + mundsc + ' / ' + estuf +' <br><br><table width=100% style=\"font-family:verdana;font-size:11px;\" ><tr bgcolor=\'#f5f5f5\' ><td><b>Unidade Meta</b></td><td><b>Qtd.</b></td><td><b>Valor</b></td></tr><tr><td>' + umedesc + '</td><td>' + qtde + '</td><td>' + valor + '</td></tr></table><br><br><div onclick=\"janela(\'painel.php?modulo=relatorio/relatorioMunicipio&acao=R&muncod=' + muncod + '\',700,500)\" style=\"cursor:pointer;width:100%;text-align:right\">Mais detalhes...</div></div>';
										
						// Verifica pontos em um mesmo lugar e move o seguinte para a direita
						if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
							var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
						else
							var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
			
						lat_ant=markers[i].getAttribute("lat");
						lng_ant=markers[i].getAttribute("lng");
												
						// Cria o marcador na tela
						var marker = createMarker(point,title,icon,html);
						map.addOverlay(marker);
				
		        }
		        setTimeout("disponibilizaMapa2()",5000);
				}else{
					disponibilizaMapa2();
					}
	        });
     } // fim da função initialize
	 
	<?}?>

	function createMarker2(posn, title, icon, html,muncod) {
      var marker = new GMarker(posn, {title: title, icon: icon, draggable:false });
      if(html != false){
	      GEvent.addListener(marker, "click", function() {
	      	marker.openInfoWindowHtml(html);
	       });
       }
      return marker;
    }

    function disponibilizaMapa2(){
    	if(document.getElementById('carregando_mapas')){
    		document.getElementById('carregando_mapas').style.display = 'none';
    	}
    }
    
    function carregandoMapa2(){
    	if(document.getElementById('carregando_mapas')){
    		document.getElementById('carregando_mapas').innerHTML = '<img src="../imagens/wait.gif" /> Carregando...';
    	}
    }
    

    function montaMapa2(){
		if(document.getElementById("mapa_indicadores")){
			initialize2();
		}
	}
	function ExibeMapa2(indid,sehid){
		inicialytebox2('_funcoes_mapa_indicadores.php?monta_mapa=true&indid=' + indid + '&sehid=' + sehid);
	}

	function localizaMapa2(muncod,latitude,longitude){
		map.setCenter(new GLatLng(longitude,latitude), parseInt(10));
		marcaMunicipios2(muncod);

	}
	
	function BuscaMunicipioEnter2(e)
	{
	    if (e.keyCode == 13)
	    {
	        BuscaMunicipio2();
	    }
	}

	function BuscaMunicipio2(){
		var mun = document.getElementById('busca_municipio');

		if(!mun.value){
			alert('Digite o Município para busca.');
			return false;
		}

		if(mun.value){
			document.getElementById('resultado_pesquisa').innerHTML = "<center>Carregando...</center>";
			var req = new Ajax.Request('_funcoes_mapa_painel_controle.php?', {
							        method:     'post',
							        parameters: 'BuscaMunicipioAjax=' + mun.value,
							        onComplete: function (res)
							        {
										document.getElementById('resultado_pesquisa').innerHTML = res.responseText;
							        }
							  });

		}
	}
	
	function marcaMunicipios2(muncod2){
		
		// Criando o ícone do mapa
		var baseIcon = new GIcon();
		baseIcon.iconSize = new GSize(9, 14);
		baseIcon.iconAnchor = new GPoint(9, 14);
		baseIcon.infoWindowAnchor = new GPoint(9, 2);

		// Criando os Marcadores com o resultado
        baseIcon.image='/imagens/icone_capacete_2.png';
        var icon = baseIcon;
         
		xml_filtro = '_funcoes_mapa_painel_controle.php?geraXML=true&muncod=' + muncod2;
		
		// Criando os Marcadores com o resultado
			GDownloadUrl(xml_filtro, function(data) {
				var xml = GXml.parse(data);
				
				var markers = xml.documentElement.getElementsByTagName("marker");
				
				
				if(markers.length > 0) {
					var lat_ant=0;
					var lng_ant=0;
				
					for (var i = 0; i < markers.length; i++) {
						
						var muncod = markers[i].getAttribute("muncod");
						if(muncod == muncod2){
							var m = i; 
						}
						var mundsc = markers[i].getAttribute("mundsc");
						title = "Município de " + mundsc + ". Clique para ver os detalhes"; 
						var estuf = markers[i].getAttribute("estuf");
						var lat = markers[i].getAttribute("lat");
						var lng = markers[i].getAttribute("lng");
						
						var populacao = markers[i].getAttribute("populacao");
						var tipo = markers[i].getAttribute("tipo");
						var grupo = markers[i].getAttribute("grupo");
						var tipoGrupo = markers[i].getAttribute("tipo_grupo");
						var percent_estado = markers[i].getAttribute("percent_estado");
						var muncodcompleto = markers[i].getAttribute("muncodcompleto");
					
						var html = "<iframe style=\"margin:0px;border:0px\" width=\"300\" height=\"300\" src=\"_funcoes_mapa_painel_controle.php?iframeBalao=1&muncod=" + muncod + "\" border=\"0\" ></iframe>";
						
						// Verifica pontos em um mesmo lugar e move o seguinte para a direita
						if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng")){
							var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
						}else{
							var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
						}
						
						lat_ant=markers[i].getAttribute("lat");
						lng_ant=markers[i].getAttribute("lng");
						
						// Cria o marcador na tela
						var marker = createMarker2(point,title,icon,html);
						map.addOverlay(marker);
						marker.openInfoWindowHtml(html);
						
		        }
		        setTimeout("disponibilizaMapa2()",5000);
				}else{
					disponibilizaMapa2();
					}
	        });
	}
	
	function exibeListaMunicipio(muncod){
		window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=municipio&muncod='+muncod,'Indicador','scrollbars=yes,height=700,width=700,status=no,toolbar=no,menubar=no,location=no');
	}

    </script>
<?if($_REQUEST['monta_mapa'] && $_REQUEST['indid']){
	?>
<body>
<?
monta_titulo( $indicador['indnome'], ' ' );

?>
<div id="pesquisa" style="margin-bottom:15px;">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
    <td align='right' width=65% class="SubTituloDireita">Buscar Município:</td>
    <td>
    	<?=campo_texto('busca_municipio','N','S',30,30,'','','','','','','id="busca_municipio"','BuscaMunicipioEnter2(event);') ?>
    	 <input type="button" value="Buscar" onclick="BuscaMunicipio2();">
    </td>
</tr>
<tr>
	<td colspan=2>
		<div id="resultado_pesquisa">
		</div>
	</td>
</tr>
</table>
</div>
<?
/*select replace(
       substring(to_char(123456789.012, '999,999,999.99'), 1, position('.' in to_char(123456789.012, '999,999,999.99'))-1)
       , ',', '.')
       || ',' ||
       substring(to_char(123456789.012, '999,999,999.99'), position('.' in to_char(123456789.012, '999,999,999.99'))+1)
 http://rogeriolino.wordpress.com/2006/12/19/javascript-settimeout-e-setinterval/
 * */
  ?>


<div id="carregando_mapas" style="position:absolute;top:50%;left:30%;z-index:999999;width:300px;text-align:center;background-color:#FFFFFF;border:#000000 solid 1px;padding:5px;"  ><img src="../imagens/wait.gif" /> Carregando mapa...</div>
<div id="mapa_indicadores" style="width:100%;height:95%"  ></div>

</body>
<script type="text/javascript">
	montaMapa2();
	
	<?php if($_REQUEST['municipio']){ ?>
	
		<?php  
		$sql = "select
							mun.muncod,
							mun.mundescricao,
							mun.estuf,
								 --############### LATITUDE ###################### --
							CASE WHEN (SPLIT_PART(munmedlat, '.', 1) <>'' AND SPLIT_PART(munmedlat, '.', 2) <>'' AND split_part(munmedlat, '.', 3) <>'') THEN
				               CASE WHEN split_part(munmedlat, '.', 4) <>'N' THEN
				                   (((split_part(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int)))*(-1)
				                ELSE
				                   ((SPLIT_PART(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int))
				               END
				            ELSE
				            -- Valores do IBGE convertidos em  decimal
				            CASE WHEN (length (munmedlat)=8) THEN
				                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
				                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
				                ELSE
				                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
				                END
				            ELSE
				                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
				                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
				                ELSE
				                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
				                END
				            END
				            END as latitude,
				            --############### FIM LATITUDE ###################### --
	
				            --############### LONGITUDE ###################### --
				            CASE WHEN (SPLIT_PART(munmedlog, '.', 1) <>'' AND SPLIT_PART(munmedlog, '.', 2) <>'' AND split_part(munmedlog, '.', 3) <>'') THEN
				               ((split_part(munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlog, '.', 1)::int))*(-1)
				            ELSE
				                -- Valores do IBGE convertidos em  decimal
				               (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1)
				            END as longitude
				            --############### FIM LONGITUDE ###################### --
	
	
	            from
	            	territorios.municipio mun
	            where
	            	mun.muncod = '{$_REQUEST['municipio']}'
	            group by
	            	mun.muncod,
					mun.mundescricao,
					mun.estuf,
					munmedlog,
					munmedlat";
			$municipio = $db->pegaLinha($sql);
		?>
		localizaMapa2('<?=$_REQUEST['municipio']?>',<?=$municipio['latitude']?>,<?=$municipio['longitude']?>)
		//alert('<?=$municipio['latitude']?> <?=$municipio['longitude']?>');
	<?php } ?>
</script>


<?}?>