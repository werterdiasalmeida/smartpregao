<?php

/**
 * Fornece dados da pilha de execução.
 * 
 * Supplies execution stack data.
 * 
 * Manipula e fornece acesso aos dados de uma determinada pilha de execução. Por
 * meio de métodos é possível caminhar entre os níveis e capturar informações
 * respectivas a cada um deles.
 * 
 * @author Renan de Lima Barbosa <renandelima@gmail.com>
 */
class BackTrace
{
	
	/**
	 * Indica que o escopo é de arquivo.
	 * 
	 * Indicates the scope is an file.
	 * 
	 * É um dos possíveis valores retornado pelo método
	 * {@link BackTrace::getScope()}. Indica que o ponteiro de execução faz
	 * referência à uma instrução cujo escopo é um arquivo.
	 * 
	 * @var string
	 */
	const SCOPE_FILE = 'file';
	
	/**
	 * Indica que o escopo é de função.
	 * 
	 * Indicates the scope is an function.
	 * 
	 * É um dos possíveis valores retornado pelo método
	 * {@link BackTrace::getScope()}. Indica que o ponteiro de execução faz
	 * referência à uma instrução localizada em uma função. Para verificar se o
	 * escopo é uma função de classe utilize as constantes
	 * {@link BackTrace::SCOPE_METHOD} e {@link BackTrace::SCOPE_STATIC}.
	 * 
	 * @var string
	 */
	const SCOPE_FUNCTION = 'function';
	
	/**
	 * Indica que o escopo é de método.
	 * 
	 * Indicates the scope is an method.
	 * 
	 * É um dos possíveis valores retornado pelo método
	 * {@link BackTrace::getScope()}. Indica que o ponteiro de execução faz
	 * referência à uma instrução localizada em um método. Para verificar se o
	 * escopo é um método estático utilize a constante
	 * {@link BackTrace::SCOPE_STATIC}.
	 * 
	 * @var string
	 */
	const SCOPE_METHOD = '->';
	
	/**
	 * Indica que o escopo é de método estático.
	 * 
	 * Indicates the scope is an static method.
	 * 
	 * É um dos possíveis valores retornado pelo método
	 * {@link BackTrace::getScope()}. Indica que o ponteiro de execução faz
	 * referência à uma instrução localizada em um método estático. Para
	 * verificar se o escopo é um método não estático utilize a constante
	 * {@link BackTrace::SCOPE_METHOD}.
	 * 
	 * @var string
	 */
	const SCOPE_STATIC = '::';
	
	/**
	 * Contém os dados do nível corrente.
	 * 
	 * Contains data of current level.
	 * 
	 * Ponteiro para um item da pilha {@link BackTrace::$aStack}. Os métodos
	 * {@link BackTrace::levelDown()} e {@link BackTrace::levelUp()} são
	 * responsáveis por manipular este atributo.
	 * 
	 * @var &string[]
	 */
	private $aCurrent = array();
	
	/**
	 * Contém os dados de todos os níveis.
	 * 
	 * Contains data of all levels.
	 * 
	 * Possui uma pilha com os dados de todos os níveis do rastreamento. O
	 * conteúdo deste atributo é o mesmo retornado pela função
	 * {@link debug_backtrace()}.
	 * 
	 * @var string[][]
	 */
	private $aStack = array();
	
	/**
	 * Indicador do nível atual.
	 * 
	 * Pointer of current level.
	 * 
	 * Contém o índice do escopo atual. O valor desta variável é um índice de
	 * {@link BackTrace::$aStack}.
	 * 
	 * @var integer
	 */
	private $iLevel = 0;
	
	/**
	 * Captura dados da pilha de execução.
	 * 
	 * Captures execution stack data.
	 * 
	 * Os dados da pilha de execução são capturados e o ponteiro é movido para o
	 * nível imediatamente inferior ao ponto de chamada deste construtor caso a
	 * pilha não seja informado pelo parâmetro, caso contrário a pilha informada
	 * é utilizada.
	 * 
	 * @param string[][] $aStack
	 * @return void
	 */
	public function __construct( array $aStack = array() )
	{
		$this->aStack = count( $aStack ) === 0 ? debug_backtrace() : $aStack;
		$this->levelTop();
	}
	
	/**
	 * Gera HTML com detalhes do rastreamento a partir do nível atual.
	 * 
	 * Creates HTML with detail of the trace.
	 * 
	 * Captura todos os dados de um rastreamento a partir do nível atual da
	 * pilha.
	 * 
	 * @return string
	 * @see BackTraceExplain::perform()
	 */
	public function explain()
	{
		return BackTraceExplain::perform( $this );
	}
	
