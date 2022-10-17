<?php
date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(1200);
/* FIM configurações */


// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once '_funcoesagendamentoindicador.php';

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "SELECT * FROM painel.detalhedadoindicador ORDER BY indid, ddiordem";
$detalhes = $db->carregar($sql);
if($detalhes[0]) {
	foreach($detalhes as $d) {
		$detalhesnor[$d['indid']][] = $d;
	}
}

if($detalhesnor) {
	foreach($detalhesnor as $indid => $dados) {
		$sql = "INSERT INTO painel.detalhetipoindicador(indid, tdistatus, tdiordem, tdidsc, tdinumero)
    	 		VALUES ('".$indid."', 'A', '1', 'DETALHES #".$indid."', '1') RETURNING tdiid;";
		
		$tdiid = $db->pegaUm($sql);
		
		if($dados) {
			foreach($dados as $d1) {
				$sql = "INSERT INTO painel.detalhetipodadosindicador(tdiid, tiddsc, tidstatus)
    					VALUES ('".$tdiid."', '".$d1['ddidsc']."', 'A') RETURNING tidid;";
				$tidid = $db->pegaUm($sql);
				$sql = "UPDATE painel.detalheseriehistorica
   						SET tidid1='".$tidid."'
 						WHERE ddiid='".$d1['ddiid']."';";
				$db->executar($sql);
			}
		}
		$db->commit();
	}
}
echo "FIM...";

?>