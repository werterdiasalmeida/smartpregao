<?php

	$pjeid = $_SESSION['pjeid'];
	$cod = $_REQUEST['cod'];

function sortmddata($array, $by, $order, $type)
{
	$sortby = "sort$by"; //This sets up what you are sorting by
	$firstval = current($array); //Pulls over the first array
	$vals = array_keys($firstval); //Grabs the associate Arrays
	foreach ($vals as $init)
	{
    	$keyname = "sort$init";
    	$$keyname = array();
	}
	foreach ($array as $key => $row)
	{
		foreach ($vals as $names)
		{
    		$keyname = "sort$names";
    		$test = array();
    		$test[$key] = $row[$names];
    		$$keyname = array_merge($$keyname,$test);
		}
	}
	if ($order == "DESC")
	{   
		if ($type == "num")
		{
			array_multisort($$sortby,SORT_DESC, SORT_NUMERIC,$array);
		}
		else 
		{
			array_multisort($$sortby,SORT_DESC, SORT_STRING,$array);
		}
	}
	else 
	{
		if ($type == "num")
		{
			array_multisort($$sortby,SORT_ASC, SORT_NUMERIC,$array);
		}
		else 
		{
			array_multisort($$sortby,SORT_ASC, SORT_STRING,$array);
		}
	}
	return $array;
}

