<?php
/*
 * Classe Modelo
 * Classe pai para as classes espelhos de banco
 * @author Orion Teles de Mesquita
 * @since 12/02/2009
 */
class Modelo extends cls_banco{

	protected $tabelaAssociativa = false;

    protected $arAtributos = [];

	public function __construct( $id = null ){

		parent::__construct();

		if( $id ){
			$this->carregarPorId( $id );
		}
	}

	public function __destruct(){

		if( isset($_SESSION['transacao'] ) ){
//			pg_query( parent::$link, 'rollback; ');
			unset( $_SESSION['transacao'] );
		}

	}

	/**
	 * Função __get
	 * Método mágico do PHP. Recupera o valor do atributo da classe caso não haja método get implementado.
	 * @param string $stAtributo - Nome do atributo que se deseja retornar
	 * @return string - Retorna o valor do atributo
	 * @access public
	 * @author Orion Teles de Mesquita
	 * @since 12/02/2009
	 * @final
	 */
    final public function __get( $stAtributo ){

    	if( property_exists( get_class( $this ), $stAtributo ) ) return $this->$stAtributo;
		elseif( array_key_exists( $stAtributo, $this->arAtributos ) ) return $this->arAtributos[$stAtributo];
		else trigger_error( "O atributo '$stAtributo' não existe!" );
    }

    /**
	 * Função __set
	 * Método mágico do PHP. Seta o valor do atributo da classe caso não haja método set implementado.
	 * @param string $stAtributo - Nome do atributo que se deseja setar
	 * @param string $valor - Valor do atributo
	 * @access public
	 * @author Orion Teles de Mesquita
	 * @since 12/02/2009
	 * @final
	 */
    final public function __set( $stAtributo, $valor ){

    	if( array_key_exists( $stAtributo, $this->arAtributos ) ) $this->arAtributos[$stAtributo] = $valor;
		else trigger_error( "O atributo '$stAtributo' não existe!" );
    }

	public function antesSalvar(){ return true; }

	public function depoisSalvar(){ return true; }

    /**
	 * Função salvar
	 * Método usado para inserção ou alteração de um registro do banco
	 * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
	 * @access public
	 * @author Orion Teles de Mesquita
	 * @since 12/02/2009
	 */
	public function salvar( $boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array() ){
		if( $boAntesSalvar ){
			if( !$this->antesSalvar() ){ return false; }
		}

		if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );

		$stChavePrimaria = $this->arChavePrimaria[0];

