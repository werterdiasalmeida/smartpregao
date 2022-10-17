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
$_SESSION['sisid'] = 4; # seleciona o sistema de segurança
$_SESSION['usucpf'] = '00000000191';
$_SESSION['usucpforigem'] = '00000000191';

$_SESSION['exercicio_atual'] = '2010';
$_SESSION['exercicio'] = '2010';

unset($_SESSION['superuser']);
$_SESSION['superuser'] = true;

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


echo "INICIO DO CONSERTO<br>";

//$sql = "SELECT indid FROM painel.indicador WHERE indstatus='A' ORDER BY indid";
$sql = "select indid, count(indid) as t from (

		select s.indid, s.sehstatus, count(*) from painel.seriehistorica s
		left join painel.indicador i on i.indid=s.indid 
		where i.indstatus='A'
		group by s.indid, s.sehstatus ORDER BY s.indid
		
		) foo 
		group by indid
		having count(indid)=1";

$indicadores = $db->carregar($sql);

foreach($indicadores as $inds) {
	/*
	 * limpando series historicas
	 */
	unset($seriemaior);
	$sql = "UPDATE painel.seriehistorica SET sehstatus='H' WHERE indid='".$inds['indid']."' AND sehstatus='A'";
	$db->executar($sql);
	$db->commit();
	$sql = "SELECT seh.sehid, seh.dpeid FROM painel.seriehistorica seh 
			LEFT JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid 
			WHERE indid='".$inds['indid']."' AND (sehstatus='A' OR sehstatus='H') ORDER BY dpedatainicio DESC, sehid DESC LIMIT 1";
	
	$seriemaior = $db->pegaLinha($sql);
	if($seriemaior) {
		$sql = "UPDATE painel.seriehistorica SET sehstatus='A' WHERE sehid='".$seriemaior['sehid']."'";
		$db->executar($sql);
		$db->commit();
		$sql = "UPDATE painel.seriehistorica SET sehstatus='I' WHERE dpeid='".$seriemaior['dpeid']."' AND indid='".$inds['indid']."' AND sehid!='".$seriemaior['sehid']."'";
		$db->executar($sql);
		$db->commit();
		$sql = "SELECT * FROM painel.seriehistorica WHERE indid='".$inds['indid']."' AND sehstatus='H' ORDER BY sehid DESC";
		$serieoutros = $db->carregar($sql);
		unset($dpe,$serieoutros);
		if($serieoutros[0]) {
			foreach($serieoutros as $ser) {
				if($dpe[$ser['dpeid']]) {
					$sql = "UPDATE painel.seriehistorica SET sehstatus='I' WHERE sehid='".$ser['sehid']."'";
					$db->executar($sql);
					$db->commit();
				} else {
					$dpe[$ser['dpeid']]=true;
				}
			}
		}
	}
	/*
	 * limpando series historicas
	 */
}


echo "FIM DO CONSERTO";


?>