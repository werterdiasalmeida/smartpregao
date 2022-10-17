<?php

header('content-type: text/html; charset=ISO-8859-1');

include_once APPRAIZ . "www/painel/_funcoes_painel_controle.php";
include_once APPRAIZ . "www/painel/redefederal/_funcoes_redefederal.php";
include_once APPRAIZ . "includes/classes/relatorio.class.inc";

// atualiza painel com as unidades da UF selecionada
if ( $_REQUEST['listarunidades'] == 1 ){
	if ($_REQUEST['estuf']){
		
		// cria o join e o filtro por região, caso exista
		switch( $_REQUEST['estuf'] ){
			case 'norte':
				$filtro['regcod'] = 1;
			break;
			case 'nordeste':
				$filtro['regcod'] = 2;
			break;
			case 'sudeste':
				$filtro['regcod'] = 3;
			break;
			case 'sul':
				$filtro['regcod'] = 4;
			break;
			case 'centrooeste':
				$filtro['regcod'] = 5;
			break;
			default:
				$filtro['estuf'] = $_REQUEST['estuf']; 
			break;
		}
			
		// lista de todas as undiades (paãs) 
		painel_lista_unidades_painel( $_REQUEST['estuf'], $_REQUEST['orgid'] );
	
		?>
		
		<!-- Quadro de Situação da Obra -->
		<div id="quadrosituacao" class="caixa_redefederal2"/>	
			<table cellspacing="1" cellpadding="3" style="width: 100%;">
				<tr>
					<td style="text-align: center; background-color: #dedede; font-weight: bold;"> Obras </td>
				</tr>
				<tr>
					<td style="padding: 0px; margin: 0px;">
						<?php painel_situacao_obras( $_SESSION['redefederal']['orgid'], $_REQUEST['estuf'] ); ?>
					</td>
				</tr>
			</table>
		</div>
       	      
		<div id="quadrosituacaoatual" class="caixa_redefederal2"/>	
			<table cellspacing="1" cellpadding="3" style="width: 100%;">
				<tr>
					<td style="text-align: center; background-color: #dedede; font-weight: bold;"> Situação Atual </td>
				</tr>
				<tr>
					<td style="padding: 0px; margin: 0px;">
						<?php painel_situacao_atual( $_SESSION['redefederal']['orgid'], $filtro ); ?>
					</td>									
				</tr>
			</table>
		</div>
		
		<?php
				$paTituloQuadro = ($_SESSION['redefederal']['orgid'] == 1) ? "Expansão da Educação Superior"
																		   : "Expansão da Educação Profissional e Tecnológica";
		?>
		
		<div id="quadrosituacaoatual" class="caixa_redefederal"/>	
				<table cellspacing="1" cellpadding="3" style="width: 100%;">
					<tr>
						<td style="text-align: center; background-color: #dedede; font-weight: bold;"> <?php echo $paTituloQuadro ?> </td>
					</tr>
					<tr>
						<td style="padding: 0px; margin: 0px;">
							<?php painel_painel_dados_sig( $_SESSION['redefederal']['orgid'], $_REQUEST['estuf'] ); ?>
						</td>									
					</tr>
				</table>
			</div>
		</div>
		       	       	
		<?
		die;
		
	}
}
// Abre árvore com os campi das unidades
if( $_REQUEST['carga'] ){
	$_SESSION['redefederal']['orgid'] = !empty($_SESSION['redefederal']['orgid']) ? $_SESSION['redefederal']['orgid'] : 1;
	painel_lista_campus_painel( $_REQUEST['carga'], $_SESSION['redefederal']['orgid'] );
	die;
	
}

if ( $_REQUEST["cargaexpansao"] ){
	painel_painel_campus_dados_sig ( $_SESSION['redefederal']['orgid'], $_REQUEST["cargaexpansao"], $_REQUEST["estuf"] );
	die;
	
}

