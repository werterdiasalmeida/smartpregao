<?php

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>
	 * Módulo: Segurança
	 * Finalidade: Permite que o usuário solicite uma nova senha.
	 * Última modificação: 26/08/2006
	 */

	function erro(){
		global $db;
		$db->commit();
		$_SESSION = array();
		$_SESSION['MSG_AVISO'] = func_get_args();
		header( "Location: ". $_SERVER['PHP_SELF'] );
		exit();
	}

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();

	// executa a rotina de recuperação de senha quando o formulário for submetido
	if ( $_POST['formulario'] ) {
		
		// verifica se a conta está ativa
		$sql = sprintf(
			"SELECT u.* FROM seguranca.usuario u WHERE u.usucpf = '%s'",
			corrige_cpf( $_REQUEST['usucpf'] )
		);
		$usuario = (object) $db->pegaLinha( $sql );
		if ( $usuario->suscod != 'A' ) {
			erro( "A conta não está ativa." );
		}
		
		$_SESSION['mnuid'] = 10;
		$_SESSION['sisid'] = 4;
		$_SESSION['exercicio_atual'] = $db->pega_ano_atual();
		$_SESSION['usucpf'] = $usuario->usucpf;
		$_SESSION['usucpforigem'] = $usuario->usucpf;
		
		// cria uma nova senha
	    //$senha = $db->gerar_senha();
	    $senha = strtoupper(senha());
		$sql = sprintf(
			"UPDATE seguranca.usuario SET ususenha = '%s', usuchaveativacao = 'f' WHERE usucpf = '%s'",
			md5_encrypt_senha( $senha, '' ),
			$usuario->usucpf
		);
		$db->executar( $sql );
		
		// envia email de confirmação
		$sql = "select ittemail from public.instituicao where ittstatus = 'A'";
		$remetente = $db->pegaUm( $sql );
		$destinatario = $usuario->usuemail;
		$assunto = "Simec - Recuperação de Senha";
	    $conteudo = sprintf(
	    	"%s %s<br/>Sua senha foi alterada para %s<br>Ao se conectar, altere esta senha para a sua senha preferida.",
	    	$usuario->ususexo == 'F' ?  'Prezada Sra.': 'Prezado Sr.',
	    	$usuario->usunome,
	    	$senha
	    );
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );
		
		$db->commit();
		$_SESSION = array();
		$_SESSION['MSG_AVISO'][] = "Recuperação de senha concluída. Em breve você receberá uma nova senha por email.";
		header( "Location: /painel/login.php" );
		exit();
	}

	if ( $_REQUEST['expirou'] ) {
		$_SESSION['MSG_AVISO'][] = "Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
	}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<title>Painel de Controle do MEC</title>
<!-- TEMAS -->
<?php 
$tema = $_REQUEST['tema'];
if($tema==''): $tema="padrao";endif;?>
<link href="novo/temas/<?php echo $tema;?>/css/estilo.css" rel="stylesheet" type="text/css" />
<link href="novo/temas/<?php echo $tema;?>/css/login.css" rel="stylesheet" type="text/css" />
<!-- FIM - TEMAS -->
</head>
	<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">
	<div style="width:100%;background-color: #ffcc00;height:21px">
		<div style="float:left" ><img src="../imagens/logo_mec_br.gif" /></div>
		<div style="float:right;margin-right:10px" ><img src="../imagens/logo_brasil.gif" /></div>
	</div>
	<div id="conteudo">
	
		<div id="subconteudo">
		
		<div id="topo">
			<!-- logo -->
			<div style="margin-top:21px"  id="logo"><img src="images/logo.png" alt="Assinatura Visual Painel Mec" /></div>
		</div>
		<?php			
			$_SESSION['MSG_AVISO'] = null;
			
			$subtitulo_modulo = 'Digite seu CPF e pressione o botão "Lembrar Senha".<br/>O Sistema enviará um e-mail para você contendo uma nova senha de acesso.<br/>'. $mensagens;
			
		?>
		<!-- cantos arredondados - topo-->
		<div id="geral_top_esq" style="margin-top:30px;">
			<div id="geral_top_dir"></div>
		</div>
		
		<!-- DIV CONTEUDO GERAL-->
		<div id="geral" style="clear:both">
			<div id="geral_top" style="height: 120px;">
		    	<div id="texto" style="height: 80px;">
		            <blockquote>
		            <?php echo $subtitulo_modulo; ?>             
		            </blockquote>
		        </div>
		<div id="login">
            <div id="login_box">
		<form method="POST" name="formulario">
			<input type=hidden name="formulario" value="1"/>
			<input type=hidden name="modulo" value="./inclusao_usuario.php"/>
			<label for="usucpf">CPF:</label>
			<input type="text" name="usucpf" value="" size="20" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" class="normal" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);">
			<input type="button" class="button2" style="margin-left:50px;width:120px;cursor:pointer;font-weight: normal" name="btinserir" value="Lembrar Senha" onclick="enviar_formulario()"/>
			<input type="button" class="button2" style="width:100px;cursor:pointer;font-weight: normal" value="Voltar" onclick="location.href='login.php'"/>
			
		</form>
		</div>
        
    </div>
    </div>
    <div id="geral_bot"><br />
</div>
</div>
</div>
</div>
</body>
</html>
<script language="javascript">

	document.formulario.usucpf.focus();

	function enviar_formulario() {
		if ( validar_formulario() ) {
			document.formulario.submit();
		}
	}

	function validar_formulario() {
		var validacao = true;
		var mensagem = '';
		if ( !validar_cpf( document.formulario.usucpf.value ) ) {
			mensagem += '\nO cpf informado não é válido.';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

</script>