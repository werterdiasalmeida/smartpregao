<? 
/*
   Sistema Simec
   Setor responsável: DTI-MEC
   Desenvolvedor: Equipe Simec
   Analista: Luiz Fernando Carvalho Mendonça
   Programador: Afonso Alves Ribeiro
   Módulo:SIS
   Finalidade: efetuar cadastro de gestores de sistemas no SIS e em outros modulos.
   */
include APPRAIZ . "includes/cabecalho.inc";
print "<br/>";

if(isset($_REQUEST['requisicao']) && $_REQUEST['requisicao'] == 'gravarGestor') {
    $gesnome             = $_POST['gesnome'];
	$cargo 		   		 = $_POST['gescargo'];
    $setor				 = $_POST['gessetor'];
    $telefone			 = $_POST['gesfone'];
    $email				 = $_POST['gesemail'];
    $gesativo            = $_POST['gesativo'];
	$gesid 				 = $_POST['gesid'];
	
	if( empty($gesid) ){
    
		if ($gesnome && $cargo && $setor && $telefone && $email && $gesativo != ''){
			$sql = "INSERT INTO seguranca.gestorsistema ( gesnome, gescargo, gessetor, gesfone, gesemail, gesativo ) 
						VALUES
					( '{$gesnome}' , '{$cargo}', '{$setor}', '{$telefone}', '{$email}', '{$gesativo}' ) RETURNING gesid";
			
			$gesid = $db->pegaUm($sql);
		
			foreach ( $_REQUEST['selecsis'] as $sis => $s ){
				$sql = "SELECT modid FROM seguranca.modulo where sisid = ".$s;
				$modulos = $db->carregar( $sql );
				if( !empty($modulos) ){ //se ele tem modulos dentro dele, a principio, temos q verificar se marcaram algum.
					foreach( $modulos as $mod ){
						if( $_REQUEST['selecmodulo'] ){
							if( $s <> ''){
								$sql = "INSERT INTO seguranca.sistemapossuigestor ( gesid, sisid, modid ) 
											VALUES
										( {$gesid} , {$s}, {$mod['modid']} )";
								$db->executar($sql);	  
							}
						}
					}
				} else { //insere sem modulos
					if( $s <> ''){
						$sql = "INSERT INTO seguranca.sistemapossuigestor ( gesid, sisid, modid ) 
									VALUES
								( {$gesid} , {$s}, null )";
						$db->executar($sql);	  
					}
				}
			}
		
			$db->commit();
			echo '<script> alert("Dados inseridos com sucesso.") </script>';
		} else {
			echo '<script> alert ("Preencha os campos obrigatorios.") </script>';		
		}
	} else {
		$sql = "UPDATE seguranca.gestorsistema SET
			   gesnome = '{$gesnome}',
			   gescargo = '{$cargo}',
			   gessetor = '{$setor}',
			   gesfone = '{$telefone}',
			   gesemail = '{$email}',
			   gesativo = '{$gesativo}'
			WHERE gesid=".$gesid;
		$db->executar($sql);
		$db->commit();	
		echo '<script> alert("Dados gravados com sucesso.") </script>';
	}
}

if(isset($_REQUEST['requisicao']) && $_REQUEST['requisicao'] == 'editarGestor') {
	$gesid = $_REQUEST['gesid']; 
    $gesnome             = $_POST['gesnome'];
	$cargo 		   		 = $_POST['gescargo'];
    $setor				 = $_POST['gessetor'];
    $telefone			 = $_POST['gesfone'];
    $email				 = $_POST['gesemail'];
    $gesativo            = $_POST['gesativo'];
    
    $sql = "UPDATE seguranca.gestorsistema SET
			   gesnome = '{$gesnome}',
			      
			WHERE gesid=$gesid";
	$db->executar($sql);
	$db->commit();	
    
}

 if(isset($_REQUEST['requisicao']) && $_REQUEST['requisicao'] == 'excluir') {
	$gesid = $_REQUEST['gesid']; 
	$sql = "UPDATE seguranca.gestorsistema SET
			   gesativo = 'I'   
			WHERE gesid=$gesid";
	$db->executar($sql);
	$db->commit();	
}