// atualiza quadro/mapa do painel de acordo com a requisicaoajax
painel_atualiza_quadro( $_REQUEST['requisicaoajax'], $_REQUEST['dado'], $_SESSION['redefederal']['orgid'], $_REQUEST['estuf'], $_REQUEST['entid']);


include_once APPRAIZ . "includes/cabecalho.inc";
print "<br/>";

if( !$cabecalho_painel ){
		   exibeAbaUsuario($_SESSION['usucpf'],$_SERVER['REQUEST_URI']);
		   monta_titulo( 'Rede Federal', '');
	}

print "<br/>";

$_SESSION['redefederal']['orgid'] = $_REQUEST["orgid"] ? $_REQUEST["orgid"] : 1;

$arMenuTipoEnsino = array( array( "link" => "painel.php?modulo=principal/redefederal/rede_federal&acao=A&orgid=1",
								  "descricao" => "Educação Superior"),
						   array( "link" => "painel.php?modulo=principal/redefederal/rede_federal&acao=A&orgid=2",
								  "descricao" => "Educação Profissional"));

print montarAbasArray( $arMenuTipoEnsino, "painel.php?modulo=principal/redefederal/rede_federal&acao=A&orgid={$_SESSION['redefederal']['orgid']}" );

$paTituloQuadro = ($_SESSION['redefederal']['orgid'] == 1) ? "Expansão da Educação Superior"
														   : "Expansão da Educação Profissional e Tecnológica";


							   
?>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />

