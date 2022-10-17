<?php
header('content-type: text/html; charset=ISO-8859-1');
ini_set("memory_limit", "1024M");
set_time_limit(0); 
include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ. 'includes/classes/relatorio.class.inc';
$indid = !$_SESSION['indid'] ? $_GET['indid'] : $_SESSION['indid'];

$sql = "select unmid, regid, indqtdevalor, indcumulativo, indcumulativovalor from painel.indicador where indid = $indid";

$dadosInd = $db->pegaLinha($sql);

if($_POST['btnsubmit'] == "1"){
	
	/* ********* *  INÍCIO - REGRA PARA PROCENTAGEM E ÍNDICE * ********* */
	if(($dadosInd['unmid'] == UNIDADEMEDICAO_PERCENTUAL || $dadosInd['unmid'] == UNIDADEMEDICAO_NUM_INDICE) && $dadosInd['regid'] != REGIONALIZACAO_BRASIL){
		
		if($dadosInd['regid'] == REGIONALIZACAO_UF && $_POST["estuf"] != "todos" && $_POST["estuf"] != "" && $_POST["estuf"] != null){
			
		}elseif($dadosInd['regid'] == REGIONALIZACAO_MUN && $_POST["muncod"] != "todos" && $_POST["muncod"] != "" && $_POST["muncod"] != null){
			
		}else{?>
			<table class="tabela" width="100%" cellSpacing="1" border=0 cellPadding="3" align="center">
				<tr><td style="text-align: center;color:#990000" >Favor selecionar <?=$dadosInd['regid'] == REGIONALIZACAO_UF ? "o Estado" : "o Município"?>.</td></tr>
			</table>
		<?php
			exit;
		}
	}
	/* ********* *  FIM - REGRA PARA PROCENTAGEM E ÍNDICE * ********* */
	
	$arrParametros = getParametrosRelatorioIndicador($indid,$_POST);
//	dbg($arrParametros['sql'],1);
	$dados = $db->carregar( $arrParametros['sql'] );
	$r = new montaRelatorio();
	$r->setAgrupador( $arrParametros['agrupadores'] , $dados ); 
	$r->setColuna( $arrParametros['colunas'] );
	$r->setTotNivel(true);
	
	if($dadosInd['indqtdevalor'] == "f"){ 
		$totalizador = $dadosInd['indcumulativo'] == "S" ? true : false;
	}else{
		$totalizador = $dadosInd['indcumulativo'] == "N" && $dadosInd['indcumulativovalor'] == "N" ? false : true;
	}
	
	$r->setTolizadorLinha( $totalizador );
	
	echo $r->getRelatorio();
	exit;
}

if($_POST['btnsubmit'] == "2"){
	
	/* ********* *  INÍCIO - REGRA PARA PROCENTAGEM E ÍNDICE * ********* */
	if(($dadosInd['unmid'] == UNIDADEMEDICAO_PERCENTUAL || $dadosInd['unmid'] == UNIDADEMEDICAO_NUM_INDICE) && $dadosInd['regid'] != REGIONALIZACAO_BRASIL){
		
		if($dadosInd['regid'] == REGIONALIZACAO_UF && $_REQUEST["estuf"] != "todos" && $_REQUEST["estuf"] != "" && $_REQUEST["estuf"] != null){
			
		}elseif($dadosInd['regid'] == REGIONALIZACAO_MUN && $_REQUEST["muncod"] != "todos" && $_REQUEST["muncod"] != "" && $_REQUEST["muncod"] != null){
		
		}else{?>
			<table class="tabela" width="100%" cellSpacing="1" border=0 cellPadding="3" align="center">
				<tr><td style="text-align: center;color:#990000" >Favor selecionar <?=$dadosInd['regid'] == REGIONALIZACAO_UF ? "o Estado" : "o Município"?>.</td></tr>
			</table>
		<?php
			exit;
		}
	}
	/* ********* *  FIM - REGRA PARA PROCENTAGEM E ÍNDICE * ********* */
	
	$arrParametros = getParametrosRelatorioIndicador($indid,$_REQUEST,true);
	$db->sql_to_excel( $arrParametros['sql'], "Relatório", $arrParametros['colunasExcel'] );
	die;
}

$agrupadorHtml =
<<<EOF
    <table>
        <tr valign="middle">
            <td>
                <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 250px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <!--
                <img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
                -->
                <img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
            </td>
            <td>
                <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 250px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
                <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
            </td>
        </tr>
    </table>
    <script type="text/javascript" language="javascript">
        limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
        limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
        {POVOAR_ORIGEM}
        {POVOAR_DESTINO}
        sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
    </script>
EOF;

//Pega os agrupadores de acordo com os Detalhamentos do Indicador
$arrAgrupadores = getAgrupadoresPorDetalhe($indid);
//Pega os agrupadores de acordo com a Regionalização do Indicador
$arrAgrPorReg = getAgrupadorPorRegionalizador($indid);
//Inclui os agrupadores de Regionalização no array de agrupadores de Detalhe
if(is_array($arrAgrPorReg)){
	foreach($arrAgrPorReg as $arrAgr){
		array_push($arrAgrupadores,$arrAgr);
	}
}

//Define se o Período é padrão
if($dadosInd['indqtdevalor'] == "f"){ 
	$dadosInd['indcumulativo'] == "S" ? $arrAgrupados = null : $arrAgrupados[] = array( "codigo" => "dp.dpedsc" , "descricao" => "Período" );
}else{
	$totalizador = $dadosInd['indcumulativo'] == "N" && $dadosInd['indcumulativovalor'] == "N" ? $arrAgrupados = null : $arrAgrupados[] = array( "codigo" => "dp.dpedsc" , "descricao" => "Período" );
}

$agrupador = new Agrupador( 'formulario', $agrupadorHtml );
$agrupador->setOrigem( 'naoAgrupador', null, $arrAgrupadores );
$agrupador->setDestino( 'agrupador',null, $arrAgrupados );

?>
<form name="formulario" method="post" id="formdashboard">
	<table class="listagem" width="100%" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloCentro">
				<?php $agrupador->exibir(); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				<input type="button" name="btnXls" value="Visualizar XLS" onclick="relatorioXLS();" style="cursor: pointer;"/>
				<input type="button" name="buttonsubmit" value="OK" onclick="exibirRelatorioDashBoard();">
				<input type="hidden" name="btnsubmit" value="1">
			</td>
		</tr>
	</table>
	<div style="width:100%;height:300px;overflow:auto;" id="divdashboard">
	</div>
</form>
