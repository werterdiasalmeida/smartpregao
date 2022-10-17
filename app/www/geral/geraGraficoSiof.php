<?php

// inicia sistema
include 'config.inc';
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . 'includes/jpgraph/jpgraph.php';
include APPRAIZ . 'includes/jpgraph/jpgraph_pie.php';
include APPRAIZ . 'includes/jpgraph/jpgraph_pie3d.php';
include APPRAIZ . 'includes/jpgraph/jpgraph_bar.php';
include APPRAIZ . "financeiro/modulos/relatorio/funcoes_consulta_financeiro.inc";
$db = new cls_banco();

// FUNÇÕES ---------------------------------------------------------------------
	
	/**
	 * Formata um número para ser impresso no gráfico
	 *
	 * @param string $val
	 * @return string
	 */
	function formataValorGrafico( $val )
	{
		return number_format( $val, 0, ",", "." );
	}
	
// FIM FUNÇÕES -----------------------------------------------------------------

// CRIA LEGENDAS ---------------------------------------------
	
	$legendas = array(
		'barra' => array(
			'Lei + Crédito',
			'Empenhado',
			'Liquidado',
			'Pago'
		),
		// a legenda do gráfico do tipo acumulado é igual ao de pizza
		'pizza' => array(
			'Dotação disponível',
			'Empenhado e não pago',
			'Liquidado e não pago',
			'Pago'
		)
	);
	
// FIM CRIA LEGENDAS -----------------------------------------

$tipo = $_REQUEST['tipo'];
$itens = cfBuscarItem( $_SESSION['consulta_financeira']['itens'], $_REQUEST['rastro'] );
$total = cfCalcularValorTotal( $itens );
$totalAgrupado = cfCalculaValorAgrupado( $itens );

//dbg( $totalAgrupado, 1 );

