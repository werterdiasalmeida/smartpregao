<?php
	// carrega as funções gerais
	require_once "config.inc";
	require_once APPRAIZ . "includes/funcoes.inc";
	require_once APPRAIZ . "includes/classes_simec.inc";
	require_once APPRAIZ . "includes/arquivo.inc";
	if ( $_SESSION['usucpf'] || $_REQUEST['tela_login'] ){
			$db = new cls_banco();
			$intIdArquivo = (integer) $_REQUEST[ "id" ];
			mostraArquivo( $intIdArquivo );
	}
?>