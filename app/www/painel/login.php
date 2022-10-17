<?php 
/**
	 * Sistema Integrado de Planejamento, Orçamento e Finanças do Ministério da Educação
	 * Setor responsvel: DTI/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Autores: Cristiano Cabral <cristiano.cabral@gmail.com>, Adonias Malosso <malosso@gmail.com> 
	 * Módulo: Segurança
	 * Finalidade: Tela de apresentação. Permite que o usuário entre no sistema.
	 * Data de criação: 24/06/2005
	 * Última modificação: 24/08/2008
	 */

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();
	
	if ( $_POST['formulario'] ) {
		$_SESSION['dmdidavaliacao'] = $_REQUEST['dmdidavaliacao'];	
		include "autenticar.inc";
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

//array de temas disponíveis
$arrTemas = array("padrao","dark","sol");
//gera temas aleatórios
$tema = !$_SESSION['tema'] ? $arrTemas[mt_rand(0, (count($arrTemas) -1))] : $_SESSION['tema'];
//$_SESSION['tema'] = $tema;
if($tema==''): $tema="padrao";endif;?>
<link href="novo/temas/<?php echo $tema;?>/css/estilo.css" rel="stylesheet" type="text/css" />
<!-- FIM - TEMAS -->

<link href="novo/temas/<?php echo $tema;?>/css/login.css" rel="stylesheet" type="text/css" />

</head>

<body>
<div style="width:100%;background-color: #ffcc00;height:21px">
	<div style="float:left" ><img src="../imagens/logo_mec_br.gif" /></div>
	<div style="float:right;margin-right:10px" ><img src="../imagens/logo_brasil.gif" /></div>
</div>
<div id="conteudo">

	<div id="subconteudo">
	
	<div id="topo">
		<!-- logo -->
		<div style="margin-top:21px" id="logo"><img src="images/logo.png" alt="Assinatura Visual Painel Mec" /></div>
	</div>
	
	<!-- cantos arredondados - topo-->
	<div id="geral_top_esq" style="margin-top:30px;">
		<div id="geral_top_dir"></div>
	</div>
	
	<!-- DIV CONTEUDO GERAL-->
	<div id="geral">
		<div id="geral_top">
	    	<div id="texto">
	            <blockquote>
	            <b>Finalidade:</b> Cadastrar e manter série histórica de informações encontrada nos produtos decorrentes de atividades de Alinhamento Estratégico, tais como: Mapas estratégicos e indicadores estratégicos. Os mapas estratégicos são representados no Painel de acordo com os eixos: Educação Básica, Educação Superior, Alfabetização, Educação Continuada e Diversidade e Educação Profissional e Técnológica.
				<br /><b>Público-Alvo:</b> Alta-Gestão e Secretários
				<br /><b>Sistemas Relacionados:</b> Não se aplica             
	            </blockquote>
	        </div>
	        
	        <div id="login">
	            <h3>ACESSO AO SISTEMA</h3>
	            <div style="color:red;font-size:12px;text-align:center;margin-top:5px;margin-bottom:5px;width:430px;">
					<? if($_SESSION['MSG_AVISO']): ?>
					<?= implode( '<p>', (array) $_SESSION['MSG_AVISO'] ); ?></p>
				<? endif; ?>
				</div>
	            <div id="login_box">
	                <form name="formulario" id="formulario" method="post" action="#">
	                <input type="hidden" name="formulario" value="1"/>
	                <input type="hidden" name="tema" value="<?php echo $tema;?>"/>
					<input type="hidden" name="dmdidavaliacao" value="<?=$_REQUEST['dmdidavaliacao']?>" />
	                <label for="usucpf">CPF:</label>
	                <input name="usucpf" onkeypress="return controlar_foco_cpf( event );" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" type="text" value="" maxlength="14" /><br />
	                <label for="ususenha">Senha:</label>
	                <input name="ususenha" onkeypress="enviar_formulario_Enter( event );" type="password" value="" /><br />
	                <label>&nbsp;</label>
	                <input type="button" onclick="enviar_formulario()" name="enviar" value="" class="button" />
	                </form>
	                <p><a href="recupera_senha.php?tema=<?php echo $tema;?>">Esqueceu sua senha?</a></p>
	            </div>
	        </div>
	        
	    </div>
	    <div id="geral_bot"><br />
	</div>
	</div>
	
	
	
	<!-- cantos arredondados - base-->
	<div id="geral_base_esq">
		<div id="geral_base_dir"></div>
	</div>	
	<!-- fim DIV GERAL-->
	</div>	
	<!-- fim DIV SUBCONTEUDO-->

</div>	
<!-- fim DIV CONTEUDO-->

</body>
</html>
<script type="text/javascript">

	if ( document.formulario.usucpf.value == '' ) {
		document.formulario.usucpf.focus();
	} else {
		document.formulario.ususenha.focus();
	}
	
	function enviar_formulario_Enter(e)
	{
	    if (e.keyCode == 13)
	    {
	        enviar_formulario();
	    }
	}

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
		if ( document.formulario.ususenha.value == "" ) {
			mensagem += '\nÉ necessário preencher a senha.';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

	function controlar_foco_cpf( evento ) {
		if ( window.event || evento.which ) {
			if ( evento.keyCode == 13) {
				return document.formulario.ususenha.focus();
			};
		} else {
			return true;
		}
	}

	function controlar_foco_senha( evento ) {
		if ( window.event || evento.which ) {
			if ( evento.keyCode == 13) {
				return enviar_formulario();
			};
		} else {
			return true;
		}
	}

</script>