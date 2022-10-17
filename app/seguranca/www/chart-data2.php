<?php

include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once "_funcoesmonitoramento.php";

// use the chart class to build the chart:
include_once "../../includes/open_flash_chart/open-flash-chart.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';


// abre conexão com o servidor de banco de dados
$db = new cls_banco();

/*
 * DADOS FUNDAMENTAIS
 * SISID, MES, ANO, TMOID
 */

//
// Example of frequence bar 
//
include_once ("../../includes/jpgraph/jpgraph.php");
include_once ("../../includes/jpgraph/jpgraph_bar.php");
include_once ("../../includes/jpgraph/jpgraph_line.php");


$dados = explode(";", $_REQUEST['dados']);

$sisid    = $dados[0];
$mes      = $dados[1];
$ano      = $dados[2];
$tmoidEsq = $dados[3];
$tmoidDir = $dados[4];

// some data
$data_1 = array();
$ndias = cal_days_in_month(CAL_GREGORIAN, $mes, $ano);
$dadostr = pegarDados(array('sisid' => $sisid, 'mes' => $mes, 'ano' => $ano));

$tipomonitoramentoEsq = tirar_acentos($db->pegaUm("SELECT tmodescricao FROM seguranca.tipomonitoramento WHERE tmoid='".$tmoidEsq."'"));
$tipomonitoramentoDir = tirar_acentos($db->pegaUm("SELECT tmodescricao FROM seguranca.tipomonitoramento WHERE tmoid='".$tmoidDir."'"));

$rangeEsq = $db->pegaUm("SELECT tmorange FROM seguranca.tipomonitoramento WHERE tmoid='".$tmoidEsq."'");
$rangeEsq = explode(";", $rangeEsq);

$newstep = false;
for($i=1;$i<=$ndias;$i++) {
	if($rangeEsq[0] > (float)$dadostr[$tmoidEsq][$i]) { $rangeEsq[0] = $dadostr[$tmoidEsq][$i]; $newstep = true;} 
	if($rangeEsq[1] < (float)$dadostr[$tmoidEsq][$i]) { $rangeEsq[1] = $dadostr[$tmoidEsq][$i]; $newstep = true;}
	$data_1[] = (($dadostr[$tmoidEsq][$i])?(float)$dadostr[$tmoidEsq][$i]:0);
}

if($newstep) {
	$rangeEsq[2] = (($rangeEsq[1]-$rangeEsq[0])/10);
	if($rangeEsq[2] > 1) $rangeEsq[2] = round($rangeEsq[2]);
	else $rangeEsq[2] = round($rangeEsq[2], 1);
}

$rangeDir = $db->pegaUm("SELECT tmorange FROM seguranca.tipomonitoramento WHERE tmoid='".$tmoidDir."'");
$rangeDir = explode(";", $rangeDir);

$newstep = false;
for($i=1;$i<=$ndias;$i++) {
	if($rangeDir[0] > (float)$dadostr[$tmoidDir][$i]) { $rangeDir[0] = $dadostr[$tmoidDir][$i]; $newstep = true;} 
	if($rangeDir[1] < (float)$dadostr[$tmoidDir][$i]) { $rangeDir[1] = $dadostr[$tmoidDir][$i]; $newstep = true;}
	$data_2[] = (($dadostr[$tmoidDir][$i])?(float)$dadostr[$tmoidDir][$i]:0);
}

if($newstep) {
	$rangeDir[2] = (($rangeDir[1]-$rangeDir[0])/10);
	if($rangeDir[2] > 1) $rangeDir[2] = round($rangeDir[2]);
	else $rangeDir[2] = round($rangeDir[2], 1);
}
/*
echo "<pre>";
print_r($data_1);
print_r($data_2);
exit;
*/

//$data_freq = array(22,20,12,10,5,4,2);
//$data_accfreq = accfreq($data_freq);

// Create the graph. 
$graph = new Graph(900,300, "auto");

// Setup some basic graph parameters
$graph->SetScale("textlin");
//$graph->SetScale('lin',$rangeEsq[0],$rangeEsq[1]);
$graph->SetY2Scale('lin',$rangeDir[0],$rangeDir[1]);
//$graph->SetY2Scale("textlin");
$graph->img->SetMargin(50,70,30,40);
$graph->yaxis->SetTitleMargin(30);
$graph->SetMarginColor('#EEEEEE');

// Setup titles and fonts
//$graph->title->Set("Frequence plot");
$graph->xaxis->title->Set("Dia");
$graph->yaxis->title->Set($tipomonitoramentoEsq);
$graph->y2axis->title->Set($tipomonitoramentoDir);

$graph->title->SetFont(FF_FONT1,FS_BOLD);
$graph->yaxis->title->SetFont(FF_FONT0,FS_NORMAL);
$graph->yaxis->title->SetColor('red');
$graph->y2axis->title->SetFont(FF_FONT0,FS_NORMAL);
$graph->y2axis->title->SetColor('blue');
$graph->xaxis->title->SetFont(FF_FONT1,FS_BOLD);

// Turn the tickmarks
$graph->xaxis->SetTickSide(SIDE_DOWN);
$graph->yaxis->SetTickSide(SIDE_LEFT);

$graph->y2axis->SetTickSide(SIDE_RIGHT);
$graph->y2axis->SetColor('black','blue');
//$graph->y2axis->SetLabelFormat('%3d.0%%');

// Create a bar pot
$bplot = new LinePlot($data_1);

