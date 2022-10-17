<?php
date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";
//$_REQUEST['baselogin'] = "simec_desenvolvimento";


// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);
/* FIM configurações */

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once '_constantes.php';
include_once '_funcoesagendamentoindicador.php';

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


echo "INICIO DO CONSERTO<br>";

$sql = "SELECT indid FROM painel.indicador WHERE indstatus='A' ORDER BY indid";
$indicadores = $db->carregar($sql);

foreach($indicadores as $inds) {
	unset($sehcons,$sehbkcons);
	$sql = "SELECT * FROM painel.seriehistorica WHERE indid='".$inds['indid']."' AND (sehstatus='A' OR sehstatus='H') ORDER BY sehdtcoleta";
	$seh = $db->carregar($sql);
	
	if($seh[0]) {
		foreach($seh as $s) {
			$sehcons[$s['dpeid']] = $s;
		}
	}
	
	$sql = "SELECT * FROM painel.seriehistorica_backup WHERE indid='".$inds['indid']."' ORDER BY sehdtcoleta";
	$sehbkp = $db->carregar($sql);
	
	if($sehbkp[0]) {
		foreach($sehbkp as $sbk) {
			$sehbkcons[$sbk['dpeid']] = $sbk;
		}
	}

	echo "-----------------------------------------------------------------------------<br>";
	
	if($sehbkcons) {
		foreach($sehbkcons as $sehbkl) {
			if(!$sehcons[$sehbkl['dpeid']]) {
				echo "Indicador ID: ".$sehbkl['indid']." _________ Periodicidade ID: ".$sehbkl['dpeid']."<br>";
				if($_REQUEST['executar']) {
					$sql = "INSERT INTO painel.seriehistorica(
				            sehid, indid, sehvalor, sehstatus, sehqtde, dpeid, sehdtcoleta, 
				            regid) 
	            			SELECT distinct sehid, indid, sehvalor, 'H' as sehstatus, sehqtde, dpeid, sehdtcoleta, regid FROM painel.seriehistorica_backup where sehid='".$sehbkl['sehid']."'";
					
					$db->executar($sql);
					
					$sql = "INSERT INTO painel.detalheseriehistorica(
				            dshid, ddiid, sehid, dshvalor, dshcod, dshcodmunicipio, dshuf, 
	            			dshqtde, tidid1, tidid2, iepid, entid, unicod) SELECT distinct * from painel.detalheseriehistorica_backup where sehid='".$sehbkl['sehid']."'";
					
					$db->executar($sql);
					
					$db->commit();
				}
			}
		}
	}
	
}


echo "FIM DO CONSERTO";


?>