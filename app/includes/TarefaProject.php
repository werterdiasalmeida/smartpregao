<?php

class TarefaProject
{
	public $acaid = 'null';
	
	public $attid = '';
	
	public $nome = '';
	
	public $duracao = '';
	
	public $inicio = '';
	
	public $termino = '';
	
	public $predecessor = '';
	
	public $nivel = '';
	
	public $descricao = '';
	
	public $filhos = array();
	
	protected static $id = 1;
	
	private static $projeto = null;
	
	public function adicionarFilho( TarefaProject $tarefa ) {
		array_push( $this->filhos, $tarefa );
	}
	
	protected function ordernar() {
		
		if ( isset( $this->nivel ) ) {
			
//			$nivel_permitido = $this->nivel + 1;
			$nivelatual = 0;
			$anterior = null;
			reset( $this->filhos );
//			$x = 0;
			foreach ( $this->filhos as $indice => $filho ) {
				
				if ( $filho->nivel > $nivelatual ) {
					if( $$nivelatual ){
						$nivelant   = $nivelatual;
						$$nivelant->adicionarFilho( $filho );
						$nivelatual  = $filho->nivel;
						$$nivelatual = $filho;
					}else{
						$nivelatual  = $filho->nivel;
						$$nivelatual = $filho;
					}
				} else {
					
					if( $filho->nivel == $nivelatual ){
						if( $$nivelant ){
							$$nivelant->adicionarFilho( $filho ); 
						}
						$$nivelatual = $filho;
					}elseif( $filho->nivel < $nivelatual ){
						
						$nivelant = $filho->nivel-1;
						if( $$nivelant ){
							
							$$nivelant->adicionarFilho( $filho );
							$nivelatual = $filho->nivel;
							$$nivelatual = $filho;
						}else{
							
							$nivelatual  = $filho->nivel;
							$$nivelatual = $filho;
						}
					}
				}
//				$x++;
			}
//			alert($x);
//			reset( $this->filhos );
//			foreach ( $this->filhos as $filho )
//			{
//				$filho->ordernar();
//			}
		}
//		reset( $this->filhos );
//		unset( $this->nivel );
	}
	
	/**
	 * @param resource $arquivo
	 * @return TarefaProject[]
	 */
	public static function lerCSV( $arquivo ) {
		
		$tarefa_pai = new self();
		$tarefa_pai->nivel = 0;
		fgetcsv( $arquivo );
//		$x;
//		$y;
		while ( $linha = fgetcsv( $arquivo, 10000, ';' ) ) {
			
			if ( count( $linha ) == 8 || count( $linha ) == 7 ) {
//				alert($linha[7]);
				$tarefa = self::criarDeCsv( $linha );
				$tarefa_pai->adicionarFilho( $tarefa );
//				$y++;
			}
//			$x++;
		}
//		alert($x.'/'.$y);
		$tarefa_pai->ordernar();
		return $tarefa_pai->filhos;
	}
	
	/**
	 * @param string[] $dados
	 * @return TarefaProject
	 */
	protected static function criarDeCsv( $dados )
	{
		$tarefa = new self();
		$tarefa->nome      = $dados[1];
		$tarefa->descricao = $dados[7];
		
		// trata data de inicio
		$inicio = explode( ' ', $dados[3] );
		$inicio_data = explode( '/', $inicio[0] );
		$inicio_horario = explode( ':', $inicio[1] );
		if ( count( $inicio_data ) != 3 )
		{
			$tarefa->inicio = 'null';
		}
		else
		{
			$tarefa->inicio =
				sprintf( '%04d', $inicio_data[2] ) . '-' .
				sprintf( '%02d', $inicio_data[1] ) . '-' .
				sprintf( '%02d', $inicio_data[0] ) . ' ' .
				sprintf( '%02d', $inicio_horario[0] ) . ':00';
		}
		// FIM trata data de inicio
		
		// trata data de termino
		$termino = explode( ' ', $dados[4] );
		$termino_data = explode( '/', $termino[0] );
		$termino_horario = explode( ':', $inicio[1] );
		if ( count( $termino_data ) != 3 )
		{
			$tarefa->termino = 'null';
		}
		else
		{
			$tarefa->termino =
				sprintf( '%04d', $termino_data[2] ) . '-' .
				sprintf( '%02d', $termino_data[1] ) . '-' .
				sprintf( '%02d', $termino_data[0] ) . ' ' .
				sprintf( '%02d', $termino_horario[0] ) . ':00';
		}
		// FIM trata data de termino
		
		// utilizado para ordenação futura
		$tarefa->nivel     = $dados[6];
		return $tarefa;
	}
	
