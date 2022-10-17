<?php
function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 


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
set_time_limit(0);
/* FIM configurações */


// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if(!$_REQUEST['duplinum']) $_REQUEST['duplinum']='10';

$Tinicio = getmicrotime();

/*
 * Carregando a estrutura de UPDATEs que deverão ser realizados 
 * para atualizar
 */
$sql = "select c.table_schema, c.table_name, c.column_name from information_schema.constraint_column_usage b
		inner join information_schema.key_column_usage c on b.constraint_name = c.constraint_name
		where b.column_name='entid' and b.table_name='entidade' AND c.table_schema <> 'entidade' and c.column_name not in ('cmpid')
		GROUP BY b.constraint_name, b.table_schema, b.table_name, b.column_name, c.table_schema, c.table_name, c.column_name
		order by 1";

$est = $db->carregar($sql);

/*
$sql = "select entnome, entnumcpfcnpj, count(funid), max(entidpadrao) as entidpadrao, count(distinct entcodent) as codent from (

		select trim(UPPER(ent.entnome)) as entnome, ent.entnumcpfcnpj, ent.entcodent, funid,
				count(ent.entid), max(ent.entid) as entidpadrao
				from entidade.entidade ent 
				left join entidade.funcaoentidade f on ent.entid=f.entid 
				left join entidade.funentassoc fe on fe.fueid=f.fueid
				left join entidade.entidade e2 on e2.entid=fe.entid
				where ent.entstatus='A' and ent.entnome is not null and funid is not null and ent.entnumcpfcnpj is not null 
				group by trim(UPPER(ent.entnome)), 
				ent.entnumcpfcnpj, ent.entcodent, funid
				order by trim(UPPER(ent.entnome)) 
		
		) foo  
		group by entnome, entnumcpfcnpj 
		having count(funid)> 1  and count(distinct entidpadrao)>1 and count(distinct entcodent)=0";
*/

$sql = "select trim(UPPER(entnome)) as entnome, entnumcpfcnpj, count(trim(UPPER(entnome))) ,max(entidpadrao) as entidpadrao from (

		select trim(UPPER(ent.entnome)) as entnome, ent.entnumcpfcnpj, ent.entcodent, funid,
				count(ent.entid), max(ent.entid) as entidpadrao
				from entidade.entidade ent 
				left join entidade.funcaoentidade f on ent.entid=f.entid 
				left join entidade.funentassoc fe on fe.fueid=f.fueid
				left join entidade.entidade e2 on e2.entid=fe.entid
				where ent.entstatus='A' and ent.entnome is not null and fe.fueid is null and funid is not null and ent.entnumcpfcnpj is not null
				group by trim(UPPER(ent.entnome)), 
				ent.entnumcpfcnpj, ent.entcodent, funid
				having count(ent.entid)>1
				order by trim(UPPER(ent.entnome))
		
		) foo 
		group by trim(UPPER(entnome)), entnumcpfcnpj 
		having count(trim(UPPER(entnome)))>1";
$ents = $db->carregar($sql);

