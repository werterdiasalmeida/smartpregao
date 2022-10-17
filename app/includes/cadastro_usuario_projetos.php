<?php

	$sql = sprintf(
		"SELECT s.* FROM seguranca.sistema s WHERE sisid = %d",
		$sisid
	);
	$sistema = (object) $db->pegaLinha( $sql );
	
	$sql = sprintf(
		"SELECT us.*, p.* FROM seguranca.usuario_sistema us LEFT JOIN seguranca.perfil  p USING ( pflcod ) WHERE us.sisid = %d AND usucpf = '%s'",
		$sistema->sisid,
		$usucpf
	);
	$usuariosistema = (object) $db->pegaLinha( $sql );
	
	$sistema->usuariosistema = $usuariosistema;
	$sistemas[] = $sistema;

?>
<tr>
	<td class="subtitulodireita" style="text-align: center" colspan="2">&nbsp;</td>
	</tr>
<tr>
	<td align='right' class="SubTituloDireita">Sistema:</td>
	<td><b><?= $sistema->sisdsc ?></b></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Status:</td>
	<td>
		<input id="status_ativo_<?= $sistema->sisid ?>" type="radio" name="status[<?= $sistema->sisid ?>]" value="A" onchange="alterar_status_sistema( <?= $sistema->sisid ?> );" <?= $usuariosistema->suscod == 'A' ? 'checked="checked"' : "" ?>/>
		<label for="status_ativo_<?= $sistema->sisid ?>">Ativo</label>
		<input id="status_pendente_<?= $sistema->sisid ?>" type="radio" name="status[<?= $sistema->sisid ?>]" value="P" onchange="alterar_status_sistema( <?= $sistema->sisid ?> );" <?= $usuariosistema->suscod == 'P' ? 'checked="checked"' : "" ?>/>
		<label for="status_pendente_<?= $sistema->sisid ?>">Pendente</label>
		<input id="status_bloqueado_<?= $sistema->sisid ?>" type="radio" name="status[<?= $sistema->sisid ?>]" value="B" onchange="alterar_status_sistema( <?= $sistema->sisid ?> );" <?= $usuariosistema->suscod == 'B' ? 'checked="checked"' : "" ?>/>
		<label for="status_bloqueado_<?= $sistema->sisid ?>">Bloqueado</label>
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		<a href="javascript: exibir_ocultar_historico('historico_<?= $sistema->sisid ?>');"><img src="/imagens/mais.gif" style="border: 0"/> histórico</a>
		<div id="historico_<?= $sistema->sisid ?>" style="width: 500px; display: none">
			<p>
			<?php
				$cabecalho = array(
					"Data",
					"Status",
					"Descrição",
				);
				$sql = sprintf(
					"SELECT to_char( hu.htudata, 'dd/mm/YYYY' ) as data, hu.suscod, hu.htudsc FROM seguranca.historicousuario hu WHERE usucpf = '%s' AND sisid = %d ORDER BY hu.htudata DESC",
					$usucpf,
					$sistema->sisid
				);
				$db->monta_lista_simples( $sql, $cabecalho, 25, 0 );
			?>
			</p>
		</div>
	</td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Justificativa:</td>
	<td>
		<div id="justificativa_on_<?= $sistema->sisid ?>" style="display: none;">
			<?= campo_textarea( 'justificativa['. $sistema->sisid .']', 'N', 'S', '', 100, 3, '' ); ?>
		</div>
		<div id="justificativa_off_<?= $sistema->sisid ?>" style="display: block; color:#909090;">
			Status não alterado.
		</div>
	</td>
