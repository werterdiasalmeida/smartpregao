<?php
include_once "config.inc";
include_once "_constantes.php";

include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

$e = new EmailAgendado();
$e->setTitle("SCDP - ALERTA - SOLICITA�AO DE DI�RIAS E PASSAGENS");

$html = "<span style=\"font-weight:bold\" >Prezados(as) Senhores(as),</span><br /><br />
<center>
<div style=\"font-size:22px;font-weight:bold;color:#FF0000\" >
Comunico  que hoje � o �ltimo dia para solicitar a emiss�o de passagens e di�rias para o dia ".date("d/m",mktime(0, 0, 0, date("m")  , date("d")+10, date("Y"))).".<br />
Fora deste prazo, somente ser� autorizada a solicita��o mediante justificativa escrita para o e-mail, <br />
<span style=\"text-decoration:underline\" >balalaica@mec.gov.br</span>.<br />
Comunico tamb�m que n�o ser� autorizada a emiss�o com pend�ncias de presta��o de contas.<br />
Um abra�o,<br /><br />
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