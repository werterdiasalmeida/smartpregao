<?php

// inicia sistema
include 'config.inc';
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "financeiro/modulos/relatorio/funcoes_consulta_financeiro.inc";
$db = new cls_banco();

$itens = cfBuscarItem( $_SESSION['consulta_financeira']['itens'], $_REQUEST['rastro'] );
$total = cfCalcularValorTotal( $itens );
$totalAgrupado = cfCalculaValorAgrupado( $itens );
$agrupadores = $_SESSION['consulta_financeira']['agrupadores'];

// rastro a ser exibido ( filtro desde o início até o item que está sendo exibido )
global $cfRastroBusca;

?>
<html>
	<head>
		<title><?= $_REQUEST['agrupador'] ?></title>
		<style>
			@media print { .notprint { display: none } }
			@media screen { .notscreen { display: none } }
			body{ font-family: arial; }
			span.imprimir { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.imprimir:hover { background-color: #d0d0d0; }
		</style>
		<script type="text/javascript">
			
			/**
			 * Altera a imagem a ser exibida.
			 *
			 * @param string
			 * @return void
			 */
			function alterarImagem( tipo )
			{
				var img = document.getElementById( 'grafico' );
				var div = document.getElementById( 'divGrafico' );
				if ( tipo == '' )
				{
					div.style.display = 'none';
					img.src = '';
					return;
				}
				var url = '/geral/geraGraficoSiof.php' +
					'?<?= cfMontarParametroRastroGrafico( $_REQUEST['rastro'] ) ?>' +
					'&tipo=' + escape( tipo );
				img.src = url;
				div.style.display = 'block';
			}
			
		</script>
	</head>
	<body>
		<span class="imprimir" onclick="window.print();"><img src="/imagens/print.gif"/></span>
		<table border="0" cellpadding="0" cellspacing="5" width="100%">
			<? if ( $_REQUEST['titulo'] ) : ?>
				<tr>
					<td align="center" style="font-size:9pt;">
						<b><?= $_REQUEST['titulo'] ?></b>
					</td>
				</tr>
			<? endif; ?>
			<? $contadorRastro = 0; ?>
			<? foreach ( $cfRastroBusca as $codRastro => $dscRastro ) : ?>
				<tr>
					<td align="center" style="font-size:9pt;">
						<?= $agrupadores[$contadorRastro] ?> <b><?= $codRastro ?></b><br/>
						<font color="#909090"><?= $dscRastro ?></font>
					</td>
				</tr>
				<? $contadorRastro++; ?>
			<? endforeach; ?>
			<tr>
				<td align="center">
					<div id="divGrafico" style="height:275px;width:550px;overflow:auto;">
						<img src="" id="grafico"/>
					</div>
				</td>
			</tr>
			<tr>
				<td align="center">
					<select name="tipo" onchange="alterarImagem( this.value );" class="notprint">
						<option value="" selected="selected">-- Escolha um tipo de gráfico --</option>
						<option value="pizza">Pizza</option>
						<option value="barra">Barra</option>
						<? //$ultimoAgrupador = array_key_exists( $contadorRastro, $agrupadores ) ? $agrupadores[$contadorRastro] : $agrupadores[$contadorRastro-1]; ?>
						<? if ( array_key_exists( $contadorRastro, $agrupadores ) == true ) : ?>
							<option value="acumulado">Acumulado por <?= $agrupadores[$contadorRastro] ?></option>
						<? endif; ?>
					</select>
				</td>
			</tr>
			<tr>
				<td>
					<br/>
					<table border="0" id="tabelaTitulos" align="center" style="font-size: 10pt;" cellpadding="3" cellspacing="0">
						<tr>
							<td width="120" align="center" style="border-bottom: 1px solid #909090;border-right: 1px solid #ababab;">Dotação disponível</td>
							<td width="120" align="center" style="border-bottom: 1px solid #909090;border-right: 1px solid #ababab;">Empenhado</td>
							<td width="120" align="center" style="border-bottom: 1px solid #909090;border-right: 1px solid #ababab;">Liquidado</td>
							<td width="120" align="center" style="border-bottom: 1px solid #909090;">Pago</td>
						</tr>
						<tr>
							<td align="center" style="border-right: 1px solid #ababab;"><? cfDesenhaValor( $total['autorizado_valor'] + $total['credito_adicional'] ); ?></td>
							<td align="center" style="border-right: 1px solid #ababab;"><? cfDesenhaValor( $total['empenhado'] ); ?></td>
							<td align="center" style="border-right: 1px solid #ababab;"><? cfDesenhaValor( $total['liquidado'] ); ?></td>
							<td align="center"><? cfDesenhaValor( $total['pago'] ); ?></td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		<script type="text/javascript">
			alterarImagem( '' );
		</script>
	</body>
</html>