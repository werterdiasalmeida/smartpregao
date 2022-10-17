<?php
/******************************************************************************
* SCRIPT PARA ATUALIZAR O MNUIDPAI A PARTIR DO MNUCODPAI DE UM SISTEMA        *
*                                                                             *
* DESENVOLVIDO POR ADONIAS MALOSSO <MALOSSO@GMAIL.COM>                        *
*                                                                             *
******************************************************************************/

include "config.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/classes_simec.inc";

//defina o sisid
define("SISID", 4);

$sqlSelMenuIdPai = "SELECT DISTINCT pai.mnuid AS idpai
	FROM seguranca.menu pai
	INNER JOIN seguranca.menu filho
		ON pai.mnucod = filho.mnucodpai
	WHERE pai.sisid = %d AND filho.mnucodpai = %d";

$sqlSelMenuIds = "SELECT mnuid, mnucodpai FROM seguranca.menu WHERE sisid = %d";
$sqlUpdMenuIdPai = "UPDATE seguranca.menu SET mnuidpai = %d WHERE mnuid = %d";

$conn = pg_connect("host=localhost dbname=simec user=phpsimec password=pgphpsimecspo");

pg_query("BEGIN");
$sql = sprintf($sqlSelMenuIds, SISID);
$rs = pg_query($conn, $sql);

while($row = pg_fetch_assoc($rs)) {
	if(!(int)$row["mnucodpai"]) continue;
	$sql = sprintf($sqlSelMenuIdPai, SISID, $row["mnucodpai"]);
	
	$pai = pg_query($conn, $sql);
	$paiid = array_shift(pg_fetch_assoc($pai));
	
	$sql = sprintf($sqlUpdMenuIdPai, $paiid, $row["mnuid"]);
	pg_query($sql);
}

$rs = pg_query("SELECT mnucod, mnucodpai, mnuid, mnuidpai FROM seguranca.menu WHERE sisid = " . SISID);
while($row = pg_fetch_assoc($rs)) {
	dbg($row);
}
pg_query("COMMIT");


// insert into seguranca.perfilmenu select '25','A',mnuid from seguranca.menu where sisid=4

// Insert into seguranca.perfil (pfldsc,pfldatainicio,pfldatafim,pflstatus,pflresponsabilidade,pflsncumulativo,pflfinalidade,pflnivel,pfldescricao,sisid,pflsuperuser) VALUES('Super usuário',Null,Null,'A','N','0',Null,1,Null,4,'1')
?>