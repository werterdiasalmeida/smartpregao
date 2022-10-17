<?php

/**
 * Manipula um objeto BackTrace e monta um HTML com seus detalhes.
 * 
 * Manipulates an BackTrace object and create a HTML with its details.
 * 
 * Constrói dados de um rastreamento de uma forma mais agradável que, por
 * exemplo, a chamada da função {@link var_dump()} tendo como parâmetro
 * {@link debug_backtrace()}. A classe foi inspirada em um nota lançada no link
 * {@link http://www.php.net/manual/en/function.debug-backtrace.php#47644}.
 * Caso os manipuladores de erros ou exceções do seu sistema utilize essa classe
 * é altamente recomendado que use também
 * {@link http://php.net/manual/en/ref.outcontrol.php controle de saída} para
 * que o rastreamento seja melhor visualizado e não seja misturado com outros
 * fontes HTML. A função {@link highlight_string()} é utilizada, qualquer
 * modificação no formato retornado implica na necessidade de revisão dessa
 * classe.
 * 
 * @author Renan de Lima Barbosa <renandelima@gmail.com>
 */
class BackTraceExplain
{
	
	/**
	 * Quantidade de linhas de código visíveis.
	 * 
	 * Amount of visible lines code.
	 * 
	 * Quantidade de linhas a serem exibidas no detalhamento antes de depois de
	 * cada mudança de escopo. O valor zero é altamente recomendado para
	 * sistemas em produção. O valor -1 ou qualquer outro negativo faz com que
	 * todas as linhas do arquivo onde ocorreu a mudança de escopo sejam
	 * exibidas.
	 * 
	 * @var integer
	 */
	const CONTEXT_LINES = 5;
	
	/**
	 * Cache de código fontes.
	 * 
	 * Source code cache.
	 * 
	 * @see BackTraceExplain::fetchSource()
	 * @var array
	 */
	protected static $aFileContent = array();
	
	/**
	 * Gerador de identificadores para detalhamento.
	 * 
	 * Identifiers generator for detailing.
	 * 
	 * @var integer
	 */
	protected static $iExplainId = 0;
	
	/**
	 * Nível inicial do rastreamento.
	 * 
	 * Trace start level.
	 * 
	 * @var integer
	 */
	protected $iStartLevel = 0;
	
	/**
	 * Rastramento a ser detalhado.
	 * 
	 * Trace to detailing.
	 * 
	 * @var BackTrace
	 */
	protected $oBackTrace = null;
	
	/**
	 * Nome da função JavaScript para ocultar e exibir elementos HTML.
	 * 
	 * JavaScript function name for to hide and show HTML elements.
	 * 
	 * @var string
	 */
	protected $sScriptFunctionName = '';
	
	/**
	 * @ignore
	 */
	protected function __construct(){}
	
	/**
	 * Captura resumo dos argumentos do nível atual do rastreamento.
	 * 
	 * Captures arguments summary of current trace level.
	 * 
	 * @return string[]
	 */
	protected function fetchArguments()
	{
		$aArgument = $this->oBackTrace->getArguments();
		if ( count( $aArgument ) === 0 )
		{
			return array();
		}
		$aReturn = array();
		foreach ( $aArgument as $mArgument )
		{
			switch( gettype( $mArgument ) )
			{
				case 'array':
					$sSummary = 'array( ' . count( $mArgument ) . ' )';
					break;
				case 'boolean':
					$sSummary = $mArgument === true ? 'true' : 'false';
					break;
				case 'double':
				case 'integer':
					$sSummary = (string) $mArgument;
					break;
				case 'NULL':
					$sSummary = 'null';
					break;
				case 'object':
					$sSummary = 'object( ' . htmlentities( get_class( $mArgument ) ) . ' )';
					break;
				case 'resource':
					$sSummary = 'resource( ' . htmlentities( strstr( $mArgument, '#' ) . ' ' . get_resource_type( $mArgument ) ) . ' )';
					break;
				case 'string':
					/*if ( strlen( $mArgument ) > 15 )
					{
						$mArgument = substr( $mArgument, 0, 12 ) . '...';
					}*/
					$sSummary = htmlentities( '"' . $mArgument . '"' );
					break;
				default:
					$sSummary = 'unknown type ' . htmlentities( gettype( $mArgument ) );
					break;
			}
			array_push( $aReturn, $sSummary );
		}
		return $aReturn;
	}
	