	/**
	 * Obtém todos os dados de todos os níveis.
	 * 
	 * Gets all data of all levels.
	 * 
	 * Retorna os dados contidos na pilha construída pelo construtor dessa
	 * classe. O formato é idêntico ao retornado pela função
	 * {@link debug_backtrace()}.
	 * 
	 * @return string[]
	 */
	public function getAll()
	{
		return $this->aStack;
	}
	
	/**
	 * Obtém os argumentos de entrada do nível atual.
	 * 
	 * Captures input arguments of current level.
	 * 
	 * Se o escopo do nível corrente for uma função ou método retorna uma lista
	 * com os parâmetros de chamada. Caso contrário retorna uma lista vazia.
	 * 
	 * @return string[]
	 */
	public function getArguments()
	{
		if ( array_key_exists( 'args', $this->aCurrent ) === false )
		{
			return array();
		}
		return (array) $this->aCurrent['args'];
	}
	
	/**
	 * Obtém o nome da classe do nível atual.
	 * 
	 * Captures class name of current level.
	 * 
	 * Se o escopo do nível atual for um método ou método estático o nome da
	 * classe é retornado. Caso contrário retornará um texto vazio. Para
	 * verificar se o escopo atual é um método, método estático, função ou
	 * arquivo utilize {@link BackTrace::getScope()}.
	 * 
	 * @return string
	 */
	public function getClass()
	{
		if ( array_key_exists( 'class', $this->aCurrent ) === false )
		{
			return '';
		}
		return (string) $this->aCurrent['class'];
	}
	
	/**
	 * Obtém todos os dados do nível corrente.
	 * 
	 * Captures all data of current level.
	 *
	 * Retorna os dados do nível corrente da pilha de execução. O formato é
	 * igual a um item da pilha retornada pela função {@link debug_backtrace()}.
	 *
	 * @return string[]
	 */
	public function getCurrent()
	{
		return $this->aCurrent;
	}
	
	/**
	 * Obtém o nome do arquivo do nível corrente.
	 * 
	 * Captures file of current level.
	 * 
	 * Retorna o caminho completo contendo o nome do arquivo do nível atual.
	 * 
	 * @param boolean $bOnlyName captura somente o nome do arquivo
	 * @return string
	 */
	public function getFile( $bOnlyName = false )
	{
		if ( array_key_exists( 'file', $this->aCurrent ) === false )
		{
			return '';
		}
		if ( $bOnlyName === true )
		{
			return basename( (string) $this->aCurrent['file'] );
		}
		return (string) $this->aCurrent['file'];
	}
	
	/**
	 * Obtém o nome da função do nível corrente.
	 * 
	 * Captures function name of current level.
	 * 
	 * Se o escopo do nível atual for uma função, método ou método estático o
	 * nome desse é retornado. Caso contrário retornará um texto vazio. Para
	 * verificar se o escopo atual é uma função, método, método estático ou
	 * arquivo utilize {@link BackTrace::getScope()}.
	 * 
	 * @return string
	 */
	public function getFunction()
	{
		if ( array_key_exists( 'function', $this->aCurrent ) === false )
		{
			return '';
		}
		return (string) $this->aCurrent['function'];
	}
	
	/**
	 * Obtém a posição do nível atual do nível corrente.
	 * 
	 * Captures position of current level.
	 * 
	 * @return integer
	 */
	public function getLevel()
	{
		return $this->iLevel;
	}
	
	/**
	 * Obtém número da linha do arquivo do nível corrente.
	 * 
	 * Captures file line name of current level.
	 * 
	 * Caso o número da linha não esteja disponível o valor 0 é retornado.
	 * 
	 * @return integer
	 */
	public function getLine()
	{
		if ( array_key_exists( 'line', $this->aCurrent ) === false )
		{
			return 0;
		}
		return (integer) $this->aCurrent['line'];
	}
	
	/**
	 * Obtém o escopo do nível corrente.
	 * 
	 * Captures scope of current level.
	 * 
	 * O tipo de escopo define o ambiente do nível atual, ele pode ser função,
	 * método (função de classe), estático (método estático) ou arquivo. Esses
	 * tipos estão definidos nessa classe pelas constantes com prefixo "SCOPE_".
	 * Os possíveis valores retornados são: {@link BackTrace::SCOPE_FILE},
	 * {@link BackTrace::SCOPE_FUNCTION}, {@link BackTrace::SCOPE_METHOD} e
	 * {@link BackTrace::SCOPE_STATIC}. Os escopos estão definidos de forma
	 * parecida com o índice "type" da lista retornada pela função
	 * {@link debug_backtrace()}, porém existe diferenciação entre o escopo
	 * função e arquivo. Para obter o mesmo comportamento desse índice "type"
	 * utilize {@link BackTrace::getType()}.
	 * 
	 * @return string
	 */
	public function getScope()
	{
		$sType = $this->getType();
		if ( $sType === '' )
		{
			$sType = $this->getFunction() === '' ? self::SCOPE_FILE : self::SCOPE_FUNCTION;
		}
		return $sType;
	}
	
