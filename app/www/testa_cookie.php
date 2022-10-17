<?php

   session_start();
   $_SESSION['teste']='12345';
    setcookie($_COOKIE['PHPSESSID']);
	header('Location: /saida_teste.php');
	exit();

?>