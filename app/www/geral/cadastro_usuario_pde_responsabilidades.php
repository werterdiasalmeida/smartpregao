<?php

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();


function pegarNumeracaoAtividade( $atiid )
{
	global $db;
	$atiid = (integer) $atiid;
	$sql = "select atiidpai, atiordem from pde.atividade where atiid = " . $atiid;
	$dados = $db->recuperar( $sql );
	if ( !$dados )
	{
		return '';
	}
	$resultado = $dados['atiordem'];
	if ( $dados['atiidpai'] && $dados['atiidpai'] != 3 )
	{
		$resultado = pegarNumeracaoAtividade( $dados['atiidpai'] ) . '.' . $resultado;
	}
	return $resultado;
}




$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if( !$pflcod && !$usucpf )
{
	?><font color="red">Requisição inválida</font><?
	exit();
}

$sqlResponsabilidadesPerfil = "
	SELECT
		tr.*
	FROM pde.tprperfil p
		INNER JOIN pde.tiporesponsabilidade tr ON
			p.tprcod = tr.tprcod
	WHERE
		p.pflcod = '%s'
	ORDER BY
		tr.tprdsc
";

$query = sprintf( $sqlResponsabilidadesPerfil, $pflcod );
$responsabilidadesPerfil = $db->carregar( $query );
if ( !$responsabilidadesPerfil || count( $responsabilidadesPerfil ) < 1 )
{
	print "<font color='red'>Não foram encontrados registros</font>";
}
else
{
	foreach ( $responsabilidadesPerfil as $rp )
	{
		//
		// monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (ação, programas, etc)
		$sqlRespUsuario = '';
		switch ( $rp['tprdsc'] )
		{
			case "Atividade": // Atividade
				$sqlRespUsuario = "
					SELECT
						a.atiid,
						a.atidescricao AS descricao
					FROM pde.usuarioresponsabilidade u
						INNER JOIN pde.atividade a ON
							a.atiid = u.atiid
					WHERE
						u.usucpf    = '%s' AND
						u.pflcod    = '%s' AND
						u.rpustatus = 'A'
					ORDER BY
						a.aticodigo,
						a.atidescricao
";
			break;
		}
		if( !$sqlRespUsuario )
		{
			continue;
		}
		$query = vsprintf( $sqlRespUsuario, array( $usucpf, $pflcod ) );
		$respUsuario = $db->carregar( $query );
		$respUsuario = $respUsuario ? $respUsuario : array();
		if ( !$respUsuario || count( $respUsuario ) < 1 )
		{
			print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>Não existem atividades associadas a este Perfil.</font>";
		}
		else
		{
			?>
			<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
				<tr>
				  <td colspan="3"><?= $rp["tprdsc"] ?></td>
				</tr>
				<tr style="color:#000000;">
			      <td valign="top" width="12">&nbsp;</td>
				  <td valign="top">Descrição</td>
			    </tr>
				<?php foreach ( $respUsuario as $ru ) : ?>
					<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='F7F7F7';" bgcolor="F7F7F7">
						<td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
						<td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;">
							<?= pegarNumeracaoAtividade( $ru['atiid'] ) ?>
							<?= $ru["descricao"] ?>
						</td>
					</tr>
				<? endforeach; ?>
				<tr>
				  <td colspan="4" align="right" style="color:000000;border-top: 2px solid #000000;">
				    Total: (<?= count($respUsuario) ?>)
				  </td>
				</tr>
			</table>
			<?
		}
	}
}

$db->close();
exit();

?>