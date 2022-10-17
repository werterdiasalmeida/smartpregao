<?php 
include_once APPRAIZ.'includes/classes/modelo/questionario/QPergunta.class.inc';
include_once APPRAIZ.'includes/classes/modelo/questionario/QItemPergunta.class.inc';

$obPergunta = new QPergunta( $_REQUEST['perid'] );

if($_POST['submeter'] == 'salvar'){	
	
	if($_POST['queid'] <> ''){
		$arCampos = array( "perid", "queid", "pertipo", "pertitulo", "perordem", "perajuda", "perobrigatorio", "permascara", "permultipla" );
		$where = "queid = {$_POST['queid']}";	
	} else if ($_POST['grpid'] <> ''){
		$arCampos = array( "perid", "grpid", "pertipo", "pertitulo", "perordem", "perajuda", "perobrigatorio", "permascara", "permultipla" );
		$where = "grpid = {$_POST['grpid']}";	
	} else if ($_POST['itpid'] <> ''){
		$arCampos = array( "perid", "itpid", "pertipo", "pertitulo", "perordem", "perajuda", "perobrigatorio", "permascara", "permultipla" );
		$where = "itpid = {$_POST['itpid']}";	
	} else if ($_POST['grpid'] <> '' && $_POST['queid'] <> ''){
		$arCampos = array( "perid", "pertipo", "pertitulo", "perordem", "perajuda", "perobrigatorio", "permascara", "permultipla" );
	} else {
		$arCampos = array( "perid", "pertipo", "pertitulo", "perordem", "perajuda", "perobrigatorio", "permascara", "permultipla" );
	}

	$obPergunta->popularObjeto( $arCampos, $arDados );
	
	$perid = $obPergunta->salvar();
	if ( $obPergunta->perid )
		$perid = $obPergunta->perid;
		
	$obPergunta->commit();
	
	if($_POST['perordem'] > $_POST['ordemi']){
		$sit = '>';
	} else {
		$sit = '>=';
	}
	
	$sql = "SELECT perid FROM questionario.pergunta WHERE $where AND perordem $sit {$_POST['perordem']} AND perid != {$perid} ORDER BY perordem";
	$arrPerid = (array) $db->carregarColuna($sql);
	if($arrPerid){
		foreach($arrPerid as $peridA){
			$i++;
//			$sql = "SELECT perordem FROM questionario.pergunta WHERE perid = $peridA";
//			$ordemA = ($db->pegaUm($sql))+1;
			$ordemA = $_POST['perordem'] + $i;
			$sql = "UPDATE questionario.pergunta SET perordem = $ordemA  WHERE perid = $peridA";
			$db->executar($sql);
		}
		$db->commit();
	}
	
	$sql = "SELECT perid FROM questionario.pergunta WHERE $where ORDER BY perordem";
	$arrPerid = (array) $db->carregarColuna($sql);
	if($arrPerid){
		$i = 0;
		foreach($arrPerid as $peridA){
			$i++;
//			$sql = "SELECT perordem FROM questionario.pergunta WHERE perid = $peridA";
//			$ordemA = ($db->pegaUm($sql))+1;
			$ordemA = $i;
			$sql = "UPDATE questionario.pergunta SET perordem = $ordemA  WHERE perid = $peridA";
			$db->executar($sql);
		}
		$db->commit();
	}
	
	
	if( $perid ){
		$sql = "SELECT 
					ip.itpid 
				FROM 
					questionario.itempergunta ip
				INNER JOIN questionario.pergunta p ON ip.itpid = p.itpid
				WHERE ip.perid = $perid"; 

		$arrItpid = (array) $db->carregarColuna($sql);
		if ($arrItpid[0]){
			$where1 = "AND itpid NOT IN (" . implode(", ", $arrItpid) . ")";
		}
		$sql = "SELECT 
					ip.itpid
				FROM 
					questionario.itempergunta ip
				INNER JOIN questionario.resposta r ON r.itpid = ip.itpid
				WHERE ip.perid = $perid"; 
		
		$arrItpid = (array) $db->carregarColuna($sql);
		if ($arrItpid[0]){
			$where2 = "AND itpid NOT IN (" . implode(", ", $arrItpid) . ")";
		}
		
		// Deleta os não respondidos
		$sql = "DELETE FROM questionario.itempergunta WHERE perid = {$perid} {$where1} {$where2}";			
		$db->executar($sql);
		if(in_array($_REQUEST['pertipo'], array('CK', 'RA', 'CM', 'CB', 'BM'))){
			if ( is_array($_POST['descricao'])){
				foreach($arrItpid as $itpid){
					
					$obItemPergunta = new QItemPergunta($itpid);
					$arCamposItem = array("perid", "itptitulo");
					$arDadosItem  = array("perid" => $perid, "itptitulo" => $_POST['descricao'][$itpid]);
					
					$obItemPergunta->popularObjeto($arCamposItem, $arDadosItem);
				//	$_POST['descricao'] = array_diff_key($_POST['descricao'], array($itpid));
					$obItemPergunta->salvar();
					$obItemPergunta->commit();
					
				}
				
				foreach($_POST['descricao'] as $itptitulo):
				
					$obItemPergunta = new QItemPergunta();
					$arCamposItem = array("perid", "itptitulo");
					$arDadosItem  = array("perid" => $perid, "itptitulo" => $itptitulo);
					
					$obItemPergunta->popularObjeto($arCamposItem, $arDadosItem);
					$obItemPergunta->salvar();
					$obItemPergunta->commit();
					
				endforeach;
				
			}
			//SE EU DELETAR TODOS OS LINKS DAQUI ELE NÃO VAI ENTRAR E CONSEQUENTEMENTE NÃO VAI DELETAR!
			if($_POST['descricaocarregado']){
				foreach($_POST['descricaocarregado'] as $itpid => $itptitulo){
					$obItemPergunta = new QItemPergunta($itpid);
					$arCamposItem = array("itptitulo");
					$arDadosItem  = array("itptitulo" => $itptitulo);
					
					$obItemPergunta->popularObjeto($arCamposItem, $arDadosItem);
					$obItemPergunta->salvar();
					$obItemPergunta->commit();
				}
			}
		} 
		$obPergunta->commit();
		echo "<script>
				alert('Operação realizada com sucesso'); 
				window.close();
				window.opener.location.replace(window.opener.location);
			  </script>";
	}
	else{
		$obPergunta->rollback();
		$obPergunta->insucesso('Falha na operação');
	}
}
// Extrai dados para Atualização
$perid			 = ($obPergunta->perid ? $obPergunta->perid : $_GET['perid']);
$grpid			 = ($obPergunta->grpid ? $obPergunta->grpid : $_GET['grpid']);
$itpid			 = ($obPergunta->itpid ? $obPergunta->itpid : $_GET['itpid']);
$queid			 = ($obPergunta->queid ? $obPergunta->queid : $_GET['queid']);
$pertipo		 = $obPergunta->pertipo;
$pertitulo		 = $obPergunta->pertitulo;
$perordem		 = $obPergunta->perordem;
$ordemi	    	 = $obPergunta->perordem;
$perajuda		 = $obPergunta->perajuda;
$perobrigatorio	 = $obPergunta->perobrigatorio;
$permascara		 = $obPergunta->permascara;
$permultipla	 = $obPergunta->permultipla;

