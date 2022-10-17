<?php

	if ( $_REQUEST['formulario'] ) {
		
		$usucpf = $_REQUEST['usucpf'];
		$usunome = $_REQUEST['usunome'];
		$codigo = $_REQUEST['codigo'];
		$suscod = $_REQUEST['suscod'];
		$muncod = $_REQUEST['muncod'];
		$pflcod = $_REQUEST['pflcod'];
		
		$condicao = array();
		if ( $_REQUEST['usucpf'] ) array_push( $condicao, " u.usucpf like '%". str_to_upper( corrige_cpf( $_REQUEST['usucpf'] ) ) ."%' " );
		if ( $_REQUEST['usunome'] ) array_push( $condicao, " u.usunome like '%". str_to_upper( $_REQUEST['usunome'] ) ."%' " );
		if ( $_REQUEST['codigo'] ) array_push( $condicao, " us.sisid = '". $_REQUEST['codigo'] ."' " );
		if ( $_REQUEST['suscod'] ) array_push( $condicao, " us.suscod = '". $_REQUEST['suscod'] ."' " );
		if ( $_REQUEST['muncod'] ) array_push( $condicao, " u.muncod like '%".$_REQUEST['muncod']."%' " );
		if ( $_REQUEST['pflcod'] ) array_push( $condicao, " p2.pflcod = '".$_REQUEST['pflcod']."' " );
		if ( empty( $condicao ) ) {
			array_push( $condicao, " 1 = 1" );
		}

		$cabecalho_usuario = array( 'CPF', 'Nome', 'Telefone', 'Órgão', 'Município', 'Estado', 'Perfil', 'Status', 'Data de Cadastro' );
		$sql_usuario = sprintf( "
			select
				u.usucpf, u.usunome, u.usufoneddd || ' ' || u.usufonenum, o.orgdsc, m.mundescricao, uf.estuf, p2.pfldsc, su.susdsc, to_char( u.usudatainc, 'dd/mm/YYYY' )
			from seguranca.usuario u
			inner join public.orgao o on o.orgcod = u.orgcod
			inner join territorios.municipio m on m.muncod = u.muncod
			inner join territorios.estado uf on uf.estuf = m.estuf
			inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
			inner join seguranca.statususuario su on su.suscod = us.suscod
			
			inner join (
				select max(p.pflcod) as perfil, pu.usucpf
				from seguranca.perfil p
				inner join seguranca.perfilusuario pu on pu.pflcod = p.pflcod
				where ( p.pflnivel, pu.usucpf ) in  (
					select max(pflnivel) as pflnivel, pu.usucpf
					from seguranca.perfil p
					inner join seguranca.perfilusuario pu on pu.pflcod = p.pflcod
					where p.sisid = %d
					group by pu.usucpf
				) group by pu.usucpf
			) as x on x.usucpf = u.usucpf
			inner join seguranca.perfil p2 on p2.pflcod = x.perfil 
			
			where %s
			group by u.usucpf, u.usunome, u.usufoneddd, u.usufonenum, o.orgdsc, m.mundescricao, uf.estuf, p2.pfldsc, su.susdsc, u.usudatainc",
			$codigo,
			implode( " and ", $condicao )
		);
	} else {
		$codigo = $_SESSION['sisid'];
	}
/*
inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf 
inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflnivel = ( select max(pflnivel) from seguranca.perfil where pflcod = pu.pflcod )
*/
?>

<?php include APPRAIZ . "includes/cabecalho.inc"; ?>
<br/>
<?php
	$db->cria_aba( $abacod_tela, $url, $parametros );
	monta_titulo( $titulo_modulo, '' );
?>


<form method="POST" name="formulario" class="notprint">
	<input type="hidden" name="formulario" value="1"/>
	<input type=hidden name="modulo" value="<?= $modulo ?>"/>
	<input type=hidden name="act" value="0"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita">CPF (ou parte do CPF):</td>
			<td><?=campo_texto('usucpf','','','',16,14,'###.###.###-##','');?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Nome completo (ou parte do nome):</td>
			<td><?=campo_texto('usunome','','','',50,50,'','');?></td>
		</tr>
		<tr>
		<td align='right' class="SubTituloDireita">Município:</td>
			<td><?=campo_texto('muncod','','','',10,7,'#######','');?></td>
		</tr>
		<?php if( $_SESSION["sisid"] == 4 ): ?>
			<tr>
				<td align='right' class="SubTituloDireita">Sistema:</td>
				<td>
					<?php
						$sql = "select s.sisid as codigo, s.sisdsc as descricao from seguranca.sistema s where s.sisstatus='A'";
						$db->monta_combo( "codigo", $sql, 'S', '', '', '' );
					?>	
				</td>
			</tr>
		<?php else: ?>
			<input type="hidden" name="codigo" value="<?= $_SESSION["sisid"] ?>"/>
		<?php endif; ?>
		<?php if( $codigo ): ?>
		<tr>
			<td align='right' class="SubTituloDireita">Perfil:</td>
			<td>
				<?php
					$pflnivel = $db->pegaUm( "select min( pflnivel ) from seguranca.perfil p inner join seguranca.perfilusuario pu on pu.pflcod = p.pflcod and pu.usucpf = '". $_SESSION['usucpf'] ."'" );
					$sql = sprintf(
						"select p.pflcod as codigo, p.pfldsc as descricao from seguranca.perfil p where p.sisid = %d and p.pflstatus = 'A' and p.pflnivel >= %d",
						$codigo,
						$pflnivel
					);
					$db->monta_combo( "pflcod", $sql, 'S', 'Todos', '', '' );
				?>	
			</td>
		</tr>
		<?php endif; ?>
		<tr bgcolor="#F2F2F2">
			<td align='right' class="SubTituloDireita">Status geral do usuário:</td>
			<td>
				<input id="status_qualquer" type="radio" name="suscod" value="" <?= $suscod == '' ? 'checked="checked"' : "" ?>/>
				<label for="status_qualquer">Qualquer</label>
				
				<input id="status_ativo" type="radio" name="suscod" value="A" <?= $suscod == 'A' ? 'checked="checked"' : "" ?>/>
				<label for="status_ativo">Ativo</label>
				
				<input id="status_pendente" type="radio" name="suscod" value="P" <?= $suscod == 'P' ? 'checked="checked"' : "" ?>/>
				<label for="status_pendente">Pendente</label>
				
				<input id="status_bloqueado" type="radio" name="suscod" value="B" <?= $suscod == 'B' ? 'checked="checked"' : "" ?>/>
				<label for="status_bloqueado">Bloqueado</label>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td ></td>
			<td><input type='button' class="botao" name='consultar' value='Consultar' onclick="ProcuraUsuario()"</td>
		</tr>
	</table>
</form>
<?php
	if ( $_REQUEST['formulario'] ) {
		$db->monta_lista( $sql_usuario, $cabecalho_usuario, 100, 20, '', '' ,'' );
	}
?>
<script language="javascript">

	function ProcuraUsuario() {
		document.formulario.submit();
	}

</script>