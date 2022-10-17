<?php


require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "www/cte/_constantes.php";
include APPRAIZ . "www/cte/_funcoes.php";

header( 'Content-Type: text/plain' );

$db = new cls_banco();

$sql = "
select
p.ptoid
from
cte.dimensao d
inner join cte.areadimensao a ON a.dimid = d.dimid
inner join cte.indicador i ON i.ardid = a.ardid
inner join cte.criterio c ON c.indid = i.indid
inner join cte.pontuacao p ON p.indid = i.indid and p.crtid = c.crtid
inner join cte.instrumentounidade iu ON iu.inuid = p.inuid
inner join cte.acaoindicador ac ON ac.ptoid = p.ptoid
left join cte.subacaoindicador s ON s.aciid = ac.aciid
where d.itrid = 2 and iu.itrid = 2 and ( c.ctrpontuacao = 3 or c.ctrpontuacao = 4 )
group by p.ptoid
";
$linhas = $db->carregar( $sql );
foreach ( $linhas as $linha )
{
	$ptoid = $linha['ptoid'];
	echo $ptoid . "\n";
	cte_removerPlano( $ptoid );
}

$db->commit();



?>