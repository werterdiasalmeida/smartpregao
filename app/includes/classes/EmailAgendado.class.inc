<?php

class EmailAgendado
	{

		protected $title;
		protected $text;
		protected $email_origem;
		protected $email_reply;
		protected $name;
		protected $arr_anexo = array();
		protected $arr_emails_destino = array();
		
		public function setTitle($title)
		{
			$this->title = $title; 
		}
		
		public function setName($name)
		{
			$this->name = $name; 
		}

		public function setEmailToReply($email)
		{
			$this->email_reply = $email; 
		}
		
		public function setText($text)
		{
			$this->text = $text; 
		}
		
		public function setEmailOrigem($email)
		{
			$this->email_origem = $email; 
		}
		
		public function getEmailsDestino()
		{
			return $this->arr_emails_destino;
		}
		
		public function setEmailsDestino($emails)
		{
			if(is_array($emails) && $emails[0]){
				foreach($emails as $e){
					if(filter_var(strtolower($e), FILTER_VALIDATE_EMAIL))
						$this->arr_emails_destino[] = strtolower($e);
				}
				$this->emails_destino = $arrEmail;
			}else{
				if(filter_var(strtolower($emails), FILTER_VALIDATE_EMAIL))
					$this->arr_emails_destino[] = $emails;
			}
		}
		
		public function setEmailsDestinoPorArquivo($path)
		{
			if(is_file($path)){
				$fp = fopen($path,'r');
				$texto = fread($fp, filesize($path));
				$arrEmails = explode(";",$texto);
				foreach($arrEmails as $email){
					if(filter_var(strtolower($email), FILTER_VALIDATE_EMAIL))
						$this->arr_emails_destino[] = strtolower($email);
				}
			}
		}
		
		public function addAnexo($path)
		{
			$this->arr_anexo[] = $path;
		}
		
		public function enviarEmails()
		{
			
			require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
			require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
			
			$mensagem = new PHPMailer();
			
			$mensagem->Host         = "200.130.3.117";
			$mensagem->Mailer       = "smtp";
			$mensagem->FromName		= !$this->name ? "SCDP" : $this->name;
			$mensagem->From 		= $this->email_origem ;
			$mensagem->Subject      = $this->title;
			$mensagem->Body 		= $this->text;
			$mensagem->IsHTML( true );
			if($this->email_reply){
				$mensagem->AddReplyTo($this->email_reply);
			}
			
			if($this->arr_anexo && is_array($this->arr_anexo) && count($this->arr_anexo) > 0){
				foreach($this->arr_anexo as $anexo){
					echo "<br />".$anexo."<br />";
					if(is_file($anexo)){
						$mensagem->AddAttachment($anexo);
						echo "<br />".$anexo." Anexado!<br />";
					}
				}
			}
			
			foreach($this->arr_emails_destino as $k => $email_destino){
				$mensagem->AddAddress($email_destino);
				$mensagem->Send();
				$mensagem->ClearAddresses();
					
			}

		}
	}