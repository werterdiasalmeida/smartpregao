<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>

<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
<?
ini_set("memory_limit", "1024M");
$filtrourlm = array();
$arrUrlInterna = array();

if($_REQUEST['estuf']) {
	$filtro[] = "ed.estuf='".$_REQUEST['estuf']."'";
	$arrUrlInterna[] = "&estuf=".$_REQUEST['estuf'];
	$filtrourlm[] = "&estuf=".$_REQUEST['estuf'];
	$titulo = "Estado: ".$db->pegaUm("SELECT estdescricao FROM territorios.estado WHERE estuf='".$_REQUEST['estuf']."'");
}
if($_REQUEST['muncod']) {
	$filtro[] = "ed.muncod='".$_REQUEST['muncod']."'";
	$arrUrlInterna[] = "&muncod=".$_REQUEST['muncod'];
	$filtrourlm[] = "&muncod=".$_REQUEST['muncod'];
	$titulo = "Município: ".$db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='".$_REQUEST['muncod']."'");
}
if($_REQUEST['unicod']) {
	$filtro[] = "ent.entunicod='".$_REQUEST['unicod']."'";
	$arrUrlInterna[] = "&unicod=".$_REQUEST['unicod'];
	$filtrourlm[] = "&unicod=".$_REQUEST['unicod'];
	$titulo = "Unidade: ".$db->pegaUm("SELECT unidsc FROM public.unidade WHERE unicod='".$_REQUEST['unicod']."'");
}
if($_REQUEST['entid']) {
	$filtro[] = "ent2.entid='".$_REQUEST['entid']."'";
	$arrUrlInterna[] = "&entid=".$_REQUEST['entid'];
	$filtrourlm[] = "&entid=".$_REQUEST['entid'];
	$titulo = "Campus: ".$db->pegaUm("SELECT entnome FROM entidade.entidade WHERE entid='".$_REQUEST['entid']."'");
}

if($_REQUEST['stoid']) {
	$filtrourlm[] = "&stoid=".$_REQUEST['stoid'];
	$titulo2 = ", ".$db->pegaUm("SELECT stodesc FROM obras.situacaoobra WHERE stoid='".$_REQUEST['stoid']."'");
}


/* montando o menu */
// monta menu padrão contendo informações sobre as entidades
$menu = array(
			  0 => array("id" => 0, "descricao" => "Todos",     			"link" => "/painel/painel.php?modulo=relatorio/relatorioObra&acao=A".(($_REQUEST['orgid'])?"&orgid=todos":"").implode("",$arrUrlInterna)),
      		  1 => array("id" => 1, "descricao" => "Educação Superior",     "link" => "/painel/painel.php?modulo=relatorio/relatorioObra&acao=A&orgid=1".implode("",$arrUrlInterna)),
			  2 => array("id" => 2, "descricao" => "Educação Profissional", "link" => "/painel/painel.php?modulo=relatorio/relatorioObra&acao=A&orgid=2".implode("",$arrUrlInterna)),
			  3 => array("id" => 3, "descricao" => "Educação Básica", 		"link" => "/painel/painel.php?modulo=relatorio/relatorioObra&acao=A&orgid=3".implode("",$arrUrlInterna)),
		  	  );

if(!$_REQUEST['orgid']) $_REQUEST['orgid']="todos";
if(!$titulo) $titulo="País: Brasil";

if($_REQUEST['orgid']) {
	if($_REQUEST['orgid'] != "todos")
		$filtro[] = "oi.orgid='".$_REQUEST['orgid']."'";
	array_unshift($arrUrlInterna, "&orgid=".$_REQUEST['orgid']);
}

if(!$_REQUEST['pagina']) {
	$_REQUEST['pagina']=0;
}
		  	  

echo "<br>"; 
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
monta_titulo( "Relatório de obras (".$titulo.$titulo2.") ", "<b><a href='/painel/painel.php?modulo=relatorio/relatorioObra&acao=A".implode("",$arrUrlInterna)."'>Listar todas as obras</a></b>");
?>
<table class="tabela" cellSpacing="1" cellPadding="3" align="center" style="border:1px solid #ccc; background-color:#fff;">
	<tr>
 		<td>
				<table cellspacing="1" cellpadding="3" width="100%">
					<tr>
						<td style="text-align: center; background-color: #dedede; font-weight: bold;"> Resumo de Obras </td>
					</tr>
					<tr>
						<td style="padding: 0px; margin: 0px;">
<?

// cria a query com as situações de obras e a quantidade
$sql = "SELECT 
				case when oi.stoid is not null 
				then '<a href=painel.php?modulo=relatorio/relatorioObra&acao=A".(($arrUrlInterna)?implode("",$arrUrlInterna):"")."&stoid='|| oi.stoid ||'>'||so.stodesc||'</a>' 
				else 'Não informado' end as descricao,
				count(oi.obrid) as total
			FROM
				obras.obrainfraestrutura oi
			LEFT JOIN
				obras.situacaoobra so ON so.stoid = oi.stoid
			LEFT JOIN
				entidade.entidade ent ON ent.entid = oi.entidunidade
			LEFT JOIN 
				entidade.entidade ent2 ON ent2.entid = oi.entidcampus 
			INNER JOIN 
				entidade.endereco ed ON ed.endid = oi.endid
			WHERE
				oi.obsstatus = 'A'
				".(($filtro)?" AND ".implode(" AND ", $filtro):"")."
			GROUP BY 
				oi.stoid, stodesc
			ORDER BY
				descricao";

