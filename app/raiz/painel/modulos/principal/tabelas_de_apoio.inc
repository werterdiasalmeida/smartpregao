<?php
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';


// Agrupando as variáveis
$tabela = $_REQUEST["tabela"];
$nome_tabela=$_REQUEST["nome_tabela"];

$titulo = "Tabelas de Apoio";
monta_titulo( $titulo,"Tabela de apoio: ".$nome_tabela."" );

// Descricao
$nome_descricao = $_REQUEST["nome_descricao"];
$descricao		= $_POST[$nome_descricao];

// Status
$nome_status = $_REQUEST["nome_status"];

// ID
$nome_id	= $_REQUEST["nome_id"];
$id_editar	= $_REQUEST["id_editar"];
$id_apagar	= $_REQUEST["id_apagar"];
//dbg($id_editar);
$novo_registro=$_REQUEST["novo_registro"];

//dbg($novo_registro);
//dbg($descricao);
//dbg($id_editar);
//dbg($nome_id);
?>
<script type="text/javascript">
<!--
	function novo(){
		window.open("painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=<?echo $tabela;?>&nome_descricao=<?echo $nome_descricao;?>&nome_status=<?echo $nome_status;?>&nome_id=<?echo $nome_id;?>&novo_registro=true",'_top');
	}
	function gravar(){
		desc=document.getElementById("<?echo $nome_descricao;?>");
		if (desc.value != ""){
			document.formTabelasApoio.submit();
		}else{
			alert("Preencha a Descrição");
		}
	}
	function excluir(id_apagar){
		window.open("painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=<?echo $tabela;?>&nome_descricao=<?echo $nome_descricao;?>&nome_status=<?echo $nome_status;?>&nome_id=<?echo $nome_id;?>&novo_registro=false&id_apagar="+id_apagar,'_top');
	}
	function editar(id_editar){
		<?php $novo_resgistro=false;?>
		window.open("painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=<?echo $tabela;?>&nome_descricao=<?echo $nome_descricao;?>&nome_status=<?echo $nome_status;?>&nome_id=<?echo $nome_id;?>&novo_registro=false&id_editar="+id_editar,'_top');
	}
	
	function addDP(perid){
		janela("painel.php?modulo=principal/popupTabelaApoioDetalhe&acao=A&perid=" + perid,600,200);
	}
	
//-->
</script>
<br>
<?php

// Manutenção dos registros

if(isset($novo_registro) and $descricao!=""){
	
//		$sql="SELECT * FROM painel.".$tabela." WHERE ".$nome_descricao."='".$descricao."'"; 
//		$resultado = $db->carregar($sql);
//
//		if ($resultado){
//			$novo=false;
//		} else {
//			$novo=true;
//		}
//dbg($novo_registro);	
	if ($novo_registro=="true"){
		$sql="INSERT INTO painel.".$tabela." (".$nome_descricao.",".$nome_status.") VALUES ('".$descricao."','A')"; 
		//	dbg($sql);
		$resultado = $db->executar($sql);
		$db->commit();
		echo "<script>
				alert('Dados salvos com sucesso.');
			  </script>";
	} else {
		$sql="UPDATE painel.".$tabela." SET ".$nome_descricao."='".$descricao."' WHERE ".$nome_id."=".$id_editar; 
		//dbg($sql);
		$db->executar($sql);
		$db->commit();
		echo "<script>
				alert('Dados salvos com sucesso.');				
		</script>";
		
	}	

}

//Apagando registro
if(isset($id_apagar) and $id_apagar!=""){
	  $sql = "DELETE FROM painel.".$tabela." WHERE ".$nome_id." = ".$id_apagar."";            
      $db->executar( $sql );
      $db->commit();
      echo "<script>
				alert('Dados salvos com sucesso.');				
		</script>";
}

// Carregar os dados na tela
if ($id_editar!="" and $id_editar!=undefined){
	$sql="SELECT * FROM painel.".$tabela." WHERE ".$nome_id."=".$id_editar; 
	$resultado = $db->carregar($sql);
}

?>
<form method="post" name="formTabelasApoio" id="formTabelasApoio" action="painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=<?echo $tabela;?>&nome_descricao=<?echo $nome_descricao;?>&nome_status=<?echo $nome_status;?>&nome_id=<?echo $nome_id."&novo_registro=".$novo_registro."&id_editar=".$id_editar;?>">
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1"	align="center">

<tr>
	<td align='right' class="SubTituloDireita">Descrição:</td>
    <td>
	<input type="text" title="" style="text-align: left; width: 53ex;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" class="normal" 
	value="<?if (isset($resultado)){echo $resultado[0][$nome_descricao];}?>" maxlength="" size="51" 
	name="<?echo $nome_descricao;?>" id="<?echo $nome_descricao;?>"/> <img src=/imagens/obrig.gif>
	<input type="button" class="botao" value="Gravar" onclick="javascript: gravar();">
	<input type="button" class="botao" value="Novo" onclick="javascript: novo();">
	<input type="button" class="botao" name="btassociar" value="Voltar" onclick="javascript: window.open('painel.php?modulo=principal/lista_tabelas_de_apoio&acao=A','_top');">
	</td>
	
