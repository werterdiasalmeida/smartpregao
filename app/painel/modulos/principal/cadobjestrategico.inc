<? 
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = "Cadastro Objetivo Estratégico";
monta_titulo( $titulo, '&nbsp;' );

if(isset($_REQUEST[evento]) && ($_REQUEST[evento] != '') ){	
	switch($_REQUEST[evento]) {
		
		case 'I':	
			$obeid		= $_REQUEST['obeid']; 
			$obenome	= $_REQUEST['obenome']; 
			$mapid		= $_REQUEST['mapid'];
			$pesid 		= $_REQUEST['pesid']; 
			$temid 		= $_REQUEST['temid']; 
			
			if($obeid){						
				
				$sql = "
				SELECT 
					obeid				
				FROM  painel.objetivoestrategico 
				WHERE 
				obeid = ".$obeid."
				";					
				$dados = $db->pegaUm($sql);	
			}
			
			// caso não exista o indicador insere um novo
			if(!$dados){	
						
				$sql_I = "INSERT 
						  INTO painel.objetivoestrategico
							  (obenome, mapid, pesid, temid)
						  VALUES 
						  	   ('".$obenome."', ".$mapid.", ".$pesid.", ".$temid.")
						  ";	
			
				$db->executar($sql_I);
			//caso exista o indicador altera os dados do mesmo	
			}else{
				
				$sql_U = "
					UPDATE painel.objetivoestrategico
					SET
					 obenome	='".$obenome."',
					 mapid		= ".$mapid.",
					 pesid 		= ".$pesid.",					
					 temid 		= ".$temid."					
					WHERE 
					obeid = 		".$obeid."	
				";	
				$db->executar($sql_U);				
			}
			$db->commit();
			echo "<script>
					alert('Dados salvos com sucesso.');
					window.location = '?modulo=principal/cadobjestrategico&acao=A';
				  </script>";
		exit;
		break;
		
		case 'A': 
			$obeid = $_REQUEST['obeid']; 			
			$sql = "
				SELECT 
					obenome, mapid, pesid, temid
				FROM  painel.objetivoestrategico 
				WHERE 
				obeid = ".$obeid."
				";	
			$dados = $db->pegaLinha($sql);
			
			if($dados){
				$obenome  			= $dados['obenome'];  			 
				$mapid				= $dados['mapid'];
				$pesid 				= $dados['pesid'];  				
				$temid   			= $dados['temid']; 				
			}		
		break;		
		case 'E':
			$obeid = $_REQUEST['obeid']; 	
			$sql_D = "
					UPDATE painel.objetivoestrategico SET obestatus = 'I' where obeid = ".$obeid;	
			$db->executar($sql_D);
			$db->commit();
			echo "<script>
					alert('Registro removido com sucesso.');
					window.location = '?modulo=principal/cadobjestrategico&acao=A';
				  </script>";
		exit;
		break;	
		
	}
}

?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
<input type="hidden" name="evento" id="evento" value="I">
<input type="hidden" name="obeid" id="obeid" value="<?=$obeid?>">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<tr>
	<td align='right' class="SubTituloDireita">Nome Objetivo:</td>
    <td><?=campo_texto('obenome','S','S','',60,100,'','');?></td>
</tr>
<tr>
	<?php 
		$sql= "SELECT 
					mapdsc AS descricao, 
					mapid AS codigo
				FROM 
					painel.mapa
		";
	?>
    <td align='right' class="SubTituloDireita">Mapa Estratégico:</td>
    <td><?=$db->monta_combo('mapid',$sql,'S','Selecione...','','','','','S');?></td>
</tr>   

<tr>
	<?php 
		$sql= "SELECT 
				pesdsc AS descricao, 
				pesid AS codigo
				FROM 		
				painel.perspectiva		 
		";
	?>
    <td align='right' class="SubTituloDireita">Perspectiva:</td>
    <td><?=$db->monta_combo('pesid',$sql,'S','Selecione...','','','','','S');?></td>
