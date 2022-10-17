<?php

/**
 *
 */

include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
monta_titulo( 'Estado do Documento', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );

$tpdid = (integer) $_REQUEST['tpdid'];
$esdid = (integer) $_REQUEST['esdid_atual'];
//$esdstatus =  $_REQUEST['esdstatus'];
$esdordem =  $_REQUEST['esdordem_atual'];
$esddsc =  $_REQUEST['esddsc'];
$evento = $_REQUEST['evento'];


if($evento != ""){	
	
	switch ( $evento ) {
		
		case 'adicionar_estado':
						
			//rotina que inclui um novo estado
			if(isset($_REQUEST['esddsc']))
			{	
				
			ce_adicionarEstado($esddsc, $tpdid, $_REQUEST['count']);

			//alterando o header para evitar reprocessameto do enviar() - que insere novos estados 
			$db->sucesso("sistema/workflow/cadastrar_estado", '&acao=A&tpdid='.$tpdid);						
			}

		break;
		
		case 'alterar_estado':						
			//rotina que altera um novo estado
			if(isset($_REQUEST['esddsc']))
			{				
				
			cd_alteraEstado($esddsc, $esdid);

			//alterando o header para evitar reprocessameto do enviar() - que insere novos estados 
			$db->sucesso("sistema/workflow/cadastrar_estado", '&acao=I&tpdid='.$tpdid);						
			}

		break;
		
		case 'remover_estado':
			
			$spq_acao = "			
				SELECT *
				FROM workflow.acaoestadodoc 
				WHERE esdidorigem = ".$esdid."
				OR esdiddestino = ".$esdid."
			";
			
			global $db;
			$resultado = $db->pegaUm($spq_acao);
			//caso exista alguma ação associado ao estado do documento a exclusão do segundo é impedida			
			
			if($resultado){
					?>
					<script>alert('Exite uma ação associada a este estado. Para remover o estado é necessário remover todos as ações primeiramente');
					history.back();
					</script>
					<?
					return;
				}			
			
				ce_excluirEstado($esdid);
				
				atualizarLista($esdordem, $tpdid);
				
		break;
		
		case 'aumentar_ordem':
			ce_aumentarOrdem($esdid, $_REQUEST['esdordem_atual'], $_REQUEST['esdid_anterior'], $_REQUEST['ordem_anterior']);
		break;
		
		case 'diminuir_ordem':			
			ce_diminuirOrdem($esdid, $_REQUEST['esdordem_atual'], $_REQUEST['esdid_posterior'], $_REQUEST['ordem_posterior']);
			
		break;		
			
		default:
		break;
	}
}

$sql_estadodoc = "SELECT
					estadodocumento.esdid,
					estadodocumento.tpdid,
					estadodocumento.esdordem,
					estadodocumento.esddsc
					FROM
					workflow.estadodocumento
					
					WHERE					
					workflow.estadodocumento.tpdid =  '".$tpdid."' 
					AND workflow.estadodocumento.esdstatus = 'A'
					
					ORDER BY esdordem ASC;
				";		
				
$resultado = $db->carregar($sql_estadodoc);	
$count = count($resultado);

if($resultado){
		
	$count++;
}

				
$sql_tipodoc = "SELECT
				 tpddsc
				 FROM
				 workflow.tipodocumento
				 WHERE tpdid = '".$tpdid."'				
			";	
$resultado_tpd = $db->carregar($sql_tipodoc);					
				
				
?>
<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">
</script>