if($_REQUEST['queid']){
	$sql = "SELECT MAX(perordem) FROM questionario.pergunta WHERE queid = $queid";
	$ordem = ($db->pegaUm($sql))+1;
} else if($_REQUEST['grpid']){
	$sql = "SELECT MAX(perordem) FROM questionario.pergunta WHERE grpid = $grpid";
	$ordem = ($db->pegaUm($sql))+1;
} else if ($_REQUEST['itpid']){
	$sql = "SELECT MAX(perordem) FROM questionario.pergunta WHERE itpid = $itpid";
	$ordem = ($db->pegaUm($sql))+1;
}

monta_titulo( 'Cadastro de Perguntas', obrigatorio().' Indica Campo Obrigatório');

if($obPergunta->perid){
	$sql = "SELECT 
				ip.itpid, itptitulo, r.resid, g.grpid, p.perid
			FROM
				questionario.itempergunta ip
			LEFT JOIN questionario.resposta r using(itpid)
			LEFT JOIN questionario.grupopergunta g using(itpid)
			LEFT JOIN questionario.pergunta p using(itpid)
			WHERE
				ip.perid = ". $obPergunta->perid;
	$arrItens = (array) $db->carregar($sql);
}
?>


<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script type="text/javascript">
		
		var d = document;
		function salvarPergunta(){
			var camposObri 		= "#pertipo#pertitulo#perordem";
			var tiposCamposObri	= '#select#textarea#texto';
			
			if(!validaForm('formulario',camposObri,tiposCamposObri,false))
					return false;
			
			d.getElementById('submeter').value = 'salvar';
			d.getElementById('formulario').submit();
		}
		
		function tipoPergunta(tipo){
			var arrTipo = Array('CK', 'RA', 'CM', 'CB', 'BM');
			
			if ( arrTipo.indexOf(tipo) > -1 ){
				$('#itens').css("display", "");		
			}else{
				$('#itens').css("display", "none");		
			}
		}
		
		function addItem(linhaDados){
			var table, totRow, metadeRow, color, img, resid, itpid, itptitulo = '' , grpid, perid;
			if(linhaDados){
				resid 	  = linhaDados[0];
				itpid 	  = linhaDados[1];
				itptitulo = linhaDados[2];
				grpid	  = linhaDados[3];
				perid	  = linhaDados[4];
			}
			
			if (resid || grpid || perid){
				name = "descricaocarregado[" + itpid + "]";
				img	 = '<img src="/imagens/excluir_01.gif" onclick="alert(\'Esse item já possui um registro vinculado.\');" >';			
			}else{
				name = "descricao[]";
				img	 = '<img src="/imagens/excluir.gif" onclick="$(\'#tableItem\')[0].deleteRow(this.parentNode.parentNode.parentNode.rowIndex);">';			
			}
			
			table = $('#tableItem');

//			totRow = table[0].rows.length;
//			metadeRow  = (totRow / 2).toFixed(0);
			
			var campo1 = '<input type="text" name="' + name + '" value="' + itptitulo + '" style="width:360px;">';
//			var campo2 = '<input type="text" nome="valor[]" style="width:140px;">';
//			table.append("<tr><td><center>" + img + "</center></td><td>" + campo1 + "</td><td>" + campo2 + "</td><tr>");
			
			table.append("<tr><td><center>" + img + "</center></td><td>" + campo1 + "</td></tr>");

			
			
		}
		</script>

	</head>
