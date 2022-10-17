<?php

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Módulo: Segurança
	 * Finalidade: Solicitação de cadastro de contas de usuário.
	 * Data de criação:
	 * Última modificação: 30/08/2006
	 */

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();

	$sisid = $_REQUEST['sisid'];
	$usucpf = $_REQUEST['usucpf'];

	// leva o usuário para o passo seguinte do cadastro
	if ( $_POST['formulario'] ) {
		$_SESSION = array();
		header( "Location: cadastrar_usuario_2.php?sisid=$sisid&usucpf=$usucpf" );
		exit();
	}

?>
<!-- 
	Sistema Integrado de Monitoramento do Ministério da Educação
	Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	Finalidade: Solicitação de cadastro de contas de usuário.
-->
<html>
	<head>
		<title><?= $GLOBALS['parametros_sistema_tela']['sigla-nome_completo']; ?></title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<style type=text/css>
			form {
				margin: 0px;
			}
		</style>
	</head>
	<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">
		<?php include "cabecalho.php"; ?>
		<br/>
		<?php
			$mensagens = '<p style="align: center; color: red; font-size: 12px">'. implode( '<br/>', (array) $_SESSION['MSG_AVISO'] ) . '</p>';
			$_SESSION['MSG_AVISO'] = null;
			$titulo_modulo = 'Solicitação de Cadastro de Usuários';
			$subtitulo_modulo = 'Preencha os Dados Abaixo e clique no botão: "Enviar Solicitação".<br/>'. obrigatorio() .' Indica Campo Obrigatório.'. $mensagens;
			monta_titulo( $titulo_modulo, $subtitulo_modulo );
		?>
		<form method="POST" name="formulario">
			<input type=hidden name="formulario" value="1"/>
			<input type=hidden name="modulo" value="./inclusao_usuario.php"/>
			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<tr bgcolor="#F2F2F2">
					<td align = 'right' class="subtitulodireita" width="150px">Módulo:</td>
					<td>
						<?php
							$sql = "select s.sisid as codigo, s.sisabrev as descricao from seguranca.sistema s where s.sisstatus='A' and sismostra='t'";
							$db->monta_combo( "sisid", $sql, 'S', "&nbsp;", 'selecionar_modulo', '' );
						?>
						<?= obrigatorio(); ?>
					</td>
			 	</tr>
				<?php if( $sisid ): ?>
					<tr>
						<td align='right' class="subtitulodireita">&nbsp;</td>
						<td>
							<?php
								$sql = sprintf( "select sisid, sisdsc, sisfinalidade, sispublico, sisrelacionado from sistema where sisid = %d", $sisid );
								$sistema = (object) $db->pegaLinha( $sql );
								if ( $sistema->sisid ) :
							?>
								<font color="#555555" face="Verdana">
									<b><?= $sistema->sisdsc ?></b><br/>
									<p><?= $sistema->sisfinalidade ?></p>
									<ul>
										<li><span style="color: #000000">Público-Alvo:</span> <?= $sistema->sispublico ?><br></li>
										<li><span style="color: #000000">Sistemas Relacionados:</span> <?= $sistema->sisrelacionado ?></li>
									</ul>
								</font>
							<?php endif; ?>
						</td>
					</tr>
				<?php endif; ?>
				<input type="hidden" name="sisfinalidade_selc" value="<?=$sisfinalidade_selc?>"/>
				<tr>
					<td align='right' class="subtitulodireita">CPF:</td>
					<td>
						<input type="text" name="usucpf" size="20" maxlength="14" value=<? print '"'.$usucpf.'"'; ?> class="normal" onKeyUp= "this.value=mascaraglobal('###.###.###-##',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : left; width:22ex;" title=''>
						<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'>
					</td>
				</tr>
				<tr bgcolor="#C0C0C0">
					<td>&nbsp;</td>
					<td>
						<input type="button" name="btinserir" value="Continuar" onclick="enviar_formulario()"/>
						&nbsp;&nbsp;&nbsp;
						<input type="Button" value="Cancelar" onclick="location.href='./login.php'"/>
					</td>
				</tr>
			</table>
		</form>
		<br/>
		<?php include "./rodape.php"; ?>
	</body>
</html>
<script language="javascript">

	function selecionar_modulo() {
		document.formulario.formulario.value = "";
		document.formulario.submit();
	}

	function enviar_formulario() {
		if ( validar_formulario() ) {
			document.formulario.submit();
		}
	}

	function validar_formulario() {
		var validacao = true;
		var mensagem = '';
		if ( document.formulario.sisid.value == "" ) {
			mensagem += '\nSelecione o módulo no qual você pretende ter acesso.';
			validacao = false;
		}
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