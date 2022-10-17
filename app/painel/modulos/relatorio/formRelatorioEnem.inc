<?php

if ($_POST['agrupador']){
	ini_set("memory_limit","256M");
	include("resultRelatorioEnem.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório ', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript"><!--
function gerarRelatorio(){
	var formulario = document.formulario;

	if (formulario.elements['agrupador'][0] == null){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}	
	
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.estado );
	selectAllOptions( formulario.municipio );
	selectAllOptions( formulario.filtroregiao );
			
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
//	ajaxRelatorio();
	formulario.submit();
	
	janela.focus();
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

//function ajaxRelatorio(){
//	var formulario = document.formulario;
//	var divRel 	   = document.getElementById('resultformulario');
//
//	divRel.style.textAlign='center';
//	divRel.innerHTML = 'carregando...';
//
//	var agrupador = new Array();	
//	for(i=0; i < formulario.agrupador.options.length; i++){
//		agrupador[i] = formulario.agrupador.options[i].value; 
//	}	
//	
//	var tipoensino = new Array();
//	for(i=0; i < formulario.f_tipoensino.options.length; i++){
//		tipoensino[i] = formulario.f_tipoensino.options[i].value; 
//	}		
//	
//	var param =  '&agrupador=' + agrupador + 
//				 '&f_tipoensino=' + tipoensino +
//				 '&f_tipoensino_campo_excludente=' + formulario.f_tipoensino_campo_excludente.checked +
//				 '&f_tipoensino_campo_flag=' + formulario.f_tipoensino_campo_flag.value;
//	 
//    var req = new Ajax.Request('academico.php?modulo=inicio&acao=C', {
//						        method:     'post',
//						        parameters: param,
//						        onComplete: function (res)
//						        {
//							    	divRel.innerHTML = res.responseText;
//						        }
//							});
//	
//}
--></script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Agrupadores</td>
		<td>
			<?
			$matriz = agrupador();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupador', null);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
	<tr><td class="SubTituloDireita" valign="top">Existência de Instituição Federal</td>
		<td>
			<input type='radio' name='exist' value='S'>Com Instituição Federal&nbsp;&nbsp;
			<input type='radio' name='exist' value='N'>Sem Instituição Federal&nbsp;&nbsp;
			<input type='radio' name='exist' value=''>Todos&nbsp;&nbsp;
		</td>
	</tr>
		<?php
			
				$stSql = "SELECT
							regcod as codigo,
							regdescricao as descricao
						FROM
							territorios.regiao";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Região', 'filtroregiao',  $stSql, $stSqlCarregados, 'Selecione a(s) Região(ões)' ); 

				$stSql = "SELECT
							estuf AS codigo,
							estdescricao AS descricao
						  FROM
						  	territorios.estado";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Estado', 'estado',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)' ); 
				
				$stSql = "SELECT
							muncod AS codigo,
							estuf || ' - ' || mundescricao AS descricao
						  FROM
						  	territorios.municipio	
						  WHERE
						  	1=1
						  ORDER BY
						  	descricao";
				$stSqlCarregados = "";
				$where = array(
								array(
										"codigo" 	=> "estuf",
										"descricao" => "Estado <b style='size: 8px;'>(sigla)</b>",
										"numeric"	=> false
							   		  )
							   );
				mostrarComboPopup( 'Município', 'municipio',  $stSql, $stSqlCarregados, 'Selecione o(s) Município(s)', $where );
			?>
					
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio();"/>
		</td>
	</tr>
</table>
</form>
</body>
</html>
<?
function agrupador(){
	return array(
				array('codigo' 	  => 'estado',
					  'descricao' => 'Estado'),	
				array('codigo' 	  => 'municipio',
					  'descricao' => 'Município'),				
				array('codigo' 	  => 'existencia',
					  'descricao' => 'Existência de Instituição Federal'),			
				array('codigo' 	  => 'regiao',
					  'descricao' => 'Região')				
				);
}
?>