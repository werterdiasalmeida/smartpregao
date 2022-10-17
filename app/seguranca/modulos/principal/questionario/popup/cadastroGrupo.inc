<?php 
include_once APPRAIZ.'includes/classes/modelo/questionario/QGrupo.class.inc';

$obGrupo = new QGrupo( $_REQUEST['grpid'] );

if($_POST['submeter'] == 'salvar'){

	if($_POST['queid'] <> ''){
		$arCampos = array( "grpid", "queid", "grptitulo", "grpstatus", "grpordem", "grpajuda" );
		$where = "queid = {$_POST['queid']}";	
	} else if($_POST['gru_grpid'] <> ''){
		$arCampos = array( "grpid", "gru_grpid", "grptitulo", "grpstatus", "grpordem", "grpajuda" );
		$where = "gru_grpid = {$_POST['gru_grpid']}";	
	} else if($_POST['itpid'] <> ''){
		$arCampos = array( "grpid", "itpid", "grptitulo", "grpstatus", "grpordem", "grpajuda" );
		$where = "itpid = {$_POST['itpid']}";
	} 
	
	$obGrupo->popularObjeto( $arCampos );
	
	$grpid = $obGrupo->salvar();
	if ( $obGrupo->grpid )
		$grpid = $obGrupo->grpid;

	$obGrupo->commit();
	/*	echo "<script>
				alert('Operação realizada com sucesso'); 
				window.close();
				window.opener.location.replace(window.opener.location);
			  </script>";
	
	}else{
		$obGrupo->rollback();
		$obGrupo->insucesso('Falha na operação');
	}
	*/
	if($_POST['grpordem'] > $_POST['ordemi']){
		$sit = '>';
	} else {
		$sit = '>=';
	}
	$sql = "SELECT grpid FROM questionario.grupopergunta WHERE $where AND grpordem $sit {$_POST['grpordem']} AND grpid != {$grpid} ORDER BY grpordem";
	$arrGrpid = (array) $db->carregarColuna($sql);
	
	if($arrGrpid){
		foreach($arrGrpid as $grpidA){
			$i++;
			$ordemA = $_POST['grpordem'] + $i;
			$sql = "UPDATE questionario.grupopergunta SET grpordem = $ordemA  WHERE grpid = $grpidA";
			$db->executar($sql);
		}
		$db->commit();
	}
	
	$sql = "SELECT grpid FROM questionario.grupopergunta WHERE $where ORDER BY grpordem";
	$arrGrpid = (array) $db->carregarColuna($sql);
//	dbg($arrGrpid, d);

	if($arrGrpid){
		$i = 0;
		foreach($arrGrpid as $grpidA){
			$i++;
			$ordemA = $i;
			$sql = "UPDATE questionario.grupopergunta SET grpordem = $ordemA  WHERE grpid = $grpidA";
			$db->executar($sql);
		}
	}
	$obGrupo->commit();
	die ("<script>
			alert('Operação realizada com sucesso'); 
			window.close();
			window.opener.location.replace(window.opener.location);
		  </script>");
	
}
// Extrai dados para Atualização
$grptitulo = $obGrupo->grptitulo;		
$grpajuda  = $obGrupo->grpajuda;		
$grpstatus = $obGrupo->grpstatus;
$grpordem  = $obGrupo->grpordem;
$queid 	   = $obGrupo->queid ? $obGrupo->queid : $_GET['queid'];
$gru_grpid = $obGrupo->gru_grpid ? $obGrupo->gru_grpid : $_GET['gru_grpid'];
$grpid 	   = $obGrupo->grpid ? $obGrupo->grpid : $_GET['grpid'];
$itpid 	   = $obGrupo->itpid ? $obGrupo->itpid : $_GET['itpid'];
$ordemi    = $obGrupo->grpordem;

monta_titulo( 'Cadastro de Grupos', obrigatorio().' Indica Campo Obrigatório');
$ordem = $grpordem;
if(!$grpordem){
	if($_REQUEST['queid']){
		$sql = "SELECT MAX(grpordem) FROM questionario.grupopergunta WHERE queid = $queid";
		$ordem = ($db->pegaUm($sql))+1;
	} else if ($_REQUEST['itpid']){
		$sql = "SELECT MAX(grpordem) FROM questionario.grupopergunta WHERE itpid = $itpid";
		$ordem = ($db->pegaUm($sql))+1;
	}else if ($_REQUEST['gru_grpid']){
		$sql = "SELECT MAX(grpordem) FROM questionario.grupopergunta WHERE gru_grpid = $gru_grpid";
		$ordem = ($db->pegaUm($sql))+1;
	}
}
?>

<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script type="text/javascript">
		function salvarGrupos(){
			var camposObri 		= "#grptitulo#grpordem#grpstatus";
			var tiposCamposObri	= '#texto#texto#radio';
			
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
<input type="hidden" id="grpid" name="grpid" value="<?=$grpid; ?>">
<input type="hidden" id="queid" name="queid" value="<?=$queid; ?>">
<input type="hidden" id="gru_grpid" name="gru_grpid" value="<?=$gru_grpid; ?>">
<input type="hidden" id="itpid" name="itpid" value="<?=$itpid; ?>">
<input type="hidden" id="ordemi" name="ordemi" value="<?=$ordemi; ?>">
<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulodireita" style="width: 20%;"><b>Descrição:</b></td>
		<td><?=campo_texto('grptitulo', 'S', 'S', 'Título do Grupo', 50, 450, '', '', '', '', 0, 'id="grptitulo"' ); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20px;"><b>Texto de ajuda:</b></td>
		<td><?=campo_textarea('grpajuda', 'N', 'S', 'Texto de ajuda', 73, 5, 2000, '', '', '', '', 'Texto de ajuda'); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" style="width: 20%;"><b>Ordem:</b></td>
		<td><?=campo_texto('grpordem', 'S', 'S', 'Ordem de Visualização', 10, 2, '[#]', '', '', '', 0, 'id="grpordem"', '', $ordem ); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20px;"><b>Status:</b></td>
		<td><input type="radio" name="grpstatus" id="grpstatus" value="A" <?=($grpstatus == 'A' || $grpstatus == '' ? "checked='checked'" : '') ?> > Ativo &nbsp;
			<input type="radio" name="grpstatus" id="grpstatus" value="I" <?=($grpstatus == 'I' ? "checked='checked'" : '') ?> > Inativo</td>
	</tr>
	<tr>
		<th colspan="2">
			<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="salvarGrupos();">&nbsp;
			<input type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="window.close();">
		</th>
	</tr>
</table>
</form>
</body>
</html>