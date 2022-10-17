<?php

# inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/envia_email_sis_geral_funcoes.inc";
$db = new cls_banco();

$suscod = $_REQUEST['statusUsuario'] ? " us.suscod = '". $_REQUEST['statusUsuario'] ."' and ": ""; 

$_REQUEST['municipios'] = $_SESSION['municipios'];

switch($_SESSION['sisarquivo']) {
	case 'cte':
		$exibefiltromunicipios = true; // Mostra o filtro por municipios
		
		if($_REQUEST['filtromun'] == 'sim' && $_REQUEST['perfil'][0]) {
			if($_REQUEST['perfil'] && $_REQUEST['municipios']) {
				// carregando os perfis escolhidos que possam ser filtrados por municipio
				$perfismun = $db->carregar("SELECT * FROM cte.tprperfil WHERE pflcod IN('". implode("','", $_REQUEST['perfil']) ."') AND tprcod = '2'");
				// se algum dos perfis selecionados pode ser filtrado por municipio
				
				if($perfismun) {
					$_REQUEST['perfil'] = array_flip($_REQUEST['perfil']);
					
					$joinIdeb = $_REQUEST['ideb'] ? " INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = ur.muncod " : "";
					$clausulaIdeb = $_REQUEST['ideb'] ?  " AND mtm.tpmid IN ( ". implode(", ", $_REQUEST['ideb'] ) ." ) " : "";					
					
					foreach($perfismun as $per) {
						$sql = "SELECT distinct u.usuemail, u.usunome FROM cte.usuarioresponsabilidade ur
								LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf  
								INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf
								$joinIdeb
								WHERE $suscod
								ur.pflcod = ". $per['pflcod'] ."
								AND us.sisid = ". $_SESSION["sisid"] ."
								AND rpustatus = 'A' 
								AND ur.muncod IN( ". stripslashes( $_REQUEST['municipios'] ) ." )
								$clausulaIdeb";
								
						$desti = $db->carregar($sql);
						// se existe algum usuario desses filtros, processar na opção de outros
						if($desti) {
							foreach($desti AS $de) {
								$_REQUEST["pessoas"] .= $de['usunome']." <".$de['usuemail'].">,"; 
							}
						}
						unset($_REQUEST['perfil'][$per['pflcod']]);							
					}
					$_REQUEST['perfil'] = array_flip($_REQUEST['perfil']);
				}
			}
		}
		break;
}

# captura as informações submetidas
$orgao = (integer) $_REQUEST["orgao"] > 2 ? $_REQUEST["orgao"] : null;
$uo = (array) $_REQUEST["unidadeorcamentaria"];
$ug = (integer) $_REQUEST["unidadegestora"] ? $_REQUEST["unidadegestora"] : null;
$perfis = (array) $_REQUEST["perfil"];
$ideb = (array) $_REQUEST["ideb"];
$outros = $_REQUEST["pessoas"];

# identifica os destinatários
$destinatarios = EmailSistema::identificar_destinatarios( $orgao, $uo, $ug, $perfis, $outros, $_REQUEST['statusUsuario'], $ideb );

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