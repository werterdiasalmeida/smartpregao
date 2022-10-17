<?php

	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
	// carrega dados referentes ao combo ( array definido na função combo_popup() )
	if ( !isset( $_SESSION['indice_sessao_combo_desembolso'][$_REQUEST['nome']] ) )
	{
		print '<html><script language="javascript"> alert( "Dados da sessão perdidos. Possivelmente sua sessão expirou." ); window.close(); </script></hmtl>';
		exit();
	}
	$dados_combo = $_SESSION['indice_sessao_combo_desembolso'][$_REQUEST['nome']];
	
	// variáveis da página
	$titulo = $dados_combo['titulo'];
	$nome_popup = $_REQUEST['nome'];
	$totalRegistros = $_REQUEST[ 'totalRegistros' ];
	$saldo = $_REQUEST[ 'saldo' ];
	$coordpje=false;
	$digit=false;

	if ($db->testa_responsavel_projespec($_SESSION['pjeid'])) $coordpje = true;
	// verifica se é digitador
	if ($db->testa_digitador($_SESSION['pjeid'],'E')) $digit = true;
	// verific se é super-usuário
	if ($db->testa_superuser())   {
		$coordpje = true;
		$digit = true;
	}
?>
<html>
	<head>
		<META http-equiv="Pragma" content="no-cache"/>
		<title><?= $titulo ?><?= $maximo != 0 ? ' - Ecolha no máximo ' . $maximo . ' itens' : '' ; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script language="JavaScript" src="../../includes/calendario.js"></script>
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
				* Armaneza o valor que ainda pode ser manipulado na previsão de desembolso
				*
				* @var float
				*/
				var saldo = parseInt( <?= $_REQUEST[ 'saldo' ] ?> );
				var totalRegistros = <?= $totalRegistros ?>;
				
				/**
				* Monta os registros de acordo com os dados preenchidos no formulário principal
				* 
				* @return void
				*/
				function montaRegistros()
				{					
					optionsAux = new Array();
					totalRegistros = 0;
					for( var i = 0 ; i < campo_select.options.length ; i++ )
					{
						optionsAux.push( {value: campo_select.options[ i ].value} )
					}
					arrAux = new Array();
					for( var i = 0 ; i < optionsAux.length ; i++ )
					{						
						arrAux = optionsAux[ i ].value.split( ' - ' );
						data = arrAux[ 0 ];
						valor = parseInt( arrAux[ 1 ] );
						criaElemento( data, valor, true );
					}
				}

				/**
				* Cria um novo registro verificando se nao ultrapassa o saldo ou iguala o saldo.
				* 
				* @return void
				*/
				function incluiPrevisao()
				{
					data = document.formulario.dataPrevisao.value;
					valor = parseInt( document.formulario.valor.value );
					if( !isNaN( valor ) && data != '' )
					{
						criaElemento( data, valor, true );
					}
					else
					{
						alert( 'Preencha os campos de data e valor corretamente' );
					}
				}		
				
						
				function criaElemento( data, valor, atualizaSaldo )
				{
					if( validaData( data ) )
					{						
						if( saldo - valor > 0 )
						{
							saldo = atualizaSaldo ? saldo - valor : saldo;
							if( window.opener.disponivel > 0 )
							{
								window.opener.disponivel = saldo;
							}
							criaLinha( data, valor.toString() );
							atualizaColunaSaldo();
						}
						else if( saldo - valor < 0 && atualizaSaldo )
						{
							alert( 'O valor ultrapassa o saldo.' );
						}													
						else
						{
							saldo = atualizaSaldo ? 0 : saldo;
							if( window.opener.disponivel > 0 )
							{
								window.opener.disponivel -= valor;
							}
							criaLinha( data, valor.toString() );
							atualizaColunaSaldo();
						}
					}
					else
					{
						alert( 'Já existe uma previsão de desembolso para esta data.' );
					}
				}
				function criaLinha( data, valor )
				{
					tabela = document.getElementById( 'registros' );
					contador = checaRegistroMaior( data, tabela );
					linha = tabela.insertRow( contador );
					linha.setAttribute( 'id', 'linha' + totalRegistros );					
					check = linha.insertCell( 0 );
					check.setAttribute( 'width', 20 );
					check.innerHTML = '<input type="checkbox" name="check_combo_desembolso" id="check' + totalRegistros + '" checked="checked" value="'+ data + ' - ' + valor + '" onclick="retiraLinha( ' + totalRegistros + ', ' + valor + ' );" />';
					texto = linha.insertCell( 1 );
					texto.style.paddingLeft = '2px';
					texto.innerHTML = data + ' - R$ ' + valor;
					totalRegistros++;
					atualizaLista();
					
				}
				function validaData( data )
				{
					tabela = document.getElementById( 'registros' );
					totalLinhas = tabela.getElementsByTagName( 'tr' ).length;
					for( var i = 0 ; i < totalLinhas ; i++ )
					{
						linha = tabela.getElementsByTagName( 'tr' )[ i ];
						valor = linha.getElementsByTagName( 'td' )[ 0 ].getElementsByTagName( 'input' )[ 0 ].value;
						if( valor.indexOf( data ) === 0 )
							return false;
					}
					return true;
				}
				
				function atualizaLista()
				{
					tabela = document.getElementById( 'registros' );
					totalLinhas = tabela.getElementsByTagName( 'tr' ).length;
					combo = campo_select;
					totalCampo = combo.options.length;
					while( combo.options.length )
					{
						combo.options[ 0 ] = null;
					}
					for( var i = 0 ; i < totalLinhas ; i++ )
					{
						linha = tabela.getElementsByTagName( 'tr' )[ i ];
						cor = i % 2 == 0 ? '#f4f4f4' : '#e0e0e0';
						linha.style.background = cor;
						linha.ordem = i;
						valor = linha.getElementsByTagName( 'td' )[ 0 ].getElementsByTagName( 'input' )[ 0 ].value;
						label = linha.getElementsByTagName( 'td' )[ 1 ].innerHTML;
						combo.options[ i ] = new Option( label, valor, false, false );
					}
					if( combo.options.length == 0 )
					{
						combo.options[0] = new Option( 'Clique Aqui para Selecionar', '', false, false );
					}
				}
				
				function retiraLinha( numLinha, valor )
				{
					tabela = document.getElementById( 'registros' );
					linha = document.getElementById( 'linha' + numLinha ).ordem;
					saldo += parseFloat( valor );
					window.opener.disponivel += parseFloat( valor );
					tabela.deleteRow( linha );
					atualizaColunaSaldo();
					atualizaLista();
				}
				
				function checaRegistroMaior( data, tabela )
				{
					total = tabela.getElementsByTagName( 'tr' ).length;
					for( var i = 0 ; i < total ; i++ )
					{
						linha = tabela.getElementsByTagName( 'tr' )[ i ]
						coluna = linha.getElementsByTagName( 'td' )[ 1 ];
						auxData = coluna.innerHTML.split( ' - ' )[ 0 ].split( '/' );
						dataLinha = auxData[ 2 ] + auxData[ 1 ] + auxData[ 0 ];
						auxData = data.split( '/' );
						dataCampo = auxData[ 2 ] + auxData[ 1 ] + auxData[ 0 ];
						if( dataLinha > dataCampo )
						{
							return i;
						}
					}
					return total;
				}
				
				
				function atualizaColunaSaldo()
				{
					colunaSaldo = document.getElementById( 'colunaSaldo' );
					colunaSaldo.innerHTML = 'R$ ' + formataNumero( saldo );
					
					if( saldo == 0 )
					{
						colunaSaldo.style.color = '#ff0000';
					}
					else
					{
						colunaSaldo.style.color = '#000000';
					}
				}
				
				function formataNumero( numero )
				{
					numero = numero.toString();
					if( numero.length > 3 )
					{
						aux = 0;
						aux2 = '999';
						while( aux2.length >= 3 )
						{
							aux1 = numero.substring( ( numero.length - ( 3 + aux ) ), numero.length );
							aux2 = numero.substring( 0, ( numero.length - ( 3 + aux ) ) );
							aux += 4;
							if( aux2.length > 0 )
							{
								numero = aux2 + '.' + aux1;
							}
						}
					}
					numero += ',00';
					return numero;
				}
			-->
			
		</script>
		<style type="text/css">
			body{margin:0; padding:0;}
		</style>
	</head>
	<body>
		<form name="formulario">
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="0" class="listagem">
				<? if( $saldo >= 0 ) : ?>
					<tr bgcolor="#cdcdcd" id="criaPrevisao">
						<td colspan="2">
							<table cellpadding="0" cellspacing="0" border="0">
								<tr>
									<td colspan="2" style="border:none;"><strong>Inserir nova previsão</strong></td>
								</tr>
								
								<tr style="border:1px solid #f5f5f5;">
									<td style="border:1px solid #f5f5f5; background:#dcdcdc;padding:0 2px;" align="right" width="90">Data:</td>
									<td style="border:1px solid #f5f5f5;background:#f5f5f5;" width="310"><?=campo_data('dataPrevisao', 'S','S','','S');?></td>
								</tr>
								<tr>
									<td style="border:1px solid #f5f5f5; background:#dcdcdc;padding:0 2px;" align="right">Valor:</td>
									<td style="border:1px solid #f5f5f5;background:#f5f5f5;" width="250"><?=campo_texto('valor','S','S','',16,12,'##############','');?>(valroes em reais inteiros)</td>
								</tr>
				<? if ($coordpje or $digitador) {	?>			
				<tr>
									<td style="border:1px solid #f5f5f5;background:#dcdcdc;padding:0 2px;">&nbsp;</td>
				
									<td align="left" style="border:1px solid #f5f5f5;background:#f5f5f5;padding:2px 0"><input type="button" value="inserir" onclick="incluiPrevisao();" /></td>
								</tr>
				<?}?>
								<tr>
									<td style="border:1px solid #f5f5f5;background:#cdcdcd;" align="right">Saldo disponível:</td>
									<td id="colunaSaldo" style="border:1px solid #f5f5f5;background:#cdcdcd;padding-left:2px;">R$ <?= number_format($saldo,2,',','.');?></td>
								<tr>
							</table>
						</td>
					</tr>
				<? else : ?>
					<tr bgcolor="#cdcdcd">
						<td style="color:#ff0000;" colspan="2">Saldo insuficiente para inserção de novas previsões de desembolso</td>
					</tr>					
				<? endif; ?>
				<tr>
					<td colspan="2">
						<table cellspacing="0" cellpadding="0" border="0" id="registros" width="100%">
							<? if( $totalRegistros > 0 ) : ?>
								<script>montaRegistros(); </script>
							<? endif; ?>
						</table>
					</td>
				</tr>			
			</table>
		</form>
	</body>
	</html>