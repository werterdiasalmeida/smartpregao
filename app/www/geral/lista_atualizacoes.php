<?php 
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "www/obras/_funcoes.php";
$db = new cls_banco();

//ação excluir
if( isset($_GET['excluir']) ){
	
	// verificando se o perfil do usuário é PERFIL_SUPERUSUARIO
	if( possuiPerfil(PERFIL_SUPERUSUARIO) ){
		$sql = "UPDATE 
					public.atualizacao
				SET 
					atustatus = 'I'
				WHERE atuid = {$_GET['excluir']}";
		
		$db->executar($sql);
		$db->commit();
		
		echo "<script type='text/javascript'>
	 			alert('Atualização excluída com sucesso.');
	 			location.href = 'lista_atualizacoes.php';
	 	      </script>";
	}else{
		//se cair aqui significa que o usuário não pode excluir as atualizações
		echo "<script type='text/javascript'>
	 			alert('Você não possui permissão para executar esta ação.');
	 			location.href = 'lista_atualizacoes.php';
	 	      </script>";
	}
	
	exit();
}// fim do if excluir

if( isset($_POST) && count($_POST) > 0 ){

	if($_POST['requisicao'] == 'salvar'){
			
		$_POST['atudsccompleta'] 	 = substr($_POST['atudsccompleta'], 0, 3000);
		$_POST['atudscsimplificada'] = substr($_POST['atudscsimplificada'], 0, 150);
			
		$sql = "INSERT INTO
						public.atualizacao
						(
							atudscsimplificada, 
							atudsccompleta, 
							atusisid,
							atudtinclusao
						)
			    VALUES (
			    			'{$_POST['atudscsimplificada']}', 
			    			'{$_POST['atudsccompleta']}', 
			    			{$_SESSION['sisid']},
			    			'".formata_data_sql($_POST['atudtinclusao'])."'
			    		)"; 
			    				
			    			$db->executar($sql);
			    			$db->commit();

    	echo "<script type='text/javascript'>
	 			alert('Atualizações cadastradas com sucesso.');
	 			location.href = 'lista_atualizacoes.php';
		 	   </script>";
		exit();			
	}// fim do if salvar

	if($_POST['requisicao'] == 'pesquisar'){
			
		$condicao = "";
			
		if($_POST['atudscsimplificada'] != ''){
			$condicao .= " AND atudscsimplificada ILIKE '%{$_POST['atudscsimplificada']}%' 
						   OR atudsccompleta ILIKE '%{$_POST['atudscsimplificada']}%' ";
		}
			
		if( ($_POST['dataInicio'] != '') && ($_POST['dataFim'] != '') ){
			$condicao .= " AND atudtinclusao BETWEEN '".formata_data_sql($_POST['dataInicio'])."' AND '".formata_data_sql($_POST['dataFim'])."' ";
		}
			
	}// fim do if pesquisar

}

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Atualizações</title>

<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">

