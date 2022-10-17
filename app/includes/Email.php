<?php

/*

drop table public.emaildestinatario;
drop table public.email;

CREATE TABLE public.email (
	emaid serial,
	emaassunto char(100) NOT NULL,
	emaconteudo text NOT NULL,
	emadata timestamp DEFAULT ('now'::text)::timestamp(6) without time zone NOT NULL,
	usucpf char(11) NOT NULL,
	CONSTRAINT email_pk PRIMARY KEY (emaid),
	CONSTRAINT email_usucpf_fk FOREIGN KEY (usucpf) REFERENCES seguranca.usuario (usucpf)
);

CREATE TABLE public.emaildestinatario (
	edeid serial,
	emaid int4 NOT NULL,
	usucpf char(11) NOT NULL,
	CONSTRAINT emaildestinatario_pk PRIMARY KEY (edeid),
	CONSTRAINT emaildestinatario_emaid_fk FOREIGN KEY (emaid) REFERENCES public.email (emaid),
	CONSTRAINT emaildestinatario_usucpf_fk FOREIGN KEY (usucpf) REFERENCES seguranca.usuario (usucpf)
);

*/

// carrega a biblioteca phpmailer
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

class Email extends PHPMailer {	
	
	/**
	 * Prefixo inserido no assunto da mensagem.
	 * 
	 * @var string
	 */
	const ASSUNTO = ' ';
	
	/**
	 * Texto a ser inserido no corpo da mensagem de cópia para o remetente.
	 * 
	 * @var string
	 */
	const TEXTO_COPIA = 'Esta mensagem é uma cópia.<br/>';
	
	/**
	 * @var cls_banco
	 */
	protected $persistencia = null;
	
	/**
	 * @return void
	 */
	public function __construct()
	{
		global $db;
		$this->persistencia = $db;
		$this->Host         = "localhost";
		$this->Mailer       = "smtp"; 
	}
	
	/**
	 * Envia um email para os destinatários informados por parâmetro.
	 * 
	 * @param array   $destinatarios Lista com cpf dos destinatarios
	 * @param string  $assunto       Assunto da mensagem
	 * @param string  $conteudo      Corpo da mensagem
	 * @param array   $arquivos      Caminho para os arquivos
	 * @param boolean $copia         Indica se o remetente deve receber uma cópia
	 * @param boolean $registrar     Indica se a mensagem deve ser registrada no banco de dados
	 * @return boolean
	 */
	public function enviar( $destinatarios, $assunto, $conteudo, $arquivos = array(), $copia = false, $registrar = true, $outros_destinatarios = array() , $boolOculto = true )
	{		
		// localiza e identifica o remetente
		$remetente = $this->pegarUsuario( $_SESSION['usucpforigem'] );
		if ( !$remetente->usucpf ) {
			return false;
		}
		$this->From     = $remetente->usuemail;
		$this->FromName = $remetente->usunome;
		
		// localiza e identifica os destinatarios
		foreach( $destinatarios as $destinatario ){
			$destinatario = $this->pegarUsuario( $destinatario );
			if ( !$destinatario->usucpf ) {
				$destinatario = null;
				unset( $destinatario );
				continue;
			}
			if( $boolOculto )
			{
				$this->AddBCC( $destinatario->usuemail, $destinatario->usunome );
			}
			else
			{
				$this->AddAddress( $destinatario->usuemail, $destinatario->usunome );
			}
		}
		
		// adicionar outros destinatarios
		foreach ( $outros_destinatarios as $desinatario )
		{
			if( $boolOculto )
			{
				$this->AddBCC( $desinatario ) ;
			}
			else
			{
				$this->AddAddress( $desinatario );
			}
		}
		
		// anexa os arquivos
		foreach ( $arquivos as $arquivo ){
			if ( !file_exists( $arquivo ) ) {
				continue;
			}
			$this->AddAttachment( $arquivo, basename( $arquivo ) );
		}
		
		// formata assunto, conteudo e envia a mensagem
		$this->Subject = self::ASSUNTO .  $assunto;
		$this->Body    =  $conteudo;		
		$this->IsHTML( true );
		set_time_limit( 180 );
		if( !$this->Send() ) {
			return false;
		}
		
		// registra o envio do email
		if ( $registrar ) {
			if( !$this->registrar( $remetente, $destinatarios, $assunto, $conteudo ) ) {
				return false;				
			}
		}
		
		// envia mensagem de cópia
		if ( $copia ) {
			$copia = new Email();
			return $copia->enviar(
				(array) $remetente->usucpf,
				$assunto,
				self::TEXTO_COPIA . $conteudo,
				$arquivos,
				false,
				false
			);
		}
		
		// confirma o envio
		return true;
	}
	
	/**
	 * Carrega informações do usuário a partir do cpf.
	 * 
	 * @return object
	 */
	public function pegarUsuario( $usucpf )
	{
		$sql = sprintf( "select usucpf, usunome, usuemail from seguranca.usuario where usucpf = '%s'", $usucpf );
		return (object) $this->persistencia->pegaLinha( $sql );
	}
	
	/**
	 * Registra o envio de um email no banco de dados
	 * 
	 * @return boolean
	 */
	protected function registrar( $remetente, $destinatarios, $assunto, $conteudo )
	{
		// registra a mensagem
		$emaid = $this->persistencia->pegaUm( "select nextval( 'public.email_emaid_seq' )" );			
		$sql = sprintf(
			"insert into public.email ( emaid, emaassunto, emaconteudo, usucpf ) values ( '%s', '%s', '%s', '%s' )",
			$emaid,
			 $assunto,
			 $conteudo,
			$remetente->usucpf
		);		
		if ( !$this->persistencia->executar( $sql ) ) {
			return false;
		}
		
		// registra os destinatários
		foreach ( $destinatarios as $destinatario ) {
			$destinatario = (object) $destinatario;
			if ( !$destinatario->usucpf ) {
				continue;
			}
			$sql = sprintf(
				"insert into public.emaildestinatario ( emaid, usucpf ) values ( '%s', '%s' )",
				$emaid,
				$destinatario->usucpf
			);
			if ( !$this->persistencia->executar( $sql ) ) {
				return false;
			}
		}
		return true;
	}
	
}

?>