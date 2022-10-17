<?php
include APPRAIZ . "includes/cabecalho.inc";


$operacao = $_REQUEST['operacao'];
$sql_alerta = "SELECT aledtincalter, aledsc, aledtinic, aledtfim, usucpf
				FROM seguranca.alerta
				WHERE sisid = ".$_SESSION['sisid']."";
$dados = $db->recuperar($sql_alerta);
//caso já exista algum alerta para o usuário nesse sistema
if($dados){
	$sql_nome = "SELECT u.usunome
						FROM seguranca.usuario u 						
						WHERE u.usucpf = '".$dados['usucpf']."' ";
	
	$nome= $db->pegaUm($sql_nome);
//cc	
}else{
	$sql_nome = "SELECT u.usunome
						FROM seguranca.usuario u 						
						WHERE u.usucpf = '".$_SESSION['usucpf']."' ";
	dump($sql_nome);
	$nome= $db->pegaUm($sql_nome);
}



if ($operacao == 'incluir') {	
	//caso já exista algum alerta para o usuário nesse sistema
	$dados = $db->pegaUm($sql_alerta);
	if($dados){
		$sql = sprintf(
					"UPDATE seguranca.alerta SET
						aledtincalter = '%s',
						aledsc = '%s',
						aledtinic = '%s',
						aledtfim = '%s',
						usucpf = '%s'				
					WHERE sisid = '%s'",
		formata_data_sql($_REQUEST['aledtincalter']),
		$_REQUEST['aledsc'],
		formata_data_sql($_REQUEST['aledtinic']),
		formata_data_sql($_REQUEST['aledtfim']),
		$_SESSION['usucpf'],
		$_SESSION['sisid']
		);	
		$db->executar( $sql );
		$db->commit();
		//caso contrário
	}else{
		$sql = sprintf(
				"INSERT INTO seguranca.alerta ( aledtincalter, aledsc, aledtinic, aledtfim, usucpf, sisid ) 
				VALUES ( '%s', '%s', '%s', '%s', '%s', '%s' )",
		formata_data_sql($_REQUEST['aledtincalter']),
		$_REQUEST['aledsc'],
		formata_data_sql($_REQUEST['aledtinic']),
		formata_data_sql($_REQUEST['aledtfim']),
		$_SESSION['usucpf'],
		$_SESSION['sisid']
		);
		$db->executar( $sql );
		$db->commit();
	}
}

$dados = $db->recuperar($sql_alerta);
//$aledtincalter = $dados['aledtincalter'];
$aledtincalter = date("Y-m-d");
$aledsc = $dados['aledsc'];
$aledtinic = $dados['aledtinic'];
$aledtfim = $dados['aledtfim'];

?>
<br />
<?php
$titulo_modulo = 'Cadastro de Mensagens de Alerta';

monta_titulo( $titulo_modulo, '' );

?>
<script
	language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">
//Editor de textos
tinyMCE.init({
	mode: "specific_textareas",
	editor_selector : "text_editor_simple",
	theme : "advanced",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
	theme_advanced_buttons1 : "cut,copy,paste,pastetext,pasteword,separator,search,replace,separator,tablecontrols, separator, preview",
	theme_advanced_buttons2 : "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright,justifyfull,separator,outdent,indent,bullist,numlist,separator,forecolor,fontselect,fontsizeselect ",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	valid_elements : "" +
	"-strong/-b[class|style]," +
	"-ol[class|style]," +
	"-ul[class|style]," +
	"-li[class|style]," +
	"#p[id|style|dir|class|align]," +
	"br," +
	"-blockquote[dir|style]," +
	"-table[border=0|cellspacing|cellpadding|width|height|class|align|summary|style|dir|id|lang|bgcolor|background|bordercolor]," +
	"-tr[id|lang|dir|class|rowspan|width|height|align|valign|style|bgcolor|background|bordercolor]," +
	"tbody[id|class]," +
	"thead[id|class]," +
	"tfoot[id|class]," +
	"-td[id|lang|dir|class|colspan|rowspan|width|height|align|valign|style|bgcolor|background|bordercolor|scope]," +
	"-th[id|lang|dir|class|colspan|rowspan|width|height|align|valign|style|scope]" ,
	language : "pt_br",
	entity_encoding : "raw"
});
</script>
	
<form method="POST" name="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<input type="hidden" name="operacao" id="operacao" value="" />
	<tr>
		<td align='right' class="SubTituloDireita">Usuário:</td>
		<td><?=campo_texto( 'nome', 'N', 'N', '', 50, 50, '', '' ); 
		?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Data de inclusão/alteração:</td>
		<td><?= campo_data( 'aledtincalter', 'N', 'N', 'S', 'S'); ?></td>
	</tr>

	<tr>
		  <td align='right' class="SubTituloDireita">Descrição da Mensagem:</td>
		
		<!--
		<td>
		<div id="justificativa_on"><?= campo_textarea( 'aledsc', 'S', 'S', '', 60, 3, '' ); ?>
		</div>		
		</td>-->
		
		<td  align='left'>
		<textarea style="width: 98%" id="aledsc" name="aledsc" cols="35" rows="15"
					class='text_editor_simple'><?php echo $aledsc?>
		</textarea>		
		</td>
		
	</tr>

	<tr>
		<td align='right' class="SubTituloDireita">Data Inicial:</td>
		<td><?= campo_data( 'aledtinic', 'S', 'S', 'S', 'S'); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Data Final:</td>
		<td><?= campo_data( 'aledtfim', 'S', 'S', 'S', 'S'); ?></td>
	</tr>


	<tr bgcolor="#C0C0C0">
		<td></td>
		<td><input type="button" class="botao" name="salvar" value="Salvar"
			onclick="javascript:gravar()" /> <input type="button" class="botao"
			name="cancelar" value="Cancelar" onclick="javascript:history.back()" />
		</td>
	</tr>
</table>
</form>
		<?
		//include 'alertarUsuario.inc';
		?>

<script type="text/javascript">

	function salvar()
	{		
		if ( validar()) {
		 	document.formulario.operacao.value = 'incluir';
			document.formulario.submit();
		}
		
	}
	
	function gravar(){	
	
		if ( validar()) {
		 	document.formulario.operacao.value = 'incluir';
			document.formulario.submit();
			alert('Alerta cadastrado com sucesso.');
		}
		
	}
	

	function validar()
	{
		var validacao = true;
		var mensagem  = 'Os seguintes campos não foram preenchidos corretamente:';		
		var aledtincalter  = document.formulario.aledtincalter;
		var aledtinic  = document.formulario.aledtinic;
		var aledtfim  = document.formulario.aledtfim;

		if ( ! validaData( aledtincalter ) ) {
			mensagem += '\nData de inclusão/alteração';
			validacao = false;
		}

		if ( document.formulario.aledsc.value == '' ) {
			mensagem += '\nDescrição da Mensagem';
			validacao = false;
		}

		if ( ! validaData( aledtinic ) ) {
			mensagem += '\nData Inicial';
			validacao = false;
		}

		if ( ! validaData( aledtfim ) ) {
			mensagem += '\nData Final';
			validacao = false;
		}		

		if ( !validacao ) {
			alert( mensagem );
		}
		
		if( ! validaDataMaior(document.formulario.aledtincalter, document.formulario.aledtinic)){
			alert('A data inicial deve ser maior que a data atual.');
			validacao = false;
		}else if( ! validaDataMaior(document.formulario.aledtinic, document.formulario.aledtfim)){
			alert('A data inicial deve ser menor que a data final.');
			validacao = false;
		}
		
		return validacao;
	}
	
</script>
