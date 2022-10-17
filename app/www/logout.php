<?php

	include "config.inc";
	
	$sisid = $_SESSION['sisid'];
	$_SESSION = array();

	if($sisid != 44)	header('Location: /login.php');
	else header('Location: /demandas/login.php');
	
	exit();

?>