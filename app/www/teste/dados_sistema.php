<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$sisid = 10;

// dados necessarios para a criacao de querys corretas
$novo_inicio_sisid = 10;
$novo_inicio_pflcod = 80;
$novo_inicio_menuid = 603;

// dados utilizados para a transformacao dos codigos dos perfis para a nova base
$traducao_pflcod = array();
$traducao_mnuid = array();

// array com dados de cada tabela
$sistema         = array();
$perfil          = array();
$usuario_sistema = array();
$menu            = array();
$perfilmenu      = array();

// ----- SISTEMA
	$sql = "select * from seguranca.sistema where sisid = " . $sisid;
	$sistema = $db->recuperar( $sql );
	$sistema = $sistema ? $sistema : array();

// ----- PERFIS DO SISTEMA
	$sql = "select * from seguranca.perfil where sisid = " . $sisid;
	$perfil = $db->carregar( $sql );
	$perfil = $perfil ? $perfil : array();
	
// ----- USUARIOS DO SISTEMA
	$sql = "select * from seguranca.usuario_sistema where sisid = " . $sisid;
	$usuario_sistema = $db->carregar( $sql );
	$usuario_sistema = $usuario_sistema ? $usuario_sistema : array();
	
// ----- MENUS DO SISTEMA
	$sql = "select * from seguranca.menu where sisid = " . $sisid . " order by mnutipo";
	$menu = $db->carregar( $sql );
	$menu = $menu ? $menu : array();

// ----- PERFIS DOS MENUS DO SISTEMA 
	$mnuids = array();
	foreach ( $menu as $item )
	{
		array_push( $mnuids, $item['mnuid'] );
	}
	$pflcods = array();
	foreach ( $perfil as $item )
	{
		array_push( $pflcods, $item['pflcod'] );
	}
	if ( count( $mnuids ) && count( $pflcods ) )
	{
		$sql = "select * from seguranca.perfilmenu where mnuid in ( " . implode( ", ", $mnuids ) . " ) and pflcod in ( '" . implode( "', '", $pflcods ) . "' )";
		$perfilmenu = $db->carregar( $sql );
		$perfilmenu = $perfilmenu ? $perfilmenu : array();
	}









header( 'Content-Type: text/plain;' );

print "start transaction;";
print "\n\n";







// ----- dados do sistema -----

