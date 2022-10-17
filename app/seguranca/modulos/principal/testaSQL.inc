<?php

monta_titulo('Teste de SQL', '');
$stSql = strtolower( $_REQUEST["sql"] );
$stSql = str_replace("{valorvinculado}", "'".$_REQUEST['vinculo']."'", $stSql);

?>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?
//select estuf as descricao, estcod as codigo from territorios.estado where estcod = 101

$strPos1 = strpos($stSql, 'descricao');
$strPos2 = strpos($stSql, 'codigo');

$query = substr($stSql, 0, strpos($stSql, 'from'));

$exVirgula = explode(',', $query);

if( count($exVirgula) < 3 ){

	if($strPos1 && $strPos2){ //verifico se usuario informou os parametros necessários para execução da query
		if($strPos1 < $strPos2){//verifico se descricao é informada primeiro que o codigo
			$arDados = $db->carregar($stSql);
			if($arDados){//verifico se query possui registros
				$cabecalho = array('Descrição', 'Código');
				$db->monta_lista($stSql, $cabecalho, 20, 5, 'N', 'center', '');
			}else{
				echo "<script>
						 alert('O SQL informado não retornou registro!');
						 window.close();
					  </script>";
			}
		}else{?>
			<script>
				 alert('Instrução SQL informada está incorreta!\nA coluna \"codigo\" não pode ser informada primeiro que a coluna \"descricao\"!');
				 window.close();
			</script><?php
		}
	}else{
		echo "<script>
				 alert('Instrução SQL informada está incorreta!');
				 window.close();
			  </script>";
	}
}else{?>
	<script>
		 alert('O SQL informado não pode ser executado.\nPorque o SQL retorna mais de duas colunas de registros!');
		 window.close();
	</script>;<?php
}
?>




