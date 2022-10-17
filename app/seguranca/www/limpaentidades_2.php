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

/* Busca todas as entidades que não estão sendo utilizadas em nenhuma tabela 
 * 
 * Ação a ser feita
 * ** deletar todas as referencias no esquema entidade 
 */
$sql = "select e.entid, e.entnome, e.entnumcpfcnpj 
		from entidade.entidade e 
		left join entidade.funcaoentidade f on f.entid=e.entid 
		where length(entnumcpfcnpj)=11 and f.fueid is null limit 5000";

$ents = $db->carregar($sql);

if($ents[0]) {
	$HTML .= "<pre>";
	foreach($ents as $en) {
		// atualizar tabelas com vinculos as entidades antigas
		$deletarentidade = true;
		if($est[0]) {
			foreach($est as $es) {	
				$sql = "SELECT ".$es['column_name']." FROM ".$es['table_schema'].".".$es['table_name']." WHERE ".$es['column_name']."='".$en['entid']."'";
				$existe = $db->pegaUm($sql);
				if($existe) {
					$deletarentidade = false;
				}
			}
			
			$sql = "select vpeid from entidade.vinculoprofissionalentidade where entid='".$en['entid']."'";
			$existe = $db->pegaUm($sql);
			if($existe) {
				$deletarentidade = false;
			}
			
		}
		
		if($deletarentidade) {
		
			$db->executar("delete from entidade.formacaoentidade where entid='".$en['entid']."'");
			$db->executar("delete from entidade.historico where entid='".$en['entid']."'");
			$db->executar("delete from entidade.endereco where entid='".$en['entid']."'");
			$db->executar("delete from entidade.entidadeendereco where entid='".$en['entid']."'");
			$db->executar("delete from entidade.entidade where entid='".$en['entid']."'");
			$HTML .= ":: ".$en['entnome']."__".$en['entnumcpfcnpj']." removido<br>";
		
		}

		

	}
} else {
	$HTML .= "Não existem entidade duplicadas com o mesmo nome, cpf/cnpj e função<br>";
}

$Tfim = getmicrotime() - $Tinicio;

$HTML .= "<br/>Executando em ".$Tfim." segundos";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "LIMPA ENTIDADES 2";
$mensagem->From 		= "simec@mec.gov.br";
$mensagem->AddAddress( "alexandre.dourado@mec.gov.br", "Alexandre Dourado" );
$mensagem->Subject = "Limpando entidades 2";
$mensagem->Body = $HTML;
$mensagem->IsHTML( true );
$mensagem->Send();

?>