</tr>
<?php if ( $usuariosistema->pflcod ): ?>
	<tr>
		<td align='right' class="SubTituloDireita">Perfil Desejado:</td>
			<td><?= $usuariosistema->pfldsc ?></td>
	</tr>
	<?php
		$sql = sprintf(
			"select distinct u.unicod, u.unidsc from public.unidade u inner join elabrev.unidproposto up using ( unicod ) where up.usucpf = '%s'",
			$usucpf
		);
		$unidades = $db->carregar( $sql );
	?>
	<?php if ( $unidades ): ?>
		<tr>
			<td align='right' class="SubTituloDireita">Unidades Propostas:</td>
			<td>
				<?php foreach ( $unidades as $unidade ): ?>
					<?= implode( ' ', $unidade ); ?><br/>
				<?php endforeach; ?>
			</td>
		</tr>
	<?php endif; ?>
	<?php
		$sql = sprintf( "SELECT DISTINCT p.prgcod, p.prgdsc AS descricao FROM elabrev.ppaprograma_proposta p INNER JOIN elabrev.progacaoproposto pp USING ( prgid ) WHERE pp.usucpf='%s' AND pp.acacod IS NULL", $usucpf );
		$programas = $db->carregar( $sql );
	?>
	<?php if ( $programas ): ?>
		<tr>
			<td align='right' class="SubTituloDireita">Programas Propostos:</td>
			<td>
				<?php foreach ( $programas as $programa ): ?>
					<?= implode( ' ', $programa ); ?><br/>
				<?php endforeach; ?>
			</td>
		</tr>
	<?php endif; ?>
	<?php
		$sql = sprintf( "SELECT DISTINCT a.prgcod, a.acacod, a.unicod, a.acadsc FROM elabrev.ppaacao_proposta a INNER JOIN elabrev.progacaoproposto pp USING ( acacod, prgid, unicod ) WHERE pp.usucpf='%s' AND pp.acacod IS NOT NULL", $usucpf );
		$acoes = $db->carregar( $sql );
	?>
	<?php if ( $acoes ): ?>
		<tr>
			<td align='right' class="SubTituloDireita">Ações Propostas:</td>
			<td>
				<?php foreach ( $acoes as $acao ): ?>
					<?= implode( ' ', $acao ); ?><br/>
				<?php endforeach; ?>
			</td>
		</tr>
	<?php endif; ?>
<?php endif; ?>
<tr>
	<td align='right' class="SubTituloDireita">Perfil:</td>
	<td>
		<?php
			
			$sql = sprintf(
				"select p.pflnivel from seguranca.perfil p inner join seguranca.perfilusuario pu on pu.pflcod=p.pflcod and pu.usucpf='%s' and p.sisid=%d order by p.pflnivel",
				$_SESSION['usucpf'],
				$sistema->sisid
			);
			$nivel = $db->pegaUm( $sql );
			
			$sql_perfil = sprintf(
				"select distinct p.pflcod as codigo, p.pfldsc as descricao from seguranca.perfil p left join seguranca.perfilusuario pu on pu.pflcod=p.pflcod  where p.pflstatus='A' and p.pflnivel >= %d and p.sisid=%d order by descricao",
				$nivel,
				$sistema->sisid
			);
			$sql = sprintf(
				"select distinct p.pflcod as codigo, p.pfldsc as descricao from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod=pu.pflcod where p.sisid=%d and pu.usucpf='%s' order by descricao",
				$sistema->sisid,
				$usucpf
			);
			$nome = 'pflcod[' . $sistema->sisid . ']';
			$$nome = $db->carregar( $sql ); 
			combo_popup( 'pflcod['. $sistema->sisid .']', $sql_perfil, 'Selecione o(s) Perfil(s)', '360x460' );
			
		?>
	</td>
</tr>

<!-- ----------------------------------------------------------- -->

<?php
	
	$sql = "SELECT * FROM pde.tiporesponsabilidade ORDER BY tprdsc";
	$responsabilidades = $db->carregar( $sql );
	$responsabilidades = $responsabilidades ? $responsabilidades : array();
	
	$sql = sprintf(
		"SELECT p.pflcod, p.pfldsc FROM perfil p INNER JOIN perfilusuario pu ON pu.pflcod = p.pflcod AND pu.usucpf = '%s' and p.sisid=%d ORDER BY p.pfldsc",
		$usucpf, $sisid
	);
	$perfisUsuario = $db->carregar( $sql );
	
