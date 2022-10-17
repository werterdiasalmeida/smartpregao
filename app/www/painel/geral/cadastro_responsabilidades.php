<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   M�dulo:cadastro_usuario_elaboracao_responsabilidades.php
   
   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if(!$pflcod && !$usucpf) {
	?><font color="red">Requisi��o inv�lida</font><?
	eixt();
}

$sqlResponsabilidadesPerfil = "SELECT tr.*
							   FROM painel.tprperfil p
							   INNER JOIN painel.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
							   WHERE tprsnvisivelperfil = TRUE AND p.pflcod = '%s'
							   ORDER BY tr.tprdsc";
$query = sprintf($sqlResponsabilidadesPerfil, $pflcod);
$responsabilidadesPerfil = $db->carregar($query);
if (!$responsabilidadesPerfil || @count($responsabilidadesPerfil)<1) {
	print "<font color='red'>N�o foram encontrados registros</font>";
}
else {
	foreach ($responsabilidadesPerfil as $rp) {
		//
		// monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (a��o, programas, etc)
		$sqlRespUsuario = "";
		switch ($rp["tprsigla"]) {
			case "I": // Indicadores 
				$aca_prg = "Indicadores";
				$sqlRespUsuario = "SELECT 
									DISTINCT 
									i.indid AS codigo, 
									i.indnome AS descricao, 
									ur.rpustatus AS status
								   FROM
								    painel.usuarioresponsabilidade ur 
								    INNER JOIN painel.indicador i ON i.indid = ur.indid
								   WHERE
								    ur.usucpf = '%s' AND 
								    ur.pflcod = '%s' AND 
								    ur.rpustatus='A'";
				break;
			case "A": // A��es
				$aca_prg = "A��es";
				$sqlRespUsuario = "SELECT DISTINCT 
									a.acaid AS codigo, 
									a.acadsc AS descricao, 
									ur.rpustatus AS status
								   FROM 
								    painel.usuarioresponsabilidade ur 
									INNER JOIN painel.acao a ON a.acaid = ur.acaid
								   WHERE 
								    ur.usucpf = '%s' AND 
								    ur.pflcod = '%s' AND 
								    ur.rpustatus='A' AND
								    a.acastatus = 'A' ";
				break;
			case "E": // Eixos
				$aca_prg = "A��es";
				$sqlRespUsuario = "SELECT DISTINCT 
									exo.exoid AS codigo, 
									exo.exodsc AS descricao, 
									ur.rpustatus AS status
								   FROM 
								    painel.usuarioresponsabilidade ur 
									INNER JOIN painel.eixo exo ON exo.exoid = ur.exoid
								   WHERE 
								    ur.usucpf = '%s' AND 
								    ur.pflcod = '%s' AND 
								    ur.rpustatus='A'";
				break;
			case "M": // Munic�pios
				$aca_prg = "Munic�pios Associados";
				$sqlRespUsuario = "
					select DISTINCT
						m.muncod as codigo,
						m.estuf || ' - ' || m.mundescricao as descricao,
						ur.rpustatus aS status
					from obras.usuarioresponsabilidade ur
						inner join territorios.municipio m on
							m.muncod = ur.muncod
					where
						ur.usucpf = '%s' and
						ur.pflcod = '%s' and
						ur.rpustatus = 'A'";
				break;
			case "O": // �rg�o
				$aca_prg = "�rg�o Associados";
				$sqlRespUsuario = "
					SELECT DISTINCT
						o.orgid AS codigo, o.orgdesc AS descricao
					FROM 
						obras.orgao AS o 
					INNER JOIN 
						obras.usuarioresponsabilidade AS ur 
					ON 
						o.orgid = ur.orgid
					WHERE 
						ur.usucpf = '%s' AND ur.pflcod = '%s' AND ur.rpustatus='A'";
				break;
			default:
				break;
		}
		
		if(!$sqlRespUsuario) continue;
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
		$respUsuario = $db->carregar($query);
		if (!$respUsuario || @count($respUsuario)<1) {
			//print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>N�o existem associa��es a este Perfil.</font>";
		}
		else {
		?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
	<tr>
	  <td colspan="3"><?=$rp["tprdsc"]?></td>
	</tr>
	<tr style="color:#000000;">
      <td valign="top" width="12">&nbsp;</td>
	  <td valign="top">C�digo</td>
	  <td valign="top">Descri��o</td>
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
	$teste = $db->carregar("SELECT DISTINCT * FROM painel.usuarioresponsabilidade WHERE usucpf = '{$usucpf}' AND pflcod = {$pflcod} AND rpustatus = 'A'");
	if (!$teste) {
		print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>N�o existem associa��es a este Perfil.</font>";
	}
}
$db->close();
exit();
?>