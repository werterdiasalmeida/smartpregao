<?php

//dump($_REQUEST);
//dump($_SESSION);

include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
monta_titulo( 'Estado do Documento', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );

$sisid = '';
$tpdid = '';
$esdid = '';
$aedid = '';

//variaveis q alimentam o formulário
if ($_POST[sisid] || $_GET[sisid]):
	$_POST[sisid] ? $sisid = (int) $_POST[sisid] : $sisid = (int) $_GET[sisid];

	if (($_POST[tpdid] || $_GET[tpdid] || $_POST[NEWtpdid]) && $_POST[NEWtpdid] != 'del'):
	
		if ($_POST[NEWtpdid])
			$tpdid = (int) $_POST[NEWtpdid];
		elseif ($_POST[tpdid])
			$tpdid = (int) $_POST[tpdid];			 
		else
			$tpdid = (int) $_GET[tpdid];
			
		 if (($_POST[esdid] || $_GET[esdid] || $_POST[NEWesdid]) && $_POST[NEWesdid] != 'del'):

		 	if ($_POST[NEWesdid])	
				$esdid = $_POST[NEWesdid];
			elseif ($_POST[esdid])
				$esdid = $_POST[esdid];
			else
				$esdid = $_GET[esdid];
							 
		 endif;
		 
	endif;		
endif;
/*
if(isset($_REQUEST['sisid'])){	
	$sisid = (integer) $_REQUEST['sisid'];	
}else{
	$sisid = (integer) $_SESSION['sisid'];
}

if ($_REQUEST[NEWtpdid] && $_REQUEST[NEWtpdid] != 'del')
	$tpdid = (integer) $_REQUEST[NEWtpdid];
elseif ($_REQUEST[NEWtpdid] == 'del')
	$tpdid = '';
elseif(isset($_REQUEST['tpdid']))
	$tpdid = (integer) $_REQUEST['tpdid'];	
else
	$tpdid = (integer) $_SESSION['tpdid'];

	
if ($_REQUEST[NEWesdid] && $_REQUEST[NEWesdid]!= 'del')
	$esdid = (integer) $_REQUEST[NEWesdid];
elseif($_REQUEST[NEWesdid]== 'del')	
	$esdid = '';
elseif(isset($_REQUEST['esdid']))
	$esdid = (integer) $_REQUEST['esdid'];	
else
	$esdid = (integer) $_SESSION['esdid'];
*/

$aedid = (integer) $_POST['aedid'];
$esdsncomentario = $_POST['esdsncomentario'];
$esdiddestino = (integer) $_POST['esdiddestino'];
$aeddscrealizar = $_POST['aeddscrealizar'];
$aedstatus = $_POST['aedstatus'];
$aeddscrealizada = $_POST['aeddscrealizada'];
$aedcondicao = $_POST['aedcondicao'];
$aedposacao = $_POST['aedposacao'];
$aedobs = $_POST['aedobs'];

$pflcod = (integer) $_POST['pflcod'];
$evento = $_POST['evento'];

$perfis = $_POST['pflcod'];


/*	
	if($tpdid == ""){
		$_SESSION['tpdid'] = "";
		$_REQUEST['sisid'] = "";
	}
	if($esdid == ""){
		$_SESSION['esdid'] = "";
		$_REQUEST['sisid'] = "";
	}
*/
if($evento != ""){	
	
	switch ( $evento ) {
		
		case 'adicionar_acao':
			ca_adicionarAcao();
		/*	
			$_SESSION['sisid'] = $_REQUEST['sisid'];
			$_SESSION['esdid'] = $_REQUEST['esdid'];
			$_SESSION['tpdid'] = $_REQUEST['tpdid'];
		*/
			//alterando o header para evitar reprocessameto do enviar() - que insere novas acoes 				
			die($db->sucesso("sistema/workflow/cadastrar_acao", '&sisid='.$_POST[sisid].'&tpdid='.$_POST[tpdid].'&esdid='.$_POST[esdid]));

						
		break;
		
		case 'alterar_acao':						
			//rotina que altera uma nova acao				
			ca_alterarAcao();	

			$esdsncomentario = "";
			$esdiddestino = "";
			$aeddscrealizar = "";
			$aedstatus = "";
			$aeddscrealizada = "";
			$aedcondicao = "";
			$aedobs = "";
			$aedposacao = "";
			$evento = ""; 

		break;
		
		case 'remover_acao':
			//rotina que exclui uma acao		
			ca_removerAcao($aedid);
			die($db->sucesso("sistema/workflow/cadastrar_acao", '&sisid='.$_POST[sisid].'&tpdid='.$_POST[tpdid].'&esdid='.$_POST[esdid]));
		break;
		
	default:
		break;
	}
}
?>

