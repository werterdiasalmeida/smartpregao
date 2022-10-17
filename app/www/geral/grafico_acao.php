<?php
ini_set( 'display_errors', 0 );
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
$mesmaximo = 1;
foreach ( (array) $db->carregar( $sql_fisico ) as $registro ) {
	if ( !$registro ) {
		break;
	}
	if($registro['refmes_ref'] > $mesmaximo) {
		$mesmaximo = $registro['refmes_ref'];
	}
	$dadosfisicopormes[((integer) $registro['refmes_ref'] - 1)] = (integer) $registro['exprealizado'];
}
for($i = 0; $i <= 11; $i++) {
	if(($mesmaximo - 1) >= $i && is_null($dadosfisicopormes[$i])) {
		$dadosfisicopormes[$i] = 0;
	} elseif(is_null($dadosfisicopormes[$i])) {
		$dadosfisicopormes[$i] = '';
	}
}
ksort($dadosfisicopormes);

// carrega os dados financeiros
$sql = sprintf("select rofmes, max(rofdata) as dataatu, sum(rofdot_ini) as rofdot_ini, sum(rofautorizado) as rofautorizado,  sum(rofempenhado) as empenhado,
				sum(rofliquidado_favorecido) as rofliquidado_favorecido, sum(rofpago) as rofpago
				from financeiro.execucao as exe
				left join monitora.acao as aca on aca.prgcod = exe.prgcod and aca.acacod = exe.acacod and aca.unicod = exe.unicod and aca.loccod = exe.loccod
				where aca.acaid = '%s' and rofano = '%s'
				group by exe.rofmes, exe.prgcod,exe.acacod, exe.unicod,exe.loccod 
				order by exe.rofmes",
				$_REQUEST['acaid'], $_SESSION['exercicio']);
$dadosfinanceiroscompletos = @$db->carregar( $sql );

$mesmaximo = 1;
if($dadosfinanceiroscompletos) {
	foreach($dadosfinanceiroscompletos as $linhafin) {
		if($linhafin['rofmes'] > $mesmaximo) {
			$mesmaximo = $linhafin['rofmes'];
		}
		$dadofinanceiropormes[((integer) $linhafin['rofmes'] - 1)] = (integer) $linhafin['rofliquidado_favorecido'];
		
	}
}
for($i = 0; $i <= 11; $i++) {
	if(($mesmaximo - 1) >= $i && is_null($dadofinanceiropormes[$i])) {
		$dadofinanceiropormes[$i] = 0;
	} elseif(is_null($dadofinanceiropormes[$i])) {
		$dadofinanceiropormes[$i] = '';
	}
}
ksort($dadofinanceiropormes);

//dbg($dadofinanceiropormes,1);
//exit;


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
$a = new LinePlot( $dadosfisicopormes );
$a->SetColor( $azul_claro );
$a->value->show();
$a->value->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$a->value->SetColor( $azul_escuro );
$a->value->SetAlign( 'right' );
$a->value->SetFormat( '%s' );
$a->mark->SetType( MARK_IMG_DIAMOND, 'blue', 0.3 );
$a->mark->SetColor( $azul_claro );
$a->mark->SetWidth( 3 );
$a->SetLegend( 'Físico' );
$grafico->Add( $a );
$grafico->yaxis->SetPos( 'min' );
$grafico->yaxis->scale->SetAutoMin(0);
$grafico->yaxis->SetColor( $azul_claro );
$grafico->yaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$grafico->yaxis->SetLabelFormatCallback( 'formatarValor' );

// FINANCEIRO
$b = new LinePlot( $dadofinanceiropormes );
$b->SetColor( $laranja_claro );
$b->value->show();
$b->value->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$b->value->SetColor( $laranja_escuro );
$b->value->SetAlign( 'left', 'bottom' );
$b->value->SetFormat( '%d' );
$b->mark->SetType( MARK_IMG_DIAMOND, 'yellow', 0.3 );
$b->mark->SetColor( $laranja_claro );
$b->mark->SetWidth( 3 );
$b->SetLegend( 'Liquidado (R$)' );
$grafico->AddY( 0, $b );
$grafico->ynaxis[0]->SetPos( 'max' );
$grafico->yaxis->scale->SetAutoMin(0);
$grafico->ynaxis[0]->SetColor( $laranja_claro );
$grafico->ynaxis[0]->SetFont( FF_VERDANA, FS_NORMAL, 8 );
$grafico->ynaxis[0]->SetLabelFormatCallback( 'formatarValor' );

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

?>