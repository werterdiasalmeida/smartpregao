<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

include APPRAIZ . "includes/jpgraph/jpgraph.php";
include APPRAIZ . "includes/jpgraph/jpgraph_gantt.php";

include APPRAIZ . "pde/www/_constantes.php";
include APPRAIZ . "pde/www/_funcoes.php";

function pegarCorBarra( $estado )
{
	switch ( strtolower( trim( $estado ) ) )
	{
		case 'em andamento':
			return array( 'lightgreen', 'darkgreen' );
		case 'suspenso':
			return array( 'lightyellow', 'darkyellow' );
		case 'cancelado':
			return array( 'red', 'red' );
		case 'concluído':
			return array( 'black', 'black' );
		case 'não iniciado':
		default:
			return array( 'white', 'darkgray' );
	}
}

$atividades = atividade_pegar_filhas( PROJETO, 1249 );
//$atividades = atividade_pegar_filhas( PROJETO, 1111 );
//$atividades = atividade_pegar_filhas( PROJETO, 1259 );

$atividades = $atividades ? $atividades : array();
$atividade_pai = $atividades[0];

$grafico = new GanttGraph( 0, 0, 'auto' );
$grafico->SetShadow( false );
$grafico->SetBox( true, array( 0, 0, 0 ) );
$grafico->scale->SetWeekStart( 1 );
$grafico->title->Set( $atividade_pai['numero'] . ' ' . $atividade_pai['atidescricao'] );
$grafico->ShowHeaders( GANTT_HYEAR | GANTT_HMONTH | GANTT_HDAY | GANTT_HWEEK );
$grafico->scale->week->SetStyle( WEEKSTYLE_FIRSTDAY );
$grafico->scale->week->SetFont( FF_FONT1 );
$grafico->title->SetFont( FF_ARIAL, FS_BOLD, 20 );

$grafico->scale->month->SetBackgroundColor( 'black:1.7' );
$grafico->scale->year->SetBackgroundColor( 'black:1.7' ); 

$grafico->scale->tableTitle->Set( 'Atividades' );
$grafico->scale->tableTitle->SetFont( FF_ARIAL, FS_NORMAL, 12 );
$grafico->scale->SetTableTitleBackground( 'black:1.7' );
$grafico->scale->tableTitle->Show( true );

// calcula data inicial e final
$data_menor = array();
$data_maior = array();
reset( $atividades );
foreach ( $atividades as $atividade )
{
	if ( $atividade['atidatainicio'] )
	{
		array_push( $data_menor, $atividade['atidatainicio'] );
	}
	if ( $atividade['atidatafim'] )
	{
		array_push( $data_maior, $atividade['atidatafim'] );
	}
}
$data_menor = min( $data_menor );
$data_menor = $data_menor ? $data_menor : date( 'Y-m-d' );
$data_maior = max( $data_maior );
$data_maior = $data_maior ? $data_maior : $data_menor;
$dados = explode( '-', $data_maior );
$data_maior = date( 'Y-m-d', mktime( 0, 0, 0, $dados[1], $dados[2] + 1, $dados[0] ) );

// calcula data para tarefas que não possuem data (data fora do range)
$dados = explode( '-', $data_menor );
$data_fora = ( $dados[0] - 1 ) . '-' . $dados[1] . '-' . $dados[2]; 

reset( $atividades );
foreach ( $atividades as $chave => $atividade )
{
	$possui_filho = $atividades[$chave+1] && $atividades[$chave+1]['profundidade'] > $atividade_pai['profundidade'];
	$profundidade = $atividade['profundidade'] - $atividade_pai['profundidade'];
	$concluido =  (integer) $atividade['atiporcentoexec'];
	$descricao = str_repeat( '   ', $profundidade ) . $atividade['numero'] . ' ' . $atividade['atidescricao'];
	if ( !$atividade['atidatainicio'] || !$atividade['atidatafim'] )
	{
		$inicio = $data_fora;
		$termino = $data_fora;
	}
	else
	{
		$inicio = $atividade['atidatainicio'];
		$termino = $atividade['atidatafim'];
	}
	$cor = pegarCorBarra( $atividade['esadescricao'] );
	
	$barra = new GanttBar( $chave, $descricao, $inicio, $termino, $concluido . '%', 10 );
	$barra->rightMark->SetType( MARK_FILLEDCIRCLE ); 
	$barra->title->SetFont( FF_FONT1, FS_NORMAL, 8 );
	$barra->title->SetColor( $cor[1] );
	$barra->SetPattern( BAND_SOLID, $cor[0], 5 );
	$barra->progress->Set( $concluido / 100 );
	$barra->progress->SetPattern( GANTT_SOLID, $cor[1] );
	$grafico->Add( $barra );
}

$grafico->SetDateRange( $data_menor, $data_maior );
$grafico->SetBackgroundGradient( 'white', 'white' );
$grafico->SetFrame( false );
$grafico->Stroke();

