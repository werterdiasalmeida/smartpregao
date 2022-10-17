<?php

/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

//$_REQUEST['baselogin'] = "simec_espelho_producao";
$_REQUEST['baselogin'] = "simec_desenvolvimento";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once '_constantes.php';
include_once '_funcoesagendamentoindicador.php';

date_default_timezone_set ('America/Sao_Paulo');

//referente painel.coleta
define("COLETA_AUTOMATICA", 2);// tipo automatica

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SIMEC Painel - Atualização dos dados";
$mensagem->From 		= "simec@mec.gov.br";
$mensagem->Subject      = "Informações desatualizadas no Painel de Controle";
$mensagem->IsHTML( true );

require_once('../webservice/painel/nusoap.php');
	
$client = new soapcliente('https://webservice.cgi2sms.com.br/axis/services/VolaSDKSecure?wsdl', true);
$err = $client->getError();
if ($err) die('<h2>Constructor error</h2><pre>' . $err . '</pre>');

$sql = "SELECT i.indid, i.indnome, i.secid, p.dpediasverificacao
		FROM painel.indicador i 
		INNER JOIN painel.periodicidade p ON i.peridatual = p.perid 
		WHERE i.indpublicado=true
		AND i.indstatus = 'A'
		AND i.secid <> 17
		ORDER BY i.secid, i.indid";
$indicadores = $db->carregar($sql);

if($indicadores[0]) {
	
	foreach($indicadores as $in) {
		
		//$sql = "SELECT indid FROM painel.indicador WHERE indid='".$in['indid']."' AND indpublicado=true AND indid NOT IN (
		//		SELECT i.indid FROM painel.indicador i 
		//		INNER JOIN painel.periodicidade p ON p.perid = i.peridatual 
		//		LEFT JOIN painel.seriehistorica s ON s.indid = i.indid 
		//		WHERE sehdtcoleta > now() - interval '".$in['dpediasverificacao']." day' 
		//		GROUP BY i.indid		
		//		)";
		
		$sql = "SELECT s.indid, '<center>'||to_char(sehdtcoleta, 'dd/mm/YYYY HH24:MI')||'</center>' as data
				FROM painel.indicador i
				LEFT JOIN painel.seriehistorica s ON s.indid = i.indid
				WHERE i.indid = '".$in['indid']."'
				AND ((s.sehdtcoleta IS NOT NULL	AND s.sehstatus <> 'I' AND now() > (SELECT sehdtcoleta + interval '".$in['dpediasverificacao']." day' FROM painel.seriehistorica WHERE indid = '".$in['indid']."' AND sehdtcoleta IS NOT NULL AND sehstatus <> 'I' ORDER BY sehdtcoleta DESC LIMIT 1))
				OR (s.indid IS NULL))
				ORDER BY s.sehdtcoleta DESC
				LIMIT 1";
				
		$indids = $db->pegaLinha($sql);
		
		if($indids) {
			//$ult = $db->pegaUm("SELECT '<center>'||to_char(sehdtcoleta, 'dd/mm/YYYY HH24:MI')||'</center>' FROM painel.seriehistorica WHERE indid='".$in['indid']."' ORDER BY sehdtcoleta DESC");
			$_ATRASO[$in['secid']][] = array("id" => $in['indid'], "nome" => $in['indnome'], "ultdata" => (($indids['indid'])?$indids['data']:"<center>Nunca foi atualizado</center>"));
		}
	}
	
	if($_ATRASO) {
		foreach($_ATRASO as $secid => $at) {
			
			$sql = "SELECT 
						r.respdddcelular::varchar||r.respcelular::varchar as telefone,
						r.respemail,
						r.respnome, 
						s.secdsc
					FROM painel.responsavelsecretaria r 
					INNER JOIN painel.secretaria s ON s.secid = r.secid
					WHERE r.respstatus = 'A'
					AND r.secid='".$secid."'";
			
			$resp = $db->carregar($sql);
			
			//$resp[0] = array("telefone" => "6181221163", "respemail" => "vitor.sad@mec.gov.br", "respnome" => "Vitor Sad", "secdsc" => "moomo mom mo momo om mo om om om om"); 
			
			unset($mensagem->to);
						
			if($resp[0]) {
				
				foreach($resp as $r) {
					$mensagem->AddAddress($r['respemail'], $r['respnome']);
					if ($r['secdsc'] == 'FNDE'){
						$mensagem->AddAddress('marcos.costa@fnde.gov.br', 'Marcos Costa');
						$mensagem->AddAddress('vitor.sad@mec.gov.br', 'Vitor Sad - DTI/MEC');
					}
					if(strlen($r['telefone']) == 10) {
						$envio = $client->call('sendMessage', array('user' => 'inep', 'password' => 'tmmjee', 'testMode' => false, 'sender' => 'SIMEC', 'target' => '55'.$r['telefone'], 'body' => 'Existem '.count($at).' indicadores da secretaria '.$r['secdsc'].' sob sua responsabilidade desatualizados, acesse simec.mec.gov.br e proceda a atualização. MEC', 'ID' => substr($r['respid'],0,6).date("Ymdhis")));
					}
				}
									
				// enviando email
				ob_start();
				$cabecalho = array("ID", "Nome do indicador", "Última atualização");
				$db->monta_lista_simples($at,$cabecalho,1000,5,'N','100%',$par2);
				$dadosserv = ob_get_contents();
				ob_end_clean();
				$mensagem->AddAddress('vitor.sad@mec.gov.br', 'Vitor Sad - DTI/MEC');
				$mensagem->AddAddress('leonardo.rezende@mec.gov.br', 'Leonardo Milhomem Rezende');
				$mensagem->AddAddress('adriano.dani@mec.gov.br', 'Adriano Almeida Dani');
				$mensagem->AddAddress('manoela.macedo@mec.gov.br', 'Manoela Dutra Macedo');
				$mensagem->AddAddress('juliana.rabelo@mec.gov.br', 'Juliana Rabelo');
				$mensagem->AddAddress('loyane.tavares@mec.gov.br', 'Loyane de Sousa Tavares');
				$mensagem->AddAddress('alexandre.damo@mec.gov.br', 'Alexandre Fogaca Damo');
				$mensagem->IsHTML(true);
				$mensagem->Body = "<style>table.listagem  {border-bottom:3px solid #DFDFDF;border-collapse:collapse;border-top:2px solid #404040;font-size:11px;padding:3px;font:8pt Arial,verdana;}body {font:12px Arial,verdana;}</style>";
				$mensagem->Body .= "<p>Prezado(a) <b>".$r['respnome']."</b>,</p>";
				$mensagem->Body .= "<p>Existem <b>".count($at)."</b> indicadores desatualizados referente a secretaria <b>".$r['secdsc']."</b>. Acesse o <a href=http://simec.mec.gov.br/ target=_blank>SIMEC - Painel</a> (http://simec.mec.gov.br/) e realize as atualizações das informações.</p>";
				$mensagem->Body .= "<p>Segue abaixo a lista de indicadores desatualizados:</p>";
				$mensagem->Body .= $dadosserv;
				$mensagem->Body .= "<p>Agradecemos a colaboração,<br/>MEC</p>";
				
				$enviosmtp = $mensagem->Send();
				
				if($enviosmtp) {
					$_LOG .= "Email enviado para ".$d['respnome']." <br /> ";
				} else {
					$_LOG .= "Problemas para enviar email ".$d['respemail']." <br /> ";
				}
				// fim enviando email
				
			}
			 
		}
	}

	
}

?>