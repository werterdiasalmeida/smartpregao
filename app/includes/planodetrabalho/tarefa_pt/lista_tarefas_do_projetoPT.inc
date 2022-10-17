<?
require_once( APPRAIZ . "includes/cabecalho.inc" );
//$objProjeto = new Projeto();
$pjeid = $_SESSION['pjeid'];
$objProjeto = new projetoPT();
$objProjeto = $objProjeto->getProjetoPeloId( $pjeid );


function projetoaberto()
 {
 	 global $db;
	 
 	// verifica se o projeto esta aberto para ser acompanhado,	//
 	// ou seja, se ele nao esta concluido, cancelado etc.		//
 	
	
	$sql = ' SELECT ' .
				'tpscod' .
			' FROM ' .
				'monitora.projetoespecial' .
			' WHERE ' .
				'pjeid' . ' = ' . $_SESSION['pjeid'];
	$sit=$db->pegaUm($sql);
	if ($sit=='11' or $sit=='10')
	{
		// se o projeto estiver Fase de planejamento ou Fase de Execução então pode acompanhar
		return true;
	}
	else
	{
		return false;
	}
}



print '<br/>';
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Estruturar o Plano de Trabalho do Projeto Especial <img src="../imagens/grupo.gif" title="Ver equipe do projeto" onclick="mostra_equipe()">  ';
monta_titulo($titulo_modulo,'');

$arrTarefasPrincipais = $objProjeto->getArrTarefasDoProjeto();
?>

	<!--  comeco da lista tarefas do projeto -->
	
		<!--[if !IE]>-->
			<LINK REL="StyleSheet" HREF='../../includes/JsLibrary/css/tabelaTarefaNotIE.css' />
		<!--<![endif]-->
		<!--[if IE]>-->
			<LINK REL="StyleSheet" HREF='../../includes/JsLibrary/css/tabelaTarefaIE.css' />
		<!--<![endif]-->
		
		<script type="text/javascript" src="../../includes/JsLibrary/_start.js"></script>
		<script type="text/javascript" src="../../includes/JsBusiness/listaTarefas.js"></script>
	<body>
		<div id="EditarTarefa">
			<center>
				<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
					<tr>
						<td align='right' class="SubTituloDireita">Projeto :</td>
						<td><b> <?= $objProjeto->getCodigo() ?> &nbsp;-&nbsp; <?= $objProjeto->getNome() ?> </b></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita">Período do Projeto:</td>
						<td>
							<b> <?= $objProjeto->getDataInicio() ?> &nbsp; - &nbsp; <?= $objProjeto->getDataFim() ?></b>
						</td>
					</tr>  
					<tr>
						<td align="right"  class="SubTituloDireita">
							GRÁFICO DE GANTT
						</td>
						<td>
							<input type="button" class='botao' 
							value='Ver Gráfico' title='Ver Gráfico.' 
							onclick="exibe_grafico( 'Projeto' , <?= $objProjeto->getId() ?> )">
							<select id="profunidadeNivel">
								<option value="1" > Apenas Nível 1 </option>
								<option value="2" > Até Nível 2 </option>
								<option value="3" > Até Nível 3 </option>
								<option value="4" > Até Nível 4 </option>
								<option value="1000" > Todos os Níveis </option>
							</select>     
						</td>
					</tr>
					<? if ( $objProjeto->getProjetoAberto()): ?>
						<tr >
							<td   class="SubTituloDireita">
								<input type="submit" value="Inserir Tarefa" 
								onclick="insertElement( this , 0, <?= $objProjeto->getId() ?>, 'TarefaPT' , function(){ window.location.reload() } )" />
							</td>
						</tr>
					<? endif ?>
				</table>
			</center>
		</div>
		<center>
			<div id="listaTarefas" >
				<table border="0" class="tabelaTarefas">
					<thead>
						<tr>
							<th class="tarefaMais">
								Codigo
							</th>
							<th class="tarefaOrdem">
								Opções
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
							<th class="tarefaTipo">
								Predecessora
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
		function weee( obj )
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
		<!-- 
		<div style="width: 100%; height: 100px; position: absolute; bottom: 50px; left: 0px" >
			<textarea onclick="weee( this )" style="width:100%; height:100%"></textarea>
		</div>
		<div id="divError" style="width: 100%; height: 50px; position: absolute; bottom: 0px; left: 0px" >
		</div>
		 -->
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