<script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

	var params;
	
	function formatarParametros(){
		//params = Form.serialize($('pesquisar'));
	}
	
	function desabilitarConteudo( id ){
		var url = window.location.href + '&carga=' + id;
		if ( document.getElementById('img'+id).name == '-' ) {
			url = url + '&subAcao=retirarCarga';
			var myAjax = new Ajax.Request(
				url,
				{
					method: 'post',
					asynchronous: false
				}
			);
		}
	}
	
	
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url){
		
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(444,357);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function displayStaticMessage(messageContent,cssClass){
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(400,250);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
		
		
	}
	
	function closeMessage(){
		messageObj.close();	
	}

	function carregar_relatorio( acao, unidade, ano ){
		
		var link = '';

		switch( acao ){
			case 'execucao':
				link = 'execucao'
			break;
		}

		// relatãrio execução REUNI
		if(acao == "execucaoreuni") {
			var janela = window.open( 'painel.php?modulo=relatorio/redefederal/relatorio_execucao_reuni_resultado&acao=A&entid=' + unidade + '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			janela.focus();
			return;
		}
		
		var url = 'painel.php?modulo=principal/redefederal/financeiro_painel&acao=A';
		var parametros = "";
		
		var myAjax = new Ajax.Request(
		url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: false,
				onComplete: function(resp) {
					window.open( 'painel.php?modulo=principal/redefederal/financeiro_painel&acao=A&relatorioajax=' + link + '&entid=' + unidade + '&ano=' + ano, 'relatorio', 'width=800,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				}
			}
		);
					
	}
	
	function abreMapa(obrid){

		var graulatitude = window.document.getElementById(obrid + "graulatitude").value;
		var minlatitude  = window.document.getElementById(obrid + "minlatitude").value;
		var seglatitude  = window.document.getElementById(obrid + "seglatitude").value;
		var pololatitude = window.document.getElementById(obrid + "pololatitude").value;
		
		var graulongitude = window.document.getElementById(obrid + "graulongitude").value;
		var minlongitude  = window.document.getElementById(obrid + "minlongitude").value;
		var seglongitude  = window.document.getElementById(obrid + "seglongitude").value;
		
		var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
		var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
		
		var janela=window.open('painel.php?modulo=principal/redefederal/mapa&acao=A&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude+'&obrid='+obrid, 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();
		
	}
	function redefederal_listaUnidades( uf, orgid ){

		var url = window.location.href;
		new Ajax.Updater('conteudolistaunidades', url,
			{
			method: 'post',
			parameters: '&listarunidades=1&estuf=' + uf + '&orgid=' + orgid,
			onComplete: function(res)
				{        
				closeMessage();    	
				}
			}
		);
		
	}
	 
	function atualiza_div( acao, dado ){

		var link = '';
		switch( acao ){
			case 'obras':
				link = 'listaobras'
			break;
			case 'unidade':
				link = 'dadosunidade'
			break;
			case 'campus':
				link = 'dadoscampus'
			break;
			case 'academico':
				link = 'dadosacademico'
			break;
			case 'concursos':
				link = 'dadosconcursos'
			break;
			case 'financeiro':
				link = 'dadosfinanceiro'
			break;
			case 'listaCampus':
				link = 'listaCampus';
			break;
			
		}

		var url = window.location.href;
		new Ajax.Updater('containerMapa', url,
			{
			method: 'post',
			parameters: '&requisicaoajax=' + link + '&dado=' + dado,
			onComplete: function(res)
				{        
					extrairScript(res.responseText);
				}
			}
		);
		
	}

	function ver_obras_situacao( stoid, estuf, entid ){

		var url = window.location.href;
		new Ajax.Updater('containerMapa', url,
			{
			method: 'post',
			parameters: '&requisicaoajax=situacaoobras&dado=' + stoid + '&estuf=' + estuf + '&entid=' + entid,
			onComplete: function(res)
				{        
				
				}
			}
		);
			
	}

</script>
<style>
		
	.caixa_redefederal{
		float: left; 
		margin: 5px;
		height: 151px; 
		width: 97%; 
		border: 1px solid #cccccc;
		overflow: auto;
	}
	
	.caixa_redefederal2{
		float: left; 
		margin: 5px;
		height: 170px; 
		width: 47%; 
		border: 1px solid #cccccc;
		overflow: auto;
	}		
	
	.titulounidade {
		background-color:#CDCDCD;
		border-top:2px solid #000000;
		border-bottom:2px solid #000000;
		text-align:center;
		font-weight: bold;
	}
	
</style>

<table id="containerSuperior" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
	<tbody>
	
		<tr>
			<td align="center" valign="top" width="45%">
				<div id="conteudolistaunidades" style="width:100%;"/>
						
					<?php painel_lista_unidades_painel( 'todos', $_SESSION['redefederal']['orgid'] ); ?>
				
					<!-- Quadro de situação da Obra -->
					<div id="quadrosituacao" class="caixa_redefederal2"/>	
						<table cellspacing="1" cellpadding="3" style="width: 100%;">
							<tr>
								<td style="text-align: center; background-color: #dedede; font-weight: bold;"> Obras </td>
							</tr>
							<tr>
								<td style="padding: 0px; margin: 0px;">
									<?php painel_situacao_obras( $_SESSION['redefederal']['orgid']  ); ?>
								</td>
							</tr>
						</table>
					</div>

					<div id="quadrosituacaoatual" class="caixa_redefederal2"/>	
						<table cellspacing="1" cellpadding="3" style="width: 100%;">
							<tr>
								<td style="text-align: center; background-color: #dedede; font-weight: bold;"> Situação Atual </td>
							</tr>
							<tr>
								<td style="padding: 0px; margin: 0px;">
									<?php painel_situacao_atual( $_SESSION['redefederal']['orgid'] ); ?>
								</td>									
							</tr>
						</table>
					</div>
					
					<div id="quadrosig" class="caixa_redefederal"/>	
						<table cellspacing="1" cellpadding="3" style="width: 100%;">
							<tr>
								<td style="text-align: center; background-color: #dedede; font-weight: bold;"> <?php echo $paTituloQuadro ?> </td>
							</tr>
							<tr>
								<td style="padding: 0px; margin: 0px;">
									<?php painel_painel_dados_sig( $_SESSION['redefederal']['orgid'], '', '' ); ?>
								</td>									
							</tr>
						</table>
					</div>
					</fieldset>		
				</div>
			</td>		
			<td align="center" valign="top" width="45%">
			<fieldset style="background: #ffffff; width: 90%"  >
				<legend> Painel </legend>
				
				<div style="width: 100%;" id="containerMapa" >
					<img src="/imagens/mapa_brasil.png" width="444" height="357" border="0" usemap="#mapaBrasil" />
					<map name="mapaBrasil" id="mapaBrasil">
						<area shape="rect" coords="388,15,427,48"   style="cursor:pointer;" onclick="redefederal_listaUnidades('todos', <?=$_SESSION['redefederal']['orgid']?>);" title="Brasil"/>							
						<area shape="rect" coords="48,124,74,151"   style="cursor:pointer;" onclick="redefederal_listaUnidades('AC', <?=$_SESSION['redefederal']['orgid']?>);" title="Acre"/>
						<area shape="rect" coords="364,147,432,161" style="cursor:pointer;" onclick="redefederal_listaUnidades('AL', <?=$_SESSION['redefederal']['orgid']?>);" title="Alagoas"/>
						<area shape="rect" coords="202,27,233,56"   style="cursor:pointer;" onclick="redefederal_listaUnidades('AP', <?=$_SESSION['redefederal']['orgid']?>);" title="Amapá"/>
						<area shape="rect" coords="89,76,133,107"   style="cursor:pointer;" onclick="redefederal_listaUnidades('AM', <?=$_SESSION['redefederal']['orgid']?>);" title="Amazonas"/>
						<area shape="rect" coords="294,155,320,183" style="cursor:pointer;" onclick="redefederal_listaUnidades('BA', <?=$_SESSION['redefederal']['orgid']?>);" title="Bahia"/>
						<area shape="rect" coords="311,86,341,114"  style="cursor:pointer;" onclick="redefederal_listaUnidades('CE', <?=$_SESSION['redefederal']['orgid']?>);" title="Cearã"/>
						<area shape="rect" coords="244,171,281,197" style="cursor:pointer;" onclick="redefederal_listaUnidades('DF', <?=$_SESSION['redefederal']['orgid']?>);" title="Distrito Federal"/>
						<area shape="rect" coords="331,215,369,242" style="cursor:pointer;" onclick="redefederal_listaUnidades('ES', <?=$_SESSION['redefederal']['orgid']?>);" title="Espírito Santo"/>
						<area shape="rect" coords="217,187,243,218" style="cursor:pointer;" onclick="redefederal_listaUnidades('GO', <?=$_SESSION['redefederal']['orgid']?>);" title="Goiás"/>
						<area shape="rect" coords="154,155,210,186" style="cursor:pointer;" onclick="redefederal_listaUnidades('MT', <?=$_SESSION['redefederal']['orgid']?>);" title="Mato Grosso"/>
						<area shape="rect" coords="156,219,202,246" style="cursor:pointer;" onclick="redefederal_listaUnidades('MS', <?=$_SESSION['redefederal']['orgid']?>);" title="Mato Grosso do Sul"/>
						<area shape="rect" coords="248,80,301,111"  style="cursor:pointer;" onclick="redefederal_listaUnidades('MA', <?=$_SESSION['redefederal']['orgid']?>);" title="Maranhão"/>
						<area shape="rect" coords="264,206,295,235" style="cursor:pointer;" onclick="redefederal_listaUnidades('MG', <?=$_SESSION['redefederal']['orgid']?>);" title="Minas Gerais"/>
						<area shape="rect" coords="188,84,217,112"  style="cursor:pointer;" onclick="redefederal_listaUnidades('PA', <?=$_SESSION['redefederal']['orgid']?>);" title="Pará"/>
						<area shape="rect" coords="368,112,433,130" style="cursor:pointer;" onclick="redefederal_listaUnidades('PB', <?=$_SESSION['redefederal']['orgid']?>);" title="Paraíba"/>
						<area shape="rect" coords="201,262,231,289" style="cursor:pointer;" onclick="redefederal_listaUnidades('PR', <?=$_SESSION['redefederal']['orgid']?>);" title="Paraná"/>
						<area shape="rect" coords="369,131,454,147" style="cursor:pointer;" onclick="redefederal_listaUnidades('PE', <?=$_SESSION['redefederal']['orgid']?>);" title="Pernambuco"/>
						<area shape="rect" coords="285,116,313,146" style="cursor:pointer;" onclick="redefederal_listaUnidades('PI', <?=$_SESSION['redefederal']['orgid']?>);" title="Piauí"/>
						<area shape="rect" coords="349,83,383,108"  style="cursor:pointer;" onclick="redefederal_listaUnidades('RN', <?=$_SESSION['redefederal']['orgid']?>);" title="Rio Grande do Norte"/>
						<area shape="rect" coords="189,310,224,337" style="cursor:pointer;" onclick="redefederal_listaUnidades('RS', <?=$_SESSION['redefederal']['orgid']?>);" title="Rio Grande do Sul"/>
						<area shape="rect" coords="302,250,334,281" style="cursor:pointer;" onclick="redefederal_listaUnidades('RJ', <?=$_SESSION['redefederal']['orgid']?>);" title="Rio de Janeiro"/>
						<area shape="rect" coords="98,139,141,169"  style="cursor:pointer;" onclick="redefederal_listaUnidades('RO', <?=$_SESSION['redefederal']['orgid']?>);" title="Rondônia"/>
						<area shape="rect" coords="112,24,147,56"   style="cursor:pointer;" onclick="redefederal_listaUnidades('RR', <?=$_SESSION['redefederal']['orgid']?>);" title="Roraima"/>
						<area shape="rect" coords="228,293,272,313" style="cursor:pointer;" onclick="redefederal_listaUnidades('SC', <?=$_SESSION['redefederal']['orgid']?>);" title="Santa Catarina"/>
						<area shape="rect" coords="233,243,268,270" style="cursor:pointer;" onclick="redefederal_listaUnidades('SP', <?=$_SESSION['redefederal']['orgid']?>);" title="São Paulo"/>
						<area shape="rect" coords="337,161,401,178" style="cursor:pointer;" onclick="redefederal_listaUnidades('SE', <?=$_SESSION['redefederal']['orgid']?>);" title="Sergipe"/>
						<area shape="rect" coords="227,130,270,163" style="cursor:pointer;" onclick="redefederal_listaUnidades('TO', <?=$_SESSION['redefederal']['orgid']?>);" title="Tocantins"/>
						<area shape="rect" coords="17,264,85,282"   style="cursor:pointer;" onclick="redefederal_listaUnidades('norte',       <?=$_SESSION['redefederal']['orgid']?>);" title="Norte" />
						<area shape="rect" coords="16,281,94,296"   style="cursor:pointer;" onclick="redefederal_listaUnidades('nordeste',    <?=$_SESSION['redefederal']['orgid']?>);" title="Nordeste" />
						<area shape="rect" coords="15,296,112,312"  style="cursor:pointer;" onclick="redefederal_listaUnidades('centrooeste', <?=$_SESSION['redefederal']['orgid']?>);" title="Centro-Oeste" />
						<area shape="rect" coords="14,312,100,329"  style="cursor:pointer;" onclick="redefederal_listaUnidades('sudeste',     <?=$_SESSION['redefederal']['orgid']?>);" title="Sudeste" />
						<area shape="rect" coords="13,329,68,344"   style="cursor:pointer;" onclick="redefederal_listaUnidades('sul', 		<?=$_SESSION['redefederal']['orgid']?>);" title="Sul" />
					</map>
				</div>
				</fieldset>
			</td>
		</tr>
	</tbody>
</table>