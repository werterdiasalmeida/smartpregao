<?php

function geraTabelaAtividades( $arrTarefasQueContenho , $boolAjaxMode = false )
{
	$arrTarefasQueContenho = orderArrayOfObjectsByMethod( $arrTarefasQueContenho , 'getCodigoUnico' );
	?>
		<script>
			window.serverSideClassName = "<?= get_class( @$arrTarefasQueContenho[0] ) ?>";
		</script>	
			<table border="0" class="tabelaTarefas" id="tabelaTarefas">
				<tbody>
					<?php foreach ( $arrTarefasQueContenho as $intPosicao => $objTarefa ) 
					{
					?>
						<tr class="tarefasFilhasAberto <?= ( ($intPosicao % 2 ) ? 'Par' : 'Impar' ) ?> <?= ( ($objTarefa->getQuantidadeDeTarefasFilhas() > 0 ) ? 'MacroEtapa' : 'Etapa' ) ?>" id="tr<?= $objTarefa->getId() ?>" parent="<?= $objTarefa->getContainerId()?>" >
							<td class="tarefaOrdem">
								-
							</td>
							<td class="tarefaOrdem">
								<span style="color:blue"	title="Cadastrar Atividade" onmouseover="this.style.cursor='pointer'">
									<img border="0" src="../imagens/gif_inclui.gif" onclick="incluirAtividade( this, <?= $objTarefa->getId() ?>, <?= $objTarefa->getContainerId() ? $objTarefa->getContainerId() : 'null' ?> )"/>
								</span>
								<span style="color:green"	title="Editar Atividade" onmouseover="this.style.cursor='pointer'">
								 	<img border="0" src="../imagens/alterar.gif" onclick="editarAtividade( this, <?= $objTarefa->getId() ?>, null )"/>
								</span>
								<span style="color:red"		title="Excluir Atividade" onmouseover="this.style.cursor='pointer'">
									<img border="0" src="../imagens/excluir.gif" onclick="excluirAtividade( this, <?= $objTarefa->getId() ?>, null )"/>
								</span>
							</td>
							<td class="tarefaNome">
								<span class="tarefaMais" name="255,255,255">
									<img src="../../includes/JsLibrary/img/more.gif" id="imgTarefa<?= $objTarefa->getId() ?>" onclick="carregaTarefasFilhas( <?= $objTarefa->getId() ?> , this )" />
								</span>
								<element class="<?= get_class( $objTarefa ) ?>" identifier="<?= $objTarefa->getId() ?>">
									<?= geraTextoClicavel( "strCodigoEstruturado" , "readonly" , $objTarefa->getCodigoEstruturado() , 80 ) ?>
								</element>
								<element class="<?= get_class( $objTarefa ) ?>" identifier="<?= $objTarefa->getId() ?>">
									<?= geraTextoClicavel( "strNome" , "string" , $objTarefa->getNome() , 80 ) ?>
								</element>
							</td>
							<!--
							<td class="tarefaNome">
								<element class="<?= get_class( $objTarefa ) ?>" identifier="<?= $objTarefa->getId() ?>">
									<?= geraTextoClicavel( "strDescricao" , "string" , $objTarefa->getDescricao() , 80 ) ?>
								</element>
							</td>
							-->
							<td class="tarefaNome">
								-
							</td>
							<td class="tarefaNome">
								-
							</td>
							<td class="tarefaOrdem">
								-
							</td>
							<td class="tarefaInicio">
								<element
								class="<?= get_class( $objTarefa )  ?>" 
								identifier="<?= $objTarefa->getId() ?>">
									<?= geraTextoClicavel( "datInicio" , "date" , $objTarefa->getDataInicio() ) ?>
								</element>
							</td>
							<td class="tarefaTermino">
								<element
								class="<?= get_class( $objTarefa )  ?>" 
								identifier="<?= $objTarefa->getId() ?>">
									<?= geraTextoClicavel( "datFim" , "date" , $objTarefa->getDataFim() ) ?>
								</element>
							</td>
							<td class="tarefaOrdem">
								-
							</td>
						</tr>
					<?php
					}
					?>
				</tbody>
			</table>
		</div>
	<?
}

?>