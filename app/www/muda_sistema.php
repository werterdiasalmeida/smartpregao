<?php


	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Marcelo Freire
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Módulo: Segurança
	 * Finalidade: Permitir que o usuário nevege entre os sistemas.
	 * Data de criação:
	 * Última modificação: 29/08/2006
	 */

	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	//echo(ob_get_level());
	
	// verifica se a sessão não expirou
	if ( !$_SESSION['usucpf'] ) {
		header( "Location: ../login.php" );
		exit();
	}
	

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();
	
	$sisid = $_REQUEST['sisid'];
	$cpf = $_SESSION['usucpf'];
	$_SESSION['usunome'] = $db->pegaUm( "SELECT usunome FROM seguranca.usuario WHERe usucpf = '". $cpf ."'" );
	
	
	// obtém os dados do módulo
	$sql = sprintf(
		"SELECT
			s.sisid, s.sisdiretorio, s.sisarquivo, s.sisdsc, s.sisurl, s.sisabrev, s.sisexercicio, s.paginainicial, p.pflnivel AS usunivel, us.susdataultacesso, us.suscod, s.sisarquivo
			FROM seguranca.usuario u
			INNER JOIN seguranca.perfilusuario pu USING ( usucpf )
			INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod
			INNER JOIN seguranca.sistema s ON p.sisid = s.sisid
			INNER JOIN seguranca.usuario_sistema us ON s.sisid = us.sisid AND u.usucpf = us.usucpf
		WHERE
			s.sisid = %d AND
			u.usucpf = '%s' AND
			us.suscod = 'A' AND
			p.pflstatus = 'A' AND
			u.suscod = 'A'
		ORDER BY p.pflnivel
		LIMIT 1",
		$sisid,
		$cpf
	);
	$sistema = (object) $db->pegaLinha( $sql );

	if ( !$sistema ) {
		$_SESSION['MSG_AVISO'][] = "Sua sessão expirou.";
		header( "Location: login.php" );
		exit();
	}
	
	if ( !$sistema->sisid ) {
		//dbg( $sql );
		//dbg( $sistema, 1 );
		$db->insucesso( 'Você não tem permissão de acesso neste módulo.' );
	}

	// carrega os dados do módulo para sessão
	foreach ( $sistema as $attribute => $value ) {
		$_SESSION[$attribute] = $value;
	}

    //dbg($_SESSION, 1);
	// atualiza a data de último acesso no módulo selecionado
	$_SESSION['usuacesso'] = date( 'Y/m/d H:i:s' );
	$sql = sprintf(
		$sql = "UPDATE seguranca.usuario_sistema SET susdataultacesso = '%s' where usucpf = '%s' and sisid = %d",
		$_SESSION['usuacesso'],
		$cpf,
		$sistema->sisid
	);
	$db->executar( $sql );
	$db->commit();

	unset($_SESSION['superuser']);
	unset($_SESSION['usuuma']);
	$_SESSION['superuser'] = $db->testa_superuser();

	// atribui o ano atual para o exercício das tarefas
	$_SESSION['exercicio_atual'] = $db->pega_ano_atual();
	$_SESSION['exercicio']       = $db->pega_ano_atual();


	// leva o usuário para a tela inicial do módulo selecionado
	$header = sprintf(
		"../%s/%s.php?modulo=%s",
		$sistema->sisdiretorio,
		$sistema->sisarquivo,
		$sistema->paginainicial
	); 

	//header( $header );
	echo '<script type="text/javascript">window.location.href=\'' , $header , '\'</script>';
	exit();
?>

