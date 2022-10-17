<?php

include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once "_funcoesmonitoramento.php";

// use the chart class to build the chart:
include_once "../../includes/open_flash_chart/open-flash-chart.php";

// generate some random data
srand((double)microtime()*1000000);


// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "SELECT tmodescricao FROM seguranca.tipomonitoramento WHERE tmoid='".$_REQUEST['tmoid']."'";
$tmodescricao = $db->pegaUm($sql);

$sql = "SELECT CASE WHEN t.tmoacao='media' THEN SUM(monvalor)/(SELECT count(sisid) FROM seguranca.sistema WHERE sisstatus='A') ELSE SUM(monvalor) END as valor, monmes, monano FROM seguranca.monitoramento m 
		LEFT JOIN seguranca.tipomonitoramento t ON m.tmoid=t.tmoid 
		WHERE m.tmoid=".$_REQUEST['tmoid']." AND mondia is null GROUP BY m.monmes, m.monano, t.tmoacao ORDER BY monano, monmes";
$datas = $db->carregar($sql);


if($datas[0]) {
	foreach($datas as $data) {
		$data_1[] = $data['valor']; 
		$_x_ax[]  = sprintf("%02d",$data['monmes'])."/".$data['monano'];
	}
}

// Example for use of JpGraph,
require_once ('../../includes/jpgraph/jpgraph.php');
require_once ('../../includes/jpgraph/jpgraph_bar.php');

// Setup the graph.
$graph = new Graph(500,340);
$graph->img->SetMargin(60,20,35,75);
$graph->img->SetAntiAliasing(false);
$graph->SetScale("textlin");
$graph->SetMarginColor("lightblue:1.1");
$graph->SetShadow();

// Set up the title for the graph
$graph->title->Set($tmodescricao);
$graph->title->SetMargin(8);
$graph->title->SetFont(FF_FONT1,FS_BOLD,12);
$graph->title->SetColor("darkred");

// Setup font for axis
$graph->xaxis->SetFont(FF_FONT1,FS_NORMAL,10);
$graph->yaxis->SetFont(FF_FONT1,FS_NORMAL,10);

// Show 0 label on Y-axis (default is not to show)
$graph->yscale->ticks->SupressZeroLabel(false);

// Setup X-axis labels
$graph->xaxis->SetTickLabels($_x_ax);
$graph->xaxis->SetLabelAngle(90);

// Create the bar pot
$bplot = new BarPlot($data_1);
$bplot->SetWidth(0.6);
$bplot->value->Show();
$bplot->value->SetAngle(90); 
$bplot->value->SetFont(FF_FONT1,FS_NORMAL,8);

// Setup color for gradient fill style
$bplot->SetFillGradient("navy:0.9","navy:1.85",GRAD_LEFT_REFLECTION);

// Set color for the frame of each bar
$bplot->SetColor("white");
$graph->Add($bplot);

// Finally send the graph to the browser
$graph->Stroke();