</tr>
<tr>
	<td> 
	<?
	//	if(isset($_REQUEST["id_apagar"])){
	//		$valor=$_REQUEST["id_apagar"];
	//		$sql="DELETE FROM painel.".$tabela." WHERE ".$nome_id."=".$valor; 
	//		$resultado = $db->executar($sql);
	//		echo "<script>
	//				alert('Dados apagados com sucesso.');
	//			  </script>";
	//		$db->commit();
	//	}
		
	?>
	</td>
</tr>
<tr>
	<td colspan=3> 
	
	<?php 
if (isset($tabela)){
	$sql="SELECT * FROM painel.".$tabela;
	$registros = $db->carregar($sql);
	echo '
		<table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center">
		<tr bgcolor=#c0c0c0 >
			<td width="20%">
			<b>Ação</b>
			</td>
			<td width="60%">
			<b>Descrição</b> 
			</td>
			<td width="20%">
			<b>Status</b>
			</td>
			
		</tr>
	';
	
	if($tabela = "periodicidade"){
		$sql = "select distinct dpeanoref
					from
						painel.detalheperiodicidade";
			$ano = $db->carregar($sql);
			!$ano? $ano = array() : $ano = $ano;
	}
	
	for($i=0;$i < count($registros);$i++){		
		if($i%2 != 0){
			$cor = "bgcolor='#F5F5F5' onmouseout=\"this.bgColor='#F5F5F5';\" onmouseover=\"this.bgColor='#ffffcc';\"";	
		}else{
			$cor = "bgcolor='#DCDCDC' onmouseout=\"this.bgColor='#DCDCDC';\" onmouseover=\"this.bgColor='#ffffcc';\"";
		}
		
		if($tabela == "periodicidade"){
			
			$sql = "select *
					from
						painel.detalheperiodicidade
					where
						perid = ".$registros[$i][$nome_id]."";
			$detalhes = $db->carregar($sql);
		}
		if($detalhes){
			$imgAdd = "<img src=\"../imagens/gif_inclui.gif\" title=\"Adicionar Detalhe\" style=\"cursor:pointer\" onclick=\"addDP({$registros[$i][$nome_id]})\" /> ";
		}
		
		echo '
			<tr '.$cor.'>
				<td>
				'.$imgAdd.' 
				<img border="0" title="Editar" onclick="editar('.$registros[$i][$nome_id].');" src="/imagens/alterar.gif" style="cursor: pointer;"/>
				<img border="0" title="Excluir" onclick="excluir('.$registros[$i][$nome_id].');" src="/imagens/excluir.gif" style="cursor: pointer;"/>
				</td>
				<td>
						'.$registros[$i][$nome_descricao].'
				</td>
				<td>
						'.$registros[$i][$nome_status].'
				</td>
			</tr>	
		 ';
		if($detalhes){
			$x = 0;
			foreach($detalhes as $det):
				if($x%2 == 0){
					$cor2 = "bgcolor='#f9f9f9' onmouseout=\"this.bgColor='#f9f9f9';\" onmouseover=\"this.bgColor='#ffffcc';\" ";	
				}else{
					$cor2 = "bgcolor='#FFFFFF' onmouseout=\"this.bgColor='#FFFFFF';\" onmouseover=\"this.bgColor='#ffffcc';\" ";
				}
				echo "<tr $cor2>
						<td>
							<img style=\"margin-left:30px;\" src=\"../imagens/seta_filho.gif\" title=\"Detalhe Periodicidade\" />
							<img border=\"0\" title=\"Editar\" onclick=\"editarDH({$det['ddiid']});\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer;\"/>
							<img border=\"0\" title=\"Excluir\" onclick=\"excluirDH({$det['ddiid']});\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer;\"/>
						</td>
						<td>
							<span style=\"margin-left:30px;\">{$det['dpedsc']}";
				
				foreach($ano as $a){
					echo " / ".$a['dpeanoref'];	
				}
							
				echo "
						</td>
						<td>
							<span style=\"margin-left:30px;\">{$det['dpestatus']}</span>
						</td>
					</tr>";
					$x++;
			endforeach;
		}
	}
}
	?>	

	</table>
<!--<tr >
	<td><a onclick="novo(); return false;" href="#"><img border="0" title="Inserir Contatos" style="cursor: pointer;" src="/imagens/gif_inclui.gif"/> Inserir </a></td><td><input type="button" class="botao" name="btassociar" value="Voltar" onclick="javascript: window.open('painel.php?modulo=principal/lista_tabelas_de_apoio&acao=A','_top');"></td>
</tr>

	--></td>
</tr>
</table>
</form>