<?php

include "config.inc";
include APPRAIZ."includes/funcoes.inc";
include APPRAIZ."includes/classes_simec.inc";

$db = new cls_banco();

// monta cabeçalho


//include APPRAIZ . 'includes/cabecalho.inc';

include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'reuni/www/funcoes.php';
include APPRAIZ . 'includes/arquivo.inc';

print '<br/>';


//monta_titulo( $titulo_modulo, '&nbsp;' );

$unicod = base64_decode($_REQUEST['unicod']);


function montaParecer($tipoParecer,$unpid){
	global $db;
	if(!isset($unpid)) return false;
	$sql = "select sitcod , pardsc , arqid from reuni.parecer where tpacod = $tipoParecer and unpid = $unpid";
	return $db->pegaLinha($sql);
}

function montaSituacao($nome='',$readOnly=false,$codParecer=''){
	global  $db;
	$sql = "select sitdsc , sitcod from reuni.situacao";
	$dado = $db->carregar($sql);
	$saida = '';
	$check = '';
    $nome  = strtolower($nome);

	if($readOnly == false) {
		if(!empty($codParecer)){
			$sql = "select sitdsc from reuni.situacao where  sitcod = $codParecer";
			return $db->pegaUm($sql);
		}else return "Não Preechido";
	}

	foreach ($dado as $row)
	{

		if($row['sitcod'] == $codParecer) $check = "checked='checked'";
		$saida .= "<input type='radio' name='situacao_$nome' $read $check value='".$row['sitcod']."'>". $row['sitdsc']	. "&nbsp;";
		$check = '';
	}
	return $saida;
}



$docid = $db->pegaUm("select docid from reuni.unidadeproposta where unicod ='". $unicod . "' ");
$sql = "select unpid from reuni.unidadeproposta where unicod ='" . $unicod ."' and docid=".$docid;

$unpid = $db->pegaUm($sql);




if(isset($_FILES["arquivo"]) and !empty($_FILES['arquivo']['name'])){

	try{

		$codArquivo = salvarArquivo($_SESSION['usucpf'],$_SESSION['sisid'],$_FILES["arquivo"],getTiposArquivoEscritorio());
		if( isset($codArquivo) )
		{
			$sql = "update reuni.parecer set arqid = $codArquivo where  unpid = $unpid and tpacod = 7 ";
			$db->executar($sql);
			$db->commit();
		}
	}catch ( Exception $objError ){
		echo '<script> alert("'.$objError->getMessage().'")</script>';
	}


}

if(isset($_FILES["arquivoSesu"]) and !empty($_FILES['arquivoSesu']['name'])){

	try
	{
		$codArquivo = salvarArquivo($_SESSION['usucpf'],$_SESSION['sisid'],$_FILES["arquivoSesu"],getTiposArquivoEscritorio());
		if( isset($codArquivo) )
		{
			$sql = "update reuni.parecer set arqid = $codArquivo where  unpid = $unpid and tpacod = 4 ";

			$db->executar($sql);
			$db->commit();
		}
	}
	catch ( Exception $objError ){
		echo '<script> alert("'.$objError->getMessage().'")</script>';
	}
}


$sesu 			= montaParecer(4,$unpid);
$adhoc 			= montaParecer(5,$unpid);
$comissao 		= montaParecer(6,$unpid);
$final 			= montaParecer(7,$unpid);

$leituraSesu     		= reuni_podeVerParecerSesu($unicod);
$leituraAdhoc    		= reuni_podeVerParecerAdhoc($unicod);
$leituraComissao 		= reuni_podeVerParecerComissao($unicod);
$leituraParecerFinal	= reuni_podeVerParecerSesuFinal($unicod);

$faseSesu 		= reuni_podeEditarParecerSesu($unicod);
$faseAdhoc 		= reuni_podeEditarParecerAdhoc($unicod);
$faseComissao 	= reuni_podeEditarParecerComissao($unicod);
$faseFinal 		= reuni_podeEditarParecerSesuFinal($unicod);

