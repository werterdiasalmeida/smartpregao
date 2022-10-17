<?
//Ajax
if ($_REQUEST['filtraAcaoAjax'] && $_REQUEST['secid']) {
	header('content-type: text/html; charset=ISO-8859-1');
	filtraAcao($_REQUEST['secid']);
	exit;
}

// verificando a validade do indicador
if(is_numeric($_REQUEST['indid'])) {
	$indid = $db->pegaUm("SELECT indid FROM painel.indicador WHERE indid='".$_REQUEST['indid']."'");
	// testando se existe
	if($indid) {
		$_SESSION['indid'] = $indid;
	} else {
		echo "<script>alert('Indicador informado não existe');window.location='?modulo=inicio&acao=C';</script>";
		exit;
	}
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// verificando se é um indicador novo
if($_REQUEST['indid'] == 'novoIndicador') {
	// carregando o menu
	$menu[0] = array("descricao" => "Lista de Indicadores", "link"=> "/painel/painel.php?modulo=inicio&acao=C");
	$menu[1] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=".$_REQUEST['indid']."&evento=".$_REQUEST['evento']."");
	// se for super usuário não precisa mostrar quais os indicadores, pois tem acesso a todos
	if(!$db->testa_superuser()) {
		$menu[2] = array("descricao" => "Meus indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&evento=M");
	}
	echo montarAbasArray($menu, "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=".$_REQUEST['indid']."&evento=".$_REQUEST['evento']."");
	$acesso = true;
	unset($_SESSION['indid']);
}

if($_SESSION['indid']):
	$db->cria_aba($abacod_tela,$url,'');
	!$_SESSION['indid']? $_SESSION['indid'] = $_REQUEST['indid']: $_SESSION['indid'] = $_SESSION['indid'];
	
	/*
	 * VALIDANDO SE USUÁRIO POSSUI PERMISSÃO PARA ACESSAR
	 * O INDICADOR
	 */
	$permissoes = verificaPerfilPainel();
	if($permissoes['verindicadores'] == 'vertodos') {
		$acesso = true;
	} else {
		$acesso = validaAcessoIndicadores($permissoes['verindicadores'], $_SESSION['indid']);
	}
	/*
	 * FIM
	 * VALIDANDO SE USUÁRIO POSSUI PERMISSÃO PARA ACESSAR
	 * O INDICADOR
	 */
endif;


$titulo = "Cadastro de Indicador";
monta_titulo( $titulo, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );

if($_SESSION['indid'] && !$_REQUEST['evento']){
	$_REQUEST['evento'] = 'A';
}

if( (isset($_REQUEST[evento]) && ($_REQUEST[evento] != '')) || $_SESSION['indid']){
	
	if($_SESSION['indid']){
		$_REQUEST['indid'] = $_SESSION['indid'];
	}
	switch($_REQUEST[evento]) {
		
		case 'I':	
						
				$indid = $_REQUEST['indid']; 			
				
				$exoid 				= $_REQUEST['exoid'];
				$unmid 				= $_REQUEST['unmid']; 
				$cliid 				= $_REQUEST['cliid'];
				$tpiid 				= $_REQUEST['tpiid'];  
				$secid  			= $_REQUEST['secid'];  
				$secidgestora		= $_REQUEST['secidgestora'];
				$acaid   			= $_REQUEST['acaid']; 
				$perid  			= $_REQUEST['perid'];  
				$estid  			= $_REQUEST['estid'];
				$regid				= $_REQUEST['regid'];
				$umeid				= $_REQUEST['umeid'];
				$colid  			= $_REQUEST['colid'];  
				$usucpf  			= $_REQUEST['usucpf'];  
				$indnome  			= $_REQUEST['indnome'];  
				$indobjetivo  		= $_REQUEST['indobjetivo']; 
				$indformula  		= $_REQUEST['indformula'];  
				$indtermos  		= $_REQUEST['indtermos'];  
				$indfontetermo  	= $_REQUEST['indfontetermo'];  
				$indnormalinicio  	= str_replace(array(".",","), array("","."), $_REQUEST['indnormalinicio']);  
				$indnormalfinal  	= str_replace(array(".",","), array("","."), $_REQUEST['indnormalfinal']);  
				$indatencaoinicio  	= str_replace(array(".",","), array("","."), $_REQUEST['indatencaoinicio']); 
				$indatencaofinal  	= str_replace(array(".",","), array("","."), $_REQUEST['indatencaofinal']);  
				$indcriticoinicio  	= str_replace(array(".",","), array("","."), $_REQUEST['indcriticoinicio']);  
				$indcriticofinal  	= str_replace(array(".",","), array("","."), $_REQUEST['indcriticofinal']);  
				$indmetavalor 		= str_replace(array(".",","), array("","."), $_REQUEST['indmetavalor']);
				$indmetadatalimite	= $_REQUEST['indmetadatalimite'];
				$indqtdevalor		= $_REQUEST['indqtdevalor'];
				$indcumulativo		= $_REQUEST['indcumulativo'];
				$indpublicado		= $_REQUEST['indpublicado'];
				$indobservacao		= $_REQUEST['indobservacao'];
				$indescala			= $_REQUEST['indescala'];
				
			if($indid){						
				
				$sql = "
				SELECT 
					indid				
				FROM  painel.indicador i
				WHERE 
				i.indid = ".$indid."
				";					
				$dados = $db->pegaUm($sql);
			}
			
			//Campos Não obrigatorios
				(!$cliid)? $cliid = 'null' : $cliid = $cliid;
				($umeid == "")? $umeid = 'null' : $umeid = $umeid;
				($regid == "")? $regid = 'null' : $regid = $regid;
				($tpiid == "")? $tpiid = '1' : $tpiid = $tpiid;
				($indnormalinicio == "")?	$indnormalinicio 	= "null" : $indnormalinicio 	= $indnormalinicio;
				($indnormalfinal == "")? 	$indnormalfinal 	= "null" : $indnormalfinal 		= $indnormalfinal;
				($indatencaoinicio == "")? 	$indatencaoinicio 	= "null" : $indatencaoinicio 	= $indatencaoinicio;
				($indatencaofinal == "")? 	$indatencaofinal 	= "null" : $indatencaofinal 	= $indatencaofinal;
				($indcriticoinicio == "")? 	$indcriticoinicio 	= "null" : $indcriticoinicio 	= $indcriticoinicio;
				($indcriticofinal == "")? 	$indcriticofinal 	= "null" : $indcriticofinal 	= $indcriticofinal;
				
				($indmetavalor == "")? 		$indmetavalor 		= "null" : $indmetavalor	 	= $indmetavalor;
				
				($secidgestora == "")? 		$secidgestora 		= "null" : $secidgestora	 	= $secidgestora;
				
				//($secidgestora == "")? 		$secidgestora 		= "null" : $secidgestora	 	= $secidgestora;
				
				
				$usucpf = $_SESSION['usucpf'];
			
			// caso não exista o indicador insere um novo
			if(!$dados){	
				$sql_I = "INSERT 
						  INTO painel.indicador
							  (unmid, cliid, tpiid, secid, secidgestora,acaid, perid, estid, colid, usucpf, indnome, indobjetivo,
							  indformula, indtermos, indfontetermo, indnormalinicio, indnormalfinal, indatencaoinicio,
							  indatencaofinal, indcriticoinicio, indcriticofinal, indmetavalor, indmetadatalimite,exoid,regid,umeid,indqtdevalor,indcumulativo,indpublicado,indescala,indobservacao)
						  VALUES 
						  	   (".$unmid.", ".$cliid.", ".$tpiid.", ".$secid.", ".$secidgestora.", ".$acaid.", ".$perid.", ".$estid.", ".$colid.", '".$usucpf."', '".$indnome."', '".$indobjetivo."',
							  '".$indformula."', '".$indtermos."', '".$indfontetermo."', ".$indnormalinicio.", ".$indnormalfinal.", ".$indatencaoinicio.",
							  ".$indatencaofinal.", ".$indcriticoinicio.", ".$indcriticofinal.", ".$indmetavalor.", '".$indmetadatalimite."','".$exoid."',$regid,$umeid,$indqtdevalor,$indcumulativo,$indpublicado,$indescala,'$indobservacao')
						  RETURNING	indid
						  ";
			
				$indid = $db->pegaUm($sql_I);
				
				$_REQUEST['obeid'] = array();
				
				foreach($_REQUEST['obeid'] as $obeid) {
					$sql = "INSERT INTO painel.indicadorobjetivo(obeid, indid) VALUES (". $obeid .", ". $indid .")";			
					$db->executar($sql);
				}	
				
			//caso exista o indicador altera os dados do mesmo	
			} else {
			
			
				$sql_U = "
					UPDATE painel.indicador
					SET
					 unmid = 				".$unmid.",
					 exoid = 				".$exoid.",
					 cliid = 				".$cliid.",
					 tpiid = 				".$tpiid.",
					 estid = 				".$estid.",
					 secid = 				".$secid.",
					 secidgestora =			".$secidgestora.",
					 acaid = 				".$acaid.",
					 perid = 				".$perid.",
					 colid = 				".$colid.",
					 usucpf = 				'".$usucpf."',
					 indnome = 				'".$indnome."',
					 indobjetivo = 			'".$indobjetivo."',
					 indformula = 			'".$indformula."',
					 indtermos = 			'".$indtermos."',
					 indfontetermo = 		'".$indfontetermo."',
					 indnormalinicio = 		".$indnormalinicio.",
					 indnormalfinal = 		".$indnormalfinal.",
					 indatencaoinicio = 	".$indatencaoinicio.",
					 indatencaofinal = 		 ".$indatencaofinal.",	
					 indcriticoinicio = 	".$indcriticoinicio.",
					 indcriticofinal = 		".$indcriticofinal.",
					 indmetavalor = 		".$indmetavalor.",
					 regid	= 				".$regid.",
					 umeid	= 				".$umeid.",
					 indmetadatalimite = 	'".$indmetadatalimite."',
					 indqtdevalor = 		'".$indqtdevalor."',
					 indcumulativo = 		'".$indcumulativo."',	
					 indpublicado = 		".($indpublicado=='' ? 'indpublicado' : "'".$indpublicado."'").",
					 indescala = 			'".$indescala."',
					 indobservacao = 		'".$indobservacao."' 
					WHERE indid = 			".$indid."	
				";
				$db->executar($sql_U);
				// se houver mudança na periodicidade ou regionalização, desabilitar as series historica
				if($_REQUEST['perid'] != $_REQUEST['perid_conf'] || $_REQUEST['regid'] != $_REQUEST['regid_conf']) {
					$sql = "UPDATE painel.seriehistorica SET sehstatus='I' WHERE indid='".$_REQUEST['indid']."'";
					$db->executar($sql);					
				}
				
				$sql = "DELETE FROM painel.indicadorobjetivo WHERE indid =  ". $indid ."";			
				$db->executar($sql);		
				
				$_POST['obeid'] = array();
				
				foreach($_POST['obeid'] as $obeid) {
					$sql = "INSERT INTO painel.indicadorobjetivo(obeid, indid) VALUES (". $obeid .", ". $indid .")";			
					$db->executar($sql);
				}
			}
			$db->commit();
			echo "<script>
					alert('Dados salvos com sucesso.');
					window.location = '?modulo=principal/cadastro&acao=A&indid=$indid&evento=A';
				  </script>";
		exit;
		break;
		
		case 'A': 
			$indid = $_REQUEST['indid']; 			
			$sql = "
				SELECT 
					unmid, indcumulativo, indpublicado, indobservacao, indescala, indqtdevalor, exoid, regid, umeid, cliid, tpiid, secid, secidgestora, acaid, perid, estid, colid, usucpf, indnome, indobjetivo,
					indformula, indtermos, indfontetermo, 
					TRIM (TO_CHAR(indnormalinicio , '999G999G999D99')) as indnormalinicio, 
					TRIM (TO_CHAR(indnormalfinal , '999G999G999D99')) as indnormalfinal, 
					TRIM (TO_CHAR(indatencaoinicio , '999G999G999D99')) as indatencaoinicio,
					TRIM (TO_CHAR(indatencaofinal , '999G999G999D99')) as indatencaofinal, 
					TRIM (TO_CHAR(indcriticoinicio , '999G999G999D99')) as indcriticoinicio, 
					TRIM (TO_CHAR(indcriticofinal , '999G999G999D99')) as indcriticofinal, 
					TRIM (TO_CHAR(indmetavalor , '999G999G999D99')) as indmetavalor, 
					indmetadatalimite				
				FROM  painel.indicador i
				WHERE 
				i.indid = ".$indid."
				";	
			$dados = $db->pegaLinha($sql);
			
			$sql_obj = "SELECT 
							o.obeid AS codigo,
							o.obenome AS descricao				
						FROM 
							painel.objetivoestrategico o
						INNER JOIN  
							painel.indicadorobjetivo io On io.obeid = o.obeid  AND io.indid = ".$indid." 
						WHERE
							obestatus = 'A' ";
			
			$obeid = $db->carregar($sql_obj);			
			
			if($dados){
				$exoid 				= $dados['exoid'];
				$unmid 				= $dados['unmid'];				 
				$cliid 				= $dados['cliid'];
				$tpiid 				= $dados['tpiid'];  
				$secid  			= $dados['secid'];
				$secidgestora		= $dados['secidgestora']; 
				$acaid   			= $dados['acaid']; 
				$perid  			= $dados['perid'];
				$regid				= $dados['regid'];
				$umeid				= $dados['umeid'];  
				$estid  			= $dados['estid'];  
				$colid  			= $dados['colid'];  
				$usucpf  			= $dados['usucpf'];  
				$indnome  			= $dados['indnome'];  
				$indobjetivo  		= $dados['indobjetivo']; 
				$indformula  		= $dados['indformula'];  
				$indtermos  		= $dados['indtermos'];  
				$indfontetermo  	= $dados['indfontetermo'];  
				$indnormalinicio  	= $dados['indnormalinicio'];  
				$indnormalfinal  	= $dados['indnormalfinal'];  
				$indatencaoinicio  	= $dados['indatencaoinicio']; 
				$indatencaofinal  	= $dados['indatencaofinal'];  
				$indcriticoinicio  	= $dados['indcriticoinicio'];  
				$indcriticofinal  	= $dados['indcriticofinal'];  
				$indmetavalor  		= $dados['indmetavalor']; 
				$indmetadatalimite 	= $dados['indmetadatalimite'];
				$indqtdevalor		= $dados['indqtdevalor'];
				$indcumulativo 		= $dados['indcumulativo'];
				$indpublicado 		= $dados['indpublicado'];
				$indescala 			= $dados['indescala'];
				$indobservacao		= $dados['indobservacao'];
			}		
		break;	
	}
}
?>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script>
function validaUnidadeMedicao(valor) {
	switch(valor) {
		case '3':
			document.getElementById('coletarvalormonetario').style.display = '';
			document.getElementById('indicarEscala').style.display = '';
			break;
		case '5':
			document.getElementById('coletarvalormonetario').style.display = 'none';
			document.formulario.indqtdevalor[1].checked=true;
			document.getElementById('indicarEscala').style.display = 'none';
			document.formulario.indescala[1].checked=true;
			break;
		default:
			document.formulario.indqtdevalor[0].checked=true;
			document.getElementById('coletarvalormonetario').style.display = 'none';
			document.formulario.indescala[0].checked=true;
			document.getElementById('indicarEscala').style.display = 'none';
	}
}
</script>
<form method="POST"  name="formulario">
<input type="hidden" name="evento" id="evento" value="I">
<input type="hidden" name="indid" id="indid" value="<?=$indid?>">
<table style="border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2">Dados gerais do indicador</td>
</tr>
<tr>
	<?php
	//Verifica se o usuário é do tipo Super Usuário, se não for, só exibe o Eixo que possui permissão.
	$arrPflcod = pegaPerfil();
			
	if(!in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) || $db->testa_superuser() || in_array(PAINEL_PERFIL_ATUALIZA_SH,$arrPflcod) || in_array(PAINEL_PERFIL_CONSULTA,$arrPflcod)){
	
		$sql= "SELECT 
			exodsc AS descricao, 
			exoid AS codigo
		FROM 
			painel.eixo
		order by
			 descricao";
	}else{
		$sql= "SELECT 
			exodsc AS descricao, 
			exoid AS codigo
		FROM 
			painel.eixo
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
						)
		order by
			 descricao";
	}
	
	$arrEixos = $db->carregar($sql);
	
	!$arrEixos? $arrEixos = array() : $arrEixos = $arrEixos;
	
	foreach($arrEixos as $eixos){
		$arrExoid[] = $eixos['codigo'];
	}
	
	if($arrEixos && $exoid && in_array(PAINEL_PERFIL_ADM_EIXO,$arrPflcod) && !in_array($exoid,$arrExoid)){
		$sql= "SELECT 
			exodsc AS descricao, 
			exoid AS codigo
		FROM 
			painel.eixo
		order by
			 descricao
		";
		
		$display_botoes = "none";
		
	}
	
	
	?>
    <td align='right' class="SubTituloDireita">Eixo:</td>
    <td><?=$db->monta_combo('exoid',$sql,(($acesso)?'S':'N'),'Selecione...',"",'','','','S','','',$exoid);?></td>
