<?

if( $_REQUEST['lesid'] ){
	include "popupLog.inc";
	die();
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando cabeçalho */
monta_titulo("Log agendamento de scripts", "Log da execução dos agendamentos");

$arCabecalho = array("Ação",
					 "Data",
					 "Arquivo");

$arConfig = array("style" 			=> "width:95%;",
				  "totalLinha" 		=> false,
				  "totalRegistro" 	=> true);


$sql = "SELECT
			ags.agsfile,
			TO_CHAR(les.lesdtexecucao, 'DD/MM/yyyy HH24:MI:SS') AS data,
			les.lesdtexecucao,
		    ags.agsid,
			les.lesid
		FROM
			seguranca.logexecucaoscripts les
		JOIN
			seguranca.agendamentoscripts ags ON ags.agsid = les.agsid
		ORDER BY
			les.lesdtexecucao DESC";

$acao = '<div style="text-align:center;">
			<img src="/imagens/consultar.gif" style="cursor:pointer" title="Visualizar Log" onclick="visualizalog({lesid});">
		 </div>';

$arParamCol[1]['type'] = Lista::TYPEDATE;

 $rs = Paginacao::getPaginacao($db, $sql, 30);

$oLista = new Lista($arConfig);
$oLista->setCabecalho( $arCabecalho );
$oLista->setAcao( $acao );
$oLista->setCorpo( $rs, $arParamCol );
$oLista->show();

?>

<table class="tabela" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class='SubTituloDireita'>
		<input type="button" name="voltar" value="Voltar" onclick="window.location='seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A';">
	</td>
</tr>
</table>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"> </script>
<script type="text/javascript">

	function visualizalog(id){
		window.open('?modulo=principal/logscripts&acao=A&lesid='+id,
				    'log',
				    'width=800,height=600,scrollbars=yes,scrolling=yes,resizebled=yes');
	}

</script>