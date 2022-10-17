<?
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if( !$pflcod && !$usucpf )
{
	?><font color="red">Requisição inválida</font><?
	exit();
}

$sqlResponsabilidadesPerfil = "SELECT tr.*
	FROM tprperfil p
	INNER JOIN monitora.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
	WHERE tprsnvisivelperfil = TRUE AND p.pflcod = '%s'
	ORDER BY tr.tprdsc";

$query = sprintf($sqlResponsabilidadesPerfil, $pflcod);
print $sql;
$responsabilidadesPerfil = $db->carregar($query);
if (!$responsabilidadesPerfil || @count($responsabilidadesPerfil)<1) {
	print "<font color='red'>Não foram encontrados registros</font>";
}
else {
	foreach ($responsabilidadesPerfil as $rp) {
		//
		// monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (ação, programas, etc)
		$sqlRespUsuario = "";
		switch ($rp["tprsigla"]) {	
			case "A": // AÇÃO
				$aca_prg = "Ações Associadas";
				$sqlRespUsuario = "SELECT a.prgcod || '.' || a.acacod || '.' || a.unicod || '.' || a.loccod AS codigo, a.acadsc AS descricao, a.prgid, a.acaid, a.acacod, u.rpustatus AS status
					FROM usuarioresponsabilidade u 
					INNER JOIN acao a ON a.acaid = u.acaid
					WHERE a.prgano = '".$_SESSION['exercicio']."' and u.usucpf = '%s' AND u.pflcod = '%s' AND u.rpustatus='A' ORDER BY a.prgcod, a.acacod, a.unicod, a.loccod";
			break;
			case "P": // PROGRAMAS 
				$aca_prg = "Programas Associados";
				$sqlRespUsuario = "SELECT p.prgcod AS codigo, p.prgdsc AS descricao, u.rpustatus AS status
					FROM usuarioresponsabilidade u 
					INNER JOIN programa p ON p.prgid = u.prgid
					WHERE p.prgano = '".$_SESSION['exercicio']."' and u.usucpf = '%s' AND u.pflcod = '%s' AND u.rpustatus='A'";
			break;
			case "E": // projetos especiais 
				$aca_prg = "Projetos Associados";
				$sqlRespUsuario = "SELECT p.pjecod AS codigo, p.pjedsc AS descricao, u.rpustatus AS status
					FROM monitora.usuarioresponsabilidade u 
					INNER JOIN monitora.projetoespecial p ON p.pjeid = u.pjeid
					WHERE p.prsano = '".$_SESSION['exercicio']."' and u.usucpf = '%s' AND u.pflcod = '%s' AND u.rpustatus='A'";
			break;
			case "U": // unidades
				$aca_prg = "Unidades Associadas";
				$sqlRespUsuario = "
					SELECT uni.unidsc as descricao, uni.unicod as codigo
					FROM monitora.usuarioresponsabilidade ur 
						INNER JOIN public.unidade uni on
							uni.unicod = ur.unicod and
							uni.unitpocod = 'U' and
							uni.unistatus = 'A'
						inner join seguranca.perfil pfl on
							pfl.pflcod = ur.pflcod
					where
						ur.prsano = '".$_SESSION['exercicio']."' and
						ur.rpustatus = 'A' and
						ur.usucpf = '%s' and
						pfl.pflcod = '%s'
				";
			break;			
		}
		
		if(!$sqlRespUsuario) continue;
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
		$respUsuario = $db->carregar($query);
		if (!$respUsuario || @count($respUsuario)<1) {
			print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>Não existem $aca_prg a este Perfil.</font>";
		}
		else {
		?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
	<tr>
	  <td colspan="3"><?=$rp["tprdsc"]?></td>
	</tr>
	<tr style="color:#000000;">
      <td valign="top" width="12">&nbsp;</td>
	  <td valign="top">Código</td>
	  <td valign="top">Descrição</td>
    </tr>
		<?
			foreach ($respUsuario as $ru) {
		?>
	<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='F7F7F7';" bgcolor="F7F7F7">
      <td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
	  <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap><?if ($rp["tprsigla"]=='A'){?><a href="monitora.php?modulo=principal/acao/cadacao&acao=C&acaid=<?=$ru["acaid"]?>&prgid=<?=$ru["prgid"]?>"><?=$ru["codigo"]?></a><?} else {print $ru["codigo"];}?></td>
	  <td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?=$ru["descricao"]?></td>
	</tr>
		<?
		}
		?>
	<tr>
	  <td colspan="4" align="right" style="color:000000;border-top: 2px solid #000000;">
	    Total: (<?=@count($respUsuario)?>)
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