?>
<!-- biblioteca javascript local -->
<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
<script type="text/javascript">
</script>
<script language="JavaScript" src="../../includes/tiny_mce.js"></script>
<script language="JavaScript">
//Editor de textos
tinyMCE.init({
	mode : "textareas",
	theme : "advanced",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
	theme_advanced_buttons1 :
	"undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright,justifyfull",
	theme_advanced_buttons2 : "",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	extended_valid_elements :
	"a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
	language : "pt_br",
	entity_encoding : "raw"
});
</script>
<form name="formulario" method="post" action="" enctype="multipart/form-data">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?if ($leituraSesu){?>
	
	<tr>
		<td class="SubTituloCentro"  colspan="2">
		<b>Parecer Geral SESU</b>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">
			<b>Qualificação:</b>
		</td>
		<td>
			<?=  montaSituacao('Sesu',$faseSesu,$sesu['sitcod'])?>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" align="right">
			<b>Observações:</b>
		</td>
		<td>	
			<?php
			$parecerSesu = $sesu['pardsc'];
			if($faseSesu)
			echo campo_textarea( 'parecerSesu', 'N', '', '',130, 20,'');
			else 
			echo strlen(trim($parecerSesu))==0 ? "Não preenchido" : $parecerSesu;
			?>
		</td>
	</tr>
	
		<tr>
			<td class="SubTituloDireita" align="right">
				<b>Arquivo</b>
			</td>
			<td>
				<?

				$codArquivo = $sesu['arqid'];

				if($faseSesu){
					echo '<input type="file" name="arquivoSesu" />';
					geraLinkParaRemoverArquivo($codArquivo);
				}


				geraLinkParaArquivo($codArquivo,true);
				?>
			</td>
		</tr>
	
	
				<tr>
				<td class="SubTituloDireita" align="right">
					
				</td>
				<td> 
					<? if($faseSesu){?>
						<input type="button" value="Salvar" name="salvarSesu" onclick="salvarParecerGeral('<?=base64_encode($unpid)?>',4);">	&nbsp;	
					<?}?>
				</td>	
			</tr>
			
			<?}?>
			
		<?if ($leituraAdhoc){?>	
		<tr>
			<td class="SubTituloCentro	" align="right" colspan="2">
				<b>Parecer Geral ADHOC</b>
			</td>
		</tr>
			<tr>
		<td class="SubTituloDireita" align="right">
			<b>Qualificação:</b>
		</td>
		<td>
			<?=  montaSituacao('Adhoc',$faseAdhoc,$adhoc['sitcod'])?>
		</td>
	</tr>	
		<tr>
			<td class="SubTituloDireita" align="right"><b>ADHOC:</b>
			</td>
			<td>
				<?php
				$parecerAdhoc = $adhoc['pardsc'];
				if($faseAdhoc)
				echo campo_textarea( 'parecerAdhoc', 'N', '', '',130, 20,'');
				else
				echo strlen(trim($parecerAdhoc))==0 ? "Não preenchido" : $parecerAdhoc;
				?>
			</td>
			
		</tr>
		<tr>
		<td class="SubTituloDireita">
		&nbsp;
		</td>
		  <td>
		  		<? if($faseAdhoc){?>
					<input type="button" value="Salvar" onclick="salvarParecerGeral('<?=base64_encode($unpid)?>',5)">
				<?}?>
			</td>
		</tr>

	
		<?}?>
		
		<?if ($leituraComissao){?>
		<tr>
			<td class="SubTituloCentro	" align="right" colspan="2">
				<b>Parecer Geral Comissão de Homologação</b>
			</td>
		</tr>
			<tr>
		<td class="SubTituloDireita" align="right">
			<b>Qualificação:</b>
		</td>
		<td>
			<?=  montaSituacao('Comissao',$faseComissao,$comissao['sitcod'])?>
		</td>
	</tr>	
		<tr>
			<td class="SubTituloDireita" align="right"><b>Comissão:</b>
			</td>
			<td>
				<?php
				$parecerComissao = $comissao['pardsc'];
				if($faseComissao)
				echo campo_textarea( 'parecerComissao', 'N', '', '',130, 20,'');
				else
				echo strlen(trim($parecerComissao))==0 ? "Não preenchido" : $parecerComissao;
				?>
			</td>
			
		</tr>
		<tr>
		<td class="SubTituloDireita">
		&nbsp;
		</td>
		  	<td>
		  		<? if($faseComissao){?>
					<input type="button" value="Salvar" onclick="salvarParecerGeral('<?=base64_encode($unpid)?>',6)">
				<?}?>
			</td>
		</tr>
		<?}?>
		
		<?if ($leituraParecerFinal){?>
	
		
		<tr>
			<td class="SubTituloCentro	" align="right" colspan="2">
				<b>Parecer Geral Final</b>
			</td>
		</tr>
			<tr>
		<td class="SubTituloDireita" align="right">
			<b>Qualificação:</b>
		</td>
		<td>
			
			<?=  montaSituacao('Final',$faseFinal,$final['sitcod'])?>
		</td>
	</tr>	
	
		<tr>
			<td class="SubTituloDireita" align="right"><b>Parecer Final:</b>
			</td>
			<td>
				<?php
				$parecerFinal = $final['pardsc'];
				if($faseFinal)
				echo campo_textarea( 'parecerFinal', 'N', '', '',130, 20,'');
				else
				echo  strlen(trim($parecerFinal))==0 ? "Não preenchido" : $parecerFinal;
				?>
			</td>
			
		</tr>
			<tr>
			<td class="SubTituloDireita" align="right"><b>Arquivo</b>
			</td>
		<td>	
			<?
			if($faseFinal){
				echo '<input type="file" name="arquivo" />';

				$codArquivo = $final['arqid'];

				geraLinkParaRemoverArquivo($codArquivo);

			}
			 ?>
			&nbsp;<?geraLinkParaArquivo( $final['arqid'] , true );?>
		</td>
		</tr>
		<tr>
		<td class="SubTituloDireita">
		&nbsp;
		</td>
		  	<td>
		  		<? if($faseFinal){?>
					<input type="button" value="Salvar" onclick="salvarParecerGeral('<?=base64_encode($unpid)?>',7)">
				<?}?>
			</td>
		</tr>
		<?}?>
		
		
		

