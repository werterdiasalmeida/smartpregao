<?php

// verifica se ocorre algum evento
if(isset($_REQUEST['evento']) && $_POST && ($_REQUEST['evento'] != '') ){
	switch($_REQUEST['evento']) {
		// atualizar os dados da secretaria
		case "G":
		$sql = "UPDATE painel.secretaria SET 
				secdsc='".$_POST['sbatitulo']."'
       		    WHERE secid=".$_POST['secid'].";";
		$db->executar($sql);

		$sql = "DELETE FROM painel.acaosecretaria WHERE secid=$_POST[secid]";
		$db->executar($sql);
		
		if(is_array($_POST['ptrid'])) {
			foreach($_POST['ptrid'] as $ptrid ) {
				$sql = "INSERT INTO painel.acaosecretaria 
					  		(secid, acaid) 
	     			  	  VALUES ($_POST[secid],$ptrid)
	     			   ";	
				$db->pegaUm($sql);
			}
		}		
		
		$db->commit();

		echo "<script>alert('Dados gravados com sucesso.'); 
					  location.href='painel.php?modulo=sistema/tabelaapoio/secretarias/inseresec&acao=A&evento=G&secid=$_POST[secid]';
			  </script>";
		exit;
						
		
		// inserir plano interno
		case 'I':
		
		$sql = "INSERT INTO painel.secretaria(
            		secdsc,secstatus)
		    	 VALUES ('".$_POST['sbatitulo']."', 'A') 
		    	 RETURNING secid;
		       ";
		$sbaid = $db->pegaUm($sql);
	
		if(is_array($_POST['ptrid'])) {
			foreach($_POST['ptrid'] as $ptrid ) {
				$sql = "INSERT INTO painel.acaosecretaria 
					  		(secid, acaid) 
	     			  	  VALUES ($sbaid,$ptrid)
	     			   ";	
				$db->pegaUm($sql);
			}
		}
		
		$db->commit();
		echo "<script>alert('Dados salvos com sucesso.');window.location = '?modulo=sistema/tabelaapoio/secretarias/inseresec&acao=A&evento=G&secid=$sbaid';</script>";
		exit;
		
	}
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

monta_titulo( "Cadastro de Secretaria", '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

if(isset($_REQUEST['secid'])) {
			$sbatitulo = $db->pegaLinha("SELECT secdsc FROM painel.secretaria WHERE secid = '".$_REQUEST['secid']."'");
					
			$sql = "SELECT ac.acaid as ptrid,ac.acadsc as ptres 
					FROM painel.acao ac, painel.acaosecretaria secac
					WHERE ac.acaid = secac.acaid AND secac.secid=$_REQUEST[secid]";
			$ptresacao = $db->carregar($sql);

			
} 

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
<input type="hidden" name="evento" id="evento" value="<? if($_REQUEST['secid'])echo 'G'; else echo 'I'; ?>">
<input type="hidden" name="sbaid"  id="sbaid" value="<?=$_REQUEST['secid'];?>">

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<tr>
	<td align='right' class="SubTituloDireita">Secretaria:</td>
    <td><? 
			if($_REQUEST['secid']){
				$sql = "select secdsc from painel.secretaria where secid = $_REQUEST[secid]";
				$valor = $db->pegaum($sql);
				$sbatitulo = $valor;
				echo "<input type='hidden' name='secid' value='$_REQUEST[secid]'>";
    			echo campo_texto('sbatitulo','N','S','',75,255,'',''); 
			}
			else {
				$sbacao = $_REQUEST['sbatitulo'];
				echo "<input type='hidden' name='dsc' value=''>";
    			echo campo_texto('sbatitulo','N','S','',75,255,'',''); 
			}	 
		?>
		<img src="../imagens/obrig.gif" border="0">
	</td>
</tr>

<tr>
	<td class="SubTituloDireita" valign="top">Ações vinculadas:</td>
    <td >
        <table cellpadding="0" border="0" width="98%"  id="orcamento"  style="BORDER-RIGHT: #C9C9C9 1px solid; BORDER-TOP: #C9C9C9 1px solid; BORDER-LEFT: #C9C9C9 1px solid; BORDER-BOTTOM: #C9C9C9 1px solid;">
		<tr>
			<td style="background-color: #C9C9C9;" align="center" nowrap><input type="hidden" name="ptrid"></td>
			
		</tr>
		<? 
		if($ptresacao[0]) {
			$valortotal = 0;
			$cor = 0;
			foreach($ptresacao as $acpl) { 
		?>
        <tr style="height:30px;<? echo (($cor%2)?"":"background-color:#DCDCDC;"); ?>" id="ptrid_<? echo $acpl['ptrid']; ?>">
			<td align="left">
				
				<? 
				echo "<input type='checkbox' checked id='check' onclick='resultado($acpl[ptrid]);'>".
				$acpl['ptres']; ?>
				<input type='hidden' name='ptrid[<? echo $acpl['ptrid']; ?>]' value='<? echo $acpl['ptrid']; ?>'>
			</td>
		</tr>
		<? 
				$cor++;
			}
		} 
		?>
        <tr>
			<td align="right" colspan="6"><input type="button" onclick="abrir_lista_acoes();" id="btn_selecionar_acaptres" value="Vincular Ações"></td>
		</tr>
        </table>
    </td>
</tr>

<tr bgcolor="#cccccc">
	      <td></td>
	  	  <td>
	  	  <input type="button" class="botao" name="btg" value="Gravar" onclick="submeter();">
	  	  <? if($_REQUEST['acaid']) { ?>
	  	  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	  	  <input type="button" class="botao" name="btassociar2" value="Novo" onclick="validabotao('N');">
	  	  <?}?>
	  	  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	  	  <input type="button" class="botao" name="btassociar3" value="Voltar" onclick="validabotao('V');"></td>
	  	  
	      </tr>
</table>
</form>
<script type="text/javascript">
function submeter() {
		if(document.formulario.sbatitulo.value==''){
			alert('O campo "Secretaria" é de preenchimento obrigatório!');
			return false;
		}
		else
			document.formulario.submit();

}

function abrir_lista_acoes() {
	var janela = window.open( '?modulo=sistema/tabelaapoio/secretarias/listaracoes&acao=A', 'blank', 'height=600,width=500,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
	janela.focus();
}

function validabotao(bot) {
	form = document.formulario;
	if(bot=='N'){
		window.location='?modulo=sistema/tabelaapoio/secretarias/inseresec&acao=A';
	}
	if(bot=='V'){
		window.location='?modulo=sistema/tabelaapoio/secretarias/cadastrosec&acao=A';
	}
}
function limitaTitulo() {
	tit = document.formulario.sbatitulo.value;
	tot_tit = tit.length;
	if(tot_tit>100){
		alert('O campo Secretaria possui o limite de 100 caracteres.');
		document.formulario.btg.focus();
	}
}

function resultado(ptrid){
	var tabela = document.getElementById('orcamento');
	tabela.deleteRow(window.document.getElementById('ptrid_'+ptrid).rowIndex);
}

</script>