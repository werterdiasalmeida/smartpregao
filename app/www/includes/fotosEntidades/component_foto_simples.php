<?
include "config.inc";
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

function deletarfotos($dados) {
	global $db;

	if($dados['entid']) {
		$fotid = $db->pegaUm("SELECT fotid FROM entidade.fotoentidade WHERE entid='".$dados['entid']."' AND fotordem='".$dados['fotordem']."'");
		
		if($fotid) {
			$db->executar("DELETE FROM entidade.fotoentidade WHERE fotid='".$fotid."'");
			$db->executar("UPDATE entidade.fotoentidade SET fotordem=(fotordem-1),fotbox='imageBox'||(fotordem-1) WHERE fotordem > ".$dados['fotordem']." AND entid='".$dados['entid']."'");
			$db->commit();
		}

	}
	exit;
}


function ordenarfotos($dados) {
	global $db;
	
	if($dados['entid']) {
		$fotatual = $db->pegaUm("SELECT fotid FROM entidade.fotoentidade WHERE entid='".$dados['entid']."' AND fotordem='".$dados['ordematual']."'");
		$fotir = $db->pegaUm("SELECT fotid FROM entidade.fotoentidade WHERE entid='".$dados['entid']."' AND fotordem='".$dados['ordemir']."'");
		if($fotatual && $fotir) {
			$db->executar("UPDATE entidade.fotoentidade SET fotordem='".$dados['ordemir']."', fotbox='imageBox".$dados['ordemir']."' WHERE fotid='".$fotatual."'");
			$db->executar("UPDATE entidade.fotoentidade SET fotordem='".$dados['ordematual']."', fotbox='imageBox".$dados['ordematual']."' WHERE fotid='".$fotir."'");
			$db->commit();
		}
	}
	exit;
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
}

header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past


header('Content-Type: text/html; charset=iso-8859-1');
?>
<html>
<head>
	<meta http-equiv="Pragma" content="no-cache">
	<title>Inserir fotos</title>
	<script language="JavaScript" src="../../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
	<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	<script type="text/javascript" src="../../includes/prototype.js"></script>
<script>
var MSIE = navigator.userAgent.indexOf('MSIE')>=0?true:false;

function ajaxatualizar(params,iddestinatario) {
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				if(iddestinatario) {
					document.getElementById(iddestinatario).innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				if(iddestinatario) {
					document.getElementById(iddestinatario).innerHTML = 'Carregando...';
				}
			}
		});
}

function excluirfotosentidade(indiceselecionado) {
	var tabela = document.getElementById('listaimagens');
	var thumbs = window.opener.document.getElementById("thumbnails");
	if((tabela.rows.length-2) > indiceselecionado) {
		if(MSIE){
			var thumb2 = thumbs.childNodes[indiceselecionado].innerHTML;
			thumbs.childNodes[indiceselecionado-1].innerHTML = thumb2;
			thumbs.childNodes[indiceselecionado].innerHTML = "";
		} else {
			var thumb2 = window.opener.document.getElementById("imageBox"+(indiceselecionado+1)).innerHTML;
			window.opener.document.getElementById("imageBox"+indiceselecionado).innerHTML = thumb2;
			window.opener.document.getElementById("imageBox"+indiceselecionado).innerHTML = "";
		}
	} else {
		if(MSIE){
			thumbs.childNodes[indiceselecionado].innerHTML = "";
		} else {
			window.opener.document.getElementById("imageBox"+indiceselecionado).innerHTML = "";
		}
	}
	tabela.deleteRow(indiceselecionado);
	if(tabela.rows.length == 2) {
		document.getElementById('submit1').disabled = true;
		document.getElementById('submit2').disabled = true;
	}
	ajaxatualizar("fotordem="+indiceselecionado+"&requisicao=deletarfotos");
	window.opener.UpdateListFotoSimples(indiceselecionado);
}


