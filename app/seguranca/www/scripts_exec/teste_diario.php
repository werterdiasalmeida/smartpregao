<?php
// carrega as funções gerais
include_once "../../global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

date_default_timezone_set ('America/Sao_Paulo');

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SISTEMA DE PROCESSAMENTO DE AGENDAMENTOS";
$mensagem->From 		= "simec@mec.gov.br";
$mensagem->AddAddress("alexandre.dourado@mec.gov.br", "Alexandre Dourado");

for($i=0;$i<10;$i++) {
	
	$mensagem->Subject = "Mensagem diaria ".($i+1);
	$mensagem->Body = "msg padrao (teste_diario.php)";
	$mensagem->IsHTML( true );
	$mensagem->Send();
}

sleep(20);
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */	

?>