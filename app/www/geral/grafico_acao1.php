<?php

// FUNÇÕES

/**
 * Formata um número para ser impresso no gráfico
 *
 * @param string $valor
 * @return string
 */
function formatarValor( $valor )
{
	return number_format( $valor, 0, ",", "." );
}

// carrega as bibliotecas
include_once 'config.inc';
require_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/funcoes.inc';

include_once APPRAIZ . '/includes/jpgraph/jpgraph.php';
include_once APPRAIZ . '/includes/jpgraph/jpgraph_line.php';
include_once APPRAIZ . '/includes/jpgraph/jpgraph_utils.inc';
include_once APPRAIZ . '/includes/jpgraph/jpgraph_bar.php';

// abre conexão com o banco espelho produção
$db = new cls_banco();

// carrega os dados físicos
$sql_fisico = sprintf(
	"select e.exprealizado, r.refmes_ref
	from monitora.referencia r
	left join monitora.execucaopto e on e.refcod = r.refcod
	where r.refdata_limite_avaliacao_aca is not null and r.refsnmonitoramento = 't' and r.refano_ref = '%s' and e.acaid = '%s'
	order by refano_ref,refmes_ref",
	$_SESSION['exercicio'],
	$_REQUEST['acaid']
);
$meses = array();
$fisico = array();
foreach ( (array) $db->carregar( $sql_fisico ) as $registro ) {
	if ( !$registro ) {
		break;
	}
	array_push( $fisico, (integer) $registro['exprealizado'] );
	array_push( $meses, (integer) $registro['refmes_ref'] );
}

// carrega os dados financeiros
$sql_financeiro = sprintf(
	"select finvlrrealizado1, finvlrrealizado2, finvlrrealizado3, finvlrrealizado4, finvlrrealizado5, finvlrrealizado6, finvlrrealizado7, finvlrrealizado8, finvlrrealizado9, finvlrrealizado10, finvlrrealizado11, finvlrrealizado12 from monitora.dadofinanceiro df where df.acaid = '%d'",
	$_REQUEST['acaid']
);
$financeiro = array();
foreach( array_values( $db->pegaLinha( $sql_financeiro ) ) as $mes => $valor ){
	if ( in_array( $mes + 1, $meses ) ) {
		array_push( $financeiro, (integer) $valor );
	}
}

if ( count( $fisico ) == 0 || count( $financeiro ) == 0 ) {
	array_push( $fisico, 0 );
	array_push( $financeiro, 0 );
}

// GRÁFICO

$grafico = new Graph( 800, 300 );
$grafico->SetMargin( 60, 80, 60, 45 );
$grafico->SetMarginColor( 'white' );
$grafico->SetShadow( true, 5, '#dddddd' );
$grafico->SetTickDensity( TICKD_SPARSE );
$grafico->SetScale( 'intlin' );
$grafico->SetYScale( 0, 'lin' );

$grafico->title->Set( 'Gráfico de Execução' );
$grafico->title->SetFont( FF_VERDANA, FS_NORMAL, 13 );
$grafico->title->SetMargin( 10 );
$grafico->SetFrameBevel( 0, false );

// CORES

$azul_claro     = '#78ADE1';
$azul_escuro    = '#303090';
$laranja_claro  = '#FFBC46';
$laranja_escuro = '#CC6000';

// FÍSICO

$a = new LinePlot( $fisico );
$a->SetColor( $azul_claro );

$a->value->show();
$a->value->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$a->value->SetColor( $azul_escuro );
$a->value->SetAlign( 'right' );
$a->value->SetFormat( '%d' );

$a->mark->SetType( MARK_CIRCLE );
$a->mark->SetColor( $azul_claro );
$a->mark->SetWidth( 3 );
$a->SetLegend( 'Físico' );

$grafico->Add( $a );
$grafico->yaxis->SetPos( 'min' );
$grafico->yaxis->SetColor( $azul_claro );
$grafico->yaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$grafico->yaxis->SetLabelFormatCallback( 'formatarValor' );

// FINANCEIRO

$b = new LinePlot( $financeiro );
$b->SetColor( $laranja_claro );

$b->value->show();
$b->value->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$b->value->SetColor( $laranja_escuro );
$b->value->SetAlign( 'left', 'bottom' );
$b->value->SetFormat( '%d' );

$b->mark->SetType( MARK_CIRCLE );
$b->mark->SetColor( $laranja_claro );
$b->mark->SetWidth( 3 );
$b->SetLegend( 'Financeiro' );

$grafico->AddY( 0, $b );
$grafico->ynaxis[0]->SetPos( 'max' );
$grafico->ynaxis[0]->SetColor( $laranja_claro );
$grafico->ynaxis[0]->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$grafico->ynaxis[0]->SetLabelFormatCallback( 'formatarValor' );

// TEMPO

$grafico->xaxis->SetTickLabels( range( 1, count( $fisico ) ) );
//$grafico->xaxis->SetLabelSide( -10 );
//$grafico->xaxis->SetLabelMargin( 20 );
$grafico->xaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$grafico->xaxis->SetColor( '#505050' );
$grafico->xaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$grafico->xaxis->SetTitle( 'Meses', 'center' );

$grafico->xaxis->title->SetColor( '#505050' );
$grafico->xaxis->title->SetMargin( 10 );
$grafico->xaxis->title->SetFont( FF_VERDANA, FS_NORMAL, 8 );

// Output line

$grafico->legend->SetLayout( LEGEND_HOR );
$grafico->legend->Pos( 0.87, 0.03, 'center' );
 
$grafico->img->SetImgFormat( 'png' );
$grafico->Stroke();

?>