// exibe na tela a lista de situações de obras
$cabecalho = array("Situação", "Qtd de Obras");
$db->monta_lista_simples( $sql, $cabecalho, 1000, 30, 'S', '100%','N');

if($_REQUEST['stoid']) {
	$filtro[] = "so.stoid='".$_REQUEST['stoid']."'";
	array_unshift($arrUrlInterna, "&stoid=".$_REQUEST['stoid']);
}
							
?>
						</td>
					</tr>
				</table>
		</td>
	</tr>
	
	
	<tr>
		<td>
				<table cellspacing="1" cellpadding="3" width="100%">
					<tr>
						<td style="text-align: center; background-color: #dedede; font-weight: bold;"> Lista de obras </td>
					</tr>
<?

$sql = "SELECT
			count(oi.obrid) as num 
		FROM
			obras.obrainfraestrutura oi
		LEFT JOIN
			obras.situacaoobra so ON so.stoid = oi.stoid  
		LEFT JOIN 
			entidade.endereco ed ON ed.endid = oi.endid
		LEFT JOIN 
			entidade.entidade ent ON ent.entid = oi.entidunidade
		LEFT JOIN 
			entidade.entidade ent2 ON ent2.entid = oi.entidcampus 
		LEFT JOIN 
			territorios.municipio tm ON tm.muncod = ed.muncod
		WHERE 
			oi.obsstatus = 'A'
			".(($filtro)?" AND ".implode(" AND ", $filtro):"");
$pages = $db->pegaUm($sql);


if( $_REQUEST['estado'] || $_SESSION['par']['estado'] ){
	$_SESSION['par']['estado'] = 1;
	$inner = "INNER JOIN entidade.funcaoentidade fe on fe.entid = ent.entid";
	$where = "and funid = 6";
}

