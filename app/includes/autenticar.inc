<?

/**
 * Sistema Integrado de Monitoramento do Minist�rio da Educa��o
 * Setor responsvel: SPO/MEC
 * Desenvolvedor: Desenvolvedores Simec
 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
 * Programadores: Ren� de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
 * M�dulo: Autentica��o
 * Finalidade: Rotina de autentica��o de usu�rios.
 * Data de cria��o: 24/06/2005
 * �ltima modifica��o: 28/08/2006
 */
// BIBLIOTECA //////////////////////////////////////////////////////////////
/**
 * Exibe mensagens de erro.
 *
 * @return void
 */

function erro()
{
    global $db;
    $db->commit();
    $_SESSION = array();
    $_SESSION['MSG_AVISO'] = func_get_args();
    header("Location: " . $_SERVER['PHP_SELF']);
    exit();
}

// CONFIGURA��O ////////////////////////////////////////////////////////////

// quantidade limite de tentativas de acesso com senha inv�lida
define('LIMITE_DE_TENTATIVAS', 4);

// lista de status de usu�rio
define('STATUS_ATIVO', 'A');
define('STATUS_PENDENTE', 'P');
define('STATUS_BLOQUEADO', 'B');

// mensagens de alera do sistema
$alerta = array(
    'cpf' => "O email ou CPF informado n�o est� cadastrado.",
    'status_pendente' => "Sua conta est� pendente para aprova��o, aguarde a avalia��o dos administradores.",
    'status_bloqueado' => "Sua conta est� bloqueada",
    'senha' => "A senha informada n�o � v�lida. Tentativas restantes: %s",
    'senha_2' => "A senha informada n�o � v�lida.",
    'tentativas' => "Voc� excedeu a quantidade de tentativas.",
    'permissao' => "Voc� n�o possui permiss�o de acesso.",
);

// VALIDA��O ///////////////////////////////////////////////////////////////


$_SESSION["NUM_IP_CLIENTE"] = $_SERVER["REMOTE_ADDR"];
$_SESSION["DES_BROWSER"] = $_SERVER["HTTP_USER_AGENT"];

// carrega os dados do formul�rio
$cpf = corrige_cpf($_POST['usucpf']);
$email = ($_POST['usucpf']);
$senha = trim($_POST['ususenha']);

// carrega os dados da conta do usu�rio a partir do cpf informado
$sql = sprintf(
    "SELECT 
            u.id,
			u.usucpf, 
			u.ususexo,
			u.ususenha, 
			u.suscod, 
			u.usutentativas, 
			u.usunome, 
			u.usuemail ,
			u.usumigrado,
			u.arqid
		 FROM 
			seguranca.usuario u
		 WHERE 
			(u.usucpf = '%s' OR u.usuemail = '%s')",
    $cpf, $email
);

$usuario = (object)$db->recuperar($sql);

// verifica se o cpf existe
if (!$usuario->usucpf) {
    erro($alerta['cpf']);
}

// verifica o status do usu�rio
switch ($usuario->suscod) {
    case STATUS_ATIVO:
        break;
    case STATUS_PENDENTE:
        erro($alerta['status_pendente']);
    case STATUS_BLOQUEADO:
    default:
        erro($alerta['status_bloqueado']);
}

// atribui��es requeridas para que a auditoria do sistema funcione
$_SESSION['sisid'] = 4; # seleciona o sistema de seguran�a
$_SESSION['usucpf'] = trim($usuario->usucpf);
$_SESSION['usuario_id'] = $usuario->id;
$_SESSION['usucpforigem'] = trim($usuario->usucpf);
$_SESSION['ususexo'] = $usuario->ususexo;
$_SESSION['usuarqid'] = $usuario->arqid;

// valida a senha informada
if (md5_decrypt_senha($usuario->ususenha, '') != $senha) {
//		$usuario->usutentativas += 1;
//		$alerta['senha'] = sprintf( $alerta['senha'], LIMITE_DE_TENTATIVAS - $usuario->usutentativas + 1 );
//		if ( $usuario->usutentativas > LIMITE_DE_TENTATIVAS ) {
//			$sql = sprintf( "UPDATE seguranca.usuario SET usutentativas = 0 WHERE usucpf = '%s'", $usuario->usucpf );
//			$db->executar( $sql );
//			$db->alterar_status_usuario( $usuario->usucpf, STATUS_BLOQUEADO, "Usu�rio bloqueado por exceder a quantidade de tentativas de login com senha inv�lida." );
//			erro( $alerta['senha'], $alerta['tentativas'], $alerta['status_bloqueado'] );
//		} else {
//			$sql = sprintf(
//				"UPDATE seguranca.usuario SET usutentativas = %d WHERE usucpf = '%s'",
//				$usuario->usutentativas,
//				$usuario->usucpf
//			);
//			$db->executar( $sql );
//			erro( $alerta['senha'] );
//		}
    erro($alerta['senha_2']);
}