	/**
	 * Constrói HTML da chamada do nível atual do rastreamento.
	 * 
	 * Creates HTML of current trace level call.
	 * 
	 * @return string
	 */
	protected function fetchCall()
	{
		if ( $this->oBackTrace->getScope() === BackTrace::SCOPE_FILE )
		{
			return '';
		}
		$iLevel = self::$iExplainId + $this->oBackTrace->getLevel();
		$sAnchor = '';
		$sArgument = '';
		foreach ( $this->fetchArguments() as $sSummary )
		{
			$sArgument .= ', ' . $sSummary;
		}
		if ( $sArgument !== '' )
		{
			$sArgument = substr( $sArgument, 1 ) . ' ';
		}
		$sClass = $this->oBackTrace->getClass();
		$sType = $this->oBackTrace->getType();
		$sFunction = $this->oBackTrace->getFunction();
		return htmlentities( $sClass . $sType . $sFunction ) . '(' . $sArgument . ')';
	}
	
	/**
	 * Constrói detalhes do nível atual do rastreamento.
	 * 
	 * Generates details of current trace level.
	 * 
	 * @return string
	 */
	protected function fetchLevel()
	{
		$sLocation = htmlentities( $this->oBackTrace->getFile() ) . ' on line ' . $this->oBackTrace->getLine();
		$sSource = $this->fetchSource();
		$sSourceId = 'BackTraceExplainSource' . ( self::$iExplainId + $this->oBackTrace->getLevel() );
		if ( $sSource !== '' )
		{
			$sLocation =
				'<a href="#" onclick="this.blur(); ' . $this->sScriptFunctionName . '( \'' . $sSourceId . '\' ); return false;">' .
					$sLocation .
				'</a>';
		}
		return
			'<div class="traceCall">' .
				$this->fetchCall() .
			'</div>' .
			'<div class="traceLocation">' .
				$sLocation .
			'</div>' .
			'<div class="traceSource" id="' . $sSourceId . '">' .
				$sSource .
			'</div>';
	}
	
