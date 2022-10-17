<?
$objAcao = Acao::getAcaoPeloId( $acaoId );
if( @$_REQUEST['correcao'] == 1 )
{
	$objTarefa = new TarefaAcao();
	$objTarefa->reparaTarefas( $objAcao );
	AbstractEntity::updateAllChangedEntities();
	$db->commit();
	dbg( 'A Correção Foi Efetivada!' );
}
$arrTarefasPrincipais = $objAcao->getArrTarefasDaAcao();

?>
	<!--  comeco da lista tarefas do projeto -->
	
		<!--[if !IE]>-->
			<LINK REL="StyleSheet" HREF='../../includes/JsLibrary/css/tabelaTarefaNotIE.css' >
		<!--<![endif]-->
		<!--[if IE]>-->
			<LINK REL="StyleSheet" HREF='../../includes/JsLibrary/css/tabelaTarefaIE.css' >
		<!--<![endif]-->
		
		<script type="text/javascript" src="../../includes/JsLibrary/_start.js"></script>
		<script type="text/javascript" src="../../includes/JsBusiness/listaTarefas.js"></script>
	<body>
	
		<form method="POST"  name="formulario">
			<input type='hidden' name="modulo" value="<?=$modulo?>">
			<input type='hidden' name="ptoid" value=<?=$ptoid?>>
			<input type='hidden' name='exclui' value=0>
			<input type='hidden' name='acao' value=<?=$_REQUEST['acao']?>>
			<input type='hidden' name='act'>
			<input type='hidden' name='act2'>
			<input type='hidden' name='ptotipo' value='<?=$_REQUEST['ptotipo']?>'>
			<center>
				<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
				<?@$db -> cabecalho_acao($_SESSION['acaid']);?>
  					
					<?
												 
					// tabela das tarefas//
						require_once( APPRAIZ."monitora/modulos/principal/acao/plantrabacao_funcoes.inc" );
					?>
			<? if( $objAcao->getTipoCodigo() == 1 ): ?>
				<tr>
					<td align='right' class="SubTituloDireita">Período da Ação:</td>
					<td>
						<b> <?= $objAcao->getDataInicio() ?> &nbsp; - &nbsp; <?= $objAcao->getDataFim() ?></b>
					</td>
				</tr>
			<? endif ?>  
			<tr>
				<td align="right"  class="SubTituloDireita">
					GRÁFICO DE GANTT
				</td>
				<td>
					<input type="button" class='botao' 
					value='Ver Gráfico' title='Ver Gráfico.' 
					onclick="exibe_grafico( 'Acao' , <?= $objAcao->getId() ?> )">
					<select id="profunidadeNivel">
						<option value="1" > Apenas Nível 1 </option>
						<option value="2" > Até Nível 2 </option>
						<option value="3" > Até Nível 3 </option>
						<option value="4" > Até Nível 4 </option>
						<option value="1000" > Todos os Níveis </option>
					</select>     
				</td>
			</tr>
			<tr >
				<td   class="SubTituloDireita">
					<input type="button" value="Inserir <?= $objAcao->getElementoNome() ?>" 
					onclick="insertElement( this , 0, <?= $objAcao->getId() ?>, 'TarefaAcao' , function(){ window.location.reload() } )" />
				</td>
			</tr>
		</table>
	</center>
</div>
		
		<div id="EditarTarefa">
		</div>
		<center>
			<div id="listaTarefas" >
				<table border="0" class="tabelaTarefas">
					<thead>
						<tr>
							<th class="tarefaMais" >
								<img src="../../includes/JsLibrary/img/more.gif" />
							</th>
							<th class="tarefaOrdem">
								Or.
							</th>
							<th class="tarefaTipo">
								Tip
							</th>
							<th class="tarefaNome">
								Titulo
							</th>
							<th class="tarefaDescricao">
								Descri&ccedil;&atilde;o
							</th>
							<th class="tarefaInicio">
								In&iacute;cio
							</th>
							<th class="tarefaTermino">
								T&eacute;rmino
							</th>
							<th class="tarefaAprovada">
								Fec.
							</th>
							<th style="width: 13px;">
							</th>
						</tr>
					</thead>
					<tbody>
					</tbody>
				</table>
			</div>
			<div id="conteudoTarefas">
			<?php
			if ( sizeof( $arrTarefasPrincipais) > 0 ) 
			{
				geraTabelaTarefas( $arrTarefasPrincipais );
			}
			?>
			</div>
		</center>
		<script>
		function EdicaoJsIE( obj )
		{
			try
			{ 
				eval( obj.value ); 
				document.getElementById( 'divError' ).innerHTML = '';
			}
			catch( e )
			{
				document.getElementById( 'divError' ).innerHTML = e.message;
			}		
		}
		</script>

	</body>
	<script>
		if( !IE )
		{
			finishing();
		}
		else
		{
			setTimeout( finishing , 1000 );
		}
	</script>
</html>
