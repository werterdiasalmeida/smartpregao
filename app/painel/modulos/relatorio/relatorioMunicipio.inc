<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
ini_set("memory_limit", "1024M");

include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setTotNivel(false);
$r->setTolizadorLinha(false);
$r->setBrasao(true);
echo $r->getRelatorio();

function monta_sql(){
		
	$sql = "select
				eix.exodsc,
				aca.acadsc,
				ind.indnome,
				mun.mundsc || ' / ' || mun.regcod as mundsc,
				coalesce(ume.umedesc,'N/A') as umedesc,
				coalesce(seh.sehqtde,0) as sehqtde,
				coalesce(seh.sehvalor,0) as sehvalor
			from
				painel.indicador ind
			inner join
				painel.acao aca ON aca.acaid = ind.acaid
			inner join
				painel.eixo eix ON eix.exoid = ind.exoid
			left join
				painel.unidademeta ume ON ume.umeid = ind.umeid
			inner join
				painel.seriehistorica seh ON seh.indid = ind.indid
			left join
				painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
			inner join
				painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			inner join
				public.municipio mun ON dsh.dshcodmunicipio = mun.muncod
			where
				mun.muncod = '{$_GET['muncod']}'
			and
				indstatus <> 'I'
			order by
				dpe.dpedatainicio desc,
				seh.sehdtcoleta desc
				
					
	";
	return $sql;
	
}

function monta_agp(){
	
	$agrupador = array("mundsc","exodsc","acadsc","indnome");
	
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"umedesc",
											"sehqtde",
											"sehvalor"			   		
										  )	  
				);
	
	foreach ($agrupador as $val): 
		switch ($val) {
		    case 'exodsc':
				array_push($agp['agrupador'], array(
													"campo" => "exodsc",
											  		"label" => "Eixo")										
									   				);				
		    	continue;
		        break;
		     case 'acadsc':
				array_push($agp['agrupador'], array(
													"campo" => "acadsc",
											 		"label" => "Ação")										
									   				);					
		    	continue;			
		        break;
		    case 'indnome':
				array_push($agp['agrupador'], array(
													"campo" => "indnome",
													"label" => "Indicador")										
											   		);	
				continue;
				break;
			 case 'mundsc':
				array_push($agp['agrupador'], array(
													"campo" => "mundsc",
													"label" => "Município")										
											   		);	
				continue;
				break;
		}
	endforeach;
	return $agp;
}

function monta_coluna(){
	$coluna    = array(
					array(
						  "campo" 	 => "umedesc",
				   		  "label" 	 => "Unidade Meta",
				   		  "type"	 => "string"
					),	
					array(
						  "campo" 	 => "sehqtde",
				   		  "label" 	 => "Quantidade",
				   		  "type"	 => "numeric"
					),
					array(
						  "campo" 	 => "sehvalor",
				   		  "label" 	 => "Valor"
				   		  
					)
				  );
				  	
	return $coluna;			  	
}
?>
</body>
</html>