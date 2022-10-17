<?php
/**
 * POG Progress Bar
 * 
 * Required
 * 		- Is required the aplication support flush.
 * 		- Only call the method PogProgressBar::setProgress() where can be print
 * 		a javascript command.
 * 
 * Recommendations
 * 		- The call to the metohd PogProgressBar::setProgress() be maded the most
 * 		possible close of the script end.
 * 		- When the process to be execute is to big, is necessary to configure
 * 		  the set_time_limit.
 * 			
 * Public Methods
 * 		- draw()		Draw the HTML of the progress bar.
 * 		- getProgress()	Returns the actual value of the percent done.
 * 		- setProgress()	Change the actual value of the percent done into range
 * 						from 0 to 100.
 * 		- setTheme 		change the actual theme (colors, styles) of the bar.
 * 
 * ----- Português ------
 * 
 * Requisitos
 * 		- É necessário que a aplicação suporte flush implícito.
 * 		- A chamada do método PogProgressBar::setProgress() deve ser realizada
 * 		  em uma área onde seja possível imprimir um código javascript.
 * 
 * Recomendações
 * 		- A chamada do método setProgress() seja realizada o mais próxima
 * 		  possível do fim do script.
 * 		- Quando o processo a ser executada é muito grande é necessário
 * 		  configurar o set_time_limit
 * 
 * Métodos Públicos
 * 		- draw()		Desenha o html da barra de progresso.
 * 		- getProgress()	Valor atual do progresso em porcentagem.
 * 		- setProgress()	Altera valor do progresso em porcentagem de 0 a 100.
 * 		- setTheme()	Altera tema (cores, estilos) da barra.
 * 
 * @author Renan de Lima <renandelima@gmail.com>
 * @author Thiago Mata <thiago.henrique.mata@gmail.com>
 * @version 0.1
 */
class PogProgressBar
{

	/**
	 * Array with the name of all progress bar maded.
	 * 
	 * Array com os nomes de todas as barras de progresso instanciadas.
	 *
	 * @var array
	 */
	static protected $arrNames = array();
	
	/**
	 * Array with the name of all javascript classes declared who controls the
	 * progress bars.
	 * 
	 * Nome das classes javascript declaradas que controla o progresso da barra.
	 *
	 * @var array
	 */
	static protected $arrJsClasses = array();

	/**
	 * Array with the options of theme to the bar.
	 * 
	 * Temas possíveis para a barra.
	 *
	 * @var array
	 */
	protected $arrThemes = array(
		'basic' => array(
			'container'	=> 'border:1px solid #b0b0b0;background-color:#d0d0d0;height:10px;width:300px;',
			'bar'		=> 'white-space:nowrap;background-color:#f0f0f0;height:10px;width:0px;font-size:8px;font-family:verdana;text-align:center;font-weight:bold;'
		),
		'blue' => array(
			'container'	=> 'border:1px solid #aaaaff;background-color:#ddddff;height:10px;width:300px;',
			'bar'		=> 'white-space:nowrap;background-color:#5050ff;height:10px;width:0px;color:white;font-size:8px;font-family:verdana;text-align:center;font-weight:bold;'
		),
		'green' => array(
			'container'	=> 'border:1px solid #50aa50;background-color:#ddffdd;height:10px;width:300px;',
			'bar'		=> 'white-space:nowrap;background-color:#30aa30;height:10px;width:0px;color:white;font-size:8px;font-family:verdana;text-align:center;font-weight:bold;'
		),
		'red' => array(
			'container'	=> 'border:1px solid #dd0000;background-color:#ffdddd;height:10px;width:300px;',
			'bar'		=> 'white-space:nowrap;background-color:#ff0011;height:10px;width:0px;color:white;font-size:8px;font-family:verdana;text-align:center;font-weight:bold;'
		)
	);

	/**
	 * Actual percent value.
	 * 
	 * Porcentagem atual.
	 *
	 * @var float
	 */
	protected $fltPercent = 0;

	/**
	 * Name of the javascript class used to control the progress bar.
	 * 
	 * Nome da classe javascript utilizada que controla o progresso da barra.
	 *
	 * @var string
	 */
	protected $strJsClass = 'PogProgressBar';

	/**
	 * Name of the progress bar.
	 *  
	 * Nome da barra de progresso.
	 *
	 * @var string
	 */
	protected $strName = '';

	/**
	 * Theme choose to the progress bar 
	 * 
	 * Tema utilizado pela barra de progresso.
	 *
	 * @var string
	 */
	protected $strTheme = 'basic';

	/**
	 * Initialize the progress bar.
	 * 
	 * Inicia a barra de progressão.
	 *
	 * @param string $strName
	 * @return void
	 */
	public function __construct( $strName )
	{
		$this->strName = (string) $strName;
		self::$arrNames[]  = $this->strName;
	}

