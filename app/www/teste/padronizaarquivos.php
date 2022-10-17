<?
// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';
require_once APPRAIZ . "www/obras/plugins/class/ClassImage.php";
// abre conexão com o servidor de banco de dados
$db = new cls_banco();


/* PADRONIZANDO ./documentos/ */

ini_set("memory_limit", "32M");

// Retirando todos as extensões do nome do arquivo... Caso a extensão não esteja no BD, insere.
$sql = "SELECT * FROM obras.arquivosobra AS obr
		LEFT JOIN public.arquivo AS arq ON arq.arqid = obr.arqid"; 
$arquivos = $db->carregar($sql);

if($arquivos) {
	foreach($arquivos as $arquivo) {
		$nomefile = explode(".", $arquivo['arqnome']);
		if(count($nomefile) > 0) {
			if(!$arquivo['arqextensao']) {
				$extsql = ", arqextensao = '". end($nomefile) ."'";
			}
			$sql = "UPDATE public.arquivo SET arqnome = '". current($nomefile) ."' $extsql WHERE arqid = '". $arquivo['arqid'] ."'";
			$db->executar($sql);
			$db->commit();
		}
	}
}

// Varrendo todos os arquivo de obras sem as extensões.. 
$sql = "SELECT * FROM obras.arquivosobra AS aobr 
		LEFT JOIN public.arquivo AS arq ON aobr.arqid = arq.arqid";
$arquivos = $db->carregar($sql);

foreach ($arquivos as $arq) {
	// Verificando se o arquivo pode ser lido..
	if(is_readable("../../arquivos/obras/documentos/".$arq['arqnome'].".".$arq['arqextensao'])) {
		// Verificando se ja existe uma pasta de acordo com o id do arquivo..
		if(!is_dir('../../arquivos/obras/'.floor($arq['arqid']/1000))) {
			$conf = mkdir(APPRAIZ.'/arquivos/obras/'.floor($arq['arqid']/1000), 0777);
			if(!$conf) {
				echo "Não foi possível criar a pasta. Verifique com o administrador.";
				exit;	
			}
		}
		$grava = true;
		// Se o arquivo for do tipo imagem, compactar..
		if($arq['arqtipo'] == 'image/jpeg') {
			$max_x = 640;
			$max_y = 500;
			$nomefoto = "imagem_".$arq['arqnome'].".".$arq['arqextensao'];
			//	pega o tamanho da imagem ($original_x, $original_y)
			list($width, $height) = getimagesize("../../arquivos/obras/documentos/".$arq['arqnome'].".".$arq['arqextensao']);
			echo "NAME:".$arq['arqnome'].".".$arq['arqextensao']."<br />";
			echo "W".$width." H".$height."<br />";
			$original_x = $width;
			$original_y = $height;
			if($original_x < 5000 && $original_y < 5000) {
				// se a largura for maior que altura
				if($original_x > $original_y) {
 					$porcentagem = (100 * $max_x) / $original_x;      
				}else {
			   		$porcentagem = (100 * $max_y) / $original_y;  
				}
				$tamanho_x = $original_x * ($porcentagem / 100);
				$tamanho_y = $original_y * ($porcentagem / 100);
				$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
				$image   = imagecreatefromjpeg("../../arquivos/obras/documentos/".$arq['arqnome'].".".$arq['arqextensao']);
				imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
				imagejpeg($image_p, "../../arquivos/obras/documentos/".$nomefoto);
				//Clean-up memory
				ImageDestroy($image_p);
				//Clean-up memory
				ImageDestroy($image);
			} else {
				// Caso o arquivo seja muito grande, não gravar e excluir do BD..
				$grava = false;
			}
		} else {
			$nomefoto = $arq['arqnome'].".".$arq['arqextensao'];
		}
		// Gravando arquivo...
		if($grava) {
			if(@copy("../../arquivos/obras/documentos/".$nomefoto, "../../arquivos/obras/".floor($arq['arqid']/1000)."/".$arq['arqid'])){
				echo "MODO 1 - Arquivo N: ". $arq['arqid'] ." transferido com sucesso.<br />";
			} else {
				echo "Problemas na cópia do arquivo N: ". $arq['arqid'] .".<br />";
				exit;
			}
		} else {
			echo "Arquivo ".$arq['arqnome'].".".$arq['arqextensao']." é muito grande<br />";
			// Excluindo do BD, arquivo muito grande..
			$sql = "DELETE FROM obras.arquivosobra WHERE aqoid = '".$arq['aqoid']."'";
			$db->executar($sql);
			$db->commit();
			$sql = "DELETE FROM public.arquivo WHERE arqid = '".$arq['arqid']."'";
			$db->executar($sql);
			$db->commit();
		}
	} else {
		echo "MODO 1 - Arquivo ".$arq['arqnome'].".".$arq['arqextensao']." não foi encontrado.<br />";
	}
}
/* FIM PADRONIZANDO ./documentos/ */

/* PADRONIZANDO ./imgs/ */

unset($arquivos);

// Corrigindo bug do arqtipo
$sql = "UPDATE public.arquivo SET arqtipo = 'image/jpeg' WHERE arqtipo = 'image/pjpeg'";
$db->executar($sql);
$db->commit();
// Corrigindo bug do arqtipo
$sql = "UPDATE public.arquivo SET arqtipo = 'image/jpeg' WHERE arqtipo = 'image'";
$db->executar($sql);
$db->commit();

// Retirando todos as extensões do nome do arquivo... Caso a extensão não esteja no BD, insere.
$sql = "SELECT * FROM obras.fotos AS fot 
		LEFT JOIN public.arquivo AS arq ON fot.arqid = arq.arqid";
$arquivos = $db->carregar($sql);

if($arquivos) {
	foreach($arquivos as $arquivo) {
		$nomefile = explode(".", $arquivo['arqnome']);
		if(count($nomefile) > 0) {
			if(!$arquivo['arqextensao']) {
				$extsql = ", arqextensao = '". end($nomefile) ."'";
			}
			$sql = "UPDATE public.arquivo SET arqnome = '". current($nomefile) ."' $extsql WHERE arqid = '". $arquivo['arqid'] ."'";
			$db->executar($sql);
			$db->commit();
		}
	}
}


$sql = "SELECT * FROM obras.fotos AS fot 
		LEFT JOIN public.arquivo AS arq ON fot.arqid = arq.arqid";
$arquivos = $db->carregar($sql);

foreach($arquivos as $arq) {
	// Verificando se a pasta existe de acordo com o id do arquivo
	if(!is_dir('../../arquivos/obras/'.floor($arq['arqid']/1000))) {
		$conf = mkdir(APPRAIZ.'/arquivos/obras/'.floor($arq['arqid']/1000), 0777);
		if(!$conf) {
			echo "Não foi possível criar a pasta. Verifique com o administrador.";
			exit;	
		}
	}
	// Copiando arquivo, este tipo ja esta sendo compactado..
	if(copy("../../arquivos/obras/imgs/".$arq['arqnome'].".".$arq['arqextensao'],"../../arquivos/obras/".floor($arq['arqid']/1000)."/".$arq['arqid'])){
		echo "MODO2 - Arquivo N: ". $arq['arqid'] ." transferido com sucesso.<br />";
	} else {
		echo "Problemas na cópia do arquivo N: ". $arq['arqid'] .".<br />";
		exit;
	}

}
echo "Fim";
/* PADRONIZANDO ./imgs/ */
?>