	/**
	 * Constrói HTML com código do arquivo do nível atual do rastreamento.
	 * 
	 * Creates HTML with file source code of current trace level.
	 * 
	 * Caso a constante {@link BackTraceExplain::CONTEXT_LINES} possua valor
	 * zero ou arquivo do nível atual do rastreamento não exista ou não seja
	 * legível um texto vazio é retornado. A função {@link highlight_string()} é
	 * utilizada, qualquer modificação no formato retornado implica na
	 * necessidade de revisão desse método.
	 * 
	 * @return string
	 * @see BackTraceExplain::CONTEXT_LINES
	 */
	protected function fetchSource()
	{
		$sFile = $this->oBackTrace->getFile();
		if ( self::CONTEXT_LINES == 0 || file_exists( $sFile ) === false || is_readable( $sFile ) === false )
		{
			return '';
		}
		// verifica se existe cache
		if ( array_key_exists( $sFile, self::$aFileContent ) === false )
		{
			self::$aFileContent[$sFile] = substr( highlight_string( file_get_contents( $sFile ), true ), 6, -7 );
		}
		$sSource = '';
		$aLine = explode( '<br />', self::$aFileContent[$sFile] );
		$iLine = $this->oBackTrace->getLine();
		// define linha inicial e final a ser exibida
		if ( self::CONTEXT_LINES < 0 )
		{
			$iLineStart = 1;
			$iLineEnd = count( $aLine );
		}
		else
		{
			$fEdge = ( self::CONTEXT_LINES - 1 ) / 3;
			$iLineStart = $iLine - ceil( $fEdge * 2 );
			$iLineEnd = $iLine + floor( $fEdge );
			if ( $iLineStart < 1 )
			{
				$iLineEnd += abs( $iLineStart ) + 1;
				$iLineStart = 1;
			}
			$iSourceLength = count( $aLine );
			if ( $iLineEnd > $iSourceLength )
			{
				$iLineStart -= $iLineEnd - $iSourceLength;
				if ( $iLineStart < 1 )
				{
					$iLineStart = 1;
				}
				$iLineEnd = $iSourceLength;
			}
		}
		// monta texto de deteção da primeira linha, que precisa de tratamento especial
		$sFirstLineDetect = "<span style=\"color: #000000\">\n<span style=\"color: #0000BB\">";
		$iFirstLineDetectLength = strlen( $sFirstLineDetect );
		// percorre linhas de código
		foreach ( $aLine as $iCurrentLine => $sLineContent )
		{
			// trata linha corrente para contagem iniciar no número 1
			$iCurrentLine++;
			// verifica se linha deve estar visível
			if ( $iCurrentLine < $iLineStart || $iCurrentLine > $iLineEnd )
			{
				// remove todo conteúdo que existir entre duas marcações HTML
				$aMatch = array();
				preg_match_all( '(<[^>]+>)', $sLineContent, $aMatch );
				if ( count( $aMatch ) > 0 && is_array( $aMatch[0] ) === true )
				{
					$sLineContent = str_replace( array( '<br/>', '&nbsp;', "\n" ), '', implode( '', $aMatch[0] ) );
				}
			}
			else
			{
				// verifica se é a primeira linha
				if ( substr( $sLineContent, 0, $iFirstLineDetectLength ) === $sFirstLineDetect )
				{
					// retira o prefixo para evitar erros de tratamento do código
					// o prefixo é adiciona de volta a linha ao final do tratamento da linha atual
					$sLineContent = substr( $sLineContent, $iFirstLineDetectLength );
					$sLinePrefix = $sFirstLineDetect;
				}
				else
				{
					$sLinePrefix = '';
				}
				// prepara linha para que ela receba a numeração com uma classe de estilo
				// adiciona uma quebra de linha visível ao final da linha
				$sLineContent = '</span>' . $sLineContent . '<br/>';
				// verifica se linha deve ser destacada
				if ( $iCurrentLine !== $iLine )
				{
					$sClass = 'traceNumberLineOff';
				}
				else
				{
					// destaca número da linha
					// adiciona classe de estilo para destaque de todo conteúdo da linha
					$sClass = 'traceNumberLineOn';
					$sLineContent = preg_replace( '/>([^<]*)</', '><span class="traceLineActive">$1</span><', $sLineContent );
				}
				$sLineContent =
					$sLinePrefix .
					'<span class="' . $sClass . '">' .
						sprintf( '%0' . strlen( $iLineEnd ) . 'd', $iCurrentLine ) .
						$sLineContent;
			}
			$sSource .= $sLineContent;
		}
		return $sSource;
	}
	
	/**
	 * Constrói HTML com detalhes de todos os níveis de um rastreamento.
	 * 
	 * Creates HTML with details of all trace levels.
	 * 
	 * Os níveis são detalhados a partir do atual. O nível do rastreamento
	 * permanece o mesmo após a execução desse método.
	 * 
	 * @param BackTrace $oBackTrace rastreamento a ser detalhado
	 * @return string
	 */
	public static function perform( BackTrace $oBackTrace = null )
	{
		if ( $oBackTrace === null )
		{
			$oBackTrace = new BackTrace();
			$oBackTrace->levelDown();
		}
		$iOriginalLevel = $oBackTrace->getLevel();
		$oExplain = new self();
		$oExplain->oBackTrace = $oBackTrace;
		$oExplain->iStartLevel = $oBackTrace->getLevel();
		$oExplain->sScriptFunctionName = 'BackTraceExplainFunction' . self::$iExplainId;
		$sTrace = '';
		do
		{
			$sTrace .=
				'<li class="traceLevel">' .
					$oExplain->fetchLevel() .
				'</li>';
		}
		while ( $oBackTrace->levelDown() === true );
		self::$iExplainId += $oBackTrace->getLevel() - $iOriginalLevel;
		$oBackTrace->setLevel( $iOriginalLevel );
		$sDir = dirname( __FILE__ ) . '/resource/';
		return
			'<style type="text/css">' .
				file_get_contents( $sDir . 'style.css' ) .
			'</style>' .
			'<script type="text/javascript">' .
				str_replace( '_TRACE_FUNCTION_', $oExplain->sScriptFunctionName, file_get_contents( $sDir . 'script.js' ) ) .
			'</script>' .
			'<ol class="trace">' .
				$sTrace .
			'</ol>';
	}
	
}