$sql_sistema =
<<<EOT
insert into seguranca.sistema ( sisid, sisdsc, sisurl, sisabrev, sisdiretorio, sisfinalidade, sisrelacionado, sispublico, sisstatus, sisexercicio, sismostra, sisemail, paginainicial )
values ( %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s' );
EOT;
	$sql = sprintf(
		$sql_sistema,
			$novo_inicio_sisid,
			$sistema['sisdsc'],
			$sistema['sisurl'],
			$sistema['sisabrev'],
			$sistema['sisdiretorio'],
			$sistema['sisfinalidade'],
			$sistema['sisrelacionado'],
			$sistema['sispublico'],
			$sistema['sisstatus'],
			$sistema['sisexercicio'],
			$sistema['sismostra'],
			$sistema['sisemail'],
			$sistema['paginainicial']
	);
	print "----- dados do sistema\n\n";
	print $sql;
	print "\n\n";
	$sql_sistema_seq =
<<<EOT
alter sequence seguranca.sistema_sisid_seq restart with %d;
EOT;
	$sql = sprintf( $sql_sistema_seq, $novo_inicio_sisid );
	print $sql;
	print "\n\n\n\n";

// ----------------------------









// ----- perfis do sistema -----

	$sql_perfil =
<<<EOT
insert into seguranca.perfil ( pflcod, pfldsc, pfldatainicio, pfldatafim, pflstatus, pflresponsabilidade, pflsncumulativo, pflfinalidade, pflnivel, pfldescricao, sisid, pflsuperuser )
values ( %d, '%s', null, null, '%s', '%s', '%s', '%s', %d, '%s', %d, '%s' );
EOT;
	print "----- dados dos perfis dos sistema\n\n";
	foreach ( $perfil as $item )
	{
		$traducao_pflcod[$item['pflcod']] = $novo_inicio_pflcod++;
		$pflcod = $traducao_pflcod[$item['pflcod']];
		$sql = sprintf(
			$sql_perfil,
				$pflcod,
				$item['pfldsc'],
				$item['pflstatus'],
				$item['pflresponsabilidade'],
				$item['pflsncumulativo'] == 't' ? 't' : 'f',
				$item['pflfinalidade'],
				$item['pflnivel'],
				$item['pfldescricao'],
				$novo_inicio_sisid,
				$item['pflsuperuser'] 
		);
		print $sql;
		print "\n\n";
	}
	
	// altera sequencia utilizada pela tabela perfil
	$sql_sistema_seq =
<<<EOT
alter sequence seguranca.perfil_pflcod_seq restart with %d;
EOT;
	$sql = sprintf( $sql_sistema_seq, $novo_inicio_pflcod - 1 );
	print $sql;
	print "\n\n\n\n";

// -----------------------------









// ----- usuarios do sistema -----

	$sql_usuario_sistema =
<<<EOT
insert into seguranca.usuario_sistema ( usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod ) values ( '%s', %d, '%s', %s, null, '%s' );
EOT;
	print "----- dados dos usuarios do sistema\n\n";
	foreach ( $usuario_sistema as $item )
	{
		$pflcod = $traducao_pflcod[$item['pflcod']];
		$sql = sprintf(
			$sql_usuario_sistema,
				$item['usucpf'],
				$novo_inicio_sisid,
				$item['susstatus'],
				$pflcod ? $pflcod : 'null',
				$item['suscod'] 
		);
		print $sql;
		print "\n\n";
	}
	print "\n\n";

// -------------------------------









// ----- menus do sistema -----

	$sql_menu =
<<<EOT
insert into seguranca.menu ( mnucod, mnucodpai, mnudsc, mnustatus, mnulink, mnutipo, mnustile, mnuhtml, mnusnsubmenu, mnutransacao, mnushow, abacod, mnuhelp, sisid, mnuid, mnuidpai, mnuordem )
values ( %d, %d, '%s', '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s', %s, '%s', %d, %d, %s, %d );
EOT;
	print "----- dados dos menus do sistema\n\n";
	foreach ( $menu as $item )
	{
		$traducao_mnuid[$item['mnuid']] = $novo_inicio_menuid++;
	}
	foreach ( $menu as $item )
	{
		$mnuidpai = $traducao_mnuid[$item['mnuidpai']]; 
		$sql = sprintf(
			$sql_menu,
				$item['mnucod'],
				$item['mnucodpai'],
				$item['mnudsc'],
				$item['mnustatus'],
				$item['mnulink'],
				$item['mnutipo'],
				$item['mnustile'],
				$item['mnuhtml'],
				$item['mnusnsubmenu'],
				$item['mnutransacao'],
				$item['mnushow'],
				$item['abacod'] ? $item['abacod'] : 'null',
				$item['mnuhelp'],
				$item['sisid'],
				$traducao_mnuid[$item['mnuid']],
				$mnuidpai ? $mnuidpai : 'null',
				$item['mnuordem'] 
		);
		print $sql;
		print "\n\n";
	}
	
	// altera sequencia utilizada pela tabela menu
	$sql_sistema_seq =
<<<EOT
alter sequence seguranca.menu_mnuid_seq restart with %d;
EOT;
	$sql = sprintf( $sql_sistema_seq, $novo_inicio_menuid - 1 );
	print $sql;
	print "\n\n\n\n";

// ----------------------------









// ----- usuarios do sistema -----

	$sql_perfilmenu =
<<<EOT
insert into seguranca.perfilmenu ( pflcod, pmnstatus, mnuid ) values ( %d, '%s', %d );
EOT;
	print "----- dados dos perfis dos menus do sistema\n\n";
	foreach ( $perfilmenu as $item )
	{
		$pflcod = $traducao_pflcod[$item['pflcod']];
		$mnuid = $traducao_mnuid[$item['mnuid']];
		$sql = sprintf(
			$sql_perfilmenu,
				$pflcod ? $pflcod : 'null',
				$item['pmnstatus'],
				$mnuid ? $mnuid : 'null'
		);
		print $sql;
		print "\n\n";
	}
	print "\n\n";

// -------------------------------




