<body topmargin="0" marginwidth="0" onload="tipoPergunta('<?=$obPergunta->pertipo?>');">
<form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
<input type="hidden" id="submeter" name="submeter" value="">
<input type="hidden" id="grpid" name="grpid" value="<?=$grpid; ?>">
<input type="hidden" id="itpid" name="itpid" value="<?=$itpid; ?>">
<input type="hidden" id="perid" name="perid" value="<?=$perid; ?>">
<input type="hidden" id="queid" name="queid" value="<?=$queid; ?>">
<input type="hidden" id="ordemi" name="ordemi" value="<?=$ordemi; ?>">
<input type="hidden" id="permultipla" name="permultipla" value="f">
<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulodireita" style="width: 20%;"><b>Tipos de Pergunta:</b></td>
		<td><?
			$array = array(
						array("codigo" => "CK", "descricao" => "Checkbox"),
						array("codigo" => "RA", "descricao" => "Radio"),
						array("codigo" => "TX", "descricao" => "Text"),
						array("codigo" => "CB", "descricao" => "Combobox"),
						array("codigo" => "CM", "descricao" => "Combobox Múltiplo"),
						array("codigo" => "DT", "descricao" => "Data"),
						array("codigo" => "TA", "descricao" => "Textarea"),
						array("codigo" => "EXF", "descricao" => "Externo"),
						array("codigo" => "EXT", "descricao" => "Externo - Tabela"),
						);
			if($pertipo){
				$db->monta_combo("pertipo", $array, 'N', 'Selecione...', 'tipoPergunta', '', '', '265', 'S', 'pertipo', '', '', 'Tipos de Pergunta' );
			} else {
				$db->monta_combo("pertipo", $array, 'S', 'Selecione...', 'tipoPergunta', '', '', '265', 'S', 'pertipo', '', '', 'Tipos de Pergunta' );
			}
		 ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20px;"><b>Descrição:</b></td>
		<td><?=campo_textarea('pertitulo', 'S', 'S', 'Descrição da pergunta', 73, 5, 300, '', '', '', '', 'Descrição da pergunta'); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20px;"><b>Texto de ajuda:</b></td>
		<td><?=campo_textarea('perajuda', 'N', 'S', 'Texto de ajuda', 73, 5, 1000, '', '', '', '', 'Texto de ajuda'); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" style="width: 20%;"><b>Máscara:</b></td>
		<td><?=campo_texto('permascara', 'N', 'S', 'Máscara', 50, 30, '', '', '', '', 0, 'id="permascara"' ); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" style="width: 20%;"><b>Ordem:</b></td>
		<td><?=campo_texto('perordem', 'S', 'S', 'Ordem de Visualização', 15, 5, '[#]', '', '', '', 0, 'id="perordem"', '', $ordem ); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20px;"><b>Obrigatório:</b></td>
		<td><input type="radio" name="perobrigatorio" id="perobrigatorio" value="1" <?=($perobrigatorio == 't' ? "checked='checked'" : '') ?> > Sim &nbsp;
			<input type="radio" name="perobrigatorio" id="perobrigatorio" value="0" <?=($perobrigatorio == 'f' || $perobrigatorio == '' ? "checked='checked'" : '') ?> > Não</td>
	</tr>
