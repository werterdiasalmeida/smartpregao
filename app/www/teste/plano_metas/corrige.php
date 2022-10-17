<?php

header( "Content-Type: text/plain;" );

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

include APPRAIZ . "www/cte/_funcoes.php";

//error_reporting( E_ALL );

$db = new cls_banco();

$sql = "
	select
		p.ptoid
	from cte.pontuacao p
		inner join cte.instrumentounidade i on
			i.inuid = p.inuid
		inner join territorios.municipio m on
			m.muncod = i.muncod and
			m.estuf = i.mun_estuf
		inner join workflow.documento d on
			d.docid = i.docid
	where
		i.itrid = " . INSTRUMENTO_DIAGNOSTICO_MUNICIPAL . " and
		d.esdid in ( 1, 2 ) and
		p.ptostatus = 'A'
";

$ptoids = $db->carregar( $sql );
$ptoids = $ptoids ? $ptoids : array();

var_dump( $ptoids );

foreach ( $ptoids as $linha )
{
	$ptoid = $linha['ptoid'];
	//echo $ptoid . "\n"; 
	cte_atualizarPlanoPequenoMunicipio( $ptoid );
}

$db->commit();


?>