<?php
//$_REQUEST['baselogin'] = "simec_espelho_producao";

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


$_TPCID = array('MUNICIPAL' => '3',
				'FEDERAL'   => '2',
				'PRIVADA'   => '4',
				'ESTADUAL'  => '1');

$_TPSID = array('EM ATIVIDADE'            => '1',
				'PARALISADA'              => '3',
				'EXTINTA NO ANO ANTERIOR' => '2',
				'EXTINTA'                 => '2');

// pegando todas a escolas que estão cadastradas em entidadedetalhe e não estão cadastradas na entidade
$sql = "select d.entcodent, d.entid from entidade.entidadedetalhe d 
		left join entidade.entidade e on e.entid=d.entid 
		where e.entid is null LIMIT 500";

$entcodent = $db->carregar($sql);

if($entcodent[0]) {
	foreach($entcodent as $entcod) {
		$sql = "select * from educacenso_tmp.tmp_escola_inep_2009 where pk_cod_entidade='".$entcod['entcodent']."'";
		$escolacorreta = $db->pegaLinha($sql);
		
		$sql = "select * from entidade.entidade where entcodent='".$entcod['entcodent']."'";
		$jaexiste = $db->pegaLinha($sql);
		
		if($escolacorreta && !$jaexiste) {
			//inserindo entidade
			$sql = "INSERT INTO entidade.entidade(entid, entnome, entstatus, tpcid, tpsid, entcodent, entdatainclusao)
	    			VALUES ('".$entcod['entid']."', '".$escolacorreta['no_escola']."', 'A', 
	    					'".$_TPCID[$escolacorreta['tp_dependencia']]."', 
	    					'".$_TPSID[$escolacorreta['situacao']]."', 
	    					'".$entcod['entcodent']."', NOW());";
			$db->executar($sql);
			$sql = "INSERT INTO entidade.funcaoentidade(
            		funid, entid, fuedata, fuestatus)
    				VALUES ('3', '".$entcod['entid']."', NOW(), 'A');";
			$db->executar($sql);
			$sql = "INSERT INTO entidade.endereco(entid, tpeid, endcep, endlog, endcom, endbai, muncod, 
            		estuf, endnum, endstatus)
    				VALUES ('".$entcod['entid']."', '1', '".$escolacorreta['num_cep']."', 
    				'".$escolacorreta['desc_endereco']."', 
    				'".$escolacorreta['desc_endereco_complemento']."', 
    				'".$escolacorreta['desc_endereco_bairro']."', 
    				'".$escolacorreta['co_municipio']."', 
    				'".$escolacorreta['sg_uf']."', 
            		'".$escolacorreta['num_endereco']."', 'A');";
			$db->executar($sql);
			$htmlemail .= "COD.".$entcod['entcodent']." | inserido com sucesso<br/>";
		} elseif(!$escolacorreta) {
			$htmlemail .= "<font color=red>COD.".$entcod['entcodent']." | não foi encontrado</font><br/>";
		} else {
			$htmlemail .= "<font color=red>COD.".$entcod['entcodent']." | existe em entidades</font><br/>";
		}
	}
}

$db->commit();

/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "MONITORAMENTO DE DETALHES ENTIDADES";
$mensagem->From 		= "simec@mec.gov.br";
$mensagem->AddAddress( "alexandre.dourado@mec.gov.br", "Alexandre Dourado" );
$mensagem->AddAddress( "daniel.brito@mec.gov.br", "Daniel Brito" );
$mensagem->Subject = "Monitoramento de detalhes entidades";
$mensagem->Body = $htmlemail;
$mensagem->IsHTML( true );
return $mensagem->Send();
?>