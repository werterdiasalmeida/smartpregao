<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

include_once "./_funcoesmonitoramento.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

/* montando cabeçalho */
$topo = "<table width='100%'>
			<tr>
			<td align='right'><b>Sistema:</b></td>
			<td>".$db->pegaUm("SELECT sisdsc FROM seguranca.sistema WHERe sisid='".$_REQUEST['sisid']."'")."</td>
			".((!$_REQUEST['link'])?"
			<td align='right'><b>Tipo de monitoramento:</b></td>
			<td>".$db->pegaUm("SELECT tmodescricao FROM seguranca.tipomonitoramento WHERe tmoid='".$_REQUEST['tmoid']."'")."</td>":"<td align='right'><b>Link:</b></td><td>".$_REQUEST['link']."</td>")."
			<td align='right'><b>Data:</b></td>
			<td>".sprintf("%02d/%02d/%04d", $_REQUEST['dia'], $_REQUEST['mes'], $_REQUEST['ano'])."</td>
			</tr>
		 </table>";

monta_titulo( "Monitoramento dos módulos", $topo);

?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title>Detalhes do monitoramento</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript">
this._closeWindows = false;
</script>
</head>
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/seguranca.js"></script>
<?

if($_REQUEST['link']) {
	
	$sql = "SELECT date_part('hour', estdata) as hora, count(*) as acessos, ROUND(CAST(AVG(estmemusa)as numeric),2) as num
			FROM seguranca.estatistica e 
			LEFT JOIN seguranca.menu m ON m.mnuid = e.mnuid and date(e.estdata)='".sprintf("%04d-%02d-%02d", $_REQUEST['ano'], $_REQUEST['mes'], $_REQUEST['dia'])."'
			WHERE m.mnulink ilike '%".$_REQUEST['link']."%'
			GROUP BY m.mnulink, hora
			ORDER BY hora";

	$cabecalho = array("Hora","Acessos","Mémoria Utilizada");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','95%',$par2);
	
} else {

	switch($_REQUEST['tmoid']) {
		case NU:
			$sql = "SELECT DISTINCT u.usucpf, u.usunome FROM seguranca.estatistica e 
					LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf 
					WHERE sisid='".$_REQUEST['sisid']."' AND to_char(e.estdata, 'YYYY-MM-DD')='".sprintf("%04d-%02d-%02d", $_REQUEST['ano'], $_REQUEST['mes'], $_REQUEST['dia'])."'";
			
			$cabecalho = array("CPF","Nome");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','95%',$par2);
			break;
		case NE:
			$sql = "SELECT me.mnudsc, '<a href=seguranca.php?modulo=principal/monitoramentopainelpaginadetalhes&acao=A&sisid=".$_REQUEST['sisid']."&ano=".$_REQUEST['ano']."&mes=".$_REQUEST['mes']."&link='||mnulink||'><img src=../imagens/eixos-mini2.png border=0></a> <a href=seguranca.php?modulo=principal/monitoramentopaineldetalheslista&acao=A&sisid=".$_REQUEST['sisid']."&ano=".$_REQUEST['ano']."&mes=".$_REQUEST['mes']."&dia=".$_REQUEST['dia']."&link='||mnulink||'><img src=../imagens/consultar.gif border=0></a> '||mnulink, COUNT(au.oid) as num FROM seguranca.auditoria au 
				    INNER JOIN seguranca.menu me ON au.mnuid=me.mnuid 
				    LEFT JOIN seguranca.usuario u ON u.usucpf=au.usucpf 
					WHERE me.sisid='".$_REQUEST['sisid']."' AND au.audtipo='X' AND to_char(auddata, 'YYYY-MM-DD')='".sprintf("%04d-%02d-%02d", $_REQUEST['ano'], $_REQUEST['mes'], $_REQUEST['dia'])."' 
					GROUP BY me.mnudsc, me.mnulink ORDER BY num";
			
			$cabecalho = array("Menu","Link","Número de erros");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','95%',$par2);
			break;
		case MU:
			$sql = "SELECT mnudsc, '<a href=seguranca.php?modulo=principal/monitoramentopainelpaginadetalhes&acao=A&sisid=".$_REQUEST['sisid']."&ano=".$_REQUEST['ano']."&mes=".$_REQUEST['mes']."&link='||mnulink||'><img src=../imagens/eixos-mini2.png border=0></a> <a href=seguranca.php?modulo=principal/monitoramentopaineldetalheslista&acao=A&sisid=".$_REQUEST['sisid']."&ano=".$_REQUEST['ano']."&mes=".$_REQUEST['mes']."&dia=".$_REQUEST['dia']."&link='||mnulink||'><img src=../imagens/consultar.gif border=0></a> '||mnulink, ROUND(CAST(AVG(estmemusa) as numeric),2) as estmemusa FROM seguranca.estatistica e 
			 		INNER JOIN seguranca.menu m ON m.mnuid=e.mnuid
					WHERE e.estmemusa IS NOT NULL AND e.sisid='".$_REQUEST['sisid']."' AND to_char(estdata, 'YYYY-MM-DD')='".sprintf("%04d-%02d-%02d", $_REQUEST['ano'], $_REQUEST['mes'], $_REQUEST['dia'])."' 
					GROUP BY mnudsc, mnulink ORDER BY estmemusa";
			
			$cabecalho = array("Menu","Link","Memória Utilizada");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','95%',$par2);
			break;
		case TM:
			$sql = "SELECT mnudsc, '<a href=seguranca.php?modulo=principal/monitoramentopainelpaginadetalhes&acao=A&sisid=".$_REQUEST['sisid']."&ano=".$_REQUEST['ano']."&mes=".$_REQUEST['mes']."&link='||mnulink||'><img src=../imagens/eixos-mini2.png border=0></a> <a href=seguranca.php?modulo=principal/monitoramentopaineldetalheslista&acao=A&sisid=".$_REQUEST['sisid']."&ano=".$_REQUEST['ano']."&mes=".$_REQUEST['mes']."&dia=".$_REQUEST['dia']."&link='||mnulink||'><img src=../imagens/consultar.gif border=0></a> '||mnulink, ROUND(CAST(AVG(esttempoexec) as numeric),2) as esttempoexec FROM seguranca.estatistica e 
			 		INNER JOIN seguranca.menu m ON m.mnuid=e.mnuid
					WHERE e.sisid='".$_REQUEST['sisid']."' AND to_char(estdata, 'YYYY-MM-DD')='".sprintf("%04d-%02d-%02d", $_REQUEST['ano'], $_REQUEST['mes'], $_REQUEST['dia'])."' 
					GROUP BY mnudsc, mnulink ORDER BY esttempoexec";
			
			$cabecalho = array("Menu","Link","Tempo de execução");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','95%',$par2);
			break;
		case NR:
			$sql = "SELECT mnudsc, '<a href=seguranca.php?modulo=principal/monitoramentopainelpaginadetalhes&acao=A&sisid=".$_REQUEST['sisid']."&ano=".$_REQUEST['ano']."&mes=".$_REQUEST['mes']."&link='||mnulink||'><img src=../imagens/eixos-mini2.png border=0></a> <a href=seguranca.php?modulo=principal/monitoramentopaineldetalheslista&acao=A&sisid=".$_REQUEST['sisid']."&ano=".$_REQUEST['ano']."&mes=".$_REQUEST['mes']."&dia=".$_REQUEST['dia']."&link='||mnulink||'><img src=../imagens/consultar.gif border=0></a> '||mnulink, COUNT(e.oid) as num FROM seguranca.estatistica e 
					INNER JOIN seguranca.menu m ON m.mnuid=e.mnuid 
					WHERE e.sisid='".$_REQUEST['sisid']."' AND to_char(e.estdata, 'YYYY-MM-DD')='".sprintf("%04d-%02d-%02d", $_REQUEST['ano'], $_REQUEST['mes'], $_REQUEST['dia'])."' 
					GROUP BY mnudsc, mnulink ORDER BY num";
			$cabecalho = array("Menu","Link","Número de requisições");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','95%',$par2);
			break;
		default:
			echo "<table width=\"95%\" align=\"center\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" style=\"color:333333;\" class=\"listagem\">
					<tr>
						<td class=\"SubTituloCentro\">Monitoramento não detalhado</td>
					</tr>
				  </table>";
	}

}
?>
<table class="tabela" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0">
	<td colspan="2">
		<input type='button' value='Voltar' onclick="window.location='seguranca.php?modulo=principal/monitoramentopaineldetalhes&acao=A&sisid=<? echo $_REQUEST['sisid']; ?>&tmoid=<? echo $_REQUEST['tmoid']; ?>&ano=<? echo $_REQUEST['ano'] ?>&mes=<? echo $_REQUEST['mes']; ?>';">
	</td>
</tr>
</table>

</body>
</html>
