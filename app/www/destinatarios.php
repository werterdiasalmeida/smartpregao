<?php

// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

function pegar_destinatarios( $orgao = null, array $uo = array(), $ug = 0, array $perfis = array() ){
	global $db;
	
	# conjunto de regras de restrição
	$restricao = array();
	
	# restringe os usuários por órgão
	if ( !empty( $orgao ) ) {
		array_push( $restricao, " orgcod = '" . $orgao ."' " );
	}
	
	# restringe os usuários por unidade orçamentária
	if ( !empty( $uo ) ) {
		array_push( $restricao, " unicod in ( '". implode( "','", $uo ) ."' ) " );
	}
	
	# restringe os usuários por unidade gestora
	if ( !empty( $ug ) ) {
		array_push( $restricao, " ungcod = '" . $ug ."' " );
	}
	
	# restringe os usuários por perfil
	$join_perfil = "";
	if ( !empty( $perfis ) ) {
		$esquema = $_SESSION["sisdiretorio"];
		$join_perfil = sprintf(
			" inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf " .
			" inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.sisid = s.sisid and p.pflstatus = 'A' and p.pflcod in ( %s ) " .
			" inner join %s.tprperfil rp on rp.pflcod = p.pflcod " .
			" inner join %s.usuarioresponsabilidade ur on ur.pflcod = rp.pflcod and ru.rpustatus = 'A' ",
			implode( ",", $perfis ),
			$esquema,
			$esquema
		);
	}
	if ( empty( $restricao ) && empty( $join_perfil ) ) {
		return array();
	}
	if ( empty( $restricao ) ) {
		array_push( $restricao, " 1 = 1 " );
	}
	$sql = sprintf(
		"select u.usunome, u.usuemail
		from seguranca.usuario u
		inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
		inner join seguranca.sistema s on s.sisid = us.sisid
		%s
		where u.suscod = 'A' and s.sisid = %d and s.sisstatus = 'A' and %s",
		$join_perfil,
		$_SESSION["sisid"],
		implode( " and ", $restricao )
	);
	$destinatarios = $db->carregar( $sql );
	if ( $destinatarios ) {
		return $destinatarios;
	}
	return array();
}

$orgao = (integer) $_REQUEST["orgao"];
$uo = (array) $_REQUEST["uo"];
$ug = (integer) $_REQUEST["ug"];
$perfis = (array) $_REQUEST["perfis"];
$outros = $_REQUEST["outros"];

$destinatarios = pegar_destinatarios( $orgao, $uo, $ug, $perfis );
foreach ( explode( ",", $outros ) as $item ) {
	if ( empty( $item ) ) {
		continue;
	}
	if ( strpos( $item, "<" ) ) {
		preg_match("/([\w\pL[:space:]]{0,})/", $item, $saida );
		$nome = $saida[0];
		preg_match("/<(.*)>/", $item, $saida );
		$email = $saida[1];
	} else {
		$nome = "";
		$email = $item;
	}
	$registro = array(
		"usunome" => trim( $nome ),
		"usuemail" => trim( $email )
	);
	array_push( $destinatarios, $registro );
}

?>

<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title>Destinatários</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<?php if( !empty( $destinatarios ) ): ?>
			<p style="font-size: 12px; font-weight: bold; margin: 5px;">Total: <?= count( $destinatarios ); ?></p>
			<table class='tabela' style="width:100%;"  cellpadding="3">
				<thead>
					<tr style="background-color: #e0e0e0">
						<td style="font-weight:bold; text-align:center; width:60%">Nome</td>
						<td style="font-weight:bold; text-align:center; width:40%">E-mail</td>
					</tr>
				</thead>
				<tbody>
				<?php foreach( $destinatarios as $indice => $destinatario ): ?>
					<?php $cor = $cor == '#fafafa' ? '#f0f0f0' : '#fafafa'; ?>
					<tr style="vertical-align:top; background-color: <?= $cor ?>" onmouseover="this.style.backgroundColor='#ffffcc';" onmouseout="this.style.backgroundColor='<?= $cor ?>';">
						<td style="text-align:left"><?= $destinatario['usunome'] ?></td>
						<td style="text-align:left"><?= $destinatario['usuemail'] ?></td>
					</tr>
					<? $total += $orcamento['orcvalor'] ?>
				<?php endforeach; ?>
				</tbody>
			</table>
		<?php else: ?>
			<table class='tabela' style="width:100%; height: 100%" cellpadding="3">
				<tbody>
					<td style="text-align:center;padding:15px;background-color:#f5f5f5;">
						Não há destinatários para os filtros indicados.
					</td>
				</tbody>
			</table>
		<?php endif; ?>
	</body>
</html>