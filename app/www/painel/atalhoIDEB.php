<?
//include_once "config.inc";
//include APPRAIZ . "includes/Snoopy.class.php";
if($_REQUEST['ideb']){  
		if($_REQUEST['muncodcompleto'] && $_REQUEST['mundescricao'] && $_REQUEST['muncod']) {
			$daurl = "http://ideb.inep.gov.br/Site/ContentManager2009.php?tipo=municipio&cod=".$_REQUEST['muncod'];
			//$daurl = "http://ideb.inep.gov.br/Site/ContentManager.php?Area=tabMun&mun={$_REQUEST['muncodcompleto']}-{$_REQUEST['mundescricao']}";
//			$snoopy = new Snoopy;
//			$snoopy->agent = "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)";
//			$url = $daurl;
//			$snoopy->fetch($url);
//			echo $snoopy->results;
			
		}
		if($_REQUEST['estcod']) {
			$daurl = "http://ideb.inep.gov.br/Site/ContentManager2009.php?tipo=estado&cod=".$_REQUEST['estcod'];
			//$daurl ="http://ideb.inep.gov.br/Site/ContentManager.php?Area=tabUF&estado=".$_REQUEST['estcod'];
//			$snoopy = new Snoopy;
//			$snoopy->agent = "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)";
//			$url = $daurl;
//			$snoopy->fetch($url);
//			echo str_replace(array("($('estado')[$('estado').selectedIndex].innerHTML)","$('nomeEstadoHolder')"),array("'".$_REQUEST['estdescricao']."'","document.getElementById('nomeEstadoHolder')"),$snoopy->results);
		}
		if($_REQUEST['esccodinep']) {
			$daurl = "http://ideb.inep.gov.br/Site/ContentManager2009.php?tipo=escola&cod=".$_REQUEST['esccodinep'];
			//$daurl ="http://ideb.inep.gov.br/Site/ContentManager.php?Area=tabUF&estado=".$_REQUEST['estcod'];
//			$snoopy = new Snoopy;
//			$snoopy->agent = "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)";
//			$url = $daurl;
//			$snoopy->fetch($url);
//			echo str_replace(array("($('estado')[$('estado').selectedIndex].innerHTML)","$('nomeEstadoHolder')"),array("'".$_REQUEST['estdescricao']."'","document.getElementById('nomeEstadoHolder')"),$snoopy->results);
		}
		header("Location: $daurl");
		die;
	}
?>