		if( $this->{$stChavePrimaria} && !$this->tabelaAssociativa ){
			$resultado = $this->alterar($arCamposNulo);
		}
		else{
			$resultado = $this->inserir();
		}
		if( $resultado ){
			if( $boDepoisSalvar ){
				$this->depoisSalvar();
			}
		}
		return $resultado;
	} // Fim salvar()

    /**
     * Persiste os $dados montando o carimbo e campos nulos de acordo com o $params
     * @param $params
     * @param $dados
     * @return bool|int
     */
    public function manter(array $params, array $dados = array()){
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        // Atribui os valores nulos quando tratados pelo limparDadosVazios
        foreach($params as $campo => $valor){
            $dados[$campo] = $valor;
        }

        $dados = $this->getDadosCarimbo($params, $dados);

        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    protected function limparDadosVazios($params)
    {
        $camposNulos = array();
        foreach ($params as $campo => $valor){
            if($valor === ''){
                $params[$campo] = null;
                $camposNulos[] = $campo;
            }
        }

        return array(
            $params,
            $camposNulos
        );
    }

    protected function getDadosCarimbo($params, $dados)
    {
        if(empty($params['id'])){
            $dadosCarimbo = array(
                'criado_em' => 'now()',
                'criado_por' => $_SESSION['usuario_id'],
            );
        }else{
            $dadosCarimbo = array(
                'atualizado_em' => 'now()',
                'atualizado_por' => $_SESSION['usuario_id'],
            );
        }

        return array_merge($dados, $dadosCarimbo);
    }

	/**
	 * Função _inserir
	 * Método usado para inserção de um registro do banco
	 * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
	 * @access private
	 * @author Orion Teles de Mesquita
	 * @since 12/02/2009
	 */
	public function inserir(){

		if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );

		$arCampos  = array();
		$arValores = array();
		$arSimbolos = array();

		foreach( $this->arAtributos as $campo => $valor ){

			if( $campo == $this->arChavePrimaria[0] && !$this->tabelaAssociativa ) continue;
			if( $valor !== null ){
				$arCampos[]  = $campo;
				$arValores[] = trim( pg_escape_string( $valor ) );
			}
		}

		if( count( $arValores ) ){
			$sql = " insert into $this->stNomeTabela ( ". implode( ', ', $arCampos   ) ." )
											  values ( '". implode( "', '", $arValores ) ."' )
					 returning {$this->arChavePrimaria[0]}";

			$stChavePrimaria = $this->arChavePrimaria[0];
			return $this->$stChavePrimaria = $this->pegaUm( $sql );
		}
	} // Fim _inserir()

	/**
	 * Função _alterar
	 * Método usado para alteração de um registro do banco
	 * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
	 * @access private
	 * @author Orion Teles de Mesquita
	 * @since 12/02/2009
	 */
	public function alterar($arCamposNulo = array()){
		if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );

		$campos = "";
		foreach( $this->arAtributos as $campo => $valor ){
			if( $valor !== null ){
				if( $campo == $this->arChavePrimaria[0] ){
					$valorCampoChave = $valor;
					continue;
				}

				$valor = pg_escape_string( $valor );

				$campos .= $campo." = '".$valor."', ";
			}
			else{
				if(in_array($campo, $arCamposNulo)) {
					$campos .= $campo." = null, ";
				}
			}
		}

		$campos = substr( $campos, 0, -2 );

		$sql = "UPDATE $this->stNomeTabela SET $campos WHERE {$this->arChavePrimaria[0]} = '{$valorCampoChave}' RETURNING {$this->arChavePrimaria[0]}";

		return $this->pegaUm( $sql );
	} // Fim _alterar()

	/**
	 * Função antesExcluir
	 * Método usado para fazer alguma funcionalidade antes da exclusão dos dados
	 * @return bool - Retorna true se não houve nada para impedir a exclusão e false caso contrário
	 * @access public
	 * @author Orion Teles de Mesquita
	 * @since 12/02/2009
	 */
	public function antesExcluir( $id = null ){ return true; }

	/**
	 * Função excluir
	 * Método usado para excluir registro do banco
	 * @param int $id - Identificador do registro a ser excluído ( se não for passado valor, exclui-se o registro do objeto carregado )
	 * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
	 * @access public
	 * @author Orion Teles de Mesquita
	 * @since 12/02/2009
	 */
	public function excluir( $id = null, $retorno = null ){
		$complemento = ";";
		if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );

		if($retorno){
			$complemento = "returning $retorno;";
		}

		if( !$this->antesExcluir($id) ) return false;

		$stChavePrimaria = $this->arChavePrimaria[0];
		$id = $id ? $id : $this->$stChavePrimaria;

		$sql = " DELETE FROM $this->stNomeTabela WHERE $stChavePrimaria = $id $complemento";
		if($retorno){
			return $this->pegaUm( $sql );
		}else{
			return $this->executar( $sql );
		}
	}

	public function excluirLogicamente($id){
        $arDados = array(
            'id' => $id,
            'excluido' => 't',
            'atualizado_em' => 'now()',
            'atualizado_por' => $_SESSION['usuario_id'],
        );

        return $this->popularDadosObjeto($arDados)->salvar();
    }

	/**
	 * Função carregarPorId
	 * Método usado para carregar um Objeto pelo ID
	 * @param int $id - Identificador do objeto a ser carregado
	 * @access public
	 * @author Orion Teles de Mesquita
	 * @since 12/02/2009
	 */
	public function carregarPorId( $id ){
		$campo = implode(', ', array_keys($this->arAtributos));
		$sql   = " SELECT {$campo} FROM $this->stNomeTabela WHERE {$this->arChavePrimaria[0]} = '".trim($id)."'; ";

		$arResultado = $this->pegaLinha( $sql );
		$this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
        return $this;
	}

	/**
	 * Função recuperarTodos
	 * Método usado para recuperar todos os registros do banco, seguinto parâmetros
	 * @param string $stCampos - String contendo nomes dos campos a serem carregados
	 * @param array $arClausulas - Array contendo dados da cláusula where
	 * @param string $stOrdem - String contendo dados da cláusula ordey by
	 * @example - $obNomeObjeto->recuperarTodos( 'campo1, campo2', array( "id = 123" ), nome )
	 * @example - $obNomeObjeto->recuperarTodos() --> Se não passar parâmetros reconhece como todos os registros do banco.
	 * @access public
	 * @author Orion Teles de Mesquita
	 * @since 12/02/2009
	 */
	public function recuperarTodos( $stCampos, $arClausulas = null, $stOrdem = null ){
		$stCampos = $stCampos == '*' ? implode(', ', array_keys($this->arAtributos)) : $stCampos;

		$sql  = " SELECT $stCampos FROM $this->stNomeTabela ";
		$sql .= $arClausulas ? " WHERE ". implode( " AND ", $arClausulas ) : "";
		$sql .= $stOrdem ? " ORDER BY $stOrdem" : "";

		$resultado = $this->carregar( $sql );

		return $resultado ? $resultado : array();

	}

	public function recuperarColuna( $stCampos, $arClausulas = null, $stOrdem = null ){
		$stCampos = $stCampos == '*' ? implode(', ', array_keys($this->arAtributos)) : $stCampos;

		$sql  = " SELECT $stCampos FROM $this->stNomeTabela ";
		$sql .= $arClausulas ? " WHERE ". implode( " AND ", $arClausulas ) : "";
		$sql .= $stOrdem ? " ORDER BY $stOrdem" : "";

		$resultado = $this->carregarColuna( $sql );

		return $resultado ? $resultado : array();

	}

	/**
	 * Função recuperarLinha
	 * Método usado para recuperar UMA tupla da tabela, seguinto parâmetros.
	 * O retorno da query PODERÁ ser carregado nos atributos da instância
	 * @param string $stCampos - String contendo nomes dos campos a serem carregados
	 * @param array $arClausulas - Array contendo dados da cláusula where
	 * @param boolean $carregaInstancia - Booleano que indica o tipo de retorno - se TRUE carrega os dados na instancia senão não.
	 * @example - $obNomeObjeto->recuperarLinha( 'campo1, campo2', array( "id = 123" ), true )
	 * @access public
	 * @author Felipe Tarchiani Cerávolo Chiavicatti
	 * @since 12/04/2011
	 */
	public function recuperarLinha( $stCampos, $arClausulas, $carregaInstancia = true){
		$stCampos = $stCampos == '*' ? implode(', ', array_keys($this->arAtributos)) : $stCampos;

		$sql  = " SELECT $stCampos FROM $this->stNomeTabela ";
		$sql .= " WHERE ". implode( " AND ", $arClausulas );

		$arResultado = $this->pegaLinha( $sql );
		if ($carregaInstancia)
			$this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
		else
			return $arResultado;
	}

	/**
	 * Função recuperarUm
	 * Método usado para recuperar UMA coluna de uma tupla da tabela, seguinto parâmetros.
	 * @param string $stCampos - String contendo nomes dos campos a serem carregados
	 * @param array $arClausulas - Array contendo dados da cláusula where
	 * @example - $obNomeObjeto->recuperarUm( 'campo1, campo2', array( "id = 123" ) )
	 * @access public
	 * @author Felipe Tarchiani Cerávolo Chiavicatti
	 * @since 12/04/2011
	 */
	public function recuperarUm( $stCampos, $arClausulas){
		$sql  = " SELECT $stCampos FROM $this->stNomeTabela ";
		$sql .= " WHERE ". implode( " AND ", $arClausulas );

		$resultado = $this->pegaUm( $sql );
		return $resultado;
	}

	public function popularObjeto( $arCampos, $arDados = null ){
		$arDados = $arDados ? $arDados : $_REQUEST;
		foreach( $arCampos as $campo ){

			if( key_exists( $campo, $arDados ) ){
				$this->$campo = $arDados[$campo];
			}
		}
		return $this;
	}

	public function popularDadosObjeto( $arDados = null ){

		$arDados = $arDados ? $arDados : $_REQUEST;
		foreach( $this->arAtributos as $campo => $valor){

			if( key_exists( $campo, $arDados ) ){
				$this->$campo = $arDados[$campo];

			}
		}
		return $this;
	}

    public function getDados(){
        return $this->arAtributos;
    }

    public function setDadosNull(){
        foreach ($this->arAtributos as $k => $v){
            $this->arAtributos[$k] = null;
        }

        return $this;
    }
	function lista($coluna = null, Array $arParamWhere = null, Array $arParamJoin = null, Array $arParam = null){
		$where 		  = "";
		$arJoin	  	  = array();
		$coluna  	  = implode(', ', (is_null($coluna) ? array_keys($this->arAtributos) : ((array) $coluna)) );
		// Parametros auxiliares
		$alias = $arParam['alias'];
		$order = $arParam['order'] ? " ORDER BY {$arParam['order']}" : "";

		// Separa os JOINs (inner, left)
		$arInner = is_array($arParamJoin['inner']) ? $arParamJoin['inner'] : array();
		$arLeft  = is_array($arParamJoin['left']) ? $arParamJoin['left'] : array();

		// Monta os INNER JOINs
		foreach($arInner as $tabela => $on){
			$arJoin[] = 'JOIN ' . $tabela . ' ON (' . $on . ')';
		}
		// Monta os LEFT JOINs
		foreach($arLeft as $tabela => $on){
			$arJoin[] = 'LEFT JOIN ' . $tabela . ' ON (' . $on . ')';
		}

		// Monta string WHERE
		$where = (count( $arParamWhere ) > 0 ? "WHERE (" . implode(') AND (', $arParamWhere) . ")" : "");
		// Monta string JOIN
		$join  = count($arJoin) ? implode(' ', $arJoin) : '';

		$sql = "SELECT
				   {$coluna}
			  	FROM
			  	   {$this->stNomeTabela} $alias
			  	{$join}
			  	{$where}
			  	{$order};";
//		dbg($sql, 1);
		return $this->carregar($sql);
	}

    /**
     * @param $id
     * @return array
     */
	public function getTodosDados($id)
    {
        $this->carregarPorId($id);
        return $this->getDados();
    }
}
