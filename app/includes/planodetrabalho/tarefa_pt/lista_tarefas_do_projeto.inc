<?
require_once( APPRAIZ . "includes/cabecalho.inc" );

$objProjeto = Projeto::getProjetoPeloId( $pjeid );
$arrTarefasPrincipais = $objProjeto->getArrTarefasDoProjeto();
$db->cria_aba($abacod_tela,$url,'');
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
