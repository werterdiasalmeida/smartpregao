<?
 /*
   Sistema Simec
   Setor respons?vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   M?dulo:cadastro_usuario_elaboracao_responsabilidades.php
   
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if(!$pflcod && !$usucpf) {
	?><font color="red">Requisi??o inv?lida</font><?
	eixt();
}

$sqlResponsabilidadesPerfil = "SELECT tr.*
	FROM cte.tprperfil p
	INNER JOIN cte.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
	WHERE tprsnvisivelperfil = TRUE AND p.pflcod = '%s'
	ORDER BY tr.tprdsc";

$query = sprintf($sqlResponsabilidadesPerfil, $pflcod);
$responsabilidadesPerfil = $db->carregar($query);
if (!$responsabilidadesPerfil || @count($responsabilidadesPerfil)<1) {
	print "<font color='red'>N?o foram encontrados registros</font>";
}
else {
	foreach ($responsabilidadesPerfil as $rp) {
		//
		// monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (a??o, programas, etc)
		$sqlRespUsuario = "";
		switch ($rp["tprsigla"]) {
			case "U": // Unidades 
				$aca_prg = "Unidades Associadas";
				$sqlRespUsuario = "SELECT u.unicod AS codigo, u.unidsc AS descricao, ur.rpustatus AS status
					FROM cte.usuarioresponsabilidade ur 
					INNER JOIN unidade u ON u.unicod = ur.unicod
					WHERE ur.usucpf = '%s' AND ur.pflcod = '%s' AND ur.rpustatus='A'";
				break;
			case "E": // Estados
				$aca_prg = "Estados Associados";
				$sqlRespUsuario = "SELECT e.estuf AS codigo, e.estdescricao AS descricao, ur.rpustatus AS status
					FROM cte.usuarioresponsabilidade ur 
					INNER JOIN territorios.estado e ON e.estuf = ur.estuf
					WHERE ur.usucpf = '%s' AND ur.pflcod = '%s' AND ur.rpustatus='A'";
				break;
			case "M": // Munic?pios
				$aca_prg = "Munic?pios Associados";
				$sqlRespUsuario = "
					select
						m.muncod as codigo,
						m.estuf || ' - ' || m.mundescricao as descricao,
						ur.rpustatus aS status
					from cte.usuarioresponsabilidade ur
						inner join territorios.municipio m on
							m.muncod = ur.muncod
					where
						ur.usucpf = '%s' and
						ur.pflcod = '%s' and
						ur.rpustatus = 'A'				
					order by
						m.estuf,
						m.mundescricao
				";
				break;
		}
		
		if(!$sqlRespUsuario) continue;
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
		$respUsuario = $db->carregar($query);
		if (!$respUsuario || @count($respUsuario)<1) {
			print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>N?o existem $aca_prg a este Perfil.</font>";
		}
		else {
		?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
	<tr>
	  <td colspan="3"><?=$rp["tprdsc"]?></td>
	</tr>
	<tr style="color:#000000;">
      <td valign="top" width="12">&nbsp;</td>
	  <td valign="top">C?digo</td>
	  <td valign="top">Descri??o</td>
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