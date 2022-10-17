<?

if($_REQUEST['acao'] == "enviarsms") {
	$pessoas = explode(",", strtolower($_REQUEST['pessoas']));
	$sql = "SELECT respid, respemail, CAST(respdddcelular AS varchar) || CAST(respcelular AS varchar) as telefone FROM painel.responsavelsecretaria WHERE LOWER(respnome) || ' <' || LOWER(respemail) || '>' IN ('".implode("','", $pessoas)."')";
	$encaminhados = $db->carregar($sql);
	
	if($encaminhados[0]) {
		require_once('../webservice/painel/nusoap.php');
		$client = new soapcliente('https://webservice.cgi2sms.com.br/axis/services/VolaSDKSecure?wsdl', true);
		$err = $client->getError();
		if($err) die('<h2>Constructor error</h2><pre>' . $err . '</pre>');
		
		foreach($encaminhados as $encam) {
			if(strlen($encam['telefone']) == 10) {
				$envio = $client->call('sendMessage', array('user' => 'inep', 'password' => 'tmmjee', 'testMode' => false, 'sender' => 'SIMEC',	'target' => '55'.$encam['telefone'], 'body' => $_REQUEST['mensagem'], 'ID' => substr($encam['usucpf'],0,6).date("Ymdhis")));
			}
		}
	}
	die("<script>alert('SMS enviado com sucesso');window.location='painel.php?modulo=principal/enviarsms&acao=A';</script>");
}


// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";

monta_titulo('Enviar SMS', '&nbsp;');

?>
<!-- AUTO COMPLEMENTO -->
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/keys/keyEvents.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/tags/suggest.js"></script>
<script>

function enviarsms(btn) {

	if(document.getElementById('mensagem').value == "") {
		alert('Mensagem é obrigatória.');
		return false;
	}
	
	document.getElementById('formulario').submit();
	btn.disabled=true
}
	

</script>
<style>
.suggestMarked {
	background-color: #AAAAFF;
	width:500px; 
	border-style: solid; 
	border-width: 1px; 
	border-color: #DDDDFF;
	border-top-width: 0px;
	position: relative;
	z-index: 100;
}

.suggestUnmarked {
	background-color: #EEEEFF;
	width:500px; 
	border-style: solid; 
	border-width: 1px; 
	border-color: #DDDDFF;
	border-top-width: 0px;
	z-index: 100;
	position: relative;
}

fieldset {
	width: 475px;
	padding: 10px;
}

fieldset input {
	width: 150px;
}

fieldset label {
	display: block;
	width: 40px;
	float: left;
}

fieldset br {
	clear: both;
}
	
</style>
<form method="post" name="formulario" id="formulario">
<input type="hidden" name="acao" value="enviarsms">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
<td class="SubTituloDireita">Destinatários:</td>
<td>
					<textarea type="text" name="pessoas" id="pessoas" style="width: 600px; height: 100px" ></textarea>
					<br/>
					<div id="divSuggestPessoas" style="position: absolute; z-index: 1000;"></div>
					<script>
						objSuggestPessoas = new window.Suggest( 
							document.getElementById( "pessoas" ) , 
							document.getElementById( "divSuggestPessoas" ),
							"suggestlist.php"
						);
					</script>
</td>
</tr>
<tr>
	<td class="SubTituloDireita">Mensagem:</td>
	<td ><? echo campo_textarea( 'mensagem', 'N', 'S', '', 120, 7, 145 ); ?></td>
</tr>
<tr bgcolor="#cccccc">
	<td colspan="2"><input type="button" name="enviassms" value="Enviar" onclick="enviarsms(this);"></td>
</tr>
</table>
</form>