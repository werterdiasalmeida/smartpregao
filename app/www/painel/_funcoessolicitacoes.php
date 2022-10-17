<?


function carregardadosmenupainelsolicitacoes() {
	global $permissoes;
	// monta menu padrão contendo informações sobre as entidades, personalizado
	if($permissoes['sou_solicitante']) {
		$menu[] = array("id" => 1, "descricao" => "Solicitações", "link" => "/painel/painel.php?modulo=principal/solicitacoes/solicitacao&acao=A".(($_REQUEST['solid'])?"&solid=".$_REQUEST['solid']:""));
	}
	
	if($_REQUEST['modulo'] == "principal/solicitacoes/encaminhamento") {
		$menu[] = array("id" => 2, "descricao" => "Encaminhamento", "link" => "/painel/painel.php?modulo=principal/solicitacoes/encaminhamento&acao=A".(($_REQUEST['solid'])?"&solid=".$_REQUEST['solid']:"").(($_REQUEST['encid'])?"&encid=".$_REQUEST['encid']:""));
	}
	
	if($_REQUEST['modulo'] == "principal/solicitacoes/resposta") {
		$menu[] = array("id" => 2, "descricao" => "Resposta", "link" => "/painel/painel.php?modulo=principal/solicitacoes/resposta&acao=A&solid=".$_REQUEST['solid']);
	}
	
	if($permissoes['sou_atendente']) {
		$menu[] = array("id" => 3, "descricao" => "Atendimento", "link" => "/painel/painel.php?modulo=principal/solicitacoes/atendimento&acao=A".(($_REQUEST['ecaid'])?"&ecaid=".$_REQUEST['ecaid']:""));
	}
	return $menu;
}

function inserirsolicitacao($dados) {
	global $db;
	
	$sql = "INSERT INTO painel.solicitacao(tipid, 
										   stsid,
										   soldesc,
										   solprazo,
										   usucpfsol,
										   soldatainclusao,
										   solporcentoexec,
										   solstatus)
			VALUES ('".$dados['tipid']."', 
					'".SITSOL_NINICIADO."', 
					'".$dados['soldesc']."', 
					'".formata_data_sql($dados['solprazodata'])." ".$dados['solprazohora']."', 
					'".$_SESSION['usucpf']."', 
					NOW(), 
					0, 
					'A') RETURNING solid;";
	
	$solid = $db->pegaUm($sql);
	
	$tipdesc = $db->pegaUm("SELECT tipdesc FROM painel.tiposolicitacao WHERE tipid='".$dados['tipid']."'");
	
	/*
	 * Enviando SMS
	 */
	$sql = "SELECT res.respid, res.respemail, CAST(res.respdddcelular as varchar)||CAST(res.respcelular as varchar) as telefone, res.respnome 
			FROM painel.responsavelsecretaria res
			WHERE res.funid='".FUNCAO_RESPONSAVEL_ASSESSORIA."' AND res.secid='".SEC_SECRETARIAEXECUTIVA."'";
	
	$assessores_sec_exe = $db->carregar($sql);
	
	if($assessores_sec_exe[0]) {
		require_once('../webservice/painel/nusoap.php');
		$client = new soapcliente('https://webservice.cgi2sms.com.br/axis/services/VolaSDKSecure?wsdl', true);
		$err = $client->getError();
		if ($err) die('<h2>Constructor error</h2><pre>' . $err . '</pre>');
		
		foreach($assessores_sec_exe as $ase) {
			if(strlen($ase['telefone']) == 10) {
				$envio = $client->call('sendMessage', array('user' => 'inep', 'password' => 'tmmjee', 'testMode' => false, 'sender' => 'SIMEC', 'target' => '55'.$ase['telefone'], 'body' => ' Painel - Uma solicitação de '.$tipdesc.' foi cadastrada no SIMEC com o código '.$solid.'. Favor acessar o sistema para realizar o atendimento.', 'ID' => substr($ase['respid'],0,6).date("Ymdhis")));
			}
		}
	}
	/*
	 * Enviando SMS
	 */
	
	/*
	 * Enviando email
	 */
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= "SIMEC - Painel";
	$mensagem->From 		= "simec@mec.gov.br";
	
	if($assessores_sec_exe[0]) {
		foreach($assessores_sec_exe as $ase) {
			$mensagem->AddAddress($ase['respemail'], $ase['respnome']);
		}
		$mensagem->Subject = "SIMEC - Painel - Solicitação de ".$tipdesc;
		$mensagem->Body = "O código da solicitação de <b>".$tipdesc."</b> é: <b>".$solid."</b>. Favor acessar o módulo Painel do sistema SIMEC para realizar o atendimento.";
		$mensagem->IsHTML( true );
		$mensagem->Send();
	}
	/*
	 * FIM
	 * Enviando email
	 */
	
	$db->commit();
	
	redirecionarPagina("Solicitação cadatrada com sucesso", "painel.php?modulo=principal/solicitacoes/solicitacao&acao=A");
	
}