</table>
</form>


<script>


function arquivo(cod){

	var request =  window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );

	request.open( 'POST', 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/ajax/arquivo.php') ;
	request.setRequestHeader( "Content-Type", "application/x-www-form-urlencoded; charset=iso-8859-1" );
	request.onreadystatechange=function()
	{
		if (request.readyState==4)
		{
			var x = request.responseText;
			alert(x);
			window.location.href = "reuni.php?modulo=principal/parecergeral&acao=C&unicod=<?=base64_encode($unicod)?>";

		}
	}
	request.send( 'cod=' +  cod );

}

function pegaRadio(objRadio){

	for ( var i = 0 ; i < objRadio.length ; i++ ){
		if (objRadio[i].checked){
			return objRadio[ i ].value;
			break ;
		}
	}
	return false;
}

function salvarParecerGeral(codUnidade,tipo)
{


	var objRadio  	  = "";
	var objCampoTexto = "";

	switch(tipo)
	{

		case 4:
		objRadio      = document.formulario.situacao_sesu;
		objCampoTexto =  tinyMCE.getContent('parecerSesu');
		break

		case 5:
		objRadio 	  = document.formulario.situacao_adhoc;
		objCampoTexto =  tinyMCE.getContent('parecerAdhoc');
		break

		case 6:
		objRadio 	  = document.formulario.situacao_comissao;
		objCampoTexto =  tinyMCE.getContent('parecerComissao');
		break

		case 7:
		objRadio 	  = document.formulario.situacao_final;
		objCampoTexto =  tinyMCE.getContent('parecerFinal');
		break

		default:
		alert('Problemas na excução do script!');
		return false;
	}




	var valorRadio = pegaRadio(objRadio);




	var mensagem = "";

	if(trim(objCampoTexto) == ''){
		mensagem = "Informe o campo Descritivo. \n";
	}


	if(valorRadio == ''){
		mensagem += "Informe o campo Qualificação. \n";
	}

	if(mensagem.length > 0){
		alert(mensagem);
		return;
	}



	var request =  window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );

	request.open( 'POST', 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/ajax/salvaParecerGeral.php');

	request.setRequestHeader( "Content-Type", "application/x-www-form-urlencoded; charset=iso-8859-1" );

	request.onreadystatechange=function()
	{
		if (request.readyState==4)
		{
			var x = request.responseText;
			alert(x);
			window.opener.reloadPage();
			document.formulario.submit();
		}
	}
	request.send( 'parecer=' + escape( objCampoTexto ) + "&codUnidade=" + codUnidade + "&tipoParecer=" + tipo + "&situacao=" + valorRadio  );

}

</script>