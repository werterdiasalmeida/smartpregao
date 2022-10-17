<?

include_once (APPRAIZ.'www/painel/_funcoes_painel_controle.php');

$_SESSION['tema'] = !$_SESSION['tema'] ? "padrao" : $_SESSION['tema'];
$_SESSION['tema'] = $_REQUEST['tema'] ? $_REQUEST['tema'] : $_SESSION['tema'];
$tema = $_SESSION['tema']; ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Painel de Controle do MEC</title>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/agrupador.js"></script>

<!-- TEMAS -->
<link href="/painel/novo/temas/<? echo $tema; ?>/css/estilo.css" rel="stylesheet" type="text/css" />
<!-- FIM - TEMAS -->

<style>
	.campoEstilo{
		width:200px;
		fontt-size:40px;
	}
</style>

<link href="/painel/novo/css/estrutura.css" rel="stylesheet" type="text/css" />

</head>

<body>

<div id="topo">
	<!-- logo -->
	<div id="logo"><img src="/painel/novo/images/logo.png" alt="Assinatura Visual Painel Mec" /></div>
   
<!-- login -->
    <div id="login">
		<div id="botoes_topo">
            <img src="/painel/novo/images/bt_impressora.png" alt="Clique para imprimir" />
            <img src="/painel/novo/images/bt_ajuda.png" alt="Precisa de ajuda?"  />
            <img src="/painel/novo/images/bt_pasta.png" alt="Abrir diretório" />
            <img src="/painel/novo/images/bt_fechar.png" onclick="window.location.href='logout.php'" alt="Sair" />
        </div>
        <div style="color:#FFFFFF;padding-right:10px;font-size:12px">
    	Seja bem vindo <b><?php echo $_SESSION['usunome']?></b>.
    	</div>
    </div>
    
</div>

<? exibeAbaUsuario2($_SESSION['usucpf'],$abaid); ?>

<!-- cantos arredondados - topo-->
<div id="geral_top_esq">
	<div id="geral_top_dir"></div>
</div>

<!-- DIV CONTEÚDO GERAL-->
<div id="geral">
	<div id="ferramentas">
    	<div id="tema">
        	<form  name="" action="#" method="post">
	    	<select name="tema" onchange="form.submit();">
            	<option value="padrao" <?php if($tema=='padrao') : echo "selected";endif; ?> >Padrão</option>
                <option value="sol" <?php if($tema=='sol') : echo "selected";endif; ?> >Sol</option>
                <option value="dark" <?php if($tema=='dark') : echo "selected";endif; ?> >Dark</option>
            </select>
        <input type="hidden" value="<?php echo $aba?>" />
	    </form>
        </div>
        
        <div id="botoessistema">
		    <img style="margin-right:10px" src="../imagens/img_indbrasil.gif" alt="Indicadores do Brasil" onclick="abrasil();"/> 
		    <img src="/painel/novo/images/bt_pesquisar_indicador.png" alt="Pesquisar Indicador" onclick="pesquisarIndicador();"/>
		    <img src="/painel/novo/images/bt_localizar_mapa.png" alt="Localizar no mapa" onclick="buscarMapa();"/>
			<img src="/painel/novo/images/bt_add_conteudo.png" style="display:<?php echo $_REQUEST['abaid'] == "paginaInicial" || !$_REQUEST['abaid'] ? "none" : "" ?>" alt="Adicionar conte&uacute;do" id="button_addCaixa" onclick="addCaixa();"/>
        </div>
    </div>

<table width="100%" border="0" cellpadding="0" cellspacing="0"><tr><td>
<div id="conteudo">
<div id="aguarde"></div>




