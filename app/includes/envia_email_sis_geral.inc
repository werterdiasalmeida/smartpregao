<?php

# carrega as bibliotecas
include_once APPRAIZ . "includes/PogProgressBar.php";
include_once APPRAIZ . "includes/envia_email_sis_geral_funcoes.inc";
include_once APPRAIZ . "includes/Agrupador.php";

if( $_REQUEST['req'] == 'exibirDestinatarios' ){

	$_SESSION['municipios'] = $_REQUEST['arMunicipio'] ? substr( $_REQUEST['arMunicipio'], 0, -2 ) : '';
    die();
} 

switch($_SESSION['sisarquivo']) {
	case 'cte':
		
		$_REQUEST['statusUsuario'] = ($_REQUEST['statusUsuario'] ) ? $_REQUEST['statusUsuario'] : 'A';
		
		$suscod = $_REQUEST['statusUsuario'] ? " us.suscod = '". $_REQUEST['statusUsuario'] ."' and ": ""; 
		$exibefiltromunicipios = true; // Mostra o filtro por municipios
		
		if($_REQUEST['filtromun'] == 'sim' && $_REQUEST['perfil'][0] ) {
			if($_REQUEST['perfil'] && $_REQUEST['municipios']) {
				
				// carregando os perfis escolhidos que possam ser filtrados por municipio
				$perfismun = $db->carregar("SELECT * FROM cte.tprperfil WHERE pflcod IN('". implode("','", $_REQUEST['perfil']) ."') AND tprcod = '2'");
				// se algum dos perfis selecionados pode ser filtrado por municipio
				if($perfismun) {
					$_REQUEST['perfil'] = array_flip($_REQUEST['perfil']);
					
					$joinIdeb = $_REQUEST['ideb'] ? " INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = ur.muncod " : "";
					$clausulaIdeb = $_REQUEST['ideb'] ?  " AND mtm.tpmid IN ( ". implode(", ", $_REQUEST['ideb'] ) ." ) " : "";
					
					foreach($perfismun as $per) {
						$sql = "SELECT distinct u.usuemail, u.usunome FROM cte.usuarioresponsabilidade ur
								LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf  
								INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf
								$joinIdeb
								WHERE $suscod 
								ur.pflcod = ". $per['pflcod'] ."
								AND us.sisid = ". $_SESSION["sisid"] ." 
								AND rpustatus = 'A' 
								AND ur.muncod IN('". implode("','", $_REQUEST['municipios']) ."')
								$clausulaIdeb";
			
						$desti = $db->carregar($sql);
						// se existe algum usuario desses filtros, processar na opção de outros
						if($desti) {
							foreach($desti AS $de) {
								$_REQUEST["pessoas"] .= $de['usunome']." <".$de['usuemail'].">,"; 
							}
						}
						unset($_REQUEST['perfil'][$per['pflcod']]);							
					}
					$_REQUEST['perfil'] = array_flip($_REQUEST['perfil']);
				}
			}
		}
	break;
}

//print_r($_REQUEST);
if ( $_REQUEST[ "formulario" ] == 2 ) {
	
	# captura as informações submetidas
	$orgao = (integer) $_REQUEST["orgao"] > 2 ? $_REQUEST["orgao"] : null;
	$tipoEnsino = (integer) $_REQUEST["tipoensino"] ? $_REQUEST["tipoensino"] : null;
	$uo = (array) $_REQUEST["unidadeorcamentaria"];
	$ug = (integer) $_REQUEST["unidadegestora"] ? $_REQUEST["unidadegestora"] : null;
	$perfis = (array) $_REQUEST["perfil"];
	$ideb = (array) $_REQUEST["ideb"];
	$outros = $_REQUEST["pessoas"];
	$assunto = $_REQUEST["assunto"];
	$conteudo = $_REQUEST["mensagem"];
	$statusUsuario = $_REQUEST["statusUsuario"];
	
	# identifica os destinatários
	$destinatarios = EmailSistema::identificar_destinatarios( $orgao, $tipoEnsino, $uo, $ug, $perfis, $outros, $statusUsuario, $ideb );

	if ( empty( $destinatarios ) ) {
		echo '<script type="text/javascript">alert( "Não há destinatários para os filtros indicados." )</script>';
	}
	//-----------------adiciona o remetente do e-mail à lista de destinatários
	else{	
	
	$sql = 			
			"select distinct u.usunome, u.usuemail, u.usucpf
			from seguranca.usuario u				
			where u.usucpf = '".$_SESSION['usucpf']."'
		    group by u.usunome, u.usuemail, u.usucpf						
		    ";
							
	$remetente = $db->carregar( $sql );

	array_push($destinatarios, $remetente);
	
	}
	//----------------------------------------------------------------------------------
/*	
	# envia as mensagens
	$mensagem = new EmailSistema();
	
	if ( !$mensagem->enviar( $destinatarios, $assunto, $conteudo, $_FILES ) ) {
		$db->rollback();
		$db->insucesso( "Ocorreu uma falha ao enviar a mensagem." );
	}
	$db->commit();
	$db->sucesso( $_REQUEST["modulo"] );
*/
}