switch ( $tipo )
{
	// ACUMULADO ---------------------------------------------------------------
	case 'acumulado':
		// prepara dados
		$legenda = $legendas['pizza'];
		$label = array();
		$valor0 = array();
		$valor1 = array();
		$valor2 = array();
		$valor3 = array();
		foreach ( $totalAgrupado as $valores )
		{
			$valor0[] = $valores['pago'];
			$valor1[] = $valores['liquidado'] - $valores['pago'];
			$valor2[] = $valores['empenhado'] - $valores['liquidado'];
			$valor3[] = $valores['autorizado_valor'] - $valores['empenhado'];
			$label[] = (string) $valores['cod'];
		}
		
		// monta plotagem
		$b0plot = new BarPlot( $valor0 );
		$b0plot->SetFillColor( 'lightred' );
		$b0plot->SetLegend( $legenda[3] );
		$b1plot = new BarPlot( $valor1  );
		$b1plot->SetFillColor( 'lightblue' );
		$b1plot->SetLegend( $legenda[2] );
		$b2plot = new BarPlot( $valor2 );
		$b2plot->SetFillColor( 'lightgreen' );
		$b2plot->SetLegend( $legenda[1] );
		$b3plot = new BarPlot( $valor3 );
		$b3plot->SetFillColor( 'lightyellow' );
		$b3plot->SetLegend( $legenda[0] );
		$plot = array( $b0plot, $b1plot, $b2plot, $b3plot );
		
		// monta gráfico
		$largura = 300 + ( count( $totalAgrupado ) * 55 );
		$graph = new Graph( $largura, 250, 'auto' ); 
		$graph->SetScale( 'textlin' );
		$graph->SetMarginColor( 'white' );
		$graph->SetFrameBevel( 0, false );
		$graph->img->SetMargin( 90, 190, 20, 50 );
		$graph->title->Set( $titulo );
		$graph->title->SetColor( 'black' );
		$graph->title->SetFont( FF_ARIAL, FS_BOLD, 10 );
		$graph->subtitle->Set( $subtitulo );
		$graph->subtitle->SetColor( 'darkslategray' );
		$graph->subtitle->SetFont( FF_ARIAL, FS_NORMAL, 10 );
		$graph->legend->Pos( 0.01, 0.38 );
		$graph->SetColor( 'white' );
		$graph->yaxis->SetColor( 'black', 'black' );
		$graph->yaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
		$graph->xaxis->SetFont( FF_ARIAL, FS_NORMAL, 8 );
		$graph->xaxis->SetColor( 'black', 'black' );
		$graph->xaxis->SetLabelAngle( 15 );
		$graph->xaxis->SetTickLabels( $label );
		$graph->legend->SetShadow( false );
		$graph->yaxis->SetLabelFormatCallback( 'formataValorGrafico' );
		
		// atribui plotagem
		$gbplot = new AccBarPlot( $plot );
		$gbplot->SetLegend( $legenda );
		
		// imprime imagem
		$graph->Add( $gbplot );
		$graph->Stroke();
		break;
	
	// FIM ACUMULADO -----------------------------------------------------------
	
	// BARRA -------------------------------------------------------------------
	case 'barra':
		// prepara dados
		$dados = array(
			$total['autorizado_valor'],
			$total['empenhado'],
			$total['liquidado'],
			$total['pago']
		);
		$legenda = $legendas['barra'];
		
		// monta gráfico
		$graph = new Graph( 500, 250, 'auto' );	
		$graph->SetFrameBevel( 0, false );
		$graph->img->SetMargin( 100, 30, 15, 20 );
		$graph->SetScale( 'textlin' );
		$graph->SetMarginColor( 'white' );
		$graph->title->Set( $titulo );
		$graph->title->SetColor( 'black' );
		$graph->title->SetFont( FF_ARIAL, FS_BOLD, 10 );
		$graph->subtitle->Set( $subtitulo );
		$graph->subtitle->SetColor( 'darkslategray' );
		$graph->subtitle->SetFont( FF_ARIAL, FS_NORMAL, 10 ); 
		$graph->yaxis->SetColor( 'black', 'black' );
		$graph->yaxis->SetFont( FF_VERDANA, FS_NORMAL, 8 );
		$graph->yaxis->SetLabelFormatCallback( 'formataValorGrafico' );
		$graph->xaxis->SetColor( 'black', 'black' );
		$graph->xaxis->SetFont( FF_ARIAL, FS_NORMAL, 8 );
		$graph->xaxis->SetTickLabels( $legenda );
		
		// monta plotagem
		$bplot = new BarPlot( $dados );
		$bplot->SetWidth( 0.3 );
		$tcol = array( 255, 210, 100 );
		$fcol = array( 255, 245, 220 );
		$bplot->SetFillGradient( $fcol, $tcol, GRAD_HOR );
		
		// imprime imagem
		$graph->Add( $bplot );
		$graph->Stroke();
		break;
	
	// FIM BARRA ---------------------------------------------------------------
	
	// PIZZA -------------------------------------------------------------------
	case 'pizza':
		// prepara dados
		$dados = array(
			$total['autorizado_valor'] - $total['empenhado'],
			$total['empenhado'] - $total['liquidado'],
			$total['liquidado'] - $total['pago'],
			$total['pago']
		);
		if ( $dados[0] < 0 )
		{
			$dados[0] = 0;
		}
		if ( $dados[1] < 0 )
		{
			$dados[1] = 0;
		}
		if ( $dados[2] < 0 )
		{
			$dados[2] = 0;
		}
		if ( $dados[3] < 0 )
		{
			$dados[4] = 0;
		}
		$legenda = $legendas['pizza'];
		
		// monta gráfico
		$graph = new PieGraph( 500, 250, 'auto' );
		$graph->SetFrameBevel( 0, false );
		$graph->title->Set( $titulo );
		$graph->title->SetFont( FF_ARIAL, FS_BOLD, 10 ); 
		$graph->title->SetColor( 'black' );
		$graph->subtitle->Set( $subtitulo );
		$graph->subtitle->SetFont( FF_ARIAL, FS_NORMAL, 10 ); 
		$graph->subtitle->SetColor( 'darkslategray' );
		$graph->legend->Pos( 0.02, 0.36 );
		$graph->legend->SetShadow( false );
		
		// monta plotagem
		$p1 = new PiePlot3d( $dados );
		$p1->SetSliceColors( array( 'lightred', 'lightblue', 'lightgreen', 'lightyellow' ) );
		$p1->SetCenter( 0.32, 0.48 );
		$p1->SetSize( 120 );
		$p1->SetAngle( 40 );
		$p1->SetStartAngle( 0 );
		$p1->value->SetFont( FF_ARIAL, FS_NORMAL, 9 );
		$p1->value->SetColor( 'darkslategray' );
		//$p1->SetEdge( 'darkslategray' );
		$p1->SetLegends( $legenda );
		
		// imprime imagem
		$graph->Add( $p1 );
		$graph->Stroke();
		break;
	
	// FIM PIZZA ---------------------------------------------------------------
}

?>