</tr> 
<tr>
	<?php 
		$sql= "SELECT 
				temdsc AS descricao, 
				temid AS codigo
				FROM 		
				painel.tema		 
		";
	?>
    <td align='right' class="SubTituloDireita">Tema:</td>
    <td><?=$db->monta_combo('temid',$sql,'S','Selecione...','','','','','S');?></td>
</tr>   

<tr bgcolor="#cccccc">
	      <td></td>
	      <td>
	      	<input type="button" class="botao" name="btassociar" value="Gravar" onclick="submeter('I', '<?=$obeid?>');">
	  	  	<input type="button" class="botao" name="btassociar" value="Novo Objetivo Estratégico" onclick="novo();">	  	  
	  	  	<input type="button" class="botao" name="btassociar" value="Limpar" onclick="limpar();">	  	  	
	  	  </td>
	  	  
	      </tr>
</table>
</form>	
<?php 
	
	$sql = "SELECT 
			o.obeid,
			o.obenome,
			m.mapdsc,
			p.pesdsc,
			t.temdsc		
			FROM  painel.objetivoestrategico o				
			INNER JOIN  painel.mapa m ON m.mapid = o.mapid	
			INNER JOIN  painel.perspectiva p ON p.pesid = o.pesid	
			INNER JOIN  painel.tema t ON t.temid = o.temid		
			WHERE o.obestatus = 'A' 				
			";
	$dados = $db->carregar($sql);
	//dump($sql);	
	if( $dados ){		
		foreach($dados as $chave => $val){	
			$acao = "<center><a href=\"painel.php?modulo=principal/cadobjestrategico&acao=A&obeid=".$val['obeid']."&evento=A\">
								<img src=\"/imagens/alterar.gif \" border=0 title=\"Visualizar\"></a>
								<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 onclick=\"excluir('".$val['obeid']."');\" title=\"Excluir\">								
								";
			$dados_array[$chave] = array("acao" 		=> $acao, 
										 "objnome" 		=> $val['obenome'], 
										 "mapa" 		=> $val['mapdsc'],
										 "perspectiva"	=> $val['pesdsc'],
										 "tema" 		=> $val['temdsc']);
		}
		 $cabecalho = array("Ações", "Nome Objetivo", "Mapa", "Perspectiva", "Tema");
   		 $db->monta_lista_array($dados_array, $cabecalho, 50, 20, '', 'center', '');
	}else{
		 $db->monta_lista_array("", $cabecalho, 50, 20, '', 'center', '');
	}	
	?>
<script type="text/javascript">

function submeter(evento, obeid){	
	if(validar()){
		document.getElementById('evento').value = evento;
		document.getElementById('obeid').value = obeid;		
		document.formulario.submit();	
	}
}

function excluir(obeid){
	document.getElementById('evento').value = 'E';
	document.getElementById('obeid').value = obeid;		
	document.formulario.submit();
}

function novo(){
	document.getElementById('evento').value = '';
	document.getElementById('obeid').value = '';	
	document.formulario.submit();		
}

function limpar(){	
	document.getElementById('evento').value = '';	
	document.formulario.obenome.value = '';		
	document.formulario.mapid.value = '';
	document.formulario.pesid.value = '';
	document.formulario.temid.value = '';
}

function validar(){	
	var msg = "";	
	if(document.formulario.obenome.value == ''){
		msg+="O preenchimento do campo \"Nome Objetivo Estratégico\" é obrigatório.\n";
	}
	if(document.formulario.mapid.value == ''){
		msg+="O preenchimento do campo \"Mapa\" é obrigatório.\n";
	}
	if(document.formulario.pesid.value == ''){
		msg+="O preenchimento do campo \"Perspectiva\" é obrigatório.\n";
	}
	if(document.formulario.temid.value == ''){
		msg+="O preenchimento do campo \"Tema\" é obrigatório.\n";
	}
	
	if(msg != ""){
		alert(msg);
		return false;
	}else return true;	
}

</script>