<!--	<tr>-->
<!--		<td class="subtitulodireita" width="20px;"><b>Múltiplo:</b></td>-->
<!--		<td><input type="radio" name="permultipla" id="permultipla" value="1" <?=($permultipla == 't' ? "checked='checked'" : '') ?> > Sim &nbsp;-->
<!--			<input type="radio" name="permultipla" id="permultipla" value="0" <?=($permultipla == 'f' ? "checked='checked'" : '') ?> > Não</td>-->
<!--	</tr>-->
	
	
	<tr id="itens" style="display:none ;">
		<td colspan="2" align="center">
			<div style="width: 95%; text-align: right;"><a onclick="addItem()">Novo Item</a></div>
			<fieldset style="background: #FFF;">
				<legend>Itens</legend>
				<div style="overflow: auto; height: 100px;">
				<table class="tabela" id="tableItem">
					<tr>
						<th width="10%">Ação</th>
						<th>Descrição</th>
					</tr>
				</table>
				</div>
			</fieldset>
		</td>
	</tr>
	
	
	<tr>
		<th colspan="2">
			<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="salvarPergunta();">&nbsp;
			<input type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="window.close();">
		</th>
	</tr>
</table>
</form>
<script type="text/javascript">
<?php 
if($obPergunta->perid){
	foreach( $arrItens as $item ):
		echo "addItem(['{$item['resid']}', '{$item['itpid']}', '{$item['itptitulo']}', '{$item['grpid']}', '{$item['perid']}']);";
	endforeach;
}
?>
</script>
</body>
</html>