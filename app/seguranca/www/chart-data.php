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
$dados = explode(";", $_REQUEST['dados']);

$sisid = $dados[0];
$mes = $dados[1];
$ano = $dados[2];
$tmoid = $dados[3];
$mnulink = $dados[4];

$data_1 = array();

$ndias = cal_days_in_month(CAL_GREGORIAN, (integer)$mes, $ano);

if($mnulink) {
	$dadostr = pegarDadosPorPagina(array('sisid' => $sisid, 'mes' => $mes, 'ano' => $ano, 'link' => $mnulink));
} else {
	$dadostr = pegarDados(array('sisid' => $sisid, 'mes' => $mes, 'ano' => $ano));
}

$range = $db->pegaUm("SELECT tmorange FROM seguranca.tipomonitoramento WHERE tmoid='".$tmoid."'");
$range = explode(";", $range);

$newstep = false;
for($i=1;$i<=$ndias;$i++) {
	if($range[0] > (float)$dadostr[$tmoid][$i]) { $range[0] = $dadostr[$tmoid][$i]; $newstep = true;} 
	if($range[1] < (float)$dadostr[$tmoid][$i]) { $range[1] = $dadostr[$tmoid][$i]; $newstep = true;}
	$data_1[] = (($dadostr[$tmoid][$i])?(float)$dadostr[$tmoid][$i]:0);
}

if($newstep) {
	$range[2] = (($range[1]-$range[0])/10);
	if($range[2] > 1) $range[2] = round($range[2]);
	else $range[2] = round($range[2], 1);
}

$title = new title( "Linha" );

$line_1_default_dot = new dot();
$line_1_default_dot->colour('#f00000');

$line_1 = new line();
$line_1->set_default_dot_style($line_1_default_dot);
$line_1->set_values( $data_1 );
$line_1->set_width( 2 );

$x = new x_axis();
$x->set_range( 1, (integer)$ndias );
$x->set_steps(1);

$y = new y_axis();
$y->set_range(ceil($range[0]), ceil($range[1]), ceil($range[2]));


$chart = new open_flash_chart();
$chart->add_element( $line_1 );
$chart->set_y_axis( $y );
$chart->set_x_axis( $x );
$chart->set_bg_colour( '#ffffff' );

echo $chart->toPrettyString();
?>