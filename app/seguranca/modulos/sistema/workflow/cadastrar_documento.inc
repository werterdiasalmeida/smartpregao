<?

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

$db->cria_aba($abacod_tela,$url,$parametros);
?>
<br>
<?
$titulo_modulo='Cadastro de Documentos';
monta_titulo($titulo_modulo,'<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

// variáveis que alimentam o formulário
$sisid = (integer) $_REQUEST['sisid'];
$tpdid   = (integer) $_REQUEST['tpdid'];
$tpddsc = $_REQUEST['tpddsc'];
$evento = $_REQUEST['evento'];

if($evento != ""){
	
	switch ( $evento ) {
		
		case 'adicionar_documento':
			
			//rotina que inclui um novo tipo de documento
			if(isset($_REQUEST['tpddsc']) && isset($_REQUEST['sisid']))
			{				
				cd_inserirTipoDoc($tpddsc, $sisid);

				$_REQUEST['evento'] = "";
				$tpddsc = "";
				
				$db->sucesso("sistema/workflow/cadastrar_documento", '');
			}

			break;

		//rotina que altera um tipo de documento
		case 'alterar_documento':
			
		if(isset($_REQUEST['tpddsc']) && isset($_REQUEST['tpdid']))
		{
			cd_alterarTipoDoc($tpddsc,$tpdid);

			$_REQUEST['evento'] = "";
			$tpddsc = "";
		}
			
		break;

		//rotina que exclui um tipo de documento
		case 'excluir_documento':
				
			if(isset($_REQUEST['tpdid']))
			{
				
				$sql_doc="SELECT
					workflow.tipodocumento.tpdid
					FROM
					workflow.tipodocumento
					Inner Join workflow.estadodocumento using (tpdid)
					WHERE
					workflow.tipodocumento.tpdid =  workflow.estadodocumento.tpdid AND
					workflow.tipodocumento.tpdid =  '".$_REQUEST['tpdid']."'
				
				";
				global $db;
				$resultado = $db->pegaUm($sql_doc);
				//caso exista algum estado associado ao tipo do documento a exclusão do segundo é impedida
				if($resultado){
					?>
					<script>alert('Exite um estado associado a este tipo. Para remover o tipo é necessário remover todos os estados primeiramente');
					history.back();
					</script>
					<?
					return;
				}				
				cd_excluirTipoDoc($_REQUEST['tpdid']);

				$_REQUEST['evento'] = "";
				$tpddsc = "";
			}
				
			break;

		default:
			break;
	}
}

?>

<form method="POST" name="formulario">

<input type='hidden' name="evento" id="evento" value="<?=$_REQUEST['evento']?>">
<input type='hidden' name="tpdid" id="tpdid" value="<?=$_REQUEST['tpdid'];?>">
<input type='hidden' name="tpddsc" id="tpddsc" value="<?=$_REQUEST['tpddsc'];?>">


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td align='right' class="SubTituloDireita">Sistema:</td>
		<td><?php
		//montado a combo do sistema
		$sql_sistema = "
							select sisid as codigo, sisdsc as descricao
							from seguranca.sistema
							where sisstatus = 'A'
						";
		
		$db->monta_combo("sisid", $sql_sistema,	"S", "Selecione",	"sisSelecionado", null,	null, null, null, "sisid");
		?></td>
	</tr>

	<tr>
		<td align='right' class="SubTituloDireita">Tipo do Documento:</td>
		
		<?		
		if (!$sisid){
		?>
		
		<td><input type="text" name="tpddsc" id="tipodocumento_dsc"
			disabled="disabled" value="<?=$tpddsc ?>" size="60"
			class="CampoEstilo" style="float: left;"  title="Tipo do documento. Para preencher é necessário escolher um sistema"/> <img src="../imagens/obrig.gif" border="0"><?				
	}else{
		?>		
		<td><input type="text" name="tpddsc" id="tipodocumento_dsc"
			value="<?=$tpddsc ?>" size="60" class="CampoEstilo"
			style="float: left;" title="Tipo do documento." /><img src="../imagens/obrig.gif" border="0"><?		
	}

	?>	
	</tr>
	<?

if ($evento != "preparar_documento" ){
	echo("<tr bgcolor='#cccccc'>
				<td></td>
				<td>
				<input type='button' class='botao' value='Incluir' id='btnovotipodoc' name='btnovotipodoc' onclick='adicionarTipoDocumento()' />
				</td>
		  </tr>");
}else{
	
	echo("<tr bgcolor='#cccccc'>
		<td></td>
		<td><input type='button' class='botao' value='Alterar' id='btalterartipodoc' name='btnovotipodoc' onclick=\"alterarTipoDocumento( ".$tpdid.", '". str_replace( "'", "\\'", $tpddsc ) ."')\" />
			<input type='button' class='botao' value='Cancelar' id='btcancelar' name='btcancelar' onclick='history.back();' /></td>
	</tr>");
}

?>	

</table>

</form>
<br>
<?php
//monta a tabela com os tipos de documentos cadastrados
if ($sisid) $wh = " sistema.sisid=".$sisid." ";
if ($wh) $wh = " where ".$wh;
$sql = "
				SELECT
				'<img
				border=\"0\"
				src=\"../imagens/alterar.gif\"
				title=\"Alterar Documento\"
				onclick=\"prepararAlterar( ' || tipodocumento.tpdid || ',' || tipodocumento.tpddsc || ',' || sistema.sisid || ' )\">
			&nbsp;&nbsp;&nbsp;
			<img
				border=\"0\"
				src=\"../imagens/excluir.gif\"
				title=\"Excluir Documento\"
				onclick=\"excluirTipoDocumento( ' || tipodocumento.tpdid || ',' || sistema.sisid || ' )\">' as acao,
				
				tipodocumento.tpddsc,
				sistema.sisdsc
				FROM 
				workflow.tipodocumento
				inner join seguranca.sistema using (sisid)
				". $wh;

$cabecalho = array( 'Ações', 'Tipo Documento','Sistema');
$db->monta_lista( $sql, $cabecalho, 200, 20, '', '', '' );
?>

<script><!--

			
	function excluirTipoDocumento(tpdid, sisid){
	
		document.getElementById("evento").value = "excluir_documento";
		document.getElementById("tpdid").value = tpdid;
		document.getElementById("sisid").value = sisid;		
		
		document.formulario.submit();		
	}	
	
	function prepararAlterar(tpdid, tpddsc, sisid){
		
	    document.getElementById("evento").value = "preparar_documento";
		document.getElementById("tpddsc").value = tpddsc;
		document.getElementById("tpdid").value = tpdid;
		document.getElementById("tipodocumento_dsc").value = tpddsc;
		document.getElementById("sisid").value = sisid;	
		document.formulario.submit();						
	}	
	
	function alterarTipoDocumento(tpdid, tpddsc){	
	
		if ( !document.formulario.sisid )
			{
				alert( 'Escolha um sistema para poder manipular os tipos de documento.' );
				document.formulario.sisid.focus();
				return;
			}
			if ( document.getElementById("tipodocumento_dsc").value == "")
			{
				alert( 'Entre um nome para o novo tipo de documento.' );
				document.formulario.tipodocumento_dsc.focus();
				return;
			}
		
		document.getElementById("evento").value = "alterar_documento";
		
		document.getElementById("tpddsc").value = tpddsc;
		document.getElementById("tpdid").value = tpdid;
		document.formulario.submit();	
			
	}		
		
	function adicionarTipoDocumento()	
	{				
			if ( !document.formulario.sisid )
			{
				alert( 'Escolha um sistema para poder manipular os tipos de documento.' );
				document.formulario.sisid.focus();
				return;
			}
			if ( document.getElementById("tipodocumento_dsc").value == "")
			{
				alert( 'Entre um nome para o novo tipo de documento.' );
				document.formulario.tipodocumento_dsc.focus();
				return;
			}		
			
			document.getElementById("evento").value = "adicionar_documento";			
			document.formulario.submit();
			
	}
	function sisSelecionado()
	{
		document.getElementById("evento").value= "pre_alterar_estado";	
		document.formulario.submit();		
	}

--></script>

<?

function cd_excluirTipoDoc($tpdid){

	$sql_excluir = "DELETE FROM workflow.tipodocumento
					WHERE tpdid='".$tpdid."'  
	";
	global $db;

	$db->executar($sql_excluir);
	$db->commit();
}

function cd_inserirTipoDoc($tpddsc, $sisid){

	$sql_inserir = "
		INSERT INTO workflow.tipodocumento 
		(tpddsc, sisid)
		VALUES ('".$tpddsc."', '".$sisid."') 
		 
	";

	//return (integer) $db->pegaUm( $sql_inserir );
	global $db;
	$db->executar($sql_inserir);
	$db->commit();

}

function cd_alterarTipoDoc($tpddsc, $tpdid){

	$sql_alterar = "UPDATE workflow.tipodocumento
	                SET tpddsc= '".$tpddsc."' 
	                WHERE tpdid ='".$tpdid."'  
	";
	global $db;
	$db->executar($sql_alterar);
	$db->commit();
}

?>
