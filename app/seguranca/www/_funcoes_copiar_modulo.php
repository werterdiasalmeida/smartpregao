<?php
function getDadosSistema($sisid = null)
{
	global $db;
	
	$sisid = !$sisid ? 1 : $sisid;
	
	$sql = sprintf("select
						sisid,
						sisdsc,
						sisabrev,
						sisdiretorio,
						sisfinalidade
					from
						seguranca.sistema
					where
						sisstatus = 'A'
					and
						sisid = %d" , $sisid );
	return $db->pegaLinha($sql);
	
}

function getTabelasSistema($sisid = null)
{
	
	global $db;
	
	$sisid = !$sisid ? 1 : $sisid;
	
	$sql = "SELECT 
				'<input type=\"checkbox\" name=\"chk_tbl[' || lower(c.relname) || ']\" id=\"tbl_' || lower(c.relname) || '\"  />' as acao,
				lower(c.relname) as tabela  
			FROM 
				pg_namespace n, pg_class c
			WHERE 
				n.oid = c.relnamespace
			AND
				c.relkind = 'r'     -- no indices
			AND
				n.nspname not like 'pg\\_%' -- no catalogs
			AND
				n.nspname != 'information_schema' -- no information_schema
			AND
				n.nspname = ( select sisdiretorio from seguranca.sistema where sisid = $sisid)
			ORDER BY 
				nspname,
				relname";
	return $db->carregar($sql);
	
}

function donwloadScript($arrRequest)
{
	global $db;
	
	$dadosSistema = getDadosSistema($arrRequest['combo_sisid']);
	
	//header( 'Content-type: application/sql; charset=UTF-8');
    //header( 'Content-Disposition: attachment; filename='.$dadosSistema['sisdiretorio'].'.sql');
	
    $arrPgDump = getPgDump($dadosSistema,$arrRequest);
    dbg($arrPgDump);
	
}

function getPgDump($dadosSistema,$arrRequest)
{
	
	if (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN') {
	    $path = "\"C:\Arquivos de programas\PostgreSQL\8.4\bin\pg_dump\"";
	} else {
	    $path = "pg_dump";
	}
	$path = "pg_dump";
        
	$arrExec[] = "$path -s -n {$dadosSistema['sisdiretorio']} -h mecsrv168 -U simec simec_desenvolvimento";
	//$arrExec[] = "$path -s -n {$dadosSistema['sisdiretorio']} -h localhost -U root -w simec";
	
	if(is_array($arrRequest['chk_tbl'])){
		foreach( $arrRequest['chk_tbl'] as $table => $key){
			$arrExec[] = "$path --inserts -n {$dadosSistema['sisdiretorio']} -a -t {$dadosSistema['sisdiretorio']}.$table -h mecsrv168 -U simec simec_desenvolvimento";
		}
	}
	
	foreach($arrExec as $k => $exec){
		exec($exec,$output[$k],$return);
		dbg($exec);
	}
	if(is_array($output)){
		foreach($output as $arrO){
			foreach($arrO as $o){
				//$arrReturn[] = utf8_decode($o)."\n";
				$arrReturn[] = utf8_decode($o)."<br />";
			}
		}
	}else{
		$arrReturn[] = $output;
	}
	return $arrReturn;
	
}