function ordenar(indiceselecionado, acao) {
	switch(acao) {
		case 'subir':
			if(indiceselecionado > 1) {
				// alterando ordem na tabela
				var tabela = document.getElementById('listaimagens');
				var linha1 = tabela.rows[indiceselecionado];
				var desc1 = tabela.rows[indiceselecionado].cells[2].firstChild.value;
				var linha2 = tabela.rows[(indiceselecionado-1)];
				var desc2 = tabela.rows[(indiceselecionado-1)].cells[2].firstChild.value;
				var celula2_1 = linha2.cells[1].innerHTML;
				var celula2_2 = linha2.cells[2].innerHTML;
				var celula1_1 = linha1.cells[1].innerHTML;
				var celula1_2 = linha1.cells[2].innerHTML;
				linha1.cells[1].innerHTML = celula2_1;
				linha1.cells[2].innerHTML = celula2_2;
				linha2.cells[1].innerHTML = celula1_1;
				linha2.cells[2].innerHTML = celula1_2;
				linha1.cells[2].firstChild.value = desc2;
				linha2.cells[2].firstChild.value = desc1;
				// alterando ordem nos thumbnails
				if(MSIE){
					var thumbs = window.opener.document.getElementById("thumbnails");
					var thumb2 = thumbs.childNodes[(indiceselecionado-1)].innerHTML;
					var thumb1 = thumbs.childNodes[indiceselecionado].innerHTML;
					thumbs.childNodes[indiceselecionado].innerHTML = thumb2;
					thumbs.childNodes[(indiceselecionado-1)].innerHTML = thumb1;
				} else {
					var thumb2 = window.opener.document.getElementById("imageBox"+(indiceselecionado-1)).innerHTML;
					var thumb1 = window.opener.document.getElementById("imageBox"+indiceselecionado).innerHTML;
					window.opener.document.getElementById("imageBox"+indiceselecionado).innerHTML = thumb2;
					window.opener.document.getElementById("imageBox"+(indiceselecionado-1)).innerHTML = thumb1;
				}
				ajaxatualizar("ordematual="+indiceselecionado+"&ordemir="+(indiceselecionado-1)+"&requisicao=ordenarfotos");
			}
			break;
		case 'descer':
			var tabela = document.getElementById('listaimagens');
			if((tabela.rows.length-2) > indiceselecionado) {
				// alterando ordem na tabela
				var tabela = document.getElementById('listaimagens');
				var linha1 = tabela.rows[indiceselecionado];
				var desc1 = tabela.rows[indiceselecionado].cells[2].firstChild.value;
				var linha2 = tabela.rows[(indiceselecionado+1)];
				var desc2 = tabela.rows[(indiceselecionado+1)].cells[2].firstChild.value;
				var celula2_1 = linha2.cells[1].innerHTML;
				var celula2_2 = linha2.cells[2].innerHTML;
				var celula1_1 = linha1.cells[1].innerHTML;
				var celula1_2 = linha1.cells[2].innerHTML;
				linha1.cells[1].innerHTML = celula2_1;
				linha1.cells[2].innerHTML = celula2_2;
				linha2.cells[1].innerHTML = celula1_1;
				linha2.cells[2].innerHTML = celula1_2;
				linha1.cells[2].firstChild.value = desc2;
				linha2.cells[2].firstChild.value = desc1;
				// alterando ordem nos thumbnails
				if(MSIE){
					var thumbs = window.opener.document.getElementById("thumbnails");
					var thumb1 = thumbs.childNodes[indiceselecionado].innerHTML;
					var thumb2 = thumbs.childNodes[indiceselecionado-1].innerHTML;
					thumbs.childNodes[indiceselecionado].innerHTML = thumb2;
					thumbs.childNodes[indiceselecionado-1].innerHTML = thumb1;
				} else {
					var thumb2 = window.opener.document.getElementById("imageBox"+indiceselecionado).innerHTML;
					var thumb1 = window.opener.document.getElementById("imageBox"+(indiceselecionado+1)).innerHTML;
					window.opener.document.getElementById("imageBox"+(indiceselecionado+1)).innerHTML = thumb2;
					window.opener.document.getElementById("imageBox"+indiceselecionado).innerHTML = thumb1;
				}
				ajaxatualizar("ordematual="+indiceselecionado+"&ordemir="+(indiceselecionado+1)+"&requisicao=ordenarfotos");
			}
			break;
	} 	
}

function validarInsercaoFiles() {
	var anexovazio = true;
	for(i=0;i<document.anexo.elements.length;i++) {
		if(document.anexo.elements[i].type == "file") {
			if(document.anexo.elements[i].value != "") {
				anexovazio = false;
			}
		}
	}
	if(anexovazio) {
		alert("Não existem arquivos anexados.");
		return false;
	} else {
		return true;
	}
}

function inserirNovosArquivos() {
	var tabela = document.getElementById('inserirarquivos');
	for(i=0;i<2;i++) {
		var line = tabela.insertRow((tabela.rows.length-2));
		var cell = line.insertCell(0);
		cell.innerHTML = "<input type=\"file\" name=\"Filedata[]\">";
	}
}

function isNumber(numero){
	var CaractereInvalido = false;
	for (i=0; i < numero.length; i++) {
		var Caractere = numero.charAt(i);
		if(Caractere != "." && Caractere != "," && Caractere != "-"){
			if (isNaN(parseInt(Caractere))) CaractereInvalido = true;
		}
	}
	return !CaractereInvalido;
}
function inserirobservacaothumnails(id, descricao) {
	var opener_container = window.opener.document.getElementById("thumbnails");
	for(var k=0;k<opener_container.childNodes.length;k++){
		var child = opener_container.childNodes;
		if(child[k].firstChild){
			var imagem = child[k].firstChild;
			if(!isNumber(imagem.id)) {
				var img  = ""+imagem.id+"";
				if(img.indexOf("_") > 15) {
					if(img == id) {
						imagem.title = descricao
					}
				} else {
					if(img.substr((img.indexOf("_")+1)) == id) {
						imagem.title = descricao
					}
				}
			}
		}

	}
}

