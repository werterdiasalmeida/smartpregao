<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:cadastro_usuario_elaboracao_responsabilidades.php
   
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if(!$pflcod && !$usucpf) {
	?><font color="red">Requisição inválida</font><?
	eixt();
}

$sqlResponsabilidadesPerfil = "SELECT tr.*
	FROM elabrev.tprperfil p
	INNER JOIN elabrev.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
	WHERE tprsnvisivelperfil = TRUE AND p.pflcod = '%s'
	ORDER BY tr.tprdsc";

$query = sprintf($sqlResponsabilidadesPerfil, $pflcod);
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
				$sqlRespUsuario = "SELECT a.prgcod || '.' || a.acacod  AS codigo, a.acadsc AS descricao, a.prgid, a.acaid, a.acacod, u.rpustatus AS status
					FROM elabrev.usuarioresponsabilidade u 
					INNER JOIN elabrev.ppaacao_proposta a ON a.acaid = u.acaid
					WHERE u.usucpf = '%s' AND u.pflcod = '%s' AND u.rpustatus='A' ORDER BY a.prgcod, a.acacod";
			break;
			case "P": // PROGRAMAS 
				$aca_prg = "Programas Associados";
				$sqlRespUsuario = "SELECT p.prgcod AS codigo, p.prgdsc AS descricao, u.rpustatus AS status
					FROM elabrev.usuarioresponsabilidade u 
					INNER JOIN elabrev.ppaprograma_proposta p ON p.prgid = u.prgid
					WHERE u.usucpf = '%s' AND u.pflcod = '%s' AND u.rpustatus='A'";
			break;
			case "U": // Unidades 
				$aca_prg = "Unidades Associadas";
				$sqlRespUsuario = "SELECT u.unicod AS codigo, u.unidsc AS descricao, ur.rpustatus AS status
					FROM elabrev.usuarioresponsabilidade ur 
					INNER JOIN unidade u ON u.unicod = ur.unicod
					WHERE ur.usucpf = '%s' AND ur.pflcod = '%s' AND ur.rpustatus='A'";
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
	  <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap><?if ($rp["tprsigla"]=='A'){?><a href="simec_er.php?modulo=principal/acao/cadacao&acao=C&acaid=<?=$ru["acaid"]?>&prgid=<?=$ru["prgid"]?>"><?=$ru["codigo"]?></a><?} else {print $ru["codigo"];}?></td>
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