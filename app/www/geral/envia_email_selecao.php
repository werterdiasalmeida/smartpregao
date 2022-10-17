<?php
# inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/envia_email_sis_geral_funcoes.inc";
$db = new cls_banco();

if($_REQUEST['ptiid']){
	
	$_SESSION['emanda']['registraemail'] = 0;
}

if ( $_REQUEST[ "enviar" ]){
	
	$arDestinatariosSelecao = array();
	if( is_array( $_REQUEST["arDestinatarios"] ) ){
		foreach( $_REQUEST["arDestinatarios"] as $indice => $stDados ){
			
			// Nome
			$arDestinatariosSelecao[$indice]['usunome'] = substr( $stDados, 0, strpos( $stDados, "||" ) );
			$restante = substr( $stDados, strpos( $stDados, "||" ) +2 );
			
			// E-mail
			$arDestinatariosSelecao[$indice]['usuemail'] = substr( $restante, 0, strpos( $restante, "||" ) );
			$restante = substr( $restante, strpos( $restante, "||" ) +2 );
					
			// CPF
			$arDestinatariosSelecao[$indice]['usucpf'] = $restante;
		}
	}
	
	if( $_REQUEST["stNomeRemetente"] && $_REQUEST["stEmailRemetente"] ){
		$remetente['usunome'] = $_REQUEST["stNomeRemetente"];
		$remetente['usuemail'] = $_REQUEST["stEmailRemetente"];
		$remetente['usucpf'] = "";
	} else {
		$sql = "select distinct u.usunome, u.usuemail, u.usucpf
				from seguranca.usuario u				
				where u.usucpf = '".$_SESSION['usucpf']."'
			    group by u.usunome, u.usuemail, u.usucpf ";
								
		$remetente = $db->carregar( $sql );
		$remetente = $remetente[0];

	}
	
	
	array_push( $arDestinatariosSelecao, $remetente );
	
	$assunto = $_REQUEST["assunto"];
	$conteudo = $_REQUEST["mensagem"];		
	//----------------------------------------------------------------------------------

	# envia as mensagens
	$mensagem = new EmailSistema();
	
	if ( !$mensagem->enviar( $arDestinatariosSelecao, $assunto, $conteudo, $_SESSION["FILES"], $remetente, $_SESSION["destino"], $_SESSION['emanda']['registraemail'] ) ) {
		$db->rollback();
		echo '<script type="text/javascript">alert( "Ocorreu uma falha ao enviar a mensagem.")</script>';
		echo '<script type="text/javascript">history.go(-2);</script>';
	}
	else{
		$db->commit();
		echo '<script type="text/javascript">alert( "Operação efetuada com sucesso.")</script>';
		echo '<script type="text/javascript">history.go(-2);</script>';
	}

die();
	
}

$destino = "";

foreach( $_FILES as $arquivo ) {
	if ( $arquivo["error"] == UPLOAD_ERR_NO_FILE ) {
		continue;
	}
	
	$destino = dirname( $arquivo["tmp_name"] ) . "/" . $arquivo["name"];
	
	if ( !move_uploaded_file( $arquivo["tmp_name"], $destino ) ) {
		return false;
	}
}
$_SESSION["destino"] = $destino;

$_SESSION["FILES"] = $_FILES;

$_REQUEST['statusUsuario'] = ($_REQUEST['statusUsuario'] ) ? $_REQUEST['statusUsuario'] : 'A';

$suscod = $_REQUEST['statusUsuario'] ? " us.suscod = '". $_REQUEST['statusUsuario'] ."' and ": ""; 
//$arUF = array();
$arMunicipios = array();

