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

$sql = "select max(e.entid) as entp, count(e.entid), e2.entid, e2.entnome from entidade.entidade e 
		left join entidade.funcaoentidade f on f.entid=e.entid 
		left join entidade.funentassoc fe on fe.fueid=f.fueid 
		left join entidade.entidade e2 on e2.entid=fe.entid 
		left join entidade.funcaoentidade f2 on f2.entid=e2.entid 
		where f.funid=2 and f2.funid=1 
		group by e2.entid, e2.entnome
		having count(e.entid)>1";

$ents = $db->carregar($sql);

if($ents[0]) {
	$HTML .= "<pre>";
	foreach($ents as $en) {
		$HTML .= ":: ".$en['entnome']."__<br>";
		
		$sql = "select f.fueid from entidade.entidade e 
				left join entidade.funcaoentidade f on f.entid=e.entid 
				left join entidade.funentassoc fe on fe.fueid=f.fueid 
				left join entidade.entidade e2 on e2.entid=fe.entid 
				left join entidade.endereco en2 on en2.entid=e2.entid
				left join entidade.funcaoentidade f2 on f2.entid=e2.entid 
				where e2.entid=".$en['entid']." and f.funid=2 and f2.funid=1 and e.entid!=".$en['entp'];
		
		$fue = $db->carregar($sql);
		
		unset($fueids);
		if($fue[0]) {
			foreach($fue as $f) {
				$fueids[] = $f['fueid'];
			}
			
		}
		if($fueids) {
			$sql = "delete from entidade.funentassoc where feaid in(select feaid from entidade.funentassoc f 
					left join entidade.funcaoentidade fe on fe.entid=f.entid
					where f.fueid in ('".implode("','", $fueids)."') and fe.funid=1)";
			echo $sql."<br>";
			$db->executar($sql);
			
		}
		
		$db->commit();
		

		

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
$mensagem->FromName		= "LIMPA ENTIDADES 4";
$mensagem->From 		= "simec@mec.gov.br";
$mensagem->AddAddress( "alexandre.dourado@mec.gov.br", "Alexandre Dourado" );
$mensagem->Subject = "Limpando entidades 4";
$mensagem->Body = $HTML;
$mensagem->IsHTML( true );
$mensagem->Send();

?>