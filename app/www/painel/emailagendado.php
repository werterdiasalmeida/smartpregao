<?php
include_once "config.inc";
include_once "_constantes.php";

include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

$e = new EmailAgendado();
$e->setTitle("SCDP - ALERTA - SOLICITAÇAO DE DIÁRIAS E PASSAGENS");

$html = "<span style=\"font-weight:bold\" >Prezados(as) Senhores(as),</span><br /><br />
<center>
<div style=\"font-size:22px;font-weight:bold;color:#FF0000\" >
Comunico  que hoje é o último dia para solicitar a emissão de passagens e diárias para o dia ".date("d/m",mktime(0, 0, 0, date("m")  , date("d")+10, date("Y"))).".<br />
Fora deste prazo, somente será autorizada a solicitação mediante justificativa escrita para o e-mail, <br />
<span style=\"text-decoration:underline\" >balalaica@mec.gov.br</span>.<br />
Comunico também que não será autorizada a emissão com pendências de prestação de contas.<br />
Um abraço,<br /><br />
<div>
<center><div style=\"font-size:11px;font-weight:bold;color:#000000\">
Jose Luis Balalaica dos Santos<br />
Chefe de Gabinete<br />
Secretaria Executiva MEC<br />
balalaica@mec.gov.br<br />
61 - 20228737
</div></center>";
echo $html;
$e->setText($html);
$e->setEmailOrigem("no-reply@mec.gov.br");
$e->setEmailToReply("balalaica@mec.gov.br");
$e->setName("SCDP");
$e->setEmailsDestinoPorArquivo(APPRAIZ . 'www/painel/email.txt');
$e->setEmailsDestino(array("julianosouza@mec.gov.br","cristianocabral@mec.gov.br","vitor.sad@mec.gov.br"));
$e->enviarEmails();