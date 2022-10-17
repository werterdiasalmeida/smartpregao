<?php
ini_set( 'display_errors', 0 );
// FUNЧеES

/**
 * Formata um nњmero para ser impresso no grсfico
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
require_once APPRAIZ . 'includes/class/dateTime.inc';
include_once APPRAIZ . 'includes/funcoes.inc';

include_once APPRAIZ . '/includes/jpgraph/jpgraph.php';
include_once APPRAIZ . '/includes/jpgraph/jpgraph_line.php';
include_once APPRAIZ . '/includes/jpgraph/jpgraph_utils.inc';
include_once APPRAIZ . '/includes/jpgraph/jpgraph_bar.php';
// abre conexуo com o banco espelho produчуo
$db = new cls_banco();
// carrega os dados
$indid = $_REQUEST['indid'];
$sql = "SELECT 
		sehvalor,
		sehreferencia
		FROM
		painel.seriehistorica 
		WHERE
		indid = ".$indid."	
		ORDER BY sehid	
";

$dados = $db->carregar($sql);
//obtendo total de registros na seriehistorica segundo seu indid
$sql_n = "SELECT 
			count (sehid) as qtd
			FROM
			painel.seriehistorica 
			WHERE 
			indid = ".$indid."";
$n = $db->pegaUm($sql_n);

//montado array de dados para os eixos y e x

$dt = new Data();
if($dados){
	foreach ($dados as $chave => $dado) {
		$ydados[] = $dado['sehvalor'];
		$xdados[] = $dt->timeStampDeUmaData($dado['sehreferencia']);
	}
}
$dateUtils = new DateScaleUtils();
list($tickPositions,$minTickPositions) = $dateUtils->GetTicks($xdados);

$grace = 400000;
$xmin = $xdados[0]-$grace;
$xmax = $xdados[$n-1]+$grace;

// Create the graph. These two calls are always required
$graph = new Graph(450,250,"auto");
//$graph->SetScale("textlin");
$graph->SetScale('intlin',0,0,$xmin,$xmax);

$graph->xaxis->SetPos('min');

// Now set the tic positions
$graph->xaxis->SetTickPositions($tickPositions,$minTickPositions);
$graph->xaxis->SetLabelFormatString('My',true);
$graph->img->SetAntiAliasing();
$graph->xgrid->Show();


// Setup margin and titles
$graph->img->SetMargin(40,20,20,40);
$graph->title->Set("Sщrie Histѓrica: Valor");
$graph->xaxis->title->Set("Data");
$graph->yaxis->title->Set("Valor");
$graph->ygrid->SetFill(true,'#EFEFEF@0.5','#F9BB64@0.5');
//$graph->SetShadow();
$graph->xgrid->Show();

// Create the plot line
$p1 = new LinePlot($ydados,$xdados);
$p1->SetColor('teal');

$graph->Add($p1);

// Output graph
$graph->Stroke();


/*
// GRСFICO
$grafico = new Graph( 800, 300 );
$grafico->SetMargin( 60, 80, 60, 45 );
$grafico->SetMarginColor( 'white' );
$grafico->SetShadow( true, 5, '#dddddd' );
$grafico->SetTickDensity( TICKD_SPARSE );
$grafico->SetScale( 'intlin' );
$grafico->SetYScale( 0, 'lin' );
$grafico->title->Set( 'Grсfico de Execuчуo' );
$grafico->title->SetFont( FF_VERDANA, FS_NORMAL, 13 );
$grafico->title->SetMargin( 10 );
$grafico->SetFrameBevel( 0, false );

// CORES
$azul_claro     = '#78ADE1';
$azul_escuro    = '#303090';
$laranja_claro  = '#FFBC46';
$laranja_escuro = '#CC6000';

// Valores
$dados = array(1,2,3,5,3,5,8,4,2);
$a = new LinePlot( $dados );
$a->SetColor( $azul_claro );
$a->value->show();
$a->value->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$a->value->SetColor( $azul_escuro );
$a->value->SetAlign( 'right' );
$a->value->SetFormat( '%s' );
$a->mark->SetType( MARK_IMG_DIAMOND, 'blue', 0.3 );
$a->mark->SetColor( $azul_claro );
$a->mark->SetWidth( 3 );
$a->SetLegend( 'Valor' );
$grafico->Add( $a );
$grafico->yaxis->SetPos( 'min' );
$grafico->yaxis->scale->SetAutoMin(0);
$grafico->yaxis->SetColor( $azul_claro );
$grafico->yaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$grafico->yaxis->SetLabelFormatCallback( 'formatarValor' );

// TEMPO
$grafico->xaxis->SetTickLabels( range( 1, 12 ) );
$grafico->xaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$grafico->xaxis->SetColor( '#505050' );
$grafico->xaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$grafico->xaxis->SetTitle( 'Meses', 'center' );
$grafico->xaxis->title->SetColor( '#505050' );
$grafico->xaxis->title->SetMargin( 10 );
$grafico->xaxis->title->SetFont( FF_VERDANA, FS_NORMAL, 8 );
// Output line
$grafico->legend->SetLayout( LEGEND_VERT );
$grafico->legend->Pos( 0.87, 0.03, 'center' );
$grafico->img->SetImgFormat( 'png' );
$grafico->Stroke();
*/
?>