<br/>
<form method="POST" name="formulario">
	
	<input type='hidden' name="count" id="count" value="<?=$count?>">
	<input type='hidden' name="evento" id="evento" value="<?=$_REQUEST['evento']?>">
	<input type='hidden' name="tpdid" id="tpdid" value="<?=$_REQUEST['tpdid'];?>">
	<input type='hidden' name="esdid_atual" id="esdid_atual" value="<?=$_REQUEST['esdid_atual'];?>">
	<input type='hidden' name="esdordem_atual" id="esdordem_atual" value="<?=$_REQUEST['esdordem_atual'];?>">
	<input type='hidden' name="esdid_anterior" id="esdid_anterior" value="<?=$_REQUEST['esdid_anterior']; ?>">
	<input type='hidden' name="esdid_posterior" id="esdid_posterior" value="<?=$_REQUEST['esdid_posterior']; ?>">
	<input type='hidden' name="ordem_anterior" id="ordem_anterior" value="<?=$_REQUEST['ordem_anterior']; ?>">
	<input type='hidden' name="ordem_posterior" id="ordem_posterior" value="<?=$_REQUEST['ordem_posterior']; ?>">
	
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">	
	
		<colgroup>
			<col style="width: 25%;"/>
			<col/>
		</colgroup>
		<tbody>
			<tr>
				<td align='right' class="SubTituloDireita">Tipo:</td>
				<td><?=$resultado_tpd[0]['tpddsc']?></td>
			</tr>
			
			<tr>
				<td align='right' class="SubTituloDireita">Descrição:</td>
				<td><?= campo_textarea( 'esddsc', 'S', 'S', '', 70, 3, 500 ); ?></td>
			</tr>
			
			<!-- 
			
			<?
				//só exibe o campo Ativo caso o usuário fro superusuario
				if($_SESSION['superuser']){
				?>
				
				<tr>
				<td align='right' class="SubTituloDireita">Ativo:</td>
				<?			
					if($esdstatus == "A"){
						echo("<td><input type='checkbox' id='esdstatus' name='esdstatus' checked='true' value='A'> </td>");
					}else{						
						echo("<td><input type='checkbox' id='esdstatus' name='esdstatus' value='A'> </td>");
					}
				?>
				
				</tr>
					
				<?					
				}				
			?>	
		 -->
			
			<tr bgcolor="#C0C0C0">
				<td>&nbsp;</td>
				<?
					if($_REQUEST['evento']=='pre_alterar_estado'){
						echo("<td><input type='button' class='botao' value='Alterar' id='btalterarestado' name='btalterarestado' onclick=\"alterarEstado()\" />
			                  <input type='button' class='botao' value='Cancelar' id='btcancelar' name='btcancelar' onclick=\"cancelar()\" /></td>");
					}else{
						echo("<td><input type='button' class='botao' name='consultar' value='Adicionar' onclick='enviar()'</td>");
					}
				?>				
			</tr>
		</tbody>
	</table>
</form>

<br/>