$(document).ready(function() {
	
	$("#novaAtualizacao").hide();
	
	$("#novo").click(function () {
		$("#resultadoAtualizacao").hide();
		$("#novaAtualizacao").show();
	})
	
	$("#voltar").click(function () {
		$("#resultadoAtualizacao").show();
		$("#novaAtualizacao").hide();
	})
	
	// validação realizada quando o usuário clicar no botão pesquisar
	$("#pesquisar").click(function () {
		
		if( ($("#dataInicio").val() != '') && ($("#dataFim").val() == '') ){
			alert('Preencha a Data Fim.');
			$("#dataFim").focus();
			return false;
			
		}else if( ($("#dataInicio").val() == '') && ($("#dataFim").val() != '') ){
			alert('Preencha a Data de Início.');
			$("#dataInicio").focus();
			return false;
		}
		return true;
	})
	
	// validação realizada quando o usuário clica no botão salvar
	$("#salvar").click(function () {
		
		if( $("#atudtinclusao").val() == '' ){
			alert('Preencha a Data de Inclusão corretamente.');
			$("#atudtinclusao").focus();
			return false;
			
		}else if( $("#atudscsimplificada").val() == '' ){
			alert('Preencha a Descrição Resumida corretamente.');
			$("#atudscsimplificada").focus();
			return false;
			
		}else if( $("#atudsccompleta").val() == '' ){
			alert('Preencha a Descrição Detalhada corretamente.');
			$("#atudsccompleta").focus();
			return false;
		}
		
		return true;
	})
	
	// botão excluir
	$("[id^='excluir_']").click(function () {
		<?php 
			 // verificando se o perfil do usuário é SUPER_USUARIO
			if( possuiPerfil(PERFIL_SUPERUSUARIO) ){
		?>
		if( confirm("Deseja realmente excluir esta Atualização?") ){
			// pegando o id da atualização
			var id = this.id.replace('excluir_','');
			location.href = 'lista_atualizacoes.php?excluir='+id;
		}else{
			return false;
		}
		return true;
		
		<?php }else{ ?>
		alert('Você não possui permissão para executar esta ação.');
		return false;
		<?php } ?>
	})
	
	// botão alterar/visualizar
	$("[id^='alterar_']").click(function () {
		// pegando o id da atualização
		var id = this.id.replace('alterar_','');
		location.href = 'lista_atualizacoes.php?atualizar='+id;
		return true;
	})
	
});

</script>

</head>
<body>

<?php
if( isset($_GET['atualizar']) ){
	
	if( (count($_POST) > 0) && (possuiPerfil(PERFIL_SUPERUSUARIO)) ){
		// atualizando
		$_POST['atudsccompleta'] 	 = substr($_POST['atudsccompleta'], 0, 3000);
		$_POST['atudscsimplificada'] = substr($_POST['atudscsimplificada'], 0, 150);
		
		$sql = "UPDATE
					public.atualizacao
			   	SET 
			   		atudscsimplificada = '{$_POST['atudscsimplificada']}', 
			   		atudsccompleta = '{$_POST['atudsccompleta']}', 
			       	atudtinclusao = '".formata_data_sql($_POST['atudtinclusao'])."' 
			 WHERE 
			   		atuid = {$_POST['atuid']}
			 AND atusisid = {$_SESSION['sisid']}";
		
		$db->executar($sql);
		$db->commit();
		
		echo "<script type='text/javascript'>
	 			alert('Atualizações alteradas com sucesso.');
	 			location.href = 'lista_atualizacoes.php';
		 	   </script>";
		exit();
		
	}else{
		// exibindo o form de alteração/visualização
		// verificando se o usuário possui o perfil necessário para alterar
		if( possuiPerfil(PERFIL_SUPERUSUARIO) ){
			$campo = "";
			$input = "S";
		}else{
			$campo = ' disabled="disabled"';
			$input = "N";
		}
		
		$sql = "SELECT 
					atudscsimplificada, 
					atudsccompleta, 
					atudtinclusao
			  	FROM 
			  		public.atualizacao
			  	WHERE
			  		atuid = {$_GET['atualizar']}
			  	AND atusisid = {$_SESSION['sisid']}";
		
		$dados = $db->pegaLinha($sql);
		
?>
	<script type="text/javascript">
		$(document).ready(function() {
			$("#novaAtualizacao").show();
		});
	</script>
	<!-- <div id="novaAtualizacao" style="border: red solid 1px;"> -->
	<div id="novaAtualizacao">
		<form name="frm" action="lista_atualizacoes.php?atualizar=<?php echo $_GET['atualizar']; ?>" method="post">
			<table width="95%" cellspacing="1" cellpadding="3" bgcolor="#f5f5f5"
				align="center" class="tabela">
				<tr>
					<td bgcolor="#DCDCDC" colspan="2" align="center" width="100%">
						<label class="TituloTela">Atualizações - <?php echo $_SESSION["sisdsc"]; ?></label>
					</td>
				</tr>
				<tr>
					<td align="center" class="subTituloCentro" colspan="2">Inserir Nova
					Atualização</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Data:</td>
					<td><?php //echo campo_data( 'atudtinclusao', 'N', $input, '', 'S', '', '', $dados['atudtinclusao'] );
							  echo campo_data2( 'atudtinclusao', 'S', $input, '', 'S', '', '', $dados['atudtinclusao'] ); 
						?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Descrição Resumida:</td>
					<td><input value="<?php echo $dados['atudscsimplificada']; ?>" type="text" name="atudscsimplificada" id="atudscsimplificada" maxlength="150"<?php echo $campo; ?>></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Descrição Detalhada:</td>
					<td><?php  echo campo_textarea( 'atudsccompleta', 'N', $input, '', '90', '10', '3000', '' , 0, '', false, NULL, $dados['atudsccompleta']); ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita"></td>
					<td><input type="submit" value="Salvar" id="salvar"<?php echo $campo; ?>>&nbsp;<input type="button" value="Voltar" onclick="location.href='lista_atualizacoes.php';"></td>
				</tr>
			</table>
			<input type="hidden" name="atuid" value="<?php echo $_GET['atualizar']; ?>">
		</form>
	</div>
		<!-- fim da div novaAtualizacao -->	
<?php		
	}
	exit();
}// fim do if atualizar
?>