function inserirfotosthumbnails(id) {
	var tabela = document.getElementById('listaimagens');
	var opener_container = window.opener.document.getElementById("thumbnails");
	for(var k=0;k<opener_container.childNodes.length;k++){
		var child = opener_container.childNodes;
		if(child[k].className == "imageBox"){
			if(child[k].firstChild){
				continue;
			} else {
				if(isNumber(id)) {
					var imagem = "<img id='"+id+"' class='imageBox_theImage' style='margin:2px;' src='../../slideshow/slideshow/verimagem.php?arqid="+id+"&_sisarquivo=entidades'>";
					child[k].innerHTML = imagem;

				} else {
					var imagem = "<img id='"+id+"' style='width:68px;height:68px;' src='../includes/fotosEntidades/resize.php?img=../../../arquivos/entidades/imgs_tmp/"+id+"&w=68&h=68'><input type='hidden' id='"+child[k].id+"_"+id+"' name='fotos["+child[k].id+"]' value='"+id+"'>";
					child[k].innerHTML = imagem;
				}
				break;
			}
		}
	}
}
	
</script>
</head>
<iframe name="iframeUpload" style="display:none;"></iframe>
<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<br />
<form method="post" id="anexo" name="anexo" enctype="multipart/form-data" action="upload_simples.php?funcao=<? echo $_REQUEST['funcao']; echo (($_REQUEST['entid'])?"&entid=".$_REQUEST['entid']:""); ?>" onsubmit="return validarInsercaoFiles();" target="iframeUpload">
<table class="listagem" width="95%" align="center" id="inserirarquivos">
<tr>
<td class="SubTituloCentro">Anexar arquivos</td>
</tr>
<tr>
<td><input type="file" name="Filedata[]"></td>
</tr>
<tr>
<td><input type="file" name="Filedata[]"></td>
</tr>
<tr>
<td>
<a href="javascript:void(0);" onclick="inserirNovosArquivos();">Clique aqui para adicionar novos arquivos</a><br />
</td>
</tr>
<tr>
<td><input type="submit" name="sub" value="Enviar"></td>
</tr>
</table>
</form>
<form method="post" name="descricao" action="gravar_obs.php?funcao=<? echo $_REQUEST['funcao']; echo (($_REQUEST['entid'])?"&entid=".$_REQUEST['entid']:""); ?>" target="iframeUpload">
<table class="listagem" width="95%" align="center" id="listaimagens">
<tr bgcolor="#C0C0C0">
<td align="right" colspan="4">
<input type="submit" value="Gravar observações" id="submit1" disabled>
</td>
</tr>
<?
if($_REQUEST['entid']) {
	$sql = "SELECT arq.arqid, arq.arqdescricao FROM entidade.fotoentidade fot
			LEFT JOIN public.arquivo arq ON arq.arqid = fot.arqid 
			WHERE entid='".$_REQUEST['entid']."' 
			ORDER BY fotordem";
	$arqssup = $db->carregar($sql);
	if($arqssup[0]) {
		$habilitarsubmitsobs = true;
		$controletextarea=0;
		foreach($arqssup as $arq) {
			echo "<tr>";
			echo "<td><img id=\"setasubir\" src=\"../../imagens/seta_cima.gif\"  style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'subir');\"> <img id=\"setadescer\" src=\"../../imagens/seta_baixo.gif\" style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'descer');\"></td>";
			echo "<td>";
			echo "<img src='../../slideshow/slideshow/verimagem.php?arqid=".$arq['arqid']."&_sisarquivo=entidades' width=100 height=100>";
			echo "</td>"; 
			echo "<td>";
			echo "<textarea  id='supobs' name='supobs[".$arq['arqid']."]' cols='40' rows='5' onmouseover='MouseOver( this );' onfocus='MouseClick( this );'  onmouseout='MouseOut( this );'  onblur='MouseBlur( this );' onkeydown='textCounter( this.form.supobs[".$arq['arqid']."], this.form.no_supobs".$controletextarea.", 255 );'  onkeyup='textCounter( this.form.supobs[".$arq['arqid']."], this.form.no_supobs".$controletextarea.", 255);'>".$arq['arqdescricao']."</textarea><br /><input readonly style='text-align:right;border-left:#888888 3px solid;color:#808080;' type='text' name='no_supobs".$controletextarea."' size='6' maxlength='6' value='255' class='CampoEstilo'><font color='red' size='1' face='Verdana'> máximo de caracteres</font>";
			echo "</td>";
			echo "<td><img id=\"excluirfotovistoria\" src=\"../../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirfotosentidade(this.parentNode.parentNode.rowIndex);\"></td>";
			echo "</tr>";
			$controletextarea++;
		}
	}
}
?>
<tr bgcolor="#C0C0C0">
<td align="right" colspan="4">
<input type="submit" value="Gravar observações" id="submit2" disabled>
</td>
</tr>
</table>
<? if($habilitarsubmitsobs) { ?>
<script>
document.getElementById('submit1').disabled = false;
document.getElementById('submit2').disabled = false;
</script>
<? }?>
</form>
<script type="text/javascript">
var opener_container = window.opener.document.getElementById("thumbnails");
var tabela = window.document.getElementById("listaimagens");
var controletextarea=0;
for(var k=0;k<opener_container.childNodes.length;k++){
	var child = opener_container.childNodes;
	if(child[k].firstChild){
		var imagem = child[k].firstChild;
		if(!isNumber(imagem.id)) {
			document.getElementById('submit1').disabled = false;
			document.getElementById('submit2').disabled = false;
			var linha  = tabela.insertRow(tabela.rows.length-1);
			linha.id=imagem.id;
			var img  = ""+imagem.id+"";
			if(img.indexOf("_") > 15) {
				var celulaordenacao = linha.insertCell(0);
				celulaordenacao.innerHTML = "<img id=\"setasubir\" src=\"../../imagens/seta_cima.gif\"  style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'subir');\"> <img id=\"setadescer\" src=\"../../imagens/seta_baixo.gif\" style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'descer');\">";
				var celulaimg = linha.insertCell(1);
				celulaimg.innerHTML = "<img src='./resize.php?img=../../../arquivos/entidades/imgs_tmp/"+img+"&w=100&h=100'>";
				var celuladsc = linha.insertCell(2);
				celuladsc.innerHTML = "<textarea  id='supobs' name='supobs["+img+"]' cols='40' rows='5' onmouseover='MouseOver( this );' onfocus='MouseClick( this );'  onmouseout='MouseOut( this );'  onblur='MouseBlur( this );' onkeydown='textCounter( this.form.supobs["+controletextarea+"], this.form.no_supobs"+controletextarea+", 255 );'  onkeyup='textCounter( this.form.supobs["+controletextarea+"], this.form.no_supobs"+controletextarea+", 255);'>"+imagem.title+"</textarea><br><input readonly style='text-align:right;border-left:#888888 3px solid;color:#808080;' type='text' name='no_supobs"+controletextarea+"' size='6' maxlength='6' value='255' class='CampoEstilo'><font color='red' size='1' face='Verdana'> máximo de caracteres</font>";
				var celuladeleta = linha.insertCell(3);
				celuladeleta.innerHTML = "<img id=\"excluirfotovistoria\" src=\"../../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirfotosentidade(this.parentNode.parentNode.rowIndex);\">";
			} else {
				var celulaordenacao = linha.insertCell(0);
				celulaordenacao.innerHTML = "<img id=\"setasubir\" src=\"../../imagens/seta_cima.gif\"  style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,'subir');\"> <img id=\"setadescer\" src=\"../../../imagens/seta_baixo.gif\" style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,\'descer\');\">";
				var celulaimg = linha.insertCell(1);
				celulaimg.innerHTML = "<img src='./resize.php?img=../../../arquivos/entidades/imgs_tmp/"+img.substr((img.indexOf("_")+1))+"&w=100&h=100'>";
				var celuladsc = linha.insertCell(2);
				celuladsc.innerHTML = "<textarea  id='supobs' name='supobs["+img.substr((img.indexOf("_")+1))+"]' cols='40' rows='5' onmouseover='MouseOver( this );' onfocus='MouseClick( this );'  onmouseout='MouseOut( this );'  onblur='MouseBlur( this );' onkeydown='textCounter( this.form.supobs["+controletextarea+"], this.form.no_supobs"+controletextarea+", 255 );'  onkeyup='textCounter( this.form.supobs["+controletextarea+"], this.form.no_supobs"+controletextarea+", 255);'>"+imagem.title+"</textarea><br><input readonly style='text-align:right;border-left:#888888 3px solid;color:#808080;' type='text' name='no_supobs"+controletextarea+"' size='6' maxlength='6' value='255' class='CampoEstilo'><font color='red' size='1' face='Verdana'> máximo de caracteres</font>";
				var celuladeleta = linha.insertCell(3);
				celuladeleta.innerHTML = "<img id=\"excluirfotovistoria\" src=\"../../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirfotosentidade(this.parentNode.parentNode.rowIndex);\">";
			}
			controletextarea++;
		}
	}

}
</script>
</body>
</html>
