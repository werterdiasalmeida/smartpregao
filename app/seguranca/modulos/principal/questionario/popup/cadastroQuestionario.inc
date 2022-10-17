<?php 
include_once APPRAIZ.'includes/classes/modelo/questionario/QQuestionario.class.inc';
include_once APPRAIZ.'includes/classes/modelo/questionario/QQuestionarioResposta.class.inc';

$obQuestionario 		= new QQuestionario( $_REQUEST['queid'] );
$obQuestionarioresposta = new QQuestionarioresposta();

if($_POST['submeter'] == 'salvar'){
	$arCampos = array( "queid", "sisid", "quetitulo", "quedescricao", "quedtinicio", "quedtfim" );	
	
	$arDados = $_POST;
	$arDados['quedtinicio'] = formata_data_sql( $arDados['quedtinicio'] );
	$arDados['quedtfim'] 	= formata_data_sql( $arDados['quedtfim'] );
	$arDados['sisid'] 	    = $arDados['sisid'];
	
	$obQuestionario->popularObjeto( $arCampos, $arDados );
	
	if( $queid = $obQuestionario->salvar() ){
		$obQuestionario->commit();
		
		
		/*    //SALVA O QUEID NA TABELA QUESTIONARIORESPOSTA!!
		 *  
		$arCampos = array( "queid", "qrptitulo", "qrpdata" );
		$arDados = array( "queid" => $queid, "qrptitulo" => 'teste', "qrpdata" => 'now()' );
		
		$obQuestionarioresposta->popularObjeto( $arCampos, $arDados );
				
		$obQuestionarioresposta->salvar();
		$obQuestionarioresposta->commit();
		*/
		
		
		echo "<script>
				alert('Operação realizada com sucesso'); 
				window.close();
				window.opener.location.replace(window.opener.location);
			  </script>";
	}
	else{
		$obQuestionario->rollback();
		$obQuestionario->insucesso('Falha na operação');
	}
}
// Extrai dados para Atualização
$quetitulo 	  = $obQuestionario->quetitulo;	
$quedescricao = $obQuestionario->quedescricao;	
list($quedtinicio) = explode(' ', $obQuestionario->quedtinicio);	
list($quedtfim) = explode(' ', $obQuestionario->quedtfim);
$sisid		  = $obQuestionario->sisid ? $obQuestionario->sisid : $_GET['sisid'];

monta_titulo( 'Cadastro de Questionários', obrigatorio().' Indica Campo Obrigatório');
?>

<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/prototype.js"></script>
		<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
		<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
		<script type="text/javascript">
		function salvarQuestionario(){
			var camposObri 		= "#quetitulo#quedescricao#quedtinicio#quedtfim";
			var tiposCamposObri	= '#texto#textarea#data#data';
			
			if(!validaForm('formulario',camposObri,tiposCamposObri,false))
					return false;
			
			document.getElementById('submeter').value = 'salvar';
			document.getElementById('formulario').submit();
		}
		</script>
	</head>
<body topmargin="0" marginwidth="0">
<form action="" method="POST" id="formulario" name="formulario">
<input type="hidden" id="submeter" name="submeter" value="">
<input type="hidden" id="queid" name="queid" value="<?=$_REQUEST['queid']; ?>">
<input type="hidden" id="sisid" name="sisid" value="<?=$sisid; ?>">
<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulodireita" style="width: 20%;"><b>Título:</b></td>
		<td><?=campo_texto('quetitulo', 'S', 'S', 'Título do quetionário', 80, 100, '', '', '', '', 0, 'id="quetitulo"' ); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20px;"><b>Descrição:</b></td>
		<td><?=campo_textarea('quedescricao', 'S', 'S', 'Descrição do quetionário', 83, 10, 2000, '', '', '', '', 'Descrição do quetionário'); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20px;"><b>Período:</b></td>
		<td>De <?=campo_data2('quedtinicio', 'N','S','Data Inicial','S','',''); ?> a <?=campo_data2('quedtfim', 'N','S','Data Final','S','',''); ?> (dd/mm/yyyy) </td>
	</tr>
	<tr>
		<th colspan="2">
			<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="salvarQuestionario();">&nbsp;
			<input type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="window.close();">
		</th>
	</tr>
</table>
</form>
</body>
</html>