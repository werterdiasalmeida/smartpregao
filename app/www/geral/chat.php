<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Cache-control: private, no-cache");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Pragma: no-cache");

require_once "config.inc";

define( 'DEBUG', false );

$cpf_origem = $_SESSION["usucpforigem"];
session_write_close();

set_time_limit(0);
ini_set("output_buffering", 0);
ini_set("implicit_flush", 1);
ignore_user_abort(false);



$mensagem = trim( strip_tags( $_REQUEST['msg'] ) );
$cpf_destino = trim( strip_tags( $_REQUEST['cpf'] ) );

if ( !$cpf_origem ) {
	?><script type="text/javascript">self.close();</script><?
}
/*
$conn = pg_connect("host=$servidor_bd port=$porta_bd user=$usuario_db password=$senha_bd dbname=$nome_bd");
$sqlUsuario = "SELECT * FROM seguranca.usuario WHERE usucpf = '%s'";
$sql = sprintf($sqlUsuario, $cpf_destino);
$rsUsuario = pg_query($sql);
$usuario = pg_fetch_assoc($rsUsuario);

$nomeRemetente = explode(" ", $usuario["usunome"]);
$nomeRemetente = ucfirst(strtolower($nomeRemetente[0]));

pg_close($conn);
*/
$nomeRemetente = $_REQUEST['nome'];
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Frameset//EN" "http://www.w3.org/TR/html4/frameset.dtd">
<!-- <?= $hash ?> -->
<html>
	<head>
		<title><?=$nomeRemetente?></title>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<style type="text/css">

#msgBuffer {
	font-family: Verdana;
	font-size: 10px;
	padding: 0px;
	width: 248px;
	height: 330px;
	border: 1px solid #333;
	background-color: #ffffff;
	overflow-x: auto;
	overflow-y: scroll;
	text-align: left;
	margin:0px;
}
#msgForm {
height: 120px;
font-family: Arial;
background-color: #fcfcfc;
font-size: 10px;
margin:0px;
}
#msg {
	padding: 0px;
	width: 246px;
	height: 50px;
}
#msgorigem
{
	padding: 3px;
	margin-bottom:2px;
	background-color: #E0ECFF;
}
#msgdestino
{
	padding: 3px;
	margin-bottom:2px;
	background-color: #efefef;
}

</style>
<script language="Javascript">

function keyHandler( e, frm ) {
   var asc = document.all ? event.keyCode : e.which;
   
   if(asc == 13) {
      enviarMsg(frm);
   }
   return asc != 13;
}

var nomeRemetente = "<?=$nomeRemetente?>";

/**
 * Criar um objeto para requisição ajax
 * 
 * @param void
 * @return XMLHttpRequest
 */
function evCriaHttpRequest2()
{	
	var http_request = false;
    if ( window.XMLHttpRequest ) { // Mozilla, Safari,...
        http_request = new XMLHttpRequest();
    } else if (window.ActiveXObject) { // IE
        try {
            http_request = new ActiveXObject( "Msxml2.XMLHTTP" );
        } catch (e) {
            try {
                http_request = new ActiveXObject( "Microsoft.XMLHTTP" );
            } catch (e) {}
        }
    }
    if ( !http_request ) {
        return false;
    }
    return http_request;
}

function apagarMensagens2( ids )
{
	try
	{
		var chHttp = evCriaHttpRequest2();
		chHttp.open( 'POST', '/estouvivo.php',false);
		chHttp.setRequestHeader( 'Content-Type', 'application/x-www-form-urlencoded' );
		chHttp.send( 'op=apagar&msglist=' + escape( ids ) );
		div_debug.innerHTML = evTrim2( chHttp.responseText );
		return evTrim2( chHttp.responseText );
	}
	catch(e) {}
}

function enviarMensagem2( usucpfdestino, msg )
{
	try
	{
		var chHttp = evCriaHttpRequest2();
		chHttp.open( 'POST', "/estouvivo.php", false );
		chHttp.setRequestHeader( 'Content-Type', 'application/x-www-form-urlencoded' );
		chHttp.send( "op=enviar&usucpfdestino=" + usucpfdestino + "&msg=" + escape( msg ) );
		return evTrim2( chHttp.responseText );
	}
	catch(e){}
	return 'erro';
}

function evTrim2( text )
{
	if( ( text == null ) || ( typeof text == "undefined" ) )
	{
		return null;
	}
	return text.replace( /^\s*|\s*$/g, "" );
}









function escreverMsg(id, data, msg, remetente)
{	
	var msgBox = document.forms[0].msg;
	var nome = remetente ? nomeRemetente : 'eu';
	var cssmsg = remetente ? 'msgdestino' : 'msgorigem';

	var msgBuffer = document.getElementById("msgBuffer");
	msgBuffer.innerHTML += "<div id=\""+cssmsg+"\"><span class=\"msgDataEnviada\">(" + data + ")</span> <b class=\"msgRemetente\">" + nome + ":</b> " + msg.substr(0,500) + "</div>\n" 
	//if (id!=0){window.opener.apagarMensagens( id );}
	if ( id != 0 )
	{
		apagarMensagens2( id );
	}
	msgBuffer.scrollTop = msgBuffer.scrollHeight - msgBuffer.clientHeight;
	self.focus();
	msgBox.focus();
}
function enviarMsg( frm )
{
	var msgBox = frm.msg;
	var msgenviada = window.opener.evTrim( msgBox.value.substr( 0, 500 ) );
	msgBox.value = '';
	if ( msgenviada != '' )
	{
		//var strRetorno = window.opener.enviarMensagem( frm.cpf.value, msgenviada );
		var strRetorno = enviarMensagem2( frm.cpf.value, msgenviada );
		if( strRetorno == 'ok' )
		{
			Stamp = new Date();
			var h = String( Stamp.getHours() );
			var m = String( Stamp.getMinutes() );
			var s = String( Stamp.getSeconds() );
	        h = (h.length > 1) ? h : "0"+h; m = (m.length > 1) ? m : "0"+m; s = (s.length > 1) ? s : "0"+s;
	        var hora = h + ":" + m + ":" + s;
		    escreverMsg( 0, hora, msgenviada, false );
		}
		
	} 

	return false;
}
</script>
</head>
	<body bgcolor="#ffffff" marginheight="0" marginwidth="0" topmargin="0" leftmargin="0">
		<div id="msgBuffer"></div>
		<div id="debug" style="background-color:yellow;height:50px;display:<?= DEBUG ? 'inline' : 'none' ?>"></div>
		<div id="msgForm">
			<form method="post" onSubmit="return(false);" style="margin:0px;">
				<input type="hidden" name="cpf" value="<?= $cpf_destino ?>"/>
				Para enviar, digite o texto e pressione &lt;Enter&gt;:
				<textarea maxlength="500" name="msg" id="msg" onkeyup="keyHandler(event,this.form);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"></textarea>
			</form>
		</div>
	</body>
</html>
<script type="text/javascript">
	self.focus();
	document.forms[0].msg.focus();
	var div_debug = document.getElementById( 'debug' );
</script>
<script type="text/javascript">
	var carregou = true;
	window.opener.evIniciarProcesso();
</script>
