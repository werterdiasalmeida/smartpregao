<?php

/**
 * @param mixed $mData
 * @param boolean $bExit
 */
function dump( $mData, $bExit = false )
{
	$oTrace = new BackTrace();
	$oTrace->levelDown();
	$sTrace = $oTrace->explain();
	?>
	<fieldset>
		<legend>Dump</legend>
		<pre><?php var_dump( $mData ); ?></pre>
		<?php echo $sTrace; ?>
	</fieldset>
	<?php
	if ( $bExit === true )
	{
		echo '<pre style="color: red">Interrupted execution in file ' . __FILE__ . ' in line ' .( __LINE__ + 1 ) . '.</pre>';
		exit();
	}
}

include_once APPRAIZ . 'includes/backtrace/BackTrace.php';
include_once APPRAIZ . 'includes/backtrace/BackTraceExplain.php';
include_once APPRAIZ . 'includes/failure/ErrorHandler.php';
include_once APPRAIZ . 'includes/failure/ExceptionHandler.php';

ErrorHandler::start();
ExceptionHandler::start();

ob_start();