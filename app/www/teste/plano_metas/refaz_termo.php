<?php

set_time_limit( 0 );

function msg( $msg )
{
	//return;
	echo $msg . "\n";
	flush();
	ob_flush();
}

header( "Content-Type: text/plain;" );

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

include APPRAIZ . "www/cte/_funcoes.php";

error_reporting( E_ALL );

$db = new cls_banco();

$sql = "
	select
		t.inuid,
		m.estuf,
		m.mundescricao
	from cte.termo t
		inner join cte.instrumentounidade i on i.inuid = t.inuid
		inner join territorios.municipio m ON m.muncod = i.muncod and m.estuf = i.mun_estuf
	where i.itrid = 2
	order by
		m.mundescricao,
		m.estuf
";

$sql = "
	select
		i.inuid,
		m.estuf,
		m.mundescricao
	from cte.instrumentounidade i 
		inner join territorios.municipio m ON m.muncod = i.muncod and m.estuf = i.mun_estuf
		inner join workflow.documento d ON d.docid = i.docid
	where i.itrid = 2 and ( d.esdid = 13 or d.esdid = 14 or d.esdid = 15 )
	order by
		m.mundescricao,
		m.estuf
";

$termos = $db->carregar( $sql );
$termos = $termos ? $termos : array();

while ( ob_get_level() > 1 )
{
	ob_end_flush();
}

msg( "Atualização de termos iniciada\n" );

msg( "Removendo termos atuais ...\n" );

$sql = "delete from cte.termosubacaoindicador";
$db->executar( $sql );

$sql = "delete from cte.termo";
$db->executar( $sql );

foreach ( $termos as $termo )
{
	msg( $termo["mundescricao"] . "/" . $termo["estuf"] . " ..." );
	cte_emitirTermo( $termo["inuid"] );
}

//$db->rollback();
$db->commit();
msg( "\nAtualização de termos finalizada!" );
