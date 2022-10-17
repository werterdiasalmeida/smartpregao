<?php
ini_set("memory_limit", "3000M");
set_time_limit(30000);

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "/var/www/simec/global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$e = new EmailAgendado();
$e->setTitle("Nota do Sistema");
$html = '<div style="font: 12pt Arial,verdana" ><center><b><span style="color:red" >NOTA do SISTEMA!</span></b><br /><br />
Alguns arquivos anexados por você foram corrompidos, para corrigir o problema você poderá enviá-los novamente seguindo os passos abaixo:<br /><br />
<b>1 – Entre no Sistema com seu login e sua senha acessando: <a href="http://'.$_SERVER['HTTP_HOST'].'">http://'.$_SERVER['HTTP_HOST'].'</a>.<br /><br />
2 – Ao entrar no sistema, aparecerá uma mensagem que irá direcioná-lo para a página de Upload de Arquivos, outra opção é acessar o Menu: Principal >> Upload de Arquivos.<br /><br />
3 – Uma lista de arquivos corrompidos será apresentada, você poderá selecionar os arquivos e clicar em SALVAR no final da página para efetuar a correção.</b><br /><br />
</center>
Caso não apareça a relação de arquivos ou a mensagem ao entrar no Sistema, desconsidere esta mensagem.<br /><br />
Contamos com sua colaboração.<br /><br />
Atenciosamente,<br /><br />
Equipe<br /><br />
Obs.: Este é um email automático enviado pelo sistema, favor não responder.</div>';
echo $html;
$e->setText($html);
$e->setName("Sistema");
$e->setEmailOrigem("no-reply@mec.gov.br");
//$e->setEmailsDestinoPorArquivo(APPRAIZ."www/painel/emailsDaniel.txt");

$sql = "select distinct 
			u.usuemail
		from
			public.arquivo a
		inner join 
			seguranca.usuario u ON u.usucpf = a.usucpf
		inner join
			seguranca.perfilusuario pu ON pu.usucpf = a.usucpf
		inner join
			seguranca.perfil pe ON pe.pflcod = pu.pflcod
		where 
			pe.sisid = 15
		and 
			(a.arqid / 1000) between 647 and 725
		and 
			a.arqid not in(select arqid from public.arquivo_recuperado)
		and
			(a.arqstatus is null or a.arqstatus != 'I'::bpchar or a.arqstatus != '0'::bpchar)";
$arrEmails = $db->carregar($sql);
foreach($arrEmails as $email){
	$arrEmail[] = $email['usuemail'];
}

$arrEmail[] = "julianosouza@mec.gov.br";
$arrEmail[] = "cristianocabral@mec.gov.br";
$e->setEmailsDestino($arrEmail);
$e->enviarEmails();