<?php
$esdid = (int) $_GET[esdid];
$tpdid = (int) $_GET[tpdid];
$sisid = (int) $_GET[sisid];

# Verifica se exitem os parametros obrigatórios
if (!$esdid || !$tpdid || !$sisid)
	die("<script>window.close();</script>");

$_POST[aedid] ? $aedid = $_POST[aedid] : $aedid = $_GET[aedid];	
$esdsncomentario = $_POST['esdsncomentario'];
$esdiddestino = (integer) $_POST['esdiddestino'];
$aeddscrealizar = $_POST['aeddscrealizar'];
$aedstatus = $_POST['aedstatus'];
$aeddscrealizada = $_POST['aeddscrealizada'];
$aedcondicao = $_POST['aedcondicao'];
$aedposacao = $_POST['aedposacao'];
$aedobs = $_POST['aedobs'];
$aedvisivel = $_POST['aedvisivel'] == "1" ? "true" : "false" ;
$aedicone = $_POST['aedicone'];

$pflcod = (integer) $_POST['pflcod'];
#$evento = $_POST['evento'];

$perfis = $_POST['pflcod'];	
	

# Salvar / Editar
if ($_POST):

	$esdsncomentario == 'S' ? $esdsncomentario = 'TRUE' : $esdsncomentario = 'FALSE';
	
	if (!$_POST[aedid]):		
		$sql_iseriracao = "
			INSERT INTO 
				workflow.acaoestadodoc (
				esdidorigem, 
				esdiddestino, 
				aeddscrealizar, 
				aedstatus, 
				aeddscrealizada, 
				esdsncomentario, 
				aedcondicao, 
				aedobs,
				aedvisivel,
				aedicone,
				aedposacao
			)VALUES(
				".$esdid.", 
				".$esdiddestino.", 
				'".$aeddscrealizar."', 
				'A', 
				'".$aeddscrealizada."',
				".$esdsncomentario.",
				'".$aedcondicao."', 
				'".$aedobs."',
				$aedvisivel,
				".(($aedicone)?"'".$aedicone."'":"NULL").",
				'".$aedposacao."'
			)
			returning  
				workflow.acaoestadodoc.aedid
		";
		
		$aedidr = $db->pegaLinha($sql_iseriracao);
		
		$k = count($perfis[$sisid]);
		
		for( $i=0; $i < $k ; $i++){			
			$sql_iserirperfil = "
			INSERT INTO 
				workflow.estadodocumentoperfil (
				pflcod, 
				aedid
			)VALUES(
				" . $perfis[$sisid][$i] .", 
				".$aedidr['aedid']."
			)
			";
				
			$db->executar( $sql_iserirperfil );
		}
		
		$db->commit();
	else:
		$sql_alteraracao = "
						UPDATE 
							workflow.acaoestadodoc
                        SET 
                        	esdidorigem = ".$esdid.", 
                        	esdiddestino = ".$esdiddestino.", 
                        	aeddscrealizar = '".$aeddscrealizar."', 
                        	aeddscrealizada = '".$aeddscrealizada."', 
                        	esdsncomentario = ".$esdsncomentario.", 
                        	aedcondicao = '".$aedcondicao."', 
                        	aedposacao = '".$aedposacao."', 
                        	aedobs = '".$aedobs."',
                        	aedvisivel = $aedvisivel,
                        	aedicone = ".(($aedicone)?"'".$aedicone."'":"NULL")."
						WHERE  
							aedid = ".$aedid."	                        
						";
		
		$db->executar($sql_alteraracao);
		
		//---- alterando o perfil -----
		
		//excluindo os perfis antigos associados à ação
		$sql_excluirPerfil = "
			DELETE FROM workflow.estadodocumentoperfil
						WHERE aedid=".$aedid."
		";
		
		$db->executar($sql_excluirPerfil);
		
		//adicionando os novos perfis associados à ação	
		$k = count($perfis[$sisid]);	
		
		for( $i=0; $i < $k ; $i++){			
			$sql_iserirperfil = "
			INSERT INTO workflow.estadodocumentoperfil 
			(pflcod, aedid)
			VALUES (" . $perfis[$sisid][$i] .", ".$aedid.")
			";
				
			$db->executar( $sql_iserirperfil );
		}
		
		$db->commit();	
	endif;
	
	die ("<script>
			alert('Operação executada com sucesso!');
			window.opener.location.replace(window.opener.location);
			window.close();
		  </script>");
	
endif;	
	
# Carrega Ação	
if ($aedid):
	$sql = sprintf("SELECT
						acd.*, 
						pflcod as perfil, 
						pflcod
					FROM workflow.acaoestadodoc acd
					LEFT JOIN workflow.estadodocumentoperfil esd ON esd.aedid = acd.aedid
					WHERE acd.aedid = %d",$aedid);
	$dados = (array) $db->carregar($sql);

	foreach ($dados[0] as $k => $val)
		${$k} = $val;			
endif;
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form action="" method="post" name="formAcao">
<div id="sp_acoes">
<?php
monta_titulo( 'Ações', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="SubTituloDireita" align="right">Estado Origem:</td>
		<td><?
		
		$sql_esdselecionado = "								
								SELECT esddsc
								FROM workflow.estadodocumento
								WHERE esdid = ".$esdid."
							 ";
		$esdselecionado = $db->pegaUm($sql_esdselecionado);
		
		echo($esdselecionado);
		
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">Estado Fim:</td>
		<td><?

		$sql_estadodocumento = "
								SELECT esdid as codigo, esddsc as descricao
								FROM workflow.estadodocumento
								WHERE tpdid = ".$tpdid." AND esdstatus = 'A'
								";

		$db->monta_combo("esdiddestino", $sql_estadodocumento, "S", "Selecione", '', '', '', '380', 'S', "esdiddestino");
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">Comentário:</td>
		<td>
		<?		
			if($esdsncomentario == 't'){
				?>
				<input type="radio" name="esdsncomentario" value="S"
					id="rd_obrigatoriedade_s"  checked="checked"> <label for="rd_obrigatoriedade_s">Sim</label>
				<input type="radio" name="esdsncomentario" value="N"
					id="rd_obrigatoriedade_n" > <label for="rd_obrigatoriedade_n">Não</label>
				<?
			}else{				
				?>
				<input type="radio" name="esdsncomentario" value="S"
					id="rd_obrigatoriedade_s"> <label for="rd_obrigatoriedade_s">Sim</label>
				<input type="radio" name="esdsncomentario" value="N"
					id="rd_obrigatoriedade_n" checked="checked"> <label for="rd_obrigatoriedade_n">Não</label>				
				<?
			}			
		?>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">Visível:</td>
		<td>
		<?		
			if($aedvisivel == 't' || !$aedvisivel){
				?>
				<input type="radio" name="aedvisivel" value="1"
					id="aedvisivel_s"  checked="checked"> <label for="aedvisivel_s">Sim</label>
				<input type="radio" name="aedvisivel" value="0"
					id="aedvisivel_n" > <label for="aedvisivel_n">Não</label>
				<?
			}else{				
				?>
				<input type="radio" name="aedvisivel" value="1"
					id="aedvisivel_s" > <label for="aedvisivel_s">Sim</label>
				<input type="radio" checked="checked" name="aedvisivel" value="0"
					id="aedvisivel_n" > <label for="aedvisivel_n">Não</label>		
				<?
			}			
		?>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">Perfil:</td>
		<td>
		<?php 
		//setando as variaveis para a combo_popup()
		$sql = sprintf(
				"select p.pflnivel from seguranca.perfil p inner join seguranca.perfilusuario pu on pu.pflcod=p.pflcod and pu.usucpf='%s' and p.sisid=%d order by p.pflnivel ASC",
				$_SESSION['usucpf'],
				$sisid
			);
			$nivel = $db->pegaUm( $sql );
				
			//caso a ação esteja sendo editada são exibidos, no combo, os perfis associados a essa ação. 
			
			$sql_perfil = sprintf(
				"select distinct p.pflcod as codigo, p.pfldsc as descricao from seguranca.perfil p left join seguranca.perfilusuario pu on pu.pflcod=p.pflcod  where p.pflstatus='A' and /*p.pflnivel >= %d and*/ p.sisid=%d order by descricao",
				$nivel,
				$sisid
				);
			$nome = 'pflcod[' . $sisid . ']';
			
			if($aedid){
				
				$sql = "SELECT	seguranca.perfil.pflcod as codigo, seguranca.perfil.pfldsc as descricao 
						FROM seguranca.perfil
						INNER JOIN workflow.estadodocumentoperfil edp on edp.pflcod = seguranca.perfil.pflcod
						WHERE  edp.aedid = ".$aedid."
						";
				$$nome = $db->carregar( $sql ); 
				
			}else{
				$$nome = array();
			}

			combo_popup( $nome, $sql_perfil, 'Selecione o(s) Perfil(s)', '360x460' );	
			
		?>
		</td>
	</tr>
	<tr>
	<td class="SubTituloDireita" align="right"> Realizar:
	</td>
	<td><?= campo_texto( 'aeddscrealizar', 'S', 'S', '', 75, 75, '', '' ); ?>
	</td>
	</tr>
	<tr>
	<td class="SubTituloDireita" align="right"> Realizado:
	</td>
	<td><?= campo_texto( 'aeddscrealizada', 'S', 'S', '', 75, 75, '', '' ); ?>
	</td>
	</tr>
	<tr>
	
	<?
	if($db->testa_superuser()){
		?>
		<td class="SubTituloDireita" align="right"> Condição:
		</td>
		<td><?= campo_texto( 'aedcondicao', 'N', 'S', '', 75, 75, '', '' ); ?>
		</td>
		</tr>
		<?
	}
	 ?>
	
	<tr>
	<td class="SubTituloDireita" align="right"> Alerta:
	</td>
	<td><?= campo_texto( 'aedobs', 'N', 'S', '', 75, 75, '', '' ); ?>	
	</td>
	</tr>
	<tr>
	<td class="SubTituloDireita" align="right"> Pos ação:
	</td>
	<td><?= campo_texto( 'aedposacao', 'N', 'S', '', 75, 75, '', '' ); ?>	
	</td>
	</tr>
	<tr>
	<td class="SubTituloCentro" colspan="2">Ícones</td>
	</tr>
	<tr>
	<td colspan="2">
	<div style=height:100px;overflow:auto;>
	<table width=100%>
	<?
	echo "<tr><td style=border-width:1px;border-style:solid;border-color:#000000;><input type=radio name=aedicone value='' ".((!$aedicone)?"checked":"")."></td><td style=border-width:1px;border-style:solid;border-color:#000000;>Nenhum ícone associado</td></tr>";
	if($handle = opendir(APPRAIZ.'/www/imagens/workflow/')) {
	    while (false !== ($file = readdir($handle))) {
	        if ($file != "." && $file != "..") {
	        	echo "<tr><td style=border-width:1px;border-style:solid;border-color:#000000;><input type=radio name=aedicone value=".$file." ".(($file==$aedicone)?"checked":"")."></td><td style=border-width:1px;border-style:solid;border-color:#000000;><img src=../imagens/workflow/".$file."> ".$file."</td></tr>";
	        }
	    }
	    closedir($handle);
	} else {
		echo "<tr><td>Não existem ícones na pasta</td></tr>";
	}
	?>
	</table>	
	</div>
	</td>
	</tr>

	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<?
			if($aedid){
				echo("<td><input type='button' class='botao' value='Alterar' id='btalterarestado' name='btalterarestado' onclick=\"validarAcao( '". $nome . "')\" />
	                 <!-- <input type='button' class='botao' value='Cancelar' id='btcancelar' name='btcancelar' onclick='cancelar()' />
	                  <input type='button' class='botao' value='Novo' id='btcancelar' name='btcancelar' onclick='cancelar()' /> -->
	                  ");
			}else{
				echo("<td><input type='button' class='botao' name='consultar' value='Adicionar Ação' onclick=\"validarAcao('" . $nome ."')\" />");
			}
		?>
			<input type='button' class='botao' value='Fechar' id='btFechar' name='btFechar' onclick='window.opener.location.replace(window.opener.location); window.close();' />
			<input type='hidden' name='aedid' value='<?php echo $aedid ?>' />
		</td>			
	</tr>
</table>
</form>
<script>
function validarAcao (nomeCombo) {
	d = document.formAcao;
	
	if (d.esdiddestino.value == '') {
		d.esdiddestino.focus();
		alert('O campo Estado Fim é obrigatório!');
		return false;
	}
	
	selectAllOptions( document.getElementById(nomeCombo) );
	
	if (document.getElementById(nomeCombo).value == '') {
		alert('O campo Perfil é obrigatório!');
		return false;
	}
	
	if (d.aeddscrealizar.value.length < 3) {
		d.aeddscrealizar.focus();
		alert('O campo Realizar é obrigatório!');
		return false;
	}
	
	if (d.aeddscrealizada.value.length < 3) {
		d.aeddscrealizada.focus();
		alert('O Campo Realizado é obrigatório!');
		return false;
	}
	
	d.submit();
}
</script>
