<?php


function atualizarPublicArquivo($arrValidacao = null) {
	
	global $db;
	
	
	if($_FILES['arquivo']) {
		
		$sql = "SELECT * FROM public.arquivo WHERE arqid IN('".implode("','", array_keys($_FILES['arquivo']['name']))."')";
		$arquivos = $db->carregar($sql);
		if($arquivos[0]) {
			
			foreach($arquivos as $arq) {
				$dados_arq[$arq['arqid']] = array('arqnome' => $arq['arqnome'],
												  'arqtipo' => $arq['arqtipo'],
												  'arqextensao' => $arq['arqextensao'],
												  'arqtamanho' => $arq['arqtamanho']);
			}
			
		}

		foreach(array_keys($_FILES['arquivo']['name']) as $arqid) {
			
			if($_FILES['arquivo']['error'][$arqid] == 0) {
			
				/*
				 * Trecho de validaчуo
				 */
				
				$validacao = true;
				unset($up);
				
				if($arrValidacao) {
					if(in_array('extensao',$arrValidacao)) {
						
						if($dados_arq[$arqid]['arqextensao'] && (strlen($dados_arq[$arqid]['arqextensao']) == 4 || strlen($dados_arq[$arqid]['arqextensao']) == 3)) {
							if(strtoupper(end(explode(".", $_FILES['arquivo']['name'][$arqid]))) != strtoupper($dados_arq[$arqid]['arqextensao'])) {
								$textoErro[$arqid] .= "Extensуo diferente do arquivo original. ";
								$validacao = false;
							}
						} else {
							$up[] = "arqextensao='".strtoupper(end(explode(".", $_FILES['arquivo']['name'][$arqid])))."'";
						}
						
					}
				}
				
				/* DANIEL PEDIU PRA NAO VALIDAR!!
				if($dados_arq[$arqid]['arqnome']) {
					if($_FILES['arquivo']['name'][$arqid] != $dados_arq[$arqid]['arqnome'].".".$dados_arq[$arqid]['arqextensao']) {
						$textoErro[$arqid] .= "Nome dos arquivos diferentes. ";
						$validacao = false;
					}
				}
				
				if($dados_arq[$arqid]['arqtamanho']) {
					if($_FILES['arquivo']['size'][$arqid] != $dados_arq[$arqid]['arqtamanho']) {
						$textoErro[$arqid] .= "Tamanho dos arquivos sуo diferentes. ";
						$validacao = false;
					}
				} else {
					$up[] = "arqtamanho='".$_FILES['arquivo']['size'][$arqid]."'";
				}
				*/
				
				/*
				 * FIM - Trecho de validaчуo
				 */
				
	
				/*
				 * Trecho de gravaчуo
				 */
				
				if($_REQUEST['_sisdiretorio']) $dir = $_REQUEST['_sisdiretorio'];
				else $dir = $_SESSION['sisdiretorio'];
				
				if($validacao) {
				
					$caminho = APPRAIZ . 'arquivos/'. $dir .'/'. floor($arqid/1000) .'/'. $arqid;
					$caminhodir = APPRAIZ . 'arquivos/'. $dir .'/'. floor($arqid/1000) .'/';
					
					if( !is_dir($caminhodir) ) {
						mkdir($caminhodir, 0777);
					}
					
					switch($_FILES['arquivo']['type'][$arqid]) {
						
						case 'image/jpe':
						case 'imagejpeg__':
						case 'image/pjpeg':
						case 'image/jpeg':
							
							ini_set("memory_limit", "128M");
							list($width, $height) = getimagesize($_FILES['arquivo']['tmp_name'][$arqid]);
							$original_x = $width;
							$original_y = $height;
							// se a largura for maior que altura
							if($original_x > $original_y) {
								$porcentagem = (100 * 640) / $original_x;
							}else {
								$porcentagem = (100 * 480) / $original_y;
							}
							$tamanho_x = $original_x * ($porcentagem / 100);
							$tamanho_y = $original_y * ($porcentagem / 100);
							$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
							$image   = imagecreatefromjpeg($_FILES['arquivo']['tmp_name'][$arqid]);
							imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
							imagejpeg($image_p, $caminho, 100);
							//Clean-up memory
							ImageDestroy($image_p);
							//Clean-up memory
							ImageDestroy($image);
							break;
						default:
							move_uploaded_file( $_FILES['arquivo']['tmp_name'][$arqid], $caminho );
							
					}
					
					$textoSucesso[$arqid] = "Inserido com sucesso";
					
					if($up) {
						$db->executar("UPDATE public.arquivo SET ".implode(",",$up)." WHERE arqid='".$arqid."'");
					}
					
					$sql = "SELECT arcid FROM public.arquivo_recuperado WHERE arqid='".$arqid."'";
					$test = $db->pegaUm($sql);
					
					if(!$test) $db->executar("INSERT INTO public.arquivo_recuperado(usucpf, arcdata, arqid, arqnome_, arqtamanho_, arqtipo_)
    								VALUES ('".$_SESSION['usucpf']."', NOW(), '".$arqid."', '".$_FILES['arquivo']['name'][$arqid]."', '".$_FILES['arquivo']['size'][$arqid]."', '".$_FILES['arquivo']['type'][$arqid]."');");
					
					
				
				
				}
				
				/*
				 * FIM - Trecho de gravaчуo
				 */
			}
			
		}
	}
	
	$db->commit();
	
	return array('TRUE' => $textoSucesso, 'FALSE' => $textoErro);
	
}

?>