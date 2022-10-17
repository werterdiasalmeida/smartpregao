<?php

/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Analista: Cristiano Cabral
   Programador: Renan de Lima (renandelima@gmail.com)
   Módulo: envia_email.inc
   Finalidade: permitir escrever e enviar email
*/

// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();
$assunto = $_REQUEST['assunto'];

if ( $_REQUEST['email'] ) {
	if ( ereg_replace( "<[^>]*>", "", $_REQUEST['email'] ) == '' ) {
		?>
			<script>
				alert ( 'O texto do e-mail não pode estar vazio.' );
				history.back();
			</script>
		<?php
		exit();
	} else {
		require APPRAIZ . '/includes/Email.php';
		$destinatarios = array( $_REQUEST['cpf'] );
		$copia_oculta = array();
		$conteudo = $_REQUEST['email'];
		if ( $_REQUEST['cco'] ) {
			array_push( $copia_oculta, $_REQUEST['cco'] );
		}
		$email = new Email();
		$email->enviar( $destinatarios, $assunto, $conteudo, array(), true, true, $copia_oculta );
		?>
			<script>
				alert( 'Email enviado com sucesso. Esta janela será fechada.' );
				window.close();
			</script>
		<?php
		exit();
	}
}

// captura dados do usuário
$sql  = "
	select
		u.usunome,
		u.usufoneddd,
		usufonenum,
		o.orgdsc,
		uni.unidsc,
		ug.ungdsc
	from usuario u
		left join public.orgao o on
			u.orgcod = o.orgcod
		left join public.unidade uni on
			uni.unicod = u.unicod and
			uni.unitpocod = 'U' and
			uni.unistatus = 'A'
		left join public.unidadegestora ug on
			ug.ungcod = u.ungcod
	where
		u.usucpf = '" . $_REQUEST['cpf'] . "'
	";
$RSu  = $db->record_set( $sql );
$resu =  $db->carrega_registro( $RSu, 0 );
extract( $resu );

?>
<html>
	<head>
		<title>Envio de Email</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href="../includes/listagem.css">
		<script language="JavaScript" src="../includes/funcoes.js"></script>
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
	</head>
	<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
		<form method="POST"  name="formulario">
			<input type=hidden name="modulo" value="<?= $modulo ?>">
			<input type=hidden name="cpf" value="<?= $_REQUEST['cpf'] ?>">
			<table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<tr>
					<td colspan="2" align="Center" bgcolor="#dedede">Enviar Email</td>
				</tr>
				<tr>
					<td align="right" class="subtitulodireita">
						Para:
					</td> 
					<td>
						<?php $nome = $usunome . ' - (' . $usufoneddd . ') ' . $usufonenum; ?>
						<?= campo_texto( 'nome', 'N', 'N', '', 70, 100, '', '' ); ?>
					</td>
				</tr>
				<tr>
					<td align="right" class="subtitulodireita">
						Órgão:
					</td> 
					<td>
						<?= campo_texto( 'orgdsc', 'N', 'N', '', 70, 100, '', '' ); ?>
					</td>
				</tr>
				<tr>
					<td align="right" class="subtitulodireita">
						Unidade Orçamentária:
					</td> 
					<td>
						<?= campo_texto( 'unidsc', 'N', 'N', '', 70, 100, '', '' ); ?>
					</td>
				</tr>
				<tr>
					<td align="right" class="subtitulodireita">
						Unidade Gestora:
					</td> 
					<td>
						<?= campo_texto( 'ungdsc', 'N', 'N', '', 70, 100, '', '' ); ?>
					</td>
				</tr>
				<tr>
					<td align="right" class="subtitulodireita">
						Cco:
					</td> 
					<td>
						<?= campo_texto( 'cco', 'N', 'S', '', 70, 100, '', '' ); ?>
					</td>
				</tr>     
				<tr>
					<td align="right" class="subtitulodireita">
						Assunto:
					</td> 
					<td>
						<?= campo_texto( 'assunto', 'S', 'S', '', 70, 100, '', '' ); ?>
					</td>
				</tr>
				<tr>
					<td align="right" class="subtitulodireita">ATENÇÃO:</td> 
					<td><b><font color="red">EVITE COPIAR TEXTOS FORMATADOS DO WORD PORQUE SE O DESTINATÁRIO UTILIZAR O OUTLOOK, A MENSAGEM PODE FICAR CONFUSA E ININTELIGÍVEL!</font></td>
				</tr>
				<tr>     
					<td colspan=2>
						<textarea name="email" cols="90" rows="20"></textarea>
					</td>
				</tr>
				<tr>
					<td colspan="2" align="right" class="subtitulodireita">
						<input type='button' class="botao" value='Enviar E-mail' onclick="envia_email();">
						&nbsp;&nbsp;&nbsp;
						<input type='button' class="botao" value='Fechar' onclick="fechar_janela();">
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>