$sql = "SELECT 
			ent.entnome as unidade,
			COALESCE(ent2.entnome, 'Não cadastrado') as campus,
			oi.stoid,
			oi.obrid, 
			oi.obrdesc as nome,
			oi.obrdtinicio, 
			oi.obrdttermino, 
			COALESCE(tm.mundescricao,'Não cadastrado')||'/'||COALESCE(ed.estuf, 'Não cadastrado') as local,
			case when oi.stoid is not null then so.stodesc else 'Não Informado' end as situacao,
			(SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual, 
			oi.obrcomposicao,
		    CASE WHEN oi.stoid NOT IN (2,3,4,5) THEN
			(CASE WHEN (CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END) > (CURRENT_DATE - integer '30') THEN '<label style=\"color:#00AA00;\">' ||  to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || '</label>'
			      WHEN ((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END) <= (CURRENT_DATE - integer '30') AND (CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END) >= (CURRENT_DATE - integer '45')) THEN '<label style=\"color:#BB9900;\">' || to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || '</label>'
			 ELSE '<label style=\"color:#DD0000;\">' || to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || '</label>' END)
			 WHEN oi.stoid = 2 THEN '<label style=\"color:#DD0000;\">' || to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || '</label>'
			 WHEN oi.stoid = 3 THEN '<label style=\"color:blue;\">' || to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || '</label>'
		    ELSE to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') END as ultimadata 			
		FROM
			obras.obrainfraestrutura oi
		LEFT JOIN
			obras.situacaoobra so ON so.stoid = oi.stoid  
		LEFT JOIN 
			entidade.endereco ed ON ed.endid = oi.endid
		LEFT JOIN 
			entidade.entidade ent ON ent.entid = oi.entidunidade
		LEFT JOIN 
			entidade.entidade ent2 ON ent2.entid = oi.entidcampus 
		LEFT JOIN 
			territorios.municipio tm ON tm.muncod = ed.muncod
		".$inner."
		WHERE 
			oi.obsstatus = 'A'
			".(($filtro)?" AND ".implode(" AND ", $filtro):"")."
			".$where."
		ORDER BY
			oi.orgid, ent.entnome, ent2.entnome, oi.obrdesc, so.stodesc 
		LIMIT 100 OFFSET ".$_REQUEST['pagina'];

//ver($sql);
/*
echo "<pre>";
echo htmlentities($sql)."<br>";
*/
		
$obras = $db->carregar($sql);

if($pages > 100) {
	echo "<tr><td>Paginação: ";
	for($i=0;$i<ceil($pages/100);$i++) {
		
		if($_REQUEST['pagina']==$i) $label = "<b>".($i+1)."</b>";
		else $label = ($i+1);
		
		echo "<a href=painel.php?modulo=relatorio/relatorioObra&acao=A".(($arrUrlInterna)?implode("",$arrUrlInterna):"")."&pagina=".$i.">".($label)."</a> ";
	}
	echo "</td></tr>";
}


if($obras[0]) {
	foreach($obras as $obra) {
		
		switch ( $obra["stoid"] ){
			case "1":
				$obra['situacao'] = '<label style="color:#00AA00">' . $obra['situacao'] . '</label>';
			break;
			case "2":
				$obra['situacao'] = '<label style="color:#DD0000">' . $obra['situacao'] . '</label>';
			break;
			case "3":
				$obra['situacao'] = '<label style="color:blue">' . $obra['situacao'] . '</label>';
			break;
			case "6":
				$obra['situacao'] = '<label style="color:#DD0000">' . $obra['situacao'] . '</label>';
			break;
		}
		
		echo "<tr><td>
		
		<table class=listagem width=100%>
			<tr>
				<td width=15% class=SubTituloDireita>Unidade:</td>
				<td width=35%>".strtoupper($obra['unidade'])."</td>
				<td width=15% class=SubTituloDireita>Campus:</td>
				<td width=35%>".strtoupper($obra['campus'])."</td>
			</tr>
			<tr>
				<td width=15% class=SubTituloDireita>Nome da obra:</td>
				<td width=35%>".$obra['nome']."</td>
				<td width=15% class=SubTituloDireita>Municipio/UF:</td>
				<td width=35%>".$obra['local']."</td>
			</tr>
			
			<tr>
				<td width=15% class=SubTituloDireita>Início programado:</td>
				<td width=35%>".(($obra['obrdtinicio'])?formata_data($obra['obrdtinicio']):"Não cadastrado")."</td>
				<td width=15% class=SubTituloDireita>Término programado:</td>
				<td width=35%>".(($obra['obrdttermino'])?formata_data($obra['obrdttermino']):"Não cadastrado")."</td>
			</tr>
			
			<tr>
				<td width=15% class=SubTituloDireita>Situação da Obra:</td>
				<td width=35%>".$obra['situacao']."</td>
				<td width=15% class=SubTituloDireita>% Executado:</td>
				<td width=35%>".$obra['percentual']."</td>
			</tr>

			
			<tr>
				<td width=15% class=SubTituloDireita>Última atualização:</td>
				<td colspan=3>".$obra['ultimadata']."</td>
			</tr>
			
			<tr>
				<td width=15% class=SubTituloDireita>Descrição:</td>
				<td width=85% colspan=3>".(($obra['obrcomposicao'])?nl2br($obra['obrcomposicao']):"Não cadastrado")."</td>
			</tr>
			
			<tr>
				<td class=SubTituloCentro colspan=4>FOTOS:</td>
			</tr>
		";
		
		$sql = "SELECT 
					arqnome, arq.arqid, arq.arqextensao, arq.arqtipo, arq.arqdescricao, 
					to_char(oar.aqodtinclusao,'dd/mm/yyyy') as aqodtinclusao 
				FROM 
					public.arquivo arq
				INNER JOIN 
					obras.arquivosobra oar ON arq.arqid = oar.arqid
				INNER JOIN 
					obras.obrainfraestrutura obr ON obr.obrid = oar.obrid 
				WHERE 
					obr.obrid='". $obra['obrid'] ."' AND
					aqostatus = 'A' AND
				   (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
				ORDER BY 
					arq.arqid DESC LIMIT 4";

		$fotos = $db->carregar($sql);
		echo "<tr><td valign=\"top\" align=\"center\" colspan=4><table><tr>";
		if($fotos[0]){
			for( $k = 0; $k < count($fotos); $k++ ){
				$_SESSION['imgparametos'][$fotos[$k]["arqid"]] = array( "filtro" => "cnt.obrid=".$obra['obrid']." AND aqostatus = 'A'", "tabela" => "obras.arquivosobra");
				echo "<td valign=\"top\" align=\"center\">";
				echo "<img id='".$fotos[$k]["arqid"]."' onclick='window.open(\"../slideshow/slideshow/ajustarimgparam3.php?pagina=0&_sisarquivo=obras&obrid={$obra['obrid']}\",\"imagem\",\"width=850,height=600,resizable=yes\");' src='../slideshow/slideshow/verimagem.php?_sisarquivo=obras&newwidth=120&newheight=90&arqid=".$fotos[$k]["arqid"]."' hspace='10' vspace='3' style='width:80px; height:80px;'/><br />".$fotos[$k]["aqodtinclusao"]."<br />".$fotos[$k]["arqdescricao"];
				echo "</td>";
			}
		} else {
			echo "<td>Não existe(m) foto(s) cadastrada(s).</td>";
		}
		echo "</tr></table></td></tr>";
		echo "</table>
		</td></tr>";
	}
} else {
	echo "<tr><td>Não existem obras cadastradas</td></tr>";
}
					
?>
				</table>
		</td>
	</tr>
</table>
</body>
</html>