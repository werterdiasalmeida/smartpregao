<?php
// carrega as funções gerais
include_once 'config.inc';
include_once APPRAIZ . 'includes/classes_simec.inc';
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script>
	function abrepopupListaArquivo(arquivo){
		window.open('http://simec-local/geral/popuplistaArquivosHabilita.php?arquivo='+arquivo, 'arquivo', 'scrollbars=no,height=550,width=800,status=no,toolbar=no,menubar=yes,location=no');
	}
</script>
<?

function retornaArquivosDiretorio($dir){
	$itens = array();
	if( file_exists($dir) ){
		$diretorio = opendir($dir);
		// monta os vetores com os itens encontrados na pasta
		while($arquivo = readdir($diretorio)){
			$itens[] = $arquivo;
		}
		// ordena o vetor de itens
		sort($itens);
		
		// percorre o vetor para fazer a separacao entre arquivos e pastas 
		foreach ($itens as $key => $lista) {
			if($lista != '.' && $lista != '..'){				
				// checa se o tipo de arquivo encontrado é uma pasta
				if(is_file($dir.'/'.$lista)){
					//arquivo
					$arq = array("codigo" => $key,
								 "descricao" => $lista);
				}else{
					//pasta
					$arq = array("codigo" => $key,
								 "descricao" => '<a href="#" onclick="abrepopupListaArquivo(\''.$lista.'\');">'.$lista.'</a>');
				}
				$arquivo[] = $arq;
			}
		}
		return $arquivo;
	}else {
		return false;
	}
}

$db = new cls_banco();
$arArquivos = array();
$arArquivos = retornaArquivosDiretorio('../../arquivos/emenda/habilita');//../../arquivos/emenda/habilita

/*echo "<pre>";
print_r( $arArquivos );
echo "</pre>";*/

$cabecalho = array("Codigo", "Arquivo");

print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	print '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">'.'Lista de Arquivos Habilita'.'</label></td></tr><tr>';
	print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';

$db->monta_lista_array($arArquivos, $cabecalho, 20, 4, 'N','Center');

?>