include APPRAIZ."includes/cabecalho.inc";
echo '<br/>';
monta_titulo( $titulo_modulo, "" );

?>

<!-- AUTO COMPLEMENTO -->
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/keys/keyEvents.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/tags/suggest.js"></script>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
    
<style>
	.suggestMarked
	{
		background-color: #AAAAFF;
		width:500px; 
		border-style: solid; 
		border-width: 1px; 
		border-color: #DDDDFF;
		border-top-width: 0px;
		position: relative;
		z-index: 100;
	}
	.suggestUnmarked
	{
		background-color: #EEEEFF;
		width:500px; 
		border-style: solid; 
		border-width: 1px; 
		border-color: #DDDDFF;
		border-top-width: 0px;
		z-index: 100;
		position: relative;
	}
	
	fieldset{
		width: 475px;
		padding: 10px;
	}
	
	fieldset input{
		width: 150px;
	}
	fieldset label{
		display: block;
		width: 40px;
		float: left;
	}
	
	fieldset br{
		clear: both;
	}
	
</style>

<!-- EDITOR DE TEXTO -->
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="javascript" type="text/javascript">
	tinyMCE.init({
	 	mode : "specific_textareas",
	    textarea_trigger : "mce_editable",
		theme : "advanced",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,forecolor,backcolor,fontsizeselect,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist, code",
		theme_advanced_buttons2 : "",
		theme_advanced_buttons3 : "",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		language : "pt_br",
		entity_encoding : "raw"
	});
</script>

