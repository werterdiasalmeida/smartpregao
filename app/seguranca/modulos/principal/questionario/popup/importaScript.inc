<?php
include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/QGeraScript.class.inc";

if( $_POST ){
	if( $_FILES['arquivo']["tmp_name"] ){
		$obGeraScript = new QGeraScript();
		$alerta = $obGeraScript->extrairArquivo( $_FILES['arquivo']["tmp_name"], $_POST['sisfiltro'] );
	} else {
		$alerta = "Erro no arquivo!";
	}
	echo "<script>
			alert( '{$alerta}' );
		</script>";
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Importação de Script de Questionários', '' );
?>
<script language="JavaScript">
	function valida(){
		if( document.getElementById( 'sisfiltro' ).value == '' ){
			alert('Você precisa selecionar o Sistema onde o Questionário será gerado.');
		} else {
			if( confirm( 'Tem certeza que deseja gerar esse questionário?' ) ){
				document.formulario.submit();
			}
		}
	}
</script>
<form  enctype="multipart/form-data" method="post" name="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td><font>Filtre o sistema:</font> 
		<? 
		$sql = "SELECT
					sisid as codigo,
					sisdsc as descricao
				FROM
					seguranca.sistema
				WHERE
					sisstatus = 'A'";
		$db->monta_combo( "sisfiltro", $sql, "S", 'Selecione um nível', '', '', '', '300','S','sisfiltro','',$_GET['sisidFiltro'], 'Sistemas');
		?>
		</td>
	</tr>	
	<tr>
		<td><font>Arquivo:</font> 
		<input type="file" name="arquivo" >
		</td>
	</tr>
	<tr>
		<td> 
			<input type="button" value="Enviar" onclick="valida();">
		</td> 
	</tr>	
</table>
</form>