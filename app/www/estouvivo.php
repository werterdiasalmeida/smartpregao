<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Cache-control: private, no-cache");
header("Last-Modified: " . gmdate( "D, d M Y H:i:s" ) . " GMT");
header("Pragma: no-cache");

require_once "config.inc";
$usunome = $_SESSION["usunome"];
$cpf = $_SESSION["usucpforigem"];

if( (time() - $_SESSION["evHoraUltimoAcesso"]) >= MAXONLINETIME or !$cpf) {
	session_unset();
	die("EXIT");
}
session_write_close();


///conexão dbsimec apoio        
$GLOBALS["connapoio"] = "host=$servidor_bd_apoio port=$porta_bd_apoio dbname=$nome_bd_apoio user=$usuario_db_apoio password=$senha_bd_apoio";
$conn = pg_connect( $GLOBALS["connapoio"] );
pg_query( $conn, "SET search_path TO seguranca,public" );
pg_set_client_encoding( $conn, 'LATIN5' );


// VERIFICA QUAL OPERAÇÂO REALIZAR
switch( $_REQUEST["op"] )
{
	case "apagar":
		error_reporting( E_ALL );
		ini_set( 'display_error', 1 );
		header( 'Content-Type: text/plain; charset=iso-8859-1' );
		$ids = trim( $_REQUEST["msglist"] );
		if ( $ids{0} == ',' )
		{
			echo 'ok';
			break;
		}
		$sqlApagar = "DELETE FROM seguranca.mensagemchat
			WHERE msgid IN ($ids)
			AND usucpfdestino = '{$_SESSION["usucpforigem"]}'";
		//print $sqlApagar;
		//exit();
		pg_query($conn, "BEGIN");
		$rs = pg_query($conn, $sqlApagar);
		pg_query($conn, "COMMIT");
		echo pg_affected_rows( $rs ) > 0 ? 'ok' : 'erro';
		//echo "ok";
	break;
	case "enviar":
		$_SESSION["evHoraUltimoAcesso"] = time();
		header('Content-Type: text/plain; charset=iso-8859-1');
		pg_query($conn, "BEGIN");
		$mensagem = substr(str_replace("]]>", "", trim(strip_tags($_REQUEST["msg"]))),0,500);
		//Insere Mensagens no Banco
			$sqlInserir = "INSERT INTO seguranca.mensagemchat (usucpforigem, usunome, usucpfdestino, msgdsc) VALUES ('%s', '%s', '%s', '%s')";
			$sql = sprintf($sqlInserir, $cpf, $usunome, $_REQUEST["usucpfdestino"], $mensagem);
			$rs = pg_query($conn, $sql);
			$sqlHistorico = "INSERT INTO seguranca.mensagemchathistorico (usucpforigem, usunome, usucpfdestino, msgdsc) VALUES ('%s', '%s', '%s', '%s')";
			$sql = sprintf($sqlHistorico, $cpf, $usunome, $_REQUEST["usucpfdestino"], $mensagem);
			$rs = pg_query($conn, $sql);
		pg_query($conn, "COMMIT");
		echo pg_affected_rows( $rs ) > 0 ? 'ok' : 'erro';
	break;
	default:
		//Controla usuarios online
		//pg_query($conn, "BEGIN");
		//	$sqlEst = "UPDATE seguranca.estatistica SET estdata = NOW() WHERE oid = " . $oid;
			//var_dump($sqlEst);
		//	pg_query($conn, $sqlEst);
		//pg_query($conn, "COMMIT");

		/*//Usuários Online dinãmico desabilitado temporariamente
		$sqlUsuariosOnline = "select qtdonline from seguranca.qtdusuariosonline where sisid = {$_SESSION["sisid"]}";
		//$sqlUsuariosOnline = "SELECT COUNT(*) FROM seguranca.usuariosonline WHERE sisid = {$_SESSION["sisid"]}";
		$rs = pg_query($conn, $sqlUsuariosOnline);
		$usuariosOnLine = 0;
		if($rs && $dados = pg_fetch_array($rs)) {
			$_SESSION['qtdusuariosonline'][$_SESSION['sisid']] = array_shift($dados);
		}
		*/
		//Pega as mensagens ainda não lidas
		$sqlChat = "SELECT mc.msgid, mc.usucpforigem , mc.msgdsc, mc.usunome, " .
				" TO_CHAR(msgdtenviada, 'DD/MM/YYYY') AS data," . 
				" TO_CHAR(msgdtenviada, 'HH24:MI:SS') AS hora" .
				" FROM seguranca.mensagemchat mc " .
				" WHERE usucpfdestino = '" . $cpf . "' " . 
				" ORDER BY mc.usucpforigem, mc.msgdtenviada";
		$rs = pg_query( $conn, $sqlChat );
		header('Content-Type: text/xml; charset=iso-8859-1');
		//print pg_num_rows($rs);
		//exit();
		echo "<evChat usuariosOnLine=\"" . $_SESSION['qtdusuariosonline'][$_SESSION['sisid']] . "\">\n";
		if( @pg_num_rows($rs) > 0 )
		{
			echo "<arrayOfMensagens>\n";
			while( $linha = pg_fetch_assoc( $rs ) )
			{
			    $nomeRemetente = explode( " ", $linha["usunome"] );
				$nomeRemetente = ucfirst( strtolower( $nomeRemetente[0] ) );
				echo "<mensagem id=\"" . $linha["msgid"] . "\" data=\"" . $linha["data"] . "\" hora=\"" . $linha["hora"] . "\" remetente=\"" . $linha["usucpforigem"] . "\" nome=\"" . $nomeRemetente . "\">";
					echo "<![CDATA[" . htmlentities(rawurldecode(utf8_encode($linha["msgdsc"]))) . "]]>";
				echo "</mensagem>\n";
			}
			echo "</arrayOfMensagens>\n";
		}
		echo "</evChat>";
	break;
}
// @pg_close($conn);
echo "\n";