?>
<tr>
	<td align='right' class="SubTituloDireita">Associação de Perfil:</td>
	<?php if( $perfisUsuario ): ?>
		<td>
			<table border="0" cellpadding="2" cellspacing="0" width="500" class="listagem" bgcolor="#fefefe">
				<tr>
					<td width="12" rowspan="2" bgcolor="#e9e9e9" align="center">&nbsp;</td>
					<td rowspan="2" align="left" bgcolor="#e9e9e9" align="center">Descrição</td>
					<td align="center" colspan="<?= count( $responsabilidades ) ?>" bgcolor="#e9e9e9" align="center" style="border-bottom: 1px solid #bbbbbb">Responsabilidades</td>
				</tr>
				<tr>
					<?php foreach( $responsabilidades as $responsabilidade ): ?>
						<td align="center" bgcolor="#e9e9e9" align="center"><?= $responsabilidade["tprdsc"] ?></td>
					<? endforeach; ?>
				</tr>
				<?php foreach( $perfisUsuario as $chave => $perfil ): ?>
					<? $idPosfixo = '_' . $sistema->sisid . '_' . $chave . '_' . $perfil['pflcod']; ?>
					<?php
						$marcado = $i++ % 2 ? '#F7F7F7' : '';
						$sql = sprintf(
							"SELECT p.*, tr.tprdsc FROM ( SELECT * FROM pde.tprperfil WHERE pflcod = '%s' ) p RIGHT JOIN pde.tiporesponsabilidade tr ON p.tprcod = tr.tprcod ORDER BY tr.tprdsc",
							$perfil["pflcod"]
						);
						$responsabilidadesPerfil = $db->carregar( $sql );
						
						// Esconde a imagem + para perfis sem responsabilidades
						$mostraMais = false;
						foreach( $responsabilidadesPerfil as $resPerfil )
						{
							if ( (boolean) $resPerfil["tprcod"] )
							{
								$mostraMais = true;
								break;
							}
						}
					?>
					<tr bgcolor="<?= $marcado ?>">
						<td style="color:#003c7b">
							<?php if ( $mostraMais ) : ?>
								<a href="javascript:abreconteudo( '../geral/cadastro_usuario_pde_responsabilidades.php?usucpf=<?= $usucpf ?>&pflcod=<?= $perfil["pflcod"] ?>', '<?= $idPosfixo ?>' ); ">
									<img src="../imagens/mais.gif" name="+" border="0" id="img<?= $idPosfixo ?>"/>
								</a>
							<?php endif; ?>
						</td>
						<td><?= $perfil["pfldsc"] ?></td>
						<?php foreach ( $responsabilidadesPerfil as $resPerfil ): ?>
							<td align="center">
								<?php if ( (boolean) $resPerfil["tprcod"] ): ?>
									<input type="button" name="btnAbrirResp<?=$perfil["pflcod"]?>" value="Atribuir" onclick="popresp_<?= $sistema->sisid ?>( <?= $perfil["pflcod"] ?>, '<?= $resPerfil["tprdsc"] ?>' )">
								<?php else: ?>
									-
								<?php endif; ?>
							</td>
						<?php endforeach; ?>
					</tr>
					<tr bgcolor="<?= $marcado ?>">
						<td colspan="10" id="td<?= $idPosfixo ?>"></td>
					</tr>
				<?php endforeach; ?>
			</table>
		</td>
	<?php else: ?>
		<td>
			<div id="justificativa_off_<?= $sistema->sisid ?>" style="display: block; color:#909090;">
				Usuário sem perfil associado.
			</div>
		</td>
	<?php endif; ?>
</tr>
<script type="text/javascript">

	var WindowObjectReference;

	function associa_perfil( sisdiretorio ) {
		WindowObjectReference = window.open( "/" + sisdiretorio + "/" + sisdiretorio + ".php?modulo=sistema/usuario/associa_usuario&acao=A&usucpf=<?= $usucpf ?>", "Associar_Perfil", "menubar=no,location=no,resizable=no,scrollbars=yes,status=no,width=400,height=480" );
	}

	function popresp_<?= $sistema->sisid ?>( pflcod, tprsigla ) {
		switch( tprsigla ){
			case 'Atividade':
				abreresp = window.open(
					"../geral/cadastro_usuario_pde_atividade.php?pflcod=" + pflcod + "&usucpf=<?=$usucpf?>",
					"popresp_<?= $sistema->sisid ?>",
					"menubar=no,location=no,resizable=no,scrollbars=no,status=yes,width=500,height=480"
				);
				break;
		}
	}

</script>