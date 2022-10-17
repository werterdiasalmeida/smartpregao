<?

include_once "./_funcoesmonitoramento.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

/* montando cabeçalho */
$topo = "<table width='100%'>
			<tr>
			<td align='right'><b>Sistema:</b></td>
			<td>".$db->pegaUm("SELECT sisdsc FROM seguranca.sistema WHERe sisid='".$_REQUEST['sisid']."'")."</td>
			<td align='right'><b>Período:</b></td>
			<td>".sprintf("%02d/%04d", $_REQUEST['mes'], $_REQUEST['ano'])."</td>
			</tr>
		 </table>";

monta_titulo( "Monitoramento dos módulos", $topo);

?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title>Detalhes do monitoramento</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript">
this._closeWindows = false;
</script>
</head>
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/seguranca.js"></script>
<script language="javascript" type="text/javascript" src="../includes/open_flash_chart/swfobject.js"></script>
<?
// carregando o número de dias do mês	
$ndias = cal_days_in_month(CAL_GREGORIAN, $_REQUEST['mes'], $_REQUEST['ano']);

$_HTML .= "<div style='margin-left:25;width:910px;overflow:auto;'>";
$_HTML .= "<table class=\"listagem\" width=\"95%\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
$_HTML .= "<tr>";
$_HTML .= "<td class=\"SubTituloCentro\">Informações</td>";

for($i=1;$i<=$ndias;$i++) {
	
	if(date("w", mktime(0,0,0,$_REQUEST['mes'],$i,$_REQUEST['ano'])) == 0 || 
   	   date("w", mktime(0,0,0,$_REQUEST['mes'],$i,$_REQUEST['ano'])) == 6) {
   	   	
		$_HTML .= "<td class=\"SubTituloCentro\" style=\"background-color:#FFFFCC;width:25px;\">".$i."</td>";
   	   	
   	} else {
	
		$_HTML .= "<td class=\"SubTituloCentro\" style=\"width:25px;\">".$i."</td>";
		
   	}
			

}
$_HTML .= "</tr>";

$dadostr = pegarDados($_REQUEST);

if($dadostr) {
	foreach($dadostr as $tmoid => $str) {
		$_HTML .= "<tr>";
		$_HTML .= "<td class=\"SubTituloDireita\" nowrap>".$db->pegaUm("SELECT tmodescricao FROM seguranca.tipomonitoramento WHERE tmoid='".$tmoid."'")."</td>";
		
		for($i=1;$i<=$ndias;$i++) {
			if(date("w", mktime(0,0,0,$_REQUEST['mes'],$i,$_REQUEST['ano'])) == 0 || 
		   	   date("w", mktime(0,0,0,$_REQUEST['mes'],$i,$_REQUEST['ano'])) == 6) {
				$_HTML .= "<td style=\"text-align:right;background-color:#FFFFCC;width:25px;\">".(($str[$i])?"<a style=\"cursor:pointer;\" onclick=\"window.location='seguranca.php?modulo=principal/monitoramentopaineldetalheslista&acao=A&sisid=".$_REQUEST['sisid']."&tmoid=".$tmoid."&ano=".$_REQUEST['ano']."&mes=".$_REQUEST['mes']."&dia=".$i."';\">".$str[$i]."</a>":"-")."</td>";
		   	} else {
				$_HTML .= "<td style=\"text-align:right;width:25px;\">".(($str[$i])?"<a style=\"cursor:pointer;\" onclick=\"window.location='seguranca.php?modulo=principal/monitoramentopaineldetalheslista&acao=A&sisid=".$_REQUEST['sisid']."&tmoid=".$tmoid."&ano=".$_REQUEST['ano']."&mes=".$_REQUEST['mes']."&dia=".$i."';\">".$str[$i]."</a>":"-")."</td>";
		   	}
		}
		
		$_HTML .= "</tr>";
	}

}

$_HTML .= "</table>";
$_HTML .= "</div>";

echo $_HTML;

include_once "../../includes/open_flash_chart/open-flash-chart-object.php";


$sql = "SELECT tmoid as codigo, tmodescricao as descricao FROM seguranca.tipomonitoramento ORDER BY tmoordem";
$tipomonitoramento = $db->carregar($sql);

?>
<table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?
if($tipomonitoramento[0]) {
	foreach($tipomonitoramento as $tpm) {
?>
<tr>
<td class="SubTituloCentro"><? echo $tpm['descricao']; ?></td>
</tr>
<tr>
<td>
<div id="grafico<? echo $tpm['codigo']; ?>"></div>
<script type="text/javascript">
swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico<? echo $tpm['codigo']; ?>", "910", "220", "9.0.0", "expressInstall.swf", {"data-file":"chart-data.php?dados=<? echo $_REQUEST['sisid'].";".$_REQUEST['mes'].";".$_REQUEST['ano'].";".$tpm['codigo']; ?>","loading":"Carregando gráfico..."} );
</script>
</td>
</tr>
<?
	}
}
?>
<tr>
<td class="SubTituloCentro">Y(Esquerda): <? $db->monta_combo('yesquerda', $tipomonitoramento, 'S', 'Selecione', '', '', '', '200', 'N', 'yesquerda'); ?>  | X(Direita): <? $db->monta_combo('ydireita', $tipomonitoramento, 'S', 'Selecione', '', '', '', '200', 'N', 'ydireita'); ?> <input type="button" name="combinar" value="Combinar" onclick="combinarGrafico('<? echo $_REQUEST['sisid']; ?>','<? echo $_REQUEST['mes']; ?>','<? echo $_REQUEST['ano'] ?>');"></td>
</tr>
<tr id="tr_grafico_mix" style="display:none">
<td>
<div id="grafico_mix"></div>
</td>
</tr>
</table>
</body>
</html>