//$bplot->SetBarCenter();
$bplot->SetColor('red');
$bplot->SetWeight(3);

//$bplot->value->SetFormat("%d");

// Create accumulative graph
$lplot = new LinePlot($data_2);
//$lplot->value->SetFormat("%d");
// We want the line plot data point in the middle of the bars
//$lplot->SetBarCenter();

// Use transperancy
//$lplot->SetFillColor('lightblue@0.6');
$lplot->SetColor('blue');
$lplot->SetWeight(3);
$graph->AddY2($lplot);

// Setup the bars
//$bplot->SetFillColor("orange@0.2");
//$bplot->SetValuePos('center');

//$bplot->value->SetFont(FF_ARIAL,FS_NORMAL,9);
//$bplot->value->Show();

// Add it to the graph
$graph->Add($bplot);

// Send back the HTML page which will call this script again
// to retrieve the image.
$graph->Stroke();











/*
$dados = explode(";", $_REQUEST['dados']);

$sisid    = $dados[0];
$mes      = $dados[1];
$ano      = $dados[2];
$tmoidEsq = $dados[3];
$tmoidDir = $dados[4];

$data_1 = array();
$ndias = cal_days_in_month(CAL_GREGORIAN, $mes, $ano);
$dadostr = pegarDados(array('sisid' => $sisid, 'mes' => $mes, 'ano' => $ano));

$tipomonitoramentoEsq = tirar_acentos($db->pegaUm("SELECT tmodescricao FROM seguranca.tipomonitoramento WHERE tmoid='".$tmoidEsq."'"));
$tipomonitoramentoDir = tirar_acentos($db->pegaUm("SELECT tmodescricao FROM seguranca.tipomonitoramento WHERE tmoid='".$tmoidDir."'"));

$title = new title( $tipomonitoramentoEsq.' X '.$tipomonitoramentoDir ) ;

$chart = new open_flash_chart();
$chart->set_title( $title );


$rangeEsq = $db->pegaUm("SELECT tmorange FROM seguranca.tipomonitoramento WHERE tmoid='".$tmoidEsq."'");
$rangeEsq = explode(";", $rangeEsq);

$newstep = false;
for($i=1;$i<=$ndias;$i++) {
	if($rangeEsq[0] > (float)$dadostr[$tmoidEsq][$i]) { $rangeEsq[0] = $dadostr[$tmoidEsq][$i]; $newstep = true;} 
	if($rangeEsq[1] < (float)$dadostr[$tmoidEsq][$i]) { $rangeEsq[1] = $dadostr[$tmoidEsq][$i]; $newstep = true;}
	$data_1[] = (($dadostr[$tmoidEsq][$i])?(float)$dadostr[$tmoidEsq][$i]:0);
}

if($newstep) {
	$rangeEsq[2] = (($rangeEsq[1]-$rangeEsq[0])/10);
	if($rangeEsq[2] > 1) $rangeEsq[2] = round($rangeEsq[2]);
	else $rangeEsq[2] = round($rangeEsq[2], 1);
}

$default_dot = new dot();
$default_dot->size(5)->colour('#DFC329');
$line_dot = new line();
$line_dot->set_default_dot_style($default_dot);
$line_dot->set_width( 4 );
$line_dot->set_colour( '#DFC329' );
$line_dot->set_values( $data_1 );
$line_dot->set_key( $tipomonitoramentoEsq, 10 );


$rangeDir = $db->pegaUm("SELECT tmorange FROM seguranca.tipomonitoramento WHERE tmoid='".$tmoidDir."'");
$rangeDir = explode(";", $rangeDir);

$newstep = false;
for($i=1;$i<=$ndias;$i++) {
	if($rangeDir[0] > (float)$dadostr[$tmoidDir][$i]) { $rangeDir[0] = $dadostr[$tmoidDir][$i]; $newstep = true;} 
	if($rangeDir[1] < (float)$dadostr[$tmoidDir][$i]) { $rangeDir[1] = $dadostr[$tmoidDir][$i]; $newstep = true;}
	$data_2[] = (($dadostr[$tmoidDir][$i])?(float)$dadostr[$tmoidDir][$i]:0);
}

if($newstep) {
	$rangeDir[2] = (($rangeDir[1]-$rangeDir[0])/10);
	if($rangeDir[2] > 1) $rangeDir[2] = round($rangeDir[2]);
	else $rangeDir[2] = round($rangeDir[2], 1);
}
$default_dot2 = new dot();
$default_dot2->size(5)->colour('#6363AC');
$line_dot2 = new line();
$line_dot2->set_default_dot_style($default_dot2);
$line_dot2->set_width( 4 );
$line_dot2->set_colour( '#6363AC' );
$line_dot2->set_values( $data_2 );
$line_dot2->set_key( $tipomonitoramentoDir, 10 );

$x = new x_axis();
$x->set_range( 1, (integer)$ndias );
$x->set_steps(1);
$chart->set_x_axis( $x );

$y = new y_axis();
$y->set_range($rangeEsq[0], $rangeEsq[1], $rangeEsq[2]); 
$chart->set_y_axis( $y );

$y2 = new y_axis_right();
$y2->set_range($rangeDir[0], $rangeDir[1], $rangeDir[2]); 
$chart->set_y_axis_right( $y2 );

// here we add our data sets to the chart:
$chart->add_element( $line_dot );
$chart->add_element( $line_dot2 );

echo $chart->toPrettyString();
*/
?>