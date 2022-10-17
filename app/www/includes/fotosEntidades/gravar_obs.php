<?php
include "config.inc";

header('Content-Type: text/html; charset=iso-8859-1');

include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db = new cls_banco();

if(count($_REQUEST['supobs'])>0) {
	
	foreach($_REQUEST['supobs'] as $key => $descricao) {
		if(is_numeric($key)) {
			$sql = "UPDATE public.arquivo SET arqdescricao='".substr($descricao,0,255)."' WHERE arqid='".$key."'";
			$db->executar($sql);
			$db->commit();
		} else {
			$fp = fopen("../../../arquivos/entidades/imgs_tmp/".$key.".d", 'w');
			fwrite($fp, $descricao);
			fclose($fp);
		}
		echo "<script>
				parent.inserirobservacaothumnails('".$key."','".$descricao."');
			  </script>";
	}
	
}

echo "<script>alert('Observações gravadas com sucesso.');</script>";

?>