<!-- EVENTOS DO FORMULÁRIO -->
<script language="javascript" type="text/javascript">
	function abrepopupMunicipio(){
		window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/combo_municipios_enviaemail.php','Municipios','width=400,height=400,scrollbars=1');
	}
	
	function abrepopupMunicipioUsuario(){
		window.open('http://<?=$_SERVER['SERVER_NAME']?>/geral/combo_municipios_enviaemail.php','Municipios','width=400,height=400,scrollbars=1');
	}
		
	function exibir_destinatarios() {
		// captura os campos
		var campo_orgao = document.getElementById( "orgao" );
		var campo_uo = document.getElementById( "unidadeorcamentaria" );
		var campo_ug = document.getElementById( "unidadegestora" );
		var campo_perfis = document.getElementById( "perfil" );
		var campo_municipios = document.getElementById( "municipios" );
		var campo_outros = document.getElementById( "pessoas" );
		var obStatusUsuario = document.getElementById( "statusUsuario" );
		var obIDEB = document.getElementById( "ideb" );
		// valores dos campos capturados
		var orgao = "";
		var uo = new Array();
		var ug = "";
		var perfis = new Array();
		var municipios = new Array();
		var arIDEB = new Array();
		var outros = "";
		// identifica o órgão selecionado
		if ( campo_orgao ) {
			if ( campo_orgao.selectedIndex != 0 ) {
				orgao = campo_orgao.options[campo_orgao.selectedIndex].value;
			}
		}
		if ( orgao ) {
			// captura as unidades orçamentárias
			if ( campo_uo ) {
				for ( i=0; campo_uo.options[i]; i++ ) {
					if ( campo_uo.options[i].value ) {
						uo.push( campo_uo.options[i].value );
					}
				}
			}
			// captura as unidades gestoras
			if ( campo_ug ) {
				ug = campo_ug.options[campo_ug.selectedIndex].value;
			}
		}
		// captura os perfis
		if ( campo_perfis ) {
			for ( i=0; campo_perfis.options[i]; i++ ) {
				if ( campo_perfis.options[i].value ) {
					perfis.push( campo_perfis.options[i].value );
				}
			}
		}
		
		// captura IDEB
		if ( obIDEB ) {
			for ( i=0; obIDEB.options[i]; i++ ) {
				if ( obIDEB.options[i].value ) {
					arIDEB.push( obIDEB.options[i].value );
				}
			}
		}
		
		// captura os municipios
		if ( campo_municipios ) {
			for ( i=0; campo_municipios.options[i]; i++ ) {
				if ( campo_municipios.options[i].value ) {
					municipios.push( campo_municipios.options[i].value );
				}
			}
		}

		// captura outros destinatários
		if ( campo_outros ) {
			outros = campo_outros.value;
		}
		
		// captura Status dos Usuários
		statusUsuario = obStatusUsuario.value;
		
		var parametros = "";
		parametros += "?pessoas=" + escape( outros );
		
		parametros += "&statusUsuario=" + statusUsuario;
		if ( orgao ) {
			parametros += "&orgao=" + orgao;
			if ( uo ) {
				for( i=0; uo[i]; i++ ){
					parametros += "&unidadeorcamentaria[]=" + uo[i];
				}
			}
			if ( ug ) {
				parametros += "&unidadegestora=" + ug;
			}
		}
		if ( perfis ) {
			for( i=0; perfis[i]; i++ ){
				parametros += "&perfil[]=" + perfis[i];
			}
		}
		
		if( arIDEB ){
			for( i=0; arIDEB[i]; i++ ){
				parametros += "&ideb[]=" + arIDEB[i];
			}
		}
		
		var arMunicipio = "";
		if ( municipios ) {
			for( i=0; municipios[i]; i++ ){
				arMunicipio += "'" +municipios[i] + "', ";
			}
		}
				
		<?		
		if($exibefiltromunicipios) {
		?>
			if(document.getElementById("filtromunsim").checked) {
			parametros += '&filtromun=sim';
			} else {
				parametros += '&filtromun=nao';
			}
		<?}?>			
			
		/*if(document.getElementById("filtromunsim").checked) {
			parametros += '&filtromun=sim';
		} else {
			parametros += '&filtromun=nao';
		}*/
		
		
		return new Ajax.Request(window.location.href,{
									method: 'post',
									parameters: {
                    					req: 'exibirDestinatarios',
                    					parametros: parametros,
                    					arMunicipio: arMunicipio
                					},
									onComplete: function(res){
										window.open( "/geral/destinatarios.php"+ parametros, "Destinatários", "resizable=no,scrollbars=yes,status=no,width=640,height=480");
									}
								});			
		
	}
</script>

<!-- FORMULÁRIO -->
<script language="javascript" type="text/javascript">
	function submeter_formulario(){
		var formulario = document.getElementById( 'formulario' );
		formulario.formulario.value = 2;
		var unidades_orcamentarias = formulario.unidadeorcamentaria;
		if ( unidades_orcamentarias ) {
			if( unidades_orcamentarias.options[0].value == "" ) {
				
			}
			selectAllOptions( unidades_orcamentarias );
		}
		
		//var estuf = formulario.estuf;
		var perfil = formulario.perfil;
		var municipiosUsuario = formulario.municipiosUsuario;
		
		if ( perfil ) {
			selectAllOptions( perfil );
		}
		
		if ( municipiosUsuario ) {
			selectAllOptions( municipiosUsuario );
		}
		/*
		if ( estuf ) {
			selectAllOptions( estuf );
		}
		*/
		selectAllOptions( document.getElementById( 'ideb' ) );
		formulario.submit();
	}
