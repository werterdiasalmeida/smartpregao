<?
//Emula outro usuário
	if ( $_POST['usucpf_simu'] && ( $_SESSION['superuser'] || $db->testa_uma() || $_SESSION['usuuma'] ) )
	{
		$_SESSION['usucpf'] = $_POST['usucpf_simu'];
	}
	
	
?>