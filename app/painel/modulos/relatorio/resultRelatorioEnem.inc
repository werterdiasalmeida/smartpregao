<?

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

if ($_POST['agrupador']){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
}
ini_set("memory_limit","250M");
set_time_limit(0);
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao(true);
$r->setTotNivel(true);
$r->setEspandir(false);
$r->setMonstrarTolizadorNivel(true);
$r->setTotalizador(true);
$r->setTolizadorLinha(true);

//$r->setBrasao(true);
echo $r->getRelatorio();
?>
</body>
</html>
<?php 
function monta_sql(){
	global $filtroSession;

	extract($_POST);
	
	$select = array();
	$from	= array();
	
	if( $estado[0] && ( $estado_campo_flag || $estado_campo_flag == '1' )){
		$where[0] = " AND m.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estado ) . "') ";		
	}
	if( $municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " AND m.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	if( $filtroregiao[0] && ($filtroregiao_campo_flag || $filtroregiao_campo_flag == '1' )){
		$where[3] = " AND reg.regcod " . (( $filtroregiao_campo_excludente == null || $filtroregiao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $filtroregiao ) . "') ";		
	}
	if( $exist ){
		$where[2] = $exist == 'S' ? "AND ( cvm.tot_uni > 0 )" : "AND ( cvm.tot_uni = 0 OR cvm.tot_uni IS NULL )";
	}

	
	$sql = "SELECT 
				m.estuf as estado,
				m.mundescricao as municipio,
				d.dshcodmunicipio 					AS muncod,
				reg.regdescricao 					AS regiao,
			    coalesce(cvm.tot_uni::numeric,0) 	AS tot_uni, 
			    coalesce(cvm.tot_cmp::numeric,0) 	AS tot_cmp, 
			    coalesce(cvm.tot_geral::numeric,0) 	AS tot_geral, 
			    coalesce(sum(qtde)::numeric,0) 		AS qtd_alunos,
			    CASE WHEN coalesce(cvm.tot_uni::numeric,0)+coalesce(cvm.tot_cmp::numeric,0) = 0 THEN 'Sem Instituição Federal' else 'Com Instituição Federal' END as existencia
			FROM
				painel.v_detalheindicadorsh d
			LEFT JOIN campus_vs_municipio cvm ON cvm.muncod = d.dshcodmunicipio
			INNER JOIN territorios.municipio m ON m.muncod = d.dshcodmunicipio
			INNER JOIN territorios.estado est ON est.estuf = m.estuf
			INNER JOIN territorios.regiao reg ON reg.regcod = est.regcod
			WHERE
				indid = 632 AND
				sehid = 2732566
				".$where[0]."
				".$where[1]."
				".$where[2]."
				".$where[3]."
			GROUP BY
				d.dshcodmunicipio,
				cvm.tot_uni,
				cvm.tot_cmp,
				cvm.tot_geral,
				m.estuf, 
				m.mundescricao,
				reg.regdescricao
			ORDER BY
				m.estuf, 
				m.mundescricao";

	return $sql;
}

function monta_agp(){
	$agrupador = $_POST['agrupador'];
		
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"tot_uni",
											"tot_cmp",
											"tot_geral",	
											"qtd_alunos"
										)	  
				);
				
	
	$count = 1;
		$i = 0;
		
	foreach ($agrupador as $val):
		if($count == 1){
			$var = $vari;
		} else {
			$var = "";		
		}
		switch ($val) {
			case 'estado':
				array_push($agp['agrupador'], array(
													"campo" => "estado",
											  		"label" => "$var Estado")										
									   				);				
		   		continue;
		    break;
		    case 'municipio':
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "$var Município")										
									   				);					
		    	continue;
		    break;		    	
		    case 'existencia':
				array_push($agp['agrupador'], array(
													"campo" => "existencia",
											  		"label" => "$var Existência de Instituição Federal")										
									   				);					
		    	continue;
		    break;		    	
		    case 'regiao':
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											  		"label" => "$var Região")										
									   				);					
		    	continue;
		    break;		    	
		}
		$count++;
	endforeach;
	return $agp;
}

function monta_coluna(){

	$coluna    = array(
						array(
							  "campo" => "qtd_alunos",
					   		  "label" => "Quantidade de Alunos<br>(A)",
							  "type"  => "numeric"
						),
						
						array(
							  "campo" => "tot_uni",
					   		  "label" => "Qtde. Campus Ed. Superior<br>(B)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "tot_cmp",
					   		  "label" => "Qtde. Campus Ed. Profissional<br>(C)",
							  "type"  => "numeric"
						),
						array(
							  "campo" => "tot_geral",
					   		  "label" => "Qtde. Unidades Geral<br>(B) + (C)",
							  "type"  => "numeric"
						)
					);

	return $coluna;			  	
	
}
?>