if ($usuario->usutentativas > 0 || $usuario->usumigrado == 't') {
    $sql = sprintf("UPDATE seguranca.usuario SET usutentativas = 0, usumigrado = FALSE WHERE usucpf = '%s'", $usuario->usucpf);
    $db->executar($sql);
}
// carrega os dados do usu�rio para a sess�o
unset($usuario->ususenha);
foreach ($usuario as $attribute => $value) {
    $_SESSION[$attribute] = $value;
}
$_SESSION['usucpforigem'] = $usuario->usucpf;
// verifica permiss�o de acesso aos m�dulos
$sql = sprintf(
    "SELECT
		s.sisid, s.sisdiretorio, s.sisarquivo, s.sisdsc, s.sisurl, s.sisabrev, s.sisexercicio, s.paginainicial, p.pflnivel AS usunivel, us.susdataultacesso
		FROM seguranca.sistema s
		INNER JOIN seguranca.usuario_sistema us USING ( sisid )
		INNER JOIN seguranca.usuario u USING ( usucpf )
		INNER JOIN seguranca.perfilusuario pu USING ( usucpf )
		INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod AND p.sisid = s.sisid
		WHERE
		us.suscod = 'A' AND
		u.usucpf = '%s' AND
		u.suscod = 'A' AND
		p.pflstatus = 'A'
		GROUP BY s.sisid, s.sisdiretorio,  s.sisarquivo, s.sisdsc, s.sisurl, s.sisabrev, s.sisexercicio, s.paginainicial, p.pflnivel, us.susdataultacesso
		ORDER BY us.susdataultacesso DESC
		LIMIT 1",
    $usuario->usucpf
);

$sistema = (object)$db->pegaLinha($sql);
if (!$sistema->sisid) {
    erro($alerta['permissao']);
}
// carrega os dados do m�dulo para a sess�o
foreach ($sistema as $attribute => $value) {
    $_SESSION[$attribute] = $value;
}

// atribui o ano atual para o exerc�cio das tarefas
$_SESSION['exercicio_atual'] = $db->pega_ano_atual();
$_SESSION['exercicio'] = $db->pega_ano_atual();

unset($_SESSION['superuser']);
$_SESSION['superuser'] = $db->testa_superuser($usuario->usucpf);
//dbg($_SESSION['superuser'],1,'00267967160');
// atualiza a data de �ltimo acesso no m�dulo selecionado
$_SESSION['usuacesso'] = date('Y/m/d H:i:s');
$sql = sprintf(
    $sql = "UPDATE seguranca.usuario_sistema SET susdataultacesso = '%s' WHERE usucpf = '%s' AND sisid = %d",
    $_SESSION['usuacesso'],
    $usuario->usucpf,
    $sistema->sisid
);
$db->executar($sql);

// TO DO: verificar se h� mensagens na caixa de entrada (aprovar e implementar fluxo)
/*	o sistema dever� disparar o processo autom�tico.
Para tanto o sistema verfica se algume j� disparou o processo para o dia escolhido. se sim, n�o faz nada, se n�o ent�o atualiza a tabela e dispara o processo.

$sql = "select ctrdata from controleprocesso where ctrdata=current_date";
if  (! $db->pegaUm($sql) )//or $_SESSION['usucpf']=='37423428787')
{
    // dispara os processos
    include APPRAIZ."includes/dispara_processo.inc";
}
*/
$totmsg = 0;
unset($_SESSION['mostramsg']);
$sql = "SELECT count(msgid) AS totmsg FROM mensagemusuario WHERE usucpf = '" . $_SESSION['usucpf'] . "' AND msulida ='f' AND msuexcluida='f'";
$_SESSION['mostramsg'] = $db->pegaUm($sql);

/*
 * O contador de tempo online na tela deve ser atualizado toda vez que o
 * usu�rio carregar uma tela do sistema. Ele � utilizado pelo "estou vivo"
 * de acordo com a constante MAXONLINETIME, definido no config.inc.
 */
$_SESSION["evHoraUltimoAcesso"] = time();

// leva o usu�rio para a tela inicial do m�dulo selecionado
$db->commit();

$header = sprintf(
    "Location: ../%s/%s.php?modulo=%s",
    $sistema->sisdiretorio,
    $sistema->sisarquivo,
    $sistema->paginainicial
);
header($header);
exit();