function atualizarsolicitacao($dados) {
	global $db;
	
	$sql = "UPDATE painel.solicitacao
   			SET tipid='".$dados['tipid']."', soldesc='".$dados['soldesc']."', solprazo='".formata_data_sql($dados['solprazodata'])." ".$dados['solprazohora']."'  
 			WHERE solid='".$dados['solid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	redirecionarPagina("Solicitação atualizada com sucesso", "painel.php?modulo=principal/solicitacoes/solicitacao&acao=A&visetapa=solicitacao");
}

function redirecionarPagina($msg, $link) {
	
	echo "<script>
			alert('".$msg."');
			window.location='".$link."';
		  </script>";
	
	exit;
}

function atualizarencaminhamento($dados) {
	global $db;
	$sql = "UPDATE painel.encaminhamento SET encdestinatario='".$dados['pessoas']."', 
										  encdesc='".$dados['encdesc']."', 
										  encprazo='".formata_data_sql($dados['encprazodata'])." ".$dados['encprazohora']."' 
 			WHERE encid='".$dados['encid']."'";
	$db->executar($sql);
	$db->commit();
	
	redirecionarPagina("Encaminhamento atualizado com sucesso", "painel.php?modulo=principal/solicitacoes/encaminhamento&acao=A&encid=".$dados['encid']);
}

