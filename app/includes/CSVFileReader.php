<?php

/**
 * Realiza leitura de um arquivo CSV.
 *
 * @since 10/11/2006
 */
class CSVFileReader
{
	
	/**
	 * Ponteiro para o arquivo que está sendo lido.
	 *
	 * @var resource
	 */
	protected $fp = null;
	
	/**
	 * Quantidade de caracteres lido.
	 *
	 * @var integer
	 */
	protected $read = 0;
	
	/**
	 * Quantidade de caracteres total do arquivo.
	 *
	 * @var integer
	 */
	protected $length = 0;
	
	/**
	 * Caracter que separa cada registros.
	 * 
	 * @var string
	 */
	protected $lineSeparator = "\n\r";
	
	/**
	 * Caracter que separa cada dado de uma coluna.
	 * 
	 * @var string
	 */
	protected $separator = ';';
	
	/**
	 * Alterar a quantidade de caracteres lidos. O parâmetro é adicionado ao
	 * valor atual lido.
	 *
	 * @param integer $length
	 * @return void
	 */
	protected function addRead( $length )
	{
		$this->read += $length;
	}
	
	/**
	 * Finaliza ponteiro do arquivo que está sendo lido.
	 *
	 * @return void
	 */
	protected function close()
	{
		if ( $this->fp == null )
		{
			return;
		}
		fclose( $fp );
	}
	
	/**
	 * Inicia ponteiro do arquivo a ser lido.
	 *
	 * @param string $file caminho para o arquivo a ser lido
	 */
	protected function open( $file )
	{
		if ( is_file( $file ) == false )
		{
			return;
		}
		$this->length = filesize( $file );
		$this->fp = fopen( $file, 'r' );
	}
	
	/**
	 * Altera o arquivo a ser lido.
	 *
	 * @param string $file caminho para o arquivo a ser lido
	 */
	public function setFile( $file )
	{
		$this->close();
		$this->open( $file );
	}
	
	/**
	 * Altera separador de linhas.
	 *
	 * @param string $lineSeparator
	 */
	public function setLineSeparator( $lineSeparator )
	{
		$this->lineSeparator = (string) $lineSeparator;
	}
	
	/**
	 * Altera separador das colunas.
	 *
	 * @param string $separator
	 */
	public function setSeparator( $separator )
	{
		$this->separator = (string) $separator;
	}
	
	/**
	 * Retorna o progresso de leitura do arquivo.
	 *
	 * @return float
	 */
	public function getProgress()
	{
		return ceil( ( $this->read / $this->length ) * 100 );
	}
	
	/**
	 * Captura um registro do arquivo CSV. Caso não existam mais linhas a serem
	 * lidas retorna nulo.
	 * 
	 * @return string[]
	 */
	public function getRow()
	{
		if ( $this->fp == null || feof( $this->fp ) == true )
		{
			return null;
		}
		$content = fgets( $this->fp );
		$this->addRead( strlen( $content ) + strlen( $this->lineSeparator ) );
		if ( $content == '' )
		{
			return null;
		}
		$content = explode( $this->separator, $content );
		foreach ( $content as &$item )
		{
			$item = trim( $item );
			if ( $item{0} == '"' && $item{strlen( $item ) - 1} == '"' )
			{
				$item = substr( $item, 1, -1 );
			}
		}
		return $content;
	}
	
}

?>