<?
if($resultado){
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<colgroup>
		<col style="width: 70px;"/>
		<col style="width: 200px;"/>
		<col/>
		<col style="width: 70px;"/>
		<col style="width: 70px;"/>		
	</colgroup>
	<?php if ( count( $resultado ) ) : ?>
		<thead>
			<tr bgcolor="#dfdfdf">
				<td style="text-align: center;"><b>Ações</b></td>
				<td style="text-align: center;"><b>Ordem</b></td>
				<td style="text-align: center;"><b>Descrição</b></td>
				<td style="text-align: center;"><b>Status</b></td>				
				<td style="text-align: center;"><b>Ordem</b></td>
			</tr>
		</thead>
		<?php $cor = ''; ?>
		<?php foreach ( $resultado as $indice => $atributo ) : 
		
			// calculando os indices
			$esdid_atual = $resultado[$indice]['esdid'] ;
			$esdordem_atual = $resultado[$indice]['esdordem'] ;
			
			$esdid_anterior = $resultado[$indice - 1]['esdid'] ;
			$esdordem_anterior = $resultado[$indice - 1]['esdordem'] ;
			
			$esdid_posterior = $resultado[$indice + 1]['esdid'] ;
			$esdordem_posterior = $resultado[$indice + 1]['esdordem'] ;
			
		?>
		
			<?php $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; ?>
			<tr bgcolor="<?= $cor ?>" onmouseout="this.style.backgroundColor='<?= $cor ?>';" onmouseover="this.style.backgroundColor='#ffffcc';">
				<td align="center">				
					<img
						align="absmiddle"
						src="/imagens/alterar.gif"
						style="cursor: pointer"
						onclick="preAlterarEstado(<?=$esdid_atual ?>, <?=$resultado[$indice]['esdordem']?>, '<?=$resultado[$indice]['esddsc']?>')"
						title="Alterar estado" />
					<img
						align="absmiddle"
						src="/imagens/excluir.gif"
						style="cursor: pointer"
						onclick="removerEstado(<?=$esdid_atual?>,<?=$esdordem_atual?> )"
						title="Excluir estado"/>
				</td>
				<td style="text-align: center;"><?= $atributo['esdordem'] ?></td>
				<td><?= $atributo['esddsc'] ?></td>
				<!-- <td style="text-align: center;"><?= $atributo['esdstatus'] ?></td> -->				
				<td style="text-align: center;">
				<?php if ( $indice == 0 ): ?>
					<img align="absmiddle" src="../imagens/seta_cimad.gif"/>
				<?php else: ?>
					<img align="absmiddle" onclick="aumentarOrdem(<?=$esdid_atual ?>, <?=$esdordem_atual ?>, <?=$esdid_anterior ?>, <?=$esdordem_anterior ?>)" title="Aumentar a ordem do estado" src="../imagens/seta_cima.gif" style="cursor:pointer;"/>
				<?php endif; ?>
				<?php if ( $indice + 1 < count( $resultado ) ): ?>
					<img align="absmiddle" onclick="diminuirOrdem(<?=$esdid_atual ?>, <?=$esdordem_atual ?>, <?=$esdid_posterior ?>, <?=$esdordem_posterior ?>)" title="Diminuir a ordem do estado" src="../imagens/seta_baixo.gif" style="cursor:pointer;"/>
				<?php else: ?>
					<img align="absmiddle" src="../imagens/seta_baixod.gif"/>
				<?php endif; ?>
				</td>
			</tr>
		<?php endforeach; ?>
	<?php else : ?>
		<tr>
			<td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; font-weight:bold; font-size: 10px;" colspan="6">
				Não há estados.
			</td>
		</tr>
	<?php endif; ?>
</table>

<?
}
?>

<script><!--
	
	function preAlterarEstado(esdid, esdordem, esddsc){
		
		document.getElementById("esdid_atual").value= esdid;
		document.getElementById("esdordem_atual").value= esdordem;		
		document.formulario.esddsc.value = esddsc;				
		//document.formulario.esdstatus.value = esdstatus;
		
		//if(document.formulario.esdstatus.value == 'A'){
		//	document.formulario.esdstatus.checked = 'true';	
		//}	
		
		document.getElementById("evento").value= "pre_alterar_estado";
		
		document.formulario.submit();
						
	}

	function alterarEstado()
	{
		
			if ( !document.formulario.esddsc.value)
			{
				alert( 'Entre uma descrição para o estado do documento.' );
				document.formulario.esddsc.focus();
			}	
		
		document.getElementById("evento").value= "alterar_estado";	
		
		document.formulario.submit();
	}
	
	function removerEstado(esdid_atual, esdordem_atual)
	{				
		document.getElementById("esdid_atual").value = esdid_atual;		
		document.getElementById("esdordem_atual").value = esdordem_atual;
		document.getElementById("evento").value= "remover_estado";
		document.formulario.submit();		
	}	
	
	
	function enviar()
	{
		if ( !document.formulario.esddsc.value)
			{
				alert( 'Entre uma descrição para o estado do documento.' );
				document.formulario.esddsc.focus();
				return;
			}	
			//alert(document.getElementById("count").value);		
			
		document.getElementById("evento").value= "adicionar_estado";		
		
			
		document.formulario.submit();
			
	}
	
	function cancelar(){
		
		document.getElementById("esdid_atual").value= 0;
		document.getElementById("esdordem_atual").value= 0;
		document.formulario.esddsc.value = '';			
		//document.formulario.esdstatus.value = '';	
		
		document.getElementById("evento").value= "";
		
		document.formulario.submit();
						
	}
	
	function aumentarOrdem(esdid_atual, esdordem_atual, esdid_anterior, ordem_anterior)
	{
		document.getElementById("esdid_atual").value = esdid_atual;
		document.getElementById("esdordem_atual").value = esdordem_atual;
		document.getElementById("esdid_anterior").value= esdid_anterior;
		document.getElementById("ordem_anterior").value = ordem_anterior;
		document.getElementById("evento").value= "aumentar_ordem";
		document.formulario.submit();	
				
	}	
	
	function diminuirOrdem(esdid_atual, esdordem_atual, esdid_posterior, ordem_posterior)
	{
		document.getElementById("esdid_atual").value = esdid_atual;
		document.getElementById("esdordem_atual").value = esdordem_atual;
		document.getElementById("esdid_posterior").value= esdid_posterior;
		document.getElementById("ordem_posterior").value = ordem_posterior;	
		document.getElementById("evento").value= "diminuir_ordem";		
		document.formulario.submit();	
	}