function inserirencaminhamento($dados) {
	global $db;
	
	$pessoas = explode(",", strtolower($dados['pessoas']));
	$sql = "SELECT respid, respemail, CAST(respdddcelular AS varchar) || CAST(respcelular AS varchar) as telefone FROM painel.responsavelsecretaria WHERE LOWER(respnome) || ' <' || LOWER(respemail) || '>' IN ('".implode("','", $pessoas)."')";
	$encaminhados = $db->carregar($sql);
	
	$sql = "INSERT INTO painel.encaminhamento(solid, encdestinatario, encdesc, encprazo, usucpfenc, encdataenc) 
            VALUES ('".$dados['solid']."', '".$dados['pessoas']."', '".$dados['encdesc']."', '".formata_data_sql($dados['encprazodata'])." ".$dados['encprazohora']."', '".$_SESSION['usucpf']."', NOW()) RETURNING encid;";
	
	$encid = $db->pegaUm($sql);
	
	// obtém o arquivo
	$arquivo = $_FILES['arquivo'];
	
	if($arquivo["name"]) {
	
		// BUG DO IE
		// O type do arquivo vem como image/pjpeg
		if($arquivo["type"] == 'image/pjpeg') {
			$arquivo["type"] = 'image/jpeg';
		}
		
		//Insere o registro do arquivo na tabela public.arquivo
		$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
		values('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".$dados["arqdescricao"]."','".$arquivo["type"]."','".$arquivo["size"]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .") RETURNING arqid;";
		$arqid = $db->pegaUm($sql);
	
		//Insere o registro na tabela obras.arquivosobra
		$sql = "INSERT INTO painel.anexo(arqid, anxdsc, encid) VALUES ('".$arqid."', '".current(explode(".", $arquivo["name"]))."', '".$encid."');";
		$db->executar($sql);
		
		if(!is_dir('../../arquivos/painel/'.floor($arqid/1000))) {
			mkdir(APPRAIZ.'/arquivos/painel/'.floor($arqid/1000), 0777);
		}
		
		$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
		
		if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
			$db->rollback();
			die("<script>alert(\"Problemas no envio do arquivo.\");</script>");
		}
	
	}
	
	$db->executar("UPDATE painel.solicitacao SET stsid='".SITSOL_EMATENDIMENTO."' WHERE solid='".$dados['solid']."'");
	
	$sql = "SELECT tipdesc FROM painel.solicitacao sol 
			LEFT JOIN painel.tiposolicitacao tps ON tps.tipid = sol.tipid 
			WHERE sol.solid='".$dados['solid']."'";
	
	$tipdesc = $db->pegaUm($sql);

	/*
	 * Enviando SMS
	 */
	if($encaminhados[0]) {
		
		require_once('../webservice/painel/nusoap.php');
		$client = new soapcliente('https://webservice.cgi2sms.com.br/axis/services/VolaSDKSecure?wsdl', true);
		$err = $client->getError();
		if($err) die('<h2>Constructor error</h2><pre>' . $err . '</pre>');
		
		foreach($encaminhados as $encam) {
			$sql = "INSERT INTO painel.encaminhados(encid, respid) VALUES ('".$encid."', '".$encam['respid']."');";
			$db->executar($sql);
			if(strlen($encam['telefone']) == 10) {
				$envio = $client->call('sendMessage', array('user' => 'inep', 'password' => 'tmmjee', 'testMode' => false, 'sender' => 'SIMEC',	'target' => '55'.$encam['telefone'], 'body' => ' Painel - Um encaminhamento de '.$tipdesc.' foi cadastrado no SIMEC com o código '.$dados['solid'].'.'.$encid.'. Favor acessar o sistema para realizar o atendimento.', 'ID' => substr($encam['usucpf'],0,6).date("Ymdhis")));
			}
		}
	}
	/*
	 * Enviando SMS
	 */
	
	/*
	 * Enviando email
	 */
	if($encaminhados[0]) {
		require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
		require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= "SIMEC - Painel";
		$mensagem->From 		= "simec@mec.gov.br";
		foreach($encaminhados as $encam) {
			$mensagem->AddAddress($encam['respemail'], $encam['respnome']);
		}
		$mensagem->Subject = "SIMEC - Painel - Encaminhamento de ".$tipdesc;
		$mensagem->Body = "O código do encaminhamento de ".$tipdesc." é:  ".$dados['solid'].'.'.$encid.". Favor acessar o módulo Painel do sistema SIMEC para realizar o atendimento.";
		$mensagem->IsHTML( true );
		$mensagem->Send();
	}
	/*
	 * FIM
	 * Enviando email
	 */
	
	$db->commit();

	
	redirecionarPagina("Encaminhamento inserido com sucesso", "painel.php?modulo=principal/solicitacoes/encaminhamento&acao=A&solid=".$dados['solid']);
	
}

function downloadarquivo($dados) {
	global $db;
	
	$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$dados['arqid'];
	$arquivo = $db->pegaLinha($sql);
	
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
	
	if ( !is_file( $caminho ) ) {
		$_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
	}
	$filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
	header( 'Content-type: '. $arquivo['arqtipo'] );
	header( 'Content-Disposition: attachment; filename='.$filename);
	readfile( $caminho );
	exit();
}