	protected static function criarDaBaseDaAtividade( $atiid, $nivel ){
		global $db;
		$sql = "
			select
				atidescricao as titulo,
				atidetalhamento as descricao,
				atidatainicio as inicio,
				atidatafim as termino
			from
				pde.atividade
			where
				atiid = $atiid and
				atistatus = 'A' and
				_atiprojeto = " . self::$projeto . "
		";
		$dados = $db->recuperar( $sql );
		$dados = $dados ? $dados : array();
		if ( count( $dados ) == 0 )
		{
			return '';
		}
		if ( $dados['inicio'] )
		{
			$inicio = explode( '-', $dados['inicio'] );
			$inicio_project = $inicio[2] . '/' . $inicio[1] . '/' . $inicio[0];
			$inicio = mktime( 0, 0, 0, $inicio[1], $inicio[2], $inicio[0] );
		}
		else
		{
			$inicio = 0;
			$inicio_project = '';
		}
		if ( $dados['termino'] )
		{
			$termino = explode( '-', $dados['termino'] );
			$termino_project = $termino[2] . '/' . $termino[1] . '/' . $termino[0];
			$termino = mktime( 0, 0, 0, $termino[1], $termino[2], $termino[0] );
		}
		else
		{
			$termino = 0;
			$termino_project = '';
		}
		if ( $inicio && $termino )
		{
			$inicio_project .= ' 00:00';
			$termino_project .= " 00:00";
			$duracao = ( $termino - $inicio ) / 86400; // em dias
			$duracao = number_format( $duracao, 1, ',', '.' );
		}
		else
		{
			$duracao = '0';
		}
		$retorno =
			self::$id . ";" .											// id
			"\"".trim( str_replace( "\n", '', $dados['titulo'] ) ) . "\";" .	// nome
			$duracao . 'd;' .											// duração
			$inicio_project . ";" .										// inicio
			$termino_project . ";" .									// término
			";" .   								// predecessoras
			$nivel . ";" .												// nível das estruturas
			"\"".trim(
				str_replace(
					array( "\n", "\r" ),
					'',
					$dados['descricao']
				)
			)."\"";															// anotações
		$sql = "
			select
				atiid
			from pde.atividade 
			where
				atiidpai = $atiid and
				atistatus = 'A' and
				_atiprojeto = " . self::$projeto . "
			order by
				atiordem
		";
		$filhos = $db->carregar( $sql );
		$filhos = $filhos ? $filhos : array();
		foreach ( $filhos as $filho )
		{
			$retorno .= "\n" . self::criarDaBaseDaAtividade( $filho['atiid'], $nivel + 1 );
			self::$id++;
		}
		return $retorno;
	}
	
	public function importarParaAtividade( $atiid ) {
		
		global $db;
		
		$atiid = (integer) $atiid;
		$sql = "
			select
				max( atiordem ) as atiordem,
				acaid
			from pde.atividade
			where
				atiidpai = " . $atiid . " and
				atistatus = 'A'
			group by
				acaid
		";
		$dados = $db->carregar( $sql );
		$dados = $dados[0] ? $dados : array();
		
		$atiordem = $dados[0]['atiordem']+1;
		if ( $dados[0]['acaid'] ) {
			$this->acaid = (integer) $dados[0]['acaid'];
		} else {
			$this->acaid = 'null';
		}
		$this->atiordem = $atiordem ? $atiordem : 1;
		$sql = "select _atiprojeto from pde.atividade where atiid = " . $atiid;
		self::$projeto = $db->pegaUm( $sql );
		$this->gravarParaAtividade( $atiid, $this->atiordem );
	}
	
	protected function gravarParaAtividade( $atiidpai, $atiordem ) {
		
		global $db;
		$sql = sprintf(
			"
				insert into pde.atividade (
					atidescricao, atidetalhamento,
					atidatainicio, atidatafim,
					atistatus, atiidpai, atiordem,
					_atiprojeto, acaid
				)  
				values ( '%s', '%s', %s, %s, '%s', %d, %d, %d, %s )  
				returning atiid; ",
			str_replace( "'", "\\'", $this->nome ),
			str_replace( "'", "\\'", $this->descricao ),
			$this->inicio != 'null' ? "'" . $this->inicio . "'" : $this->inicio,
			$this->termino != 'null' ? "'" . $this->termino . "'" : $this->termino,
			'A',
			$atiidpai,
			$atiordem,
			self::$projeto,
			$this->acaid
		);
		//echo "<br><br><br><br><br>" . $sql;
		$this->atiid = $db->pegaUm( $sql );
		$db->commit();
		reset( $this->filhos );
		$ordem = 1;
		foreach ( $this->filhos as $filho ) {
			
			$filho->gravarParaAtividade( $this->atiid, $ordem );
			$ordem++;
		}
	}
	
	public static function exportarDeAtividade( $atiid, $incluirRaiz = true )
	{
		global $db;
		$atiid = (integer) $atiid;
		
		// inicia texto de retorno
		$atividades = "Id;Nome;Duração;Início;Término;Predecessoras;Nível da estrutura de tópicos;Anotações";
		
		// captura id do projeto
		$sql = "select _atiprojeto from pde.atividade where atiid = " . $atiid;
		self::$projeto = $db->pegaUm( $sql );
		
		// zera contador de id para exportação
		self::$id = 1;
		
		// verifica se é para carregar somente os filhos (excluindo id do atual)
		if ( $incluirRaiz == true )
		{
			$atividades .= "\n" . self::criarDaBaseDaAtividade( $atiid, 1 );
		}
		else
		{
			$sql = "
				select
					atiid
				from pde.atividade 
				where
					atiidpai = $atiid and
					atistatus = 'A' and
					_atiprojeto = " . self::$projeto . "
				order by
					atiordem
			";
			$filhos = $db->carregar( $sql );
			$filhos = $filhos ? $filhos : array();
			foreach ( $filhos as $filho )
			{
				$atividades .= "\n" . self::criarDaBaseDaAtividade( $filho['atiid'], 1 );
			}
		}
		return $atividades;
	}
	
}

?>