<?php
// Corrige 

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "www/cte/_constantes.php";
include APPRAIZ . "www/cte/_funcoes.php";

$db = new cls_banco();

header( 'Content-Type: text/plain' );

function formata( $valor )
{
	return str_replace( "'", "\\'", utf8_decode( $valor ) );
}

function faz( $campo )
{
	global $db;
	
/*
	$sql = "
select
    sbaid,
	" . $campo . "
from cte.subacaoindicador
where
    " . $campo . " like '%' || chr( 10 ) || '%' or
    " . $campo . " like '%' || chr( 13 ) || '%' or
    " . $campo . " like '%Ã§Ã£%' or
    " . $campo . " like '%ÇÃ%' or
    " . $campo . " like '%Ãµ%' or
    " . $campo . " like '%çã%' or
    " . $campo . " like '%çõ%' or
    " . $campo . " like '%área%' or
    " . $campo . " like '%õ%' or
    " . $campo . " like '%ç%' or
    " . $campo . " like '%ã%' or
    " . $campo . " like '%é%' or
    " . $campo . " like '%É%' or
    " . $campo . " like '%ó%' or
    " . $campo . " like '%á%' or
    " . $campo . " like '%ê%'
";
*/

	$sql = "
select
    subacao.sbaid,
	" . $campo . "
from cte.dimensao d 
inner join cte.areadimensao area ON area.dimid = d.dimid 
inner join cte.indicador i ON i.ardid = area.ardid 
inner join cte.pontuacao p ON p.indid = i.indid and p.ptostatus = 'A' 
inner join cte.criterio c ON c.crtid = p.crtid 
inner join cte.instrumentounidade iu ON iu.inuid = p.inuid 
inner join cte.acaoindicador acao ON acao.ptoid = p.ptoid 
inner join cte.subacaoindicador subacao ON subacao.aciid = acao.aciid 
inner join cte.unidademedida u on u.undid = subacao.undid 
inner join cte.formaexecucao f on f.frmid = subacao.frmid 
left join cte.statussubacao ss on ss.ssuid = subacao.ssuid 
where iu.inuid = 736
order by d.dimcod, area.ardcod, i.indcod, subacao.oid ";

	
	$linhas = $db->carregar( $sql );
	$linhas = $linhas ? $linhas : array();
	
	foreach ( $linhas as $linha )
	{
		$sql = "
update cte.subacaoindicador subacao set " . $campo . " = '" . formata( $linha[$campo] ) . "' where subacao.sbaid = " . formata( $linha['sbaid'] ) . ";";
		echo $sql;
		//$db->executar( $sql );
	}
}

faz( 'subacao.sbastgmpl' );
faz( 'subacao.sbaprm' );
faz( 'subacao.sbauntdsc' );

//$db->commit();
$db->rollback();

