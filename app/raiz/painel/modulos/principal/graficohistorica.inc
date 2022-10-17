<?
ini_set( "memory_limit", "1024M" ); // ...
set_time_limit(0);

!$_REQUEST['indid']? $indid = $_SESSION['indid'] : $indid = $_REQUEST['indid'];

if(!$_SESSION['indid']){
	$_SESSION['indid'] = $indid;
}

//Ajax
if ($_REQUEST['carrega'] && $indid) {
	header('content-type: text/html; charset=ISO-8859-1');
	carregaSerieHistoria($indid,$_REQUEST['pesquisa']);
	exit;
}

if ($_REQUEST['ufidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	carregaMunicipios($_REQUEST['ufidAjax'],$_REQUEST['muncod']);
	exit;
}

if ($_REQUEST['muncodAjaxEscolas']) {
	header('content-type: text/html; charset=ISO-8859-1');
	carregaEscolas($_REQUEST['muncodAjaxEscolas'],$_REQUEST['esccodinep']);
	exit;
}

if ($_REQUEST['muncodAjaxIES']) {
	header('content-type: text/html; charset=ISO-8859-1');
	carregaIES($_REQUEST['muncodAjaxIES'],$_REQUEST['ies']);
	exit;
}



if($_REQUEST['addDetalheSH']){
	header('content-type: text/html; charset=ISO-8859-1');
	addDetalheSH($_REQUEST['indid'],$_POST['sehid'],$_REQUEST['dshid'],$_REQUEST['valor'],$_REQUEST['dshcod'],$_REQUEST['municipio'],$_REQUEST['estado'],$_REQUEST['quantidade'],$_REQUEST['dpeid'],$_REQUEST['detalhe'],$_REQUEST['sehdtcoleta'],$_REQUEST['escola'],$_REQUEST['ies']);
	//parameters: '&addDetalheSH=true' + dshid + '&dshid=' + dshid + '&indid=' + indid + '&sehid=' + sehid + '&quantidade=' + quantidade.value + '&estado=' + estado + '&municipio=' + municipio + '&valor=' + valor + '&dpeid=' + dpeid2 + '&detalhe=' + detalhe + '&sehdtcoleta=' + sehdtcoleta2,
	exit;
}


if($_REQUEST['editarDetalheSH']){
	header('content-type: text/html; charset=ISO-8859-1');
	editarDetalheSH($_REQUEST['indid'],$_REQUEST['sehid'],$_REQUEST['editarDetalheSH']);
	//parameters: '&editarDetalheSH=' + dshid + '&sehid=' + sehid + '&indid=' + indid,
	exit;
}


if($_REQUEST['excluirDetalheSH']){
	header('content-type: text/html; charset=ISO-8859-1');
	excluirDetalheSH($_REQUEST['indid'],$_REQUEST['sehid'],$_REQUEST['excluirDetalheSH']);
	exit;
}

if($_REQUEST['excluirSH']){
	header('content-type: text/html; charset=ISO-8859-1');
	excluirSerieHistorica($_REQUEST['excluirSHindid'],$_REQUEST['excluirSH']);
	exit;
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = "Série Histórica";
monta_titulo( $titulo, '&nbsp;' );

cabecalhoIndicador($indid);
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/calendario.js"></script>
<script>
	function extraiScript(texto){  
		//desenvolvido por Skywalker.to, Micox e Pita.  
		//http://forum.imasters.uol.com.br/index.php?showtopic=165277  
		var ini, pos_src, fim, codigo;  
		var objScript = null;  
		ini = texto.indexOf('<script', 0)  
		while (ini!=-1){  
			var objScript = document.createElement("script");  
			//Busca se tem algum src a partir do inicio do script  
			pos_src = texto.indexOf(' src', ini)  
			ini = texto.indexOf('>', ini) + 1;
	
			//Verifica se este e um bloco de script ou include para um arquivo de scripts  
			if (pos_src < ini && pos_src >=0){//Se encontrou um "src" dentro da tag script, esta e um include de um arquivo script  
				//Marca como sendo o inicio do nome do arquivo para depois do src  
				ini = pos_src + 4;  
				//Procura pelo ponto do nome da extencao do arquivo e marca para depois dele  
				fim = texto.indexOf('.', ini)+4;  
				//Pega o nome do arquivo  
				codigo = texto.substring(ini,fim);  
				//Elimina do nome do arquivo os caracteres que possam ter sido pegos por engano  
				codigo = codigo.replace("=","").replace(" ","").replace("\"","").replace("\"","").replace("\'","").replace("\'","").replace(">","");  
				// Adiciona o arquivo de script ao objeto que sera adicionado ao documento  
				objScript.src = codigo;  
			}else{
			//Se nao encontrou um "src" dentro da tag script, esta e um bloco de codigo script  
				// Procura o final do script
				fim = texto.indexOf('</script', ini);  
				// Extrai apenas o script
				codigo = texto.substring(ini,fim);  
				// Adiciona o bloco de script ao objeto que sera adicionado ao documento  
				objScript.text = codigo;
			}
			
			//Adiciona o script ao documento  
			document.body.appendChild(objScript);
			
			// Procura a proxima tag de <script>  
			ini = texto.indexOf('<script', fim);
			
			//Limpa o objeto de script  
			objScript = null;  
		}  
	} 

	function carregaSerieHistoria(){
		var pesquisa = document.getElementById('pesquisa').value;
		var req = new Ajax.Request('painel.php?modulo=principal/graficohistorica&acao=A', {
				        method:     'post',
				        parameters: '&carrega=true&pesquisa=' + pesquisa,
				        onComplete: function (res)
				        {	
							document.getElementById('SerieHistoria').innerHTML = res.responseText;
				        }
				  });
	}
	
	function mudaMunicipios(ufid,muncod){
		if(!document.getElementsByName('estado')){
			return false;
		}else{
			if(!ufid){
				return false;
			}else{

				var req = new Ajax.Request('painel.php?modulo=principal/graficohistorica&acao=A', {
				        method:     'post',
				        parameters: '&ufidAjax=' + ufid + '&muncod=' + muncod,
				        onComplete: function (res)
				        {	
							document.getElementById('exibe_municipios').innerHTML = res.responseText;
							
				        }
				  });
			}
		}
	}
	
	function mudaEscola(muncod,esccodinep){
		if(!document.getElementById('escola')){
			return false;
		}else{
			if(!muncod){
				return false;
			}else{
				var req = new Ajax.Request('painel.php?modulo=principal/graficohistorica&acao=A', {
				        method:     'post',
				        parameters: '&muncodAjaxEscolas=' + muncod + '&esccodinep=' + esccodinep,
				        onComplete: function (res)
				        {	
							document.getElementById('exibe_escolas').innerHTML = res.responseText;
				        }
				  });
			}
		}
	}
	
	
	function mudaIES(muncod,iesid){
		if(!document.getElementById('ies')){
			return false;
		}else{
			if(!muncod){
				return false;
			}else{

				var req = new Ajax.Request('painel.php?modulo=principal/graficohistorica&acao=A', {
				        method:     'post',
				        parameters: '&muncodAjaxIES=' + muncod + '&ies=' + iesid,
				        onComplete: function (res)
				        {	
							document.getElementById('exibe_ies').innerHTML = res.responseText;
							
				        }
				  });
			}
		}
	}
	
	
	function addDetalheSH(dshid){
		//var dshid = document.getElementById('dshid').value;
		var indid = <?=$indid;?>;
		var sehid = document.getElementById('SHsehid').value;
		var quantidade = document.getElementById('quantidade');
		var detalhe = document.getElementById('detalhe');
		var uf = document.getElementById('estado');
		var mun = document.getElementById('municipio');
		var valor2 = document.getElementById('valor');
		var dpeid = document.getElementById('dpeid');
		var sehdtcoleta = document.getElementById('sehdtcoleta');
		var escola = document.getElementById('escola');
		var ies = document.getElementById('ies');
		
		if(sehdtcoleta){
			if(!sehdtcoleta.value){
					alert('Informe a Data.');
					sehdtcoleta.focus();
					return false;
			}else{
				var sehdtcoleta2 = sehdtcoleta.value;
			}
		}else{
			sehdtcoleta2 = '';
		}
		
		if(dpeid){
			if(!dpeid.value){
					alert('Informe o Período.');
					dpeid.focus();
					return false;
			}else{
				var dpeid2 = dpeid.value;
			}
		}else{
			var dpeid2 = "";
		}
		
		if(uf){
			var estado = uf.value;
			if(!estado){
				alert('Selecione o Estado.');
				uf.focus();
				return false;
			}
		}else{
			var estado = "";
		}
		if(mun){
			var municipio = mun.value;
			if(!municipio){
				alert('Selecione o Município');
				mun.focus();
				return false;
			}
		}else{
			var municipio = "";
		}
		
		if(escola){
			if(!escola.value){
					alert('Selecione a Escola.');
					escola.focus();
					return false;
			}else{
				var escola2 = escola.value;
			}
		}else{
			escola2 = "";
		}
		
		if(ies){
			if(!ies.value){
					alert('Selecione o IES.');
					ies.focus();
					return false;
			}else{
				var ies2 = ies.value;
			}
		}else{
			ies2 = "";
		}
		
		if(!quantidade.value){
				alert('Informe a Quantidade');
				quantidade.focus();
				return false;
		}
		
		if(detalhe){
				if(!detalhe.value){
					alert('Selecione o Detalhe');
					detalhe.focus();
					return false;
				}else{
					detalhe = detalhe.value;
				}
		}
		else{
			var detalhe = '';
		}
			
		if(valor2){
			var valor = valor2.value;
			if(!valor){
				alert('Informe o Valor');
				valor2.focus();
				return false;
			}
		}else{
			var valor = "";
		}
				
		var req = new Ajax.Request('painel.php?modulo=principal/graficohistorica&acao=A', {
				        method:     'post',
				        parameters: '&addDetalheSH=true' + dshid + '&dshid=' + dshid + '&indid=' + indid + '&sehid=' + sehid + '&quantidade=' + quantidade.value + '&estado=' + estado + '&municipio=' + municipio + '&valor=' + valor + '&dpeid=' + dpeid2 + '&detalhe=' + detalhe + '&sehdtcoleta=' + sehdtcoleta2 + '&escola=' + escola2 + '&ies=' + ies2,
				        onComplete: function (res)
				        {	
							document.getElementById('exibeDetalheSH').innerHTML = res.responseText;
							extraiScript(res.responseText);
							
							quantidade.value= '';
							document.getElementById('dshid').value = '';
							if(uf)
								uf.value = '';
							if(mun){
								mun.value = '';
								mun.setAttribute('disabled','disabled');
							}
							if(detalhe){
								document.getElementById('detalhe').value = '';
							}
							if(valor2)
								valor2.value = '';
							if(dpeid)
								dpeid.setAttribute('disabled','disabled');
							if(sehdtcoleta)
								sehdtcoleta.setAttribute('disabled','disabled');
							if(escola){
								escola.value='';
								escola.setAttribute('disabled','disabled');
							}
							if(ies){
								ies.value='';
								ies.setAttribute('disabled','disabled');
							}

				        }
				        	
				  });
	}
	
	function excluirDetalheSH(dshid){
		var indid = <?=$indid;?>;
		var sehid = document.getElementById('SHsehid').value;
		
		if(confirm('Deseja realmente Excluir?')){	
			if(!dshid){
				alert('Operação não Disponível!');
				return false;
			}
				var req = new Ajax.Request('painel.php?modulo=principal/graficohistorica&acao=A', {
						        method:     'post',
						        parameters: '&excluirDetalheSH=' + dshid + '&sehid=' + sehid + '&indid=' + indid,
						        onComplete: function (res)
						        {	
									document.getElementById('exibeDetalheSH').innerHTML = res.responseText;
									
						        }
						  });
		}
	}
	
	function editarDetalheSH(dshid){
		var indid = <?=$indid;?>;
		var sehid = document.getElementById('SHsehid').value;
		var req = new Ajax.Request('painel.php?modulo=principal/graficohistorica&acao=A', {
						        method:     'post',
						        parameters: '&editarDetalheSH=' + dshid + '&sehid=' + sehid + '&indid=' + indid,
						        onComplete: function (res)
						        {	
									document.getElementById('campos').innerHTML = res.responseText;
									extraiScript(res.responseText);
									document.getElementById('referencia').focus();
									document.getElementById('quantidade').focus();
																		
						        }
			});
	}
	
	function excluirSH(sehid){
		var indid = <?=$indid;?>;
		if(confirm('Deseja realmente excluir?')){
			var req = new Ajax.Request('painel.php?modulo=principal/graficohistorica&acao=A', {
						        method:     'post',
						        parameters: '&excluirSH=' + sehid + '&excluirSHindid=' + indid,
						        onComplete: function (res)
						        {	
									document.getElementById('SerieHistoria').innerHTML = res.responseText;
									extraiScript(res.responseText);
									alert('Operação Realizada com Sucesso!');
						        }
			});
		}
	}
	
	function escreveNaDiv(id_div,texto){
		if(document.getElementById(id_div)){
			document.getElementById(id_div).innerHTML += texto;
		}
	}

</script>
<?if(!$_REQUEST['novo'] && !$_REQUEST['sehid']): ?>

	<?php 
	
	$sql = "select
				colid
			from
				painel.indicador
			where 
				indid = $indid";
	
	$colid = $db->pegaUm($sql); 
	if($colid != 2):?>
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td colspan="2" align="right" >
				<? 
				$sql = "select 
						per.perid			
					FROM  
						painel.indicador i
					INNER JOIN	
						painel.periodicidade per ON per.perid = i.perid 
					WHERE 
						i.indid = $indid";
				$perid = $db->pegaUm($sql);
				
				if($perid != 5){
				
					$sql = "select 
									count(dpeid) as count
								from
									painel.detalheperiodicidade
								where
									perid = $perid
								and 
									dpestatus = 'A'
								and
									dpeid not in(
		
												select distinct
														dpe.dpeid
													from
														painel.detalheperiodicidade dpe
													inner join
														painel.seriehistorica seh ON dpe.dpeid = seh.dpeid
													inner join
														painel.indicador ind ON ind.indid= seh.indid
													where
														seh.indid = $indid
												)
								";
						
						$dpeid = $db->pegaUm($sql);
						
						if($dpeid == 0){
							$incluiSH = '<input type="button" value="Incluir Dados" onclick="alert(\'Série Histórica Completa! Operação não Permitida!\')" />';
						}
						else{
							$incluiSH = '<input type="button" value="Incluir Dados" onclick="window.location.href=\'painel.php?modulo=principal/graficohistorica&acao=A&novo=true\';" />';
						}
					}
					else{
						$incluiSH = '<input type="button" value="Incluir Dados" onclick="window.location.href=\'painel.php?modulo=principal/graficohistorica&acao=A&novo=true\';" />';
					}
					
					$arrPflcod = pegaPerfil();
					
					if(in_array(PAINEL_PERFIL_CONSULTA,$arrPflcod)){
						$incluiSH = '<input type="button" value="Incluir Dados" onclick="alert(\'Operação não Permitida!\')" />';
					}
					
					if(in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod) && !$db->testa_superuser()){
						$sql= "SELECT 
									indid
								FROM 
									painel.indicador
								WHERE
									acaid in (
												select 
													distinct acaid
												from
													painel.usuarioresponsabilidade
												where
													usucpf = '{$_SESSION['usucpf']}'
												and
													rpustatus = 'A'
												)";
						$arrIndicadores = $db->carregar($sql);
					}
					if(in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) && !$db->testa_superuser()){
						$sql= "SELECT 
									indid
								FROM 
									painel.indicador
								WHERE
									exoid in (
												select 
													distinct exoid
												from
													painel.usuarioresponsabilidade
												where
													usucpf = '{$_SESSION['usucpf']}'
												and
													rpustatus = 'A'
												)";
						$arrIndicadores = $db->carregar($sql);
					}
					
					if($arrIndicadores){
						foreach($arrIndicadores as $Ind){
							$arrInd[] = $Ind['indid'];
						}
					}else{
						$arrIndicadores = array();
					}
					
					if((in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) || in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod)) && !in_array($indid,$arrInd)){
						$incluiSH = '<input type="button" value="Incluir Dados" onclick="alert(\'Operação não Permitida!\')" />';
					}
				?>
				
				
				<?=$incluiSH?>
				</td>
			</tr>
		</table>
	<?endif; ?>
	<div id="SerieHistoria">
	<? carregaSerieHistoria($indid,$dsc); ?>
	</div>
<? endif; ?>
<?if($_REQUEST['novo'] || $_REQUEST['sehid']): ?>
	<? formularioSerieHistorica($indid,$_REQUEST['sehid']); ?>
<? endif; ?>
<div id="erro"></div>