switch($_SESSION['sisarquivo']) {
	case 'cte':
		$exibefiltromunicipios = true; // Mostra o filtro por municipios
		
		if($_REQUEST['filtromun'] == 'sim' && $_REQUEST['perfil'][0] ) {
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
								AND ur.muncod IN('". implode("','", $_REQUEST['municipios']) ."')
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
$tipoEnsino = (integer) $_REQUEST["tipoensino"] ? $_REQUEST["tipoensino"] : null;
$uo = (array) $_REQUEST["unidadeorcamentaria"];
$ug = (integer) $_REQUEST["unidadegestora"] ? $_REQUEST["unidadegestora"] : null;
$perfis = (array) $_REQUEST["perfil"];
$ideb = (array) $_REQUEST["ideb"];
$outros = $_REQUEST["pessoas"];
//$arUF = $_REQUEST["estuf"];
$arMunicipios = $_REQUEST["municipiosUsuario"];
$assunto = $_REQUEST["assunto"];
$conteudo = $_REQUEST["mensagem"];
$statusUsuario = $_REQUEST["statusUsuario"];
$cargo = $_REQUEST["cargo"];

# identifica os destinatários
$destinatarios = EmailSistema::identificar_destinatarios( $orgao, $tipoEnsino, $uo, $ug, $perfis, $outros, $statusUsuario, $ideb, $arMunicipios, $cargo );
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
			
			<form id="formulario" method="post" name="formulario" enctype="multipart/form-data">
				<input type="submit" name="enviar" value="Enviar" />				
				<table class='tabela' style="width:100%;"  cellpadding="3">
					<thead>
						<tr style="background-color: #f0f0f0">
							<td style="text-align:left"><input checked="checked" type="checkbox" id="selecionarTodos" onclick="javascript: marcarTodosChecks( 'selecionarTodos', 'arDestinatarios[]' );" /></td>
							<td colspan="4" style="text-align:left">Selecionar Todos</td>
						</tr>
						<tr style="background-color: #e0e0e0">
							<td style="font-weight:bold; text-align:center; width:2%">&nbsp;</td>
							<td style="font-weight:bold; text-align:center; width:50%">Nome</td>
							<td style="font-weight:bold; text-align:center; width:30%">E-mail</td>
							<td style="font-weight:bold; text-align:center; width:05%">Estado</td>
							<td style="font-weight:bold; text-align:center; width:13%">Município</td>
						</tr>
					</thead>
					<tbody>
					<?php foreach( $destinatarios as $indice => $destinatario ): ?>
						<?php $cor = $cor == '#fafafa' ? '#f0f0f0' : '#fafafa'; ?>
						<tr style="vertical-align:top; background-color: <?php echo $cor; ?>;" onmouseover="this.style.backgroundColor='#ffffcc';" onmouseout="this.style.backgroundColor='<?php echo $cor; ?>';">
							<td style="text-align:left">
								<input 	name="arDestinatarios[]" type="checkbox" checked="checked" value="<?php echo $destinatario['usunome']. '||' . $destinatario['usuemail']. '||' .$destinatario['usucpf'] ?>" />
							</td>
							<td style="text-align:left"><?= $destinatario['usunome']?></td>
							<td style="text-align:left"><?= $destinatario['usuemail'] ?></td>
							<td style="text-align:left"><?= $destinatario['regcod'] ?></td>
							<td style="text-align:left"><?= $destinatario['mundescricao'] ?></td>
						</tr>
						<? $total += $orcamento['orcvalor'] ?>
					<?php endforeach; ?>
					</tbody>
				</table>
				<div>
				<input type="hidden" value="<?php echo $_REQUEST["stNomeRemetente"] ?>" name="stNomeRemetente" id="stNomeRemetente" />
				<input type="hidden" value="<?php echo $_REQUEST["stEmailRemetente"] ?>" name="stEmailRemetente" id="stEmailRemetente" />
				<input type="hidden" value="<?php echo $_REQUEST["assunto"]; ?>" name="assunto" id="assunto" />
				<input type="hidden" value="<?php echo str_replace( '"', "'", $_REQUEST["mensagem"] ) ?>" name="mensagem" id="mensagem" />
				<input type="submit" name="enviar" value="Enviar" />
				<input type="button" value="Voltar" name="voltar" onclick="javascript: history.go(-1);" />
				</div>
			</form>	
		<?php else: ?>
			<table class='tabela' style="width:100%; height: 100%" cellpadding="3">
				<tbody>
					<tr>
						<td style="text-align:center;padding:15px;background-color:#f5f5f5;">
							Não há destinatários para os filtros indicados.<br />
							<input type="button" value="Voltar" name="voltar" onclick="javascript: history.go(-1);" />
						</td>
					</tr>
				</tbody>
			</table>
		<?php endif; ?>
	</body>
</html>