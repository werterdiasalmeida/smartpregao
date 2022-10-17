<?php
// Parâmetros para a nova conexão com o banco do Painel Público
$servidor_bd = '192.168.222.17';
$porta_bd    = '5432';
$nome_bd     = 'dbsimec';
$usuario_db  = 'simec';
$senha_bd    = 'phpsimecalt';
// Parâmetros para (teste com nova conexão no Espelho de Produção)
//$nome_bd     = 'simec_espelho_producao';
//$servidor_bd = 'mecsrv168';
//$porta_bd    = '5432';
//$usuario_db  = 'simec';
//$senha_bd    = 'phpsimecao';

// Cria o novo objeto de conexão
$db2 = new cls_banco();

if($_REQUEST['requisicao']) {
	if($_REQUEST['db_sub']=='sim') $db = new cls_banco();;
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');

$titulo = "Painel Público";
monta_titulo( $titulo, '&nbsp;' );

$indid = !$_REQUEST['indid'] ? $_SESSION['indid'] : $_REQUEST['indid'];

$arrSimec   = getDadosIndicador($indid,$db);
$arrPublico = getDadosIndicador($indid,$db2);

?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>

function exportarsehcsv(sehid, db_sub) {
	window.location='painel.php?modulo=principal/painelPublico&acao=A&requisicao=exportarsehcsv&db_sub='+db_sub+'&sehid='+sehid;
}

function sincronizarPainelPublico(sehid) {
	var conf = confirm("Deseja realmente sincronizar esta série historica?");
	if(conf) {
		divCarregando();
		$.ajax({
	   		type: "POST",
	   		url: "painel.php?modulo=principal/painelPublico&acao=A",
	   		data: "requisicao=sincronizarPainelPublico&sehid="+sehid,
	   		success: function(msg){
	   			divCarregado();
	   			alert(msg);
	   			dadosPainelPublico();
	   		}
	 		});
 	}
 		
}

function sincronizarTodasSeh(sehids) {
	var seharr;
	var conf = confirm("Deseja realmente sincronizar todas as séries historicas?");
	if(conf) {
		seharr = sehids.split(";");
		divCarregando();
		for(var i=0;i<seharr.length;i++) {
			$.ajax({
		   		type: "POST",
		   		url: "painel.php?modulo=principal/painelPublico&acao=A",
		   		data: "requisicao=sincronizarPainelPublico&sehid="+seharr[i],
	   			async: false,
		   		success: function(msg){}
		 		});
		 		
		}
	}
	
	divCarregado();
	dadosPainelPublico();
}

function excluirSerieHistoricaPainelPublico(sehid){
	var conf = confirm("Deseja realmente excluir esta série historica?");
	if(conf) {
		$.ajax({
	   		type: "POST",
	   		url: "painel.php?modulo=principal/painelPublico&acao=A",
	   		data: "requisicao=removerSerieHistoricaPainelPublico&sehid="+sehid,
	   		success: function(msg){
	   			alert("Série Histórica excluída com sucesso");
	   		}
	 		});

		dadosPainelPublico();
	}

}


function dadosPainelPublico() {

	$.ajax({
   		type: "POST",
   		url: "painel.php?modulo=principal/painelPublico&acao=A",
   		data: "requisicao=dadosSincronizacao",
   		success: function(msg){
   			document.getElementById('dados').innerHTML = msg;
   		}
 		});

}

function sincronizarIndicador()
{
	if(confirm("Deseja realmente sincronizar o Indicador?")){
		$('[name=sincronizar]').val('Sincronizando...');
		$('[name=sincronizar]').attr('disabled','disabled');
		$.ajax({
	   		type: "POST",
	   		url: "painel.php?modulo=principal/painelPublico&acao=A",
	   		data: "requisicao=sicronizarIndicador&indid=<? echo $indid ?>",
	   		success: function(msg){
				var resultado = msg;
				if(resultado.length > 5){
					alert('Não foi possível realizar a sincronia!');
					document.getElementById('dados').innerHTML = msg;
					//window.location.reload();
				}else{
					window.open('http://painel.mec.gov.br/limparCache.php?close=true');
					alert('Dados Sincronizados com Sucesso!');
					window.location.reload();
				} 
	   		}
	 		});
	 }
}

</script>

<style>
	.SubtituloTabela{background-color:#cccccc}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.justificado{text-align: justify}
	.diferente{background-color:#FFFACD;font-weight:bold}
</style>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="50%" colspan="2" class="SubtituloTabela center bold" >Painel de Controle</td>
		<td colspan="2" class="SubtituloTabela center bold" >Painel Público</td>
	</tr>
	<tr>
		<td width="15%" class="SubtituloDireita" >Identificador:</td>
		<td><?php echo $arrSimec['indid']; ?></td>
		<td width="15%" class="SubtituloDireita" >Identificador:</td>
		<td <?php echo $arrSimec['indid'] != $arrPublico['indid'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['indid']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Indicador:</td>
		<td><?php echo $arrSimec['indnome']; ?></td>
		<td class="SubtituloDireita" >Indicador:</td>
		<td <?php echo $arrSimec['indnome'] != $arrPublico['indnome'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['indnome']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Eixo:</td>
		<td><?php echo $arrSimec['exodsc']; ?></td>
		<td class="SubtituloDireita" >Eixo:</td>
		<td <?php echo $arrSimec['exodsc'] != $arrPublico['exodsc'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['exodsc']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Secretaria/Autarquia Executora:</td>
		<td><?php echo $arrSimec['secretaria']; ?></td>
		<td class="SubtituloDireita" >Secretaria/Autarquia Executora:</td>
		<td <?php echo $arrSimec['secretaria'] != $arrPublico['secretaria'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['secretaria']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Ação:</td>
		<td><?php echo $arrSimec['acadsc']; ?></td>
		<td class="SubtituloDireita" >Ação:</td>
		<td <?php echo $arrSimec['acadsc'] != $arrPublico['acadsc'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['acadsc']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Secretaria/Autarquia Gestora:</td>
		<td><?php echo $arrSimec['secdsc']; ?></td>
		<td class="SubtituloDireita" >Secretaria/Autarquia Gestora:</td>
		<td <?php echo $arrSimec['secdsc'] != $arrPublico['secdsc'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['secdsc']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Objetivo Indicador:</td>
		<td><?php echo $arrSimec['indobjetivo']; ?></td>
		<td class="SubtituloDireita" >Objetivo Indicador:</td>
		<td <?php echo $arrSimec['indobjetivo'] != $arrPublico['indobjetivo'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['indobjetivo']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Observação:</td>
		<td><?php echo $arrSimec['indobservacao']; ?></td>
		<td class="SubtituloDireita" >Observação:</td>
		<td <?php echo $arrSimec['indobservacao'] != $arrPublico['indobservacao'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['indobservacao']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Fórmula:</td>
		<td><?php echo $arrSimec['indformula']; ?></td>
		<td class="SubtituloDireita" >Fórmula:</td>
		<td <?php echo $arrSimec['indformula'] != $arrPublico['indformula'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['indformula']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Termos:</td>
		<td><?php echo $arrSimec['indtermos']; ?></td>
		<td class="SubtituloDireita" >Termos:</td>
		<td <?php echo $arrSimec['indtermos'] != $arrPublico['indtermos'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['indtermos']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Fontes por Termo:</td>
		<td><?php echo $arrSimec['indfontetermo']; ?></td>
		<td class="SubtituloDireita" >Fontes por Termo:</td>
		<td <?php echo $arrSimec['indfontetermo'] != $arrPublico['indfontetermo'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['indfontetermo']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Unidade de Medição:</td>
		<td><?php echo $arrSimec['unmdesc']; ?></td>
		<td class="SubtituloDireita" >Unidade de Medição:</td>
		<td <?php echo $arrSimec['unmdesc'] != $arrPublico['unmdesc'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['unmdesc']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Exibir produto da unidade como:</td>
		<td><?php echo $arrSimec['umedesc']; ?></td>
		<td class="SubtituloDireita" >Exibir produto da unidade como:</td>
		<td <?php echo $arrSimec['umedesc'] != $arrPublico['umedesc'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['umedesc']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Coletar valor monetário?</td>
		<td><?php echo $arrSimec['valormonetario']; ?></td>
		<td class="SubtituloDireita" >Coletar valor monetário?</td>
		<td <?php echo $arrSimec['valormonetario'] != $arrPublico['valormonetario'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['valormonetario']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Exibir dados acumulados entre os períodos?</td>
		<td><?php echo $arrSimec['qndtacumulada']; ?></td>
		<td class="SubtituloDireita" >Exibir dados acumulados entre os períodos?</td>
		<td <?php echo $arrSimec['qndtacumulada'] != $arrPublico['qndtacumulada'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['qndtacumulada']; ?></td>
	</tr>
	<?php if($arrSimec['valormonetario'] == "Sim" || $arrPublico['valormonetario'] == "Sim"):?>
		<tr>
			<td class="SubtituloDireita" >Exibir valor monetário acumulado entre os períodos?</td>
			<td><?php echo $arrSimec['valoracumulado']; ?></td>
			<td class="SubtituloDireita" >Exibir valor monetário acumulado entre os períodos?</td>
			<td <?php echo $arrSimec['valoracumulado'] != $arrPublico['valoracumulado'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['valoracumulado']; ?></td>
		</tr>
	<?php endif; ?>
	<tr>
		<td class="SubtituloDireita" >Publicado?</td>
		<td><?php echo $arrSimec['publicado']; ?></td>
		<td class="SubtituloDireita" >Publicado?</td>
		<td <?php echo $arrSimec['publicado'] != $arrPublico['publicado'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['publicado']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Módulo Público?</td>
		<td><?php echo $arrSimec['publico']; ?></td>
		<td class="SubtituloDireita" >Módulo Público?</td>
		<td <?php echo $arrSimec['publico'] != $arrPublico['publico'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['publico']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Periodicidade do Indicador:</td>
		<td><?php echo $arrSimec['perdsc']; ?></td>
		<td class="SubtituloDireita" >Periodicidade do Indicador:</td>
		<td <?php echo $arrSimec['perdsc'] != $arrPublico['perdsc'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['perdsc']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Estilo:</td>
		<td><?php echo $arrSimec['estdsc']; ?></td>
		<td class="SubtituloDireita" >Estilo:</td>
		<td <?php echo $arrSimec['estdsc'] != $arrPublico['estdsc'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['estdsc']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Regionalização:</td>
		<td><?php echo $arrSimec['regdescricao']; ?></td>
		<td class="SubtituloDireita" >Regionalização:</td>
		<td <?php echo $arrSimec['regdescricao'] != $arrPublico['regdescricao'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['regdescricao']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Periodicidade de atualização:</td>
		<td><?php echo $arrSimec['perdata']; ?></td>
		<td class="SubtituloDireita" >Periodicidade de atualização:</td>
		<td <?php echo $arrSimec['perdata'] != $arrPublico['perdata'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['perdata']; ?></td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >Coleta:</td>
		<td><?php echo $arrSimec['coldsc']; ?></td>
		<td class="SubtituloDireita" >Coleta:</td>
		<td <?php echo $arrSimec['coldsc'] != $arrPublico['coldsc'] ? "class='diferente'" : ""; ?> ><?php echo $arrPublico['coldsc']; ?></td>
	</tr>
	<?php $arrDetSimec   = getDetalhesIndicador($indid,$db); ?>
	<?php $arrDetPublico = getDetalhesIndicador($indid,$db2); ?>
	<?php if($arrDetSimec): ?>
		<tr>
			<td colspan="4" class="SubtituloTabela bold center" >Detalhes do Indicador</td>
		</tr>
		<?php $n = 0; ?>
		<?php foreach($arrDetSimec as $tdiid => $arrDS): ?>
			<tr>
				<td class="SubtituloDireita" >Detalhe:</td>
				<td><?php echo $arrDS['tdidsc']; ?> / Ordem: <?php echo $arrDS['tdiordem']; ?></td>
				<td class="SubtituloDireita" >Detalhe:</td>
				<td <?php echo ($arrDS['tdidsc'] != $arrDetPublico[$tdiid]['tdidsc']) || ($arrDS['tdiordem'] != $arrDetPublico[$tdiid]['tdiordem']) ? "class='diferente'" : ""; ?> ><?php echo $arrDetPublico[$tdiid]['tdidsc']; ?> / Ordem:  <?php echo $arrDetPublico[$tdiid]['tdiordem'] ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita" ><?php echo $arrDS['tdidsc']; ?>:</td>
				<td>
					<?php if(is_array($arrDS['tipo'])): ?>
						<?php foreach($arrDS['tipo'] as $key => $tipo): ?>
							<?php echo " - ".$tipo['tiddsc'] ?>
							<?php echo ($key == (count($arrDS['tipo']) - 1) ? "." : ";<br />") ?>
						<?php endforeach; ?>
					<?php else: ?>
						Não existem detalhes atribuídos!
					<?php endif;?>
				</td>
				<td class="SubtituloDireita" ><?php echo $arrDetPublico[$tdiid]['tdidsc']; ?>:</td>
				<td>
					<?php if(is_array($arrDetPublico[$tdiid]['tipo'])): ?>
						<?php foreach($arrDS['tipo'] as $key => $tipo): ?>
							<?php if($arrDetPublico[$tdiid]['tipo'][$key]['tiddsc'] != $tipo['tiddsc']): ?>
								<?php echo "<span class='diferente' > - ".$arrDetPublico[$tdiid]['tipo'][$key]['tiddsc']. "</span>"?>
							<?php else: ?>
								<?php echo " - ".$arrDetPublico[$tdiid]['tipo'][$key]['tiddsc']?>
							<?php endif; ?>
							<?php echo ($key == (count($arrDetPublico[$tdiid]['tipo']) - 1) ? "." : ";<br />") ?>
						<?php endforeach; ?>
					<?php else: ?>
						Não existem detalhes atribuídos!
					<?php endif;?>
				</td>
			</tr>
			<?php $n++; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	<tr>
		<td colspan="4" class="SubtituloTabela center bold" ><input type="button" name="sincronizar" value="Sincronizar Indicador" onclick="sincronizarIndicador()" /></td>
	</tr>
</table>

<div id="dados"></div>
<script>
$(document).ready(function() {
	dadosPainelPublico();
});
</script>