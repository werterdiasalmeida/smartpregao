<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

include APPRAIZ . 'includes/cabecalho.inc';
#include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo('Relatório de Super Usuário', '');

extract($_POST);
?>
<form action="" method="post" name="filtro" id="filtro">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				CPF
			</td>
			<td width="80%">
			<?=campo_texto('cpf','N','S','',17,15,'###.###.###-##','');?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Módulo
			</td>
			<td width="80%">
			<?php 
			$sql = "SELECT
					 	sisid AS codigo,
					 	sisdsc AS descricao
					FROM
						seguranca.sistema
					WHERE
						sisstatus = 'A'
					ORDER BY
						sisdsc;";
			$db->monta_combo('sistema',$sql,'S','Todos','','');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Status
			</td>
			<td width="80%">
			<?php 
			$sql = array(array("codigo"    => "A",
							   "descricao" => "Ativo"),
						 array("codigo"	   => "P",
						 	   "descricao" => "Pendente"),
						 array("codigo"	   => "B",
						 	   "descricao" => "Bloqueado"));
						 
			$db->monta_combo('status',$sql,'S','Todos','','');
			?>
			</td>
		</tr>		
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Status Geral
			</td>
			<td width="80%">
			<?php 
			$sql = array(array("codigo"    => "A",
							   "descricao" => "Ativo"),
						 array("codigo"	   => "P",
						 	   "descricao" => "Pendente"),
						 array("codigo"	   => "B",
						 	   "descricao" => "Bloqueado"));
						 
			$db->monta_combo('status_geral',$sql,'S','Todos','','');
			?>
			</td>
		</tr>			
		<tr>
			<td class="SubTituloDireita" valign="top">
				&nbsp;
			</td>
			<td class="SubTituloDireita" style="text-align:left;">
				<input type="submit" name="filtrar" value="Consultar" />			
			</td>
		</tr>		
	</table>
</form>
<BR>
<?php
if ($_POST){ 
$sql = "SELECT
		 u.usucpf,
		 '<a href=\"seguranca.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf=' || u.usucpf || '\" title=\"Gerenciar Usuário\">' || u.usunome || '</a>',
		 s.sisdsc,
		 CASE us.suscod
		 	WHEN 'A' THEN 'Ativo'
		 	WHEN 'P' THEN 'Pendente'
		 	ELSE 'Bloqueado'
		 END AS status,
		 CASE u.suscod
		 	WHEN 'A' THEN 'Ativo'
		 	WHEN 'P' THEN 'Pendente'
		 	ELSE 'Bloqueado'
		 END AS status_geral		 
		FROM
		 seguranca.perfil p
		 INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod
		 INNER JOIN seguranca.usuario u ON u.usucpf = pu.usucpf
		 INNER JOIN seguranca.sistema s ON s.sisid = p.sisid
		 INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf AND
					    							us.sisid = s.sisid
		WHERE
		 pflsuperuser = 't'
		 ".($_POST['sistema'] ? ' AND s.sisid = '.$_POST['sistema'] : '')."
		 ".($_POST['status'] ? " AND us.suscod = '".$_POST['status']."'" : '')."
		 ".($_POST['status_geral'] ? " AND u.suscod = '".$_POST['status_geral']."'" : '')."
		 ".($_POST['cpf'] ? " AND u.usucpf = '".pega_numero($_POST['cpf'])."'" : '')."
		ORDER BY
		 sisdsc";

$cabecalho = array("CPF", "Usuário", "Módulo", "Status", "Status Geral");
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
}
?>
<script type="text/javascript">
function enviarFormulario(){
	var d = document;

	d.filtro.target = 'resultado';
	var janela = window.open( '', 'resultado', 'width=700,height=600,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );	
	d.filtro.submit();
	janela.focus();
}
</script>