	/**
	 * Draw the progress bar. This method should be call after the choose of the
	 * Theme.
	 * 
	 * Desenha a barra de progressão. Esse método deve ser chamado depois de ter
	 * configurado o tema
	 * 
	 * @see PogProgressBar::setTheme()
	 * @return void
	 */
	public function draw()
	{
		$arrTheme = $this->arrThemes[$this->strTheme];
		?>
			<div id="pbContainer<?php print $this->getSufix(); ?>" style="<?php print  $arrTheme['container']; ?>">
				<div id="pbBar<?php print $this->getSufix(); ?>" style="<?php print  $arrTheme['bar']; ?>"></div>
			</div>
		<?php
		$this->drawJsLibrary();
		$this->flush();
	}

	/**
	 * Declare the javascript class who will control the progress bar. To
	 * replace the controller you must overwrite this method since the
	 * javascript class declare have the method refresh who receive the percent
	 * value into a range from 0 to 100.
	 * 
	 * Declara a classe javascript que controla a barra de progressão. Para
	 * substituir o controlador você deve sobreescrever esse método de forma que
	 * a classe javascript declarada contenha o método "refresh" que receba a
	 * porcentagem de 0 a 100.
	 * 
	 * @return void
	 */
	protected function drawJsClass()
	{
		?>
			<script type="text/javascript">
				function <?php print $this->strJsClass; ?>( strSufix )
				{

					this.construct = function construct( strSufix )
					{
						this.objBar = document.getElementById( 'pbBar' + strSufix );
						this.objContainer = document.getElementById( 'pbContainer' + strSufix );
						this.intPercent = 0;
					}

					this.refresh = function refresh( fltPercent )
					{
						this.intPercent = parseInt( fltPercent );
						this.objBar.innerHTML = this.intPercent.toFixed( 0 ) + ' %';
						this.objBar.style.width = ( ( this.intPercent / 100 ) * ( this.objContainer.offsetWidth - 2 ) ) + 'px';
					}

					this.construct( strSufix );

				}
			</script>
		<?php
	}

	/**
	 * Initialize the javascript required to manipulate the progress bar.
	 * 
	 * Inicia javascript necessário para a manipulação da barra de progressão.
	 *
	 * @return void
	 */
	protected function drawJsLibrary()
	{
		if ( !in_array( $this->strJsClass, self::$arrJsClasses ) )
		{
			$this->drawJsClass();
			array_push( self::$arrJsClasses, $this->strJsClass );
		}
		?>
			<script type="text/javascript">
				pb<?php print $this->getSufix(); ?> = new <?php print $this->strJsClass; ?>( '<?php print $this->getSufix(); ?>' );
			</script>
		<?php
	}

	/**
	 * Print all the buffer content.
	 * 
	 * Imprime tudo que estiver em buffer.
	 *
	 * @return void
	 */
	protected function flush()
	{
		while ( ob_get_level() )
		{
			ob_end_flush();
		}
		flush();
	}

	/**
	 * Returns the progress actual.
	 * 
	 * Captura o progresso atual.
	 * 
	 * @return float
	 */
	public function getProgress()
	{
		return $this->fltPercent;
	}

	/**
	 * Returns the sufix from the bar. Can be used as unique identifier to the
	 * bar.
	 * 
	 * Captura o sufixo da barra. Pode ser utilizado como identificador único da
	 * barra.
	 * 
	 * @return float
	 */
	protected function getSufix()
	{
		return array_search( $this->strName , self::$arrNames );
	}

	/**
	 * Change the progress var value. The controller of the bar into the client
	 * side is defined into the method drawJsClass.
	 * 
	 * Altera o progresso da barra. O controle da barra, propriamente dito, é
	 * realizado pela classe javascript definida no método drawJsClass.
	 *
	 * @see PogProgressBar::drawJsClass()
	 * @param float $fltPercent
	 * @return void
	 */
	public function setProgress( $fltPercent )
	{
		if ( $fltPercent == $this->fltPercent || $fltPercent > 100 || $fltPercent < 0 )
		{
			return;
		}
		$this->fltPercent = $fltPercent;
		?>
			<script type="text/javascript">
				pb<?php print $this->getSufix(); ?>.refresh( <?php print $fltPercent; ?> );
			</script>
		<?php
		$this->flush();
	}

	/**
	 * Change the theme of the progress bar. This method must be called before
	 * the draw, otherwise this method will have no usefull effect.
	 * 
	 * Altera o tema da barra. Este método deve ser utilizado antes da chamada
	 * do draw, se for chamado depois a alteração não surtirá efeito.
	 * 
	 * @see PogProgressBar::draw()
	 * @param string $strTheme
	 */
	public function setTheme( $strTheme )
	{
		if ( !array_key_exists( $strTheme, $this->arrThemes ) )
		{
			return;
		}
		$this->strTheme = $strTheme;
	}

}

?>