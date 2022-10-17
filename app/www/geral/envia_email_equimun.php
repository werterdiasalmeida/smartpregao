<?
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$sql = "SELECT us.usuemail FROM cte.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario us ON ur.usucpf = us.usucpf
				WHERE ur.pflcod = '130'
				AND ur.muncod = '".$_REQUEST['muncod']."'";
//ver($sql);
$destinatarios = $db->carregar($sql);

if ($_REQUEST['email']){

  // envia email
  for($x=0; $x<count($destinatarios); $x++){    
  	  
	$assunto = $_REQUEST['assunto'];
	$mensagem = $_REQUEST['email'];  
	$de = $_SESSION['usuemail'];	
	$cc = '';
	$cco = '';
  
	enviar_email_usuario($de, $destinatarios[$x]['usuemail'], $assunto, $mensagem, $cc, $cco);  	
  }  
  ?>
      <script>
         alert('Email enviado com sucesso. Esta janela será fechada.')
         window.close();
      </script>
  <?
  exit();

}
?>
<html>
<head>
<title>Envio de Email</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>

</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
    <center>
    <table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
     <tr>
	 <td colspan="2" align="Center" bgcolor="#dedede">Enviar Email</td>
	 </tr>
	  <tr>
        <td align="right" class="subtitulodireita">Para:</td> 
        <td>
        <?php foreach($destinatarios as $destinatario) echo $destinatario['usuemail'].", "; ?>
        </td>
     </tr>          
	  <tr>
        <td align="right" class="subtitulodireita">Assunto:</td> 
        <td><?=campo_texto('assunto','S','S','',70,100,'','');?></td>
     </tr>
	  <tr>
        <td align="right" class="subtitulodireita">ATENÇÃO:</td> 
        <td><b><font color="red">EVITE COPIAR TEXTOS FORMATADOS DO WORD PORQUE SE O DESTINATÁRIO UTILIZAR O OUTLOOK, A MENSAGEM PODE FICAR CONFUSA E ININTELIGÍVEL!</font></td>
     </tr> 
     <tr>        
        <td colspan="2">
        <? $email= '';?>
        <center>
        <?=campo_textarea('email','N','S','',110,20,'');?>
        </center>
        </td>
     </tr>
	 <tr>
	 <td colspan="2" align="right" class="subtitulodireita"><input type='button' class="botao" value='Enviar E-mail' onclick="envia_email()">&nbsp;&nbsp;&nbsp;<input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>
	 </tr>
  </table>
</form>
<script language="JavaScript" src="../includes/tiny_mce.js"></script> 
<script type="text/javascript">
	// Editor de textos
	tinyMCE.init({
		mode : "textareas",
		theme : "advanced",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,forecolor,backcolor,fontsizeselect,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist, code",
		theme_advanced_buttons2 : "",
		theme_advanced_buttons3 : "",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		language : "pt_br",
		entity_encoding : "raw"
	});
	function fechar_janela() {
				window.close();
			}
			
	function envia_email() {
		if ( !validaBranco( document.formulario.assunto, 'Assunto' ) ) {
			return;
		}
		document.formulario.submit();
	}
</script>
</body>
</html>