?>
	<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
	<script >
	function gravarGestor() {
			selectAllOptions( formulario.selecsis );
			selectAllOptions( formulario.selecmodulo );
			$('#requisicao').val("gravarGestor");
			$('#formulario').submit();
	}

	function alterarGestor(gesid) {
		window.location.href = "seguranca.php?modulo=sistema/cadastro/cadastrarGestor&acao=A&gesid=" +gesid;	
	}

	function excluirGestor(gesid) {
		if (confirm ('Deseja realmente excluir este gestor?')){
			window.location = "seguranca.php?modulo=sistema/cadastro/consultarGestor&acao=A&requisicao=excluir&gesid=" +gesid;
		}
	}

	function isPreenchido(idCampo) {
		return $(idCampo) != "";
	}

	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
	
	</script>	

	<?php
		$db->cria_aba( $abacod_tela, $url, '' );
		$titulo_modulo = "Cadastro de Gestores de Sistemas";
		monta_titulo( $titulo_modulo, '<div style="margin:3px;" align="right" class="' . $class . '"></div>' );
		$gesid = $_REQUEST['gesid'];
		?>
	
	<form method="post" name="formulario" id="formulario"> 
		<input type="hidden" id="gesid" name="gesid" value="<?=$gesid?>">
		<input type="hidden" id="requisicao" name="requisicao" value="gravarGestor">
		<table align="center" width="95%" cellSpacing="1" cellPadding="3" class="tabela" bgcolor="#f5f5f5" >
	      <tr>
	        <td align='right' class="SubTituloDireita" width="15%">Nome:</td>
	        <? if( $gesid ){ $sql = "SELECT * FROM seguranca.gestorsistema WHERE gesid = ".$gesid;
	        	$variavel = $db->pegaLinha( $sql ); }?>
	        <td colspan="2"><?= campo_texto('gesnome','S','S','',50,50,'','','','','','','',$variavel['gesnome'],'','');?></td>
	      </tr>
   	      <tr>
	        <td align='right' class="SubTituloDireita">Cargo:</td>
	        <td colspan="2"><?= campo_texto('gescargo','S','S','',50,50,'','','','','','','',$variavel['gescargo'],'','');?></td>
	      </tr>
	      <tr>
	        <td align='right' class="SubTituloDireita">Setor:</td>
	        <td colspan="2"><?= campo_texto('gessetor','S','S','',50,50,'','','','','','','',$variavel['gessetor'],'','');?></td>
	      </tr>
	      <tr>
	        <td align='right' class="SubTituloDireita">telefone:</td>
	        <td colspan="2"><?= campo_texto('gesfone','S','S','',50,50,'','','','','','','',$variavel['gesfone'],'','');?></td>
	      </tr>
	      <tr>
	        <td align='right' class="SubTituloDireita">Email:</td>
	        <td colspan="2"><?= campo_texto('gesemail','S','S','',50,50,'','','','','','','',$variavel['gesemail'],'','');?></td>
	      </tr>
	      <tr>
	        <td colspan="2"><?$stSql = "SELECT
										sisid AS codigo,
										sisdsc AS descricao
									  FROM
									  	seguranca.sistema";
	        	if( $_REQUEST['gesid'] ){
	        		$sqlCarr = "SELECT distinct sp.sisid as codigo, s.sisdsc AS descricao FROM seguranca.sistemapossuigestor sp INNER JOIN seguranca.sistema s ON s.sisid = sp.sisid WHERE gesid = ".$_REQUEST['gesid'];
	        	}
				mostrarComboPopup( 'Sistema:', 'selecsis',  $stSql, $sqlCarr, 'Selecione o(s) Sistema(s)' ); ?> </td>
	      </tr>
	      <tr>
	        <td colspan="2"><?$stSql = "SELECT
										modid AS codigo,
										moddsc AS descricao
									  FROM
									  	seguranca.modulo";
	        	if( $_REQUEST['gesid'] ){
	        		$sqlCarr = "SELECT distinct sp.modid as codigo, m.moddsc AS descricao FROM seguranca.sistemapossuigestor sp INNER JOIN seguranca.modulo m ON m.modid = sp.modid WHERE gesid = ".$_REQUEST['gesid'];
	        	}
				mostrarComboPopup( 'Modulo:', 'selecmodulo',  $stSql, $sqlCarr, 'Selecione o(s) Modulo(s)' );
				 ?> </td>
	      </tr>
	      <tr>
	      <?php 
	      	$ckativo = '';
	      	$ckinativo = '';
	      	if( $variavel['gesativo'] == 'A' ){ $ckativo = "checked=checked"; } else { $ckinativo = "checked=checked"; }  ?>
	        <td align='right' class="SubTituloDireita">Status:</td>
	        <td>
	        <input type="radio" name="gesativo" <? echo $ckativo; ?> value="A"> Ativo
			<input type="radio" name="gesativo" <? echo $ckinativo; ?> value="I"> Inativo
			</td> 
	      </tr>
	      <tr>
	      	<td align='right' class="SubTituloEsquerda"></td>
			<td id="td1" align='right' class="SubTituloEsquerda">
	        	<input type="Button" style="width:80px;" id="gravar" name="gravar" value="Salvar" onclick="gravarGestor()">
			</td>
	      </tr>
	      
		</table>
	</form>

	<table align="center" width="95%" border="0" cellpadding="2" cellspacing="0" class="listagem" style="color:#333333">
		<tbody>
			<?
			$sql_lista = "
					select distinct
		 				   '<img 	src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"  
									id=\"img ' ||  gs.gesid || '\" name=\"+\" 
									onclick=\"javascript:alterarGestor(' || gs.gesid || ');\"/>'
			
							|| '&nbsp;'
									
		 				   '<img 	src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"  
									id=\"img ' ||  gs.gesid || '\" name=\"+\" 
									onclick=\"javascript:excluirGestor(' || gs.gesid || ');\"/>'							
									
									as acoes,
						   gs.gesid,
						   gs.gesnome as gestor, 
						   gs.gessetor as setor, 
						   gs.gesfone as telefone, 
						   gs.gesemail as email, 
						   gs.gesativo as status
						   -- sistema.sisdsc 
						   
						from 
						seguranca.gestorsistema as  gs
						left join seguranca.sistemapossuigestor sisgest on gs.gesid = sisgest.gesid";

			if( is_combo_set('gesid') ) {
				$gesid = $_REQUEST['gesid']; 
				$sql_lista .= " and gs.gesid = $gesid ";
			}
			
			$sql_lista.= " ORDER BY gs.gesid DESC";
			   
			$cabecalho = array("Ação","Id","Nome","Setor","Telefone","Email","Status");
			$db->monta_lista($sql_lista,$cabecalho,60,20,'','','');
			?>
		</tbody>
	</table>	
<?

function is_combo_set($campo='') {
	$auxiliar = $_REQUEST[$campo];
	return isset($auxiliar) && $auxiliar != "" && $auxiliar != "x";
}?>