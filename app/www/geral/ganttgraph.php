<?php
	require_once 'config.inc';
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$pjeid = md5_decrypt($_REQUEST['cp'],'');
	
	//$ptoid = $_REQUEST['ptoid'];
	include_once (APPRAIZ."/includes/jpgraph/jpgraph.php");
	include_once (APPRAIZ."/includes/jpgraph/jpgraph_gantt.php");
	$db = new cls_banco();


function abreviatura($nome)	
{
	
	$str = str_split($nome);
    $nomabrev=$str[0];
    for ($j=1;$j<count($str);$j++)
     {
	   if (ord($str[$j])==32) $nomabrev .= $str[$j+1];
     }
    return $nomabrev;
}

	//setlocale (LC_ALL, 'pt_BR');
	$sql = "select pjedsc, to_char( pjedataini, 'dd/mm/yyyy' ) as pjedataini, to_char( pjedatafim, 'dd/mm/yyyy' ) as pjedatafim from monitora.projetoespecial where pjeid=".$pjeid;
	$projeto = $db->pegaLinha( $sql );
    if ($ptoid)
    {
		$sqlAlt ="select pt.ptoordem,
					 pt.ptoordem_antecessor,
					 pt.ptodata_ini, 
					 pt.ptodata_fim, 
					 pt.ptodsc,  
					 pt.ptoid, 
					 pt.ptotipo,
					 pt.ptocod,
					 pt.usucpf, 
					 pt.ptoprevistoexercicio as previsto,
					 sum( ep.exprealizado ) as realizado,  
					 sum( ep.expfinanceiro ) as gasto, 
					 ( ( sum( ep.exprealizado ) / pt.ptoprevistoexercicio ) * 100 ) as porcentagem  
			from monitora.planotrabalho pt 
			left join monitora.execucaopje ep using ( ptoid )  
			where pt.pjeid = " . $pjeid . " and (pt.ptoid=$ptoid or pt.ptoid_pai=$ptoid) and pt.ptostatus='A' 
			group by pt.ptoordem, pt.ptoordem_antecessor, pt.ptodata_ini, pt.ptodata_fim,pt.ptodsc, pt.ptoid,pt.ptotipo,pt.ptocod,pt.usucpf, pt.ptoprevistoexercicio 
			order by pt.ptoordem";
    }
    else 
    {
    	$sqlAlt ="select pt.ptoordem,
					 pt.ptoordem_antecessor,
					 pt.ptodata_ini, 
					 pt.ptodata_fim, 
					 pt.ptodsc,  
					 pt.ptoid, 
					 pt.ptotipo,
					 pt.ptocod,
					 pt.usucpf, 
					 pt.ptoprevistoexercicio as previsto,
					 sum( ep.exprealizado ) as realizado,  
					 sum( ep.expfinanceiro ) as gasto, 
					 ( ( sum( ep.exprealizado ) / pt.ptoprevistoexercicio ) * 100 ) as porcentagem  
			from monitora.planotrabalho pt 
			left join monitora.execucaopje ep using ( ptoid )  
			where pt.pjeid = " . $pjeid . " and pt.ptostatus='A' 
			group by pt.ptoordem, pt.ptoordem_antecessor, pt.ptodata_ini, pt.ptodata_fim,pt.ptodsc, pt.ptoid,pt.ptotipo,pt.ptocod,pt.usucpf, pt.ptoprevistoexercicio 
			order by pt.ptoordem";    	
    }
	$resultado = $db->carregar( $sqlAlt );
	//dbg($sqlAlt,1);
	$graph = new GanttGraph(0,0,'auto');
	//$graph ->scale->SetDateLocale( "pt_BR" );
	$graph->SetShadow();
	
	// Add title and subtitle
	$graph->title->Set( $projeto[ 'pjedsc' ] );
	$graph->title->SetFont(FF_ARIAL,FS_BOLD,8);
	$graph->subtitle->Set( $projeto[ 'pjedataini' ] . ' - ' . $projeto[ 'pjedatafim' ] );
	
	// Show day, week and month scale
	$graph->ShowHeaders(GANTT_HWEEK | GANTT_HMONTH);
	
	// Instead of week number show the date for the first day in the week
	// on the week scale
	$graph->scale->week->SetStyle(WEEKSTYLE_FIRSTDAY);
	
	// Make the week scale font smaller than the default
	$graph->scale->week->SetFont(FF_FONT0);
	
	// Use the short name of the month together with a 2 digit year
	// on the month scale
	$graph->scale->month->SetStyle(MONTHSTYLE_SHORTNAMEYEAR2);
	
	// Format the bar for the first activity
	// ($row,$title,$startdate,$enddate)
	foreach( $resultado as $key=>$linha )
	{
		$nomeresp='';
		if ($linha['usucpf']){
		    $sqlnome="select usunome from seguranca.usuario where usucpf='".$linha['usucpf']."'";
		    $nomeresp=abreviatura($db->pegaUm($sqlnome));
		    if (! $nomeresp) $nomeresp='';

		}
		$tsAux = strtotime( $linha[ 'ptodata_ini' ] );
		$dataIni = date( 'd/m/Y', $tsAux );
		$tsAux = strtotime( $linha[ 'ptodata_fim' ] );
		$dataFim = date( 'd/m/Y', $tsAux );
		$txtAlt =
				"Início: " . $dataIni . "<br />" .
				"Término: " . $dataFim . "<br />" .
				"Previsto: " . formata_valor( $linha['previsto'], 0 ) . "<br/>" .
				"Executado: " . formata_valor( $linha['realizado'], 0 ) . "<br/>" .
				"Gasto: R$ " . formata_valor( $linha['gasto'], 0 ) . "<br/>" .
				"Percentual: " . formata_valor( $linha['porcentagem'], 2 ) . "%";

		if ($linha[ 'ptotipo' ]=='M')
		{
		$activity = new GanttBar($key, $linha[ 'ptodsc' ], $linha[ 'ptodata_ini' ], $linha[ 'ptodata_fim' ],$nomeresp,1);		
		}else {

			$activity = new GanttBar($key, '   '.$linha[ 'ptodsc' ], $linha[ 'ptodata_ini' ], $linha[ 'ptodata_fim' ],$nomeresp);
		}
		if($linha['ptoordem_antecessor']) {
			//$activity->SetConstrain($linha['ptoordem_antecessor'],CONSTRAIN_STARTEND);
		}
		
		$activity->SetCSIMTarget('#" onmouseover="return escape(\'' . $txtAlt .'\')"','Go back 1');
		// Yellow diagonal line pattern on a red background
		$activity->SetPattern(BAND_RDIAG,"blue");
		if ($linha[ 'ptotipo' ]=='P') 	$activity->SetFillColor("gray"); 
		else $activity->SetFillColor("red");
		if ($linha[ 'porcentagem' ]>100) $linha[ 'porcentagem' ]=100;
		$activity->progress->Set( $linha[ 'porcentagem' ] / 100 );
		// Finally add the bar to the graph
		$graph->Add($activity);

	}
	// ... and display it
	$graph->StrokeCSIM( 'ganttgraph.php' );
	
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 