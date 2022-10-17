<?php
// carrega as funções gerais
require_once "config.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/arquivo.inc";

$db = new cls_banco();
$intIdArquivo = substr($_REQUEST['id'],0,strpos($_REQUEST['id'],"_"));
$strNomeArquivo = substr($_REQUEST['id'],strpos($_REQUEST['id'],"_")+1);
$funcao = base64_decode($_REQUEST['parametro']);



if(removeArquivo( $intIdArquivo , $strNomeArquivo)){

	if(!empty($funcao)){
		$parametro = explode(',',$funcao);

		$funcao = (array_shift($parametro));

		call_user_func_array($funcao,$parametro);
	}
	echo "<script>
		alert('Arquivo removido com sucesso.');
		window.location.href = '{$_SERVER['HTTP_REFERER']}';
		</script>";
}else {
	echo "<script>
		alert('Não foi possivel remover o arquivo.');
		window.location.href = '{$_SERVER['HTTP_REFERER']}';
		</script>";
}
?>