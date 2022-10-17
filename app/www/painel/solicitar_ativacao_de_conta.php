<?php

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>
	 * Módulo: Autenticação
	 * Finalidade: Permite que o usuário solicite justificadamente a ativação da sua conta.
	 * Data de criação: 24/06/2005
	 * Última modificação: 29/08/2006
	 */

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conexão com o banco de dados
	$db = new cls_banco();

	$usucpf = $_REQUEST['usucpf'];
	$sisid = $_REQUEST['sisid'];

	if ( $_REQUEST['formulario'] ) {
		$cpf = corrige_cpf( $_POST['usucpf'] );
		$justificativa = trim( $_POST['htudsc'] );
		
		// carrega os dados do usuário
		$sql = sprintf(
			"SELECT u.usucpf, u.suscod FROM seguranca.usuario u WHERE u.usucpf = '%s'",
			$cpf
		);
		$usuario = (object) $db->recuperar( $sql );
		
		// atribuições requeridas para que a auditoria do sistema funcione
		$_SESSION['sisid'] = 4; # seleciona o sistema de segurança
		$_SESSION['usucpf'] = $usuario->usucpf;
		$_SESSION['usucpforigem'] = $usuario->usucpf;
		$_SESSION['superuser'] = $db->testa_superuser( $usuario->usucpf );
		
		$descricao = "Usuário solicitou a ativação da conta e apresentou a seguinte justificativa: ". $justificativa;
		if ( $usuario->usucpf ) {
			if ( $sisid ) {
				$sql = sprintf(
					"SELECT us.* FROM seguranca.usuario_sistema us WHERE us.sisid = %d AND us.usucpf = '%s'",
					$sisid,
					$usuario->usucpf
				);
				$usuario_sistema = (object) $db->pegaLinha( $sql );
				if ( $usuario_sistema->suscod == 'B' ) {
					$db->alterar_status_usuario( $cpf, 'P', $descricao, $usuario_sistema->sisid );
				}
			} else if ( $usuario->suscod == 'B' ) {
				$db->alterar_status_usuario( $cpf, 'P', $descricao );
			}
		}
		$db->commit();
		$_SESSION['MSG_AVISO'] = array( "Seu pedido foi submetido e será avaliado em breve." );
		
		header( "Location: login.php" );
		exit();
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
			$mensagens = '<p style="align: center; color: red; font-size: 12px">'. implode( '<br/>', (array) $_SESSION['MSG_AVISO'] ) . '</p>';
			$_SESSION['MSG_AVISO'] = null;
			$titulo_modulo = 'Solicitação de Ativação Conta';
			$subtitulo_modulo = 'Preencha os Dados e clique no botão: "Enviar Solicitação"';
			
			
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
			<label for="usucpf">Justificativa:</label>
			<textarea style="width:300px;margin-bottom: 10px" name="htudsc" rows="3" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="width: 100ex;"><?= $observacao ?></textarea>
			<input type="button" class="button2" style="margin-left:50px;width:120px;cursor:pointer;font-weight: normal" name="btinserir" value="Enviar Solicitação" onclick="enviar_formulario()"/>
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
		if ( document.formulario.usucpf.value == "" ) {
			mensagem += '\nInforme o cpf.';
			validacao = false;
		}
		if ( document.formulario.htudsc.value == "" ) {
			mensagem += '\nVocê deve justificar o pedido.';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

</script>