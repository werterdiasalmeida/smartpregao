<?php

	include "config.inc";
	$tema = $_SESSION['tema'];
	$_SESSION = array();
	$_SESSION['tema'] = $tema;
	header('Location: /painel/login.php');
	
	exit();

?>