if($ents[0]) {
	$HTML .= "<pre>";
	foreach($ents as $en) {
		$HTML .= ":: ".$en['entnome']."__".$en['entnumcpfcnpj']."<br>";
		
		$sql = "select f.fueid, f.funid, fe.entid from entidade.entidade e 
				left join entidade.funcaoentidade f on e.entid=f.entid 
				left join entidade.funentassoc fe on fe.fueid=f.fueid 
				where e.entid=".$en['entidpadrao'];
		
		$fuess = $db->carregar($sql);
		
		unset($cfunid);
		
		if($fuess[0]) {
			foreach($fuess as $fue) {
				$cfunid[$fue['funid']] = array("fueid" => $fue['fueid'], "entidassoc" => $fue['entid']);
			}
		}
		
		$sql = "select e.entid as ent, f.fueid, f.funid, fe.entid from entidade.entidade e 
				left join entidade.funcaoentidade f on e.entid=f.entid 
				left join entidade.funentassoc fe on fe.fueid=f.fueid 
				where entnome ilike '".$en['entnome']."' and entnumcpfcnpj='".$en['entnumcpfcnpj']."' and e.entid!='".$en['entidpadrao']."'";

		$fuess = $db->carregar($sql);
		
		unset($entantigos);
		
		if($fuess[0]) {
			foreach($fuess as $fue) {
				
				$entantigos[] = $fue['ent'];
				
				if($fue['funid']) {
					if(!$cfunid[$fue['funid']]) {
						
						$sql = "update entidade.funcaoentidade set entid='".$en['entidpadrao']."' WHERE fueid='".$fue['fueid']."'";
						$db->executar($sql);
						
						$cfunid[$fue['funid']] = array("fueid" => $fue['fueid'], "entidassoc" => $fue['entid']);
						
					}
				}
			}
		}
		
		$db->commit();
		
		if(count($entantigos)>0) {
		
			if(count($entantigos)==1) {
				$clausula_ = "='".$entantigos[0]."'";
			} else {
				$clausula_ = "IN('".implode("','",$entantigos)."')";
			}
			
			// atualizar tabelas com vinculos as entidades antigas
			if($est[0]) {
				foreach($est as $es) {	
					$sql = "SELECT ".$es['column_name']." FROM ".$es['table_schema'].".".$es['table_name']." WHERE ".$es['column_name']." ".$clausula_;
					$existe = $db->pegaUm($sql);
					
					if($existe) {
						$sql = "UPDATE ".$es['table_schema'].".".$es['table_name']." SET ".$es['column_name']."='".$en['entidpadrao']."' WHERE ".$es['column_name']." ".$clausula_.";";
						$res = $db->executar($sql);
						$HTML .= "&nbsp;&nbsp; |=> Na estrutura ".$es['table_schema'].".".$es['table_name']." foram atualizados: ".pg_affected_rows($res)."<br>";
					}
				}
			}
			$existe_d = $db->pegaUm("SELECT foeid FROM entidade.formacaoentidade where entid ".$clausula_);
			if($existe_d) {
				$sql = "UPDATE entidade.formacaoentidade SET entid='".$en['entidpadrao']."' WHERE entid ".$clausula_.";";
				$db->executar($sql);
			}			
			$existe_d = $db->pegaUm("SELECT vpeid FROM entidade.vinculoprofissionalentidade where entid ".$clausula_);
			if($existe_d) {
				$sql = "UPDATE entidade.vinculoprofissionalentidade SET entid='".$en['entidpadrao']."' WHERE entid ".$clausula_.";";
				$db->executar($sql);
			}			
			
			
			$existe_d = $db->pegaUm("SELECT hstid FROM entidade.historico where entid ".$clausula_);
			if($existe_d) {
				$sql = "delete from entidade.historico where entid ".$clausula_.";";
				$res = $db->executar($sql);
				$HTML .= "&nbsp;&nbsp; |=> Deletando entidade.historico: ".trim(pg_affected_rows($res))."<br>";
			}
			$existe_d = $db->pegaUm("SELECT endid FROM entidade.entidadeendereco where entid ".$clausula_);
			if($existe_d) {
				$sql = "delete from entidade.entidadeendereco where entid ".$clausula_.";";
				$res = $db->executar($sql);
				$HTML .= "&nbsp;&nbsp; |=> Deletando entidade.entidadeendereco: ".pg_affected_rows($res)."<br>";
			}
			$existe_d = $db->pegaUm("SELECT endid FROM entidade.endereco where entid ".$clausula_);
			if($existe_d) {
				$sql = "delete from entidade.endereco where entid ".$clausula_.";";
				echo $sql."<br>";
				$res = $db->executar($sql);
				$HTML .= "&nbsp;&nbsp; |=> Deletando entidade.endereco: ".pg_affected_rows($res)."<br>";
			}
			$existe_d = $db->carregar("SELECT fueid FROM entidade.funcaoentidade where entid ".$clausula_);
			if($existe_d[0]) {
				
				unset($fuied);
				foreach($existe_d as $d) {
					$fuied[] = $d['fueid'];
				}
				
				$sql = "delete from entidade.funentassoc where fueid IN('".implode("','",$fuied)."');";
				$res = $db->executar($sql);
				$HTML .= "&nbsp;&nbsp; |=> Deletando entidade.funentassoc: ".trim(pg_affected_rows($res))."<br>";
				
				$sql = "delete from entidade.funcaoentidade where entid ".$clausula_.";";
				$res = $db->executar($sql);
				$HTML .= "&nbsp;&nbsp; |=> Deletando entidade.funcaoentidade: ".trim(pg_affected_rows($res))."<br>";
			}
			
			$sql = "delete from entidade.entidade where entid ".$clausula_.";";
			$res = $db->executar($sql);
			$HTML .= "&nbsp;&nbsp; |=> Deletando entidade.entidade: ".pg_affected_rows($res)."<br>";
			
			$db->commit();
		
		}
		

	}
} else {
	$HTML .= "Não existem entidade duplicadas com o mesmo nome, cpf/cnpj e função<br>";
}


$db->commit();


$Tfim = getmicrotime() - $Tinicio;

$HTML .= "<br/>Executando em ".$Tfim." segundos";


require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "LIMPA ENTIDADES 3";
$mensagem->From 		= "simec@mec.gov.br";
$mensagem->AddAddress( "alexandre.dourado@mec.gov.br", "Alexandre Dourado" );
$mensagem->Subject = "Limpando entidades 3";
$mensagem->Body = $HTML;
$mensagem->IsHTML( true );
$mensagem->Send();

?>