<?php
include_once 'config.inc';

require_once APPRAIZ . 'includes/phpmailer-5.2.21/PHPMailerAutoload.php';

$emailDestino = '';

$mail = new PHPMailer();
//$mail->SMTPDebug = 3; // Enable verbose debug output
$mail->isSMTP(); // Set mailer to use SMTP
$mail->Host = $email_host; // Specify main and backup SMTP servers
$mail->SMTPAuth = $email_auth; // Enable SMTP authentication
$mail->Username = $email_login; // SMTP username
$mail->Password = $email_pass; // SMTP password
$mail->SMTPSecure = $email_secure; // Enable TLS encryption, `ssl` also accepted
$mail->Port = $email_port; // TCP port to connect to

// https://github.com/PHPMailer/PHPMailer/wiki/Troubleshooting#php-56-certificate-verification-failure
$mail->SMTPOptions = array(
	'ssl' => array(
		'verify_peer' => false,
		'verify_peer_name' => false,
		'allow_self_signed' => true
	)
);

$mail->setFrom($email_from, $email_from_name);
$mail->addAddress($emailDestino); // Add a recipient
$mail->isHTML(true); // Set email format to HTML

$mail->Subject = 'Email de teste';
$mail->Body    = 'Corpo do email de <strong>teste</strong>';
$mail->AltBody = 'Corpo do email em plain';

if(!$mail->send()) {
	echo 'Message could not be sent.';
	echo 'Mailer Error: ' . $mail->ErrorInfo;
} else {
	echo 'Message has been sent';
}



/*
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
*/



/*
$mensagem = new PHPMailer();

$mensagem->persistencia = $db;

$mail->SMTPDebug = 3;

$mensagem->Host = $email_host;
$mensagem->Mailer = $email_mailer;
$mensagem->From = $email_from;

if (!empty($email_auth)) {
	$mensagem->SMTPAuth = $email_auth;
}
if (!empty($email_login)) {
	$mensagem->Username = $email_login;
}
if (!empty($email_pass)) {
	$mensagem->Password = $email_pass;
}
if (!empty($email_port)) {
	$mensagem->Port = $email_port;
}
if (!empty($email_secure)) {
	$mensagem->SMTPSecure = $email_secure;
}
if (!empty($email_debug)) {
	$mensagem->SMTPDebug = $email_debug;
}

$mensagem->SMTPOptions = array(
	'ssl' => array(
		'verify_peer' => false,
		'verify_peer_name' => false,
		'allow_self_signed' => true
	)
);

$mensagem->FromName = "Administrador do Sistema";

$mensagem->AddAddress( $emailDestino );
$rodape = <<<RODAPE
	<br/><br/>
	Atenciosamente, <br/ >
TESTE EMAIL
RODAPE;

$mensagem->Subject = $assunto;
$mensagem->Body = "teste mensagem <br>\n". $rodape;
$mensagem->IsHTML( true );
$mensagem->Send();

var_dump($mensagem);
*/


//$teste = enviar_email('teste@php.pro.br', $emailDestino, 'teste', 'teste envio');
//var_dump($teste);
exit;