function inseriratendimento($dados) {
	global $db;
	
	$sql = "INSERT INTO painel.atendimento(atdtxtresposta, ecaid, atddataatend) VALUES ('".$dados['atdtxtresposta']."', '".$dados['ecaid']."', NOW()) RETURNING atdid;";
	$atdid = $db->pegaUm($sql);
	
	if($_FILES['arquivo']['name'][0]) {
		for($i=0;$i<count($_FILES['arquivo']['name']);$i++) {
			if($_FILES['arquivo']['name'][$i]) {
				//Insere o registro do arquivo na tabela public.arquivo
				$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
				values('".current(explode(".", $_FILES['arquivo']['name'][$i]))."','".end(explode(".", $_FILES['arquivo']['name'][$i]))."','".$dados["arqdescricao"]."','".$_FILES['arquivo']['type'][$i]."','".$_FILES['arquivo']['size'][$i]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION['usucpf']."',". $_SESSION['sisid'] .") RETURNING arqid;";
				$arqid = $db->pegaUm($sql);
				
				//Insere o registro na tabela obras.arquivosobra
				$sql = "INSERT INTO painel.anexo(arqid, anxdsc, atdid) VALUES ('".$arqid."', '".(($dados['arquivonome'][$i])?$dados['arquivonome'][$i]:"Nome em branco")."', '".$atdid."');";
				$db->executar($sql);
				
				if(!is_dir('../../arquivos/painel/'.floor($arqid/1000))) {
					mkdir(APPRAIZ.'/arquivos/painel/'.floor($arqid/1000), 0777);
				}
				
				$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
				
				if ( !move_uploaded_file( $_FILES['arquivo']['tmp_name'][$i], $caminho ) ) {
					$db->rollback();
					die("<script>alert(\"Problemas no envio do arquivo.\");</script>");
				}
			}
		}
	}
	
	$db->commit();
	
	$sql = "SELECT tps.tipdesc, sol.solid, enc.encid FROM painel.solicitacao sol 
			LEFT JOIN painel.tiposolicitacao tps ON tps.tipid = sol.tipid 
			LEFT JOIN painel.encaminhamento enc ON enc.solid = sol.solid 
			LEFT JOIN painel.encaminhados ena ON ena.encid = enc.encid
			WHERE ena.ecaid='".$dados['ecaid']."'";
	
	$dsol = $db->pegaLinha($sql);
	
	/*
	 * Enviando SMS
	 */
	$sql = "SELECT res.respid, res.respemail, CAST(res.respdddcelular as varchar)||CAST(res.respcelular as varchar) as telefone, res.respnome 
			FROM painel.responsavelsecretaria res
			WHERE res.funid='".FUNCAO_RESPONSAVEL_ASSESSORIA."' AND res.secid='".SEC_SECRETARIAEXECUTIVA."'";
	
	$assessores_sec_exe = $db->carregar($sql);
	
	if($assessores_sec_exe[0]) {
		require_once('../webservice/painel/nusoap.php');
		$client = new soapcliente('https://webservice.cgi2sms.com.br/axis/services/VolaSDKSecure?wsdl', true);
		$err = $client->getError();
		if ($err) die('<h2>Constructor error</h2><pre>' . $err . '</pre>');
		
		foreach($assessores_sec_exe as $ase) {
			if(strlen($ase['telefone']) == 10) {
				$envio = $client->call('sendMessage', array('user' => 'inep', 'password' => 'tmmjee', 'testMode' => false, 'sender' => 'SIMEC', 'target' => '55'.$ase['telefone'], 'body' => ' Painel - O encaminhamento de '.$dsol['tipdesc'].'  com o código '.$dsol['solid'].'.'.$dsol['encid'].' foi respondido. Favor acessar o módulo Painel do sistema SIMEC para realizar o atendimento.', 'ID' => substr($ase['respid'],0,6).date("Ymdhis")));
			}
		}
	}
	/*
	 * Enviando SMS
	 */
	
	/*
	 * Enviando email
	 */
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= "SIMEC - Painel";
	$mensagem->From 		= "simec@mec.gov.br";
	
	if($assessores_sec_exe[0]) {
		foreach($assessores_sec_exe as $ase) {
			$mensagem->AddAddress($ase['respemail'], $ase['respnome']);
		}
		$mensagem->Subject = "SIMEC - Painel – Resposta do encaminhamento de ".$dsol['tipdesc'];
		$mensagem->Body = "O encaminhamento de ".$dsol['tipdesc']." com o código ".$dsol['solid'].'.'.$dsol['encid']." foi respondido. Favor acessar o módulo Painel do sistema SIMEC para realizar o atendimento.
		";
		$mensagem->IsHTML( true );
		$mensagem->Send();
	}
	/*
	 * FIM
	 * Enviando email
	 */
	
	
	
	redirecionarPagina("Atendimento inserido com sucesso", "painel.php?modulo=principal/solicitacoes/atendimento&acao=A");
	
}

function atualizaratendimento($dados) {
	global $db;
	
	$sql = "UPDATE painel.atendimento SET atdtxtresposta='".$dados['atdtxtresposta']."' WHERE atdid='".$dados['atdid']."'";
	$db->executar($sql);
	$db->commit();
	
	redirecionarPagina("Atendimento atualizado com sucesso", "painel.php?modulo=principal/atendimento&acao=A&ecaid=".$_REQUEST['ecaid']);
	
}


