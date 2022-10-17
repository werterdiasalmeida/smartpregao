<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


switch($_REQUEST['acao']) {
	case 'limparpasta':
		if($handle = opendir(APPRAIZ.$_REQUEST['diretorio'])) {
		    while (false !== ($file = readdir($handle))) {
		    	if(!is_dir(APPRAIZ.$_REQUEST['diretorio']."/".$file))
		    		unlink(APPRAIZ.$_REQUEST['diretorio']."/".$file);
		    }
		    closedir($handle);
		}
		die("<script>
				alert('Arquivos foram deletados com sucesso');
				window.location='diretorios.php?diretorio={$_REQUEST['diretorio']}';
			 </script>");
	case 'pegararquivo':
		ob_clean();
		header("Content-Type: application/save"); 
		header("Content-Length:".filesize(APPRAIZ.$_REQUEST['diretorio'])); 
		header('Content-Disposition: attachment; filename="' . $_REQUEST['diretorio'] . '"'); 
		header("Content-Transfer-Encoding: binary");
		header('Expires: 0'); 
		header('Pragma: no-cache');
		readfile(APPRAIZ.$_REQUEST['diretorio']);
		exit;
		
}

if(!$_REQUEST['diretorio']) {
	$_REQUEST['diretorio'] = 'arquivos';
}
echo "<table>";
if($handle = opendir(APPRAIZ.$_REQUEST['diretorio'])) {
	echo "<tr><td colspan=2><a style=cursor:pointer; onclick=history.go(-1);>Voltar</a></td></tr>";
    while (false !== ($file = readdir($handle))) {
    	if ($file != "." && $file != "..") {
    	
	    	if(is_dir(APPRAIZ.$_REQUEST['diretorio']."/".$file))
	       		echo "<tr><td><a href=diretorios.php?diretorio={$_REQUEST['diretorio']}/{$file}>".$file."</a></td></tr>";
	       	else 
	       		echo "<tr><td><a href=diretorios.php?diretorio={$_REQUEST['diretorio']}/{$file}&acao=pegararquivo>".$file."</a></td></tr>";
       		
    	}
    }
    closedir($handle);
} else {
	echo "<tr><td>Não existem ícones na pasta</td></tr>";
}

echo "</table>";

echo "<table>
		<tr><td><a href=diretorios.php?diretorio={$_REQUEST['diretorio']}&acao=limparpasta>Limpar pasta</td></tr>
	  </table>";


?>