--></script>

<?

/**
 * função que atualiza as ordens dos estados da lista de estados
 *
 * @param unknown_type $_esdordem - representa a ordem q foi eliminada da lista
 * @param unknown_type $_tpdid - representa o tipo do sistema q pertence os estados desta lista
 * @param unknown_type $sentido - representa o sentido da atualização da lista, para cima = 'cima', para baixo = 'baixo'
 */
function atualizarLista($_esdordem, $_tpdid){
	global $db;
	
	$sql_update="
		UPDATE workflow.estadodocumento  
		SET esdordem = (esdordem - 1) 
		WHERE esdordem > '".$_esdordem."' AND tpdid = '".$_tpdid."' AND esdstatus = 'A'
	";
	
	$db->executar($sql_update);
	$db->commit();
	
}

function ce_adicionarEstado($esddsc, $tpdid, $esdordem){
	global $db;
	
/*	if($esdstatus != 'A'){
		$esdstatus = 'I';
	}*/
	
	$sql_inserir = "
		INSERT INTO workflow.estadodocumento 
		(esdstatus, esddsc, tpdid, esdordem)
		VALUES ('A', '".$esddsc."', '".$tpdid."', '".$esdordem."') 
		 
	";
	$db->executar($sql_inserir);
	$db->commit();
}

function ce_aumentarOrdem($_esdid_atual, $_ordem_atual, $_esdid_anterior, $_ordem_anterior){
	global $db;
	
	$sql_update="UPDATE
			 workflow.estadodocumento 
			 SET esdordem = ".$_ordem_anterior." 
			 WHERE esdid = ".$_esdid_atual." AND esdstatus = 'A'
			 ";
			
	$db->executar($sql_update);
	$db->commit();	
			
	$sql_update="UPDATE
			 workflow.estadodocumento 
			 SET esdordem = ".$_ordem_atual." 
			 WHERE esdid = ".$_esdid_anterior." AND esdstatus = 'A'
			 ";
			
	$db->executar($sql_update);
	$db->commit();		
}

function ce_diminuirOrdem($_esdid_atual, $_ordem_atual,  $_esdid_posterior, $_ordem_posterior){
	global $db;
	
	$sql_update="UPDATE
			 workflow.estadodocumento 
			 SET esdordem = ".$_ordem_posterior." 
			 WHERE esdid = ".$_esdid_atual."
			 ";
			
	$db->executar($sql_update);
	$db->commit();		
			
	$sql_update="UPDATE
			 workflow.estadodocumento 
			 SET esdordem = ".$_ordem_atual." 
			 WHERE esdid = ".$_esdid_posterior."
			 ";
			
	$db->executar($sql_update);
	$db->commit();
	
}

function ce_excluirEstado($esdid){
	global $db;
		
	$sql_update="
		UPDATE workflow.estadodocumento  
		SET esdstatus = 'I'
		WHERE esdid='".$esdid."'
	";
	$db->executar($sql_update);
	$db->commit();
}

function cd_alteraEstado($esddsc, $esdid){
	global $db;
	
	if(empty($esdstatus)) $esdstatus = "I";  
	
	$sql_alterar = "UPDATE workflow.estadodocumento
	                SET esddsc= '".$esddsc."'
	                WHERE esdid ='".$esdid."'  
	";
	
	$db->executar($sql_alterar);
	$db->commit();
}
?>