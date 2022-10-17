<?
 /* 
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Fernando Bagno <fernandobagno@gmail.com>
   Módulo:grupo_perfil.inc
   Finalidade: permitir o controle de cadastro dos perfis
   */

if($_REQUEST["sisid"]) $_SESSION['sisidold'] = $_REQUEST["sisid"];

switch( $_REQUEST["requisicao"] ){
	
	case "lista":
		
		if ( $_REQUEST["sisid"] ){
			$sql = "SELECT
						'<center>
							<img src=\"/imagens/alterar.gif\" onclick=\"alterarGrupo( ' || modid || ' );\" style=\"cursor:pointer;\"/>
							<img src=\"/imagens/excluir.gif\" onclick=\"excluirGrupo( ' || modid || ' );\" style=\"cursor:pointer;\"/>
						</center>',
						modtitulo,
						moddsc
					FROM
						seguranca.modulo
					WHERE
						sisid = {$_REQUEST["sisid"]} AND modstatus = 'A'
					ORDER BY
						2";
			
			$cabecalho = array( "Ação", "Título", "Descrição");
			$db->monta_lista( $sql, $cabecalho, 20, 4, 'N','center', '', '', '', '' );
		
		}
			
		die;
		
	break;
	
	case "busca":
		
		$sql = "SELECT
					modid,
					modtitulo,
					moddsc
				FROM
					seguranca.modulo
				WHERE
					modid = {$_REQUEST["modid"]}";
		
		$dados = $db->pegaLinha( $sql );
		$dados["modtitulo"] = iconv("ISO-8859-1", "UTF-8", $dados["modtitulo"]);
		$dados["moddsc"] = iconv("ISO-8859-1", "UTF-8", $dados["moddsc"]);
		
		echo json_encode($dados);
		die;
		
	break;
	
	case "inserir":
		
		$existePerfil = $db->pegaUm( "SELECT modtitulo FROM seguranca.modulo 
										WHERE modtitulo = '{$_REQUEST["modtitulo"]}' AND sisid = {$_REQUEST["sisid"]} AND modstatus = 'A'" );

		if ( $existePerfil ){
			
			print "<script>
					   alert('Este Grupo já está cadastrado!');
					   history.back(-1);
				   </script>";
			
		}else{
			
			$db->executar( "INSERT INTO seguranca.modulo( sisid, 
														  modtitulo, 
														  moddsc,
														  modstatus )
												 VALUES( {$_REQUEST["sisid"]},
												 		 '{$_REQUEST["modtitulo"]}',
												 		 '{$_REQUEST["moddsc"]}',
												 		 'A' )" );
			
			$db->commit();
			$db->sucesso( "sistema/grupo_perfil" );
			
		}
		
	break;
	
	case "alterar":
		
		$db->executar( "UPDATE 
							seguranca.modulo 
						SET
							modtitulo 			= '{$_REQUEST["modtitulo"]}',
							moddsc   			= '{$_REQUEST["moddsc"]}'
						WHERE
							modid = {$_REQUEST["modid"]}" );
		
		$db->commit();
		$db->sucesso( "sistema/grupo_perfil" );
		
	break;
	
	case "excluir":
		
		$db->executar( "UPDATE seguranca.modulo SET modstatus = 'I' WHERE modid = {$_REQUEST["modid"]}" );
		$db->commit();
		$db->sucesso( "sistema/grupo_perfil" );
		
	break;
	
}

$caminhoAtual = $_SERVER["REQUEST_URI"];
$posicaoCaminho = strpos($caminhoAtual, 'acao');
$caminhoAtual = substr($caminhoAtual, 0 , $posicaoCaminho);

// Pega url para os js
$posicaoCaminhoJs = strpos($caminhoAtual, '?');
$caminhoAtualJs = substr($caminhoAtual, 0 , $posicaoCaminhoJs);

include APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Incluir novo módulo" , '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript"><!--
	
	var caminhoAtualJs = '<?php echo $caminhoAtualJs; ?>';
	
	function listaGrupos( id ){
	
		var url = caminhoAtualJs + '?modulo=sistema/grupo_perfil&acao=I&requisicao=lista&sisid=' + id;
			
		var myAjax = new Ajax.Updater(
			"listaGrupos",
			url,
			{
				method: 'get',
				asynchronous: false
			});
	}
	
	function validaInclusao(){
		
		var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
		var validacao = true;

		var formulario = document.getElementById('formulario');
		
		var sisid = document.formulario.sisid.value;
		if (sisid == ''){
			mensagem += 'Sistema \n';
			validacao = false;
		}
		
		var modtitulo = document.formulario.modtitulo.value;
		if (modtitulo == ''){
			mensagem += 'Título \n';
			validacao = false;
		}
		
		var moddsc = document.formulario.moddsc.value;
		if (moddsc == ''){
			mensagem += 'Descrição \n';
			validacao = false;
		}

		
		if ( !validacao ){
			alert( mensagem );
			return false;
		}else{
			formulario.submit();
		}
			
	}
	
	function alterarGrupo( modid ){
	
		var url = caminhoAtualJs + '?modulo=sistema/grupo_perfil&acao=I&requisicao=busca';
		var parametros = "&modid=" + modid;
	
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: false,
				onComplete: function(resp) {
					
					var json = resp.responseText.evalJSON();
					
					$('modid').value = json.modid;
					$('modtitulo').value = json.modtitulo;
					$('moddsc').value = json.moddsc;

				}
				
			}
		);
	
		$('requisicao').value = 'alterar';
		$('btnacao').value = 'Alterar';
		
		
	
	}
	
	function excluirGrupo( modid ){
		if( confirm("Deseja realmente excluir este grupo?") ){
			window.location.href = caminhoAtualJs + '?modulo=sistema/grupo_perfil&acao=I&requisicao=excluir&modid=' + modid;
		}
	}
	
