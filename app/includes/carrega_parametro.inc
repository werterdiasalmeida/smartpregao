<?php
$sql = sprintf("SELECT 
					parnome,
					parvalor
				FROM 
					seguranca.parametro
				WHERE
					sisid = %s OR sisid IS NULL",
				$_SESSION['sisid']);
$dados = $db->carregar($sql);
$dados = $dados ? $dados : array();
foreach ($dados as $dados){
	define($dados['parnome'], $dados['parvalor']);
}	