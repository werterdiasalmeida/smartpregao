<?php

	// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	
	if($_SESSION['indice_sessao_combo_popup'][$_REQUEST['nome']]["param_conexao"]) {
		// Novos parâmetros de conexão
		$servidor_bd = $_SESSION['indice_sessao_combo_popup'][$_REQUEST['nome']]["param_conexao"]["servidor_bd"];
		$porta_bd    = $_SESSION['indice_sessao_combo_popup'][$_REQUEST['nome']]["param_conexao"]["porta_bd"];
		$nome_bd     = $_SESSION['indice_sessao_combo_popup'][$_REQUEST['nome']]["param_conexao"]["nome_bd"];
		$usuario_db  = $_SESSION['indice_sessao_combo_popup'][$_REQUEST['nome']]["param_conexao"]["usuario_db"];
		$senha_bd    = $_SESSION['indice_sessao_combo_popup'][$_REQUEST['nome']]["param_conexao"]["senha_bd"];
	}

	$db = new cls_banco();

	// desabilita cache do navegador
	// necessário quando um popup como mesmo nome é chamado em telas diferentes
	/*header( "Cache-Control: no-store, no-cache, must-revalidate" );
	header( "Cache-Control: post-check=0, pre-check=0", false );
	header( "Cache-control: private, no-cache" );
	header( "Last-Modified: " . gmdate( "D, d M Y H:i:s" ) . " GMT" );
	header( "Pragma: no-cache" );*/
	//controla cache de navegação
	header( "Connection: Keep-Alive" );
	header("Cache-Control: no-store, no-cache, must-revalidate");
	header("Cache-Control: post-check=0, pre-check=0", false);
	header("Cache-control: private, no-cache");
	header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
	header("Pragma: no-cache");
	//header( "Content-type: text/plain; charset=iso-8859-1" );

	// carrega dados referentes ao combo ( array definido na função combo_popup() )
	if ( !isset( $_SESSION['indice_sessao_combo_popup'][$_REQUEST['nome']] ) )
	{
		print '<html><script language="javascript"> alert( "Dados da sessão perdidos. Possivelmente sua sessão expirou." ); window.close(); </script></hmtl>';
		exit();
	}
	$dados_combo = $_SESSION['indice_sessao_combo_popup'][$_REQUEST['nome']];
		
	// variáveis da página
	$sql = $dados_combo['sql'];
	
	$titulo 	   = $dados_combo['titulo'];
	$maximo 	   = $dados_combo['maximo'];
	$codigos_fixos = $dados_combo['codigos_fixos'];
	$mensagem_fixo = $dados_combo['mensagem_fixo'];
	$nome_popup    = $_REQUEST['nome'];
	$whereArr	   = $dados_combo['where'];
	$mostraPesquisa	= $dados_combo['mostraPesquisa'];
	$mostraPesquisaTemp	= $dados_combo['mostraPesquisa'];
	$intervalo	   = $dados_combo['intervalo'];
	$arrVisivel	   = $dados_combo['arrVisivel'];
	$arrOrdem	   = $dados_combo['arrOrdem'];
	$htmlReplace	= $dados_combo['htmlReplace'];
	
	if(!is_array($arrVisivel)){
		$arrVisivel = array($arrVisivel);
	}
	if(!is_array($arrVisivel)){
		$arrOrdem = array($arrOrdem);
	}
	
	if ( is_array($_POST['filtro']) ){
		foreach ($_POST['filtro'] as $k => $val):
			${'filtro['.$k.']'} = $val;		
			if (trim($val) != ''){
				$where[] = $_POST['numeric'][$k] ? "$k = '".intval($val)."'" : "upper($k) like upper('%$val%')"; 
			}
		endforeach;

		if ($where && strpos(strtolower($sql), 'where')){
			$sql = str_replace( 'where', 'where ' . implode(' AND ',$where) .  ' AND ', strtolower($sql) );
			$mostraPesquisaTemp = true;
		}elseif ($where && strpos(strtolower($sql), 'group by')){ 
			$sql = str_replace( 'group by', 'where ' . implode(' AND ',$where) . ' group by ', strtolower($sql) );
			$mostraPesquisaTemp = true;
		}elseif ($where && strpos(strtolower($sql), 'order by')){ 
			$sql = str_replace( 'order by', 'where ' . implode(' AND ',$where) . ' order by ', strtolower($sql) );			
			$mostraPesquisaTemp = true;
		}else{
			if(!$mostraPesquisa){
				$mostraPesquisaTemp = false;			
			}
		}
		
	}
	
	// verifica se a requisição é para exibir o nome e codigo de um item
	// o codigo é impresso na primeira linha, a descrição é exibido na linha seguinte
	if ( isset( $_REQUEST['pegar_dados_item'] ) == true )
	{
		header( "Content-type: text/plain; charset=iso-8859-1" );
		$sql = strtolower($sql);
		$cod = $_REQUEST['codigo_busca'];
		// pega nome do campo que determina codigo
		$nomeCampoDescricao = '';
		$posicaoSelect = strpos( $sql, 'select' );
		if ( $posicaoSelect === false )
		{
			$posicaoSelect = strpos( $sql, 'SELECT' );
		}
		$posicaoSelect += 6;
		$nomeCampoDescricao = trim( substr( $sql, $posicaoSelect, strpos( $sql, 'as codigo' ) - $posicaoSelect ) );

		$where = " " . $nomeCampoDescricao . " = '" . $cod . "' ";
		
		#removendo distinct desnecessário
		$where = str_replace('distinct', '', $where);
		
		if ( strpos( strtolower($sql), 'where' ) !== false )
		{	
			$sql = str_replace( 'where', 'where ' . $where . ' and ', strtolower($sql) );
		}
		else if ( strpos( strtolower($sql), 'group by' ) !== false )
		{
			$sql = str_replace( 'group by', 'where ' . $where . ' group by ', strtolower($sql) );
		}
		else if ( strpos( strtolower($sql), 'order by' ) !== false )
		{
			$sql = str_replace( 'order by', 'where ' . $where . ' order by ', strtolower($sql) );
		}
		else {
			$sql .= ' '.$where;
		}
		
		#alteração feita por wesley romualdo
		#não estava conseguindo consultar por codigo
		#alterei esta linha: $dados = $db->carregar( $sql ); para esta $dados = $db->carregar( strtoupper($sql) );
		$dados = $db->carregar( strtoupper($sql) );
		#fim da alteração feita por wesley romualdo
		if ( $dados )
		{
			print $dados[0]['codigo'] . "\n" . $dados[0]['descricao'];
		}
		exit();
	}
	
	if($mostraPesquisaTemp){
		$registros = $db->carregar( $sql );
	}
	
	$codigoNoNome = false;	// indica se o nome contém o código
							// necessário para querys que colocam o código na descrição
	if ( !$registros )
	{
		$registros = array();
	}
	else
	{
		$codigoNoNome = strpos( $registros[0]['descricao'], '-' ) !== false;
	}
	
	$_REQUEST['ordemCampo'] = !$_REQUEST['ordemCampo'] && is_array($arrOrdem) ? "dsc" : $_REQUEST['ordemCampo'];
	
	$ordemCampo = $_REQUEST['ordemCampo'] == 'dsc' ? 'dsc' : 'cod' ;
	$ordemDirecao = $_REQUEST['ordemDirecao'] == 'd' ? 'd' : 'a' ;
	$registrosOrdenados = array();
	foreach ( $registros as $i => $item )
	{
		if ( !$item['descricao'] )
		{
			continue;
		}
		//$chave = $ordemCampo == 'cod' ? $item['codigo'] : $item['descricao'];
		if ( $ordemCampo == 'dsc' )
		{
			if ( strpos( $item['descricao'], ' - ' ) )
			{
				$chave = trim ( substr( $item['descricao'], strpos( $item['descricao'], '-' ) + 1 ) );
			}
			else
			{
				$chave = $item['descricao'];
			}
		}
		else
		{
			if ( strpos( $item['descricao'], ' - ' ) )
			{
				$chave = trim ( substr( $item['descricao'], 0, strpos( $item['descricao'], '-' ) - 1 ) );
			}
			else
			{
				$chave = $item['codigo'];
			}
		}
		/*
		$chave = $ordemCampo == 'cod' ? $item['codigo'] : $item['descricao'];
		if ( $ordemCampo == 'dsc' && $codigoNoNome )
		{
			$chave = trim ( substr( $chave, strpos( $chave, '-' ) + 1 ) );
		}
		*/
		$registrosOrdenados[$chave . $item['codigo']] = $item;
	}
	$ordemDirecao == 'a' ? ksort( $registrosOrdenados ) : krsort( $registrosOrdenados );
	$registros = $registrosOrdenados;
	