</tr>
<tr>
	<?php 
		$sql= "SELECT 
			secdsc AS descricao, 
			secid AS codigo
		FROM 		
			painel.secretaria
		WHERE
			secstatus = 'A'
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Secretaria/Autarquia Executora:</td>
    <td><?=$db->monta_combo('secid',$sql,(($acesso)?'S':'N'),'Selecione...',"filtraAcao",'','','','S','','',$secid);?></td>
</tr> 

<tr style="display: none" >
	<?php 
		$sql= "SELECT 
			u.usunome AS descricao, 
			u.usucpf AS codigo
			FROM 		
			seguranca.usuario u	
			INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf  
			WHERE us.sisid = ".$_SESSION['sisid']."		
		";
	?>
    <td align='right' class="SubTituloDireita">Responsável - Usuário:</td>
    <td><?=$db->monta_combo('usucpf',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?></td>
</tr> 
<tr>
	<?php 
		//Verifica se o usuário é do tipo Super Usuário, se não for, só exibe as Ações permitidas.
		if(!in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod) || $db->testa_superuser()  || in_array(PAINEL_PERFIL_ATUALIZA_SH,$arrPflcod) || in_array(PAINEL_PERFIL_CONSULTA,$arrPflcod)  ){
		
			$sql= "SELECT 
						acadsc AS descricao, 
						acaid AS codigo
					FROM 		
						painel.acao
					WHERE
						acastatus = 'A'
					order by
						 descricao";
		}else{
			$sql= "SELECT 
				acadsc AS descricao, 
				acaid AS codigo
			FROM 
				painel.acao
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
							)
				and acastatus = 'A'	
			order by
				 descricao";
		}

		$arrAcoes = $db->carregar($sql);
		
		!$arrAcoes? $arrAcoes = array() : $arrAcoes = $arrAcoes;
	
		foreach($arrAcoes as $acoes){
			$arrAcaid[] = $acoes['codigo'];
		}
		
		if($arrAcoes && $acaid && in_array(PAINEL_PERFIL_ADM_ACAO,$arrPflcod) && !in_array($acaid,$arrAcaid)){
			$sql= "SELECT 
						acadsc AS descricao, 
						acaid AS codigo
					FROM 		
						painel.acao
					WHERE
						acastatus = 'A'
					order by
						 descricao";
			
			$display_botoes = "none";
			
		}

	?>
    <td align='right' class="SubTituloDireita">Ação:</td>
    <td id="td_acao" ><?=$db->monta_combo('acaid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S','','',$acaid);?></td>