--></script>
<form id="formulario" name="formulario" action="" method="post">
	<input type="hidden" id="requisicao" name="requisicao" value="inserir"/>
	<input type="hidden" id="modid" name="modid" value=""/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">Sistema:</td>
			<td>
				<?php
					if($_SESSION['sisidold']){
						$sisid = $_SESSION['sisidold'];
					}
					else{
						$sisid = $_SESSION['sisid'];	
					}
					
					$sql = "SELECT DISTINCT 
								sisid AS codigo, 
								sisdsc AS descricao 
							FROM 
								sistema 
							ORDER BY 
								descricao";
	 				
					$habilSistema = $_SESSION['sisid'] == 4 ? "S" : "N";
					
					$db->monta_combo( "sisid", $sql, $habilSistema, "Selecione o Sistema", "listaGrupos", "", "", "", "S", "sisid" );
					
				?>
					   
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Título:</td>
			<td>
				<?php echo campo_texto( 'modtitulo', 'S', 'S', '', 45, 45, '', '', 'left', '', 0, "id='modtitulo'"); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Descrição:</td>
			<td>
				<?php echo campo_textarea('moddsc','S','S','',96,5,''); ?>
				
			</td>
		</tr>
		<tr bgcolor="#CCCCCC">
			<td></td>
			<td>
				<input type="button" name="btnacao" id="btnacao" value="Incluir" onclick="validaInclusao();" class="botao">
			</td>
		</tr>
		<tr>
			<td colspan="2" class="subTituloCentro"> Lista de Módulos </td>
		</tr>
	</table>
</form>
<div id="listaGrupos">
	<?php 
	
		$sql = "SELECT
					'<center>
						<img src=\"/imagens/alterar.gif\" onclick=\"alterarGrupo( ' || modid || ' );\" style=\"cursor:pointer;\"/>
						<img src=\"/imagens/excluir.gif\" onclick=\"excluirGrupo( ' || modid || ' );\" style=\"cursor:pointer;\"/>
					</center>',
					modtitulo,
					moddsc
				FROM
					seguranca.modulo
				WHERE
					sisid = {$sisid} AND modstatus = 'A'
				ORDER BY
					2";
		
		$cabecalho = array( "Ação", "Título", "Descrição");
		$db->monta_lista( $sql, $cabecalho, 20, 4, 'N','center', '', '', '', '' );
		
	?>
</div>