<form method="post" name="formulario" action="seguranca.php?modulo=sistema/workflow/cadastrar_acao&acao=A">

<input type='hidden' name="evento" id="evento" value="<?=$evento?>">
<input type='hidden' name="aedid" id="aedid" value="<?=$aedid?>">


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="SubTituloDireita" align="right">Sistema:</td>
		<td><?
		$sql_sistema = "
							SELECT sisid as codigo, sisdsc as descricao
							FROM seguranca.sistema
							WHERE sisstatus = 'A'
						";

		$db->monta_combo("sisid", $sql_sistema, "S", "Selecione", "carregarSis", '', '', '', '', "sisid");
		?>
		</td>
	</tr>
	<tr>
	<?
	if($sisid)
	{
		echo("<td class='SubTituloDireita' align='right'>Tipo Documento:</td>
			  <td valign='middle'>");

		$sql_tipodocumento = "
								SELECT 
									tpdid as codigo, tpddsc as descricao
								FROM 
									workflow.tipodocumento
								WHERE
									tpdstatus = 'A' AND 
									sisid = ".$sisid." 
							  ";

		$db->monta_combo("tpdid", $sql_tipodocumento, "S", "Selecione", "carregarTpd", '', '', '', '', "tpdid");

		echo(" <a style='display:inline; margin-left:4px; cursor:pointer;' onclick=\"w = window.open('seguranca.php?modulo=sistema/workflow/popupTipDoc&acao=I&sisid=".$sisid."&tpdid='+document.formulario.tpdid.value,'Janela1','scrollbars=yes,location=no,toolbar=no,menubar=no,width=430,height=230'); w.focus();\" >Editar</a><input name=\"NEWtpdid\" type=\"hidden\"></td>");
	}
	?>
	</tr>
	<tr>

	<?
	#print_r($_POST);
	#echo "<BR>sisid = > $sisid <br> tpdid => $tpdid<br> esdid => $esdid";	
	if($tpdid && $sisid)
	{	

		echo("<td class='SubTituloDireita' align='right'>Estado Documento:</td>
			  <td>");
		$sql_estadodocumento = "
								SELECT esdid as codigo, esddsc as descricao
								FROM workflow.estadodocumento
								WHERE tpdid = ".$tpdid." AND esdstatus = 'A'
								ORDER BY esdordem ASC";

		$db->monta_combo("esdid", $sql_estadodocumento, "S", "Selecione", "carregarEsd", '', '', '', '', "esdid");

		echo(" <a style='display:inline; margin-left:4px; cursor:pointer;' onclick=\"w = window.open('seguranca.php?modulo=sistema/workflow/popupEstDoc&acao=I&tpdid=".$tpdid."&esdid='+document.formulario.esdid.value,'Janela2','scrollbars=yes,location=no,toolbar=no,menubar=no,width=430,height=230'); w.focus();\" >Editar</a><input name=\"NEWesdid\" type=\"hidden\"></td>");
	}

	?>
	</tr>
	<tr>
		<td bgcolor="#c0c0c0" align="center" colspan="2">&nbsp;</td>
	</tr>
</table>

<?
//monta a parte de configuraçãoes das ações somente se o sistema e o tipo de documento tiverem sidos escolhidos
if(($tpdid !="" )  && ($sisid !="" ) && ($esdid !="" )){
?>

<div id="sp_acoes">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="TituloTela" bgcolor="dcdcdc" align="center" colspan="2"><b>Ações</b>
		</td>
	</tr>	
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

		$db->monta_combo("esdiddestino", $sql_estadodocumento, "S", "Selecione", "carregarEsdDestino", '', '', '', 'S', "esdiddestino");
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">Comentário:</td>
		<td>
		<?		
			if($esdsncomentario == 'S'){
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
	<!--  
			
			<tr>
				<td align='right' class="SubTituloDireita">Ativo:</td>
				<?				
					if($aedstatus == "I"){
						echo("<td><input type='checkbox' id='aedstatus' name='aedstatus'> </td>");
					}else{	
						echo("<td><input type='checkbox' id='aedstatus' name='aedstatus' checked='true' value='A'> </td>");
					}
				?>
				
			</tr>
	-->
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
				"select distinct p.pflcod as codigo, p.pfldsc as descricao from seguranca.perfil p left join seguranca.perfilusuario pu on pu.pflcod=p.pflcod  where p.pflstatus='A' and p.pflnivel >= %d and p.sisid=%d order by descricao",
				$nivel,
				$sisid
				);
			
			$nome = 'pflcod[' . $sisid . ']';
			
			if($evento == 'pre_alterar_acao'){
				
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
	<tr bgcolor="#C0C0C0">
				<td>&nbsp;</td>
				<?
					if($evento == 'pre_alterar_acao'){
						echo("<td><input type='button' class='botao' value='Alterar' id='btalterarestado' name='btalterarestado' onclick=\"alterarAcao( '". $nome . "')\" />
			                  <input type='button' class='botao' value='Cancelar' id='btcancelar' name='btcancelar' onclick='cancelar()' />
			                  <input type='button' class='botao' value='Novo' id='btcancelar' name='btcancelar' onclick='cancelar()' />
			                  </td>");
					}else{
						echo("<td><input type='button' class='botao' name='consultar' value='Adicionar Ação' onclick=\"adicionarAcao('" . $nome ."')\"</td>");
					}
				?>				
	</tr>
	
</table>
</form>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" 	align="center">	
	<tr>
		<td class="SubTituloDireita">Ações Cadastradas:<br />
		<b><label id="lb_estado2"></label></b></td>
		<td><?
		//monta a tabela com os tipos de documentos cadastrados
		if ($esdid) $wh = " acaoestadodoc.esdidorigem=".$esdid." AND acaoestadodoc.aedstatus = 'A'";
		if ($wh) $wh = " where ".$wh;
		$sql = "
						SELECT
						'<img
								border=\"0\"
								src=\"../imagens/alterar.gif\"
								title=\"Alterar Documento\"
								onclick=\"preAlterarAcao( '
									|| coalesce( acaoestadodoc.aedid, 0 ) || ', '
									|| coalesce( acaoestadodoc.esdidorigem, 0 ) || ', '
									|| coalesce( acaoestadodoc.esdiddestino, 0 ) || ', \''
									|| coalesce( acaoestadodoc.aeddscrealizar, '' ) || '\', \''
									|| coalesce( acaoestadodoc.aeddscrealizada, '' ) || '\', \''
									|| ( case when acaoestadodoc.esdsncomentario then '1' else '0' end )|| '\', \''
									|| coalesce( acaoestadodoc.aedcondicao, '' )|| '\', \''
									|| coalesce( acaoestadodoc.aedposacao, '' )|| '\', \''
									|| coalesce( acaoestadodoc.aedobs, '' )|| '\')\">
							&nbsp;&nbsp;&nbsp;
							<img
								border=\"0\"
								src=\"../imagens/excluir.gif\"
								title=\"Excluir Documento\"
								onclick=\"if (confirm(\'Deseja apagar a ação?\'))excluirAcao(' || acaoestadodoc.aedid || ')\">' as acao,
							acaoestadodoc.aeddscrealizar,
							acaoestadodoc.aeddscrealizada,
							ori.esddsc,
							coalesce( des.esddsc, '' )
						FROM 
						workflow.acaoestadodoc
							inner join workflow.estadodocumento ori on ori.esdid = acaoestadodoc.esdidorigem
							inner join workflow.estadodocumento des on des.esdid = acaoestadodoc.esdiddestino
						
						". $wh. "
							GROUP BY acaoestadodoc.aedid, acaoestadodoc.esdidorigem, acaoestadodoc.esdiddestino, 
							acaoestadodoc.aeddscrealizar, acaoestadodoc.aedstatus, acaoestadodoc.aeddscrealizada, 
							acaoestadodoc.esdsncomentario,  acaoestadodoc.aedcondicao,acaoestadodoc.aedposacao, acaoestadodoc.aedobs, 
							acaoestadodoc.aeddscrealizar, acaoestadodoc.aeddscrealizada, ori.esddsc, des.esddsc 
						";

		$cabecalho = array( 'Ações', 'DSC Realizar','DSC Realizada', 'Origem', 'Destino');
		$db->monta_lista( $sql, $cabecalho, 200, 20, '', '', '' );
		?></td>
	</tr>
	<?
	
	/*	if($sisid == ""){
			$_SESSION['sisid'] = "";
			$_REQUEST['sisid'] = "";
		}
	if($tpdid == ""){
		$_SESSION['tpdid'] = "";
		$_REQUEST['sisid'] = "";
	}
	if($esdid == ""){
		$_SESSION['esdid'] = "";
		$_REQUEST['sisid'] = "";
	}
	*/
	
	//dump($_SESSION['sisid']).die;
	
	/*$_SESSION['sisid'] = $sisid;
	$_SESSION['tpdid'] = $tpdid;
	$_SESSION['esdid'] = $esdid;*/
	
	?>
	
</table>

		<? } ?>


<script><!--

	d = document; 

	function carregarSis(){
	    d = document;
	    
	    //setando os options para um valor nulo (o primeiro), sabendo q a primeira opcao é "selecione"
	    var comboTpd = d.getElementById('tpdid');
	    var comboEsd = d.getElementById('esdid');
		/*
		if(comboTpd){
			comboTpd.options[0].selected = true;
			comboTpd.value = "";
		}
    	
    	if(comboEsd){
    		comboEsd.options[0].selected = true;
    		comboEsd.value = "";
    	}
		*/
		for (i=0; i < d.formulario.length; i++){

			if (d.formulario.elements[i].id != 'sisid')
				d.formulario.elements[i].value = '';
				
		}
									
		d.formulario.submit();
	}
	
	function carregarTpd(){		
		
		/*
		var comboEsd = document.getElementById('esdid');
		if(comboEsd){
			comboEsd.options[0].selected = true;
			comboEsd.value = "";
		}	
		*/
		for (i=0; i < d.formulario.length; i++){
			
			if (d.formulario.elements[i].id != 'sisid' && d.formulario.elements[i].id != 'tpdid')
				d.formulario.elements[i].value = '';
				
		}		
		
		document.formulario.submit();
	}
	
	function carregarEsd(){

		for (i=0; i < d.formulario.length; i++){
			
			if (d.formulario.elements[i].id != 'sisid' && d.formulario.elements[i].id != 'tpdid' && d.formulario.elements[i].id != 'esdid')
				d.formulario.elements[i].value = '';
				
		}	
	
		document.formulario.submit();
	}
	
	function carregarEsdDestino() {		
	}
	
	function adicionarAcao(nomeCombo){
	
		if(document.getElementById("esdiddestino").value == ''){
			alert("A escolha de um estado de destino é obrgitário!!!");
			return;
		}
		
		selectAllOptions( document.getElementById(nomeCombo) );
		
		if(document.getElementById(nomeCombo).value == ''){
		 	alert("O Campo Perfil é obrgitário!!!");
			return;	
		}
		if(document.formulario.aeddscrealizar.value == ''){
		 	alert("O Campo Realizar é obrgitário!!!");
			return;	
		}
		if(document.formulario.aeddscrealizada.value == ''){
			alert("O campo A Realizar é obrgitário!!!");
			return;
		}		
				
		document.getElementById("evento").value = "adicionar_acao";		
		
				
		document.formulario.submit();
	}
	
	function preAlterarAcao(aedid, esdid, esdiddestino, aeddscrealizar, aeddscrealizada, esdsncomentario, aedcondicao, aedposacao, aedobs){
		
		document.getElementById("aedid").value= aedid;
		document.getElementById("esdid").value= esdid;		
		document.formulario.esdiddestino.value = esdiddestino;				
		document.formulario.aeddscrealizar.value = aeddscrealizar;	
		//if(document.formulario.aedstatus.value == 'A'){
		//	document.formulario.aedstatus.checked = 'true';	
		//}		
		document.formulario.aeddscrealizada.value = aeddscrealizada;	
		document.formulario.esdsncomentario.value = esdsncomentario;		
		if(document.formulario.esdsncomentario.value == '1' ){
			document.getElementById("rd_obrigatoriedade_s").checked = 'true';	
		}			
		document.formulario.aedcondicao.value = aedcondicao;
		document.formulario.aedposacao.value = aedposacao;	
		document.formulario.aedobs.value = aedobs;								
		document.getElementById("evento").value= "pre_alterar_acao";
		
		document.formulario.submit();
	}
	
	function cancelar(){
		
		document.getElementById("aedid").value= aedid;
		document.getElementById("esdid").value= esdid;		
		document.formulario.esdiddestino.value = '';				
		document.formulario.aeddscrealizar.value = '';			
		//document.formulario.aedstatus.value = '';	
		document.formulario.aeddscrealizada.value = '';	
		document.formulario.esdsncomentario.value = '';				
		document.formulario.aedcondicao.value = '';
		document.formulario.aedposacao.value = '';
		document.formulario.aedobs.value = '';					
		document.getElementById("evento").value= '';		
		document.formulario.submit();
	}
	
	function alterarAcao(nomeCombo){
	
		if(document.getElementById("esdiddestino").value == ''){
			alert("A escolha de um estado de destino é obrgitário!!!");
			return;
		}
		if(document.formulario.aeddscrealizar.value == ''){
		 	alert("O Campo Realizar é obrgitário!!!");
			return;	
		}
		if(document.formulario.aeddscrealizada.value == ''){
			alert("O campo Realizar é obrgitário!!!");
			return;
		}	
	
		document.getElementById("evento").value= "alterar_acao";		
			
		selectAllOptions( document.getElementById(nomeCombo) );
				
		document.formulario.submit();		
	}
	
	function excluirAcao(aedid){	
	
		document.formulario.esdiddestino.value = '';				
		document.formulario.aeddscrealizar.value = '';			
		//document.formulario.aedstatus.value = '';	
		document.formulario.aeddscrealizada.value = '';	
		document.formulario.esdsncomentario.value = '';				
		document.formulario.aedcondicao.value = '';
		document.formulario.aedposacao.value = '';
		document.formulario.aedobs.value = '';
		document.getElementById("aedid").value = aedid;
		document.getElementById("evento").value= "remover_acao";
		document.formulario.submit();		
	}
	
--></script>

<?
	function ca_adicionarAcao(){
		
		global  $db,
				$esdid, 
				$esdiddestino, 
				$aeddscrealizar, 
				//$aedstatus,
				$aeddscrealizada, 
				$esdsncomentario,
				$aedcondicao,
				$aedposacao,							 
				$aedobs, 
				$sisid,				
				$perfis;
				
		//if(empty($aedstatus)) $aedstatus = "I"; else $aedstatus = "A"; 
				
		$esdsncomentario == 'S' ? $esdsncomentario = 'TRUE' : $esdsncomentario = 'FALSE';
				
		$sql_iseriracao = "
			INSERT INTO workflow.acaoestadodoc 
			(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada, esdsncomentario, aedcondicao, aedobs)
			VALUES (".$esdid.", ".$esdiddestino.", '".$aeddscrealizar."', 'A', '".$aeddscrealizada."',".$esdsncomentario.",'".$aedcondicao."', '".$aedobs."')
			returning  workflow.acaoestadodoc.aedid
		";
		
		$aedidr = $db->pegaLinha($sql_iseriracao);
		
		$k = count($perfis[$sisid]);
		
		for( $i=0; $i < $k ; $i++){			
			$sql_iserirperfil = "
			INSERT INTO 
				workflow.estadodocumentoperfil 
				(pflcod, aedid)
			VALUES 
				(" . $perfis[$sisid][$i] .", ".$aedidr['aedid'].")
			";
				
			$db->executar( $sql_iserirperfil );
		}
		
		$db->commit();
	}
	
	function ca_alterarAcao(){
		
		global  $db,
				$esdid, 
				$esdiddestino, 
				$aeddscrealizar, 
				//$aedstatus,
				$aeddscrealizada, 
				$esdsncomentario,
				$aedcondicao,
				$aedposacao, 
				$aedobs,
				$aedid,
				$perfis,
				$sisid; 

		//------ alterando a ação ------		
		
		//if(empty($aedstatus)) $aedstatus = "I"; else $aedstatus = "A"; 
				
		$esdsncomentario == 'S' ? $esdsncomentario = 'TRUE' : $esdsncomentario = 'FALSE';
		
		$sql_alteraracao = "
							UPDATE workflow.acaoestadodoc
	                        SET esdidorigem = ".$esdid.", esdiddestino = ".$esdiddestino.", aeddscrealizar = '".$aeddscrealizar."',
	                        aeddscrealizada = '".$aeddscrealizada."', esdsncomentario = ".$esdsncomentario.",
	                        aedcondicao = '".$aedcondicao."',aedposacao = '".$aedposacao."', aedobs = '".$aedobs."'
							WHERE  aedid = ".$aedid."	                        
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
	}
	
	function ca_removerAcao($aedid){
		global $db;	
			
		$sql_confere = sprintf("SELECT
									COUNT(aedid) as total	
								FROM
									workflow.historicodocumento
								WHERE
									aedid = %d", $aedid);
		$determina = $db->pegaUm($sql_confere);
		
		//excluindo a ação				
		if ($determina):		
			$sql_excluir = "UPDATE 
								workflow.acaoestadodoc
	                        SET 
	                        	aedstatus = 'I'
							WHERE  
								aedid = ".$aedid;
		else:
			$sql_excluir = "DELETE								
							FROM
								workflow.estadodocumentoperfil
							WHERE
								aedid = ".$aedid;
			$db->executar ($sql_excluir);
			
			unset($sql_excluir,$determina,$sql_confere);
			$sql_excluir = "DELETE
							FROM
								workflow.acaoestadodoc
							WHERE
								aedid = ".$aedid;	
		endif;
		
		$db->executar($sql_excluir);
		
		$db->commit();
	}
?>