</script>
<form action="/geral/envia_email_selecao.php" id="formulario" method="post" name="formulario" enctype="multipart/form-data">
	<input type="hidden" name="formulario" value="1"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col style="width: 25%;">
			<col style="width: 75%;">
		</colgroup>
		<tbody>
			<tr>
				<td align='right' class="SubTituloDireita">Órgão:</td>
				<td>
					<?php
					/*$sql = "SELECT ee.entid as codigo, 
							   case when coalesce(ee.entorgcod, '' ) != ''
							      then ee.entorgcod || ' - ' || ee.entnome 
							   else ee.entnome 
							   end as descricao 
							FROM 
							   entidade.entidade ee
							WHERE funid in (31,32) ORDER BY ee.entnome";*/
					$sql = "SELECT orgcod as codigo, 
							   orgdsc as descricao 
							FROM 
							   public.orgao
							WHERE organo = '".$_SESSION['exercicio']."' ORDER BY orgdsc";
					
					$arrOrgaos = $db->carregar( $sql );
					$arrOrgaos = $arrOrgaos ? $arrOrgaos : array();
					?>
					<!--  select class="CampoEstilo" id="orgao" name="orgao" style="width: 500px;" onchange="document.formulario.submit();" -->
					<select class="CampoEstilo" id="orgao" name="orgao" style="width: 500px;" >
						<option value="1"> - Selecione - </option>
						<option value="2">Todos</option>
						<?php foreach( $arrOrgaos as $arrOrgao ): ?>
							<?php if ( $_REQUEST["orgao"] !=  $arrOrgao[ 'codigo' ] ): ?>
								<option value="<?= $arrOrgao[ 'codigo' ] ?>" >
									<?= $arrOrgao[ 'descricao' ] ?>
								</option>
							<?php else: ?>
								<option selected value="<?= $arrOrgao[ 'codigo' ] ?>" >
									<?= $arrOrgao[ 'descricao' ] ?>
								</option>
							<?php endif; ?>
						<?php endforeach; ?>
					</select>
				</td>
			</tr>
			<? if( $_SESSION["sisid"] == 56 ){ //verifica se o sistema e o academico ?>			
			
			<tr>
				<td align='right' class="SubTituloDireita">Tipo de Ensino:</td>
				<?php
					$sql = "SELECT 
							  orgid as codigo,
							  orgdesc as descricao
							FROM 
							  academico.orgao
							ORDER BY orgdesc";
				?>
				<td><?=$db->monta_combo("tipoEnsino",$sql, 'S',' - Selecione - ','', '', '',250,'N','tipoEnsino'); ?></td>
			</tr>
			<?} ?>
			
			<? if( $_SESSION["sisid"] == 57 ){ //verifica se o sistema e o academico ?>			
			
			<tr>
				<td align='right' class="SubTituloDireita">Iniciativa:</td>
				<?php
					$sql = "SELECT 
								iniid as codigo,
								ininome as descricao
							FROM emenda.iniciativa i 
							WHERE i.inistatus = 'A'";
				?>
				<td>
					<?=$db->monta_combo("ptiid",$sql, 'S',' - Selecione - ','', '', '',250,'N','ptiid'); ?>
					<!-- input type="checkbox" id="iniusuario" name="iniusuario">Usuário do sistema -->
					<input type="checkbox" id="inisomente" name="inisomente">Somente iniciativa
				</td>
			</tr>
			<? } ?>
			
			<?php if( $_REQUEST["orgao"] > 2 ): ?>
			<tr>
				<td align='right' class="SubTituloDireita">Unidade Orçamentária:</td>
				<td>
					<?php
					$sql =
						"SELECT 
							unidade.unicod AS codigo, 
							unidade.unidsc AS descricao
						 FROM 
						 	public.unidade AS unidade
						 INNER JOIN 
						 	seguranca.usuario AS usuario ON usuario.unicod = unidade.unicod
						 WHERE 
						 	usuario.entid = '{$_REQUEST["orgao"]}' AND unitpocod = 'U'
						 GROUP BY 
						 	codigo, descricao
						 ORDER BY 
						 	unidsc";
					combo_popup(
						'unidadeorcamentaria', 
						$sql, 
						'Selecione o(s) Grupo(s) de Unidade(s)', 
						'400x400', 
						0, 
						array(), 
						'', 
						'S', 
						true,
						false,
						10,
						500 ,
						'',
						''
					);
					?>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Unidade Gestora:</td>
				<td>
					<?php
					$sql =
						"SELECT 
							ungcod AS codigo, 
							ungdsc AS descricao 
						 FROM 
						 	public.unidadegestora
						 WHERE 
						 	unitpocod = 'U'
 						 ORDER BY 
							ungdsc";
					$arrUnidadesGestoras = $db->carregar( $sql );
					?>
					<select class="CampoEstilo"  id="unidadegestora" name="unidadegestora" style="width: 500px;">
						<option value="0" <?= ( $_REQUEST["unidadegestora"] == 0 ? 'selected' : '') ?> >Todos</option>
						<?php foreach( $arrUnidadesGestoras as $arrUnidadeGestora ): ?>
							<?php if ( $_REQUEST["unidadegestora"] !=  $arrUnidadeGestora[ 'codigo' ] ): ?>
								<option value="<?= $arrUnidadeGestora[ 'codigo' ] ?>" >
									<?= $arrUnidadeGestora[ 'descricao' ] ?>
								</option>
							<?php else: ?>
								<option value="<?= $arrUnidadeGestora[ 'codigo' ] ?>" >
									<?= $arrUnidadeGestora[ 'descricao' ] ?>
								</option>
							<?php endif; ?>
						<?php endforeach; ?>
					</select>
				</td>
			</tr>
			<?php endif; ?>			
			<tr>
				<td align='right' class="SubTituloDireita">Perfil:</td>
				<td>
					<?php
					$sql =
						"select pflcod as codigo, pfldsc as descricao 
						from perfil
						where sisid = '{$_SESSION["sisid"]}' and pflstatus = 'A'
						order by pfldsc";

					combo_popup( 'perfil', $sql, 'Selecione os Perfis', '400x400', 0, array(), '', 'S', false, false, 10, 500, '', '' );
					?>		
				</td>
			</tr>
			<tr id="trfiltromunicipio">
				<td align='right' class="SubTituloDireita">Municípios dos usuários:</td>
				<td>
					<select multiple="multiple" size="7" name="municipiosUsuario[]" id="municipiosUsuario" ondblclick="abrepopupMunicipioUsuario();" class="CampoEstilo" style="width:400px;" >
						<option value="">Duplo clique para selecionar da lista</option>
			        </select>
				</td>
			</tr>
			<!--				
			<tr id="trfiltrouf">
				<td align='right' class="SubTituloDireita">UF:</td>
				<td>
					<?php
						$sqlComboUF = " select estuf as codigo, estdescricao as descricao
										from territorios.estado
										order by estdescricao";
						echo combo_popup( "estuf", $sqlComboUF, "UF", "400x400", 0, array(), "", "S", false, false, 5, 300 );
					?>
				</td>
			</tr>			
			-->
			<?
			// Exibe o filtro por municipios
			if($exibefiltromunicipios) {
			?>
			<tr>
				<td align='right' class="SubTituloDireita">Status do Usuário:</td>
				<td>
					<select class="CampoEstilo" id="statusUsuario" name="statusUsuario">
						<?php 
						
						$sql = "select suscod, susdsc from seguranca.statususuario";	
						$coStatus = $db->carregar( $sql );
					
						foreach( $coStatus as $arStatus ){ 
							
							$selected = $arStatus["suscod"] == $_REQUEST["statusUsuario"] ? 'selected="selected" ' : "" ?>
						
							<option <?= $selected; ?> value="<?php echo $arStatus["suscod"] ?>"><?php echo $arStatus["susdsc"] ?></option>
						<?php } ?>
						<option value="0">Todos</option>
					</select>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Deseja filtrar o perfil por municípios ?</td>
				<td><input type="radio" name="filtromun" id="filtromunsim" value="sim" onclick="if(this.checked){document.getElementById('trfiltromunicipio').style.display = '';}else{document.getElementById('trfiltromunicipio').style.display = 'none';}"> Sim&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="radio" name="filtromun" id="filtromunnao" value="nao" onclick="if(this.checked){document.getElementById('trfiltromunicipio').style.display = 'none';}else{document.getElementById('trfiltromunicipio').style.display = '';}" checked> Não</td>
			</tr>
			<tr id="trfiltromunicipio" style="display:none">
				<td align='right' class="SubTituloDireita">Municípios:</td>
				<td>
				<select multiple="multiple" size="5" name="municipios[]" 
        		id="municipios"  
        		ondblclick="abrepopupMunicipio();"  
        		class="CampoEstilo" style="width:400px;" >
				<option value="">Duplo clique para selecionar da lista</option>
		        </select>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Classificação IDEB</td>
				<td>
					<?php
					$agrupador = new Agrupador( "formulario" );
					
					$sql = sprintf( "select tpmid, tpmdsc from territorios.tipomunicipio where gtmid = 2 and tpmstatus = 'A'" );
					$tipos = $db->carregar( $sql );
					
					$origem = array();
					$destino = array();
					if ( $tipos ) {
						foreach ( $tipos as $tipo ) {
							if ( in_array( $tipo['tpmid'], (array) $_REQUEST['ideb'] ) ) {
								$destino[] = array(
									'codigo'    => $tipo['tpmid'],
									'descricao' => $tipo['tpmdsc']
								);
							} else {
								$origem[] = array(
									'codigo'    => $tipo['tpmid'],
									'descricao' => $tipo['tpmdsc']
								);
							}
						}
					}
					
					$agrupador->setOrigem( "ideb_", null, $origem );
					$agrupador->setDestino( "ideb", null, $destino );
					$agrupador->exibir();
					?>
				</td>
			</tr>			
			<? } ?>	
			<tr>
				<td align='right' class="subtitulodireita">Função/Cargo:</td>
				<td>
		
					<?php
						//$sql = "select carid as codigo, cardsc as descricao from public.cargo";
						$sql = "select carid as codigo, cardsc as descricao from public.cargo where carid <> 9";
						$db->monta_combo( "cargo", $sql, 'S', 'Selecione', '', '', '', '', 'N', "cargo", '' );
					?>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Outros Destinatários:</td>
				<td>
					<textarea type="text" name="pessoas" id="pessoas" style="width: 500px; height: 100px" ><?= $_REQUEST["pessoas"] ?></textarea>
					<br/>
					<div id="divSuggestPessoas" style="position: absolute; z-index: 1000;"></div>
					<script>
						objSuggestPessoas = new window.Suggest( 
							document.getElementById( "pessoas" ) , 
							document.getElementById( "divSuggestPessoas" ),
							"suggestlist.php"
						);
					</script>
				</td>
			</tr>
			<tr>
				<td colspan="2" style="text-align: center;" class="subtitulodireita">&nbsp;</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Assunto:</td>
				<td><input class="normal" type="text" value="<?= $_REQUEST["assunto"] ?>" id="assunto"  name="assunto" style="width: 500px" /></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Anexo:</td>
				<td><input type="file" name="anexo" /></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Mensagem:</td>
				<td>
					<textarea mce_editable="true" id="mensagem"  name="mensagem" style="width: 500px; height: 150px"><?= $_REQUEST["mensagem"] ?></textarea>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Remetente:</td>
				<td>
					<fieldset>
						<legend>Dados do Remetente</legend>
						<label for="stNomeRemetente">Nome: </label><input class="normal" type="text" name="stNomeRemetente" id="stNomeRemetente" /><br />
						<label for="stEmailRemetente">E-Mail: </label><input class="normal" type="text" name="stEmailRemetente" id="stEmailRemetente" /><br /><br />
						* No caso do não preenchimento dos campos acima, o destinatário será o usuário logado no sistema. 
					</fieldset>
				</td>
			</tr>
			<tr bgcolor="#C0C0C0">
				<td></td>
				<td>
					<input type='button' class="botao" name='EnviarEmail' value='Enviar' onclick="submeter_formulario();"/>
					<!-- span style="cursor: pointer; color:#FF0000;" onclick="exibir_destinatarios();">Clique aqui para verificar a lista de destinátarios.</span -->
				</td>
			</tr>
		</tbody>
	</table>
</form>