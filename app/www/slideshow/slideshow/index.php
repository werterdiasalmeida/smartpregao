<?

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';
// abre conexão com o servidor de banco de dados
$db = new cls_banco();
function buscarFotos($tabelacontrole, $filtro, $pagina = 0) {
	global $db;
	if(!$pagina) $pagina = 0;
	
	$sql = "SELECT arq.arqid, arq.arqdescricao, arqdata FROM ". $tabelacontrole ." AS cnt 
			LEFT JOIN public.arquivo AS arq ON arq.arqid = cnt.arqid 
			WHERE ". $filtro." AND  
			(arqtipo = 'image/jpeg' OR
			 arqtipo = 'image/gif' OR
			 arqtipo = 'image/png') 
			ORDER BY arq.arqid
			LIMIT 16 OFFSET ".($pagina*16);
	return $db->carregar($sql);
}
function buscarTotalRegistros($tabelacontrole, $filtro) {
	global $db;
	$sql = "SELECT COUNT(arq.arqid) AS totalregistros FROM ". $tabelacontrole ." AS cnt 
			LEFT JOIN public.arquivo AS arq ON arq.arqid = cnt.arqid 
			WHERE ". $filtro . " AND 
			(arqtipo = 'image/jpeg' OR
			 arqtipo = 'image/gif' OR
			 arqtipo = 'image/png')";
	return current($db->carregar($sql));
}

?>



<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>Galeria de fotos</title>
<link rel="stylesheet" href="../_common/css/main.css" type="text/css" media="all">
<link href="slideshow.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="../_common/js/mootools.js"></script>
<script type="text/javascript" src="../utils/backgroundSlider.js"></script>
<script type="text/javascript" src="slideshow.js"></script>
</head>
<body>
	<input type=hidden id=fotoinicio value="-1">
	<div id="container">
		<div id="example">
		<div id="slideshowContainer" class="slideshowContainer"></div>
			<div id="thumbnails">
<?
if($_SESSION['imgparams']['tabela']) {
	$fotos = buscarFotos($tabela = $_SESSION['imgparams']['tabela'], $filtro = $_SESSION['imgparams']['filtro'], $pagina = $_REQUEST['pagina']);
} else {
	echo "<script>
			alert('Erro de parâmetro da sessão. Acesse a área \"FALE CONOSCO\" no rodapé da página e entre em contato com o suporte.');
			window.close();
		  </script>";
}
if($fotos) {
	$i = -1;
	foreach($fotos as $foto) {
		$vjscript[] = "show.descricao[". $i ."] = '". preg_replace("/\r\n|\n|\r/", "<br>", addslashes($foto['arqdescricao'])) ."<br><b>Data de inclusão:</b> ".formata_data($foto['arqdata'])."';";

?>
		<a href="verimagem.php?newwidth=640&newheight=480<? echo (($_REQUEST["_sisarquivo"])?"&_sisarquivo=".$_REQUEST["_sisarquivo"]:""); ?>&arqid=<? echo $foto['arqid']; ?>" class="slideshowThumbnail"><img src="verimagem.php?newwidth=50&newheight=50<? echo (($_REQUEST["_sisarquivo"])?"&_sisarquivo=".$_REQUEST["_sisarquivo"]:""); ?>&arqid=<? echo $foto['arqid']; ?>" id="<? echo $foto['arqid']; ?>" onclick="show.paused = true; document.getElementById('bplay').style.display = 'inline'; document.getElementById('pause').style.display = 'none';"  border="0" /></a>
<?
		if($foto['arqid'] == $_REQUEST['arqid']) echo "<script>document.getElementById(\"fotoinicio\").value = ". $i .";</script>";
		$i++;
	}
	$total = buscarTotalRegistros($tabela = $_SESSION['imgparams']['tabela'], $filtro = $_SESSION['imgparams']['filtro']);
	for($i = 0; $i < ceil(current($total)/16); $i++ ) {
		$page[] = "<a href=index.php?pagina=". $i .(($_REQUEST["_sisarquivo"])?"&_sisarquivo=".$_REQUEST["_sisarquivo"]:"").">".(($i==$_REQUEST['pagina'])?"<b>".($i+1)."</b>":($i+1))."</a>";
	}
	
}
?>
			<p style="text-align: center; ">
			<?
			if(count($page) > 1) { 
				echo implode(" | ", $page);
			} 
			?></p>
			<br />
			<form method="post" action="../../geral/downloadfiles.php" target="popup" onsubmit="window.open('', 'popup', 'width=5,height=5');" id="download">
			<p style="text-align: center;"><input type="image" src="../_common/img/bdownload.jpg" title="Baixar todas"><br /><b>Baixar todas</b></a></p>
			<? 
			if($fotos[0]) { 
				foreach($fotos as $fot) {
					echo "<input type='hidden' name='fotosselecionadas[]' value='".$fot['arqid']."'>";				
				}
			} 
			?>
			</form>
		  </div>
			<script type="text/javascript">
		  	window.addEvent('domready',function(){
				var obj = {
					wait: 3000, 
					effect: 'fade',
					duration: 1000, 
					loop: true, 
					thumbnails: true,
					backgroundSlider: true,
					onClick: false
				}
				show = new SlideShow('slideshowContainer','slideshowThumbnail',obj);
				<? echo ((count($vjscript) > 0)?implode("", $vjscript):''); ?>
				show.play();
			});
		  </script>
		</div>
	</div>
	<div id="rodape">
	<img src="../_common/img/bplay.jpg" id="bplay" title="Play" onclick="show.paused = false; this.style.display = 'none'; document.getElementById('pause').style.display = 'inline'; show.play(); return false;" border="0"> <img src="../_common/img/bpause.jpg"  onclick="this.style.display = 'none'; document.getElementById('bplay').style.display = 'inline'; show.stop(); show.paused = false; return false;" id="pause" style="display:none;" border="0"> <img src="../_common/img/bprevious.jpg" title="Voltar foto" onclick="show.previous(); document.getElementById('bplay').style.display = 'inline'; document.getElementById('pause').style.display = 'none'; show.paused = true; return false;" border="0"> <img src="../_common/img/bnext.jpg" onclick="show.play(); document.getElementById('bplay').style.display = 'inline'; document.getElementById('pause').style.display = 'none'; show.paused = true; return false;" border="0"> <img src="../_common/img/bdownload.jpg" border="0" title="Download da foto" onclick="window.open('../../geral/downloadfiles.php?enderecoabsolutoarquivo='+show.images[show.image], 'popup', 'width=5,height=5');">
	</div>
	<div class="descricao" id="descricaoimagem"></div>
</body>
</html>