?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title><?= $titulo ?><?= $maximo != 0 ? ' - Ecolha no máximo ' . $maximo . ' itens' : '' ; ?></title>
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
				 * Objeto do campo select da página origem. Variável global utilizada
				 * pelas funções de manipulação da listagem e do combo.
				 * 
				 * @var object
				 */
				var campo_select = window.opener.document.getElementById( nome_popup );
				
				/**
				 * Quantidade de itens selecionados.
				 * 
				 * @var integer
				 */
				var itens_selecionados = 0;
				
				/**
				 * Quantidade máxima de itens que podem ser selecionados.
				 * 
				 * @var integer
				 */
				var maximo = <?= $maximo ?>;
				
				/**
				 * Códigos que não podem ser removidos do combo.
				 * 
				 * @var integer
				 */
				var codigos_fixos = new Array();
				<? foreach ( $codigos_fixos as $codigo_fixo ) : ?>
					<? if ( $codigo_fixo ) : ?>
						codigos_fixos[codigos_fixos.length] = '<?= str_replace( "'", "\\'", $codigo_fixo ) ?>';
					<? endif; ?>
				<? endforeach; ?>
				
				/**
				 * Mensagem exibida quando o usuário tenta remover um item fixo.
				 * 
				 * @var integer
				 */
				var mensagem_fixo = '<?= str_replace( "'", "\\'", $mensagem_fixo ) ?>';
				
				/**
				 * Adiciona ou remove um item do campo select da página origem
				 * 
				 * @param string cod
				 * @param string descricao
				 * @param object checkbox
				 * @return void
				 */
				function combo_popup_altera_item( cod, descricao, checkbox )
				{
					if ( checkbox.checked == true )
					{
						var atingiu_maximo = false;
						// verifica se há limite de itens que podem ser selecionados
						// verifica se limite foi ultrapassado
						if ( maximo != 0 && itens_selecionados >= maximo )
						{
							checkbox.checked = false;
							atingiu_maximo = true;
							alert( 'A quantidade máxima de itens que podem ser selecionados é <?= $maximo ?>.' );
						}
						if ( atingiu_maximo == false )
						{
							itens_selecionados++;
							window.opener.combo_popup_adicionar_item( nome_popup, cod, descricao, true );
						}
					}
					else
					{
						if ( itens_selecionados > 0 )
						{
							itens_selecionados--;
						}
						var fixo = false;
						var j = codigos_fixos.length;
						for ( var i = 0; i < j; i++ )
						{
							if ( codigos_fixos[i] == cod )
							{
								checkbox.checked = true;
								fixo = true;
								break;
							}
						}
						if ( fixo == false )
						{
							window.opener.combo_popup_remover_item( nome_popup, cod, true );
						}
					}
					combo_popup_atualiza_checkbox_geral();
					if ( fixo )
					{
						alert( mensagem_fixo );
					}
				}
				
				/**
				 * Altera o status de todos os checkbox da tela de acordo com o
				 * status do checkbox_geral
				 * 
				 * @return void
				 */
				function combo_popup_atualiza_checkbox_geral()
				{
					// caso haja limite de itens que podem ser selecionados a ação é cancelada
					if ( maximo != 0 )
					{
						return;
					}
					var novo_status = true;
					var lista = document.combo_popup.combo_popup_checkbox;
					var j = lista.length;
					for ( var i = 0; i < j; i++ )
					{
						if ( lista[i].checked == false )
						{
							novo_status = false;
							break;
						}
					}
					document.combo_popup.combo_popup_check_geral_0.checked = novo_status;
					document.combo_popup.combo_popup_check_geral_1.checked = novo_status;
				}
				
				/**
				 * Altera o status do checkbox_geral a partir dos status de todos
				 * os checkbox da tela
				 * 
				 * @param integer check_geral
				 * @return void
				 */
				function combo_popup_altera_checkbox_geral( check_geral, funcao )
				{
					// caso haja limite de itens que podem ser selecionados a ação é cancelada
					if ( maximo != 0 )
					{
						return;
					}
					var lista = document.combo_popup.combo_popup_checkbox;
					if ( check_geral == 0 )
					{
						var status = document.combo_popup.combo_popup_check_geral_0.checked;
						document.combo_popup.combo_popup_check_geral_1.checked = status;
					}
					else
					{
						var status = document.combo_popup.combo_popup_check_geral_1.checked;
						document.combo_popup.combo_popup_check_geral_0.checked = status;
					}
					var dados = new Array();
					var j = lista.length;

					if (typeof j != 'number'){
						j = 1;
						lista = new Array(lista);
					}
					
					var h = 0;
					var fixo = false;
					var algum_fixo = false;
					for ( var i = 0; i < j; i++ )
					{
						if ( status != lista[i].checked )
						{
							fixo = false;
							if ( status == false )
							{
								var k = codigos_fixos.length;
								for ( var l = 0; l < k; l++ )
								{
									if ( codigos_fixos[l] == lista[i].value )
									{
										lista[i].checked = true;
										fixo = true;
										algum_fixo = true;
										break;
									}
								}
							}
							if ( fixo == false )
							{
								lista[i].checked = status;
								dados[h++] = new Array( lista[i].value, lista[i].getAttribute( 'descricao' ) );
							}
						}

						if(funcao != "")
						{
							window.opener.eval(funcao)(lista[i]);
						}
					}
					if ( status == true )
					{
						window.opener.combo_popup_adicionar_itens( nome_popup, dados );
					}
					else
					{
						window.opener.combo_popup_remover_itens( nome_popup, dados );
					}
					if ( algum_fixo == true )
					{
						combo_popup_atualiza_checkbox_geral();
						alert( mensagem_fixo );
					}
				}
				function combo_popup_altera_checkbox_geral_intervalo( check_geral, funcao ) //Selecionar um intervalo
				{
					// caso haja limite de itens que podem ser selecionados a ação é cancelada
					if ( maximo != 0 )
					{
						return;
					}
					var lista = document.combo_popup.combo_popup_checkbox;

					if ( check_geral == 0 )
					{
						var status = document.combo_popup.combo_popup_check_geral_intervalo_0.checked;
					}

					var dados = new Array();
					var j = lista.length;
					var h = 0;
					var fixo = false;
					var inicial = false;
					var Iinicial = 0;
					var final = false;
					var Ifinal = 0;
					var testa = 0;
					for ( var i = 0; i < j; i++ )
					{
						if( status == true ){
							if( lista[i].checked ){ //verifica itens marcados
								inicial = true;
								Iinicial = i;
								if( testa == 1 ){
									final = true;
									Ifinal = i;
								}
								testa = 1;
							}
							
							
							if ( status != lista[i].checked )
							{
								fixo = false;
								if ( fixo == false )
								{
									if( inicial == true && final == false){
										lista[i].checked = status;
										dados[h++] = new Array( lista[i].value, lista[i].getAttribute( 'descricao' ) );
									}
								}
							}
						} else	if( status == false ){
							inicial = true;
							lista[i].checked = false;
							dados[h++] = new Array( lista[i].value, lista[i].getAttribute( 'descricao' ) );
						}

						if(funcao != "")
						{
							window.opener.eval(funcao)(lista[i]);
						}
					}
					if( inicial == false ){
						alert('Selecione um Intervalo');
						document.combo_popup.combo_popup_check_geral_intervalo_0.checked = false;
					}
					if ( status == true )
					{
						window.opener.combo_popup_adicionar_itens( nome_popup, dados );
					}
					else
					{
						window.opener.combo_popup_remover_itens( nome_popup, dados );
					}
				}
				
			-->
			
		</script>
	</head>
	<?php 
						
	if($_REQUEST["funcao"])
	{
		$funcaoJS = 'window.opener.' . $_REQUEST["funcao"] . '(this);';
	}
	else
	{
		$funcaoJS = '';
	}
	
	?>
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<?php echo monta_busta( $whereArr );?>	
		<form name="combo_popup">
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<tr bgcolor="#cdcdcd">
					<td width="1" style="text-align: center;">
						<? if ( $maximo == 0 && count( $registros ) ) : ?>
							<input type="checkbox" name="combo_popup_check_geral_0" type="checkbox" value="" onclick="javascript:combo_popup_altera_checkbox_geral( 0 , '<?=$_REQUEST["funcao"]?>' );"/>
						<? else : ?>-<? endif; ?>
					</td>
					<td colspan="2" valign="top">
						<strong><?= $titulo ?></strong>
						<?= $maximo != 0 ? ' (máximo ' . $maximo . ' itens) ' : '' ; ?>
					</td>
				</tr>
				<?php if( $intervalo ){ ?>	
					<tr bgcolor="#cdcdcd">
						<td width="1" style="text-align: center;">
							<? if ( $maximo == 0 && count( $registros ) ) : ?>
								<input type="checkbox" name="combo_popup_check_geral_intervalo_0" type="checkbox" value="" onclick="javascript:combo_popup_altera_checkbox_geral_intervalo( 0 , '<?=$_REQUEST["funcao"]?>' );"/>
							<? else : ?>-<? endif; ?>
						</td>
						<td colspan="2" valign="top">
							<strong>Selecione o Intervalo</strong>
							<?= $maximo != 0 ? ' (máximo ' . $maximo . ' itens) ' : '' ; ?>
						</td>
					</tr>
				<?php } ?>	
				<tr bgcolor="#dcdcdc">
					<td width="1" style="text-align: center;">&nbsp;</td>
					<?php if($arrVisivel[0] == null || ( is_array($arrVisivel) && in_array("codigo",$arrVisivel) ) ): ?>
						<td width="20%" style="text-align: center;">
							<? $ordemUrl = '?nome=' . urlencode( $nome_popup ) . '&ordemCampo=cod&ordemDirecao=' ; ?>
							<a href="<?= $ordemUrl ?><?= $ordemDirecao == 'd' || $ordemCampo == 'dsc' ? 'a' : 'd' ?>">
								Código
							</a>
							<? if ( $ordemCampo == 'cod' ) : ?>
								<img src="/imagens/seta_ordem<?= $ordemDirecao == 'a' ? 'ASC' : 'DESC' ?>.gif" align="absmiddle"/>
							<? endif; ?>
						</td>
						<?php $colspan = 1; $percent_width = "80%" ?>
					<?php else: ?>
						<?php $colspan = 2; $percent_width = "100%" ?>
					<?php endif; ?>
					<td width="<?php echo $percent_width ?>" colspan="<?php echo $colspan ?>" style="text-align: center;">
						<? $ordemUrl = '?nome=' . urlencode( $nome_popup ) . '&ordemCampo=dsc&ordemDirecao=' ; ?>
						<a href="<?= $ordemUrl ?><?= $ordemDirecao == 'd' || $ordemCampo == 'cod' ? 'a' : 'd' ?>">
							Descrição
						</a>
						<? if ( $ordemCampo == 'dsc' ) : ?>
							<img src="/imagens/seta_ordem<?= $ordemDirecao == 'a' ? 'ASC' : 'DESC' ?>.gif" align="absmiddle"/>
						<? endif; ?>
					</td>
				</tr>
				<? if ( count( $registros ) ) : ?>
					<? $posicao = 0; ?>
					<? foreach ( $registros as $registro ) : 
						if ($htmlReplace){
							if (!is_array($arraySearchHTML)){
								$arraySearchHTML = array_keys($registro);
								$arraySearchHTML = '{'.implode('},{', $arraySearchHTML).'}';
								$arraySearchHTML = explode(',', $arraySearchHTML);
							}
							$arrayReplaceHTML = array_values($registro);
							$htmlDescricao = str_replace($arraySearchHTML, $arrayReplaceHTML, $htmlReplace);
						}else{
							$htmlDescricao = trim($registro['descricao'] );
						}
						$codigo = htmlentities( trim($registro['codigo'] )); 
						$descricao = htmlentities( trim($registro['descricao'] )); 
						$descricao = str_replace("'", "\'", $descricao);
						//$descricao =  $registro['descricao'] ;
						//$arrVisivel
						//$arrOrdem ?>
						<? $onclick = 'javascript:combo_popup_altera_item( \'' . $registro['codigo'] . '\', \'' . $descricao . '\', this ); '.$funcaoJS; ?>
						<tr bgcolor="#<?= $posicao % 2 == 0 ? 'f4f4f4' : 'e0e0e0' ?>">
							<td width="1">
								<input descricao="<?= $descricao ?>" name="combo_popup_checkbox" type="checkbox" id="combo_popup_checkbox_<?= $registro['codigo'] ?>" value="<?= $registro['codigo'] ?>" onclick="<?= $onclick ?>"/>
							</td>
							<td colspan="1">
								<?= $codigo ?>
							</td>
							<td colspan="1">
								<?= $htmlDescricao ?>
							</td>
						</tr>
						<? $posicao++; ?>
					<? endforeach; ?>
				<? else : ?>
					<tr>
						<td colspan="3" align="center">Nenhum registro encontrado</td>
					</tr>
				<? endif; ?>
				<tr bgcolor="#cdcdcd">
					<td width="1" style="text-align: center;">
						<? if ( $maximo == 0 && count( $registros ) ) : ?>
							<input type="checkbox" name="combo_popup_check_geral_1" type="checkbox" value="" onclick="javascript:combo_popup_altera_checkbox_geral( 1, '<?=$_REQUEST["funcao"]?>' );"/>
						<? else : ?>-<? endif; ?>
					</td>
					<td colspan="2" valign="top" class="cabecalho">
						<strong><?= $titulo ?></strong>
						<?= $maximo != 0 ? ' (máximo ' . $maximo . ' itens) ' : '' ; ?>
						<input type="button" name="ok" value="Ok" onclick="self.close();">
					</td>
				</tr>
			</table>
		</form>
		<script type="text/javascript">
			
			<!--
				
				// faz ficar 'selecionado' os itens da lista que estão no select da página origem.
				if ( campo_select.options[0].value != '' )
				{
					var j = campo_select.options.length;
					for ( var i = 0; i < j; i++ )
					{
						if( document.getElementById( 'combo_popup_checkbox_' + campo_select.options[i].value ) ){
							document.getElementById( 'combo_popup_checkbox_' + campo_select.options[i].value ).checked = true;
						}
						itens_selecionados++;
						if ( maximo != 0 && itens_selecionados >= maximo )
						{
							break;
						}
					}
				}
				
				combo_popup_atualiza_checkbox_geral();
				
			-->
			
		</script>
	</body>
</html>
<?php 
function monta_busta( $array = array() ){
	global $titulo;
	
	$form = '';
	if (is_array($array)){
		$form = '<form action="" method="post" name="form_filtro">
				 <table  class="tabela" style="width:100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr bgcolor="#cdcdcd">
						<td align="center" width="100%" colspan="2" height="25" ><label style="color: rgb(0, 0, 0); font-size:12px;" class="TituloTela">'.$titulo.'</label></td>
					</tr>';			
		
		foreach ($array as $k):
				global ${$k['codigo']}; 
				$form .= '<tr>
							<td class="SubTituloDireita">'.$k['descricao'].'</td>
							<td>
								'.campo_texto('filtro['.$k['codigo'].']','','','',30,100, ($k['numeric'] ? '###################################################' : '' ) ,'').'
								<input type="hidden" name="numeric['.$k['codigo'].']" value="'.$k['numeric'].'">
							</td>
						  </tr>';
		endforeach;
		
		$form .= '	<tr bgcolor="#cdcdcd">
						<td height="20">&nbsp;</td>
						<td><input type="submit" value="Filtrar"></td>
					  </tr>
				  </table>
				  </form>';
	}
	return $form;
}
?>