require_once "config.inc";
	define( "APP_PLANO_TRABALHO" , APPRAIZ . "includes/planodetrabalho/tarefa_pt/" );
	define( "APP_PLANO_TRABALHO_ACAO" , APPRAIZ . "includes/planodetrabalho/tarefa_acao/" );
	
	require_once ( APPRAIZ . "includes/funcoes.inc" );
	require_once ( APPRAIZ . "includes/classes_simec.inc" );
	include_once ( APPRAIZ . "/includes/jpgraph/jpgraph.php" );
	include_once ( APPRAIZ . "/includes/jpgraph/jpgraph_gantt.php" );
	
	
	$db = new cls_banco();
	$nivel = @$_REQUEST['nivel'];
	if ( !$nivel ) $nivel = 1000;

	function abreviatura($nome)	
	{
		$str = str_split($nome);
    	$nomabrev=$str[0];
    	for( $j = 1 ; $j < count( $str ) ; $j++ )
    	{
	   		if ( ord( $str[ $j ] ) == 32 ) $nomabrev .= $str[ $j + 1 ];
     	}
		return $nomabrev;
	}
	
	function montaElementoPorTarefa( Tarefa $objTarefa )
	{
		$objElemento = array();
		$objElemento[ 'nome']				= $objTarefa->getNome();
		$objElemento[ 'dataInicio'] 		= date( 'm/d/Y' , $objTarefa->getDataInicioTimestamp() );
		$objElemento[ 'dataFim']			= date( 'm/d/Y' , $objTarefa->getDataFimTimestamp() );
		$objElemento[ 'previsto']			= '0';
		$objElemento[ 'realizado']			= '0';
		$objElemento[ 'gasto']				= '1000';
		$objElemento[ 'porcentagem']		= $objTarefa->getPrevisaoMeta();
		try
		{
			$objElemento[ 'nomeResponsavel']	= $objTarefa->getDono() ? $objTarefa->getDono()->getNome() : '';
		}
		catch( Exception $objException )
		{
			$objElemento[ 'nomeResponsavel']	= '';
		}
		$objElemento[ 'qtdFilhos']			= $objTarefa->getQuantidadeDeTarefasFilhas();
		$objElemento[ 'codigo']				= $objTarefa->getCodigoUnico();
		return $objElemento;
	}
	
	function geraArrElementosPorProjeto( Projeto $objProjeto , $intTarefaPai = null, $intProfundidadeLimite = null, $strUsuarioRestricao = null )
	{
		if( $intTarefaPai )
		{
			$objTarefa = new TarefaPT();
			$objTarefa = $objTarefa->getTarefaPeloId( $intTarefaPai );
			$arrNivelRaiz = array( $objTarefa );
		}
		else
		{
			$arrNivelRaiz = $objProjeto->getArrTarefasDoProjeto();
		}
		$arrNivelRaiz = orderArrayOfObjectsByMethod( $arrNivelRaiz , 'getDataInicio' );
		geraArrDescendentesEmProfundidade( $arrNivelRaiz , $arrTarefasEmProfundidade , 1 , $intProfundidadeLimite );
		
		$arrElementos = array();
		foreach( $arrTarefasEmProfundidade as $objTarefa )
		{
			$objElemento = montaElementoPorTarefa( $objTarefa );
			$arrElementos[] = $objElemento;
		}
		return $arrElementos;
	}
	
	function geraArrElementosPorAcao( Acao $objAcao , $intTarefaPai = null, $intProfundidadeLimite = null, $strUsuarioRestricao = null )
	{
		if( $intTarefaPai )
		{
			$objTarefa = new TarefaAcao();
			$objTarefa = $objTarefa->getTarefaPeloId( $intTarefaPai );
			$arrNivelRaiz = array( $objTarefa );
		}
		else
		{
			$arrNivelRaiz = $objAcao->getArrTarefasDaAcao();
		}
		$arrNivelRaiz = orderArrayOfObjectsByMethod( $arrNivelRaiz , 'getDataInicio' );
		geraArrDescendentesEmProfundidade( $arrNivelRaiz , $arrTarefasEmProfundidade , 1 , $intProfundidadeLimite );
		
		$arrElementos = array();
		foreach( $arrTarefasEmProfundidade as $objTarefa )
		{
			$objElemento = montaElementoPorTarefa( $objTarefa );
			$arrElementos[] = $objElemento;
		}
		return $arrElementos;
	}
	
	function geraArrDescendentesEmProfundidade( $arrTarefas , &$arrTodosOsNiveis , $intProfundidadeAtual = 1 , $intProfundidadeMaxima = null )
	{
		$arrTarefas = orderArrayOfObjectsByMethod(  $arrTarefas  , 'getPosicao' );
		foreach( $arrTarefas as $objTarefa )
		{
			$arrTodosOsNiveis[] = $objTarefa;
			if( ( $intProfundidadeAtual < $intProfundidadeMaxima ) || ( $intProfundidadeMaxima === null ) )
			{
				$arrFilhas = $objTarefa->getArraydeTarefasqueContenho();
				$arrFilhas = orderArrayOfObjectsByMethod( $arrFilhas , 'getDataInicio' );
				geraArrDescendentesEmProfundidade( $arrFilhas , $arrTodosOsNiveis , $intProfundidadeAtual + 1 ,$intProfundidadeMaxima );
			}
		}
	}
	
	function montaGantt( $strTitulo , $subTitulo , $arrElementos , $dataLimiteInicioTimestamp , $dataLimiteFimTimestamp )
	{
		
		#0. Monta o Elemento Gantt
		$graph = new GanttGraph( 0 , 0 , 'auto');
		$graph->SetShadow();
		
		#1. Add title and subtitle
		$graph->title->Set( $strTitulo );
		$graph->title->SetFont(FF_ARIAL,FS_BOLD,8);
		$graph->subtitle->Set( $subTitulo );
		
		
		$intPeriodo = $dataLimiteFimTimestamp - $dataLimiteInicioTimestamp;

		$intSegundo = 1;
		$intMinuto	= 60	* $intSegundo;
		$intHora	= 60	* $intMinuto;
		$intDia 	= 24	* $intHora;
		$intMes		= 30	* $intDia;
		$intAno		= 365	* $intDia;
		
		$arrEscalas = array();
		
		#3. Adaptando os estilos conforme a escala
		
		if ( $intPeriodo < ( 3 * $intMes ) )
		{
			$arrEscalas[ 'dias' ] = DAYSTYLE_ONELETTER;
			$arrEscalas[ 'meses' ] = MONTHSTYLE_LONGNAMEYEAR4;			
		}
		else if ( $intPeriodo < 6 * $intMes )
		{
			$arrEscalas[ 'dias' ] = DAYSTYLE_SHORTDATE4;
			$arrEscalas[ 'meses' ] = MONTHSTYLE_LONGNAMEYEAR4;			
						
		}
		else if ( $intPeriodo < $intAno )
		{
			$arrEscalas[ 'semanas' ] = WEEKSTYLE_FIRSTDAY;
			$arrEscalas[ 'meses' ] = MONTHSTYLE_LONGNAMEYEAR4;			
		}
		else if ( $intPeriodo < 4 * $intAno )
		{
			$arrEscalas[ 'meses' ] = MONTHSTYLE_SHORTNAME;	
		}
		else if ( $intPeriodo < 6 * $intAno )
		{
			$arrEscalas[ 'anos' ] = 1;	
		}
		else
		{
			$arrEscalas[ 'anos' ] = 2;	
		}
		
		foreach( $arrEscalas as $strScale => $strStyle )
		{
			switch( $strScale )
			{
				case 'dias':
				{
					$graph->ShowHeaders(GANTT_HDAY | GANTT_HMONTH );
					$graph->scale->day->setStyle( $strStyle );
					break;	
				}
				case 'semanas':
				{
					$graph->ShowHeaders(GANTT_HWEEK | GANTT_HMONTH );
					$graph->scale->week->setStyle( $strStyle );
					$graph->scale->week->SetFont(FF_FONT0);
					break;	
				}
				case 'meses':
				{
					$graph->ShowHeaders( GANTT_HMONTH );
					$graph->scale->month->setStyle( $strStyle );
					break;	
				}
				case 'anos':
				{
					if( $strStyle == 1 )
					{
						$graph->ShowHeaders( GANTT_HMONTH | GANTT_HYEAR );
					}
					else
					{
						$graph->ShowHeaders( GANTT_HYEAR );
					}
					break;	
				}
			}
		}
		
		
		#6. Format the bar for the first activity
		# ($row,$title,$startdate,$enddate)
		foreach( $arrElementos as $key => $objElemento )
		{
			#7.	Gambi antiga que eu ainda nao vou mecher
			
			$espaco='';
			for ($n = 2 ; $n <= $objElemento[ 'nivel' ] ; $n++ )
			{
				$espaco .='__';
			}
			
			$strDescricao = $espaco . $objElemento[ 'codigo' ] . '-' . $objElemento[ 'nome' ];
			
			$txtAlt =
					"Nome: "		. $objElemento[ 'nome' ]	. "<br />" .
					"Início: "		. $objElemento[ 'dataInicio' ]	. "<br />" .
					"Término: " 	. $objElemento[ 'dataFim']		. "<br />" .
					"Previsto: " 	. formata_valor( $objElemento[ 'previsto' ]		, 0 ) . "<br/>" .
					"Executado: "	. formata_valor( $objElemento[ 'realizado' ]	, 0 ) . "<br/>" .
					"Gasto: R$ "	. formata_valor( $objElemento[ 'gasto' ]		, 0 ) . "<br/>" .
					"Responsavel: "	. $objElemento[ 'nomeResponsavel' ]. "<br/>" .
					"Percentual: "	. formata_valor( $objElemento[ 'porcentagem' ]	, 2 ) . "%";
	
			#8.
			$activity = new GanttBar(
				$key, 
				$strDescricao, 
				$objElemento[ 'dataInicio' ], 
				$objElemento[ 'dataFim'],
				abreviatura( $objElemento[ 'nomeResponsavel' ] )
			);
			
			
			$activity->SetCSIMTarget( '#" onmouseover="return escape(\'' . $txtAlt .'\')"' , 'Go back 1' );
			
			#9. Yellow diagonal line pattern on a red background
			
			$activity->SetPattern( BAND_RDIAG , "blue" );
			
			if( $objElemento[ 'qtdFilhos' ] > 0 )
			{
				$activity->SetFillColor("gray"); 
			}
			else
			{
				$activity->SetFillColor("red");
			}
			
			if ( $objElemento[ 'porcentagem' ] > 100 )
			{
				$objElemento[ 'porcentagem' ] = 100;
			}
			
			$activity->progress->Set( $objElemento[ 'porcentagem' ] / 100 );
			
			#10. Finally add the bar to the graph
			$graph->Add( $activity );
		}
		
		#11. ... and display it
		$graph->StrokeCSIM( 'gantt.php' );
		?>
			<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 		
		<?
	}
	
	$intContainerId			= $_REQUEST[ 'containerId' ];
	$intTarefaId 			= $_REQUEST[ 'tarefa' ];
	$intNivel 				= $_REQUEST[ 'nivel' ];
	$strClasseContainer		= $_REQUEST[ 'container' ];
	$strUsuarioRestricao	= $_REQUEST[ 'usuario' ];

	switch( $strClasseContainer )
	{
		case 'Projeto':
		{
			require_once ( APP_PLANO_TRABALHO . 'arquivos_requeridosPT.inc' );
			$objProjeto = new ProjetoPT();
			$objProjeto = $objProjeto->getProjetoPeloId( $intContainerId );
			$arrElementos = geraArrElementosPorProjeto( $objProjeto , $intTarefaId , $intNivel , $strUsuarioRestricao );
			$strNome =  $objProjeto->getNome();
			$strTitulo = $objProjeto->getDataInicio() . ' - ' . $objProjeto->getDataFim();
			$dataLimiteInicio =  $objProjeto->getDataInicioTimestamp();
			$dataLimiteFim =  $objProjeto->getDataFimTimestamp();
			break;
		}
		case 'Acao':
		{
			require_once ( APP_PLANO_TRABALHO_ACAO . 'arquivos_requeridosAcao.inc' );
			$objAcao = new Acao();
			$objAcao = $objAcao->getAcaoPeloId( $intContainerId );
			$arrElementos = geraArrElementosPorAcao( $objAcao , $intTarefaId , $intNivel , $strUsuarioRestricao );
			$strNome =  $objAcao->getNome();
			$strTitulo = $objAcao->getDataInicio() . ' - ' . $objAcao->getDataFim();
			$dataLimiteInicioTimestamp =  $objAcao->getDataInicioTimestamp();
			$dataLimiteFimTimestamp =  $objAcao->getDataFimTimestamp();
			break;
		}
		default:
		{
			throw new Exception( 'Tipo de Classe Container Invalido' );
		}
	}
	
	montaGantt( $strNome , $strTitulo , $arrElementos , $dataLimiteInicioTimestamp , $dataLimiteFimTimestamp );
?>