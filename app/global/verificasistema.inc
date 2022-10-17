<?php
/**
	 * Solução paleativa para o problema de navegação entre sistemas. Esta
	 * rotina tenta adivinhar qual módulo o usuário teve a intenção de acessar.
	 * A decisão é tomada a partir da url solicitada pelo usuário no qual ele
	 * indica o diretório e a ação pretendida.
	 */
	preg_match( '/\/([a-zA-Z_]*)\//', $_SERVER['REQUEST_URI'], $sisdiretorio );
	$sisdiretorio = $sisdiretorio[1];
	preg_match( '/\/([a-zA-Z_]*)\.php/', $_SERVER['REQUEST_URI'], $sisarquivo );
	$sisarquivo = $sisarquivo[1];

	if (($sisdiretorio != NULL) and ($sisarquivo != $_SESSION['sisarquivo'] or $sisdiretorio != $_SESSION['sisdiretorio']) and $_SESSION['usucpf'])
	{
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
				s.sisdiretorio = '%s' AND
				s.sisarquivo = '%s' AND
				u.usucpf = '%s' AND
				us.suscod = 'A' AND
				p.pflstatus = 'A' AND
				u.suscod = 'A'
			ORDER BY p.pflnivel
			LIMIT 1",
			$sisdiretorio,
			$sisarquivo,
			$_SESSION['usucpf']
		);
		$link = pg_connect("host=".$GLOBALS["servidor_bd"]." port=".$GLOBALS["porta_bd"]." dbname=".$GLOBALS['nome_bd']."  user=".$GLOBALS["usuario_db"] ." password=".$GLOBALS["senha_bd"] ."");
		pg_query($link, "SET search_path TO seguranca,monitora,elabrev,public");
		if(($RS = pg_query($link,$sql)) && (pg_num_rows($RS)>=1)) { $sistema = pg_fetch_assoc($RS, $linha);}

        $sistema = $sistema ? $sistema : [];
		// carrega os dados do módulo para sessão
		foreach ( $sistema as $attribute => $value ) {
			$_SESSION[$attribute] = $value;
		}

		// pg_close($link);
}