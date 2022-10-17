<?
/*
 * TESTANDO SE A VARIAVEL FOI SELECIONADA
 * INDID É OBRIGATORIO
 */
if(!$_SESSION['indid']) {
	echo "<script>
			alert('Variáveis não foram selecionadas');
			window.location='?modulo=inicio&acao=C';
		  </script>";
	exit;
}

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);
/* FIM configurações */


if($_REQUEST['requisicao']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicao']($_REQUEST);
}

if($_REQUEST['arquivoCodigo']) {
	header('content-type: text/html; charset=ISO-8859-1');
	arquivoCodigo($_REQUEST['arquivoCodigo']);
}



// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";

$db->cria_aba($abacod_tela,$url,'');
/*
 * Criando título da TELA
 */
$titulo = "Agendamento de carga da série histórica";
monta_titulo( $titulo, '&nbsp;');
/*
 * Criando cabeçalho especifico do Indicador
 */

cabecalhoIndicador($_SESSION['indid']);

$indicadordados = $db->pegaLinha("SELECT * FROM painel.indicador WHERE indid='".$_SESSION['indid']."'");

$arrPflcod = pegaPerfil();
	
if(in_array(PAINEL_PERFIL_CONSULTA,$arrPflcod))
	$formhabilitado="N";
else
	$formhabilitado="S";


if(in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod) && !$db->testa_superuser()){
	$sql= "SELECT 
				indid
			FROM 
				painel.indicador
			WHERE
				acaid in (
							select 
								distinct acaid
							from
								painel.usuarioresponsabilidade
							where
								usucpf = '{$_SESSION['usucpf']}'
							and
								rpustatus = 'A'
							)";
	$arrIndicadores = $db->carregar($sql);
}
if(in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) && !$db->testa_superuser()){
	$sql= "SELECT indid	FROM painel.indicador WHERE exoid in (select distinct exoid	from painel.usuarioresponsabilidade	where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A')";
	$arrIndicadores = $db->carregar($sql);
}

if($arrIndicadores){
	foreach($arrIndicadores as $Ind){
		$arrInd[] = $Ind['indid'];
	}
}else{
	$arrIndicadores = array();
}

if((in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) || in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod)) && !in_array($_SESSION['indid'],$arrInd)){
	$formhabilitado="N";
}	
	
if(!$indicadordados) {
	die("Erro, redirecionar para página inicial");
}
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/painel.js"></script>
<form method="post" name="formulario" id="formulario" enctype="multipart/form-data" action="" target="iframeUpload">
<input type="hidden" name="requisicao" value="enviarAgendamento">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Agendamento da carga do indicador</td>
</tr>
<tr>
	<td class="SubTituloDireita">Manual para carga :</td>
	<td><b><a style="cursor:pointer;" onclick="verdicionariocarga();">CLIQUE AQUI</a></b></td>
</tr>
<tr>
	<td class="SubTituloDireita">Agendar publicação da série para :</td>
	<td><? echo campo_data( 'agddataprocessamento', 'N', $formhabilitado, '', 'S' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Arquivo :</td>
	<td><input type="file" name="arquivo" id="arquivocsv" <? echo (($formhabilitado=="N")?"disabled":""); ?>> <input type="button" value="Enviar" onclick="validarEnvioCarga('<? echo (($db->testa_superuser())?"sim":"nao"); ?>','<? echo date("d/m/Y");  ?>');" <? echo (($formhabilitado=="N")?"disabled":""); ?>></td>
</tr>
</table>
</form>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr><td class="SubTituloCentro">Log do upload</td></tr>
<tr><td>
<div id="1click"></div>
<iframe name="iframeUpload" id="iframeresp" style="width:100%;height:250px;border:0px solid #fff;"></iframe>
</td></tr>
<tr><td class="SubTituloCentro">Agendamentos cadastrados</td></tr>
<tr><td id="listaagendamento"></td></tr>
</table>
<script>
ajaxatualizar('requisicao=carregaragendamento','listaagendamento');
</script>