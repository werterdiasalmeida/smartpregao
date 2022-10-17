<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

/**
 * Simple function to replicate PHP 5 behaviour
 */
function microtime_float() 
{ 
    list($usec, $sec) = explode(" ", microtime()); 
    return ((float)$usec + (float)$sec); 
} 

$time_start = microtime_float();

// carrega as bibliotecas internas do sistema
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

/* configurações */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações */



$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


/*
 * VERIFICANDO DUPLICIDADE DE ENDEREÇOS
 */
$sql = "select ent.entid, ent.entnome, en.tpeid, count(en.endid) from entidade.entidade ent 
		left join entidade.endereco en on en.entid=ent.entid 
		group by ent.entid, ent.entnome, en.tpeid
		having count(en.endid) > 1";

$entidades = $db->carregar($sql);

$htmlemail .= "Endereços duplicados: ".(($entidades[0])?count($entidades):"0")."<br>";

if($_REQUEST['fix']==true) {
	if($entidades[0]) {
		foreach($entidades as $ent) {
			$sql = "SELECT endid FROM entidade.endereco WHERE entid='".$ent['entid']."' AND tpeid='".$ent['tpeid']."' ORDER BY endid DESC LIMIT 1";
			$endid = $db->pegaUm($sql);
			$sql = "DELETE FROM entidade.historico WHERE entid='".$ent['entid']."' AND endid!='".$endid."'";
			$db->executar($sql);
			$sql = "DELETE FROM entidade.endereco WHERE entid='".$ent['entid']."' AND tpeid='".$ent['tpeid']."' AND endid!='".$endid."'";
			$db->executar($sql);
		}
	}
}
$db->commit();

/*
 * VERIFICANDO NOMES DUPLICADOS MAIS DE 40X
 */


$sql = "select trim(ent.entnome) as entnome, count(ent.entid) from entidade.entidade ent 
		where entstatus='A' and entnome is not null
		group by trim(ent.entnome)
		having count(ent.entid)>".$_REQUEST['numero']." LIMIT 15";

$entidadesx = $db->carregar($sql);

$htmlemail .= "Nomes duplicados (".$_REQUEST['numero']."): ".(($entidadesx[0])?count($entidadesx):"0")."<br>";

// verificando todos os nomes duplicados
if($_REQUEST['fix']==true) {
	if($entidadesx[0]) {
		$c=1;
		foreach($entidadesx as $ent) {
			if($ent['entnome']) {
				// buscando a entidade que vai permanecer
				$sql = "SELECT entid FROM entidade.entidade WHERE entnome='".$ent['entnome']."' ORDER BY entid DESC LIMIT 1";
				$ultent = $db->pegaUm($sql);
				// pegando os dados das demais entidades
				$sql = "SELECT entid FROM entidade.entidade WHERE entnome='".$ent['entnome']."' AND entid!='".$ultent."' ORDER BY entid ASC";
				$ents = $db->carregar($sql);
				
				for($i=0;$i<count($ents);$i++) {
					$sql = "select f.funid, fe.feaid, fe.entid from entidade.funcaoentidade f 
							left join entidade.funentassoc fe on f.fueid=fe.fueid 
							where f.entid='".$ents[$i]['entid']."'";
					$ff = $db->carregar($sql);
					unset($feaidin);
					if($ff[0]) {
						foreach($ff as $funcao) {
							$sql = "select fueid from entidade.funcaoentidade where entid='".$ultent."' and funid='".$funcao['funid']."'";
							$ffi = $db->pegaUm($sql);
							if($funcao['feaid'])$feaidin[] = $funcao['feaid'];
							if(!$ffi) {
								$fueid = $db->pegaUm("insert into entidade.funcaoentidade(entid, funid) values('".$ultent."','".$funcao['funid']."') RETURNING fueid;");
								if($funcao['entid'])$db->executar("insert into entidade.funentassoc(fueid, entid) values('".$fueid."','".$funcao['entid']."')");
							}
						}
					}
					$db->executar("update cte.instrumentounidadeescola set entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update cte.qtdfisicoano SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update cte.bandalarga SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update academico.usuarioresponsabilidade SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update obras.usuarioresponsabilidade SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update pdeescola.usuarioresponsabilidade SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update rehuf.usuarioresponsabilidade SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update sig.usuarioresponsabilidade SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					
					
					$t1 = $db->pegaUm("select * from pdeescola.memaiseducacao where entid='".$ultent."'");
					if(!$t1) $db->executar("update pdeescola.memaiseducacao SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					else $db->pegaUm("delete from pdeescola.memaiseducacao where entid='".$ents[$i]['entid']."'");
					
					$db->executar("update pdeescola.meloteimpressao SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update pdeescola.mepessoal SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update pdeescola.pdeescola SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
	
					
					$db->executar("update cte.conteudoppp SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update cte.escolaproep SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update cte.bandalarga SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update cte.composicaopessoa SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update cte.entidadeexecucaosubacao SET eesentescola='".$ultent."' where eesentescola='".$ents[$i]['entid']."'");
					
					$db->executar("update cte.escolavalores SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update cte.instrumentounidadeescola SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update cte.qtdfisicoano SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					$db->executar("update cte.qtdfisicoanohistorico SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					
					$db->executar("update obras.supervisao SET supvistoriador='".$ultent."' where supvistoriador='".$ents[$i]['entid']."'");
					$db->executar("update obras.obrainfraestrutura SET entidunidade='".$ultent."' where entidunidade='".$ents[$i]['entid']."'");
					$db->executar("update obras.obrainfraestrutura SET entidigrejaconstrutora='".$ultent."' where entidigrejaconstrutora='".$ents[$i]['entid']."'");
					$db->executar("update obras.obrainfraestrutura SET entidcampus='".$ultent."' where entidcampus='".$ents[$i]['entid']."'");
					
					$db->executar("update entidade.funentassoc SET entid='".$ultent."' where entid='".$ents[$i]['entid']."'");
					
					if($feaidin) $db->executar("delete from entidade.funentassoc where feaid in('".implode("','",$feaidin)."')");
					$db->executar("delete from entidade.historico where entid='".$ents[$i]['entid']."'");
					$db->executar("delete from entidade.funcaoentidade where entid='".$ents[$i]['entid']."'");
					$db->executar("delete from entidade.endereco where entid='".$ents[$i]['entid']."'");
					$db->executar("delete from entidade.entidadeendereco where entid='".$ents[$i]['entid']."'");
					$db->executar("delete from entidade.entidade where entid='".$ents[$i]['entid']."'");
					$db->commit();
				}
			}
		}
	}
}

$time_end = microtime_float();

$time = $time_end - $time_start;

$htmlemail .= "Tempo de execução: ".$time." segundos";


/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "MONITORAMENTO DE ENTIDADES";
$mensagem->From 		= "simec@mec.gov.br";
$mensagem->AddAddress( "alexandre.dourado@mec.gov.br", "Alexandre Dourado" );
$mensagem->Subject = "Monitoramento de entidades";
$mensagem->Body = $htmlemail;
$mensagem->IsHTML( true );
return $mensagem->Send();

?>