</tr>
<tr>
	<?php 
		$sql= "SELECT 
			secdsc AS descricao, 
			secid AS codigo
		FROM 		
			painel.secretaria
		WHERE
			secstatus = 'A'
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Secretaria/Autarquia Gestora:</td>
    <td><?=$db->monta_combo('secidgestora',$sql,(($acesso)?'S':'N'),'Selecione...',"",'','','','S','','',$secidgestora);?></td>
</tr> 
<tr style='display:none'>
	<?php 
		$sql= "SELECT 
			tpidsc AS descricao, 
			tpiid AS codigo
		FROM 		
			painel.tipoindicador
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Tipo Indicador:</td>
    <td><?=$db->monta_combo('tpiid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?></td>
</tr>
<tr>
	<td width="25%" align='right' class="SubTituloDireita">Nome Indicador:</td>
    <td><?=campo_texto('indnome','S',(($acesso)?'S':'N'),'',100,200,'','');?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Objetivo Indicador:</td>
    <td><?=campo_textarea( 'indobjetivo', 'S', (($acesso)?'S':'N'), '', 60, 2, 200 );?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Observação:</td>
    <td><?=campo_textarea( 'indobservacao', 'N', (($acesso)?'S':'N'), '', 98, 5, 2000 );?></td>
</tr> 
<tr>
<tr>
	<td class="SubTituloEsquerda" colspan="2">Dados de exibição</td>
</tr>
<tr>
	<?php 
		$sql= "SELECT 
			unmdesc AS descricao, 
			unmid AS codigo
		FROM 
			painel.unidademedicao
		order by
			 descricao
		";
	?>
    <td align='right' class="SubTituloDireita">Unidade de Medição:</td>
    <td><?=$db->monta_combo('unmid',$sql,(($acesso)?'S':'N'),'Selecione...','validaUnidadeMedicao','','','','S');?></td>
</tr>  
<tr>
	<?php 
		$sql= "SELECT 
			umedesc AS descricao, 
			umeid AS codigo
		FROM 		
			painel.unidademeta	
		where 
			umestatus = 'A'
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Exibir produto da unidade como:</td>
    <td><?=$db->monta_combo('umeid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?></td>
</tr>

<tr style="display: none" >
	<?php 
		$sql= "SELECT 
			clidsc AS descricao, 
			cliid AS codigo
		FROM 		
			painel.classeindicador
		order by
			 descricao		 
		";
	?>
    <td align='right' class="SubTituloDireita">Classe do Indicador:</td>
    <td><?=$db->monta_combo('cliid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','N');?></td>
</tr>
</tr>   
<tr style="display: none" id="coletarvalormonetario">
	<?php
		$checkNao = "";
		$checkSim = "";
		$indqtdevalor != "t" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
	?>
    <td align='right' class="SubTituloDireita">Coletar valor monetário?</td>
    <td><input type="radio" value="false" <?=$checkNao?> name="indqtdevalor" />Não <input type="radio" <?=$checkSim?> value="true" name="indqtdevalor" />Sim</td>
</tr>
<tr style="display: none" id="indicarEscala">
<?php
//Pergunta para a escala
$checkNaoEsc = "";
$checkSimEsc = "";
$indescala != "t" ? $checkNaoEsc = "checked=checked" : $checkSimEsc = "checked=checked";

?>
    <td align='right' class="SubTituloDireita">Utilizar escala de exibição?</td>
    <td><input type="radio" value="false" <?=$checkNaoEsc?> name="indescala" />Não <input type="radio" <?=$checkSimEsc?> value="true" name="indescala" />Sim</td>
</tr>

<tr>
	<?php
		$checkNaoAc = "";
		$checkSimAc = "";
		$indcumulativo != "t" ? $checkNaoAc = "checked=checked" : $checkSimAc = "checked=checked";
	?>
    <td align='right' class="SubTituloDireita">Exibir dados acumulados entre os períodos?</td>
    <td><input type="radio" value="false" <?=$checkNaoAc?> name="indcumulativo" />Não <input type="radio" <?=$checkSimAc?> value="true" name="indcumulativo" />Sim</td>
</tr>
<script>
validaUnidadeMedicao('<? echo $unmid; ?>');
</script>
	<?php
//Testa se é Super Usuário ou Gestor PDE ou Equipe de apoio ao Gestor PDE
$sql = "select count(*)
		from seguranca.perfilusuario pu, seguranca.perfil p 
		where p.pflcod = pu.pflcod and 
			  pu.usucpf = '{$_SESSION['usucpf']}' and 
			  p.sisid = {$_SESSION['sisid']} and
			  pflstatus = 'A' and
			  pu.pflcod in (349,272,271)";
			  
$permite = $db->pegaUm($sql);
		$checkNaoPub = "";
		$checkSimPub = "";
		$indpublicado != "t" ? $checkNaoPub = "checked=checked" : $checkSimPub = "checked=checked";
		!$indpublicado ? $checkSimPub = "checked=checked" : $checkNaoPub = "checked=checked";

if($permite>0 && (in_array(PAINEL_PERFIL_SUPER_USUARIO,$arrPflcod) || in_array(GESTOR_PDE,$arrPflcod) || in_array(EQUIPE_APOIO_GESTOR_PDE,$arrPflcod))){
	?>
<tr>	
    <td align='right' class="SubTituloDireita">Publicado?</td>
    <td><input type="radio" value="false" <?=$checkNaoPub?> name="indpublicado" />Não <input type="radio" <?=$checkSimPub?> value="true" name="indpublicado" />Sim</td>
</tr>
<?php }
else {
if($indpublicado=='')
	$indpublicado = 'true';
	echo "<input type='hidden' name='indpublicado' value='$indpublicado'>";
}

?>
</table>

<table style="border-top:0px;border-top:0px;border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloEsquerda" colspan="2">Dados sobre a coleta do indicador</td>
</tr>
<tr style="display:none" >
	<?php 
		$sql= "SELECT 
			pesdsc AS descricao, 
			pesid AS codigo
		FROM 		
			painel.perspectiva
		order by
			 descricao
		";
	?>
    <td align='right' class="SubTituloDireita">Perspectiva:</td>
    <td><?=$db->monta_combo('pesid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?></td>
</tr>
<tr style="display:none" >
	<?php 
		$sql= "SELECT 
			temdsc AS descricao, 
			temid AS codigo
		FROM 		
			painel.tema
		order by
			 descricao 
		";
	?>
    <td width="25%" align='right' class="SubTituloDireita">Elemento Estratégico:</td>
    <td><?=$db->monta_combo('temid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?></td>
</tr>  
<tr>
    <td align='right' class="SubTituloDireita">Fórmula:</td>
    <td><?=campo_textarea( 'indformula', 'N', (($acesso)?'S':'N'), '', 98, 5, 1000 );?></td>
</tr> 
<tr>
    <td align='right' class="SubTituloDireita">Termos:</td>
    <td><?= campo_textarea( 'indtermos', 'N', (($acesso)?'S':'N'), '', 98, 5, 1000 );?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Fontes por Termo:</td>
    <td><?= campo_textarea( 'indfontetermo', 'N', (($acesso)?'S':'N'), '', 98, 5, 1000 );?></td>
</tr>
<tr>
	<?php 
		$sql= "SELECT 
			perdsc AS descricao, 
			perid AS codigo
		FROM 
			painel.periodicidade
		WHERE
			perstatus = 'A'	
		order by
			 descricao		 
		";
	?>
    <td width="25%" align='right' class="SubTituloDireita">Periodicidade:</td>
    <td><?=$db->monta_combo('perid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?> <input type="hidden" name="perid_conf" id="perid_conf" value="<? echo $perid; ?>"></td>
</tr> 
<tr>
	<?php 
		$sql= "SELECT 
			estdsc AS descricao, 
			estid AS codigo
		FROM 		
			painel.estilo
		order by
			 descricao	 
		";
	?>
    <td align='right' class="SubTituloDireita">Estilo:</td>
    <td><?=$db->monta_combo('estid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S','esdid','',$estid);?></td>
</tr> 
<tr>
	<?php 
		$sql= "SELECT 
			coldsc AS descricao, 
			colid AS codigo
		FROM 		
			painel.coleta
		order by
			 descricao		 
		";
	?>
    <td align='right' class="SubTituloDireita">Coleta:</td>
    <td><?=$db->monta_combo('colid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?></td>
</tr>
<tr>
	<?php 
		$sql= "SELECT 
			regdescricao AS descricao, 
			regid AS codigo
		FROM 		
			painel.regionalizacao
		order by
			 descricao		 
		";
	?>
    <td align='right' class="SubTituloDireita">Regionalização:</td>
    <td><?=$db->monta_combo('regid',$sql,(($acesso)?'S':'N'),'Selecione...','','','','','S');?> <input type="hidden" name="regid_conf" id="regid_conf" value="<? echo $regid; ?>"></td>
</tr>
<tr style="display: none" >
	<td align='right' class="SubTituloDireita">Objetivo Estratégico:</td>
    <td>
    	<?php
    	
    	$sqlobj= " SELECT
						obeid AS codigo,
						obenome AS descricao
					FROM 
						painel.objetivoestrategico
					WHERE
						obestatus = 'A'
					order by
			 			descricao
					";
		combo_popup( "obeid", $sqlobj, "Objetivos Estratégicos", "192x400", 0, array(), "", "S", false, false, 5, 400 );
		?>
	</td>
</tr>
</table>
<table style="display:none" style="border-top:0px;border-bottom:0px;"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td colspan="3" style="text-align: left;font-weight: bold;" bgcolor="#DCDCDC" >Informe as faixas para os níveis de medição abaixo:</td>
</tr>
<tr>
	<td width="10%" rowspan="2" align="center"  bgcolor="#DCDCDC"><b>Normal</b></td>
	<td width="15%" align='right' class="SubTituloDireita"> Início:</td>
    <td><?=campo_texto('indnormalinicio','S','S','',20,'','###.###.###,##','');?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita"> Final:</td>
    <td><?=campo_texto('indnormalfinal','S','S','',20,'','###.###.###,##','');?></td>
</tr>
<tr>
	<td rowspan="2" align="center"  bgcolor="#DCDCDC"><b>Atenção</b></td>
	<td align='right' class="SubTituloDireita">Início:</td>
    <td><?=campo_texto('indatencaoinicio','S','S','',20,'','###.###.###,##','');?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Final:</td>
    <td><?=campo_texto('indatencaofinal','S','S','',20,'','###.###.###,##','');?></td>
</tr>
<tr>
	<td rowspan="2" align="center"  bgcolor="#DCDCDC"><b>Crítico</b></td>
	<td align='right' class="SubTituloDireita">Início:</td>
    <td><?=campo_texto('indcriticoinicio','S','S','',20,'','###.###.###,##','');?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Final:</td>
    <td><?=campo_texto('indcriticofinal','S','S','',20,'','###.###.###,##','');?></td>
</tr>
</table>
<?php if($display_botoes != "none" && !in_array(PAINEL_PERFIL_ATUALIZA_SH,$arrPflcod) && !in_array(PAINEL_PERFIL_CONSULTA,$arrPflcod)){ ?>
	<table style="border-top:0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#cccccc">
		      <td style="text-align: center" >
		      <input type="button" class="botao" name="btassociar" value="Gravar" onclick="submeter('I', '<?=$indid?>');">
		  	  	<input type="button" class="botao" name="btassociar" value="Novo Indicador" onclick="novo();">	  	  
		  	  	<input type="button" class="botao" name="btassociar" value="Limpar" onclick="limpar();">	  	  	
		  	  </td>
		  	  
		      </tr>
	</table>
<? } ?>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>	
<script type="text/javascript">

function submeter(evento, indid){	
	if(validar()){
		selectAllOptions( document.getElementById( 'obeid' ) );
		document.getElementById('evento').value = evento;
		document.getElementById('indid').value = indid;	
		document.formulario.submit();	
	}
}


function filtraAcao(secid) {
	if(!secid){
		return false;
	}
	var destino = document.getElementById("td_acao");
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: "filtraAcaoAjax=true&" + "secid=" + secid,
			asynchronous: false,
			onComplete: function(resp) {
				if(destino) {
					destino.innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				destino.innerHTML = 'Carregando...';
			}
		});
}

function novo(){
	window.location.href = "?modulo=principal/cadastro&acao=A&novoIndicador=true";		
}

function limpar(){	
	document.getElementById('evento').value = '';	
	document.formulario.unmid.value = '';		
	document.formulario.cliid.value = '';
	document.formulario.tpiid.value = '';
	document.formulario.acaid.value = '';
	document.formulario.perid.value = '';
	document.formulario.estid.value = '';
	document.formulario.colid.value = '';
	document.formulario.usucpf.value = '';
	document.formulario.secid.value = '';
	document.formulario.secidgestora.value = '';
	document.formulario.indnome.value = '';
	document.formulario.indobjetivo.value = '';
	document.formulario.indformula.value = '';
	document.formulario.indtermos.value = ''; 
	document.formulario.indfontetermo.value = '';
	document.formulario.indnormalinicio.value = '';
	document.formulario.indobservacao.value = '';
	document.formulario.indnormalfinal.value = '';
	document.formulario.indatencaoinicio.value = '';
	document.formulario.indatencaofinal.value = '';
	document.formulario.indcriticoinicio.value = '';
	document.formulario.indcriticofinal.value = '';
	document.formulario.indmetavalor.value = '';
}

function validar(){	
	var msg = "";	
	if(document.formulario.unmid.value == ''){
		msg+="O preenchimento do campo \"Unidade de Medição\" é obrigatório.\n";
	}
	if(document.formulario.exoid.value == ''){
		msg+="O preenchimento do campo \"Eixo\" é obrigatório.\n";
	}
	if(document.formulario.acaid.value == ''){
		msg+="O preenchimento do campo \"Ação\" é obrigatório.\n";
	}
	if(document.formulario.perid.value == ''){
		msg+="O preenchimento do campo \"Periodicidade\" é obrigatório.\n";
	}
	if(document.formulario.estid.value == ''){
		msg+="O preenchimento do campo \"Estilo\" é obrigatório.\n";
	}
	if(document.formulario.colid.value == ''){
		msg+="O preenchimento do campo \"Coleta\" é obrigatório.\n";
	}
	if(document.formulario.secid.value == ''){
		msg+="O preenchimento do campo \"Secretaria/Autarquia Executora\" é obrigatório.\n";
	}
	if(document.formulario.indnome.value == ''){
		msg+="O preenchimento do campo \"Nome Indicador\" é obrigatório.\n";
	}
	if(document.formulario.indobjetivo.value == ''){
		msg+="O preenchimento do campo \"Objetivo Indicador\" é obrigatório.\n";
	}
	if(document.formulario.umeid.value == ''){
		msg+="O preenchimento do campo \"Unidade Meta\" é obrigatório.\n";
	}
	if(document.formulario.regid.value == ''){
		msg+="O preenchimento do campo \"Regionalização\" é obrigatório.\n";
	}

	if(msg != "") {
		alert(msg);
		return false;
	} else {
		if(document.formulario.perid.value != document.formulario.perid_conf.value || document.formulario.regid.value != document.formulario.regid_conf.value) {
			var conf = confirm("Houve a mudança nos dados Periodicidade e/ou Regionalização, assim todos os dados cadastrados ate o momento serão desabilitados. Deseja continuar?")
			if(conf) {
				return true;			
			} else {
				return false;
			}
		} else {
			return true;
		}
	}	
}
</script>