	/**
	 * Obtém o tipo do nível corrente.
	 * 
	 * Captures type of current level.
	 * 
	 * O tipo de escopo define o ambiente do nível atual, ele pode ser método
	 * (função de classe), estático (método estático) ou nenhum. Para obter um
	 * maior detalhamento do escopo utilize {@link BackTrace::getScope()}.
	 * 
	 * @return string
	 */
	public function getType()
	{
		if ( array_key_exists( 'type', $this->aCurrent ) === false )
		{
			return '';
		}
		return (string) $this->aCurrent['type'];
	}
	
	/**
	 * Move o ponteiro de rastreamento para o nível mais baixo.
	 * 
	 * Moves trace pointer to last level.
	 * 
	 * @return void
	 */
	public function levelBottom()
	{
		$this->setLevel( count( $this->aStack ) - 1 );
	}
	
	/**
	 * Move o ponteiro de rastreamento para baixo.
	 * 
	 * Moves trace pointer to down.
	 * 
	 * Quando o valor retronado é verdadeiro a classe passa a manipular as
	 * informações do nível imediatamente inferior ao atual. Caso contrário,
	 * quando o retorno é falso, o ponteiro não é alterado. A quantidade de
	 * níveis a ser abaixado é por padrão o valor 1, porém ele pode ser
	 * redefinido pelo primeiro parâmetro. Caso não seja possível descer a
	 * quantidade de níveis desejado o segundo parâmetro, que por padrão é
	 * falso, indica se é para descer até onde for possível.
	 * 
	 * @param integer $iDepth
	 * @param boolean $bTry caso o nível não exista tentar até onde for possível
	 * @return boolean
	 */
	public function levelDown( $iDepth = 1, $bTry = false )
	{
		$iDepth = (integer) $iDepth;
		$bReturn = $this->setLevel( $this->iLevel + $iDepth );
		if ( $bReturn === false && $bTry === true )
		{
			$iDepth > 0 ? $this->levelTop() : $this->levelBottom();
		}
		return $bReturn;
	}
	
	/**
	 * Move o ponteiro de rastreamento para o nível mais alto.
	 * 
	 * Moves trace pointer to first level.
	 * 
	 * @return void
	 */
	public function levelTop()
	{
		$this->setLevel( 0 );
	}
	
	/**
	 * Move o ponteiro de rastreamento para cima.
	 * 
	 * Moves trace pointer to up.
	 * 
	 * Quando o valor retronado é verdadeiro a classe passa a manipular as
	 * informações do nível imediatamente superior ao atual. Caso contrário,
	 * quando o retorno é falso, o ponteiro não é alterado. A quantidade de
	 * níveis a ser subido é por padrão o valor 1, porém ele pode ser redefinido
	 * pelo primeiro parâmetro. Caso não seja possível subir a quantidade de
	 * níveis desejado o segundo parâmetro, que por padrão é falso, indica se é
	 * para subir até onde for possível.
	 * 
	 * @param integer $iDepth
	 * @param boolean $bTry caso o nível não exista tentar até onde for possível
	 * @return boolean
	 */
	public function levelUp( $iDepth = 1, $bTry = false )
	{
		$iDepth = (integer) $iDepth;
		$bReturn = $this->setLevel( $this->iLevel - $iDepth );
		if ( $bReturn === false && $bTry === true )
		{
			$iDepth > 0 ? $this->levelBottom() : $this->levelTop();
		}
		return $bReturn;
	}
	
	/**
	 * Move o ponteiro de rastreamento.
	 * 
	 * Moves trace pointer.
	 * 
	 * Quando o valor retronado é verdadeiro a classe passa a manipular as
	 * informações do nível indicado pelo parâmetro. Caso contrário, quando o
	 * retorno é falso, o ponteiro não é alterado.
	 * 
	 * @param integer $iLevel nível do rastreamento
	 * @return boolean
	 */
	public function setLevel( $iLevel )
	{
		$iLevel = (integer) $iLevel;
		if ( array_key_exists( $iLevel, $this->aStack ) === true )
		{
			$this->iLevel = $iLevel;
		}
		$this->aCurrent = &$this->aStack[$this->iLevel];
		return $this->iLevel === $iLevel;
	}
	
}