<!-- <div id="resultadoAtualizacao" style="border: red solid 1px;"> -->
<div id="resultadoAtualizacao"><?php
// Verificando se o usuário está logado
if( isset($_SESSION['usucpf']) ){

	$sql = "SELECT
				'<center>
					<img style=\"cursor: pointer;\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar\" id=\"alterar_'|| atuid ||'\" title=\"alterar\" alt=\"alterar\">
					&nbsp;
					<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" id=\"excluir_'|| atuid ||'\" title=\"excluir\" alt=\"excluir\">
				</center>' as acao,
				
				'<center>' || atuid || '</center>' as sequencia,
			
				CASE WHEN atudtinclusao = '".date('Y-m-d')."' THEN
					'&nbsp;
					<font color=\"#dd0000\" title=\"Alterações Recentes\" alt=\"Alterações Recentes\">'|| to_char(atudtinclusao, 'DD/MM/YYYY') ||'</font>'
				ELSE
					CASE WHEN atudtinclusao BETWEEN CAST(now() AS date) - interval'7 days' AND now() THEN
						'&nbsp;
						<font color=\"#0066cc\" title=\"Alterações há 1 semana\" alt=\"Alterações há 1 semana\">'|| to_char(atudtinclusao, 'DD/MM/YYYY') ||'</font>'
					ELSE
						CASE WHEN atudtinclusao BETWEEN CAST(now() AS date) - interval'15 days' AND now() THEN
							'&nbsp;
							<font color=\"#006400\" title=\"Alterações há 15 dias\" alt=\"Alterações há 15 dias\">'|| to_char(atudtinclusao, 'DD/MM/YYYY') ||'</font>'
						ELSE
							'&nbsp;
							<font color=\"#000000\" title=\"Alterações há mais de 15 dias\" alt=\"Alterações há mais de 15 dias\">'|| to_char(atudtinclusao, 'DD/MM/YYYY') ||'</font>'
						END
					END
				END
				as datainclusao,
				atudscsimplificada as descricao
			FROM 
				public.atualizacao
			WHERE 
				atustatus = 'A'
			AND atusisid = {$_SESSION["sisid"]}
			{$condicao}
			ORDER BY atudtinclusao DESC;";
			?>
<form name="frm" action="" method="post">
<table width="95%" cellspacing="1" cellpadding="3" bgcolor="#f5f5f5"
	align="center" class="tabela">
	<tr>
		<td bgcolor="#DCDCDC" colspan="2" align="center" width="100%"><label
			class="TituloTela">Atualizações - <?php echo $_SESSION["sisdsc"]; ?></label></td>
	</tr>
	<tr>
		<td align="center" class="subTituloCentro" colspan="2">Argumentos de
		Pesquisa</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Texto:</td>
		<td><input type="text" name="atudscsimplificada"
			value="<?php echo (isset($_POST['atudscsimplificada']) ? $_POST['atudscsimplificada'] : ''); ?>"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Entre:</td>
		<td><?php //echo campo_data( 'dataInicio', 'N', 'S', '', 'S', '', '', (isset($_POST['dataInicio']) ? formata_data_sql($_POST['dataInicio']) : '') );
				  echo campo_data2( 'dataInicio', 'N', 'S', '', 'S', '', '', (isset($_POST['dataInicio']) ? formata_data_sql($_POST['dataInicio']) : '') );
			?>
		&nbsp;&nbsp;e&nbsp;&nbsp; <?php //echo campo_data( 'dataFim', 'N', 'S', '', 'S', '', '', (isset($_POST['dataFim']) ? formata_data_sql($_POST['dataFim']) : '') );
										echo campo_data2( 'dataFim', 'N', 'S', '', 'S', '', '', (isset($_POST['dataFim']) ? formata_data_sql($_POST['dataFim']) : '') );
								  ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"></td>
		<td><input type="submit" value="Pesquisar" id="pesquisar"></td>
	</tr>
</table>
<table width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td>
			<fieldset style="text-align: center;">
				<legend>LEGENDA</legend>
				<img src="../imagens/p_vermelho.gif" style="vertical-align: middle;"/> Alterações Recentes &nbsp;&nbsp;&nbsp;
				<img src="../imagens/p_azul.gif" style="vertical-align: middle;"/> Alterações há 1 semana &nbsp;&nbsp;&nbsp;
				<img src="../imagens/p_verde.gif" style="vertical-align: middle;"/> Alterações há 15 dias &nbsp;&nbsp;&nbsp;
				<img src="../imagens/p_preto.GIF" style="vertical-align: middle;"/> Alterações há mais de 15 dias &nbsp;&nbsp;&nbsp;
			</fieldset>
		</td>
	</tr>
</table>
<input type="hidden" name="requisicao" value="pesquisar"></form>
			<?php
			$cabecalho = array( "Ação", "Sequência", "Data", "Descrição" );
			$db->monta_lista($sql, $cabecalho, 50, 20, '', 'center', '');
			?>
<table align="center" width="95%">
	<tr bgcolor="#DCDCDC">
		<td><input type="button" style="cursor: pointer;" id="novo"
			value="Novo"></td>
	</tr>
</table>
</div>
<!-- fim da div resultadoAtualizacao -->

<!-- <div id="novaAtualizacao" style="border: red solid 1px;"> -->
<div id="novaAtualizacao">
<form name="frmNovo" action="" method="post">
<table width="95%" cellspacing="1" cellpadding="3" bgcolor="#f5f5f5"
	align="center" class="tabela">
	<tr>
		<td bgcolor="#DCDCDC" colspan="2" align="center" width="100%"><label
			class="TituloTela">Atualizações - <?php echo $_SESSION["sisdsc"]; ?></label></td>
	</tr>
	<tr>
		<td align="center" class="subTituloCentro" colspan="2">Inserir Nova
		Atualização</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data:</td>
		<td><?php //echo campo_data( 'atudtinclusao', 'N', 'S', '', 'S', '', '', date('Y-m-d') );
				  echo campo_data2( 'atudtinclusao', 'S', 'S', '', 'S', '', '', date('Y-m-d') ); 
			?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição Resumida:</td>
		<td><input type="text" name="atudscsimplificada"
			id="atudscsimplificada" maxlength="150"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição Detalhada:</td>
		<td><?php  echo campo_textarea( 'atudsccompleta', 'N', 'S', '', '90', '10', '3000', '' , 0, '', false, NULL, @$dados[0]['chkobscompigreja']); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita"></td>
		<td><input type="submit" value="Salvar" id="salvar">&nbsp;<input
			type="button" value="Voltar" id="voltar"></td>
	</tr>
</table>
<input type="hidden" name="requisicao" value="salvar"></form>
</div>
<!-- fim da div novaAtualizacao -->

<?php
}else{
	exit();
}

?>
</body>
</html>
