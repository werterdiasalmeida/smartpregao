<?php

	// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();

	// carrega dados referentes ao combo ( array definido na função combo_popup() )
	if ( !isset( $_SESSION['indice_sessao_texto_popup'][$_REQUEST['nome']] ) )
	{
		print '<html><script language="javascript"> alert( "Dados da sessão perdidos. Possivelmente sua sessão expirou." ); window.close(); </script></hmtl>';
		exit();
	}
	$dados_combo = $_SESSION['indice_sessao_texto_popup'][$_REQUEST['nome']];
	
	// variáveis da página
	$sql = $dados_combo['sql'];
	$titulo = $dados_combo['titulo'];
	$nome_popup = $_REQUEST['nome'];
	$registros = $db->carregar( $sql );
	if ( !$registros )
	{
		$registros = array();
	}

?>
<html>
	<head>
		<META http-equiv="Pragma" content="no-cache"/>
		<title><?= $titulo ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript">
			
			<!--
				
				window.focus();
				
				/**
				 * Nome do popup que está sendo manipulado.
				 * 
				 * @var string
				 */
				var nome_popup = '<?= str_replace( "'", "\\'", $nome_popup ) ?>';
				
				/**
				 * Objeto do campo texto da página origem.
				 * 
				 * @var object
				 */
				var campo_texto = window.opener.document.getElementById( nome_popup );
				
				/**
				 * Altera o valor do campo texto da janela origem.
				 * 
				 * @param string valor
				 * @return void
				 */
				function texto_popup_seleciona( valor, descricao )
				{
					<? if( $_REQUEST[ "label" ] ) : ?>
						window.opener.document.getElementById( ( 'label_popup_' + nome_popup ) ).innerHTML = descricao;
					<?endif;?>
					campo_texto.value = valor;
					campo_texto.focus();
					window.close();
				}
				
			-->
			
		</script>	
	</head>
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<?php
$linha = array();
if (is_array($registros)):
	foreach ($registros as $registro):
		$descricao = htmlentities( $registro['descricao'] );
		$valor = htmlentities( $registro['codigo'] );
		$onclick = 'javascript:combo_popup_altera_item( \'' . $registro['codigo'] . '\', \'' . $descricao . '\', this )';
		
		if ( $_REQUEST[ "mostraCodigo" ] ) :
			$linha[] = array("codigo"    => "<a href=\"javascript:texto_popup_seleciona( '{$valor}', '{$descricao}' );\">
												{$valor}
											 </a>",
							 "descricao" => "<a href=\"javascript:texto_popup_seleciona( '{$valor}', '{$descricao}' );\">
												{$descricao}
											 </a>"
							);
		else:
			$linha[] = array(
							 "descricao" => "<a href=\"javascript:texto_popup_seleciona( '{$valor}', '{$descricao}' );\">
												{$descricao}
											 </a>"
							);
		endif;
	endforeach;
endif;
/*
if ( $_REQUEST[ "mostraCodigo" ] )
	$cabecalho = array ("Cód.","Descrição");
else
	$cabecalho = array ("Descrição");
*/		
$db->monta_lista( $linha, $cabecalho, 50, 10, '', '' ,'' );

/*
		<form name="texto_popup">
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<? if ( count( $registros ) ) : ?>
					<? foreach ( $registros as $posicao => $registro ) : ?>
						<? $descricao = htmlentities( $registro['descricao'] ); ?>
						<? $valor = htmlentities( $registro['codigo'] ); ?>
						<? $onclick = 'javascript:combo_popup_altera_item( \'' . $registro['codigo'] . '\', \'' . $descricao . '\', this )'; ?>
						<tr bgcolor="#<?= $posicao % 2 == 0 ? 'f4f4f4' : 'e0e0e0' ?>">
						<? if ( $_REQUEST[ "mostraCodigo" ] ) : ?>
							<td>
								<a href="javascript:texto_popup_seleciona( '<?= $registro['codigo'] ?>', '<?= $descricao?>' );">
									<?= $valor ?>
								</a>
							</td>
						<? endif;?>
							<td>
								<a href="javascript:texto_popup_seleciona( '<?= $registro['codigo'] ?>', '<?= $descricao ?>' );">
									<?= $descricao ?>
								</a>
							</td>
						</tr>
					<? endforeach; ?>
				<? else : ?>
					<tr>
						<td colspan="2" align="center">Nenhum registro encontrado</td>
					</tr>
				<? endif; ?>
			</table>
		</form>
*/
?>
	</body>
</html>