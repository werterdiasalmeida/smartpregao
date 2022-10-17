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
			combo_popup( $nome, $sql_perfil, 'Selecione o(s) Perfil(s)', '360x460' );
		?>
	</td>
</tr>