function inserirresposta($dados) {
	global $db;
	
	$sql = "INSERT INTO painel.resposta(solid, rsptxtresposta, usucpfresposta, rspdataresposta)
    		VALUES ('".$dados['solid']."', '".$dados['rsptxtresposta']."', '".$_SESSION['usucpf']."', NOW()) RETURNING rspid;";
	
	$rspid = $db->pegaUm($sql);
	
	if($_FILES['arquivo']['name'][0]) {
		for($i=0;$i<count($_FILES['arquivo']['name']);$i++) {
			
			//Insere o registro do arquivo na tabela public.arquivo
			$sql = "INSERT INTO public.arquivo 	(arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid)
			values('".current(explode(".", $_FILES['arquivo']['name'][$i]))."','".end(explode(".", $_FILES['arquivo']['name'][$i]))."','".$dados["arqdescricao"]."','".$_FILES['arquivo']['type'][$i]."','".$_FILES['arquivo']['size'][$i]."','".date('Y-m-d')."','".date('H:i:s')."','".$_SESSION['usucpf']."',". $_SESSION['sisid'] .") RETURNING arqid;";
			$arqid = $db->pegaUm($sql);
			
			//Insere o registro na tabela obras.arquivosobra
			$sql = "INSERT INTO painel.anexo(arqid, anxdsc, rspid) VALUES ('".$arqid."', '".(($dados['arquivonome'][$i])?$dados['arquivonome'][$i]:"Nome em branco")."', '".$rspid."');";
			$db->executar($sql);
			
			if(!is_dir('../../arquivos/painel/'.floor($arqid/1000))) {
				mkdir(APPRAIZ.'/arquivos/painel/'.floor($arqid/1000), 0777);
			}
			
			$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
			
			if ( !move_uploaded_file( $_FILES['arquivo']['tmp_name'][$i], $caminho ) ) {
				$db->rollback();
				echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
				exit;
			}
		}
	}
	
	
	if($dados['arqatendentes']) {
		
		foreach($dados['arqatendentes'] as $anxid) {
			$sql = "UPDATE painel.anexo SET rspid = '".$rspid."' WHERE anxid='".$anxid."'";
			$db->executar($sql);
		}
		
	}
	
	$db->executar("UPDATE painel.solicitacao SET stsid='".SITSOL_FINALIZADO."' WHERE solid='".$dados['solid']."'");
	
	$db->commit();
	
	$sql = "SELECT usucpfsol FROM painel.solicitacao sol 
			WHERE sol.solid='".$dados['solid']."'";
	
	$usucpfsol = $db->pegaUm($sql);
	
	/*
	 * Enviando SMS
	 */
	/*
	require_once('../webservice/painel/nusoap.php');
	$client = new soapcliente('https://webservice.cgi2sms.com.br/axis/services/VolaSDKSecure?wsdl', true);
	$err = $client->getError();
	if ($err) {
	    die('<h2>Constructor error</h2><pre>' . $err . '</pre>');
	}
	if($dadosusus[$usucpfsol]) {
		$envio = $client->call('sendMessage', array('user' => 'inep', 'password' => 'tmmjee', 'testMode' => false, 'sender' => 'SIMEC', 'target' => $dadosusus[$usucpfsol], 'body' => 'Solicitação finalizada', 'ID' => substr($usucpfsol,0,6).date("Ymdhis")));
	}
	*/
	/*
	 * Enviando SMS
	 */
	
	
	/*
	 * Enviando email
	 */
	/*
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= "SIMEC";
	$mensagem->From 		= "simec@mec.gov.br";
	$mensagem->AddAddress("alexandre.dourado@mec.gov.br", "Alexandre Dourado");
	$mensagem->AddAddress("priscila.vilaca@mec.gov.br", "Priscila Vilaça");
	$mensagem->Subject = "Solicitação atendida";
	$mensagem->Body = "Solicitação respondida com sucesso";
	$mensagem->IsHTML( true );
	$mensagem->Send();
	*/
	/*
	 * FIM
	 * Enviando email
	 */
	
	redirecionarPagina("Resposta inserida com sucesso", "painel.php?modulo=principal/solicitacoes/solicitacao&acao=A");
}

function arquivarsolicitacao($dados) {
	global $db;
	$sql = "UPDATE painel.solicitacao SET stsid='".SITSOL_ARQUIVADO."' WHERE solid='".$dados['solid']."'";
	$db->executar($sql);
	$db->commit();
	
	redirecionarPagina("Solicitação arquivada com sucesso", "painel.php?modulo=principal/solicitacoes/solicitacao&acao=A");
	
}
?>