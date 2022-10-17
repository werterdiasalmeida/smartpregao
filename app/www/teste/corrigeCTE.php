<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "www/cte/_constantes.php";
include APPRAIZ . "www/cte/_funcoes.php";

$db = new cls_banco();

header( 'Content-Type: text/plain' );

$sql = "
	select
		i.inuid
	from cte.instrumentounidade i
		inner join workflow.documento d on
			d.docid = i.docid
	where
		i.itrid = "  . INSTRUMENTO_DIAGNOSTICO_MUNICIPAL . " and -- instrumento municipal
		d.esdid = " . CTE_ESTADO_ANALISE . " and -- em fase de analise
		i.inuid not in ( -- sem cópia gerada
			select
				inuid
			from cte.pontuacao
			where
				ptostatus = 'C'
			group by
				inuid
		)
	group by
		i.inuid order by inuid
";
$linhas = $db->carregar( $sql );
$linhas = $linhas ? $linhas : array();
echo date( "Y-m-d H:i:s" ) . "\n\n";
foreach ( $linhas as $linha )
{
	$inuid = (integer) $linha['inuid'];
	echo "inuid: " . $inuid . "\n";
	ob_flush();
	flush();
	cte_copiarPlanoAcao( $inuid );
}
echo "\n" . date( "Y-m-d H:i:s" ) . "\n";

$db->commit();

?>